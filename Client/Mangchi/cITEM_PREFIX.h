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
#include "cITEM.H"
#include "cDECODER.H"
#include "CItemEffectDefine.H"

class cITEM_PREFIX_OLD
{
public:
	class cTYPE
	{
	public:
		int			m_wType;
		char		m_strName[128];
	};

	WORD			m_wSerial;
	WORD			m_wDiscernmentCode;
	WORD			m_wType;
	short			m_aValue[2][2];//수치 1,2의 최대 최소치
	short			m_sValue3;//3번째 수치
	char			m_str1stPrefix[20],m_str2stPrefix[20];
	WORD			m_wDropLevel;
	WORD			m_wPrefixForm;//접두사 타입 0은 명사,1은 형용사

	DWORD			m_dwPrice;//가격
	WORD			m_wComputePriceMethod;	//	가격 계산 방법
	WORD			m_wPriceFactor;	//	곱할 것인가 더할 것인가

	BYTE			m_aEnableSet[4];	//	처음,두번째,세번째,덤-_-
	BYTE			m_aStickableItem[eIK_ENCHANT_ITEM+1];
};

class cITEM_PREFIX_OLD2
{
public:
	class cTYPE
	{
	public:
		int			m_wType;
		char		m_strName[128];
	};

	WORD			m_wSerial;
	WORD			m_wDiscernmentCode;
	WORD			m_wType;
	short			m_aValue[2][2];//수치 1,2의 최대 최소치
	short			m_sValue3;//3번째 수치
	char			m_str1stPrefix[20],m_str2stPrefix[20];
	WORD			m_wDropLevel;
	WORD			m_wPrefixForm;//접두사 타입 0은 명사,1은 형용사

	DWORD			m_dwPrice;//가격
	WORD			m_wComputePriceMethod;	//	가격 계산 방법
	WORD			m_wPriceFactor;	//	곱할 것인가 더할 것인가

	BYTE			m_aEnableSet[4];	//	처음,두번째,세번째,덤-_-
	BYTE			m_aStickableItem[dITEM_KIND_COUNT];
};

class	cITEM_PREFIX_MANAGER
{
	static	BOOL			s_isVirgin;

public:
	cSTRING					m_string;
	cDECODER				m_decoder;

	cDEN<cITEM_PREFIX>		m_den;


							cITEM_PREFIX_MANAGER();

	inline	void			init(){m_den.init(dMAX_ITEM_PREFIX_COUNT);}
	inline	void			reset(){m_den.reset();}
	inline	int				getCount(){return m_den.getCount();}

	inline	int				add(cITEM_PREFIX *_lpPrefix){return	m_den.add(_lpPrefix);}
	inline	BOOL			insert(int _iIndex,cITEM_PREFIX *_lpPrefix)	{return	m_den.insert(_iIndex,_lpPrefix);}

	inline	BOOL			remove(int _iIndex){return	m_den.remove(_iIndex);}
	inline	cITEM_PREFIX*	get(int _iIndex){return	m_den.get(_iIndex);}

	cITEM_PREFIX*			getByKind(int _iOption,int *_lpIndex);
	cITEM_PREFIX*			getIndexByDiscernmentCode(int _iDiscernmentCode);//식별 코드로 검색한다.
	cITEM_PREFIX*			findOverlappedData(char *_strPrefix1,char *_strPrefix2,WORD _wDiscernmentCode,int _iExcept=0xffff);
	void					resetColorizeEffect();	//	컬러라이즈 효과들 모두 초기화
	int						getPrefixType(char *_strPrefix);//_strPrefix라는 이름을 가진 접두사의 기본 타입을 찾는다.
	cITEM_PREFIX*			getPrefix(char *_strPrefix);//_strPrefix라는 이름을 가진 접두사의 기본 타입을 찾는다.

	int						getIndex(int _iSerial);
	char*					getComment(cITEM_PREFIX_INFO *_lpPrefixInfo);

};

extern	cITEM_PREFIX::cTYPE		g_aItemPrefixType[dMAX_ITEM_PREFIX_TYPE_COUNT];
extern	cITEM_PREFIX_MANAGER	g_itemPrefix;

#endif