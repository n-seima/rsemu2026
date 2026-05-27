#ifndef _classPACKET_CONTROLSERVER_H
#define _classPACKET_CONTROLSERVER_H

#include "cPACKET_BASE.h"

#pragma pack(1)

/////////////////////////////////////////////////////////
//	PACKET TYPE
/////////////////////////////////////////////////////////

//旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//早CONTROL SERVER < -- > Admin Tool
#define dACMSG_OPEN			0x2101
#define dACMSG_CLOSE		0x2102

struct	ACMSG_OPEN
{
	cMSG_BASE_TYPE	base;

	char	strName[64];
};


struct	ACMSG_CLOSE
{
	cMSG_BASE_TYPE	base;

	char	strName[64];
};

#pragma pack()

#endif	//	_classPACKET_CONTROLSERVER_H
