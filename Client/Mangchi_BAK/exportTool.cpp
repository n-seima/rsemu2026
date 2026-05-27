#include "cMAIN.H"
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "cTEXT.H"
#include "cDRAW.H"
#include "cMAPWND.H"
#include "cTOOLWND.H"
#include "cOBJECT.H"
#include "cBUILDING.H"
#include "cAREA.H"
#include "cSET.H"
#include "cMESSAGE.H"
#include "cKARMA.H"
#include "cPROJECT.H"
#include "cACTOR.H"
#include "cSKILL.H"
#include "CSetShop.H"
#include "CSetDialogList.H"
#include "CSetCustomItem.H"
#include "CTitle.H"
#include "localize.H"
#include "CLoadTextCode.h"
#include "CArea.h"
#include "secretDungeon.h"
#include "arca.H"
#include "trapSkill.H"


BOOL
cMAIN::exportItemInfo(HWND hwnd)	// 09.07.20 수정.
{
	cFILE	file;
	int		i;

	file.Open("item Info.txt","wb");
	file.writeStreamCR("번호	아이템 이름	최소 대미지	최대 대미지");
//		file.writeStreamCR("%s%-45s%-14s%-14s","번호", "아이템이름", "최소데미지", "최대데미지");
	for (i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if (g_aBasicItem[i].m_iSerial	==	0xffffffff)
			continue;

//		file.writeStreamCR("%-14d%-45s%-14d%-14d%-14d",g_aBasicItem[i].m_iSerial,g_aBasicItem[i].m_strName,g_aBasicItem[i].m_wMinDamage,g_aBasicItem[i].m_wMaxDamage,g_aBasicItem[i].m_wKind);
		file.writeStreamCR("%d	%s	%d	%d	%d",g_aBasicItem[i].m_iSerial,g_aBasicItem[i].m_strName,g_aBasicItem[i].m_wMinDamage,g_aBasicItem[i].m_wMaxDamage,g_aBasicItem[i].m_wKind);
	}

	file.Close();

	file.Open("item Prefix Info.txt","wb");
	file.writeStreamCR("인덱스	접두사1	접두사2	최소 수치	최대 수치	최소 시간(초)	최대시간(초)");
//	file.writeStreamCR("%-14s%-25s%-25s%-14s%-14s%-14s%-14s","인덱스","접두사1","접두사2","최소수치","최대수치","최소시간(초)","최대시간(초)");

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(i);

		if (lpPrefix	==	NULL)
			continue;

		file.writeStreamCR("%d	%s	%s	%d	%d	%d	%d",i,lpPrefix->m_str1stPrefix,lpPrefix->m_str2stPrefix,lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1],lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]);
//		file.writeStreamCR("%-14d%-25s%-25s%-14d%-14d%-14d%-14d",i,lpPrefix->m_str1stPrefix,lpPrefix->m_str2stPrefix,lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1],lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]);
	}

	file.Close();

	cMSG::Put("아이템 정보 추출 완료","item Info.txt\nitem Prefix Info.txt\n\n두 파일을 확인해 주세요.");

	return	TRUE;
}

BOOL
cMAIN::exportShopInfo(HWND hwnd)
{
	cPROJECT::SetMapFolder();

	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

	if	(!lpFiles)
		return	ERRMSG("error!!","can not find map list!");

	cFILE	file;
	int		i;

	file.Open(_ms("%s/shop.txt",_logFolder),"wt");

	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);

		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);

		for (int iShop=0;iShop<dMAX_SHOP;iShop++)
		{
			if	(g_lpActmap->m_shop.m_aShop[iShop].m_wSerial	==	0xffff)
				continue;

			CShop	*lpShop	=	&g_lpActmap->m_shop.m_aShop[iShop];

			file.writeStream("%s	%s	%d％	%dGold\n",lpFile->String,lpShop->m_strName,lpShop->m_wPriceFactor,lpShop->m_dwLimitPrice);

			for	(int iItem=0;iItem<lpShop->m_denItem.getMaxCount();iItem++)
			{
				CShopItem	*lpSellItem	=	lpShop->m_denItem.get(iItem);

				if	(!lpSellItem)
					continue;

				cBASIC_ITEM	*lpBaseItem	=	&g_aBasicItem[lpSellItem->m_wBaseItem];

				file.writeStream("    %s:%d\n",lpBaseItem->m_strName,lpSellItem->m_dwPrice);
			}
			file.writeStream("================================\n");
		}

		SetLogFolder();
		Draw();
		Sleep(1);
	}

	file.Close();

	KILL(lpFiles);

	return	TRUE;
}

BOOL
cMAIN::exportKarma(HWND hwnd)
{
	cFILE		file;
	int			i;
	cSTRINGS	*lpFiles;
	
	file.Open("카르마 리스트.txt","wb");

	cPROJECT::SetMapFolder();
	
	SetCurrentDirectory("expansion");
	
	lpFiles	=	cFOLDER::GetFiles("*.rmd");
	
	if	(!lpFiles)
		return	ERRMSG("error!!","can not find map list!");
	
	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);
		
		cPROJECT::SetMapFolder();
		SetCurrentDirectory("expansion");
		cMAP::Load(lpFile->String,0);
		g_aMap[0].exportKarma(&file);
		
		SetLogFolder();
		Draw();
		Sleep(1);
	}
	
	KILL(lpFiles);


	cPROJECT::SetMapFolder();

	lpFiles	=	cFOLDER::GetFiles("*.rmd");
	
	if	(!lpFiles)
		return	ERRMSG("error!!","can not find map list!");

	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);
		
		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);
		g_aMap[0].exportKarma(&file);
		
		SetLogFolder();
		Draw();
		Sleep(1);
	}
	
	KILL(lpFiles);

	return TRUE;
}


