#include	"cItemDefine.h"

int
cExtraItemDefine::getRemainTimeEfficency()
{
	CTimeInfo	curTime,endTime;

	curTime.update();

	endTime.m_wYear		=	(WORD)DecodeItemExpireFullYear(m_year,curTime.m_wYear);
	endTime.m_wMonth	=	m_month;
	endTime.m_wDay		=	m_day;
	endTime.m_wHour		=	m_hour;
	endTime.m_wMinute	=	m_minute;

	int	iCurTime		=	curTime.getMinuteValue(2004);
	int	iEndTime		=	endTime.getMinuteValue(2004);

	int	iRemainTime		=	iEndTime-iCurTime;
	int	iRemainMana		=	iRemainTime*100/dITEM_FULL_CHARGE_TIME;

//	※ 판매 장비 아이템의 마력 충전양의 -50%, 최대치는 +500%	
	iRemainMana			=	max(iRemainMana,-50);
	iRemainMana			=	min(iRemainMana,500);

	return	iRemainMana;
}

BOOL
cExtraItemDefine::updateCurrentEfficency()
{
//	※ 판매 장비 아이템의 기본 사용 기간은 열흘
//	※ 판매 장비 아이템의 사용 기간은 '마력 충전양'이라는 이름으로 관리하며 %로 표시. 잔여 시간이 열흘이면 100%로 표시. 그 이상이나 이하이면 % 계산하여 표시한다.																														
//	※ 판매 장비 아이템의 사용 기간이 0일 이하로 떨어지면 마력 충전양이 마이너스 수치가 된다.																														
//	※ 판매 장비 아이템의 마력 충전양의 -50%, 최대치는 +500%																														
//	
//	※ 판매 장비 아이템의 마력 충전양이 마이너스로 떨어지면 무기의 성능이 저하된다.	
//		0%~-10% : 모든 접두사의 성능이 50%로 떨어짐 (접두사에 붙어있는 모든 부수적인 수치가 50%로 감소함, 예외적인 경우는 따로 기술)
//		-11%~-49% : 모든 접두사의 효과가 사라짐
//		-50% : 모든 접두사의 효과가 사라지며 아이템의 기본 수치, 성능이 원본 아이템과 같아짐
//	※ 판매 장비 아이템의 마력 충전양이 +154% 이상이 되면 무기의 성능이 약간 좋아진다.	
//		+154%~+211% : 모든 접두사의 성능이 한단계 업그레이드 또는 10% 향상
//		+222%~+500% :  : 모든 접두사의 성능이 한단계 업그레이드 또는 15% 향상
//	
//	※ 판매 장비 아이템의 마력 충전양의 -50%, 최대치는 +500%	
//	
//	※ 마력 충전 키트를 사용했을 때 충전양이 마이너스라면 기존 충전양을 0%로  간주하여 충전함 (-50%에서 사용하면 +100%를 사용하면 +50%가 아니라 +100%가 됨)	
	int	iRemainMana		=	getRemainTimeEfficency();
	int	iOldEfficiency	=	m_sCurrentEfficiency;

//	m_year	=	5;m_month	=	1;m_day	=	5;m_hour	=	13;m_minute=	24;

	m_sCurrentEfficiency=	100;

	if (iRemainMana	<=	0	&&	iRemainMana	>=	-10)
		m_sCurrentEfficiency	=	50;
	if (iRemainMana	<=	-11	&&	iRemainMana	>=	-49)
		m_sCurrentEfficiency	=	0;
	if (iRemainMana	<=	-50)
		m_sCurrentEfficiency	=	-1;
	if (iRemainMana	>=	154	&&	iRemainMana	<=	221	)
		m_sCurrentEfficiency	=	110;
	if (iRemainMana	>=	222	&&	iRemainMana	<=	500	)
		m_sCurrentEfficiency	=	115;

	if (iOldEfficiency	!=	m_sCurrentEfficiency)
		return	TRUE;

	return	FALSE;
}

