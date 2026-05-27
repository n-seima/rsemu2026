#include "stdafx.h"
#include "cpattnctrl.hpp"

#include <stdio.h>

CPattnCtrl::CPattnCtrl() : CPattn()
{
	memset((BYTE*)this+sizeof(CPattn), 0, sizeof(CPattnCtrl)-sizeof(CPattn));
	m_iLayerCount	= 1;
	m_cpLayer[0]	= new CPattn;
	
	strcpy(m_bpNameStr, "레이어 01");
	strcpy(m_cpLayer[0]->m_bpNameStr, "레이어 01");

	m_pattns = NULL;

	m_Offset[0].x = -1;		m_Offset[0].y = 0;
	m_Offset[1].x = -1;		m_Offset[1].y = -1;
	m_Offset[2].x = 0;		m_Offset[2].y = -1;
	m_Offset[3].x = 1;		m_Offset[3].y = -1;
	m_Offset[4].x = 1;		m_Offset[4].y = 0;
	m_Offset[5].x = 1;		m_Offset[5].y = 1;
	m_Offset[6].x = 0;		m_Offset[6].y = 1;
	m_Offset[7].x = -1;		m_Offset[7].y = 1;

	m_wSampleCount = 0;

	m_bFlatPattn = true;
}

CPattnCtrl::~CPattnCtrl()
{
	if(m_pattns)		delete []m_pattns;
	if(m_pwPattnIndex)	delete []m_pwPattnIndex;
	for(int i=0; i<m_iLayerCount; i++)
		delete m_cpLayer[i];
	memset(GetCPattn(), 0, sizeof(CPattn));
}

BOOL CPattnCtrl::Resize(int _iX, int _iY, int _XRange, int _YRange)
{
//	새로 만들어서 초기화 시킨다.
	int NumIndex = _XRange*_YRange;

	LPWORD PattnIndex		= new WORD[NumIndex];
	LPPattnSample Pattns	= new PattnSample[NumIndex];
	if( (!PattnIndex) || (!Pattns) )
	{
		if(PattnIndex)	delete []PattnIndex;
		if(Pattns)		delete []Pattns;
		return false;
	}

	memset(PattnIndex, 0, NumIndex*2);
	memset(Pattns, 0, sizeof(PattnSample)*NumIndex);

	int ZeroFlagCount = GetPattnCount(0);
	if(!ZeroFlagCount)	ZeroFlagCount = 1;
	for(int i=0; i<NumIndex; i++)
		PattnIndex[i] = rand()%ZeroFlagCount;

//	타일이 자연스럽게 연결돼게 미리 준비한다.
	int RangeY = (_YRange<m_wYRange) ? _YRange : m_wYRange;
	int	iEndY	=	min(m_wYRange ,_YRange+_iY);
	int	iEndX	=	min(m_wXRange ,_XRange+_iX);
	int RangeX = (_XRange<m_wXRange) ? _XRange : m_wXRange;
	int reCot;

	LPBYTE lpFlags;

	if(_XRange+_iX > m_wXRange && _iX < m_wXRange)
	{
		for(int y=0; y<iEndY; y++)
		{
			lpFlags = mGetPattn_NoCheck(m_wXRange-1, y)->bLand;
			reCot = lpFlags[0]>>4;
			if( (lpFlags[1]>>4) > reCot )
				reCot = lpFlags[1]>>4;
			if( (lpFlags[2]>>4) > reCot )
				reCot = lpFlags[2]>>4;
			if( (lpFlags[3]>>4) > reCot )
				reCot = lpFlags[3]>>4;
			for(i=0; i<=reCot; i++)	
				PutCullCliff(m_wXRange-1, y);
		}
	}
	if(_YRange+_iY > m_wYRange && _iY < m_wYRange)
	{
		for(int x=0; x<iEndX; x++)
		{
			lpFlags = mGetPattn_NoCheck(x, m_wYRange-1)->bLand;
			reCot = lpFlags[0]>>4;
			if( (lpFlags[1]>>4) > reCot )
				reCot = lpFlags[1]>>4;
			if( (lpFlags[2]>>4) > reCot )
				reCot = lpFlags[2]>>4;
			if( (lpFlags[3]>>4) > reCot )
				reCot = lpFlags[3]>>4;
			for(i=0; i<=reCot; i++)	
				PutCullCliff(x, m_wYRange-1);
		}
	}
//	원래 있던 데이터를 복사한다.
	LPWORD lpSrcIndex			= PattnIndex;
	LPPattnSample lpSrcPattn	= Pattns;
	LPWORD lpDesIndex			= m_pwPattnIndex;
	LPPattnSample lpDesPattn	= m_pattns;	
	
	if(_iX <= m_wXRange && _iY <= m_wYRange)
	{
		for(int y=_iY;y<iEndY;++y)
		{
			int iSrcAddr	= (y - _iY)*_XRange;
			int iDestAddr	= y*m_wXRange + _iX;
			int iWidth		= min(_iX + _XRange, m_wXRange) - _iX;

			if(y < m_wYRange)
			{
				memcpy(lpSrcIndex+iSrcAddr, lpDesIndex+iDestAddr, iWidth*2);
				memcpy(lpSrcPattn+iSrcAddr, lpDesPattn+iDestAddr, iWidth*sizeof(PattnSample));	
			}			
		}
	}

	m_wXRange = _XRange;
	m_wYRange = _YRange;
	m_dwIndexCount = NumIndex;

	if(m_pwPattnIndex)	delete m_pwPattnIndex;
	if(m_pattns)		delete m_pattns;

	m_pwPattnIndex	= PattnIndex;
	m_pattns		= Pattns;

	return true;
}

