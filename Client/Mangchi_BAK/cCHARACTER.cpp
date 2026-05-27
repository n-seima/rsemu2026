#include "cCHARACTER.H"
#include "cSetDialogNpcData.h"					//	----------------------------------	추가
#include	"CSetDialogMonster.h"					//	11.23 추가
int
GetCorrectExpByLevelGap(int iSharedExp,int _iPlayerLevel)
{
	LONGLONG	llExp		=	iSharedExp;
	
	if	(_iPlayerLevel >=250)
	{
		float	fExp	=	(float)llExp;
		float	fLevel	=	(float)_iPlayerLevel;
		fExp	=	fExp*(1.0f-((fLevel-245.0f)/1800.0f));
		llExp	=	(LONGLONG)fExp;
	}
	
	iSharedExp	=	(int)llExp;
	
	return	max(iSharedExp,1);
}

//
//
cJOB*
cCHARACTER::getJob()
{
	return	g_denJob.getJob(m_wBaseJob);
}	//	cCHARACTER::getJob()

BOOL
cCHARACTER::isExistEvent()
{
	for (int i=0;i<eMECC_COUNT;i++)
		if	(m_pEvent[i])
			return	TRUE;

	return	FALSE;
}

//
//	복사
void
cCHARACTER::copy(cCHARACTER *_lpCharacter)
{
	reset();

	memcpy(this,_lpCharacter,sizeof(CCharacterBaseInfo));

	for (int i=0;i<eMECC_COUNT;i++)
	{
		if	(_lpCharacter->m_pEvent[i] == NULL)
			continue;

		KILL(m_pEvent[i]);

		m_pEvent[i]	=	new CKarmaContainer();
		m_pEvent[i]->copy(_lpCharacter->m_pEvent[i]);
	}
}

///////////////////////////////////////////////////////////////////////
//	직업 세트
///////////////////////////////////////////////////////////////////////
//
//	생성자
cCHARACTER_SET::cCHARACTER_SET()
{
	reset();
}	//	cCHARACTER_SET::cCHARACTER_SET()

//
//	해당 인덱스의 이름을 얻어온다.
char*
cCHARACTER_SET::getName(int index)
{
	if (index >= dJOB_COUNT	||	index	<	0		)	return	NULL;
	if (m_aCharacter[index].m_wSerial	==	0xffff	)	return	NULL;

	return	m_aCharacter[index].m_strName;
}	//	cCHARACTER_SET::GetName(int index)

//
//	초기화
void
cCHARACTER_SET::reset()
{
	for(int i=0;i<dJOB_COUNT;i++)	m_aCharacter[i].reset();

	m_iCount			=	0;
	m_iSelectCharacter	=	0xffff;

}	//	cCHARACTER_SET::Reset()

//
//	직업 추가
BOOL
cCHARACTER_SET::add(cCHARACTER *_lpCharacter)
{
	int	index	=	getAvailIndex();
	if (index	==	0xffff)	return	FALSE;

	m_aCharacter[index].copy(_lpCharacter);
	m_aCharacter[index].m_wSerial	=	index;

	if (m_iSelectCharacter	==	0xffff)	m_iSelectCharacter	=	index;

	m_iCount++;

	return	TRUE;
}	//	cCHARACTER_SET::add(cCHARACTER *_lpCharacter)

/*
//
//	직업 추가
BOOL
cCHARACTER_SET::add(cCHARACTER *_lpCharacter)
{
	int	index	=	getAvailIndex();
	if (index	==	0xffff)	return	FALSE;

	m_aCharacter[index].m_wSerial		=	index;
	m_aCharacter[index].m_wBaseJob		=	_iJob;
	m_aCharacter[index].m_wMinLevel		=	_iMinLevel;
	m_aCharacter[index].m_wMaxLevel		=	_iMaxLevel;
	strcpy(m_aCharacter[index].m_strName,_strName);

	if (m_iSelectCharacter	==	0xffff)	m_iSelectCharacter	=	index;

	m_iCount++;

	return	TRUE;
}	//	cCHARACTER_SET::add(cCHARACTER *_lpCharacter)
*/

