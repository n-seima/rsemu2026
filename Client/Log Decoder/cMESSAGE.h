#ifndef _classMESSAGE_H
#define _classMESSAGE_H

#include "SFC.H"

class cMSG
{
public:
						cMSG();

	static	int 		YESNO(char *headmsg,char *mainmsg,...);
	static	int 		YESNOCANCEL(char *headmsg,char *mainmsg,...);
	static	void		Test(int num=-1);

	static	BOOL		Put	 (HWND hwnd,char *msg);
	static	BOOL		Put  (char *headmsg,char *mainmsg,...);
	static	BOOL		Put  (char *msg,...);
	static	BOOL		Output(char *msg,...);

	static	BOOL		Error(char *headmsg,char *mainmsg,...);
	static	BOOL		Error(char *msg,...);

	static	BOOL		DXError		(char *msg,HRESULT index);
	static	BOOL		DSError		(char *msg,HRESULT index);
	static	BOOL		DDError		(char *msg,HRESULT index);
	static	BOOL		D3DError	(char *msg,HRESULT index);
	static	BOOL		D3DRMError	(char *msg,HRESULT index);

};

extern	BOOL	Error();

#endif