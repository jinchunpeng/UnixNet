#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "udpSocket.h"

CUdpSocket::CUdpSocket()
{
    memset(&m_stSerAddr, 0x0, sizeof(sockaddr_in));
    m_iUdpSkt = -1;
}

CUdpSocket::~CUdpSocket()
{
	m_bThreadFlg = false;
    udpClose();
}


sint32  CUdpSocket::udpSock(const uint32 uiSerIP, const uint16 usPort)
{
    m_iUdpSkt = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_iUdpSkt < 0)
    {
        ENPLOG;
        return -1;
    }

    sint32 iOpt = 1;
	sint32 iRet = setsockopt(m_iUdpSkt, SOL_SOCKET, SO_REUSEADDR, &iOpt, sizeof(sint32));
	if(iRet)
	{
		ELOG("CUdpSocket:setsockopt failed, errno [%d], strerror[%s]", errno, strerror(errno));
		udpClose();
		return -1;
	}

    m_stSerAddr.sin_family = AF_INET;
    m_stSerAddr.sin_port   = htons(usPort);
	if(uiSerIP)
	{
	    m_stSerAddr.sin_addr.s_addr = htonl(uiSerIP);
	}
	else
	{
		m_stSerAddr.sin_addr.s_addr = INADDR_ANY;
	}
    
    sint32 iRet = bind(m_iUdpSkt, (sockaddr*)m_stSerAddr, sizeof(m_stSerAddr));
    CHK_UNEQ_RETURN(iRet, 0, -1);
    return 0;
}

sint32  CUdpSocket::udpWait(uint32 uIP, uint16 sPort, PFNPKG pfunPkg)
{
	sint32 iRt = udpSock(uIP, sPort);
	if(iRt < 0)
	{
		ELOG("CUdpSocket udpSock error.");
		return -1;
	}
	iRt = bind(m_iUdpSkt, (sockaddr *)&m_stSerAddr, sizeof(m_stSerAddr));
	if(iRt != 0)
	{
		ELOG("CUdpSocket bind error. Ret%d,errno%d,str %s", iRt, errno, strerror(errno));
		return -1;
	}
	struct timeval tm;
	uint8 czMsg[1024];
	m_bThreadFlg = true;
	while(m_bThreadFlg)
	{
		tm.tv_sec  = 1;
		tm.tv_usec = 0;
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(m_iUdpSkt, &rfds);
		iRt = select(m_iUdpSkt + 1, &rfds, NULL, NULL, &tm);
		if(iRt < 0)
		{
			break;
		}
		else if(iRt > 0)
		{
			if(FD_ISSET(m_iUdpSkt, &rfds))
			{
				memset(czMsg, 0x0, sizeof(czMsg));
				struct sockaddr_in tadr;
				bzero(&tadr, sizeof(sockaddr_in));
				sint32 cl = udpRecv(czMsg, sizeof(czMsg), 0, &tadr, 0);
				if(pfunPkg)
				{
					pfunPkg(&tadr, czMsg, cl);
				}
			}
		}
		else if(0 == iRt)
		{
			continue;
		}
	}
	return 0;
}

void CUdpSocket::changeBlockingMode(sint32 iSkt, sint32 iNoBlocking)
{
    if(1 == iNoBlocking)
    {
        fcntl(iSkt, F_SETFL, O_NONBLOCK);
    }
    else
    {
        fcntl(iSkt, F_SETFL, fcntl(iSkt, F_GETFL) & ~O_NONBLOCK);
    }
}

sint32  CUdpSocket::udpSend(uint8 *pMsg, sint32 iLen, sint32 iSkt, struct sockaddr_in *pStAddr)
{
    if(!pMsg)
    {
        ENPLOG;
        return -1;
    }
    struct sockaddr_in *pstToAddr = &m_stSerAddr;
    if(pStAddr)
    {
        pstToAddr = pStAddr;
    }

    sint32 iSock = m_iUdpSkt;
    if(iSkt)
    {
        iSock = iSkt;
    }

    socklen_t iSlen = sizeof(struct sockaddr);
    sint32 iSnl = sendto(iSock, pMsg, iLen, 0, (struct sockaddr *)pstToAddr, iSlen);
    if(iSnl != iLen)
    {
        if(errno == ENOBUFS)
        {
            return 0;
        }
		ELOG("sendto Err, errno%d, strNo %s, iSnl %d", errno, strerror(errno), iSnl);
        return -1;
    }
    return 0;
}

sint32  CUdpSocket::udpRecv(uint8 *pMsg, sint32 iLen, sint32 iSkt, struct sockaddr_in *pStAddr, sint32 iBlk)
{
    if(!pMsg || iLen < 0)
    {
        ENPLOG;
        return -1;
    }
    struct sockaddr_in *pstToAddr = &m_stSerAddr;
    if(pStAddr)
    {
        pstToAddr = pStAddr;
    }
    sint32 iSock = m_iUdpSkt;
    if(iSkt)
    {
        iSock = iSkt;
    }
    
    socklen_t iSlen = sizeof(struct sockaddr);
    if(iBlk)
    {
        changeBlockingMode(iSock, 1);// 1 unblock
    }
    sint32 iRcl = recvfrom(iSock, pMsg, iLen, 0, (struct sockaddr*)pstToAddr, &iSlen);
    if(iBlk)
    {
        changeBlockingMode(iSock, 0);
    }
    if(0 >= iRcl && (EWOULDBLOCK != errno || EAGAIN != errno))
    {
        ELOG("Socket errno %d, strerror %s", errno, strerror(errno));
        return -1;
    }
    return iRcl;
}

void CUdpSocket::udpClose()
{
    if(m_iUdpSkt > 0)
    {
        close(m_iUdpSkt);
        m_iUdpSkt = -1;
    }
}


