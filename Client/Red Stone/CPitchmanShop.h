#ifndef _classPitchmanShop_h
#define _classPitchmanShop_h

#include "SFC.H"
#include "CPitchmanShopDefine.H"
#include "CGamePlay.H"

#define	dMAX_PITCHMAN_SHOP_COUNT	1024
#define	dBORDER_PITCHMAN_SHOP		30000

class	CPitchmanShopInfo
{
public:
	WORD			m_wSerial;
	WORD			m_wSignboardShape;
	WORD			m_wTextColor;

	WORD			m_bf1IsBold			: 1;
	WORD			m_bf1IsShadowText	: 1;
	WORD			m_bf2Status			: 2;
	WORD			m_bf12Owner			: 12;
	WORD			m_wBoardLineCount;

	WORD			m_wX,m_wY;
	char			m_strName[dPITCHMAN_SHOP_NAME_LENGTH];
};


class	CPitchmanShop : public CGamePlay
{
public:
	WORD				m_wCount,m_wFocusShop,m_wIsWaitWorkResult,m_wRequireInfoShopCount;
	WORD				m_awRequireInfoShopList[dMAX_ASK_SIMPLE_PITCHMAN_SHOP_INFO_COUNT];
	WORD				m_awRequireInfoShopListRequestCount[dMAX_PITCHMAN_SHOP_COUNT];
	CPitchmanShopInfo	m_aShops[dMAX_PITCHMAN_SHOP_COUNT];

	void				reset();

	BOOL				setShopInfo(CSimplePitchmanShopInfo *_lpInfo,char *_lpstrShopName);
	BOOL				addTemplate(int _iSerial,int _iX,int _iY);

	void				addRequireInfoShop(int _iSerial);
	void				sendStackedRequireInfoShopList();
	BOOL				remove(int _iSerial);
	void				changeStatus(CSimplePitchmanShopInfo *_lpInfo);
	void				draw(int _iSerial);
	int					getViewShop(cINDEX *_lpIndexes,cRECT *_lpRectViewport);
	BOOL				isFocus(int _iSerial);
	BOOL				isExistCrashShop(int _iX,int _iY);
	void				setInfoForShopKeeper(int _iSerial,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText);

	void				update();
	int					getFocusShop();
};

extern	CPitchmanShop	g_pitchmanShop;

#endif