void CPattnCtrl::SetStart(BYTE flag)
{
	int count = GetPattnCount(flag);

	if(!count)
		count = 1;

	for(int i=0; i<(int)m_dwIndexCount; i++)
		m_pwPattnIndex[i] = m_layerIndexRange[flag] + (rand()%count);
}

void CPattnCtrl::PutPattnIndex(int XRange, int YRange, BYTE bFlag, LPWORD lpwPattnIndex, LPWORD lpwHeight)
{
	IsFlat();

	m_wXRange = XRange;
	m_wYRange = YRange;
	m_dwIndexCount = XRange*YRange;

	DWORD dwSampleFlag = (m_bSampleFlag[bFlag]<<24)|(m_bSampleFlag[bFlag]<<16)|(m_bSampleFlag[bFlag]<<8)|(m_bSampleFlag[bFlag]);

	int i, n;

	if(m_pwPattnIndex)	delete m_pwPattnIndex;
	if(m_pattns)		delete m_pattns;
	
	m_pwPattnIndex	= new WORD[m_dwIndexCount];
	m_pattns		= new PattnSample[m_dwIndexCount];
	memset(m_pattns, 0, sizeof(PattnSample)*m_dwIndexCount);

	if(!m_pwPattnIndex)		return;
	if(!m_pattns)			return;

	for(i=0; i<(int)m_dwIndexCount; i++)
		m_pattns[i].dwLand = dwSampleFlag;

	if(!lpwPattnIndex)
	{
		SetStart(bFlag);
		lpwPattnIndex = m_pwPattnIndex;
	}

//	index를 설정하거...
	for(i=0; i<(int)m_dwIndexCount; i++)
	{
		for(n=1; n<m_iLayerCount; n++)
			if((lpwPattnIndex[i]>=m_layerIndexRange[n-1])&&(lpwPattnIndex[i]<m_layerIndexRange[n]))
				break;
		n--;
		m_pwPattnIndex[i] = (n<<8)|(lpwPattnIndex[i]-m_layerIndexRange[n]);
	}

//	PattnSample도 설정해야 함...
	int x, y, index;
	LPCPattn vCliff;

	for(i=0; i<(int)m_dwIndexCount; i++)
	{
		x = i%m_wXRange;
		y = i/m_wXRange;

		index = GetMapIndex(x, y);
		*(LPPattn)(m_pattns+i) = *GetPattn(index);

		vCliff = m_cpLayer[HIBYTE(index)];
		BYTE *land = m_pattns[i].bLand;

		WORD wHeight = (lpwHeight) ? lpwHeight[i] : 0;
		BYTE cliffID = BYTE( wHeight<<4 );

		if( (vCliff->m_bForm==dPATTN_CLIFF_BORDER) && ((wHeight>>8)!=NULL) )
		{
			for(n=0; n<4; n++)
				land[n] = ( ((wHeight&0x0f)+(((wHeight>>(8+n)))&1)) <<4 ) | (land[n]&0x0f);
			m_pattns[i].lpCliff = vCliff;
		}
		else
		{
			if( (vCliff->m_bForm==dPATTN_UNDER_BORDER) && ((wHeight>>8)!=NULL) )
			{
				BYTE low = vCliff->m_bLowFlag1;
				BYTE high = vCliff->m_bHighFlag1;
				for(n=0; n<4; n++)
					land[n] = ((wHeight>>(8+n))&1) ? high : low;
				m_pattns[i].lpUnderCliff = vCliff;
			}

			for(n=0; n<4; n++)
				land[n] = (m_bFlatPattn) ? land[n] : cliffID | (land[n]&0x0f);
		}
	}
}

