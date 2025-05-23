#pragma once
#include <winsock2.h>
#include <string>
#include <vector>
#include <Handler.h>

class Server : public Handler
{
    private:
        static int sckaddrsize;
        sockaddr_in udp_addr;
        sockaddr_in tcp_addr;

        char* poll_socket(SOCKET* sock, SOCKADDR* addr);
        char* get_ip(sockaddr_in* sockaddr);
        void respond_socket(SOCKET* sock, SOCKADDR* addr, const std::string& payload);
    public:
        Server(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port);

        char* poll_udp();
        char* poll_tcp();
        void respond_udp(const std::string& payload);
        void respond_tcp(const std::string& payload);

        std::string get_ip_udp();
        std::string get_ip_tcp();
};