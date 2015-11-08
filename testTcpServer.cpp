#include "tcpSocket.h"
#include "types.h"
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define   SERVER_IP    "0.0.0.0" 
#define   SERVER_PORT  6000
#define   BUFFER_LEN   256

void cmdHandle(sint32 iSock,  CTcpSocket* pTcpSocket)
{
	char strTemp[BUFFER_LEN];
	sint32 iLen;
	while ( (iLen = pTcpSocket->tcpRcv(iSock, strTemp, iLen) ) > 0 )
	{
		//ELOG("");
		pTcpSocket->tcpSnd(iSock, strTemp, iLen);
	}
	ELOG("disconnect\n");
	close(iSock);
}

int main(int argn, char **argv)
{	
	CTcpSocket oTcpSocket;
	
	sint32 iTcpSock = oTcpSocket.tcpSock(SERVER_IP, SERVER_PORT);
	CHK_EQ_RETURN(iTcpSock, -1, -1);
	
	while(1)
	{
		sockaddr_in oAddr;
		sint32      iLen;
		sint32 iAcceptSock = oTcpSocket.tcpAccept(iTcpSock, &oAddr, iLen);
		
		if (iAcceptSock < 0)
		{
			ENPLOG;
			close(iTcpSock);
			return 0;
		}
		else
		{
			//new accept
			////getpeerName getsockname
			sockaddr_in oAddr1;
			sockaddr_in oAddr2;
			
			socklen_t len1;
			socklen_t len2;
			char strTemp1[BUFFER_LEN];
			char strTemp2[BUFFER_LEN];
			getsockname(iAcceptSock, (sockaddr *)&oAddr1, &len1);
			getpeername(iAcceptSock, (sockaddr *)&oAddr2, &len2);
			
			inet_ntop(AF_INET, &oAddr1.sin_addr.s_addr, strTemp1, sizeof(strTemp1));
			inet_ntop(AF_INET, &oAddr2.sin_addr.s_addr, strTemp2, sizeof(strTemp2));
			
			ILOG("addr1 %s, addr2 %s\n", strTemp1, strTemp2);	
			
		}	
		if ( 0 == fork() )
		{
			cmdHandle(iAcceptSock, &oTcpSocket);	
			close(iTcpSock);
			return 0;
		}
		close(iAcceptSock);
	}
	
	close(iTcpSock);
}