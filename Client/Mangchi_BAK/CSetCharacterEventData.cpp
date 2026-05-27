#include	"cSET.H"
#include	"cSKILL.H"
#include	"cJOB.H"
#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAIN.H"
#include	"cITEM_PREFIX_EFFECT.H"
#include	"resource.H"
#include	"cKARMA.H"

int				cSET_CHARACTER::s_iSelectCondition	=	eMECC_KILLED_BY_PLAYER;
CKarmaContainer	cSET_CHARACTER::s_aEvent[eMECC_COUNT];
CKarma			*cSET_CHARACTER::s_lpStoredKarma;
cCHARACTER		*cSET_CHARACTER::s_lpEditCharacter;

//
//	이벤트 설정
void
cSET_CHARACTER::SetEvent(HWND _hDlg,cCHARACTER *_lpEditCharacter)
{
	int	iTime	=	timeGetTime();
	s_lpEditCharacter	=	_lpEditCharacter;
	s_lpStoredKarma		=	cKARMA_WND::s_aStoredKarma;

	for (int i=0;i<eMECC_COUNT;i++)
		s_aEvent[i].copy(_lpEditCharacter->m_pEvent[i],TRUE);

	cKARMA_WND::s_bIsFieldKarma	=	FALSE;

	if	(DialogBox( s_hInst,"SET_CHARACTER_EVENT_DATA",_hDlg,(DLGPROC)s_procSetCharacterEventData)==IDOK)
		return;
}

void
cSET_CHARACTER::InitSetCharacterEventData(HWND _hDlg)
{
	HWND	hEventConditionList	=	GetDlgItem(_hDlg,IDC_EVENT_CONDITION_LIST);

	cCOMBOBOX::Reset(hEventConditionList);

	int		iIndex;
	for (iIndex=0;;iIndex++)
	{
		if (stricmp(g_strMonsterEventCheckCondition[iIndex],"end")==0)	break;

		cCOMBOBOX::Add(hEventConditionList,g_strMonsterEventCheckCondition[iIndex],iIndex);
	}

	cCOMBOBOX::SelectByData(hEventConditionList,s_iSelectCondition);

	g_lpAKC	=	&s_aEvent[s_iSelectCondition];

	cKARMA_WND::UpdateKarmaDlg(_hDlg,g_lpAKC);
}

//
//	카르마 선택
//
void
cSET_CHARACTER::SelectCondition(HWND _hDlg)
{
	HWND	hKarmaGroup	=	GetDlgItem(_hDlg,IDC_EVENT_CONDITION_LIST);	//	카르마 그룹 리스트

	int		iSelectGroup	=	cCOMBOBOX::GetData(hKarmaGroup);

	if	(iSelectGroup	==	s_iSelectCondition)
		return;

	s_iSelectCondition	=	iSelectGroup;

	g_lpAKC	=	&s_aEvent[s_iSelectCondition];

	cKARMA_WND::UpdateKarmaDlg(_hDlg,g_lpAKC);
}

//
//	이벤트 제거
void
cSET_CHARACTER::RemoveEvent(HWND hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0)
		return;

	HWND	hList				=	GetDlgItem(hDlg,IDC_KARMA_LIST);
	int		aiSelectKarma[c_iCopyKarmaBufferSize];
	int		iSelectKarmaCount	=	cLISTBOX::GetSelect(hList,aiSelectKarma,c_iCopyKarmaBufferSize);

	if	(iSelectKarmaCount	<=	0)
		return;

	if	(cMSG::YESNO(dMSG_REMOVE_SELECT_KARMA,dMSG_QUESTION_CONTINUE)	==	IDNO)
		return;

	for	(int i=0;i<iSelectKarmaCount;i++)
	{
		g_lpAKC->remove(aiSelectKarma[i]-i);

		g_iSelectKarma	=	aiSelectKarma[i]-i-1;
	}

	g_iSelectKarma	=	max(g_iSelectKarma,0);

	g_lpActmap->LostVirgin();

	cKARMA_WND::UpdateKarmaDlg(hDlg);
}

//
//	작업 완료
void
cSET_CHARACTER::CompleteWork(HWND hDlg)
{
	int	i;

	for (i=0;i<eMECC_COUNT;i++)
	{
		KILL(s_lpEditCharacter->m_pEvent[i]);

		if (s_aEvent[i].m_wKarmaCount == 0) continue;

		s_lpEditCharacter->m_pEvent[i]	=	new CKarmaContainer();
		s_lpEditCharacter->m_pEvent[i]->copy(&s_aEvent[i]);
		s_lpEditCharacter->m_pEvent[i]->setTitle(g_strMonsterEventCheckCondition[i]);
	}
}

void
cSET_CHARACTER::UpEvent(HWND hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0	||	g_iSelectKarma	<=	0)
		return;

	CKarma	temp;

	temp.copy(&g_lpAKC->m_pKarma[g_iSelectKarma]);
	g_lpAKC->m_pKarma[g_iSelectKarma].copy(&g_lpAKC->m_pKarma[g_iSelectKarma-1]);
	g_lpAKC->m_pKarma[g_iSelectKarma-1].copy(&temp);
	g_iSelectKarma--;

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
}