void CPattnCtrl::GetPattnIndex(LPWORD lpwPattnIndex, LPWORD lpwHeight)
{
	IsFlat();

	BYTE *Land;
	WORD CliffID, x, y;
	LPPattnSample lpSample;

	for(int i=0; i<(int)m_dwIndexCount; i++)
	{
		x = i%m_wXRange;
		y = i/m_wXRange;

		lpwPattnIndex[i] = m_layerIndexRange[HIBYTE(m_pwPattnIndex[i])]+LOBYTE(m_pwPattnIndex[i]);

		lpSample	= mGetPattn(x, y);
		Land		= lpSample->bLand;

		CliffID = (Land[0]<Land[1]) ? Land[0] : ( (Land[1]<Land[0]) ? Land[1] : ((Land[2]<Land[0]) ? Land[2] : ((Land[3]<Land[0]) ? Land[3] : Land[0]) ) );
		CliffID = CliffID>>4;

		if(lpSample->lpCliff)
			for(int n=0; n<4; n++)
				CliffID = CliffID | ( ((Land[n]>>4)-CliffID) << (8+n) );

		if(lpSample->lpUnderCliff)
		{
			BYTE low = lpSample->lpUnderCliff->m_bLowFlag1;
			BYTE high = lpSample->lpUnderCliff->m_bHighFlag1;
			for(int n=0; n<4; n++)
				CliffID = CliffID | ( ((Land[n]&0x0f)==high) << (8+n) );
		}

		lpwHeight[i] = (m_bFlatPattn) ? 0 : CliffID;
	}
}

void CPattnCtrl::SetSelectLayer(int index)
{
	*m_cpLayer[m_iSelectLayerIndex]	= *GetCPattn();
	*GetCPattn()					= *m_cpLayer[index];
	m_iSelectLayerIndex = index;
}

void CPattnCtrl::AddNewLayer(BYTE form, BYTE flag1, BYTE flag2)
{
	m_cpLayer[m_iLayerCount] = new CPattn;
	sprintf(m_cpLayer[m_iLayerCount]->m_bpNameStr, "레이어 0%d", m_iLayerCount+1);
	m_iLayerCount++;
}

void CPattnCtrl::CopyLayer()
{
	m_cpLayer[m_iLayerCount] = new CPattn;
	sprintf(m_cpLayer[m_iLayerCount]->m_bpNameStr, "레이어 0%d", m_iLayerCount+1);

	LPBYTE lpImage = new BYTE[m_iPattnCount*dPATTN_IMAGE_SIZE];
	memcpy(lpImage, m_bpImage, m_iPattnCount*dPATTN_IMAGE_SIZE);
	m_cpLayer[m_iLayerCount]->InsertImage(lpImage, m_iImageHeight);
	delete []lpImage;

	LPPattn lpSrcPattn = m_cpLayer[m_iLayerCount]->m_cpPattn;
	LPPattn lpDesPattn = m_cpPattn;

	for(int i=0; i<m_iPattnCount; i++)
	{
		lpSrcPattn[i].bLandLT = lpDesPattn[i].bLandLT;
		lpSrcPattn[i].bLandRT = lpDesPattn[i].bLandRT;
		lpSrcPattn[i].bLandLB = lpDesPattn[i].bLandLB;
		lpSrcPattn[i].bLandRB = lpDesPattn[i].bLandRB;
	}

	m_cpLayer[m_iLayerCount]->m_bFlag[0] = m_bFlag[0];
	m_cpLayer[m_iLayerCount]->m_bFlag[1] = m_bFlag[1];
	m_cpLayer[m_iLayerCount]->m_bForm = m_bForm;
	m_cpLayer[m_iLayerCount]->m_bHeight = m_bHeight;
	m_cpLayer[m_iLayerCount]->m_range = m_range;

	m_iLayerCount++;
}

