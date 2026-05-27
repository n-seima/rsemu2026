#include "URdoBuffer.h"


URdoBuffer::URdoBuffer(){
	m_pMap = NULL;

	memset(&m_sFixedObjectInfo,0,sizeof(sOBJECT_INFO));
	memset(&m_sTinyObjectInfo,0,sizeof(sOBJECT_INFO));
	memset(&m_sBuildingInfo,0,sizeof(sOBJECT_INFO));

}
URdoBuffer::~URdoBuffer(){
	m_pMap = NULL;

	deleteBuffer();
	
}
BOOL URdoBuffer::Init(cMAP* _pMap){

	if(_pMap == NULL){
		return FALSE;
	}

	m_pMap	=	_pMap;

	return TRUE;
	
}
void URdoBuffer::addBuffer(){	// 09.07.15
	
	addTileBuffer();
	addFixedObjectBuffer();		
	addTinyObjectBuffer();
	addBuildingBuffer();
	addMapBuffer();
	addActorBuffer();
	
}


void URdoBuffer::addTileBuffer(){			/// 09.07.15 타일버퍼추가.

	int xsize = m_pMap->m_pPattern->m_wXRange;
	int ysize = m_pMap->m_pPattern->m_wYRange;
	
	
	PattnSample		*patterns	= new PattnSample[xsize * ysize];
	
	
	memset(patterns, 0, sizeof(PattnSample)*xsize * ysize);
	
	
	
	memcpy(patterns, m_pMap->m_pPattern->m_pattns, sizeof(PattnSample)*xsize * ysize);
	
	
	while(m_deqPatternSampleBuffer.empty() != TRUE && m_itrPatternSampleBuffer+1 != m_deqPatternSampleBuffer.end()){
		
		std::deque<PattnSample*>::iterator itr = m_deqPatternSampleBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqPatternSampleBuffer.pop_back();
		
	}
	
	
	
	m_deqPatternSampleBuffer.push_back(patterns);
	
	
	
	if(m_deqPatternSampleBuffer.size() == 1){
		
		m_itrPatternSampleBuffer	= m_deqPatternSampleBuffer.begin();
		
	}else{
		++m_itrPatternSampleBuffer;
	}
	
	
	
	if(m_deqPatternSampleBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<PattnSample*>::iterator itr = m_deqPatternSampleBuffer.begin();		
		pKILL((*itr));
		m_deqPatternSampleBuffer.pop_front();
		
	}
	
}


void URdoBuffer::addFixedObjectBuffer(){			// 09.07.16 fixedObject버퍼 추가.

	
	int iObjectLastIndex					= m_pMap->m_object.m_denFixedObject.getLastIndex();
	int iRookie								= m_pMap->m_object.m_denFixedObject.getRookieSerial();
	int iCount								= m_pMap->m_object.m_denFixedObject.getCount();


	cFIXED_OBJECT_INFO*	pFixedObject		= new cFIXED_OBJECT_INFO[iObjectLastIndex+1];
	cFIXED_OBJECT_INFO* lpList				= m_pMap->m_object.m_denFixedObject.getList();
	
	
	
	memset(pFixedObject, 0, sizeof(cFIXED_OBJECT_INFO)*(iObjectLastIndex+1));
	
	memcpy(pFixedObject, lpList, sizeof(cFIXED_OBJECT_INFO)*(iObjectLastIndex+1));
	

	while(m_deqFixedObjectBuffer.empty() != TRUE && m_itrFixedObjectBuffer+1 != m_deqFixedObjectBuffer.end())
	{
		
		std::deque<cFIXED_OBJECT_INFO*>::iterator itr = m_deqFixedObjectBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqFixedObjectBuffer.pop_back();

		m_sFixedObjectInfo.deqSize.pop_back();
		m_sFixedObjectInfo.deqCount.pop_back();
		m_sFixedObjectInfo.deqRookie.pop_back();

		
	}
	
	m_deqFixedObjectBuffer.push_back(pFixedObject);
	
	m_sFixedObjectInfo.deqSize.push_back(iObjectLastIndex + 1);
	m_sFixedObjectInfo.deqCount.push_back(iCount);
	m_sFixedObjectInfo.deqRookie.push_back(iRookie);
	
	if(m_deqFixedObjectBuffer.size()==1){
		
		m_itrFixedObjectBuffer			= m_deqFixedObjectBuffer.begin();
		m_sFixedObjectInfo.itrSize		= m_sFixedObjectInfo.deqSize.begin();
		m_sFixedObjectInfo.itrCount		= m_sFixedObjectInfo.deqCount.begin();
		m_sFixedObjectInfo.itrRookie	= m_sFixedObjectInfo.deqRookie.begin();

		
	}else{
		++m_itrFixedObjectBuffer;
		
		++m_sFixedObjectInfo.itrSize;	
		++m_sFixedObjectInfo.itrCount;	
		++m_sFixedObjectInfo.itrRookie;


	}
	
	if(m_deqFixedObjectBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<cFIXED_OBJECT_INFO*>::iterator itr = m_deqFixedObjectBuffer.begin();		
		pKILL((*itr));
		m_deqFixedObjectBuffer.pop_front();
		
		m_sFixedObjectInfo.deqSize.pop_front();
		m_sFixedObjectInfo.deqCount.pop_front();
		m_sFixedObjectInfo.deqRookie.pop_front();
		
	}
	
	
	
}


