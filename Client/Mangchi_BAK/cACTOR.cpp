#include	"cACTOR.H"
#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAP.H"
#include "cSetDialogNpcData.h"					//	----------------------------------	추가
#include	"CSetDialogMonster.h"					//	11.23 추가
cPOWER_INFO	g_aPowers[50]	=
{
	{eABP_LEVEL				,dPOWER_VALUE		,"레벨"				,1	,1000			},
	{eABP_STRENGTH			,dPOWER_VALUE		,"힘"				,5	,1000			},
	{eABP_DEXTERITY			,dPOWER_VALUE		,"민첩성"			,5	,1000			},
	{eABP_CONSTITUTION		,dPOWER_VALUE		,"건강"				,5	,1000			},
	{eABP_WISDOM			,dPOWER_VALUE		,"지혜"				,5	,1000			},
	{eABP_INTELLIGENCE		,dPOWER_VALUE		,"지식"				,5	,1000			},
	{eABP_CHARISMA			,dPOWER_VALUE		,"카리스마"			,5	,1000			},
	{eABP_LUCK				,dPOWER_VALUE		,"운"				,5	,1000			},

	{eABP_ATTACK			,dPOWER_VALUE		,"공격력"			,1	,65535			},
	{eABP_DEFENSE			,dPOWER_VALUE		,"방어력"			,1	,65535			},
	{eABP_FIRE_RESIST		,dPOWER_PERCENTAGE	,"불 저항"			,0	,100			},
	{eABP_WATER_RESIST		,dPOWER_PERCENTAGE	,"물 저항"			,0	,100			},
	{eABP_WIND_RESIST		,dPOWER_PERCENTAGE	,"바람 저항"		,0	,100			},
	{eABP_EARTH_RESIST		,dPOWER_PERCENTAGE	,"대지 저항"		,0	,100			},
	{eABP_LIGHT_RESIST		,dPOWER_PERCENTAGE	,"빛 저항"			,0	,100			},
	{eABP_DARK_RESIST		,dPOWER_PERCENTAGE	,"어둠 저항"		,0	,100			},
	{eABP_MAGIC_RESIST		,dPOWER_PERCENTAGE	,"마법 저항"		,0	,100			},
	{0xffff},
};

cPOWER_INFO	g_aLimitedPowers[50]	=
{
	{eABP_LEVEL						,dPOWER_VALUE		,"체력"				,10	,10000			},
	{eABP_STRENGTH					,dPOWER_VALUE		,"파워 게이지"		,1	,5000			},
	{0xffff},
};


cACTOR::~cACTOR()
{
	KILL(m_pCC);
}

cCHARACTER*
cACTOR::getCharacter()
{
	return	&g_lpActmap->m_setCharacter.m_aCharacter[m_wCharacter];
}

void	
cACTOR::deepCopy(cACTOR* _pActor)						// 09.07.29   깊은 복사.
{
	memcpy(m_awLinkMonster ,_pActor->m_awLinkMonster,sizeof(WORD) * dMAX_LINK_MONSTER_COUNT);
	strcpy(m_strName, _pActor->m_strName);
	// insu add
	m_wGenerateMethod = _pActor->m_wGenerateMethod;
	m_wGenerateAroundPosX1 = _pActor->m_wGenerateAroundPosX1;
	m_wGenerateAroundPosY1 = _pActor->m_wGenerateAroundPosY1;
	m_wGenerateAroundPosX2 = _pActor->m_wGenerateAroundPosX2;
	m_wGenerateAroundPosY2 = _pActor->m_wGenerateAroundPosY2;

	m_dwRegenerationCycle	= _pActor->m_dwRegenerationCycle;
	m_wWanderCycle			= _pActor->m_wWanderCycle;
	m_wMoveArea				= _pActor->m_wMoveArea;
	// insu add end	
	if	(_pActor->m_pCC == NULL)
		return;

	if	(m_pCC == _pActor->m_pCC)
	{
		m_pCC = NULL;
	}
	else
	if	(m_pCC)
	{
		KILL(m_pCC);
	}

	m_pCC = new CConversationContainer();
	
	m_pCC->copy(_pActor->m_pCC);
	
}
//
//	대화 설정
void
cACTOR::setConversation(CConversationContainer	*_lpCc)
{
	pKILL(m_pCC);

	if (!_lpCc)	return;

	if (_lpCc->m_wConversationCount <= 0 )	return;

	m_pCC	=	new CConversationContainer();

	m_pCC->copy(_lpCc);
}