//
//	직업 제거
BOOL
cCHARACTER_SET::remove(int index)
{	// 마지막 수정일  : 09.10.30
	int		i;

	if (m_iCount	<=					0	) return FALSE;
	if (index		>=		dMAX_CHARACTER	) return FALSE;
	if (m_aCharacter[index].m_wBaseJob==	0xffff	) return FALSE;

	m_aCharacter[index].m_wBaseJob	=	0xffff;
	m_aCharacter[index].m_wSerial		=	0xffff;
	m_iCount--;

	if (m_iCount > 0)
	{
		if (m_aCharacter[m_iSelectCharacter].m_wBaseJob	==	0xffff)
		{
			for (i=index;i<dJOB_COUNT;i++)
			{
				if (m_aCharacter[i].m_wBaseJob	!=	0xffff)
				{
					m_iSelectCharacter	=	i;
					cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
					CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
					CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);

					return	TRUE;
				}
			}

			for (i=index;i>=0;i--)
			{
				if (m_aCharacter[i].m_wBaseJob	!=	0xffff)
				{
					m_iSelectCharacter	=	i;
					cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
					CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
					CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
					return	TRUE;
				}
			}
		}
	}
	else
	{
		m_iSelectCharacter	=	0xffff;
	}
	cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
	CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
	CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
	return	TRUE;
}	//	cCHARACTER_SET::remove(int index)


//
//	선택한 직업 제거
//
BOOL
cCHARACTER_SET::removeSelectCharacter()
{
	return	remove(m_iSelectCharacter);
}	//	cCHARACTER_SET::RemoveHero()

//
//	선택된 직업이 없다면 젤 처음에 있는넘으러 선택한다.
//
void
cCHARACTER_SET::setSelectCharacter()
{
	if (m_iCount	==	0)
	{	m_iSelectCharacter	=	0xffff;

		return;
	}

	for (int i=0;i<dJOB_COUNT;i++)
		if (m_aCharacter[i].m_wBaseJob	!=	0xffff)
		{
			m_iSelectCharacter	=	i;
			return;
		}
}	//	cCHARACTER_SET::SetSelectJob()

//
//	선택된 직업을 리턴
//
cCHARACTER*
cCHARACTER_SET::getSelectCharacter()
{
	if (m_iSelectCharacter	>=	dJOB_COUNT)	return NULL;
	if (m_aCharacter[m_iSelectCharacter].m_wSerial	==	0xffff)	return	NULL;

	return	&m_aCharacter[m_iSelectCharacter];
}	//	cCHARACTER_SET::GetSelectJob()

//
//	index번째 캐릭터를 리턴한다.
//
cCHARACTER*
cCHARACTER_SET::getCharacter(int index)
{
	if (m_aCharacter[index].m_wBaseJob	==	0xffff)	return	NULL;
	if (m_aCharacter[index].m_wSerial	==	0xffff)	return	NULL;

	return	&m_aCharacter[index];
}	//	cCHARACTER_SET::getCharacter(int index)

//
//	index번째 캐릭터의 직업을 리턴한다.
//
cJOB*
cCHARACTER_SET::getJob(int index)
{
	cCHARACTER	*lpCharacter	=	getCharacter(index);

	if (!lpCharacter)	return	NULL;

	return	lpCharacter->getJob();
}	//	cCHARACTER_SET::getJob(int index)

//
//	선택된 직업의 몸통 사이즈
//
int
cCHARACTER_SET::getBodySize()
{
	if (m_iSelectCharacter	>=	dMAX_CHARACTER)	return 0xffff;

	int	iBody	=	m_aCharacter[m_iSelectCharacter].getBody();

	return	g_pBody[iBody]->m_iCrashSize;
}	//	cCHARACTER_SET::getBodySize()

//
//	선택된 직업의 몸통 사이즈
//
int
cCHARACTER_SET::getBodySize(int index)
{
	if (index	>=	dMAX_CHARACTER)	return 0xffff;

	int	iBody	=	m_aCharacter[index].getBody();

	return	g_pBody[iBody]->m_iCrashSize;
}	//	cCHARACTER_SET::getBodySize()

