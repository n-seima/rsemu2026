#pragma	once

#include "jobDefine.h"
#include "sfc.h"

#define	dALLIGNMENT_ABSOLUTE_EVIL					1		//	절대 악
#define	dALLIGNMENT_EVIL							2		//	악
#define	dALLIGNMENT_NEUTRAL							3		//	중립
#define	dALLIGNMENT_GOOD							4		//	선
#define	dALLIGNMENT_ABSOLUTE_GOOD					5		//	절대 선

// 1견습기사
// 2전사
// 3마법사
// 4늑대인간
// 5사제
// 6타락천사
// 7도둑
// 8무도가
// 9마법창병
// 10마법궁수
// 11조련사
// 12소환사
// 13공주
// 14마법소녀
// 15강신술사
// 16악마
// 17영술사
// 18투사
// 19광학사
// 20수인

const	int		c_aUseBulletJob[]	=
{
	0,0,	// 견습기사/ 전사
	0,0,	// 마법사/늑대인간.
	0,0,	// 사제/타락천사
	0,0,	// 도둑/무도가
	0,1,	// 마법창병/마법궁수
	0,0,	// 조련사/ 소환사
	1,0,	// 공주/마법소녀
	0,0,	// 강신술사/악마
	0,0,	// 영술사/투사
	0,0,	// 광학사 / 수인
};


const	int		c_aUseShieldJob[]	=
{
	1,0,	// 견습기사/ 전사
	0,0,	// 마법사/늑대인간.
	1,0,	// 사제/타락천사
	0,0,	// 도둑/무도가
	0,0,	// 마법창병/마법궁수
	0,0,	// 조련사/ 소환사
	0,0,	// 공주/마법소녀
	0,0,	// 강신술사/악마
	0,0,	// 영술사/투사
	0,0,	// 광학사 / 수인
};

const	int		c_aUseWeaponJob[]	=
{
	1,1,	// 견습기사/ 전사
	1,1,	// 마법사/늑대인간.
	1,1,	// 사제/타락천사
	1,0,	// 도둑/무도가
	1,1,	// 마법창병/마법궁수
	1,1,	// 조련사/ 소환사
	1,1,	// 공주/마법소녀
	1,1,	// 강신술사/악마
	1,1,	// 영술사/투사
	1,1,	// 광학사 / 수인
};

const	int		c_aVisibleWeaponJob[]	=
{
	1,1,	// 견습기사/ 전사
	1,0,	// 마법사/늑대인간.
	1,1,	// 사제/타락천사
	1,0,	// 도둑/무도가
	1,1,	// 마법창병/마법궁수
	1,1,	// 조련사/ 소환사
	1,1,	// 공주/마법소녀
	0,1,	// 강신술사/악마
	1,1,	// 영술사/투사
	1,0,	// 광학사 / 수인
};

class	cDEFAULT_JOB_DATA
{
public:
	WORD	m_aWeapon[2];
	WORD	m_aWeaponCount[2];
	WORD	m_wShield;
	WORD	m_wBullet;
	WORD	m_wBulletCount;
	WORD	m_wGlove;
	WORD	m_wBelt;
	WORD	m_wDefaultSkill;
	WORD	m_wSlot0Item,m_wSlot0ItemCount;
	WORD	m_wStatus;
	

			cDEFAULT_JOB_DATA()
			{
				memset(this,0xff,sizeof(cDEFAULT_JOB_DATA));
				m_aWeaponCount[0]	=	0;
				m_aWeaponCount[1]	=	0;
				m_wBulletCount		=	0;
				m_wDefaultSkill		=	0xffff;
				m_wStatus			=	0;
			}
};	//	class	cDEFAULT_EQUIPPED_ITEM



extern	cDEFAULT_JOB_DATA	g_aPlayer1LvSetting[dPLAYER_JOB_COUNT];

const	BOOL	c_aIsMelee[]	=
{
	TRUE,TRUE,		//	견습기사/전사
	FALSE,TRUE,		//	마법사/늑대인간
	TRUE,FALSE,		//	사제/타락천사
	FALSE,TRUE,		//	도둑/무도가
	TRUE,FALSE,		//	마법 창검사/마법 궁수	
	FALSE,FALSE,	//	조련사/소환사	
	FALSE,FALSE,	//	공주/마법 소녀	
	FALSE,FALSE,	//	네크로맨서/악마
	TRUE,TRUE,		//	영술사/투사
	FALSE,TRUE,		//	광학사/수인
};

const	BOOL	c_aIsMale[]	=
{
	TRUE,TRUE,	//	견습기사/전사
	TRUE,TRUE,	//	마법사/늑대인간
	TRUE,TRUE,	//	사제/타락천사
	TRUE,TRUE,	//	도둑/무도가
	FALSE,FALSE,//	마법 창검사/마법 궁수	
	FALSE,FALSE,//	조련사/소환사	
	FALSE,FALSE,//	공주/마법 소녀	
	FALSE,FALSE,//	네크로맨서/악마
	FALSE,FALSE,//	영술사/투사
	TRUE,TRUE,	//	광학사/수인
};

const	BOOL	c_aFreeJobChangeJob[]	=	//	CP에 상관없이 직업 전환이 가능하냐?
{
	TRUE,TRUE,	//	견습기사/전사
	FALSE,TRUE,	//	마법사/늑대인간
	FALSE,TRUE,	//	사제/타락천사
	TRUE,TRUE,	//	도둑/무도가
	TRUE,TRUE,	//	마법 창검사/마법 궁수
	TRUE,TRUE,	//	조련사/소환사
	FALSE,TRUE,	//	공주/마법 소녀
	FALSE,TRUE,	//	네크로맨서/악마
	TRUE,TRUE,	//	영술사/투사
	FALSE,TRUE,	//	광학사/수인
};

inline	int	
GetAllignment(int _iValue)		//	성향을 구한다.
{
	if	(_iValue	>=	97)	
		return	dALLIGNMENT_ABSOLUTE_GOOD;
	if	(_iValue	>=	66)	
		return	dALLIGNMENT_GOOD;
	if	(_iValue	>=	-20)
		return	dALLIGNMENT_NEUTRAL;
	if	(_iValue	>=	-89)
		return	dALLIGNMENT_EVIL;

	return	dALLIGNMENT_ABSOLUTE_EVIL;
}

extern	int	g_aiLevelUpValue[][10];
