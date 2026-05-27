#ifndef CPATTNCTRL_H
#define CPATTNCTRL_H

#include "cpattn.hpp"

#define dPUT_LEFT		0
#define dPUT_LT			1
#define dPUT_TOP		2
#define dPUT_RT			3
#define dPUT_RIGHT		4
#define dPUT_RB			5
#define dPUT_BOTTOM		6
#define dPUT_LB			7

#define LT_MASK		0x01000000
#define RT_MASK		0x00010000
#define LB_MASK		0x00000100
#define RB_MASK		0x00000001
#define L_MASK		0x01000100
#define R_MASK		0x00010001
#define T_MASK		0x01010000
#define B_MASK		0x00000101
#define LB_L_B_MASK	0x01000101
#define RB_R_B_MASK	0x00010101
#define LT_L_T_MASK	0x01010100
#define RT_R_T_MASK	0x01010001

#define RT_LB_MASK	0x00010100
#define RB_LT_MASK	0x01000001

typedef struct PattnSample{
	union{		
		struct{
			BYTE bLandLT;
			BYTE bLandRT;
			BYTE bLandLB;
			BYTE bLandRB;
		};
		DWORD dwLand;
		BYTE bLand[4];
	};
	LPBYTE		lpImage;
	LPCPattn	lpCliff;
	LPCPattn	lpUnderCliff;
}*LPPattnSample;

#define mGetPattn(X, Y)	( m_pattns+ (((Y)<0)?0:(((Y)>=m_wYRange)?m_wYRange-1:(Y))) * m_wXRange + (((X)<0)?0:(((X)>=m_wXRange)?m_wXRange-1:(X))) )
#define mGetPattn_NoCheck(X, Y) ( m_pattns + (Y) * m_wXRange + (X) )

class CPattnCtrl : public CPattn{
public:
	BOOL		Load(LPSTR filename);

	void		Init(int XRange, int YRange)
	{				PutPattnIndex(XRange, YRange, 0, NULL, NULL);						}

	void		SetStart(BYTE flag);

	void		PutPattnIndex(int XRange, int YRange, BYTE bFlag, LPWORD lpwPattnIndex, LPWORD lpwHeight);
	void		GetPattnIndex(LPWORD lpwPattnIndex, LPWORD lpwHeight);

	BOOL		Resize(int _iX, int _iY, int _XRange, int _YRange);

	void		PutCliff(int x, int y);								//	절벽을 새로 만든다.
	void		PutCullCliff(int x, int y);
	void		PutTile(int x, int y, BYTE flag, BOOL IsExpand);	//	IsExpand는 절벽이 있을 경우 처리에 관한것이다. true라면 확장을 하고 false라면 깍아내린다.

	BOOL		IsFlat();

	//	lpbFlag에 미리 메모리가 적절히 할당돼어있어야 한다.
	void		GetNormalFlag(BYTE *lpbFlag, int *iFlagCount);
	void		GetCliffFlag(BYTE *lpbFlag, int *iFlagCount);

	LPPattn		GetFlagFromPattn(BYTE flag);

	int			GetPattnCount();			//	절벽과 ,경계 패턴을 포함한 모든 패턴의 갯수...
	int			GetPattnCount(BYTE flag);	//	념겨진 속성의 갯수...

	WORD		m_wXRange;			// 맵의 X사이즈
	WORD		m_wYRange;			// 맵의 Y사이즈
	DWORD		m_dwIndexCount;		// x*y
	WORD		m_wSampleCount;
	BYTE		m_bSampleFlag[20];

	CPattnCtrl();
	~CPattnCtrl();

//*********************	이 아래의 함수와 변수들은 사용하지 말것... *********************//
public:
	BOOL		IsCliffPattn(int index);
	LPPattn		GetPattn(int index);
	void		SetPattn(WORD index);

	WORD		GetMapIndex(int x, int y);	
	void		SetMapIndex(int x, int y, WORD index);
	void		SetMapIndex(int x, int y, WORD index, LPPattnSample lpSample, BOOL isSameSample=true);

	BYTE		GetMapIndex(int x, int y, BOOL isHighIndex);
	void		SetMapIndex(int x, int y, BYTE layer, BYTE pattn);

	void		SetSelectLayer(int index);
	void		AddNewLayer(BYTE form, BYTE flag1, BYTE flag2);

