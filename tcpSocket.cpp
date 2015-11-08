#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "tcpSocket.h"

CTcpSocket::~CTcpSocket()
{
	
}


//client
sint32 CTcpSocket::tcpClientSock()
{
	sint32 iSkt = socket(AF_INET, SOCK_STREAM, 0);
	return iSkt;
}

//server
sint32 CTcpSocket::tcpSock(const char *pAdr, uint16 usPort)
{
	CHK_EQ_RETURN(pAdr, NULL, -1);
	sockaddr_in tLocAddr;
	bzero(&tLocAddr, sizeof(tLocAddr));

	//inet_pton(AF_INET, pAdr, &tLocAddr.sin_addr);
	tLocAddr.sin_family = AF_INET;    //未进行初始化，绑定失败
	//tLocAddr.sin_family = 0;
	tLocAddr.sin_addr.s_addr  = inet_addr(pAdr);
	tLocAddr.sin_port = htons(usPort);

	sint32 iSkt = socket(AF_INET, SOCK_STREAM, 0);
	if (iSkt < 0)
	{
		return -1;
	}

	sint32 iRet = bind(iSkt, (sockaddr *)&tLocAddr, sizeof(tLocAddr));
	ILOG("ERROR = %s\n", strerror(errno));
	CHK_UNEQ_RETURN(iRet, 0, -1);

	iRet = listen(iSkt, 1024);
	CHK_UNEQ_RETURN(iRet, 0, -1);
	
	ILOG("listening...");
	return iSkt;
}

sint32 CTcpSocket::tcpConnect(sint32 iSkt, struct sockaddr_in *pAdr, sint32 iLen)
{
	return connect(iSkt, (sockaddr *)pAdr, iLen);
}

sint32 CTcpSocket::tcpAccept(sint32 iSkt, struct sockaddr_in* pAdr, sint32 iLen)
{
	return accept(iSkt, (sockaddr *)pAdr, (socklen_t*)&iLen);
}

sint32 CTcpSocket::tcpSnd(sint32 iSkt, char* pMsg, int iLen)
{
	return send(iSkt, pMsg, iLen, 0);
}

sint32 CTcpSocket::tcpRcv(sint32 iSkt, char* pMsg, int iLen)
{
	return recv(iSkt, pMsg, iLen, 0);
}
