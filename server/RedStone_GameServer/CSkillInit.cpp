#include "CActiveSkill.H"
#include "rscomm.H"
#include "mess_sign.H"

cSKILL_MANAGER	g_sm;

//
//	이름으로 기술의 인덱스를 구함
int
cSKILL_MANAGER::getIndex(char *skillName,int job)
{
	for (int i=0;i<dMAX_SKILL;i++)
		if (STRICMP(skillName,g_aSkill[i].m_strName)==0 )
			if (g_aSkill[i].m_wEnableJob == job || g_aSkill[i].m_wEnableJob == 0xffff || g_aSkill[i].m_wEnableJob == dJOB_CODE_ALL)
				return i;

	return 0xffff;
}	//	cSKILL_MANAGER::getIndex(char *skillName)

//
//	스킬 그룹 인덱스를 얻어 오자.
int
cSKILL_MANAGER::getSkillGroupIndex(char *_strName,int _iJob)
{
	int	i;

	for (i=0;i<m_iSkillGroupCount;i++)
	{
		if (m_aSkillGroup[i].m_wJob	==	_iJob)
		{
			if (STRICMP(m_aSkillGroup[i].m_strName,_strName)	==	0)
				return	i;
		}
	}

	return	0xffff;
}

//
//	스킬 데이터를 데이터 화일에서 읽어 들인다.
BOOL
cSKILL_MANAGER::loadData(char *fn)
{
	char	*lpstrFileName	=	fn;

// #ifdef	_FOR_JAPAN
// 	cMessSign	messSign;
// 
// 	lpstrFileName	=	messSign.makeDecodeFile(lpstrFileName);
// 
// #ifdef	_DEBUG
// 	if	(!messSign.m_iIsSuccess)
// 		Beep(2200,200);
// #endif
// 
// #endif

	cFILE	file;

	if	(!file.Open(lpstrFileName,"rb"))
		return	ERRMSG("loading skill data","- Failed : can not find data file");	//	데이터 폴더 찾기

	UINT	uiHeaderSign;
	int		iVersion		=	-1,iNation;

	file.Read(&uiHeaderSign,4);		//	스킬의 수

	if	(uiHeaderSign	==	dSKILL_DATA_HEADER_SIGN)
		file.Read(&iVersion,4);		//	스킬의 수
	else
		file.Seek(0,SEEK_SET);

	file.Read(&m_iSkillCount,4);		//	스킬의 수
	file.Read(&iNation,4);		//	스킬의 수

#ifdef	_FOR_JAPAN
	iNation	=	eNationJapan;
#endif

	cMessSign::SetDataEncodeTable(iNation);

	file.Read(g_aSkill,sizeof(CSkill)*dMAX_SKILL);		//	스킬의 수

	file.Read(&m_iSkillGroupCount,4);		//	스킬의 수
	file.Read(m_aSkillGroup,m_iSkillGroupCount*sizeof(cSKILL_GROUP));		//	스킬의 수
	file.Close();

	if	(iVersion	>=	eSKILL_DATA_VERSION_ENCRYPT)
	{
		cMessSign::DecodeDataBuffer2(&m_iSkillCount,4);
		
		for (int i=0;i<dMAX_SKILL;i++)
			cMessSign::DecodeDataBuffer2(g_aSkill+i,sizeof(CSkill));
	}

	if	(g_aSkill[12].m_wSerial != 12)
		return	ERRMSG("loading skill data","- Failed incorrect skill data");	//	데이터 폴더 찾기

	m_iMiniPetCombiSkillCount	=	0;

	for (int i=0;i<dMAX_SKILL;i++)
	{
		if	(g_aSkill[i].m_wSerial	==	0xffff)
			continue;

		if	(g_aSkill[i].m_bf1IsObitianSkill)
			m_iObitianSkill		=	i;
		if	(g_aSkill[i].m_bf1IsDashBladeSkill)
			m_iDashBladeSkill	=	i;
		if	(g_aSkill[i].m_bf1IsAstroBowSkill)
			m_iAstroBowSkill	=	i;
		if	(g_aSkill[i].m_bf1IsCristalWaterSkill)
			m_iCristalWaterSkill=	i;

		if	(g_aSkill[i].m_wType==	eSKILL_TYPE_MINIPET_COMBI)
			m_aiMiniPetCombiSkill[m_iMiniPetCombiSkillCount++]	=	i;
	}

	if	(!file.Open("data/fighterData.dat","rb"))
		return	ERRMSG("loading skill data","- Failed - can not find 'data/fighterData.dat' file");	//	데이터 폴더 찾기

	file.Read(&g_fighterAttackInfo,sizeof(g_fighterAttackInfo));
	file.Close();

	return	TRUE;
}