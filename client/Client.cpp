#include <Client.h>
#include <iostream>

Client::Client(int buff_size, const char* client_ipaddr) : buff_size(buff_size), 
                                SendSocketUDP(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)),
                                SendSocketTCP(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(client_ipaddr);
    bind(SendSocketUDP, (SOCKADDR *) &client_addr, sizeof(client_addr));
    bind(SendSocketTCP, (SOCKADDR *) &client_addr, sizeof(client_addr));
}

Client::~Client()
{
    closesocket(SendSocketUDP);
    closesocket(SendSocketTCP);
}

std::vector<std::string> Client::feed_request(const std::string& message, const char* server_ipaddr, unsigned short port)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

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
        setsockopt(SendSocketUDP, SOL_SOCKET, SO_BROADCAST, (char*)& OptionValue, sizeof(OptionValue));
        sendto(SendSocketUDP, msg, buff_size, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
        
        int sz = sizeof(RecvAddr);
        memset(msg, 0, buff_size);
        recvfrom(SendSocketUDP, msg, buff_size, 0, (SOCKADDR *)&RecvAddr, &sz);
        std::string response{msg};
        free(msg);

        WSACleanup();
        return {response};
    }
    else
    {
        std::vector<std::string> cat_responses;
        for(int i=0; i<message.length()/buff_size; i++)
        {
            // cutting out segment
            std::string segment = message.substr(i*(buff_size-2), std::min<int>((i+1)*(buff_size-2), message.length()));
            segment += "~";
            segment += std::to_string(i);

            // allocating payload with segment data
            char* msg = (char*)malloc(buff_size);
            const char* str = segment.c_str();
            memcpy(msg, str, segment.length());
            msg[segment.length()] = 0;

            BOOL OptionValue=false;
            setsockopt(SendSocketUDP, SOL_SOCKET, SO_BROADCAST, (char*)& OptionValue, sizeof(OptionValue));
            sendto(SendSocketUDP, msg, buff_size, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
            
            int sz = sizeof(RecvAddr);
            memset(msg, 0, buff_size);
            recvfrom(SendSocketUDP, msg, buff_size, 0, (SOCKADDR *)&RecvAddr, &sz);
            cat_responses.push_back(msg);
            free(msg);
        }

        WSACleanup();
        return cat_responses;
    }
}