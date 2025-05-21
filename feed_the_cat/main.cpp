#include <winsock2.h>
#include <conio.h>
#include <stdio.h>
#include <enviroments.h>

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	
	char RecvBuf[CAT_BUFF_SIZE];
	SOCKET RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in RecvAddr;
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(FEED_PORT);
	RecvAddr.sin_addr.s_addr = INADDR_ANY;
	
	sockaddr_in SendAddr;
	int sckaddrsize = sizeof(sockaddr_in);

	size_t maxdgr = 0;
	int optsize = sizeof (size_t);
	getsockopt(RecvSocket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*) &maxdgr, &optsize);
	printf("Maximum cat-datagram is %d...\n", maxdgr);

	if (bind(RecvSocket, (SOCKADDR *)&RecvAddr, sckaddrsize) == SOCKET_ERROR)
	{
		printf("Error: %d\n", WSAGetLastError());
		closesocket(RecvSocket);
		printf("Press any kay to continue...\n");
		return 1;
	};
	printf("Cat is listening...\n", maxdgr);

	while (!_kbhit())
	{
	    int dgrlen = recvfrom(RecvSocket, RecvBuf, CAT_BUFF_SIZE, 0, (SOCKADDR *)&SendAddr, &sckaddrsize);
	    printf("Received %d bytes from %s\n", dgrlen, inet_ntoa(SendAddr.sin_addr));
	}
	closesocket(RecvSocket);
	WSACleanup();
	printf("Press any kay to continue...\n");
	getch();
	return 0;
}