//
//	선택한 캐릭터의 직업을 리턴한다.
//
int
cCHARACTER_SET::getSelectCharacterJob()
{
	return	m_aCharacter[m_iSelectCharacter].m_wBaseJob;
}	//	cCHARACTER_SET::getSelectCharacterJob()

//
//	선택된 직업의 기본 직업
//
int
cCHARACTER_SET::getSelectCharacterBody()
{
	if (m_iSelectCharacter	>=	dJOB_COUNT)	return 0xffff;

	return	m_aCharacter[m_iSelectCharacter].getBody();
}	//	cCHARACTER_SET::GetDefaultJob()

//
//	index번째 직업의 기본 직업
//
int
cCHARACTER_SET::getBody()
{
	if (m_iSelectCharacter	>=	dMAX_CHARACTER)	return 0xffff;
	if (m_aCharacter[m_iSelectCharacter].m_wBaseJob	==	0xffff)	return	0xffff;

	return	m_aCharacter[m_iSelectCharacter].getBody();
}	//	cCHARACTER_SET::GetDefaultJob(int index)

//
//	index번째 캐릭터의 바디-_-를 리턴한다.
int
cCHARACTER_SET::getBody(int index)
{
	if (index	>=	dMAX_CHARACTER)	return 0xffff;
	if (m_aCharacter[index].m_wBaseJob	==	0xffff)	return	0xffff;

	return	g_denJob.getJob(m_aCharacter[index].m_wBaseJob)->m_wBody;
}	//	cCHARACTER_SET::getBody(int index)

//
//	count번째(유효환 직업들 중에서) 직업의 인덱스를 구한다.
//
int
cCHARACTER_SET::getIndex(int count)
{
	if (count	>=	m_iCount)	return	0xffff;

	int	iCount	=	0;

	for (int i=0;i<dJOB_COUNT;i++)
	{
		if (m_aCharacter[i].m_wSerial	!=	0xffff)
		{
			if (iCount == count) return i;

			iCount++;
		}
	}

	return	0xffff;
}	//	cCHARACTER_SET::GetHeroIndex(int count)

//
//	name이라는 이름을 가진 캐릭터의 인덱스를 리턴한다.
int
cCHARACTER_SET::getIndex(char *name)
{
	int	i;

	for (i=0;i<dMAX_CHARACTER;i++)
		if (m_aCharacter[i].m_wSerial	!=	0xffff)
			if (stricmp(m_aCharacter[i].m_strName,name)==0)	return	i;

	return	0xffff;
}	//	cCHARACTER_SET::getIndex(char *name)

//
//	직업을 하나씩 얻어온다.
cCHARACTER*
cCHARACTER_SET::getNextCharacter()
{
	//	직업을 하나씩 얻어오기 위해 마지막에 얻어온 직업을 기억하기 위해..
	//	m_iAutoIndex,m_iAutoIndexCount가 필요하다.
	if (m_iAutoIndexCount	>=	m_iCount)	return	NULL;

	for (int i = m_iAutoIndex;i<dMAX_CHARACTER;i++)
	{
		if (isAvailCharacter(i))
		{
			m_iAutoIndex	=	i+1;
			m_iAutoIndexCount++;

			return	&m_aCharacter[i];
		}
	}

	return	NULL;
}	//	cCHARACTER_SET::getNextJob()

//
//	쭈욱~~ 훑어서... 사라진 직업을 캐릭터로 가지고 있는건 지워버린다.
BOOL
cCHARACTER_SET::reBuild()
{
	BOOL	isRemovedAnyone	=	FALSE;

	for (int i = 0;i<dMAX_CHARACTER;i++)
		if (isAvailCharacter(i))
			if	(g_denJob.m_aJobList[m_aCharacter[i].m_wBaseJob].m_iSerial	==	0xffff)
			{
				remove(i);
				isRemovedAnyone	=	TRUE;
			}

	return	isRemovedAnyone;
}	//	cCHARACTER_SET::reBuild()
