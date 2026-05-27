// TrayIcon.h: interface for the cTrayIcon class.
//////////////////////////////////////////////////////////////////////
#pragma once


class cTrayIcon
{
private:
	NOTIFYICONDATA	m_tnd;
	CArray<HICON, HICON> m_animateIconList;

	// 애니메이션 관련
	int m_iAnimationDelayTime;
	int m_iCurrentAnimationIcon;
	BOOL m_bStopAnimation;
	UINT m_uiSavedIcon;	
	HANDLE m_hThread;

private:
	BOOL stepAnimation();

	static UINT __stdcall animationThread(LPVOID _lpData);

public:
	cTrayIcon();
	cTrayIcon(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
	~cTrayIcon();
	
	BOOL create(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
	BOOL setTooltipText(TCHAR *_pszContent);														// 툴팁	
	BOOL showBalloon(TCHAR *_pszContent, TCHAR *_pszTitle, DWORD _dwIconType, UINT _uiTimeout);		// 말풍선

	BOOL setIcon(HICON hIcon);
	BOOL setIcon(LPCTSTR lpIconName);
	BOOL setIcon(UINT nIDResource);
	BOOL setStandardIcon(LPCTSTR lpIconName);
	BOOL setStandardIcon(UINT nIDResource);
	HICON getIcon() const;
	void removeIcon();

	BOOL setAnimationIconList(UINT _uiFirstIconID, UINT _uiLastIconID); 
	BOOL animationIcon(int _iAnimationDelayTime);
	void stopAnimation();

	//Change or retrieve the window to send notification messages to
	BOOL setNotificationWnd(CWnd* pNotifyWnd);
	CWnd *getNotificationWnd() const;
};



