#include "stdafx.h"
#include "AdminTool.h"
#include "CharacterInfo.h"
#include "CScript.h"
#include "cADO_ADMIN.h"
#include "afxdlgs.h"
#include "cDECODER.h"
#include "atlbase.h"
#include "CSkill.h"
#include "cITEM_PREFIX.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_LOGSERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "SFC.h"
#include "ExpCalc.h"
#include "io.h"
#include "CMessage.h"
#include "cPACKET_TOOLLOGSERVER.h"
#include "AdminLogCode.h"
#include "CTitle.h"
#include "questList.h"
#include "quest.h"
#include "guildInfo.h"
#include "ExpCalc.h"

BOOL
cPLAYER_DATA::operateBreedingRecordBook050708(cFILE *_lpFile)
{
// 	int		i;
	int		iBookCount	=	0;

// 	for (i=0;i<dOWN_ITEM_COUNT;i++)
// 	{
// 		cITEM	*lpItem				=	(cITEM	*)&m_aItems[i];
// 
// 		if	(!lpItem->isBreedingRecordBook())
// 			continue;
// 
// 		cPetDefine	petInfo;
// 
// 		memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
// 		petInfo.m_dwExperience		=	lpItem->m_dwSerial;
// 
// 		_lpFile->writeStreamCR("[\"%s\"/\"%s\"] \"%s\" %d/%d",m_strId,m_strName,petInfo.m_strName,petInfo.m_bf10Level,petInfo.m_dwExperience);
// 		_lpFile->writeStreamCR(" ");
// 
// 		iBookCount++;
// 	}
// 
// 	for (i=0;i<m_bf6ExtraInventorySize;i++)
// 	{
// 		cITEM		*lpItem			=	(cITEM	*)&m_aExtraInventory[i];
// 
// 		if	(!lpItem->isBreedingRecordBook())
// 			continue;
// 
// 		cPetDefine	petInfo;
// 
// 		memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
// 		petInfo.m_dwExperience		=	lpItem->m_dwSerial;
// 
// 		_lpFile->writeStreamCR("[\"%s\"/\"%s\"] \"%s\" %d/%d",m_strId,m_strName,petInfo.m_strName,petInfo.m_bf10Level,petInfo.m_dwExperience);
// 		_lpFile->writeStreamCR(" ");
// 
// 		iBookCount++;
// 	}

	return	iBookCount;
}

