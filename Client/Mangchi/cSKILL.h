#ifndef _classSKILL_H
#define _classSKILL_H

#include	"SFC.H"
#include	"CSkillDefine.H"
#include	"guild_skill_define.H"

class	cSKILL : public CSkillDefine
{
public:
						cSKILL()
						{
							Reset();
						}

	void				Reset()
						{
							strcpy(m_strName,"이름 없으.. --;;");

							m_wEnableJob				=	0xffff;
							m_wSerial					=	0xffff;
							m_wIconIndex				=	0xffff;
						}

	static	int			s_iSkillCount;
	static	int			s_iSkillGroupCount;

	static	BOOL		Init(char *fn);
	static	BOOL		SaveData(char *fn);
	static	BOOL		Add(cSKILL *skill);
	static	int			GetIndex(char *skillName,int job);
};

extern	cSKILL		g_aSkill[dMAX_SKILL];

#pragma	pack(2)

class	cABILITY
{
public:
	WORD			m_wSkill;
	short			m_sLevel;

					cABILITY()
					{
						reset();
					}

	void			reset()
					{
						m_wSkill			=	0xffff;
						m_sLevel			=	0;
					}

	void			set(cABILITY	*lpAbilityData)
					{
						reset();

						m_wSkill			=	lpAbilityData->m_wSkill;
						m_sLevel			=	lpAbilityData->m_sLevel;
					}
};

#pragma	pack()


#define	dSKILL_COUNT_FOR_GROUP		10
#define	dSKILL_GROUP_COUNT			160
#define	dSKILL_GROUP_NAME_LENGTH	22

class cSKILL_GROUP
{
public:
	WORD		m_wSerial;
	WORD		m_wJob;			//	가능 직업
	WORD		m_wSkilCount;	//	스킬의 수
	WORD		m_aSkill[dSKILL_COUNT_FOR_GROUP];
	char		m_strName[dSKILL_GROUP_NAME_LENGTH];

				cSKILL_GROUP()
				{
					m_wSerial	=	0xffff;
					m_wJob		=	0xffff;
					m_wSkilCount=	0;
				}

	static	int	Get(char *_strName,int _iJob);
};

class	CGuildSkill : public CGuildSkillDefine
{
public:
		CGuildSkill();
};

#define	dMAX_GUILD_SKILL_COUNT					150	//	최대 길드 스킬 수

class	CGuildSkillManager
{
public:
	int				m_iSkillCount;
	CGuildSkill		m_aSkills[dMAX_GUILD_SKILL_COUNT];
	
	BOOL			loadData(char *_lpstrFn=NULL);
	BOOL			saveData(char *_strFileName);
};

extern	CGuildSkillManager	g_gsManager;
extern	cSKILL_GROUP		g_aSkillGroup[dSKILL_GROUP_COUNT];

#endif