#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "Client.h"
#include "CGame.h"
#include "packetManager.h"

//
//	파티에 가입
void
cACTOR::joinParty(int _iPartySerial)
{
	setPartySerial(_iPartySerial);

	g_game.sendBPUpdatePlayerInfo(this);

	CClient	*lpClient	=	getClient();

	if	(lpClient)
		lpClient->SetPartySerial(_iPartySerial);
}
//
//	파티에서 나왔다.
void
cACTOR::leaveParty()
{
	setPartySerial(0xffff);

	g_game.sendBPUpdatePlayerInfo(this);

	CClient	*lpClient	=	getClient();

	if	(lpClient)
		lpClient->SetPartySerial(0xffff);
}

//
//	파티에서 나왔다.
void
cACTOR::leavePartyByLogout()
{
	if	(getPartySerial()	!=	0xfffF)
	{
		cParty	*lpParty	=	getParty();

		if	(lpParty)
		{
			CLOG("party","leavePartyByLogout [%s]",m_strName);
			g_game.sendWPPartyWork(lpParty->m_wSerial,ePW_LEAVE_PARTY,m_strName);
		}
		else
			CLOG("party","leavePartyByLogout");
	}

	leaveParty();
}

//
//	파티 정보 체크
void
cACTOR::checkPartyInfo()
{
	if	(getPartySerial()	!=	0xffff)
	{
		cParty	*lpParty	=	getParty();

		if	(lpParty	==	NULL)
			CLOG("party","checkPartyInfo");
		else
		if	(m_bf1IsSendReapirPartyDisconnectedStatusMessage	==	FALSE)
		{
			cPARTY_MEMBER*	lpMemberInfo	=	lpParty->getMemberInfoByName(m_strName,m_dwNameHashCode);

			if	(lpMemberInfo)
				if	(lpMemberInfo->m_bf1IsDisconnected)
				{
					m_bf1IsSendReapirPartyDisconnectedStatusMessage	=	TRUE;
					g_game.sendWPPartyWork(getPartySerial(),ePW_IS_CONNECT_USER,m_strName);
				}
		}
	}
}

//
//	파티 리더냐?
BOOL
cACTOR::isPartyLeader()
{
	if (getPartySerial()	==	0xffff)
		return	FALSE;

	cParty	*lpParty=	getParty();

	if (!lpParty)
	{
		CLOG("party","isPartyLeader");

		return	FALSE;
	}

	return	lpParty->isLeader(m_strName);
}

//
//	파티를 얻어라
cParty*
cACTOR::getParty()
{
	if (getPartySerial()	==	0xffff)
		return	NULL;

	cParty	*lpParty	=	g_pPartyManager->get(getPartySerial());

	if (!lpParty || lpParty->getMemberInfoByName(m_strName) == NULL)
	{
		sendDissolutionPartyMessage(eDPC_INCORRECT_PARTY_INFO);
		
		g_game.sendWPCheckPartyInfoByActor(this);

		if (lpParty)
			CLOG("party","getParty 파티에는 없다고 나와요. %d '%s:%s'",getPartySerial(),m_strName,lpParty->m_strPartyName);
		else
			CLOG("party","getParty 파티에는 없다고 나와요. %d '%s:NULL'",getPartySerial(),m_strName);

		leaveParty();

		return	NULL;
	}

	return	lpParty;
}

BOOL
cACTOR::isPartyMember(cACTOR *_lpActor)
{
	cACTOR	*lpThis	=	this;

	if	(isPet() || isSummonBeast())
		lpThis		=	getTamer();

	if	(!lpThis)
		return	FALSE;

	if	(_lpActor->isPet() || _lpActor->isSummonBeast())
	{
		_lpActor	=	_lpActor->getTamer();

		if	(_lpActor==	NULL)
			return	FALSE;
	}

	if	(_lpActor->getPartySerial()	!=	0xffff	&&	_lpActor->getPartySerial() == lpThis->getPartySerial())
		return	TRUE;

	if	(_lpActor->m_wSerialInField	==	m_wTamer)
		return	TRUE;

	if	(_lpActor->m_wSerialInField	==	m_wSerialInField)
		return	TRUE;

	return	FALSE;
}

