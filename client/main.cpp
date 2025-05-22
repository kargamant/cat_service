#include <winsock2.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <Client.h>
#include <enviroments.h>


int main(int argc, char* argv[])
{
	
    if (argc > 1)
		SERVER_IPADDR = argv[1];
	
    Client client{CAT_BUFF_SIZE, CLIENT_IPADDR};

    BOOL flag = true;
    while(flag)
    {
        std::string message;
        std::cout << "enter message to send or q to leave: ";
        std::getline(std::cin, message);

        if(message == "q")
        {
            flag = false;
        }
        else
        {
            auto responses = client.feed_request(message, SERVER_IPADDR, FEED_PORT);
            for(auto& response: responses)
                std::cout << "Cat response: " << response << std::endl;
        }
    }
    
    // SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// size_t maxdgr = 0;
	// int optsize = sizeof (size_t);
	// getsockopt(SendSocket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*) &maxdgr, &optsize);
	// printf("Maximum datagram is %d...\n", maxdgr);

	// sockaddr_in RecvAddr;
	// RecvAddr.sin_family = AF_INET;
	// RecvAddr.sin_port = htons(FEED_PORT);

	// BOOL OptionValue=true, flag = true;
	// while (flag)
	// {
    //     std::string message;
    //     std::cout << "enter message to send or q to leave: ";
    //     std::getline(std::cin, message);

    //     if(message == "q")
    //     {
    //         flag = false;
    //     }
    //     else if(message.length() <= CAT_BUFF_SIZE)
    //     {
    //         char* msg = (char*)malloc(CAT_BUFF_SIZE);
    //         const char* str = message.c_str();
    //         memcpy(msg, str, message.length());
    //         msg[message.length()] = 0;

    //         OptionValue=false;
    //         setsockopt(SendSocket, SOL_SOCKET, SO_BROADCAST, (char*)& OptionValue, sizeof(OptionValue));
            
    //         RecvAddr.sin_addr.s_addr = inet_addr(SERVER_IPADDR);
    //         sendto(SendSocket, msg, CAT_BUFF_SIZE, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
            
    //         int sz = sizeof(RecvAddr);
    //         memset(msg, 0, CAT_BUFF_SIZE);
    //         recvfrom(SendSocket, msg, CAT_BUFF_SIZE, 0, (SOCKADDR *)&RecvAddr, &sz);
    //         printf("Cat response: %s\n", msg);
    //         free(msg);
    //     }
    //     else
    //     {
            
    //         for(int i=0; i<message.length()/CAT_BUFF_SIZE; i++)
    //         {
    //             std::string segment = message.substr(i*(CAT_BUFF_SIZE-2), std::min<int>((i+1)*(CAT_BUFF_SIZE-2), message.length()));
    //             segment += "~";
    //             segment += std::to_string(i);

    //             char* msg = (char*)malloc(CAT_BUFF_SIZE);
    //             const char* str = segment.c_str();
    //             memcpy(msg, str, segment.length());
    //             msg[segment.length()] = 0;
    
    //             OptionValue=false;
    //             setsockopt(SendSocket, SOL_SOCKET, SO_BROADCAST, (char*)& OptionValue, sizeof(OptionValue));
                
    //             RecvAddr.sin_addr.s_addr = inet_addr(SERVER_IPADDR);
    //             sendto(SendSocket, msg, CAT_BUFF_SIZE, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
                
    //             int sz = sizeof(RecvAddr);
    //             memset(msg, 0, CAT_BUFF_SIZE);
    //             recvfrom(SendSocket, msg, CAT_BUFF_SIZE, 0, (SOCKADDR *)&RecvAddr, &sz);
    //             printf("Cat response: %s\n", msg);
    //             free(msg);
    //         }
    //     }
	// }
    // printf("Closing client\n");
	// closesocket(SendSocket);
	// WSACleanup();
	return 0;
}

