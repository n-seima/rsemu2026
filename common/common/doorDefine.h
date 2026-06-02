#ifndef	_classDoorDefine_h
#define	_classDoorDefine_h

#include	"SFC.h"

#define	dMAX_DOOR_IN_MAP			64
#define	dMAX_DOOR_OBJECT_IN_MAP		(dMAX_DOOR_IN_MAP*2)
#define	dLINK_WITH_AREA_DOOR_COUNT	4

//
//	문 상태
enum
{
	eDS_CLOSE,
	eDS_OPEN,
	eDOOR_STATUS_COUNT,
};


class	cDoorObjectSimpleInfo
{
public:
	WORD	m_wX,m_wY;
	WORD	m_wObjectIndex,m_wObjectImage;
};

#endif