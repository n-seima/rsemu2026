#include "CPitchmanShop.h"
#include "CImageData.h"
#include "CMap.h"
#include "CMessage.h"
#include "CHero.h"
#include "CActor.h"

CPitchmanShop	g_pitchmanShop;

#define	dSIGNBOARD_WIDTH		175
#define	dSIGNBOARD_PIECE_COUNT	8
#define	dPITCHMAN_SHOP_SIZE		150

enum
{
	eSB_LEFT_POLL,
	eSB_RIGHT_POLL,
	eSB_POLL_IMAGE_COUNT,
};

void
CPitchmanShop::reset()
{
	m_wCount			=	0;
	m_wIsWaitWorkResult	=	FALSE;
	m_wFocusShop		=	0xffff;

	memset(&m_aShops,0xff,sizeof(m_aShops));
}

BOOL
CPitchmanShop::setShopInfo(CSimplePitchmanShopInfo *_lpInfo,char *_lpstrShopName)
{
	CPitchmanShopInfo	*lpShop	=		&m_aShops[_lpInfo->m_bf10Serial];

	lpShop->m_wSerial			=	_lpInfo->m_bf10Serial;
	lpShop->m_wSignboardShape	=	_lpInfo->m_bf6SignboardShape;
	lpShop->m_wTextColor		=	_lpInfo->m_wTextColor;

	lpShop->m_bf1IsBold			=	_lpInfo->m_bf1IsBold;
	lpShop->m_bf1IsShadowText	=	_lpInfo->m_bf1IsShadowText;	
	lpShop->m_bf2Status			=	_lpInfo->m_bf2Status;
	lpShop->m_bf12Owner			=	_lpInfo->m_bf12Owner;

	lpShop->m_wX				=	_lpInfo->m_wX;
	lpShop->m_wY				=	_lpInfo->m_wY;

	lpShop->m_wBoardLineCount	=	1;

	if (lpShop->m_bf2Status	==	ePSS_OPEN)
		strcpy(lpShop->m_strName,_lpstrShopName);
	else
		strcpy(lpShop->m_strName," ");

	CActor	*lpActor	=	g_am.getTestedActor(_lpInfo->m_bf12Owner);

	if (lpActor)
	{
		lpActor->m_wPitchmanShopSerial	=	_lpInfo->m_bf10Serial;
		lpActor->setDirect(dDIRECT_S);
	}

	return	TRUE;
}

BOOL
CPitchmanShop::addTemplate(int _iSerial,int _iX,int _iY)
{
	m_aShops[_iSerial].m_bf2Status	=	ePSS_REQUIRE_INFO;
	m_aShops[_iSerial].m_wSerial	=	_iSerial;
	m_aShops[_iSerial].m_wX			=	_iX;
	m_aShops[_iSerial].m_wY			=	_iY;

	return	TRUE;
}


BOOL
CPitchmanShop::remove(int _iSerial)
{
	m_aShops[_iSerial].m_wSerial	=	0xffff;

	CActor	*lpActor	=	g_am.getTestedActor(m_aShops[_iSerial].m_bf12Owner);

	if (lpActor)
		lpActor->m_wPitchmanShopSerial	=	0xffff;

	return	TRUE;
}

void
CPitchmanShop::addRequireInfoShop(int _iSerial)
{
	if (m_wRequireInfoShopCount	>=	dMAX_ASK_SIMPLE_PITCHMAN_SHOP_INFO_COUNT)
		return;

	for (int i=0;i<m_wRequireInfoShopCount;i++)
		if (m_awRequireInfoShopList[i]	==	_iSerial)	//	이미 리스트에 있다.
			return;

	if (m_aShops[_iSerial].m_bf2Status	==	ePSS_WAIT_INFO)	//	정보를 기다리는 중이다.
	{
		m_awRequireInfoShopListRequestCount[_iSerial]++;	//	서버에서 응답 기다리는중

		if (m_awRequireInfoShopListRequestCount[_iSerial] < dSYNC_FPS*5)	//	5초동안 기다렸다가 응답이 없음 다시 요청
			return;
	}

	m_awRequireInfoShopListRequestCount[_iSerial]		=	0;
	m_awRequireInfoShopList[m_wRequireInfoShopCount++]	=	_iSerial;
}

void
CPitchmanShop::sendStackedRequireInfoShopList()
{
	if (m_wRequireInfoShopCount	==	0)
		return;

	s_agent.sendAskSimplePitchmanShopInfoList(m_awRequireInfoShopList,m_wRequireInfoShopCount);

	for (int i=0;i<m_wRequireInfoShopCount;i++)
	{
		int	iSerial	=	m_awRequireInfoShopList[i];

		m_aShops[iSerial].m_bf2Status	=	ePSS_WAIT_INFO;
	}

	m_wRequireInfoShopCount	=	0;
}

