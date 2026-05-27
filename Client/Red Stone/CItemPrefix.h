/********************************************************************
	created:	2002/11/15
	created:	15:11:2002   18:51
	filename: 	C:\Work\Project Red Stone\Mangchi\cITEM_PREFIX.h
	file path:	C:\Work\Project Red Stone\Mangchi
	file base:	cITEM_PREFIX
	file ext:	h
	author:		Young-chan LEE
	
	purpose:	아이템 접두사
*********************************************************************/

#ifndef _classITEM_RARE_OPTION_
#define _classITEM_RARE_OPTION_

#include "SFC.H"
#include "cItem.H"
#include "CItemEffectDefine.H"
#include "commonClass.H"

class cITEM_PREFIX_MANAGER
{
public:
	int							m_iCount;
	cSTRING						m_string;
	CDecoder					m_decoder;


	int							getIndex(int _iSerial);
	char*						getComment(CItemPrefixInfo *_lpPrefixInfo,BOOL _bIsWantRange=FALSE);
	char*						getComment(cUniqueData *_lpUnique);
	char*						getComment(int _iPrefix);

	int							getPrefixCountByType(int _iType);
	int							getPrefixLevel(int _iPrefix);
	int							getPrefixByType(int _iType,int _iLevel);

	BOOL						isLevelUpAble(int _iItemKind,int _iPrefix,int *_lpiNextPrefix);

};

extern	cITEM_PREFIX_MANAGER	g_itemPrefix;
extern	cITEM_PREFIX			g_aBackupItemPrefix[dMAX_ITEM_PREFIX_COUNT];
extern	cITEM_PREFIX			g_aItemPrefix[dMAX_ITEM_PREFIX_COUNT];

#endif