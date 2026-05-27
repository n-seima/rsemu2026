#ifndef _classUpgradeShop_H_
#define	_classUpgradeShop_H_

#include "CGamePlay.H"


class	CUpgradeShop	:	public CGamePlay
{
private:

	int			m_iUpgradeWeaponItemSlot;			// 강화할 무기 슬롯
	int			m_iUpgradeItemSlot;					// 프리미엄 강화제 슬롯
	int			m_iUpgradeNormalItemSlot;			// 노멀 강화제 슬롯
	cRECT		m_rtUpgradeWeaponItemSlot;			// 강화할 무기 슬롯 사이즈
	cRECT		m_rtUpgradeItemSlot;				// 프리미엄 강화제 슬롯 사이즈
	cRECT		m_rtUpgradeNormalItemSlot;				// 일반 강화제 슬롯 사이즈~~~
	cRECT		m_rtUpgradeWindow;					// 강화창 사이즈.
	cRECT		m_rtCosmicPowerSlot;				// 코스믹 파워 아이템 사이즈	~~~~
	cRECT		m_rtNextLevelItemSlot;				// 다음 단계 아이템 슬롯 사이즈~~~

	int			m_iHeightRate	;					//높이 비율 (%)로 하셈..
	int			m_iSlotScale;						// 슬롯 스케일~~~~(%)로..하셈...
	int			m_iCosmicPowerItemSerial;					// 코스믹파워시리얼. 없으면 0xffff
public:
	int			m_iDragItem;
	BOOL		m_bUpgradePerfect;
	CUpgradeShop();
	
	void	init();	
	void	open();
	void	close();
	void	update();
	void	draw();
	void	drawText();

	void	drawToolTip();
	BOOL	isOpened();
	BOOL	isInSlot(cRECT* _rtSlot, int _iPosX, int _iPosY);
	int		getFocusInItem();

	int		getUpgradeWeaponItemSlot(){return m_iUpgradeWeaponItemSlot;}
	int		getUpgradeItemSlot(){return m_iUpgradeItemSlot;}
	int		getUpgradeNormalItemSlot(){return m_iUpgradeNormalItemSlot;}
	cRECT*	getUpgradeWindowRect(){return &m_rtUpgradeWindow;}
	
	void	setUpgradeWeaponItemSlot(int _iSlot){m_iUpgradeWeaponItemSlot = _iSlot;}
	void	setUpgradeItemSlot(int _iSlot){m_iUpgradeItemSlot = _iSlot;}
	void	setUpgradeNormalItemSlot(int _iSlot){m_iUpgradeNormalItemSlot = _iSlot;}
	
	void	operateDropItem();
	void	operateDragItemInUpgradeWindow();
	int		getWidthGap(int _iFirstWidth, int _iSecondWidth);			// 두 길이의 밸런스 좌표??를 구하는?흠..
	int 	getCosmicPowerItemSerial();					//	 코스믹파워아이템이 시리얼 리턴?? 09.08.28
	virtual ~CUpgradeShop();

};

extern	CUpgradeShop	g_gwUpgradeShop;

#endif