void URdoBuffer::addTinyObjectBuffer(){			// 09.07.16 TinyObject버퍼 추가.
	

	int iObjectLastIndex				= m_pMap->m_object.m_denTinyObject.getLastIndex();
	int iRookie							= m_pMap->m_object.m_denTinyObject.getRookieSerial();
	int iCount							= m_pMap->m_object.m_denTinyObject.getCount();

	cTINY_OBJECT_INFO*	pTinyObject		= new cTINY_OBJECT_INFO[iObjectLastIndex+1];
	cTINY_OBJECT_INFO*	lpList			= m_pMap->m_object.m_denTinyObject.getList();
	

	memset(pTinyObject, 0, sizeof(cTINY_OBJECT_INFO)*(iObjectLastIndex+1));

	memcpy(pTinyObject, lpList, sizeof(cTINY_OBJECT_INFO)*(iObjectLastIndex+1));
	
	while(m_deqTinyObjectBuffer.empty() != TRUE && m_itrTinyObjectBuffer+1 != m_deqTinyObjectBuffer.end())
	{
		
		std::deque<cTINY_OBJECT_INFO*>::iterator itr = m_deqTinyObjectBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqTinyObjectBuffer.pop_back();

		m_sTinyObjectInfo.deqSize.pop_back();
		m_sTinyObjectInfo.deqCount.pop_back();
		m_sTinyObjectInfo.deqRookie.pop_back();
		
	}
	
	m_deqTinyObjectBuffer.push_back(pTinyObject);

	m_sTinyObjectInfo.deqSize.push_back(iObjectLastIndex + 1);
	m_sTinyObjectInfo.deqCount.push_back(iCount);
	m_sTinyObjectInfo.deqRookie.push_back(iRookie);
	
	
	if(m_deqTinyObjectBuffer.size()==1){
		
		m_itrTinyObjectBuffer			= m_deqTinyObjectBuffer.begin();
		m_sTinyObjectInfo.itrSize		= m_sTinyObjectInfo.deqSize.begin();
		m_sTinyObjectInfo.itrCount		= m_sTinyObjectInfo.deqCount.begin();
		m_sTinyObjectInfo.itrRookie		= m_sTinyObjectInfo.deqRookie.begin();
		
		
	}else{
		++m_itrTinyObjectBuffer;
		++m_sTinyObjectInfo.itrSize;	
		++m_sTinyObjectInfo.itrCount;	
		++m_sTinyObjectInfo.itrRookie;
	}
	
	if(m_deqTinyObjectBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<cTINY_OBJECT_INFO*>::iterator itr = m_deqTinyObjectBuffer.begin();		
		pKILL((*itr));
		m_deqTinyObjectBuffer.pop_front();
		m_sTinyObjectInfo.deqSize.pop_front();
		m_sTinyObjectInfo.deqCount.pop_front();
		m_sTinyObjectInfo.deqRookie.pop_front();
		
	}
	
	
}
void URdoBuffer::addBuildingBuffer(){			// 09.07.16 Building버퍼 추가.
	

	int iObjectLastIndex				= m_pMap->m_building.m_den.getLastIndex();
	int iRookie							= m_pMap->m_building.m_den.getRookieSerial();
	int iCount							= m_pMap->m_building.m_den.getCount();


	cBUILDING_INFO*	pBuilding		= new cBUILDING_INFO[iObjectLastIndex+1];
	cBUILDING_INFO*	lpList			= m_pMap->m_building.m_den.getList();
	

	memset(pBuilding, 0, sizeof(cBUILDING_INFO)*(iObjectLastIndex+1));

	memcpy(pBuilding, lpList, sizeof(cBUILDING_INFO)*(iObjectLastIndex+1));
	
	while(m_deqBuildingBuffer.empty() != TRUE && m_itrBuildingBuffer+1 != m_deqBuildingBuffer.end())
	{
		
		std::deque<cBUILDING_INFO*>::iterator itr = m_deqBuildingBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqBuildingBuffer.pop_back();
		m_sBuildingInfo.deqSize.pop_back();
		m_sBuildingInfo.deqCount.pop_back();
		m_sBuildingInfo.deqRookie.pop_back();
		
	}
	
	m_deqBuildingBuffer.push_back(pBuilding);

	m_sBuildingInfo.deqSize.push_back(iObjectLastIndex + 1);
	m_sBuildingInfo.deqCount.push_back(iCount);
	m_sBuildingInfo.deqRookie.push_back(iRookie);
	
	
	if(m_deqBuildingBuffer.size()==1){
		
		m_itrBuildingBuffer			= m_deqBuildingBuffer.begin();
		m_sBuildingInfo.itrSize		= m_sBuildingInfo.deqSize.begin();
		m_sBuildingInfo.itrCount		= m_sBuildingInfo.deqCount.begin();
		m_sBuildingInfo.itrRookie		= m_sBuildingInfo.deqRookie.begin();
		
		
	}else{
		++m_itrBuildingBuffer;
		++m_sBuildingInfo.itrSize;	
		++m_sBuildingInfo.itrCount;	
		++m_sBuildingInfo.itrRookie;
	}
	
	if(m_deqBuildingBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<cBUILDING_INFO*>::iterator itr = m_deqBuildingBuffer.begin();		
		pKILL((*itr));
		m_deqBuildingBuffer.pop_front();
		m_sBuildingInfo.deqSize.pop_front();
		m_sBuildingInfo.deqCount.pop_front();
		m_sBuildingInfo.deqRookie.pop_front();
		
	}

	
}