void
cSET_CHARACTER::DownEvent(HWND hDlg)
{
	if	(g_iSelectKarma	>=	g_lpAKC->getKarmaCount()-1)
		return;

	CKarma	temp;

	temp.copy(&g_lpAKC->m_pKarma[g_iSelectKarma]);
	g_lpAKC->m_pKarma[g_iSelectKarma].copy(&g_lpAKC->m_pKarma[g_iSelectKarma+1]);
	g_lpAKC->m_pKarma[g_iSelectKarma+1].copy(&temp);
	g_iSelectKarma++;

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
}

//
//	이벤트 복사
void
cSET_CHARACTER::CopyEvent(HWND hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0)
		return;

	int		aiSelectKarma[c_iCopyKarmaBufferSize];

	HWND	hList				=	GetDlgItem(hDlg,IDC_KARMA_LIST);
	int		iSelectKarmaCount	=	cLISTBOX::GetSelect(hList,aiSelectKarma,c_iCopyKarmaBufferSize);

	if	(iSelectKarmaCount	<=	0)
		return;

	for	(int i=0;i<iSelectKarmaCount;i++)
	{
		CKarma	*lpKarma	=	&g_lpAKC->m_pKarma[aiSelectKarma[i]];

		s_lpStoredKarma[i].copy(lpKarma);
		s_lpStoredKarma[i].setTitle(_ms("%s[2]",lpKarma->m_pstrTitle));
	}

	cKARMA_WND::s_iCopiedKaramaCount	=	iSelectKarmaCount;

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
}

//
//	이벤트 붙여넣기
void
cSET_CHARACTER::PasteEvent(HWND hDlg)
{
	if	(cKARMA_WND::s_iCopiedKaramaCount	==	0)
		return;		//	조건의 수,반응의 수

	int	iAddedKarma;

	for	(int i=0;i<cKARMA_WND::s_iCopiedKaramaCount;i++)
		iAddedKarma		=	g_lpAKC->add(&s_lpStoredKarma[i]);

	if	(iAddedKarma	==	0xffff)
		return;

	g_lpAKC->selectKarma(iAddedKarma);	//	선택

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
	g_lpActmap->LostVirgin();	//	맵의 카르마 세트를 편집중이다.
}

//
//
LRESULT CALLBACK
cSET_CHARACTER::s_procSetCharacterEventData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG	:
			InitSetCharacterEventData(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
			{
				case	IDOK						:
				{
					CompleteWork(hDlg);
					EndDialog(hDlg,IDOK);
					break;
				}

				case	IDCANCEL					:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_NEW						:
				{
					CKarma *lpNewKarma	=	cKARMA_WND::AddKarma(hDlg,g_strMonsterEventCheckCondition[s_iSelectCondition],NULL,eADD_KARMA_NORMAL);

					if (!lpNewKarma)	break;

					int	iAddedKarma		=	g_lpAKC->add(lpNewKarma);

					if (iAddedKarma		==	0xffff)		break;

					g_lpAKC->selectKarma(iAddedKarma);	//	선택
					cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
					g_lpActmap->LostVirgin();	//	맵의 카르마 세트를 편집중이다.

					break;
				}

				case	IDC_EDIT					:
				{
					CKarma	*lpKarma	=	g_lpAKC->getSelectKarma();
					CKarma	*lpAddKarma	=	cKARMA_WND::AddKarma(hDlg,g_strMonsterEventCheckCondition[s_iSelectCondition],lpKarma,eADD_KARMA_NORMAL);

					if (!lpAddKarma)	break;

					lpKarma->copy(lpAddKarma);
					g_lpAKC->selectKarma(lpKarma->m_wSerial);	//	선택
					cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트

					g_lpActmap->LostVirgin();	//	맵의 카르마 세트를 편집중이다.
					break;
				}

				case	IDC_REMOVE					:
					RemoveEvent(hDlg);
					break;

				case	IDC_PASTE					:
				{
					PasteEvent(hDlg);
					break;
				}

				case	IDC_UP						:
				{
					UpEvent(hDlg);
					break;
				}
				case	IDC_DOWN					:
				{
					DownEvent(hDlg);
					break;
				}

				case	IDC_COPY					:
				{
					CopyEvent(hDlg);
					break;
				}

				case	IDC_KARMA_LIST		:	//	카르마 리스트를 클릭했다.
				{
					cKARMA_WND::SelectKarma(hDlg);
					break;
				}

				case	IDC_EVENT_CONDITION_LIST	:	//	카르마 그룹 리스트를 클릭했다.
					SelectCondition(hDlg);
					break;
			}			
			break;
	}

	return (0L);
}	//	cSET_CHARACTER::s_procSetCharacterEventData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)