BOOL
cPLAYER_DATA::addTitle(int _iTitle,int _iTitleLevel)
{
	int	i;

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	_iTitle)
		{
			lpTitleInfo->m_bLevel	=	_iTitleLevel;

			return	TRUE;
		}
	}

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	0xff)
		{
			lpTitleInfo->m_bTitle		=	_iTitle;
			lpTitleInfo->m_bLevel		=	_iTitleLevel;

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
cPLAYER_DATA::removeTitle(int _iTitle)
{
	int	i;

	for (i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo	*lpTitleInfo	=	&m_aTitle[i];

		if	(lpTitleInfo->m_bTitle	==	_iTitle)
		{
			lpTitleInfo->m_bTitle		=	0xff;
			lpTitleInfo->m_bLevel		=	0;

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
cPLAYER_DATA::decreaseExp(DWORD _dwDecreaseExp)
{
	while(1)
	{
		if	(m_iExperience	>=	_dwDecreaseExp)
		{
			m_iExperience	-=	_dwDecreaseExp;

			return	TRUE;
		}

		if	(m_iLevel	<=	1)
		{
			ERRMSG("error","cPLAYER_DATA::decreaseExp");

			m_iExperience	=	0;

			return	FALSE;
		}

		m_iLevel--;

		_dwDecreaseExp		-=	m_iExperience;
		m_iExperience		=	g_aExpTable[m_iLevel];
	}

	return	TRUE;
}

enum
{
	ePSL_NORMAL,
	ePSL_LITE_LITE,
	ePSL_LITE_PREMIUM,
	ePSL_LITE_PLATINUM,
	ePSL_LITE,
	ePSL_PREMIUM,
	ePSL_PLATINUM,
};

int
cPLAYER_DATA::getBoostExperienceValue()
{
	int		iPreminumServiceLevel		=	0,i;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cITEM	*lpItem				=	(cITEM	*)&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		cBASIC_ITEM		*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
			continue;

		iPreminumServiceLevel	=	max((WORD)lpBasicItem->m_attr.premiumLevel,iPreminumServiceLevel);
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cITEM		*lpItem			=	(cITEM	*)&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		cBASIC_ITEM		*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
			continue;

		iPreminumServiceLevel	=	max((WORD)lpBasicItem->m_attr.premiumLevel,iPreminumServiceLevel);
	}

	int	iCorrectExp			=	100;

	if (iPreminumServiceLevel == ePSL_LITE)
		iCorrectExp			=	120;
	if (iPreminumServiceLevel == ePSL_PREMIUM)
		iCorrectExp			=	150;
	if (iPreminumServiceLevel == ePSL_PLATINUM)
		iCorrectExp			=	200;
	if (iPreminumServiceLevel == ePSL_LITE_LITE)
		iCorrectExp			=	120;
	if (iPreminumServiceLevel == ePSL_LITE_PREMIUM)
		iCorrectExp			=	150;
	if (iPreminumServiceLevel == ePSL_LITE_PLATINUM)
		iCorrectExp			=	200;

	return	iCorrectExp;
}

BOOL
cPLAYER_DATA::operateCorrectExpWork050706(int _iCorrectExp,cFILE *_lpFile,char *_lpstrWord)
{
// 	if	(_lpstrWord	==	NULL)
// 		_lpstrWord	=	" ";
// 
// 	int			iOldLevel	=	m_iLevel;
// 	int			iOldExp		=	m_iExperience,i;
// 
// 	if	(_iCorrectExp)
// 	{
// 		decreaseExp(_iCorrectExp);
// 		_lpFile->writeStreamCR("\"%s\"/\"%s\" \"%s\" [%d/%d] -%d [%d/%d] %s",m_strId,m_strName,g_strJobName[m_wJob],iOldLevel,iOldExp,_iCorrectExp,m_iLevel,m_iExperience,_lpstrWord);
// 	}
// 
// 	for	(i=0;i<dOWN_PET_COUNT;i++)
// 	{
// 		if	(m_aPet[i].m_bf10Level	>=	(DWORD)m_iLevel)
// 		{
// 			if	(_iCorrectExp	==	0)
// 			{
// 				_lpFile->writeStreamCR("\"%s\"/\"%s\" \"%s\" [%d/%d] -%d [%d/%d] %s",m_strId,m_strName,g_strJobName[m_wJob],iOldLevel,iOldExp,_iCorrectExp,m_iLevel,m_iExperience,_lpstrWord);
// 
// 				_iCorrectExp	=	1;
// 			}
// 
// 			_lpFile->writeStreamCR("    \"pet level down\" [%d/%d] -> [%d/%d]",m_aPet[i].m_bf10Level,m_aPet[i].m_dwExperience,m_iLevel,m_iExperience);
// 
// 			m_aPet[i].m_bf10Level		=	m_iLevel;
// 			m_aPet[i].m_dwExperience	=	m_iExperience;
// 		}
// 	}
// 
// 	{
// 		int		i;
// 		int		iBookCount	=	0;
// 
// 		for (i=0;i<dOWN_ITEM_COUNT;i++)
// 		{
// 			cITEM	*lpItem				=	(cITEM	*)&m_aItems[i];
// 
// 			if	(!lpItem->isBreedingRecordBook())
// 				continue;
// 
// 			cPetDefine	petInfo;
// 
// 			memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
// 			petInfo.m_dwExperience		=	lpItem->m_dwSerial;
// 
// 			if	(petInfo.m_bf10Level	>	(DWORD)m_iLevel)
// 			{
// 				int	iOldLevel			=	petInfo.m_bf10Level;
// 				int	iOldExp				=	petInfo.m_dwExperience;
// 				petInfo.m_bf10Level		=	m_iLevel;
// 				petInfo.m_dwExperience	=	m_iExperience;
// 
// 				lpItem->m_dwSerial		=	petInfo.m_dwExperience;
// 				char	*lpData			=	(char *)lpItem;
// 
// 				memcpy(lpData+6,&petInfo,sizeof(cPetDefine)-4);
// 
// 				memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
// 				petInfo.m_dwExperience		=	lpItem->m_dwSerial;
// 
// 				if	(_iCorrectExp	==	0)
// 				{
// 					_lpFile->writeStreamCR("\"%s\"/\"%s\" \"%s\" [%d/%d] -%d [%d/%d] %s",m_strId,m_strName,g_strJobName[m_wJob],iOldLevel,iOldExp,_iCorrectExp,m_iLevel,m_iExperience,_lpstrWord);
// 
// 					_iCorrectExp	=	1;
// 				}
// 
// 				_lpFile->writeStreamCR("    \"pet level down\" book [%d/%d] -> [%d/%d]",iOldLevel,iOldExp,m_iLevel,m_iExperience);
// 			}
// 		}
// 
// 		for (i=0;i<m_bf6ExtraInventorySize;i++)
// 		{
// 			cITEM		*lpItem			=	(cITEM	*)&m_aExtraInventory[i];
// 
// 			if	(!lpItem->isBreedingRecordBook())
// 				continue;
// 
// 			cPetDefine	petInfo;
// 
// 			memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
// 			petInfo.m_dwExperience		=	lpItem->m_dwSerial;
// 
// 			if	(petInfo.m_bf10Level	>	(DWORD)m_iLevel)
// 			{
// 				int	iOldLevel			=	petInfo.m_bf10Level;
// 				int	iOldExp				=	petInfo.m_dwExperience;
// 				petInfo.m_bf10Level		=	m_iLevel;
// 				petInfo.m_dwExperience	=	m_iExperience;
// 
// 				lpItem->m_dwSerial		=	petInfo.m_dwExperience;
// 				char	*lpData			=	(char *)lpItem;
// 
// 				memcpy(lpData+6,&petInfo,sizeof(cPetDefine)-4);
// 
// 				memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
// 				petInfo.m_dwExperience		=	lpItem->m_dwSerial;
// 
// 				if	(_iCorrectExp	==	0)
// 				{
// 					_lpFile->writeStreamCR("\"%s\"/\"%s\" \"%s\" [%d/%d] -%d [%d/%d] %s",m_strId,m_strName,g_strJobName[m_wJob],iOldLevel,iOldExp,_iCorrectExp,m_iLevel,m_iExperience,_lpstrWord);
// 
// 					_iCorrectExp	=	1;
// 				}
// 
// 				_lpFile->writeStreamCR("    \"pet level down\" book [%d/%d] -> [%d/%d]",iOldLevel,iOldExp,m_iLevel,m_iExperience);
// 			}
// 		}
// 	}
// 
// 	for	(i=0;i<dEQUIPMENT_PART_COUNT;i++)
// 	{
// 		cITEM	*lpItem	=	(cITEM	*)&m_aEquip[i];
// 
// 		if	(lpItem->getBasicItem()	==	NULL	||	lpItem->isExtraItem())
// 			continue;
// 
// 		int	iEquipLevel	=	lpItem->getRequireLevel();
// 
// 		if	(lpItem->getRequireLevel()	>	m_iLevel)
// 		{
// 			_lpFile->writeStream("    \"IT\" %s[lv %d] [%d:%d] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d\n",lpItem->getName(),iEquipLevel,
// 									lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
// 									lpItem->m_aOption[0],lpItem->m_aOption[1],
// 									lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
// 									lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
// 									lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
// 									lpItem->getBitFieldValue()	);
// 
// 			lpItem->m_bf1IsSpecialItem				=	TRUE;
// 
// 			cSpecialItem	*lpSPItem				=	lpItem->getSpecialItem();
// 
// 			lpSPItem->m_bf1IsReversionItem			=	TRUE;
// 			lpSPItem->m_bf1IsReversionItemWhenEquip	=	FALSE;	//	착용하면 귀속 되는 아이템
// 			lpSPItem->m_bf1IsSetItem				=	FALSE;	//	세트 아이템이다.
// 			lpSPItem->m_bf1IsUnknown				=	FALSE;	//	무슨 아이템인지 모르겠다.
// 			lpSPItem->m_bf11SValue					=	FALSE;	//	세트 아이템이나 조각 아이템 인덱스
// 			lpSPItem->m_bf10EquipLevel				=	m_iLevel;
// 		}
// 	}
// 	_lpFile->writeStreamCR(" ");

	return	TRUE;
}

cITEM*
cPLAYER_DATA::findQuestItem(int _iKind,int _iQuest,int _iTimes)
{
	int	iSlot,iCount=0;

	for (iSlot=0;iSlot<dOWN_ITEM_COUNT;iSlot++)
	{
		cITEM		*lpItem		=	(cITEM	*)&m_aItems[iSlot];
		cBASIC_ITEM	*lpBaseItem	=	lpItem->getBasicItem();

		if	(!lpBaseItem)
			continue;

		if	(lpBaseItem->m_questKind!=	_iKind	||
			lpBaseItem->m_questIndex!=	_iQuest	)
			continue;

		if	(iCount	==	_iTimes)
			return	lpItem;

		iCount++;
	}

	for (iSlot=0;iSlot<m_bf6ExtraInventorySize;iSlot++)
	{
		cITEM		*lpItem		=	(cITEM	*)&m_aExtraInventory[iSlot];
		cBASIC_ITEM	*lpBaseItem	=	lpItem->getBasicItem();

		if	(!lpBaseItem)
			continue;

		if	(lpBaseItem->m_questKind!=	_iKind	||
			lpBaseItem->m_questIndex!=	_iQuest	)
			continue;

		if	(iCount	==	_iTimes)
			return	lpItem;

		iCount++;
	}

	return	NULL;
}

tsProcessQuestField	*
cPLAYER_DATA::getQuest(int _iKind,int _iQuest)
{
	for	(int iSlot=0;iSlot<dMAX_PROCESS_QUEST_COUNT;iSlot++)
	{
		tsProcessQuestField*	lpQuestInfo		=	&m_aProcessQuest[iSlot];

		if	(lpQuestInfo->isActive	==	FALSE)
			continue;

		if	(lpQuestInfo->questKind	==	(DWORD)_iKind	&&	lpQuestInfo->questIndex	==	(DWORD)_iQuest)
			return	lpQuestInfo;
	}

	return	NULL;
}

BOOL
cPLAYER_DATA::removeQuest(int _iKind,int _iQuest)
{
	for	(int iSlot=0;iSlot<dMAX_PROCESS_QUEST_COUNT;iSlot++)
	{
		tsProcessQuestField*	lpQuestInfo		=	&m_aProcessQuest[iSlot];

		if	(lpQuestInfo->isActive	==	FALSE)
			continue;

		if	(lpQuestInfo->questKind	==	(DWORD)_iKind	&&	lpQuestInfo->questIndex	==	(DWORD)_iQuest)
		{
			lpQuestInfo->isActive	=	FALSE;
			break;
		}
	}

	while(1)
	{
		cITEM*	lpItem	=	findQuestItem(_iKind,_iQuest,0);

		if	(lpItem)
			lpItem->reset();
		else
			break;
	}

	return	NULL;
}

tsProcessQuestField	*
cPLAYER_DATA::getValidQuest()
{
	for	(int iSlot=0;iSlot<dMAX_PROCESS_QUEST_COUNT;iSlot++)
	{
		tsProcessQuestField*	lpQuestInfo		=	&m_aProcessQuest[iSlot];

		if	(lpQuestInfo->isActive	==	FALSE)
			return	lpQuestInfo;
	}

	return	NULL;
}

cITEM*
cPLAYER_DATA::findValidItemSlot()
{
	int	iSlot;

	for (iSlot=0;iSlot<getInventorySize();iSlot++)
	{
		cITEM		*lpItem		=	(cITEM	*)&m_aItems[iSlot];

		if	(lpItem->m_wBaseItem	==	0xffff)
			return	lpItem;
	}

	for (iSlot=0;iSlot<m_bf6ExtraInventorySize;iSlot++)
	{
		cITEM		*lpItem		=	(cITEM	*)&m_aExtraInventory[iSlot];

		if	(lpItem->m_wBaseItem	==	0xffff)
			return	lpItem;
	}

	return	NULL;
}

BOOL
cPLAYER_DATA::setCompleteQuestStatus(int _iKind,int _iQuest,int _iStatus)
{
	if	(_iKind	==	eQT_NORMAL)
	{
		int	iSlot	=	_iQuest/8;

		if	(iSlot	>=	dQUEST_BUFFER_SIZE)
		{
			iSlot	-=	dQUEST_BUFFER_SIZE;


			if	(_iStatus	==	1)
				m_aCompleteQuest2[iSlot]	|=	(1<<(_iQuest%8));
			else
			{
				if	(isCompleteQuest(_iKind,_iQuest))
					m_aCompleteQuest2[iSlot]	-=	(1<<(_iQuest%8));
			}
		}
		else
		{
			if	(_iStatus	==	1)
				m_aCompleteQuest[iSlot]	|=	(1<<(_iQuest%8));
			else
			{
				if	(isCompleteQuest(_iKind,_iQuest))
					m_aCompleteQuest[iSlot]	-=	(1<<(_iQuest%8));
			}
		}

		return	TRUE;
	}

	int	iSlot	=	_iQuest/8;

	if	(_iStatus	==	1)
	{
		if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
			m_aCompleteArbeit2[iSlot-dARBEIT_BUFFER_SIZE] |= (1<<(_iQuest%8));
		else
			m_aCompleteArbeit[iSlot] |= (1<<(_iQuest%8));
	}
	else
	{
		if	(isCompleteQuest(_iKind,_iQuest))
		{
			if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
				m_aCompleteArbeit2[iSlot-dARBEIT_BUFFER_SIZE] -= (1<<(_iQuest%8));
			else
				m_aCompleteArbeit[iSlot] -= (1<<(_iQuest%8));
		}

	}

	return	TRUE;
}

BOOL
cPLAYER_DATA::isCompleteQuest(int _iKind,int _iQuest)
{
	if	(_iKind	==	eQT_NORMAL)
	{
		int	iSlot	=	_iQuest/8;

		if	(iSlot	<	dQUEST_BUFFER_SIZE)
		{
			if	(m_aCompleteQuest[iSlot]&(1<<(_iQuest%8)))
				return	TRUE;
		}
		else
		{
			iSlot	-=	dQUEST_BUFFER_SIZE;

			if	(m_aCompleteQuest2[iSlot]&(1<<(_iQuest%8)))
				return	TRUE;
		}

		return	FALSE;
	}

	int	iSlot	=	_iQuest/8;

	if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
	{
		iSlot	-=	dARBEIT_BUFFER_SIZE;

		if	(m_aCompleteArbeit2[iSlot]&(1<<(_iQuest%8)))
			return	TRUE;
	}
	else
	{
		if	(m_aCompleteArbeit[iSlot]&(1<<(_iQuest%8)))
			return	TRUE;
	}

	return	FALSE;
}

void 
cPLAYER_DATA::recovery050818QuestResetUser(int _iSelectWorld)
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	SetLogFolder();

	if	(!g_db.Connect(	sList[_iSelectWorld].strDBIP	,strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source_2",1))
	{
		ERRMSG("error!",g_strToolMessage[50]);
		return;
	}

	{
		cFILE		file,fileError;

		file.Open(_ms("_text/퀘스트 리셋 유저 [%s][%d] 정보.txt",m_strId,m_wAvatarIndex),"wb",TRUE);
		fileError.Open("_text/퀘스트 리셋 유저 정보[추출 에러!!].txt","wb",TRUE);

		CAvatarManager	m_am;

		if	(!g_db.loadData(NULL,m_strName,&m_am))
		{
			fileError.writeStreamCR("find error!! : %s",m_strName);
			fileError.Close();
			return;
		}

		if	(m_am.m_iLoadDataCount	<=	0)
		{
			fileError.writeStreamCR("find error!! : %s",m_strName);
			fileError.Close();
			return;
		}

		CAvatarData	*lpPlayer	=	&m_am.m_aData[0];

		file.writeStreamCR("player	\"%s\"",lpPlayer->m_strName);

		int	iOwnQuestCount		=	0;

		for	(int iSlot=0;iSlot<dMAX_PROCESS_QUEST_COUNT;iSlot++)
		{
			tsProcessQuestField*	lpQuestInfo		=	&lpPlayer->m_aProcessQuest[iSlot];

			if	(lpQuestInfo->isActive	==	FALSE)
				continue;

			CQuestItem	*lpQuest	=	GetQuest(lpQuestInfo->questKind,lpQuestInfo->questIndex);

			if	(!lpQuest)
			{
				lpQuestInfo->isActive	=	FALSE;

				fileError.writeStreamCR("invalidQuest : %s",lpPlayer->m_strName);
				continue;
			}

			iOwnQuestCount++;

			file.writeStreamCR("	quest	\"%s\" %d,%d,%d,%d,%d,%d,%d",lpQuest->m_strName,lpQuestInfo->isActive,lpQuestInfo->questKind,lpQuestInfo->questProcessLevel,lpQuestInfo->questIndex,lpQuestInfo->questValue,lpQuestInfo->isComplete,lpQuestInfo->countdownStatus);

			int	iCount	=	0;

			while(1)
			{
				cITEM	*lpQuestItem	=	lpPlayer->findQuestItem(lpQuestInfo->questKind,lpQuestInfo->questIndex,iCount);

				iCount++;

				if	(!lpQuestItem)
					break;
				
				file.writeStreamCR("		item [%d:%d] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d",
									lpQuestItem->m_wBaseItem,lpQuestItem->m_dwSerial,lpQuestItem->m_bCount,lpQuestItem->m_bDurability,
									lpQuestItem->m_aOption[0],lpQuestItem->m_aOption[1],
									lpQuestItem->m_aPrefix[0].m_wPrefix,lpQuestItem->m_aPrefix[0].m_aValue[0],lpQuestItem->m_aPrefix[0].m_aValue[1],
									lpQuestItem->m_aPrefix[1].m_wPrefix,lpQuestItem->m_aPrefix[1].m_aValue[0],lpQuestItem->m_aPrefix[1].m_aValue[1],
									lpQuestItem->m_aPrefix[2].m_wPrefix,lpQuestItem->m_aPrefix[2].m_aValue[0],lpQuestItem->m_aPrefix[2].m_aValue[1],
									lpQuestItem->getBitFieldValue());
			}
		}

		if	(iOwnQuestCount	==	0)
			file.writeStreamCR("questCountZero");

		file.writeStreamCR(" ");
		file.writeStreamCR("end");
		file.Close();
		fileError.Close();
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		cDECODER			decoder;

		cFILE				fileError;
		BOOL				bIsSuccess	=	FALSE;
		char				strQuestName[64];
		BOOL				bResult;
		tsProcessQuestField	questInfo;
		BOOL				bIsCompleteQuest;
		char				strErrorFileName[512];

		sprintf(strErrorFileName,"_text/퀘스트 리셋 유저 [%s][%d] 처리 에러!!.txt",m_strId,m_wAvatarIndex);

		fileError.Open(strErrorFileName,"wb",TRUE);

		SetLogFolder();

		if	(!decoder.Upload(_ms("_text/퀘스트 리셋 유저 [%s][%d] 정보.txt",m_strId,m_wAvatarIndex),NULL))
		{
			ERRMSG("error","'_text/퀘스트 리셋 유저 [%s][%d] 정보.txt' 파일이 없어요",m_strId,m_wAvatarIndex);
			return;
		}

		while(1)
		{
			char	*lpstrText	=	decoder.GetWord();

			if	(stricmp(lpstrText,"end")	==	0)
			{
				bIsSuccess	=	TRUE;
				break;
			}

			if	(stricmp(lpstrText,"player")	==	0)
			{
				lpstrText			=	decoder.GetStream();
				continue;
			}

			if	(stricmp(lpstrText,"quest")	==	0)
			{
				bIsCompleteQuest	=	FALSE;
				lpstrText			=	decoder.GetStream();

				strcpy(strQuestName,lpstrText);

				questInfo.isActive			=	decoder.GetNumber(&bResult);
				questInfo.questKind			=	decoder.GetNumber(&bResult);
				questInfo.questProcessLevel	=	decoder.GetNumber(&bResult);
				questInfo.questIndex		=	decoder.GetNumber(&bResult);
				questInfo.questValue		=	decoder.GetNumber(&bResult);
				questInfo.isComplete		=	decoder.GetNumber(&bResult);
				questInfo.countdownStatus	=	decoder.GetNumber(&bResult);

				bIsCompleteQuest			=	isCompleteQuest(questInfo.questKind,questInfo.questIndex);

				if	(bIsCompleteQuest)
				{
					fileError.writeStreamCR("alreadyCompleteQuest \"%s\" \"%s\"",m_strName,strQuestName);
					continue;
				}

				tsProcessQuestField	*lpExistQuest	=	getQuest(questInfo.questKind,questInfo.questIndex);

				if	(lpExistQuest)
				{
					if	(lpExistQuest->questProcessLevel	>=	questInfo.questProcessLevel)
					{
						fileError.writeStreamCR("existSameQuest \"%s\" \"%s\"",m_strName,strQuestName);
						break;
					}

					removeQuest(questInfo.questKind,questInfo.questIndex);
				}

				tsProcessQuestField	*lpQuestSlot	=	getValidQuest();

				if	(!lpQuestSlot)
				{
					fileError.writeStreamCR("canNotFindValidQeustSlot	\"%s\" \"%s\"",m_strName,strQuestName);
					break;
				}

				memcpy(lpQuestSlot,&questInfo,sizeof(questInfo));

				continue;
			}

			if	(stricmp(lpstrText,"item")	==	0)
			{
				cITEM	item;

				item.m_wBaseItem				=	(WORD)decoder.GetNumber(&bResult);
				item.m_dwSerial					=	(DWORD)decoder.GetNumber(&bResult);
				item.m_bCount					=	(BYTE)decoder.GetNumber(&bResult);
				item.m_bDurability				=	(BYTE)decoder.GetNumber(&bResult);

				item.m_aOption[0]				=	(BYTE)decoder.GetNumber(&bResult);
				item.m_aOption[1]				=	(BYTE)decoder.GetNumber(&bResult);

				item.m_aPrefix[0].m_wPrefix		=	(WORD)decoder.GetNumber(&bResult);
				item.m_aPrefix[0].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bResult);
				item.m_aPrefix[0].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bResult);

				item.m_aPrefix[1].m_wPrefix		=	(WORD)decoder.GetNumber(&bResult);
				item.m_aPrefix[1].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bResult);
				item.m_aPrefix[1].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bResult);

				item.m_aPrefix[2].m_wPrefix		=	(WORD)decoder.GetNumber(&bResult);
				item.m_aPrefix[2].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bResult);
				item.m_aPrefix[2].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bResult);

				item.setBitFieldValue(decoder.GetNumber(&bResult));

				if	(bIsCompleteQuest)
					continue;

				cITEM	*lpValidSlot			=	findValidItemSlot();

				if	(lpValidSlot)
				{
					memcpy(lpValidSlot,&item,sizeof(item));
					continue;
				}

				fileError.writeStreamCR("inventoryFull	\"%s\" \"%s\"",m_strName,strQuestName);

				break;
			}

			if	(stricmp(lpstrText,"questCountZero")	==	0)
			{
				fileError.writeStreamCR("questCountZero	\"%s\"",m_strName);
				break;
			}
			break;
		}

		if	(bIsSuccess	==	FALSE)
			ERRMSG("error!!","문제가 있어서 제대로 처리 안됨. '%s' 파일 내용을 확인해 보세요",strErrorFileName);
		else
			cMSG::Put("작업 완료","퀘스트 복구 작업이 완료 되었습니다.");
	}
}

