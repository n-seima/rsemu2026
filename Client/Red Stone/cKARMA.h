//	개     요 : 트리거 시스템에 이은 카르마 시스템
//	
//	작성 날짜 : 2000.12.04

#ifndef _classKARMA_H
#define	_classKARMA_H

#include "cNEMO.H"

#define	dKARMA_GROUP_COUNT			64		//	카르마 그룹 최대 수치
#define	dKARMA_COUNT				256		//	카르마 최대 수치

#define	dTRIGGER_COUNT				256		//	트리거 최대 수치
#define	dREACTION_COUNT				256		//	반응 최대 수치

#define	dINSTANT_KARMA_GROUP		0		//	현재 맵에서만 적용되는 카르마 그룹
#define	dPROJECT_KARMA_GROUP		1		//	프로젝트 전체에서 적용되는 카르마 그룹

#define	IDC_KARMA_COMBO				1000
#define	IDC_KARMA_SPIN				2000
#define	IDC_KARMA_SPINBOX			3000
#define	IDC_KARMA_NUMBERBOX			4000
#define	IDC_KARMA_EDITBOX			5000

#define	dKARMA_LOCAL_KARMA_SET		0
#define	dKARMA_PROJECT_KARMA_SET	1

extern	int				g_iSelectKarma,g_iSelectTrigger,g_iSelectReaction,g_iLastTrigger,g_iLastReaction;
extern	int				g_iSelectTerm;

//	카르마가 이루어질 방아쇠
class cKARMA_TRIGGER
{
public:
	WORD		m_wItem;
	int			m_aValue[6];

				cKARMA_TRIGGER()	{	Reset();}

	void		Reset()
				{	memset(m_aValue,0,sizeof(m_aValue));
					m_wItem		=	0xffff;
				}
};

//	카르마의 반응
class cKARMA_REACTION
{
public:
	WORD		m_wItem;
	int			m_aValue[6];
	int			m_iStringLocate;

				cKARMA_REACTION()	{	Reset();	}

	void		Reset()
				{
					m_iStringLocate	=	0;
					m_wItem			=	0xffff;
					memset(m_aValue,0,sizeof(m_aValue));
				}

	char*		GetString();
};

#define	dKARMA_NAME_LENGTH	64

//	카르마 시스템
class cKARMA
{
public:
	int					m_iSerial;								//	카르마 시리얼
	int					m_iExcuteCount;							//	몇번이나 실행하냐?
	BOOL				m_isActive;								//	살아있냐?
	BOOL				m_isInfinity;
	BOOL				m_isAndTrigger,m_isExclusiveReaction;	//	조건이 엔드냐?
	int					m_iReactionCount,m_iTriggerCount;		//	조건의 수,반응의 수

	cKARMA_TRIGGER		*m_pTrigger;
	cKARMA_REACTION		*m_pReaction;

						cKARMA()
						{	m_pTrigger				=	NULL;
							m_pReaction				=	NULL;
							Reset();
						}

						~cKARMA()
						{	Reset();
						}

	void				Update();
	void				Reset();
	void				MakeBuffer();
};	//	class cKARMA

#define	dKARMA_GROUP_INFO_SIZE	76

//	카르마 그룹. 그룹단위로 관리하자.
class cKARMA_GROUP
{
public:
	int					m_iSerial;					//	카르마 그룹 시리얼
	BOOL				m_isEnable;					//	활성화 상태냐?
	int					m_iKarmaCount;				//	카르마의 수
	cKARMA				m_aKarma[dKARMA_COUNT];		//	카르마들

						cKARMA_GROUP()
						{	
							m_iKarmaCount	=	0;
							m_iSerial		=	0xffff;
							m_isEnable		=	FALSE;
						}

	void				Update();
	void				Reset();
};	//	class cKARMA_GROUP

class	cKARMA_SET
{
public:
	cKARMA_GROUP		m_aKarmaGroup[dKARMA_GROUP_COUNT];
	int					m_iGroupCount;

						cKARMA_SET()
						{
							m_iGroupCount	=	0;
						}

	void				Reset();

	void				Update();
};	//	class	cKARMA_SET

extern	cKARMA_SET		g_ProjectKarma,g_LocalKarma;

#endif