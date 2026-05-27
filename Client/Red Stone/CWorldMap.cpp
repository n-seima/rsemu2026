#include "CWindowInterface.h"
#include "CHero.h"
#include "CParty.h"
#include "cMAP.h"
#include "CMessage.h"

/************************************************************************/
// 경과:
//	-버튼 콘트롤 구현되어있음. 그러나, 파티창밖에 제작이 안되서 숨겼음
//  -따라서 몇몇 부분은 강제적으로 고정시켰음.
//  -메모리 할당& 그림 로드 부분에서 문제가 있음. 팀장님과 상의 요망
//  -채팅창 축소 기능은 다음에.
/************************************************************************/

CWorldMap		g_gwWorldMap;
CTextDC			*l_lpTempText;
const	int		c_dungenListFontSize	=	13;

//
//	Minpmap Display Icon Image
enum
{
	eMDII_WEAPON_SHOP	=	35,
};

//	g_sprWorldMap	월드맵 관련 스프라이트 data/interface/game/worldMap.sd	
//	m_bmRadioButton		버튼 메뉴
BOOL
CWorldMap::open(BOOL _bIsForGateGlove,int _iItemSlotIndex)
{
	if	(!m_info.readData())
		return	FALSE;

	cINPDEV::Reset();

	m_iUseItemSlotIndex	=	_iItemSlotIndex;
	m_bIsForGateGlove	=	_bIsForGateGlove;

	m_rectInfoFrame.set(5,450,750,595);

	if (g_iScreenWidth != 800)
	{
		int iRate = (int)(g_iScreenWidth*100)/800;
		int iAddY = g_iScreenHeight-600;
		
		m_rectInfoFrame.set(5, 450+iAddY, g_iScreenWidth-((50*iRate)/100), 595+iAddY);
	}

	m_info.setCurrentField(g_hero.m_wCurrentField);

	// 게임 초기화
	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_iPopupInterface		=	ePIW_WORLD_MAP;
	m_iWorldmapMenu			= 0xffff;
	m_iSelectPartyMember	=	-1;
	m_iSelectQuest			=	0;

	m_bmRadioButton.init(&g_sprWorldMap,20,dBAR_RADIO);

	CDecoder	decoder;

	if (!decoder.Upload("data/interface/game/wm_config.csv"))
		return	FALSE;

	s_bIsFullScreenInterface	=	TRUE;

	int	iMenu	=	0,i;

	for (i=0;i<9;i++)
		decoder.getCSVWord();
	
	while(1)
	{
		BOOL	bIsEnd		=	FALSE;
		CPos	posRefit;

		int		aiList[10];
		int		iX			=	decoder.getCSVNumber(&bIsEnd);
		int		iY			=	decoder.getCSVNumber(&bIsEnd);
		int		iNormal		=	decoder.getCSVNumber(&bIsEnd);
		int		iActive		=	decoder.getCSVNumber(&bIsEnd);
		int		iSelected	=	decoder.getCSVNumber(&bIsEnd);
		int		iChecked	=	decoder.getCSVNumber(&bIsEnd);
		int		iDisable	=	decoder.getCSVNumber(&bIsEnd);
		int		iCount		=	decoder.getCSVNumberList(aiList,&bIsEnd);

		posRefit.x			=	aiList[0];
		posRefit.y			=	aiList[1];

		decoder.getCSVWord(&bIsEnd);

		if	(bIsEnd	==	TRUE)
			break;

		if	(iMenu	<	3	||	iMenu	>	5)
		{
			int iAddX = 0;
			int iAddY = 0;
			if (g_iScreenWidth != 800)
			{
				iAddX = g_iScreenWidth-800;
				iAddY = g_iScreenHeight-600;
			}					

			if (g_iScreenWidth != 800 && iMenu == 6)
			{
				m_bmRadioButton.addImageBar(g_pstrCommonMessage[dWM_MENU_MESSAGE_BEGIN+iMenu],eWMM_FIELD_INFO+iMenu,iX+iAddX,iY+iAddY,iNormal,iActive,iSelected,iChecked,iDisable);
				m_bmRadioButton.correctTextPos(eWMM_FIELD_INFO+iMenu,posRefit.x,posRefit.y);
				continue;
			}
			m_bmRadioButton.addImageBar(g_pstrCommonMessage[dWM_MENU_MESSAGE_BEGIN+iMenu],eWMM_FIELD_INFO+iMenu,iX+iAddX,iY,iNormal,iActive,iSelected,iChecked,iDisable);
			m_bmRadioButton.correctTextPos(eWMM_FIELD_INFO+iMenu,posRefit.x,posRefit.y);
		}

		iMenu++;
	}

	if	(m_bIsForGateGlove)
	{
		m_bmRadioButton.hide(eWMM_PARTY_INFO);
		m_bmRadioButton.hide(eWMM_QUEST_INFO);
		// insu add
		m_iWorldmapMenu = eWMM_FIELD_INFO;
		// insu add end
	}

	m_bDetailView = FALSE;
	m_iDetailFocusQuest = 0xffff;

	m_bmRadioButton.check(eWMM_FIELD_INFO);

	setQuestField();

	return	TRUE;
}

int
CWorldMap::getInfoSurfaceFieldIndex(int _iFieldindex)
{
	int iIndex = g_gwWorldMap.m_info.getSurfaceField(_iFieldindex);
	
	if (iIndex	==	-1)
	{
		char	strFileName[64];
		
		strncpy(strFileName,g_map.m_strFileName+5,3);
		
		strFileName[3]		=	NULL;
		
		for (int i=0; i<g_gwWorldMap.m_info.m_iFieldInfoCount;i++)
			if (STRICMP(g_gwWorldMap.m_info.m_aFieldList[i].m_strFileHeader,strFileName)==0)
				iIndex	=	i;
	}


	return iIndex;
}


// insu function add
BOOL
CWorldMap::setInforeadData()
{
	return m_info.readData();
}
// insu function add end


// insu function add
void
CWorldMap::setInfoCurrentField(WORD _wField)
{
	m_info.setCurrentField(_wField);
}
// insu function add end

// insu function add
int 
CWorldMap::getInfoCurrentFieldIndex()
{
	return m_info.m_iCurrentFieldIndex;
}
// insu function add end


// insu function add
void
CWorldMap::getInfoShortestNode(int _iBegin,int _iEnd,cWM_road *_lpRoad,cWM_road *_lpResultRoad)
{
	m_info.getShortestNode(_iBegin, _iEnd, _lpRoad, _lpResultRoad);
}
// insu function add end


