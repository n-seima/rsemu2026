#include "CGamePlay.H"
#include "cEvent.H"
#include "cHero.H"
#include "cGuild.H"
#include "CDebugWindow.H"

int		CGamePlay::s_iCountDownForDecreaseSystemMessageLine	=	0;

//
//	시스템 메시지 박스 초기화
void
CGamePlay::InitSystemMessageBox()
{
	cRECT	rect;
	
	rect.set(5,10  ,460,1024);
	s_tlSystem.init(&rect,&s_text,9,256);	//	초기화
	s_tlSystem.setLineGap(2);

	int	iCorrectBI_X	=	g_iScreenWidth-800;
	int	iCorrectBI_Y	=	g_iScreenHeight-600;

	s_inputChatting.init(s_hWnd,s_hInst,&s_text,1);
	s_inputChatting.set(79+14+26+iCorrectBI_X,492+6+iCorrectBI_Y-1,
						0,
						79+14+26+iCorrectBI_X,492+6+iCorrectBI_Y-1,
						574,13,
						dCHAT_LENGTH-dNAME_LENGTH-2,s_strChatMessage);
	s_inputChatting.setColor(0,WHITE);
}

//
//	시스템 메시지 추가
void
CGamePlay::OperateSystemMessageBox()	//	메시지 추가
{
	if (s_iCountDownForDecreaseSystemMessageLine	==	0)
	{
		s_iCountDownForDecreaseSystemMessageLine	=	dSYNC_FPS*3;
		s_tlSystem.decreaseLineCount();
	}

	s_iCountDownForDecreaseSystemMessageLine--;
}

//
//	시스템 메시지 추가
void
CGamePlay::AddSystemMessage(DWORD _dwColor,char *_strText,...)	//	메시지 추가
{
#ifdef	_DEBUG
	if	(g_bIsMovieMode)
		return;
#endif
	s_iCountDownForDecreaseSystemMessageLine	=	dSYNC_FPS*10;

	char	strText[512];

	va_list		argptr;
	va_start(argptr, _strText);
	vsprintf(strText,_strText,argptr);
	va_end(argptr);

	s_tlSystem.addColorText(_dwColor,strText,TRUE);
}

void
CGamePlay::DrawSystemMessageBox()
{
#ifdef	_DEBUG
	if	(g_bIsHideDamageAndHideName)
		return;
	if	(g_bIsMovieMode)
		return;
#endif

	if	(s_tlSystem.getLineCount() == 0)
		return;

	int		iBoostGoldTime	=	g_iBoostGoldSecondByTreasureMapEventInServer+g_hero.m_wBoostGoldSecondByTreasureMapEvent;
	int		iBoostExpTime	=	g_iBoostExpSecondByTreasureMapEventInServer+g_hero.m_wBoostExpSecondByTreasureMapEvent;
	int		iSize			=	s_tlSystem.getMaxLengthInViewText()+8;
	int		iDY				=	0;

	if	(iBoostGoldTime+iBoostExpTime+g_hero.m_bonusExp.m_wTime+g_hero.m_bonusItemDropChance.m_wTime	!=	0)
		iDY	=	20;

	if	(s_bIsGuildBattleField	&&	g_guildBattleInfo.m_bIsBeginBattle)
		iDY	=	32;

	if	(s_bIsGuildPointBattleField)
		iDY	=	20;

	if	(g_bIsObserverModeForDuel)
		iDY	=	0;

	s_tlSystem.setRefitPos(0,iDY);

	cRECT	rect;

	rect.set(s_tlSystem.getRect());

	rect.add(0,iDY);

	rect.y2	=	rect.y1+s_tlSystem.getHeight();
	rect.x2	=	rect.x1+iSize;

	_DEEPGRAY	=	_DEEPGRAY;

	if	(g_bIsObserverModeForDuel)
		s_ttCommon.drawBoxAddBorderSize(&rect,0,32);
	else
		s_ttCommon.drawBoxAddBorderSize(&rect,0,10);

	s_tlSystem.drawText();

	s_tlSystem.setRefitPos();
}