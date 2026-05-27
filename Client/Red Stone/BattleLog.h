#ifndef	_classBattleLog_
#define	_classBattleLog_

#include	"CSkill.h"
#include	"cACTOR.h"



class BattleLog
{ // 마지막수정일 : 09.09.23 
public:
	
	enum		// 09.09.22 배틀로그 공격 종류.
	{
			eATTACK_NORMAL,
			eATTACK_MISS,
			eATTACK_DODGE,
			eATTACK_BLOCK,
			eATTACK_CRUSH,
			eATTACK_CRITICAL,
			eATTACK_DOUBLE_CRITICAL,
			eATTACK_LUCKY_HIT_OR_DAMAGE,
			eATTACK_LUCKY_DODGE,
			eATTACK_LUCKY_DODGE_CRITICAL,
			eATTACK_HARD_BLOW,

	};

	BOOL	m_bIsActive;		//발동 여부
	BOOL	m_bIsShowDamage;		//대미지보기..!
	BOOL	m_bIsShowMonsterDamage;	// 몬스터대미지출력!
	HANDLE	m_hFile;

	void	Init();
	void	WriteLog(char* _lpAttacker , char* _lpDefender , CSkill* _lpSkill , int _iPhysicalDamage , int _iMagicDamage, int _iAttackClass = eATTACK_NORMAL);
	void	WriteExpLog(int _iExp);
	void	showDamage(char* _lpAttacker , char* _lpDefender , CSkill* _lpSkill , int _iPhysicalDamage , int _iMagicDamage, int _iAttackClass = eATTACK_NORMAL, BOOL _bIsMonster = FALSE);
	BattleLog()
	{
		m_bIsActive = FALSE;
		m_hFile		= NULL;
		m_bIsShowDamage = FALSE;
		m_bIsShowMonsterDamage = FALSE;
	}

	~BattleLog()
	{
		CloseHandle(m_hFile);
	}
	
};

extern	BattleLog	g_BattleLog;

#endif