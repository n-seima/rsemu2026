/*************************************************************************************************
	cOBJECT.H

	Red Stone	오브젝트 관리 클래스... 덤으로 건물도... --;; 데이터형이 같은지라 얼렁뚱땅 끼워 넣
	다.

	작성  날짜 : 2000.12.18
	작  성  자 : 이영찬

*************************************************************************************************/
#ifndef _classOBJECT_H
#define _classOBJECT_H

#include "cNEMO.H"

#define	dMAX_TINY_OBJECT_COUNT		10000
#define	dMAX_FIXED_OBJECT_COUNT		10000
#define	dOBJECT_IMAGE_COUNT			1024

#define	dOBJECT_STATIC			0
#define	dOBJECT_FREE			1
#define	dOBJECT_BUILDING		2

class cTINY_OBJECT_INFO
{
public:
	CPos			m_pos;
	WORD			m_wObject;			//	인덱스,세부 인덱스
};

#define	dMAX_ADD_ON_OBJECT_COUNT	8

class cADD_ON_OBJECT
{
public:
	WORD			m_wObject;		//	오브젝트
	short			m_sDx,m_sDy;	//	좌표
};

class cSAVE_FIXED_OBJECT_INFO
{
public:
	WORD			m_wSerial;
	CPos			m_pos;
	WORD			m_wObject;
	cADD_ON_OBJECT	m_aAddonObject[dMAX_ADD_ON_OBJECT_COUNT];
	WORD			m_wIsPutShadow;
};

enum
{
	eOIS_NORMAL,
	eOIS_SHINING,
	eOIS_SHAKE,
	eOIS_TRAP,
	eOIS_HIDE,
};

class cFIXED_OBJECT_INFO
{
public:
	WORD			m_wObject;
	cADD_ON_OBJECT	m_aAddonObject[dMAX_ADD_ON_OBJECT_COUNT];
	WORD			m_wIsPutShadow;

	WORD			m_bf1IsDoor			:	1;
	WORD			m_bf1IsActive		:	1;
	WORD			m_bf1IsOpened		:	1;
	WORD			m_bf1IsDisarmed		:	1;
	WORD			m_bf1IsUnlocked		:	1;
	WORD			m_bf1IsHide			:	1;
	WORD			m_bf1IsDetectedDoor	:	1;
	WORD			m_bf1IsDetectedTrap	:	1;
	WORD			m_bf1IsFocused		:	1;
	WORD			m_bf1IsLinkSecretDungeon	:	1;

	WORD			m_wImageStatus;
	WORD			m_wUpkeepTime;
	WORD			m_wX,m_wY;
};

#define	dMAX_ADDON_IMAGE_COUNT				20
#define	dMAX_ADDON_OBJECT_COUNT_ON_BUILDING	4
#define	dMAX_BUILDING_COUNT_IN_MAP			255

class	cSAVE_BUILDING_INFO
{
public:
	WORD			m_wSerial;
	CPos			m_pos;
	WORD			m_wBuilding;
	WORD			m_isPutShadow;
	WORD			m_wAddonBlockCount,m_wAddonObjectCount;	//	덧붙는 이미지의 수
	WORD			m_aAddonBlock[dMAX_ADDON_IMAGE_COUNT];
	cADD_ON_OBJECT	m_aAddonObject[dMAX_ADDON_OBJECT_COUNT_ON_BUILDING];
};

class	cBUILDING_INFO
{
public:
	WORD			m_wBuilding;
	WORD			m_isPutShadow;
	WORD			m_wAddonBlockCount,m_wAddonObjectCount;	//	덧붙는 이미지의 수
	WORD			m_aAddonBlock[dMAX_ADDON_IMAGE_COUNT];
	cADD_ON_OBJECT	m_aAddonObject[dMAX_ADDON_OBJECT_COUNT_ON_BUILDING];
};

class	cOBJECT
{
public:
	int						m_iTileSet;		//	오브젝트 세트와 건물 세트
	BOOL					m_isMCU;

	BOOL					m_bIsGuildHall;
	int						m_iGS_Nar,m_iGS_Nen,m_iGS_Sul,m_iGS_Kemen,m_iGS_Lark,m_iGS_Siroio;
	BYTE					m_abGuildStatueStatus[6][5];

