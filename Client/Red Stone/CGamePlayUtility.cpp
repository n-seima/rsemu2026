#include "CGamePlay.H"
#include "CSound.H"
#include "CMessage.H"
#include "cMap.H"

char		CGamePlay::s_strCurrentPlayBgm[256];
int			CGamePlay::s_iPlayBgmIndex	=	0;
int			CGamePlay::s_iAskItemPackNameCount		=	0;
WORD		CGamePlay::s_awAskItemPackNameList[32];
DWORD		CGamePlay::s_dwLastItemPackNameAskTime	=	0;
DWORD		CGamePlay::s_dwResultPopupwindow		=	0;

WORD		CGamePlay::s_wAskArcaInfoCount			=	0;
WORD		CGamePlay::s_awAskArcaInfoList[32];
DWORD		CGamePlay::s_dwLastArcaInfoAskTime		=	0;

void
CGamePlay::ChangeCursorShape(HCURSOR _hCursor)
{
	if (s_hCurrentCursor!=	_hCursor)
		SetCursor(_hCursor);

	s_hCurrentCursor	=	_hCursor;
}

char*
CGamePlay::GetMoneyString(LONGLONG _llMoney)
{
	static	char	s_strGold[40];
	BOOL	bIsAsia	=	FALSE;

#ifdef	_FOR_KOREA
	bIsAsia	=	TRUE;
#elif	_FOR_JAPAN
	bIsAsia	=	TRUE;
#elif	_FOR_CHINA
	bIsAsia	=	TRUE;
#endif

	if (bIsAsia)
	{
		int	iUk	=	_llMoney/100000000,iMan=(_llMoney/10000)%10000,iGold=_llMoney%10000;

		if	(iUk && iMan && iGold)
		{
			sprintf(s_strGold,"%d%s %d%s %d",iUk,dMSG_HUNDREAD_MILLION,iMan,dMSG_TEN_THOUND,iGold);
		}
		else
		if	(iUk && iMan)
		{
			sprintf(s_strGold,"%d%s %d%s",iUk,dMSG_HUNDREAD_MILLION,iMan,dMSG_TEN_THOUND);
		}
		else
		if	(iUk && iGold)
		{
			sprintf(s_strGold,"%d%s %d",iUk,dMSG_HUNDREAD_MILLION,iGold);
		}
		else
		if	(iUk)
		{
			sprintf(s_strGold,"%d%s",iUk,dMSG_HUNDREAD_MILLION);
		}
		else
		if	(iMan && iGold)
		{
			sprintf(s_strGold,"%d%s %d",iMan,dMSG_TEN_THOUND,iGold);
		}
		else
		if	(iMan)
		{
			sprintf(s_strGold,"%d%s",iMan,dMSG_TEN_THOUND);
		}
		else
		{
			sprintf(s_strGold,"%d",iGold);
		}
	}
	else
	{
		int	iD3	=	_llMoney/1000000000,iUk	=	(_llMoney/1000000)%1000,iMan=(_llMoney/1000)%1000,iGold=_llMoney%1000;

		if	(iD3)
		{
			sprintf(s_strGold,"%d,%.3d,%.3d,%.3d",iD3,iUk,iMan,iGold);
		}
		else
		if	(iUk)
		{
			sprintf(s_strGold,"%d,%.3d,%.3d",iUk,iMan,iGold);
		}
		else
		if	(iMan)
		{
			sprintf(s_strGold,"%d,%.3d",iMan,iGold);
		}
		else
			sprintf(s_strGold,"%d",iGold);
	}

	return	s_strGold;
}

//
//	BGM 연주
BOOL
CGamePlay::PlayBgm(char *_lpstrFileName)
{
	if	(_lpstrFileName)
	{
		sprintf(s_strCurrentPlayBgm,"data/bgm/%s",_lpstrFileName);

		if	(!IsFile(s_strCurrentPlayBgm))
			return	FALSE;
	}

	if	(!g_config.m_bIsMusicOn || s_strCurrentPlayBgm[0] == NULL)
	{
		s_oggPlayer.stop();

		return TRUE;
	}

	s_oggPlayer.play(s_strCurrentPlayBgm);
	s_oggPlayer.setVolume(g_config.m_iMusicVolume*10);

	return TRUE;
}

