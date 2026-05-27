#include "CGamePlay.H"
#include "CMessage.H"
#include "CEvent.H"
#include "cHero.H"
#include "treasureMapDefine.H"

int			CGamePlay::s_iNoticeTime	=	-1;
int			CGamePlay::s_iAlpha			=	-1;
int			CGamePlay::s_iLineGap		=	-1;

char		CGamePlay::s_strNotice[dMAX_NOTICE_MESSAGE_SIZE];
WORD		CGamePlay::s_wNoticeTimeForOutPut	=	0;
WORD		CGamePlay::s_wEffectflickeringly	=	0;


enum
{
	eNTBI_OPENER	=	85,
	eNTBI_BODY,
	eNTBI_CLOSER,

	eBOOST_EXP_INFO_BAR		=	88,
	eBOOST_GOLD_INFO_BAR	=	89,
};

//
//	공지 초기화
void
CGamePlay::InitNoticeBox()
{
	s_iNoticeTime	=	-1;
}

//
//	공지
void
CGamePlay::Notice(char *_lpstrMessage , WORD _wTime , WORD _wEffectflickeringly,int _iAlpha,int _iLineGap)
{
	s_iNoticeTime			=	timeGetTime();
	s_wNoticeTimeForOutPut	=	_wTime;
	s_wEffectflickeringly	=	_wEffectflickeringly;

	s_iAlpha				=	_iAlpha;
	s_iLineGap				=	_iLineGap;

	strcpy(s_strNotice,_lpstrMessage);
}

//
//	출력
void
CGamePlay::DrawNotice()
{
	int	iWidth	=	g_iScreenHeight;
	int	iY		=	2;

	if	(s_bIsGuildBattleField && g_hero.m_wOperatorLevel	>=	4)
		iY		=	62;

	if	(s_iNoticeTime	==	-1)
		return;

	if	(STRICMP(s_strNotice,"")	==	0)
	{
		s_iNoticeTime	=	-1;

		return;
	}

	if	(s_iNoticeTime+1000*s_wNoticeTimeForOutPut	<	timeGetTime()	||	s_iNoticeTime	>	timeGetTime())		//공지 시간 수정 가능하게 ...
	{
		s_iNoticeTime	=	-1;

		return;
	}

	if(s_wEffectflickeringly)				//깜빡이는 이펙트 넣을 건지.
		if	(s_iFrameCounter%(dSYNC_FPS*4)	<	dSYNC_FPS/4)
			return;

	int	iStackLineGap	=	-1;

	if	(s_iLineGap	!=	-1)
	{
		iStackLineGap	=	s_textBold14.getLineGap();
		s_textBold14.setLineGap(s_iLineGap);
	}

	int	iMinWidth	=	g_sprInterface2.getSpriteWidth(eNTBI_OPENER)+g_sprInterface2.getSpriteWidth(eNTBI_CLOSER);
	iWidth			=	min(s_textBold14.getMaxLineSize(s_strNotice),iWidth);
	iWidth			=	max(iWidth,iMinWidth);
	int	iImageWidth	=	iWidth-iMinWidth+40;

	int	iBoxWidth	=	min(s_textBold14.getPixelSize(s_strNotice)+10,iWidth);
	int	iLineCount	=	s_textBold14.getLineCount(iWidth+1,s_strNotice);
	int	iRate		=	100;
	int	iCorrectTextY=	8;

	{
		int	iBoxHeight	=	66;
		int	iSpare		=	8;
		int	iViewHeight	=	iBoxHeight-iSpare*2;

		int	iTextHeight	=	iLineCount*s_textBold14.getLineThick()-s_textBold14.getLineGap();
		iRate			=	max(iTextHeight*100/iViewHeight,50);

		iCorrectTextY	=	iSpare*iRate/100;

		iViewHeight		=	iViewHeight*iRate/100;
		if	(iViewHeight	>	iTextHeight)	
			iCorrectTextY	+=	(iViewHeight-iTextHeight)/2;

	}

	if	(s_iAlpha	!=	-1)
		g_sprInterface2.put3PieceBar(g_iScreenWidth/2-iImageWidth/2,iY,eNTBI_OPENER,iImageWidth,100,iRate,dPUT_ALPHA_BLENDING,s_iAlpha);
	else
		g_sprInterface2.put3PieceBar(g_iScreenWidth/2-iImageWidth/2,iY,eNTBI_OPENER,iImageWidth,100,iRate);

	s_textBold14.cPut((cDRAW::WIDTH-iWidth)/2,iY+iCorrectTextY,iWidth,200,0,WHITE,s_strNotice);

	if	(iStackLineGap	!=	-1)
		s_textBold14.setLineGap(iStackLineGap);
}
/*
//
//	출력
void
CGamePlay::DrawNotice()
{
	int	iWidth	=	600;

	if (s_iNoticeTime	==	-1)
		return;

	if	(STRICMP(s_strNotice,"")	==	0)
	{
		s_iNoticeTime	=	-1;

		return;
	}

	if	(s_iNoticeTime+1000*30	<	timeGetTime()	||	s_iNoticeTime	>	timeGetTime())
	{
		s_iNoticeTime	=	-1;

		return;
	}

	if	(s_iFrameCounter%(dSYNC_FPS*4)	<	dSYNC_FPS/4)
		return;

	s_textBold14.pushShadowStatus(FALSE);

//	int	iBoxHeight	=	s_textTemp.getTextHeight(iWidth,s_strNotice);
	int	iBoxWidth	=	min(s_textBold14.getPixelSize(s_strNotice)+10,iWidth);
	int	iBoxHeight	=	s_textBold14.getTextHeight(iWidth,s_strNotice);

	cRECT	rect;

	rect.set((cDRAW::WIDTH-iBoxWidth)/2,10,(cDRAW::WIDTH-iBoxWidth)/2+iBoxWidth,10+iBoxHeight);

	s_ttDialogFrame.drawBoxAddBorderSize(&rect,_WHITE,32);

	s_textBold14.cPut((cDRAW::WIDTH-iWidth)/2,10,iWidth,200,0,0,s_strNotice);

	s_textBold14.popShadowStatus();
}*/