int
cACTOR::operateDamagePool(int _iDamage,cACTOR *_lpAttacker)
{
	if	(m_damagePool.m_wRemainTime	==	0)
		return	_iDamage;

	cParty	*lpParty		=	getParty();

	WORD	awTargetList[dPARTY_MAXPLAYER];
	int		iSharedCount	=	0;
	int		iLimitRange		=	m_damagePool.m_wRange;
	int		iInRangeMember	=	0;
	int		iReturnDamage	=	_iDamage*m_damagePool.m_wEfficient/100;
	int		iShareDamage	=	_iDamage-iReturnDamage;
	WORD	awUsedMemberStatus[dPARTY_MAXPLAYER][2];

	iLimitRange	*=	iLimitRange;

	memset(awUsedMemberStatus,0,sizeof(awUsedMemberStatus));
	memset(awTargetList,0xffff,sizeof(awTargetList));

//	같은 필드에 없거나 죽었거나 너무 멀리 있는 예외 멤버들 골라냄
	if	(lpParty)
	{
		for	(int iMember=0;iMember<lpParty->m_wMemberCount;iMember++)
		{
			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(!lpMember	||	!isOnSameField(lpMember)	||	lpMember->isDeath())
				continue;

			int		iRange		=	GetOvalRange(m_pos.x,m_pos.y,lpMember->m_pos.x,lpMember->m_pos.y);

			if	(iRange		>	iLimitRange)
				continue;

			awTargetList[iInRangeMember++]	=	lpMember->m_wSerialInField;

			if	(m_wSerialInField	==	lpMember->m_wSerialInField)
				awUsedMemberStatus[iMember][0]		=	TRUE;
		}
	}
	else
	{
		awTargetList[iInRangeMember++]	=	m_wSerialInField;
		awUsedMemberStatus[0][0]		=	TRUE;
	}

	if	(iInRangeMember	==	0)
		return	_iDamage;

	SG_WIDE_AREA_DAMAGE		packet;

	packet.base.set(sizeof(packet),dSG_WIDE_AREA_DAMAGE);
	packet.bf6Count		=	0;
	packet.bf12Skill	=	0;
	packet.bf8Level		=	0;
	packet.bf12Caster	=	m_wSerialInField;
	packet.bf1IsDamagePool=	FALSE;

	BOOL	bIsCheckPet	=	FALSE;

	while(1)
	{
		int		iTopLevel=0;
		cACTOR	*lpCurrentTarget	=	NULL;

		if	(bIsCheckPet)	//	파티 멤버중에 맞을 넘이 없다. 펫 뒤져!!
		{
			for	(int iMember=0;iMember<iInRangeMember;iMember++)
			{
				cACTOR	*lpMember	=	m_lpField->getActor(awTargetList[iMember]);
				cACTOR	*lpPet		=	NULL;
				
				if	(!lpMember)
					continue;

				for	(int iPet=awUsedMemberStatus[iMember][1];iPet<4;iPet++)	//	펫 4개
				{
					if	(iPet	==	0)
						lpPet	=	lpMember->getPetActor(0);
					else
					if	(iPet	==	1)
						lpPet	=	lpMember->getPetActor(1);
					else
					if	(iPet	==	2)
						lpPet	=	lpMember->getSummonBeastActor(0);
					else
					if	(iPet	==	3)
						lpPet	=	lpMember->getSummonBeastActor(1);

					awUsedMemberStatus[iMember][1]++;

					if	(!lpPet	||	lpPet->isDeath())	//	없다.
						continue;

					int		iRange		=	GetOvalRange(m_pos.x,m_pos.y,lpPet->m_pos.x,lpPet->m_pos.y);

					if	(iRange		>	iLimitRange)
						continue;

					lpCurrentTarget		=	lpPet;	//	이넘이다.

					break;
				}
			}
		}
		else
		{
			for	(int iMember=0;iMember<iInRangeMember;iMember++)
			{
				if	(awUsedMemberStatus[iMember][0])
					continue;

				cACTOR	*lpMember	=	m_lpField->getActor(awTargetList[iMember]);

				if	(lpMember	&&	lpMember->getLevel()			>=	iTopLevel)
				{
					lpCurrentTarget					=	lpMember;
					iTopLevel						=	lpMember->getLevel();
					awUsedMemberStatus[iMember][0]	=	1;

					break;
				}
			}
		}

		if	(lpCurrentTarget	==	NULL)	//	타겟이 없다.
		{
			if	(bIsCheckPet)
			{
				if	(iSharedCount	==	0)	//	펫 체크 하고 있는데 
					return	_iDamage;

				break;
			}

			bIsCheckPet	=	TRUE;
			continue;
		}

		packet.aData[iSharedCount].wActor			=	lpCurrentTarget->m_wSerialInField;
		packet.bf6Count++;
		iSharedCount++;

		if	(iSharedCount	>=	m_damagePool.m_wLinkCount)
			break;
	}

	iShareDamage	=	max(iShareDamage/iSharedCount,1);

	for	(int iTarget=0;iTarget<iSharedCount;iTarget++)
	{
		cACTOR	*lpTarget	=	m_lpField->getActor(packet.aData[iTarget].wActor);

		if	(lpTarget)
			lpTarget->reduceHP(iShareDamage,_lpAttacker,0,TRUE);

		packet.aData[iTarget].wPhysicalDamage	=	max(iShareDamage/100,1);
		packet.aData[iTarget].wMagicDamage		=	0;
		packet.aData[iTarget].dwResultField		=	dATTACK_RESULT_DAMAGE_POOL+dATTACK_RESULT_FIELD_HIT;
	}
	
	packet.bf1IsDamagePool	=	TRUE;
	packet.base.wSize		=	sizeof(packet)-sizeof(packet.aData)+packet.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);
	packet.sRemainCP		=	m_iCP/100;

	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);

	sendEtcWork(eEW_SHARE_DAMAGE_BY_DAMAGE_POOL,(_iDamage-iReturnDamage)/100);

	return	iReturnDamage;
}

