#include "cSKILL.H"
#include "cDECODER.H"
#include "cJOB.H"
#include "mess_sign.H"
#include "localize.H"

cSKILL				g_aSkill[dMAX_SKILL];
cSKILL_GROUP		g_aSkillGroup[dSKILL_GROUP_COUNT];
int					cSKILL::s_iSkillCount	=	0;
int					cSKILL::s_iSkillGroupCount;

//
//	초기화
BOOL
cSKILL::Init(char *fn)
{
	cFILE	file;

	if	(!file.Open(fn,"rb"))
		return	ERRMSG("Skill initialization error!!","%s file not found.",fn);

	UINT	uiHeaderSign;
	int		iVersion		=	-1;

	file.Read(&uiHeaderSign,4);		//	스킬의 수

	if	(uiHeaderSign	==	dSKILL_DATA_HEADER_SIGN)
		file.Read(&iVersion,4);		//	스킬의 수
	else
		file.Seek(0,SEEK_SET);

	file.Read(&s_iSkillCount,4);

	int		iLocalizeNation	= eNationJapan;

	if	(iVersion	>=	eSKILL_DATA_VERSION_ENCRYPT2)
		file.Read(&iLocalizeNation,sizeof(iLocalizeNation));

	file.Read(g_aSkill,sizeof(cSKILL)*dMAX_SKILL);
	file.Read(&s_iSkillGroupCount,4);
	file.Read(g_aSkillGroup,sizeof(cSKILL_GROUP)*s_iSkillGroupCount);
	file.Close();

	if	(iVersion	>=	eSKILL_DATA_VERSION_ENCRYPT)
	{
		if	(iVersion	>=	eSKILL_DATA_VERSION_ENCRYPT2)
			cMessSign::SetDataEncodeTable(iLocalizeNation);
		else
			cMessSign::SetDataEncodeTable(-1);

		cMessSign::DecodeDataBuffer2(&s_iSkillCount,4);
		
		for (int i=0;i<dMAX_SKILL;i++)
			cMessSign::DecodeDataBuffer2(g_aSkill+i,sizeof(cSKILL));
	}

	if	(g_aSkill[7].m_wSerial	!=	7	||	g_aSkill[17].m_wSerial	!=	17)
		return	ERRMSG("Skill initialization error!!","Skill data is incorrect.");

	g_gsManager.loadData();

	return	TRUE;
}

BOOL
cSKILL::SaveData(char *fn)
{
	cFILE	file;

	if	(!file.Open(fn,"wb"))
		return	ERRMSG("Skill initialization error!!","Cannot open file %s.",fn);

	UINT	uiHeaderSign	=	dSKILL_DATA_HEADER_SIGN;
	int		iVersion		=	eSKILL_DATA_VERSION_CURRENT;

	int		iSaveSkillCount	=	s_iSkillCount;

	cMessSign::SetDataEncodeTable(g_iLocalizeNation);
	cMessSign::EncodeDataBuffer2(&iSaveSkillCount,4);

	file.Write(&uiHeaderSign,4);		//	헤더 사인
	file.Write(&iVersion,4);		//	버젼
	file.Write(&iSaveSkillCount,4);

	{
		int		iLocalizeNation	= eNationJapan;
		iLocalizeNation			=	g_iLocalizeNation;
		file.Write(&iLocalizeNation,sizeof(g_iLocalizeNation));
	}

	{
		for (int i=0;i<dMAX_SKILL;i++)
		{
			cSKILL	saveData;

			memcpy(&saveData,g_aSkill+i,sizeof(cSKILL));
			cMessSign::EncodeDataBuffer2(&saveData,sizeof(cSKILL));
			file.Write(&saveData,sizeof(cSKILL));
		}
	}

	file.Write(&s_iSkillGroupCount,4);
	file.Write(g_aSkillGroup,sizeof(cSKILL_GROUP)*s_iSkillGroupCount);
	file.Close();

	g_gsManager.loadData();

	return	TRUE;
}

//
//	이름으로 기술의 인덱스를 구함
//	char	*skillName	-	인덱스를 찾을 기술이름
//	return	value		-	인덱스를 리턴하고 찾지 못하면 0xffff을 리턴
int
cSKILL::GetIndex(char *skillName,int job)
{
	for (int i=0;i<dMAX_SKILL;i++)
		if	(stricmp(skillName,g_aSkill[i].m_strName)==0 )
			if	(g_aSkill[i].m_wEnableJob == job || g_aSkill[i].m_wEnableJob == 0xffff	||	job	==	0xffff)
				return i;

	return 0xffff;
}

CGuildSkillManager	g_gsManager;

CGuildSkill::CGuildSkill()
{
}

//
//	데이터 읽기
BOOL
CGuildSkillManager::loadData(char *_lpstrFn)
{
	cFILE	file;

	if	(_lpstrFn	==	NULL)
		_lpstrFn	=	"data/guildSkill.dat";

	if (!file.Open(_lpstrFn,"rb"))
		return	FALSE;

	file.Read(&m_iSkillCount,4);
	file.Read(m_aSkills,sizeof(g_gsManager.m_aSkills));
	file.Close();

	return	TRUE;
}

//	데이터 저장
BOOL
CGuildSkillManager::saveData(char *_lpstrName)
{
	cFILE	file;

	if (!file.Open(_lpstrName,"wb"))
		return	FALSE;

	file.Write(&m_iSkillCount,4);
	file.Write(m_aSkills,sizeof(m_aSkills));
	file.Close();

	return	TRUE;
}
