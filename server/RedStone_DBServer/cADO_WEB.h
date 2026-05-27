// cADO_WEB.h: interface for the cADO_WEB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADO_WEB_H__A6BF0825_C836_47E9_BDE2_616E31FD9244__INCLUDED_)
#define AFX_CADO_WEB_H__A6BF0825_C836_47E9_BDE2_616E31FD9244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "definePacketData.h"
#include <icrsint.h> 
#include "cADO_BASE.h"
#include "cPACKET_DBSERVER.h"

#define	dMSGTYPE_CHARACTER		1	//	캐릭터 생성,삭제, 로그인, 로그아웃
#define	dMSGTYPE_BANK			2	//	현금 구매한 아이템의 사용 관련

typedef struct BUYLIST
{
	char	ItemNo[32];
	int		Count;
	int		Sqn;
}stBuyList;

class cADO_WEB	:	public cADO_BASE
{
public:
	cADO_WEB();
	virtual ~cADO_WEB();

	stBuyList		buyList[10];
	int				buyListCount;

	void	__cdecl SendInfo(char * _id,WORD _type,char * _sname,char * _txt,...); // 현재 사용하지 않음. 2008.01.07
	//	구매 아이템 관련 정보도 여기서 처리할 것!!
	void	GetBuyList(char * _strId,DGMSG_BUYLIST * list);
	void	GetInfoOfItemList(DGMSG_PREMIUM_ITEM_PRICE_INFO * list);
	WORD	UseBuy(int _iSqn,char * _strId,char * _strServerName, char * _strAvaName, UTime * time,int _lev,WORD _job);
	void	insertPremiumItemToWebDB(G2D_BUY_PREMIUM_ITEM_TO_WEB_DB * _packet);

	//	게임내 당근 사용
	void	UseCarrot(GDMSG_USE_CARROT_LOG * rPacket,DGMSG_USE_CARROT_RESULT * sPacket);
	void	UseCarrot_OGP(GDMSG_USE_CARROT_LOG * rPacket,DGMSG_USE_CARROT_RESULT * sPacket);
};

class cADO_CARROT	:	public cADO_BASE
{
public:
	cADO_CARROT();
	virtual ~cADO_CARROT();

	int			GetCarrot_OGP(char * _strId, char * _strIp, char * _strToken);
	int			GetCarrot(char * _strId);	//	남은 당근 갯수 확인 
};

/*
*/
#endif // !defined(AFX_CADO_WEB_H__A6BF0825_C836_47E9_BDE2_616E31FD9244__INCLUDED_)
