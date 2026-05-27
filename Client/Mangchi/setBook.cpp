#include "setBook.h"
#include "resource.h"

HWND					cSetBook::s_hWnd		=	NULL;
cLISTCONTROL			cSetBook::s_listBook;	//	던젼 리스트
cLISTCONTROL			cSetBook::s_listCategory;	//	층별 정보 리스트
cLISTCONTROL			cSetBook::s_listPage;	//	층별 정보 리스트

cBookInfo				cSetBook::s_editBook;
cBookCategory			cSetBook::s_editCategory;
cBookPageInfo			cSetBook::s_editPage;
int						cSetBook::s_iSelectBook;
int						cSetBook::s_iSelectCategory;
int						cSetBook::s_iSelectPage;
HFONT					cSetBook::s_hFont;

BOOL
cSetBook::Open(HWND _hWnd)	//	시작
{
	if	(DialogBox( s_hInst, "BOOK",_hWnd,(DLGPROC)ProcDlg)	==	IDCANCEL)
		return	FALSE;

	DeleteObject(s_hFont);

	return	TRUE;
}

void
cSetBook::InitData(HWND _hDlg)
{
	HWND	hBookList,hCategoryList,hPageList;

	hBookList			=	GetDlgItem(_hDlg,IDC_BOOK_LIST);
	hCategoryList		=	GetDlgItem(_hDlg,IDC_CATEGORY_LIST);
	hPageList			=	GetDlgItem(_hDlg,IDC_PAGE_LIST);

	s_hWnd				=	_hDlg;

	s_iSelectBook		=	0;
	s_iSelectCategory	=	0;
	s_iSelectPage		=	0;

	//	바닥에 딸어지는 아이템 데이터/리스트
	{
		s_listBook.Init		(hBookList	,2		,FALSE	,ProcBookList);		//	2개의 컬럼
		s_listBook.AddColumn("Index" ,6*7 ,eLCOM_CENTER);
		s_listBook.AddColumn("Name" ,6*39 ,eLCOM_CENTER);
	}

	{
		s_listCategory.Init		(hCategoryList	,4		,FALSE	,ProcBookCategoryList);		//	2개의 컬럼
		s_listCategory.AddColumn("Index" ,6*6 ,eLCOM_CENTER);
		s_listCategory.AddColumn("Name" ,6*46 ,eLCOM_CENTER);
		s_listCategory.AddColumn("Number" ,6*6 ,eLCOM_CENTER);
		s_listCategory.AddColumn("Def" ,6*5 ,eLCOM_CENTER);
	}

	{
		s_listPage.Init		(hPageList	,6		,FALSE	,ProcBookPageList);		//	2개의 컬럼
		s_listPage.AddColumn("Page" ,6*6 ,eLCOM_CENTER);
		s_listPage.AddColumn("Content" ,6*21 ,eLCOM_NORMAL);
		s_listPage.AddColumn("Number" ,6*6 ,eLCOM_CENTER);
		s_listPage.AddColumn("Def" ,6*5 ,eLCOM_CENTER);
		s_listPage.AddColumn("Remove" ,6*7 ,eLCOM_CENTER);
		s_listPage.AddColumn("Trade" ,6*6 ,eLCOM_CENTER);
	}

	{
		HWND	hComboImage		=	GetDlgItem(_hDlg,IDC_PAGE_IMAGE);
		cCOMBOBOX::Add(hComboImage,"default",0);
	}

	{
		HWND	hPageText	=	GetDlgItem(_hDlg,IDC_PAGE_CONTENTS);

		s_hFont				=	CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, "바탕");

		SendMessage(hPageText,WM_SETFONT,(WPARAM)s_hFont, 0 );
	}

	UpdateBookList(_hDlg,0);

	Draw(_hDlg);
}

void
cSetBook::Draw(HWND hDlg)
{
	s_listBook.Draw();
	s_listCategory.Draw();
	s_listPage.Draw();
}

