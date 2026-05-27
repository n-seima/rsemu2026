#include "stdafx.h"
#include "cpattnctrl.hpp"

/////////////////////////////////////////////////////////////////////////////////////////
//									절벽의 형태를 맞춘다.
void CPattnCtrl::SetCliffHill(int x, int y)
{
	int x2, y2;
	SetRect(&m_hillRng, m_hillRng.left, m_hillRng.top-10, m_hillRng.right, m_hillRng.bottom+10);
	if(m_hillRng.left<0)	m_hillRng.left	= 0;
	if(m_hillRng.top<0)		m_hillRng.top	= 0;
	if(m_hillRng.right>m_wXRange)	m_hillRng.right		= m_wXRange;
	if(m_hillRng.bottom>m_wYRange)	m_hillRng.bottom	= m_wYRange;

//	절벽의 위쪽 부분을 기준으로(TOP, LT_L_T, RT_R_T 마스크속성을 가진...) 
//	절벽이 유지돼어야 하는 갯수를 충족하는지 알아보고 그렇지 않음 제거한다.
	for(x2=m_hillRng.left; x2<m_hillRng.right; x2++)
		for(y2=m_hillRng.top; y2<m_hillRng.bottom; )
			y2 = SetCliffCheck(x2, y2);

//	부적절한 물속성의 경우 메워버린다.
	for(x2=m_hillRng.left; x2<m_hillRng.right; x2++)
		for(y2=m_hillRng.top; y2<m_hillRng.bottom; y2++)
			SetUnderCliffHill(x2, y2);

//	절벽경계를 검사하여 아랫쪽 더미들을 표시한다.
	for(x2=m_hillRng.left; x2<m_hillRng.right; x2++)
		for(y2=m_hillRng.top; y2<m_hillRng.bottom; y2++)
			SetCliffNormal(x2, y2);

	for(x2=m_hillRng.left; x2<m_hillRng.right; x2++)
		for(y2=m_hillRng.top; y2<m_hillRng.bottom; )
			y2 = SetCliffLowHighHill(x2, y2);

	for(x2=m_hillRng.left; x2<m_hillRng.right; x2++)
		for(y2=m_hillRng.top; y2<m_hillRng.bottom; y2++)
			//y2=SetCliffLeftRightNormal(x2, y2);
			SetCliffLeftRightNormal(x2, y2);
}

//	ㄱ자 형태중 부적절한것을 메워버린다.
int CPattnCtrl::SetCliffCheck2(int x, int y)
{
	LPPattnSample lpPattn = mGetPattn_NoCheck(x, y);
	BYTE *bLand = lpPattn->bLand;
	BYTE low	= bLand[0];
	BYTE high	= (low!=bLand[3]) ? bLand[3]  : ( (low==bLand[1]) ? bLand[2] : bLand[1] );
	if(low>high) {	low		= high;		high	= bLand[0];			}

	DWORD bangMask = (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);
	switch(bangMask)	{
	case LB_MASK:	case RB_MASK:	break;
	default:						return y+1;
	}

	int bNormal = lpPattn->lpCliff->m_range.bottom-1;

	DWORD lrLand = bLand[2] | bLand[3]<<8 | bLand[2]<<16 | bLand[3]<<24;
	for(int y2=y+1; ; y2++)
	{
		if(y2>=m_wYRange)									return y2;
		if( mGetPattn_NoCheck(x, y2)->dwLand != lrLand )	break;

		if(y2-y>=bNormal)	return y2+1;
	}

	DWORD ltrtLand = bLand[2] | bLand[3]<<8 | high<<16 | high<<24;
	if( mGetPattn_NoCheck(x, y2)->dwLand != ltrtLand)		return y2+1;

	PattnSample fillPattn;

	DWORD dwHigh = high | high<<8 | high<<16 | high<<24;
	for(int y3=y; y3<=y2; y3++)
	{
		fillPattn.dwLand = dwHigh;
		SetMapSample(x, y3, fillPattn);

		if( x>0 )
		{
			fillPattn = *mGetPattn_NoCheck(x-1, y3);
			fillPattn.bLandRT = fillPattn.bLandRB = high;
			SetMapSample(x-1, y3, fillPattn);
		}

		if( x+1<m_wYRange )
		{
			fillPattn = *mGetPattn_NoCheck(x+1, y3);
			fillPattn.bLandLT = fillPattn.bLandLB = high;
			SetMapSample(x+1, y3, fillPattn);
		}
	}

	return y2+1;
}

//절벽을 최소 필요 높이 단위가 있는지를 체크하고 없으면 제거...
int CPattnCtrl::SetCliffCheck(int x, int y)
{
	LPPattnSample lpPattn = mGetPattn_NoCheck(x, y), lpSample;
	if(!lpPattn->lpCliff)					return y+1;

	BYTE *bLand = lpPattn->bLand;
	BYTE low	= bLand[0];
	BYTE high	= (low!=bLand[3]) ? bLand[3]  : ( (low==bLand[1]) ? bLand[2] : bLand[1] );
	if(low>high) {	low		= high;		high	= bLand[0];			}

//	LT_L_T, RT_R_T혹은 TOP속성이 아니라면 검사할 필요없다.
	if((bLand[0]!=low)||(bLand[1]!=low))
		return SetCliffCheck2(x, y);

	DWORD flatHighID = high&0xf0;
	flatHighID = flatHighID | flatHighID<<8 | flatHighID<<16 | flatHighID<<24;

//	절벽이 존재하기 위해서는 한줄당 기본적으로 최소 2개 최대 4개의 패턴이 있어야 한다.
	for(int i=1; ; i++)	
	{
		if(i>=m_wYRange)	return y+i;

		lpSample = mGetPattn(x, y+i);

		if(lpSample->lpCliff)	
		{
		//normal은 절벽이 아랫쪽 경계와 위쪽경계의 특성에 따른 최소 구성갯수를 정한다.
		//예를 들어 top이나 bottom형경계는 각각 아랫쪽과 위쪽으로 평면요소가 1씩 필요하다.
			int normal	= 3-(bLand[2]!=bLand[3]);
			int isESet	= ((lpSample->bLandLB!=low)||(lpSample->bLandRB!=low));
			isESet		|= (((lpSample->bLandLT==low)||(lpSample->bLandRT==low))&&(lpSample->bLandLB==high)&&(lpSample->bLandRB==high));
			normal		-= isESet;
			if(i>=normal)	return y+1;
			break;
		}
	}

// 여기까지 오면 이 절벽은 없애줘야하는 불량 절벽이다.

	PattnSample fillPattn, lowPattn = {low, low, low, low, NULL, };

//	가장 위쪽 설정...
	lpSample = mGetPattn(x, y);
	if(lpSample->bLandLB==high)
	{
		fillPattn = *mGetPattn(x-1, y);
		fillPattn.bLandRB = low;
		SetMapSample(x-1, y, fillPattn);
	}
	if(lpSample->bLandRB==high)
	{
		fillPattn = *mGetPattn(x+1, y);
		fillPattn.bLandLB = low;
		SetMapSample(x+1, y, fillPattn);
	}
	fillPattn = *lpSample;
	fillPattn.bLandLB = fillPattn.bLandRB = low;
	SetMapSample(x, y, fillPattn);

//	중간부분...	
	for(int k=1; k<i; k++)
	{
		SetMapSample(x, y+k, lowPattn);

		fillPattn = *mGetPattn(x-1, y+k);
		fillPattn.bLandRT = fillPattn.bLandRB = low;
		SetMapSample(x-1, y+k, fillPattn);

		fillPattn = *mGetPattn(x+1, y+k);
		fillPattn.bLandLT = fillPattn.bLandLB = low;
		SetMapSample(x+1, y+k, fillPattn);
	}

//	아랫쪽..
	lpSample = mGetPattn(x, y+k);

	if(lpSample->bLandLT==high)
	{
		fillPattn = *mGetPattn(x-1, y+k);
		fillPattn.bLandRT = low;
		SetMapSample(x-1, y+k, fillPattn);
	}

	if(lpSample->bLandRT==high)
	{
		fillPattn = *mGetPattn(x+1, y+k);
		fillPattn.bLandLT = low;
		SetMapSample(x+1, y+k, fillPattn);
	}

	fillPattn = *lpSample;
	fillPattn.bLandLT = fillPattn.bLandRT = low;
	SetMapSample(x, y+k, fillPattn);

	return y+k+1;
}

int CPattnCtrl::SetCliffLeftRightHighHill(int x, int y)
{
	int index = GetMapIndex(x, y);
	LPPattn lpPattn = GetPattn(index);

	BYTE flag = lpPattn->bLandLT;
	DWORD dwFlag = flag<<24 | flag<<16 | flag<<8 | flag;

	//단일 속성이 아니라면...
	if( lpPattn->dwLand!=dwFlag )					return y+1;

	if((y==0)||(y+1==m_wYRange))					return y+1;

	LPPattnSample lpSample	= mGetPattn_NoCheck(x, y);

	int index0 = GetMapIndex(x, y-1)&0x00ff;
	int index1 = index&0x00ff;
	int index2 = GetMapIndex(x, y+1)&0x00ff;

	if((index2==index1+1)||(index0==index1-1))		return y+1;

	LPCPattn lpCliff = (lpSample->lpCliff) ? lpSample->lpCliff : lpSample->lpUnderCliff;

	BYTE Left = (x>0) ? mGetPattn_NoCheck(x-1, y)->bLandRT : 0;
	BYTE Right = (x+1<m_wXRange) ? mGetPattn_NoCheck(x+1, y)->bLandLT : 0;

	if(x==0) (Right==lpCliff->m_bLowFlag1) ? Left = lpCliff->m_bHighFlag1 : Left = lpCliff->m_bLowFlag1;
	if(x+1==m_wXRange) (Left==lpCliff->m_bLowFlag1) ? Right = lpCliff->m_bHighFlag1 : Right = lpCliff->m_bLowFlag1;

	if( lpSample->lpCliff )
	{
		if(Left>Right)
		{
			Left = (Left&0x0f) | 0x10;
			Right &= 0x0f;
		}
		else
		{
			Right = (Right&0x0f) | 0x10;
			Left &= 0x0f;
		}
	}

	SetMapIndex(x, y, HIBYTE(GetMapIndex(x, y)), lpCliff->GetRandom(Left, Right, Left, Right));

	return y+1;
}

