#pragma once
#include <winsock2.h>
#include <string>
#include <vector>

class Client
{
    private:
        SOCKET SendSocketUDP;
        SOCKET SendSocketTCP;
        sockaddr_in client_addr;
        int buff_size;
    public:
        Client(int buff_size, const char* client_ipaddr);
        ~Client();

        std::vector<std::string> feed_request(const std::string& message, const char* server_ipaddr, unsigned short port);
};