void URdoBuffer::addMapBuffer(){			// 09.07.16 맵버퍼 추가.
	
	int xsize = m_pMap->m_pPattern->m_wXRange;
	int ysize = m_pMap->m_pPattern->m_wYRange;
	
	//VV ----tile 맵
	LPWORD	pattern		= new WORD[xsize * ysize];

	memset(pattern, 0, sizeof(WORD)*xsize * ysize);
	memcpy(pattern, m_pMap->m_pPattern->m_pwPattnIndex, sizeof(WORD)*xsize * ysize);
	
	
	while(m_deqTileBuffer.empty() != TRUE && m_itrTileBuffer+1 != m_deqTileBuffer.end()){
		
		std::deque<LPWORD>::iterator itr = m_deqTileBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqTileBuffer.pop_back();
		
	}
	m_deqTileBuffer.push_back(pattern);
	
	if(m_deqTileBuffer.size()==1){
		
		m_itrTileBuffer			= m_deqTileBuffer.begin();
		
	}else{
		++m_itrTileBuffer;
	}
	
	
	if(m_deqTileBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<LPWORD>::iterator itr = m_deqTileBuffer.begin();		
		pKILL((*itr));
		m_deqTileBuffer.pop_front();
		
	}
	//AA ----tile 맵	
	
	WORD* pObjectMap	=	new WORD[m_pMap->m_iRealWidth*m_pMap->m_iRealHeight];
	
	
	memset(pObjectMap		,0xff	,m_pMap->m_iRealWidth*m_pMap->m_iRealHeight*2);
	
	memcpy(pObjectMap, m_pMap->m_pObjectMap, sizeof(WORD)*m_pMap->m_iRealWidth*m_pMap->m_iRealHeight);
	
	while(m_deqObjectMapBuffer.empty() != TRUE && m_itrObjectMapBuffer+1 != m_deqObjectMapBuffer.end())
	{
		
		std::deque<WORD*>::iterator itr = m_deqObjectMapBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqObjectMapBuffer.pop_back();
		
	}
	
	m_deqObjectMapBuffer.push_back(pObjectMap);
	
	if(m_deqObjectMapBuffer.size()==1){
		
		m_itrObjectMapBuffer			= m_deqObjectMapBuffer.begin();
		
	}else{
		++m_itrObjectMapBuffer;
	}
	
	if(m_deqObjectMapBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<WORD*>::iterator itr = m_deqObjectMapBuffer.begin();		
		pKILL((*itr));
		m_deqObjectMapBuffer.pop_front();
		
	}
	// AA --- ObjectMap

	//VV -----CrashMap
	BYTE* pCrashMap	=	new BYTE[m_pMap->m_iRealWidth*m_pMap->m_iRealHeight];
	
	
	memset(pCrashMap		,0	,m_pMap->m_iRealWidth*m_pMap->m_iRealHeight);
	
	memcpy(pCrashMap, m_pMap->m_pCrashMap, sizeof(BYTE)*m_pMap->m_iRealWidth*m_pMap->m_iRealHeight);
	
	while(m_deqCrashMapBuffer.empty() != TRUE && m_itrCrashMapBuffer+1 != m_deqCrashMapBuffer.end())
	{
		
		std::deque<BYTE*>::iterator itr = m_deqCrashMapBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqCrashMapBuffer.pop_back();
		
	}
	
	m_deqCrashMapBuffer.push_back(pCrashMap);
	
	if(m_deqCrashMapBuffer.size()==1){
		
		m_itrCrashMapBuffer			= m_deqCrashMapBuffer.begin();
		
	}else{
		++m_itrCrashMapBuffer;
	}
	
	if(m_deqCrashMapBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<BYTE*>::iterator itr = m_deqCrashMapBuffer.begin();		
		pKILL((*itr));
		m_deqCrashMapBuffer.pop_front();
		
	}
	//AA -----CrashMap

	//VV -----BuildingMap
	BYTE* pBuildingMap	=	new BYTE[m_pMap->m_iRealWidth*m_pMap->m_iRealHeight];
	
	
	memset(pBuildingMap ,0 , m_pMap->m_iRealWidth*m_pMap->m_iRealHeight);
	
	memcpy(pBuildingMap, m_pMap->m_pBuildingMap, sizeof(BYTE)*m_pMap->m_iRealWidth*m_pMap->m_iRealHeight);
	
	while(m_deqBuildingMapBuffer.empty() != TRUE && m_itrBuildingMapBuffer+1 != m_deqBuildingMapBuffer.end())
	{
		
		std::deque<BYTE*>::iterator itr = m_deqBuildingMapBuffer.end();		
		--itr;
		pKILL((*itr));
		m_deqBuildingMapBuffer.pop_back();
		
	}
	
	m_deqBuildingMapBuffer.push_back(pBuildingMap);
	
	if(m_deqBuildingMapBuffer.size()==1){
		
		m_itrBuildingMapBuffer			= m_deqBuildingMapBuffer.begin();
		
	}else{
		++m_itrBuildingMapBuffer;
	}
	
	if(m_deqBuildingMapBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<BYTE*>::iterator itr = m_deqBuildingMapBuffer.begin();		
		pKILL((*itr));
		m_deqBuildingMapBuffer.pop_front();
		
	}
	//AA -----BuildingMap	
}