/*int CPattnCtrl::SetCliffLowHighHill(int x, int y)
{
	BYTE byIndex = GetMapIndex(x, y);
	BYTE byLayer = HIBYTE( bIndex );

	LPCPattn pLayer = m_cpLayer[byLayer];

	if( pLayer->m_bHeight <= 1 )
		return y+1;

	LPBYTE pby = (BYTE*)&pLayer->m_cpPattn[byIndex].dwLand;

	if( (pby[0]!=pby[1]) || (pby[0]!=pby[2]) || (pby[2]!=pby[3]) )
		return y+1;

	if( ( byLayer == HIBYTE(GetMapIndex(x, y-1)) ) || ( byLayer == HIBYTE(GetMapIndex(x, y+1)) ) )
		return y+1;

	LPPattnSample lpSample	= mGetPattn_NoCheck(x, y);
	if( (lpSample->lpCliff) || (lpSample->lpUnderCliff) )			//	절벽 경계는 사용하지 않는다.
		return y+1;

	return y+1;
}*/

//	요것은 절벽을 만들거나 지울때 나타나는 지우지 못한 더미들을 없애준다.
int CPattnCtrl::SetCliffLowHighHill(int x, int y)
{
	LPPattnSample lpSample	= mGetPattn_NoCheck(x, y);

	//	절벽경계라면...
	if( (lpSample->lpCliff) || (lpSample->lpUnderCliff) )
		return SetCliffLeftRightHighHill(x, y);

	BYTE flag = lpSample->bLandLT;
	DWORD dwFlag = flag<<24 | flag<<16 | flag<<8 | flag;

	//	단일 속성이 아니라면...
	if( lpSample->dwLand!=dwFlag )
		return y+1;

	WORD index = GetMapIndex(x, y);

	int hiIndex		= HIBYTE(index);
	LPCPattn layer	= m_cpLayer[hiIndex];

	//	레이어가 일반이라면...
	if(layer->m_bForm==dPATTN_NORMAL)
		return y+1;

	LPBYTE pLands;

	//	여기까지 오면 잘못됄소지가 있는 더미들을 가르키고 있다.
	if(layer->m_bLowFlag1==GetPattn(index)->bLandLT)
	{
		int bottom		= layer->m_range.bottom;

		if( GetMapIndex(x, y-1, true) == hiIndex )	
		{
			for(int i=1; i<bottom; i++)
			{
				lpSample = mGetPattn(x, y-i);
				if((lpSample->lpCliff==layer)||(lpSample->lpUnderCliff==layer))
					return y+1;

				pLands = &GetPattn(GetMapIndex(x, y-i))->bLandLT;
				if( (pLands[0]!=pLands[1]) || (pLands[0]!=pLands[2]) || (pLands[2]!=pLands[3]) )
					return y+1;
			}
		}
	}
	else
	{
		lpSample	= mGetPattn(x, y-1);
		pLands		= lpSample->bLand;
		if((lpSample->lpUnderCliff==layer)||(lpSample->lpCliff==layer))
			if( (pLands[0] == pLands[1]) && (pLands[2] == pLands[3]) && (pLands[0]!=pLands[2]) )
				return y+1;

		lpSample	= mGetPattn(x, y+1);
		pLands		= lpSample->bLand;
		if((lpSample->lpUnderCliff==layer)||(lpSample->lpCliff==layer))
			if( (pLands[0] == pLands[1]) && (pLands[2] == pLands[3]) && (pLands[0]!=pLands[2]) )
				return y+1;
	}

	PattnSample fill = {flag, flag, flag, flag, NULL, };
	SetMapSample(x, y, fill);

	 return y+1;
}

//	UnderCliff가 돼기에 부적절한 것을 high로 메워버린다.
void CPattnCtrl::SetUnderCliffHill(int x, int y)
{
	LPPattnSample lpSample	= mGetPattn(x, y);
	LPCPattn lpUnder		= lpSample->lpUnderCliff;
	if(!lpUnder)						return;

	BYTE *land = lpSample->bLand;

	BYTE low	= lpUnder->m_bLowFlag1 | (land[0]&0xf0);
	BYTE high	= lpUnder->m_bHighFlag1 | (land[0]&0xf0);

	if((land[2]!=low)&&(land[3]!=low))	return;

	DWORD bangMask = (land[0]==low)<<24|(land[1]==low)<<16|(land[2]==low)<<8|(land[3]==low);
	switch(bangMask)
	{
	case B_MASK:case LB_L_B_MASK:case RB_R_B_MASK:case LB_MASK:case RB_MASK:	break;
	default:		return;
	}

	RECT range = lpUnder->m_range;

	DWORD bangMask2 = 0;
	LPPattnSample lpTemp;

	BYTE *bLand;

//	T_MASK LT_L_T_MASK RT_R_T_MASK형식과 만나는지 검사...
	for(int i=1; ; i++)
	{
		// 자리를 보고 누울 공간이 충분하다면 리턴...
		if((i>=range.bottom)||(i>=m_wYRange))	return;

		lpTemp = mGetPattn(x, y+i);

		if(lpTemp->lpUnderCliff)
		{
			bLand		= lpTemp->bLand;
			bangMask2	= (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);

			switch(bangMask2)
			{
			case LT_L_T_MASK:	case RT_R_T_MASK:
				//위의 속성에 아래의 속성이 만난다면 i는 한칸더 메워줘야함...
				if( (bangMask==LB_MASK) || (bangMask==LB_MASK) )
				{
					bLand = mGetPattn(x, y+i+1)->bLand;
					bangMask2 = (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);
					i += ( (bangMask2==LT_MASK) || (bangMask2==RT_MASK) );
				}
				break;

			case T_MASK:	case LT_MASK:	case RT_MASK:	break;

			default:					continue;
			}

			break;
		}
	}

// 여기까지 오면 뭔가 문제있는 속성임...

	int k;
	PattnSample fill;

	switch(bangMask)
	{
	case B_MASK:	case LB_L_B_MASK:	case RB_R_B_MASK:
		if((land[0]==high)&&(bLand[2]==high))	{
			for(k=0; k<=i; k++)	{
				fill.dwLand = mGetPattn(x, y+k)->dwLand;
				fill.bLandLT = fill.bLandLB = high;
				SetMapSample(x, y+k, fill);

				fill.dwLand = mGetPattn(x-1, y+k)->dwLand;
				fill.bLandRT = fill.bLandRB = high;
				SetMapSample(x-1, y+k, fill);
			}
		}
		if((land[1]==high)&&(bLand[3]==high))	{
			for(k=0; k<=i; k++)	{
				fill.dwLand = mGetPattn(x, y+k)->dwLand;
				fill.bLandRT = fill.bLandRB = high;
				SetMapSample(x, y+k, fill);

				fill.dwLand = mGetPattn(x+1, y+k)->dwLand;
				fill.bLandLT = fill.bLandLB = high;
				SetMapSample(x+1, y+k, fill);
			}
		}
		break;
	case LB_MASK:
		for(k=0; k<=i; k++)	{
			fill.dwLand = mGetPattn(x, y+k)->dwLand;
			fill.bLandLT = fill.bLandLB = high;
			SetMapSample(x, y+k, fill);

			fill.dwLand = mGetPattn(x-1, y+k)->dwLand;
			fill.bLandRT = fill.bLandRB = high;
			SetMapSample(x-1, y+k, fill);
		}
		break;
	case RB_MASK:
		for(k=0; k<=i; k++)	{
			fill.dwLand = mGetPattn(x, y+k)->dwLand;
			fill.bLandRT = fill.bLandRB = high;
			SetMapSample(x, y+k, fill);

			fill.dwLand = mGetPattn(x+1, y+k)->dwLand;
			fill.bLandLT = fill.bLandLB = high;
			SetMapSample(x+1, y+k, fill);
		}
		break;	
	default:
		return;
	}
}

/*BOOL CPattnCtrl::SetCliffLeftRightNormal(int x, int y)
{
	LPPattnSample lpSample = mGetPattn_NoCheck(x, y);
	if(	(!lpSample->lpUnderCliff) && (!lpSample->lpCliff)	)				return y+1;

	BYTE *land = &GetPattn(GetMapIndex(x, y))->bLandLT;
	if( (land[0]==land[1]) || (land[0]!=land[2]) || (land[1]!=land[3]) )	return y+1;

	DWORD lrland = GetPattn(GetMapIndex(x, y))->dwLand;

	LPCPattn lpCliff = (lpSample->lpCliff) ? lpSample->lpCliff : lpSample->lpUnderCliff;
	int normal	= lpCliff->m_range.left;

	for(int y2=y-1; y2>=0; y2--)
		if(lrland != GetPattn(GetMapIndex(x, y2))->dwLand)
			break;

	int sY = (++y2);
	
//	가끔씩 left, right와 비슷한 경계처리 더미들이 있으니 이걸 제거한다.	
	for(y2++; y2<m_wYRange; y2++)
		if(lrland != GetPattn(GetMapIndex(x, y2))->dwLand)
			break;
	
	int nCot	= (y2-sY)/normal*normal;
	int pattn, layer	= GetMapIndex(x, y, true);

	for(int i=0; i<nCot; i+=normal)
	{
		//pattn = GetMapIndex(x, sY+i, false);
		pattn = lpCliff->GetRandom(land[0], land[1], land[2], land[3]);

		SetMapIndex(x, sY+i, layer, pattn);

		for(int k=1; k<normal; k++)		SetMapIndex(x, sY+i+k, layer, pattn+k);
	}

	switch((y2-sY)-nCot)
	{
	case 1:	SetMapIndex(x, sY+nCot, layer, lpCliff->GetLast(land[0], land[1], land[2], land[3]) );
			break;
	case 2:	SetMapIndex(x, sY+nCot, layer, lpCliff->GetLast(land[0], land[1], land[2], land[3], -2) );
			SetMapIndex(x, sY+nCot+1, layer, lpCliff->GetLast(land[0], land[1], land[2], land[3], -1) );
			break;
	}

	return y2;
}*/

