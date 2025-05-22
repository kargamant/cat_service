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
    WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	
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
    client.~Client();
    WSACleanup();
    
	return 0;
}

