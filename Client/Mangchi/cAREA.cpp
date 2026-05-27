#include "cDRAW.H"
#include "cTEXT.H"
#include "cAREA.H"
#include "cMAIN.H"
#include "cMAP.H"
#include "cTOOLWND.H"
#include "cMESSAGE.H"
#include "cMAPWND.H"
#include "cPROJECT.H"
#include "arca.H"
#include "cSET.H"

cAREA_INFO	*g_lpSelectArea	=	NULL;
cANM		g_anmGate;

//
//	초기화
void
cAREA_INFO::reset()
{
	memset(this,0,sizeof(this)-4);
	m_wSerial		=	0xffff;
	m_wKind			=	dAREA_NORMAL;
	m_wValue		=	0;

	pKILL(m_pString);
	m_cc.reset();

	if (m_wKind	==	dAREA_TRAP)
	{
		cAreaTrap	*lpTrap	=	(cAreaTrap	*)this;

		lpTrap->m_wTrapLevel		=	10;
		lpTrap->m_wTrapRefreshTime	=	0;
		lpTrap->m_wDetectTrapLevel	=	0;
		lpTrap->m_wDisarmTrapLevel	=	0;
		lpTrap->m_wTrapSkill1		=	0xffff;
		lpTrap->m_wTrapSkill2		=	0xffff;
		lpTrap->m_wTrap1ActiveChance=	0;
		lpTrap->m_wTrap2ActiveChance=	0;
		lpTrap->m_wTrapRange		=	100;
		lpTrap->m_wTrapRangeMethod	=	0;
		lpTrap->m_wTrapTarget		=	0;
	}
}

void
cAREA_INFO::resetForSpecialArea()
{
	if (m_wKind	==	dAREA_TRAP)
	{
		cAreaTrap	*lpTrap			=	(cAreaTrap	*)this;

		lpTrap->m_wTrapLevel		=	10;
		lpTrap->m_wTrapRefreshTime	=	0;
		lpTrap->m_wDetectTrapLevel	=	0;
		lpTrap->m_wDisarmTrapLevel	=	0;
		lpTrap->m_wTrapSkill1		=	0xffff;
		lpTrap->m_wTrapSkill2		=	0xffff;
		lpTrap->m_wTrap1ActiveChance=	0;
		lpTrap->m_wTrap2ActiveChance=	0;
		lpTrap->m_wTrapRange		=	100;
		lpTrap->m_wTrapRangeMethod	=	0;
		lpTrap->m_wTrapTarget		=	0;
	}
	if (m_wKind	==	dAREA_DOOR)
	{
		cAreaDoor	*lpDoor				=	(cAreaDoor	*)this;

		lpDoor->m_wRegenPeriod			=	30;		//	생성 빈도 (분)	40	40	해당 시각 마다 비밀 문이 생성될지 결정한다.	
		lpDoor->m_wUpkeepPeriod			=	30;	//	지속 시간 (분)	30	30	시간이 지나면 사라진다.	이미 파티에 종속된 비밀 문이 있을 경우 비밀 문의 존재 여부와는 상관없이 해당 파티에게는 탐지 했던 위치에 표시되고 다른 사람들은 볼 수 없다.
		lpDoor->m_wGenChance			=	100;		//	생성 확률 설정 (%)	1	100	비밀 문이 각각 생성될 확률	

		lpDoor->m_iDurability			=	10000;	//	보유 HP
		lpDoor->m_iDefense				=	1000;
		lpDoor->m_wDestroyMinimumLevel	=	1;		//	파괴 가능 최소 레벨

		lpDoor->m_bSecretLevel			=	0;		//	비밀 레벨
		lpDoor->m_bDetectTrapLevel		=	0;
		lpDoor->m_bDisarmTrapLevel		=	0;	//	함정 탐지 레벨,함정 해체 레벨
		lpDoor->m_bUnlockLevel			=	0;	//	자물쇠 해체 레벨
		lpDoor->m_wUpkeepDismantlingTime=	30;	//	해체 상태 지속 시간
		lpDoor->m_bf1IsDestroyable		=	TRUE;	//	파괴 가능 여부

		lpDoor->m_wTrapLevel			=	0;
		lpDoor->m_wTrapSkill1			=	0xffff;
		lpDoor->m_wTrapSkill1ActiveChance=	0;
		lpDoor->m_wTrapSkill2			=	0xffff;
		lpDoor->m_wTrapSkill2ActiveChance=	0;

		lpDoor->m_bDefensePercent			= 0;		// 물리/마법 저항력(%) 09.08.13  
	}
}

//
//	복사~~ -o-
void
cAREA_INFO::copy(cAREA_INFO *_areaInfo)
{
	x1				=	_areaInfo->x1;
	y1				=	_areaInfo->y1;
	x2				=	_areaInfo->x2;
	y2				=	_areaInfo->y2;
	m_wKind			=	_areaInfo->m_wKind;
	m_wValue		=	_areaInfo->m_wValue;
	m_wMoveGate		=	_areaInfo->m_wMoveGate;
	m_bGate			=	_areaInfo->m_bGate;
	m_gateDirect	=	_areaInfo->m_gateDirect;
	m_gateShape		=	_areaInfo->m_gateShape;
	m_bf15LinkObject=	_areaInfo->m_bf15LinkObject;	//	이 영역과 링크된 오브젝트
	m_bf1IsTestPortal=	_areaInfo->m_bf1IsTestPortal;	//	테스트중인 영역이다.

	strcpy(m_strMoveGateName,_areaInfo->m_strMoveGateName);

	m_cc.copy(&_areaInfo->m_cc);

	setString(_areaInfo->m_pString);
}

