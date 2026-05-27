#include "CMessage.h"
#include "CDecoder.h"
#include "CLoadTextCode.h"
#include "CGamePlay.h"
#include "CItemEffect.h"
#include "CItemPrefix.h"
#include "CTitle.h"
#include "helpScreen.h"
#include "cGAME_TITLE.h"
#include "miniPet.h"
#include "mess_sign.H"

CLoadTextData	g_loaderText;
BOOL			CLoadTextData::s_bIsExist	=	FALSE;	//	싱글톤
char			*g_alpstrStateName[dSTATE_TYPE_COUNT];

CLoadTextData::CLoadTextData()
{
	if (s_bIsExist)
		MessageBox(GetActiveWindow(),"CLoadTextData class already defined","error",MB_OK);

	memset(this,0,sizeof(CLoadTextData));

	s_bIsExist	=	TRUE;
}

CLoadTextData::~CLoadTextData()
{
	int	i;

	if (m_iCommonMessageCount)
	{
		for (i=0;i<m_iCommonMessageCount;i++)
			pKILL(g_pstrCommonMessage[i]);

		KILL(g_pstrCommonMessage);
	}
	if (m_iEquipUseLimitContentsTextCount)
	{
		for (i=0;i<m_iEquipUseLimitContentsTextCount;i++)
			pKILL(g_pstrEquipUseLimitContents[i]);

		KILL(g_pstrEquipUseLimitContents);
	}
	if (m_iEquipmentAllignmentTextCount)
	{
		for (i=0;i<m_iEquipmentAllignmentTextCount;i++)
			pKILL(g_pstrEnableAllignment[i]);

		KILL(g_pstrEnableAllignment);
	}
	if (m_iEquipmentNameTextCount)
	{
		for (i=0;i<m_iEquipmentNameTextCount;i++)
			pKILL(g_pstrEquipmentName[i]);

		KILL(g_pstrEquipmentName);
	}
	if	(m_iGuildRankTextCount)
	{
		for (i=0;i<m_iGuildRankTextCount;i++)
			pKILL(g_pstrGuildRank					[i]);
		pKILL(g_pstrGuildRank					);
	}
	if	(m_iHeroJobTextCount)
	{
		for (i=0;i<m_iHeroJobTextCount;i++)
			pKILL(g_pstrHeroJobName				[i]);
		pKILL(g_pstrHeroJobName				);
	}
	if	(m_iCorrectMagicResistanceMessageCount)
	{
		for (i=0;i<m_iCorrectMagicResistanceMessageCount;i++)
			pKILL(g_pstrCorrectMagicResistance	[i]);
		pKILL(g_pstrCorrectMagicResistance	);
	}
	if	(m_iNpcNameTextCount)
	{
		for (i=0;i<m_iNpcNameTextCount;i++)
			pKILL(g_pstrNpcName					[i]);
		pKILL(g_pstrNpcName					);
	}
	if	(m_iRegistServerMessageCount)
	{
		for (i=0;i<m_iRegistServerMessageCount;i++)
			pKILL(g_pstrRegistServerMessage		[i]);
		pKILL(g_pstrRegistServerMessage		);
	}
	if	(m_iSlanderTextCount)
	{
		for (i=0;i<m_iSlanderTextCount;i++)
			pKILL(g_pstrSladerFilter				[i]);
		pKILL(g_pstrSladerFilter				);
	}
	if	(m_iTipCount)
	{
		for (i=0;i<m_iTipCount;i++)
			pKILL(g_pstrTip						[i]);
		pKILL(g_pstrTip						);
	}
	if	(m_iMonsterGradeCount)
	{
		for (i=0;i<m_iMonsterGradeCount;i++)
			pKILL(g_pstrMonsterGrade				[i]);
		pKILL(g_pstrMonsterGrade				);
	}
	if	(m_iMonsterTypeCount)
	{
		for (i=0;i<m_iMonsterTypeCount;i++)
			pKILL(g_pstrMonsterType				[i]);
		pKILL(g_pstrMonsterType				);
	}
	if	(m_iSummonBeastCount)
	{
		for (i=0;i<m_iSummonBeastCount;i++)
			pKILL(g_pstrSummonBeastName			[i]);
		pKILL(g_pstrSummonBeastName			);
	}
	if	(m_iNameFilterCount)
	{
		for (i=0;i<m_iNameFilterCount;i++)
			pKILL(g_pstrNameFilter				[i]);
		pKILL(g_pstrNameFilter				);
	}
	if	(m_iExtraItemGradeCount)
	{
		for (i=0;i<m_iExtraItemGradeCount;i++)
			pKILL(g_pstrExtraItemGrade			[i]);
		pKILL(g_pstrExtraItemGrade			);
	}
	if	(m_iServerCommentCount)
	{
		for (i=0;i<m_iServerCommentCount;i++)
			pKILL(g_pstrServerComment				[i]);
		pKILL(g_pstrServerComment				);
	}
	if	(m_iStatusTooltipCount)
	{
		for (i=0;i<m_iStatusTooltipCount;i++)
			pKILL(g_pstrStatusTooltip				[i]);
		pKILL(g_pstrStatusTooltip				);
	}
	if	(m_iMonsterDicCount)
	{
		for (i=0;i<m_iMonsterDicCount;i++)
			pKILL(g_pstrMonsterDicMessage				[i]);
		pKILL(g_pstrMonsterDicMessage				);
	}
	if	(m_iGreateGodGraceCount)
	{
		for (i=0;i<m_iGreateGodGraceCount;i++)
			pKILL(g_pstrGreateGodGrace				[i]);
		pKILL(g_pstrGreateGodGrace				);
	}
}