void
cSetBook::ApplyItemComment(HWND hDlg)
{
	char	strText[1024*10];

	GetDlgItemText(hDlg,IDC_ITEM_COMMENT,strText,1024*10-2);

	if	(strText	<=	0)
	{
		cMSG::Error("error!","Enter a book item description.");
		return;
	}

	if	(!g_book.setBookItemComment(s_iSelectBook,strText))
	{
		cMSG::Error("error!","You must select a book first.");
		return;
	}

	UpdateBookList(hDlg,s_iSelectBook);
	Draw(hDlg);
}

void
cSetBook::AddBook(HWND hDlg)
{
	char	strBookName[c_iBookNameLength];

	GetDlgItemText(hDlg,IDC_BOOK_NAME,strBookName,c_iBookNameLength-2);
	BOOL	bIsBlockToDestroy	=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_DESTROY_BOOK);
	BOOL	bIsBlockToTrade		=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_TRADE_BOOK);
	BOOL	bIsBlockToStore		=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_STORE_BANK_BOOK);

	if	(strBookName	<=	0)
	{
		cMSG::Error("error!","Enter the book name.");
		return;
	}

	if	(!g_book.addBook(strBookName,bIsBlockToDestroy,bIsBlockToTrade,bIsBlockToStore))
	{
		cMSG::Error("error!","No more books can be added.");
		return;
	}

	UpdateBookList(hDlg,g_book.m_iBookCount-1);
	Draw(hDlg);
}

void
cSetBook::ModifyBookName(HWND hDlg)
{
	char	strBookName[c_iBookNameLength];

	GetDlgItemText(hDlg,IDC_BOOK_NAME,strBookName,c_iBookNameLength-2);

	BOOL	bIsBlockToDestroy	=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_DESTROY_BOOK);
	BOOL	bIsBlockToTrade		=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_TRADE_BOOK);
	BOOL	bIsBlockToStore		=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_STORE_BANK_BOOK);


	if	(strBookName	<=	0)
	{
		cMSG::Error("error!","Enter the book name.");
		return;
	}

	if	(!g_book.modifyBookData(s_iSelectBook,strBookName,bIsBlockToDestroy,bIsBlockToTrade,bIsBlockToStore))
	{
		cMSG::Error("error!","You must select a book first.");
		return;
	}

	g_book.getBook(s_iSelectBook);

	UpdateBookList(hDlg,s_iSelectBook);
	Draw(hDlg);
}

void
cSetBook::RemoveBook(HWND hDlg)
{
	if	(!g_book.removeBook(s_iSelectBook))
	{
		cMSG::Error("error!","You must select a book first.");
		return;
	}

	UpdateBookList(hDlg,s_iSelectBook);
	Draw(hDlg);
}

void
cSetBook::ChangeBook(HWND hDlg,int _iBook)
{
	UpdateBookList(s_hWnd,_iBook);
	Draw(hDlg);
}

void
cSetBook::UpdateBookList(HWND hDlg,int _iSelect)
{
	s_iSelectBook	=	_iSelect;

	s_listBook.ResetItems();

	for (int i = 0; i<g_book.m_iBookCount;i++)
	{
		cBookInfo	*lpBook	=	&g_book.m_aBook[i];

		s_listBook.AddItem(0,_ms("%.3d",i+1),i);
		s_listBook.AddItem(1,lpBook->m_strBookName);
		s_listBook.IncreaseLineCounter();
	}

	cBookInfo	*lpBook	=	g_book.getBook(s_iSelectBook);

	if	(lpBook)
	{
		SetDlgItemText(hDlg,IDC_BOOK_NAME,lpBook->m_strBookName);

		if	(lpBook->m_pstrText	==	NULL)
			lpBook->setItemComment(" ");

		SetDlgItemText(hDlg,IDC_ITEM_COMMENT,lpBook->m_pstrText);

		CheckDlgButton(hDlg,IDC_IS_BLOCK_DESTROY_BOOK,lpBook->m_bf1IsBlockDestroy);
		CheckDlgButton(hDlg,IDC_IS_BLOCK_TRADE_BOOK,lpBook->m_bf1IsBlockTrade);
		CheckDlgButton(hDlg,IDC_IS_BLOCK_STORE_BANK_BOOK,lpBook->m_bf1IsBlockStoreBank);
	}
	else
	{
		SetDlgItemText(hDlg,IDC_BOOK_NAME,"");
		SetDlgItemText(hDlg,IDC_ITEM_COMMENT," ");

		CheckDlgButton(hDlg,IDC_IS_BLOCK_DESTROY_BOOK,FALSE);
		CheckDlgButton(hDlg,IDC_IS_BLOCK_TRADE_BOOK,FALSE);
		CheckDlgButton(hDlg,IDC_IS_BLOCK_STORE_BANK_BOOK,FALSE);
	}

	s_listBook.selectByData(0,_iSelect);

	UpdateCategoryList(hDlg,0);
}

