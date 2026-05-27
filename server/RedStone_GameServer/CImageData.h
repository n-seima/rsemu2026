#ifndef	_classIMAGE_DATA_H
#define	_classIMAGE_DATA_H

#include "cIMAGE.H"
#include "CImageDataDefine.H"

#define	dBODY_COUNT									300

class	CImageDataManager	:	public CSaveImageIndexData
{
public:
	BOOL				init();	//	√±≠
	void				close();

	BOOL				loadJobBodyData();
	BOOL				loadEffect();

};

extern	cBODY					g_aBody[dBODY_COUNT];
extern	cBODY					g_aEffect[dEFFECT_IMAGE_DATA_COUNT];
extern	CImageDataManager		g_idm;

#endif