void 
URdoBuffer::addActorBuffer()
{		
	

	cACTOR_SET*		pActor		= new cACTOR_SET;

	pActor->reset();
	memcpy(pActor, &m_pMap->m_setActor, sizeof(cACTOR_SET));
	
	if(m_pMap->m_setActor.m_iSelectedActorCount>0){
		memcpy(pActor->m_aSelectedActor, m_pMap->m_setActor.m_aSelectedActor, sizeof(int) * (dMAX_ACTOR_COUNT+1));
	}


	int iCount	= m_pMap->m_setActor.m_iCount;
	int iCnt	= 0;
	for(int i=0; i<dMAX_ACTOR_COUNT && iCnt <iCount; ++i)
	{
		
		cACTOR* pAc = m_pMap->m_setActor.getActor(i);

		if(!pAc)
			continue;
		++iCnt;
		if(!pAc->m_pCC)
			continue;

		pActor->m_aActor[i].deepCopy(pAc);

	}

	
	while(m_deqActorBuffer.empty() != TRUE && m_itrActorBuffer+1 != m_deqActorBuffer.end())
	{
		
		std::deque<cACTOR_SET*>::iterator itr = m_deqActorBuffer.end();		
		--itr;
		KILL((*itr));
		m_deqActorBuffer.pop_back();
		
	}
	
	m_deqActorBuffer.push_back(pActor);


	
	if(m_deqActorBuffer.size()==1){
		
		m_itrActorBuffer			= m_deqActorBuffer.begin();
	
		
	}else{
		++m_itrActorBuffer;
	}
	
	if(m_deqActorBuffer.size() > dEDIT_BUFFER_COUNT){
		
		std::deque<cACTOR_SET*>::iterator itr = m_deqActorBuffer.begin();		
		KILL((*itr));
		m_deqActorBuffer.pop_front();

	}

}

void URdoBuffer::deleteBuffer(){	// 09.07.15
	
	deleteTileBuffer();
	deleteFixedObjectBuffer();
	deleteTinyObjectBuffer();
	deleteBuildingBuffer();
	deleteMapBuffer();
	deleteActorBuffer();

	
}

void URdoBuffer::deleteTileBuffer(){		/// 09.07.15 타일패턴삭제.	
	
	m_itrPatternSampleBuffer = m_deqPatternSampleBuffer.begin();
	
	while(m_itrPatternSampleBuffer != m_deqPatternSampleBuffer.end()){
		
		pKILL((*m_itrPatternSampleBuffer));
		++m_itrPatternSampleBuffer;		
		
	}
	
	m_deqPatternSampleBuffer.clear();
	
}



void URdoBuffer::deleteFixedObjectBuffer(){		// 09.07.16 fixedObject버퍼 삭제.
	
	m_itrFixedObjectBuffer = m_deqFixedObjectBuffer.begin();
	
	while(m_itrFixedObjectBuffer != m_deqFixedObjectBuffer.end()){
		
		pKILL((*m_itrFixedObjectBuffer));
		++m_itrFixedObjectBuffer;
	
		
	}
	
	m_deqFixedObjectBuffer.clear();
	m_sFixedObjectInfo.deqSize.clear();
	m_sFixedObjectInfo.deqRookie.clear();
	m_sFixedObjectInfo.deqCount.clear();
	
	m_itrFixedObjectBuffer = m_deqFixedObjectBuffer.begin();

	m_sFixedObjectInfo.itrSize		=	m_sFixedObjectInfo.deqSize.begin();
	m_sFixedObjectInfo.itrRookie	=	m_sFixedObjectInfo.deqRookie.begin();
	m_sFixedObjectInfo.itrCount		=	m_sFixedObjectInfo.deqCount.begin();
	
}



void URdoBuffer::deleteTinyObjectBuffer(){		// 09.07.16 TinyObject버퍼 삭제.
	
	m_itrTinyObjectBuffer = m_deqTinyObjectBuffer.begin();
	
	while(m_itrTinyObjectBuffer != m_deqTinyObjectBuffer.end()){
		
		pKILL((*m_itrTinyObjectBuffer));
		++m_itrTinyObjectBuffer;
		
	}
	
	m_deqTinyObjectBuffer.clear();
	m_sTinyObjectInfo.deqSize.clear();
	m_sTinyObjectInfo.deqRookie.clear();
	m_sTinyObjectInfo.deqCount.clear();
	
	m_itrTinyObjectBuffer			= m_deqTinyObjectBuffer.begin();
	m_sTinyObjectInfo.itrSize		=	m_sTinyObjectInfo.deqSize.begin();
	m_sTinyObjectInfo.itrRookie		=	m_sTinyObjectInfo.deqRookie.begin();
	m_sTinyObjectInfo.itrCount		=	m_sTinyObjectInfo.deqCount.begin();
	
	
}

void URdoBuffer::deleteBuildingBuffer(){		// 09.07.16 Building버퍼 삭제.
	
	m_itrBuildingBuffer = m_deqBuildingBuffer.begin();
	
	while(m_itrBuildingBuffer != m_deqBuildingBuffer.end()){
		
		pKILL((*m_itrBuildingBuffer));
		++m_itrBuildingBuffer;
		
	}
	
	m_deqBuildingBuffer.clear();
	m_sBuildingInfo.deqSize.clear();
	m_sBuildingInfo.deqRookie.clear();
	m_sBuildingInfo.deqCount.clear();
	
	m_itrBuildingBuffer				= m_deqBuildingBuffer.begin();
	m_sBuildingInfo.itrSize			=	m_sBuildingInfo.deqSize.begin();
	m_sBuildingInfo.itrRookie		=	m_sBuildingInfo.deqRookie.begin();
	m_sBuildingInfo.itrCount		=	m_sBuildingInfo.deqCount.begin();
	
	
}

