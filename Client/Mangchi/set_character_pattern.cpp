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
		s_listPattern.AddColumn("name"			,6*30	,eLCOM_CENTER);
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
		cMSG::Put("Error adding pattern!","The name is too short.");

		return;
	}

	while(1)
	{
		cCharacterPatternPiece	*lpPiece	=	&pattern.m_aPattern[pattern.m_wCount];

		char	*lpstrCode	=	decoder.GetWord();

		if	(stricmp(lpstrCode,"isFinishCode") == 0)
			continue;
		if	(stricmp(lpstrCode,"isEndCode") == 0)
			break;

		if	(stricmp(lpstrCode,"Move") == 0)
		{
			lpPiece->m_wPattern	=	eCP_move;

			int	iTargetX	=	decoder.GetNumber();
			int	iTargetY	=	decoder.GetNumber();

			lpPiece->m_aiValue[0]	=	iTargetX;
			lpPiece->m_aiValue[1]	=	iTargetY;
		}
		else
		if	(stricmp(lpstrCode,"Time delay") == 0)
		{
			lpPiece->m_wPattern	=	eCP_delay_time;

			int	iDelayTime		=	decoder.GetNumber();

			lpPiece->m_aiValue[0]	=	iDelayTime;
		}
		else
		if	(stricmp(lpstrCode,"action") == 0)
		{
			lpPiece->m_wPattern	=	eCP_use_action;

			char	*lpstrAction=	decoder.GetWord();
			int		iAction		=	-1,i;

			for	(i=0;i<100;i++)
			{
				if	(stricmp(lpstrAction,_ms("action%d",i))==0)
				{
					iAction	=	i;
					break;
				}
			}
			
			if	(iAction	==	-1)
			{
				cMSG::Put("Action pattern registration error!","There is no action called '%s'.",lpstrAction);

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
				cMSG::Put("Action pattern registration error!","There is no direction called '%s'.",lpstrDirect);

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
		if (stricmp(lpstrCode, "Use local skill") == 0)
		{
			lpPiece->m_wPattern	=	eCP_use_local_skill;

			char	*lpstrSkill	=	decoder.GetStream();

			int		iSkill		=	cSKILL::GetIndex(lpstrSkill,0xffff);

			if	(iSkill	==	0xffff)
			{
				cMSG::Put("Error registering local skill use pattern!", "Skill '%s' not found.", lpstrSkill);

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
		if	(stricmp(lpstrCode,"Use skill") == 0)
		{
			lpPiece->m_wPattern	=	eCP_use_skill;

			char	*lpstrSkill	=	decoder.GetStream();

			int		iSkill		=	cSKILL::GetIndex(lpstrSkill,0xffff);

			if	(iSkill	==	0xffff)
			{
				cMSG::Put("Skill usage pattern registration error!","Skill '%s' not found.",lpstrSkill);

				return;
			}

			char	*lpstrLevel	=	decoder.GetWord();

			if	(stricmp(lpstrLevel,"level")	!=	0)
			{
				cMSG::Put("Skill usage pattern registration error!","This is where the word level should be.");

				return;
			}


			int		iLevel		=	decoder.GetNumber();

			if	(iLevel	<	0	||	iLevel	>	100)
			{
				cMSG::Put("Skill usage pattern registration error!","Unable to set %d level.",iLevel);

				return;
			}

			lpPiece->m_aiValue[0]=	iSkill;
			lpPiece->m_aiValue[1]=	iLevel;
		}
		else
		if	(stricmp(lpstrCode,"Invulnerability") == 0)
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
				cMSG::Put("Invulnerability pattern registration error!","The meaning of the text '%s' is unknown.",lpstrOnOff);

				return;
			}
		}
		else
		if	(stricmp(lpstrCode,"Pattern call") == 0)
		{
			lpPiece->m_wPattern		=	eCP_call_pattern;

			int		iActiveChance	=	decoder.GetNumber();
			char	*lpstrPattern	=	decoder.GetStream();

			int		iPatternIndex	=	s_editPattern.getPatternIndexByName(lpstrPattern);

			if	(iPatternIndex	==	0xffff)
			{
				cMSG::Put("Pattern call pattern registration error!","The pattern called '%s' could not be found.",lpstrPattern);

				return;
			}

			if	(_bIsModifySelectPattern	&&	iPatternIndex	==	s_iSelectPattern)
			{
				cMSG::Put("Pattern call pattern registration error!","You cannot call yourself.",lpstrPattern);

				return;
			}

			lpPiece->m_aiValue[0]	=	iActiveChance;
			lpPiece->m_aiValue[1]	=	iPatternIndex;
		}
		else
		{
			cMSG::Put("Pattern registration error!","The pattern '%s' could not be found.",lpstrCode);
			return;
		}

		pattern.m_wCount++;
	}

	if	(pattern.m_wCount	==	0)
	{
		cMSG::Put("Pattern registration error!","The contents of the pattern are empty.");
		return;
	}

	int	iSerial;

	if	(_bIsModifySelectPattern)
	{
		cCharacterPattern	*lpCurrent	=	s_editPattern.get(s_iSelectPattern);

		if	(!lpCurrent)
		{
			cMSG::Put("Error adding pattern!","Please select a pattern first.");

			return;
		}

		if	(s_editPattern.getPatternByName(pattern.m_strName,s_iSelectPattern))
		{
			cMSG::Put("Error adding pattern!","The pattern name already exists.");

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
			cMSG::Put("Error adding pattern!","The pattern name already exists.");

			return;
		}

		iSerial		=	s_editPattern.add(&pattern);
	}

	if	(iSerial	==	0xffff)
	{
		cMSG::Put("Pattern registration error!","No more patterns can be registered.");
		return;
	}

	UpdatePatternList(hDlg,iSerial);

	if	(_bIsModifySelectPattern)
		cMSG::Put("Pattern modification complete","Complete");
	else
		cMSG::Put("Pattern registration","A new pattern has been registered.");
}

void
cSetCharacterPattern::RemovePattern(HWND hDlg)
{
	if (cMSG::YESNO("Remove pattern", "Do you want to remove the selected pattern?") == IDNO)
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
					script.Add("Move %d,%d",lpPiece->m_aiValue[0],lpPiece->m_aiValue[1]);
					break;
				case	eCP_delay_time		:
					script.Add("Time delay %d",lpPiece->m_aiValue[0]);
					break;
				case	eCP_use_action		:
					script.Add("Action Action%d %s %d times %dFPS",lpPiece->m_aiValue[0],g_str8Direct[lpPiece->m_aiValue[1]],lpPiece->m_aiValue[2],lpPiece->m_aiValue[ 3]);
					break;
				case	eCP_use_skill		:
					script.Add("Use skill \"%s\" level %d",g_aSkill[lpPiece->m_aiValue[0]].m_strName,lpPiece->m_aiValue[1]);
					break;

				case	eCP_use_local_skill	:
					script.Add("Use local skill \"%s\" %d,%d %d,%d",g_aSkill[lpPiece->m_aiValue[0]].m_strName,
							lpPiece->m_aiValue[1],lpPiece->m_aiValue[2],lpPiece->m_aiValue[3],lpPiece->m_aiValue[4]);
					break;

				case	eCP_set_invincible	:
				{
					char	*strOnOff[]	=	{"on","off"};

					script.Add("Invulnerability %s",strOnOff[lpPiece->m_aiValue[0]]);
					break;
				}

				case	eCP_call_pattern	:
				{
					cCharacterPattern	*lpPattern	=	s_editPattern.get(lpPiece->m_aiValue[1]);

					char	*lpstrPattern;

					if	(lpPattern	==	NULL)
						lpstrPattern	=	"Erased pattern!!";
					else
						lpstrPattern	=	lpPattern->m_strName;

					script.Add("Pattern call %d%% \"%s\"",lpPiece->m_aiValue[0],lpstrPattern);
					
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
					cHelpDialog::Open(hDlg, "QUEST_SCRIPT_HELP", "pattern help", "pattern_help.txt");
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