//
//	영역 그려주기~~
void
cAREA_INFO::draw(int dx,int dy,BOOL hb,BOOL select,int area)
{
	cRECT x1y1,x2y1,x1y2,x2y2;

	cRECT	rect;
	rect.Set(x1,y1,x2,y2);

	rect.Move(dx,dy);

	if	(hb || (m_wSerial == g_lpActmap->m_pArea->m_iSelectArea && select))
	{
		if (m_wKind	==	dAREA_TRAP)
			cDRAW::FillHB(_RED,rect.x1,rect.y1,rect.x2,rect.y2);
		else
			cDRAW::FillHB(0,rect.x1,rect.y1,rect.x2,rect.y2);
	}

	WORD	wOutlineColor,wHalfColor;

	if	(m_wSerial	==	g_lpActmap->m_pArea->m_iSelectArea && select)
	{	
		if	(m_wKind	==	dAREA_TRAP)
			wHalfColor	=	_LTRED;
		else
			wHalfColor	=	_LTGREEN;

		x1y1.Set(rect.x1-3,rect.y1-3,rect.x1+3,rect.y1+3);
		x2y1.Set(rect.x2-3,rect.y1-3,rect.x2+3,rect.y1+3);
		x1y2.Set(rect.x1-3,rect.y2-3,rect.x1+3,rect.y2+3);
		x2y2.Set(rect.x2-3,rect.y2-3,rect.x2+3,rect.y2+3);

		cDRAW::Box(wHalfColor,rect.x1,rect.y1,rect.x2,rect.y2);
		cDRAW::Fill(wHalfColor,x1y1.x1,x1y1.y1,x1y1.x2,x1y1.y2);
		cDRAW::Fill(wHalfColor,x2y1.x1,x2y1.y1,x2y1.x2,x2y1.y2);
		cDRAW::Fill(wHalfColor,x1y2.x1,x1y2.y1,x1y2.x2,x1y2.y2);
		cDRAW::Fill(wHalfColor,x2y2.x1,x2y2.y1,x2y2.x2,x2y2.y2);
	}
	else
	{
		if	(m_wKind	==	dAREA_TRAP)
			wOutlineColor	=	_LTRED;
		else
			wOutlineColor	=	_WHITE;

		cDRAW::Box(wOutlineColor,rect.x1,rect.y1,rect.x2,rect.y2);
	}

	char	*strOpenClose[]	=	{"close","open"};

//	cTEXT::SetBoldFont();

	if	(!hb)
	{
		int	iWidth	=	strlen(m_strName)*8;

		if (rect.x1 >= -iWidth && rect.x1 <= g_MAPWND.WIDTH+iWidth &&	rect.y1 >= -20     && rect.y1 <= g_MAPWND.HEIGHT+20)
		{
		cTEXT::Put(rect.x1+6,rect.y1+2,0,m_strName);
		cTEXT::Put(rect.x1+5,rect.y1+2,0,m_strName);
		cTEXT::Put(rect.x1+4,rect.y1+2,0,m_strName);
		cTEXT::Put(rect.x1+6,rect.y1+3,0,m_strName);

		cTEXT::Put(rect.x1+4,rect.y1+3,0,m_strName);
		cTEXT::Put(rect.x1+6,rect.y1+4,0,m_strName);
		cTEXT::Put(rect.x1+5,rect.y1+4,0,m_strName);
		cTEXT::Put(rect.x1+4,rect.y1+4,0,m_strName);

			// 09.12.18
		cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
			//cTEXT::Put(rect.x1+5,rect.y1+3 ,RGB(255,255,255),m_strName);
		}
	}
	else
	{
		rect.y1	+=	2;

		switch(m_wKind)
		{
			case	dAREA_NOTICE_BOARD	:
				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
			cTEXT::Put(rect.x1+5,rect.y1+16,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NOTICE_BOARD],g_strAreaKind[m_wKind]);
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,RGB(0,255,0),m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+16,LTYELLOW,g_strAreaKind[m_wKind]);
				break;

			case	dAREA_DOOR			:
			{
				if	(m_wValue	>	1)
					m_wValue	=	0;

				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
			cTEXT::Put(rect.x1+5,rect.y1+16,cSET_FONT::s_OldColorOption[eCOLOR_AREA_DOOR],_ms("%s",strOpenClose[m_wValue]));
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,RGB(255,255,255),m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+16,LTYELLOW,_ms("%s",strOpenClose[m_wValue]));
				break;
			}

			case	dAREA_PORTAL		:
			{
				if	(m_gateShape == ePAS_INNER_PORTAL || m_gateShape == ePAS_INNER_DUNGEON || m_gateShape == ePAS_INNER_TOWER || m_gateShape == ePAS_INNER_BUILDING)
					g_anmGate.PutReg(rect.centerX(),rect.centerY(),4,m_gateDirect,0);

				if	(m_gateShape >= ePAS_BUILDING_GATE && m_gateShape < ePAS_ONE_WAY_GATE)
					g_anmGate.PutReg(rect.centerX(),rect.centerY(),1,m_gateDirect,0);

				if	(m_gateShape == ePAS_NORMAL	||	m_gateShape == ePAS_ENTER_GUILD_HALL	||	m_gateShape == ePAS_LEAVE_GUILD_HALL)
					g_anmGate.PutReg(rect.centerX(),rect.centerY(),3,m_gateDirect,0);

				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,LTGREEN,m_strName);

				if	(m_pString)
				{
					int	iSize	=	cTEXT::GetSize("출구 ");

					rect.y1		+=	16;
					
					// 09.12.18
				cTEXT::Put(rect.x1+5,rect.y1,cSET_FONT::s_OldColorOption[eCOLOR_PORTAL_EXIT] ,"Exit ");
				cTEXT::Put(rect.x1+5+iSize,rect.y1,cSET_FONT::s_OldColorOption[eCOLOR_PORTAL_LOAD_MAP],m_pString);
					//cTEXT::Put(rect.x1+5,rect.y1,WHITE,"출구 ");
					//cTEXT::Put(rect.x1+5+iSize,rect.y1,YELLOW,m_pString);
					
					if (m_strMoveGateName[0])
					{
						rect.y1	+=	14;
						// 09.12.18
					cTEXT::Put(rect.x1+5+iSize,rect.y1,cSET_FONT::s_OldColorOption[eCOLOR_PORTAL_CMAP],_ms("%s:%d",m_strMoveGateName,m_wSerial));
						//cTEXT::Put(rect.x1+5+iSize,rect.y1,LTYELLOW,_ms("%s:%d",m_strMoveGateName,m_wSerial));
						
					}
				}

				if (m_gateShape	==	ePAS_HIDDEN)
				{
					rect.y1	+=	16;
					// 09.12.18
				cTEXT::Put(rect.x1+5,rect.y1,cSET_FONT::s_OldColorOption[eCOLOR_PORTAL_HIDDEN],"Hidden Portal");
					//cTEXT::Put(rect.x1+5,rect.y1,LTYELLOW,"감춰진 포탈");
				}
				if (m_gateShape	==	ePAS_HIDDEN_INNER_PORTAL)
				{
					rect.y1	+=	16;
					// 09.12.18
				cTEXT::Put(rect.x1+5,rect.y1,cSET_FONT::s_OldColorOption[eCOLOR_PORTAL_HIDDEN_IN],"Hidden internal movement portal");
					//cTEXT::Put(rect.x1+5,rect.y1,LTYELLOW,"감춰진 내부 이동 포탈");
				}
				if	(m_gateShape	==	ePAS_ONE_WAY_GATE)
				{
					rect.y1	+=	16;
					// 09.12.18
				cTEXT::Put(rect.x1+5,rect.y1,cSET_FONT::s_OldColorOption[eCOLOR_PORTAL_ONE_WAY],"One-way portal");
					//cTEXT::Put(rect.x1+5,rect.y1,LTYELLOW,"일방 통행 포탈");
				}
				break;
			}

			case	dAREA_NORMAL		:
				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NORMAL],m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,WHITE,m_strName);
				break;

			case	dAREA_TRAP			:
				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
			cTEXT::Put(rect.x1+5,rect.y1+15 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_TRAP],"Trap");
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,WHITE,m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+15 ,LTYELLOW,"함정");
				break;

			case	dAREA_EVENT_AREA	:
			{
				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
			cTEXT::Put(rect.x1+5,rect.y1+15 ,cSET_FONT::s_OldColorOption[eCOLOR_EVENT_AREA],"Event Area");
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,WHITE,m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+15 ,LTYELLOW,"이벤트 영역");
				break;
			}

			case	dAREA_ARCA			:
			{
				cAreaArca	*lpAreaArca	=	(cAreaArca	*)this;
				cArcaInfo	*lpArca		=	g_arcaManager.get(lpAreaArca->m_wLinkArca);

				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,WHITE,m_strName);

				if (lpArca == NULL)
				cTEXT::Put(rect.x1+5,rect.y1+15 ,cSET_FONT::s_OldColorOption[eCOLOR_ARCA],"Treasure chest: Link problem");
					//cTEXT::Put(rect.x1+5,rect.y1+15 ,LTYELLOW,"보물 상자 : 링크 문제 있음");
				else
				cTEXT::Put(rect.x1+5,rect.y1+15 ,cSET_FONT::s_OldColorOption[eCOLOR_ARCA],_ms("Treasure chest: %s",lpArca->m_strName));
					//cTEXT::Put(rect.x1+5,rect.y1+15 ,LTYELLOW,_ms("보물 상자 : %s",lpArca->m_strName));
				break;
			}

			case	dAREA_NAMED_AREA	:
				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_NAME_AREA],m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,LTCYAN,m_strName);
				break;

			case	dAREA_START_AREA	:
			case	dAREA_GUILD_BATTLE	:
			case	dAREA_OX_AREA_O		:
			case	dAREA_OX_AREA_X		:
			case	dAREA_GUILD_CREST	:
				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3,cSET_FONT::s_OldColorOption[eCOLOR_AREA_ETC],g_strAreaKind[m_wKind]);
				//cTEXT::Put(rect.x1+5,rect.y1+3,LTYELLOW,g_strAreaKind[m_wKind]);
				break;

			case	dAREA_MONSTER_ACTIVE:
			{
				// 09.12.18
			cTEXT::Put(rect.x1+5,rect.y1+3 ,cSET_FONT::s_OldColorOption[eCOLOR_AREA_NAME],m_strName);
			cTEXT::Put(rect.x1+5,rect.y1+16,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_ACTIVE],dMSG_MONSTER_ACTIVE_AREA);
				//cTEXT::Put(rect.x1+5,rect.y1+3 ,WHITE,m_strName);
				//cTEXT::Put(rect.x1+5,rect.y1+16,LTYELLOW,dMSG_MONSTER_ACTIVE_AREA);
				
				cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
				lpActorSet->drawMonsterAreaInfo(area, &rect);
				break;
			}
		}
	}