//
//	dx,dy로 보정한 곳에 찍어준다.
void
cACTOR::put(int dx,int dy)
{
	if (m_iSerial	==	0xffff)	return;

	cCHARACTER	*lpCharacter	=	getCharacter();

	int		iBody	=	getBody();
	cANMQ	*lpBody	=	g_pBody[iBody];
	int		x		=	m_pos.x	-	dx;
	int		y		=	m_pos.y	-	dy;
	cJOB	*lpJob	=	getJob();

	int		i,j;

	cANMQ	*lpVBody=	NULL;

	if	(lpJob->m_wVariation)
		lpVBody	=	g_apMonsterVariation[iBody][lpJob->m_wVariation-1];

	if (g_aMonsterPalette[iBody].m_wCount)
	{
		memcpy(lpBody->pPLT,&g_aMonsterPalette[iBody].m_pPalette[lpJob->m_wPalette],512);
	}

	int		iDirect	=	m_wDirect;
	int		iBodySize=	lpBody->m_iCrashSize	-	dACTOR_BODY_SIZE_CORRECT;

	if (m_wDirect	==	8)	iDirect	=	4;

	int		x1		=	(m_pos.x	-	iBodySize)	/	dTILE_REAL_WIDTH;
	int		x2		=	(m_pos.x	+	iBodySize)	/	dTILE_REAL_WIDTH;
	int		y1		=	(m_pos.y	-	iBodySize/2)/	dTILE_REAL_HEIGHT;
	int		y2		=	(m_pos.y	+	iBodySize/2)/	dTILE_REAL_HEIGHT;

	BOOL isCrash	=	FALSE;

	for (i=y1;i<=y2;i++)
		for (j=x1;j<=x2;j++)
			if (g_lpActmap->isCrashTile(j,i))	isCrash	=	TRUE;

	if	(isCrash)
		lpBody->PutRed(x,y,iDirect,lpCharacter->m_wScaleX,lpCharacter->m_wScaleY);
	else
		lpBody->Put(x,y,iDirect,lpCharacter->m_wScaleX,lpCharacter->m_wScaleY);

	if	(lpVBody)
	{
		if	(isCrash)
			lpVBody->PutRed(x,y,iDirect,lpCharacter->m_wScaleX,lpCharacter->m_wScaleY);
		else
			lpVBody->Put(x,y,iDirect,lpCharacter->m_wScaleX,lpCharacter->m_wScaleY);
	}

	if	(m_isSelected)
	{	
		lpBody->PutLayer(x,y,iDirect,_LTGREEN,lpCharacter->m_wScaleX,lpCharacter->m_wScaleY);
	
		if	(lpVBody)
			lpVBody->PutLayer(x,y,iDirect,_LTGREEN,lpCharacter->m_wScaleX,lpCharacter->m_wScaleY);

		if (m_wLinkMonsterCount	>	0)
		{
			for (i=0;i<m_wLinkMonsterCount;i++)
			{
				int	iDestX,iDestY;

				cACTOR	*lpActor	=	&g_lpActmap->m_setActor.m_aActor[m_awLinkMonster[i]];

				iDestX	=	lpActor->m_pos.x-dx;
				iDestY	=	lpActor->m_pos.y-dy;

				cDRAW::Line(_LTYELLOW,x,y,iDestX,iDestY);
			}
		}
	}
}	//	cACTOR::Put(int dx,int dy)

//
//	dx,dy로 보정한 곳에 찍어준다.
void
cACTOR::putName(int dx,int dy)
{
	if	(m_iSerial	==	0xffff)
		return;

	int		iBody	=	getBody();
	cANMQ	*lpBody	=	g_pBody[iBody];
	int		x		=	m_pos.x	-	dx;
	int		y		=	m_pos.y	-	dy;

	int		iDirect	=	m_wDirect;
	int		iBodySize=	lpBody->m_iCrashSize	-	dACTOR_BODY_SIZE_CORRECT;

	if (m_wDirect	==	8)	iDirect	=	4;

	int			xs;
	cCHARACTER	*lpCharacter	=	getCharacter();
	WORD		wFillColor		=	0;
	DWORD		dwTextColor		=	WHITE;
	cJOB	*lpJob	=	getJob();

	if (g_aMonsterPalette[iBody].m_wCount)
	{
		memcpy(lpBody->pPLT,&g_aMonsterPalette[iBody].m_pPalette[lpJob->m_wPalette],512);
	}

	lpBody->PutHalf(x,y,iDirect,lpCharacter->m_wScaleX,lpCharacter->m_wScaleY);

	if	(m_isSelected)
		dwTextColor	=	LTGREEN;

	if (m_wType==dTEAM_NPC||m_wType>dTEAM_MONSTER)
	{
		xs	=	cTEXT::GetSize(_ms("[%d]%s",m_iSerial,m_strName));
		cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+12);
		cTEXT::cPut(x+1,y,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_NAME],_ms("[%d]%s",m_iSerial,m_strName));
	}
	else
	{
		xs	=	cTEXT::GetSize(_ms("[%d]%s %d~%d",m_iSerial,m_strName,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel));
		cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+12);
		cTEXT::cPut(x+1,y,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_NAME],_ms("[%d]%s %d~%d",m_iSerial,m_strName,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel));
	}

//	리더나 피보호자 표기
	if(m_wLinkMonsterCount	>	0)
	{
		char	strText[128];

		y	+=	15;

		strcpy(strText,_ms("%s(%d)",g_strLinkType[m_wLinkType],m_wLinkMonsterCount+1));

		if (m_bf1IsLeader	&&	m_isWard	)	strcpy(strText,_ms("%s - %s/%s",strText,dMSG_LEADER,dMSG_WARD));
		if (!m_bf1IsLeader	&&	m_isWard	)	strcpy(strText,_ms("%s - %s",strText,dMSG_WARD));
		if (m_bf1IsLeader	&&	!m_isWard	)	strcpy(strText,_ms("%s - %s",strText,dMSG_LEADER));

		xs	=	cTEXT::GetSize(strText);

		cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+12);
		// 09.12.18
		cTEXT::cPut(x,y,cSET_FONT::s_OldColorOption[eCOLOR_LEADER],strText);
		//cTEXT::cPut(x,y,LTYELLOW,strText);

		y	-=	15;
	}	//	리더나 피보호자 표기

	if (m_wType	>=	eNPC_WEAPON_MERCHANT)	//	무기 상인
	{
		y	-=	15;

		dwTextColor	=	RGB(255,128,255);

		xs	=	cTEXT::GetSize(g_strNpcCategorty[m_wType]);
		cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+12);
		// 09.12.18
		cTEXT::cPut(x,y,cSET_FONT::s_OldColorOption[eCOLOR_NPC_TYPE],g_strNpcCategorty[m_wType]);
		//cTEXT::cPut(x,y,dwTextColor,g_strNpcCategorty[m_wType]);
		
	}
	else
	if (m_pCC)
	{
		y	-=	15;

		dwTextColor	=	RGB(255,128,255);

/*		if (m_pCC->findShopContents())
		{
			xs	=	cTEXT::GetSize(dMSG_OWN_SHOP_NPC);
			cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+12);
			cTEXT::cPut(x,y,dwTextColor,dMSG_OWN_SHOP_NPC);
		}
		else*/
		{
			xs	=	cTEXT::GetSize(dMSG_OWN_SPEECH_NPC);
			cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+12);
			
			// 09.12.18
			cTEXT::cPut(x,y,cSET_FONT::s_OldColorOption[eCOLOR_NPC_TYPE],dMSG_OWN_SPEECH_NPC);
			//cTEXT::cPut(x,y,dwTextColor,dMSG_OWN_SPEECH_NPC);
			
		}
	}

	if	(lpCharacter->m_bf8Skin)
	{
		char	strText[128];

		y	-=	15;

		dwTextColor	=	RGB(255,128,255);

		sprintf(strText, "스킨 [%s]",g_astrNpcSkin[lpCharacter->m_bf8Skin]);

		xs	=	cTEXT::GetSize(strText);
		cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+12);
		// 09.12.08
		cTEXT::cPut(x,y,cSET_FONT::s_OldColorOption[eCOLOR_SKIN],strText);
		//cTEXT::cPut(x,y,dwTextColor,strText);
	}

	if	(m_wMoveArea	!=	0xffff)
	{
		cAREA_INFO	*lpArea	=	&g_lpActmap->m_pArea->m_aArea[m_wMoveArea];

		if (lpArea->m_wSerial	==	0xffff)
		{
			y	+=	15;
			cTEXT::cPut(x,y,LTRED,dMSG_WRONG_MOVE_AREA);
		}
	}
}

