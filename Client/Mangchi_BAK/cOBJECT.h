#ifndef _classOBJECT_H
#define _classOBJECT_H

#include "SFC.H"
#include "cIMAGE.H"

#define	dMAX_OBJECT		10000
#define	dMAX_OVERLAP	100

#define	dVALID_OBJECT		0xffff
#define	dVALID_FIXED_OBJECT	0xff

class cSAVE_TINY_OBJECT_INFO
{
public:
	cPOINT			m_pos;
	WORD			m_wObject;			//	인덱스,세부 인덱스
};

class cTINY_OBJECT_INFO	:	public cSAVE_TINY_OBJECT_INFO
{
public:
	WORD			m_wSerial;
	BOOL			m_isSelected;		//	선택 되었다?

					cTINY_OBJECT_INFO()
					{
						reset();
					};
	inline	void	copy(cTINY_OBJECT_INFO *_lpObject)
	{
		memcpy(this,_lpObject,sizeof(cTINY_OBJECT_INFO));
	}

	void			reset()
	{
		m_wSerial		=	0xffff;
		m_isSelected	=	FALSE;
		m_wObject		=	0xffff;
	}
};

class cADD_ON_OBJECT
{
public:
	WORD			m_wObject;		//	오브젝트
	short			m_sDx,m_sDy;	//	좌표

	BOOL			isSame(cADD_ON_OBJECT* _lpAddOnObject)
	{
		if(m_wObject != _lpAddOnObject->m_wObject)
			return FALSE;

		if(m_sDx != _lpAddOnObject->m_sDx)
			return FALSE;

		if(m_sDy != _lpAddOnObject->m_sDy)
			return FALSE;

		return TRUE;
	}
};

#define	dMAX_ADD_ON_OBJECT_COUNT	8

class cSAVE_FIXED_OBJECT_INFO_OLD_031009
{
public:
	WORD			m_wSerial;
	cPOINT			m_pos;
	WORD			m_wObject;
	cADD_ON_OBJECT	m_aAddonObject[10];
};

class cSAVE_FIXED_OBJECT_INFO
{
public:
	WORD			m_wSerial;
	cPOINT			m_pos;
	WORD			m_wObject;
	cADD_ON_OBJECT	m_aAddonObject[dMAX_ADD_ON_OBJECT_COUNT];
	WORD			m_wIsPutShadow;
};

class	cFIXED_OBJECT_INFO	:	public cSAVE_FIXED_OBJECT_INFO
{
public:
	BOOL			m_isSelected;

					cFIXED_OBJECT_INFO()
					{
						reset();
					}

	inline	void	copy(cFIXED_OBJECT_INFO *_lpObject) 
	{
		memcpy(this,_lpObject,sizeof(cFIXED_OBJECT_INFO));
	}

	void			reset()
	{
		m_wSerial		=	0xffff;
		m_wObject		=	0xffff;
		m_isSelected	=	FALSE;
		m_wIsPutShadow	=	TRUE;

		memset(m_aAddonObject,0xff,sizeof(m_aAddonObject));
	}

	BOOL			addAddonObject(int _iObject,int _iX,int _iY)
	{
		for (int i=0;i<dMAX_ADD_ON_OBJECT_COUNT;i++)
		{
			if (m_aAddonObject[i].m_wObject	==	0xffff)
			{
				m_aAddonObject[i].m_wObject	=	_iObject;
				m_aAddonObject[i].m_sDx		=	_iX-m_pos.x;
				m_aAddonObject[i].m_sDy		=	_iY-m_pos.y;

				return	TRUE;
			}
		}

		return	FALSE;
	}

	void			moveAddonObject(int _iIndex,int _iDx,int _iDy)
	{
		if (m_aAddonObject[_iIndex].m_wObject	==	0xffff)	return;

		m_aAddonObject[_iIndex].m_sDx	+=	_iDx;
		m_aAddonObject[_iIndex].m_sDy	+=	_iDy;
	}

	void				removeAddonObject(int _iIndex)
	{
		m_aAddonObject[_iIndex].m_wObject	=	0xffff;

		if (_iIndex	<	dMAX_ADD_ON_OBJECT_COUNT-1)
			memcpy(m_aAddonObject+_iIndex,m_aAddonObject+_iIndex+1,sizeof(cADD_ON_OBJECT)*(dMAX_ADD_ON_OBJECT_COUNT-_iIndex-1));
	}
};

//JBC	08-08-12
struct COPY_BLOCK_SAVE
{
	int		iObject;
	int		iX;
	int		iY;
};

class cOBJECT
{
public:
	cDEN<cFIXED_OBJECT_INFO>	m_denFixedObject;
	cDEN<cTINY_OBJECT_INFO>		m_denTinyObject;
	
