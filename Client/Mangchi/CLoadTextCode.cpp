#include	"CMessage.h"
#include	"CLoadTextCode.h"
#include	"CTitle.h"
#include	"CItem.h"
#include	"commonClass.h"

char	**g_pstrCommonMessage;
char	**g_pstrRegistServerMessage;
char	**g_pstrSladerFilter;
char	**g_pstrNameFilter;
char	**g_pstrCorrectMagicResistance;
char	**g_pstrTip;
char	**g_pstrEnableAllignment;	//	착용/사용 가능한 성향
char	**g_pstrEquipUseLimitContents;
char	**g_pstrEquipmentName;	//	장비 이름
char	**g_pstrGuildRank;
char	**g_pstrHeroJobName;
char	**g_pstrNpcName;
char	**g_pstrMonsterGrade;
char	**g_pstrMonsterType;
char	**g_pstrSummonBeastName;
char	**g_pstrExtraItemGrade;
char	**g_pstrServerComment;
char	**g_pstrStatusTooltip;
char	**g_pstrMonsterDic;
char	**g_pstrGGGWord;

char	g_strFontName[64];
char	g_strMessageBoxTitleFontName[64];
char	g_strCheckFontName[64];
char	g_strPitchmanShopTextName[64];

CLoadTextData	g_loaderText;
BOOL			CLoadTextData::s_bIsExist	=	FALSE;	//	싱글톤

class	COperatorCommandInfo
{
public:
	WORD	m_wCommand;
	char	m_strComment[64];
	char	m_strCommand[20];
};

COperatorCommandInfo		g_aOperatorCommandInfo[100];

CLoadTextData::CLoadTextData()
{
	if (s_bIsExist)
		MessageBox(GetActiveWindow(),"CLoadTextData class already defined","error",MB_OK);

	memset(this,0,sizeof(CLoadTextData));

	s_bIsExist	=	TRUE;
}

CLoadTextData::~CLoadTextData()
{
	close();
}

void
CLoadTextData::close()
{
	int	i;

	if (m_iCommonMessageCount)
	{
		for (i=0;i<m_iCommonMessageCount;i++)
			pKILL(g_pstrCommonMessage[i]);

		KILL(g_pstrCommonMessage);
	}
}

BOOL
CLoadTextData::run(char *_lpstrFodler)
{
	if	(!loadTextScriptFile(_lpstrFodler))
	{
		SetLogFolder();

		return	ERRMSG("error","in localize common text");
	}

	SetLogFolder();

	return	TRUE;
}

char **
CLoadTextData::decodeSimpleTextScriptFile2(char *_lpstrFn,int *_lpiCount)
{
	char		**lpTextData;

	if	(!m_decoder.Upload(_lpstrFn,NULL))
		return	FALSE;

	int			iMessageCount	=	0;
	char		*lpStream;

	while(1)
	{
		int	iIndex		=	m_decoder.GetNumber();

		if	(iIndex		==	0x7fffffff)
			break;

		m_decoder.GetNumber();

		lpStream	=	m_decoder.GetStream();

		iMessageCount++;
	}

	lpTextData		=	new char * [iMessageCount];
	iMessageCount	=	0;

	m_decoder.rewind();

	while(1)
	{
		int	iIndex		=	m_decoder.GetNumber();

		if	(iIndex		==	0x7fffffff)
			break;

		m_decoder.GetNumber();

		lpStream	=	m_decoder.GetStream();

		int	iStrlen	=	strlen(lpStream);

		lpTextData[iMessageCount]	=	new char [iStrlen+1];
		strcpy(lpTextData[iMessageCount],lpStream);

		iMessageCount++;
	}

	*_lpiCount	=	iMessageCount;

	return	lpTextData;
}

