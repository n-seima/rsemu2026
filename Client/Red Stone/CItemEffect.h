#ifndef _classITEM_EFFECT_H
#define _classITEM_EFFECT_H

#include "cItem.H"
#include "CItemEffectDefine.H"
#include "commonClass.H"

class	cITEM_GENERATE_DATA_DECODER
{
public:
	int					getIndex(int serial);
	char*				getComment(cITEM_GENERATE_DATA *_lpData,cItem *_lpItem ,cBasicItem* _lpBasicItem = NULL);
	void				applyFormulaValue(int _iEffect,int _iOrder,  int* _iValue);

	static	CDecoder	s_decoder;
	static	cSTRING		s_string;
};

extern	cITEM_GENERATE_DATA_DECODER	g_itemGenerateDataDecoder;

#endif
