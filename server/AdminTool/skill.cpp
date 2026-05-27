#include	"stdafx.h"
#include	"skill.h"
#include	"CSkill.h"
#include	"CMessage.h"
#include	"mess_sign.h"
#include	"atlbase.h"
#include	"AdminTool.h"

int			g_iSkillCount	=	0;
char		g_strDataFolder[512];

BOOL
SetGameFolder()
{
	SetCurrentDirectory(g_strGameFolder);

	return	FALSE;	
}

BOOL
LoadSkillData()
{
	SetGameFolder();

	if	(g_aSkill[7].m_wSerial	==	7)
	{
		SetCurrentDirectory(g_strLogFolder);
		return	TRUE;
	}

	if	(stricmp(g_aPlayerDefaultJobData[15].m_strId,"gogooma") != 0)
	{
		FILE	*fp;

		fp	=	fopen("data/defaultJob.dat","rb");

		if	(!fp)
		{
			cMSG::Put(g_strToolMessage[95]);
			SetCurrentDirectory(g_strLogFolder);
			return	FALSE;
		}

		fseek(fp,0,SEEK_END);

		int	iSize	=	ftell(fp);

		fseek(fp,0,SEEK_SET);

		fread(g_aPlayerDefaultJobData,sizeof(g_aPlayerDefaultJobData),1,fp);

		fclose(fp);

		if	(stricmp(g_aPlayerDefaultJobData[15].m_strId,"gogooma") != 0)
		{
			cMSG::Put(g_strToolMessage[96]);
			SetCurrentDirectory(g_strLogFolder);
			return	FALSE;
		}
	}

	char	*lpstrFileName	=	"data/skill.dat";

#ifdef _FOR_KOREA
	lpstrFileName	=	"data/skillToAdmin.dat";

	if (!IsFile(lpstrFileName))
		lpstrFileName	=	"data/skill.dat";
#else
	lpstrFileName	=	"data/skill2.dat";
#endif	

// #ifdef	_FOR_JAPAN
// 	cMessSign	messSign;
// 
// 	lpstrFileName	=	messSign.makeDecodeFile(lpstrFileName);
// 
// #ifdef	_DEBUG
// 	if	(!messSign.m_iIsSuccess)
// 		Beep(1200,200);
// #endif

// #endif

	FILE	*fp	=	fopen(lpstrFileName,"rb");

	if (!fp)
	{
		cMSG::Put(g_strToolMessage[97]);
		SetCurrentDirectory(g_strLogFolder);
		return	FALSE;
	}

	UINT	uiHeaderSign;
	int		iVersion		=	-1,iNation;

	fread(&uiHeaderSign,4,1,fp);		//	스킬의 수

	if	(uiHeaderSign	==	dSKILL_DATA_HEADER_SIGN)
		fread(&iVersion,4,1,fp);		//	스킬의 수
	else
		fseek(fp,0,SEEK_SET);

	fread(&g_iSkillCount,4,1,fp);
	fread(&iNation,4,1,fp);

	cMessSign::SetDataEncodeTable(iNation);

	fread(g_aSkill,sizeof(g_aSkill),1,fp);
	fclose(fp);

	SetCurrentDirectory(g_strLogFolder);

	if	(iVersion	>=	eSKILL_DATA_VERSION_ENCRYPT)
	{
		cMessSign::DecodeDataBuffer2(&g_iSkillCount,4);

		for (int i=0;i<dMAX_SKILL;i++)
			cMessSign::DecodeDataBuffer2(g_aSkill+i,sizeof(cSKILL));
	}

	if	(g_aSkill[7].m_wSerial	!=	7)
		cMSG::Error("error","this computer's red stone client not lastest version");

	return	TRUE;
}
