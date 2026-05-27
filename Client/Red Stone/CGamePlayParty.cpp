#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "secretDungeonDefine.H"
#include "cParty.h"

int					CGamePlay::s_iPartyCount	=	0;
int					CGamePlay::s_iSelectParty	=	0xffff;
CPatyInfo			CGamePlay::s_partyInfo;
//CUnionPartyInfo		CGamePlay::s_unionPartyInfo;
char				CGamePlay::s_strPartyJoinAskerName[dNAME_LENGTH];
int					CGamePlay::s_iFirstPartyIndex	=	0;
int					CGamePlay::s_iLastPartyIndex	=	0;


#define dNAME_WIDTH								98
#define dNAME_WIDTH_PLUS_LR						(dNAME_WIDTH + 4)
#define	dMINIMUM_MEMBER_NAME_BAR_SIZE_AT_FIELD	50
#define	dMEMBER_NAME_BAR_POS_AT_FIELD			492
//
//	파티창 초기화
void
CGamePlay::ResetPartyInfo()
{
	s_partyInfo.reset();
	s_partyInfo.resetPartyList();
}

void
CGamePlay::ChangePartyStatus(SG_CHANGE_PARTY_STATUS *_lpPacket)
{
	if (s_partyInfo.m_bf3GoldShareMethod	!=	_lpPacket->bf3GoldShareMethod)
		AddSystemMessage(WHITE,dMSG_CHANGE_GOLD_SHARING_METHOD_FORM,s_partyInfo.getGoldSharingMethodString(_lpPacket->bf3GoldShareMethod));
	if (s_partyInfo.m_bf3ItemShareMethod	!=	_lpPacket->bf3ItemShareMethod)
		AddSystemMessage(WHITE,dMSG_CHANGE_ITEM_SHARING_METHOD_FORM,s_partyInfo.getItemSharingMethodString(_lpPacket->bf3ItemShareMethod));
	if (s_partyInfo.m_bf3PartyObject		!=	_lpPacket->bf3PartyObject	)
		AddSystemMessage(WHITE,dMSG_CHANGE_PARTY_OBJECT_OBJECT_FORM,s_partyInfo.getPartyObjectString(_lpPacket->bf3PartyObject));
	if (s_partyInfo.m_bf1IsOpenParty		!=	_lpPacket->bf1IsOpenParty	)
		AddSystemMessage(WHITE,dMSG_CHANGE_PARTY_STATUS_FORM,s_partyInfo.getPartyStatusString(_lpPacket->bf1IsOpenParty));

	s_partyInfo.m_bf3GoldShareMethod=	_lpPacket->bf3GoldShareMethod;
	s_partyInfo.m_bf3ItemShareMethod=	_lpPacket->bf3ItemShareMethod;
	s_partyInfo.m_bf3PartyObject	=	_lpPacket->bf3PartyObject	;
	s_partyInfo.m_bf1IsOpenParty	=	_lpPacket->bf1IsOpenParty	;
}

//
//	파티정보 설정
void
CGamePlay::SetPartyInfo(SG_PARTY_INFO *_lpPartyInfo)
{
	BOOL	bIsFirst	=	FALSE;

	if	(s_partyInfo.m_wMemberCount	==	0	&&	s_partyInfo.m_bIsExistPreviousField	==	FALSE)
		bIsFirst	=	TRUE;

	CPartyInfoInPacket	*lpPartyInfo=	&_lpPartyInfo->partyInfo;

	s_partyInfo.m_wMemberCount		=	lpPartyInfo->m_bf4MemberCount;
	s_partyInfo.m_wIsWaitPartyInfo	=	FALSE;
	s_partyInfo.m_wGateField		=	lpPartyInfo->m_wGateField;
	s_partyInfo.m_bf10GateArea		=	lpPartyInfo->m_bf10GateArea		;
	s_partyInfo.m_bf3GoldShareMethod=	lpPartyInfo->m_bf3GoldShareMethod;
	s_partyInfo.m_bf3ItemShareMethod=	lpPartyInfo->m_bf3ItemShareMethod;
	s_partyInfo.m_bf3PartyObject	=	lpPartyInfo->m_bf3PartyObject	;
	s_partyInfo.m_bf1IsOpenParty	=	lpPartyInfo->m_bf1IsOpenParty	;
	s_partyInfo.m_wSerial			=	lpPartyInfo->m_bf10Serial;

	if	(s_partyInfo.m_wGateField	==	g_hero.m_wCurrentField)
		g_map.m_area.m_wInstanceFieldGate	=	lpPartyInfo->m_bf10GateArea;

	g_hero.setPartySerial(lpPartyInfo->m_bf10Serial);

	strcpy(s_partyInfo.m_strName,lpPartyInfo->m_strName);
	s_partyInfo.resetPartyList();

	for (int i=0;i<s_partyInfo.m_wMemberCount;i++)
	{
		CPartyMemberInfoInPacket	*lpInfo		=	&lpPartyInfo->m_aMemberList[i];

		s_partyInfo.setMemberInfo(i,lpInfo->m_strName,lpInfo->m_bf8Job,lpInfo->m_bf10Level,lpInfo->m_wSerial,lpInfo->m_wFieldSerial,bIsFirst,
									lpInfo->m_bf9IF,lpInfo->m_bf4IFFloor,lpInfo->m_bf1IsDisconnected,lpInfo->m_bf4PlaySecretDungeonCount,lpInfo->m_iDuelVP);
	}

	if	(lpPartyInfo->m_bf1IsCreate)
		g_msgBox.cPopup(dMSG_CREATE_PARTY,dMSG_CREATE_PARTY_MESSAGE,dMSG_OK);
	else
	{
		if	(bIsFirst)
			AddSystemMessage(LTGREEN,dMSG_JOIN_PARTY_FORM,s_partyInfo.m_strName);
	}
}

