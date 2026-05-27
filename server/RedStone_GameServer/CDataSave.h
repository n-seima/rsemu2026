#ifndef _classDataSave_H
#define _classDataSave_H

#include "cACTOR.H"

#define	dMAX_SAVE_DATA_COUNT	1024

class	CDataSaveManager
{
	class	CData
	{
	public:
		CPlayerSaveData		m_data;
		WORD				m_wIsDeathData;
		WORD				m_wIsRequireSendLog;
	};
	
	CData					*m_pData;
	CRITICAL_SECTION		m_csWork;
	int						m_iQueSize,m_iFirstQue,m_iLastQue,m_iCount,m_iMaxQueCount;		//	큐 사이즈 첫번째,마지막 큐, 쌓여 있는 데이터 수
	BOOL					m_bIsActiveManager,m_bIsReadyForEndManager;

public:
							CDataSaveManager();
							~CDataSaveManager();

	void					init(int _iQueSize=dMAX_SAVE_DATA_COUNT);	//	초기화
	void					close();									//	종료
	void					reset();									//	리셋

	inline	int				getCount()				{return	m_iCount;}
	inline	void			readyForEndManager()	{m_bIsReadyForEndManager=TRUE;}	//	메니저 종료 준비
	inline	BOOL			isActiveManager()		{return	m_bIsActiveManager;}	//	메니저의 모든 작업이 완료 되었다.

	BOOL					add(CPlayerSaveData *_lpSaveData,BOOL _bIsRequireSendLog);	//	패킷 추가
	BOOL					remove(char *_lpstrName);
	BOOL					pop(CData* _lpSaveData);	//	큐에서  빼낸다.
	void					update();
};

extern	CDataSaveManager*	g_pDsManager;

#endif