#include "setMainQuest.h"
#include "resource.h"

HWND					cSetMainQuest::s_hWnd		=	NULL;
cLISTCONTROL			cSetMainQuest::s_listChapter;	//	던젼 리스트
cLISTCONTROL			cSetMainQuest::s_listHighQuest;	//	층별 정보 리스트

cChapterInfo			cSetMainQuest::s_editChapter;
int						cSetMainQuest::s_iSelectChapter;
int						cSetMainQuest::s_iSelectHighQuest;

BOOL
cSetMainQuest::Open(HWND _hWnd)	//	시작
{
	if	(DialogBox( s_hInst, "QUEST_MAIN",_hWnd,(DLGPROC)ProcDlg)	==	IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cSetMainQuest::InitData(HWND _hDlg)
{
	HWND	hChapterList,hHighQuestList;

	hChapterList		=	GetDlgItem(_hDlg,IDC_CHAPTER_LIST);
	hHighQuestList		=	GetDlgItem(_hDlg,IDC_HIGH_QUEST_LIST);

	s_hWnd				=	_hDlg;

	s_iSelectChapter	=	0;
	s_iSelectHighQuest	=	0;
	
	//	바닥에 딸어지는 아이템 데이터/리스트
	{
		s_listChapter.Init		(hChapterList	,2		,FALSE	,ProcChapterList);		//	2개의 컬럼
		s_listChapter.AddColumn("Index"		,6*7	,eLCOM_CENTER);
		s_listChapter.AddColumn("제목"			,6*39	,eLCOM_CENTER);

		s_listHighQuest.Init		(hHighQuestList	,2		,FALSE	,ProcHighQuestList);		//	2개의 컬럼
		s_listHighQuest.AddColumn("Index"		,6*7	,eLCOM_CENTER);
		s_listHighQuest.AddColumn("이름"			,6*39	,eLCOM_CENTER);

	}

	UpdateChapterList(_hDlg,0);

	Draw(_hDlg);
}

void
cSetMainQuest::Draw(HWND hDlg)
{
	s_listChapter.Draw();
	s_listHighQuest.Draw();
}

void
cSetMainQuest::AddChapter(HWND hDlg)
{
	char	strChapterName[c_iChapterNameLength];

	GetDlgItemText(hDlg,IDC_CHAPTER_NAME,strChapterName,c_iChapterNameLength-2);

	if	(!g_chapterManager.addChapter(strChapterName))
	{
		cMSG::Error("error!","더 이상의 챕터를 추가 할 수 없습니다.");
		return;
	}

	UpdateChapterList(hDlg,g_chapterManager.m_iChapterCount-1);
	Draw(hDlg);
}

void
cSetMainQuest::ModifyChapterName(HWND hDlg)
{
	char	strChapterName[c_iChapterNameLength];

	GetDlgItemText(hDlg,IDC_CHAPTER_NAME,strChapterName,c_iChapterNameLength-2);

	if	(!g_chapterManager.modifyChapterName(s_iSelectChapter,strChapterName))
	{
		cMSG::Error("error!","선택한 챕터가 없습니다. 먼저 챕터를 선택해 주시기 바랍니다.");
		return;
	}

	UpdateChapterList(hDlg,s_iSelectChapter);
	Draw(hDlg);
}

void
cSetMainQuest::RemoveChapter(HWND hDlg)
{
	if	(!g_chapterManager.removeChapter(s_iSelectChapter))
	{
		cMSG::Error("error!","선택한 챕터가 없습니다. 먼저 챕터를 선택해 주시기 바랍니다.");
		return;
	}

	UpdateChapterList(hDlg,s_iSelectChapter);
	Draw(hDlg);
}

void
cSetMainQuest::ChangeChapter(HWND hDlg,int _iChapter)
{
	UpdateChapterList(hDlg,_iChapter);
	Draw(hDlg);
}

void
cSetMainQuest::UpdateChapterList(HWND hDlg,int _iSelect)
{
	s_iSelectChapter	=	_iSelect;

	s_listChapter.ResetItems();

	for (int i = 0; i<g_chapterManager.m_iChapterCount;i++)
	{
		cChapterInfo	*lpChapter	=	g_chapterManager.get(i);;

		if	(lpChapter	==	NULL)
			break;

		s_listChapter.AddItem(0,_ms("%.3d",i),i);
		s_listChapter.AddItem(1,lpChapter->m_strName);
		s_listChapter.IncreaseLineCounter();
	}

	s_listChapter.selectByData(0,_iSelect);

	UpdateHighQuestList(hDlg,0);

	cChapterInfo	*lpChapter	=	g_chapterManager.get(s_iSelectChapter);

	if	(lpChapter)
	{
		SetDlgItemText(s_hWnd,IDC_SELECT_CHAPTER_CONTENTS,lpChapter->m_strComment);
		SetDlgItemText(s_hWnd,IDC_SELECT_CHAPTER_EPILOG,lpChapter->m_strEpilogComment);
	}
}

void
cSetMainQuest::AddHighQuest(HWND hDlg)
{
	char	strHighQuestName[c_iHighQuestNameLength];

	GetDlgItemText(hDlg,IDC_HIGH_QUEST_NAME,strHighQuestName,c_iHighQuestNameLength-2);

	cChapterInfo	*lpChapter	=	g_chapterManager.get(s_iSelectChapter);

	if	(!lpChapter)
	{
		cMSG::Error("error!","먼저 챕터를 선택해 주시기 바랍니다.");
		return;
	}

	if	(!lpChapter->addHighQuest(strHighQuestName))
	{
		cMSG::Error("error!","현재 챕터에 더 이상의 상위 퀘스트를 추가 할 수 없습니다.");
		return;
	}

	UpdateHighQuestList(hDlg,lpChapter->m_iHighQuestCount-1);
	Draw(hDlg);
}

void
cSetMainQuest::ModifyHighQuestName(HWND hDlg)
{
	char	strHighQuestName[c_iHighQuestNameLength];

	GetDlgItemText(hDlg,IDC_HIGH_QUEST_NAME,strHighQuestName,c_iHighQuestNameLength-2);

	cChapterInfo	*lpChapter	=	g_chapterManager.get(s_iSelectChapter);

	if	(!lpChapter)
	{
		cMSG::Error("error!","먼저 챕터를 선택해 주시기 바랍니다.");
		return;
	}

	if	(!lpChapter->modifyHighQuestName(s_iSelectHighQuest,strHighQuestName))
	{
		cMSG::Error("error!","먼저 상위 퀘스트를 선택해 주시기 바랍니다.");
		return;
	}

	UpdateHighQuestList(hDlg,s_iSelectHighQuest);
	Draw(hDlg);
}

void
cSetMainQuest::ChangeHighQuest(HWND hDlg,int _iHighQuest)
{
	UpdateHighQuestList(hDlg,_iHighQuest);
}

void
cSetMainQuest::RemoveHighQuest(HWND hDlg)
{
	if	(!g_chapterManager.removeHighQuest(s_iSelectChapter,s_iSelectHighQuest))
	{
		cMSG::Error("error!","먼저 챕터와 상위 퀘스트를 선택해 주시기 바랍니다.");
		return;
	}

	UpdateHighQuestList(hDlg,s_iSelectHighQuest);
	Draw(hDlg);
}

void
cSetMainQuest::UpdateHighQuestList(HWND hDlg,int _iSelect)
{
	s_iSelectHighQuest	=	_iSelect;

	s_listHighQuest.ResetItems();

	cChapterInfo	*lpChapter	=	g_chapterManager.get(s_iSelectChapter);

	if	(lpChapter)
		for (int i = 0; i<lpChapter->m_iHighQuestCount;i++)
		{
			s_listHighQuest.AddItem(0,_ms("%.3d",i),i);
			s_listHighQuest.AddItem(1,lpChapter->m_strHighQuest[i]);
			s_listHighQuest.IncreaseLineCounter();
		}

	s_listHighQuest.selectByData(0,_iSelect);
}

void
cSetMainQuest::ApplyChapterContents(HWND hDlg)
{
	cChapterInfo	*lpChapter	=	g_chapterManager.get(s_iSelectChapter);

	if	(!lpChapter)
		return;

	char	strChapterComment[c_iChapterCommentLength];

	GetDlgItemText(s_hWnd,IDC_SELECT_CHAPTER_CONTENTS,strChapterComment,c_iChapterCommentLength-2);
	lpChapter->changeComment(strChapterComment);

	char	strChapterEpilogComment[c_iChapterEpilogCommentLength];

	GetDlgItemText(s_hWnd,IDC_SELECT_CHAPTER_EPILOG,strChapterEpilogComment,c_iChapterEpilogCommentLength-2);

	lpChapter->changeEpilogComment(strChapterEpilogComment);
}

LRESULT CALLBACK
cSetMainQuest::ProcDlg(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
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
				case	IDC_ADD_TO_CHAPTER		:
					ApplyChapterContents(hDlg);
					AddChapter(hDlg);
					break;
				case	IDC_MODIFY_CHAPTER_NAME		:
					ApplyChapterContents(hDlg);
					ModifyChapterName(hDlg);
					break;
				case	IDC_REMOVE_SELECT_CHAPTER	:
					ApplyChapterContents(hDlg);
					RemoveChapter(hDlg);
					break;
				case	IDC_ADD_HIGH_QUEST			:
					ApplyChapterContents(hDlg);
					AddHighQuest(hDlg);
					break;
				case	IDC_MODIFY_HIGH_QUEST_NAME	:
					ApplyChapterContents(hDlg);
					ModifyHighQuestName(hDlg);
					break;
				case	IDC_REMOVE_SELECT_HIGH_QUEST:
					ApplyChapterContents(hDlg);
					RemoveHighQuest(hDlg);
					break;

				case	IDCANCEL	:
				case	IDOK		:
					ApplyChapterContents(hDlg);
					EndDialog(hDlg,IDOK);
					break;

				default		:
					break;
			}
			break;
	}

	return (0L);
}

LRESULT CALLBACK
cSetMainQuest::ProcChapterList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listChapter.GetSelectData();

			if (iSelect	==	0x7fffffff)
				break;

			ApplyChapterContents(hDlg);
			ChangeChapter(hDlg,iSelect);
			Draw(hDlg);
			break;
		}
	}

	return (0L);
}

LRESULT CALLBACK
cSetMainQuest::ProcHighQuestList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listHighQuest.GetSelectData();

			if (iSelect	==	0x7fffffff)
				break;

			ApplyChapterContents(hDlg);
			ChangeHighQuest(hDlg,iSelect);
			Draw(hDlg);
			break;
		}
	}

	return (0L);
}