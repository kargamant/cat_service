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
        for(int i=0; i<message.length()/(buff_size-2) + 1; i++)
        {
            // cutting out segment
            std::string segment = message.substr(i*(buff_size-2), buff_size-2);
            int seg_sz = segment.length();

            if(seg_sz == buff_size -2)
            {
                segment += "~";
                segment += std::to_string(i);
            }

            // std::cout << "bounds: " << i*(buff_size-2) << (i+1)*(buff_size-2) << std::endl;
            // std::cout << "segment: " << segment << std::endl;
            // std::cout << "mesage: " << message.length() << std::endl;

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