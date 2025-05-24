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

char* Server::poll_udp()
{
    char* RecvBuf = (char*)malloc(buff_size);
    recvfrom(SocketUDP, RecvBuf, buff_size, 0, (SOCKADDR*)&udp_addr, &sckaddrsize);
    return RecvBuf;
}

char* Server::poll_tcp()
{
    // Listen on the socket.
    //printf("is_listening: %d\n", is_listening);
    if(!is_listening)
    {
        if (listen(SocketTCP, 1 ) == SOCKET_ERROR)
            return "";
        is_listening = true;
    }

    // Accept connections.
    int connection_time = 0;
    int option_sz = sizeof(int);
    getsockopt(SocketTCP, SOL_SOCKET, SO_CONNECT_TIME, (char*)&connection_time, &option_sz);
    //printf("conn_time: %d\n", connection_time);
    if(connection_time == -1)
    {
        SOCKET AcceptSocket;
        while (1) {
            AcceptSocket = SOCKET_ERROR;
            while ( AcceptSocket == SOCKET_ERROR ) {
                AcceptSocket = accept(SocketTCP, NULL, NULL);
            }
            SocketTCP = AcceptSocket; 
            break;
        }
    }

    char* RecvBuf = (char*)malloc(buff_size);
    int bytes = recv(SocketTCP, RecvBuf, buff_size, 0 );
    if(bytes == -1)
    {
        free(RecvBuf);
        return NULL;
    }

    while(!bytes && bytes != WSAECONNRESET)
    {
        bytes = recv(SocketTCP, RecvBuf, buff_size, 0 );
    }
    return RecvBuf;
}

void Server::respond_udp(const std::string& payload)
{
    sendto(SocketUDP, payload.c_str(), payload.length(), 0, (SOCKADDR*)&udp_addr, sckaddrsize);
}

void Server::respond_tcp(const std::string& payload)
{
    send(SocketTCP, payload.c_str(), payload.length(), 0 );
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