void
CGamePlay::OperateUnionPartyMemberInfo(SG_UNION_PARTY_MEMBER_INFO *_info)
{
	strcpy(s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][_info->wPartyMemberIndex].m_strName , _info->Info.m_strName);
	s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][_info->wPartyMemberIndex].m_wHP					= _info->Info.m_wHP;
	s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][_info->wPartyMemberIndex].m_dwState				= _info->Info.m_dwState;
	s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][_info->wPartyMemberIndex].m_wUnionPartySerial	= _info->wUnionPartyIndex;
}

void
CGamePlay::OperateUnionPartyInfo(SG_UNION_PARTY_INFO *_info)
{
	for (int i =0  ;i < _info->wEffectivenessCount ; i++)
	{
		strcpy(s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][i].m_strName			, _info->Info[i].m_strName);
		s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][i].m_wHP					= _info->Info[i].m_wHP;
		s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][i].m_dwState				= _info->Info[i].m_dwState;
		s_unionPartyInfo.m_wUnionPartyInfo[_info->wUnionPartyIndex][i].m_wUnionPartySerial		= _info->wUnionPartyIndex;
	}
}

//
//연합 파티가 해체 되업음을 알려 준다.
void
CGamePlay::OperateUnionPartyDissolution(SG_UNION_PARTY_WORK *_lpDissolutionInfo)
{
	s_unionPartyInfo.reset();

	switch(_lpDissolutionInfo->wValue[0])
	{
	case eDPC_NORMAL:
		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_DISSOLUTION_UNION_PARTY);
		break;
	}
}


void
CGamePlay::ReplayJoinUnionParty(SG_UNION_PARTY_WORK *_lpAskerInfo)
{
	BOOL bReplay = _lpAskerInfo->wValue[0];

	if (bReplay)
	{
		s_unionPartyInfo.m_bIsActiveUnionParty	=	 TRUE;
		s_unionPartyInfo.m_wUnionPartyCount		=	_lpAskerInfo->wValue[1];			//몇 연합 파티인가?
	}
}


void
CGamePlay::AskJoinUnionParty(SG_ASK_JOIN_UNION_PARTY *_lpAskerInfo)
{
	char	strMessage[1024];		
	char	*lpstrObject	=	s_partyInfo.getPartyObjectString(_lpAskerInfo->wAskerPartyPurpose);
	ZeroMemory(strMessage , 1024);

	strcpy(s_strPartyJoinAskerName,_lpAskerInfo->strAskerName);
	sprintf(strMessage,dMSG_PARTY_LEADER_AND_MEMBER_COUNT_FORM,s_strPartyJoinAskerName,_lpAskerInfo->wAskerLevel,g_pstrHeroJobName[_lpAskerInfo->wAskerJob]);
	strcat(strMessage , "\n");
	strcat(strMessage , _ms(dMSG_PARTY_MEMBER_INFO_SUMMARY_FORM , _lpAskerInfo->wAskerPartyCount , _lpAskerInfo->wAskerPartyAverageLevel));
	strcat(strMessage , "\n");
	strcat(strMessage , dMSG_PARTY_OBJECT );
	strcat(strMessage , " " );
	strcat(strMessage , _ms("<c:LTGREEN>[%s]<n>" , lpstrObject));
		
	g_msgBox.cPopup(dMSG_INVITATION_UNION_PARTY,strMessage,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_ACCEPT_JOIN_UNION_PARTY_REQUEST,eMBI_DECLINE_JOIN_UNION_PARTY_REQUEST);
}

//
//	파티 참가 요청
void
CGamePlay::AskJoinParty(SG_ASK_JOIN_PARTY *_lpAskerInfo)
{
	char	strMessage[256];		

	if	(_lpAskerInfo->bf1IsJoinPartyApplication)
		s_partyInfo.addRequestJoinPartyUser(_lpAskerInfo->strAskerName,_lpAskerInfo->wAskerJob,_lpAskerInfo->wAskerLevel);
	else
	{
		strcpy(s_strPartyJoinAskerName,_lpAskerInfo->strAskerName);
		sprintf(strMessage,dMSG_ARE_YOU_JOIN_PARTY_FORM,s_strPartyJoinAskerName,g_pstrHeroJobName[_lpAskerInfo->wAskerJob],_lpAskerInfo->wAskerLevel);

		g_msgBox.cPopup(dMSG_RECEIVE_PARTY_JOIN_REQUEST,strMessage,dMSG_YES,dMSG_NO);		
		
		if(_lpAskerInfo->bf1IsGuildMemberJoinParty)
			g_msgBox.setButtonId(eMBI_ACCEPT_GUILD_MEMBER_JOIN_PARTY_REQUEST,eMBI_DECLINE_GUILD_MEMBER_JOIN_PARTY_REQUEST);
		else
			g_msgBox.setButtonId(eMBI_ACCEPT_JOIN_PARTY_REQUEST,eMBI_DECLINE_JOIN_PARTY_REQUEST);
	}
}

