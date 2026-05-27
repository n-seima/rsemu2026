#ifndef _classGAME_OPENING_H_
#define	_classGAME_OPENING_H_

#include "CGame.H"

////////////////////////////////////////////////////////////////////////
//	게임 초기화(시작하자마자..)
////////////////////////////////////////////////////////////////////////
class	cGAME_OPENING	:	public CGame
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