//
//	dx,dy로 보정한 곳에 찍어준다.
void
cACTOR::putScale(int dx,int dy,int _iTBsize,int _iScale)
{
	if (m_iSerial	==	0xffff)
		return;

	int	x			=	m_pos.x	- dx ;
	int	y			=	m_pos.y	- dy;

	x	=	x*_iScale/100;
	y	=	y*_iScale/100 +_iTBsize;

	int			xs;
	cCHARACTER	*lpCharacter	=	getCharacter();
	WORD		wFillColor		=	0;
	WORD		wBoxColor		=	_WHITE;
	DWORD		dwTextColor		=	WHITE;

	if (m_wLinkMonsterCount	>	0)
	{
		dwTextColor	=	LTCYAN;
		wBoxColor	=	_LTCYAN;
	}

	if (m_isSelected)
	{
		dwTextColor	=	LTGREEN;
		wBoxColor	=	_LTGREEN;
	}

	xs	=	cTEXT::GetSize(_ms("%s[%d~%d]",m_strName,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel));
	cDRAW::FillHB(wFillColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+14);
	cDRAW::Box(wBoxColor,x - xs/2-2 ,y-2 ,x + xs/2+2,y+14);
	cTEXT::cPut(x+1,y+1,dwTextColor,_ms("%s[%d~%d]",m_strName,lpCharacter->m_wMinLevel,lpCharacter->m_wMaxLevel));
}

//
//	캐릭터 몸통에 충돌하는지 확인한다.
//
BOOL
cACTOR::isCrash(int _iX1,int _iY1,int _iX2,int _iY2)
{
	int	iBodySize	=	g_pBody[getBody()]->m_iCrashSize	-	dACTOR_BODY_SIZE_CORRECT;

	int	x1			=	(m_pos.x	-	iBodySize)	/	dTILE_REAL_WIDTH;
	int	x2			=	(m_pos.x	+	iBodySize)	/	dTILE_REAL_WIDTH;
	int	y1			=	(m_pos.y	-	iBodySize/2)/	dTILE_REAL_HEIGHT;
	int	y2			=	(m_pos.y	+	iBodySize/2)/	dTILE_REAL_HEIGHT;

//	MSGOUT("%s : %d,%d-%d,%d vs %d,%d-%d,%d",m_strName,x1,y1,x2,y2,_iX1,_iY1,_iX2,_iY2);

	if (_iX1 > x2) return FALSE;
	if (_iX2 < x1) return FALSE;
	if (_iY1 > y2) return FALSE;
	if (_iY2 < y1) return FALSE;

	return	TRUE;
}

//
//	올라갈 수 없는 지형에 자리잡고있다.
//
BOOL
cACTOR::isMisplaced()
{
	int	iBodySize	=	g_pBody[getBody()]->m_iCrashSize	-	dACTOR_BODY_SIZE_CORRECT;

	int	x1			=	(m_pos.x	-	iBodySize)	/	dTILE_REAL_WIDTH;
	int	x2			=	(m_pos.x	+	iBodySize)	/	dTILE_REAL_WIDTH;
	int	y1			=	(m_pos.y	-	iBodySize/2)/	dTILE_REAL_HEIGHT;
	int	y2			=	(m_pos.y	+	iBodySize/2)/	dTILE_REAL_HEIGHT;

	for (int y=y1;y<=y2;y++)
		for (int x=x1;x<=x2;x++)
			if (g_lpActmap->isCrashTile(x,y))	return	TRUE;

	if (m_wMoveArea	!=	0xffff)
	{
		cAREA_INFO	*lpArea	=	&g_lpActmap->m_pArea->m_aArea[m_wMoveArea];

		if (lpArea->m_wSerial	==	0xffff)	return	TRUE;
	}


	return	FALSE;
}	//	cACTOR::isMisplaced()

//
//	캐릭터 몸통끼리 충돌하는지 확인한다.
//
BOOL
cACTOR::isTouchedRange(int x,int y,int _iRange)
{
	int	size	=	g_pBody[getBody()]->m_iCrashSize;

	if	(GetOvalRange(m_pos.x,m_pos.y,x,y)	<=	(size+_iRange)*(size+_iRange))	return	TRUE;

	return	FALSE;
}	//	cACTOR::isTouchedRange(int x,int y,int _iRange)

//
//	링크 몬스터중 _iIndex를 제거
void
cACTOR::removeLinkMonster(int _iIndex)
{
	if (m_wLinkMonsterCount	>	0)
	{
		for (int i=0;i<m_wLinkMonsterCount;i++)
		{
			if (m_awLinkMonster[i]	==	_iIndex)
			{
				if (i	<	m_wLinkMonsterCount-1)
					memcpy(&m_awLinkMonster[i],&m_awLinkMonster[i+1],(m_wLinkMonsterCount-i-1)*sizeof(WORD));

				m_wLinkMonsterCount--;

				return;
			}
		}
	}
}	//	cACTOR::removeLinkMonster(int _iIndex)

//
//	리더로 설정
void
cACTOR::setLeader(BOOL _isLeader)
{
	if (m_wLinkMonsterCount	==	0)	return;
	
	m_bf1IsLeader	=	TRUE;

	g_lpActmap->LostVirgin();
}	//	cACTOR::setLeader(BOOL _isLeader)