//	cTEXT::SetNormalFont();
}	//	cAREA_INFO::draw(int dx,int dy,BOOL hb,BOOL select,int area)

//
//	영역 그려주기~~
void
cAREA_INFO::drawScale(int dx,int dy,int _iTbSize,int _iScale)
{
	cRECT x1y1,x2y1,x1y2,x2y2;

	cRECT	rect;
	rect.Set(x1,y1,x2,y2);
	rect.Move(-dx,-dy);

	rect.x1	=	rect.x1*_iScale/100;
	rect.y1	=	rect.y1*_iScale/100+_iTbSize;
	rect.x2	=	rect.x2*_iScale/100;
	rect.y2	=	rect.y2*_iScale/100+_iTbSize;

	if (m_wSerial == g_lpActmap->m_pArea->m_iSelectArea)
	{
		x1y1.Set(rect.x1-3,rect.y1-3,rect.x1+3,rect.y1+3);
		x2y1.Set(rect.x2-3,rect.y1-3,rect.x2+3,rect.y1+3);
		x1y2.Set(rect.x1-3,rect.y2-3,rect.x1+3,rect.y2+3);
		x2y2.Set(rect.x2-3,rect.y2-3,rect.x2+3,rect.y2+3);

		cDRAW::Box(_LTGREEN,rect.x1,rect.y1,rect.x2,rect.y2);
		cDRAW::Fill(_LTGREEN,x1y1.x1,x1y1.y1,x1y1.x2,x1y1.y2);
		cDRAW::Fill(_LTGREEN,x2y1.x1,x2y1.y1,x2y1.x2,x2y1.y2);
		cDRAW::Fill(_LTGREEN,x1y2.x1,x1y2.y1,x1y2.x2,x1y2.y2);
		cDRAW::Fill(_LTGREEN,x2y2.x1,x2y2.y1,x2y2.x2,x2y2.y2);
	}
	else cDRAW::Box(_WHITE,rect.x1,rect.y1,rect.x2,rect.y2);


//			cDRAW::Box(_LTGREEN,rect.x1,rect.y1,rect.x2,rect.y2);
//	else	cDRAW::Box(_WHITE,rect.x1,rect.y1,rect.x2,rect.y2);

	dx	=	rect.x1-3;
	dy	=	rect.y1-3;

cTEXT::Put(dx+6,dy+2,0,m_strName);
cTEXT::Put(dx+5,dy+2,0,m_strName);
cTEXT::Put(dx+4,dy+2,0,m_strName);
cTEXT::Put(dx+6,dy+3,0,m_strName);

//	cTEXT::Put(dx+5,dy+3,0,m_strName);
cTEXT::Put(dx+4,dy+3,0,m_strName);
cTEXT::Put(dx+6,dy+4,0,m_strName);
cTEXT::Put(dx+5,dy+4,0,m_strName);
cTEXT::Put(dx+4,dy+4,0,m_strName);

	switch(m_wKind)
	{
		case	dAREA_NOTICE_BOARD	:
		cTEXT::Put(dx+5,dy+3 ,RGB(0,255,0),m_strName);
			break;

		case	dAREA_DOOR			:
		case	dAREA_GUILD_CREST	:
		cTEXT::Put(dx+5,dy+3 ,RGB(255,255,255),m_strName);
			break;

		case	dAREA_PORTAL		:
		{
		cTEXT::Put(dx+5,dy+3 ,RGB(255,255,0),m_strName);
			break;
		}

		case	dAREA_NORMAL		:
		cTEXT::Put(dx+5,dy+3 ,RGB(255,255,255),m_strName);
			break;

		case	dAREA_NAMED_AREA	:
		cTEXT::Put(dx+5,dy+3 ,LTCYAN,m_strName);
			break;

		case	dAREA_START_AREA	:
		cTEXT::Put(dx+5,dy+3 ,RGB(255,255,0),m_strName);
			break;

		case	dAREA_MONSTER_ACTIVE:
		cTEXT::Put(dx+5,dy+3 ,RGB(255,128,255),m_strName);
			break;
	}


//cTEXT::Put(,RGB(255,255,255),m_strName);
}	//	cAREA_INFO::draw(int dx,int dy,BOOL hb,BOOL select)

