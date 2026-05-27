#ifndef _classAREA_H
#define _classAREA_H

#include "cNEMO.H"
#include "doorDefine.H"
#include "areaDefine.H"
#include "cHitEffect.H"

#define	dMAX_AREA				(256*2)

#define	dT_AREA_ARCA			100
#define	dT_AREA_GUILD_STRUCTURE	101

#define	dPORTAL_COUNT			18

#define	dSIZEOF_AREA_NAME		32

#pragma pack(2)

class	cAREA_OLD_SAVE_DATA
{
public:
	WORD			m_wSerial;
	int				x1,y1,x2,y2;
	WORD			m_wKind;			//	영역 분류
	WORD			m_wValue;			//	open?close? / direct? / string size /
	BYTE			m_bGate;
	BYTE			m_gateDirect: 3;
	BYTE			m_gateShape	: 5;
	WORD			m_wMoveGate;//	입구,출구
	char			m_strName[dSIZEOF_AREA_NAME];
	WORD			m_bf15LinkObject : 15;	//	이 영역과 링크된 오브젝트
	WORD			m_bf1IsTestPortal: 1;	//	테스트중인 영역이다.
	char			m_strGateName[dSIZEOF_AREA_NAME];
};

class	cAREA_SAVE_DATA
{
public:
	WORD			m_wSerial;
	int				x1,y1,x2,y2;
	WORD			m_wKind;			//	영역 분류
	WORD			m_wValue;			//	open?close? / direct? / string size /
	BYTE			m_bGate;
	BYTE			m_gateDirect: 3;
	BYTE			m_gateShape	: 5;
	WORD			m_wMoveGate;//	입구,출구
	char			m_strName[dSIZEOF_AREA_NAME];
	WORD			m_bf15LinkObject : 15;	//	이 영역과 링크된 오브젝트
	WORD			m_bf1IsTestPortal: 1;	//	테스트중인 영역이다.
	char			m_strGateName[100];
	WORD			m_wSeasonVariable;
};

class	cAreaArcaInfo	:	public cAreaArca
{
public:
};

class	cAreaTrapInfo	:	public cAreaTrap
{
public:
	WORD			m_wIsActive;
};

class	cAreaDoorInfo	:	public cAreaDoor
{
public:
	WORD			m_bf1IsActive		:	1;		//	이 문은 현재 활성화 되어 있다.
	WORD			m_bf1IsOpened		:	1;
	WORD			m_bf1IsDisarmed		:	1;
	WORD			m_bf1IsUnlocked		:	1;
	WORD			m_bf1IsHide			:	1;		//	이 문은 현재 숨어 있다.
	WORD			m_bf1IsDetectDoor	:	1;		//	이 문은 현재 숨어 있다.
	WORD			m_bf1IsDetectTrap	:	1;		//	이 문은 현재 숨어 있다.
	WORD			m_bf1IsActiveByKarma:	1;		//	이 문은 현재 숨어 있다.
	WORD			m_bf1IsOpenedByHero	:	1;		//	주인공이 연문이다.

	WORD			m_wRemainDurability;
	WORD			m_awLinkObject[dLINK_WITH_AREA_DOOR_COUNT];
	cHitInfoManager	m_hitEffect;

	void			update();
	void			updateForSecretDungeonGate(BOOL _bIsVisible);
	void			hit(cAbility *_lpAbility,CHitInfo *_lpHitInfo);
};

class	cAreaEvent
{
public:
	WORD			m_wSerial;
	int				x1,y1,x2,y2;
	WORD			m_wKind;			//	영역 분류
	WORD			m_wValue;			//	open?close? / direct? / string size /
	BYTE			m_bGate;
	BYTE			m_gateDirect: 3;
	BYTE			m_gateShape	: 5;
	WORD			m_wMoveGate;//	입구,출구
	char			m_strName[dSIZEOF_AREA_NAME];
	WORD			m_bf15LinkObject : 15;	//	이 영역과 링크된 오브젝트
	WORD			m_bf1IsTestPortal: 1;	//	테스트중인 영역이다.
	
	WORD			m_wLinkObject;
	WORD			m_bf1Npc		: 1;		//1		09.08.12
	WORD			m_bf1ChangeObject	: 1;	//2		10.02.25
	WORD			m_bf7StartYear		: 7;	//9		10.02.25
	WORD			m_bf4StartMonth		: 4;	//13	10.02.25
	WORD			:0;
	WORD			m_bf5StartDay		: 5;	//5		10.02.25
	WORD			m_bf5StartHour		: 5;	//10	10.02.25
	WORD			m_bf6StartMinute	: 6;	//16	10.02.25

	WORD			m_bf7EndYear		: 7;	//7		10.02.25
	WORD			m_bf4EndMonth		: 4;	//11	10.02.25
	WORD			m_bf5EndDay			: 5;	//16	10.02.25

	WORD			m_bf5EndHour		: 5;	//5		10.02.25
	WORD			m_bf6EndMinute		: 6;	//11	10.02.25
	WORD			:0;							//09.08.12

	int				m_iChangeObjectIndex;		//10.02.25

};
class cAreaInfo
{
public:
	WORD			m_wSerial;
	int				x1,y1,x2,y2;
	WORD			m_wKind;			//	영역 분류
	WORD			m_wValue;			//	open?close? / direct? / string size /
	BYTE			m_bGate;
	BYTE			m_gateDirect: 3;
	BYTE			m_gateShape	: 5;
	WORD			m_wMoveGate;//	입구,출구
	char			m_strName[dSIZEOF_AREA_NAME];
	WORD			m_bf15LinkObject : 15;	//	이 영역과 링크된 오브젝트
	WORD			m_bf1IsTestPortal: 1;	//	테스트중인 영역이다.
	char			m_aBuffer[100];
	char			*m_pstrString;

