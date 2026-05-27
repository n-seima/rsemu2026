#ifndef _classGuardianPost_H
#define _classGuardianPost_H

#include "cNEMO.H"
#include "CGamePlay.H"

#define	dMAX_GUARDIAN_POST_COUNT	256
#define	dBORDER_GUARDIAN_POST		20000

class		CGuardianPost
{
public:
	WORD			m_wSerial,m_wRegSerial;
	WORD			m_wImage;
	WORD			m_wAnm,m_wDirect,m_wFrame,m_wFrameCounter,m_wMaxFrame,m_wFPS;
	CPos			m_pos;

	void			init(int _iSerial,int _iRegSerial,int _iImage,int _iX,int _iY);
	void			reset();
	void			update();
	void			put();
};

class	CGuardianPostManager
{
public:
	WORD			m_wCount;
	CGuardianPost	m_aPost[dMAX_GUARDIAN_POST_COUNT];

	void			reset();
	int				getValidSlotIndex();
	BOOL			addPost(int _iX,int _iY,int _iImage,int _iRegSerial);
	void			update();
	BOOL			removePost(int _iRegSerial);
	int				getPostIndexByRegSerial(int _iRegSerial);
	int				getViewPost(cINDEX *_lpIndexes,cRECT *_lpRectViewport);
};

extern	CGuardianPostManager	g_gpManager;

#endif