//
//	파티 멤버 추가
void
CGamePlay::JoinPartyMember(SG_JOIN_PARTY_MEMBER *_lpMemberInfo)
{
	s_partyInfo.addMember(	_lpMemberInfo->wIndex,_lpMemberInfo->strName,_lpMemberInfo->wJob,_lpMemberInfo->wLevel,
							_lpMemberInfo->wSerialInField,_lpMemberInfo->wField,
							_lpMemberInfo->bf10IFSerial,_lpMemberInfo->bf4IFFloor,_lpMemberInfo->bf4SPC,
							_lpMemberInfo->iDuelVP);	//	시리얼(필드에서 같은 필드에 없으면 0xffff)

	if (STRICMP(g_hero.m_strName,_lpMemberInfo->strName) != 0)
		AddSystemMessage(WHITE,dMSG_ADD_PARTYMEMBER_FORM,_lpMemberInfo->strName,g_pstrHeroJobName[_lpMemberInfo->wJob],_lpMemberInfo->wLevel);
}

//
//	파티에서 이탈
void
CGamePlay::OperatePartyWork(SG_PARTY_WORK *_lpPacket)
{
	CPartyMemberInfo	*lpMember	=	&s_partyInfo.m_aMemberList[_lpPacket->wMemberIndex];

	switch(_lpPacket->wWork)
	{
		case	ePW_LEAVE_PARTY		:
		case	ePW_BANISH_MEMBER	:
			if (lpMember->m_wSerial	==	g_lpHero->m_wSerial)
			{
				AddSystemMessage(LTGREEN,dMSG_YOU_LEAVE_PARTY);
				s_partyInfo.reset();

				g_map.m_area.m_wInstanceFieldGate	=	0xffff;
			}
			else
			{
				AddSystemMessage(LTGREEN,dMSG_LEAVE_PARTY_FORM,lpMember->m_strName);

				s_partyInfo.removeMember(_lpPacket->wMemberIndex);	//	시리얼(필드에서 같은 필드에 없으면 0xffff)

				if	(s_partyInfo.m_wMemberCount	<=	1)
				{
					AddSystemMessage(LTGREEN,dMSG_DISSOLUTION_PARTY);
					s_partyInfo.reset();
				}
				else
					AddSystemMessage(LTGREEN,dMSG_CHANGE_PARTY_LEADER_FORM,s_partyInfo.m_aMemberList[0].m_strName);
			}
			break;

		case	ePW_CHANGE_LEADER	:	
			s_partyInfo.changeLeader(_lpPacket->wMemberIndex);	//	시리얼(필드에서 같은 필드에 없으면 0xffff)
			AddSystemMessage(LTGREEN,dMSG_CHANGE_PARTY_LEADER_FORM,s_partyInfo.m_aMemberList[0].m_strName);
			break;

		case	ePW_IS_CONNECT_USER	:	
			s_partyInfo.changeConnectStatus(_lpPacket->wMemberIndex,FALSE);	//	시리얼(필드에서 같은 필드에 없으면 0xffff)
			break;
	}
}

//
//	파티멤버 정보 업데이트
void
CGamePlay::ChangePartyName(char *_lpstrPartyName)
{
	strcpy(s_partyInfo.m_strName,_lpstrPartyName);

	AddSystemMessage(LTGREEN,dMSG_CHANGE_PARTY_NAME_FORM,_lpstrPartyName);
}

//
//	파티 해체 메시지
void
CGamePlay::DissolutionParty()
{
	if (s_partyInfo.m_wMemberCount	>	0)
		AddSystemMessage(LTGREEN,dMSG_DISSOLUTION_PARTY);

	s_partyInfo.reset();

	if (g_map.m_area.m_wInstanceFieldGate	!=	0xffff)
		s_agent.sendAskDoorInfo(g_map.m_area.m_wInstanceFieldGate);

	g_map.m_area.m_wInstanceFieldGate	=	0xffff;
}

void
CGamePlay::UpdatePartyMemberHP(SG_UPDATE_PARTY_MEMBER_HP *_lpPacket)
{
	CActor	*lpActor	=	g_am.getTestedActor(_lpPacket->wActor,FALSE);

	if	(!lpActor || lpActor->isHero())
		return;

	lpActor->m_iMaxHP	=	_lpPacket->dwMaxHP;
	
	LONGLONG	llMaxHp	=	lpActor->m_iMaxHP;
	lpActor->m_iHP		=	(int)(llMaxHp * _lpPacket->wRemainHPRate/60000);

	CPartyMemberInfo	*lpMember	=	s_partyInfo.getMemberByName(lpActor->m_strName);

	if	(!lpMember)
		return;

	llMaxHp				=	_lpPacket->wRemainHPRate;
	lpMember->m_wHP		=	llMaxHp*100/60000;
}

