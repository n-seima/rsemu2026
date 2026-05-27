#include "cBUILDING.H"
#include "cMAIN.H"
#include "cMAPWND.H"

int _cdecl	
cBUILDING_INFO::Compare( const void *a, const void *b)
{
	return *((WORD*)a)-*((WORD*)b);
//	return(stricmp(((cSTRING *)a)->String,((cSTRING *)b)->String));
}

//
//	리셋
void
cBUILDING_INFO::reset()
{
	m_isPutShadow		=	TRUE;
	m_isSelected		=	FALSE;
	m_wSerial			=	0xffff;
	m_wAddonBlockCount	=	0;
	m_wAddonObjectCount	=	0;	//	덧붙는 블럭의 수
	
	memset(m_aAddonBlock,0xff,sizeof(m_aAddonBlock));

	for (int i=0;i<dMAX_ADDON_OBJECT_COUNT_ON_BUILDING;i++)	m_aAddonObject[i].m_wObject	=	0xffff;
}

//
//	_iBlock이 체크 되어 있냐?
BOOL
cBUILDING_INFO::isCheckedBlock(int _iBlock)
{
	for (int i=0;i<m_wAddonBlockCount;i++)
		if (m_aAddonBlock[i]	==	_iBlock)	return	TRUE;

	return	FALSE;
}	//	cBUILDING_INFO::isCheckedBlock(int _iIndex)

//
//	_iBlock의 인덱스 구하기
int
cBUILDING_INFO::getBlockIndex(int _iBlock)
{
	for (int i=0;i<m_wAddonBlockCount;i++)
		if (m_aAddonBlock[i]	==	_iBlock)	return	i;

	return	0xffff;
}	//	cBUILDING_INFO::getBlockIndex(int _iIndex)

//
//	블럭 토글
void
cBUILDING_INFO::toggleBlock(int _iBlock)
{
	int	iBlockIndex	=	getBlockIndex(_iBlock);

	if (iBlockIndex	==	0xffff)
	{
		m_aAddonBlock[m_wAddonBlockCount++]	=	_iBlock;

		sortAddonBuildingBlock();

		return;
	}

	m_wAddonBlockCount--;

	memcpy(m_aAddonBlock+iBlockIndex,m_aAddonBlock+iBlockIndex+1,sizeof(WORD)*(m_wAddonBlockCount-iBlockIndex));

	sortAddonBuildingBlock();
}	//	cBUILDING_INFO::toggleBlock(int _iBlock)

//
//	cx,cy에 뭔가 건물이-_-?
BOOL
cBUILDING_INFO::isFocus(int cx,int cy,BYTE *_lpCrash)
{
	int		x			=	m_pos.x/dTILE_WIDTH;
	int		y			=	m_pos.y/dTILE_HEIGHT;
	int		xs			=	256;
	int		ys			=	256;
	int		xp			=	x-128;
	int		yp			=	y-128;
	int		bufferPoint	=	0,count,xPoint,size;

	int		dx			=	x-cx;
	int		dy			=	y-cy;

	if (dx < 0)	dx	=	-dx;
	if (dy < 0)	dy	=	-dy;

	if (dx > 10 || dy >10) return	FALSE;

	for(int j=0;j<ys;j++)
	{	count	=	_lpCrash[bufferPoint++];
		xPoint	=	xp;

		for(int i=0;i<count;i++)
		{	xPoint	+=	_lpCrash[bufferPoint++];
			size	=	_lpCrash[bufferPoint++];

			for (int pos=0;pos<size;pos++)
				if (xPoint + pos == cx && yp+j == cy) return	TRUE;

			bufferPoint	+=	size;
			xPoint		+=	size;
		}
	}

	return FALSE;
}	//	cBUILDING_INFO::isFocus(int cx,int cy,BYTE *_lpCrash)


//
//	에드온 오브젝트 붙여!!
BOOL
cBUILDING_INFO::addAddonObject(int _iObject,int _iX,int _iY)
{
	if (m_wAddonObjectCount	>=	dMAX_ADDON_OBJECT_COUNT_ON_BUILDING)	return	FALSE;

	m_aAddonObject[m_wAddonObjectCount].m_wObject	=	_iObject;
	m_aAddonObject[m_wAddonObjectCount].m_sDx		=	_iX-m_pos.x;
	m_aAddonObject[m_wAddonObjectCount].m_sDy		=	_iY-m_pos.y;

	m_wAddonObjectCount++;

	return	TRUE;
}	//	cBUILDING_INFO::addAddonObject(int _iObject,int _iX,int _iY)

