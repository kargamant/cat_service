#include <Handler.h>

Handler::Handler(int buff_size, const char* ipaddr) : buff_size(buff_size), 
                                SocketUDP(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)),
                                SocketTCP(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
}

Handler::~Handler()
{
    closesocket(SocketUDP);
    closesocket(SocketTCP);
}