BOOL CPattnCtrl::SetCliffLeftRightNormal(int x, int y)
{
	LPPattnSample lpSample = mGetPattn(x, y);
	if(	(!lpSample->lpUnderCliff) && (!lpSample->lpCliff)	)				return false;

	BYTE *land = &GetPattn(GetMapIndex(x, y))->bLandLT;
	if( (land[0]==land[1]) || (land[0]!=land[2]) || (land[1]!=land[3]) )	return false;

	DWORD l_r_land = GetPattn(GetMapIndex(x, y))->dwLand;

	if(GetPattn(GetMapIndex(x, y-1))->dwLand==l_r_land)	
	{
		if(GetPattn(GetMapIndex(x, y+1))->dwLand!=l_r_land)	
		{
			BOOL isFind = false;
			for(int i=1; (!isFind); i++){
				if(y-i<0)	{
					isFind = true;
					break;
				}

				if( GetPattn(GetMapIndex(x, y-i))->dwLand!=l_r_land )	{
					isFind = true;
					break;
				}
			}

			if( (isFind) && (y-(i-1)<m_hillRng.top) )
				return SetCliffLeftRightNormal(x, y-(i-1));
		}

		return false;
	}

	for(int i=1; ; i++)
		if( GetPattn(GetMapIndex(x, y+i))->dwLand!=l_r_land )
			break;

	int layer	= GetMapIndex(x, y, true);

	LPCPattn lpCliff	= (lpSample->lpUnderCliff) ? lpSample->lpUnderCliff : lpSample->lpCliff;
	int normal			= lpCliff->m_range.left, k, pattn;
	int cha				= i%normal, last1, last2, index;

	switch(normal)
	{
	case 2:	last2 = last1 = lpCliff->GetLast(land[0], land[1], land[2], land[3]);	break;
	case 3:	last1	= lpCliff->GetLast(land[0], land[1], land[2], land[3]);
			last2	= lpCliff->GetLast(land[0], land[1], land[2], land[3], -2);
			break;
	}

	switch(cha)
	{
	case 1:	SetMapIndex(x, y+i-1, layer, last1);
			break;
	case 2:	SetMapIndex(x, y+i-2, layer, last2);
			SetMapIndex(x, y+i-1, layer, last2+1);
			break;
	}

	i = i/normal*normal;
	for(int n=0; n<i; )
	{
		index	= GetMapIndex(x, y+n, false);
		if( (index!=last1) && (index!=last2) )
			pattn = index/normal*normal;
		else
			pattn = lpCliff->GetRandom(land[0], land[1], land[2], land[3]);

		SetMapIndex(x, y+n, layer, pattn);
		n++;

		if(n>=i)	return true;

		for(k=1; k<normal; k++, n++)
		{			
			SetMapIndex(x, y+n, layer, pattn+k);
			if(n>=i)	return true;
		}
	}

	return true;
}

//절벽형 패턴은(물지형은 물론) y축으로 세트를 이루므로 이를 반영한다.
void CPattnCtrl::SetCliffNormal(int x, int y)
{
	LPPattnSample	lpSample	= mGetPattn(x, y);
	LPPattn			lpPattn		= GetPattn(GetMapIndex(x, y));

	int n;
	BYTE *sLand = lpSample->bLand;
	BYTE *bLand = (BYTE*)&lpPattn->bLandLT;	

	BYTE low, high, slow, shigh;
	LPCPattn lpUnder;

	BYTE N_SLand[4] = {sLand[0], sLand[1], sLand[2], sLand[3]};
	
	if(lpSample->lpUnderCliff)	{
		lpUnder	= lpSample->lpUnderCliff;
		low		= lpUnder->m_bLowFlag1;
		high	= lpUnder->m_bHighFlag1;
		slow	= low | (*sLand&0xf0);
		shigh	= lpUnder->m_bHighFlag1 | (*sLand&0xf0);
	}
	else if(lpSample->lpCliff) {
		lpUnder	= lpSample->lpCliff;
		slow	= sLand[0];
		shigh	= (slow!=sLand[3]) ? sLand[3]  : ( (slow==sLand[1]) ? sLand[2] : sLand[1] );
		if(slow>shigh) 	{		slow = shigh;		shigh = sLand[0];		}		
		low			= (slow&0x0f);
		int	height	= (slow&0xf0)>>4;
		N_SLand[0] = (((N_SLand[0]>>4)-height)<<4)|(N_SLand[0]&0x0f);
		N_SLand[1] = (((N_SLand[1]>>4)-height)<<4)|(N_SLand[1]&0x0f);
		N_SLand[2] = (((N_SLand[2]>>4)-height)<<4)|(N_SLand[2]&0x0f);
		N_SLand[3] = (((N_SLand[3]>>4)-height)<<4)|(N_SLand[3]&0x0f);
	}
	else 
		return;

	RECT range			= lpUnder->m_range;

	// ㄱ자가 있는지 조사해서 있다면 ㄱ자 형태가 됄수 있도록 한다.	
	if( (sLand[2]!=sLand[3])&&(sLand[0]==shigh)&&(sLand[1]==shigh) )
	{
		DWORD dwLand = sLand[2] | (sLand[3]<<8) | (sLand[2]<<16) | (sLand[3]<<24);

		for(int n=1; (mGetPattn(x, y+n)->dwLand==dwLand)&&(y+n<m_wYRange); n++);

		if(n>=range.bottom)
			SetMapIndex(x, y+1, GetMapIndex(x, y, true), lpUnder->GetLast(N_SLand[2], N_SLand[3], low, low, -2));
	}
	else if( (sLand[0]!=sLand[1])&&(sLand[2]==slow)&&(sLand[3]==slow) )// 끝을 찿는다.
	{
		DWORD dwEnd = shigh | (shigh<<8) | (sLand[0]<<16) | (sLand[1]<<24);
		DWORD dwLand = sLand[0] | (sLand[1]<<8) | (sLand[0]<<16) | (sLand[1]<<24);

		for(int n=1; (mGetPattn(x, y-n)->dwLand==dwLand)&&(y-n>=0); n++);

		int oldIndex = GetMapIndex(x, y, false);

		if( (n>1) || (y-n<0) )	{
			if((lpUnder->GetLast(N_SLand[0], N_SLand[1], N_SLand[2], N_SLand[3], 0)!=oldIndex)&&
				(lpUnder->GetLast(N_SLand[0], N_SLand[1], N_SLand[2], N_SLand[3], -1)!=oldIndex))
				SetMapIndex(x, y, GetMapIndex(x, y, true), lpUnder->GetLast(bLand[0], bLand[1], bLand[2], bLand[3], -(rand()%2)));
		}else{			
			if((lpUnder->GetLast(N_SLand[0], N_SLand[1], N_SLand[2], N_SLand[3], 0)==oldIndex)||
				(lpUnder->GetLast(N_SLand[0], N_SLand[1], N_SLand[2], N_SLand[3], -1)==oldIndex)||
				(lpUnder->GetLast(N_SLand[0], N_SLand[1], N_SLand[2], N_SLand[3], -2)==oldIndex))
				SetMapIndex(x, y, GetMapIndex(x, y, true), lpUnder->GetRandom(bLand[0], bLand[1], bLand[2], bLand[3]));
		}
	}//	가끔 좌우 패턴이 ㄱ자로 돼어있기도 하다. 이를 수정해야 한다.	
	else if( (lpSample->dwLand!=lpPattn->dwLand) && (sLand[0]!=sLand[1]) && (sLand[0]==sLand[2]) && (sLand[1]==sLand[3]) )
	{
		if( (bLand[2]==low) && (bLand[3]==low) && (bLand[0]!=bLand[1]) )
		{
			PattnSample	fill = {sLand[0], sLand[1], sLand[0], sLand[1], NULL, };

			BYTE *temp	= &GetPattn(GetMapIndex(x, y-1))->bLandLT;
			if( (temp[0]==low) && (temp[1]==low) && (temp[2]!=temp[3]) && (fill.dwLand==mGetPattn(x, y+1)->dwLand) )
			{				
				SetMapSample(x, y, fill);
				SetMapSample(x, y+1, fill);
				return;
			}
		}
	}

	//순서대로 만들어나간다.
	switch((bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low))
	{
	case T_MASK:	SetMapIndex(x, y+1, GetMapIndex(x, y, true), GetMapIndex(x, y, false)+1);
					break;
	case B_MASK:	SetMapIndex(x, y-1, GetMapIndex(x, y, true), GetMapIndex(x, y, false)-1);
					for(n=1; n<range.bottom; n++)
						SetMapIndex(x, y+n, GetMapIndex(x, y, true), GetMapIndex(x, y, false)+n);
					break;
	case LB_L_B_MASK:	case RB_R_B_MASK:
		range.bottom -= (lpUnder->GetLast(bLand[0], bLand[1], bLand[2], bLand[3], -2)==GetMapIndex(x, y, false));
		SetMapIndex(x, y-1, GetMapIndex(x, y, true), GetMapIndex(x, y, false)-1);
		for(n=1; n<range.bottom; n++)
			SetMapIndex(x, y+n, GetMapIndex(x, y, true), GetMapIndex(x, y, false)+n);
		break;
	}	
}
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
//						여기서 부터 절벽을 깍기 위한 함수들이다.
/////////////////////////////////////////////////////////////////////////////////////////