//
//	요인으로 설정
void
cACTOR::setWard(BOOL _isWard)
{
	if (m_wLinkMonsterCount	==	0)	return;

	m_isWard	=	TRUE;

	g_lpActmap->LostVirgin();
}	//	cACTOR::setWard(BOOL _isWard)

//
//	링크 설정
void
cACTOR::setLinkNpc(int *_lpLinkList,int _iCount)
{
	int	iLinkNpcCount	=	0,i;

	for (i=0;i<_iCount;i++)	
	{
		int	iNpc	=	_lpLinkList[i];

		if (iNpc	!=	m_iSerial)	m_awLinkMonster[iLinkNpcCount++]	=	iNpc;
	}

	m_wLinkMonsterCount	=	iLinkNpcCount;
	
	g_lpActmap->LostVirgin();
}	//	cACTOR::setLinkNpc(int *_lpLinkList,int _iCount)

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//	액터 세트
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
//	액터 세트 초기화
//
cACTOR_SET::cACTOR_SET()
{
	m_iCount					=	0;
	m_iCurrentMissplaceActor	=	0;
}	//	cACTOR_SET::cACTOR_SET()

//
//	액터 세트 리셋
//
void
cACTOR_SET::reset()
{
	m_iCount				=	0;
	m_iCurrentMissplaceActor=	0;

	for (int i=0;i<dMAX_ACTOR_COUNT;i++)	m_aActor[i].reset();

/*	MSGOUT("size fo cACTOR_SET        : %d",sizeof(cACTOR_SET));
	MSGOUT("size fo cACTOR            : %d",sizeof(cACTOR));
	MSGOUT("size fo cJOB_BASE_DATA    : %d",sizeof(cJOB_BASE_DATA));
	MSGOUT("size fo cJOB_DATA_CORRECT : %d",sizeof(cJOB_DATA_CORRECT));

	MSGOUT("size fo cEQUIPMENT        : %d",sizeof(cITEM)*50);
	MSGOUT("size fo cITEM             : %d",sizeof(cITEM));
	MSGOUT("size fo m_aAbility        : %d",sizeof(m_aActor[0].m_aAbility));
	*/
}

//
//	비어 있는 인데스
//
int
cACTOR_SET::getValidIndex()
{
	for (int i=0;i<dMAX_ACTOR_COUNT;i++)
		if (m_aActor[i].m_iSerial	==	0xffff)	return	i;

	return	0xffff;
}	//	cACTOR_SET::GetValidIndex()

//
//	x,y checkSize + 10 만한 캐릭터를 찍을 수 있나 확인한다.
//
int
cACTOR_SET::getCrashActor(int x,int y,int checkSize)
{
	int		i,iCount=	0;

	if	(checkSize	==	0xffff)
		checkSize	=	g_lpActmap->m_setCharacter.getBodySize();

	int	iBodySize	=	2;//checkSize-dACTOR_BODY_SIZE_CORRECT;

	int	x1			=	(x	-	iBodySize)	/	dTILE_REAL_WIDTH;
	int	x2			=	(x	+	iBodySize)	/	dTILE_REAL_WIDTH;
	int	y1			=	(y	-	iBodySize/2)/	dTILE_REAL_HEIGHT;
	int	y2			=	(y	+	iBodySize/2)/	dTILE_REAL_HEIGHT;

	for (i=0;i<dMAX_ACTOR_COUNT;i++)	//	캐릭터들
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			if	(GetOvalRange(x,y,m_aActor[i].m_pos.x,m_aActor[i].m_pos.y)	<	2*2)
				return	i;
		}
	}

	return	0xffff;
}	//	cACTOR_SET::getCrashActor(int x,int y,int checkSize)

//
//	 캐릭터를 x,y에 찍으려고 하는데.. 충돌 일으키지 않는지 확인해 본다.
//
BOOL
cACTOR_SET::isCrash(int x,int y,int _iBodySize)
{
	if (_iBodySize	==	0xffff)
		_iBodySize	=	g_lpActmap->m_setCharacter.getBodySize();

	int	iBodySize	=	_iBodySize	-	dACTOR_BODY_SIZE_CORRECT;
	int	i,j;

	int	x1			=	(x	-	iBodySize)	/	dTILE_REAL_WIDTH;
	int	x2			=	(x	+	iBodySize)	/	dTILE_REAL_WIDTH;
	int	y1			=	(y	-	iBodySize/2)/	dTILE_REAL_HEIGHT;
	int	y2			=	(y	+	iBodySize/2)/	dTILE_REAL_HEIGHT;

	for (i=y1;i<=y2;i++)
	{
		for (j=x1;j<=x2;j++)
			if (g_lpActmap->isCrashTile(j,i))
				return	TRUE;
	}

	if	(getCrashActor(x,y)	!=	0xffff)
		return	TRUE;

	return	FALSE;
}	//	cACTOR_SET::isCrash(int x,int y)

//
//	액터 추가
int
cACTOR_SET::add(cACTOR *_lpActor)
{// 마지막 수정일 : 09.10.30
	if	(isCrash(_lpActor->m_pos.x,_lpActor->m_pos.y,_lpActor->getBodySize()))
		return	0xffff;

	int	index;

	index		=	getFreeIndex();
	if (index	==	0xffff)	return	0xffff;

	memcpy(&m_aActor[index],_lpActor,sizeof(cACTOR));

	m_aActor[index].m_iSerial	=	index;

	if (_lpActor->getJob()->m_wType	==	dJOB_GROUP_MONSTER)	m_aActor[index].m_wType	=	eNPC_MONSTER;
	if (_lpActor->getJob()->m_wType	==	dJOB_GROUP_NPC)		m_aActor[index].m_wType	=	eNPC_NORMAL;

	m_iCount++;
	cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
	CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
	CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
	return	index;
}	//	cACTOR_SET::Add(cACTOR *lpActor)