// insu function add
// 해당 던젼의 인덱스를 포함하고 있는 부모필드는 찾는다.
int
CWorldMap::getParentFieldIndex(int _iIndex)
{
	for(int i=0 ; i<dMAX_MAIN_FIELD_COUNT ; i++)
	{
		for(int j=0 ; j<g_gwWorldMap.m_info.m_aFieldList[i].m_wLinkDungeonCount ; j++)
		{
			if (g_gwWorldMap.m_info.m_aFieldList[i].m_awLinkDungeon[j] == _iIndex)
				return g_gwWorldMap.m_info.m_aFieldList[i].m_wFieldSerial;
		}
	}
	return 0xffff;
}
// insu function add end
// insu function add
void
CWorldMap::UpdateTotalInfo()
{
	m_strDungeonInfo.Reset();
	for(int i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
	{
		m_MiniQuestInfo.m_strQuestInfo[i].Reset();
		m_MiniQuestInfo.m_strQuestName[i].Reset();
	}

	if (m_info.m_iFocusFieldIndex == -1)
		return;
	
	cWM_mainFieldInfo	*lpInfo	=	&m_info.m_aFieldList[m_info.m_iFocusFieldIndex];

	// 필드이름
	if (g_aMapSimpleInfo[lpInfo->m_wFieldSerial].m_bf8Type == eFT_VILLAGE)
		wsprintf(m_MiniQuestInfo.m_strName, "%s", g_aMapSimpleInfo[lpInfo->m_wFieldSerial].m_strName);
	else
		wsprintf(m_MiniQuestInfo.m_strName, "%s\n<c:WHITE>(Lv<c:LTGREEN> %d~%d<c:WHITE>)<n>", g_aMapSimpleInfo[lpInfo->m_wFieldSerial].m_strName,lpInfo->m_wLowLevel,lpInfo->m_wTopLevel);

	// 필드이름의 높이를 구한다.
	m_MiniQuestInfo.m_iFieldNameHeight = CGame::s_textBold13.getTextHeight(dWM_BASIC_WINDOW_WIDTH-10, m_MiniQuestInfo.m_strName);

	cSTRING	QuestInfoString;	// 임시로 필요한 string

	m_posPut.Set(lpInfo->m_rectFrame.x2, lpInfo->m_rectFrame.y1);
	
	
	m_strDungeonInfo.Add("<c:LTGREEN>%s<n>\n", dMSG_WM_FIELD_INFO);
	// 연결된 던전 레벨정보 
	for (i=0;i<lpInfo->m_wLinkDungeonCount;i++)
	{
		int				iLinkDungeon	=	lpInfo->m_awLinkDungeon[i];

		if (m_info.m_aDungeonList[iLinkDungeon].m_wIsNew == 0xffff)
		{
			m_strDungeonInfo.Add("%s\n", dMSG_WM_VALID_LINKED_DUNGEON);
			continue;
		}

		cWM_dungeonInfo	*lpDungenInfo	=	NULL;
		char			strDungeonName[64];
		
		if (iLinkDungeon	!=	0xffff)
			lpDungenInfo	=	&m_info.m_aDungeonList[iLinkDungeon];

		if (lpDungenInfo	==	NULL)
		{			
			m_strDungeonInfo.Add("%s\n", dMSG_WM_VALID_LINKED_DUNGEON);
			break;
		}
		if(lpDungenInfo->m_wDungeonCount)
		{
			int		iDungeonSerial	=	lpDungenInfo->m_awDungeonList[0];
			strcpy(strDungeonName,g_aMapSimpleInfo[iDungeonSerial].m_strName);

			int		iNameLength		=	strlen(strDungeonName);

			if (strDungeonName[iNameLength-2]	==	'B')
				strDungeonName[iNameLength-2]	=	NULL;
			if (strDungeonName[iNameLength-3]	==	'B')
				strDungeonName[iNameLength-3]	=	NULL;
			if (strDungeonName[iNameLength-1]	==	'F')
			{
				if (strDungeonName[iNameLength-3]	>=	'0'	&&	strDungeonName[iNameLength-3]	<=	'9'	)
					strDungeonName[iNameLength-3]	=	NULL;
				else
					strDungeonName[iNameLength-2]	=	NULL;
			}
			m_strDungeonInfo.Add("%s (Lv <c:LTGREEN>%d<n>~<c:LTGREEN>%d<n>)\n", strDungeonName, lpDungenInfo->m_wLowLevel, lpDungenInfo->m_wHighLevel);
		}
	}// 연결된 던전 레벨정보 끝
	
	m_strDungeonInfo.Add("\n<c:LTGREEN>%s<n>\n", dMSG_WM_QUEST_INFO);

	// 퀘스트 관련 정보를 얻어온다.	
	CQuest	*lpQuestManager	=	NULL;
	
	int iQLinkFieldCnt = 0;
	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (g_hero.m_aProcessQuest[i].isActive == FALSE)
			continue;
		if (g_hero.m_aProcessQuest[i].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if (g_hero.m_aProcessQuest[i].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;

		CQuestItem				*lpQuestItem	= lpQuestManager->getQuest(g_hero.m_aProcessQuest[i].questIndex);
		int						iProcessLevel	= g_hero.m_aProcessQuest[i].questProcessLevel;
		CQuestItem::cQuestInfo	*lpQuestInfo	= lpQuestItem->getInfo(iProcessLevel);
		int						iClientField	= lpQuestItem->getClientField(iProcessLevel);

		tsProcessQuestField		*lpQuestExplain	= &g_hero.m_aProcessQuest[i];
		char					*lpstrExplain	= lpQuestItem->getExplain(lpQuestExplain->questProcessLevel, lpQuestExplain->isComplete);
							

		if (lpQuestInfo->m_wLinkFieldCount)
		{
			BOOL bLinkQ = FALSE;
			for(int j=0 ; j<lpQuestInfo->m_wLinkFieldCount ; j++)
			{
				//BOOL bLinkQ = FALSE;
				if (lpQuestInfo->m_pLinkField[j] == lpInfo->m_wFieldSerial)	// 퀘스트목록의 필드번호와 마우스로 가르키고 있는 필드가 같다면..
					bLinkQ = TRUE;					

				if (!bLinkQ)
				{
					if (getInfoSurfaceFieldIndex(lpQuestInfo->m_pLinkField[j]) == getInfoSurfaceFieldIndex(lpInfo->m_wFieldSerial))
						bLinkQ = TRUE;						
				}
			} //

				if (bLinkQ)
				{
					// 관련NPC부분
					BOOL bAddTitle = FALSE;
					if (lpQuestInfo->m_wLinkNpcCount)
					{						
						for(int k=0; k<lpQuestInfo->m_wLinkNpcCount ; k++)
						{
							int	iField	=	lpQuestInfo->m_pNpcList[k].m_wField;							
							if (iField	==	0xffff)
								continue;							
							iField		=	m_info.getSurfaceField(iField);							
							if (iField	==	-1)
								continue;					
							
							if (bAddTitle == FALSE)
							{
								QuestInfoString.Add("<c:CTBLUE>%s<n>\n", dMSG_LINK_QUEST_NPC);
								bAddTitle = TRUE;
							}
							QuestInfoString.Add("  <c:LTYELLOW>%s<n>  ", lpQuestInfo->m_pNpcList[k].m_pStrName);
							bAddTitle = TRUE;
						}
					}
					// 관련NPC부분 끝

					// 관련Monster부분
					bAddTitle	=	FALSE;
					if (lpQuestInfo->m_wTargetMonsterCount)
					{
						for (int k=0; k<lpQuestInfo->m_wTargetMonsterCount ; k++)
						{
							int		iField			=	lpQuestInfo->m_pTargetMonsterList[k].m_wField;
							char	*lpstrMonsterName=	lpQuestInfo->m_pTargetMonsterList[k].m_pStrName;
							
							if (iField	==	0xffff	||	lpstrMonsterName	==	NULL)
								continue;							
							iField		=	m_info.getSurfaceField(iField);							
							if (iField	!=	m_info.m_iFocusFieldIndex)
								continue;

							if (bAddTitle	==	FALSE)
							{
								if (QuestInfoString.String != NULL)
									QuestInfoString.Add("\n<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_MONSTER);
								else
									QuestInfoString.Add("<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_MONSTER);
								bAddTitle	=	TRUE;
							}
							QuestInfoString.Add("  <c:LTYELLOW>%s<n>  ",lpstrMonsterName);
						}			
					}// 관련Monster부분 끝

					// 관련Item부분
					if (lpQuestInfo->m_wRequireItemCount)
					{
						if (QuestInfoString.String != NULL)
							QuestInfoString.Add("\n<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_ITEM);
						else
							QuestInfoString.Add("<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_ITEM);
						for (int k=0 ; k <lpQuestInfo->m_wRequireItemCount ; k++)
						{
							int iItem		= lpQuestInfo->m_pRequireItemList[k*2];
							int	iItemCount	= lpQuestInfo->m_pRequireItemList[k*2+1];
							int	iCount		= g_hero.getItemCountInInventory(iItem);
							char	strColor[12]=	"WHITE";
							
							if (iCount	>=	iItemCount)
								strcpy(strColor,"LTGREEN");
							
							QuestInfoString.Add("<c:%s>%s - %d/<c:LTYELLOW>%d<n>",strColor,g_aBasicItem[iItem].m_strName,iCount,iItemCount);			
						}
					}// 관련Item부분 끝
					
					// 퀘스트설명부분
					if (lpstrExplain)
					{
						if (QuestInfoString.String != NULL)
							QuestInfoString.Add("\n<c:CTBLUE>%s<n>\n",dMSG_QUEST_INFO);
						else
							QuestInfoString.Add("<c:CTBLUE>%s<n>\n",dMSG_QUEST_INFO);
						QuestInfoString.Add("%s",lpstrExplain);
					}// 퀘스트설명부분 끝
				}
			//}
			iQLinkFieldCnt += lpQuestInfo->m_wLinkFieldCount;
		}

		if (QuestInfoString.String != NULL)
		{
			m_MiniQuestInfo.m_strQuestName[i].Add("<c:WHITE>%s<n>\n", lpQuestItem->m_strName);
			m_MiniQuestInfo.m_strQuestInfo[i].Add("%s\n", QuestInfoString.String);
			QuestInfoString.Reset();
		}		
		else
		{
			m_MiniQuestInfo.m_strQuestName[i].Add("%s\n", lpQuestItem->m_strName);			
		}
	}

	
	// 창이 뜨는 위치를 조절한다. - x 위치
	int imarginX = 55;
	if (m_posPut.x+dWM_BASIC_WINDOW_WIDTH+imarginX > g_iScreenWidth)
	{
		m_posPut.x = lpInfo->m_rectFrame.x1 - dWM_BASIC_WINDOW_WIDTH - 10;
		for(int j=0 ; j <dMAX_PROCESS_QUEST_COUNT ; j++)
		{
			m_MiniQuestInfo.m_rtQuestpos[j].x1 = m_posPut.x;
			m_MiniQuestInfo.m_rtQuestpos[j].x2 = m_posPut.x+dWM_BASIC_WINDOW_WIDTH;
		}
	}

	// 창이 뜨는 위치를 조절한다. - y 위치

	int iY = m_posPut.y+CGame::s_sText.getTextHeight(dWM_BASIC_WINDOW_WIDTH-10, m_strDungeonInfo.String)+10+
			CGame::s_textBold13.getTextHeight(dWM_BASIC_WINDOW_WIDTH-10, m_MiniQuestInfo.m_strName)+10;
	for( i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
	{
		if (m_MiniQuestInfo.m_strQuestName[i].String != NULL)
			iY = iY + CGame::s_sText.getTextHeight(dWM_BASIC_WINDOW_WIDTH, m_MiniQuestInfo.m_strQuestName[i].String) + 5;
	}
	if (iY > g_iScreenHeight)
	{
		m_posPut.y -= (iY-g_iScreenHeight);
		for(int j=0 ; j<dMAX_PROCESS_QUEST_COUNT ; j++)
		{
			m_MiniQuestInfo.m_rtQuestpos[j].y1 -=(iY-g_iScreenHeight);
			m_MiniQuestInfo.m_rtQuestpos[j].y2 -=(iY-g_iScreenHeight);
		}
	}
}
// insu function add end



// insu function add
void
CWorldMap::DrawTotalInfo()
{
	int idrawBoxAlpha = 20;		// 박스 투명도
	int iLineGap = 17;			// 퀘스트리스트 출력시 한줄의 픽셀값
	if (m_info.m_iFocusFieldIndex == -1 && m_bDetailView == FALSE)
		return;

	if (m_iWorldmapMenu != 0xffff)
		return;

	// 필드이름 출력
	cRECT rtPut = {m_posPut.x, m_posPut.y, m_posPut.x+dWM_BASIC_WINDOW_WIDTH, m_posPut.y+m_MiniQuestInfo.m_iFieldNameHeight+10};
	s_ttCommon.drawBox(&rtPut, 0, idrawBoxAlpha);
	
	cWM_mainFieldInfo	*lpInfo	=	&m_info.m_aFieldList[m_info.m_iFocusFieldIndex];
	if (m_bDetailView)
		lpInfo	=	&m_info.m_aFieldList[m_iViewField];

	int iX = rtPut.x1+5;
	int iY = rtPut.y1+5;
	s_textBold13.cPut(iX, iY, dWM_BASIC_WINDOW_WIDTH-10, m_MiniQuestInfo.m_iFieldNameHeight, 
		0, LTYELLOW, m_MiniQuestInfo.m_strName);

	// 잠금 표시를 뿌려준다.
	if (m_bDetailView)
		g_sprInterface.Put(m_posPut.x-g_sprInterface.getSpriteWidth(370), m_posPut.y, 370);

	if (m_strDungeonInfo.String == NULL)
		return;


	iY += (m_MiniQuestInfo.m_iFieldNameHeight+10);
	
	// 포커스가 간 퀘스트리스트 목록에 표시
	if (m_iDetailFocusQuest != 0xffff)
		cDRAW::FillAB(0, m_MiniQuestInfo.m_rtQuestpos[m_iDetailFocusQuest].x1, m_MiniQuestInfo.m_rtQuestpos[m_iDetailFocusQuest].y1-1, m_MiniQuestInfo.m_rtQuestpos[m_iDetailFocusQuest].x2, m_MiniQuestInfo.m_rtQuestpos[m_iDetailFocusQuest].y2+1, 20);


	// 필드정보 출력
	rtPut.set(m_posPut.x, rtPut.y2, m_posPut.x+dWM_BASIC_WINDOW_WIDTH, rtPut.y2);
	for(int i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
	{		
		if	(!g_hero.m_aProcessQuest[i].isActive)
			continue;
		rtPut.y2 += iLineGap;
	}
	int istrHeight = s_sText.getTextHeight(dWM_BASIC_WINDOW_WIDTH-10, m_strDungeonInfo.String); // 필드정보 ~ 퀘스트정보 까지의 높이
	rtPut.y2 += (istrHeight+7); 
	s_ttCommon.drawBox(&rtPut, 0, idrawBoxAlpha);

	s_sText.put(iX, iY, dWM_BASIC_WINDOW_WIDTH-10, 500, 0, WHITE, m_strDungeonInfo.String);
	iY += istrHeight;
	
	for(i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
	{
		if	(!g_hero.m_aProcessQuest[i].isActive)
		{
			m_MiniQuestInfo.m_rtQuestpos[i].set(-1, -1, -1, -1);
			continue;
		}
		m_MiniQuestInfo.m_rtQuestpos[i].set(iX-5, iY, iX-5+(dWM_BASIC_WINDOW_WIDTH), iY+iLineGap-1);
		s_sText.put(iX, iY+2, dWM_BASIC_WINDOW_WIDTH-10, iLineGap, 0, GRAY, m_MiniQuestInfo.m_strQuestName[i].String);
		iY += iLineGap;
	}
}

// insu function add end



// insu function add
void
CWorldMap::DrawQuestInfo()
{
	if (m_bDetailView == FALSE || m_iDetailFocusQuest == 0xffff)
		return;

	int idrawBoxAlpha = 32;
	int istrHeight = 0;
	istrHeight = CGame::s_sText.getTextHeight(dWM_BASIC_WINDOW_WIDTH, dMSG_WORLD_UNLINKED_QUEST);
	if (m_MiniQuestInfo.m_strQuestInfo[m_iDetailFocusQuest].String != NULL)
		 istrHeight = CGame::s_sText.getTextHeight(dWM_BASIC_WINDOW_WIDTH, m_MiniQuestInfo.m_strQuestInfo[m_iDetailFocusQuest].String);
	cRECT rect;
	rect.set(	m_posQuest.x,
				m_posQuest.y,
				m_posQuest.x+dWM_BASIC_WINDOW_WIDTH+10,
				m_posQuest.y+istrHeight+10);
	s_ttCommon.drawBox(&rect, 0, idrawBoxAlpha);

	if (m_MiniQuestInfo.m_strQuestInfo[m_iDetailFocusQuest].String != NULL)
		CGame::s_sText.put(m_posQuest.x+5, m_posQuest.y+5, dWM_BASIC_WINDOW_WIDTH, istrHeight, 0, WHITE, m_MiniQuestInfo.m_strQuestInfo[m_iDetailFocusQuest].String);
	else
		CGame::s_sText.put(m_posQuest.x+5, m_posQuest.y+5, dWM_BASIC_WINDOW_WIDTH, istrHeight, 0, WHITE, dMSG_WORLD_UNLINKED_QUEST);
}
// insu function add end





void
CWorldMap::close()
{
	// 게임 마무리
	cINPDEV::ReleaseExclusive();
	s_iPopupInterface	=	ePIW_NONE;
	m_bmRadioButton.close();

	s_bIsFullScreenInterface	=	FALSE;
}
void
CWorldMap::drawFieldInfoTab()
{
	int		iFocusField	=	m_info.m_iFocusFieldIndex,i;

	if (iFocusField	==	-1)
		iFocusField	=	m_info.m_iSelectFieldIndex;

	int iAddX = 0;
	int iAddY = 0;
	if (g_iScreenWidth != 800 && m_rectInfoFrame.x2-m_rectInfoFrame.x1 > 800)
	{
		iAddX = (m_rectInfoFrame.x2-m_rectInfoFrame.x1)-800;
		iAddY = g_iScreenHeight-600;
		g_sprInterface.Put(m_rectInfoFrame.x1+iAddX, m_rectInfoFrame.y1, 122);
		g_sprInterface.Put(m_rectInfoFrame.x1+iAddX, m_rectInfoFrame.y2-4, 123);
	}

	s_ttFrame.drawBox(&m_rectInfoFrame);

	CPos	posCenter;

	m_rectInfoFrame.getCenter(&posCenter);

	cWM_mainFieldInfo	*lpInfo	=	&m_info.m_aFieldList[iFocusField];

	if(iFocusField != -1)		// 포커스가 되어있으면...
	{
		if (lpInfo->m_wLowLevel	==	0)
			s_textBold16.cPut(posCenter.x,m_rectInfoFrame.y1+6,LTYELLOW,g_aMapSimpleInfo[lpInfo->m_wFieldSerial].m_strName);
		else
			s_textBold16.cPut(posCenter.x,m_rectInfoFrame.y1+6,LTYELLOW,"%s <c:WHITE>(Lv<c:LTGREEN> %d~%d<c:WHITE>)<n>",g_aMapSimpleInfo[lpInfo->m_wFieldSerial].m_strName,lpInfo->m_wLowLevel,lpInfo->m_wTopLevel);
	}

	cDRAW::Line(0xffff, m_rectInfoFrame.x1+35, 476+iAddY, m_rectInfoFrame.x2-10, 476+iAddY);
	cDRAW::Line(0     , m_rectInfoFrame.x1+37, 478+iAddY, m_rectInfoFrame.x2-8, 478+iAddY);

	int	iY	=	m_rectInfoFrame.y1+8+24;

	g_sprWorldMap.Put(26,iY-14,36);
	s_textBold14.put(40,iY-24,WHITE,dMSG_WM_FIELD_INFO);

	for (i=0;i<lpInfo->m_wLinkDungeonCount;i++)
	{
		int				iLinkDungeon	=	lpInfo->m_awLinkDungeon[i];
		cWM_dungeonInfo	*lpDungenInfo	=	NULL;
		
		if (iLinkDungeon	!=	0xffff)
			lpDungenInfo	=	&m_info.m_aDungeonList[iLinkDungeon];

		if (lpDungenInfo	==	NULL)
		{
			s_text13.cPut(posCenter.x,iY,WHITE,dMSG_WM_VALID_LINKED_DUNGEON);
			break;
		}

		if (m_info.m_aDungeonList[iLinkDungeon].m_wIsNew == 0xffff)
			continue;

		if (lpDungenInfo->m_wDungeonCount)
		{
			int		iDungeonSerial	=	lpDungenInfo->m_awDungeonList[0];
			char	strDungeonName[64];

			strcpy(strDungeonName,g_aMapSimpleInfo[iDungeonSerial].m_strName);

			int		iNameLength		=	strlen(strDungeonName);

			if (strDungeonName[iNameLength-2]	==	'B')
				strDungeonName[iNameLength-2]	=	NULL;
			if (strDungeonName[iNameLength-3]	==	'B')
				strDungeonName[iNameLength-3]	=	NULL;
			if (strDungeonName[iNameLength-1]	==	'F')
			{
				if (strDungeonName[iNameLength-3]	>=	'0'	&&	strDungeonName[iNameLength-3]	<=	'9'	)
					strDungeonName[iNameLength-3]	=	NULL;
				else
					strDungeonName[iNameLength-2]	=	NULL;
			}

			s_text13.cPut(posCenter.x,iY,WHITE,"%s (Lv <c:LTGREEN>%d<n>~<c:LTGREEN>%d<n>)",strDungeonName,lpDungenInfo->m_wLowLevel,lpDungenInfo->m_wHighLevel);
		}

		iY	+=	15;
	}

	lpInfo	=	&m_info.m_aFieldList[m_info.m_iCurrentFieldIndex];

	lpInfo->m_rectFrame.getCenter(&posCenter);

	if (timeGetTime()%1200 < 1000)
		g_sprWorldMap.Put(posCenter.x,posCenter.y,g_iWorldMapJobIconIndex[g_hero.m_wJob]);
}




// insu function add
// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃퀘스트 관련필드를 월드맵인덱스에 맞도록 찾아 m_vtiQFIndexList에     ┃
// ┃기록한다.                                            			     ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
void
CWorldMap::setQuestField()
{
	m_vtiQFIndexList.clear();
	CQuest	*lpQuestManager	=	NULL;
	for (int i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (g_hero.m_aProcessQuest[i].isActive	==	FALSE		)
			continue;
		if (g_hero.m_aProcessQuest[i].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if (g_hero.m_aProcessQuest[i].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;
		
		int			iQuestIndex	=	g_hero.m_aProcessQuest[i].questIndex;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);
		tsProcessQuestField		*lpSelectQuest	=	&g_hero.m_aProcessQuest[i];
		int						iProcessLevel	=	lpSelectQuest->questProcessLevel;
		CQuestItem::cQuestInfo	*lpQuestText	=	lpQuest->getInfo(iProcessLevel);
		int iNpc = 0;
						
		for(int j=0 ; j<lpQuestText->m_wLinkFieldCount ; j++)
		{
			int iResult = g_map.getDungeonFirstFloor(lpQuestText->m_pLinkField[j]);
			iResult = m_info.getSurfaceField(iResult);

			BOOL bAdd = TRUE;
			for(int k=0 ; k<m_vtiQFIndexList.size() ; k++)
			{
				if (m_vtiQFIndexList[k] == iResult)
					bAdd = FALSE;	
			}
			if (bAdd)
				m_vtiQFIndexList.push_back(iResult);
		}
	}
}
// insu function add end



// insu function add
int
CWorldMap::getInfoSurfaceField()
{
	return m_info.m_iSurfaceField;
}


int
CWorldMap::getInfoCurrentField()
{
	return m_info.m_iCurrentField;
}
// insu function add end



void
CWorldMap::drawPartyInfoTab()
{
	int		i;

	int iAddX = 0;
	int iAddY = 0;
	if (g_iScreenWidth != 800 && m_rectInfoFrame.x2-m_rectInfoFrame.x1 > 800)
	{
		iAddX = (m_rectInfoFrame.x2-m_rectInfoFrame.x1)-800;
		iAddY = g_iScreenHeight-600;
		g_sprInterface.Put(m_rectInfoFrame.x1+iAddX, m_rectInfoFrame.y1, 122);
		g_sprInterface.Put(m_rectInfoFrame.x1+iAddX, m_rectInfoFrame.y2-4, 123);
	}

	s_ttFrame.drawBox(&m_rectInfoFrame);

	CPos	posCenter;
	cRECT	rect;

	m_rectInfoFrame.getCenter(&posCenter);

	s_textBold16.cPut(posCenter.x,m_rectInfoFrame.y1+6,LTYELLOW,dMSG_WM_PARTY_MEMBER_LIST);

	cDRAW::Line(0xffff, m_rectInfoFrame.x1+35, 476+iAddY, m_rectInfoFrame.x2-10, 476+iAddY);
	cDRAW::Line(0     , m_rectInfoFrame.x1+37, 478+iAddY, m_rectInfoFrame.x2-8, 478+iAddY);

	int	iY	=	m_rectInfoFrame.y1+8+24;

	g_sprWorldMap.Put(26,iY-14,33);
	s_textBold14.put(40,iY-24,WHITE,dMSG_WM_PARTY_INFO);

	if (s_partyInfo.m_wMemberCount	==	0)
		s_text13.cPut(posCenter.x,iY,WHITE,dMSG_WM_VALID_PARTY);
	else
	{
		for (i=0;i<s_partyInfo.m_wMemberCount;i++)
		{
			if (strcmp(s_partyInfo.m_aMemberList[i].m_strName,g_hero.m_strName)	==	0)
				continue;

			m_rectInfoFrame.getCenter(&posCenter);
			
			rect.set(40+5,iY-1,740-5,iY+14);
			
			if (i	==	m_iSelectPartyMember)
			{
				cDRAW::FillHB(_LTBLUE,&rect);
				s_text13.pushShadowStatus(FALSE);
				s_text13.cPut(posCenter.x,iY,LTYELLOW,s_partyInfo.m_aMemberList[i].m_strName);
				s_text13.popShadowStatus();
			}
			else
				if (i	==	m_iFocusPartyMember)
					s_text13.cPut(posCenter.x,iY,LTYELLOW,s_partyInfo.m_aMemberList[i].m_strName);
				else
					s_text13.cPut(posCenter.x,iY,WHITE,s_partyInfo.m_aMemberList[i].m_strName);
				
				iY	+=	15;


			CPartyMemberInfo	*lpMemberInfo	=	&s_partyInfo.m_aMemberList[i];
			int					iMemberField	=	m_info.getSurfaceField(lpMemberInfo->m_wFieldSerial);
			cWM_mainFieldInfo	*lpFieldInfo	=	&m_info.m_aFieldList[iMemberField];
			
			lpFieldInfo->m_rectFrame.getCenter(&posCenter);
			
			if (i == m_iSelectPartyMember)
			{
				if (timeGetTime()%1200 < 1000)
 					g_sprWorldMap.Put(posCenter.x,posCenter.y,g_iWorldMapJobIconIndex[lpMemberInfo->m_wJob]);
			}
			else
				g_sprWorldMap.Put(posCenter.x,posCenter.y,g_iWorldMapJobIconIndex[lpMemberInfo->m_wJob]);

		}
	}

}

void
CWorldMap::drawQuestInfoTab()
{
	int		i;

	int iAddX = 0;
	int iAddY = 0;
	int iRate = 100;
	if (g_iScreenWidth != 800 && m_rectInfoFrame.x2-m_rectInfoFrame.x1 > 800)
	{
		iAddX = (m_rectInfoFrame.x2-m_rectInfoFrame.x1)-800;
		iAddY = g_iScreenHeight-600;
		g_sprInterface.Put(m_rectInfoFrame.x1+iAddX, m_rectInfoFrame.y1, 122);
		g_sprInterface.Put(m_rectInfoFrame.x1+iAddX, m_rectInfoFrame.y2-4, 123);
		
		iRate = (int)(g_iScreenWidth*100)/800;
		iAddX -= ((50*iRate)/100);
	}

	s_ttFrame.drawBox(&m_rectInfoFrame);

	CPos	posCenter;
	cRECT	rect;

	m_rectInfoFrame.getCenter(&posCenter);

	s_textBold16.cPut(posCenter.x,m_rectInfoFrame.y1+6,LTYELLOW,dMSG_WM_PROCESS_QUEST);

	cDRAW::Line(0xffff, m_rectInfoFrame.x1+35, 476+iAddY, m_rectInfoFrame.x2-10, 476+iAddY);
	cDRAW::Line(0     , m_rectInfoFrame.x1+37, 478+iAddY, m_rectInfoFrame.x2-8, 478+iAddY);

	int	iY	=	m_rectInfoFrame.y1+8+24;

	g_sprWorldMap.Put(26,iY-14,31);
	s_textBold14.put(40,iY-24,WHITE,dMSG_WM_QUEST_INFO);

	if (g_hero.getProcessQeustCount()	==	0)
	{
		s_text13.cPut(posCenter.x+iAddX,iY,WHITE,dMSG_WM_NOT_EXIST_PROCESS_QUEST);

		return;
	}

	{
		cWM_mainFieldInfo	*lpCurrentField	=	&m_info.m_aFieldList[m_info.m_iCurrentFieldIndex];

		lpCurrentField->m_rectFrame.getCenter(&posCenter);

		if (timeGetTime()%1200 < 1000)
			g_sprWorldMap.Put(posCenter.x,posCenter.y,g_iWorldMapJobIconIndex[g_hero.m_wJob]);
	}

	CQuest	*lpQuestManager	=	NULL;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (g_hero.m_aProcessQuest[i].isActive	==	FALSE		)
			continue;
		if (g_hero.m_aProcessQuest[i].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		if (g_hero.m_aProcessQuest[i].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;

		int			iQuestIndex	=	g_hero.m_aProcessQuest[i].questIndex;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);
		char		strQuestName[128];

		strcpy(strQuestName,lpQuest->m_strName);

		if (g_hero.m_aProcessQuest[i].isComplete)
			sprintf(strQuestName,"%s (<c:LTGREEN>C<n>)",lpQuest->m_strName);

		int iReduceX = 65;
		if (g_iScreenWidth != 800 && m_rectInfoFrame.x2-m_rectInfoFrame.x1 > 800)
		{
			iReduceX = (iReduceX*iRate)/100;
		}

		rect.set(40+5,iY-1,g_iScreenWidth-iReduceX,iY+13);
		rect.getCenter(&posCenter);
		
		if (i	==	m_iSelectQuest)
		{
			cDRAW::FillHB(_LTBLUE,&rect);
			s_text13.pushShadowStatus(FALSE);
			s_text13.cPut(posCenter.x,iY,LTYELLOW,strQuestName);
			s_text13.popShadowStatus();

			tsProcessQuestField		*lpSelectQuest	=	&g_hero.m_aProcessQuest[m_iSelectQuest];
			int						iProcessLevel	=	lpSelectQuest->questProcessLevel;
			CQuestItem::cQuestInfo	*lpQuestText	=	lpQuest->getInfo(iProcessLevel);
			int						iClientField	=	lpQuest->getClientField(iProcessLevel),iNpc;

			if (iClientField	!=	0xffff)
			{
				iClientField	=	m_info.getSurfaceField(iClientField);

				if (iClientField!=	-1)
				{
					CPos	pos;

					m_info.m_aFieldList[iClientField].m_rectFrame.getCenter(&pos);

					if ((timeGetTime()+200)%1200 < 1000)
					{
						g_sprInterface2.Put(pos.x+1,pos.y+1,eMDII_WEAPON_SHOP+eNPC_PROCESS_QUEST_CLIENT-3,g_iGameScale,g_iGameScale,dPUT_COLOR,0);
						g_sprInterface2.Put(pos.x,pos.y,eMDII_WEAPON_SHOP+eNPC_PROCESS_QUEST_CLIENT-3,g_iGameScale,g_iGameScale);
					}
				}
			}
			for (iNpc=0;iNpc<lpQuestText->m_wTargetMonsterCount;iNpc++)
			{
				int	iMonsterField	=	lpQuestText->m_pTargetMonsterList[iNpc].m_wField;

				if (iMonsterField	==	0xffff)
					continue;

				iMonsterField	=	m_info.getSurfaceField(iMonsterField);

				if (iMonsterField==	-1)
					continue;

				CPos	pos;

				m_info.m_aFieldList[iMonsterField].m_rectFrame.getCenter(&pos);

				if ((timeGetTime()+400)%1200 < 1000)
				{
					g_sprInterface2.Put(pos.x+1,pos.y+1,eMDII_WEAPON_SHOP+eNPC_PROCESS_QUEST_TARGET_MONSTER-3,g_iGameScale,g_iGameScale,dPUT_COLOR,0);
					g_sprInterface2.Put(pos.x,pos.y,eMDII_WEAPON_SHOP+eNPC_PROCESS_QUEST_TARGET_MONSTER-3,g_iGameScale,g_iGameScale);
				}
			}
			for (iNpc=0;iNpc<lpQuestText->m_wLinkNpcCount;iNpc++)
			{
				int	iField	=	lpQuestText->m_pNpcList[iNpc].m_wField;

				if (iField	==	0xffff)
					continue;

				iField		=	m_info.getSurfaceField(iField);

				if (iField	==	-1)
					continue;

				CPos	pos;

				m_info.m_aFieldList[iField].m_rectFrame.getCenter(&pos);

				if ((timeGetTime()+600)%1200 < 1000)
				{
					g_sprInterface2.Put(pos.x+1,pos.y+1,eMDII_WEAPON_SHOP+eNPC_PROCESS_QUEST_REFERENCE_NPC-3,g_iGameScale,g_iGameScale,dPUT_COLOR,0);
					g_sprInterface2.Put(pos.x,pos.y,eMDII_WEAPON_SHOP+eNPC_PROCESS_QUEST_REFERENCE_NPC-3,g_iGameScale,g_iGameScale);
				}
			}
		}
		else
		if (i	==	m_iFocusQuest)
			s_text13.cPut(posCenter.x,iY,LTYELLOW,strQuestName);
		else
			s_text13.cPut(posCenter.x,iY,WHITE,strQuestName);

		iY	+=	15;
	}
}

void
CWorldMap::draw()
{
// 월드맵 상태가 아니면 종료.
	if(s_iPopupInterface	!=	ePIW_WORLD_MAP)
		return;
// 월드맵 출력
	int iRate = 100; // 기본비율 100

	if (g_iScreenWidth != 800)
		iRate = (int)(g_iScreenWidth*100)/800;

	g_sprWorldMap.Put(0,0,0, iRate, iRate);
// 버튼 컨트롤 출력
	m_info.drawFieldFrame(m_iWorldmapMenu);
	m_bmRadioButton.draw();

#ifdef	_DEBUG
	for (int i=0;i<m_info.m_iFieldInfoCount;i++)
	{
		cWM_mainFieldInfo	*lpFieldInfo	=	&m_info.m_aFieldList[i];

		s_bText.pushShadowStatus(FALSE);
		s_bText.put(lpFieldInfo->m_rectFrame.x1,lpFieldInfo->m_rectFrame.y1,0,"%d",i);
		s_bText.popShadowStatus();
	}

	s_bText.cPut(g_iScreenWidth/2,30,WHITE,"%d,%d",s_posMouse.x,s_posMouse.y);
#endif

	switch(m_iWorldmapMenu)
	{
		case	eWMM_FIELD_INFO		:
			drawFieldInfoTab();
			break;
		case	eWMM_PARTY_INFO		:
			drawPartyInfoTab();
			break;
		case	eWMM_QUEST_INFO		:
			drawQuestInfoTab();
			break;
	}
	// insu modify
	if (!m_bIsForGateGlove)
	{
		DrawTotalInfo();
		DrawQuestInfo();
	}
	// insu modify end
}

void
CWorldMap::updateFieldInfoTab()
{
	s_isClickedLeftButton	=	s_isClickedLeftButton;

	m_info.update(s_posMouse.x,s_posMouse.y,s_isTouchLeftButton,m_bIsForGateGlove);

	if	(m_bIsForGateGlove	&&	m_info.m_iSelectFieldSerial	!=	-1)
	{
		if	(g_hero.m_wCurrentField		!=	m_info.m_iSelectFieldSerial)
		{
			m_iSelectFieldSerial		=	m_info.m_iSelectFieldSerial;
			m_info.m_iSelectFieldSerial	=	-1;

			char	strText[256];

			if	(g_hero.m_bf1IsNetCafeUser	||	m_iUseItemSlotIndex	!=	-1) 
				sprintf(strText,dMSG_WARP_NET_CAFE_USER_CONFIRM_FORM,g_aMapSimpleInfo[m_iSelectFieldSerial].m_strName);
			else
				sprintf(strText,dMSG_USE_GATE_GLOVE_CONFIRM_FORM,g_aMapSimpleInfo[m_iSelectFieldSerial].m_strName);
				
			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_USE_GATE_GLOVE);
		}
	}
}

void
CWorldMap::updatePartyInfoTab()
{
	int		i;
	CPos	posCenter;

	m_rectInfoFrame.getCenter(&posCenter);

	int		iY					=	m_rectInfoFrame.y1+8+24;
	BOOL	bIsCorrectMember	=	FALSE;

	m_iFocusPartyMember			=	-1;

	for (i=0;i<s_partyInfo.m_wMemberCount;i++)
	{
 		if (strcmp(s_partyInfo.m_aMemberList[i].m_strName,g_hero.m_strName)	==	0)
 			continue;

		cRECT	rectTitle;

		rectTitle.set(m_rectInfoFrame.x1,iY,m_rectInfoFrame.x2,iY+15);
		
		if (rectTitle.isIn(s_posMouse.x,s_posMouse.y))
			m_iFocusPartyMember	=	i;

		if (s_isTouchLeftButton	&&	m_iFocusPartyMember	!=	-1)
			m_iSelectPartyMember=	m_iFocusPartyMember;

		if (m_iSelectPartyMember==	-1)
			m_iSelectPartyMember=	i;

// 		if (m_iSelectPartyMember==	m_iSelectPartyMember)
// 			bIsCorrectMember	=	FALSE;

		iY	+=	15;
	}

// 	if (bIsCorrectMember	==	FALSE)
// 		m_iSelectPartyMember=	-1;
}

void
CWorldMap::updateQuestInfoTab()
{
	int		i;
	CPos	posCenter;
	cSTRING	tooltip;
	cRECT	rect;

	m_info.update(s_posMouse.x,s_posMouse.y,s_isTouchLeftButton);
	m_rectInfoFrame.getCenter(&posCenter);

	int		iY					=	m_rectInfoFrame.y1+8+24;

	m_iFocusQuest				=	-1;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (g_hero.m_aProcessQuest[i].isActive	==	FALSE		)
			continue;

		int iReduceX = 65;
		if (g_iScreenWidth != 800 && m_rectInfoFrame.x2-m_rectInfoFrame.x1 > 800)
		{
			int iRate = (int)(g_iScreenWidth*100)/800;
			iReduceX = (iReduceX*iRate)/100;
		}
		rect.set(40+5,iY-1,g_iScreenWidth-iReduceX,iY+13);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			m_iFocusQuest	=	i;

			CQuest				*lpQuestManager;
			tsProcessQuestField	*lpQuestInfo	=	&g_hero.m_aProcessQuest[i];
			int					iProcessLevel	=	lpQuestInfo->questProcessLevel;

			if (lpQuestInfo->questKind	==	eQT_NORMAL	)
				lpQuestManager	=	&g_quest;
			if (lpQuestInfo->questKind	==	eQT_ARBEIT	)
				lpQuestManager	=	&g_questArbeit;

			int						iQuestIndex		=	g_hero.m_aProcessQuest[i].questIndex;
			CQuestItem				*lpSelectQuest	=	lpQuestManager->getQuest(iQuestIndex);
			CQuestItem::cQuestInfo	*lpQuestText	=	lpSelectQuest->getInfo(iProcessLevel);

			rect.set(5,m_rectInfoFrame.y1-1,200,g_iScreenHeight);

			char	*lpstrExplain		=	lpSelectQuest->getExplain(lpQuestInfo->questProcessLevel,lpQuestInfo->isComplete);
			char	*lpstrClientName	=	lpSelectQuest->getClientName(iProcessLevel);
			char	*lpstrQuestValue	=	lpSelectQuest->getValueString(iProcessLevel);
			int		iClientField		=	lpSelectQuest->getClientField(iProcessLevel);

			if (iClientField	==	0xffff && lpstrClientName)
			{
				tooltip.Add("<c:CTBLUE>%s<n>\n",dMSG_QUEST_CLIENT_INFO);
				tooltip.Add("%s\n\n",lpstrClientName);
			}
			
			if (lpstrQuestValue)
			{
				char	*lpstrText	=	_ms(lpstrQuestValue,lpQuestInfo->questValue);
				tooltip.Add("<c:CTBLUE>%s<n>\n",dMSG_PROCESS_STATUS);

				tooltip.Add("%s\n\n",lpstrText);
			}

			if (lpQuestText->m_wRequireItemCount)
			{
				tooltip.Add("<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_ITEM);

				for (i=0;i<lpQuestText->m_wRequireItemCount;i++)
				{
					int		iItem		=	lpQuestText->m_pRequireItemList[i*2];
					int		iItemCount	=	lpQuestText->m_pRequireItemList[i*2+1];
					int		iCount		=	g_hero.getItemCountInInventory(iItem);
					char	strColor[12]=	"WHITE";

					if (iCount	>=	iItemCount)
						strcpy(strColor,"LTGREEN");

					tooltip.Add("<c:%s>%s - %d/<c:LTYELLOW>%d<n>\n",strColor,g_aBasicItem[iItem].m_strName,iCount,iItemCount);
				}
				tooltip.Add("\n");
			}

			if (lpstrExplain)
			{
				tooltip.Add("<c:CTBLUE>%s<n>\n",dMSG_QUEST_INFO);
				tooltip.Add("%s",lpstrExplain);
			}

			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,tooltip.String,eTFM_BOTTOM);
		}

		if (s_isTouchLeftButton	&&	m_iFocusQuest	!=	-1)
			m_iSelectQuest	=	m_iFocusQuest;

		if (m_iSelectQuest	==	-1)
			m_iSelectQuest	=	i;

		iY	+=	15;
	}

	if (m_iSelectQuest	==	-1)
		return;

	CQuest	*lpQuestManager	=	NULL;

	tsProcessQuestField		*lpSelectQuest	=	&g_hero.m_aProcessQuest[m_iSelectQuest];
	
	if (lpSelectQuest->questKind	==	eQT_NORMAL	)
		lpQuestManager	=	&g_quest;
	if (lpSelectQuest->questKind	==	eQT_ARBEIT	)
		lpQuestManager	=	&g_questArbeit;

	CQuestItem				*lpQuest		=	lpQuestManager->getQuest(lpSelectQuest->questIndex);
	int						iProcessLevel	=	lpSelectQuest->questProcessLevel;
	CQuestItem::cQuestInfo	*lpQuestText	=	lpQuest->getInfo(iProcessLevel);
	int						iClientField	=	lpQuest->getClientField(iProcessLevel);
	int						iFocusFieldIndex=	m_info.m_iFocusFieldIndex;

	if (iFocusFieldIndex	!=	-1)
	{
		rect.set(&m_info.m_aFieldList[iFocusFieldIndex].m_rectFrame);
		rect.add(rect.getWidth()+2,0);
	}

	if (iClientField!=	0xffff)
	{
		iClientField	=	m_info.getSurfaceField(iClientField);

		if (iClientField==	iFocusFieldIndex)
		{
			char	*lpstrClientName		=	lpQuest->getClientName(iProcessLevel);

			if (lpstrClientName)
			{
				tooltip.Add("<c:CTBLUE>%s<n>\n",dMSG_QUEST_CLIENT_INFO);
				tooltip.Add("  <c:LTYELLOW>%s<n>\n\n",lpstrClientName);
			}
		}
	}
	if (lpQuestText->m_wLinkNpcCount)
	{
		BOOL	bAddTitle	=	FALSE;

		for (i=0;i<lpQuestText->m_wLinkNpcCount;i++)
		{
			int		iField			=	lpQuestText->m_pNpcList[i].m_wField;
			char	*lpstrNpcName	=	lpQuestText->m_pNpcList[i].m_pStrName;

			if (iField	==	0xffff	||	lpstrNpcName	==	NULL)
				continue;

			iField		=	m_info.getSurfaceField(iField);

			if (iField	!=	iFocusFieldIndex)
				continue;

			if (bAddTitle	==	FALSE)
			{
				tooltip.Add("<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_NPC);
				bAddTitle	=	TRUE;
			}
			tooltip.Add("  <c:LTYELLOW>%s<n>\n",lpstrNpcName);
		}

		if (bAddTitle)
			tooltip.Add("\n");
	}
	if (lpQuestText->m_wTargetMonsterCount)
	{
		BOOL	bAddTitle	=	FALSE;

		for (i=0;i<lpQuestText->m_wTargetMonsterCount;i++)
		{
			int		iField			=	lpQuestText->m_pTargetMonsterList[i].m_wField;
			char	*lpstrMonsterName=	lpQuestText->m_pTargetMonsterList[i].m_pStrName;

			if (iField	==	0xffff	||	lpstrMonsterName	==	NULL)
				continue;

			iField		=	m_info.getSurfaceField(iField);

			if (iField	!=	iFocusFieldIndex)
				continue;

			if (bAddTitle	==	FALSE)
			{
				tooltip.Add("<c:CTBLUE>%s<n>\n",dMSG_LINK_QUEST_MONSTER);
				bAddTitle	=	TRUE;
			}
			tooltip.Add("  <c:LTYELLOW>%s<n>\n",lpstrMonsterName);
		}

		if (bAddTitle)
			tooltip.Add("\n");
	}

	if (tooltip.Length > 3)
	{
		if (tooltip.String[tooltip.Length-2]	==	10)
			tooltip.String[tooltip.Length-2]	=	0;

		s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,tooltip.String);
	}
}

BOOL
CWorldMap::update()
{
// 월드맵 상태가 아니면 종료.
	if(s_iPopupInterface	!=	ePIW_WORLD_MAP)
		return	FALSE;

	if (cINPDEV::IsExclusive())
		return	FALSE;

	switch(m_iWorldmapMenu)
	{
		case	eWMM_FIELD_INFO		:
			updateFieldInfoTab();
			break;
		case	eWMM_PARTY_INFO		:
			updatePartyInfoTab();
			break;
		case	eWMM_QUEST_INFO		:
			updateQuestInfoTab();
			break;
	}
	// insu add	
	if (s_isClickedLeftButton)
	{
		m_bDetailView = (m_bDetailView)? FALSE : TRUE;
		if (!m_bDetailView)
		{
			UpdateTotalInfo();
			m_iDetailFocusQuest = 0xffff;
		}
		m_iViewField = m_info.m_iFocusFieldIndex;
		if (m_info.m_iFocusFieldIndex == -1 && m_bDetailView)
			m_bDetailView = FALSE;
	}
	
	m_info.update(s_posMouse.x,s_posMouse.y,s_isTouchLeftButton);

	if (m_iPreFocusField != m_info.m_iFocusFieldIndex && m_bDetailView == FALSE)
		UpdateTotalInfo();

	if (m_bDetailView) // 마우스를 눌러서 고정이 됐을때!
	{
		// 포커스가 가있는 퀘스트를 찾는다.
		for(int i=0 ; i<dMAX_PROCESS_QUEST_COUNT ; i++)
		{
			if (m_MiniQuestInfo.m_rtQuestpos[i].isIn(s_posMouse.x, s_posMouse.y))
			{
				m_iDetailFocusQuest = i;

				// 퀘스트정보창이 뜨는 위치를 설정한다.
				m_posQuest.x = m_posPut.x+dWM_BASIC_WINDOW_WIDTH;
				if (m_posQuest.x+(dWM_BASIC_WINDOW_WIDTH+10) > g_iScreenWidth)
					m_posQuest.x = m_posPut.x - (dWM_BASIC_WINDOW_WIDTH+10);

				m_posQuest.y = m_posPut.y;
				int istrHeight = 0;
				istrHeight = CGame::s_sText.getTextHeight(dWM_BASIC_WINDOW_WIDTH, dMSG_WORLD_UNLINKED_QUEST);
				if (m_MiniQuestInfo.m_strQuestInfo[m_iDetailFocusQuest].String != NULL)
					istrHeight = CGame::s_sText.getTextHeight(dWM_BASIC_WINDOW_WIDTH, m_MiniQuestInfo.m_strQuestInfo[m_iDetailFocusQuest].String);
				if (m_posQuest.y+istrHeight > g_iScreenHeight)
					m_posQuest.y -= (m_posQuest.y+istrHeight - g_iScreenHeight);
				break;
			}
			m_iDetailFocusQuest = 0xffff;
		}		
	}
	m_iPreFocusField = m_info.m_iFocusFieldIndex;
	// insu add end
	DWORD	dwMenu	=	m_bmRadioButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

	switch(dwMenu)
	{
		case	eWMM_FIELD_INFO		:
		case	eWMM_PARTY_INFO		:
		case	eWMM_QUEST_INFO		:
			// insu add
			if (m_iWorldmapMenu == dwMenu)
			{
				m_iWorldmapMenu = 0xffff;
				break;
			}
			// insu add end
			m_iWorldmapMenu		=	dwMenu;
			break;
		case	eWMM_RETURN_TO_GAME	:
			close();
			break;
	}

	return	TRUE;
}

BOOL
CWorldMap::isOpened()
{
	if	(s_iPopupInterface	!=	ePIW_WORLD_MAP)
		return	FALSE;

	return	TRUE;
}