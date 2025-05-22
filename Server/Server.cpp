#include <Server.h>

int Server::sckaddrsize = sizeof(sockaddr_in);

Server::Server(int buff_size, const char* server_ipaddr, unsigned short server_port) : Handler(buff_size, server_ipaddr)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);
	addr.sin_addr.s_addr = inet_addr(server_ipaddr);
    bind(SocketUDP, (SOCKADDR *) &addr, sizeof(addr));
    bind(SocketTCP, (SOCKADDR *) &addr, sizeof(addr));	
}

char* Server::poll_socket(SOCKET* sock)
{
    char* RecvBuf = (char*)malloc(buff_size);
    recvfrom(*sock, RecvBuf, buff_size, 0, (SOCKADDR *)&addr, &sckaddrsize);
    return RecvBuf;
}

char* Server::poll_udp()
{
    return poll_socket(&SocketUDP);
}

char* Server::poll_tcp()
{
    return poll_socket(&SocketTCP);
}

void Server::respond_socket(SOCKET* sock, const std::string& payload)
{
    sendto(*sock, payload.c_str(), payload.length(), 0, (SOCKADDR*)&addr, sckaddrsize);
}

void Server::respond_udp(const std::string& payload)
{
    respond_socket(&SocketUDP, payload);
}

void Server::respond_tcp(const std::string& payload)
{
    respond_socket(&SocketTCP, payload);
}
