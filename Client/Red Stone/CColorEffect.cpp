#include	"CColorEffect.h"
#include	"CItem.h"

CColorEffect	g_colorEffect;

void
CPrefixColorizeEffectInfo::set(int _iMethod,int _iIntensity,int _iSpeed,int _iColor1,int _iColor2)
{
	if (_iColor2 == 0xffffffff)
		_iColor2	=	_iColor1;

	m_wColor1	=	_iColor1;
	m_wColor2	=	_iColor2;
	m_wIntensity=	_iIntensity;
	m_wMethod	=	_iMethod;
	m_wSpeed	=	_iSpeed;
}

void
CColorEffect::init()
{
	m_aPrefixColorizeEffect[ePOE_PHYSICAL_DAMAGE_1TH].set(ePCEM_BRIGHT,200				,400);
	m_aPrefixColorizeEffect[ePOE_PHYSICAL_DAMAGE_2TH].set(ePCEM_BRIGHT,300				,800);
	m_aPrefixColorizeEffect[ePOE_PHYSICAL_DAMAGE_3TH].set(ePCEM_BRIGHT,400				,1000);
	m_aPrefixColorizeEffect[ePOE_FIRE_DAMAGE_1TH].set(ePCEM_COLORIZE,20					,200,RGB24To16(250,71,16));
	m_aPrefixColorizeEffect[ePOE_FIRE_DAMAGE_2TH].set(ePCEM_COLORIZE,40					,400,RGB24To16(255,0,0),RGB24To16(255,168,0));
	m_aPrefixColorizeEffect[ePOE_FIRE_DAMAGE_3TH].set(ePCEM_COLORIZE,60					,800,RGB24To16(255,0,0),RGB24To16(255,168,0));
	m_aPrefixColorizeEffect[ePOE_WATER_DAMAGE_1TH].set(ePCEM_COLORIZE,20				,200,RGB24To16(16,148,250));
	m_aPrefixColorizeEffect[ePOE_WATER_DAMAGE_2TH].set(ePCEM_COLORIZE,40				,400,RGB24To16(0,84,255),RGB24To16(0,234,255));
	m_aPrefixColorizeEffect[ePOE_WATER_DAMAGE_3TH].set(ePCEM_COLORIZE,80				,800,RGB24To16(0,84,255),RGB24To16(0,234,255));
	m_aPrefixColorizeEffect[ePOE_WIND_DAMAGE_1TH].set(ePCEM_COLORIZE,20					,200,RGB24To16(0,128,10));
	m_aPrefixColorizeEffect[ePOE_WIND_DAMAGE_2TH].set(ePCEM_COLORIZE,40					,400,RGB24To16(0,93,11),RGB24To16(19,247,57));
	m_aPrefixColorizeEffect[ePOE_WIND_DAMAGE_3TH].set(ePCEM_COLORIZE,80					,800,RGB24To16(0,93,11),RGB24To16(19,247,57));
	m_aPrefixColorizeEffect[ePOE_EARTH_DAMAGE_1TH].set(ePCEM_COLORIZE,20				,200,RGB24To16(129,180,0));
	m_aPrefixColorizeEffect[ePOE_EARTH_DAMAGE_2TH].set(ePCEM_COLORIZE,40				,400,RGB24To16(164,120,1),RGB24To16(255,255,0));
	m_aPrefixColorizeEffect[ePOE_EARTH_DAMAGE_3TH].set(ePCEM_COLORIZE,80				,800,RGB24To16(164,120,1),RGB24To16(255,255,0));
	m_aPrefixColorizeEffect[ePOE_LIGHT_DAMAGE_1TH].set(ePCEM_COLORIZE,20				,200,RGB24To16(255,255,219));
	m_aPrefixColorizeEffect[ePOE_LIGHT_DAMAGE_2TH].set(ePCEM_COLORIZE,40				,400,RGB24To16(255,248,185),RGB24To16(255,255,255));
	m_aPrefixColorizeEffect[ePOE_LIGHT_DAMAGE_3TH].set(ePCEM_COLORIZE,80				,800,RGB24To16(255,248,185),RGB24To16(255,255,255));
	m_aPrefixColorizeEffect[ePOE_DARK_DAMAGE_1TH].set(ePCEM_COLORIZE,20					,200,RGB24To16(57,69,64));
	m_aPrefixColorizeEffect[ePOE_DARK_DAMAGE_2TH].set(ePCEM_COLORIZE,40					,400,RGB24To16(178,178,177),RGB24To16(0,0,0));
	m_aPrefixColorizeEffect[ePOE_DARK_DAMAGE_3TH].set(ePCEM_COLORIZE,80					,800,RGB24To16(178,178,177),RGB24To16(0,0,0));
	m_aPrefixColorizeEffect[ePOE_DEFENSIVE_POWER_1TH].set(ePCEM_COLORIZE,20				,200,RGB24To16(150,148,148));
	m_aPrefixColorizeEffect[ePOE_DEFENSIVE_POWER_2TH].set(ePCEM_COLORIZE,40				,400,RGB24To16(226,165,35),RGB24To16(235,174,43));
	m_aPrefixColorizeEffect[ePOE_DEFENSIVE_POWER_3TH].set(ePCEM_COLORIZE,80				,800,RGB24To16(226,165,35),RGB24To16(235,174,43));
	m_aPrefixColorizeEffect[ePOE_BASIC_POWER_1TH].set(ePCEM_COLORIZE,20					,200,RGB24To16(199,74,230));
	m_aPrefixColorizeEffect[ePOE_BASIC_POWER_2TH].set(ePCEM_COLORIZE,40					,400,RGB24To16(138,18,172),RGB24To16(159,41,184));
	m_aPrefixColorizeEffect[ePOE_BASIC_POWER_3TH].set(ePCEM_COLORIZE,80					,800,RGB24To16(138,18,172),RGB24To16(159,41,184));
	m_aPrefixColorizeEffect[ePOE_INCREASE_HP_1TH].set(ePCEM_COLORIZE,20					,200,RGB24To16(255,87,14));
	m_aPrefixColorizeEffect[ePOE_INCREASE_HP_2TH].set(ePCEM_COLORIZE,40					,400,RGB24To16(219,22,21),RGB24To16(255,74,18));
	m_aPrefixColorizeEffect[ePOE_INCREASE_HP_3TH].set(ePCEM_COLORIZE,80					,800,RGB24To16(219,22,21),RGB24To16(255,74,18));
	m_aPrefixColorizeEffect[ePOE_INCREASE_CP_1TH].set(ePCEM_COLORIZE,20					,200,RGB24To16(12,172,255));
	m_aPrefixColorizeEffect[ePOE_INCREASE_CP_2TH].set(ePCEM_COLORIZE,40					,400,RGB24To16(33,68,244),RGB24To16(12,50,247));
	m_aPrefixColorizeEffect[ePOE_INCREASE_CP_3TH].set(ePCEM_COLORIZE,80					,800,RGB24To16(33,68,244),RGB24To16(12,50,247));
	m_aPrefixColorizeEffect[ePOE_ELEMENTS_RESISTANCE_1TH].set(ePCEM_COLORIZE,20			,200,RGB24To16(255,255,57));	
	m_aPrefixColorizeEffect[ePOE_ELEMENTS_RESISTANCE_2TH].set(ePCEM_COLORIZE,400		,400,RGB24To16(255,255,57));
	m_aPrefixColorizeEffect[ePOE_ELEMENTS_RESISTANCE_3TH].set(ePCEM_COLORIZE,80			,800,RGB24To16(48,192,57));
	m_aPrefixColorizeEffect[ePOE_STRANGE_STATUS_RESISTANCE_1TH].set(ePCEM_COLORIZE,20	,200,RGB24To16(48,192,57));
	m_aPrefixColorizeEffect[ePOE_STRANGE_STATUS_RESISTANCE_2TH].set(ePCEM_COLORIZE,40	,400,RGB24To16(54,129,58),RGB24To16(38,165,8));
	m_aPrefixColorizeEffect[ePOE_STRANGE_STATUS_RESISTANCE_3TH].set(ePCEM_COLORIZE,80	,800,RGB24To16(54,129,58),RGB24To16(38,165,8));
	m_aPrefixColorizeEffect[ePOE_THORN_1TH].set(ePCEM_COLORIZE,20						,200,RGB24To16(110,108,115));
	m_aPrefixColorizeEffect[ePOE_THORN_2TH].set(ePCEM_COLORIZE,40						,400,RGB24To16(82,16,33),RGB24To16(10,1,15));
	m_aPrefixColorizeEffect[ePOE_THORN_3TH].set(ePCEM_COLORIZE,80						,800,RGB24To16(82,16,33),RGB24To16(10,1,15));

/*	m_w1stFireDamageColor		=	RGB24To16(250,71,	16);
	m_w2stFireDamageColor1		=	RGB24To16(255,0,0	);
	m_w2stFireDamageColor2		=	RGB24To16(255,168,0	);

	m_w1stWaterDamageColor		=	RGB24To16(16,148,250);
	m_w2stWaterDamageColor1		=	RGB24To16(0,84,255);
	m_w2stWaterDamageColor2		=	RGB24To16(0,234,255);

	m_w1stWindDamageColor		=	RGB24To16(0,128,10);
	m_w2stWindDamageColor1		=	RGB24To16(0,93,11);
	m_w2stWindDamageColor2		=	RGB24To16(19,247,57);

	m_w1stEarthDamageColor		=	RGB24To16(129,180,0);
	m_w2stEarthDamageColor1		=	RGB24To16(164,120,1);
	m_w2stEarthDamageColor2		=	RGB24To16(255,255,0);

	m_w1stLightDamageColor		=	RGB24To16(255,255,219);
	m_w2stLightDamageColor1		=	RGB24To16(255,248,185);
	m_w2stLightDamageColor2		=	RGB24To16(255,255,255);

	m_w1stDarkDamageColor		=	RGB24To16(57,69,64);
	m_w2stDarkDamageColor1		=	RGB24To16(178,178,177);
	m_w2stDarkDamageColor2		=	RGB24To16(0,0,0);

	m_w1stIncreaseDefensivePower=	RGB24To16(150,148,148);
	m_w2stIncreaseDefensivePower=	RGB24To16(226,165,35);
	m_w3stIncreaseDefensivePower=	RGB24To16(235,174,43);

	m_w1stIncreaseBasicPower	=	RGB24To16(199,74,230);
	m_w2stIncreaseBasicPower	=	RGB24To16(138,18,172);
	m_w3stIncreaseBasicPower	=	RGB24To16(159,41,184);

	m_w1stIncreaseHP			=	RGB24To16(255,87,14);
	m_w2stIncreaseHP			=	RGB24To16(219,22,21);
	m_w3stIncreaseHP			=	RGB24To16(255,74,18);

	m_w1stIncreaseCP			=	RGB24To16(12,172,255);
	m_w2stIncreaseCP			=	RGB24To16(33,68,244 );
	m_w3stIncreaseCP			=	RGB24To16(12,50,247);

	m_w1stStrageStatusResistance=	RGB24To16(48,192,57);
	m_w2stStrageStatusResistance=	RGB24To16(54,129,58);
	m_w3stStrageStatusResistance=	RGB24To16(38,165,8);

	m_w1stThorn					=	RGB24To16(110,108,115);
	m_w2stThorn					=	RGB24To16(82,16,33);
	m_w3stThorn					=	RGB24To16(10,1,15);
	*/
}