void
cSetBook::AddCategory(HWND hDlg)
{
	char	strCategoryName[c_iBookCategoryNameLength];

	GetDlgItemText(hDlg,IDC_CATEGORY_NAME,strCategoryName,c_iBookCategoryNameLength-2);

	cBookInfo	*lpBook	=	g_book.getBook(s_iSelectBook);

	if	(!lpBook)
	{
		cMSG::Error("error!","Please select a book first.");
		return;
	}

	BOOL	bIsDefaultCategory	=	IsDlgButtonChecked(hDlg,IDC_IS_DEFAULT_CATEGORY);
	int		iValue;
	BOOL	bTemp;

	iValue	=	GetDlgItemInt(hDlg,IDC_CATEGORY_VALUE,&bTemp,FALSE);

	if	(bTemp	==	FALSE)
	{
		cMSG::Error("error!","Please enter the relevant number.");
		return;
	}

	if	(strlen(strCategoryName) ==	0)
	{
		cMSG::Error("error!","Please enter the category name.");
		return;
	}

	if	(!lpBook->addCategory(strCategoryName,bIsDefaultCategory,iValue))
	{
		cMSG::Error("error!","No more categories can be added.");
		return;
	}

	UpdateCategoryList(hDlg,lpBook->m_wCategoryCount-1);

	Draw(hDlg);
}

void
cSetBook::ModifyCategoryName(HWND hDlg)
{
	char	strCategoryName[c_iBookCategoryNameLength];

	GetDlgItemText(hDlg,IDC_CATEGORY_NAME,strCategoryName,c_iBookCategoryNameLength-2);

	cBookInfo	*lpBook	=	g_book.getBook(s_iSelectBook);

	if	(!lpBook)
	{
		cMSG::Error("error!","Please select a book first.");
		return;
	}

	BOOL	bIsDefaultCategory	=	IsDlgButtonChecked(hDlg,IDC_IS_DEFAULT_CATEGORY);
	int		iValue;
	BOOL	bTemp;

	iValue	=	GetDlgItemInt(hDlg,IDC_CATEGORY_VALUE,&bTemp,FALSE);

	if	(bTemp	==	FALSE)
	{
		cMSG::Error("error!","Please enter the relevant number.");
		return;
	}

	if	(strlen(strCategoryName) ==	0)
	{
		cMSG::Error("error!","Please enter the category name.");
		return;
	}
	
	if	(!lpBook->modifyCategoryName(s_iSelectCategory,strCategoryName,bIsDefaultCategory,iValue))
	{
		cMSG::Error("error!","Please select a category first.");
		return;
	}

	cBookCategory	*lpCategory	=	lpBook->getCategory(s_iSelectCategory);

	if	(bIsDefaultCategory)
		for (int i=0;i<lpCategory->m_iPageCount;i++)
			lpCategory->m_aPage[i].m_bf1IsDefaultPage	=	TRUE;

	UpdateCategoryList(hDlg,s_iSelectCategory);

	Draw(hDlg);
}

void
cSetBook::RemoveCategory(HWND hDlg)
{
	cBookInfo	*lpBook	=	g_book.getBook(s_iSelectBook);

	if	(!lpBook)
	{
		cMSG::Error("error!","Please select a book first.");
		return;
	}

	if	(!lpBook->removeCategory(s_iSelectCategory))
	{
		cMSG::Error("error!","You must select a category first.");
		return;
	}

	UpdateCategoryList(hDlg,s_iSelectCategory);
	Draw(hDlg);

}