	cOBJ_ANM		*m_pFixedObjectImage,*m_pTinyObjectImage;
	int				m_iFixedObjectImageCount,m_iTinyObjectImageCount;
	int				m_iSelectFixedObject,m_iSelectTinyObject;
	int				m_iTileSet;
	int				m_iSelectFixedObjectCount,m_iSelectTinyObjectCount;
	int				m_iSelectAddonObject;

	BYTE			*m_pSelectFixedObjectList,*m_pSelectTinyObjectList;

	//JBC	08-08-12
	COPY_BLOCK_SAVE *m_ptagBlockSave;			//정적 오브젝트를 보관
	COPY_BLOCK_SAVE *m_ptagTinyBlockSave;		//티니 오브젝트를 보관
	int				CopyStaticObject();
	int				CopyTinyObject();
	BOOL			PasteStaticObject(int _iCount);
	BOOL			PasteTinyObject(int _iTinyCount , int _iCount);
	cPOINT			GetLeftBouttomPoint(int _iCount);

					cOBJECT();

	BOOL			isDoor(int _iIndex);

	void			put(int _iDx,int _iDy,cTINY_OBJECT_INFO *_lpObjcet);
	void			putScale(int _iDx,int _iDy,cTINY_OBJECT_INFO *_lpObjcet,int _iScale);
	void			putScale(int _iDx,int _iDy,cTINY_OBJECT_INFO *_lpObjcet,int _iTBSize,int _iScale);

	void			put(int _iDx,int _iDy,int _iIndex);
	void			putScale(int _iDx,int _iDy,int _iIndex,int _iScale);
	void			putScale(int _iDx,int _iDy,int _iIndex,int _iTBSize,int _iScale);

	void			drawAllRapeObject(cRECT *mapClient,int dx,int dy);
	void			drawAllRapeObjectScale(cRECT *mapClient,int dx,int dy,int _iScale);
	void			drawAllRapeObjectScale(cRECT *mapClient,int dx,int dy,int _iTBSize,int _iScale);
	void			drawRapeObjectScale(cRECT *mapClient,int dx,int dy,int _iScale);

	void			drawAllFloatObject(cRECT *mapClient,int dx,int dy);
	void			selectFixedObject(int object,BOOL shift,BOOL control);
	void			selectTinyObject(int object,BOOL shift,BOOL control);
	int				getStandObjects(cINDEX *indexes,cRECT *viewport);	//	뷰포트 안에 캐릭터들을 구한다.
	int				getStandObjects(cINDEX *indexes);	//	뷰포트 안에 캐릭터들을 구한다.
	int				getSelectedTinyObjectCount();					// 09.07.28  선택된 tinyObjectCount

	BOOL			init(int tileSet,cNUX *nux);						//	초기화

	inline	cTINY_OBJECT_INFO	*getTinyObject(int _iIndex){return	m_denTinyObject.get(_iIndex);}
	inline	int		getTinyObjectCount()	{return	m_denTinyObject.getCount();}
	inline	BYTE*	getTinyObjectCrashInfo(int _iObject){return	m_pTinyObjectImage[_iObject].pCRASH;}
	inline	BYTE*	getTinyObjectBlankInfo(int _iObject){return	m_pTinyObjectImage[_iObject].pBLOCK;}
	inline	BYTE*	getTinyObjectOverlapInfo(int _iObject){return	m_pTinyObjectImage[_iObject].pOVERLAPBLOCK;}
	inline	cOBJ_ANM*	getTinyObjectImage(int _iObject){return	&m_pTinyObjectImage[_iObject];}

	inline	cFIXED_OBJECT_INFO	*getFixedObject(int _iIndex){return	m_denFixedObject.get(_iIndex);}
	inline	int		getFixedObjectCount()	{return	m_denFixedObject.getCount();}
	inline	BYTE*	getFixedObjectCrashInfo(int _iObject){return	m_pFixedObjectImage[_iObject].pCRASH;}
	inline	BYTE*	getFixedObjectBlankInfo(int _iObject){return	m_pFixedObjectImage[_iObject].pBLOCK;}
	inline	BYTE*	getFixedObjectOverlapInfo(int _iObject){return	m_pFixedObjectImage[_iObject].pOVERLAPBLOCK;}
	inline	cOBJ_ANM*	getFixedObjectImage(int _iObject){return	&m_pFixedObjectImage[_iObject];}

	void			reset();									//	데이터 리셋
	void			close();

	void			putFixedObject(cFIXED_OBJECT_INFO *_lpObject,int _iX,int _iY);//등록된 오브젝트 찍기-o-
	void			putFixedObjectScale(cFIXED_OBJECT_INFO *_lpObject,int _iX,int _iY,int _iScale);//등록된 오브젝트 찍기-o-
	void			putFixedObjectScale(cFIXED_OBJECT_INFO *_lpObject,int _iX,int _iY,int _iTBSize,int _iScale);//등록된 오브젝트 찍기-o-