char **
CLoadTextData::decodeSimpleTextScriptFile(char *_lpstrFn,int *_lpiCount)
{
	char		**lpTextData;

	if (!m_decoder.Upload(_lpstrFn,NULL))
		return	FALSE;

	int			iMessageCount	=	0;
	char		*lpStream;

	while(1)
	{
		lpStream	=	m_decoder.GetStream();

		if (stricmp(lpStream,"__end") == 0)
			break;

		iMessageCount++;
	}

	lpTextData		=	new char * [iMessageCount];
	iMessageCount	=	0;

	m_decoder.rewind();

	while(1)
	{
		lpStream	=	m_decoder.GetStream();

		if (stricmp(lpStream,"__end") == 0)
			break;

		int	iStrlen	=	strlen(lpStream);

		lpTextData[iMessageCount]	=	new char [iStrlen+1];
		strcpy(lpTextData[iMessageCount],lpStream);

		iMessageCount++;
	}

	*_lpiCount	=	iMessageCount;

	return	lpTextData;
}
/*
char **
CLoadTextData::decodeSimpleTextScriptFile(char *_lpstrFn,int *_lpiCount)
{
	char		**lpTextData;

	if (!m_decoder.Upload(_lpstrFn,NULL))
		return	FALSE;

	int			iMessageCount	=	0;
	char		*lpStream;

	while(1)
	{
		lpStream	=	m_decoder.GetStream();

		if (stricmp(lpStream,"__end") == 0)
			break;

		iMessageCount++;
	}

	lpTextData		=	new char * [iMessageCount];
	iMessageCount	=	0;

	m_decoder.rewind();

	while(1)
	{
		lpStream	=	m_decoder.GetStream();

		if (stricmp(lpStream,"__end") == 0)
			break;

		int	iStrlen	=	strlen(lpStream);

		lpTextData[iMessageCount]	=	new char [iStrlen+1];
		strcpy(lpTextData[iMessageCount],lpStream);

		iMessageCount++;
	}

	*_lpiCount	=	iMessageCount;

	return	lpTextData;
}
*/
BOOL
CLoadTextData::decodeFontScript()
{
	if	(!m_decoder.Upload("_fontName.txt",NULL))
	{
		m_decoder.setForcedJapan(FALSE);
		m_decoder.setForcedThai(FALSE);
		return	FALSE;
	}

	while(1)
	{
		char	*lpstrWord	=	m_decoder.GetWord();
		char	*lpstrStream;

		if (stricmp(lpstrWord,"normal") == 0)
		{
			lpstrStream		=	m_decoder.GetStream();
			strcpy(g_strFontName,lpstrStream);
			continue;
		}

		if (stricmp(lpstrWord,"chatting") == 0)
		{
			lpstrStream		=	m_decoder.GetStream();
			strcpy(g_strCheckFontName,lpstrStream);
			continue;
		}

		if (stricmp(lpstrWord,"messageBoxTitle") == 0)
		{
			lpstrStream		=	m_decoder.GetStream();
			strcpy(g_strMessageBoxTitleFontName,lpstrStream);
			continue;
		}

		if (stricmp(lpstrWord,"streetStall") == 0)
		{
			lpstrStream		=	m_decoder.GetStream();
			strcpy(g_strPitchmanShopTextName,lpstrStream);
			continue;
		}

		if (stricmp(lpstrWord,"__end")	== 0)
			break;
	}

	return	TRUE;
}

BOOL
CLoadTextData::decodeDebugScript()
{
	char		*lpstrText;

	m_iDebugCommandCount	=	0;

	if (!m_decoder.Upload("debug.txt",NULL))
		return	FALSE;

	m_decoder.GetWord();

	while(1)
	{
		int	iCommand	=	m_decoder.GetNumber();

		g_aOperatorCommandInfo[m_iDebugCommandCount].m_wCommand	=	iCommand;

		if (iCommand	==	-1)
		{
			g_aOperatorCommandInfo[m_iDebugCommandCount].m_wCommand	=	0xffff;
			m_iDebugCommandCount++;
			break;
		}

		lpstrText		=	m_decoder.GetStream();

		strcpy(g_aOperatorCommandInfo[m_iDebugCommandCount].m_strComment,lpstrText);
		
		lpstrText		=	m_decoder.GetStream();

		strcpy(g_aOperatorCommandInfo[m_iDebugCommandCount].m_strCommand,lpstrText);

		m_iDebugCommandCount++;
	}

	return	TRUE;
}

BOOL
CLoadTextData::decodeItemExplainScript()
{
	char		*lpstrText;

	m_iItemExplainCount	=	0;

	if (!m_decoder.Upload("itemExplain.txt",NULL))
		return	FALSE;

	while(1)
	{
		int	iIndex		=	m_decoder.GetNumber();

		if	(iIndex		==	0x7fffffff)
			break;

		m_decoder.GetNumber();

		lpstrText		=	m_decoder.GetStream();

		if (stricmp(lpstrText,"__end")	==	0)
			break;

		strcpy(g_aItemEffectHyperText[iIndex].m_strReferenceText,lpstrText);

		m_iItemExplainCount++;
	}

	return	TRUE;
}


BOOL
CLoadTextData::decodeItemPrefixScript()
{
	char		*lpstrText;
	
	m_iItemPrefixCount	=	0;

	if (!m_decoder.Upload("itemPrefix.txt",NULL))
		return	FALSE;

	while(1)
	{
		int	iIndex		=	m_decoder.GetNumber();

		if	(iIndex		==	0x7fffffff)
			break;

		m_decoder.GetNumber();

		lpstrText		=	m_decoder.GetStream();

		if (stricmp(lpstrText,"__end")	==	0)
			break;
		
		strcpy(g_aPrefixHyperText[iIndex].m_strReferenceText,lpstrText);

		m_iItemPrefixCount++;
	}

	return	TRUE;
}

