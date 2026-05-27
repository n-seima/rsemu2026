#ifndef _classGAME_INIT_H_
#define	_classGAME_INIT_H_

#include "CGame.H"

////////////////////////////////////////////////////////////////////////
//	게임 초기화(시작하자마자..)
////////////////////////////////////////////////////////////////////////
class	cGAME_INIT		:	public CGame
{
public:
	static	int			s_iDataLoadingStatus;
	static	cTHREAD		s_thLoading;
	static	cSPRITE		s_sprLoadImage;

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


	static	void		LoadData(void* _lpData);
};

#endif