	void			putFixedObject( int index,int x,int y,int xs,int ys);	//	찍어주기(사이즈에 맞춤)
	void			putFixedObject( int index,int x,int y,BOOL red=FALSE);	//	찍기
	void			putFixedObjectLayer( int index,int x,int y);				//	레이어 찍기
	void			putFixedObjectLayerScale( int index,int x,int y,int _iScale);				//	레이어 찍기
	void			putFixedObjectShadow( int index,int x,int y);			//	그림자 찍기
	void			putFixedObjectShadowScale( int index,int x,int y,int _iScale);			//	그림자 찍기
	void			putFixedObjectScale( int index,int x,int y,int _iScale);	//	찍기
	void			putFixedObjectScale( int index,int x,int y,int _iTBSize,int _iScale);	//	찍기

	void			putTinyObject( int index,int x,int y,int xs,int ys);		//	찍어주기(사이즈에 맞춤)
	void			putTinyObject( int index,int x,int y,BOOL red=FALSE);		//	찍기
	void			putTinyObjectLayer( int index,int x,int y);				//	레이어 찍기
	void			putTinyObjectLayerScale( int index,int x,int y,int _iScale);				//	레이어 찍기
	void			putTinyObjectShadow( int index,int x,int y);				//	그림자
	void			putTinyObjectScale( int index,int x,int y,int _iScale);		//	찍어주기(사이즈에 맞춤)
	void			putTinyObjectShadowScale( int index,int x,int y,int _iScale);				//	그림자

	void			select(cRECT *_lpRect,BOOL _bIsSelectSameObjectWithSelectedObject=FALSE);				//	오브젝트 선택
	void			freeSelect();

	BOOL			isCrashTinyObject(int object,int x,int y);			//	충돌 체크

	void			removeSelectedAll();								//	선택된거 전부 제거
	inline	BOOL	removeTinyObject(int _iIndex){return 	m_denTinyObject.remove(_iIndex);}//	제거
	inline	BOOL	removeFixedObject(int _iIndex){return 	m_denFixedObject.remove(_iIndex);}//	제거


	void			checkSelectFixedObjectShadow(BOOL _bIsCheck);	//	선택된 고정 오브젝트의 그림자 상태를 설정한다.
	int				getSelectedFixedObjectCount();	//선택된 고정된 오브젝트 수
	int				getSelectedObjectCount();		//선택된 오브젝트 수
	int				getSelectedObject(BOOL _bIsWantObject=TRUE);			//선택된 오브젝트

	int				getSelectFixedObject(int _iObject);//선택된 오브젝트들중 하나 선택
	int				getSelectTinyObject(int _iObject);//선택된 오브젝트들중 하나 선택
	int				addTinyObject(int _iObject,int _iX,int _iY);//추가
	int				addFixedObject(int _iObject,int _iX,int _iY);//추가

	BOOL			lineUpSaveObject(int index);
	void			rebuildFixedObjectList();

	inline	BOOL	isBridgeObject(int _iObject)
	{
		if (_iObject < 0 ||  _iObject >= m_iTinyObjectImageCount ) return FALSE;

		return m_pTinyObjectImage[_iObject].IsBridge();
	}
	inline	BOOL	isDoorObject(int _iObject)
	{
		if (_iObject < 0 ||  _iObject >= m_iFixedObjectImageCount ) return FALSE;

		return m_pFixedObjectImage[_iObject].isDoor();
	}
	inline	BOOL	isFixedObject(int _iObject)
	{
		if (_iObject < 0 ||  _iObject >= m_iTinyObjectImageCount ) return FALSE;

		return m_pTinyObjectImage[_iObject].IsFixed();
	}
	inline	BOOL	isSelectTinyObject(int index)
	{
		if (index >= m_iTinyObjectImageCount)	return FALSE;

		return	m_pSelectTinyObjectList[index];
	}

	inline	BOOL	isSelectFixedObject(int index)
	{
		if (index >= m_iFixedObjectImageCount)	return FALSE;

		return	m_pSelectFixedObjectList[index];
	}
	inline	BOOL	isRapeObject(int _iObject)
	{
		if (m_pTinyObjectImage[_iObject].GetAttr()	==	dOBJECT_RAPE)	return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isStandObject(int _iObject)
	{
		if (m_pTinyObjectImage[_iObject].GetAttr()	==	dOBJECT_NORMAL)	return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isFloatObject(int _iObject)
	{
		if (m_pTinyObjectImage[_iObject].GetAttr()	==	dOBJECT_FLOAT)	return	TRUE;

		return	FALSE;
	}
};

extern	cSAVE_TINY_OBJECT_INFO	LineUpedObject[dMAX_OBJECT];
extern	cINDEX					LineUpRape[100];
extern	cINDEX					LineUpNormal[100];
extern	cINDEX					LineUpFloat[100];

#endif