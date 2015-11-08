#include "tcpSocket.h"
#include "types.h"
#include <sys/types.h>
#include <unistd.h>

#define   SERVER_IP    "0.0.0.0" 
#define   SERVER_PORT  6000
#define   BUFFER_LEN   256

void cmdHandle(sint32 iSock,  CTcpSocket* pTcpSocket)
{
	char strTemp[BUFFER_LEN];
	sint32 iLen;
	while ( pTcpSocket->tcpRcv(iSock, strTemp, iLen) > 0 )
	{
		//ELOG("");
		pTcpSocket->tcpSnd(iSock, strTemp, iLen);
	}
	ELOG("disconnect\n");
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
		}
		else
		{
			//new accept
			////getpeerName getsockname
			ELOG("");	
			
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