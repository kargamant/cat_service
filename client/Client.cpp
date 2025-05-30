#include <Client.h>
#include <iostream>

Client::Client(int buff_size, const char* client_ipaddr) : Handler(buff_size, client_ipaddr)
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(client_ipaddr);
    bind(SocketUDP, (SOCKADDR *) &addr, sizeof(addr));
    bind(SocketTCP, (SOCKADDR *) &addr, sizeof(addr));
}

std::vector<std::string> Client::feed_request(const std::string& message, const char* server_ipaddr, unsigned short port)
{


    //creating addr for server
	sockaddr_in RecvAddr;
	RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_addr.s_addr = inet_addr(server_ipaddr);
	RecvAddr.sin_port = htons(port);


    if(message.length() <= buff_size)
    {
        // allocing payload
        char* msg = (char*)malloc(buff_size);
        const char* str = message.c_str();
        memcpy(msg, str, message.length());
        msg[message.length()] = 0;

        //setting bc option and sending
        BOOL OptionValue=false;
        setsockopt(SocketUDP, SOL_SOCKET, SO_BROADCAST, (char*)& OptionValue, sizeof(OptionValue));
        sendto(SocketUDP, msg, buff_size, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
        
        int sz = sizeof(RecvAddr);
        memset(msg, 0, buff_size);
        recvfrom(SocketUDP, msg, buff_size, 0, (SOCKADDR *)&RecvAddr, &sz);
        std::string response{msg};
        free(msg);

        return {response};
    }
    else
    {
        std::vector<std::string> cat_responses;
        for(int i=0; i<message.length()/(buff_size-3) + 1; i++)
        {
            // cutting out segment
            std::string segment = message.substr(i*(buff_size-3), buff_size-3);
            int seg_sz = segment.length();

            if(seg_sz == buff_size -3)
            {
                segment += "~";
                segment += std::to_string(i);
            }

            // allocating payload with segment data
            char* msg = (char*)malloc(buff_size);
            const char* str = segment.c_str();
            memcpy(msg, str, segment.length());
            msg[segment.length()] = 0;


            BOOL OptionValue=false;
            setsockopt(SocketUDP, SOL_SOCKET, SO_BROADCAST, (char*)& OptionValue, sizeof(OptionValue));
            sendto(SocketUDP, msg, buff_size, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
            
            int sz = sizeof(RecvAddr);
            memset(msg, 0, buff_size);
            recvfrom(SocketUDP, msg, buff_size, 0, (SOCKADDR *)&RecvAddr, &sz);
            cat_responses.push_back(msg);
            free(msg);
        }

        return cat_responses;
    }
}

bool Client::connect_server(const char* server_ipaddr, unsigned short port)
{
    //creating addr for server
	sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(server_ipaddr);
	ServerAddr.sin_port = htons(port);

    return (connect(SocketTCP, (SOCKADDR*) &ServerAddr, sizeof(ServerAddr)) != SOCKET_ERROR);
}

std::vector<std::string> Client::pet_request(const std::string& message, const char* server_ipaddr, unsigned short port)
{
    if(message.length() <= buff_size)
    {
        char* buff = (char*)malloc(buff_size);
        memcpy(buff, message.c_str(), message.length());
        buff[message.length()] = 0;

        send(SocketTCP, buff, buff_size, 0);
        memset(buff, 0, buff_size);
        int bytes = recv(SocketTCP, buff, buff_size, 0 );
        if(bytes==-1)
            return {};
        return {buff};
    }
    else
    {
        std::vector<std::string> responses;
        for(int i=0; i<message.length()/(buff_size-1) + 1; i++)
        {
            std::string segment = message.substr(i*(buff_size-1), (buff_size-1));
            
            char* buff = (char*)malloc(buff_size);
            memcpy(buff, segment.c_str(), buff_size-1);
            buff[buff_size] = 0;

            send(SocketTCP, buff, buff_size, 0);
            memset(buff, 0, buff_size);
            int bytes = recv(SocketTCP, buff, buff_size, 0 );
            buff[buff_size] = 0;
            if(bytes==-1 || bytes==0)
                responses.push_back("");
            responses.push_back(buff);
        }
        return responses;
    }
}

void Client::dissconnect()
{
    WSASendDisconnect(SocketTCP, 0);
}