//
//	index에 캐릭터를 얻어 온다
cACTOR*
cACTOR_SET::getActor(int index)
{
	if (index>=dMAX_ACTOR_COUNT || index<0)	return	NULL;
	if (m_aActor[index].m_iSerial	==	0xffff)	return	NULL;

	return	&m_aActor[index];
}

//
//	index에 캐릭터를 제거한다.
BOOL
cACTOR_SET::remove(int	index)
{// 마지막 수정일 : 09.10.30
	if (m_aActor[index].m_iSerial	==	0xffff)	return	FALSE;

	m_aActor[index].reset();

	m_iCount--;

	g_lpActmap->LostVirgin();

	removeThisActorAtLinkMonsterList(index);
	cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
	CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
	CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
	return	TRUE;
}	//	cACTOR_SET::remove(int	index)


//
//	이름 바꾸기
void
cACTOR_SET::changeName(WORD _wCharacterSerial,char *_lpstrName)
{// 마지막 수정일 : 09.10.30
	int	i;

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		if(m_aActor[i].m_wCharacter	==	_wCharacterSerial)
			strncpy(m_aActor[i].m_strName,_lpstrName,19);
	}
	cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
	CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
	CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
}	//	cACTOR_SET::changeName(WORD _wCharacterSerial,char *_lpstrName)

//
//	job의 직업을 가진 캐릭터를 제거한다.
BOOL
cACTOR_SET::removeHero(int	job)
{// 마지막 수정일 : 09.10.30
	for (int i=0;i<dMAX_ACTOR_COUNT;i++)
		if (m_aActor[i].m_iSerial	!=	0xffff)
			if (m_aActor[i].m_wJob	==	job)
				return	remove(i);
	cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
	CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
	CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
	return	FALSE;
}	//	cACTOR_SET::removeHero(int	job)


void							//				---------------- 새로 만듬
cACTOR_SET::removeAllNpc()
{
	int	i,iCount,iActorCount=m_iCount;

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<iActorCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			if(m_aActor[i].m_wType != eNPC_MONSTER)
				remove(i);

			iCount++;
		}
	}
}
void		
cACTOR_SET::removeAllMonster()
{	// 09.11.23 수정
	int	i,iCount,iActorCount=m_iCount;

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<iActorCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			if(m_aActor[i].m_wType == eNPC_MONSTER)
				remove(i);

			iCount++;
		}
	}
}
//
//	뷰포트 안에 캐릭터들을 구한다.
//
int
cACTOR_SET::getActors(cINDEX *indexes,cRECT *viewport)
{	int i,count=0,iCount=0;

	if (m_iCount	<=	0)	return	0;

	viewport->x1	-=	100;
	viewport->y1	-=	200;
	viewport->x2	+=	100;
	viewport->y2	+=	200;

	for (i=0;i<dMAX_ACTOR_COUNT && iCount < m_iCount;i++)	//	바닥에 깔리는 오브젝트
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{	if (viewport->isIN(m_aActor[i].m_pos.x,m_aActor[i].m_pos.y))
			{	
				indexes[count].Value	=	m_aActor[i].m_pos.y;
				indexes[count].Index	=	i+dBORDER_ACTOR;

				count++;
			}

			iCount++;
		}
	}

	viewport->x1	+=	100;
	viewport->y1	+=	200;
	viewport->x2	-=	100;
	viewport->y2	-=	200;

	return count;
}	//	cOBJECT::GetActors(cINDEX *indexes,cRECT *viewport)

//
//	모든 캐릭터 선택 취소
void
cACTOR_SET::freeSelect()
{
	for (int i=0;i<dMAX_ACTOR_COUNT;i++)	m_aActor[i].m_isSelected	=	FALSE;
}	//	cACTOR_SET::freeSelect()

//
//	특정 영역 안에 캐릭터를 선택한다.
void
cACTOR_SET::select(cRECT *rect,cRECT *screenRect)
{	cRECT	map;
	WORD	i;
	int		iCount=0;

	if ((!KEY[dKEY_SHIFT]) && (!KEY[dKEY_CONTROL]))	freeSelect();

	cRECT	objectCrash;

	if (KEY[dKEY_CONTROL])	//	컨트롤 키를 눌렀다.
	{	WORD	select	=	0xffff;

		if (rect->size()	<	4)	//	똑같은 넘들은 모두.. 선택..
		{
			for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
			{
				if (m_aActor[i].m_iSerial	!=	0xffff)
				{
					if (m_aActor[i].isTouchedRange(rect->x1,rect->y1))		//	
					{
//						if (m_aActor[i].m_isSelected && !KEY[dKEY_SHIFT])	m_aActor[i].m_isSelected	=	FALSE;
//						else
						select	=	m_aActor[i].m_wJob;
						break;
					}

					iCount++;
				}
			}

			if (select!=0xffff)
			{	if (!KEY[dKEY_SHIFT])   freeSelect();	//	쉬프트 키를 안 눌렀으면 전부 선택 해제

				for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
				{
					if (m_aActor[i].m_iSerial	!=	0xffff)
					{
						if (m_aActor[i].m_wJob	==	select)
							if (screenRect->isIN(m_aActor[i].m_pos.x,m_aActor[i].m_pos.y))		//	
								m_aActor[i].m_isSelected	=	TRUE;

						iCount++;
					}
				}

				return;
			}
		}
		else	//	DeSelect
		{
			for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
			{
				if (m_aActor[i].m_iSerial	!=	0xffff)
				{
					if (rect->isIN(m_aActor[i].m_pos.x,m_aActor[i].m_pos.y))		//	
						if (m_aActor[i].m_isSelected)	m_aActor[i].m_isSelected	=	FALSE;

					iCount++;
				}
			}

			return;
		}
	}

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			if (rect->isIN(m_aActor[i].m_pos.x,m_aActor[i].m_pos.y))		//	
				m_aActor[i].m_isSelected	=	TRUE;

			iCount++;
		}
	}
}	//	cACTOR_SET::Select(cRECT *rect,cRECT *screenRect)


