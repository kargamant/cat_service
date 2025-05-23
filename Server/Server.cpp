#include <Server.h>

int Server::sckaddrsize = sizeof(sockaddr_in);

Server::Server(int buff_size, const char* server_ipaddr, unsigned short server_udp_port, unsigned short server_tcp_port) : Handler(buff_size, server_ipaddr)
{
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_port = htons(server_udp_port);
	udp_addr.sin_addr.s_addr = inet_addr(server_ipaddr);

	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_port = htons(server_tcp_port);
	tcp_addr.sin_addr.s_addr = inet_addr(server_ipaddr);

    bind(SocketUDP, (SOCKADDR *) &udp_addr, sizeof(udp_addr));
    bind(SocketTCP, (SOCKADDR *) &tcp_addr, sizeof(tcp_addr));	
}

char* Server::poll_socket(SOCKET* sock, SOCKADDR* addr)
{
    char* RecvBuf = (char*)malloc(buff_size);
    recvfrom(*sock, RecvBuf, buff_size, 0, addr, &sckaddrsize);
    return RecvBuf;
}

char* Server::poll_udp()
{
    return poll_socket(&SocketUDP, (SOCKADDR*)&udp_addr);
}

char* Server::poll_tcp()
{
    return poll_socket(&SocketTCP, (SOCKADDR*)&tcp_addr);
}

void Server::respond_socket(SOCKET* sock, SOCKADDR* addr, const std::string& payload)
{
    sendto(*sock, payload.c_str(), payload.length(), 0, addr, sckaddrsize);
}

void Server::respond_udp(const std::string& payload)
{
    respond_socket(&SocketUDP, (SOCKADDR*)&udp_addr, payload);
}

void Server::respond_tcp(const std::string& payload)
{
    respond_socket(&SocketTCP, (SOCKADDR*)&tcp_addr, payload);
}

char* Server::get_ip(sockaddr_in* sockaddr)
{
    return inet_ntoa(sockaddr->sin_addr);
}

std::string Server::get_ip_udp()
{
    return get_ip(&udp_addr);
}

std::string Server::get_ip_tcp()
{
    return get_ip(&tcp_addr);
}
