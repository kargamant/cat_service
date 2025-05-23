#include <Cat.h>
#include <Server.h>

class Orchestrator
{
    private:
        Cat cat;
        Server server;
        
        std::unordered_map<std::string, std::vector<std::string>> user_dgram_map;
        
        char* listen_feed_port();
        std::string process_feed_request(char* buff);
        void feed_response(const std::string& response);
    public:
        Orchestrator(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port);

        void run_feed();
};