#include "CPartyWindow.H"
#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"
#include "partyDefine.H"

#define	dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH			240
#define	dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT	16

BOOL
CPartyWindow::isInRequestJoinPartyUserList()
{
	int	iCount	=	s_partyInfo.m_requestJoinPartyUserList.m_iUserCount;
	
	if	(iCount	<=	0)
		return	FALSE;

	cRECT	rect;

	int	iX	=	m_posPartyWindow.x-dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH;
	int	iY	=	m_posPartyWindow.y+5;

	rect.set(iX,iY,iX+dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH-5,iY+(iCount+1)*dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT-5);

	if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		return	TRUE;

	return	FALSE;
}


void
CPartyWindow::updateRequestJoinPartyUserList()
{
	s_partyInfo.m_requestJoinPartyUserList.update();
	s_partyInfo.resetNewRequestJoinPartyUserFlag();
	s_partyInfo.m_requestJoinPartyUserList.m_iFocusUser	=	0xffff;

	int	iCount	=	s_partyInfo.m_requestJoinPartyUserList.m_iUserCount;

	if	(iCount	<=	0	||	!g_lpHero	||	g_lpHero->isPartyLeader()	==	FALSE)
	{
		s_partyInfo.m_requestJoinPartyUserList.reset();
		return;
	}

	if	(s_partyInfo.m_bf1IsOpenParty	==	FALSE)
		return;

	cRECT	rect;
		
	int	iX	=	m_posPartyWindow.x-dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH;
	int	iY	=	m_posPartyWindow.y+5;

	rect.set(iX,iY,iX+dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH-5,iY+(iCount+1)*dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT-5);

	if	(rect.isIn(s_posMouse.x,s_posMouse.y)	==	FALSE)
		return;

	iY	+=	dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT;

	for	(int i=iCount;i;)
	{
		--i;

		CPatyInfo::cRequestJoinPartyUserInfo*	lpInfo	=	&s_partyInfo.m_requestJoinPartyUserList.m_aList[i];

		rect.y1	=	iY;
		rect.y2	=	iY+dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT;

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_partyInfo.m_requestJoinPartyUserList.m_iFocusUser	=	i;

			break;
		}

		iY	+=	dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT;
	}

	if	(s_isClickedLeftButton	&&	s_partyInfo.m_requestJoinPartyUserList.m_iFocusUser	!=	0xffff)
	{
		CPatyInfo::cRequestJoinPartyUserInfo*	lpInfo	=	&s_partyInfo.m_requestJoinPartyUserList.m_aList[s_partyInfo.m_requestJoinPartyUserList.m_iFocusUser];

		char	strAccept[128],strRemove[128],strReset[128];

		sprintf(strAccept,dMSG_JPRUL_ACCEPT_JOIN_PARTY_REQUEST_FORM,lpInfo->m_strName);
		sprintf(strRemove,dMSG_JPRUL_REMOVE_USER_FROM_JOIN_PARTY_USER_LIST_FORM,lpInfo->m_strName);
		sprintf(strReset,dMSG_JPRUL_RESET_JOIN_PARTY_USER_LIST);

		s_pmCommon.popup(m_posPartyWindow.x,iY);

		s_pmCommon.addMenu(eJPRUL_ACCEPT_JOIN_PARTY_REQUEST,strAccept);
		s_pmCommon.addMenu(eJPRUL_REMOVE_USER_FROM_JOIN_PARTY_USER_LIST,strRemove);
		s_pmCommon.addMenu(eJPRUL_RESET_JOIN_PARTY_USER_LIST,strReset);

		strcpy(s_partyInfo.m_requestJoinPartyUserList.m_strWorkUser,lpInfo->m_strName);
	}

}

void
CPartyWindow::drawRequestJoinPartyUserList()
{
	int	iCount	=	s_partyInfo.m_requestJoinPartyUserList.m_iUserCount;

	if	(s_partyInfo.m_bf1IsOpenParty	==	FALSE)
		return;
	if	(iCount	<=	0)
		return;
	if	(!g_lpHero	||	g_lpHero->isPartyLeader()	==	FALSE)
		return;

	cRECT	rect;
		
	int	iX	=	m_posPartyWindow.x-dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH;
	int	iY	=	m_posPartyWindow.y+5;

	rect.set(iX,iY,iX+dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH-5,iY+(iCount+1)*dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT-5);

	s_ttCommon.drawBoxAddBorderSize(&rect,0);

	s_text.putRF(iX+dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH-5,iY,GOLDYELLOW,dMSG_REQUEST_JOIN_PARTY_USER_LIST);
	iY	+=	dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT;

	for	(int i=iCount;i;)
	{
		--i;

		CPatyInfo::cRequestJoinPartyUserInfo*	lpInfo	=	&s_partyInfo.m_requestJoinPartyUserList.m_aList[i];

		int		iSpentTime	=	(timeGetTime()-lpInfo->m_dwAskTime)/1000;
		iSpentTime			=	min(99*60+59,iSpentTime);

		char	strText[256];
		DWORD	dwTextColor	=	WHITE;

		wsprintf(strText,"%.2d:%.2d %s Lv%d %s",iSpentTime/60,iSpentTime%60,lpInfo->m_strName,lpInfo->m_wLevel,g_pstrHeroJobName[lpInfo->m_wJob]);

		if	(i	==	s_partyInfo.m_requestJoinPartyUserList.m_iFocusUser)
			dwTextColor	=	LTGREEN;

		s_text.putRF(iX+dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_WIDTH-5,iY,dwTextColor,strText);

		iY	+=	dREQUEST_JOIN_PARTY_USER_LIST_WINDOW_LIST_HEIGHT;
	}
}