//
//	에드온 오브젝트 제거!!
void
cBUILDING_INFO::removeAddonObject(int _iIndex)
{
	if (m_wAddonObjectCount	<=	0)	return;

	m_aAddonObject[_iIndex].m_wObject	=	0xffff;

	m_wAddonObjectCount--;
	
	if (_iIndex	<	dMAX_ADDON_OBJECT_COUNT_ON_BUILDING-1)
		memcpy(m_aAddonObject+_iIndex,m_aAddonObject+_iIndex+1,sizeof(cADD_ON_OBJECT)*(dMAX_ADDON_OBJECT_COUNT_ON_BUILDING-_iIndex-1));
}	//	cBUILDING_INFO::removeAddonObject(int _iIndex)

//
//	이동
void
cBUILDING_INFO::moveAddonObject(int _iIndex,int _iDx,int _iDy)
{
	if (m_aAddonObject[_iIndex].m_wObject	==	0xffff)	return;

	m_aAddonObject[_iIndex].m_sDx	+=	_iDx;
	m_aAddonObject[_iIndex].m_sDy	+=	_iDy;
}	//	cBUILDING_INFO::moveAddonObject(int _iIndex,int _iDx,int _iDy)

//
//	
void
cBUILDING_INFO::sortAddonBuildingBlock()
{
	qsort(m_aAddonBlock,m_wAddonBlockCount,sizeof(WORD) ,Compare);
}	//	cBUILDING_INFO::sortAddonBuildingBlock()

cBUILDING::cBUILDING()
{
	m_pData	=	NULL;
	reset();
}	//	cBUILDING::cBUILDING()

cBUILDING::~cBUILDING()
{
	close();
}	//	cBUILDING::~cBUILDING()

//
//	초기화
void
cBUILDING::reset()
{
	m_den.init(dMAX_BUILDING_COUNT_IN_MAP);
	m_iDataCount		=	0;
	m_iSelectAddonObject=	0;
}	//	cBUILDING::reset()

//
//	닫기
void
cBUILDING::close()
{
	m_den.close();
	pKILL(m_pData);

	m_iDataCount	=	0;
}	//	cBUILDING::close()

//
//	초기화
BOOL
cBUILDING::init(int _iTileSet,cNUX *_lpNux)
{
	if (!_lpNux)
	{
		if (!SetCurrentDirectory(_ms("%s/data/mapset/%s/Buildings",_logFolder,g_strTileSet[_iTileSet*2])))
			return ERRMSG("Error in cBUILDING::Init","Building data folder not found");
	}
	else
	{
	}

	cSTRINGS	*buildingList	=	cFOLDER::GetFiles("*.rbd",NULL,TRUE);//	화일들을 얻어 온다.
	
	close();	//	각종 데이터 초기화

	if (!buildingList)
	{	MSGOUT("'0' numbers building loaded");
		return TRUE;
	}

	m_pData			=	new	cOBJ_ANM	[buildingList->Count];
	m_iDataCount	=	0;

	cSTRING *fname	=	buildingList->Pick();

	for (int i=0;i<buildingList->Count;i++)
	{	if (m_pData[m_iDataCount].Load(fname->String,TRUE,_lpNux))	m_iDataCount++;
		fname	=	fname->pNEXT;
	}

	SetCurrentDirectory(_logFolder);

	KILL(buildingList);

	m_den.init(dMAX_BUILDING_COUNT_IN_MAP);

	return TRUE;
}	//	cBUILDING::init(int _iTileSet,cNUX *_lpNux)

//
//	건물 추가
int
cBUILDING::add(int _iX,int _iY,int _iBuilding)
{
	cBUILDING_INFO	*lpBuilding	=	m_den.getRookie();

	if (!lpBuilding)	return	0xffff;

	lpBuilding->reset();

	lpBuilding->m_wSerial	=	m_den.getRookieSerial();
	lpBuilding->m_pos.x		=	_iX;
	lpBuilding->m_pos.y		=	_iY;
	lpBuilding->m_wBuilding	=	_iBuilding;

	m_den.increaseCount();

	return	lpBuilding->m_wSerial;
}	//	cBUILDING::add(int _iX,int _iY,int _iBuilding)


