#ifndef	_windowExtendAPI_H
#define	_windowExtendAPI_H

#include	"SFC.H"
#include	"COMMCTRL.H"

class cListView
{
public:
	HIMAGELIST	m_hImage, m_hSmall;
	HWND		m_hWND;
	HINSTANCE	m_hInst;

				cListView();

	BOOL		init(HWND hwnd,HINSTANCE hinst,DWORD exStyle=0);

	void		close();
	void		addImage(char *iconName);
	void		addColumn(int index,int width,char *str,int _iSort=LVCFMT_LEFT);
	void		reset();
	void		addItem(int index,int subItem,char *str,int _iData=-1,int _iImage=-1);
	void		setItem(int index,int subItem,char *str,int _iData=-1,int _iImage=-1);
	void		delItem(int index);
	int			getSelectedItem();
	int			getSelectedItemLParam(BOOL *_lpbResult);

	int			getSelectedCount();
	int*		getSelectedItem(int *_lpCount);

	int			selectItem(int _iLine);
	int			getLineCount();

	void		delColumn(int index);
	int			getLParam(int _iLine,int _iRow,BOOL *_bIsSuccess);

	void		setTextColor(int red,int green,int blue);
	void		setTextColor(DWORD color);
	void		setBkColor(int red,int green,int blue);
	void		setBkColor(DWORD color);
	void		setColor(DWORD text,DWORD bk);

	void		setText(int subItem,int iItem,char *str);

//	컬럼을 초기화 하기 위해서 만들어 둡니다. - 지현 (08/05/06)
public:
	int			m_iColumnCount;
	void		resetColumn();	//	컬럼뿐만 아니라 데이터도 초기화 한다.
};

#endif