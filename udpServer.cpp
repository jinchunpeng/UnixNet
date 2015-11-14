#include "udpSocket.h"
#include "types.h"
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>


//#define   SERVER_IP      "192.168.8.102"
//#define   CLIENT_IP		 "192.168.1.100"
//#define   CLIENT_PORT    6018

#define   SERVER_PORT    6600
#define   MSSAGE_BUF     256

int main()
{
	CUdpSocket udpSocket;
	udpSocket.udpSock(0, SERVER_PORT);

	sockaddr_in oSockAddr;
	memset(&oSockAddr, 0, sizeof(oSockAddr));

	while (1)
	{
		char strMsg[MSSAGE_BUF] = {'\0'};
		sint32 iLen = udpSocket.udpRecv((uint8* )strMsg, sizeof(strMsg), 0, &oSockAddr, 0);

		if (iLen == 0)
		{

			ELOG("Socket errno %d, strerror %s", errno, strerror(errno));
			continue;
		}

		char strTemp[MSSAGE_BUF] = {'\0'};
		inet_ntop(AF_INET, &oSockAddr.sin_addr.s_addr, strTemp, sizeof(strTemp));
		ELOG("recvfrome %s Data %s\n", strTemp, strMsg);
		
		udpSocket.udpSend((uint8* )strMsg, strlen(strMsg), 0, &oSockAddr);
	}
}






