//
//	특정 영역 안에 캐릭터를 선택한다.
void
cACTOR_SET::selectSameKind()
{
	WORD	i;
	int		iCount=0;
	WORD	wJob = 0xffff;

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
	{
		if(m_aActor[i].m_isSelected	==	TRUE)
		{
			wJob = m_aActor[i].m_wCharacter;
			break;
		}
	}

	freeSelect();

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff && wJob == m_aActor[i].m_wCharacter)
		{
			m_aActor[i].m_isSelected	=	TRUE;

			iCount++;
		}
	}
}	//	cACTOR_SET::Select(cRECT *rect,cRECT *screenRect)

//
//	특정 영역 안에 캐릭터를 선택한다.
void
cACTOR_SET::selectAll()
{
	WORD	i;
	int		iCount=0;
	
	freeSelect();
	
	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			m_aActor[i].m_isSelected	=	TRUE;
			
			iCount++;
		}
	}
}	//	cACTOR_SET::Select(cRECT *rect,cRECT *screenRect)

//
//	모든 캐릭터를 제거한다.
void
cACTOR_SET::removeAll()
{
	int	i,iCount,iActorCount=m_iCount;

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<iActorCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			remove(i);

			iCount++;
		}
	}

	m_iCount	=	0;
}	//	cACTOR_SET::removeAll()

//
//	모든 캐릭터를 제거한다.
void
cACTOR_SET::removeSelectedActors()
{
	int	i,iCount,iActorCount=m_iCount;

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<iActorCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			if (m_aActor[i].m_isSelected)
				remove(i);

			iCount++;
		}
	}
}	//	cACTOR_SET::removeAll()

//
//	비어있는 캐릭터 영웅 인덱스를 얻어 온다.
int
cACTOR_SET::getFreeIndex()
{
	for (int i=0;i<dMAX_ACTOR_COUNT;i++)
		if (m_aActor[i].m_iSerial	==	0xffff)	return	i;

	return	0xffff;
}	//	cACTOR_SET::getFreeIndex()

//
//	제대로 찍혀 있는 Actor의 수
int
cACTOR_SET::getCorrectCount()
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_ACTOR_COUNT;i++)
		if (m_aActor[i].m_iSerial	!=	0xffff)
			if (!m_aActor[i].isMisplaced())	iCount++;

	return	iCount;
}	//	cACTOR_SET::getCorrectCount()

//
//	지워진 캐릭터 
BOOL
cACTOR_SET::reBuild()
{
	int		i;
	BOOL	isRemovedAnyone	=	FALSE;

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			if (!g_lpActmap->m_setCharacter.isAvailCharacter(m_aActor[i].m_wCharacter))
			{
				remove(i);
				isRemovedAnyone	=	TRUE;
			}
		}

	return		isRemovedAnyone;
}	//	cACTOR_SET::reBuild()

//
//	선택되어 있는 액터들을 구한다.
int
cACTOR_SET::getSelectedActor()
{
	int		i,iCount=0;

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
		if (m_aActor[i].m_iSerial	!=	0xffff)
			if (m_aActor[i].m_isSelected)
				m_aSelectedActor[iCount++]	=	i;

	m_iSelectedActorCount	=	iCount;

	return	m_iSelectedActorCount;
}	//	cACTOR_SET::getSelectedActor(int *_lpSelectedAcotrList)

//
//	_iIndex번째 액터를 리더로 설정
void
cACTOR_SET::setLeader(int _iIndex,BOOL _isLeader)
{
	cACTOR	*lpLeader	=	&m_aActor[_iIndex];

	if (lpLeader->m_wLinkMonsterCount	==	0)	return;

	if(_isLeader)	//	리더 설정하는거면... 다른 넘들은 전부 리더 아닌걸로..
		for (int i=0;i<lpLeader->m_wLinkMonsterCount;i++)
		{
			int		iLinkMonster=	lpLeader->m_awLinkMonster[i];
			cACTOR	*lpActor	=	&m_aActor[iLinkMonster];

			lpActor->m_bf1IsLeader	=	FALSE;
		}

	lpLeader->m_bf1IsLeader	=	_isLeader;
}	//	cACTOR_SET::setLeader(int _iIndex,BOOL _isLeader)

//
//	_iIndex번째 액터를 피보호자로 설정
void
cACTOR_SET::setWard(int _iIndex,BOOL _isWard)
{
	cACTOR	*lpWard	=	&m_aActor[_iIndex];

	if (lpWard->m_wLinkMonsterCount	==	0)	return;

	lpWard->m_isWard	=	_isWard;
}	//	cACTOR_SET::setWard(int _iIndex,BOOL _isWard)

//
//	선택된 몬스터들을 서로 링크 시킨다.
void
cACTOR_SET::linkSelectedMonster()
{
	int	iCount	=	getSelectedActor(),i;

	if (iCount	<=	1)	return;

	if (iCount	>=	dMAX_LINK_MONSTER_COUNT)
	{
		ERRMSG("error",dMSG_TOO_MANY_LINK_MONSTER);

		return;
	}

	removeSelectedMonstersLink();	//	먼저 기존의 링크를 제거한다.

	for (i=0;i<iCount;i++)
	{
		cACTOR	*lpActor	=	&m_aActor[m_aSelectedActor[i]];

		lpActor->setLinkNpc(m_aSelectedActor,iCount);

	}
}	//	cACTOR_SET::linkSelectedMonster()

//
//	단방향? 쌍방향? 링크
void
cACTOR_SET::setLinkType(int _iIndex,int _iType)
{
	cACTOR	*lpActor	=	&m_aActor[_iIndex];
	int		i;

	lpActor->m_wLinkType=	_iType;

	for (i=0;i<lpActor->m_wLinkMonsterCount;i++)
	{
		int	iLinkActor	=	lpActor->m_awLinkMonster[i];

		m_aActor[iLinkActor].m_wLinkType=	_iType;
	}

}	//	cACTOR_SET::setLinkDirect(int _iIndex,int _iType)
//
//	선택된 몬스터들의 링크를 제거한다.
void
cACTOR_SET::removeSelectedMonstersLink()
{
	int	iCount	=	getSelectedActor(),i;

	if (iCount	<=	0)	return;

	for (i=0;i<iCount;i++)
	{
		cACTOR	*lpActor	=	&m_aActor[m_aSelectedActor[i]];

		lpActor->m_wLinkMonsterCount	=	0;
		lpActor->m_bf1IsLeader			=	FALSE;
		lpActor->m_isWard				=	FALSE;

		removeThisActorAtLinkMonsterList(m_aSelectedActor[i]);
	}

	g_lpActmap->LostVirgin();
		
}	//	cACTOR_SET::removeSelectedMonstersLink()

