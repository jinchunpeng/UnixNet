#include "udpSocket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define   SERVER_IP      "192.168.1.102"
#define   SERVER_PORT    6600
#define   MSSAGE_BUF     256

int main()
{
	CUdpSocket oUdpSock;
	
	oUdpSock.udpClientSock();
	
	sockaddr_in oSockAddr;
	oSockAddr.sin_family = AF_INET;
	oSockAddr.sin_port	 = htons(SERVER_PORT);
	//oSockAddr.sin_addr.s_addr = 
	inet_pton(AF_INET, SERVER_IP, &oSockAddr.sin_addr.s_addr);
	
	char strMsg[MSSAGE_BUF];
	while ( scanf("%s", strMsg) )
	{	
		ELOG("%s\n", strMsg);
		oUdpSock.udpSend((uint8*)strMsg, strlen(strMsg), 0, &oSockAddr);

		ELOG("pre udpRecv\n");
		oUdpSock.udpRecv((uint8*)strMsg, sizeof(strMsg), 0, &oSockAddr,0);
		ELOG("aft udpRecv\n");
		ELOG("recv Data:%s\n", strMsg);
	}
	
}