BOOL
CLoadTextData::init()
{
	BOOL	bIsSuccessful;

#ifdef	_IS_DEV_CLIENT
	if	(g_bIsDevPC	==	FALSE	||	!loadTextScriptFile())
	{
		SetLogFolder();
		bIsSuccessful	=	load();
	}
	else
		bIsSuccessful	=	save();

#else
	bIsSuccessful	=	load();
#endif

	g_lpstrBannerType[0]		=	dMSG_BANNER_BASIC_TYPE;

	g_lpstrGuildPetName[1]		=	dMSG_GUILD_PET_OBITIAN;
	g_lpstrGuildPetName[2]		=	dMSG_GUILD_PET_DASH_BLADE;
	g_lpstrGuildPetName[3]		=	dMSG_GUILD_PET_ASTRO_BOW;
	g_lpstrGuildPetName[4]		=	dMSG_GUILD_PET_CRISTAL_WATER;

	g_lpstrGuildGuardianName[1]	=	dMSG_GUILD_GUARDIAN_SHIELD_CRAFT	;
	g_lpstrGuildGuardianName[2]	=	dMSG_GUILD_GUARDIAN_ELEMENTAL_POST	;
	g_lpstrGuildGuardianName[3]	=	dMSG_GUILD_GUARDIAN_ETHER_SHELL		;
	g_lpstrGuildGuardianName[4]	=	dMSG_GUILD_GUARDIAN_HOLLOW_KNIGHT	;

	g_lpstrRedStoneReceiver[0]	=	dMSG_HEAVEN_GAE;
	g_lpstrRedStoneReceiver[1]	=	dMSG_HELL_GAE;
	g_lpstrRedStoneReceiver[2]	=	dMSG_RED_DEVIL;

	g_alpstrStateName[eSTATE_STRENGTH	]	=	dMSG_STRENGTH;
	g_alpstrStateName[eSTATE_AGILITY	]	=	dMSG_AGILITY;
	g_alpstrStateName[eSTATE_CONSTITUTION]	=	dMSG_HEALTH;
	g_alpstrStateName[eSTATE_INTELLIGENCE]	=	dMSG_INTELLIGENCE;
	g_alpstrStateName[eSTATE_WISDOM	]		=	dMSG_WISDOM;
	g_alpstrStateName[eSTATE_CHARISMA	]	=	dMSG_CHARISMA;
	g_alpstrStateName[eSTATE_LUCK	]		=	dMSG_LUCK;

	SetLogFolder();

#ifdef	_DEBUG
	char		*strNation[]	=	{"china","japan","usa","korea","thai","end"};
	
	CDecoder	decoder;

	int			iNation	=	0;

	while(1)
	{
		if	(STRICMP(strNation[iNation],"end")	==	0)
			break;

		if	(decoder.Upload(_ms("data/login server list_%s.txt",strNation[iNation]),NULL))
		{
			int		iCount	=	0;
			CGame::s_iLoginServerLocalCount	=	0;

			char	strName[40];
			char	strIP[20];

			while(1)
			{
				char	*lpstrText	=	decoder.getStream();

				if	(STRICMP(lpstrText,"end")	==	0)
					break;

				strcpy(strName,lpstrText);
				
				lpstrText	=	decoder.getStream();
				strcpy(strIP,lpstrText);

				strcpy(CGame::s_aLoginServerIPList[iCount].m_strLocalName,strName);
				strcpy(CGame::s_aLoginServerIPList[iCount].m_strIP,strIP);
				EncodeAddrBuffer(0x1eaf,(BYTE*)CGame::s_aLoginServerIPList[iCount].m_strIP,64);

				if	(iCount	==	0)
					memcpy(CGame::s_strLoginServerAddress,CGame::s_aLoginServerIPList[iCount].m_strIP,64);
				
				iCount++;

			}

			CGame::s_iLoginServerLocalCount	=	iCount;

#ifdef	_IS_DEV_CLIENT
			cFILE	file;

			file.Open(_ms("data/@@lsaf_%s.dat",strNation[iNation]),"wb");
			file.Write(&iCount,4);
			file.Write(CGame::s_aLoginServerIPList,sizeof(CGame::s_aLoginServerIPList));
			file.Close();
#endif
		}

		iNation++;
	}
#endif
	
	SetLogFolder();


	return	bIsSuccessful;
}

