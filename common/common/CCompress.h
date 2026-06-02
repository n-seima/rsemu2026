/********************************************************************
	created:	2002/04/19
	created:	19:4:2002   14:32
	filename: 	C:\Work\Project Red Stone\Red Stone\cCOMPRESS.h
	file path:	C:\Work\Project Red Stone\Red Stone
	file base:	cCOMPRESS
	file ext:	h
	author:		
	
	purpose:	
*********************************************************************/

#ifndef	__cCOMPRESS_H_
#define	__cCOMPRESS_H_

#include "SFC.H"

BYTE*	CompressData(BYTE *_lpSourceData,int _iDataSize,int *_lpResultSize);	//	_lpSourceData를 압축해서 리턴한다.
BYTE*	DecompressData(BYTE *_lpSourceData,int _iDataSize,int *_lpResultSize);//	_lpSourceData의 압축을 풀어서 리턴한다.

#endif