	BOOL		Save(LPSTR filename);

	int			m_iLayerCount;
	int			m_iSelectLayerIndex;
	LPCPattn	m_cpLayer[256];

	POINT		m_Offset[8];

	LPPattnSample GetSample2Protected(int x, int y);

	void		DeleteLayer();
	void		CopyLayer();
	void		MoveLast();
	void		SetFriend();

	BYTE		CullCliff(BYTE flag);
	LPWORD		m_pwPattnIndex;	// 09.07.15  원래 protected : 변수.
	PattnSample	*m_pattns;		// 09.07.15  원래 protected : 변수.
protected:
	//LPWORD		m_pwPattnIndex;
	WORD		m_layerIndexRange[255];	
	//PattnSample	*m_pattns;

	BYTE		Layer2Index(LPCPattn lpLayer);

	void	PutTileUnder(int x, int y, LPPattnSample lpPattn);

	void	PutTile(int x, int y, LPPattnSample lpPutPattn, int Bang);
	BOOL	SetBangPattn(int x, int y, PattnSample &PS, BYTE Bang, LPBYTE fillFlag);

	void	GetTMFlag(int x, int y, BYTE Bang, LPPattnSample lpPutPattn, BYTE &flag1, BYTE &flag2, BYTE &sampleFlag1, BYTE &sampleFlag2);
	BYTE	GetNearFlag(int x, int y, BYTE Bang, BYTE flag);
	void	SetMapSample(int x, int y, PattnSample &PutSample);

	WORD	GetRandom(LPPattn lpPattn);

	LPCPattn	GetLayerFromMap(WORD x, WORD y);

	void	SetCliffHill(int x, int y);
	int		SetCliffCheck(int x, int y);
	int		SetCliffCheck2(int x, int y);
	void	SetCliffNormal(int x, int y);
	BOOL	SetCliffLeftRightNormal(int x, int y);
	void	SetUnderCliffHill(int x, int y);

	int		SetCliffLowHighHill(int x, int y);
	int		SetCliffLeftRightHighHill(int x, int y);

	void	SetUnderCliff(int x, int y, int Bang);

	PattnSample Flag2Sample(int x, int y, BYTE flag, BOOL isCliffHigh=true);

	BOOL	PutFriend(int x, int y, LPPattnSample lpPattn);

	void	PutTile(int x, int y, PattnSample &sample);
	void	SetCliff(int x, int y, BYTE Bang, BYTE afterCliffID);

	void	SetCliffUp(int x, int y, BYTE Bang, int CurrentID);
	void	SetCliffDown(int x, int y, BYTE Bang, int CurrentID);
	BOOL	SetCliffLow(int x, int y, BYTE Bang, DWORD bangMask, BYTE low, BYTE high);

	void	PutExpandCliff(int x, int y, PattnSample &sample);

	void	RePutTile();
	void	AddRePutList(int x, int y, PattnSample &Insert);
	void	SetCliffUp_TOP(int x, int y, int CurrentID, int iPush, int iBorder, int isStartLowFlag);
	void	SetCliffUp_LT_RT(int x, int y, int Bang, int CurrentID, int iPush, int iBorder, int isStartLowFlag);

private:
	char	m_lpff[512][512];

	RECT	m_hillRng;
};

inline int	CPattnCtrl::GetPattnCount()
{
	int count = 0;
	for(int i=0; i<m_iLayerCount; i++)
		count += m_cpLayer[i]->m_iPattnCount;
	return count;
}

inline int	CPattnCtrl::GetPattnCount(BYTE flag)
{
	for(int i=0; i<m_iLayerCount; i++)
	{
		if(m_cpLayer[i]->m_bForm!=dPATTN_NORMAL)	continue;
		if(m_cpLayer[i]->IsBorder())				continue;
		if(m_cpLayer[i]->m_bFlag[0]!=flag)			continue;
		return m_cpLayer[i]->m_iPattnCount;
	}
	return 0;
}

inline LPPattn CPattnCtrl::GetFlagFromPattn(BYTE flag)
{
	for(int i=0; i<m_iLayerCount; i++)
	{
		if(m_cpLayer[i]->m_bForm!=dPATTN_NORMAL)	continue;
		if(m_cpLayer[i]->IsBorder())				continue;
		if(m_cpLayer[i]->m_bFlag[0]!=flag)			continue;
		return m_cpLayer[i]->m_cpPattn;
	}
	return NULL;
}

