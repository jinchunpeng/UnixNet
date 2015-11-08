#ifndef _TCP_SOCKET_H__
#define _TCP_SOCKET_H__

#include <netinet/in.h> //sockaddr_in
#include "types.h"

class CTcpSocket
{
private:
    
public:
    ~CTcpSocket();
    
    sint32 tcpClientSock();
    sint32 tcpSock(const char *pAdr, uint16 usPort);
    //sint32 tcpListen(sint32 iSkt, sint32 iNum);
    
    sint32 tcpConnect(sint32 iSkt, struct sockaddr_in *pAdr, sint32 iLen);
    sint32 tcpAccept(sint32 iSkt, struct sockaddr_in *pAdr, sint32 iLen);    
    sint32 tcpSnd(sint32 iSkt, char* pMsg, sint32 iLen);
    sint32 tcpRcv(sint32 iSkt, char* pMsg, sint32 iLen);
    
};

#endif
