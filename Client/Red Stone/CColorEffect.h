#ifndef	_classColorEffect_
#define	_classColorEffect_

#include "CNemo.H"
#include "CGame.H"

#define	dMAX_PREFIX_EFFECT_COLOR_COUNT	100

//	Prefix Colorize Effect Method
enum
{
	ePCEM_COLORIZE,
	ePCEM_BRIGHT,
};

class	CPrefixColorizeEffectInfo
{
public:
	WORD	m_wMethod;	//	은은하게~ 급격하게~ 이따금
	WORD	m_wColor1,m_wColor2;
	WORD	m_wIntensity,m_wSpeed;

	void	set(int _iMethod,int _iIntensity,int _iSpeed,int _iColor1=0xffffffff,int _iColor2=0xffffffff);
};


class	CColorEffect	:	public	CGame		//	캐릭터 몸에 나타나는 각종 마법효과
{
public:
	CPrefixColorizeEffectInfo	m_aPrefixColorizeEffect[dMAX_PREFIX_EFFECT_COLOR_COUNT];

	WORD	m_w1stFireDamageColor,m_w2stFireDamageColor1,m_w2stFireDamageColor2;
	WORD	m_w1stWaterDamageColor,m_w2stWaterDamageColor1,m_w2stWaterDamageColor2;
	WORD	m_w1stWindDamageColor,m_w2stWindDamageColor1,m_w2stWindDamageColor2;
	WORD	m_w1stEarthDamageColor,m_w2stEarthDamageColor1,m_w2stEarthDamageColor2;
	WORD	m_w1stLightDamageColor,m_w2stLightDamageColor1,m_w2stLightDamageColor2;
	WORD	m_w1stDarkDamageColor,m_w2stDarkDamageColor1,m_w2stDarkDamageColor2;
	WORD	m_w1stIncreaseDefensivePower,m_w2stIncreaseDefensivePower,m_w3stIncreaseDefensivePower;
	WORD	m_w1stIncreaseBasicPower	,m_w2stIncreaseBasicPower,m_w3stIncreaseBasicPower	;
	WORD	m_w1stIncreaseHP,m_w2stIncreaseHP,m_w3stIncreaseHP;
	WORD	m_w1stIncreaseCP,m_w2stIncreaseCP,m_w3stIncreaseCP;
	WORD	m_w1stStrageStatusResistance,m_w2stStrageStatusResistance,m_w3stStrageStatusResistance;
	WORD	m_w1stThorn,m_w2stThorn,m_w3stThorn;

	void	init();
};

extern	CColorEffect	g_colorEffect;

#endif