BOOL
CLoadTextData::decodeTitleScript()
{
	char		*lpstrText;

	m_iTitleCount	=	0;

	if (!m_decoder.Upload("title.txt",NULL))
		return	FALSE;

	while(1)
	{
		int	iIndex		=	m_decoder.GetNumber();

		if	(iIndex		==	0x7fffffff)
			break;

		m_decoder.GetNumber();

		lpstrText		=	m_decoder.GetStream();

		if (stricmp(lpstrText,"__end")	==	0)
			break;

		strcpy(g_aTitleHyperText[iIndex].m_strReferenceText,lpstrText);

		m_iTitleCount++;
	}

	return	TRUE;
}
BOOL
CLoadTextData::decodeHelpScript()
{
	char		*lpstrText;

	m_iHelpCount		=	0;

	if (!m_decoder.Upload("innerHelp.txt",NULL))
		return	FALSE;

	while(1)
	{
		lpstrText		=	m_decoder.GetStream();

		if (stricmp(lpstrText,"__end")	==	0)
			break;

		strcpy(g_aHelpTextInfo[m_iHelpCount].m_strText,lpstrText);

		m_iHelpCount++;
	}

	return	TRUE;
}

BOOL
CLoadTextData::decodeEventMobScript()
{
	char		*lpstrText;
	WORD		wBody;

	m_iEventMobCount	=	0;

	if (!m_decoder.Upload("eventMob.txt",NULL))
		return	FALSE;

	while(1)
	{
		lpstrText	=	m_decoder.GetStream();

		if (stricmp(lpstrText,"__end")	==	0)
			break;

		wBody		=	m_decoder.GetNumber();

		strcpy(g_aEventMobList[m_iEventMobCount].m_strMobName,lpstrText);
		g_aEventMobList[m_iEventMobCount].m_wBody	=	wBody;

		m_iEventMobCount++;
	}

	return	TRUE;
}

BOOL
CLoadTextData::loadTextScriptFile(char *_lpstrFodler)
{
	if	(strstr(_lpstrFodler,"japan"))
	{
		if	(strstr(_lpstrFodler,"lastest")==NULL)
			m_decoder.setForcedJapan(TRUE);
	}

	if	(strstr(_lpstrFodler,"thai"))
	{
		if	(strstr(_lpstrFodler,"lastest")==NULL)
			m_decoder.setForcedThai(TRUE);
	}

	SetLogFolder();
	SetCurrentDirectory(_ms("localize/%s/import",_lpstrFodler));

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

	g_pstrCommonMessage			=	decodeSimpleTextScriptFile2("common.txt",&m_iCommonMessageCount);
	g_pstrEnableAllignment		=	decodeSimpleTextScriptFile("equipmentAllignment.txt",&m_iEquipmentAllignmentTextCount);
	g_pstrEquipUseLimitContents	=	decodeSimpleTextScriptFile("power.txt",&m_iEquipUseLimitContentsTextCount);
	g_pstrEquipmentName			=	decodeSimpleTextScriptFile("equipmentName.txt",&m_iEquipmentNameTextCount);
	g_pstrGuildRank				=	decodeSimpleTextScriptFile("guildRank.txt",&m_iGuildRankTextCount);
	g_pstrHeroJobName			=	decodeSimpleTextScriptFile("heroJobText.txt",&m_iHeroJobTextCount);
	g_pstrCorrectMagicResistance=	decodeSimpleTextScriptFile("magicResistance.txt",&m_iCorrectMagicResistanceMessageCount);
	g_pstrNpcName				=	decodeSimpleTextScriptFile2("npcName.txt",&m_iNpcNameTextCount);
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
	//g_pstrMonsterDic			=	decodeSimpleTextScriptFile("MonsterDic.txt",&m_iMonsterDicCount);
	g_pstrGGGWord				=	decodeSimpleTextScriptFile2("GreateGodGrace.txt",&m_iGGGWordCount);

	m_decoder.setForcedJapan(FALSE);
	m_decoder.setForcedThai(FALSE);

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
//	if (!g_pstrMonsterDic				)	return	FALSE;
	if (!g_pstrGGGWord					)	return	FALSE;

	SetLogFolder();

	return	save(_lpstrFodler);
}

