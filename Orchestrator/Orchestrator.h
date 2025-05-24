#include <Cat.h>
#include <Server.h>

class Orchestrator
{
    private:
        Cat cat;
        Server server;
        std::string log_file;
        
        std::unordered_map<std::string, std::vector<std::string>> user_dgram_map;
        std::unordered_map<std::string, std::vector<int>> user_response_db;

        char* listen_feed_port();
        std::string process_feed_request(char* buff);
        void feed_response(const std::string& response);
        
        char* listen_pet_port();
        std::string process_pet_request(char* buff);
        void pet_response(const std::string& response);

        void load_db();
        void offload_db();
    public:
        Orchestrator(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port, const std::string& log_file);

        void run_feed();
        bool run_pet();
};