inline void CPattnCtrl::GetNormalFlag(BYTE *lpbFlag, int *iFlagCount)
{
	*iFlagCount = 0;
	for(int i=0; i<m_iLayerCount; i++)
	{
		if(m_cpLayer[i]->m_bForm!=dPATTN_NORMAL)	continue;
		if(m_cpLayer[i]->IsBorder())				continue;
		lpbFlag[*iFlagCount] = m_cpLayer[i]->m_bFlag[0];
		(*iFlagCount)++;
	}
}

inline void CPattnCtrl::GetCliffFlag(BYTE *lpbFlag, int *iFlagCount)
{
	*iFlagCount = 0;
	for(int i=0; i<m_iLayerCount; i++)
	{
		if(m_cpLayer[i]->m_bForm!=dPATTN_CLIFF_BORDER)		continue;
		lpbFlag[*iFlagCount] = m_cpLayer[i]->m_bLowFlag1;
		(*iFlagCount)++;
	}
}

inline PattnSample CPattnCtrl::Flag2Sample(int x, int y, BYTE flag, BOOL isCliffHigh/*=true*/)
{
	BYTE cullFlag = CullCliff(flag);

	PattnSample sample = {flag, flag, flag, flag, NULL};

	for(int i=0; i<m_iLayerCount; i++)	{
		if((m_cpLayer[i]->m_bLowFlag1==cullFlag)&&(m_cpLayer[i]->m_bForm==dPATTN_UNDER_BORDER))
			sample.lpUnderCliff = m_cpLayer[i];
		if((m_cpLayer[i]->m_bLowFlag1==cullFlag)&&(m_cpLayer[i]->m_bForm==dPATTN_CLIFF_BORDER))
			sample.lpCliff		= m_cpLayer[i];
	}

	LPPattnSample lpSample;
	for(int y2=y; y2>=0; y2--)
	{
		lpSample = mGetPattn(x, y2);
		if(!lpSample->lpCliff)			continue;
		if(y-y2>=lpSample->lpCliff->m_range.bottom)
			y2 = y;
		break;
	}

	BYTE *land	= mGetPattn(x, y2)->bLand;
	BYTE low	= land[0], high = land[1];
	if(low==high)	high = land[2];
	if(low==high)	high = land[3];
	if(low>high) 	{		low = high;		high = land[0];		}

	BYTE cliffID = (isCliffHigh) ? high&0xf0 : low&0xf0;

	if(!m_bFlatPattn)
		for(i=0; i<4; i++)	sample.bLand[i] |= cliffID;

	return sample;
}

inline BYTE CPattnCtrl::GetMapIndex(int x, int y, BOOL isHighIndex)
{
	return (BYTE)(GetMapIndex(x, y)>>(8*isHighIndex));
}

inline void CPattnCtrl::SetMapIndex(int x, int y, BYTE layer, BYTE pattn)
{
	SetMapIndex(x, y, (layer<<8)|pattn);
}

inline WORD	CPattnCtrl::GetRandom(LPPattn lpPattn)
{
	Pattn pattn = {lpPattn->bLandLT, lpPattn->bLandRT, lpPattn->bLandLB, lpPattn->bLandRB, };
	//Pattn pattn = {lpPattn->bLandLT&0x0f, lpPattn->bLandRT&0x0f, lpPattn->bLandLB&0x0f, lpPattn->bLandRB&0x0f, };
	BYTE flag[2] = {pattn.bLandLT, pattn.bLandRT};
	if(flag[0]==flag[1])	flag[1] = pattn.bLandLB;
	if(flag[0]==flag[1])	flag[1] = pattn.bLandRB;
	if(flag[0]>flag[1])	{
		flag[0] = flag[1];
		flag[1] = pattn.bLandLT;
	}

	if( (flag[0]>>4) == (flag[1]>>4) )	//절벽 경계가 아닐 때...
	{
		if(!m_bFlatPattn)
		{
			flag[0] &= 0x0f;
			flag[1] &= 0x0f;
		}
	}
	else
	{
		int	height	= (flag[0]&0xf0)>>4;
		flag[0]		= (flag[0]&0x0f);
		flag[1]		= 16 | (flag[1]&0x0f);
		pattn.bLandLT = (((pattn.bLandLT>>4)-height)<<4)|(pattn.bLandLT&0x0f);
		pattn.bLandRT = (((pattn.bLandRT>>4)-height)<<4)|(pattn.bLandRT&0x0f);
		pattn.bLandLB = (((pattn.bLandLB>>4)-height)<<4)|(pattn.bLandLB&0x0f);
		pattn.bLandRB = (((pattn.bLandRB>>4)-height)<<4)|(pattn.bLandRB&0x0f);
	}

	for(int i=0; i<m_iLayerCount; i++)
		if( (m_cpLayer[i]->m_bFlag[0]==flag[0]) && (m_cpLayer[i]->m_bFlag[1]==flag[1]) )
			return (i<<8) | m_cpLayer[i]->GetRandom(pattn.bLandLT, pattn.bLandRT, pattn.bLandLB, pattn.bLandRB);

	return 0;
}

