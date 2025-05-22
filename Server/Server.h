#pragma once
#include <winsock2.h>
#include <string>
#include <vector>
#include <Handler.h>

class Server : public Handler
{
    private:
        static int sckaddrsize;
        char* poll_socket(SOCKET* sock);
        void respond_socket(SOCKET* sock, const std::string& payload);
    public:
        Server(int buff_size, const char* server_ipaddr, unsigned short server_port);

        char* poll_udp();
        char* poll_tcp();
        void respond_udp(const std::string& payload);
        void respond_tcp(const std::string& payload);
};