void URdoBuffer::deleteMapBuffer(){		// 09.07.16 맵버퍼 삭제.
	// VV TileMap
	m_itrTileBuffer = m_deqTileBuffer.begin();
	while(m_itrTileBuffer != m_deqTileBuffer.end()){
		
		pKILL((*m_itrTileBuffer));
		++m_itrTileBuffer;		
		
	}
	m_deqTileBuffer.clear();
	m_itrTileBuffer = m_deqTileBuffer.begin();
	// AA TIleMap

	// VV ObjectMap
	m_itrObjectMapBuffer = m_deqObjectMapBuffer.begin();
	while(m_itrObjectMapBuffer != m_deqObjectMapBuffer.end()){
		
		pKILL((*m_itrObjectMapBuffer));
		++m_itrObjectMapBuffer;		
		
	}

	m_deqObjectMapBuffer.clear();
	m_itrObjectMapBuffer = m_deqObjectMapBuffer.begin();
	// AA ObjectMap

	// VV CrashMap
	m_itrCrashMapBuffer = m_deqCrashMapBuffer.begin();
	while(m_itrCrashMapBuffer != m_deqCrashMapBuffer.end()){
		
		pKILL((*m_itrCrashMapBuffer));
		++m_itrCrashMapBuffer;		
		
	}

	m_deqCrashMapBuffer.clear();
	m_itrCrashMapBuffer = m_deqCrashMapBuffer.begin();
	// AA CrashMap
	
	//VV BuildingMap
	m_itrBuildingMapBuffer = m_deqBuildingMapBuffer.begin();
	while(m_itrBuildingMapBuffer != m_deqBuildingMapBuffer.end()){
		
		pKILL((*m_itrBuildingMapBuffer));
		++m_itrBuildingMapBuffer;		
		
	}

	m_deqBuildingMapBuffer.clear();
	m_itrBuildingMapBuffer = m_deqBuildingMapBuffer.begin();
	// AA BuildingMap

}

void
URdoBuffer::deleteActorBuffer()
{		/// 09.07.15 	
	
	int iSize	=m_deqActorBuffer.size();

	if(m_deqActorBuffer.size())
	{
		m_itrActorBuffer = m_deqActorBuffer.begin();
		
		while(m_itrActorBuffer != m_deqActorBuffer.end())
		{
			KILL((*m_itrActorBuffer));
			++m_itrActorBuffer;		
			
		}
		
		m_deqActorBuffer.clear();
		m_itrActorBuffer = m_deqActorBuffer.begin();
	}
}


void URdoBuffer::Undo(){									// 09.07.15 되돌리기
	
	UndoTile();
	UndoFixedObject();
	UndoTinyObject();
	UndoBuilding();
	UndoMapBuffer();
	UndoActor();


}

void URdoBuffer::UndoTile(){								// 09.07.15 되돌리기.
	
	
	if(m_itrPatternSampleBuffer	== m_deqPatternSampleBuffer.end()){
		return;
	}

	
	if(m_itrPatternSampleBuffer != m_deqPatternSampleBuffer.begin()){
		--m_itrPatternSampleBuffer;	
	}
		
	memcpy(m_pMap->m_pPattern->m_pattns,	(*m_itrPatternSampleBuffer), sizeof(PattnSample) * m_pMap->m_iHeight * m_pMap->m_iWidth);
	
	
	
}
void URdoBuffer::UndoFixedObject(){
	

	if(m_itrFixedObjectBuffer == m_deqFixedObjectBuffer.begin() || m_itrFixedObjectBuffer	== m_deqFixedObjectBuffer.end()){
		
		return;
		
	}else if(m_itrFixedObjectBuffer != m_deqFixedObjectBuffer.begin()){
		
		--m_itrFixedObjectBuffer;

		--m_sFixedObjectInfo.itrSize;
		--m_sFixedObjectInfo.itrRookie;
		--m_sFixedObjectInfo.itrCount;


		
	}
	
	int iLastIndex		= m_pMap->m_object.m_denFixedObject.getLastIndex();
	int iBufferSize		= (*m_sFixedObjectInfo.itrSize);

	cFIXED_OBJECT_INFO* lpList = m_pMap->m_object.m_denFixedObject.getList();

	cFIXED_OBJECT_INFO* lpList2 = (*m_itrFixedObjectBuffer);



	memcpy(lpList , lpList2, sizeof(cFIXED_OBJECT_INFO)*iBufferSize);

	m_pMap->m_object.m_denFixedObject.setRookie((*m_sFixedObjectInfo.itrRookie));
	m_pMap->m_object.m_denFixedObject.setCount((*m_sFixedObjectInfo.itrCount));

	for (int i = iBufferSize;i<=iLastIndex ;++i)
	{
		lpList[i].reset();
		
	}
	
	
}
void URdoBuffer::UndoTinyObject(){

	if(m_itrTinyObjectBuffer	== m_deqTinyObjectBuffer.begin() || 
		m_itrTinyObjectBuffer	== m_deqTinyObjectBuffer.end()){
		
		return;
		
	}else if(m_itrTinyObjectBuffer != m_deqTinyObjectBuffer.begin()){
		
		--m_itrTinyObjectBuffer;
		--m_sTinyObjectInfo.itrSize;
		--m_sTinyObjectInfo.itrRookie;
		--m_sTinyObjectInfo.itrCount;

		
	}
	
	int iLastIndex	=	m_pMap->m_object.m_denTinyObject.getLastIndex();
	int iBufferSize =	(*m_sTinyObjectInfo.itrSize);

	
	cTINY_OBJECT_INFO* lpList = m_pMap->m_object.m_denTinyObject.getList();
	cTINY_OBJECT_INFO* lpList2 = (*m_itrTinyObjectBuffer);


	memcpy(lpList , lpList2, sizeof(cTINY_OBJECT_INFO)*iBufferSize);

	m_pMap->m_object.m_denTinyObject.setRookie((*m_sTinyObjectInfo.itrRookie));
	m_pMap->m_object.m_denTinyObject.setCount((*m_sTinyObjectInfo.itrCount));
	
	
	for (int i = iBufferSize;i<=iLastIndex ;++i)
	{
		lpList[i].reset();
	}

}