BOOL CPattnCtrl::Save(LPSTR filename)
{
	char buf[1024];

	m_layerIndexRange[0] = 0;
	for(int i=1; i<m_iLayerCount; i++)
		m_layerIndexRange[i] = m_layerIndexRange[i-1]+m_cpLayer[i-1]->m_iPattnCount;
	m_layerIndexRange[i] = m_layerIndexRange[i-1]+m_cpLayer[i-1]->m_iPattnCount;

	*m_cpLayer[m_iSelectLayerIndex] = *GetCPattn();

//Write File
	sprintf(buf, "%s.mpw", filename);
	HANDLE fp = CreateFile(buf, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	BOOL isSucess = (fp==INVALID_HANDLE_VALUE) ? FALSE : TRUE;
	DWORD aWrite;

	WriteFile(fp, &m_iLayerCount, 4, &aWrite, NULL);
	WriteFile(fp, m_layerIndexRange, 2*255, &aWrite, NULL);

	for(i=0; i<m_iLayerCount; i++)
		isSucess &= m_cpLayer[i]->SavePattn(fp);

	CloseHandle(fp);

//Read File
	sprintf(buf, "%s.mpr", filename);

	fp = CreateFile(buf, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	isSucess &= (fp==INVALID_HANDLE_VALUE) ? FALSE : TRUE;

	WORD	m_iPattnCount	= m_layerIndexRange[m_iLayerCount];

	char IDStr[40] = MPRHEADER;
	WriteFile(fp, IDStr, 40, &aWrite, NULL);

//	이미지는 4개로 쪼개어 져서 들어간다.
	WORD iPattnCount = m_iPattnCount;
	WriteFile(fp, &iPattnCount, 2, &aWrite, NULL);

	WORD ssX = dPATTN_XSIZE;
	WORD ssY = dPATTN_YSIZE;
	WriteFile(fp, &ssX, 2, &aWrite, NULL);
	WriteFile(fp, &ssY, 2, &aWrite, NULL);

	for(i=0; (isSucess) && (i<m_iLayerCount); i++)
	{
		LPBYTE lpImage = m_cpLayer[i]->m_bpImage;

		isSucess &= WriteFile(fp, lpImage, m_cpLayer[i]->m_iPattnCount*dPATTN_IMAGE_SIZE, &aWrite, NULL);
	}

	CloseHandle(fp);

	return isSucess;
}

BOOL CPattnCtrl::IsFlat()
{
	m_bFlatPattn = true;

	for(int i=-1; ++i<m_iLayerCount; )
		m_bFlatPattn &= (m_cpLayer[i]->m_bHighFlag1==m_cpLayer[i]->m_bLowFlag1) &&
							(m_cpLayer[i]->m_bFlag[0]==m_cpLayer[i]->m_bFlag[1]);

	return m_bFlatPattn;
}

BOOL CPattnCtrl::Load(LPSTR filename)
{
	char buf[1024];
	sprintf(buf, "%s.mpw", filename);

	HANDLE fp = CreateFile(buf, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	BOOL isSucess = TRUE;
	DWORD aRead;

	ReadFile(fp, &m_iLayerCount, 4, &aRead, NULL);
	ReadFile(fp, m_layerIndexRange, 2*255, &aRead, NULL);

	for(int i=0; i<m_iLayerCount; i++)
	{
		if(m_cpLayer[i])	delete m_cpLayer[i];

		m_cpLayer[i]	=	new CPattn;

		isSucess &= m_cpLayer[i]->LoadPattn(fp);
	}

	m_bFlatPattn = true;

	for(i=-1; ++i<m_iLayerCount; )
		m_bFlatPattn &= (m_cpLayer[i]->m_bHighFlag1==m_cpLayer[i]->m_bLowFlag1) &&
							(m_cpLayer[i]->m_bFlag[0]==m_cpLayer[i]->m_bFlag[1]);

	m_iSelectLayerIndex = 0;
	*GetCPattn() = *m_cpLayer[0];

	CloseHandle(fp);

	SetFriend();

	return isSucess;
}

void CPattnCtrl::MoveLast()
{
	int algnCount = m_iLayerCount-1;
	for(int i=m_iSelectLayerIndex; i<algnCount; i++)
		*m_cpLayer[i] = *m_cpLayer[i+1];
	*m_cpLayer[i] = *GetCPattn();
	*GetCPattn() = *m_cpLayer[m_iSelectLayerIndex];
}

// 기본적으로 절벽을 확장한다.
// 왜냐면 절벽속성과 먼속성일경우 깨질수 있으므로 미리 확장한다.
void CPattnCtrl::PutExpandCliff(int x, int y, PattnSample &sample)
{
	BYTE height = sample.bLandLT & 0xf0;
	LPPattnSample lpCliff;

	if(!sample.lpCliff)
	{
		lpCliff = mGetPattn(x, y);
		if(lpCliff->lpCliff)
		{
			BYTE flag = height|(lpCliff->bLandLT&0x0f);
			PattnSample expand = {flag, flag, flag, flag};
			PutTile(x, y, expand);
		}

		for(int i=0; i<8; i++)
		{
			lpCliff = mGetPattn(x+m_Offset[i].x, y+m_Offset[i].y);
			if(!lpCliff->lpCliff)	continue;

			BYTE flag = height|(lpCliff->bLandLT&0x0f);
			PattnSample expand = {flag, flag, flag, flag};
			PutTile(x+m_Offset[i].x, y+m_Offset[i].y, expand);
		}
	}
}

struct RePutList{
	int x, y;
	PattnSample pattnSample;
	RePutList	*pNext;
};

RePutList	freeList[200], *RePut = freeList;

void CPattnCtrl::RePutTile()
{
	return;

	for(RePutList *pList=RePut->pNext; pList; pList=pList->pNext)
		PutTile(pList->x, pList->y, pList->pattnSample);
}

void CPattnCtrl::AddRePutList(int x, int y, PattnSample &Insert)
{
	for(RePutList *pList=RePut; (pList->pNext); pList=pList->pNext);
	pList->pNext = pList+1;
	pList = pList->pNext;

	pList->x			= x;
	pList->y			= y;
	pList->pattnSample	= Insert;
	pList->pNext		= NULL;
}

void CPattnCtrl::PutCliff(int x, int y)
{
	RePut->pNext = NULL;

	if( IsFlat() )
		return;

	if( (x<0) || (y<0) || (x>=m_wXRange) || (y>=m_wYRange) )	return;

	BYTE flag = 0xff;
	for(int i=0; i<m_iLayerCount; i++)
	{
		if(m_cpLayer[i]->m_bForm!=dPATTN_CLIFF_BORDER)	continue;

		flag = m_cpLayer[i]->m_bFlag[0];
		break;
	}
	if(flag==0xff)					return;

	SetRect(&m_hillRng, x-10, y-20, x+10, y+20);	

	PattnSample sample	= Flag2Sample(x, y, flag);
	int cliffID			= sample.bLandLT&0xf0;
	int newID			= ((cliffID>>4)+1) << 4;
	int bottom			= sample.lpCliff->m_range.bottom;

	if( ((newID>>4)<0) || ((newID>>4)>7) )	return;

	PattnSample &oldPattn	= sample;
	PattnSample  newPattn	= {newID|flag, newID|flag, newID|flag, newID|flag, };
	
	//터를 닦아 놓는다.
	PutTile(x, y, oldPattn);

	for(i=0; i<8; i++)
		PutTile(x+m_Offset[i].x, y+m_Offset[i].y, oldPattn);

	PutTile(x, y-2, oldPattn);
	PutTile(x-1, y-2, oldPattn);
	PutTile(x+1, y-2, oldPattn);

	for(i=0; i<bottom-1; i++)
	{
		PutTile(x, y+2+i, oldPattn);
		PutTile(x-1, y+2+i, oldPattn);
		PutTile(x+1, y+2+i, oldPattn);
	}

	PutTile(x, y, newPattn);

	//RePutTile();

	SetCliffHill(x, y);
}

void CPattnCtrl::PutCullCliff(int x, int y)
{
	RePut->pNext = NULL;

	if( IsFlat() )
		return;

	if( (x<0) || (y<0) || (x>=m_wXRange) || (y>=m_wYRange) )	return;

	BYTE flag = 0xff;
	for(int i=0; i<m_iLayerCount; i++)
	{
		if(m_cpLayer[i]->m_bForm!=dPATTN_CLIFF_BORDER)	continue;
		flag = m_cpLayer[i]->m_bFlag[0];
		break;
	}
	if(flag==0xff)			return;

	if(i==m_wSampleCount)	return;

	SetRect(&m_hillRng, x-10, y-20, x+10, y+20);

 	PattnSample sample	= Flag2Sample(x, y, flag, true);

	PutTile(x, y, sample);

	for(i=0; i<4; i++)
		if( (sample.bLand[i]>>4)!=0 )
			sample.bLand[i] = ((sample.bLand[i]>>4)-1)<<4 | (sample.bLand[i]&0x0f);

	PutExpandCliff(x, y, sample);

	PutTile(x, y, sample);	

	SetCliffHill(x, y);
}

void CPattnCtrl::PutTile(int x, int y, BYTE flag, BOOL IsExpand)
{
	RePut->pNext = NULL;

	if(IsFlat())
	{
		PattnSample sample	= Flag2Sample(x, y, flag, IsExpand);
		SetMapSample(x, y, sample);
	}
	else
	{
		if( (x<0) || (y<0) || (x>=m_wXRange) || (y>=m_wYRange) )
			return;

		for(int i=0; i<m_wSampleCount; i++)
			if(flag==m_bSampleFlag[i])
				break;

		if(i==m_wSampleCount)
			return;

		SetRect(&m_hillRng, x-10, y-20, x+10, y+20);

		PattnSample sample	= Flag2Sample(x, y, flag, IsExpand);

		if(IsExpand)
			PutExpandCliff(x, y, sample);

		if(!sample.lpUnderCliff)
			PutTile(x, y, sample);
		else
		{
			PutFriend(x, y, &sample);
			PutTileUnder(x, y, &sample);
		}

		//RePutTile();

		SetCliffHill(x, y);
	}
}

//////////////////////////////////////////////////////////////////////////////////////
void CPattnCtrl::PutTile(int x, int y, PattnSample &sample)
{
	if( (x<0) || (y<0) || (x>=m_wXRange) || (y>=m_wYRange) )	return;

	PutFriend(x, y, &sample);

	SetMapSample(x, y, sample);

 	for(int i=0; i<8; i++)
 		PutTile(x+m_Offset[i].x, y+m_Offset[i].y, &sample, i);
}

//	이웃한 속성과,... 그리고 샘플 플래그를 등록한다.
void CPattnCtrl::SetFriend()
{
//	일단 속성별루 friend를 모조리 기록하구...
	//	friend기록할시에 앞의 2개는 절벽friend이다.
	int i, n, k;
	LPSTR lpff[256];

	for(i=0; i<256; i++)
		for(n=0; n<256; n++)
			m_lpff[i][n] = -1;

	for(i=0; i<256; i++)
		lpff[i] = m_lpff[i];

	m_wSampleCount = 0;
	memset(m_bSampleFlag, 0, 20);

	BYTE isCliff[8] = {0, };
	for(i=0; i<m_iLayerCount; i++)
	{
		switch(m_cpLayer[i]->m_bForm)
		{
		case dPATTN_CLIFF_BORDER:	isCliff[m_cpLayer[i]->m_bLowFlag1] = true;	break;

		case dPATTN_NORMAL:
			if(!m_cpLayer[i]->IsBorder()) 
				m_bSampleFlag[m_wSampleCount++] = m_cpLayer[i]->m_bFlag[0];
			break;
		}
	}

	for(n=0; n<8; n++)
	{
		if(isCliff[n])
		{
			lpff[n][0] = 8<<4 | n;
			lpff[n][1] = 1<<4 | n;
		}
		lpff[n]++;
	}
	for(i=1; i<8; i++)
	{
		for(n=0; n<8; n++)
		{
			if(isCliff[n])
			{
				lpff[i<<4|n][0] = (i-1)<<4 | n;
				lpff[i<<4|n][1] = (i+1)<<4 | n;
			}
			lpff[i<<4|n]++;
		}
	}

	for(i=0; i<m_iLayerCount; i++)
		if( (m_cpLayer[i]->IsBorder()) && (m_cpLayer[i]->m_bForm!=dPATTN_CLIFF_BORDER) )
			for(int n=0; n<2; n++)
				*(++lpff[m_cpLayer[i]->m_bFlag[n]])=m_cpLayer[i]->m_bFlag[1-n];

	for(i=0; i<8; i++)
		for(lpff[i]=m_lpff[i]+2, n=1; n<8; n++)
			for(k=2; lpff[i][0]!=-1; lpff[i]++, k++)
				m_lpff[n<<4|i][k] = lpff[i][0] | n<<4;

	for(n=1; n<8; n++)
		for(i=0; i<8; i++)
			for(k=2; k<8; k++)
				m_lpff[n<<4|i][k] = n<<4 | m_lpff[i][k];

	for(i=0; i<m_iLayerCount; i++)
		m_cpLayer[i]->SetRandom();
}

BOOL CPattnCtrl::PutFriend(int x, int y, LPPattnSample lpPattn)
{
	//친구가 별루 없는 녀석은 일단 친구를 만나게 해주거...
	char oneFriend = lpPattn->bLandLT;
	BYTE *land = mGetPattn(x, y)->bLand;
	for(int i=0; i<4; i++)
	{
		if(m_lpff[land[i]][3]!=-1)			continue;

		oneFriend = m_lpff[land[i]][2];
		break;
	}

	if(lpPattn->bLandLT==oneFriend)
		if(m_lpff[oneFriend][3]==-1)
			oneFriend = m_lpff[oneFriend][2];

	if(	( CullCliff(lpPattn->bLandLT)==CullCliff(oneFriend) )||
		( CullCliff(lpPattn->bLandLT)==CullCliff(land[i]) )	)
		return FALSE;

	PattnSample sample = {oneFriend, oneFriend, oneFriend, oneFriend, };

	PutTile(x, y, sample);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//lpPutPattn는 이미 먼저 put돼었던 타일이다.
void CPattnCtrl::PutTile(int x, int y, LPPattnSample lpPutPattn, int Bang)
{
	if( (x<0) || (y<0) || (x>=m_wXRange) || (y>=m_wYRange) )	return;

	if(x<m_hillRng.left)	m_hillRng.left=x-1;
	if(x>=m_hillRng.right)	m_hillRng.right=x+2;
	if(y<m_hillRng.top)		m_hillRng.top=y-1;
	if(y>=m_hillRng.bottom)	m_hillRng.bottom=y+2;

	BYTE flag1, flag2, sampleFlag1, sampleFlag2;

	LPPattnSample lpInitSample = mGetPattn(x, y);

	//일단 방향별루 사용돼는 찍을 패턴과 원 패턴을 가져와보구...
	GetTMFlag(x, y, Bang, lpPutPattn, flag1, flag2, sampleFlag1, sampleFlag2);

	//채울 flag를 결정하거...
	BYTE fillFlag[2] = { flag1, (flag1!=flag2) ? flag2 : GetNearFlag(x, y, Bang, flag1) };

	PattnSample PutSample = *lpPutPattn;
	BOOL isWorking = SetBangPattn(x, y, PutSample, Bang, fillFlag);

	SetMapSample(x, y, PutSample);

	for(int i=0; i<8; i++)
		if( (isWorking&(1<<i)) != NULL )
			PutTile(x+m_Offset[i].x, y+m_Offset[i].y, &PutSample, i);

	SetUnderCliff(x, y, Bang);
	SetCliff(x, y, Bang, fillFlag[0]&0xf0);
}

/////////////////////////////////////////////////////////////////////////////////////////
//									Protected!!!

//	정해진 fillFlag로 방향별로 채울뿐만 아니라... 이미 채울 필요없이 같다면 FALSE를 리턴...

//	만들어진 타일속성을 방향별로 찍어야 됄곳에 찍는다.
BOOL CPattnCtrl::SetBangPattn(int x, int y, PattnSample &PS, BYTE Bang, LPBYTE fillFlag)
{
	BOOL Working = 0;

	PattnSample &OS = *mGetPattn(x, y);

	switch(Bang)
	{
	case dPUT_LEFT:	PS.bLandRT = PS.bLandLT;		PS.bLandRB = PS.bLandLB;	break;
	case dPUT_TOP:	PS.bLandLB = PS.bLandLT;		PS.bLandRB = PS.bLandRT;	break;
	case dPUT_RIGHT:	PS.bLandLT = PS.bLandRT;	PS.bLandLB = PS.bLandRB;	break;
	case dPUT_BOTTOM:	PS.bLandLT = PS.bLandLB;	PS.bLandRT = PS.bLandRB;	break;
	case dPUT_LT:	PS.bLandRB = PS.bLandLT;		break;
	case dPUT_RT:	PS.bLandLB = PS.bLandRT;		break;
	case dPUT_RB:	PS.bLandLT = PS.bLandRB;		break;
	case dPUT_LB:	PS.bLandRT = PS.bLandLB;		break;
	}

#define dSETBANGPATTN(BLAND) PS.BLAND = (OS.BLAND==fillFlag[0]) ? fillFlag[0] : fillFlag[1]
	switch(Bang)
	{
	case dPUT_LEFT:		dSETBANGPATTN(bLandLT);	dSETBANGPATTN(bLandLB);	break;
	case dPUT_TOP:		dSETBANGPATTN(bLandLT);	dSETBANGPATTN(bLandRT);	break;
	case dPUT_RIGHT:	dSETBANGPATTN(bLandRT);	dSETBANGPATTN(bLandRB);	break;
	case dPUT_BOTTOM:	dSETBANGPATTN(bLandLB);	dSETBANGPATTN(bLandRB);	break;
	case dPUT_LT:	dSETBANGPATTN(bLandLT);	dSETBANGPATTN(bLandLB);	dSETBANGPATTN(bLandRT);	break;
	case dPUT_RT:	dSETBANGPATTN(bLandRT);	dSETBANGPATTN(bLandLT);	dSETBANGPATTN(bLandRB);	break;
	case dPUT_RB:	dSETBANGPATTN(bLandRB);	dSETBANGPATTN(bLandRT);	dSETBANGPATTN(bLandLB);	break;
	case dPUT_LB:	dSETBANGPATTN(bLandLB);	dSETBANGPATTN(bLandRB);	dSETBANGPATTN(bLandLT);	break;
	}

#define dWORKING(BANG)					\
	switch(BANG)						\
	{									\
	case dPUT_LEFT:		if( (PS.bLandLT!=OS.bLandLT) || (PS.bLandLB!=OS.bLandLB) )	Working |= dMaskLeft;	break;	\
	case dPUT_TOP:		if( (PS.bLandLT!=OS.bLandLT) || (PS.bLandRT!=OS.bLandRT) )	Working |= dMaskTop;	break;	\
	case dPUT_RIGHT:	if( (PS.bLandRT!=OS.bLandRT) || (PS.bLandRB!=OS.bLandRB) )	Working |= dMaskRight;	break;	\
	case dPUT_BOTTOM:	if( (PS.bLandLB!=OS.bLandLB) || (PS.bLandRB!=OS.bLandRB) )	Working |= dMaskBottom;	break;	\
	}
	switch(Bang)
	{
	case dPUT_LT:	if( PS.bLandLT != OS.bLandLT )	Working |= dMaskLT;	dWORKING(dPUT_LEFT);	dWORKING(dPUT_TOP);		break;
	case dPUT_RT:	if( PS.bLandRT != OS.bLandRT )	Working |= dMaskRT;	dWORKING(dPUT_RIGHT);	dWORKING(dPUT_TOP);		break;
	case dPUT_RB:	if( PS.bLandRB != OS.bLandRB )	Working |= dMaskRB;	dWORKING(dPUT_RIGHT);	dWORKING(dPUT_BOTTOM);	break;
	case dPUT_LB:	if( PS.bLandLB != OS.bLandLB )	Working |= dMaskLB;	dWORKING(dPUT_LEFT);	dWORKING(dPUT_BOTTOM);	break;
	default:	dWORKING(Bang);
	}

	return Working;
}

// 원타일속성들과 새로찍혀질 타일속성중 방향별로 꼭 찍혀야 돼는 부분의 속성2개를 추출한다.
void CPattnCtrl::GetTMFlag(int x, int y, BYTE Bang, LPPattnSample lpPutPattn, BYTE &flag1, BYTE &flag2, BYTE &sampleFlag1, BYTE &sampleFlag2)
{
	LPPattnSample lpSample = mGetPattn(x, y);

	switch(Bang)
	{
	case dPUT_LEFT:		flag1 = lpPutPattn->bLandLB;		flag2 = lpPutPattn->bLandLT;
						sampleFlag1 = lpSample->bLandRB;	sampleFlag2 = lpSample->bLandRT;
						break;
	case dPUT_LT:		flag1 = lpPutPattn->bLandLT;		flag2 = lpPutPattn->bLandLT;
						sampleFlag1 = lpSample->bLandRB;	sampleFlag2 = lpSample->bLandRB;
						break;
	case dPUT_TOP:		flag1 = lpPutPattn->bLandLT;		flag2 = lpPutPattn->bLandRT;
						sampleFlag1 = lpSample->bLandLB;	sampleFlag2 = lpSample->bLandRB;
						break;
	case dPUT_RT:		flag1 = lpPutPattn->bLandRT;		flag2 = lpPutPattn->bLandRT;
						sampleFlag1 = lpSample->bLandLB;	sampleFlag2 = lpSample->bLandLB;
						break;
	case dPUT_RIGHT:	flag1 = lpPutPattn->bLandRT;		flag2 = lpPutPattn->bLandRB;
						sampleFlag1 = lpSample->bLandLT;	sampleFlag2 = lpSample->bLandLB;
						break;
	case dPUT_RB:		flag1 = lpPutPattn->bLandRB;		flag2 = lpPutPattn->bLandRB;
						sampleFlag1 = lpSample->bLandLT;	sampleFlag2 = lpSample->bLandLT;
						break;
	case dPUT_BOTTOM:	flag1 = lpPutPattn->bLandRB;		flag2 = lpPutPattn->bLandLB;
						sampleFlag1 = lpSample->bLandRT;	sampleFlag2 = lpSample->bLandRB;
						break;
	case dPUT_LB:		flag1 = lpPutPattn->bLandLB;		flag2 = lpPutPattn->bLandLB;
						sampleFlag1 = lpSample->bLandRT;	sampleFlag2 = lpSample->bLandRT;
						break;
	}
}

//	새로 찍혀질 타일속성을 결정한다.
BYTE CPattnCtrl::GetNearFlag(int x, int y, BYTE Bang, BYTE flag)
{
	int i, n;

//**** 여기 저기 보이는 +2는 절벽 플래그를 검색하지 않게 하기 위해서다.

	//flag와 이웃돼는 지형을 뽑아낸다.
	char *FFlags = m_lpff[flag]+2;
	
	POINT pos[4]		= { {x-1, y+1}, {x-1, y-1}, {x+1, y-1}, {x+1, y+1} };
	BYTE lands[4]		= {dLandRT, dLandRB, dLandLB, dLandLT};

	for(n=((Bang==dPUT_LEFT)?1:Bang/2), i=0; i<4; i++, n=(n+1)%4)
	{
		//Bang의 방향에 따라 맵상에서 이웃한 지형을 뽑아낸다.
		BYTE around = mGetPattn(pos[n].x, pos[n].y)->bLand[lands[n]];
		if((pos[n].x<0)||(pos[n].y<0)||(pos[n].x>=m_wXRange)||(pos[n].y>=m_wYRange))
			around = mGetPattn(x, y)->bLand[3-lands[n]];

		//미리 뽑아낸 FFlags인자를 검색하여 공통돼는 것을 리턴...
		for(char *lpFFlag=FFlags; *lpFFlag!=-1; lpFFlag++)
			if(around==*lpFFlag)
				return *lpFFlag;
	}

/*---------------절벽을 위해서 추가됀 부분...------------------*/
	//서로 다른 높이에 위치한 것이라면...
	BYTE *height = mGetPattn(x, y)->bLand;
	for(i=0; i<4; i++)	{		
		if( (height[i]>>4) != (flag>>4) )
			if( m_lpff[flag][0] != -1 )	//그리고 절벽타일이 있다면...
				return (flag&0x0f) | ( ( ((flag>>4)>(height[i]>>4))?(flag>>4)-1:(flag>>4)+1 ) << 4 );
	}
/*-------------------------------------------------------------*/

	//if(!Bang)	//방향의 첫번째인 LEFT일때 한번 실행...
	//위에서 찾지 못하였을 때 지형과 찍을 플래그 사이의 중간 존재를 알아낸다.
	char *lpff = m_lpff[mGetPattn(x, y)->bLand[0]]+2;
	for(; *lpff!=-1; lpff++)
		for(char *lpFFlag=FFlags; *lpFFlag!=-1; lpFFlag++)
			if(*lpff==*lpFFlag)	return *lpff;

	return FFlags[0];
}

LPCPattn CPattnCtrl::GetLayerFromMap(WORD x, WORD y)
{
	if(x>=m_wXRange)	return NULL;
	if(y>=m_wYRange)	return NULL;

	return m_cpLayer[HIBYTE( GetMapIndex(x, y) )];
}

void CPattnCtrl::SetMapSample(int x, int y, PattnSample &PutSample)
{
	if((x<0)||(y<0)||(x>=m_wXRange)||(y>=m_wYRange))	return;

	WORD index = GetRandom((LPPattn)&PutSample);

	*mGetPattn(x, y) = PutSample;
	mGetPattn(x, y)->lpUnderCliff = (m_cpLayer[HIBYTE(index)]->m_bForm==dPATTN_UNDER_BORDER)?m_cpLayer[HIBYTE(index)]:NULL;
	mGetPattn(x, y)->lpCliff = (m_cpLayer[HIBYTE(index)]->m_bForm==dPATTN_CLIFF_BORDER)?m_cpLayer[HIBYTE(index)]:NULL;

	SetMapIndex(x, y, index);
}
/////////////////////////////////////////////////////////////////////////////////////////

BYTE CPattnCtrl::CullCliff(BYTE flag)
{
	return (m_bFlatPattn==(BYTE)true) ? flag : (flag&0x0f);
}