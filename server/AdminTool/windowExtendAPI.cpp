#include	"windowExtendAPI.h"

cListView::cListView()
{
	m_hImage	=	NULL;
	m_hSmall	=	NULL;
	m_iColumnCount = 0;
}

void
cListView::close()
{	
	if (!IsWindow(m_hWND))
		return;

	if ( ListView_GetImageList( m_hWND, LVSIL_SMALL ) )
        ImageList_Destroy( ListView_GetImageList( m_hWND, LVSIL_SMALL ) );
	if ( ListView_GetImageList( m_hWND, LVSIL_NORMAL ) )
		ImageList_Destroy( ListView_GetImageList( m_hWND, LVSIL_NORMAL ) );
}

BOOL
cListView::init(HWND _hWND,HINSTANCE hinst,DWORD exStyle)
{	
	close();

	m_hWND	=	_hWND;
	m_hInst	=	hinst;

	if (!IsWindow(m_hWND)) 
		return cMSG::Error("리스트뷰 초기화 에러!!","윈도우나 이미지 생성이 안됩니다.");

	if (exStyle)
		ListView_SetExtendedListViewStyle(m_hWND,exStyle);

	return TRUE;
}

void
cListView::addImage(char *iconName)
{
	if (!IsWindow(m_hWND))
		return;

	if (!m_hImage || !m_hSmall)
	{
		m_hImage	=	ImageList_Create( 32, 32, ILC_COLOR8 | ILC_MASK, 4, 1 ); 
		m_hSmall	=	ImageList_Create( 16, 16, ILC_COLOR8 | ILC_MASK, 4, 1 );

		ListView_SetImageList( m_hWND, m_hImage, LVSIL_NORMAL );
		ListView_SetImageList( m_hWND, m_hSmall, LVSIL_SMALL );
	}

	ImageList_AddIcon( m_hImage, LoadIcon( m_hInst, iconName ) );
    ImageList_AddIcon( m_hSmall, LoadIcon( m_hInst, iconName ) );
}

void
cListView::addColumn(int index,int width,char *str,int _iSort)
{	
	if (!IsWindow(m_hWND))
		return;

	LV_COLUMN	col;

    col.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ; 
    col.fmt		= _iSort;
    col.cx		= width; 

	col.pszText = str; 
	col.iSubItem= index;
	ListView_InsertColumn( m_hWND, index, &col );
	++m_iColumnCount;
}

void
cListView::reset()
{	
	ListView_DeleteAllItems(m_hWND);
}


void
cListView::addItem(int index,int subItem,char *str,int _iData,int _iImage)
{
	if (!IsWindow(m_hWND))
		return;

	LV_ITEM   item;

	item.mask		= LVIF_TEXT;

	if (_iImage	!=	-1)
	{
		item.mask	|= LVIF_IMAGE;
		item.iImage	= _iImage;
	}

	if (_iData	!=	-1)
	{
		item.mask	|= LVIF_PARAM;
		item.lParam	= _iData;	
	}

	item.pszText	= str;
	item.iSubItem	= subItem;
	item.iItem		= index;

	ListView_InsertItem( m_hWND, &item );
}

void
cListView::setItem(int index,int subItem,char *str,int _iData,int _iImage)
{	
	if (!IsWindow(m_hWND))
		return;

	LV_ITEM   item;

	item.mask		= LVIF_TEXT;

	if (_iImage	!=	-1)
	{
		item.mask	|= LVIF_IMAGE;
		item.iImage	= _iImage;
	}

	if (_iData	!=	-1)
	{
		item.mask	|= LVIF_PARAM;
		item.lParam	= _iData;	
	}

	item.pszText	= str;
	item.iSubItem	= subItem;
	item.iItem		= index;

	ListView_SetItem( m_hWND, &item );
}

void
cListView::setTextColor(int red,int green,int blue)
{
	ListView_SetTextColor(m_hWND,RGB(red,green,blue));
}

void
cListView::setTextColor(DWORD color)
{
	ListView_SetTextColor(m_hWND,color);
}

void
cListView::setBkColor(int red,int green,int blue)
{
	ListView_SetBkColor(m_hWND,RGB(red,green,blue));
}

void
cListView::setBkColor(DWORD color)
{
	ListView_SetBkColor(m_hWND,color);
}

void
cListView::setColor(DWORD text,DWORD bk)
{
	setTextColor(text);
	setBkColor(bk);
}

void
cListView::setText(int iItem,int subItem,char *str)
{
	ListView_SetItemText(m_hWND,iItem,subItem,str);

}

void
cListView::delItem(int index)
{
	if (!IsWindow(m_hWND)) 
		return;

	ListView_DeleteItem(m_hWND, index);
}

VOID
cListView::delColumn(int index)
{
	if (!IsWindow(m_hWND)) 
		return;

	ListView_DeleteColumn(m_hWND, index);
}

int
cListView::getSelectedItem()
{	
	return ListView_GetNextItem(m_hWND,-1,LVNI_SELECTED);
}

int
cListView::getSelectedItemLParam(BOOL *_lpbResult)
{
	int	iSelectItem	=	getSelectedItem();

	if (iSelectItem	==	-1)
	{
		*_lpbResult	=	FALSE;
		return	-1;
	}

	return	getLParam(iSelectItem,0,_lpbResult);
}

int
cListView::getSelectedCount()
{	
	return ListView_GetSelectedCount(m_hWND);
}

int*
cListView::getSelectedItem(int *_lpCount)
{
	int count	=	getSelectedCount(),index	=	-1;

	*_lpCount	=	count;

	if (count<1)
		return NULL;
	
	static int *indexes	=	NULL;

	indexes		=	new int [count];
	count		=	0;

	while( (index = ListView_GetNextItem(m_hWND,index,LVNI_SELECTED)) > -1)
		indexes[count++]	=	index;

	return indexes;
}
int
cListView::getLineCount()
{	
	return ListView_GetItemCount(m_hWND);
}

int
cListView::getLParam(int _iLine,int _iRow,BOOL *_bIsSuccess)
{	
	LV_ITEM   item;

	item.mask		=	LVIF_PARAM;
    item.iItem		=	_iLine;
    item.iSubItem	=	_iRow;

	*_bIsSuccess	=	ListView_GetItem(m_hWND,&item);

	return	item.lParam;
}

int
cListView::selectItem(int _iLine)
{
	ListView_SetItemState(m_hWND,_iLine,LVIS_SELECTED,LVIS_SELECTED);

	return	TRUE;
}


/////////////////////////////////////////////
//
void
cListView::resetColumn()	//	컬럼뿐만 아니라 데이터도 초기화 한다.
{
	reset();

	while(m_iColumnCount >= 0)
	{
		delColumn(0);
		--m_iColumnCount;
	}
}