#ifndef __URDOBUFFER_H__
#define __URDOBUFFER_H__

#include "cMAPWND.H"
#include <deque>

//	버퍼 개수

#define dEDIT_BUFFER_COUNT	10


using std::deque;

struct sOBJECT_INFO
{
		std::deque<int>				deqSize;				// 오브젝트 총버퍼사이즈
		std::deque<int>::iterator	itrSize;				
		std::deque<int>				deqRookie;				// 마지막에 생성된 위치(index)
		std::deque<int>::iterator	itrRookie;
		std::deque<int>				deqCount;				// 오브젝트 갯수. 
		std::deque<int>::iterator	itrCount;
};

class URdoBuffer
{

private:

	cMAP*										m_pMap;								// 현재 맵정보.

	std::deque<LPWORD>							m_deqTileBuffer;					// 09.07.15  타일맵정보
	std::deque<LPWORD>::iterator				m_itrTileBuffer;					// 09.07.15  타일맵정보
	
	std::deque<PattnSample*>					m_deqPatternSampleBuffer;			// 09.07.15  타일패턴정보
	std::deque<PattnSample*>::iterator			m_itrPatternSampleBuffer;			// 09.07.15  타일패턴정보

	std::deque<cFIXED_OBJECT_INFO*>				m_deqFixedObjectBuffer;				// 09.07.16 FixedObject 정보
	std::deque<cFIXED_OBJECT_INFO*>::iterator	m_itrFixedObjectBuffer;				// 09.07.16 FixedObject 정보

	std::deque<cTINY_OBJECT_INFO*>				m_deqTinyObjectBuffer;				// 09.07.16 TinyObject 정보
	std::deque<cTINY_OBJECT_INFO*>::iterator	m_itrTinyObjectBuffer;				// 09.07.16 TinyObject 정보

	std::deque<WORD*>							m_deqObjectMapBuffer;				// 09.07.16 오브젝트맵 정보..
	std::deque<WORD*>::iterator					m_itrObjectMapBuffer;				// 09.07.15 오브젝트맵 정보

	std::deque<BYTE*>							m_deqCrashMapBuffer;				// 09.07.16 crash맵정보( 충돌관련?)
	std::deque<BYTE*>::iterator					m_itrCrashMapBuffer;				// 09.07.16 crash맵정보( 충돌관련?)	

	std::deque<BYTE*>							m_deqBuildingMapBuffer;				// 09.07.16 건물맵정보
	std::deque<BYTE*>::iterator					m_itrBuildingMapBuffer;				// 09.07.16 건물맵정보	

	std::deque<cBUILDING_INFO*>					m_deqBuildingBuffer;				// 09.07.17	건물정보
	std::deque<cBUILDING_INFO*>::iterator		m_itrBuildingBuffer;				// 09.07.17	건물정보

	std::deque<cACTOR_SET*>						m_deqActorBuffer;
	std::deque<cACTOR_SET*>::iterator 			m_itrActorBuffer;

	sOBJECT_INFO								m_sFixedObjectInfo;					// 09.07.17 fixed기타정보
	sOBJECT_INFO								m_sTinyObjectInfo;					// 09.07.17 Tiny기타정보
	sOBJECT_INFO								m_sBuildingInfo;					// 09.07.17 건물기타정보



public:
	URdoBuffer();
	~URdoBuffer();
	BOOL	Init(cMAP* _pMap);						// 09.07.17 초기화.
	void	addBuffer();							// 09.07.15 모든 버퍼 추가.
	void	addTileBuffer();						// 09.07.15 타일버퍼 추가.
	void	addFixedObjectBuffer();					// 09.07.16 fixedObject버퍼 추가.
	void	addTinyObjectBuffer();					// 09.07.16 TinyObject버퍼 추가.
	void	addBuildingBuffer();					// 09.07.16 building버퍼 추가.
	void	addMapBuffer();							// 09.07.16 맵버퍼 추가.
	void	addActorBuffer();
	
	void	deleteBuffer();							// 09.07.15 버퍼 삭제.
	void	deleteTileBuffer();						// 09.07.15 타일버퍼 삭제.
	void	deleteFixedObjectBuffer();				// 09.07.16 fixedObject버퍼 삭제.
	void	deleteTinyObjectBuffer();				// 09.07.16 TinyObject버퍼 삭제.
	void	deleteBuildingBuffer();					// 09.07.16 building 삭제.
	void	deleteMapBuffer();						// 09.07.16 맵버퍼 삭제.
	void	deleteActorBuffer();
	

	BOOL	isEqualBuffer();						// 09.07.16 이전버퍼와 현재 저장할 버퍼 비교.
	
	void	Undo();									// 09.07.15 되돌리기
	void	Redo();									// 09.07.15 앞으로.
	void	UndoTile();								// 09.07.15 타일패턴.
	void	RedoTile();								// 09.07.15 타일패턴.
	void	UndoFixedObject();						// 09.07.16 fixedobject 뒤로
	void	UndoTinyObject();						// 09.07.16 tinyobject 뒤로
	void	UndoBuilding();							// 09.07.17 building 뒤로
	void	RedoFixedObject();						// 09.07.16 fixedobject 앞으로
	void	RedoTinyObject();						// 09.07.16 tinyobject 앞으로
	void	RedoBuilding();							// 09.07.17 buliding 앞으로
	void	UndoMapBuffer();						// 09.07.16 map 뒤로
	void	RedoMapBuffer();						// 09.07.16	map 앞으로
	void	UndoActor();
	void	RedoActor();

};















#endif