void
CAvatarManager::checkIncorrectItemData()
{
	int	i,iItem,iCount=0;

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		CItemDefine	*lpItem	=	&m_bank.m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		((cITEM*)lpItem)->repairItemIncorrectInfo();
	}

	for (i=0;i<m_iLoadDataCount;i++)
	{
		CAvatarData	*lpPlayer	=	&m_aData[i];

		int	iInventorySize		=	lpPlayer->getInventorySize();

		for (iItem=0;iItem<iInventorySize;iItem++)
		{
			cITEM	*lpItem		=	(cITEM	*)&lpPlayer->m_aItems[iItem];

			if	(lpItem->m_wBaseItem	==	0xffff)
				continue;

			lpItem->repairItemIncorrectInfo();
		}

		for (iItem=0;iItem<lpPlayer->m_bf6ExtraInventorySize;iItem++)
		{
			cITEM	*lpItem		=	(cITEM	*)&lpPlayer->m_aExtraInventory[iItem];

			if	(lpItem->m_wBaseItem	==	0xffff)
				continue;

			lpItem->repairItemIncorrectInfo();
		}

		for (iItem=0;iItem<dEQUIPMENT_PART_COUNT;iItem++)
		{
			cITEM	*lpItem		=	(cITEM	*)&lpPlayer->m_aEquip[iItem];

			if	(lpItem->m_wBaseItem	==	0xffff)
				continue;

			lpItem->repairItemIncorrectInfo();
		}
	}
}

