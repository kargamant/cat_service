#pragma once
#include <winsock2.h>
#include <string>
#include <vector>
#include <Handler.h>

class Client : public Handler
{
    private:
        sockaddr_in addr;
    public:
        Client(int buff_size, const char* client_ipaddr);

        std::vector<std::string> feed_request(const std::string& message, const char* server_ipaddr, unsigned short port);
};