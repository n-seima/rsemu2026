#pragma	once

#include <winsock2.h>
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_DBSERVER.h"

class	CClient;

extern	void	OperateTransAvatarToGVGPacket(CClient *_lpWorldServer,WDMSG_TRANS_AVATAR_TO_GVG *_lpPacket);
extern	void	OperateW2D_ASK_GVG_AVATAR_ORG_DATA(CClient *_lpWorldServer,W2D_ASK_GVG_AVATAR_ORG_DATA *_lpPacket);
extern	void	SendSimpleWork(CClient *_lpServer,int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
