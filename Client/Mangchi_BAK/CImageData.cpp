#include "CImageData.h"

CEffectDataManager	g_em;

BOOL
CEffectDataManager::init()
{
	cFILE	file;

	if (!file.Open("data/effect_data.dat","rb"))	return	ERRMSG("Error in CEffectDataManager::init","effect_data.dat");
	int i =0;
	file.Read(&i,4);
	file.Read(this,sizeof(CClientSaveImageData));
	file.Read(m_aInfo,sizeof(CEffectDataInfo)*m_iEffectCount);
	file.Close();

	return	TRUE;
}	//	CEffectDataManager::init()