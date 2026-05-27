// cProfile.h: interface for the cProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPROFILE_H__3CE561AC_A013_4E5C_B513_CAE5E456DA8A__INCLUDED_)
#define AFX_CPROFILE_H__3CE561AC_A013_4E5C_B513_CAE5E456DA8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WINDOWS_
#include <windows.h>
#endif
#include <stdio.h>

#define dMAXPROFILE_COUNT		512

/*
#ifndef _CUSTOM_DEBUG
	#define dPROFILESTART(p,name)	p = new cProfile(name)
	#define dPROFILEEND()			delete		profile;
	#define dPROFILE_START(name)	profile->Begin(name)
	#define dPROFILE_END(name)		profile->End(name)
#else
*/
	#define dPROFILESTART(p,name)	(void(0))
	#define dPROFILEEND()			(void(0))
	#define dPROFILE_START(name)	(void(0))
	#define dPROFILE_END(name)		(void(0))
//#endif

class cProfileItem
{
private:
	char		name[128];
	int			starttime;		//	검사 시작한 시간
	BOOL		isCheckCount;	//	호출 횟수를 호가인할 것인가..
	int			count;			//	해당 함수가 호출된 횟수
	FILE	*	fp;
	
public:
	cProfileItem(){	memset(name,0,128);};
	~cProfileItem(){};
	int			GetCount(){			return count;			}
	char	*	GetName(){			return name;			}
	BOOL		GetisCheckCount(){	return isCheckCount;	}
	
	void	Init(FILE *f,char *n,BOOL isCCount = TRUE);
	void	Begin();
	void	End();
};

class cProfile  
{
private:
	char		name[128];
	FILE	*	fp;

public:
	cProfileItem	pItem[dMAXPROFILE_COUNT];

	cProfile(char *n,char *path);
	virtual ~cProfile(){	close();	}
	void	close();

	void	Begin(char *n);
	void	End(char *n);
};

#endif // !defined(AFX_CPROFILE_H__3CE561AC_A013_4E5C_B513_CAE5E456DA8A__INCLUDED_)
