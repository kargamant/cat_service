#include <Orchestrator.h>

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
    auto cat_response = cat.process_message(buff);
    if(cat_response.second == CatState::Amused)
    {
        
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