//
//	파티멤버 정보 업데이트
void
CGamePlay::UpdatePartyMemberInfo(SG_UPDATE_PARTY_MEMBER_INFO *_lpPartyInfo)
{
	int	iMemberIndex	=	_lpPartyInfo->bIndex;
	int	iRestHP			=	_lpPartyInfo->bRestHp;
	int	iLevel			=	_lpPartyInfo->wLevel;
	int	iFieldSerial	=	_lpPartyInfo->wField;

	s_partyInfo.m_aMemberList[iMemberIndex].m_wHP					=	iRestHP;
	s_partyInfo.m_aMemberList[iMemberIndex].m_wLevel				=	iLevel;
	s_partyInfo.m_aMemberList[iMemberIndex].m_wInstanceField		=	_lpPartyInfo->bf10InstanceField;
	s_partyInfo.m_aMemberList[iMemberIndex].m_wInstanceFieldFloor	=	_lpPartyInfo->bf4InstanceFieldFloor;
	s_partyInfo.m_aMemberList[iMemberIndex].m_wIsDisconnected		=	_lpPartyInfo->bf1IsDisconnected;
	s_partyInfo.m_aMemberList[iMemberIndex].m_wPlaySecretDungeonCount=	_lpPartyInfo->bf4SecretDungeonPlayCount;
	s_partyInfo.m_aMemberList[iMemberIndex].m_wJob					=	_lpPartyInfo->bf8Job;
	s_partyInfo.m_aMemberList[iMemberIndex].m_iDuelVP				=	_lpPartyInfo->iDuelVP;

	if	(iFieldSerial	!=	0xffff)
		s_partyInfo.m_aMemberList[iMemberIndex].m_wFieldSerial	=	iFieldSerial;

	if	(_lpPartyInfo->bf1IsDisconnected	&&	iMemberIndex	==	0)
	{
		s_partyInfo.checkDisconnectLeader();

		AddSystemMessage(LTGREEN,dMSG_CHANGE_PARTY_LEADER_FORM,s_partyInfo.m_aMemberList[0].m_strName);
	}

	CActor	*lpActor	=	g_am.getTestedActor(s_partyInfo.m_aMemberList[iMemberIndex].m_wSerial,FALSE);

	if	(!lpActor || lpActor->isHero())
		return;

	lpActor->m_iHP		=	iRestHP*lpActor->m_iMaxHP/100;
	lpActor->m_wLevel	=	iLevel;
}

void
CGamePlay::DrawActorNameBarAtFieldForUnionParty(int _iX,int _iY,char *_lpstrName,
												int _iHPRate,BOOL _bIsNotSameField,int _bIsDisConnected,DWORD _dwTextColor)
{
	int		iNameWidth		=	dNAME_WIDTH;
	int		iBarWidth		=	(iNameWidth+5)*_iHPRate/100;
	int		iNameBarIndex	=	eNBI_RED_LEFT;
	
	if	(_bIsNotSameField)
	{
		iNameBarIndex	=	eNBI_GRAY_LEFT;
		iBarWidth		=	iNameWidth+5;
	}
	
	if	(_bIsDisConnected)
	{
		iNameBarIndex	=	eNBI_BLACK_LEFT;
		iBarWidth		=	iNameWidth+5;
	}
	
	g_sprNameBar.Put			(_iX,_iY,0				,100,100,dPUT_HALF_BLENDING);		//5
	g_sprNameBar.putClipedImage	(_iX,_iY,2				,iNameWidth-2,100,100,dPUT_HALF_BLENDING);
	g_sprNameBar.Put			(_iX+iNameWidth-2,_iY,1				,100,100,dPUT_HALF_BLENDING);		//5
	
	g_sprNameBar.putClipedImage	(_iX-2,_iY,iNameBarIndex  ,iBarWidth-5);		//5
	g_sprNameBar.putClipedImage	(_iX-2,_iY,iNameBarIndex+2,min(iBarWidth-5,iNameWidth));
	if(_iHPRate == 100)
		g_sprNameBar.putClipedImage	(_iX+iNameWidth-2,_iY,iNameBarIndex+1  ,iBarWidth-5);		//5
	
	s_sText.put(_iX,_iY+3,_dwTextColor,_lpstrName);
}


void
CGamePlay::DrawActorNameBarAtField(int _iX,int _iY,char *_lpstrName,int _iHPRate,int _iSerial,int _bIsDisConnected,DWORD _dwTextColor,int _iJob)
{
	//int		iNameWidth		=	max(s_sText.getPixelSize(_lpstrName)+5,dMINIMUM_MEMBER_NAME_BAR_SIZE_AT_FIELD);
	int		iNameWidth		=	dNAME_WIDTH;
	int		iBarWidth		=	(iNameWidth+5)*_iHPRate/100;
	int		iNameBarIndex	=	eNBI_RED_LEFT;

	if	(_iSerial		==	0xffff)
	{
		iNameBarIndex	=	eNBI_GRAY_LEFT;
		iBarWidth		=	iNameWidth+5;
	}

	if	(_bIsDisConnected)
	{
		iNameBarIndex	=	eNBI_BLACK_LEFT;
		iBarWidth		=	iNameWidth+5;
	}

	g_sprNameBar.Put			(_iX,_iY,0				,100,100,dPUT_HALF_BLENDING);		//5
	g_sprNameBar.putClipedImage	(_iX,_iY,2				,iNameWidth-2,100,100,dPUT_HALF_BLENDING);
	g_sprNameBar.Put			(_iX+iNameWidth-2,_iY,1				,100,100,dPUT_HALF_BLENDING);		//5

 	g_sprNameBar.putClipedImage	(_iX-2,_iY,iNameBarIndex  ,iBarWidth-5);		//5
 	g_sprNameBar.putClipedImage	(_iX-2,_iY,iNameBarIndex+2,min(iBarWidth-5,iNameWidth));
	if(_iHPRate == 100)
 		g_sprNameBar.putClipedImage	(_iX+iNameWidth-2,_iY,iNameBarIndex+1  ,iBarWidth-5);		//5


	s_sText.put(_iX,_iY+3,_dwTextColor,_lpstrName);
}