BOOL
cMAIN::exportExpReward(HWND hwnd)
{
	SetLogFolder();
	cFILE		file;
	int			i;
	cSTRINGS	*lpFiles;

	file.Open("경험치 보상 리스트.txt","wb");

	cPROJECT::SetMapFolder();

	SetCurrentDirectory("expansion");

	lpFiles	=	cFOLDER::GetFiles("*.rmd");

	if	(!lpFiles)
		return	ERRMSG("error!!","can not find map list!");

	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);

		cPROJECT::SetMapFolder();
		SetCurrentDirectory("expansion");
		cMAP::Load(lpFile->String,0);
		g_aMap[0].exportExpReward(&file);

		SetLogFolder();
		Draw();
		Sleep(1);
	}

	KILL(lpFiles);


	cPROJECT::SetMapFolder();

	lpFiles	=	cFOLDER::GetFiles("*.rmd");

	if (!lpFiles)
		return	ERRMSG("error!!","can not find map list!");

	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);

		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);

		g_aMap[0].exportExpReward(&file);

		SetLogFolder();
		Draw();
		Sleep(1);
	}

	KILL(lpFiles);

	file.Close();

	cMSG::Put("ok","경험치 보상 추출 완료");

	return	TRUE;
}


BOOL
cMAIN::exportMonsterInfo(HWND hwnd)
{
	int			i;
	char		strFileName[512];
	cFILE		file;

	SetLogFolder();

	sprintf(strFileName,"%s/monsterInfo.txt",_logFolder);

	file.Open(strFileName,"wt");

	cPROJECT::SetMapFolder();

	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

	if	(!lpFiles)
		return	ERRMSG("error!!","can not find map list!");

	file.writeString("필드번호\t필드 명\t시리얼\t이름\t직업\t이름\t최소레벨\t최대레벨\t종류\t등급\t평균 공격력\t평균 방어력\t평균 체력\t불 저항\t물 저항\t대지 저항\t바람 저항\t빛 저항\t어둠 저항\t평균 경험치\n");
//	file.writeString("%-14s│%-50s│%-5d││%-25s│%-25s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│%-14s│\n",
//		"필드번호", "필드명","이름","직업이름","최소레벨","최대레벨", "종류", "등급", "평균공격력", "평균방어력","평균체력","불저항력","물저항력","대지저항력","바람저항력","빛저항력","어둠저항력","평균경험치");
//	file.writeString("=========================================================================================================================================================================================================================================================================================================================================================\n");

	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);

		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);
	/*	
		if	(g_lpActmap->m_sCorrectMonsterFireResistance+
			g_lpActmap->m_sCorrectMonsterWaterResistance+
			g_lpActmap->m_sCorrectMonsterWindResistance+
			g_lpActmap->m_sCorrectMonsterEarthResistance+
			g_lpActmap->m_sCorrectMonsterLightResistance+
			g_lpActmap->m_sCorrectMonsterDarkResistance)			//	어둠 저항력 0% 100.00%
		{
			cMSG::Put(lpFile->String,"필드에 몬스터 마법 저항 보정이 있다");
		}
*/
		for (int j=0;j<dMAX_CHARACTER;j++)
		{
			cCHARACTER	*lpCharacter	=	&g_aMap[0].m_setCharacter.m_aCharacter[j];

			if (lpCharacter->m_wSerial	==	0xffff)
				continue;

			cJOB		*lpJob			=	&g_denJob.m_aJobList[lpCharacter->m_wBaseJob];

			if	(lpJob->m_wType			!=	1)
				continue;

			char	strIndex[8];

			strncpy(strIndex,g_aMap[0].m_strFName+1,3);

			strIndex[3]	=	NULL;

			int	iLevel	=	(lpCharacter->m_wMinLevel+lpCharacter->m_wMaxLevel)/2;
			int	iHP,iExp;

			{
				LONGLONG	llExp	=	lpJob->m_dwDeathExp;

				llExp	*=	(iLevel+4);
				llExp	*=	10;
				llExp	=	llExp*lpCharacter->m_wCorrectExp/100;
				iExp	=	(int)llExp/100;

				iExp	=	GetCorrectExpByLevelGap(iExp,iLevel);
			}

			{
				LONGLONG	llMaxHP			=	lpJob->m_iMaxHP;
				int			iConstitution	=	(int)lpJob->m_sConstitution*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

				llMaxHP	=	lpJob->m_iMaxHP+iConstitution*lpJob->m_wCorrectMaxHPConstitutionFactor*10+iLevel*lpJob->m_wCorrectMaxHPFactor*10;
				llMaxHP	=	llMaxHP*lpCharacter->m_wCorrectHP/100;
				iHP		=	(int)llMaxHP/100;
			}

			int	iWisdom,iConstitution,iStrength,iDP,iAP=0;

			if	(lpCharacter->m_bf1IsReviseIntelligence+lpJob->m_bf1IsReviseIntCalc)
				iWisdom	=	(int)lpJob->m_sWisdom*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
			else
				iWisdom	=	(int)lpJob->m_sIntelligence*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

			iConstitution	=	(int)lpJob->m_sConstitution*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
			iStrength		=	(int)lpJob->m_sStrength*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

			int	iFireResistance	=	lpJob->m_sFireResistance+iWisdom*5/100;
			int	iWaterResistance=	lpJob->m_sWaterResistance+iWisdom*5/100;
			int	iWindResistance	=	lpJob->m_sWindResistance+iWisdom*5/100;
			int	iEarthResistance=	lpJob->m_sEarthResistance+iWisdom*5/100;
			int	iLightResistance=	lpJob->m_sLightResistance+iWisdom*5/100;
			int	iDarkResistance	=	lpJob->m_sDarkResistance+iWisdom*5/100;
			

			{
				int	iWeaponMinDamage=	(int)lpJob->m_sMinDamage*100+(iLevel-1)*lpJob->m_wMinDamageLevelFactor;
				int	iWeaponMaxDamage=	(int)lpJob->m_sMaxDamage*100+(iLevel-1)*lpJob->m_wMaxDamageLevelFactor;
				int	iDamage			=	(iWeaponMinDamage+iWeaponMaxDamage)/2;
				iDamage				+=	iDamage*iStrength/200;

				iAP					=	iDamage;
			}

			{
				LONGLONG	llDP	=	(int)lpJob->m_sDefensivePower*100+(iLevel-1)*lpJob->m_wDefensePowerLevelFactor;
				llDP				=	llDP*(100+iConstitution)/100;
				llDP				+=	llDP*lpCharacter->m_wCorrectFinalDamage/10000;

				iDP		=	(int)llDP;
			}

			

//			file.writeString("%-14s│%-50s│%-5d│%-25s│%-25s│%-14d│%-14d│%-14s│%-14s│%-14.2f│%-14.2f│%-14d│%-14d│%-14d│%-14d│%-14d│%-14d│%-14d│%-14d│\n",
//				strIndex,g_lpActmap->m_strName,lpJob->m_iSerial, lpCharacter->m_strName,lpJob->m_strName,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel,
//				g_strJobCategory[lpJob->m_wKind],g_strMonsterLevel[lpJob->m_wMonsterLevel],(iAP/100.0)+(iAP%100)/100,(iDP/100.0)+(iDP%100)/100,iHP,iFireResistance,iWaterResistance,iWindResistance,iEarthResistance,iLightResistance,iDarkResistance,iExp
//				);
			file.writeString("%s\t%s\t%d\t%s\t%s\t%d\t%d\t%s\t%s\t%.2f\t%.2f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				strIndex,g_lpActmap->m_strName,lpJob->m_iSerial, lpCharacter->m_strName,lpJob->m_strName,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel,
				g_strJobCategory[lpJob->m_wKind],g_strMonsterLevel[lpJob->m_wMonsterLevel],(iAP/100.0)+(iAP%100)/100,(iDP/100.0)+(iDP%100)/100,iHP,iFireResistance,iWaterResistance,iWindResistance,iEarthResistance,iLightResistance,iDarkResistance,iExp
				);
		}
	//	file.writeString("─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");
		SetLogFolder();
		Draw();
		Sleep(1);
	}

	KILL(lpFiles);

	file.Close();

	SetLogFolder();

	cMSG::Put("export all monster Info","complete");

	return	TRUE;
}