BOOL
CLoadTextData::exportText(char *_lpstrFodler)
{
	load();
	SetLogFolder();
	SetCurrentDirectory(_lpstrFodler);

	char	strDir[256];
	GetCurrentDirectory(256,strDir);

	int		i;

	cFILE	file;

	{
		file.Open("itemExplain.txt","wb");

		for (i=0;i<m_iItemExplainCount;i++)
			file.writeStreamCR("%3d  [%d] \"%s\" \n",g_aItemEffectHyperText[i].m_iSerial,GetTextCheckSum(g_aItemEffectHyperText[i].m_strReferenceText),g_aItemEffectHyperText[i].m_strReferenceText);

		file.Close();
	}

	{
		file.Open("itemPrefix.txt","wb");

		for (i=0;i<m_iItemPrefixCount;i++)
			file.writeStreamCR("%3d  [%d] \"%s\" \n",i,GetTextCheckSum(g_aPrefixHyperText[i].m_strReferenceText),g_aPrefixHyperText[i].m_strReferenceText);

		file.Close();
	}

	{
		file.Open("title.txt","wb");

		for (i=0;i<m_iTitleCount;i++)
			file.writeStreamCR("%3d  [%d] \"%s\" \n",i,GetTextCheckSum(g_aTitleHyperText[i].m_strReferenceText),g_aTitleHyperText[i].m_strReferenceText);

		file.Close();
	}

	{
		file.Open("common.txt","wb");

		for (i=0;i<m_iCommonMessageCount;i++)
			file.writeStreamCR("%3d  [%d] \"%s\" \n",i,GetTextCheckSum(g_pstrCommonMessage[i]),g_pstrCommonMessage[i]);

		file.Close();
	}

	{
		file.Open("GreateGodGrace.txt","wb");
		
		for (i=0;i<m_iGGGWordCount;i++)
			file.writeStreamCR("%3d  [%d] \"%s\" \n",i,GetTextCheckSum(g_pstrGGGWord[i]),g_pstrGGGWord[i]);
		
		file.Close();
	}

	{
		file.Open("npcName.txt","wb");

		for (i=0;i<m_iNpcNameTextCount;i++)
			file.writeStreamCR("%3d  [%d] \"%s\" \n",i,GetTextCheckSum(g_pstrNpcName[i]),g_pstrNpcName[i]);

		file.Close();
	}

	SetLogFolder();

	return	TRUE;
}

void
CLoadTextData::saveSimpleTextData(cFILE *_lpFile,char **_lpTextData,int _iCount)
{
	int		i;
	WORD	wStrlen;

	_lpFile->Write(&_iCount,4);

	for (i=0;i<_iCount;i++)
	{
		wStrlen	=	(WORD)strlen(_lpTextData[i])+1;

		_lpFile->Write(&wStrlen,2);
		_lpFile->Write(_lpTextData[i],wStrlen);
	}
}

BOOL
CLoadTextData::save(char* _lpstrFodler)
{
	SetLogFolder();
	SetCurrentDirectory(_ms("localize/%s/result data",_lpstrFodler));

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
//	saveSimpleTextData(&file,g_pstrMonsterDic,m_iMonsterDicCount);
	saveSimpleTextData(&file,g_pstrGGGWord,m_iGGGWordCount);


	file.Write(g_strFontName,sizeof(g_strFontName));
	file.Write(g_strMessageBoxTitleFontName,sizeof(g_strMessageBoxTitleFontName));
	file.Write(g_strCheckFontName,sizeof(g_strCheckFontName));
	file.Write(g_strPitchmanShopTextName,sizeof(g_strPitchmanShopTextName));

	file.Write(&m_iDebugCommandCount,4);
	file.Write(g_aOperatorCommandInfo,sizeof(COperatorCommandInfo)*m_iDebugCommandCount);

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

	if (!file.Open("nameFilter.dat","wb"))
		return	FALSE;

	saveSimpleTextData(&file,g_pstrNameFilter,m_iNameFilterCount);
	file.Close();

	SetLogFolder();

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

	if (!file.Open("textData.dat","rb"))
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
//	g_pstrMonsterDic			=	loadSimpleTextData(&file,&m_iMonsterDicCount);
	g_pstrGGGWord				=	loadSimpleTextData(&file,&m_iGGGWordCount);

	file.Read(g_strFontName,sizeof(g_strFontName));
	file.Read(g_strMessageBoxTitleFontName,sizeof(g_strMessageBoxTitleFontName));
	file.Read(g_strCheckFontName,sizeof(g_strCheckFontName));
	file.Read(g_strPitchmanShopTextName,sizeof(g_strPitchmanShopTextName));

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

// 	{
// 		file.Open("c:/itemExplain2.txt","wb");
// 		
// 		for (i=0;i<m_iItemExplainCount;i++)
// 			file.writeStreamCR("%3d  [%d] \"%s\" \n",i,g_aItemEffectHyperText[i].m_iSerial,g_aItemEffectHyperText[i].m_strReferenceText);
// 		
// 		file.Close();
// 	}


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

	SetLogFolder();

	return	TRUE;
}