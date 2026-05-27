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
#include "CDecoder.H"
#include "CItem.H"


#define	dMAX_ITEM_PREFIX_TYPE_COUNT			200
#define	dMAX_ITEM_PREFIX_COUNT				1024

class cPREFIX_EFFECT
{
public:
	int							m_iSerial;
	char						m_strReferenceText[256];
};

class cITEM_PREFIX_MANAGER
{
public:
	int							m_iCount;
	cSTRING						m_string;
	CDecoder					m_decoder;


	int							getIndex(int _iSerial);
	char*						getComment(CItemPrefixInfo *_lpPrefixInfo);
	char*						getComment(cUniqueData *_lpUnique);
};

extern	cITEM_PREFIX_MANAGER	g_itemPrefix;
extern	cITEM_PREFIX			g_aItemPrefix[dMAX_ITEM_PREFIX_COUNT];
extern	cITEM_PREFIX			g_aBackupItemPrefix[dMAX_ITEM_PREFIX_COUNT];

#endif