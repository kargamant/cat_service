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
    // example: @YadykinIgorMihailovich007 - whiskey~
    // example2: @VladimirIbnSergeevichStepanov123456789MoskvaSanktPiterBurgSamara - whiskey~
    WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	
    Client client{CAT_BUFF_SIZE, CLIENT_IPADDR};

    bool tcp_mode = argc > 1 ? !strcmp(argv[1], "-t") : false;
    
    if(tcp_mode && !client.connect_server(SERVER_IPADDR, PET_PORT))
    {
        std::cout << "Initial connect failed" << std::endl;
        WSACleanup();
	    return 1;
    }

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
            auto responses = tcp_mode ? client.pet_request(message, SERVER_IPADDR, PET_PORT) : 
                                        client.feed_request(message, SERVER_IPADDR, FEED_PORT);
            if(tcp_mode && responses.empty())
                break;
            for(auto& response: responses)
                std::cout << "Cat response: " << response << std::endl;
        }
    }
    std::cout << "Client closed" << std::endl;
    WSACleanup();
    
	return 0;
}

