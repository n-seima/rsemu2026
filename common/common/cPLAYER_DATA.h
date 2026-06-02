#ifndef _classPLAYER_DATA
#define	_classPLAYER_DATA

#include "definePacketData.h"
#include "playerJobCommon.h"
//#include "cITEM.h"
//#include <windows.h>

//
//	Check Actor Data Result
enum
{
	eCADR_ISOK,
	eCADR_IS_NPC,
	eCADR_IS_NPC2,
	eCADR_VALID_SERIAL,
	eCADR_INCORRECT_JOB,
	eCADR_INCORRECT_ITEM_SLOT_COUNT,
	eCADR_INCORRECT_LEVEL,
	eCADR_INCORRECT_SKILL_DATA,
	eCADR_MISMATCH_SKILL_AND_JOB,
	eCADR_MISMATCH_NAME_BY_CLIENT,
	eCADR_MISMATCH_ID_BY_CLIENT,
	eCADR_INCORRECT_STATE_POINT,
};

const	int	c_aItemCount[10]	=	{31 ,32 ,33 ,34 ,36 ,37 ,38 ,39 ,40 ,42};
class	cITEM;

#define	dDEFAULT_ITEM_SLOT_COUNT	30
//
//	액터 데이터(서버에 저장할때 이 데이터를 저장한다.)
class	cPLAYER_DATA	:	public CPlayerSaveDataForServerDefine
{
public:

	inline	int			getInventorySize()
	{
		int	iInventorySize	=	dDEFAULT_ITEM_SLOT_COUNT;

		for (int j=0;j<dMAX_PLAYER_TITLE_COUNT;j++)
			if (m_aTitle[j].m_bTitle == 2)	//	짐꾼
				iInventorySize	=	c_aItemCount[m_aTitle[j].m_bLevel-1];

		return	iInventorySize;
	}

	void				getAvatarData(cAVATAR_INFO *_lpAvataInfo);

	void				initializeJobData(char *_strId,char *_strName,int _iJob,int _iField,int _iServerType=-1);
	void				rebuildSkillInfo();

	int					getCurrentStatePointSum();
	int					getCurrentStatePointSumByLevel();
	
	BOOL				getLevelBySkillPoint(int *_lpiLevel,int *_lpiExp,int *_lpiSkillPoint);
	BOOL				getCurrentSkillPointSum();
	int					isCorrectSkillData();

	int					getCurrentSkillPointByLevel();
	int					checkData();

	int					getValidItemSlot();
	BOOL				resetStatusAndSkillPoint();
	BOOL				resetSkillPoint();
	BOOL				resetStatePoint();

	BOOL				convertData(cP_PLAYER_SAVE_DATA *_lpData);
//	playerData.cpp
	int					getBoostExperienceValue();
	BOOL				operateCorrectExpWork050706(int _iCorrectExp,cFILE *_lpFile,char *_lpstrWord);	
	BOOL				operateBreedingRecordBook050708(cFILE *_lpFile);
	void				recovery050818QuestResetUser(int _iSelectWorld);

	cITEM*				findQuestItem(int _iKind,int _iQuest,int _iTimes);
	cITEM*				findValidItemSlot();
	tsProcessQuestField*	getQuest(int _iKind,int _iQuest);
	BOOL					removeQuest(int _iKind,int _iQuest);
	tsProcessQuestField*	getValidQuest();
	BOOL				isCompleteQuest(int _iKind,int _iQuest);
	BOOL				setCompleteQuestStatus(int _iKind,int _iQuest,int _iOnOff);

	BOOL				decreaseExp(DWORD _dwDecreaseExp);
	BOOL				increaseExp(int _iIncreaseExp);
	BOOL				addTitle(int _iTitle,int _iTitleLevel);
	BOOL				removeTitle(int _iTitle);

	static	BOOL		Init();
};	//	#define		dABILITY_COUNT						52

extern	cPLAYER_DATA	g_aPlayerDefaultJobData[dPLAYER_JOB_COUNT];
extern	BOOL			LoadDefaultPlayerData(char *fn);
extern	DWORD			g_aExpTable[2000];

#define dEQUIP_WEAPON			0
#define dEQUIP_WEAPON_2			17
#define dEQUIP_SHIELD_BULLET	1
#define	dEQUIP_ARMOR			2			//	갑옷

#define	dOWN_ITEM_COUNT								42
#define	dEQUIPMENT_PART_COUNT						23
#define	dDEFAULT_ITEM_SLOT_COUNT					30

#endif