//
// _iIndex를 링크하고 있는 것-_-들의 리스트에서 _iIndex를 제거
void
cACTOR_SET::removeThisActorAtLinkMonsterList(int _iIndex)
{
	int	i,iCount;

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			m_aActor[i].removeLinkMonster(_iIndex);
			iCount++;
		}
	}
}	//	cACTOR_SET::removeThisActorAtLinkMonsterList(int _iIndex)

//
//	_iCharacter인 액터의 수 리턴
int
cACTOR_SET::getCharacterCount(int _iCharacter)
{
	int	i,iCount,iCharacterCount	=	0;

	for (i=0,iCount=0;i<dMAX_ACTOR_COUNT && iCount<m_iCount;i++)
	{
		if (m_aActor[i].m_iSerial	!=	0xffff)
		{
			if (m_aActor[i].m_wCharacter	==	_iCharacter)	iCharacterCount++;

			iCount++;
		}
	}

	return	iCharacterCount;
}	//	cACTOR_SET::getCharacterCount(int _iCharacter)

//
//	스피커들 중에 _iIndex번째 녀석을 찾는다.
cACTOR*
cACTOR_SET::getSpeaker(int _iIndex)
{
	int	i,iCount=0;

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR *lpActor	=	getActor(i);

		if (!lpActor						)	continue;
		if (lpActor->m_pCC==NULL	)	continue;

		if(_iIndex	==	iCount)	return	lpActor;
		iCount++;
	}

	return	NULL;
}	//	cACTOR_SET::getSpeaker(int _iIndex)

//
//	스피커들 중에 _iIndex번째 녀석을 찾는다.
int
cACTOR_SET::getSpeakerIndex(int _iIndex)
{
	int	i,iCount=0;

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR *lpActor	=	getActor(i);

		if	(!lpActor						)	continue;
		if	(lpActor->m_pCC==NULL	)	continue;
		if	(i	==	_iIndex					)	return	iCount;

		iCount++;
	}

	return	0xffff;
}	//	cACTOR_SET::getSpeakerIndex(int _iIndex)

//
//
BOOL
cACTOR_SET::moveSelectActor(int _iDx,int _iDy)
{
	int	i,iCount=0;

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR *lpActor	=	getActor(i);

		if	(!lpActor				)	continue;
		if	(!lpActor->m_isSelected	)	continue;

		lpActor->m_pos.x	+=	_iDx;
		lpActor->m_pos.y	+=	_iDy;

		iCount++;
	}

	if (iCount)	
	{
		g_lpActmap->m_isVIRGIN	=	FALSE;
		return	TRUE;
	}

	return	FALSE;
}	//	cACTOR_SET::moveSelectActor(int _iDx,int _iDy)
char* GetCommaIndex(int index)
{
	char str[128];
	sprintf(str, "%d", index);

    char *pstr = new char[128];
    char *rpstr;

    int i = 0, k = 0, Index = 0;
    while(str[i] != NULL)   {
        if(isdigit(str[i]))
			pstr[Index++] = str[i];
        
        i++;
    }
 
    pstr[Index] = NULL;
    unsigned int num = atoi(pstr);
 
    if(num / 1000 >= 1)  {
        int ci = strlen(pstr) / 3;
        if(strlen(pstr) % 3 == 0)
        ci -= 1;

        int ti = strlen(pstr) + ci;
        rpstr = new char[ti + 1];
        rpstr[ti] = NULL;

        for(i = ti - 1, Index = 0, k = strlen(pstr)-1; i >= 0; i--)
		{
            if(Index == 3) {
                rpstr[i] = ',';
                Index = 0;
            }
            else {
                rpstr[i] = pstr[k];
                k--;
                Index++;
            }
        }
        return rpstr;
    }
    else
        return pstr;
}