void
cACTOR::sendUnionJoinParty(cACTOR* _lpAsker)
{
	SG_ASK_JOIN_UNION_PARTY	packet;
	cParty*		lpParty = _lpAsker->getParty();


	if(!_lpAsker)
		return;

	if(!lpParty)
		return;

	ZeroMemory(&packet , sizeof(SG_ASK_JOIN_UNION_PARTY));
	packet.base.set(sizeof(SG_ASK_JOIN_UNION_PARTY),dSG_ASK_JOIN_UNION_PARTY);

	strcpy(packet.strAskerName,_lpAsker->m_strName);		//	파티 참가 요청한 사람 이름
	
	m_dwRequestPartyPlayerNameHashCode	=	GetHashCode(_lpAsker->m_strName);
	
	packet.wAskerJob					=	_lpAsker->m_wJob;
	packet.wAskerLevel					=	_lpAsker->m_iLevel;
	packet.wAskerPartyCount				=	lpParty->getMemberCount();
	packet.wAskerPartyAverageLevel		=	lpParty->getAverageLevel();			//	
	packet.wAskerPartyPurpose			=	lpParty->m_bf3PartyObject;			//	파티의 목적.
	strcpy(packet.strPartyName , lpParty->m_strPartyName);
	
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

void
cACTOR::sendJoinParty(int _iAskerLevel,int _iAskerJob,char *_lpstrAskerName,BOOL _bIsJoinPartyApplication /* = FALSE */, BOOL _bIsGuildMemberJoinParty/* = FALSE */)
{
	SG_ASK_JOIN_PARTY	packet;
	
	packet.base.set(sizeof(SG_ASK_JOIN_PARTY),dSG_ASK_JOIN_PARTY);
	strcpy(packet.strAskerName,_lpstrAskerName);		//	파티 참가 요청한 사람 이름
	
	m_dwRequestPartyPlayerNameHashCode	=	GetHashCode(_lpstrAskerName);
	
	packet.bf1IsJoinPartyApplication	=	_bIsJoinPartyApplication;
	packet.bf1IsGuildMemberJoinParty	=	_bIsGuildMemberJoinParty;
	packet.wAskerJob					=	_iAskerJob;
	packet.wAskerLevel					=	_iAskerLevel;			//	요청한 넘의 직업과 레벨
	
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

BOOL
cACTOR::acceptJoinPartyApplication(char *_lpstrAskerName,int _iAnswer)
{
	cParty	*lpParty	=	getParty();

	if	(_iAnswer	==	eJPA_OK)
	{
		if	(isPartyLeader() == FALSE)	//	파티 리더가 아니다.
			return	FALSE;

		lpParty	=	getParty();

		if	(!lpParty || lpParty->m_wSerial	==	0xffff)
			return	FALSE;

		if	(lpParty->isFullParty()	||	lpParty->m_bf1IsOpenParty	==	FALSE)
			return	FALSE;

		if	(lpParty->m_wGateField	!=	0xffff)	//	비밀던젼에 있다.
			return	FALSE;
	}

	g_game.sendWPJoinPartyApplicationAnswer(m_strName,_lpstrAskerName,_iAnswer,lpParty);

	return	TRUE;
}

BOOL
cACTOR::receivePermitJoinPartyApplication(char *_lpstrPartyName,int _iPartySerial)
{
	if	(getPartySerial()	!=	0xffff)	//	파티에 소속해 있다.
		return	FALSE;

	g_game.sendWPJoinPartyByApplication(_lpstrPartyName,_iPartySerial,this);

	return	TRUE;
}