cITEM*
CAvatarManager::getExistItemInBank(CItemDefine* _lpItem)
{
	if	(m_bIsInvalidBankData)
		return	NULL;

	int	i;

	for (i=dMAX_ITEM_IN_BANK;i;)
	{
		--i;

		cITEM	*lpItem	=	(cITEM*)&m_bank.m_aItems[i];

		if	(lpItem->isSameItem((cITEM*)_lpItem))	
			return	(cITEM*)lpItem;
	}

	return	NULL;
}

cITEM*
CAvatarManager::getFreeSlotInBank()
{
	if	(m_bIsInvalidBankData)
		return	NULL;

	int	i;

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		CItemDefine	*lpItem	=	&m_bank.m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)	
			return	(cITEM*)lpItem;
	}

	return	NULL;
}

cITEM*
CAvatarManager::getItem(WORD _wBaseItem,DWORD _dwSerial,char *_lpstrOwner)
{
	int	i,iItem,iEquip;

	for (i=0;i<m_iLoadDataCount;i++)
	{
		CAvatarData	*lpPlayer	=	&m_aData[i];

		int	iInventorySize		=	lpPlayer->getInventorySize();

		for (iItem=0;iItem<iInventorySize;iItem++)
		{
			if	(lpPlayer->m_aItems[iItem].m_dwSerial	== _dwSerial && lpPlayer->m_aItems[iItem].m_wBaseItem == _wBaseItem)
			{
				if	(_lpstrOwner)
					strcpy(_lpstrOwner,lpPlayer->m_strName);

				return	(cITEM *)&lpPlayer->m_aItems[iItem];
			}
		}

		for (iItem=0;iItem<lpPlayer->m_bf6ExtraInventorySize;iItem++)
		{
			if	(lpPlayer->m_aExtraInventory[iItem].m_dwSerial	== _dwSerial && lpPlayer->m_aExtraInventory[iItem].m_wBaseItem == _wBaseItem)
			{
				if	(_lpstrOwner)
					strcpy(_lpstrOwner,lpPlayer->m_strName);

				return	(cITEM *)&lpPlayer->m_aExtraInventory[iItem];
			}
		}

		for (iEquip=0;iEquip<dEQUIPMENT_PART_COUNT;iEquip++)
			if	(lpPlayer->m_aEquip[iEquip].m_dwSerial	== _dwSerial && lpPlayer->m_aEquip[iEquip].m_wBaseItem == _wBaseItem)
			{
				if	(_lpstrOwner)
					strcpy(_lpstrOwner,lpPlayer->m_strName);

				return	(cITEM *)&lpPlayer->m_aEquip[iEquip];
			}
	}

	return	NULL;
}

cITEM*
CAvatarManager::getItemInBank(WORD _wBaseItem,DWORD _dwSerial)
{
	if	(m_bIsInvalidBankData)
		return	NULL;

	int	i;

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		CItemDefine	*lpItem		=	&m_bank.m_aItems[i];

		if	(lpItem->m_wBaseItem	==	_wBaseItem && (lpItem->m_dwSerial == _dwSerial || _dwSerial == 0xffffffff))
			return	(cITEM*)lpItem;
	}

	return	NULL;
}

int				
CAvatarManager::getItemInBankCount(int _iBaseItem)
{
	int	iCount = 0;
	for (int i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		CItemDefine	*lpItem		=	&m_bank.m_aItems[i];

		if	(lpItem->m_wBaseItem	==	_iBaseItem)
			iCount	+=	lpItem->m_bCount;
	}

	return iCount;
}
