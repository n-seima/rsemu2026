#include "CPartyWindow.H"
#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"
#include "partyDefine.H"

const	int	c_iPartyListBarHeight	=	75;

int l_aiJobIconIdex[] = 
{	// interface2.sd 파일.. 직업아이콘 인덱스.
eJOB_SMALL_ICON_KNIGHT,	
eJOB_SMALL_ICON_WARRIOR,
eJOB_SMALL_ICON_WIZARD,
eJOB_SMALL_ICON_WEREWOLF,
eJOB_SMALL_ICON_FALLEN_ANGEL,
eJOB_SMALL_ICON_ROGUE,
eJOB_SMALL_ICON_FIGHTER,
eJOB_SMALL_ICON_MAGIC_LANCER,
eJOB_SMALL_ICON_MAGIC_ARCHER,
eJOB_SMALL_ICON_BEAST_TAMER,
eJOB_SMALL_ICON_SUMMONER,
eJOB_SMALL_ICON_PRINCESS,
eJOB_SMALL_ICON_MAGICAL_GIRL,
eJOB_SMALL_ICON_NECROMANCER,
eJOB_SMALL_ICON_DEVIL,
eJOB_SMALL_ICON_KNIGHT,
eJOB_SMALL_ICON_SOUL_BRINGER,
eJOB_SMALL_ICON_CHAMPION	,
eJOB_SMALL_ICON_OPTICALIST,
};


void
CPartyWindow::receivePartyList(int _iPartyCount,cPartyInfoForPL *_lpList)
{
	s_partyInfo.receivePartyList(_iPartyCount,_lpList);
}

int
CPartyWindow::getFocusParty()
{
	if	(s_partyInfo.m_iPartyCount	<=	0)
		return	-1;

	int		iBarWidth	=	256;
	int		iX,iY;

	iX		=	m_posPartyWindow.x+9;
	iY		=	m_posPartyWindow.y+2;

	int		i,iCount=0;
	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iBarHeight	=	c_iPartyListBarHeight;

	CPos	pos;

	pos.Set(iX+4,iY);

	for (i=0;i<s_partyInfo.m_iPartyCount;i++)
	{
		cRECT	rectBar;

		rectBar.set(pos.x,pos.y,pos.x+iBarWidth,pos.y+iBarHeight);

		pos.y	+=	iBarHeight;

		if	(rectBar.isIn(s_posMouse.x,s_posMouse.y))
			return	i;
	}

	return	-1;
}