char **
CLoadTextData::decodeSimpleTextScriptFile(char *_lpstrFn,int *_lpiCount)
{
#ifdef	_IS_DEV_CLIENT
	char		**lpTextData;
	CDecoder	decoder;

	if (!decoder.Upload(_lpstrFn,NULL))
		return	FALSE;

	int		iMessageCount	=	0;
	char	*lpStream;

	while(1)
	{
		lpStream	=	decoder.getStream();

		if (STRICMP(lpStream,"__end") == 0)
			break;

		iMessageCount++;
	}

	lpTextData		=	new char * [iMessageCount];
	iMessageCount	=	0;

	decoder.rewind();

	while(1)
	{
		lpStream	=	decoder.getStream();

		if (STRICMP(lpStream,"__end") == 0)
			break;

		int	iStrlen	=	strlen(lpStream);

		lpTextData[iMessageCount]	=	new char [iStrlen+1];
		strcpy(lpTextData[iMessageCount],lpStream);

		iMessageCount++;
	}

	*_lpiCount	=	iMessageCount;

	return	lpTextData;
#endif

	return	NULL;
}

BOOL
CLoadTextData::decodeFontScript()
{
#ifdef	_IS_DEV_CLIENT
	CDecoder	decoder;

	if (!decoder.Upload("_fontName.txt",NULL))
		return	FALSE;

	while(1)
	{
		char	*lpstrWord	=	decoder.getWord();
		char	*lpstrStream;

		if (STRICMP(lpstrWord,"normal") == 0)
		{
			lpstrStream		=	decoder.getStream();
			strcpy(g_strFontName,lpstrStream);
			continue;
		}

		if (STRICMP(lpstrWord,"chatting") == 0)
		{
			lpstrStream		=	decoder.getStream();
			strcpy(g_strCheckFontName,lpstrStream);
			continue;
		}

		if (STRICMP(lpstrWord,"messageBoxTitle") == 0)
		{
			lpstrStream		=	decoder.getStream();
			strcpy(g_strMessageBoxTitleFontName,lpstrStream);
			continue;
		}

		if (STRICMP(lpstrWord,"streetStall") == 0)
		{
			lpstrStream		=	decoder.getStream();
			strcpy(g_strPitchmanShopTextName,lpstrStream);
			continue;
		}

		if (STRICMP(lpstrWord,"__end")	== 0)
			break;
	}

#endif
	return	TRUE;
}

