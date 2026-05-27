// TrayIcon.h: interface for the cTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#pragma once



class cTrayIcon
{
public:
	cTrayIcon();
	cTrayIcon(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
	~cTrayIcon();

	//Create the tray icon
	BOOL create(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);

	//Change or retrieve the Tooltip text
	BOOL setTooltipText(LPCTSTR pszTooltipText);
	BOOL setTooltipText(UINT nID);
	CString getTooltipText() const;

	//Change or retrieve the icon displayed
	BOOL setIcon(HICON hIcon);
	BOOL setIcon(LPCTSTR lpIconName);
	BOOL setIcon(UINT nIDResource);
	BOOL setStandardIcon(LPCTSTR lpIconName);
	BOOL setStandardIcon(UINT nIDResource);
	HICON getIcon() const;
	void removeIcon();

	//Change or retrieve the window to send notification messages to
	BOOL setNotificationWnd(CWnd* pNotifyWnd);
	CWnd *getNotificationWnd() const;

protected:
	NOTIFYICONDATA	m_tnd;


	
};