void
CPartyWindow::drawPartyList()
{
	int		iBarWidth	=	256;
	int		iX,iY;

	iX		=	m_posPartyWindow.x+9;
	iY		=	m_posPartyWindow.y+2;

	if	(s_partyInfo.m_iPartyCount	<=	0)
	{
		if	(s_partyInfo.m_iPartyCount	==	0)
		{
			s_text.put(iX+4,iY+8+6 ,iBarWidth-12,100,0,WHITE,dMSG_PARTY_LIST_COMMENT);
			if(m_iPartyBarmenuStatus	==	ePBMS_PARTY_UNION)
			{
				s_text.put(iX+4,iY+8+42 ,iBarWidth-12,100,0,WHITE,dMSG_PARTY_LIST_COMMENT2);
				s_text.put(iX+4,iY+8+74 ,iBarWidth-12,100,0,WHITE,dMSG_PARTY_LIST_COMMENT3);
			}
		}
		else
			s_text.put(iX+4,iY+8+6 ,iBarWidth-12,100,0,LTYELLOW,dMSG_NOT_EXIST_A_JOIN_ABLE_PARTY);

		return;
	}

	int		i,iCount=0;
	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iBarHeight	=	c_iPartyListBarHeight;

	CPos	pos;

	pos.Set(iX+4,iY-4);

	for (i=0;i<s_partyInfo.m_iPartyCount;i++)
	{
		cPartyInfoForPL	*lpInfo		=	&s_partyInfo.m_aPartyList[i];

		int	iMasterJob	=	lpInfo->m_aMemberList[0].m_bf8Job;
		int	iMasterLevel=	lpInfo->m_aMemberList[0].m_bf10Level;

		if	(i	==	s_partyInfo.m_iSelectParty)
			g_sprInterface.Put(pos.x-5,pos.y+9,ePWI_SELECT_PANNEL,100,180,dPUT_ALPHA_BLENDING,8);

		int		iAddY		=	0;
		char	strProspectus[256];
		char	*lpstrObject	=	s_partyInfo.getPartyObjectString(lpInfo->m_bf3PartyObject);

		sprintf(strProspectus,dMSG_PARTY_PROSPECTUS_FORM,lpstrObject,lpInfo->m_strPartyProspectus);

		int	iLineCount	=	s_text.getLineCount(iBarWidth-10,strProspectus);

		if	(iLineCount	<=	1)
			iAddY		=	6;

		g_sprInterface.Put(iX+8,pos.y+30+iAddY,eGBSW_1+i);

		s_text.put(pos.x+14,pos.y+iAddY+10,WHITE,dMSG_PARTY_LEADER_AND_MEMBER_COUNT_FORM,lpInfo->m_strMasterName,iMasterLevel,g_pstrHeroJobName[iMasterJob]);
		s_text.put(pos.x+14,pos.y+iAddY+24,WHITE,dMSG_PARTY_MEMBER_INFO_SUMMARY_FORM,lpInfo->m_bf3MemberCount,lpInfo->getAverageLevel());
		s_text.put(pos.x,pos.y+iAddY+38,WHITE,dMSG_PREFERENCE_JOB);

		int	iWidth	=	s_text.getPixelSize(dMSG_PREFERENCE_JOB);

		if	(lpInfo->m_dwPreferenceJobMask	==	0)
			s_text.put(pos.x+iWidth+6,pos.y+iAddY+38,LTORANGE,dMSG_NOTHING);
		else
		{
			int	iFavoriteJobCount	=	0;

			for	(int iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
			{
				int	iMask	=	1;

				iMask		<<=	iJob;

				if	(lpInfo->m_dwPreferenceJobMask & iMask)			//수정..
				{
					g_sprInterface2.Put(pos.x+iWidth+4+iFavoriteJobCount*22,pos.y+iAddY+37,l_aiJobIconIdex[iJob]);

					iFavoriteJobCount++;

					if	(iFavoriteJobCount	>=	8)
						break;
				}
			}
		}

		s_text.setLineGap(1);
		s_text.someCodeIgnoreOff();
		s_text.put(pos.x,pos.y+iAddY+54,iBarWidth-10,32,0,WHITE,strProspectus);
		s_text.someCodeIgnoreOn();
		s_text.setLineGap(2);

		if	(i	>=	5)
			continue;

		pos.y	+=	iBarHeight;

		g_sprInterface.Put(pos.x-6,pos.y+4,ePWI_BORDER);
	}

	if	(s_partyInfo.m_iFocusParty	!=	-1)
	{
		cPartyInfoForPL	*lpInfo	=	&s_partyInfo.m_aPartyList[s_partyInfo.m_iFocusParty];

		cRECT	rect;

		iX		=	m_posPartyWindow.x;
		iY		=	m_posPartyWindow.y+s_partyInfo.m_iFocusParty*iBarHeight+4;

		rect.set(iX,iY,iX+75,iY+26);

		char	strTT[1024]	=	"";

		lstrcat(strTT,dMSG_PL_PARTY_MEMBER_INFO);

		for	(int i=0;i<lpInfo->m_bf3MemberCount;i++)
			lstrcat(strTT,_ms("  Lv %3d %s\n",lpInfo->m_aMemberList[i].m_bf10Level,g_pstrHeroJobName[lpInfo->m_aMemberList[i].m_bf8Job]));

		s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strTT,eTFM_LEFT);
	}
}