class	cFieldInfoForDropItemInfo
{
public:
	int	m_iMonsterCount;
	int	m_aiMonsterCount[100];	//	레벨/10 당 몬스터 수
	int	m_iField;

	cFieldInfoForDropItemInfo()
	{
		memset(m_aiMonsterCount,0,sizeof(m_aiMonsterCount));
	}
};

class	cDropItemInfo
{
public:
	int		m_iCount;
	cFieldInfoForDropItemInfo	m_aFieldList[1024];
	
	cDropItemInfo()
	{
		m_iCount	=	0;
	}

	void	addField(int _iField,int _iBeginLevel,int _iEndLevel)
	{
		for (int i=0;i<m_iCount;i++)
		{
			if	(m_aFieldList[i].m_iField	==	_iField)
			{
				for (int iLevel=_iBeginLevel/10;iLevel<=_iEndLevel/10;iLevel++)
					m_aFieldList[i].m_aiMonsterCount[iLevel]++;

				m_aFieldList[m_iCount].m_iMonsterCount++;
				return;
			}
		}

		{
			for (int iLevel=_iBeginLevel/10;iLevel<=_iEndLevel/10;iLevel++)
				m_aFieldList[m_iCount].m_aiMonsterCount[iLevel]	=	1;

			m_aFieldList[m_iCount].m_iMonsterCount	=	1;
			m_aFieldList[m_iCount].m_iField			=	_iField;
		}

		m_iCount++;
	}
};

class	cFieldInfoForDropItem
{
public:
	cDropItemInfo	m_aDropItem[dCOUNT_OF_ITEM_KIND];
};

BOOL
cMAIN::exportAllMapDropItemInfo(HWND hwnd)
{
	cFieldInfoForDropItem	*pList	=	new cFieldInfoForDropItem();

	int			i;

	cPROJECT::SetMapFolder();

	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

	if	(!lpFiles)
		return	ERRMSG("error!!","can not find map list!");

	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);

		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);

		int	iFieldSerial	=	atoi(lpFile->String+1);
		
		cACTOR_SET	*lpActorList	=	&g_aMap[0].m_setActor;

		int iCount	= lpActorList->m_iCount;
		int iCnt	= 0;

		for(int j=0; j<dMAX_ACTOR_COUNT && iCnt <iCount; ++j)
		{
			cACTOR* lpActor	=	lpActorList->getActor(j);

			if	(!lpActor)
				continue;

			++iCnt;

			cJOB	*lpJob	=	lpActor->getJob();

			for (int iDI = 0; iDI<dDROPPING_ITEM_COUNT;iDI++)
			{
				cDroppingItem	*lpDropItem	=	&lpJob->m_aDroppingItem[iDI];

				if	(lpDropItem->m_wItemType	>=	dCOUNT_OF_ITEM_KIND)
					continue;

				pList->m_aDropItem[lpDropItem->m_wItemType].addField(iFieldSerial,lpActor->getCharacter()->m_wMinLevel,lpActor->getCharacter()->m_wMaxLevel);
			}
		}

		Draw();
		Sleep(1);
	}

	KILL(lpFiles);

	SetLogFolder();

	for (int iLevel=0;iLevel<100;iLevel++)
	{
		cFILE	file;
		int		iBeginLevel	=	iLevel*10+1;
		int		iEndLevel	=	(iLevel+1)*10;

		file.Open(_ms("dropItemInfo[Lv %.3d~%.3d].txt",iBeginLevel,iEndLevel),"wt");

		file.writeString("아이템 타입\t필드\n");

		for (i=0;i<eIK_END;i++)
		{
			cDropItemInfo	*lpInfo	=	&pList->m_aDropItem[i];

			file.writeString(g_strItemCategory[i]);

			if	(lpInfo->m_iCount	==	0)
			{
				file.writeString("\n");
				continue;
			}

			for (int j=0;j<lpInfo->m_iCount;j++)
			{
				cFieldInfoForDropItemInfo	*lpFieldInfo	=	&lpInfo->m_aFieldList[j];

				if	(lpFieldInfo->m_aiMonsterCount[iLevel])
					file.writeString("\t%d\t%d개",lpFieldInfo->m_iField,lpFieldInfo->m_aiMonsterCount[iLevel]);
			}

			file.writeString("\n");
			fflush(file.fp);
		}

		file.Close();
	}
	KILL(pList);

	cMSG::Put("export all monster Info","complete");

	return	TRUE;
}