BOOL
CLoadTextData::decodeDebugScript()
{
#ifdef	_IS_DEV_CLIENT
	CDecoder	decoder;
	char		*lpstrText;

	m_iDebugCommandCount	=	0;

	if (!decoder.Upload("debug.txt",NULL))
		return	FALSE;

	decoder.getWord();

	while(1)
	{
		int	iCommand	=	decoder.getNumber();

		g_aOperatorCommandInfo[m_iDebugCommandCount].m_wCommand	=	iCommand;

		if (iCommand	==	-1)
		{
			g_aOperatorCommandInfo[m_iDebugCommandCount].m_wCommand	=	0xffff;
			m_iDebugCommandCount++;
			break;
		}

		lpstrText		=	decoder.getStream();

		strcpy(g_aOperatorCommandInfo[m_iDebugCommandCount].m_strComment,lpstrText);
		
		lpstrText		=	decoder.getStream();

		strcpy(g_aOperatorCommandInfo[m_iDebugCommandCount].m_strCommand,lpstrText);

		m_iDebugCommandCount++;
	}

#endif
	return	TRUE;
}

BOOL
CLoadTextData::decodeItemExplainScript()
{
#ifdef	_IS_DEV_CLIENT

	m_iItemExplainCount	=	0;

	while(g_aItemEffectHyperText[m_iItemExplainCount].m_iSerial	!=	0xffff)
		m_iItemExplainCount++;

#endif
	return	TRUE;
}

BOOL
CLoadTextData::decodeItemPrefixScript()
{
#ifdef	_IS_DEV_CLIENT
	
	m_iItemPrefixCount	=	0;

	while(g_aPrefixHyperText[m_iItemPrefixCount].m_iSerial	!=	0xffff)
		m_iItemPrefixCount++;

#endif
	return	TRUE;
}

BOOL
CLoadTextData::decodeTitleScript()
{
#ifdef	_IS_DEV_CLIENT

	m_iTitleCount	=	0;

	while(g_aTitleHyperText[m_iTitleCount].m_iSerial	!=	0xffff)
		m_iTitleCount++;

#endif
	return	TRUE;
}

BOOL
CLoadTextData::decodeHelpScript()
{
#ifdef	_IS_DEV_CLIENT
	CDecoder	decoder;
	char		*lpstrText;

	m_iHelpCount		=	0;

	if (!decoder.Upload("innerHelp.txt",NULL))
		return	FALSE;

	while(1)
	{
		lpstrText		=	decoder.getStream();

		if (STRICMP(lpstrText,"__end")	==	0)
			break;

		strcpy(g_aHelpTextInfo[m_iHelpCount].m_strText,lpstrText);

		m_iHelpCount++;
	}

#endif
	return	TRUE;
}

BOOL
CLoadTextData::decodeEventMobScript()
{
#ifdef	_IS_DEV_CLIENT
	CDecoder	decoder;
	char		*lpstrText;
	WORD		wBody;
	m_iEventMobCount	=	0;

	if (!decoder.Upload("eventMob.txt",NULL))
		return	FALSE;

	while(1)
	{
		lpstrText	=	decoder.getStream();
		if (STRICMP(lpstrText,"__end")	==	0)
			break;

		wBody		=	decoder.getNumber();

		strcpy(g_aEventMobList[m_iEventMobCount].m_strMobName,lpstrText);
		g_aEventMobList[m_iEventMobCount].m_wBody	=	wBody;

		m_iEventMobCount++;
	}

#endif
	return	TRUE;
}