void URdoBuffer::UndoBuilding(){

	if(m_itrBuildingBuffer	== m_deqBuildingBuffer.begin() || 
		m_itrBuildingBuffer	== m_deqBuildingBuffer.end()){
		
		return;
		
	}else if(m_itrBuildingBuffer != m_deqBuildingBuffer.begin()){
		
		--m_itrBuildingBuffer;
		--m_sBuildingInfo.itrSize;
		--m_sBuildingInfo.itrRookie;
		--m_sBuildingInfo.itrCount;

		
	}
	
	int iLastIndex	=	m_pMap->m_building.m_den.getLastIndex();
	int iBufferSize =	(*m_sBuildingInfo.itrSize);

	
	cBUILDING_INFO* lpList = m_pMap->m_building.m_den.getList();
	cBUILDING_INFO* lpList2 = (*m_itrBuildingBuffer);


	memcpy(lpList , lpList2, sizeof(cBUILDING_INFO)*iBufferSize);

	m_pMap->m_building.m_den.setRookie((*m_sBuildingInfo.itrRookie));
	m_pMap->m_building.m_den.setCount((*m_sBuildingInfo.itrCount));
	
	
	for (int i = iBufferSize;i<=iLastIndex ;++i)
	{
		lpList[i].reset();
	}

}


void URdoBuffer::UndoMapBuffer(){
	
	
	//VV ----tileMap
	if(m_itrTileBuffer == m_deqTileBuffer.begin() || m_itrTileBuffer == m_deqTileBuffer.end()){
		return;
	}
	--m_itrTileBuffer;
	if(m_itrTileBuffer != m_deqTileBuffer.begin()){
		
	}
	
	memcpy(m_pMap->m_pPattern->m_pwPattnIndex,	(*m_itrTileBuffer), sizeof(WORD)* m_pMap->m_iHeight * m_pMap->m_iWidth);
	//AA	---- TileMap

	//VV	----ObjectMap
	if(m_itrObjectMapBuffer == m_deqObjectMapBuffer.end()){	
		return;
	}

	if(m_itrObjectMapBuffer != m_deqObjectMapBuffer.begin()){
		--m_itrObjectMapBuffer;
	}
	
	memcpy(m_pMap->m_pObjectMap,	(*m_itrObjectMapBuffer), sizeof(WORD)	* m_pMap->m_iRealWidth * m_pMap->m_iRealHeight);	
	//AA	---- ObjectMap

	//VV	--- CrashMap
	if(m_itrCrashMapBuffer == m_deqCrashMapBuffer.end()){	
		return;
	}
	
	if(m_itrCrashMapBuffer != m_deqCrashMapBuffer.begin()){
		--m_itrCrashMapBuffer;
	}
	
	memcpy(m_pMap->m_pCrashMap,	(*m_itrCrashMapBuffer), sizeof(BYTE)	* m_pMap->m_iRealWidth * m_pMap->m_iRealHeight);	
	//AA --CrashMap

	//VV	----BuildingMap
	if(m_itrBuildingMapBuffer == m_deqBuildingMapBuffer.end()){	
		return;
	}
	
	if(m_itrBuildingMapBuffer != m_deqBuildingMapBuffer.begin()){
		--m_itrBuildingMapBuffer;
	}
	
	memcpy(m_pMap->m_pBuildingMap,	(*m_itrBuildingMapBuffer), sizeof(BYTE)	* m_pMap->m_iRealWidth * m_pMap->m_iRealHeight);	
	//AA	---BuildingMap

}

void URdoBuffer::UndoActor(){								// 09.07.15 되돌리기.

	if(m_itrActorBuffer	== m_deqActorBuffer.end()){
		return;
	}

	
	if(m_itrActorBuffer != m_deqActorBuffer.begin()){
		--m_itrActorBuffer;	
	}
	m_pMap->m_setActor.reset();	
	memcpy(&m_pMap->m_setActor,	(*m_itrActorBuffer), sizeof(cACTOR_SET));

	
	if((*m_itrActorBuffer)->m_iSelectedActorCount>0){
		memcpy(m_pMap->m_setActor.m_aSelectedActor, (*m_itrActorBuffer)->m_aSelectedActor, sizeof(int) * (dMAX_ACTOR_COUNT+1));
	}
	

	int iCount	= (*m_itrActorBuffer)->m_iCount;
	int iCnt	= 0;

	for(int i=0; i<dMAX_ACTOR_COUNT && iCnt <iCount; ++i)
	{
		
		cACTOR* pAc = (*m_itrActorBuffer)->getActor(i);

		if(!pAc)
			continue;

		++iCnt;
		if(!pAc->m_pCC)
			continue;

		m_pMap->m_setActor.m_aActor[i].deepCopy(pAc);


	}
		
	
}
void URdoBuffer::Redo(){									// 09.07.15 앞으로.
	
	RedoTile();
	RedoFixedObject();
	RedoTinyObject();
	RedoBuilding();
	RedoMapBuffer();
	RedoActor();

}




