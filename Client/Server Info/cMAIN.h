#ifndef _classMAIN_H
#define _classMAIN_H

#include "SFC.H"
#include "cWINDOW.H"
#include "resource.h"

#define	dMAX_MAP_COUNT_IN_SERVER	2048
#define	dMAX_SERVER_COUNT			100

class cMAP_INFO
{
public:
	int		m_iSerial;
	int		m_iType;
	char	m_strName[64];
	int		m_iServerSerial;

	cMAP_INFO()
	{
		m_iSerial	=	0xffff;
	}
};

class cSERVER_INFO
{
public:
	int			m_iSerial;
	int			m_iCount;
	char		m_strName[64];
	char		m_strIP[20];
	int			m_iPort;
	cMAP_INFO	m_aMap[dMAX_MAP_COUNT_IN_SERVER];

				cSERVER_INFO()
				{// 
					m_iSerial	=	0xffff;
				}
};

class cMAIN
{
public:
	static	int					s_iServerCount;
	static	cSERVER_INFO		s_aServerList[dMAX_SERVER_COUNT];
	static	cLISTVIEW			s_lvMap;

	static	cWND				*pWND;
	static	cDIBWND				*pDIBWND;
	static	HWND				hWND;
	static	HINSTANCE			hINST;
	static	cSTRINGS			*s_pFiles;
	static	cTREEVIEW			s_treeMap;
	static	int					s_iSelectMap,s_iSelectServer;
	static	HTREEITEM			s_aTree[dMAX_SERVER_COUNT];

	static	BOOL				Init(HINSTANCE hInst);
	static	BOOL				InitDlg(HWND _hWnd);
	static	BOOL				ReadData(char *_lpstrFn);
	static	BOOL				SaveData(char *_strFn);
	static	BOOL				Save(HWND _hWnd);
	static	BOOL				Load(HWND _hWnd);
	static	BOOL				SortServerMapList(HWND hwnd);

	static	void				FillData(HWND _hDlg);
	static	BOOL				Exit();
	static	BOOL				Run();
	static	void				Resize();

	static	BOOL				AddServer(HWND _hDlg);	//	서버 추가
	static	BOOL				AddMap(HWND _hDlg);		//	맵 추가
	static	int					GetSelectServer(HWND _hDlg);

	static	BOOL				RemoveServer(HWND _hDlg);//	선택한 서버 제거
	static	BOOL				RemoveMap(HWND _hDlg);//	선택한 맵 제거

	static	BOOL				UpdateSelectServer(HWND _hDlg);	//	선택된 서버 업데이트
	static	BOOL				UpdateSelectMap(HWND _hDlg);	//	선택된 맵 업데이트

	static	int					FindValidMapSerial();	//	비어있는 맵 시리얼 검색
	static	cSERVER_INFO*		FindServer(char *_strName,int _iExceptServer=0xffff);
	static	cMAP_INFO*			FindMap(char *_strName,int _iExceptServer=0xffff,int _iExceptMap=0xffff);
	static	cMAP_INFO*			FindMap(int _iSerial,int _iExceptServer=0xffff,int _iExceptMap=0xffff);

	static	void				MakeMapTree(HWND _hWnd);
	static	void				UpdateMapTree(HWND _hWnd);	//	맵 트리 업데이트
	static	BOOL				SelectMap(HWND _hWnd);		//	맵 선택
	static	void				ExportMapList();
	static	void				FillMapList(HWND _hWnd);
	static	void				UpdateMapList();

	static	void				PopupMenu();
	static	LRESULT CALLBACK	ProcNameFilter(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static	LRESULT CALLBACK	DlgProc(HWND,UINT,WPARAM,LPARAM);


};

extern	BOOL	bRUNAPPLICATION,bACTIVEAPP;

#endif