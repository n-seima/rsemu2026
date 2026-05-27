#include "book_MonsterDictionary.h"
#include "book.h"
#include "cACTOR.h"
#include "CImageData.H"
#include "CMessage.H"

cBookMonDic	g_gwBookMonDic;

//모션 이넘문.
enum{
	eMOTION_WALK,			//걷기
	eMOTION_RUN,			//뛰기.
	eMOTION_STOP,			//멈춤.
	eMOTION_HITBYOTHER,		//맞는 모션.
	eMOTION_TRANSFORM1,		//변신(사용 안함)
	eMOTION_TRANSFORM2,		//변신(사용 안함)
	eMOTION_DEAD,			//죽음
	eMOTION_SITDOWN,		//앉기(사용 안함)
	eMOTION_ATTACK1,		//공격1.
	eMOTION_ATTACK2,		//공격(사용 안함)
	eMOTION_END				
};

#define dINDEX_BOARDER													130	//인덱스에서 왼쪽 항과 오른쪽 항의 넓이.

//스크립트 TXT 디파인. ㅎㅎㅎㅎ
#define dMONSTERDICMSG_MONSTERDIC										g_pstrMonsterDicMessage[0]
#define dMONSTERDICMSG_NAME												g_pstrMonsterDicMessage[1]
#define dMONSTERDICMSG_TYPE												g_pstrMonsterDicMessage[2]
#define dMONSTERDICMSG_REGENERATIONTIME									g_pstrMonsterDicMessage[3]
#define dMONSTERDICMSG_EXP												g_pstrMonsterDicMessage[4]
#define dMONSTERDICMSG_MOVESPEED										g_pstrMonsterDicMessage[5]
#define dMONSTERDICMSG_ATTACKSPEED										g_pstrMonsterDicMessage[6]

#define dMONSTERDICMSG_WALK												g_pstrMonsterDicMessage[7]
#define dMONSTERDICMSG_RUN												g_pstrMonsterDicMessage[8]
#define dMONSTERDICMSG_STOP												g_pstrMonsterDicMessage[9]
#define dMONSTERDICMSG_HITBYOTHER										g_pstrMonsterDicMessage[10]
#define dMONSTERDICMSG_ATTACK											g_pstrMonsterDicMessage[11]
#define dMONSTERDICMSG_DEATH											g_pstrMonsterDicMessage[12]

#define dMONSTERDICMSG_TURNLEFT											g_pstrMonsterDicMessage[13]
#define dMONSTERDICMSG_TURNRIGHT										g_pstrMonsterDicMessage[14]

#define dMONSTERDICMSG_STRENGTH											g_pstrMonsterDicMessage[15]//힘
#define dMONSTERDICMSG_AGILITY											g_pstrMonsterDicMessage[16]//민첩
#define dMONSTERDICMSG_CONSTITUTION										g_pstrMonsterDicMessage[17]//건강
#define dMONSTERDICMSG_WISDOM											g_pstrMonsterDicMessage[18]//지혜
#define dMONSTERDICMSG_INTELLIGENCE										g_pstrMonsterDicMessage[19]//지능
#define dMONSTERDICMSG_CHARISMA											g_pstrMonsterDicMessage[20]//카리스마
#define dMONSTERDICMSG_LUCK												g_pstrMonsterDicMessage[21]//운
#define dMONSTERDICMSG_SIGHT											g_pstrMonsterDicMessage[22]//시야
#define dMONSTERDICMSG_MINDAMAGE										g_pstrMonsterDicMessage[23]//최소 공격력
#define dMONSTERDICMSG_MAXDAMAGE										g_pstrMonsterDicMessage[24]//최대 공격력
#define dMONSTERDICMSG_DEFENSIVEPOWER									g_pstrMonsterDicMessage[25]//방어력

//출력
void
cBookMonDic::draw()
{
	g_sprBook.Put(0,0,eBI_BOOK);
	
	if	(m_iWhere	==	eBW_INDEX)
	{
		drawIndex();
	}
	else
	{
		drawPage();
		m_bmButton.draw();
		m_bmMotionButton.draw();
	}

	m_button.draw();
}


