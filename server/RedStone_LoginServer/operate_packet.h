#pragma	once

#include	"SFC.h"
#include	"cPACKET_WORLDSERVER.h"

class	cSOCKET;
class	CClient;

extern	void	SendSimpleWork(cSOCKET *_lpServer,int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);

extern	void	OperateSimpleWorldWork(cSOCKET *_lpSocket,WORLDtoSERVERS_SIMPLE_WORK *_lpPacket);
extern	void	OperateWorldLifeSignPacket(cSOCKET *_lpSocket,WORLDtoSERVERS_LIFE_SIGN *_lpPacket);
extern	void	OperateTransAvatarToGVGServerPacket(cSOCKET *_lpSocket,WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER *_lpPacket);
extern	void	OperateTransAvatarToGVGResultPacket(WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT *_lpPacket);	//	아바타 전송 결과 처리

extern	void	OperateTransGuildToGVGServerPacket(cSOCKET *_lpSocket,WORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER *_lpPacket);
extern	void	OperateTransGuildToGVGResultPacket(WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT *_lpPacket);	//	길드 전송 결과 처리
extern	void	OperateChangeGVGServerGuildMaster(WORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER *_lpPacket);

extern	void	OperateW2L_TOSS_ASK_GVG_AVATAR_ORG_DATA(cSOCKET *_lpGVGWorldSocket,W2L_TOSS_ASK_GVG_AVATAR_ORG_DATA *_lpPacket);
extern	void	OperateW2L_ASK_GVG_AVATAR_ORG_DATA_RESULT(W2L_ASK_GVG_AVATAR_ORG_DATA_RESULT *_lpPacket);
extern	void	OperateW2L_GVG_AVATAR_ORG_DATA(W2L_GVG_AVATAR_ORG_DATA* _lpPacket);


