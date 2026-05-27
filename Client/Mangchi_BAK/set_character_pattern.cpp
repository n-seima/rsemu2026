#include "set_character_pattern.h"
#include "resource.h"
#include "cMAPWND.h"

cCharacterPatternPack	cSetCharacterPattern::s_editPattern;
int						cSetCharacterPattern::s_iSelectPattern;

HWND					cSetCharacterPattern::s_hWnd;
cLISTCONTROL			cSetCharacterPattern::s_listPattern;	//	던젼 리스트

BOOL
cSetCharacterPattern::Open(HWND _hWnd)	//	시작
{
	if	(DialogBox( s_hInst, "PATTERN_LIST",_hWnd,(DLGPROC)ProcDlg)	==	IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cSetCharacterPattern::InitData(HWND _hDlg)
{
	SetDlgItemText(_hDlg,IDC_NAME,"");

	memcpy(&s_editPattern,&g_lpActmap->m_cpp,sizeof(s_editPattern));

	HWND	hPatternList;

	hPatternList		=	GetDlgItem(_hDlg,IDC_PATTERN_LIST);

	s_hWnd				=	_hDlg;

	s_iSelectPattern	=	0;

	{
		s_listPattern.Init		(hPatternList	,2		,FALSE	,ProcPatternList);		//	2개의 컬럼
		s_listPattern.AddColumn("Index"			,6*7	,eLCOM_CENTER);
		s_listPattern.AddColumn("이름"			,6*30	,eLCOM_CENTER);
	}

	{
		HWND	hPatternEx	=	GetDlgItem(_hDlg,IDC_PATTERN_EX);

		cCOMBOBOX::Reset(hPatternEx);

		for	(int i=0;;i+=2)
		{
			if	(stricmp(g_strPatternExample[i],"end")==0)
				break;

			cCOMBOBOX::Add(hPatternEx,g_strPatternExample[i]);
		}

		cCOMBOBOX::Select(hPatternEx,0);
	}

	UpdatePatternList(_hDlg,0);

	Draw(_hDlg);
}

void
cSetCharacterPattern::Draw(HWND hDlg)
{
	s_listPattern.Draw();
}

void
cSetCharacterPattern::AddPattern(HWND hDlg,BOOL _bIsModifySelectPattern)
{
	char		*pPatternText;
	cDECODER	decoder;

	pPatternText	=	new char [1024*20];

	GetDlgItemText(hDlg,IDC_PATTERN_SCRIPT,pPatternText,1024*20);

	decoder.UploadBuffer(pPatternText,NULL);

	pKILL(pPatternText);

	cCharacterPattern	pattern;

	pattern.reset();

	GetDlgItemText(hDlg,IDC_NAME,pattern.m_strName,sizeof(pattern.m_strName)-2);

	if	(strlen(pattern.m_strName) <= 1)
	{
		cMSG::Put("패턴 추가 에러!","너무 짧은 이름입니다.");

		return;
	}

	while(1)
	{
		cCharacterPatternPiece	*lpPiece	=	&pattern.m_aPattern[pattern.m_wCount];

		char	*lpstrCode	=	decoder.GetWord();

		if	(stricmp(lpstrCode,"isFinishCode")	==	0)
			continue;
		if	(stricmp(lpstrCode,"isEndCode")	==	0)
			break;

		if	(stricmp(lpstrCode,"이동")	==	0)
		{
			lpPiece->m_wPattern	=	eCP_move;

			int	iTargetX	=	decoder.GetNumber();
			int	iTargetY	=	decoder.GetNumber();

			lpPiece->m_aiValue[0]	=	iTargetX;
			lpPiece->m_aiValue[1]	=	iTargetY;
		}
		else
		if	(stricmp(lpstrCode,"시간지연")	==	0)
		{
			lpPiece->m_wPattern	=	eCP_delay_time;

			int	iDelayTime		=	decoder.GetNumber();

			lpPiece->m_aiValue[0]	=	iDelayTime;
		}
		else
		if	(stricmp(lpstrCode,"액션")	==	0)
		{
			lpPiece->m_wPattern	=	eCP_use_action;

			char	*lpstrAction=	decoder.GetWord();
			int		iAction		=	-1,i;

			for	(i=0;i<100;i++)
			{
				if	(stricmp(lpstrAction,_ms("액션%d",i))==0)
				{
					iAction	=	i;
					break;
				}
			}
			
			if	(iAction	==	-1)
			{
				cMSG::Put("액션 패턴 등록 에러!","'%s' 라는 액션은 없습니다.",lpstrAction);

				return;
			}

			char	*lpstrDirect=	decoder.GetWord();

			int		iDirect	=	-1;

			for	(i=0;i<8;i++)
			{
				if	(stricmp(g_str8Direct[i],lpstrDirect)==0)
				{
					iDirect	=	i;
					break;
				}
			}

			if	(iDirect	==	-1)
			{
				cMSG::Put("액션 패턴 등록 에러!","'%s' 라는 방향은 없습니다.",lpstrDirect);

				return;
			}

			int	iActionCount		=	decoder.GetNumber();
			decoder.GetWord();

			int	iFPS				=	decoder.GetNumber();
			decoder.GetWord();

			lpPiece->m_aiValue[0]	=	iAction;
			lpPiece->m_aiValue[1]	=	iDirect;
			lpPiece->m_aiValue[2]	=	iActionCount;
			lpPiece->m_aiValue[3]	=	iFPS;
		}
		else
		if	(stricmp(lpstrCode,"지역스킬사용")	==	0)
		{
			lpPiece->m_wPattern	=	eCP_use_local_skill;

			char	*lpstrSkill	=	decoder.GetStream();

			int		iSkill		=	cSKILL::GetIndex(lpstrSkill,0xffff);

			if	(iSkill	==	0xffff)
			{
				cMSG::Put("지역스킬사용 패턴 등록 에러!","'%s' 스킬을 찾을 수 없습니다.",lpstrSkill);

				return;
			}

			int		iX1		=	decoder.GetNumber();
			int		iY1		=	decoder.GetNumber();
			int		iX2		=	decoder.GetNumber();
			int		iY2		=	decoder.GetNumber();

			lpPiece->m_aiValue[0]=	iSkill;
			lpPiece->m_aiValue[1]=	iX1;
			lpPiece->m_aiValue[2]=	iY1;
			lpPiece->m_aiValue[3]=	iX2;
			lpPiece->m_aiValue[4]=	iY2;
		}
		else
		if	(stricmp(lpstrCode,"스킬사용")	==	0)
		{
			lpPiece->m_wPattern	=	eCP_use_skill;

			char	*lpstrSkill	=	decoder.GetStream();

			int		iSkill		=	cSKILL::GetIndex(lpstrSkill,0xffff);

			if	(iSkill	==	0xffff)
			{
				cMSG::Put("스킬사용 패턴 등록 에러!","'%s' 스킬을 찾을 수 없습니다.",lpstrSkill);

				return;
			}

			char	*lpstrLevel	=	decoder.GetWord();

			if	(stricmp(lpstrLevel,"레벨")	!=	0)
			{
				cMSG::Put("스킬사용 패턴 등록 에러!","레벨 이라는 단어가 있어야 할 위치입니다.");

				return;
			}


			int		iLevel		=	decoder.GetNumber();

			if	(iLevel	<	0	||	iLevel	>	100)
			{
				cMSG::Put("스킬사용 패턴 등록 에러!","%d 레벨을 설정 할 수 없습니다.",iLevel);

				return;
			}

			lpPiece->m_aiValue[0]=	iSkill;
			lpPiece->m_aiValue[1]=	iLevel;
		}
		else
		if	(stricmp(lpstrCode,"무적상태")	==	0)
		{
			lpPiece->m_wPattern	=	eCP_set_invincible;

			char	*lpstrOnOff	=	decoder.GetWord();

			if	(stricmp(lpstrOnOff,"on")	==	0)
				lpPiece->m_aiValue[0]	=	0;
			else
			if	(stricmp(lpstrOnOff,"off")	==	0)
				lpPiece->m_aiValue[0]	=	1;
			else
			{
				cMSG::Put("무적상태 패턴 등록 에러!","'%s' 라는 텍스트의 의미를 알 수 없습니다.",lpstrOnOff);

				return;
			}
		}
		else
		if	(stricmp(lpstrCode,"패턴호출")	==	0)
		{
			lpPiece->m_wPattern		=	eCP_call_pattern;

			int		iActiveChance	=	decoder.GetNumber();
			char	*lpstrPattern	=	decoder.GetStream();

			int		iPatternIndex	=	s_editPattern.getPatternIndexByName(lpstrPattern);

			if	(iPatternIndex	==	0xffff)
			{
				cMSG::Put("패턴호출 패턴 등록 에러!","'%s' 라는 패턴을 찾을 수 없습니다.",lpstrPattern);

				return;
			}

			if	(_bIsModifySelectPattern	&&	iPatternIndex	==	s_iSelectPattern)
			{
				cMSG::Put("패턴호출 패턴 등록 에러!","자기 자신을 호출 할 수는 없습니다.",lpstrPattern);

				return;
			}

			lpPiece->m_aiValue[0]	=	iActiveChance;
			lpPiece->m_aiValue[1]	=	iPatternIndex;
		}
		else
		{
			cMSG::Put("패턴 등록 에러!","'%s' 라는 패턴을 찾을 수 없습니다.",lpstrCode);
			return;
		}

		pattern.m_wCount++;
	}

	if	(pattern.m_wCount	==	0)
	{
		cMSG::Put("패턴 등록 에러!","패턴의 내용이 비어 있습니다.");
		return;
	}

	int	iSerial;

	if	(_bIsModifySelectPattern)
	{
		cCharacterPattern	*lpCurrent	=	s_editPattern.get(s_iSelectPattern);

		if	(!lpCurrent)
		{
			cMSG::Put("패턴 추가 에러!","먼저 패턴을 선택해 주세요.");

			return;
		}

		if	(s_editPattern.getPatternByName(pattern.m_strName,s_iSelectPattern))
		{
			cMSG::Put("패턴 추가 에러!","이미 존재하는 패턴 이름입니다.");

			return;
		}

		memcpy(lpCurrent,&pattern,sizeof(pattern));

		lpCurrent->m_wSerial	=	s_iSelectPattern;
		iSerial					=	s_iSelectPattern;
	}
	else
	{
		if	(s_editPattern.getPatternByName(pattern.m_strName))
		{
			cMSG::Put("패턴 추가 에러!","이미 존재하는 패턴 이름입니다.");

			return;
		}

		iSerial		=	s_editPattern.add(&pattern);
	}

	if	(iSerial	==	0xffff)
	{
		cMSG::Put("패턴 등록 에러!","더 이상의 패턴을 등록 할 수 없습니다.");
		return;
	}

	UpdatePatternList(hDlg,iSerial);

	if	(_bIsModifySelectPattern)
		cMSG::Put("패턴 수정 완료","완료");
	else
		cMSG::Put("패턴 등록","새로운 패턴을 등록 했습니다.");
}

void
cSetCharacterPattern::RemovePattern(HWND hDlg)
{
	if	(cMSG::YESNO("패턴 제거","선택한 패턴을 제거 하시겠습니까?")	==	IDNO)
		return;

	s_editPattern.remove(s_iSelectPattern);

	int	iSerial	=	s_editPattern.getNextPatternSerial(s_iSelectPattern);

	if	(iSerial==	0xffff)
		iSerial	=	s_editPattern.getPrePatternSerial(s_iSelectPattern);

	UpdatePatternList(hDlg,iSerial);
}

void
cSetCharacterPattern::FillPatternScript(HWND hDlg)
{
	cCharacterPattern	*lpSelect	=	s_editPattern.get(s_iSelectPattern);

	if	(lpSelect)
	{
		SetDlgItemText(hDlg,IDC_NAME,lpSelect->m_strName);

		cSTRING	script;

		for (int i=0;i<lpSelect->m_wCount;i++)
		{
			cCharacterPatternPiece	*lpPiece	=	&lpSelect->m_aPattern[i];

			switch(lpPiece->m_wPattern)
			{
				case	eCP_move			:
					script.Add("이동 %d,%d",lpPiece->m_aiValue[0],lpPiece->m_aiValue[1]);
					break;
				case	eCP_delay_time		:
					script.Add("시간지연 %d",lpPiece->m_aiValue[0]);
					break;
				case	eCP_use_action		:
					script.Add("액션 액션%d %s %d회 %dFPS",lpPiece->m_aiValue[0],g_str8Direct[lpPiece->m_aiValue[1]],lpPiece->m_aiValue[2],lpPiece->m_aiValue[3]);
					break;
				case	eCP_use_skill		:
					script.Add("스킬사용 \"%s\" 레벨 %d",g_aSkill[lpPiece->m_aiValue[0]].m_strName,lpPiece->m_aiValue[1]);
					break;

				case	eCP_use_local_skill	:
					script.Add("지역스킬사용 \"%s\" %d,%d  %d,%d",g_aSkill[lpPiece->m_aiValue[0]].m_strName,
							lpPiece->m_aiValue[1],lpPiece->m_aiValue[2],lpPiece->m_aiValue[3],lpPiece->m_aiValue[4]);
					break;

				case	eCP_set_invincible	:
				{
					char	*strOnOff[]	=	{"on","off"};

					script.Add("무적상태 %s",strOnOff[lpPiece->m_aiValue[0]]);
					break;
				}

				case	eCP_call_pattern	:
				{
					cCharacterPattern	*lpPattern	=	s_editPattern.get(lpPiece->m_aiValue[1]);

					char	*lpstrPattern;

					if	(lpPattern	==	NULL)
						lpstrPattern	=	"지워진 패턴!!";
					else
						lpstrPattern	=	lpPattern->m_strName;

					script.Add("패턴호출 %d%% \"%s\"",lpPiece->m_aiValue[0],lpstrPattern);
					
					break;
				}
			}

			script.Add("%c%c",0x0d,0x0a);
		}

		SetDlgItemText(hDlg,IDC_PATTERN_SCRIPT,script.String);
	}
	else
	{
		SetDlgItemText(hDlg,IDC_NAME,"");
		SetDlgItemText(hDlg,IDC_PATTERN_SCRIPT,"");
	}
}

void
cSetCharacterPattern::UpdatePatternList(HWND hDlg,int _iSelect)
{
	if	(s_editPattern.get(_iSelect)	==	NULL)
		_iSelect	=	s_editPattern.getFirstPatternSerial();

	s_iSelectPattern	=	_iSelect;

	s_listPattern.ResetItems();

	for (int i = 0; i<c_iCharacterPatternCountInField;i++)
	{
		cCharacterPattern	*lpPattern	=	s_editPattern.get(i);;

		if	(lpPattern	==	NULL)
			continue;

		s_listPattern.AddItem(0,_ms("%.3d",i),i);
		s_listPattern.AddItem(1,lpPattern->m_strName);
		s_listPattern.IncreaseLineCounter();
	}

	s_listPattern.selectByData(0,_iSelect);

	FillPatternScript(hDlg);
	Draw(hDlg);
}

void
cSetCharacterPattern::CopyEx(HWND hDlg)
{
	HWND	hPatternEx	=	GetDlgItem(hDlg,IDC_PATTERN_EX);

	int		iPattern	=	cCOMBOBOX::GetSelect(hPatternEx);

	char	strEx[512];

	sprintf(strEx,"%s%c%c",g_strPatternExample[iPattern*2+1],0x0d,0x0a);

	cMSG::Copy2ClipBoard(strEx);
}

LRESULT CALLBACK
cSetCharacterPattern::ProcDlg(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	
		case WM_INITDIALOG	:
			InitData(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDC_MODIFY_SELECT_PATTERN	:
					AddPattern(hDlg,TRUE);
					break;
					
				case	IDC_ADD:
					AddPattern(hDlg);
					break;

				case	IDC_REMOVE:
					RemovePattern(hDlg);
					break;

				case	IDOK		:
				{
					memcpy(&g_lpActmap->m_cpp,&s_editPattern,sizeof(s_editPattern));
					g_lpActmap->LostVirgin();
					EndDialog(hDlg,IDOK);

					g_MAPWND.Draw();
					break;
				}

				case	IDCANCEL	:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_PATTERN_HELP	:
					cHelpDialog::Open(hDlg,"QUEST_SCRIPT_HELP","패턴 도움말","pattern_help.txt");
					break;

				case	IDC_PATTERN_EX		:
				{
					switch (HIWORD(wParam))
                    {
						case	CBN_SELCHANGE	:
						{
							if	(LOWORD( wParam )	==	IDC_PATTERN_EX)
								CopyEx(hDlg);

							break;
						}
					}
					break;
				}

				default		:
					break;
			}
			break;
	}

	return (0L);

}

LRESULT CALLBACK
cSetCharacterPattern::ProcPatternList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listPattern.GetSelectData();

			if	(iSelect	==	0x7fffffff)
				break;

			UpdatePatternList(s_hWnd,iSelect);
			Draw(s_hWnd);
			break;
		}
	}

	return (0L);
}