void
CGamePlay::DrawDuelActorNameBar(int _iX,int _iY,int _iWidth,int _iFillRate,int _iBarIndex,char *_lpstrText,DWORD _dwTextColor,BOOL _bIsLeft)
{
//	_iFillRate	=	3;
	int	iBarWidth	=	(_iWidth+5)*_iFillRate/100;
	int	iFullWidth	=	_iWidth+5;
	int	iXScale		=	100,iYSCale	=	100;

	g_sprNameBar.putClipedImage	(_iX,_iY,eNBI_BLACK_BODY,_iWidth,iXScale,iYSCale);

	if	(_bIsLeft)
	{
		g_sprNameBar.Put			(_iX+_iWidth,_iY,eNBI_BLACK_RIGHT,iXScale,iYSCale);		//5
		g_sprNameBar.putClipedImage	(_iX,_iY,_iBarIndex+2,min(iBarWidth,_iWidth),iXScale,iYSCale);
		g_sprNameBar.putClipedImage	(_iX+_iWidth,_iY,_iBarIndex+1,iBarWidth-(_iWidth+5)+5,iXScale,iYSCale);
	}
	else
	{
		g_sprNameBar.Put			(_iX,_iY,eNBI_BLACK_LEFT,iXScale,iYSCale);		//5
		g_sprNameBar.putClipedImage	(_iX,_iY,_iBarIndex  ,iBarWidth,iXScale,iYSCale);		//5
		g_sprNameBar.putClipedImage	(_iX,_iY,_iBarIndex+2,iBarWidth-5,iXScale,iYSCale);
	}

	s_bText.pushShadowStatus(FALSE);

	s_bText.cPut(_iX+2+1+_iWidth/2,_iY+2-1,0,_lpstrText);
	s_bText.cPut(_iX+2+1+_iWidth/2,_iY+2+1,0,_lpstrText);
	s_bText.cPut(_iX+2-1+_iWidth/2,_iY+2-1,0,_lpstrText);
	s_bText.cPut(_iX+2-1+_iWidth/2,_iY+2+1,0,_lpstrText);
	s_bText.cPut(_iX+2+_iWidth/2,_iY+2,WHITE,_lpstrText);

	s_bText.popShadowStatus();
}

//
//	멤버 이름 출력
void
CGamePlay::DrawMemberNameAtField(int _iX , int _iY,int _iMemberIndex)
{
	CPartyMemberInfo	*lpMember	=	s_partyInfo.getMember(_iMemberIndex);

	if	(!lpMember)
		return;

	CActor	*lpActor	=	g_am.getTestedActor(lpMember->m_wSerial);

	if	(lpActor && lpActor->m_wSyncStatusWithServer == eSSWS_COMPLETE)
		lpMember->m_wHP	=	lpActor->m_iHP*100/lpActor->m_iMaxHP;

	int		iNameWidth		=	dNAME_WIDTH;//max(s_sText.getPixelSize(lpMember->m_strName)+5,dMINIMUM_MEMBER_NAME_BAR_SIZE_AT_FIELD);
	int		iX				=	_iX-iNameWidth;
	int		iTextColor		=	WHITE;

	if	(_iMemberIndex	==	s_partyInfo.m_wFocusMember)
		iTextColor		=	LTGREEN;

	DrawActorNameBarAtField(iX,_iY,lpMember->m_strName,lpMember->m_wHP,lpMember->m_wSerial,lpMember->m_wIsDisconnected,iTextColor,-1);

	lpMember->m_wNameBarSizeAtField	=	iNameWidth+5;
}

//
//	멤버 이름 출력
void
CGamePlay::DrawPetNameAtField(int _iY,int _iPetIndex)
{
	CActor	*lpActor		=	g_hero.getPetActor(_iPetIndex);
	cPet	*lpPet			=	g_hero.getPet(_iPetIndex);

	int		iNameWidth		=	max(s_sText.getPixelSize(lpActor->m_strName)+5,dMINIMUM_MEMBER_NAME_BAR_SIZE_AT_FIELD);
	int		iBarWidth		=	(iNameWidth+5)*lpActor->m_iHP/max(lpActor->m_iMaxHP,1);
	int		iNameBarIndex	=	eNBI_GREEN_LEFT;
	int		iX				=	g_iScreenWidth-iNameWidth;

	g_sprNameBar.Put			(iX,_iY,0				,100,100,dPUT_HALF_BLENDING);		//5
	g_sprNameBar.putClipedImage	(iX,_iY,2				,iNameWidth,100,100,dPUT_HALF_BLENDING);

	g_sprNameBar.putClipedImage	(iX,_iY,iNameBarIndex  ,iBarWidth-5);		//5
	g_sprNameBar.putClipedImage	(iX,_iY,iNameBarIndex+2,min(iBarWidth-5,iNameWidth));

	if (_iPetIndex	==	s_partyInfo.m_wFocusPet)
			s_sText.putRF(g_iScreenWidth-2,_iY+3,LTGREEN,lpActor->m_strName);
	else	s_sText.putRF(g_iScreenWidth-2,_iY+3,WHITE,lpActor->m_strName);

	lpPet->m_wNameBarSizeAtField	=	iNameWidth+5;
}

