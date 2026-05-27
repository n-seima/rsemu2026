#ifndef	_classPacketManager_h
#define	_classPacketManager_h

#include "SFC.H"
#include "RSComm.H"
#include "cPACKET_GAMESERVER.h"

class	cCFPM	:	public	SG_PACKET_TRAILER
{
public:
						cCFPM();
	virtual				~cCFPM();

	BOOL				init();
	BOOL				add(void *_lpPacket,int _iPacketSize);
	void				operate();
	inline	void		inlineReset()
	{
		base.wType		=	dSG_PACKET_TRAILER;
		base.wSize		=	0;
		m_iCount		=	0;
	}

	BOOL				send(int _iClient);
};

class	cPacketManager
{
public:
	cCFPM			*m_pClient;

					cPacketManager();
	virtual			~cPacketManager();

	BOOL			init();
	BOOL			add(int _iClientSerial,void *_lpPacket,int _iPacketSize);
	inline	cCFPM*	inlineGetClient(int _iClientSerial)
	{
		if	(_iClientSerial	<	0	||	_iClientSerial	>=	dGAME_MAX_USER_COUNT)
			return	NULL;

		return	&m_pClient[_iClientSerial];
	}

	void			operate();
};

extern	cPacketManager	g_userPM;	//	user packet manager

#endif