	BYTE					m_aExistTinyObject[dOBJECT_IMAGE_COUNT],m_aExistFixedObject[dOBJECT_IMAGE_COUNT];
	cOBJ_ANM				m_aTinyObjectImage[dOBJECT_IMAGE_COUNT],m_aFixedObjectImage[dOBJECT_IMAGE_COUNT],m_aBuildingImage[dOBJECT_IMAGE_COUNT];
	int						m_iFixedObjectImageCount,m_iTinyObjectImageCount,m_iBuildingImageCount;	//	데이터 수(이미지)
	BOOL					m_bIsCheckObjectInfoFailed;

	int						m_iTinyObjectCount,m_iFixedObjectCount,m_iBuildingCount;			//	오브젝트 수
	cFIXED_OBJECT_INFO		m_aFixedObjectList[dMAX_FIXED_OBJECT_COUNT];	//	오브젝트(임의의 위치에 찍히는..)
	cTINY_OBJECT_INFO		m_aTinyObjectList[dMAX_TINY_OBJECT_COUNT];		//	오브젝트(임의의 위치에 찍히는..)
	cBUILDING_INFO			m_aBuildingList[dMAX_BUILDING_COUNT_IN_MAP];
	cINDEX					m_aRapes[100],m_aStands[100],m_aFloats[100];	//	오브젝트 정렬

							cOBJECT();
							~cOBJECT();

	void					close();

	void					reset();
	void					resetTinyObjectList();
	void					resetObjectData();
	void					resetBuildingData();

	BOOL					readObjectListInMap(cFILE *_lpFile,BOOL _bIsBuildExistObjectList);
	BOOL					loadImageData(int _iObjectSet,int _iScale,BOOL _bIsLoadOnlyExistObject=FALSE,cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL);

	void					drawRapes(int dx,int dy);//바닥에 깔리는 넘들 다 그린다.
	void					drawFloatObject(int dx,int dy);//둥둥 떠 다니는 넘들 다 그린다.

	inline	cOBJ_ANM*		getFixedObjectImage(int _iObject)
	{
		if (_iObject	>=	m_iFixedObjectImageCount)
			return	NULL;
		
		return &m_aFixedObjectImage[_iObject];
	}
	inline	cOBJ_ANM*		getFixedObjectImageByData(int _iObject)
	{
		return &m_aFixedObjectImage[m_aFixedObjectList[_iObject].m_wObject];
	}

	void					changeDoorStatus(int _iObjectIndex,BOOL _bIsSleep,BOOL _bIsHide,BOOL _bIsLocked,BOOL _bIsTrap,BOOL _bIsClose,BOOL _bIsDetectedDoor,BOOL _bIsDetectedTrap,BOOL _bIsLinkSecretDungeon);
	void					changeImageStatus(int _iObjectIndex,int _iStatus,int _iUpkeepTime,BOOL _bIsDecreaseUpkeepTime=FALSE);
	void					setFocusObject(int _iObjectIndex,BOOL _bIsFocus);
	BOOL					isDoor(int _iIndex)
	{
		cFIXED_OBJECT_INFO	*lpObject	=	&m_aFixedObjectList[_iIndex];

		return	m_aFixedObjectImage[lpObject->m_wObject].isDoor();
	}

	void					putFixedObject(int _iIndex,int _iX,int _iY,BOOL _bIsWantPutOnlyRape=FALSE,BOOL _bIsPutShadow=TRUE,BOOL _bIsPutImage=TRUE);
	void					putTinyObject(int _iIndex,int _iDx,int _iDy);
	void					putBuilding(int _iIndex,int _iX,int _iY);
	int						getViewObjects(cINDEX *_lpIndexList,cRECT *_lpRectViewPort);	//	소트할 오브젝트들을 구한다.

	static	int				s_aObjectCheckInfo[100][3];
	static	BOOL			ConvertFileName();
	static	BOOL			ExportCheckObjectInfo();
	static	BOOL			LoadCheckObjectInfo();

};

extern	int		g_iEventObjectGrade;


#endif