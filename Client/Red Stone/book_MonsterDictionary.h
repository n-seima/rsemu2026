//////////////////////////////////////////////////////////////////////////////////////////////////////////
//						몬스터 도감 클래스(2008 - 07 - 28) - 책인터페이스를 사용.
//																		- JBC
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef	_classBookMonsterDictionary_h
#define	_classBookMonsterDictionary_h

#include "CGamePlay.H"
#include "cItem.H"
#include "book_gpw.h"
#include "cJOB.H"

//책에 들어갈수 있는 최고의 몬스터 종류.
#define				dMAX_MONSTER_KIND		50

//출력 정보를 저장하기 위한 클래스 출력정보를 추가하고 싶으면 여기서 하자.
class cBookMonDicInfo
{
public:
	char*			m_pastrName[dMAX_MONSTER_KIND];					//이름
	CJobInfo*		m_paJobInfo[dMAX_MONSTER_KIND];					//잡정보. 이걸로 여러가지 정보를 알아냄.
	int				m_iaRegenerationCycle[dMAX_MONSTER_KIND];		//리젠 타임.
	WORD			m_waActorKind[dMAX_MONSTER_KIND];				//종류 인간형 언데드형 등등..
	int				m_iCount;										//배열의 유효 갯수를 저장..
	cJOB*			m_pJob;											//포인터로 동적할당하여 사용. 여기서도 정보를 알아냄..
	int				m_ipJobCount;									//동적할당한 갯수...
	WORD			m_waMoveSpeed[dMAX_MONSTER_KIND];				//이속.
	WORD			m_waAttackSpeed[dMAX_MONSTER_KIND];				//공속.
	DWORD			m_dwaExp[dMAX_MONSTER_KIND];					//몬스터가 주는 경험치..

	//WORD			m_waLevel[dMAX_MONSTER_KIND];					//기본 레벨.

	short			m_saStrength[dMAX_MONSTER_KIND];						//	힘			5 1,000
	short			m_saAgility[dMAX_MONSTER_KIND];						//	민첩성		5 1,000
	short			m_saConstitution[dMAX_MONSTER_KIND];					//	건강		5 1,000
	short			m_saWisdom[dMAX_MONSTER_KIND];						//	지혜		5 1,000
	short			m_saIntelligence[dMAX_MONSTER_KIND];					//	지식		5 1,000
	short			m_saCharisma[dMAX_MONSTER_KIND];						//	카리스마	5 1,000
	short			m_saLuck[dMAX_MONSTER_KIND];							//	운			5 1,000
	short			m_saSight[dMAX_MONSTER_KIND];						//	시야

	short			m_saMinDamage[dMAX_MONSTER_KIND];						//	최소 데미지
	short			m_saMaxDamage[dMAX_MONSTER_KIND];						//	최대 데미지
	short			m_saDefensivePower[dMAX_MONSTER_KIND];					//	방어력
};

class cBookMonDic	:	public cBookGPW , public cBookMonDicInfo
{
public:
	//책을 제어하기위한 변수.
	BOOL			m_bOpen;										//열려있는가?
	BOOL			m_bIsInit;										//초기화는 햇니?
	cBARMENU		m_bmButton , m_bmMotionButton;					//m_bmButton 회전 버턴 m_bmMotionButton 에니메이션 버턴.

	//몬스터 출력을 위한 정보.
	WORD			m_wDirect,m_wFrame , m_wFrameCounter , m_wMaxFrame , m_wFPS , m_wMotion;
	
	BOOL			isPopupMenuBook();
	BOOL			isOpened();
	void			update();
	int				getFocusIndex();
	void			drawPage();
	void			draw();
	inline int		getTotalPage(){return m_iCount * 2;}
	void			setData(cJOB *_jobData , int _ipJobCount);		//시작 로딩시에 잡데이터를 잃어드릴때 m_pJob 에 동적할당하여 데이터를 세팅해주는 함수.

	void			reset();										//초기화 함수. 맴버변수를 초기화 할때 쓰자.
	void			open();
	void			close();
	void			drawIndex();
	
	void			updateButton();
	void			SerchMonsterInMap();
	BOOL			isExistInStrArry(char **_ppstrName , char* _strSerch , int _nMaxDestArry);//이름으로 검색해서 이미존재하는지 여부를 판단하는 함수.
	
	~cBookMonDic();
	cBookMonDic();
	//void			update();
};

extern	cBookMonDic	g_gwBookMonDic;

#endif