//
//	멤버 이름 출력
void
CGamePlay::DrawSummonBeastNameAtField(int _iY,int _iPetIndex)
{
	CActor			*lpActor		=	g_hero.getSummonBeastActor(_iPetIndex);
	cSummonBeast	*lpSummonBeast	=	g_hero.getSummonBeast(_iPetIndex);

	int		iNameWidth		=	max(s_sText.getPixelSize(lpActor->m_strName)+5,dMINIMUM_MEMBER_NAME_BAR_SIZE_AT_FIELD);
	int		iBarWidth		=	(iNameWidth+5)*lpActor->m_iHP/max(lpActor->m_iMaxHP,1);
	int		iNameBarIndex	=	eNBI_BLUE_LEFT;
	int		iX				=	g_iScreenWidth-iNameWidth;

	g_sprNameBar.Put			(iX,_iY,0				,100,100,dPUT_HALF_BLENDING);		//5
	g_sprNameBar.putClipedImage	(iX,_iY,2				,iNameWidth,100,100,dPUT_HALF_BLENDING);

	g_sprNameBar.putClipedImage	(iX,_iY,iNameBarIndex  ,iBarWidth-5);		//5
	g_sprNameBar.putClipedImage	(iX,_iY,iNameBarIndex+2,min(iBarWidth-5,iNameWidth));

	if	(_iPetIndex	==	s_partyInfo.m_wFocusSummonBeast)
		s_sText.putRF(g_iScreenWidth-2,_iY+3,LTGREEN,lpActor->m_strName);
	else
		s_sText.putRF(g_iScreenWidth-2,_iY+3,WHITE,lpActor->m_strName);

	lpSummonBeast->m_wNameBarSizeAtField	=	iNameWidth+5;
}

//
//	플드에 파티원 정보 그려주기
void
CGamePlay::DrawPartyMemberNameAtField()
{
//	int j ;

	if	(g_bIsObserverModeForDuel)
	{
		int		iBarHeight		=	g_sprNameBar.getSpriteHeight(0);
		int		iBarWidth		=	120;
		int		aiBarShape[2]	=	{eNBI_BLUE_LEFT,eNBI_RED_LEFT};
		int		aiPos[2]		=	{0,0};
		BOOL	abPos[2]		=	{TRUE,FALSE};

		aiPos[1]		=	g_iScreenWidth-iBarWidth;

		for (int iTeam=0;iTeam<2;iTeam++)
		{
			int	iMemberCount	=	0;
			int	iX				=	iTeam*g_iScreenWidth;

			for (int i=0;i<g_am.m_iActorCount;i++)
			{
				CActor	*lpActor	=	&g_aActor[g_am.m_aActorList[i]];

				if(lpActor->isPlayer())
				{
					if	(iTeam	!=	lpActor->m_wTeam)
						continue;
					if	(lpActor->m_iMaxHP	==	0 || lpActor->m_wOperatorLevel	>=	4)
						continue;
					
					int	iY		=	g_iScreenHeight-iMemberCount*36-16;
					int	iHPRate	=	lpActor->m_iHP*100/lpActor->m_iMaxHP;
					
					DrawDuelActorNameBar(aiPos[iTeam],iY,iBarWidth,iHPRate,aiBarShape[iTeam],_ms("%d/%d",lpActor->m_iHP/100,lpActor->m_iMaxHP/100),WHITE,abPos[iTeam]);
					
					iY	-=	14;
					
					int		iJob			=	lpActor->m_wJob;
					int		iFPS			=	g_aJobIcon[iJob].GetFPS(0);
					int		iFrameCount		=	g_aJobIcon[iJob].GetFrameCount(0);
					
					s_bText.pushShadowStatus(FALSE);
					
					if	(iTeam	==	0)
					{
						s_bText.put(aiPos[iTeam]+2+30+1,iY+1,0,lpActor->m_strName);
						s_bText.put(aiPos[iTeam]+2+30+1,iY-1,0,lpActor->m_strName);
						s_bText.put(aiPos[iTeam]+2+30-1,iY+1,0,lpActor->m_strName);
						s_bText.put(aiPos[iTeam]+2+30-1,iY-1,0,lpActor->m_strName);
						
						s_bText.put(aiPos[iTeam]+2+30,iY,WHITE,lpActor->m_strName);
						
						g_aJobIcon[iJob].PutReg(aiPos[iTeam]+1,iY-2,0,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount);
					}
					else
					{
						s_bText.putRF(g_iScreenWidth-32+1,iY+1,0,lpActor->m_strName);
						s_bText.putRF(g_iScreenWidth-32+1,iY-1,0,lpActor->m_strName);
						s_bText.putRF(g_iScreenWidth-32-1,iY+1,0,lpActor->m_strName);
						s_bText.putRF(g_iScreenWidth-32-1,iY-1,0,lpActor->m_strName);
						
						s_bText.putRF(g_iScreenWidth-32,iY,WHITE,lpActor->m_strName);
						
						g_aJobIcon[iJob].PutReg(g_iScreenWidth-30,iY-2,0,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount);
					}
					
					s_bText.popShadowStatus();
				}
				else
				{
					if	(iTeam	!=	lpActor->m_wTeam)
						continue;
					if	(lpActor->m_iMaxHP	==	0 || lpActor->m_wOperatorLevel	>=	4)
						continue;
					
					int	iY		=	g_iScreenHeight-iMemberCount*36-16;
					int	iHPRate	=	lpActor->m_iHP*100/lpActor->m_iMaxHP;

					int PetHpMoveX	=	15;

					if(iTeam == 0)
						PetHpMoveX	=	-15;
					
					DrawDuelActorNameBar(aiPos[iTeam] + PetHpMoveX,iY,iBarWidth,iHPRate,aiBarShape[iTeam],_ms("%d/%d",lpActor->m_iHP/100,lpActor->m_iMaxHP/100),WHITE,abPos[iTeam]);
					
					iY	-=	14;
					
					s_bText.pushShadowStatus(FALSE);
					
					if	(iTeam	==	0)
					{
						s_bText.put(aiPos[iTeam]+2+30+1,iY+1,0,lpActor->m_strName);
						s_bText.put(aiPos[iTeam]+2+30+1,iY-1,0,lpActor->m_strName);
						s_bText.put(aiPos[iTeam]+2+30-1,iY+1,0,lpActor->m_strName);
						s_bText.put(aiPos[iTeam]+2+30-1,iY-1,0,lpActor->m_strName);
						
						s_bText.put(aiPos[iTeam]+2+30,iY,WHITE,lpActor->m_strName);
					}
					else
					{
						s_bText.putRF(g_iScreenWidth-32+1,iY+1,0,lpActor->m_strName);
						s_bText.putRF(g_iScreenWidth-32+1,iY-1,0,lpActor->m_strName);
						s_bText.putRF(g_iScreenWidth-32-1,iY+1,0,lpActor->m_strName);
						s_bText.putRF(g_iScreenWidth-32-1,iY-1,0,lpActor->m_strName);
						
						s_bText.putRF(g_iScreenWidth-32,iY,WHITE,lpActor->m_strName);
					}
					
					s_bText.popShadowStatus();
				}


				iMemberCount++;
			}
		}

		return;
	}	//여기 까지 듀얼 옵져버 모드용.


	int		iBarHeight				=	g_sprNameBar.getSpriteHeight(0),i;
	int		iY						=	dMEMBER_NAME_BAR_POS_AT_FIELD	-	(max(s_partyInfo.m_wMemberCount-1,0)+g_hero.getPetCount()+g_hero.getSummonBeastCount())*iBarHeight + g_iCorrectBI_Y;
	BOOL	bIsIncorrectPartyInfo	=	TRUE;

	if (s_iPopupInterface	)
		return;

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		CActor	*lpPet	=	g_hero.getPetActor(i);
		int		iPutPetBarY	= iY;// + iBarHeight;

		if (!lpPet)
			continue;

// 		if(s_unionPartyInfo.m_bIsActiveUnionParty && g_iSeasonVariable >= 3)
// 		{
// 			iPutPetBarY	+= iBarHeight;
// 
// 			for (j = 0 ; j < dPARTY_MAXPLAYER ; j++)
// 			{
// 				CUnionPartyMemberInfo*			CUnionPartyMemberInfo	=	s_unionPartyInfo.getUnionPartyInfo(0 , j);
// 
// 				if(CUnionPartyMemberInfo->m_wUnionPartySerial == 0xffff)
// 					continue;
// 
// 				iPutPetBarY	-=	iBarHeight;
// 			}
// 			
// 		}

		DrawPetNameAtField(iPutPetBarY,i);

		iY	+=	iBarHeight;
	}

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		CActor	*lpPet	=	g_hero.getSummonBeastActor(i);
		int		iPutPetBarY	= iY;// + iBarHeight;

		if (!lpPet)
			continue;

