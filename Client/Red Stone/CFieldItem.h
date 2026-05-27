#ifndef _classFIELD_ITEM_H
#define	_classFIELD_ITEM_H

#include "cNEMO.H"
#include "cItem.H"
#include "CImageData.H"
#include "CGamePlay.H"

#define	dFIELD_ITEM_COUNT			1024
#define	dPICK_ABLE_RANGE_FOR_CLIENT	40000	// 20240907 sacchan 10000 40000
#define	dPARTY_FLAG					0x8000

#pragma	pack(2)
//
//	필드에 떨어진 아이템
class	cDroppedItem
{
public:
	WORD					m_wSerial;//시리얼
	WORD					m_wBaseItem;
	WORD					m_wType;	//	레어~
	DWORD					m_dwCount;
	CPos					m_pos;//어디에 떨어져 있냐?
	cRECT					m_rect;
	WORD					m_wPixelWidth;

							cDroppedItem()	{reset();}

	inline	cBasicItem*		getBasicItem()	{return	&g_aBasicItem[m_wBaseItem];}//기본 아이템 정보
	inline	void			copy(cDroppedItem *_lpDroppedItem)	{memcpy(this,_lpDroppedItem,sizeof(cDroppedItem));}

	void					put(int _iDx,int _iDy,BOOL _isOutline);
	inline	BOOL			isFocus(int x,int y)
	{
		cRECT	rect;

		getRect(&rect);

		return rect.isIn(x,y);
	}

	inline	void			getRect(cRECT*	_lpRect)
	{
		int	iShape		=	g_aBasicItem[m_wBaseItem].m_wFieldShape;

		if (m_wBaseItem	==	dITEM_MONEY)
		{
					if (iShape	>=	1000)	iShape+=3;
			else	if (iShape	>=	100)	iShape+=2;
			else	if (iShape	>=	10)		iShape+=1;
		}

		g_sprFieldItem.getRect(iShape,_lpRect);
		_lpRect->add(m_pos.x,m_pos.y);
		_lpRect->expand(5,5);
	}
	char*					getName();

	inline	void			reset()
	{
		memset(this,0,sizeof(cDroppedItem));
		m_wSerial		=	0xffff;//시리얼
		m_wBaseItem		=	0xffff;//돈 -o-
	}
};	//	class	cDroppedItem

#pragma	pack()

#define	dMAX_DROPPED_ITEM			1024
#define	dFIELD_ITEM_SHINING_PERIOD	(dSYNC_FPS*5)

class	cFieldItem	:	public CGamePlay
{
public:
	cDen<cDroppedItem>		m_den;
	int						m_iFocusItem;
	int						m_iPickItem;
	int						m_iFrameCounter,m_iFrame,m_iRemainTimeForShinigItem;
	BOOL					m_bIsShowDroppedItemName;
	int						m_iFindRectangleCount;

							cFieldItem();
							~cFieldItem();

	inline	void			init()	{m_den.init(dMAX_DROPPED_ITEM);}//초기화
	void					reset();//	리셋
	inline	void			close()	{m_den.close();}//종료
	void					update();	//	업데이트
	void					updateNameTagBox();

	BOOL					isCrashRectangle(cRECT *_lpRect,int _iCount);
	void					getSuitableRectangle(cDroppedItem *_lpDroppedItem,int _iCount);
	void					getNotCrashRectangle(cRECT *_lpRect,int _iCount,int _iDirect);
	cDroppedItem*			pickNearestItem(int _iX,int _iY);

	inline	int				getCount(){return m_den.getCount();}//카운트
	inline	cDroppedItem*	get(int _iIndex){return	m_den.get(_iIndex);}
	inline	cDroppedItem*	getFocusedItem(){return	m_den.get(m_iFocusItem);}	//	포커스된 아이템 리턴
	inline	int				add(cDroppedItem *_lpItem)
	{
		_lpItem->m_wPixelWidth	=	0;
		
		return m_den.add(_lpItem);
	}//추가
	inline	int				insert(cDroppedItem *_lpItem)
	{
		_lpItem->m_wPixelWidth	=	0;
		return m_den.insert(_lpItem->m_wSerial,_lpItem);
	}//추가
	inline	void			releasePickItem()	//	아이템 줍기 취소.
	{
		m_iPickItem			=	0xffff;
		m_iFocusItem		=	0xffff;
	}

	inline	void			rmovePickItem()	
	{
		remove(m_iPickItem);
		releasePickItem();
	}

	void					remove(int _iSerial);//_iSerial제거

	inline	void			put(int _iIndex,int _iDx,int _iDy,BOOL _isOutLine)
	{
		cDroppedItem	*lpDroppedItem	=	m_den.get(_iIndex);

		lpDroppedItem->put(_iDx,_iDy,_isOutLine);
	}

	BOOL					findFocusItem(int _iX,int _iY,BOOL _bIsFindFocus);	//	포커스 아이템 찾기
	void					draw(int _iDx,int _iDy);		//	그려주기..
	void					drawAllFieldItemName(int _iDx,int _iDy);
	void					addPaint(int _iDx,int _iDy);//	빤짝이 그려주기..
};

extern	cFieldItem			g_fieldItem;

#endif