//
//	찍어줘!!
void
cBUILDING::putRegisteredBuilding(int _iIndex,int _iX,int _iY,cOBJ_ANM *_lpObjectImage)
{
	cBUILDING_INFO	*lpBuilding	=	m_den.get(_iIndex);

	if (!lpBuilding)	return;

	int	iBuilding	=	lpBuilding->m_wBuilding,i;

	if (lpBuilding->m_isPutShadow)
	{
		m_pData[iBuilding].PutShadow(0,_iX,_iY);
		for (i=0;i<lpBuilding->m_wAddonBlockCount;i++)	m_pData[iBuilding].PutShadow(lpBuilding->m_aAddonBlock[i],_iX,_iY);
	}

	m_pData[iBuilding].Put(0,_iX,_iY);

	if (lpBuilding->m_isSelected)	m_pData[iBuilding].PutLayer(0,_iX,_iY);

	for (i=0;i<lpBuilding->m_wAddonBlockCount;i++)	m_pData[iBuilding].Put(lpBuilding->m_aAddonBlock[i],_iX,_iY);
	for (i=0;i<lpBuilding->m_wAddonObjectCount;i++)
	{
		int	iObject	=	lpBuilding->m_aAddonObject[i].m_wObject;
		_lpObjectImage[iObject].Put(0,_iX+lpBuilding->m_aAddonObject[i].m_sDx,_iY+lpBuilding->m_aAddonObject[i].m_sDy);

		if (lpBuilding->m_isPutShadow)
			_lpObjectImage[iObject].PutShadow(0,_iX+lpBuilding->m_aAddonObject[i].m_sDx,_iY+lpBuilding->m_aAddonObject[i].m_sDy);

		if (lpBuilding->m_isSelected)	
			if(m_iSelectAddonObject	==	i)
				_lpObjectImage[iObject].PutLayer(0,_iX+lpBuilding->m_aAddonObject[i].m_sDx,_iY+lpBuilding->m_aAddonObject[i].m_sDy);
	}
}	//	cBUILDING::put(int _iIndex)

//
//	찍어줘!!
void
cBUILDING::putRegisteredBuildingScale(int _iIndex,int _iX,int _iY,cOBJ_ANM *_lpObjectImage,int _iScale)
{
	cBUILDING_INFO	*lpBuilding	=	m_den.get(_iIndex);

	if (!lpBuilding)	return;

	int	iBuilding	=	lpBuilding->m_wBuilding,i;

	if (lpBuilding->m_isPutShadow)
	{
		m_pData[iBuilding].PutShadowScale(0,_iX,_iY,_iScale);
		for (i=0;i<lpBuilding->m_wAddonBlockCount;i++)	m_pData[iBuilding].PutShadowScale(lpBuilding->m_aAddonBlock[i],_iX,_iY,_iScale);
	}

	m_pData[iBuilding].PutScale(0,_iX,_iY,_iScale);
	if (lpBuilding->m_isSelected)	m_pData[iBuilding].PutLayerScale(0,_iX,_iY,_iScale);

	for (i=0;i<lpBuilding->m_wAddonBlockCount;i++)	m_pData[iBuilding].PutScale(lpBuilding->m_aAddonBlock[i],_iX,_iY,_iScale);
	for (i=0;i<lpBuilding->m_wAddonObjectCount;i++)
	{
		int	iObject	=	lpBuilding->m_aAddonObject[i].m_wObject;
		_lpObjectImage[iObject].PutScale(0,_iX+lpBuilding->m_aAddonObject[i].m_sDx,_iY+lpBuilding->m_aAddonObject[i].m_sDy,_iScale);

		if (lpBuilding->m_isPutShadow)
			_lpObjectImage[iObject].PutShadowScale(0,_iX+lpBuilding->m_aAddonObject[i].m_sDx,_iY+lpBuilding->m_aAddonObject[i].m_sDy,_iScale);
	}
}	//	cBUILDING::put(int _iIndex)

void
cBUILDING::put(int index,int x,int y,BOOL red)
{	if (index >= m_iDataCount ||  index < 0) return;

	if (red)	m_pData[index].PutRed(x,y);
	else		m_pData[index].Put(x,y);
}	//	cBUILDING::put(int index,int x,int y,BOOL red)