void
cSetBook::ChangeCategory(HWND hDlg,int _iSelect)
{
	UpdateCategoryList(hDlg,_iSelect);
	Draw(hDlg);
}

void
cSetBook::UpdateCategoryList(HWND hDlg,int _iSelect)
{
	s_iSelectCategory	=	_iSelect;

	s_listCategory.ResetItems();

	cBookInfo	*lpBook	=	g_book.getBook(s_iSelectBook);

	if	(lpBook)
	{
		for (int i = 0; i<lpBook->m_wCategoryCount;i++)
		{
			cBookCategory	*lpCategory	=	lpBook->getCategory(i);

			s_listCategory.AddItem(0,_ms("%.2d",i+1),i);
			s_listCategory.AddItem(1,lpCategory->m_strCategoryName);
			s_listCategory.AddItem(2,_ms("%d",lpCategory->m_wValue));

			if	(lpCategory->m_wIsDefaultCategory)
				s_listCategory.AddItem(3,"O");
			else
				s_listCategory.AddItem(3,"X");


			s_listCategory.IncreaseLineCounter();
		}

		s_listCategory.selectByData(0,_iSelect);

		cBookCategory	*lpCategory	=	lpBook->getCategory(s_iSelectCategory);

		if	(lpCategory)
		{
			SetDlgItemInt(hDlg,IDC_CATEGORY_VALUE,lpCategory->m_wValue,FALSE);
			SetDlgItemText(hDlg,IDC_CATEGORY_NAME,lpCategory->m_strCategoryName);
			CheckDlgButton(hDlg,IDC_IS_DEFAULT_CATEGORY,lpCategory->m_wIsDefaultCategory);
		}
		else
		{
			SetDlgItemInt(hDlg,IDC_CATEGORY_VALUE,0,FALSE);
			SetDlgItemText(hDlg,IDC_CATEGORY_NAME,"");
			CheckDlgButton(hDlg,IDC_IS_DEFAULT_CATEGORY,FALSE);
		}
	}

	UpdatePageList(hDlg,0);
}

void
cSetBook::AddPage(HWND hDlg,BOOL _bIsInsert)
{
	cBookCategory	*lpCategory	=	g_book.getCategory(s_iSelectBook,s_iSelectCategory);

	if	(!lpCategory)
	{
		cMSG::Error("error!","Please select a category first.");
		return;
	}
	
	HWND	hComboImage		=	GetDlgItem(hDlg,IDC_PAGE_IMAGE);
	int		iImage			=	cCOMBOBOX::GetData(hComboImage);
	BOOL	bIsDefaultPage	=	IsDlgButtonChecked(hDlg,IDC_IS_DEFAULT_PAGE);
	BOOL	bIsBlockToTrade	=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_TRADE_PAGE);
	BOOL	bIsBlockToDestroy=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_DESTROY_PAGE);
	BOOL	bIsBlockToStoreBank	=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_STORE_BANK_PAGE);

	int		iValue;
	BOOL	bTemp;

	iValue	=	GetDlgItemInt(hDlg,IDC_PAGE_VALUE,&bTemp,FALSE);

	char	strPageContents[c_iMaxBookPageTextLength];

	GetDlgItemText(hDlg,IDC_PAGE_CONTENTS,strPageContents,c_iMaxBookPageTextLength-2);

	if	(bTemp	==	FALSE)
	{
		cMSG::Error("error!","Please enter the relevant number.");
		return;
	}

	if	(strlen(strPageContents) ==	0)
	{
		cMSG::Error("error!","Please enter the page content.");
		return;
	}

	int	iPage	=	-1;

	if	(_bIsInsert)
		iPage	=	s_iSelectPage;

	if	(!lpCategory->addPage(strPageContents,iImage,bIsDefaultPage,iValue,bIsBlockToTrade,bIsBlockToDestroy,bIsBlockToStoreBank,iPage))
	{
		cMSG::Error("error!","No more pages can be added.");

		return;
	}

	if	(_bIsInsert)
		iPage	=	s_iSelectPage;
	else
		iPage	=	lpCategory->m_iPageCount-1;

	UpdatePageList(hDlg,iPage);
	Draw(hDlg);
}