//초기화
void 
cBookMonDic::reset()
{
	m_wDirect = dDIRECT_S ;			//아래를 보게 초기화.
	m_wFrameCounter = 0;			//프레임 카운터랑 프레임을 다른 클래스와 다르게 썻으니 주의 하세요.
	m_wFrame = 0 ;
	m_wMotion = eMOTION_STOP;
	m_bOpen = FALSE;
	m_bIsInit = FALSE;
	m_iCount =0;

	for(int i = 0 ; i < dMAX_MONSTER_KIND ; i++)
	{
		m_pastrName[i] = NULL;
		m_paJobInfo[i] = NULL;
		m_iaRegenerationCycle[i] = NULL;
		m_waActorKind[i] = NULL;
		m_waMoveSpeed[i] = NULL;
		m_waAttackSpeed[i] = NULL;
		m_dwaExp[i] = NULL;
	}
}


//책을 열었을때..
void
cBookMonDic::open()
{
	if (m_bOpen == FALSE)			
		m_bOpen = TRUE;
	else
		return;						//열려있으면 리턴.

	if (g_map.m_bf4FieldType !=	eFT_BATTLE_FIELD)		//전투필드가 아니면.
	{
		reset();										//초기화 해주고.
		return;											//리턴.
	}

	if	(m_bIsInit	==	FALSE)
	{
		m_button.init(&g_sprBook,6);							//책의 모서리등의 메뉴 초기화
		m_bmButton.init(&g_sprInterface,2);						//방향 전환 버턴.
		m_bmMotionButton.init(&g_sprInterface,eMOTION_END);		//에니메이션 전환 버턴.
	}

	m_wFrameCounter = 0;
	m_wFrame = 0;
	m_wMotion = eMOTION_STOP;
	m_wDirect = dDIRECT_S;
	
	int	iUpRightButtonWidth		=	g_sprBook.getSpriteWidth(eBI_EXIT_NORMAL);
	int	iBottomRightButtonWidth	=	g_sprBook.getSpriteWidth(eBI_NEXT_PAGE_NORMAL);
	int	iBottomButtonHeight		=	g_sprBook.getSpriteHeight(eBI_NEXT_PAGE_NORMAL)+20;
	
	m_button.addImageBar("",eBM_INDEX			,0	,20	,eBI_INDEX_NORMAL		,eBI_INDEX_LIGHT	,eBI_INDEX_LIGHT);
	m_button.addImageBar("",eBM_EXIT			,g_iScreenWidth-iUpRightButtonWidth	,20	,eBI_EXIT_NORMAL		,eBI_EXIT_LIGHT		,eBI_EXIT_LIGHT);
	m_button.addImageBar("",eBM_PREVIOUS_PAGE	,0	,g_iScreenHeight-iBottomButtonHeight,eBI_PRE_PAGE_NORMAL	,eBI_PRE_PAGE_LIGHT	,eBI_PRE_PAGE_LIGHT);
	m_button.addImageBar("",eBM_NEXT_PAGE		,g_iScreenWidth-iBottomRightButtonWidth,g_iScreenHeight-iBottomButtonHeight,eBI_NEXT_PAGE_NORMAL	,eBI_NEXT_PAGE_LIGHT,eBI_NEXT_PAGE_LIGHT);

	for(int i = 0; i <= eBM_NEXT_PAGE ; i++)
		m_button.setClickMargin(eBM_INDEX + i,1,1);

	m_bmButton.addImageBar(dMONSTERDICMSG_TURNLEFT, 0, 100 , 500 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);
	m_bmButton.addImageBar(dMONSTERDICMSG_TURNRIGHT, 1, 200 , 500 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);
	m_bmButton.setClickMargin(0,1,1);
	m_bmButton.setClickMargin(1,1,1);
	m_bmButton.setTextColor(0 , BLACK , FALSE);
	m_bmButton.setTextColor(1 , BLACK , FALSE);

	//JBC
	m_bmMotionButton.addImageBar(dMONSTERDICMSG_WALK, eMOTION_WALK				, 50 , 400 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);
	m_bmMotionButton.addImageBar(dMONSTERDICMSG_RUN, eMOTION_RUN				, 120 , 400 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);
	m_bmMotionButton.addImageBar(dMONSTERDICMSG_STOP, eMOTION_STOP				, 190 , 400 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);
	m_bmMotionButton.addImageBar(dMONSTERDICMSG_HITBYOTHER,	 eMOTION_HITBYOTHER	, 260 , 400 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);
	m_bmMotionButton.addImageBar(dMONSTERDICMSG_ATTACK, eMOTION_ATTACK1			, 50 , 430 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);
	m_bmMotionButton.addImageBar(dMONSTERDICMSG_DEATH, eMOTION_DEAD				, 120 , 430 ,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff , eBUTTON_DISABLE);	
	m_bmMotionButton.setClickMargin(eMOTION_WALK,1,1);
	m_bmMotionButton.setClickMargin(eMOTION_RUN,1,1);
	m_bmMotionButton.setClickMargin(eMOTION_STOP,1,1);
	m_bmMotionButton.setClickMargin(eMOTION_HITBYOTHER,1,1);
	m_bmMotionButton.setClickMargin(eMOTION_ATTACK1,1,1);
	m_bmMotionButton.setClickMargin(eMOTION_DEAD,1,1);
	m_bmMotionButton.setTextColor(eMOTION_WALK , BLACK , FALSE);
	m_bmMotionButton.setTextColor(eMOTION_RUN , BLACK , FALSE);
	m_bmMotionButton.setTextColor(eMOTION_STOP , BLACK , FALSE);
	m_bmMotionButton.setTextColor(eMOTION_HITBYOTHER , BLACK , FALSE);
	m_bmMotionButton.setTextColor(eMOTION_ATTACK1 , BLACK , FALSE);
	m_bmMotionButton.setTextColor(eMOTION_DEAD , BLACK , FALSE);

	ClosePopupWindow();						//열려있는 팝업 윈도우가 있으면 닫아준다.
	
	SerchMonsterInMap();					//맵에있는 몬스터를 검색한다.

	s_iPopupInterface		=	ePIW_BOOK_MONSTER_DICTIONARY;
	s_bIsFullScreenInterface=	TRUE;
	m_iWhere				=	eBW_INDEX;
	m_iCurrentPage			=	0;

	m_bIsInit = TRUE;
}

