#ifndef _ClassDIALOG_H
#define _ClassDIALOG_H

#include "cNEMO.H"

#define	dMAX_CONTENTS_COUNT		20
#define	dCONTENTS_TEXT_LENGTH	256
#define	dSPEECH_LENGTH			2048

class	CSpeech
{
public:
	WORD			m_wOwner,m_wContentsCount,m_wSpeechIndex,m_wDialogBoxSize;
	char			m_strText[dSPEECH_LENGTH];
	char			m_strContents[dMAX_CONTENTS_COUNT][dCONTENTS_TEXT_LENGTH];
	cRECT			m_arectSelectSpeech[dMAX_CONTENTS_COUNT];
	BOOL			m_bIsActive;

					CSpeech()	{reset();}
					~CSpeech()	{reset();}

	void			reset()
	{
		m_wContentsCount	=	0;
		m_wOwner			=	0xffff;
		m_wSpeechIndex		=	0xffff;
		m_bIsActive			=	FALSE;
	}

	void			close()
	{
		reset();
	}

	void			init(int _iOwner,int _iSpeechIndex,char *_lpstrText,int _iDialogBoxSize)
	{
		m_wOwner		=	_iOwner;
		m_wSpeechIndex	=	_iSpeechIndex;
		m_bIsActive		=	TRUE;
		m_wContentsCount=	0;
		m_wDialogBoxSize=	_iDialogBoxSize;

		strcpy(m_strText,_lpstrText);
	}

	void			addContent(char *_lpstrString)
	{
		m_bIsActive			=	TRUE;

		sprintf(m_strContents[m_wContentsCount],"<b>%d<b>) %s",m_wContentsCount+1,_lpstrString);

		m_wContentsCount++;
	}

	inline	char*	getSpeech()	{return	m_strText;}
	inline	char*	getSelectSpeech(int _iIndex)
	{
		if (_iIndex >= m_wContentsCount) return	NULL;

		return	m_strContents[_iIndex];
	}
};

#endif