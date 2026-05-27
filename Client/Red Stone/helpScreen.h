#ifndef _classHelpScreen_h
#define	_classHelpScreen_h

#include "CGamePlay.h"

enum
{
	eHT_GAME,
	eHT_OX_QUIZ,
	eHT_WORD_QUIZ,
};

class	cHelpScreen	: public CGamePlay
{
private:
	int						m_iHelpPixelHeight;
	int						m_iSelectSimpleHelp;
	int						m_iHelpType,m_iHelpBoxHeight;
	BOOL					readSimpleText();

public:
	void					open();
	void					close();
	void					draw();

	void					update();
	BOOL					isOpened();
};

class	cSimpleHelpTextInfo
{
public:
	char	m_strTitle[64];
	char	*m_pstrText;

	cSimpleHelpTextInfo()
	{
		m_pstrText	=	NULL;
	}

	void	reset()
	{
		pKILL(m_pstrText);
	}
	void	setText(char *_lpstrText)
	{
		pKILL(m_pstrText);
		m_pstrText	=	new char [strlen(_lpstrText)+1];

		strcpy(m_pstrText,_lpstrText);
	}
};

extern	cSimpleHelpTextInfo		g_astrSimpleHelp[10];
extern	cHelpScreen				g_gwHelp;

#endif