int
cAREA_INFO::grap(int x,int y,int _iScale)
{	cRECT x1y1,x2y1,x1y2,x2y2;

	cRECT	rect;
	rect.Set(x1,y1,x2,y2);

	int	iGap	=	3*100/_iScale;

//	if (_iScale	==	100)
	{
		x1y1.Set(rect.x1-iGap,rect.y1-iGap,rect.x1+iGap,rect.y1+iGap);
		x2y1.Set(rect.x2-iGap,rect.y1-iGap,rect.x2+iGap,rect.y1+iGap);
		x1y2.Set(rect.x1-iGap,rect.y2-iGap,rect.x1+iGap,rect.y2+iGap);
		x2y2.Set(rect.x2-iGap,rect.y2-iGap,rect.x2+iGap,rect.y2+iGap);

		if (x1y1.isIN(x,y)) return 1;
		if (x2y1.isIN(x,y)) return 2;
		if (x1y2.isIN(x,y)) return 3;
		if (x2y2.isIN(x,y)) return 4;
	}

	if (rect.isIN(x,y)) return 5;

	return 0;
}

void
cAREA_INFO::move(int dx,int dy)
{	x1	+=	dx,y1	+=	dy,x2	+=	dx,y2	+=	dy;
}

cAREA::cAREA()
{
	for (int i=0;i<dMAX_AREA;i++)
	{	m_aArea[i].m_wSerial	=	0xffff;
		m_aArea[i].m_wValue	=	0;
		m_aArea[i].m_wKind		=	dAREA_NORMAL;
	}

	m_iCount		=	0;
	m_iNewSerial	=	0;
	m_isResizeArea	=	FALSE;
	m_isMoveArea	=	FALSE;

	cRECT	rect;
	rect.Set(0,0,0,0);

	m_iSelectArea	=	2;				//	선택

	Add(dMSG_WHOLE_AREA	,&rect);
	Add(dMSG_SCREEN_AREA,&rect);
}