BOOL
CLoadTextData::loadTextScriptFile()
{
#ifdef	_IS_DEV_CLIENT
	SetCurrentDirectory(g_strLaunchFolder);
	SetCurrentDirectory("text");

	char	strDir[256];
	GetCurrentDirectory(256,strDir);

	if (!decodeFontScript())
		return	FALSE;

	if (!decodeDebugScript())
		return	FALSE;

	if (!decodeItemExplainScript())
		return	FALSE;

	if (!decodeItemPrefixScript())
		return	FALSE;

	if (!decodeTitleScript())
		return	FALSE;

	if (!decodeHelpScript())
		return	FALSE;

	if (!decodeEventMobScript())
		return	FALSE;

	g_pstrCommonMessage			=	decodeSimpleTextScriptFile("common.txt",&m_iCommonMessageCount);
	g_pstrEnableAllignment		=	decodeSimpleTextScriptFile("equipmentAllignment.txt",&m_iEquipmentAllignmentTextCount);
	g_pstrEquipUseLimitContents	=	decodeSimpleTextScriptFile("power.txt",&m_iEquipUseLimitContentsTextCount);
	g_pstrEquipmentName			=	decodeSimpleTextScriptFile("equipmentName.txt",&m_iEquipmentNameTextCount);
	g_pstrGuildRank				=	decodeSimpleTextScriptFile("guildRank.txt",&m_iGuildRankTextCount);
	g_pstrHeroJobName			=	decodeSimpleTextScriptFile("heroJobText.txt",&m_iHeroJobTextCount);
	g_pstrCorrectMagicResistance=	decodeSimpleTextScriptFile("magicResistance.txt",&m_iCorrectMagicResistanceMessageCount);
	g_pstrNpcName				=	decodeSimpleTextScriptFile("npcName.txt",&m_iNpcNameTextCount);
	g_pstrRegistServerMessage	=	decodeSimpleTextScriptFile("server.txt",&m_iRegistServerMessageCount);
	g_pstrSladerFilter			=	decodeSimpleTextScriptFile("slanderFilter.txt",&m_iSlanderTextCount);
	g_pstrTip					=	decodeSimpleTextScriptFile("tip.txt",&m_iTipCount);
	g_pstrMonsterGrade			=	decodeSimpleTextScriptFile("monsterGrade.txt",&m_iMonsterGradeCount);
	g_pstrMonsterType			=	decodeSimpleTextScriptFile("monsterType.txt",&m_iMonsterTypeCount);
	g_pstrSummonBeastName		=	decodeSimpleTextScriptFile("summonBeast.txt",&m_iSummonBeastCount);
	g_pstrNameFilter			=	decodeSimpleTextScriptFile("nameFilter.txt",&m_iNameFilterCount);
	g_pstrExtraItemGrade		=	decodeSimpleTextScriptFile("extraItemGrade.txt",&m_iExtraItemGradeCount);
	g_pstrServerComment			=	decodeSimpleTextScriptFile("serverComment.txt",&m_iServerCommentCount);
	g_pstrStatusTooltip			=	decodeSimpleTextScriptFile("statusToolTip.txt",&m_iStatusTooltipCount);	
//	g_pstrMonsterDicMessage		=	decodeSimpleTextScriptFile("MonsterDic.txt",&m_iMonsterDicCount);//JBC	몬스터 도감 관련 텍스트 08-07-28
	g_pstrGreateGodGrace		=	decodeSimpleTextScriptFile("GreateGodGrace.txt",&m_iGreateGodGraceCount);//JBC	거신의 은총 관련 텍스트 08-10-22

	if (!g_pstrCommonMessage			)	return	FALSE;
	if (!g_pstrEnableAllignment			)	return	FALSE;
	if (!g_pstrEquipUseLimitContents	)	return	FALSE;
	if (!g_pstrEquipmentName			)	return	FALSE;
	if (!g_pstrGuildRank				)	return	FALSE;
	if (!g_pstrHeroJobName				)	return	FALSE;
	if (!g_pstrCorrectMagicResistance	)	return	FALSE;
	if (!g_pstrNpcName					)	return	FALSE;
	if (!g_pstrRegistServerMessage		)	return	FALSE;
	if (!g_pstrSladerFilter				)	return	FALSE;
	if (!g_pstrTip						)	return	FALSE;
	if (!g_pstrMonsterGrade				)	return	FALSE;
	if (!g_pstrMonsterType				)	return	FALSE;
	if (!g_pstrSummonBeastName			)	return	FALSE;
	if (!g_pstrNameFilter				)	return	FALSE;
	if (!g_pstrExtraItemGrade			)	return	FALSE;
	if (!g_pstrServerComment			)	return	FALSE;
	if (!g_pstrStatusTooltip			)	return	FALSE;
//	if (!g_pstrMonsterDicMessage		)	return	FALSE;//JBC	몬스터 도감 관련 스크립트 08-07-28
	if (!g_pstrGreateGodGrace			)	return FALSE;

	SetMinipetTypeName();

	SetLogFolder();

	return	save();

#endif

	return	TRUE;
}