					cAreaInfo()
					{	
						m_wSerial	=	0xffff;
						m_pstrString=	NULL;
					}
					~cAreaInfo()
					{	
						pKILL(m_pstrString);
					}

	void			reset()
	{
		m_wSerial	=	0xffff;
		pKILL(m_pstrString);
	}

	BOOL			isIn(int x,int y,BOOL _bIsRegardTrap=FALSE);

	inline	void	getCenter(CPos* _lpPos)	{_lpPos->x=(x2-x1)/2+x1,_lpPos->y=(y2-y1)/2+y1;}
	char*			getString();
	inline	BOOL	isDoor()
	{
		if (m_wKind	==	dAREA_DOOR)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isArca()
	{
		if (m_wKind	==	dT_AREA_ARCA)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isEventArea()
	{
		if (m_wKind	==	dAREA_EVENT_AREA	)
			return	TRUE;
		return	FALSE;
	}	//	이벤트 영역이냐?
	inline	BOOL	isGuildCrest()
	{
		if (m_wKind	==	dAREA_GUILD_CREST)
			return	TRUE;
		return	FALSE;
	}	//	이벤트 영역이냐?

};

#pragma pack()

#define	dMAX_NAMED_AREA_COUNT		100

class cAREA
{
public:
	cAreaInfo			m_aArea[dMAX_AREA];
	WORD				m_wTouhableAreaCount,m_wVisibleAreaCount,m_aTouchableArea[dMAX_AREA],m_aVisibleArea[dMAX_AREA];
	WORD				m_wNormalAreaCount,m_aNormalArea[dMAX_AREA];
	WORD				m_wNamedAreaCount,m_aNamedArea[dMAX_NAMED_AREA_COUNT];
	WORD				m_wTrapAreaCount,m_awTrapArea[dMAX_AREA];
	WORD				m_wInstanceFieldGate;

	int					m_iCount,m_iFocusedArea,m_iMoveArea;
	DWORD				m_dwFrameCounter;

	void				insert(int _iIndex,cAreaInfo *_lpArea,BOOL _bIsGuildBattleLobby,char *_lpstrText=NULL);
	void				reset();
	int					addArea(cAreaInfo *_lpArea);
	BOOL				removeArea(int _iSerial);
	int					getFocusArea(int _iX,int _iY,BOOL _bIsFindFocusArea,BOOL _bIsIgnoreSpecialArea=FALSE);
	void				update();
	void				addVisibleArea(int _iIndex);
	inline	void		addNormalArea(int _iIndex)		{m_aNormalArea[m_wNormalAreaCount++]		=	_iIndex;}
	inline	void		addTrapArea(int _iIndex)		{m_awTrapArea[m_wTrapAreaCount++]		=	_iIndex;}
	void				addTouchableArea(int _iIndex,BOOL _bIsVisible=FALSE);
	void				removeTouchableArea(int _iIndex);
	inline	void		addNamedArea(int _iIndex)		{m_aNamedArea[m_wNamedAreaCount++]			=	_iIndex;}

	void				drawGateWay(int _iX,int _iY,int _iFrameCounter,BOOL _bIsAddPaint=FALSE);
	inline	void		releaseMoveArea()	{m_iFocusedArea	=	0xffff,m_iMoveArea		=	0xffff;}

	int					getAreaKind(int _iIndex);
	BOOL				isGate(int _iIndex);
	BOOL				isTouchableArea(int _iIndex);
	inline	cAreaTrapInfo*	getTrapArea(int _iIndex)
	{
		return	(cAreaTrapInfo*)&m_aArea[m_awTrapArea[_iIndex]];
	}
	inline	cAreaDoorInfo*	getDoorArea(int _iSerial)
	{
		return	(cAreaDoorInfo*)getArea(_iSerial);
	}
	void				activeTrap(int _iIndex);
	void				disactiveTrap(int _iIndex);

	inline	cAreaInfo*	getFocusedArea()
	{
		if (m_iFocusedArea	<	0	&&	m_iFocusedArea>=dMAX_AREA)
			return	NULL;

		if (m_iFocusedArea == 0xffff)
		{
			return NULL;
		}

		try
		{
			if (m_aArea[m_iFocusedArea].m_wSerial == 0xffff)
				return NULL;
		}
		catch (std::exception& e)
		{
			return NULL;
		}


		return	&m_aArea[m_iFocusedArea];
	}
	inline	cAreaInfo*	getTouchableArea(int _iIndex)//	이름붙은 영역 얻기
	{
		return	&m_aArea[m_aTouchableArea[_iIndex]];
	}

	inline	cAreaInfo*	getNameArea(int _iIndex)//	이름붙은 영역 얻기
	{
		return	&m_aArea[m_aNamedArea[_iIndex]];
	}
	inline	cAreaInfo*	getArea(int _iIndex)//	이름붙은 영역 얻기
	{
		return	&m_aArea[_iIndex];
	}

	inline	BOOL		isInFocusArea(int _iX,int _iY)
	{
		if (m_iFocusedArea	==	0xffff)	return	FALSE;

		return	m_aArea[m_iFocusedArea].isIn(_iX,_iY);
	}

	inline	BOOL		getFocusAreaCenterPos(CPos *_lpPos)
	{
		if (m_iFocusedArea	==	0xffff)	return	FALSE;

		m_aArea[m_iFocusedArea].getCenter(_lpPos);

		return	TRUE;
	}

	void				buildInfo();

};

#endif