cAREA::~cAREA()
{
}

void
cAREA::Reset()
{	for (int i=0;i<dMAX_AREA;i++)
	{	m_aArea[i].m_wSerial	=	0xffff;
		m_aArea[i].m_wValue	=	0;
		m_aArea[i].m_wKind		=	dAREA_NORMAL;
		m_aArea[i].m_wSeasonVariable = 0;
	}

	m_iCount		=	0;
	m_iNewSerial	=	0;
	m_isResizeArea	=	FALSE;
	m_isMoveArea	=	FALSE;

	cRECT	rect;
	rect.Set(0,0,0,0);

	m_iSelectArea	=	2;				//	선택

	g_lpSelectArea	=	&m_aArea[2];

	Add(dMSG_WHOLE_AREA	,&rect);
	Add(dMSG_SCREEN_AREA,&rect);
}

BOOL
cAREA::Add(char *str,cRECT *rect)
{
	if	(m_iCount >= dMAX_AREA)
		return	ERRMSG(_ms("Area Count Full (max %d)",dMAX_AREA));

	m_aArea[m_iNewSerial].m_wSerial	=	m_iNewSerial;

	memcpy(&m_aArea[m_iNewSerial].x1,rect,sizeof (cRECT));
	strncpy(m_aArea[m_iNewSerial].m_strName,str,dSIZEOF_AREA_NAME);

	m_iCount++;

	int	index	=	m_iNewSerial;

	for (WORD i=m_iNewSerial+1;i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial	==	0xffff)
		{
			m_iNewSerial	=	i;
			break;
		}

	return index;
}

int
cAREA::Add(char *str,int kind,int data,int range)
{
	if (m_iCount >= dMAX_AREA)
		return ERRMSG(_ms("Area Count Full (max %d)",dMAX_AREA));

	m_aArea[m_iNewSerial].m_wSerial	=	m_iNewSerial;
	strncpy(m_aArea[m_iNewSerial].m_strName,str,sizeof(m_aArea[m_iNewSerial].m_strName));
	m_aArea[m_iNewSerial].x1		=	data;
	m_aArea[m_iNewSerial].x2		=	range;
	m_aArea[m_iNewSerial].m_wKind	=	kind;

	m_iCount++;

	int	index	=	m_iNewSerial;

	for (WORD i=m_iNewSerial+1;i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial	==	0xffff)
		{
			m_iNewSerial	=	i;
			break;
		}

	return index;
}

BOOL
cAREA::Add(cAREA_INFO *lpArea,CConversationContainer *_lpCC)
{
	int	index	=	lpArea->m_wSerial;

	if (index >=	dMAX_AREA || index < 0)
		return FALSE;

	memcpy(&m_aArea[index],lpArea,sizeof(cAREA_INFO)- 4);

	if	(m_aArea[index].isEventArea())
	{
		cAreaEvent	*lpEventArea	=	(cAreaEvent	*)&m_aArea[index];

		if	(lpEventArea->m_wLinkObject	==	0)
			lpEventArea->m_wLinkObject	=	0xffff;
	}

	if (_lpCC)
		m_aArea[index].m_cc.copy(_lpCC);
	else
		m_aArea[index].m_cc.reset();

	if (lpArea->m_pString)
		m_aArea[index].setString(lpArea->m_pString);

	return	TRUE;
}

BOOL
cAREA::Del(WORD index)
{
	if (index == 0xffff)
		return Del(m_iSelectArea);

	if (index <= 0 || index >= dMAX_AREA	)
		return FALSE;
	if (m_aArea[index].m_wSerial == 0xffff	)
		return FALSE;

	m_aArea[index].m_wSerial	=	0xffff;
	strcpy(m_aArea[index].m_strName,"Removed Area");

	if (index < m_iNewSerial) m_iNewSerial	=	index;
	m_iCount	--;

	if (index	==	m_iSelectArea)	g_lpSelectArea	=	NULL;

	g_lpActmap->LostVirgin();

	return TRUE;
}

