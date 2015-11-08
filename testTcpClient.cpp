#include "tcpSocket.h"
#include "types.h"

#define   SERVER_IP     "127.0.0.1"
#define   SERVER_PORT   6000
#define   BUFFER_LEN    256
int main()
{
	CTcpSocket oTcpSocket;
	sint32 iClientSock = oTcpSocket.tcpClientSock();
	
	if ( oTcpSocket.tcpConnect() != 0)
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
	}
	
	close(iClientSock);
	exit(0);
}