void CPattnCtrl::SetCliffUp_TOP(int x, int y, int CurrentID, int iPush, int iBorder, int isStartLowFlag)
{
	LPCPattn lpCliff	= mGetPattn(x, y-iBorder)->lpCliff;
	BYTE	highID		= ((CurrentID>>4)+1)<<4;
	BYTE	low			= CurrentID | lpCliff->m_bLowFlag1;
	BYTE	high		= highID | lpCliff->m_bLowFlag1;
	PattnSample	lowPattn	= {low, low, low, low, NULL, };
	PattnSample	highPattn	= {high, high, high, high, NULL, };

//절벽이 작다면 제거한다.
	for(int k=iBorder+1; k<iPush; k++)
	{
		LPPattnSample lpHighSample = mGetPattn(x, y-k);

		if(!lpHighSample->lpCliff)											continue;
		if( (lpHighSample->bLandLT!=low) || (lpHighSample->bLandRT!=low) )	continue;

		for(int n=0; n<k; n++)
			SetMapSample(x, y-n, lowPattn);
		PattnSample fill = *mGetPattn(x, y-n);
		fill.bLandLB = fill.bLandRB = low;
		SetMapSample(x, y-n, fill);					//여기에 뭐가 있을지 모르므로... 뒤처리까지 해준다.
		return;
	}

	//밀기 전에 위쪽절벽을 먼저 밀어준다.
	for(k=iBorder+1; k<iPush; k++)
	{
		BYTE *highLand = mGetPattn(x, y-k)->bLand;
		if( ((highLand[0]&0xf0)>CurrentID) && ((highLand[1]&0xf0)>CurrentID) && ((highLand[2]&0xf0)>CurrentID) &&((highLand[3]&0xf0)>CurrentID) )
		{
			if(highPattn.dwLand==*(DWORD*)highLand)
				if(!mGetPattn(x, y-k-1)->lpUnderCliff)
					PutTile(x, y-k, &highPattn, dPUT_TOP);
			SetMapSample(x, y-k, highPattn);
		}
	}
	BYTE *highLand = mGetPattn(x, y-k)->bLand;
	if( ((highLand[0]&0xf0)>CurrentID) && ((highLand[1]&0xf0)>CurrentID) && ((highLand[2]&0xf0)>CurrentID) &&((highLand[3]&0xf0)>CurrentID) )
		PutTile(x, y-k, &highPattn, dPUT_TOP);

//여기까지 왔다면 위쪽절벽은 처리가 돼었다. 이제 설정한다.
	for(k=1-isStartLowFlag; k<iPush-1; k++)
		SetMapSample(x, y-k, lowPattn);

	PattnSample fill = *mGetPattn(x, y-k);
	fill.bLandLB = fill.bLandRB = low;
	SetMapSample(x, y-k, fill);
}

// iPush 밀어줘야 돼는 총 길이 이거... iBorder는 위쪽절벽경계와의 거리 CurrentID는 낮은 지형 절벽 아디...
void CPattnCtrl::SetCliffUp_LT_RT(int x, int y, int Bang, int CurrentID, int iPush, int iBorder, int isStartLowFlag)
{
	LPCPattn lpCliff	= mGetPattn(x, y-iBorder)->lpCliff;
	BYTE	highID		= ((CurrentID>>4)+1)<<4;
	BYTE	low			= CurrentID | lpCliff->m_bLowFlag1;
	BYTE	high		= highID | lpCliff->m_bLowFlag1;
	PattnSample	lowPattn	= {low, low, low, low, NULL, };
	PattnSample	highPattn	= {high, high, high, high, NULL, };

	BYTE side = (Bang==dPUT_LT) ? dPUT_LEFT : dPUT_RIGHT;

//절벽이 작다면 제거한다.
	for(int k=iBorder+1; k<iPush; k++)
	{
		LPPattnSample lpHighSample = mGetPattn(x, y-k);

		if(!lpHighSample->lpCliff)
		{
			if( (lpHighSample->bLandLB==low) && (lpHighSample->bLandRB==low) )
				return;
			continue;
		}

		if( (lpHighSample->bLandLT!=low) || (lpHighSample->bLandRT!=low) )	continue;

		for(int n=0; n<=k; n++)
			PutTile(x, y-n, &lowPattn, side);
		return;
	}

//밀기 전에 위쪽절벽을 먼저 밀어준다.
	for(k=iBorder+1; k<iPush; k++)
	{
		BYTE *highLand = mGetPattn(x, y-k)->bLand;
		if( ((highLand[0]&0xf0)>CurrentID) && ((highLand[1]&0xf0)>CurrentID) && ((highLand[2]&0xf0)>CurrentID) &&((highLand[3]&0xf0)>CurrentID) )
			PutTile(x, y-k, &highPattn, Bang);
	}
	BYTE *highLand = mGetPattn(x, y-k)->bLand;
	if( ((highLand[0]&0xf0)>CurrentID) && ((highLand[1]&0xf0)>CurrentID) && ((highLand[2]&0xf0)>CurrentID) &&((highLand[3]&0xf0)>CurrentID) )
		PutTile(x, y-k, &highPattn, Bang);

//여기까지 왔다면 위쪽절벽은 처리가 돼었다. 이제 설정한다.
	for(k=1-isStartLowFlag; k<iPush-1; k++)
		PutTile(x, y-k, &lowPattn, side);

	PattnSample fill = *mGetPattn(x, y-k);
	
	fill.bLand[0] = (fill.bLand[0]&0xf0) | (low&0x0f);
	fill.bLand[1] = (fill.bLand[1]&0xf0) | (low&0x0f);	
	if(Bang==dPUT_LT)
	{
		fill.bLand[3] = low;
		fill.bLand[2] = (fill.bLand[2]&0xf0) | (low&0x0f);
	}
	else
	{
		fill.bLand[2] = low;
		fill.bLand[3] = (fill.bLand[3]&0xf0) | (low&0x0f);
	}
	for(int n=0; n<4; n++)
		if((fill.bLand[n]&0xf0)>highID)
			fill.bLand[n] = highID | (fill.bLand[n]&0x0f);

	SetMapSample(x, y-k, fill);

//	마무리로 설정한다.
	BYTE *borLand = mGetPattn(x, y-k)->bLand;
	if(Bang==dPUT_LT)
	{
		if((borLand[0]==high)&&(borLand[2]==high))
			PutTile(x-1, y-k, &highPattn, dPUT_LEFT);
		if(borLand[0]==high)
			PutTile(x-1, y-k-1, &highPattn, dPUT_LT);
		if((borLand[0]==high)&&(borLand[1]==high))
			PutTile(x, y-k-1, &highPattn, dPUT_TOP);
	}else{
		LPPattnSample lpss = mGetPattn(x, y-k-1);
		if((borLand[0]==high)&&(borLand[1]==high))
			PutTile(x, y-k-1, &highPattn, dPUT_TOP);
		if(borLand[1]==high)
			PutTile(x+1, y-k-1, &highPattn, dPUT_RT);
		if((borLand[1]==high)&&(borLand[3]==high))
			PutTile(x+1, y-k, &highPattn, dPUT_RIGHT);
	}

//	역쉬... ㄱ자를 처리하기 위해서 ㄱ자가 돼기 힘들다면 옆을 부드럽게 깍는당... ㅋㅋㅋ
	if( (borLand[0]!=high) || (borLand[1]!=high) || (borLand[2]==borLand[3]) )	return;

	y -= k;
	LPPattnSample lpSample;
	int bottom		= lpCliff->m_range.bottom;
	DWORD sideLand	= borLand[2] | borLand[3]<<8 | borLand[2]<<16 | borLand[3]<<24;	

	for(int i=1; ; i++)
	{
		if( y+i>=m_wYRange )						return;

		lpSample = mGetPattn(x, y+i);
		if( lpSample->dwLand != sideLand )
		{
			if(i==1)								return;
			BOOL isLB_RB = ((lpSample->bLandRB==low)&&(lpSample->bLandLB==low)&&(lpSample->bLandLT!=lpSample->bLandRT));
			//ㄱ자가 됄 조건에 합당하당...
			if(i>=bottom+isLB_RB)					return;
			break;
		}
	}

	PutTile(x, y+bottom, &lowPattn, dPUT_TOP);
	PutTile(x+m_Offset[Bang].x, y+bottom, &lowPattn, Bang);
}

