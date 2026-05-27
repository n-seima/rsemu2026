#ifndef _classAREA_H
#define _classAREA_H

#include "SFC.H"
#include "cIMAGE.H"
#include "CDialog.H"
#include "areaDefine.H"

extern	cANM	g_anmGate;

#define	dMAX_AREA				256

#define	dAREA_RANGE				20
#define	dAREA_HERO_RANGE		21

#define	dPORTAL_COUNT			18

#define	dAREA_VALUE_COUNT		5

#pragma	pack(2)

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
	char			m_strMoveGateName[dSIZEOF_AREA_NAME];
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
	WORD			m_bf1Npc		: 1;		//09.08.12
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
	char			m_strMoveGateName[100];
	WORD			m_wSeasonVariable;						//JBC		시즌 변수 추가.			08-10-31

	BOOL			isSame(cAREA_SAVE_DATA* _lpAreaSaveData)
	{

		if(m_wSerial != _lpAreaSaveData->m_wSerial)
			return FALSE;

		if(x1 != _lpAreaSaveData->x1)
			return FALSE;

		if(y1 != _lpAreaSaveData->y1)
			return FALSE;

		if(x2 != _lpAreaSaveData->x2)
			return FALSE;

		if(y2 != _lpAreaSaveData->y2)
			return FALSE;

		if(m_wKind != _lpAreaSaveData->m_wKind)
			return FALSE;

		if(m_wValue != _lpAreaSaveData->m_wValue)
			return FALSE;

		if(m_bGate != _lpAreaSaveData->m_bGate)
			return FALSE;

		if(m_gateDirect != _lpAreaSaveData->m_gateDirect)
			return FALSE;

		if(m_gateShape != _lpAreaSaveData->m_gateShape)
			return FALSE;

		if(m_wMoveGate != _lpAreaSaveData->m_wMoveGate)
			return FALSE;

		if(strcmp(m_strName,_lpAreaSaveData->m_strName) != 0)
			return FALSE;

		if(m_bf15LinkObject != _lpAreaSaveData->m_bf15LinkObject)
			return FALSE;

		if(m_bf1IsTestPortal != _lpAreaSaveData->m_bf1IsTestPortal)
			return FALSE;

		if(strcmp(m_strMoveGateName,_lpAreaSaveData->m_strMoveGateName) != 0)
			return FALSE;
		
		if(m_wSeasonVariable != _lpAreaSaveData->m_wSeasonVariable)
			return FALSE;

		return TRUE;
	}

};

class cAREA_INFO	:	public cAREA_SAVE_DATA
{
public:
	CConversationContainer	m_cc;	//	대화 컨테이너
	char					*m_pString;

						cAREA_INFO()
						{
							memset(this,0,sizeof(cAREA_INFO)-4);

							m_wSerial		=	0xffff;
							m_wKind			=	dAREA_NORMAL;
							m_pString		=	NULL;
							m_wValue		=	0;
							m_bGate			=	0;
							m_gateShape		=	0;
							m_gateDirect	=	0;
							m_wMoveGate		=	0xffff;

							m_bf15LinkObject=	0x7000;	//	이 영역과 링크된 오브젝트
							m_bf1IsTestPortal=	FALSE;	//	테스트중인 영역이다.

							m_cc.reset();
						}

						~cAREA_INFO()
						{
							pKILL(m_pString);
						}

	void				copy(cAREA_INFO *_areaInfo);		//	복사~~ -o-
	void				reset();
	void				resetForSpecialArea();	


	void				draw(int dx,int dy,BOOL hb,BOOL select, int area);	// 09.11.30 수정
	void				drawScale(int dx,int dy,int _iTbSize,int _iScale);
	inline	BOOL		isIn(int x,int y)
	{
		if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return TRUE;

		return FALSE;
	}

	inline	DWORD	getSize()
					{	DWORD xs=abs(x2-x1)+1;
						DWORD ys=abs(y2-y1)+1;

						return xs*ys;
					}

	int				grap(int x,int y,int _iScale=100);
	void			move(int dx,int dy);

	void			setString(char *str)
					{	pKILL(m_pString);

						if (str == NULL)
						{	m_wValue	=	0;
							return;
						}

						m_wValue		=	strlen(str)+1;
						m_pString		=	new char [m_wValue];
						strcpy(m_pString,str);
					}

	void			emptyBuffer()	{	pKILL(m_pString);}

	inline	BOOL	isRangeArea()	{if (m_wKind	==	dAREA_RANGE			)	return	TRUE;return FALSE;}
	inline	BOOL	isHeroRange()	{if (m_wKind	==	dAREA_HERO_RANGE	)	return	TRUE;return FALSE;}
	inline	BOOL	isPortal()		{if (m_wKind	==	dAREA_PORTAL		)	return	TRUE;return	FALSE;}		//	포탈이냐?
	inline	BOOL	isMonsterDen()	{if (m_wKind	==	dAREA_MONSTER_ACTIVE)	return	TRUE;return	FALSE;}		//	포탈이냐?
	inline	BOOL	isStartArea()	{if (m_wKind	==	dAREA_START_AREA	)	return	TRUE;return	FALSE;}	//	시작 영역이냐?
	inline	BOOL	isGuildBattleArea(){if (m_wKind==	dAREA_GUILD_BATTLE	)	return	TRUE;return	FALSE;}	//	시작 영역이냐?
	inline	BOOL	isNamedArea()	{if (m_wKind	==	dAREA_NAMED_AREA	)	return	TRUE;return	FALSE;}	//	시작 영역이냐?
	inline	BOOL	isEventArea()	{if (m_wKind	==	dAREA_EVENT_AREA	)	return	TRUE;return	FALSE;}	//	시작 영역이냐?
	inline	BOOL	isGuildCrest()	{if (m_wKind	==	dAREA_GUILD_CREST	)	return	TRUE;return	FALSE;}	//	시작 영역이냐?
	inline	BOOL	isDoor()		{if (m_wKind	==	dAREA_DOOR			)	return	TRUE;return	FALSE;}	//	시작 영역이냐?
	inline	int		getPortalType()	{return	m_wValue;}