//
//	BGM 연주
void
CGamePlay::PlayBgm(int _iIndex,BOOL _bIsMapSerial)
{
	int	iBgmCount	=	0;

	for (int i=0;;i++)
	{
		if (STRICMP(g_strBgmFile[i],"end")==0)
			break;

		iBgmCount++;
	}

	if	(_bIsMapSerial)
		_iIndex	=	g_esm.m_awBgm[_iIndex]-1;
	
	if	(_iIndex < 0 || _iIndex >= iBgmCount	||	_bIsMapSerial	==	FALSE)
	{
		_iIndex	=	g_map.m_aBgmList[0];

		if	(_iIndex	<	9)
		{
			int	aiReplaceBgm[]	=	{2,6,0,4,3,5,7,1,8};

			_iIndex		=	aiReplaceBgm[_iIndex];
		}
	}

	if	(_iIndex < 0 || _iIndex >= iBgmCount)
		return;

	PlayBgm(g_strBgmFile[_iIndex]);
}
//
//	초기화
void
CFieldTitle::init(char *_lpstrTitle, int _iY)		// 09.09.02
{
	if (m_iState	!=	eFIELD_TITLE_SLEEP)
		return;

	m_iState		=	eFIELD_TITLE_APPEARENCE;
	m_iPutPosY		= _iY;		
	strcpy(m_strTitle,_lpstrTitle);
	
	m_avPosX.init(g_iScreenWidth,10,20,30,eACCEL_ARRIVE,100);
	m_avPosX.setBeginValueForAccelSpeed(500);
	m_iBarAlphaValue	=	0;
}

//
//	그리기
void
CFieldTitle::draw()		// 09.09.02
{
	if (m_iState	==	eFIELD_TITLE_SLEEP)
		return;

	s_textLarge32.setShadowMargin(4);

	int	iHeight		=	s_textLarge32.getFontHeight() + 5;

	int	iAlhpaValue	=	min(m_iBarAlphaValue/4,16);
	iAlhpaValue		=	max(iAlhpaValue,0);

	if (m_iState	!=	eFIELD_TITLE_STAY)
		cDRAW::FillAB(0,0,m_iPutPosY,g_iScreenWidth,m_iPutPosY + 10+iHeight,iAlhpaValue);
	else
		cDRAW::FillHB(0,0,m_iPutPosY,g_iScreenWidth,m_iPutPosY + 10+iHeight);

	s_textLarge32.put(m_avPosX.getValue(),m_iPutPosY + 10,WHITE,m_strTitle);
}

//
//	업데이트
void
CFieldTitle::update()
{
	if (m_iState==eFIELD_TITLE_SLEEP)
		return;

	switch(m_iState)
	{
		case	eFIELD_TITLE_APPEARENCE	:
			m_iBarAlphaValue++;
			if(m_avPosX.update())
			{
				m_iState		=	eFIELD_TITLE_STAY;
				m_iStayTime		=	dSYNC_FPS*2;
				m_iBarAlphaValue=	16*4;
			}
			break;

		case	eFIELD_TITLE_STAY		:
			m_iStayTime--;
			if (m_iStayTime	==	0)
			{
				m_iState			=	eFIELD_TITLE_DISAPPEAR;
				m_avPosX.init(10,-g_iScreenWidth,dSYNC_FPS/2,40,eACCEL_START,100);
			}
			break;

		case	eFIELD_TITLE_DISAPPEAR	:
			m_iBarAlphaValue-=	2;
			if(m_avPosX.update())
				m_iState	=	eFIELD_TITLE_SLEEP;
			break;

		case	eFIELD_TITLE_SLEEP		:
			break;
	}
}

void
CGamePlay::AddAskItemPackName(int _iIndex)
{
	if (s_iAskItemPackNameCount	>=	32)
		return;

	for (int i=0;i<s_iAskItemPackNameCount;i++)
		if (s_awAskItemPackNameList[i]	==	_iIndex)
			return;

	s_awAskItemPackNameList[s_iAskItemPackNameCount++]	=	_iIndex;
	_iIndex	=	_iIndex;
}

void
CGamePlay::SendAskItemPackName()
{
	if (s_iAskItemPackNameCount	==	0)
		return;

	if (s_dwLastItemPackNameAskTime+1000	>	timeGetTime())
		return;

	s_dwLastItemPackNameAskTime	=	timeGetTime();

	s_agent.sendAskItemPackName(s_iAskItemPackNameCount,s_awAskItemPackNameList);

	s_iAskItemPackNameCount		=	0;
}

void
CGamePlay::AddAskArcaInfo(int _iIndex)
{
	if (s_wAskArcaInfoCount	>=	32)
		return;

	for (int i=0;i<s_wAskArcaInfoCount;i++)
		if (s_awAskArcaInfoList[i]	==	_iIndex)
			return;

	s_awAskArcaInfoList[s_wAskArcaInfoCount++]	=	_iIndex;
	_iIndex	=	_iIndex;
}

void
CGamePlay::SendAskArcaInfo()
{
	if (s_wAskArcaInfoCount	==	0)
		return;

	if (s_dwLastArcaInfoAskTime+1000	>	timeGetTime())
		return;

	s_dwLastArcaInfoAskTime	=	timeGetTime();

	s_agent.sendAskArcaInfo(s_wAskArcaInfoCount,s_awAskArcaInfoList);

	s_wAskArcaInfoCount		=	0;
}