void CPattnCtrl::SetCliffUp(int x, int y, BYTE Bang, int CurrentID)
{	
	switch(Bang)	//	TOP LT RT일 경우에만 처리한다.
	{	case dPUT_LEFT:case dPUT_RIGHT:	return;}

	LPCPattn lpCliff = NULL;

	for(int i=0; y-i>=0; i++)	{
		lpCliff = mGetPattn(x, y-i)->lpCliff;
		if(lpCliff)		break;		//기준으로 삼을 절벽이 검색돼었다.
		if(i>15)		break;		//15보다 크다면 계속할 이유가 없다.
	}

	if(!lpCliff)	return;

	BYTE	highID	= ((CurrentID>>4)+1)<<4;
	BYTE	low			= CurrentID | lpCliff->m_bLowFlag1;
	BYTE	high		= highID | lpCliff->m_bLowFlag1;

	PattnSample	lowPattn	= {low, low, low, low, NULL, };
	PattnSample highPattn	= {high, high, high, high, NULL, };
	PattnSample	bottomPattn = {high, high, low, low, NULL, };

	PattnSample	highSample = *mGetPattn(x, y-i);
	if((highSample.bLandLT<=low)&&(highSample.bLandRT<=low)&&(highSample.bLandLB<=low)&&(highSample.bLandRB<=low))
		return;	

	int bottom		= lpCliff->m_range.bottom;		//요넘의 길이...	
	LPPattnSample	lpStart = mGetPattn(x, y);

	BOOL isStartLow = ((lpStart->dwLand&0x0f0f0f0f)==(lowPattn.dwLand&0x0f0f0f0f));

	int iPush = bottom+(!isStartLow);

	if(i>=bottom)	return;			//계속할 이유가 없다.

//여기까지 왔다면 뭔가 밀어주기를 원하는 것이다.

	switch(Bang)
	{
	case dPUT_TOP:	SetCliffUp_TOP(x, y, CurrentID, iPush, i, isStartLow);	break;
	case dPUT_LT:	case dPUT_RT:
		if(!isStartLow)
		{
			PutTile(x, y-1, &lowPattn, dPUT_TOP);
			PutTile(x+m_Offset[Bang].x, y, &lowPattn, (Bang==dPUT_LT)?dPUT_LEFT:dPUT_RIGHT);
			PutTile(x+m_Offset[Bang].x, y-1, &lowPattn, Bang);
		}
		else
			SetCliffUp_LT_RT(x, y, Bang, CurrentID, iPush, i, isStartLow);
	}

	DWORD bottomCliff	= high | high<<8 | low<<16 | low<<24;
	DWORD topCliff	= low | low<<8 | high<<16 | high<<24;

	//역쉬 아래 위로 더 찍어주기는 있겠지요.
	switch(Bang)	{
	case dPUT_TOP:	case dPUT_LT:	case dPUT_RT:
		y -= (iPush-1);
		if(mGetPattn(x, y)->dwLand!=bottomCliff)		break;
		SetMapSample(x, y-1, highPattn);
		PutTile(x-1, y-1, &highPattn, dPUT_LEFT);
		PutTile(x-1, y-2, &highPattn, dPUT_LT);
		PutTile(x-0, y-2, &highPattn, dPUT_TOP);
		PutTile(x+1, y-2, &highPattn, dPUT_RT);
		PutTile(x+1, y-1, &highPattn, dPUT_RIGHT);
		return;
	case dPUT_BOTTOM: case dPUT_RB:	case dPUT_LB:
		if(mGetPattn(x, y)->dwLand!=topCliff)			break;
		SetMapSample(x, y+1, highPattn);
		PutTile(x+1, y+1, &highPattn, dPUT_RIGHT);
		PutTile(x+1, y+2, &highPattn, dPUT_RB);
		PutTile(x-0, y+2, &highPattn, dPUT_BOTTOM);
		PutTile(x-1, y+2, &highPattn, dPUT_LB);
		PutTile(x-1, y+1, &highPattn, dPUT_LEFT);
		return;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
//						여기까지 부터 절벽을 깍기 위한 함수들이다.
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//									UnderTile!!!!!!!
void CPattnCtrl::PutTileUnder(int x, int y, LPPattnSample lpPattn)
{
	POINT pos;
	BYTE *land				= lpPattn->bLand;
	RECT range				= lpPattn->lpUnderCliff->m_range;
	LPCPattn lpUnderCliff	= lpPattn->lpUnderCliff;
	BYTE low				= lpUnderCliff->m_bLowFlag1 | (*land&0xf0);
	BYTE highLand			= lpUnderCliff->m_bHighFlag1 | (*land&0xf0);
	PattnSample highPattn	= {highLand, highLand, highLand, highLand, NULL, };

	DWORD topCliff			= highLand | highLand<<8 | low<<16 | low<<24;
	DWORD bottomCliff		= low | low<<8 | highLand<<16 | highLand<<24;

	for(int n=0; n<range.bottom; n++)	SetMapSample(x, y-n, *lpPattn);

#define IF_NOTONEPATTN(X, Y, P) land = mGetPattn(X, Y)->bLand;	if((land[0]!=P)||(land[1]!=P)||(land[2]!=P)||(land[3]!=P))
#define IF_PATTNINCLUDE(X, Y, P) land = mGetPattn(X, Y)->bLand;	if((land[0]==P)||(land[1]==P)||(land[2]==P)||(land[3]==P))
#define CHECK_L_R_END(X, Y)	{ dSETPOINT(pos, X, Y);	land = mGetPattn(pos.x, pos.y)->bLand;	isC = ( (land[0]!=land[1])&&(land[2]==low)&&(land[3]==low) );	if(isC)	break;	}

#define dSETPOINT(POS, X, Y)	POS.x = X;	POS.y = Y

	//절벽이 이뤄져야 돼는 필수 범위만큼 설정하도록 한다.
	for(int i=0; i<8; i++)
	{
		switch(i)
		{
		case dPUT_LEFT:
			for(n=range.top-1; n>-range.bottom; n--)
			{	IF_NOTONEPATTN(x+m_Offset[i].x, y+n, low)	PutTile(x+m_Offset[i].x, y+n, lpPattn, i);	}
			break;
		case dPUT_RIGHT:
			for(n=-range.bottom+1; n<range.top; n++)
			{	IF_NOTONEPATTN(x+m_Offset[i].x, y+n, low)	PutTile(x+m_Offset[i].x, y+n, lpPattn, i);	}
			break;
		case dPUT_TOP:	case dPUT_LT:	case dPUT_RT:
			dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.bottom);
			IF_NOTONEPATTN(pos.x, pos.y, low)	PutTile(pos.x, pos.y, lpPattn, i);
			break;
		case dPUT_BOTTOM: case dPUT_RB:	case dPUT_LB:
			dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.top);
			IF_NOTONEPATTN(pos.x, pos.y, low)	PutTile(pos.x, pos.y, lpPattn, i);
			break;
		}
	}

	//절벽세트가 모두 준비 돼지 않은 관계로 필요에 의해서 형태를 더 찍어줘야한다.
	for(i=0; i<8; i++)
	{
		BOOL isC = false;
		switch(i)
		{
			case dPUT_TOP:			//2칸 이내에 있는 같은 절벽과 합친다.
				dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.bottom);
				land = mGetPattn(pos.x, pos.y-2)->bLand;	isC = ((land[0]==low)&&(land[1]==low));
				land = mGetPattn(pos.x, pos.y-1)->bLand;	isC |= ((land[0]==low)&&(land[1]==low));
				land = mGetPattn(pos.x, pos.y)->bLand;		isC &= ((land[0]==highLand)||(land[1]==highLand));
				if(isC)		PutTileUnder(pos.x, y-2, lpPattn);
				land = mGetPattn(pos.x, pos.y)->bLand;		isC = ((land[0]==highLand)&&(land[1]==highLand));
				if(!isC)			break;
				land = mGetPattn(pos.x, pos.y-1)->bLand;	isC &= ((land[0]==low)||(land[1]==low));
				land = mGetPattn(pos.x, pos.y-2)->bLand;	isC |= ((land[0]==low)&&(land[1]==low));
				if(isC)		PutTileUnder(pos.x, y-2, lpPattn);
				break;
			case dPUT_BOTTOM:		//2칸 이내에 있는 같은 절벽과 합친다.
				dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.top);
				land = mGetPattn(pos.x, pos.y+2)->bLand;	isC = ((land[2]==low)&&(land[3]==low));
				land = mGetPattn(pos.x, pos.y+1)->bLand;	isC |= ((land[2]==low)&&(land[3]==low));
				land = mGetPattn(pos.x, pos.y)->bLand;		isC &= ((land[2]==highLand)||(land[3]==highLand));
				if(isC)		PutTileUnder(pos.x, y+1+range.bottom, lpPattn);
				land = mGetPattn(pos.x, pos.y)->bLand;		isC = ((land[2]==highLand)&&(land[3]==highLand));
				if(!isC)			break;
				land = mGetPattn(pos.x, pos.y+1)->bLand;	isC &= ((land[2]==low)||(land[3]==low));
				land = mGetPattn(pos.x, pos.y+2)->bLand;	isC |= ((land[2]==low)&&(land[3]==low));
				if(isC)		PutTileUnder(pos.x, y+1+range.bottom, lpPattn);
				break;
			case dPUT_LT:	case dPUT_RT:		//지원하지 않는 형태가 있을땐 이를 메운다.
				dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.bottom);
				land = mGetPattn(pos.x, pos.y)->bLand;
				if((land[0]==land[3])&&(land[1]==land[2])&&(land[0]!=land[1]))
					PutTileUnder(pos.x, pos.y, lpPattn);
				break;
			case dPUT_RB:	case dPUT_LB:
				dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.top);
				land = mGetPattn(pos.x, pos.y)->bLand;
				if((land[0]==land[3])&&(land[1]==land[2])&&(land[0]!=land[1]))
					PutTileUnder(pos.x, pos.y, lpPattn);
				break;			
		}
	}	

	//ㄱ자가 돼려면 까다로운 조건이 있다. 그 조건에 맞추는 작업을 한다.
	for(i=0; i<8; i++)
	{
		BOOL isC = false;

		switch(i)	{
		case dPUT_TOP:	case dPUT_LT:	case dPUT_RT:
			CHECK_L_R_END(x+m_Offset[i].x, y+m_Offset[i].y*range.bottom)
			break;
		case dPUT_RB:	case dPUT_LB:
			CHECK_L_R_END(x+m_Offset[i].x, y+m_Offset[i].y*range.top);
			break;
		case dPUT_LEFT:
			for(n=range.top-1; n>-range.bottom; n--)
				CHECK_L_R_END(x+m_Offset[i].x, y+n)
			break;
		case dPUT_RIGHT:
			for(n=-range.bottom+1; n<range.top; n++)
				CHECK_L_R_END(x+m_Offset[i].x, y+n)
			break;
		}

		if(!isC)	continue;

		DWORD dwEnd = highLand | (highLand<<8) | (land[0]<<16) | (land[1]<<24);
		DWORD dwEndInv = highLand | (highLand<<8) | (land[1]<<16) | (land[0]<<24);
		DWORD dwLand = land[0] | (land[1]<<8) | (land[0]<<16) | (land[1]<<24);
		BOOL  isInv = false;

		for(int n=1; (mGetPattn(pos.x, pos.y-n)->dwLand==dwLand)&&(pos.y-n>=0); n++);

		if( ((n<=1) && (pos.y-n>=0)) || (n>range.bottom) || (pos.y-n<0) )		continue;

		PutTileUnder(pos.x, pos.y-n+(range.bottom+1), lpPattn);
	}

	// 위나 아랫쪽 방향일 경우 high속성을 더 찍어 줘야 한다.	
	for(i=0; i<8; i++)
	{
		switch(i)	{
		case dPUT_TOP:	case dPUT_LT:	case dPUT_RT:
			dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.bottom);
			if(mGetPattn(pos.x, pos.y)->dwLand!=topCliff)	break;
			SetMapSample(pos.x, pos.y-1, highPattn);
			PutTile(pos.x-1, pos.y-1, &highPattn, dPUT_LEFT);
			PutTile(pos.x-1, pos.y-2, &highPattn, dPUT_LT);
			PutTile(pos.x-0, pos.y-2, &highPattn, dPUT_TOP);
			PutTile(pos.x+1, pos.y-2, &highPattn, dPUT_RT);
			PutTile(pos.x+1, pos.y-1, &highPattn, dPUT_RIGHT);
			break;
		case dPUT_BOTTOM: case dPUT_RB:	case dPUT_LB:
			dSETPOINT(pos, x+m_Offset[i].x, y+m_Offset[i].y*range.top);
			if(mGetPattn(pos.x, pos.y)->dwLand!=bottomCliff)	break;
			SetMapSample(pos.x, pos.y+1, highPattn);
			PutTile(pos.x+1, pos.y+1, &highPattn, dPUT_RIGHT);
			PutTile(pos.x+1, pos.y+2, &highPattn, dPUT_RB);
			PutTile(pos.x-0, pos.y+2, &highPattn, dPUT_BOTTOM);
			PutTile(pos.x-1, pos.y+2, &highPattn, dPUT_LB);
			PutTile(pos.x-1, pos.y+1, &highPattn, dPUT_LEFT);
		}
	}

