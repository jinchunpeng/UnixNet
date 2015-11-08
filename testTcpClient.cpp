
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "tcpSocket.h"
#include "types.h"

#define   SERVER_IP     "127.0.0.1"
#define   SERVER_PORT   6000
#define   BUFFER_LEN    256
int main()
{
	CTcpSocket oTcpSocket;
	sint32 iClientSock = oTcpSocket.tcpClientSock();
	
	struct sockaddr_in serverAddr;
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr.s_addr);
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_family= AF_INET;
	
	if ( oTcpSocket.tcpConnect(iClientSock, &serverAddr, sizeof(serverAddr)) != 0)
	{
		ENPLOG;
		return 0;
	}
	
	char strTemp[BUFFER_LEN];
	int  iLen;
	while (scanf("%s", strTemp))
	{
		oTcpSocket.tcpSnd(iClientSock, strTemp, strlen(strTemp));
		
		iLen = sizeof(strTemp);
		oTcpSocket.tcpRcv(iClientSock, strTemp, iLen);
		
		ELOG("recv from server %s\n", strTemp);
	}
	
	close(iClientSock);
	exit(0);
}