// 		if(s_unionPartyInfo.m_bIsActiveUnionParty && g_iSeasonVariable >= 3)
// 		{
// 			iPutPetBarY	+= iBarHeight;
// 
// 			for (j = 0 ; j < dPARTY_MAXPLAYER ; j++)
// 			{
// 				CUnionPartyMemberInfo*			CUnionPartyMemberInfo	=	s_unionPartyInfo.getUnionPartyInfo(0 , j);
// 				
// 				if(CUnionPartyMemberInfo->m_wUnionPartySerial == 0xffff)
// 					continue;
// 				
// 				iPutPetBarY	-=	iBarHeight;
// 			}
// 			
// 		}

		DrawSummonBeastNameAtField(iPutPetBarY,i);

		iY	+=	iBarHeight;
	}
	
	if (s_partyInfo.m_wMemberCount	<=	0	)
		return;

	if(s_unionPartyInfo.m_bIsActiveUnionParty && g_iSeasonVariable >= dSEASON_VARIABLE_PARTY_UNION)
	{
// 		for (i=0;i<dUNION_PARTY_COUNT;i++)
// 		{
// 			for (j = 0 ; j < dPARTY_MAXPLAYER ; j++)
// 			{
// 				CUnionPartyMemberInfo*			CUnionPartyMemberInfo	=	s_unionPartyInfo.getUnionPartyInfo(i , j);
// 					
// 				if(CUnionPartyMemberInfo->m_wUnionPartySerial == 0xffff)
// 					continue;
// 
// 				int		iX			=	g_iScreenWidth - (dNAME_WIDTH + 4 + 2 * i) * (i + 1);
// 				int		TempiY		=	iY- iBarHeight * (j);
// 				DWORD	dwTextColor	=	WHITE;
// 
// 				if(s_unionPartyInfo.m_wFocusParty == i && s_unionPartyInfo.m_wFocusPartyMember == j)
// 					dwTextColor = LTGREEN;
// 
// 				DrawActorNameBarAtFieldForUnionParty(iX , TempiY, CUnionPartyMemberInfo->m_strName , CUnionPartyMemberInfo->m_wHP , 
// 					CUnionPartyMemberInfo->m_dwState & dUNION_PARTY_MEMBER_STATE_OTHERFILED , 
// 					CUnionPartyMemberInfo->m_dwState & dUNION_PARTY_MEMBER_STATE_OTHERFILED , dwTextColor);
// 			}
// 		}
	}
	else
	{
		for (i=0;i<s_partyInfo.m_wMemberCount;i++)
		{
			CPartyMemberInfo	*lpMember	=	s_partyInfo.getMember(i);
				
			if (lpMember->m_wSerial	==	g_lpHero->m_wSerial)
			{

				bIsIncorrectPartyInfo=	FALSE;
				continue;
			}
			int		iX		=	g_iScreenWidth;
				
			DrawMemberNameAtField(iX , iY,i);
				
		iY	+=	iBarHeight;
		}
	}
}