//	교차 패턴은 존재 하지 않는다. 교차 패턴이 발생하면 이를 없애 줘야한다.
//	교차패턴이란... LT RB가 같은 타일이고 RT LB가 같은 패턴... NORMAL패턴에는 존재한다.
//	절벽형에는 교차 패턴이 없을 뿐 아니라 부족한 ㄱ자 형태 때문에 교차패턴의 적용범위가 좀더 넓다.
	for(i=0; i<8; i++)
	{
		switch(i)	{
		case dPUT_LT:	case dPUT_RT:
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

void CPattnCtrl::SetCliff(int x, int y, BYTE Bang, BYTE afterCliffID)
{
	LPPattnSample lpOld	= mGetPattn(x-m_Offset[Bang].x, y-m_Offset[Bang].y);	

	SetCliffDown(x, y, Bang, afterCliffID);
	SetCliffUp(x, y, Bang, afterCliffID);	
}

//	터를 확보하는 작용을 한다.
BOOL CPattnCtrl::SetCliffLow(int x, int y, BYTE Bang, DWORD bangMask, BYTE low, BYTE high)
{
	int i, n;
	PattnSample lowPattn = {low, low, low, low, };
	PattnSample highPattn = {high, high, high, high, };

	LPPattnSample lpSample = mGetPattn(x, y);
	LPCPattn lpCliff = lpSample->lpCliff;
	int bottom = lpCliff->m_range.bottom;

	//합칠필요가 있다면 합친다.
	switch(bangMask)
	{
	case T_MASK:
		for(i=1; i<bottom; i++)	{
			LPPattnSample lpSample = mGetPattn(x, y-i);
			if(lpSample->lpCliff)	{
				if( (lpSample->bLand[0]!=high) && (lpSample->bLand[1]!=high) && (lpSample->bLand[2]!=high) && (lpSample->bLand[3]!=high) )	break;
				if(lpSample->bLand[0]==high)
					for(n=0; n<=i; n++)
					{
						PutTile(x, y-n, &highPattn, dPUT_RIGHT);
						PutTile(x-1, y-n, &highPattn, dPUT_LEFT);
					}
				if(lpSample->bLand[1]==high)
					for(n=0; n<=i; n++)
					{
						PutTile(x, y-n, &highPattn, dPUT_LEFT);
						PutTile(x+1, y-n, &highPattn, dPUT_RIGHT);
					}
				return FALSE;
			}
		}
		break;	
	case LT_L_T_MASK:
		for(i=1; i<bottom; i++)	{
			LPPattnSample lpSample = mGetPattn(x, y-i);
			if(lpSample->lpCliff)	{
				if( (lpSample->bLand[0]!=high) && (lpSample->bLand[1]!=high) && (lpSample->bLand[2]!=high) && (lpSample->bLand[3]!=high) )	break;
				for(n=0; n<=i; n++)
				{
					PutTile(x, y-n, &highPattn, dPUT_LEFT);
					PutTile(x+1, y-n, &highPattn, dPUT_RIGHT);
				}
				PutTile(x, y-n, &highPattn, dPUT_LT);
				PutTile(x+1, y-n, &highPattn, dPUT_RT);
				return FALSE;
			}
		}
		break;
	case RT_R_T_MASK:
		for(i=1; i<bottom; i++)	{
			LPPattnSample lpSample = mGetPattn(x, y-i);
			if(lpSample->lpCliff)	{
				if( (lpSample->bLand[0]!=high) && (lpSample->bLand[1]!=high) && (lpSample->bLand[2]!=high) && (lpSample->bLand[3]!=high) )	break;
				for(n=0; n<=i; n++)
				{
					PutTile(x, y-n, &highPattn, dPUT_RIGHT);
					PutTile(x-1, y-n, &highPattn, dPUT_LEFT);
				}
				PutTile(x, y-n, &highPattn, dPUT_RT);
				PutTile(x-1, y-n, &highPattn, dPUT_LT);
				return FALSE;
			}
		}
		break;
	case B_MASK:
		for(i=1; i<bottom; i++)	{
			LPPattnSample lpSample = mGetPattn(x, y+i);
			if(lpSample->lpCliff)	{
				if( (lpSample->bLand[0]!=high) && (lpSample->bLand[1]!=high) && (lpSample->bLand[2]!=high) && (lpSample->bLand[3]!=high) )	break;
				if(lpSample->bLand[0]==high)
					for(n=0; n<=i; n++)
					{
						PutTile(x, y+n, &highPattn, dPUT_RIGHT);
						PutTile(x-1, y+n, &highPattn, dPUT_LEFT);
					}
				if(lpSample->bLand[1]==high)
					for(n=0; n<=i; n++)
					{
						PutTile(x, y+n, &highPattn, dPUT_LEFT);
						PutTile(x+1, y+n, &highPattn, dPUT_RIGHT);
					}			
				return FALSE;
			}
		}
		break;
	case LB_L_B_MASK:
		for(i=1; i<bottom; i++)	{
			LPPattnSample lpSample = mGetPattn(x, y+i);
			if(lpSample->lpCliff)	{
				if( (lpSample->bLand[0]!=high) && (lpSample->bLand[1]!=high) && (lpSample->bLand[2]!=high) && (lpSample->bLand[3]!=high) )	break;
				for(n=0; n<=i; n++)
				{
					PutTile(x, y+n, &highPattn, dPUT_LEFT);
					PutTile(x+1, y+n, &highPattn, dPUT_RIGHT);
				}
				PutTile(x, y+n, &highPattn, dPUT_LB);
				PutTile(x+1, y+n, &highPattn, dPUT_RB);
				return FALSE;
			}
		}
		break;
	case RB_R_B_MASK:
		for(i=1; i<bottom; i++)	{
			LPPattnSample lpSample = mGetPattn(x, y+i);
			if(lpSample->lpCliff)	{
				if( (lpSample->bLand[0]!=high) && (lpSample->bLand[1]!=high) && (lpSample->bLand[2]!=high) && (lpSample->bLand[3]!=high) )	break;
				for(n=0; n<=i; n++)
				{
					PutTile(x-1, y+n, &highPattn, dPUT_LEFT);
					PutTile(x, y+n, &highPattn, dPUT_RIGHT);					
				}
				PutTile(x, y+n, &highPattn, dPUT_RB);
				PutTile(x-1, y+n, &highPattn, dPUT_LB);
				return FALSE;
			}
		}
		break;
	}

	DWORD LB_TOP = high | high<<8 | low<<16 | high<<24;
	DWORD RB_TOP = high | high<<8 | high<<16 | low<<24;

	//낮은 지형으루 터를 확보한다.
	switch(bangMask)
	{
	case L_MASK:	PutTile(x+m_Offset[dPUT_LEFT].x, y+m_Offset[dPUT_LEFT].y, &lowPattn, dPUT_LEFT);	break;
	case R_MASK:	PutTile(x+m_Offset[dPUT_RIGHT].x, y+m_Offset[dPUT_RIGHT].y, &lowPattn, dPUT_RIGHT);	break;
	case T_MASK:	PutTile(x+m_Offset[dPUT_TOP].x, y+m_Offset[dPUT_TOP].y, &lowPattn, dPUT_TOP);		break;
	//case LT_MASK:	PutTile(x+m_Offset[dPUT_LT].x, y+m_Offset[dPUT_LT].y, &lowPattn, dPUT_LT);			break;
	//case RT_MASK:	PutTile(x+m_Offset[dPUT_RT].x, y+m_Offset[dPUT_RT].y, &lowPattn, dPUT_RT);			break;
	case LT_L_T_MASK:	PutTile(x+m_Offset[dPUT_LEFT].x, y+m_Offset[dPUT_LEFT].y, &lowPattn, dPUT_LEFT);
						PutTile(x+m_Offset[dPUT_TOP].x, y+m_Offset[dPUT_TOP].y, &lowPattn, dPUT_TOP);
						PutTile(x+m_Offset[dPUT_LT].x, y+m_Offset[dPUT_LT].y, &lowPattn, dPUT_LT);
						break;

	case RT_R_T_MASK:	PutTile(x+m_Offset[dPUT_RIGHT].x, y+m_Offset[dPUT_RIGHT].y, &lowPattn, dPUT_RIGHT);
						PutTile(x+m_Offset[dPUT_TOP].x, y+m_Offset[dPUT_TOP].y, &lowPattn, dPUT_TOP);
						PutTile(x+m_Offset[dPUT_RT].x, y+m_Offset[dPUT_RT].y, &lowPattn, dPUT_RT);
						break;

	case B_MASK:		for(i=1; i<bottom; i++)	{
							SetMapSample(x, y+i, lowPattn);
							PutTile(x, y+i, &lowPattn, dPUT_BOTTOM);
						}
						PutTile(x, y+i, &lowPattn, dPUT_BOTTOM);
						break;

	case LB_L_B_MASK:	if( mGetPattn(x-1, y-1)->dwLand == RB_TOP )	{
							for(i=1; i<bottom; i++)	{
								SetMapSample(x, y+i, lowPattn);
								PutTile(x, y+i, &lowPattn, dPUT_BOTTOM);
							}
							PutTile(x, y+i, &lowPattn, dPUT_BOTTOM);
							break;
						}

						for(i=0; i<bottom-1; i++)	{								
								PutTile(x-1, y+i, &lowPattn, dPUT_LEFT);
								PutTile(x, y+i+1, &lowPattn, dPUT_BOTTOM);
								PutTile(x-1, y+i+1, &lowPattn, dPUT_LB);
								SetMapSample(x, y+i+1, lowPattn);
						}
						PutTile(x-1, y+i, &lowPattn, dPUT_LEFT);
						PutTile(x, y+i+1, &lowPattn, dPUT_BOTTOM);
						PutTile(x-1, y+i+1, &lowPattn, dPUT_LB);
						break;

	case RB_R_B_MASK:	if( mGetPattn(x+1, y-1)->dwLand == LB_TOP )	{
							for(i=1; i<bottom; i++)	{
								SetMapSample(x, y+i, lowPattn);
								PutTile(x, y+i, &lowPattn, dPUT_BOTTOM);							
							}
							PutTile(x, y+i, &lowPattn, dPUT_BOTTOM);
							break;
						}

						for(i=0; i<bottom-1; i++)	{
							PutTile(x+1, y+i, &lowPattn, dPUT_RIGHT);
							PutTile(x+1, y+i+1, &lowPattn, dPUT_RB);
							PutTile(x, y+i+1, &lowPattn, dPUT_BOTTOM);
							SetMapSample(x, y+i+1, lowPattn);
						}
						PutTile(x+1, y+i, &lowPattn, dPUT_RIGHT);
						PutTile(x+1, y+i+1, &lowPattn, dPUT_RB);
						PutTile(x, y+i+1, &lowPattn, dPUT_BOTTOM);
						break;
	}
	return TRUE;
}

void CPattnCtrl::SetCliffDown(int x, int y, BYTE Bang, int CurrentID)
{
	int i, n;
	LPPattnSample lpSample = mGetPattn(x, y);

	if(!lpSample->lpCliff)	{
		switch(Bang){
		case dPUT_TOP: 	case dPUT_BOTTOM:
			if(mGetPattn(x, y+m_Offset[Bang].y)->lpCliff)
				SetCliffDown(x, y+m_Offset[Bang].y, Bang, CurrentID);
			break;
		case dPUT_LT: case dPUT_RT: 
			if(mGetPattn(x, y-1)->lpCliff)
				SetCliffDown(x, y+m_Offset[dPUT_TOP].y, dPUT_TOP, CurrentID);
			break;
		case dPUT_LB: case dPUT_RB:
			if(mGetPattn(x, y+1)->lpCliff)
				SetCliffDown(x, y+m_Offset[dPUT_BOTTOM].y, dPUT_BOTTOM, CurrentID);
			break;
		}
		return;
	}

	for(i=0; i<4; i++)
		if(lpSample->bLand[i]>CurrentID)
			return;

	//low와 high를 추출해내구...
	BYTE *bLand	= lpSample->bLand;
	BYTE low	= bLand[0];
	BYTE high	= (low!=bLand[3]) ? bLand[3]  : ( (low==bLand[1]) ? bLand[2] : bLand[1] );
	if(low>high) 
	{	low		= high;		high	= bLand[0];			}

	PattnSample highPattn = {high, high, high, high, }, *lpTemp;

	DWORD bangMask = (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);
	if(!SetCliffLow(x, y, Bang, bangMask, low, high))	return;
	
	LPCPattn lpCliff	= lpSample->lpCliff;
	
	RECT range			= lpCliff->m_range;

	DWORD mask			= (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);
	DWORD topFlag		= low|low<<8|high<<16|high<<24;
	DWORD bottomFlag	= high|high<<8|low<<16|low<<24;
	DWORD fillHigh		= high|high<<8|high<<16|high<<24;
	
	BOOL isWorking;

	//거지같은 타일 맞추기 땜에 위나 아래로 향할때 한칸더 찍어줘야 할때가 있다.
	switch(Bang)
	{
	case dPUT_TOP: case dPUT_LT: case dPUT_RT:
		if(mask!=T_MASK)			break;

		lpTemp = mGetPattn(x, y+2);
		isWorking = ( (lpTemp->bLand[0]!=high)||(lpTemp->bLand[1]!=high) );
		isWorking |= (lpTemp->lpUnderCliff!=NULL)&&(lpTemp->bLand[0]==lpTemp->bLand[1])&&(lpTemp->bLand[2]==lpTemp->bLand[3]);

		lpTemp = mGetPattn(x, y+1);
		isWorking |= (lpTemp->dwLand!=fillHigh);

		//왜냐면 이런 겨우라면 밑에서 깍아주기위해서 설정돼었다는 생각이 드므로 인하여...
		if((lpTemp->lpCliff)&&((lpTemp->bLandLB==low)||(lpTemp->bLandRB==low)))	isWorking = 0;

		if(!isWorking)	break;

		SetMapSample(x, y, highPattn);
		PutTile(x, y-1, &highPattn, dPUT_TOP);
		PutTile(x-1, y, &highPattn, dPUT_LEFT);
		PutTile(x+1, y, &highPattn, dPUT_RIGHT);
		PutTile(x-1, y-1, &highPattn, dPUT_LT);
		PutTile(x+1, y-1, &highPattn, dPUT_RT);
		return;

	case dPUT_BOTTOM: case dPUT_LB: case dPUT_RB:
		if(mask!=B_MASK)			break;

		lpTemp = mGetPattn(x, y-2);
		isWorking = ( (lpTemp->bLand[2]!=high)||(lpTemp->bLand[3]!=high) );
		isWorking |= (lpTemp->lpUnderCliff!=NULL)&&(lpTemp->bLand[0]==lpTemp->bLand[1])&&(lpTemp->bLand[2]==lpTemp->bLand[3]);
		isWorking |= (mGetPattn(x, y-1)->dwLand!=fillHigh);
		if(!isWorking)	{
			for(i=1; i<=range.bottom; i++)	{
				LPPattnSample lpSample = mGetPattn(x, y-i);
				if(lpSample->lpCliff)	{
					if( ((lpSample->bLand[0]>>4) > (high>>4))||((lpSample->bLand[1]>>4) > (high>>4)) )
						isWorking=true;
					break;
				}
			}
		}

		if(!isWorking)	break;

		SetMapSample(x, y, highPattn);
		PutTile(x-1, y, &highPattn, dPUT_LEFT);
		PutTile(x+1, y, &highPattn, dPUT_RIGHT);
		PutTile(x+1, y+1, &highPattn, dPUT_RB);
		PutTile(x, y+1, &highPattn, dPUT_BOTTOM);
		PutTile(x-1, y+1, &highPattn, dPUT_LB);
		return;
	}

//	역시 ㄱ자 형태일때에는 타일 부족 현상으로 인하야 모종의 조치가 피요하다.
//	그 조치란 것이 움푹들어간 양옆의 타일을 메워서 원만하게 만든다.
	switch(Bang)
	{
	case dPUT_LB: case dPUT_RB:
		if( (bLand[0]!=bLand[1]) && (bLand[2]==low) && (bLand[3]==low) )
		{
			DWORD dwLand = bLand[0] | (bLand[1]<<8) | (bLand[0]<<16) | (bLand[1]<<24);
			DWORD dwEnd = high | (high<<8) | (bLand[0]<<16) | (bLand[1]<<24);
			DWORD dwEndInv = high | (high<<8) | (bLand[1]<<16) | (bLand[0]<<24);

			BYTE *landTemp = (BYTE*)&dwLand;

			for(n=1; (mGetPattn(x, y-n)->dwLand==dwLand)&&(y-n>0); n++);
		
			if( (n>1) &&  (n<=range.bottom) && (y-n>=0) && (mGetPattn(x, y-n)->dwLand==dwEnd) )
			{
				for(i=n; i>1; i--)
				{
					SetMapSample(x, y-i, highPattn);
					PutTile(x, y-i+1, &highPattn, dPUT_BOTTOM);
					PutTile(x+((landTemp[0]==low)?-1:1), y-i, &highPattn, (landTemp[0]==low)?dPUT_LEFT:dPUT_RIGHT);
					PutTile(x+((landTemp[0]==low)?-1:1), y+1-i, &highPattn, (landTemp[0]==low)?dPUT_LB:dPUT_RB);
				}
				return;
			}		
		}
	}

//	역시 ㄱ자 형태일때 들어간 부분이라 움푹 나온 부분까지 메워준당 ㅋㅋㅋ
//	그 조치란 것이 튀어나온 부분까지 맞춰서 메운당ㅋㅋㅋ
	switch(Bang)
	{
	case dPUT_LB: case dPUT_RB: case dPUT_BOTTOM:
		if( (bLand[2]!=bLand[3]) && (bLand[0]==high) && (bLand[1]==high) )
		{
			DWORD dwLand = bLand[2] | (bLand[3]<<8) | (bLand[2]<<16) | (bLand[3]<<24);
			DWORD dwEnd = bLand[2] | bLand[3]<<8 | low<<16 | low<<24;

			BYTE *landTemp = (BYTE*)&dwLand;

			for(n=1; (mGetPattn(x, y+n)->dwLand==dwLand)&&(y+n<m_wYRange); n++);
		
			if( (n>1) &&  (n<=range.bottom) && (y+n<m_wYRange) && (mGetPattn(x, y+n)->dwLand==dwEnd) )
			{
				for(i=0; i<n-1; i++)
				{
					SetMapSample(x, y+i, highPattn);
					PutTile(x, y+i+1, &highPattn, dPUT_BOTTOM);
					PutTile(x+((landTemp[2]==low)?-1:1), y+i, &highPattn, (landTemp[2]==low)?dPUT_LEFT:dPUT_RIGHT);
					PutTile(x+((landTemp[2]==low)?-1:1), y+i+1, &highPattn, (landTemp[2]==low)?dPUT_LB:dPUT_RB);
				}
				return;
			}		
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void CPattnCtrl::SetUnderCliff(int x, int y, int Bang)
{ 	
	int i, n;
	LPPattnSample lpSample = mGetPattn(x, y);

	if(!lpSample->lpUnderCliff)	{
		switch(Bang)
		{
		case dPUT_TOP: 	case dPUT_BOTTOM:
			if(mGetPattn(x, y+m_Offset[Bang].y)->lpUnderCliff)
				SetUnderCliff(x, y+m_Offset[Bang].y, Bang);
			break;
		case dPUT_LT: case dPUT_RT: 
			if(mGetPattn(x, y-1)->lpUnderCliff)
				SetUnderCliff(x, y+m_Offset[dPUT_TOP].y, dPUT_TOP);
			break;
		case dPUT_LB: case dPUT_RB:
			if(mGetPattn(x, y+1)->lpUnderCliff)
				SetUnderCliff(x, y+m_Offset[dPUT_BOTTOM].y, dPUT_BOTTOM);
			break;
		}
		return;
	}

	BYTE *bLand = lpSample->bLand;
	BYTE low = lpSample->lpUnderCliff->m_bLowFlag1 | (*bLand&0xf0);
	BYTE high = lpSample->lpUnderCliff->m_bHighFlag1 | (*bLand&0xf0);

	LPCPattn lpUnder = lpSample->lpUnderCliff;

	RECT range = lpUnder->m_range;

	DWORD mask			= (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);
	DWORD topFlag		= low|low<<8|high<<16|high<<24;
	DWORD bottomFlag	= high|high<<8|low<<16|low<<24;
	DWORD fillHigh		= high|high<<8|high<<16|high<<24;

	PattnSample highPattn = {high, high, high, high, }, *lpTemp;

	BOOL isWorking;

	//거지같은 타일 맞추기 땜에 위나 아래로 향할때 한칸더 찍어줘야 할때가 있다.
	switch(Bang)
	{
	case dPUT_TOP: case dPUT_LT: case dPUT_RT:
		if(mask!=T_MASK)			break;

		lpTemp = mGetPattn(x, y+2);
		isWorking = ( (lpTemp->bLand[0]!=high)||(lpTemp->bLand[1]!=high) );
		if(lpTemp->lpCliff)	{
			BYTE *land = lpTemp->bLand;
			isWorking = ( (land[0]!=land[2]) && (land[0]==land[1]) && (land[2]==land[3]) );
		}
		isWorking |= (mGetPattn(x, y+1)->dwLand!=fillHigh);
		if(!isWorking)	break;

		SetMapSample(x, y, highPattn);
		PutTile(x, y-1, &highPattn, dPUT_TOP);
		PutTile(x-1, y, &highPattn, dPUT_LEFT);
		PutTile(x+1, y, &highPattn, dPUT_RIGHT);
		PutTile(x-1, y-1, &highPattn, dPUT_LT);
		PutTile(x+1, y-1, &highPattn, dPUT_RT);
		return;

	case dPUT_BOTTOM: case dPUT_LB: case dPUT_RB:
		if(mask!=B_MASK)			break;

		lpTemp = mGetPattn(x, y-2);
		isWorking = ( (lpTemp->bLand[2]!=high)||(lpTemp->bLand[3]!=high) );
		if(lpTemp->lpCliff)	{
			BYTE *land = lpTemp->bLand;
			isWorking = ( (land[0]!=land[2]) && (land[0]==land[1]) && (land[2]==land[3]) );
		}
		isWorking |= (mGetPattn(x, y-1)->dwLand!=fillHigh);
		if(!isWorking)	break;

		SetMapSample(x, y, highPattn);
		PutTile(x-1, y, &highPattn, dPUT_LEFT);
		PutTile(x+1, y, &highPattn, dPUT_RIGHT);
		PutTile(x+1, y+1, &highPattn, dPUT_RB);
		PutTile(x, y+1, &highPattn, dPUT_BOTTOM);
		PutTile(x-1, y+1, &highPattn, dPUT_LB);
		return;
	}

//	역시 ㄱ자 형태일때에는 타일 부족 현상으로 인하야 모종의 조치가 피요하다.
//	그 조치란 것이 움푹들어간 양옆의 타일을 메워서 원만하게 만든다.
	switch(Bang)
	{
	case dPUT_LB: case dPUT_RB:
		if( (bLand[0]!=bLand[1]) && (bLand[2]==low) && (bLand[3]==low) )
		{
			DWORD dwLand = bLand[0] | (bLand[1]<<8) | (bLand[0]<<16) | (bLand[1]<<24);
			DWORD dwEnd = high | (high<<8) | (bLand[0]<<16) | (bLand[1]<<24);
			DWORD dwEndInv = high | (high<<8) | (bLand[1]<<16) | (bLand[0]<<24);

			BYTE *landTemp = (BYTE*)&dwLand;

			for(n=1; (mGetPattn(x, y-n)->dwLand==dwLand)&&(y-n>0); n++);
		
			if( (n>1) &&  (n<=range.bottom) && (y-n>=0) && (mGetPattn(x, y-n)->dwLand==dwEnd) )
			{
				for(i=n; i>1; i--)
				{
					SetMapSample(x, y-i, highPattn);
					PutTile(x, y-i+1, &highPattn, dPUT_BOTTOM);
					PutTile(x+((landTemp[0]==low)?-1:1), y-i, &highPattn, (landTemp[0]==low)?dPUT_LEFT:dPUT_RIGHT);
					PutTile(x+((landTemp[0]==low)?-1:1), y+1-i, &highPattn, (landTemp[0]==low)?dPUT_LB:dPUT_RB);
				}
				return;
			}		
		}
	}

//	역시 ㄱ자 형태일때 들어간 부분이라 움푹 나온 부분까지 메워준당 ㅋㅋㅋ
//	그 조치란 것이 튀어나온 부분까지 맞춰서 메운당ㅋㅋㅋ
	switch(Bang)
	{
	case dPUT_LB: case dPUT_RB: case dPUT_BOTTOM:
		if( (bLand[2]!=bLand[3]) && (bLand[0]==high) && (bLand[1]==high) )
		{
			DWORD dwLand = bLand[2] | (bLand[3]<<8) | (bLand[2]<<16) | (bLand[3]<<24);
			DWORD dwEnd = bLand[2] | bLand[3]<<8 | low<<16 | low<<24;

			BYTE *landTemp = (BYTE*)&dwLand;

			for(n=1; (mGetPattn(x, y+n)->dwLand==dwLand)&&(y+n<m_wYRange); n++);
		
			if( (n>1) &&  (n<=range.bottom) && (y+n<m_wYRange) && (mGetPattn(x, y+n)->dwLand==dwEnd) )
			{
				for(i=0; i<n-1; i++)
				{
					SetMapSample(x, y+i, highPattn);
					PutTile(x, y+i+1, &highPattn, dPUT_BOTTOM);
					PutTile(x+((landTemp[2]==low)?-1:1), y+i, &highPattn, (landTemp[2]==low)?dPUT_LEFT:dPUT_RIGHT);
					PutTile(x+((landTemp[2]==low)?-1:1), y+i+1, &highPattn, (landTemp[2]==low)?dPUT_LB:dPUT_RB);
				}
				return;
			}
		}
	}

//	lt_rt_end와 lb_rb_end가 아래 위로 맞닿아 있다면...(그래서 타일이 깨진다면....)

	switch(mask)
	{
	case RT_LB_MASK:	case RB_LT_MASK:
		AddRePutList(x, y, highPattn);
		return;
	}

	switch(Bang)
	{
	case dPUT_LB: case dPUT_RB:
		if( (mask!=LB_L_B_MASK) && (mask!=RB_R_B_MASK) )	break;

		for(i=1; y+i<m_wYRange; i++)	{
			if(i>=range.bottom)			break;

			lpTemp = mGetPattn(x, y+i);
			if(!lpTemp->lpUnderCliff)	continue;

			bLand = lpTemp->bLand;
			DWORD emask = (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);

			switch(emask)	{
			case LT_L_T_MASK:
				if(mask==LB_L_B_MASK)	break;
				AddRePutList(x, y+1, highPattn);
				break;
			case RT_R_T_MASK:
				if(mask==RB_R_B_MASK)	break;
				AddRePutList(x, y+1, highPattn);
				break;
			}
			break;
		}
		break;

	case dPUT_LT: case dPUT_RT:
		if( (mask!=LT_L_T_MASK) && (mask!=RT_R_T_MASK) )	break;

		for(i=1; y-i>=0; i++)	{
			if(i>=range.bottom)			break;

			lpTemp = mGetPattn(x, y-i);
			if(!lpTemp->lpUnderCliff)	continue;

			bLand = lpTemp->bLand;
			DWORD emask = (bLand[0]==low)<<24|(bLand[1]==low)<<16|(bLand[2]==low)<<8|(bLand[3]==low);

			switch(emask)	{
			case LB_L_B_MASK:
				if(mask==LT_L_T_MASK)	break;
				AddRePutList(x, y-1, highPattn);
				break;
			case RB_R_B_MASK:
				if(mask==RT_R_T_MASK)	break;
				AddRePutList(x, y-1, highPattn);
				break;
			}
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////