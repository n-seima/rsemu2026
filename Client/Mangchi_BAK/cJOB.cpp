#include	"cJOB.H"
#include	"cMESSAGE.H"
#include	"cMAP.H"
#include	"resource.H"

int	g_aiEnableJobIDCList[]	=	
{
	IDC_LIMIT_KNIGHT,IDC_LIMIT_WARRIOR,IDC_LIMIT_WIZARD,IDC_LIMIT_WEREWOLF,IDC_LIMIT_PRIEST,IDC_LIMIT_FALLEN_ANGEL,
	IDC_LIMIT_ROGUE,IDC_LIMIT_FIGHTER,IDC_LIMIT_MAGIC_LANCER,IDC_LIMIT_MAGIC_ARCHER,IDC_LIMIT_BEAST_TAMER,IDC_LIMIT_SUMMONER,
	IDC_LIMIT_PRINCESS,IDC_LIMIT_MAGICAL_GIRL,IDC_LIMIT_NECROMANCER,IDC_LIMIT_DEVIL,IDC_LIMIT_SOUL_TAKER, IDC_LIMIT_CHAMPION,IDC_LIMIT_OPTICALIST,IDC_LIMIT_BEAST_MAN,-1
};

cJOB_DEN		g_denJob;

///////////////////////////////////////////////////////////////////////
//	직업 세트
///////////////////////////////////////////////////////////////////////

//
//	복사-_- 한다.
void
cJOB::copy(cJOB *_lpJob)
{
	int	iSerial	=	m_iSerial;

	memcpy(this,_lpJob,sizeof(cJOB));

	m_iSerial	=	iSerial;
}

/////////////////////////////////////////////////////////
//	정적 멤버
/////////////////////////////////////////////////////////
BOOL
cJOB::Init()
{
	g_BodyList.init();

	return	TRUE;
}

int
cJOB::GetCharacterCount()
{
	return	g_lpActmap->m_setCharacter.m_iCount;
}


//
//	job번재 직업이 있나 확인한다.
BOOL
cJOB::IsAvailJob(int job)
{
	if (GetJob(job))	return	TRUE;

	return	FALSE;
}

//
//	등록된 직업죽에 job번째 직업을 리턴한다.
cJOB*
cJOB::GetJob(int job)
{
	return	g_denJob.getJob(job);
}

//
//		job의 직업 이름을 얻는다.
char*
cJOB::GetName(int job)
{
	return	g_denJob.getJob(job)->m_strName;
}


//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//	직업.. 군-_-
//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆

cJOB_DEN::cJOB_DEN()
{
	init();
}

cJOB_DEN::~cJOB_DEN()
{
	reset();
}

//
//	초기화
void
cJOB_DEN::init()
{
	for (int	i=0;i<dMAX_JOB_COUNT;i++)
		m_aJobList[i].m_iSerial	=	0xffff;

	m_iCount	=	0;
	m_iRookie	=	0;				//	카운트,루키
}

//
//	제거
void
cJOB_DEN::reset()
{
	for (int	i=0;i<dMAX_JOB_COUNT;i++)
		m_aJobList[i].reset();

	m_iCount	=	0;
	m_iRookie	=	0;				//	카운트,루키
}

//
//	_iIndex번째 직업을 얻는다.
cJOB*
cJOB_DEN::getJob(int _iIndex)
{
	if (_iIndex	>=	dMAX_JOB_COUNT)
		return	NULL;

	if (m_aJobList[_iIndex].m_iSerial	==	0xffff)
		return	NULL;

	return	&m_aJobList[_iIndex];
}

//
//	_strName이란 이름의 직업을 찾는다.
cJOB*
cJOB_DEN::getJob(char *_strName)
{
	for (int	i=0;i<dMAX_JOB_COUNT;i++)
		if	(m_aJobList[i].m_iSerial	!=	0xffff)
			if	(stricmp(_strName,m_aJobList[i].m_strName)==0)
				return	&m_aJobList[i];

	return	NULL;
}

//
//	직업 추가
cJOB*
cJOB_DEN::addJob(cJOB *_lpJob)
{
	int	iSerial	=	m_iRookie;

	if	(m_iCount	>=	dMAX_JOB_COUNT)
		return	NULL;

	m_aJobList[m_iRookie].copy(_lpJob);

	m_aJobList[m_iRookie].m_iSerial	=	m_iRookie;

	for (int i=m_iRookie+1;i<dMAX_JOB_COUNT;i++)
		if (m_aJobList[i].m_iSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iCount++;

	return	&m_aJobList[iSerial];
}

//
//	 _iIndex 번째 직읍을 제거
BOOL
cJOB_DEN::remove(int _iIndex)
{
	if (m_aJobList[_iIndex].m_iSerial	==	0xffff)	return	FALSE;
	m_aJobList[_iIndex].reset();

	m_iCount--;

	if (m_iRookie	>	_iIndex)	m_iRookie	=	_iIndex;

	return	TRUE;
}

//
//	_strName을 제거
BOOL
cJOB_DEN::remove(char *_strName)
{
	cJOB	*lpJob	=	getJob(_strName);

	if (!lpJob)	return	FALSE;

	return	remove(lpJob->m_iSerial);
}

//
//	m_iCount와 m_iRookie를 다시 설정한다.
void
cJOB_DEN::rebuild()
{
	int		i;

	m_iCount	=	0;
	m_iRookie	=	0xffff;

	for (i=0;i<dMAX_JOB_COUNT;i++)	if (m_aJobList[i].m_iSerial	!=	0xffff)	m_iCount++;
	for (i=0;i<dMAX_JOB_COUNT;i++)	if (m_aJobList[i].m_iSerial	==	0xffff)	{m_iRookie	=	i;break;}

}

BOOL
cJOB_DEN::saveData(char *_lpstrFileName)
{
	cFILE	file;

	if	(!file.Open(_lpstrFileName,"wb"))
	{
		SetLogFolder();

		return	ERRMSG(dMSG_WRITE_FILE_ERROR,_ms("%s\n\n\"%s\"",dMSG_CAN_NOT_OPEN_FOLLOW_FILE,_lpstrFileName));
	}

	file.Write(&m_iCount,4);

	for (int i=0;i<m_iCount;i++)
	{
		cJOB	*lpJob	=	getJob(i);

		if (lpJob)
			file.Write(lpJob,sizeof(cJOB));
	}

	file.Close();

	SetLogFolder();

	return	TRUE;
}