//
//	필드에 멤버이름 바를 업데이트
void
CGamePlay::UpdateMemberNameBarAtField()
{
	int		iBarHeight	=	g_sprNameBar.getSpriteHeight(0),i;
	int		iY			=	dMEMBER_NAME_BAR_POS_AT_FIELD	-	(max(s_partyInfo.m_wMemberCount-1,0)+g_hero.getPetCount()+g_hero.getSummonBeastCount())*iBarHeight+ g_iCorrectBI_Y;
	cRECT	rectBar;


	if (s_partyInfo.m_wMemberCount)
	{
		for (i=0;i<s_partyInfo.m_wMemberCount;i++)
		{
			CPartyMemberInfo	*lpMember	=	s_partyInfo.getMember(i);

			if (lpMember->m_wSerial	==	g_lpHero->m_wSerial	)
				continue;
			if (lpMember->m_wSerial	==	0xffff				)
				continue;

			g_aActor[lpMember->m_wSerial].m_wLevel	=	lpMember->m_wLevel;
		}
	}

	s_partyInfo.m_wFocusMember		=	0xffff;
	s_partyInfo.m_wFocusPet			=	0xffff;
	s_partyInfo.m_wFocusSummonBeast	=	0xffff;

	s_unionPartyInfo.m_wFocusParty			=	0xffff;
	s_unionPartyInfo.m_wFocusPartyMember	=	0xffff;

	if (s_iPopupInterface					)
		return;

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		cPet	*lpPet	=	g_hero.getPet(i);

		if (!lpPet)
			continue;

		iY	+=	iBarHeight;

		if (lpPet->m_wNameBarSizeAtField	==	0)
			continue;

		rectBar.set(g_iScreenWidth-lpPet->m_wNameBarSizeAtField,iY-iBarHeight,g_iScreenWidth,iY);

		if(s_unionPartyInfo.m_bIsActiveUnionParty)
			rectBar.set(g_iScreenWidth-lpPet->m_wNameBarSizeAtField,iY-iBarHeight-iBarHeight,g_iScreenWidth,iY-iBarHeight);

		if (rectBar.isIn(s_posMouse.x,s_posMouse.y))
			s_partyInfo.m_wFocusPet	=	i;
	}

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		cSummonBeast	*lpSummonBeast	=	g_hero.getSummonBeast(i);

		if (!lpSummonBeast)
			continue;

		iY	+=	iBarHeight;

		if (lpSummonBeast->m_wNameBarSizeAtField	==	0)
			continue;

		rectBar.set(g_iScreenWidth-lpSummonBeast->m_wNameBarSizeAtField,iY-iBarHeight,g_iScreenWidth,iY);

		if(s_unionPartyInfo.m_bIsActiveUnionParty)
			rectBar.set(g_iScreenWidth-lpSummonBeast->m_wNameBarSizeAtField,iY-iBarHeight - iBarHeight,g_iScreenWidth,iY - iBarHeight);

		if (rectBar.isIn(s_posMouse.x,s_posMouse.y))
			s_partyInfo.m_wFocusSummonBeast	=	i;
	}

	int		iBarHeightTemp	=	492;

	//연합 파티는 다르게 처리한다.
	if(s_unionPartyInfo.m_bIsActiveUnionParty)
	{
		if (s_unionPartyInfo.m_wUnionPartyCount		<=	0)
			return;

		for (i=0;i<s_unionPartyInfo.m_wUnionPartyCount;i++)
		{
			int iX = g_iScreenWidth - (dNAME_WIDTH_PLUS_LR * i);
			iY = iBarHeightTemp;

			for (int j = 0 ; j < dPARTY_MAXPLAYER ; j++)
			{	
				if(s_unionPartyInfo.m_wUnionPartyInfo[i][j].m_wUnionPartySerial == 0xffff)
					break;

				rectBar.set(iX - dNAME_WIDTH_PLUS_LR,iY - iBarHeight,iX,iY);
						
				if (rectBar.isIn(s_posMouse.x,s_posMouse.y))
				{
					s_unionPartyInfo.m_wFocusParty			=	i;
					s_unionPartyInfo.m_wFocusPartyMember	=	j;
				}

				iY -= iBarHeight;

			}
		}
	}
	else				//연합 파티가 아닌 일반파티 일때 
	{
		if (s_partyInfo.m_wMemberCount	<=	0	)
			return;
		
		for (i=0;i<s_partyInfo.m_wMemberCount;i++)
		{
			CPartyMemberInfo	*lpMember	=	s_partyInfo.getMember(i);
			
			if (lpMember->m_wSerial				==	g_lpHero->m_wSerial	)
				continue;
			
			iY	+=	iBarHeight;
			
			if (lpMember->m_wNameBarSizeAtField	==	0					)
				continue;
			
			rectBar.set(g_iScreenWidth-lpMember->m_wNameBarSizeAtField,iY-iBarHeight,g_iScreenWidth,iY);
			
			if (rectBar.isIn(s_posMouse.x,s_posMouse.y))
				s_partyInfo.m_wFocusMember	=	i;
		}

	}
}