void
CLoadTextData::saveSimpleTextData(cFILE *_lpFile,char **_lpTextData,int _iCount)
{
#ifdef	_IS_DEV_CLIENT
	int		i;
	WORD	wStrlen;

	_lpFile->Write(&_iCount,4);

	for (i=0;i<_iCount;i++)
	{
		wStrlen	=	(WORD)strlen(_lpTextData[i])+1;

		_lpFile->Write(&wStrlen,2);
		_lpFile->Write(_lpTextData[i],wStrlen);
	}
#endif
}

BOOL
CLoadTextData::save()
{
#ifdef	_IS_DEV_CLIENT
	SetLogFolder();
	SetCurrentDirectory("data");

	cFILE	file;

	if (!file.Open("textData.dat","wb"))
		return	FALSE;

	saveSimpleTextData(&file,g_pstrCommonMessage,m_iCommonMessageCount);
	saveSimpleTextData(&file,g_pstrEnableAllignment,m_iEquipmentAllignmentTextCount);
	saveSimpleTextData(&file,g_pstrEquipUseLimitContents,m_iEquipUseLimitContentsTextCount);
	saveSimpleTextData(&file,g_pstrEquipmentName,m_iEquipmentNameTextCount);
	saveSimpleTextData(&file,g_pstrGuildRank,m_iGuildRankTextCount);
	saveSimpleTextData(&file,g_pstrHeroJobName,m_iHeroJobTextCount);
	saveSimpleTextData(&file,g_pstrCorrectMagicResistance,m_iCorrectMagicResistanceMessageCount);
	saveSimpleTextData(&file,g_pstrNpcName,m_iNpcNameTextCount);
	saveSimpleTextData(&file,g_pstrRegistServerMessage,m_iRegistServerMessageCount);
	saveSimpleTextData(&file,g_pstrSladerFilter,m_iSlanderTextCount);
	saveSimpleTextData(&file,g_pstrTip,m_iTipCount);
	saveSimpleTextData(&file,g_pstrMonsterGrade,m_iMonsterGradeCount);
	saveSimpleTextData(&file,g_pstrMonsterType,m_iMonsterTypeCount);
	saveSimpleTextData(&file,g_pstrSummonBeastName,m_iSummonBeastCount);
	saveSimpleTextData(&file,g_pstrExtraItemGrade,m_iExtraItemGradeCount);
	saveSimpleTextData(&file,g_pstrServerComment,m_iServerCommentCount);
	saveSimpleTextData(&file,g_pstrStatusTooltip,m_iStatusTooltipCount);
//	saveSimpleTextData(&file,g_pstrMonsterDicMessage,m_iMonsterDicCount);//JBC	몬스토 도감 관련 텍스트.	08-07-28
	saveSimpleTextData(&file,g_pstrGreateGodGrace,m_iGreateGodGraceCount);//JBC	거신의 은총 관련 텍스트.	08-10-22
	

	file.Write(g_strFontName,sizeof(g_strFontName));
	file.Write(g_strMessageBoxTitleFontName,sizeof(g_strMessageBoxTitleFontName));
	file.Write(g_strCheckFontName,sizeof(g_strCheckFontName));
	file.Write(g_strPitchmanShopTextName,sizeof(g_strPitchmanShopTextName));

	file.Write(&m_iDebugCommandCount,4);
	file.Write(g_aOperatorCommandInfo,sizeof(COperatorCommandInfo)*m_iDebugCommandCount);

	{
		for (int i=0;i<m_iItemExplainCount;i++)
			if	(g_aItemEffectHyperText[i].m_iSerial	!=	i)
				cMSG::Error("error!!","g_aItemEffectHyperText 의 배열 인덱스와 시리얼이 일치 하지 않습니다!!\n\n%s\n\n[%d/%d]",
				g_aItemEffectHyperText[i].m_strReferenceText,i,g_aItemEffectHyperText[i].m_iSerial);
	}

	file.Write(&m_iItemExplainCount,4);
	file.Write(g_aItemEffectHyperText,sizeof(CItemGenerateEffectText)*m_iItemExplainCount);

	file.Write(&m_iItemPrefixCount,4);
	file.Write(g_aPrefixHyperText,sizeof(CPrefixEffectText)*m_iItemPrefixCount);

	file.Write(&m_iTitleCount,4);
	file.Write(g_aTitleHyperText,sizeof(CTitleEffectText)*m_iTitleCount);

	file.Write(&m_iHelpCount,4);
	file.Write(g_aHelpTextInfo,sizeof(cHelpTextInfo)*m_iHelpCount);

	file.Write(&m_iEventMobCount,4);
	file.Write(g_aEventMobList,sizeof(cEventMobText)*m_iEventMobCount);


	file.Close();

	if	(!file.Open("nameFilter.dat","wb"))
		return	FALSE;

	saveSimpleTextData(&file,g_pstrNameFilter,m_iNameFilterCount);
	file.Close();

	SetLogFolder();


#endif
	return	TRUE;
}