void
cAREA::DrawAll(int dx,int dy,BOOL hb,BOOL select,BOOL _bIsDrawOnlyDoor)
{
	for (int i=2,index = 2;i<dMAX_AREA && index < m_iCount;i++)
		if (m_aArea[i].m_wSerial !=0xffff)
		{	
			index++;

			if (i	==	m_iSelectArea)
				continue;

			if (_bIsDrawOnlyDoor && m_aArea[i].m_wKind != dAREA_DOOR)
				continue;

			m_aArea[i].draw(dx,dy,hb,select,i);
		}

	if (m_iSelectArea > 0 &&  m_iSelectArea < dMAX_AREA)
	{
		if (m_aArea[m_iSelectArea].m_wSerial	!=	0xffff)
		{
			if (_bIsDrawOnlyDoor && m_aArea[m_iSelectArea].m_wKind != dAREA_DOOR)
				return;

			m_aArea[m_iSelectArea].draw(dx,dy,hb,select,m_iSelectArea);
		}
	}
}

void
cAREA::DrawAllByScale(int dx,int dy,int _iTbSize,int _iScale,BOOL _bIsDrawOnlyDoor)
{
	for (int i=2,index = 2;i<dMAX_AREA && index < m_iCount;i++)
		if (m_aArea[i].m_wSerial !=0xffff)
		{	
			if (_bIsDrawOnlyDoor && m_aArea[i].m_wKind != dAREA_DOOR)
				continue;
			
			index++;
			m_aArea[i].drawScale(dx,dy,_iTbSize,_iScale);
		}
}

//
//	x,y의 영역을 찾아서 리턴
int
cAREA::GetFocusArea(int x,int y,int _iLastIndex)
{
	for (int i=_iLastIndex;i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial!=0xffff)
			if (m_aArea[i].isIn(x,y))
				return	i;

	return 0xffff;
}

BOOL
cAREA::Select(int x,int y)
{	int		zone[dMAX_AREA];
	int		count=0;

	g_lpSelectArea	=	&m_aArea[m_iSelectArea];

	for (int index=2,i=2;index<m_iCount && i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial!=0xffff)
		{
			index++;

			if (i	!=	m_iSelectArea)
				if (m_aArea[i].isIn(x,y))
					zone[count++]	=	i;
		}

	if (count == 2)
	{
		Select(zone[0]);
		return TRUE;
	}

	if (count > 0)
	{	DWORD	minSize	=0xffffffff,size;
		int		area	=1;

		for (i=0;i<count;i++)
		{	size	=	m_aArea[zone[i]].getSize();

			if (size < minSize )
			{	area	=	zone[i];
				minSize	=	size;
			}
		}

		Select(area);

		return TRUE;
	}

	return FALSE;
}

BOOL
cAREA::Select(int x,int y,int _iScale)
{	int		zone[dMAX_AREA];
	int		count=0;

	g_lpSelectArea	=	&m_aArea[m_iSelectArea];

	for (int index=2,i=2;index<m_iCount && i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial!=0xffff)
		{
			index++;

			if (i	!=	m_iSelectArea)
				if (m_aArea[i].isIn(x,y))
					zone[count++]	=	i;
		}

	if (count == 2)
	{
		Select(zone[0]);
		return TRUE;
	}

	if (count > 0)
	{	DWORD	minSize	=0xffffffff,size;
		int		area	=1;

		for (i=0;i<count;i++)
		{	size	=	m_aArea[zone[i]].getSize();

			if (size < minSize )
			{	area	=	zone[i];
				minSize	=	size;
			}
		}

		Select(area);

		return TRUE;
	}

	return FALSE;
}

void
cAREA::Select(int area)
{
	if(area != 0xffff)	m_iSelectArea		=	area;
	else				return;

	g_lpSelectArea	=	&m_aArea[area];

	if (KEY[dKEY_CONTROL] || LEFTDBLCLICK)
	{	
		int x = (g_lpSelectArea->x1 + (g_lpSelectArea->x2-g_lpSelectArea->x1)/2)/dTILE_REAL_WIDTH  
				- g_MAPWND.TPW/2;
		int y = (g_lpSelectArea->y1 + (g_lpSelectArea->y2-g_lpSelectArea->y1)/2)/dTILE_REAL_HEIGHT 
				- g_MAPWND.TPH/2;

		if (g_lpSelectArea->m_wKind	==	dAREA_RANGE)
		{
			cAREA_INFO	*lpArea	=	&g_lpActmap->m_pArea->m_aArea[g_lpSelectArea->x1];

			x	=	(lpArea->x1 + (lpArea->x2-lpArea->x1)/2)/dTILE_REAL_WIDTH  
					- g_MAPWND.TPW/2;
			y	=	(lpArea->y1 + (lpArea->y2-lpArea->y1)/2)/dTILE_REAL_HEIGHT 
					- g_MAPWND.TPH/2;
		}

		if (g_lpSelectArea->m_wKind	<	dAREA_HERO_RANGE)
			g_lpActmap->setPos(x,y);
	}

	TOOLWND.Draw();
}