void
CPitchmanShop::draw(int _iSerial)
{
	CPitchmanShopInfo	*lpShop	=	&m_aShops[_iSerial];

	if	(lpShop->m_wSerial	==	0xffff)
		return;

	if	(lpShop->m_bf2Status	==	ePSS_REQUIRE_INFO)
	{
		addRequireInfoShop(_iSerial);
		return;
	}

	if	(lpShop->m_bf2Status	==	ePSS_WAIT_INFO)
		return;

	CPos	pos;
	int		iX,iY;
	cRECT	rect;
	int		iShape			=	lpShop->m_wSignboardShape;
	char	*lpstrName		=	lpShop->m_strName;

	cSPRITE	*lpsprBoard		=	&g_asprPitchmanShop[iShape];

	iX	=	lpShop->m_wX-g_map.m_pos.x;
	iY	=	lpShop->m_wY-g_map.m_pos.y;

	int		aiSignBoardCorrectHeight[10]	=	
	{
		0,		-9,		-2,		0,		7,		-7,		0,		0,		0,		0,
	};

//	pos.y	+=	iDY;

	CTextDC	*lpTextDC	=	NULL;
	int		iBoard		=	0;

	if (lpShop->m_bf1IsBold)
		lpTextDC	=	&s_textBold13;
	else
		lpTextDC	=	&s_text13;

	lpTextDC->setShadowMargin(1);
	lpTextDC->someCodeIgnoreOn();	//	스크립트를 쌩까고 출력
	lpTextDC->userReturnOn();
	lpTextDC->scriptPutOff();	//	스크립트를 쌩까고 출력

	lpTextDC->pushShadowStatus(lpShop->m_bf1IsShadowText);

	pos.x	=	iX;
	pos.y	=	iY;

	int		iDY	=	dCOS_TABLE[(s_iFrameCounter+lpShop->m_wSerial)*4%360]*2/256;

	pos.y	+=	iDY+3	;
	rect.x1	=	pos.x-dSIGNBOARD_WIDTH/2;
	rect.x2	=	pos.x+dSIGNBOARD_WIDTH/2;

	int	iLineCount	=	lpTextDC->getLineCount(dSIGNBOARD_WIDTH,lpstrName,TRUE,TRUE);

	if	(iLineCount	>=	3)
		iBoard		=	1;

	lpsprBoard->PutShadow(pos.x,pos.y+20,0);

	if	(m_wFocusShop	==	lpShop->m_wSerial)
	{
		WORD	*lpPalette		=	lpsprBoard->getOrgPalette();

		static	WORD	awPlt[256];
		EffectPalette(dPLT_EFFECT_BRIGHT,lpPalette,awPlt,150,0);

		lpsprBoard->setPalette(awPlt);
	}
	else
		lpsprBoard->restorePalette();

	lpsprBoard->Put(pos.x,pos.y,iBoard);

	pos.y	+=	aiSignBoardCorrectHeight[iShape];
	pos.y	-=	100;
	rect.y1	=	pos.y-8-iBoard*16;
	rect.y2	=	pos.y+20;

	if	(lpShop->m_bf2Status	==	ePSS_READY)
	{
		if	(g_hero.m_wPitchmanShopSerial	!=	_iSerial)
			lpstrName	=	dMSG_READY_TO_OPEN_PITCHMAN_SHOP;
	}

	{
		DWORD	dwColor	=	RGB565To24(lpShop->m_wTextColor);

		lpTextDC->textMixOff();
		lpTextDC->cPut(rect.x1,rect.y1,rect.getWidth(),rect.getHeight(),0,dwColor,dFITTEXT_CENTER,lpstrName);
		lpTextDC->textMixOn();
	}

	lpTextDC->scriptPutOn();	//	스크립트를 쌩까고 출력
	lpTextDC->popShadowStatus();
	lpTextDC->someCodeIgnoreOff();	//	스크립트를 쌩까고 출력
	lpTextDC->userReturnOff();
}