void URdoBuffer::RedoTile(){								// 09.07.15 앞으로.

	if(m_itrPatternSampleBuffer +1 == m_deqPatternSampleBuffer.end()){
		return;
	}
	
	++m_itrPatternSampleBuffer;
	
	memcpy(m_pMap->m_pPattern->m_pattns, (*m_itrPatternSampleBuffer), sizeof(PattnSample) * m_pMap->m_iHeight * m_pMap->m_iWidth);
	
}






void URdoBuffer::RedoFixedObject(){
	
	
	if(m_itrFixedObjectBuffer+1 == m_deqFixedObjectBuffer.end()){
		return;	
	}
	
	++m_itrFixedObjectBuffer;
	++m_sFixedObjectInfo.itrSize;
	++m_sFixedObjectInfo.itrRookie;
	++m_sFixedObjectInfo.itrCount;



	
	int iLastIndex	=	m_pMap->m_object.m_denFixedObject.getLastIndex();
	int iBufferSize =	(*m_sFixedObjectInfo.itrSize);

	
	cFIXED_OBJECT_INFO* lpList = m_pMap->m_object.m_denFixedObject.getList();
	m_pMap->m_object.m_denFixedObject.setRookie((*m_sFixedObjectInfo.itrRookie));
	m_pMap->m_object.m_denFixedObject.setCount((*m_sFixedObjectInfo.itrCount));
	memcpy(lpList , (*m_itrFixedObjectBuffer), sizeof(cFIXED_OBJECT_INFO)*iBufferSize);
	
	
	for (int i = iBufferSize;i<=iLastIndex ;++i)
	{
		lpList[i].reset();
		
	}
	
	
}

void URdoBuffer::RedoTinyObject(){

	if(m_itrTinyObjectBuffer+1 == m_deqTinyObjectBuffer.end()){
		return;	
	}

	
	++m_itrTinyObjectBuffer;
	++m_sTinyObjectInfo.itrSize;
	++m_sTinyObjectInfo.itrRookie;
	++m_sTinyObjectInfo.itrCount;
	
	int iLastIndex	=	m_pMap->m_object.m_denTinyObject.getLastIndex();
	int iBufferSize =	(*m_sTinyObjectInfo.itrSize);

	
	cTINY_OBJECT_INFO* lpList = m_pMap->m_object.m_denTinyObject.getList();
	m_pMap->m_object.m_denTinyObject.setRookie((*m_sTinyObjectInfo.itrRookie));
	m_pMap->m_object.m_denTinyObject.setCount((*m_sTinyObjectInfo.itrCount));
	memcpy(lpList , (*m_itrTinyObjectBuffer), sizeof(cTINY_OBJECT_INFO)*iBufferSize);
	
	
	for (int i = iBufferSize;i<=iLastIndex ;++i)
	{
		lpList[i].reset();
		
	}
	
}

void URdoBuffer::RedoBuilding(){

	if(m_itrBuildingBuffer+1 == m_deqBuildingBuffer.end()){
		return;	
	}

	
	++m_itrBuildingBuffer;
	++m_sBuildingInfo.itrSize;
	++m_sBuildingInfo.itrRookie;
	++m_sBuildingInfo.itrCount;
	
	int iLastIndex	=	m_pMap->m_building.m_den.getLastIndex();
	int iBufferSize =	(*m_sBuildingInfo.itrSize);

	
	cBUILDING_INFO* lpList = m_pMap->m_building.m_den.getList();
	m_pMap->m_building.m_den.setRookie((*m_sBuildingInfo.itrRookie));
	m_pMap->m_building.m_den.setCount((*m_sBuildingInfo.itrCount));
	memcpy(lpList , (*m_itrBuildingBuffer), sizeof(cBUILDING_INFO)*iBufferSize);
	
	
	for (int i = iBufferSize;i<=iLastIndex ;++i)
	{
		lpList[i].reset();
		
	}
	
}


void URdoBuffer::RedoMapBuffer(){

	// VV ---TileMap
	if(m_itrTileBuffer+1 == m_deqTileBuffer.end()){
		return;	
	}
	
	++m_itrTileBuffer;
	
	memcpy(m_pMap->m_pPattern->m_pwPattnIndex,	(*m_itrTileBuffer), sizeof(WORD)	* m_pMap->m_iHeight * m_pMap->m_iWidth);	
	// AA --- TileMap
	
	// VV --- ObjectMap
	if(m_itrObjectMapBuffer+1 == m_deqObjectMapBuffer.end()){
		return;	
	}
	
	++m_itrObjectMapBuffer;
	
	memcpy(m_pMap->m_pObjectMap,	(*m_itrObjectMapBuffer), sizeof(WORD)	* m_pMap->m_iHeight * m_pMap->m_iWidth);
	// AA --- ObjectMap
	
	// VV --- CrashMap
	if(m_itrCrashMapBuffer+1 == m_deqCrashMapBuffer.end()){
		return;	
	}
	
	++m_itrCrashMapBuffer;
	
	memcpy(m_pMap->m_pCrashMap,	(*m_itrCrashMapBuffer), sizeof(BYTE)	* m_pMap->m_iHeight * m_pMap->m_iWidth);
	// AA --- CrashMap
	
	// VV --- BuildingMap
	if(m_itrBuildingMapBuffer+1 == m_deqBuildingMapBuffer.end()){
		return;	
	}
	
	++m_itrBuildingMapBuffer;
	
	memcpy(m_pMap->m_pBuildingMap,	(*m_itrBuildingMapBuffer), sizeof(BYTE)	* m_pMap->m_iHeight * m_pMap->m_iWidth);
	// AA --- BuildingMap
}