//책이 열려 있냐??
BOOL
cBookMonDic::isPopupMenuBook()
{
	if	(s_iPopupInterface	==	ePIW_BOOK_MONSTER_DICTIONARY)
		return TRUE;

	return FALSE;
}


//책이 오픈돼 있냐??
BOOL
cBookMonDic::isOpened()
{
	if	(s_iPopupInterface	==	ePIW_BOOK_MONSTER_DICTIONARY)
		return	m_bOpen;
	
	return	m_bOpen;
}


//맵에있는 몬스터를 검색..
void
cBookMonDic::SerchMonsterInMap()
{
	if(m_bIsInit == TRUE)
		return;

	for (int i =0 ;  i < 2048 ; i++)
	{

		if(g_map.m_aSaveActor[i].m_iSerial == -1)			//시리얼값이 세팅돼지 않았으면.
			continue;

		if(g_map.m_aSaveActor[i].m_wActorKind != eNPC_MONSTER)			//몬스터가 아니면.
			continue;
		
		if(isExistInStrArry(m_pastrName , g_map.m_aSaveActor[i].m_strName , m_iCount))	//이미 이름이 검색된 녀석이면.
			continue;

		m_pastrName[m_iCount] = g_map.m_aSaveActor[i].m_strName;		//이름 저장
		m_paJobInfo[m_iCount] = g_jm.getJob(g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]+dJOB_BORDER);	//잡정보 저장.
		m_iaRegenerationCycle[m_iCount] = g_map.m_aSaveActor[i].m_iRegenerationCycle;	//리젠시간
		m_waActorKind[m_iCount] = m_paJobInfo[m_iCount]->m_wKind;			//종류(인간 언데 등등.)

		m_waMoveSpeed[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_wMoveSpeed;		//이속.
		m_waAttackSpeed[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_wAttackSpeed;	//공속.
		m_dwaExp[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_dwDeathExp;			//경험치.

		m_saStrength[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sStrength;
		m_saAgility[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sAgility;
		m_saConstitution[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sConstitution;
		m_saWisdom[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sWisdom;
		m_saIntelligence[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sIntelligence;
		m_saCharisma[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sCharisma;
		m_saLuck[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sLuck;
		m_saSight[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sSight;

		m_saMinDamage[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sMinDamage;						//	최소 데미지
		m_saMaxDamage[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sMaxDamage;						//	최대 데미지
		m_saDefensivePower[m_iCount] = m_pJob[g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]].m_sDefensivePower;					//	방어력

		m_iCount++;			//유효 범위 증가.

		if(m_iCount >= dMAX_MONSTER_KIND)			//유효범위가 최대 배열을 초과하면 더이상 작업하지 않는다...
			return;
	}
}


//페이지를 그림.
void
cBookMonDic::drawPage()
{
	//완정 중요 포인트.... ㅠ.ㅠ; 이거찾느라 3일간 뻘짓.... ㅠ.ㅠ;
	//CJobInfo* lpJob = g_jm.getJob(g_jm.m_aJobList[g_map.m_aSaveActor[i].m_wCharacter]+dJOB_BORDER);

	int		iNowIndex = m_iCurrentPage / 2;						//현재쪽을 구한다.
	cANM* lpBody = GetBody(m_paJobInfo[iNowIndex]->m_iBody);	//현재쪽의 출력되는 에니메이션을 구한다.

	//이미지 로딩 안되있으면 로딩 한다.
	if	(lpBody == NULL || lpBody->m_pAnmData == NULL)
		g_im.loadIMonsterBody(m_paJobInfo[iNowIndex]->m_iBody,m_paJobInfo[iNowIndex]->m_iVariation);

	//에니메이션 관련 변수
	m_wFPS				=	lpBody->GetFPS(m_wMotion);			
	m_wMaxFrame			=	lpBody->GetFrameCount(m_wMotion);

	s_text13M.pushShadowStatus(FALSE);			//그림자 꺼주고.
	s_text13MA90.pushShadowStatus(FALSE);
	s_textBold18M.pushShadowStatus(FALSE);

	//왼쪽 페이지
	g_sprBook.Put(80,157,eBI_VALID_PAGE,100,100,dPUT_ALPHA_BLENDING,4);	//책 스프라이트 출력.
	g_sprBook.Put(210	,565,eBI_LEFT_PAGE_PANNEL);
	s_text13M.cPut(210	,565-7,WHITE,"%.3d",m_iCurrentPage);

	if(lpBody)				//몬스터 찍어주기.
	{
		lpBody->setPalette(g_aPalette[m_paJobInfo[iNowIndex]->m_iBody].m_apPalette[m_paJobInfo[iNowIndex]->m_wPalette]);
		lpBody->Put(210 , 300 , m_wMotion , m_wDirect , m_wFrame , 150 , 150);
	}

	//오른쪽 페이지
	g_sprBook.Put(465,157,eBI_VALID_PAGE,100,100,dPUT_ALPHA_BLENDING,4);
	g_sprBook.Put(595	,565,eBI_RIGHT_PAGE_PANNEL);
	s_text13M.cPut(595	,565-7,WHITE,"%.3d",m_iCurrentPage+1);			//하단의 페이지 수  출력.

	s_textBold18M.cPut(600,85,RGB(159,116,27),dMONSTERDICMSG_NAME);		//이름.
	s_textBold18M.cPut(600,120,BLACK,m_pastrName[iNowIndex]);
	
	s_text13M.cPut(500,180,RGB(159,116,27),dMONSTERDICMSG_TYPE);		//타입.
	s_text13M.cPut(700,180,BLACK, g_pstrMonsterType[m_waActorKind[iNowIndex]]);

	s_text13M.cPut(500,200,RGB(159,116,27),dMONSTERDICMSG_REGENERATIONTIME);	//리젠시간.
	s_text13M.cPut(700,200,BLACK,_ms("%d",m_iaRegenerationCycle[iNowIndex]));

	s_text13M.cPut(500,220,RGB(159,116,27),dMONSTERDICMSG_EXP);			//경험치.
	s_text13M.cPut(700,220,BLACK,_ms("%d",m_dwaExp[iNowIndex]));

	s_text13M.cPut(500,240,RGB(159,116,27),dMONSTERDICMSG_MOVESPEED);	//이속.
	s_text13M.cPut(700,240,BLACK,_ms("%d",m_waMoveSpeed[iNowIndex]));

	s_text13M.cPut(500,260,RGB(159,116,27),dMONSTERDICMSG_ATTACKSPEED);	//공속.
	s_text13M.cPut(700,260,BLACK,_ms("%d",m_waAttackSpeed[iNowIndex]));

	s_text13M.cPut(500,300,RGB(159,116,27),g_pstrMonsterDicMessage[15]);	//힘.
	s_text13M.cPut(700,300,BLACK,_ms("%d",m_saStrength[iNowIndex]));

	s_text13M.cPut(500,320,RGB(159,116,27),g_pstrMonsterDicMessage[16]);	//민첩.
	s_text13M.cPut(700,320,BLACK,_ms("%d",m_saAgility[iNowIndex]));

	s_text13M.cPut(500,340,RGB(159,116,27),g_pstrMonsterDicMessage[17]);	//건강.
	s_text13M.cPut(700,340,BLACK,_ms("%d",m_saConstitution[iNowIndex]));

	s_text13M.cPut(500,360,RGB(159,116,27),g_pstrMonsterDicMessage[18]);	//지혜.
	s_text13M.cPut(700,360,BLACK,_ms("%d",m_saWisdom[iNowIndex]));

	s_text13M.cPut(500,380,RGB(159,116,27),g_pstrMonsterDicMessage[19]);	//지능.
	s_text13M.cPut(700,380,BLACK,_ms("%d",m_saIntelligence[iNowIndex]));

	s_text13M.cPut(500,400,RGB(159,116,27),g_pstrMonsterDicMessage[20]);	//카리스마.
	s_text13M.cPut(700,400,BLACK,_ms("%d",m_saCharisma[iNowIndex]));

	s_text13M.cPut(500,420,RGB(159,116,27),g_pstrMonsterDicMessage[21]);	//운.
	s_text13M.cPut(700,420,BLACK,_ms("%d",m_saLuck[iNowIndex]));

	s_text13M.cPut(500,440,RGB(159,116,27),g_pstrMonsterDicMessage[22]);	//시야.
	s_text13M.cPut(700,440,BLACK,_ms("%d",m_saSight[iNowIndex]));

	s_text13M.cPut(500,460,RGB(159,116,27),g_pstrMonsterDicMessage[23]);	//최소 공격력.
	s_text13M.cPut(700,460,BLACK,_ms("%d",m_saMinDamage[iNowIndex]));

	s_text13M.cPut(500,480,RGB(159,116,27),g_pstrMonsterDicMessage[24]);	//최대 공격력.
	s_text13M.cPut(700,480,BLACK,_ms("%d",m_saMaxDamage[iNowIndex]));

	s_text13M.cPut(500,500,RGB(159,116,27),g_pstrMonsterDicMessage[25]);	//방어력.
	s_text13M.cPut(700,500,BLACK,_ms("%d",m_saDefensivePower[iNowIndex]));

	if	(m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;
	}

	if	(m_wFrame	>=	m_wMaxFrame)		//	한 에니메이션이 끝났다.
	{
		m_wFrame = 0;

		if(m_wMotion >= eMOTION_HITBYOTHER)
			m_wMotion = eMOTION_STOP;			//죽거나 맞거나 공격에니메이션이면 정지모션으로 돌아간다.
	}

	m_wFrameCounter += m_wFPS;				//프레임 갱신.
}


//목차를 그려 준다.
void
cBookMonDic::drawIndex()
{
	int				iX,iY;

	s_textBold18.pushShadowStatus(FALSE);		//그림자 꺼주고.
	s_textBold24M.pushShadowStatus(FALSE);
	s_textBold14M.pushShadowStatus(FALSE);

	//왼쪽 페이지
	iX	=	65;
	iY	=	220;
	
	g_sprBook.Put(36,58,eBI_COVER_FRAME);
	g_sprBook.Put(210,410,eBI_CHAPTER_DECORATION);
	
	s_textBold24M.cPut(iX,iY,294,60,0,0,dFITTEXT_CENTER,dMONSTERDICMSG_MONSTERDIC);	//"몬스터도감"글자 출력.

	//오른쪽 페이지
	g_sprBook.Put(426,58,eBI_INDEX_FRAME);
	s_textBold18.cPut(600,85,RGB(159,116,27),"INDEX");

	iX	=	480;
	iY	=	120;

	for (int i = 0 ; i < m_iCount ; i++)
	{
		if(m_iFocusIndex == i)
			s_textBold14M.put(iX,iY,dINDEX_BOARDER - 10,20,0,RED, m_pastrName[i]);			//이름 출력.
		else
			s_textBold14M.put(iX,iY,dINDEX_BOARDER - 10,20,0,0, m_pastrName[i]);

		iY += 16;

		if(i == 24)					//24번째면 우측 항으로 개행...
		{
			iX	=	480 + dINDEX_BOARDER;
			iY	=	120;
		}
	}

	s_textBold18.popShadowStatus();
	s_textBold24M.popShadowStatus();
	s_textBold14M.popShadowStatus();
}


//마우스가 잇는 인덱스를 알아낸다.
int
cBookMonDic::getFocusIndex()
{
	if	(m_iWhere	!=	eBW_INDEX)
		return	-1;

	int iX	=	480;
	int iY	=	120;
	
	for	(int i=0; i < m_iCount ;i++)
	{
		cRECT	rect;
		
		int iXPixelSize = s_textBold14M.getPixelSize(m_pastrName[i]);

		iXPixelSize = min(iXPixelSize , dINDEX_BOARDER - 10);			//오른쪽 열까지 넘어가는 텍스트 길이를 위하여 값을 잘라줌.

		rect.set(iX,iY,iX+iXPixelSize,iY+16);			
		
		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;
		
		iY	+=	16;

		if(i == 24)					//24번째면 우측 항으로 개행...
		{
			iX	=	480 + dINDEX_BOARDER;
			iY	=	120;
		}
	}
	
	return	-1;
}


//생성자. 포인터를 초기화 하자.
cBookMonDic::cBookMonDic()
{
	m_pJob = NULL;

	reset();
}


//소멸자 잊지말고 해제..
cBookMonDic::~cBookMonDic()
{
	if(m_pJob)
		delete [] m_pJob ; m_pJob = NULL;
}


//같은 이름이 배열내에 존재하는지 검사한다.
BOOL
cBookMonDic::isExistInStrArry(char **_ppstrName , char* _strSerch , int _nMaxDestArry)
{
	for (int i = 0 ; i < _nMaxDestArry ; i++)
	{
		if(_ppstrName[i] == NULL)					//비어있는 칸이면.
			continue;

		if (strcmp(_ppstrName[i], _strSerch) == 0)		//같으면 TRUE를 리턴.
			return TRUE;
	}

	return FALSE;
}


//닫을때.
void
cBookMonDic::close()
{
	s_bIsFullScreenInterface=	FALSE;
	s_iPopupInterface		=	ePIW_NONE;
	m_bOpen					=	FALSE;
}





//업데이트.
void
cBookMonDic::update()
{
	if(!isPopupMenuBook())
		return;
	
	m_iFocusIndex	=	getFocusIndex();
	
	if	(m_iFocusIndex	!=	-1	&&	s_isClickedLeftButton)
	{	
		m_iCurrentPage		=	m_iFocusIndex * 2;
		m_iWhere			=	eBW_CONTENTS;
	}
	
	DWORD	dwMenu	=	m_button.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);
	
	switch(dwMenu)
	{
	case	eBM_INDEX			:
		m_iWhere	=	eBW_INDEX;
		break;
	case	eBM_EXIT			:
		close();
		break;
	case	eBM_PREVIOUS_PAGE	:
		if	(m_iCurrentPage	>	0)
			m_iCurrentPage-=2;
		else
			m_iWhere		=	eBW_INDEX;
		break;
	case	eBM_NEXT_PAGE		:
		if	(m_iWhere	==	eBW_INDEX)
		{
			m_iCurrentPage	=	0;
			m_iWhere		=	eBW_CONTENTS;
			break;
		}
		if	(m_iCurrentPage	<	getTotalPage()-2)
			m_iCurrentPage	+=	2;
		break;
	}

	updateButton();
}


//바 메뉴 업데이트..
void
cBookMonDic::updateButton()
{
	DWORD	dwMenu	=	m_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
	case 0: 
		if(m_wDirect == dDIRECT_NW)
		{
			m_wDirect = dDIRECT_N;
			break;
		}
		m_wDirect++;
		break;
	case 1:
		if(m_wDirect == dDIRECT_N)
		{
			m_wDirect = dDIRECT_NW;
			break;
		}
		m_wDirect--;
		break;
	}

	DWORD dwMotionMenu	= m_bmMotionButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if(dwMotionMenu < eMOTION_END && dwMotionMenu >= eMOTION_WALK)
	{
		m_wMotion = dwMotionMenu;
		m_wFrameCounter = 0;
		m_wFrame = 0;
		m_wMaxFrame = 0;
	}
}


//데이터를 세팅해준다. 동적할당과 함께...
void 
cBookMonDic::setData(cJOB *_jobData , int _ipJobCount)
{
	if(m_pJob == NULL)
	{
		m_pJob = new cJOB [_ipJobCount];
		m_ipJobCount = 0;
	}
	
	memcpy(&m_pJob[m_ipJobCount] , _jobData , sizeof(cJOB));

	m_ipJobCount++;
}
