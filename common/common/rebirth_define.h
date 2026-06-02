#pragma	

#define	dGAIN_EXP_PENELTY_LEVEL_FOR_REBIRTH				300

#ifdef	_FOR_JAPAN
const	int		c_aiGainExpPeneltyForRebirth[]	=	{0,0,5,10,15,20};
#else
const	int		c_aiGainExpPeneltyForRebirth[]	=	{0,0,0,0,0,0};
#endif

#define	dEXP_PENELTY_LEVEL_FOR_REBIRTH				300

#define	dSTATE_POINT_FOR_EVERY_REBIRTH				100
#define	dSKILL_POINT_FOR_EVERY_REBIRTH				100
#define	dLIMIT_REBIRTH_COUNT						5		//	최대 전생 횟수

const	int		c_aBonusStatePointForRebirth[]	=	{0,100,300,600,1000,1500};
const	int		c_aBonusSkillPointForRebirth[]	=	{0,100,300,600,1000,1500};