BOOL
cMAIN::exportCurrentFieldNpcInfo(HWND hwnd)
{
	g_lpActmap->exportNpcInfoWithDialog();

	return	TRUE;
}

BOOL
cMAIN::exportNpcInfo(HWND hwnd)
{
	int			i;
	char		strFileName[512];
	cFILE		file;

	SetLogFolder();

	sprintf(strFileName,"%s/npcInfo.txt",_logFolder);

	file.Open(strFileName,"wb");

	cPROJECT::SetMapFolder();

	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");
	file.writeStreamCR("%s\t%s\t%s\t%s","필드번호","이름","좌표X","좌표Y");
	if	(!lpFiles)
		return	ERRMSG("error!!","can not find map liZst!");

	for (i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);

		cPROJECT::SetMapFolder();
		cMAP::Load(lpFile->String,0);
		
		for (int j=0;j<dMAX_ACTOR_COUNT;j++)
		{
			cACTOR	*lpActor	=	&g_lpActmap->m_setActor.m_aActor[j];

			if	(lpActor->m_iSerial	==	0xffff	||	lpActor->m_wType	==	0	||	lpActor->m_wType	==	2)
				continue;

			char	strIndex[8];

			strncpy(strIndex,g_aMap[0].m_strFName+1,3);

			strIndex[3]	=	NULL;

			file.writeStreamCR("%s\t%s\t%d\t%d",strIndex,lpActor->m_strName,lpActor->m_pos.x,lpActor->m_pos.y);
		}

		SetLogFolder();
		Draw();
		Sleep(1);
	}

	KILL(lpFiles);

	file.Close();

	SetLogFolder();

	cMSG::Put("export all npc Info","complete");

	return	TRUE;
}

BOOL
cMAIN::operateTemplateWork(HWND hwnd)
{
	cPROJECT::SetMapFolder();

	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

	SetLogFolder();

	if (!lpFiles)
		return	ERRMSG("error!!","can not find map list!");

	int	iWorkMapCount	=	0;

	for (int i=0;i<lpFiles->Count;i++)
	{
		cSTRING	*lpFile	=	lpFiles->Pick(i);

		char	strIndex[10];

		strncpy(strIndex,lpFile->String+1,3);
		strIndex[3]	=	NULL;
		int	iIndex	=	atoi(strIndex);

		if (!IsConvertMap(iIndex))
			continue;

		cPROJECT::SetMapFolder();

		cMAP::Load(lpFile->String,0);

//		exportMiniMapImage(hwnd,25,TRUE);

		for (int iArea=0;iArea<dMAX_AREA;iArea++)
		{
			cAREA_INFO	*lpArea		=	&g_aMap[0].m_pArea->m_aArea[iArea];;

			if (!lpArea)
				continue;

			if (lpArea->m_wKind	==	dAREA_DOOR)
			{
				cAreaDoor	*lpDoor	=	(cAreaDoor	*)lpArea;

				lpDoor->m_iDurability				=	max(3000,3000*lpDoor->m_wTrapLevel);
				lpDoor->m_iDefense					=	0;
				lpDoor->m_wUpkeepDismantlingTime	=	30;
				lpDoor->m_bf1IsTestPortal			=	FALSE;
				lpDoor->m_bf1IsDestroyable			=	TRUE;
/*문 수정 데이터
마법/물리 저항 = 0
문 파괴 가능 = O
내구력 = 3000 * 함정 레벨, 3000 중 큰값
자물쇠 해체 지속시간 = 30초*/
			}
/*
			if (lpArea->m_wKind	==	dAREA_TRAP)
			{
				cAreaTrap	*lpTrap	=	(cAreaTrap	*)lpArea;

				lpTrap->m_wTrapRefreshTime			=	30;
				lpTrap->m_bf1IsTestPortal			=	FALSE;
			}

			if (lpArea->m_wKind	==	dAREA_ARCA)
			{
				cAreaArca	*lpArca	=	(cAreaArca	*)lpArea;

				lpArca->m_bf1IsTestPortal			=	FALSE;
			}
*/
		}
//	문의 마법/물리 저항 = 0, 자물쇠 해체 지속시간 15, 테스트 중 체크 표시 해제
//	함정의 해체 지속시간 30, 테스트 중 체크 표시 해제
		char	strDest[256];

		sprintf(strDest,"../%s",g_aMap[0].m_strFName);

		cPROJECT::SaveMap(g_aMap[0].m_strFName,0,strDest);

		SetLogFolder();
		Draw();
		Sleep(1);

		iWorkMapCount++;
	}

	SetLogFolder();

	cMSG::Put("map work","complete [%d] numbers map data",iWorkMapCount);

	KILL(lpFiles);

	return	TRUE;
}