BOOL
CPitchmanShop::isFocus(int _iSerial)
{
	CPitchmanShopInfo	*lpShop	=	&m_aShops[_iSerial];

	if	(lpShop->m_wSerial	==	0xffff)
		return	FALSE;

	if	(lpShop->m_bf2Status	==	ePSS_WAIT_INFO)
		return	FALSE;

	CPos	pos;
	int		iShape			=	lpShop->m_wSignboardShape;
	cRECT	rect;
	char	*lpstrName		=	lpShop->m_strName;

	int		aiSignBoardCorrectHeight[10]	=	
	{
		0,		-9,		-2,		0,		7,		-7,		0,		0,		0,		0,
	};

	pos.x	=	lpShop->m_wX-g_map.m_pos.x+aiSignBoardCorrectHeight[iShape];
	pos.y	=	lpShop->m_wY-g_map.m_pos.y-100;

	rect.x1	=	pos.x-dSIGNBOARD_WIDTH/2;
	rect.x2	=	pos.x+dSIGNBOARD_WIDTH/2;
	rect.y1	=	pos.y-8;
	rect.y2	=	pos.y+20;

	if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		return	TRUE;

	return	FALSE;

}

int
CPitchmanShop::getViewShop(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_PITCHMAN_SHOP_COUNT;i++)
	{
		if	(m_aShops[i].m_wSerial == 0xffff)
			continue;

		if	(g_hero.m_bf1IsHidePitchmanShop)
			if	(m_aShops[i].m_wSignboardShape == 0 && m_aShops[i].m_wTextColor == 0 && m_aShops[i].m_bf1IsBold == FALSE)
				continue;

		if	(_lpRectViewport->isIn(m_aShops[i].m_wX,m_aShops[i].m_wY))
		{
			_lpIndexes[iCount].Index	=	dBORDER_PITCHMAN_SHOP+i;
			_lpIndexes[iCount].Value	=	m_aShops[i].m_wY;
			iCount++;
		}
	}

	return	iCount;
}

int
CPitchmanShop::getFocusShop()
{
	if	(s_posMouse.x >= g_iScreenWidth-(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))
		return	0xffff;

	for (int i=0;i<dMAX_PITCHMAN_SHOP_COUNT;i++)
	{
		if	(m_aShops[i].m_wSerial == 0xffff)
			continue;

		if	(m_aShops[i].m_wSerial == g_hero.m_wPitchmanShopSerial)
			continue;

		if	(m_aShops[i].m_bf2Status	==	ePSS_REQUIRE_INFO)
			continue;

		if	(g_hero.m_bf1IsHidePitchmanShop)
			if	(m_aShops[i].m_wSignboardShape == 0 && m_aShops[i].m_wTextColor == 0 && m_aShops[i].m_bf1IsBold == FALSE)
				continue;

		if	(isFocus(i))
			return	i;
	}

	return	0xffff;
}

void
CPitchmanShop::update()
{
	if	((s_iFrameCounter % dSYNC_FPS) == 0)
	{
		sendStackedRequireInfoShopList();
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON) || m_wIsWaitWorkResult)
	{
		m_wFocusShop	=	0xffff;
		return;
	}

	if	(IsOpenedEventWindow())
		return;

	m_wFocusShop	=	getFocusShop();

	if	(s_isLeftButtonPressed	==	FALSE)
		cINPDEV::ReleaseExclusive();

	if	(s_isTouchLeftButton	&&	m_wFocusShop	!=	0xffff)
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON);

		m_wIsWaitWorkResult	=	TRUE;
		s_agent.sendAskPitchmanShopInfo(m_wFocusShop);
		g_hero.sendStop();
	}
}

void
CPitchmanShop::setInfoForShopKeeper(int _iSerial,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText)
{
	if (strlen(_lpstrName) >= 2)
		strcpy(m_aShops[_iSerial].m_strName,_lpstrName);

	m_aShops[_iSerial].m_wSignboardShape	=	_iSignboardShape;
	m_aShops[_iSerial].m_wTextColor			=	_wTextColor;
	m_aShops[_iSerial].m_bf1IsBold			=	_bIsBold;
	m_aShops[_iSerial].m_bf1IsShadowText	=	_bIsShadowText;
}

BOOL
CPitchmanShop::isExistCrashShop(int _iX,int _iY)
{
	for (int i=0;i<dMAX_PITCHMAN_SHOP_COUNT;i++)
	{
		if (m_aShops[i].m_wSerial == 0xffff || m_aShops[i].m_wSignboardShape == 0xffff)
			continue;

		if (g_hero.m_bf1IsHidePitchmanShop)
			if (m_aShops[i].m_wSignboardShape == 0 && m_aShops[i].m_wTextColor == 0 && m_aShops[i].m_bf1IsBold == FALSE)
				continue;

		int	iRange	=	GetOvalRange(m_aShops[i].m_wX,m_aShops[i].m_wY,_iX,_iY);
		if (iRange	<=	dPITCHMAN_SHOP_SIZE*dPITCHMAN_SHOP_SIZE)
			return	TRUE;
	}

	return	FALSE;
}