	inline	BOOL	isSpecialArea()
	{
		if (isPortal())				return	TRUE;
		if (isNamedArea())			return	TRUE;
		if (isStartArea())			return	TRUE;
		if (isGuildBattleArea())	return	TRUE;
		if (isEventArea())			return	TRUE;
		if (isDoor())				return	TRUE;

		return	FALSE;
	}
	BOOL	isSame(cAREA_INFO* _lpAreaInfo)
	{
		if(!((cAREA_SAVE_DATA* )this)->isSame((cAREA_SAVE_DATA* )_lpAreaInfo))
			return FALSE;
	
		if(!m_cc.isSame(&_lpAreaInfo->m_cc))
			return FALSE;

		return TRUE;
	}
};
#pragma	pack()

class cAREA
{
public:
	cAREA_INFO		m_aArea[dMAX_AREA];
	int				m_iCount;
	int				m_iNewSerial;
	BOOL			m_isResizeArea,m_isMoveArea;
	int				m_iSelectArea;
	int				m_iSerchAreaIndex;
	int				m_aPortal[dPORTAL_COUNT];
//	WORD			m_awAreaValue[dAREA_VALUE_COUNT];


					cAREA();
					~cAREA();

	void			Reset();
	int				Add(char *str,cRECT *rect);
	int				Add(char *str,int kind,int data,int range);
	BOOL			Add(cAREA_INFO *lpArea,CConversationContainer *_lpKC);

	BOOL			Del(WORD index = 0xffff);
	int				GetFocusArea(int x,int y,int _iLastIndex=2);	//	x,y의 영역
	BOOL			Select(int x,int y);
	BOOL			Select(int x,int y,int _iScale);
	void			Select(int area=0xffff);
	void			DrawAll(int dx,int dy,BOOL hb,BOOL select,BOOL _bIsDrawOnlyDoor);
	void			DrawAllByScale(int dx,int dy,int _iTbSize,int _iScale,BOOL _bIsDrawOnlyDoor);
	BOOL			Update(int x,int y);
	BOOL			Update(int x,int y,int _iScale,int _iTBSize);
	int				GetFirstArea();
	int				GetArea(char *an);
	void			SetNewArea();

	BOOL			isExistDoorAreaPos(int _iX,int _iY);

	BOOL			isPortal(int index)			{	if (m_aArea[index].m_wKind	==	dAREA_PORTAL		)return	TRUE;return FALSE;}
	BOOL			IsNormal(int index)			{	if (m_aArea[index].m_wKind	==	dAREA_NORMAL		)return	TRUE;return FALSE;}
	BOOL			IsNoticeBoard(int index)	{	if (m_aArea[index].m_wKind	==	dAREA_NOTICE_BOARD	)return	TRUE;return FALSE;}
	BOOL			IsDoor(int index)			{	if (m_aArea[index].m_wKind	==	dAREA_DOOR			)return	TRUE;return FALSE;}
	BOOL			IsOpen(int index)			{	return m_aArea[index].m_wValue;}

	BOOL			Open(int index)				{	m_aArea[index].m_wValue	=	TRUE;}
	BOOL			Close(int index)			{	m_aArea[index].m_wValue	=	FALSE;}
	int				getStartArea();

	BOOL			linkGate();		//	게이트들 연결
	BOOL			checkTwoWayLinkGate(char *_lpstrMapname);

	inline	BOOL	IsAvail(int index)				{	if (m_aArea[index].m_wSerial	==	0xffff				)	return	FALSE;return	TRUE;}
	inline	char	*GetString(int index)			{	return	m_aArea[index].m_pString;}
	inline	char	*GetName(int index)				{	return	m_aArea[index].m_strName;}
	inline	int		GetValue(int index)				{	return	m_aArea[index].m_wValue;}
	inline	void	SetValue(int index,int value)	{	m_aArea[index].m_wValue	=	value;}
	inline	void	setFirstArea()					{	m_iSerchAreaIndex	=	0;}
	inline	cAREA_INFO*	getNextArea()
	{
		if (m_iSerchAreaIndex	>=	dMAX_AREA)	return	NULL;

		while(	m_iSerchAreaIndex	<	dMAX_AREA	&&	m_aArea[m_iSerchAreaIndex].m_wSerial	==	0xffff)
			m_iSerchAreaIndex++;

		if (m_iSerchAreaIndex	>=	dMAX_AREA)	return	NULL;

		return	&m_aArea[m_iSerchAreaIndex++];
	}
};

extern	cAREA_INFO	*g_lpSelectArea;
extern	char		*g_strAreaKind[];
extern	char		*g_strPortalDirect[];
extern	char		*g_strPortalShape[];
extern	char		*g_strSeasonKind[];
extern	char		*g_strSwapSlotKind[];


#endif