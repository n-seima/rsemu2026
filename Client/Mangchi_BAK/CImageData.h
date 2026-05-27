#ifndef _classIMAGE_DATA_H
#define _classIMAGE_DATA_H

#include "SFC.h"
#include "CImageDataDefine.H"
class	CClientSaveImageData	:	public CSaveImageIndexData
{
public:
	WORD			m_wSmellOfDeathImage,m_wMiniPetSummon;
	WORD			m_wRebirthImage,m_awRebirthMarkImage[5];
	WORD			m_wSoundOfLeadersBellImage,m_wSoundOfLeadersBellMarkImage;
};

class	CEffectDataManager	: public CClientSaveImageData
{
public:
	CEffectDataInfo		m_aInfo[dEFFECT_IMAGE_DATA_COUNT];

	BOOL				init();
};

extern	CEffectDataManager	g_em;

#endif