void
cSetBook::ModifyPage(HWND hDlg)
{
	cBookCategory	*lpCategory	=	g_book.getCategory(s_iSelectBook,s_iSelectCategory);

	if	(!lpCategory)
	{
		cMSG::Error("error!","Please select a category first.");
		return;
	}

	HWND	hComboImage		=	GetDlgItem(hDlg,IDC_PAGE_IMAGE);
	int		iImage			=	cCOMBOBOX::GetData(hComboImage);
	BOOL	bIsDefaultPage	=	IsDlgButtonChecked(hDlg,IDC_IS_DEFAULT_PAGE);
	BOOL	bIsBlockToTrade	=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_TRADE_PAGE);
	BOOL	bIsBlockToDestroy=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_DESTROY_PAGE);
	BOOL	bIsBlockToStoreBank	=	IsDlgButtonChecked(hDlg,IDC_IS_BLOCK_STORE_BANK_PAGE);

	int		iValue;
	BOOL	bTemp;

	iValue	=	GetDlgItemInt(hDlg,IDC_PAGE_VALUE,&bTemp,FALSE);

	char	strPageContents[c_iMaxBookPageTextLength];

	GetDlgItemText(hDlg,IDC_PAGE_CONTENTS,strPageContents,c_iMaxBookPageTextLength-2);

	if	(lpCategory->m_wIsDefaultCategory)
		bIsDefaultPage	=	TRUE;

	if	(bTemp	==	FALSE)
	{
		cMSG::Error("error!","Please enter the relevant number.");
		return;
	}

	if	(strlen(strPageContents) ==	0)
	{
		cMSG::Error("error!","Please enter the page content.");
		return;
	}

	if	(!lpCategory->modifyPage(s_iSelectPage,strPageContents,iImage,bIsDefaultPage,iValue,bIsBlockToTrade,bIsBlockToDestroy,bIsBlockToStoreBank))
	{
		cMSG::Error("error!","Please select a page first.");
		return;
	}

	UpdatePageList(hDlg,s_iSelectPage);
	Draw(hDlg);
}

void
cSetBook::RemovePage(HWND hDlg)
{
	cBookCategory	*lpCategory	=	g_book.getCategory(s_iSelectBook,s_iSelectCategory);

	if	(!lpCategory)
	{
		cMSG::Error("error!","Please select a category first.");
		return;
	}

	if	(!lpCategory->removePage(s_iSelectPage))
	{
		cMSG::Error("error!","Please select a page first.");
		return;
	}

	UpdatePageList(hDlg,s_iSelectPage);
	Draw(hDlg);
}

void
cSetBook::ChangePage(HWND hDlg,int _iPage)
{
	UpdatePageList(hDlg,_iPage);
	Draw(hDlg);
}