BOOL
cAREA::Update(int x,int y)
{
	if (m_iSelectArea==0 || m_aArea[m_iSelectArea].m_wSerial == 0xffff)
		return FALSE;

	static	cRECT	oldArea;
	static	int		oldX,oldY;

	if (!LBUTTON || KEY[dKEY_SHIFT])
	{	
		if (m_isResizeArea	|| m_isMoveArea)
		{
			m_isResizeArea	=	0;
			m_isMoveArea	=	0;
			return TRUE;
		}

		return FALSE;
	}

	if (!m_isResizeArea && !m_isMoveArea)
	{	int iGrapedArea = m_aArea[m_iSelectArea].grap(x,y);

		if (iGrapedArea	== 0)
		{	
			Select(x,y);
			iGrapedArea	= m_aArea[m_iSelectArea].grap(x,y);
		}

		switch(iGrapedArea)
		{	case	1	:
			case	2	:
			case	3	:
			case	4	:
				m_isResizeArea	=	iGrapedArea;
				break;
			case	5	:
				m_isMoveArea	=	TRUE;
				oldX		=	x;
				oldY		=	y;
				memcpy(&oldArea,&m_aArea[m_iSelectArea].x1,sizeof(oldArea));
				break;
		}
	}

	switch(m_isResizeArea)
	{	case	1	:
			if (x+20 > m_aArea[m_iSelectArea].x2) x=m_aArea[m_iSelectArea].x2-20;
			if (y+20 > m_aArea[m_iSelectArea].y2) y=m_aArea[m_iSelectArea].y2-20;
			m_aArea[m_iSelectArea].x1=x,m_aArea[m_iSelectArea].y1=y;
			g_lpActmap->LostVirgin();
			return TRUE;

		case	2	:
			if (x-20 < m_aArea[m_iSelectArea].x1) x=m_aArea[m_iSelectArea].x1+20;
			if (y+20 > m_aArea[m_iSelectArea].y2) y=m_aArea[m_iSelectArea].y2-20;
			m_aArea[m_iSelectArea].x2=x,m_aArea[m_iSelectArea].y1=y;
			g_lpActmap->LostVirgin();
			return TRUE;

		case	3	:
			if (x+20 > m_aArea[m_iSelectArea].x2) x=m_aArea[m_iSelectArea].x2-20;
			if (y-20 < m_aArea[m_iSelectArea].y1) y=m_aArea[m_iSelectArea].y1+20;
			m_aArea[m_iSelectArea].x1=x,m_aArea[m_iSelectArea].y2=y;
			g_lpActmap->LostVirgin();
			return TRUE;

		case	4:
			if (x-20 < m_aArea[m_iSelectArea].x1) x=m_aArea[m_iSelectArea].x1+20;
			if (y-20 < m_aArea[m_iSelectArea].y1) y=m_aArea[m_iSelectArea].y1+20;
			m_aArea[m_iSelectArea].x2=x,m_aArea[m_iSelectArea].y2=y;
			g_lpActmap->LostVirgin();
			return TRUE;
	}

	if (m_isMoveArea)
	{
		int xs	=	x	-	oldX,ys	=	y-oldY;
		
		if (xs!=0 || ys!=0)
			g_lpActmap->LostVirgin();

		memcpy(&m_aArea[m_iSelectArea].x1,&oldArea,sizeof(oldArea));
		m_aArea[m_iSelectArea].move(xs,ys);

		return TRUE;
	}

	return FALSE;
}

BOOL
cAREA::Update(int x,int y,int _iScale,int _iTBSize)
{
	if (_iScale	==	100)
		return	Update(x,y);

	if (m_iSelectArea==0 || m_aArea[m_iSelectArea].m_wSerial == 0xffff)
		return FALSE;

	static	cRECT	oldArea;
	static	int		oldX,oldY;

	if (!LBUTTON || KEY[dKEY_SHIFT])
	{
		if (m_isResizeArea	|| m_isMoveArea)
		{
			m_isResizeArea	=	0;
			m_isMoveArea	=	0;

			return TRUE;
		}

		return FALSE;
	}

	if (!m_isResizeArea && !m_isMoveArea)
	{
		int iGrapedArea = m_aArea[m_iSelectArea].grap(x,y,g_iScale);

		if (iGrapedArea	== 0)
		{	
			Select(x,y);
			iGrapedArea	= m_aArea[m_iSelectArea].grap(x,y,g_iScale);
		}

		switch(iGrapedArea)
		{	case	1	:
			case	2	:
			case	3	:
			case	4	:
				m_isResizeArea	=	iGrapedArea;
				break;

			case	5	:
				m_isMoveArea	=	TRUE;
				oldX		=	x;
				oldY		=	y;
				memcpy(&oldArea,&m_aArea[m_iSelectArea].x1,sizeof(oldArea));
				break;
		}
	}

	switch(m_isResizeArea)
	{	case	1	:
			if (x+20 > m_aArea[m_iSelectArea].x2) x=m_aArea[m_iSelectArea].x2-20;
			if (y+20 > m_aArea[m_iSelectArea].y2) y=m_aArea[m_iSelectArea].y2-20;
			m_aArea[m_iSelectArea].x1=x,m_aArea[m_iSelectArea].y1=y;
			g_lpActmap->LostVirgin();
			return TRUE;

		case	2	:
			if (x-20 < m_aArea[m_iSelectArea].x1) x=m_aArea[m_iSelectArea].x1+20;
			if (y+20 > m_aArea[m_iSelectArea].y2) y=m_aArea[m_iSelectArea].y2-20;
			m_aArea[m_iSelectArea].x2=x,m_aArea[m_iSelectArea].y1=y;
			g_lpActmap->LostVirgin();
			return TRUE;

		case	3	:
			if (x+20 > m_aArea[m_iSelectArea].x2) x=m_aArea[m_iSelectArea].x2-20;
			if (y-20 < m_aArea[m_iSelectArea].y1) y=m_aArea[m_iSelectArea].y1+20;
			m_aArea[m_iSelectArea].x1=x,m_aArea[m_iSelectArea].y2=y;
			g_lpActmap->LostVirgin();
			return TRUE;

		case	4:
			if (x-20 < m_aArea[m_iSelectArea].x1) x=m_aArea[m_iSelectArea].x1+20;
			if (y-20 < m_aArea[m_iSelectArea].y1) y=m_aArea[m_iSelectArea].y1+20;
			m_aArea[m_iSelectArea].x2=x,m_aArea[m_iSelectArea].y2=y;
			g_lpActmap->LostVirgin();
			return TRUE;
	}

	if (m_isMoveArea)
	{	int xs	=	x	-	oldX,ys	=	y-oldY;
		if (xs!=0 || ys!=0)	g_lpActmap->LostVirgin();

		memcpy(&m_aArea[m_iSelectArea].x1,&oldArea,sizeof(oldArea));
		m_aArea[m_iSelectArea].move(xs,ys);

		return TRUE;
	}

	return FALSE;
}


