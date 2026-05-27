#ifndef	_classFireWork_h
#define	_classFireWork_h

#include "sfc.h"

const	int	cMaxFireWorkCount	=	4000;

class	cFIELD;
class	cACTOR;

class	cFireWorkItem
{
public:
	WORD				m_wSerial;

	WORD				m_wLastPosX,m_wLastPosY;
	int					m_iOwnerZoneSerial;
	DWORD				m_dwOwnerNameHashCode;

	WORD				m_wFireWorkType;
	WORD				m_wFireWorkUpkeepTime;
	WORD				m_wRemainFireWorkTime;
	WORD				m_wRemainFireWorkCount;
	WORD				m_wUniqueFieldSerial;

						cFireWorkItem()
						{
							m_wSerial	=	0xffff;
						}

	void				copy(cFireWorkItem *_lpBanner);
	void				reset();
	BOOL				updateFireWork();	//	TRUE 값을 리턴하면 이 넘은 죽여 버린다.

	BOOL				fireFireWork(int _iX,int _iY,int _iDestX,int _iDestY);
};

class	cFireWorkManager
{
public:
	cDen<cFireWorkItem>	m_den;

						cFireWorkManager()
						{
							m_den.init(cMaxFireWorkCount);
						}

						~cFireWorkManager()
						{
						}

	void				reset();
	cFireWorkItem*		get(int _iIndex);
	cFireWorkItem*		getByActor(int _iZoneSerial);
	int					add(cACTOR *_lpOwner,int _iKind,int _iUpkeepTime);
	BOOL				remove(int _iIndex);
	BOOL				fireFireWork(cACTOR *_lpOwner,int _iSerial,int _iX,int _iY,int _iDestX,int _iDestY);

	void				operateOncePerSecond();

};

extern	cFireWorkManager	g_fireWorkManager;

#endif