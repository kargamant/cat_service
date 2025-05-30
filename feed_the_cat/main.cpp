#include <winsock2.h>
#include <conio.h>
#include <stdio.h>
#include <enviroments.h>
#include <Orchestrator.h>
#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "Hello" << std::endl;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	
	char* ip_addr = argc > 1 ? argv[1] : NULL;
    if(ip_addr)
        SERVER_IPADDR = ip_addr;

	bool tcp_mode = argc > 2 ? !strcmp(argv[2], "-t") : false;

	std::cout << "Started up on: " << SERVER_IPADDR << std::endl;

	Orchestrator orch{CAT_BUFF_SIZE, SERVER_IPADDR, FEED_PORT, PET_PORT, tcp_mode, LOG_FILE_NAME};

	std::cout << "Created orch" << std::endl;

	
	while (!kbhit())
	{
		std::cout << "Cat is listening..." << std::endl;
		
		if(tcp_mode)
		{
			bool res = orch.run_pet();
			if(!res)
				break;
		}
		else
			orch.run_feed();
	}
	std::cout << "Cat closed. Bye!" << std::endl;
	WSACleanup();
	return 0;
}