void
CGamePlay::DrawBoostExpAndGoldEventInfo()
{
	int	iBoostGoldTime	=	g_iBoostGoldSecondByTreasureMapEventInServer+g_hero.m_wBoostGoldSecondByTreasureMapEvent;
	int	iBoostExpTime	=	g_iBoostExpSecondByTreasureMapEventInServer+g_hero.m_wBoostExpSecondByTreasureMapEvent;

	int	iX=2,iY=2;

	if	(iBoostExpTime)
	{
		g_sprInterface2.Put(iX,iY,eBOOST_EXP_INFO_BAR);
		s_bText.put(iX+44,iY+5,WHITE,"%d%% %.2d:%.2d",dBOOST_EXP_BY_TREASURE_MAP,iBoostExpTime/60,iBoostExpTime%60);

		iX	+=	g_sprInterface2.getSpriteWidth(eBOOST_EXP_INFO_BAR)+2;
	}

	if	(iBoostGoldTime)
	{
		g_sprInterface2.Put(iX,iY,eBOOST_GOLD_INFO_BAR);

		s_bText.put(iX+40,iY+5,WHITE,"%d%% %.2d:%.2d",dBOOST_GOLD_BY_TREASURE_MAP,iBoostGoldTime/60,iBoostGoldTime%60);
		iX	+=	g_sprInterface2.getSpriteWidth(eBOOST_GOLD_INFO_BAR)+2;
	}

	char	strText[128]	=	"";

	if	(g_hero.m_bonusExp.m_wTime)
		sprintf(strText,dMSG_INCREASE_EXP_FORM,g_hero.m_bonusExp.getCorrectedValue());

	if	(g_hero.m_bonusItemDropChance.m_wTime)
		sprintf(strText+strlen(strText),dMSG_INCREASE_ITEM_DROP_CHANCE_FORM,g_hero.m_bonusItemDropChance.getCorrectedValue());

	if	(strlen(strText))
	{
		iX	+=	6;
		int		iWidth	=	s_text.getPixelSize(strText);

		cRECT	rect;

		rect.set(iX,iY+6,iX+iWidth,iY+15);

		s_ttDialogFrame.drawBoxAddBorderSize(&rect,_WHITE,32);

		s_text.pushShadowStatus(FALSE);
		s_text.put(iX,iY+5,0,strText);
		s_text.popShadowStatus();

		iX		+=	iWidth+10;
	}
}