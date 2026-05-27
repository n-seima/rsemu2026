#ifndef _classGAME_LOGO_H_
#define	_classGAME_LOGO_H_

#include "CGame.H"

////////////////////////////////////////////////////////////////////////
//	게임 로고
////////////////////////////////////////////////////////////////////////
class	cGAME_LOGO	:	public CGame
{
public:
	static	BOOL		Run();
	static	BOOL		Draw();

	static	BOOL		UpdateInit();
	static	BOOL		UpdateReady();
	static	BOOL		UpdateMain();
	static	BOOL		UpdateFinish();
	static	BOOL		UpdateClose();

	static	void		DrawInit();
	static	void		DrawReady();
	static	void		DrawMain();
	static	void		DrawFinish();
	static	void		DrawClose();

	static	BOOL		(*fpUpdate[dSTEP_COUNT])();
	static	void		(*fpDetailDraw[dSTEP_COUNT])();
};

#endif