void
cBUILDING::putLayer(int index,int x,int y)
{	if (index >= m_iDataCount ||  index < 0) return;

	m_pData[index].PutLayer(x,y);
}	//	cBUILDING::putLayer(int index,int x,int y)

void
cBUILDING::put(int index,int x,int y,int xs,int ys)
{	if (index >= m_iDataCount) return;

	m_pData[index].FitPut(x,y,xs,ys);
}	//	cBUILDING::put(int index,int x,int y,int xs,int ys)

//
//	찍어주기(사이즈에 맞춤)
void
cBUILDING::put(int _iBuilding,int _iIndex,int _iX,int _iY,int _iXs,int _iYs)
{
	if (_iBuilding >= m_iDataCount) return;

	m_pData[_iBuilding].FitPut(_iIndex,_iX,_iY,_iXs,_iYs);
}	//	cBUILDING::put(int _iBuilding,int _iIndex,int _iX,int _iY,int _iXs,int _iYs)

//
//	_lpRect안에 건물 선택
void
cBUILDING::select(cRECT *_lpRect)		// 09.08.03	
{
	BOOL	isOne	=	FALSE;

	if (!KEY[dKEY_SHIFT]&&!KEY[dKEY_CONTROL])	freeSelectedAll();	//	쉬프트키 안 눌렀으면 선택된거 전부 취소

	if (_lpRect->size()	<	16)	isOne	=	TRUE;

	int	iMx		=	_lpRect->x1/dTILE_WIDTH;
	int	iMy		=	_lpRect->y1/dTILE_HEIGHT;

	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_BUILDING_COUNT_IN_MAP && iCount < m_den.getCount();iIndex++)
	{
		cBUILDING_INFO	*lpBuilding	=	m_den.get(iIndex);

		if(!lpBuilding)	continue;

		if (isOne)
		{
			BYTE *lpCrash	=	getCrashInfo(lpBuilding->m_wBuilding);

			if (lpBuilding->isFocus(iMx,iMy,lpCrash))	lpBuilding->m_isSelected	=	TRUE;

		}else{

			if (_lpRect->isIN(lpBuilding->m_pos.x,lpBuilding->m_pos.y)){
				if(KEY[dKEY_CONTROL]){
					lpBuilding->m_isSelected	=	FALSE;
				}else{
					lpBuilding->m_isSelected	=	TRUE;	
				}
			}
		}

		iCount++;
	}
}	//	cBUILDING::select(cRECT *_lpRect)

//
//	선택된거 전부 취소
void
cBUILDING::freeSelectedAll()
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_BUILDING_COUNT_IN_MAP && iCount < m_den.getCount();iIndex++)
	{
		cBUILDING_INFO	*lpBuilding	=	m_den.get(iIndex);

		if(!lpBuilding)	continue;

		lpBuilding->m_isSelected	=	FALSE;

		iCount++;
	}
}	//	cBUILDING::freeSelect()

//
//	선택된 건물 모두 제거
void
cBUILDING::removeSelectedAll()
{
	int	iIndex=0;

	for (iIndex=0;iIndex<m_den.getMaxCount();iIndex++)
	{
		cBUILDING_INFO	*lpBuilding	=	m_den.get(iIndex);

		if	(!lpBuilding)	continue;
		if	(!lpBuilding->m_isSelected)	continue;

		m_den.remove(iIndex);
	}
}	//	cBUILDING::removeSelectedBuilding()

//
//	_iMx,_iMy 근처에 있는 건물을 찾는다.
int
cBUILDING::getFocusBuilding(int _iMx,int _iMy)
{
	int	iIndex=0,iCount=0;

	for (iIndex=0,iCount=0;iIndex<dMAX_BUILDING_COUNT_IN_MAP && iCount < m_den.getCount();iIndex++)
	{
		cBUILDING_INFO	*lpBuilding	=	m_den.get(iIndex);

		if(!lpBuilding)	continue;

		BYTE *lpCrash	=	getCrashInfo(lpBuilding->m_wBuilding);

		if (lpBuilding->isFocus(_iMx,_iMy,lpCrash))	return	iIndex;

		iCount++;
	}

	return	0xffff;
}	//	cBUILDING::getFocusBuilding(int _iMx,int _iMy)