void cMAIN::exportActorKarma(){

	cFILE		file;
		

	cSTRING strFileName;
	strFileName.Reset();
	

	cPROJECT::SetMapFolder();
	strFileName.Add(g_lpActmap->m_strFName);
	strFileName.Add("ActorKarma.txt");
	file.Open(strFileName.String,"wb");
	file.writeStreamCR("파일명 : %-25s 맵이름 : %-25s", g_lpActmap->m_strFName, g_lpActmap->m_strName);


	int iCount = g_lpActmap->m_setActor.m_iCount;
	int iCnt = 0;
	
	HWND hProg = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD |WS_VISIBLE | WS_BORDER,  300,300,500,10, hWND, NULL, hINST, NULL);
	SendMessage(hProg, PBM_SETRANGE, 0, MAKELPARAM(0, iCount));
	SendMessage(hProg, PBM_SETPOS, 0, 0);

	for(int i=0;i<dMAX_ACTOR_COUNT && iCnt<iCount;++i){

		cACTOR* pAc = g_lpActmap->m_setActor.getActor(i);
	
		if(!pAc)
			continue;
		
		++iCnt;

		 if(!pAc->m_pCC) 
			 continue;

		SendMessage(hProg, PBM_SETPOS, i, 0);
		cJOB* pJ = pAc->getJob();
		char* pJobName = g_strNpcCategorty[pAc->m_wType];
		char* pToolName = NULL;
		if(pJ){
			pToolName = pJ->m_strName;
		}	
		
		file.writeStreamCR("번호 : %4d 이름 : %-15s 직업 : %-15s 등급: %-25s  좌표 : X=%3d, Y=%3d",
			pAc->m_iSerial, pAc->getName(), pJobName, pToolName,pAc->m_pos.x/dTILE_WIDTH,pAc->m_pos.y/dTILE_HEIGHT);
		file.writeStreamCR("%-25s","대사리스트");
		for(int j=0;j<pAc->m_pCC->m_wConversationCount;++j){
			
			CConversation* pCs = &pAc->m_pCC->m_pConversation[j];			
			if(!pCs) continue;
			file.writeStreamCR("\t%3d. %-25s",j+1,pCs->getTitle());
			file.writeStreamCR("\t\t%-25s","선택문리스트");
			for(int k=0;k<pCs->m_wContentsCount;++k){

				CKarmaContainer* pKc = &pCs->m_pContents[k];
				
				if(!pKc) continue;
	
				file.writeStreamCR("\t\t\t%3d. %-25s",k+1,pKc->m_pstrTitle);
				file.writeStreamCR("\t\t\t\t%-25s","처리내용");
				for(int l=0;l<pKc->getKarmaCount();++l){
					
					CKarma* pK = &pKc->m_pKarma[l];
					if(!pK) continue;
					file.writeStreamCR("\t\t\t\t%3d. %-25s",l+1  ,pK->getTitle());
					char* pChAnd = NULL;
					char* pChNot = NULL;
					if(pK->m_isWantAndCondition == TRUE){
						pChAnd = "And";
					}else{
						pChAnd = "Or";
					}
					if(pK->m_isWantNotComplete == TRUE){
						pChNot = "O";

					}else{
						pChNot = "X";
					}
						

					file.writeStreamCR("\t\t\t\t\t발동확률[%4d%%] 체크주기[%4d] 발동횟수제한[%4d] 활성트리거 주건수[%4d]",
						pK->m_wActivateChance, pK->m_wActivePeriod,pK->m_wActiveCount, pK->m_bf7ActivateTriggerCount);
					file.writeStreamCR("\t\t\t\t\t[트리거](조건[%s])",pChAnd);
					for(int m=0;m<pK->m_wTriggerCount;++m){
						CKarmaItem* pTg = &pK->m_pTrigger[m];
						if(!pTg) continue;
						
						char	*lpText		=	cKARMA_WND::KarmaTextOut(pTg,TRUE);
						file.writeStreamCR("\t\t\t\t\t%3d. %-25s",m+1,lpText);
				
					}
					file.writeStreamCR("\t\t\t\t\t[리액션](조건이 맞지않을때 발동[%s])", pChNot);	
					
					for(int n=0;n<pK->m_wReactionCount;++n){
						CKarmaItem* pRg = &pK->m_pReaction[n];
						if(!pRg) continue;
						BOOL	bSpeech		=	FALSE;
						char	*lpText		=	cKARMA_WND::KarmaTextOut(pRg,&bSpeech, FALSE);
						if(bSpeech){
							file.writeStreamCR("\t\t\t\t\t대화[%3d. %s] 으로 이동한다.",pRg->m_aValue[0]+1, pAc->m_pCC->m_pConversation[pRg->m_aValue[0]].getTitle());
						}else{
							file.writeStreamCR("\t\t\t\t\t%3d. %-25s",n+1,lpText);
						}
					}
				}
			}
		}
		file.writeStreamCR("──────────────────────────────────────────────────────────────");
	}
	DestroyWindow(hProg);
	file.Close();

}
void cMAIN::exportFieldKarma(){

	cFILE		file;
		

	cSTRING strFileName;
	strFileName.Reset();
	
	strFileName.Add(g_lpActmap->m_strFName);
	strFileName.Add("FieldKarma.txt");
	file.Open(strFileName.String,"wb");
	file.writeStreamCR("파일명 : %-25s 맵이름 : %-25s", g_lpActmap->m_strFName, g_lpActmap->m_strName);
	int iCount = g_lpActmap->m_karma.m_wKarmaCount;
	HWND hProg = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD |WS_VISIBLE | WS_BORDER,300,300,500,10, hWND, NULL, hINST, NULL);
	SendMessage(hProg, PBM_SETRANGE, 0, MAKELPARAM(0, iCount));
	SendMessage(hProg, PBM_SETPOS, 0, 0);
	for(int i=0;i<iCount;++i){
	SendMessage(hProg, PBM_SETPOS, i, 0);
		CKarma* pK = &g_lpActmap->m_karma.m_pKarma[i];
		
		if(!pK) continue;
		
		file.writeStreamCR("번호 : %3d\t이름 : %-25s", i, pK->m_pstrTitle);

		char* pChAnd = NULL;
		char* pChNot = NULL;
		if(pK->m_isWantAndCondition == TRUE){
			pChAnd = "And";
		}else{
			pChAnd = "Or";
		}
		if(pK->m_isWantNotComplete == TRUE){
			pChNot = "O";
			
		}else{
			pChNot = "X";
		}
		
		
		file.writeStreamCR("\t발동확률[%4d%%] 체크주기[%4d] 발동횟수제한[%4d] 활성트리거 주건수[%4d]",
			pK->m_wActivateChance, pK->m_wActivePeriod,pK->m_wActiveCount, pK->m_bf7ActivateTriggerCount);
		file.writeStreamCR("\t[트리거](조건[%s])",pChAnd);
		for(int m=0;m<pK->m_wTriggerCount;++m){
			CKarmaItem* pTg = &pK->m_pTrigger[m];
			if(!pTg) continue;
			
			char	*lpText		=	cKARMA_WND::KarmaTextOut(pTg,TRUE);
			file.writeStreamCR("\t\t%3d. %-25s",m+1,lpText);
			
		}
		file.writeStreamCR("\t[리액션](조건이 맞지않을때 발동[%s])", pChNot);	
		
		for(int n=0;n<pK->m_wReactionCount;++n){
			CKarmaItem* pRg = &pK->m_pReaction[n];
			if(!pRg) continue;			
			char	*lpText		=	cKARMA_WND::KarmaTextOut(pRg,FALSE);
			file.writeStreamCR("\t\t%3d. %-25s",n+1,lpText);
			
		}
		file.writeStreamCR("──────────────────────────────────────────────────────────────");
		
	}
	
	DestroyWindow(hProg);
	file.Close();

}
void cMAIN::exportMonsterKarma(){

	cFILE		file;
		

	cSTRING strFileName;
	strFileName.Reset();
	cPROJECT::SetMapFolder();
	strFileName.Add(g_lpActmap->m_strFName);
	strFileName.Add("MonsterKarma.txt");
	file.Open(strFileName.String,"wb");
	file.writeStreamCR("파일명 : %-25s 맵이름 : %-25s", g_lpActmap->m_strFName, g_lpActmap->m_strName);
	
	int iCount	= g_lpActmap->m_setCharacter.m_iCount;
	int iCnt	= 0;
	HWND hProg = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD |WS_VISIBLE | WS_BORDER, 300,300,500,10, hWND, NULL, hINST, NULL);
	SendMessage(hProg, PBM_SETRANGE, 0, MAKELPARAM(0, iCount));
	SendMessage(hProg, PBM_SETPOS, 0, 0);




	for(int i=0;i<dMAX_ACTOR_COUNT && iCnt < iCount;++i){
		cCHARACTER* pCs = g_lpActmap->m_setCharacter.getCharacter(i);
		SendMessage(hProg, PBM_SETPOS, i, 0);
		
		if(!pCs)
			continue;
		++iCnt;
		if(!pCs->isExistEvent())
			continue;


		char* pToolName = NULL;
		cJOB* pJ = pCs->getJob();
		if(pJ){
			pToolName = pJ->m_strName;
		}	
		
		
		file.writeStreamCR("번호 : %4d 이름 : %-15s 등급 : %-25s  ", pCs->m_wSerial, pCs->m_strName, pToolName);
		file.writeStreamCR("\t이벤트");
		for(int j=0;j<4;++j){

			CKarmaContainer* pKc = pCs->m_pEvent[j];
			if(!pKc) continue;
			
			file.writeStreamCR("\t%-25s",pKc->m_pstrTitle);

			for(int k=0;k<pKc->m_wKarmaCount;++k){
				
				CKarma* pK = &pKc->m_pKarma[k];
				
				if(!pK) continue;
				
				
				
				char* pChAnd = NULL;
				char* pChNot = NULL;
				if(pK->m_isWantAndCondition == TRUE){
					pChAnd = "And";
				}else{
					pChAnd = "Or";
				}
				if(pK->m_isWantNotComplete == TRUE){
					pChNot = "O";
					
				}else{
					pChNot = "X";
				}
				
				file.writeStreamCR("\t%3d.%-25s",pK->m_wSerial, pK->getTitle());
				file.writeStreamCR("\t\t발동확률[%4d%%] 체크주기[%4d] 발동횟수제한[%4d] 활성트리거 주건수[%4d]",
					pK->m_wActivateChance, pK->m_wActivePeriod,pK->m_wActiveCount, pK->m_bf7ActivateTriggerCount);
				file.writeStreamCR("\t\t[트리거](조건[%s])",pChAnd);
				for(int m=0;m<pK->m_wTriggerCount;++m){
					CKarmaItem* pTg = &pK->m_pTrigger[m];
					if(!pTg) continue;
					
					char	*lpText		=	cKARMA_WND::KarmaTextOut(pTg,TRUE);
					file.writeStreamCR("\t\t%3d. %-25s",m+1,lpText);
					
				}
				file.writeStreamCR("\t\t[리액션](조건이 맞지않을때 발동[%s])", pChNot);	
			
			
				for(int n=0;n<pK->m_wReactionCount;++n){
					CKarmaItem* pRg = &pK->m_pReaction[n];
					if(!pRg) continue;			
					char	*lpText		=	cKARMA_WND::KarmaTextOut(pRg,FALSE);
					file.writeStreamCR("\t\t%3d. %-25s",n+1,lpText);
					
				}
				
				
				
			}
			

		}
		file.writeStreamCR("──────────────────────────────────────────────────────────────");
	
	}
	
	DestroyWindow(hProg);
	file.Close();

}
void cMAIN::exportAreaKarma(){

	cFILE		file;
		

	cSTRING strFileName;
	strFileName.Reset();
	
	strFileName.Add(g_lpActmap->m_strFName);
	strFileName.Add("AreaKarma.txt");
	file.Open(strFileName.String,"wb");
	file.writeStreamCR("파일명 : %-25s 맵이름 : %-25s", g_lpActmap->m_strFName, g_lpActmap->m_strName);
	int iCount = g_lpActmap->m_pArea->m_iCount;
	HWND hProg = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD |WS_VISIBLE | WS_BORDER, 300,300,500,10, hWND, NULL, hINST, NULL);
	SendMessage(hProg, PBM_SETRANGE, 0, MAKELPARAM(0, iCount));
	SendMessage(hProg, PBM_SETPOS, 0, 0);
	for(int i=0;i<iCount;++i){
		cAREA_INFO* pAi = &g_lpActmap->m_pArea->m_aArea[i];
		if(!pAi) continue; 
		SendMessage(hProg, PBM_SETPOS, i, 0);
		file.writeStreamCR("번호 : %3d. 이름 : %-25s", pAi->m_wSerial, pAi->m_strName);
		WORD wKind = pAi->m_wKind;
		
		switch(wKind)
		{
		case dAREA_NORMAL :
			file.writeStreamCR("\t영역 : 일반 영역");
			break;
		case dAREA_NOTICE_BOARD	:
			file.writeStreamCR("\t영역 : 게시판");
			file.writeStreamCR("\t내용 : %s",pAi->m_pString);
			break;
		case	dAREA_DOOR :
			{
				file.writeStreamCR("\t영역 : 문");
				cAreaDoor *lpDoor =	(cAreaDoor	*)pAi;
				lpDoor->m_bDetectTrapLevel;
				file.writeStreamCR("\t생성빈도         : %5d\t\t\t\t생성확률             : %5d",lpDoor->m_wRegenPeriod,lpDoor->m_wGenChance);
				file.writeStreamCR("\t유지시간         : %5d\t\t\t\t비밀레벨             : %5d",lpDoor->m_wUpkeepPeriod,lpDoor->m_bSecretLevel);
				file.writeStreamCR("\t내구력           : %5d\t\t\t\t파괴 가능 최소 레벨  : %5d",lpDoor->m_iDurability,lpDoor->m_wDestroyMinimumLevel);
				file.writeStreamCR("\t마법/물리 저항   : %5d",lpDoor->m_iDefense);
				file.writeStreamCR("\t좌물쇠레벨       : %5d\t\t\t\t좌물쇠 해제 지속시간 : %5d",lpDoor->m_bUnlockLevel,lpDoor->m_wUpkeepDismantlingTime);
				file.writeStreamCR("\t문파괴가능[%s]", ((lpDoor->m_bf1IsDestroyable == 1) ? "O" : "X" ));
				file.writeStreamCR("\t함정레벨         : %5d\t\t\t\t함정타겟             : %s",lpDoor->m_wTrapLevel,g_strTrapTarget[lpDoor->m_wTrapTarget]);
				file.writeStreamCR("\t함정탐지레벨     : %5d\t\t\t\t함정해제레벨         : %5d",lpDoor->m_bDetectTrapLevel,lpDoor->m_bDisarmTrapLevel);
				file.writeStreamCR("\t함정스킬1        : %-25s\t\t발동확률              : %3d%%",g_trapSkillManager.m_aTrapSkill[lpDoor->m_wTrapSkill1].m_strName,lpDoor->m_wTrapSkill1ActiveChance);
				file.writeStreamCR("\t함정스킬2        : %-25s\t\t발동확률              : %3d%%",g_trapSkillManager.m_aTrapSkill[lpDoor->m_wTrapSkill2].m_strName,lpDoor->m_wTrapSkill2ActiveChance);
				cSecretDungeon	*lpSD	=	g_sdManager.get(lpDoor->m_wLinkSecretDungeon);
				file.writeStreamCR("\t연결된 비밀던전  : %s",lpSD->m_strName);
				
				
			}
			break;
		case dAREA_PORTAL :
			file.writeStreamCR("\t영역 : 출입구");
			file.writeStreamCR("\tShape : %s", g_strPortalShape[pAi->m_gateShape]);
			file.writeStreamCR("\t출입구 Direct : %s", g_strPortalDirect[pAi->m_gateDirect]);
			file.writeStreamCR("\t연결된 장소 : %s", pAi->m_strMoveGateName);
			file.writeStreamCR("\t불러들일 파일 : %s", pAi->m_pString);
			
			break;
		case dAREA_MONSTER_ACTIVE :
			file.writeStreamCR("\t영역 : 몬스터 활동 영역");
			break;
		case dAREA_START_AREA :
			file.writeStreamCR("\t영역 : 시작 영역");
			break;
		case dAREA_NAMED_AREA :
			file.writeStreamCR("\t영역 : 이름 표시 영역");
			break;
		case dAREA_GUILD_BATTLE	:
			file.writeStreamCR("\t영역 : 길드전 영역");
			break;
		case dAREA_OX_AREA_O :
			file.writeStreamCR("\t영역 : OX 퀴즈 영역(O)");
			break;
		case dAREA_OX_AREA_X :
			file.writeStreamCR("\t영역 : OX 퀴즈 영역(X)");
			break;
		case dAREA_REVIVE_AREA :
			file.writeStreamCR("\t영역 : 부활 장소");
			break;
		case dAREA_HUNTING_AREA :
			file.writeStreamCR("\t영역 : 사냥 영역");
			break;
		case dAREA_SECRET_DUNGEON_EXIT :
			file.writeStreamCR("\t영역 : 비던 출구");
			break;
			
		case dAREA_TRAP	:
			{
				file.writeStreamCR("\t영역 : 함정");
				cAreaTrap	*lpTrap			=	(cAreaTrap	*)pAi;
				file.writeStreamCR("\t레벨      : %5d\t\t\t\t함정해제 지속시간 : %d", lpTrap->m_wTrapLevel, lpTrap->m_wTrapRefreshTime);
				file.writeStreamCR("\t탐지레벨  : %5d\t\t\t\t해제레벨          : %d", lpTrap->m_wDetectTrapLevel, lpTrap->m_wDisarmTrapLevel);
				file.writeStreamCR("\t함정스킬1 : %-25s\t\t발동확률            : %3d%%",g_trapSkillManager.m_aTrapSkill[lpTrap->m_wTrapSkill1].m_strName,lpTrap->m_wTrap1ActiveChance);
				file.writeStreamCR("\t함정스킬2 : %-25s\t\t발동확률            : %3d%%",g_trapSkillManager.m_aTrapSkill[lpTrap->m_wTrapSkill2].m_strName,lpTrap->m_wTrap2ActiveChance);
				file.writeStreamCR("\t함정타겟  : %s",g_strTrapTarget[lpTrap->m_wTrapTarget]);
			}
			break;
		case dAREA_EVENT_AREA :
			{
				file.writeStreamCR("\t영역 : 이벤트 영역");
				int iCount = pAi->m_cc.m_wConversationCount;
				for(int j=0;j<iCount;++j){
					
					CConversation* pCs = &pAi->m_cc.m_pConversation[j];			
					if(!pCs) continue;
					file.writeStreamCR("\t%3d. %-25s",j+1,pCs->getTitle());
					file.writeStreamCR("\t\t%-25s","선택문리스트");
					for(int k=0;k<pCs->m_wContentsCount;++k){
						
						CKarmaContainer* pKc = &pCs->m_pContents[k];
						
						if(!pKc) continue;
						
						file.writeStreamCR("\t\t\t%3d. %-25s",k+1,pKc->m_pstrTitle);
						file.writeStreamCR("\t\t\t\t%-25s","처리내용");
					
						for(int l=0;l<pKc->getKarmaCount();++l){
							
							CKarma* pK = &pKc->m_pKarma[l];
							if(!pK) continue;
							file.writeStreamCR("\t\t\t\t%3d. %-25s",l+1  ,pK->getTitle());
							char* pChAnd = NULL;
							char* pChNot = NULL;
							if(pK->m_isWantAndCondition == TRUE){
								pChAnd = "And";
							}else{
								pChAnd = "Or";
							}
							if(pK->m_isWantNotComplete == TRUE){
								pChNot = "O";
								
							}else{
								pChNot = "X";
							}
							
							
							file.writeStreamCR("\t\t\t\t\t발동확률[%4d%%] 체크주기[%4d] 발동횟수제한[%4d] 활성트리거 주건수[%4d]",
								pK->m_wActivateChance, pK->m_wActivePeriod,pK->m_wActiveCount, pK->m_bf7ActivateTriggerCount);
							file.writeStreamCR("\t\t\t\t\t[트리거](조건[%s])",pChAnd);
							for(int m=0;m<pK->m_wTriggerCount;++m){
								CKarmaItem* pTg = &pK->m_pTrigger[m];
								if(!pTg) continue;
								
								char	*lpText		=	cKARMA_WND::KarmaTextOut(pTg,TRUE);
								file.writeStreamCR("\t\t\t\t\t%3d. %s",m+1,lpText);
								
							}
							file.writeStreamCR("\t\t\t\t\t[리액션](조건이 맞지않을때 발동[%s])", pChNot);	
							
							for(int n=0;n<pK->m_wReactionCount;++n){
								CKarmaItem* pRg = &pK->m_pReaction[n];
								if(!pRg) continue;
								BOOL	bSpeech		=	FALSE;
								char	*lpText		=	cKARMA_WND::KarmaTextOut(pRg,&bSpeech, FALSE);
								if(bSpeech){
									file.writeStreamCR("\t\t\t\t\t대화[%3d. %s] 으로 이동한다.",pRg->m_aValue[0]+1, pAi->m_cc.m_pConversation[pRg->m_aValue[0]].getTitle());
								}else{
									file.writeStreamCR("\t\t\t\t\t%3d. %s",n+1,lpText);
								}
							}
						}
						
					}
					
				}
			}
			break;
		case dAREA_ARCA	:
			{
			cAreaArca	*lpAreaArca	=	(cAreaArca	*)pAi;
			cArcaInfo	*lpArca		=	g_arcaManager.get(lpAreaArca->m_wLinkArca);
			file.writeStreamCR("\t영역 : 보물상자");
			file.writeStreamCR("\t보물상자 이름 : %s",lpArca->m_strName);
			}
			break;
		case dAREA_GUILD_CREST :
			file.writeStreamCR("\t영역 : 길드 문장");
			break;
		}
	
	file.writeStreamCR("──────────────────────────────────────────────────────────────");


	}

	DestroyWindow(hProg);


	file.Close();
}