char**
CLoadTextData::loadSimpleTextData(cFILE *_lpFile,int *_lpiCount)
{
	char	**lpTextData;
	int		i;
	WORD	wStrlen;

	_lpFile->Read(_lpiCount,4);

	lpTextData	=	new char * [*_lpiCount];

	for (i=0;i<*_lpiCount;i++)
	{
		_lpFile->Read(&wStrlen,2);

		lpTextData[i]	=	new char [wStrlen];

		_lpFile->Read(lpTextData[i],wStrlen);
	}

	return	lpTextData;
}


BOOL
CLoadTextData::load()
{
	SetLogFolder();
	SetCurrentDirectory("data");

	cFILE	file;

	char	*lpstrFileName	=	"textData.dat";
// 
// #ifdef	_FOR_JAPAN
// 	cMessSign	messSign;
// 
// 	lpstrFileName	=	messSign.makeDecodeFile(lpstrFileName);
// 
// #ifdef	_DEBUG
// 	if	(!messSign.m_iIsSuccess)
// 		Beep(1200,200);
// #endif
// 
// #endif

	if	(!file.Open(lpstrFileName,"rb"))
		return	FALSE;

	g_pstrCommonMessage			=	loadSimpleTextData(&file,&m_iCommonMessageCount);
	g_pstrEnableAllignment		=	loadSimpleTextData(&file,&m_iEquipmentAllignmentTextCount);
	g_pstrEquipUseLimitContents	=	loadSimpleTextData(&file,&m_iEquipUseLimitContentsTextCount);
	g_pstrEquipmentName			=	loadSimpleTextData(&file,&m_iEquipmentNameTextCount);
	g_pstrGuildRank				=	loadSimpleTextData(&file,&m_iGuildRankTextCount);
	g_pstrHeroJobName			=	loadSimpleTextData(&file,&m_iHeroJobTextCount);
	g_pstrCorrectMagicResistance=	loadSimpleTextData(&file,&m_iCorrectMagicResistanceMessageCount);
	g_pstrNpcName				=	loadSimpleTextData(&file,&m_iNpcNameTextCount);
	g_pstrRegistServerMessage	=	loadSimpleTextData(&file,&m_iRegistServerMessageCount);
	g_pstrSladerFilter			=	loadSimpleTextData(&file,&m_iSlanderTextCount);
	g_pstrTip					=	loadSimpleTextData(&file,&m_iTipCount);
	g_pstrMonsterGrade			=	loadSimpleTextData(&file,&m_iMonsterGradeCount);
	g_pstrMonsterType			=	loadSimpleTextData(&file,&m_iMonsterTypeCount);
	g_pstrSummonBeastName		=	loadSimpleTextData(&file,&m_iSummonBeastCount);
	g_pstrExtraItemGrade		=	loadSimpleTextData(&file,&m_iExtraItemGradeCount);
	g_pstrServerComment			=	loadSimpleTextData(&file,&m_iServerCommentCount);
	g_pstrStatusTooltip			=	loadSimpleTextData(&file,&m_iStatusTooltipCount);	
	//g_pstrMonsterDicMessage		=	loadSimpleTextData(&file,&m_iMonsterDicCount);//JBC	몬스토 도감 관련 텍스트.	08-07-28
	g_pstrGreateGodGrace		=	loadSimpleTextData(&file,&m_iGreateGodGraceCount);//JBC	거신의 은총 관련 텍스트.	08-10-22

	file.Read(g_strFontName,sizeof(g_strFontName));
	file.Read(g_strMessageBoxTitleFontName,sizeof(g_strMessageBoxTitleFontName));
	file.Read(g_strCheckFontName,sizeof(g_strCheckFontName));
	file.Read(g_strPitchmanShopTextName,sizeof(g_strPitchmanShopTextName));

// 	HANDLE		hFile;
// 	DWORD		dwRead;
// 	
// 	hFile = CreateFile("JBC Test.txt", GENERIC_WRITE, 0, NULL,
// 		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
// 	
// 	WriteFile(hFile, g_strFontName, sizeof(g_strFontName) ,&dwRead, NULL);
// 	WriteFile(hFile, g_strMessageBoxTitleFontName, sizeof(g_strMessageBoxTitleFontName) ,&dwRead, NULL);
// 	WriteFile(hFile, g_strCheckFontName, sizeof(g_strCheckFontName) ,&dwRead, NULL);
// 	WriteFile(hFile, g_strPitchmanShopTextName, sizeof(g_strPitchmanShopTextName) ,&dwRead, NULL);
// 	
// 	CloseHandle(hFile);

	file.Read(&m_iDebugCommandCount,4);
	file.Read(g_aOperatorCommandInfo,sizeof(COperatorCommandInfo)*m_iDebugCommandCount);

	for (int i=0;i<100;i++)
	{
		if (i < m_iDebugCommandCount)
			g_aOperatorCommandInfo[i].m_wCommand	=	i;
		else
			g_aOperatorCommandInfo[i].m_wCommand	=	0xffff;
	}

	g_aOperatorCommandInfo[m_iDebugCommandCount-1].m_wCommand	=	0xffff;

	file.Read(&m_iItemExplainCount,4);
	file.Read(g_aItemEffectHyperText,sizeof(CItemGenerateEffectText)*m_iItemExplainCount);

	file.Read(&m_iItemPrefixCount,4);
	file.Read(g_aPrefixHyperText,sizeof(CPrefixEffectText)*m_iItemPrefixCount);

	file.Read(&m_iTitleCount,4);
	file.Read(g_aTitleHyperText,sizeof(CTitleEffectText)*m_iTitleCount);

	file.Read(&m_iHelpCount,4);
	file.Read(g_aHelpTextInfo,sizeof(cHelpTextInfo)*m_iHelpCount);

	file.Read(&m_iEventMobCount,4);
	file.Read(g_aEventMobList,sizeof(cEventMobText)*m_iEventMobCount);

	g_aHelpTextInfo[m_iHelpCount].m_wTextOutputMethod	=	0xffff;
	
	file.Close();

	if	(!file.Open("nameFilter.dat","rb"))
		return	FALSE;

	g_pstrNameFilter			=	loadSimpleTextData(&file,&m_iNameFilterCount);

	SetMinipetTypeName();

	SetLogFolder();

	return	TRUE;
}