#include "CDataSave.h"
#include "life_code.h"
#include "GameDLL.H"

CDataSaveManager*	g_pDsManager;

CDataSaveManager::CDataSaveManager()
{
	reset();
	m_pData			=	NULL;
	m_iQueSize		=	0;
	m_iMaxQueCount	=	0;

	FPInitCS(&m_csWork);
}

CDataSaveManager::~CDataSaveManager()
{
	close();
}

//
//	초기화
void
CDataSaveManager::init(int _iQueSize)
{
	pKILL(m_pData);

	m_iQueSize				=	_iQueSize;
	m_pData					=	new	CData	[m_iQueSize];
	m_bIsActiveManager		=	TRUE;
	m_bIsReadyForEndManager	=	FALSE;

	reset();
}

//
//	종료
void
CDataSaveManager::close()
{
	DeleteCriticalSection(&m_csWork);
	pKILL(m_pData);
	m_iQueSize	=	0;
	reset();
}

//
//	초기화
void
CDataSaveManager::reset()
{
	m_iFirstQue		=	0;
	m_iLastQue		=	0;
	m_iCount		=	0;
}

//
//	패킷 추가
BOOL
CDataSaveManager::add(CPlayerSaveData *_lpSaveData,BOOL _bIsRequireSendLog)
{
	mCS(m_csWork);

	if	(m_iCount	>=	m_iQueSize)
	{
		return	FALSE;
	}

	memcpy(&m_pData[m_iLastQue].m_data,_lpSaveData,sizeof(CPlayerSaveData));

	m_pData[m_iLastQue].m_wIsDeathData		=	FALSE;
	m_pData[m_iLastQue].m_wIsRequireSendLog	=	_bIsRequireSendLog;

	m_iLastQue++;
	m_iCount++;

	if	(m_iCount	>	m_iMaxQueCount		)
		m_iMaxQueCount	=	m_iCount;
	if	(m_iLastQue	>=	m_iQueSize			)
		m_iLastQue	=	0;	//	처음으로 돌아간다.

	return	TRUE;
}

//
//	패킷 추가
BOOL
CDataSaveManager::remove(char *_lpstrName)
{
	mCS(m_csWork);

	int	iIndex	=	m_iFirstQue;

	for (int i=m_iCount;i;--i)
	{
		if	(STRCMP(m_pData[iIndex].m_data.m_strName,_lpstrName)==0)
			m_pData[iIndex].m_wIsDeathData	=	TRUE;

		iIndex++;

		if (iIndex	>=	dMAX_SAVE_DATA_COUNT)	iIndex	=	0;
	}

	return	TRUE;
}

//
//	큐에서 패킷을 꺼낸다.
BOOL
CDataSaveManager::pop(CData* _lpSaveData)
{
	mCS(m_csWork);

	if (m_iCount	<=	0			)
		return	FALSE;

	memcpy(_lpSaveData,&m_pData[m_iFirstQue],sizeof(CData));

	m_iFirstQue++;
	m_iCount--;

	if	(m_iFirstQue	>=	dMAX_SAVE_DATA_COUNT)
		m_iFirstQue=0;	//	처음으로 돌아간다.

	return	TRUE;
}

//
//	업데이트
void
CDataSaveManager::update()
{
	CData	saveData;

	while(1)
	{
		Sleep(1);

		g_lifeCodeMgr.beginCheck(eLIFE_CODE_DATA_SAVE);

		if	(!pop(&saveData))
		{
			if	(m_bIsReadyForEndManager)
				break;

			continue;
		}

		cACTOR	*lpActor	=	(cACTOR *)&saveData.m_data;

		if	(saveData.m_wIsDeathData)
			continue;

		lpActor->sendSaveDataToDBFile(TRUE,saveData.m_wIsRequireSendLog);
	}

	m_bIsActiveManager	=	FALSE;

	g_lifeCodeMgr.endCheck(eLIFE_CODE_DATA_SAVE);
}