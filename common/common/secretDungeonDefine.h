#ifndef	_classSecretDungeonDefine_h
#define	_classSecretDungeonDefine_h

#ifndef _WINDOWS_
#include <windows.h>
#endif

#define	dMAX_FLOOR_COUNT_FOR_SECRET_DUNGEON		10
#define	dMAX_SERIAL_FOR_GUILD_DUNGEON			30
#define	dSECRET_DUNGEON_NAME_LENGTH				56
#define	dSECRET_DUNGEON_IDCODE_LENGTH			32
#define	dMAX_SECRET_DUNGEON_COUNT				256
#define	dINSTANCE_FIELD_FLAG					0x800	//	필드 인덱스가 2048 이상이면 비던 취급 한다.
#define	dENTER_SECRET_DUNGEON_LIMIT_PER_ONE_DAY	2

#define	dSERECT_DUNGEON_KEY_REGEN_PERIOD		10

#define	dMAX_DUNGEON_VALUE_COUNT				50
#define	dDUNGEON_VALUE_TEXT_LENGTH				64

class	cSecretDungeonFloorDefine
{
public:
	WORD	m_wCorrectExperience;
	WORD	m_wIsRegenMonster;

	void	reset()
	{
		m_wCorrectExperience	=	100;
		m_wIsRegenMonster		=	FALSE;
	}
};

class	cSecretDungeonDefine
{
public:
	WORD						m_wSerial;
	WORD						m_wLimitCount;
	WORD						m_bf8FloorCount		:	8;
	WORD						m_bf1IsTestDungeon	:	1;	//	테섭에서만 나온다.
	WORD						m_bf1IsTowerOfOrdeal:	1;	//	시련의 탑이다.
	WORD						m_bf1IsGuildDungen	:	1;	//	길드 던젼이다.
	WORD						m_bf5GuildDungenSerial	:	5;	//	몇번째 길드 던젼이냐? 
	char						m_strName[dSECRET_DUNGEON_NAME_LENGTH];
	WORD						m_wActiveKeyCount;
	WORD						m_wMinLevel,m_wMaxLevel;
	WORD						m_wUpkeepTime;
	char						m_strIDCode[dSECRET_DUNGEON_IDCODE_LENGTH];

	cSecretDungeonFloorDefine	m_aFloor[dMAX_FLOOR_COUNT_FOR_SECRET_DUNGEON];
};

#endif