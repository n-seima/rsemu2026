#include "CActor.H"
#include "CDebugWindow.h"
//
//	말하기
void
CActor::talk(char *_lpText,int _iTime)
{
//	if	(isMonster())
//		return;
	m_wIsWarningMessage	=	FALSE;

	strncpy(m_strTalk,_lpText,sizeof(m_strTalk)-1);

	m_wTalkTime	=	_iTime;
}

//
//	말하기
void
CActor::warning(char *_lpText,int _iTime)
{
#ifdef	_DEBUG
	if(g_bIsMovieMode)
		return;
#endif
	WarningMessage(_lpText);

	return;

	m_wIsWarningMessage	=	TRUE;
	strncpy(m_strTalk,_lpText,sizeof(m_strTalk)-1);

	m_wTalkTime	=	_iTime;
}