void
cACTOR_SET::drawMonsterAreaInfo(int _index, cRECT *_rect)
{
	int i,cnt = 0;
	int iAllHP = 0, iAllDEF = 0, iAllRegenCycle = 0, iAllSkillDamage = 0, itotExp = 0;
	int iAllFire = 0, iAllWater = 0, iAllWind = 0, iAllLight = 0, iAllEarth = 0, iAllDark = 0;

	int x = _rect->x1 + 5;
	int y = _rect->y1 + 42;
		
	for (i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR *lpActor	=	getActor(i);
		
		if	(!lpActor							)	continue;
		if	(lpActor->m_wMoveArea != _index		)	continue;
		if	( lpActor->m_wType != eNPC_MONSTER	)	continue;

		cnt++;
		
		cJOB		*lpJob			= lpActor->getJob();
		cCHARACTER	*lpCharacter	= lpActor->getCharacter();
		
		LONGLONG	llMaxHP			=	lpJob->m_iMaxHP;
		int			iLevel	=	(lpCharacter->m_wMinLevel+lpCharacter->m_wMaxLevel)/2;
		int			iConstitution	=	(int)lpJob->m_sConstitution*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		llMaxHP	=	lpJob->m_iMaxHP+iConstitution*lpJob->m_wCorrectMaxHPConstitutionFactor*10+iLevel*lpJob->m_wCorrectMaxHPFactor*10;
		llMaxHP	=	llMaxHP*lpCharacter->m_wCorrectHP/100;
		int iHP =	(int)llMaxHP/100;
		
		iAllHP += iHP;

		double E59	= iLevel;
		double E65	= lpJob->m_sDefensivePower;
		double E66	= lpJob->m_wDefensePowerLevelFactor * 0.01;
		double E67	= lpJob->m_sConstitution;
		double E68	= lpJob->m_wBasicPowerLevelFactor * 0.001;
		
		double result = ((E65*100)+(E59-1)*(E66*100))*(100+(E67*((E59-1)*(E68*1000)+100000)/100000))/100/100;

		iAllDEF += (int)result;
		iAllRegenCycle += lpActor->m_dwRegenerationCycle;

		cABILITY	*lpAbility	=	(cABILITY*)&lpJob->m_aAbility[0];
		cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

		double C72	= lpSkill->m_sAttackPercentage;
		double C73	= lpSkill->m_sAttackPercentagePerLevel;
		double C75	= lpJob->m_aLevelForAddSkillLevel[0] * 0.01;
		double C74	= (int)((10000+C72+(C73*(E59/C75)))/100);

		double C65	= lpJob->m_sMinDamage;
		double C66	= lpJob->m_wMinDamageLevelFactor * 0.01;
		double C69	= lpJob->m_sStrength;
		double C70	= lpJob->m_wBasicPowerLevelFactor * 0.001;
		double C67	= lpJob->m_sMaxDamage;
		double C68	= lpJob->m_wMaxDamageLevelFactor * 0.01;
		
		double H61	=((((C65*100)+(E59-1)*(C66*100))+((C65*100)+(E59-1)*(C66*100))*C69*((E59-1)*(C70*1000)+100000)/100000/200)/100)*C74/100;
		double H62	=((int((C67*100)+(E59-1)*(C68*100))+((C67*100)+(E59-1)*(C68*100))*C69*((E59-1)*(C70*1000)+100000)/100000/200)/100)*C74/100;

		int damage = (int)((H61 + H62) / 2);
		iAllSkillDamage += damage;
				
		LONGLONG lExp	= lpJob->m_dwDeathExp;

		lExp	*=	(iLevel + 4);
		lExp	*=	10;
		lExp	=	lExp * lpCharacter->m_wCorrectExp/100;

		int iExp = (int)lExp/100;

		iExp = GetCorrectExpByLevelGap(iExp,iLevel);

		itotExp += iExp;
		
		iAllFire	+= lpJob->m_sFireResistance;
		iAllWater	+= lpJob->m_sWaterResistance;
		iAllWind	+= lpJob->m_sWindResistance;
		iAllLight	+= lpJob->m_sLightResistance;
		iAllEarth	+= lpJob->m_sEarthResistance;
		iAllDark	+= lpJob->m_sDarkResistance;
	}

	// 09.12.18
	cTEXT::Put(x,y		,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("영역 크기 : %d x %d",(_rect->x2 - _rect->x1), (_rect->y2 - _rect->y1)));	
	//cTEXT::Put(x,y		,LTYELLOW, _ms("영역 크기 : %d x %d",(_rect->x2 - _rect->x1), (_rect->y2 - _rect->y1)));	
	
	if( cnt != 0)
	{
		iAllRegenCycle = iAllRegenCycle / cnt;
		
		cTEXT::Put(x,y+13*1 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("H P       : %s / %s",GetCommaIndex(iAllHP),GetCommaIndex(iAllHP/cnt)));
		cTEXT::Put(x,y+13*2 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("방어력    : %s"		,GetCommaIndex(iAllDEF/cnt)));
		cTEXT::Put(x,y+13*3 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("스킬공격력: %s"		,GetCommaIndex(iAllSkillDamage/cnt)));
		cTEXT::Put(x,y+13*4 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("리젠 주기 : %d시간 %02d분 %02d초", iAllRegenCycle/60/60, (iAllRegenCycle/60)%60, iAllRegenCycle%60));
		cTEXT::Put(x,y+13*5 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("총 경험치 : %s"		,GetCommaIndex(itotExp)));
		cTEXT::Put(x,y+13*7 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("불 꽃     : %d %%"	,iAllFire/cnt));
		cTEXT::Put(x,y+13*8 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("물        : %d %%"	,iAllWater/cnt));
		cTEXT::Put(x,y+13*9 ,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("바 람     : %d %%"	,iAllWind/cnt));
		cTEXT::Put(x,y+13*10,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("빛        : %d %%"	,iAllLight/cnt));
		cTEXT::Put(x,y+13*11,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("대 지     : %d %%"	,iAllEarth/cnt));
		cTEXT::Put(x,y+13*12,cSET_FONT::s_OldColorOption[eCOLOR_MONSTER_INFO], _ms("어둠      : %d %%"	,iAllDark/cnt));

	//	cTEXT::Put(x,y+13*1 ,LTYELLOW, _ms("H P       : %s / %s",GetCommaIndex(iAllHP),GetCommaIndex(iAllHP/cnt)));
	//	cTEXT::Put(x,y+13*2 ,LTYELLOW, _ms("방어력    : %s"		,GetCommaIndex(iAllDEF/cnt)));
	//	cTEXT::Put(x,y+13*3 ,LTYELLOW, _ms("스킬공격력: %s"		,GetCommaIndex(iAllSkillDamage/cnt)));
	//	cTEXT::Put(x,y+13*4 ,LTYELLOW, _ms("리젠 주기 : %d시간 %02d분 %02d초", iAllRegenCycle/60/60, (iAllRegenCycle/60)%60, iAllRegenCycle%60));
	//	cTEXT::Put(x,y+13*5 ,LTYELLOW, _ms("총 경험치 : %s"		,GetCommaIndex(itotExp)));
	//	cTEXT::Put(x,y+13*7 ,LTYELLOW, _ms("불 꽃     : %d %%"	,iAllFire/cnt));
	//	cTEXT::Put(x,y+13*8 ,LTYELLOW, _ms("물        : %d %%"	,iAllWater/cnt));
	//	cTEXT::Put(x,y+13*9 ,LTYELLOW, _ms("바 람     : %d %%"	,iAllWind/cnt));
	//	cTEXT::Put(x,y+13*10,LTYELLOW, _ms("빛        : %d %%"	,iAllLight/cnt));
	//	cTEXT::Put(x,y+13*11,LTYELLOW, _ms("대 지     : %d %%"	,iAllEarth/cnt));
	//	cTEXT::Put(x,y+13*12,LTYELLOW, _ms("어둠      : %d %%"	,iAllDark/cnt));
	}
}
