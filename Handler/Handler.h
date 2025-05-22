#pragma once
#include <winsock2.h>
#include <string>
#include <vector>

class Handler
{
    protected:
        SOCKET SocketUDP;
        SOCKET SocketTCP;
        sockaddr_in addr;
        int buff_size;
    public:
        Handler(int buff_size, const char* ipaddr);
        ~Handler();
};