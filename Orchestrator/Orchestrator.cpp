#include "Orchestrator.h"
#include <iostream>
#include <numeric>

Orchestrator::Orchestrator(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port) : 
server(buff_size, server_ipaddr, server_udp_port, server_tcp_port)
{
}

char* Orchestrator::listen_feed_port()
{
    return server.poll_udp();
}

std::string Orchestrator::process_feed_request(char* buff)
{
    std::string msg{buff};
    auto cat_response = cat.process_message(msg);
    if(cat_response.second == CatState::Amused)
    {
        int seg_num = ((char)cat_response.first.back()) - '0';
        auto& vec = user_dgram_map[server.get_ip_udp()];
        vec.insert(vec.begin() + seg_num, msg.substr(0, msg.length()-2));
        // for(auto& item: vec)
        //     std::cout << item << ", ";
        // std::cout << std::endl;
    }
    else if(cat_response.second == CatState::EndOfSeg)
    {
        auto& vec = user_dgram_map[server.get_ip_udp()];
        vec.push_back(msg);
        // for(auto& item: vec)
        //     std::cout << item << ", ";
        // std::cout << std::endl;
        std::string collected_msg = std::accumulate(vec.begin(), vec.end(), std::string(""));
        return cat.process_message(collected_msg).first;
    }
    
    return cat_response.first;
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
}