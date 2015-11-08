#ifndef _UDP_SOCKET_H__
#define _UDP_SOCKET_H__
#include <netinet/in.h>
#include "types.h"

typedef void (*PFNPKG)(sockaddr_in *pAdr, uint8 *pMsg, sint32 iLen);

class CUdpSocket
{
private:
	sockaddr_in     m_stSerAddr;
	sint32    		m_iUdpSkt;
	bool            m_bThreadFlg;
public:
	CUdpSocket();
	~CUdpSocket();
	sint32  udpSock(const uint32 uiSerIP, const uint16 usPort);
	sint32  udpWait(uint32 uIP, uint16 sPort, PFNPKG pfunPkg);
	void    changeBlockingMode(sint32 iSkt, sint32 iNoBlocking);
	sint32  udpSend(uint8 *pMsg, sint32 iLen, sint32 iSkt = 0, struct sockaddr_in *pStAddr = NULL);
	sint32  udpRecv(uint8 *pMsg, sint32 iLen, sint32 iSkt = 0, struct sockaddr_in *pStAddr = NULL, sint32 iBlk = 1);
	void    udpClose();
};

#endif
