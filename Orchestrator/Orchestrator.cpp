#include "Orchestrator.h"
#include <iostream>
#include <numeric>
#include <fstream>

Orchestrator::Orchestrator(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port, const std::string& log_file) : 
server(buff_size, server_ipaddr, server_udp_port, server_tcp_port), log_file(log_file)
{
    load_db();
}

char* Orchestrator::listen_feed_port()
{
    return server.poll_udp();
}

std::string Orchestrator::process_feed_request(char* buff)
{
    std::string msg{buff};
    auto cat_response = cat.process_message(msg);
    if(cat_response.state == CatState::Amused)
    {
        int seg_num = ((char)cat_response.message.back()) - '0';
        auto& vec = user_dgram_map[server.get_ip_udp()];
        vec.insert(vec.begin() + seg_num, msg.substr(0, msg.length()-2));
        // for(auto& item: vec)
        //     std::cout << item << ", ";
        // std::cout << std::endl;
    }
    else if(cat_response.state == CatState::EndOfSeg)
    {
        auto& vec = user_dgram_map[server.get_ip_udp()];
        vec.push_back(msg);
        // for(auto& item: vec)
        //     std::cout << item << ", ";
        // std::cout << std::endl;
        std::string collected_msg = std::accumulate(vec.begin(), vec.end(), std::string(""));

        vec.clear();
    
        cat_response = cat.process_message(collected_msg);
    }
    
    if(cat_response.state == CatState::Eaten || cat_response.state == CatState::Ignore)
        user_response_db[cat_response.client_name].push_back((int)cat_response.state);
    return cat_response.message;
}

void Orchestrator::feed_response(const std::string& response)
{
    server.respond_udp(response);
}

void Orchestrator::run_feed()
{
    char* buf = listen_feed_port();

	std::string cat_response = process_feed_request(buf);
    free(buf);
	
    feed_response(cat_response);
    offload_db();
}

char* Orchestrator::listen_pet_port()
{
    return server.poll_tcp();
}

std::string Orchestrator::process_pet_request(char* buff)
{
    return "aboba";
}

void Orchestrator::pet_response(const std::string& response)
{
    server.respond_tcp(response);
}

bool Orchestrator::run_pet()
{
    //std::cout << "listening" << std::endl;
    char* buf = listen_pet_port();

    if(!buf)
        return false;

    //std::cout << "processing" << std::endl;
    //printf("buff: %d\n", strlen(buf));
    std::string cat_response = process_pet_request(buf);
    free(buf);
	
    //std::cout << "responding" << std::endl;
    pet_response(cat_response);
    offload_db();

    return true;
}

void Orchestrator::load_db()
{
    std::ifstream log{log_file, std::ios::in};
    
    std::regex log_re{"([a-zA-Z0-9]+) ([0-9])"};
    std::smatch match;

    while(!log.eof())
    {
        std::string line;
        std::getline(log, line);

        if(std::regex_match(line, match, log_re))
        {
            std::string user_name = match.str(1);
            int code = stoi(match.str(2));
            user_response_db[user_name].push_back(code);
        }
    }
}

void Orchestrator::offload_db()
{
    std::ofstream log{log_file, std::ios::out};

    for(auto& user: user_response_db)
    {
        for(auto& response: user.second)
            log << user.first << " " << response << std::endl;
    }
}