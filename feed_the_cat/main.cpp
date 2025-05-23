#include <winsock2.h>
#include <conio.h>
#include <stdio.h>
#include <enviroments.h>
#include <Orchestrator.h>
#include <iostream>

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	
	Orchestrator orch{CAT_BUFF_SIZE, SERVER_IPADDR, FEED_PORT, PET_PORT};

	while (!_kbhit())
	{
		std::cout << "Cat is listening..." << std::endl;
		
		orch.run_feed();
	}
	std::cout << "Cat closed. Bye!" << std::endl;
	WSACleanup();
	return 0;
}