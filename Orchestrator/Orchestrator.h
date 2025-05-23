#include <Cat.h>
#include <Server.h>

class Orchestrator
{
    private:
        Cat cat;
        Server server;
        std::string log_file;
        
        std::unordered_map<std::string, std::vector<std::string>> user_dgram_map;
        
        char* listen_feed_port();
        std::string process_feed_request(char* buff);
        void feed_response(const std::string& response);
        void log_response(CatResponse& cr);
    public:
        Orchestrator(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port, const std::string& log_file);

        void run_feed();

};