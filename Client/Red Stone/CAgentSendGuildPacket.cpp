#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"

//
//	길드 만들기 전송
BOOL
CAgent::sendMakeGuild(char *_lpstrGuildName,char *_lpstrSubGuildManter)
{
	CG_CREATE_GUILD				packet;

	if (g_hero.m_bf1IsWaitGuildCommandResult)	return	FALSE;

	g_hero.m_bf1IsWaitGuildCommandResult	=	TRUE;

	packet.base.set(sizeof(CG_CREATE_GUILD),dCG_CREATE_GUILD);

	strcpy(packet.strGuildName,_lpstrGuildName);
	strcpy(packet.strSubGuildMasterName,_lpstrSubGuildManter);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	길드 정보 요청
BOOL
CAgent::sendAskGuildInfo(BOOL _bIsIgnoreSkill)
{
	CG_ASK_GUILD_INFO			packet;

	packet.base.set(sizeof(CG_ASK_GUILD_INFO),dCG_ASK_GUILD_INFO);
	packet.wIsIgnoreSkillData	=	_bIsIgnoreSkill;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	길드 멤버 리스트 요청
BOOL
CAgent::sendAskGuildMemberList(BOOL _bISInit)
{
	CG_ASK_GUILD_MEMBER_LIST	packet;

	packet.base.set(sizeof(packet),dCG_ASK_GUILD_MEMBER_LIST);

	packet.bIsinit	=	_bISInit;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	길드 참가 신청
BOOL
CAgent::sendRequestJoinGuild(int _iTargetSerial)
{
	CG_ASK_JOIN_GUILD			packet;

	packet.wTargetSerial	=	_iTargetSerial;

	packet.base.set(sizeof(CG_ASK_JOIN_GUILD),dCG_ASK_JOIN_GUILD);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//
BOOL
CAgent::sendReplyJoinGuild(int _iReply,char *_lpstrName,int _iSerial)
{
	CG_REPLY_ASK_JOIN_GUILD		packet;

	packet.base.set(sizeof(packet),dCG_REPLY_ASK_JOIN_GUILD);

	packet.wResult		=	_iReply;
	packet.wAskerSerial	=	_iSerial;

	strcpy(packet.strAskerName,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	길드 스킬레벨 올리기
BOOL
CAgent::sendIncreaseGuildSkillLevel(int _iSkill)
{
	CG_INCREASE_GUILD_SKILL_LEVEL	packet;

	mINIT_PACKET(packet,dCG_INCREASE_GUILD_SKILL_LEVEL);
	packet.wSkill	=	_iSkill;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//
BOOL
CAgent::sendAskGuildBattleInfoByTimeOrder(int _iTimeOrder)
{
	CG_ASK_GUILD_BATTLE_SCHEDULE	packet;

	packet.base.set(sizeof(packet),dCG_ASK_GUILD_BATTLE_SCHEDULE);
	packet.wTimeOrder	=	_iTimeOrder;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	길드전 신청
BOOL
CAgent::sendRequestGuildBattle(int _iTimeOrder,int _iFieldOrder)
{
	CG_REQUEST_GUILD_BATTLE	packet;

	packet.base.set(sizeof(packet),dCG_REQUEST_GUILD_BATTLE);
	packet.wTimeOrder	=	_iTimeOrder;
	packet.wFieldOrder	=	_iFieldOrder;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	길드전 취소
BOOL
CAgent::sendCancelGuildBattle(int _iTimeOrder)
{
	CG_CANCEL_GUILD_BATTLE	packet;

	packet.base.set(sizeof(packet),dCG_CANCEL_GUILD_BATTLE);
	packet.wTimeOrder	=	_iTimeOrder;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendCloseGuildBattleScheduler()
{
	CG_CLOSE_GUILD_BATTLE_SCHEDULER	packet;

	packet.base.set(sizeof(packet),dCG_CLOSE_GUILD_BATTLE_SCHEDULER);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	길드마크 선택
BOOL
CAgent::sendSelectGuildMark(CGuildMarkInfo *_lpMarkInfo)
{
	CG_SELECT_GUILD_MARK	packet;

	packet.base.set(sizeof(packet),dCG_SELECT_GUILD_MARK);
	memcpy(&packet.guildMarkInfo,_lpMarkInfo,sizeof(packet.guildMarkInfo));

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	길드마크 정보 요청
BOOL
CAgent::sendAskGuildMarkInfo(int _iGuildSerial)
{
	CG_ASK_GUILD_MARK_INFO	packet;

	packet.base.set(sizeof(packet),dCG_ASK_GUILD_MARK_INFO);

	packet.wGuildSerial	=	_iGuildSerial;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendChangeGuildNotice(char *_lpstrNotice)
{
	CG_ASK_CHANGE_GUILD_NOTICE				packet;

	packet.base.set(sizeof(packet),dCG_ASK_CHANGE_GUILD_NOTICE);
	strcpy(packet.strNotice,_lpstrNotice);

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendChangeGuildNotice(char *_lpstrNotice)

BOOL
CAgent::sendChangeGuildTax(int _iTax)
{
	CG_ASK_CHANGE_GUILD_EXP_TAX				packet;

	packet.base.set(sizeof(packet),dCG_ASK_CHANGE_GUILD_EXP_TAX);
	packet.wTax	=	_iTax;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendChangeGuildTax(s_iBookedChangeGuildTax)

BOOL
CAgent::sendDismissSubGuildMaster(char *_lpstrName)
{
	CG_ASK_DISMISS_SUB_GUILD_MASTER		packet;

	packet.base.set(sizeof(packet),dCG_ASK_DISMISS_SUB_GUILD_MASTER);
	strcpy(packet.strSubGuildMaster,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAppointmentSubGuildMaster(char *_lpstrName)
{
	CG_ASK_APPOINTMENT_SUB_GUILD_MASTER		packet;

	packet.base.set(sizeof(packet),dCG_ASK_APPOINTMENT_SUB_GUILD_MASTER);
	strcpy(packet.strSubGuildMaster,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAppointmentGuildCongress(char *_lpstrName)
{
	CG_ASK_APPOINTMENT_GUILD_CONGRESS	packet;

	packet.base.set(sizeof(packet),dCG_ASK_APPOINTMENT_GUILD_CONGRESS);
	strcpy(packet.strGuildCongress,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendDismissGuildCongress(char *_lpstrName)
{
	CG_ASK_DISMISS_GUILD_CONGRESS	packet;

	packet.base.set(sizeof(packet),dCG_ASK_DISMISS_GUILD_CONGRESS);
	strcpy(packet.strGuildCongress,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendRegistRegularGuildMember(char *_lpstrName)
{
	CG_REGIST_REGULAR_GUILD_MEMBER	packet;

	packet.base.set(sizeof(packet),dCG_REGIST_REGULAR_GUILD_MEMBER);
	strcpy(packet.strName	,	_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendBanishGuildMember(char *_lpstrName)
{
	CG_ASK_BANISH_GUILD_MEMBER				packet;

	packet.base.set(sizeof(packet),dCG_ASK_BANISH_GUILD_MEMBER);
	strcpy(packet.strMember,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendChangeGuildMaster(char *_lpstrName)
{
	CG_ASK_CHANGE_GUILD_MASTER			packet;

	packet.base.set(sizeof(packet),dCG_ASK_CHANGE_GUILD_MASTER);
	strcpy(packet.strMaster,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendLeaveGuild()
{
	CG_ASK_LEAVE_GUILD						packet;

	packet.base.set(sizeof(packet),dCG_ASK_LEAVE_GUILD);

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendLeaveGuild()

BOOL
CAgent::sendDissolutionGuild()
{
	CG_ASK_DISSOLUTION_GUILD	packet;

	packet.base.set(sizeof(packet),dCG_ASK_DISSOLUTION_GUILD);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendReadyToMakeGuild()
{
	CG_READY_TO_MAKE_GUILD					packet;

	packet.base.set(sizeof(packet),dCG_READY_TO_MAKE_GUILD);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendGuildCheckSum(int _iCheckSum)
{
	CG_GUILD_CHECK_SUM	packet;

	packet.base.set(sizeof(packet),dCG_GUILD_CHECK_SUM);
	packet.iCheckSum	=	_iCheckSum;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

