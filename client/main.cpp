#include <winsock2.h>
#include <conio.h>
#include <enviroments.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	
    if (argc > 1)
		SERVER_IPADDR = argv[1];
	
    SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	size_t maxdgr = 0;
	int optsize = sizeof (size_t);
	getsockopt(SendSocket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*) &maxdgr, &optsize);
	printf("Maximum datagram is %d...\n", maxdgr);

	sockaddr_in RecvAddr;
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(FEED_PORT);

	BOOL OptionValue=true, flag = true;
	while (flag)
	{
        char* msg = (char*)malloc(CAT_BUFF_SIZE);
        printf("enter message to send: ");
        scanf("%[^\n]%*c", msg);

        if(*msg == EOF)
        {
            flag = false;
        }
        else
        {
            OptionValue=false;
			setsockopt(SendSocket, SOL_SOCKET, SO_BROADCAST, (char*)& OptionValue, sizeof(OptionValue));
			RecvAddr.sin_addr.s_addr = inet_addr(SERVER_IPADDR);
			printf("Sending '%s' to the %s...\n", msg, SERVER_IPADDR);
			sendto(SendSocket, msg, CAT_BUFF_SIZE, 0, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));
            int sz = sizeof(RecvAddr);
            memset(msg, 0, CAT_BUFF_SIZE);
            recvfrom(SendSocket, msg, CAT_BUFF_SIZE, 0, (SOCKADDR *)&RecvAddr, &sz);
            printf("Cat response: %s\n", msg);
        }
	}
    printf("Closing client\n");
	closesocket(SendSocket);
	WSACleanup();
	return 0;
}

