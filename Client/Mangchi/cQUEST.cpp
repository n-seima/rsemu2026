#include "CQuest.H"
#include "cMESSAGE.H"

CQuest			g_quest;
CQuest			g_questArbeit;

//
//	등록된 퀘스트를 전부 리셋 시킨다.
void
CQuest::reset()
{
	int	i,iCount=0;

	for (i=0;i<dQUEST_COUNT && iCount < m_iCount;i++)
	{
		if (m_aQuest[i].m_wSerial	!=	0xffff)
		{
			m_aQuest[i].reset();

			iCount++;
		}
	}

	m_iCount	=	0;
}

void
CQuest::rebuild()
{
	m_iCount	=	0;

	for (int i=0;i<dQUEST_COUNT;i++)
	{
		if	(m_aQuest[i].m_wSerial	!=	0xffff)
		{
			if	(i>=213)
				i	=	i;
			m_iCount++;
		}
	}
}


//
//	이베트 퀘스트 초기화
void
CQuest::initEventQuest()
{
	int	i;

	for (i=0;i<dEVENT_QUEST_COUNT;i++)
	{
		CQuestItem	item;

		item.reset();

		item.m_wSerial	=	i;
		sprintf(item.m_strName,"event #%.2d",i+1);
		sprintf(item.m_astrExplain[0],"hohoho");

		add(&item);
	}
}

//
//	name이라는 이름을 가진 퀘스틀 찾는다.
int
CQuest::getIndex(char *name)
{
	int	i,iCount=0;

	for (i=0;i<dQUEST_COUNT && iCount < m_iCount;i++)
	{
		if (m_aQuest[i].m_wSerial	!=	0xffff)
		{
			if (stricmp(name,m_aQuest[i].m_strName)	==	0)	return	i;

			iCount++;
		}
	}

	return	0xffff;
}

//
//	새로운 퀘스트를 추가한다.
BOOL
CQuest::add(CQuestItem *_lpQuest)
{
	if (getIndex(_lpQuest->m_strName)	!=	0xffff)	
		return ERRMSG(dMSG_ADD_QUEST_ERROR,dMSG_EXIST_SAME_NAME_QUEST);

	m_aQuest[m_iCount].copy(_lpQuest);
	m_aQuest[m_iCount].m_wSerial	=	m_iCount;

	m_iCount++;

	return	TRUE;
}

//
//	index번째 퀘스트를 지운다.
BOOL
CQuest::remove(int _iIndex,BOOL _bIsArbeit)
{
	if (!_bIsArbeit)
		if (_iIndex < dEVENT_QUEST_COUNT	||	_iIndex >= m_iCount			)	return FALSE;

	m_aQuest[_iIndex].m_wQuestType	=	eQT_DEATH;

	strcpy(m_aQuest[_iIndex].m_strName,"Removed Quest");

	return	TRUE;
}

//
//	name이라는 이름을 가진 퀘스트를 지운다.
BOOL
CQuest::remove(char *_lpstrName,BOOL _bIsArbeit)
{
	int iIndex	=	getIndex(_lpstrName);

	return	remove(iIndex,_bIsArbeit);
}

//
//	퀘스트 이름을 리턴한다.
char*
CQuest::getName(int index)
{
	return	m_aQuest[index].m_strName;
}