int
cAREA::GetArea(char *an)
{
	for (int index=2,i=2;index<m_iCount && i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial!=0xffff)
		{
			if (stricmp(m_aArea[i].m_strName,an)==0)	return	i;

			index++;
		}

	return	0xffff;
}

//
//	필드 전체,화면 전체를 제외한 첫번째 영역을 찾는다.
//
int	
cAREA::GetFirstArea()
{
	if (m_iCount	<=	2)	return	0xffff;

	for (int i=2;i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial	!=	0xffff)
			return	i;

	return	0xffff;
}	//	cAREA::GetFirstArea()

//
//	새로운 영역을 설정한다.
//
void
cAREA::SetNewArea()
{
	for (int i=0;i<dMAX_AREA;i++)
		if (m_aArea[i].m_wSerial	==	0xffff)
		{
			m_iNewSerial	=	i;

			return;
		}
}	//	cAREA::SetNewArea()

//
//	시작 영역을 구한다.
int
cAREA::getStartArea()
{
	for (int i=0;i<dMAX_AREA;i++)
	{
		if (m_aArea[i].m_wSerial	==	0xffff	)	continue;
		if (m_aArea[i].isStartArea()			)	return i;
	}

	return	0xffff;
}	//	cAREA::getStartArea()

//
//	게이트들 연결
BOOL
cAREA::linkGate()
{
	BOOL	bisNotExistNotLinkedGate	=	TRUE;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAREA_INFO	*lpArea		=	&m_aArea[i];

		if	(lpArea->m_wSerial	==	0xffff	)
			continue;
		if	(!lpArea->isPortal()			)
			continue;
		if	(lpArea->m_gateShape	==	ePAS_ONE_WAY_GATE	||	lpArea->m_gateShape	==	ePAS_ENTER_GUILD_HALL	||	
			lpArea->m_gateShape	==	ePAS_LEAVE_GUILD_HALL)
			continue;

		if	(!lpArea->m_pString)
		{
			lpArea->m_wMoveGate		=	0xffff;
			bisNotExistNotLinkedGate=	FALSE;
			continue;
		}

		lpArea->m_wMoveGate		=	cMAP::GetPortalArea(lpArea->m_pString,lpArea->m_strMoveGateName);

		if	(lpArea->m_wMoveGate	==	0xffff)
			bisNotExistNotLinkedGate	=	FALSE;
	}

	cPROJECT::SetMapFolder();

	return	bisNotExistNotLinkedGate;
}

//
//
BOOL
cAREA::checkTwoWayLinkGate(char *_lpstrMapname)
{
	BOOL	bisNotExistNotLinkedGate	=	TRUE;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAREA_INFO	*lpArea		=	&m_aArea[i];

		if (lpArea->m_wSerial	==	0xffff	)	continue;
		if (!lpArea->isPortal()				)	continue;

		if(!lpArea->m_pString)
		{
			lpArea->m_wMoveGate		=	0xffff;
			bisNotExistNotLinkedGate=	FALSE;
			continue;
		}

		if (!cMAP::CheckPortalArea(lpArea->m_pString,lpArea->m_strName,lpArea->m_strMoveGateName))
		{
			int iResult = cMSG::YESNO("Do you want to continue?", "The following areas are not two-way links.\n\nField '%s' - '%s'\nField '%s' - '%s'", _lpstrMapname, lpArea->m_strName, lpArea->m_pString, lpArea->m_strMoveGateName);

			if (iResult	==	IDNO)	return	FALSE;
		}
	}

	return	bisNotExistNotLinkedGate;
}	//	cAREA::checkTwoWayLinkGate()

BOOL
cAREA::isExistDoorAreaPos(int _iX,int _iY)
{
	for (int i=0;i<dMAX_AREA;i++)
	{
		cAREA_INFO	*lpArea		=	&m_aArea[i];

		if	(lpArea->m_wSerial	==	0xffff	)
			continue;
		if	(!lpArea->isDoor()				)
			continue;

		if	(lpArea->isIn(_iX,_iY))
			return	TRUE;
	}

	return	FALSE;
}
