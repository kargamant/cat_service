#pragma once
#include <winsock2.h>
#include <ws2def.h>
#include <mswsock.h>
#include <string>
#include <vector>
#include <Handler.h>

class Server : public Handler
{
    private:
        static int sckaddrsize;
        sockaddr_in udp_addr;
        sockaddr_in tcp_addr;
        bool is_listening=false;

        char* get_ip(sockaddr_in* sockaddr);
    public:
        Server(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port);

        char* poll_udp();
        char* poll_tcp();
        void respond_udp(const std::string& payload);
        void respond_tcp(const std::string& payload);

        std::string get_ip_udp();
        std::string get_ip_tcp();
        std::string get_peer_ip_tcp();
};