void
cSetBook::UpdatePageList(HWND hDlg,int _iSelect)
{
	s_iSelectPage	=	_iSelect;

	s_listPage.ResetItems();

	cBookCategory	*lpCategory	=	g_book.getCategory(s_iSelectBook,s_iSelectCategory);

	if	(lpCategory)
	{
		int	iPage	=	g_book.getPageCount(s_iSelectBook,s_iSelectCategory);

		for (int i = 0; i<lpCategory->m_iPageCount;i++)
		{
			cBookPageInfo	*lpPage	=	lpCategory->getPage(i);

			s_listPage.AddItem(0,_ms("%.2d/%.2d",i+1,iPage+1),i);
			s_listPage.AddItem(1,lpPage->getContents(18));
			s_listPage.AddItem(2,_ms("%d",lpPage->m_wValue));

			if	(lpPage->m_bf1IsDefaultPage)
				s_listPage.AddItem(3,"O");
			else
				s_listPage.AddItem(3,"X");

			if	(lpPage->m_bf1IsBlockDestroyPage)
				s_listPage.AddItem(4,"X");
			else
				s_listPage.AddItem(4,"O");

			if	(lpPage->m_bf1IsBlockTradePage)
				s_listPage.AddItem(5,"X");
			else
				s_listPage.AddItem(5,"O");

			s_listPage.IncreaseLineCounter();
			iPage++;
		}

		s_listPage.selectByData(0,_iSelect);

		cBookPageInfo	*lpPage	=	lpCategory->getPage(s_iSelectPage);

		HWND	hComboImage		=	GetDlgItem(hDlg,IDC_PAGE_IMAGE);
		
		if	(lpPage)
		{
			SetDlgItemInt(hDlg,IDC_PAGE_VALUE,lpPage->m_wValue,FALSE);
			CheckDlgButton(hDlg,IDC_IS_DEFAULT_PAGE,lpPage->m_bf1IsDefaultPage);
			CheckDlgButton(hDlg,IDC_IS_BLOCK_TRADE_PAGE,lpPage->m_bf1IsBlockTradePage);
			CheckDlgButton(hDlg,IDC_IS_BLOCK_DESTROY_PAGE,lpPage->m_bf1IsBlockDestroyPage);
			CheckDlgButton(hDlg,IDC_IS_BLOCK_STORE_BANK_PAGE,lpPage->m_bf1IsBlockStoreBank);

			SetDlgItemText(hDlg,IDC_PAGE_CONTENTS,lpPage->getContents());
			cCOMBOBOX::SelectByData(hComboImage,lpPage->m_wPageImage);
		}
		else
		{
			SetDlgItemInt(hDlg,IDC_PAGE_VALUE,0,FALSE);
			CheckDlgButton(hDlg,IDC_IS_DEFAULT_PAGE,FALSE);
			CheckDlgButton(hDlg,IDC_IS_BLOCK_TRADE_PAGE,FALSE);
			CheckDlgButton(hDlg,IDC_IS_BLOCK_DESTROY_PAGE,FALSE);
			CheckDlgButton(hDlg,IDC_IS_BLOCK_STORE_BANK_PAGE,FALSE);

			SetDlgItemText(hDlg,IDC_PAGE_CONTENTS,"");
			cCOMBOBOX::SelectByData(hComboImage,0);
		}
	}

}

LRESULT CALLBACK
cSetBook::ProcDlg(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
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
				case	IDC_APPLY_ITEM_COMMENT	:
					ApplyItemComment(hDlg);
					break;
				case	IDC_ADD_BOOK	:
					AddBook(hDlg);
					break;
				case	IDC_MODIFY_BOOK_NAME	:
					ModifyBookName(hDlg);
					break;
				case	IDC_REMOVE_BOOK	:
					RemoveBook(hDlg);
					break;
				case	IDC_ADD_CATEGORY	:
					AddCategory(hDlg);
					break;
				case	IDC_MODIFY_CATEGORY	:
					ModifyCategoryName(hDlg);
					break;
				case	IDC_REMOVE_CATEGORY	:
					RemoveCategory(hDlg);
					break;
				case	IDC_INSERT_PAGE		:
					AddPage(hDlg,TRUE);
					break;
				case	IDC_ADD_PAGE	:
					AddPage(hDlg);
					break;
				case	IDC_MODIFY_PAGE	:
					ModifyPage(hDlg);
					break;
				case	IDC_REMOVE_PAGE	:
					RemovePage(hDlg);
					break;

				case	IDOK		:
				case	IDCANCEL	:
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
cSetBook::ProcBookList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listBook.GetSelectData();

			if (iSelect	==	0x7fffffff)
				break;

			ChangeBook(s_hWnd,iSelect);
			Draw(s_hWnd);
			break;
		}
	}

	return (0L);
}

LRESULT CALLBACK
cSetBook::ProcBookCategoryList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listCategory.GetSelectData();

			if (iSelect	==	0x7fffffff)
				break;

			ChangeCategory(s_hWnd,iSelect);
			Draw(s_hWnd);
			break;
		}
	}

	return (0L);
}

LRESULT CALLBACK
cSetBook::ProcBookPageList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listPage.GetSelectData();

			if	(iSelect	==	0x7fffffff)
				break;

			ChangePage(s_hWnd,iSelect);
			Draw(s_hWnd);
			break;
		}
	}

	return (0L);
}