void URdoBuffer::RedoActor(){								// 09.07.15 앞으로.

	if(m_itrActorBuffer +1 == m_deqActorBuffer.end()){
		return;
	}
	
	++m_itrActorBuffer;
	m_pMap->m_setActor.reset();
	memcpy(&m_pMap->m_setActor, (*m_itrActorBuffer), sizeof(cACTOR_SET));

	if((*m_itrActorBuffer)->m_iSelectedActorCount>0){
		memcpy(m_pMap->m_setActor.m_aSelectedActor, (*m_itrActorBuffer)->m_aSelectedActor, sizeof(int) * (dMAX_ACTOR_COUNT+1));
	}
	
	
	int iCount	= (*m_itrActorBuffer)->m_iCount;
	int iCnt	= 0;
	
	for(int i=0; i<dMAX_ACTOR_COUNT && iCnt <iCount; ++i)
	{
		
		cACTOR* pAc = (*m_itrActorBuffer)->getActor(i);
		
		if(!pAc)
			continue;
		++iCnt;
		if(!pAc->m_pCC)
			continue;

		
		m_pMap->m_setActor.m_aActor[i].deepCopy(pAc);
		
		
	}
}


BOOL 
URdoBuffer::isEqualBuffer()
{							/// 09.07.16 이전버퍼와 현재 버퍼 비교 .
	// return :
	//	FALSE = 현재버퍼와 이전버퍼는 다르다.
	//  TRUE  = 같다.

	int xsize = m_pMap->m_pPattern->m_wXRange;
	int ysize = m_pMap->m_pPattern->m_wYRange;
	

	if((*m_sFixedObjectInfo.itrCount) != m_pMap->m_object.getFixedObjectCount()){
		return FALSE;
	}

	if((*m_sTinyObjectInfo.itrCount) != m_pMap->m_object.getTinyObjectCount()){
		return FALSE;
	}

	if((*m_itrActorBuffer)->m_iCount != m_pMap->m_setActor.m_iCount){
		return FALSE;
	}
	
	
	int	iSelectObjectCount	=	m_pMap->m_object.getSelectedFixedObjectCount();			// 09.07.27 선택한오브젝트변경체크.
	int	iCount				=	0;
	for (int i=0;i<dMAX_OBJECT&& iCount < iSelectObjectCount;i++)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	m_pMap->m_object.m_denFixedObject.get(i);
		
		if	(!lpObject)
			continue;
				
		if	(!lpObject->m_isSelected)
			continue;
		++iCount;
		int iObject = (*m_itrFixedObjectBuffer)[i].m_wObject;
		if(iObject !=lpObject->m_wObject)
		{
			return FALSE;	
		}
		// insu add
		if((*m_itrFixedObjectBuffer)[i].m_wIsPutShadow != lpObject->m_wIsPutShadow)
			return FALSE;
		// insu add end
		for(int j=0;j<dMAX_ADD_ON_OBJECT_COUNT;++j)									// 09.07.29  오브젝트 에드온체크..
		{

			if( (*m_itrFixedObjectBuffer)[i].m_aAddonObject[j].m_sDx != lpObject->m_aAddonObject[j].m_sDx )
				return FALSE;
			if( (*m_itrFixedObjectBuffer)[i].m_aAddonObject[j].m_sDy != lpObject->m_aAddonObject[j].m_sDy )
				return FALSE;
			if( (*m_itrFixedObjectBuffer)[i].m_aAddonObject[j].m_wObject != lpObject->m_aAddonObject[j].m_wObject)
				return FALSE;
		}
	}

	int iActorCount = m_pMap->m_setActor.m_iCount;
	iCount = 0;
	for(int iActor=0;iActor<dMAX_ACTOR_COUNT && iCount<iActorCount ; ++iActor)
	{
		cACTOR* pAb = (*m_itrActorBuffer)->getActor(iActor);
		cACTOR* pAm = m_pMap->m_setActor.getActor(iActor);

		if(!pAm)
			continue;
		++iCount;
		if( pAb->m_pos.isSame(pAm->m_pos.x, pAm->m_pos.y) == FALSE)
			return FALSE;

	}

	if(m_deqTileBuffer.empty() != TRUE ){
		
		int x = m_pMap->m_pos.x;
		int y = m_pMap->m_pos.y;
		
		for(int i = y; i< y+g_MAPWND.TPH && i <m_pMap->m_iRealHeight; ++i){
			
			for(int j=x;j<x+g_MAPWND.TPW && j <m_pMap->m_iRealWidth; ++j){
				
				if(j<0 || j >= xsize){
					break;
				}
				int a = (*m_itrTileBuffer)[i*xsize+j];
				if((*m_itrTileBuffer)[i*xsize+j] != m_pMap->m_pPattern->GetMapIndex(j,i)){
					return FALSE;
				}
				if((*m_itrObjectMapBuffer)[i*xsize+j] != m_pMap->m_pObjectMap[i*xsize+j]){
					return FALSE;
				}
				if((*m_itrBuildingMapBuffer)[i*xsize+j] != m_pMap->m_pBuildingMap[i*xsize+j]){
					return FALSE;
				}
				if((*m_itrCrashMapBuffer)[i*xsize+j] != m_pMap->m_pCrashMap[i*xsize+j]){
					return FALSE;
				}
				
			}
		}	
	}
	
	return TRUE;
	
}