inline LPPattnSample CPattnCtrl::GetSample2Protected(int x, int y)
{	
	return	mGetPattn(x, y);
}

inline void	CPattnCtrl::DeleteLayer()
{
	if( (m_iSelectLayerIndex>-1) && (m_cpLayer[m_iSelectLayerIndex]) )
	{
		delete m_cpLayer[m_iSelectLayerIndex];

		m_cpLayer[m_iSelectLayerIndex]  = NULL;

		for(int i=m_iSelectLayerIndex+1; i<m_iLayerCount; i++)	{
			m_cpLayer[i-1] = m_cpLayer[i];
			m_cpLayer[i] = NULL;
		}

		m_iLayerCount--;
		
		if(m_iSelectLayerIndex>=m_iLayerCount)
			m_iSelectLayerIndex = m_iLayerCount-1;
		*GetCPattn() = *m_cpLayer[m_iSelectLayerIndex];

		SetFriend();
	}	
}

inline BYTE CPattnCtrl::Layer2Index(LPCPattn lpLayer)
{
	for(int i=0; i<m_iLayerCount; i++)
		if(m_cpLayer[i]==lpLayer)
			return i;
	return 0;
}

inline LPPattn	CPattnCtrl::GetPattn(int index)
{
	int layerIndex = HIBYTE(index);
	int pattnIndex = LOBYTE(index);

	if(layerIndex>=m_iLayerCount)							layerIndex = 0;
	if(pattnIndex>=m_cpLayer[layerIndex]->m_iPattnCount)	pattnIndex = 0;

	return m_cpLayer[layerIndex]->m_cpPattn+pattnIndex;
}

inline BOOL	CPattnCtrl::IsCliffPattn(int index)//(int index, LPBYTE lpFlags)
{
	int layerIndex = HIBYTE(index);
	int pattnIndex = LOBYTE(index);

	if(m_cpLayer[layerIndex]->m_bForm == dPATTN_NORMAL)
		return false;

	return true;
}

inline void CPattnCtrl::SetMapIndex(int x, int y, WORD index)
{
	if((x<0)||(x>=m_wXRange)||(y<0)||(y>=m_wYRange))	return;
	m_pwPattnIndex[m_wXRange*y+x]	= index;
}

inline void CPattnCtrl::SetMapIndex(int x, int y, WORD index, LPPattnSample lpSample, BOOL isSameSample/*=true*/)
{
	if((x<0)||(x>=m_wXRange)||(y<0)||(y>=m_wYRange))	return;

	m_pwPattnIndex[m_wXRange*y+x]	= index;

	if(isSameSample)
	{
		LPPattnSample pSample = mGetPattn(x, y);
		*pSample			= *lpSample;
		*(LPPattn)pSample	= *GetPattn(index);
		if(m_cpLayer[HIBYTE(index)]->m_bForm==dPATTN_UNDER_BORDER)
			pSample->lpUnderCliff	= m_cpLayer[HIBYTE(index)];
		if(m_cpLayer[HIBYTE(index)]->m_bForm==dPATTN_CLIFF_BORDER)
			pSample->lpCliff		= m_cpLayer[HIBYTE(index)];
	}
}

inline WORD CPattnCtrl::GetMapIndex(int x, int y)
{
	return ((x<0)||(x>=m_wXRange)||(y<0)||(y>=m_wYRange)) ? 0 : m_pwPattnIndex[m_wXRange*y+x];
}

#endif