#include "trapSkill.h"
#include "cDecoder.h"
#include "CImageData.h"

cTrapSkillManager	g_trapSkillManager;

CDecoderKeyword		l_aTrapEffect[]	=
{
	{"불대미지"			,eTSE_FIRE_DAMAGE			},
	{"물대미지"			,eTSE_WATER_DAMAGE			},
	{"대지대미지"		,eTSE_EARTH_DAMAGE			},
	{"바람대미지"		,eTSE_WIND_DAMAGE			},
	{"빛대미지"			,eTSE_LIGHT_DAMAGE			},
	{"어둠대미지"		,eTSE_DARK_DAMAGE			},
	{"물리대미지"		,eTSE_PHYSICAL_DAMAGE		},
	{"장님"				,eTSE_BLIND					},
	{"얼음결정"			,eTSE_CLOD					},
	{"얼음"				,eTSE_FREEZE				},
	{"악몽"				,eTSE_NIGHTMARE				},
	{"전신마비"			,eTSE_PARALYSIS				},
	{"혼란"				,eTSE_CONFUSE				},
	{"공격력저하"		,eTSE_DECLINE_ATTACK_POWER	},
	{"방어력저하"		,eTSE_DECLINE_DEFENSIVE_POWER},
	{"명중률저하"		,eTSE_DECLINE_HIT_CHANCE	},
	{"회피율저하"		,eTSE_DECLINE_DODGE_CHANCE	},
	{"공격속도저하"		,eTSE_DECLINE_ATTACK_SPEED	},
	{"이동속도저하"		,eTSE_DECLINE_MOVE_SPEED	},
	{"집중력감소"		,eTSE_DECLINE_CONCENTRATION	},
	{"속성저항력저하"	,eTSE_DECLINE_MAGIC_RESISTANCE},
	{"저항력약화"		,eTSE_WEAKNING_RESISTANCE	},
	{"end"				,dCODE_END					}
};

CDecoderKeyword	l_aTrapHitEffectOuptuPart[]	=
{
	{	"타격영역"			,eHEOP_HIT_ZONE						},	//	혼란
	{	"발"				,eHEOP_FOOT							},	//	혼란
	{	"어깨"				,eHEOP_SHOULDER						},	//	어깨
	{	"머리위"			,eHEOP_ON_THE_HEAD					},	//	어깨
	{	""					,dCODE_END							}
};	//	CDecoderKeyword	l_aSkillExtraEffect[]

cTrapSkill*
cTrapSkillManager::getSkill(int _iIndex)
{
	if (_iIndex	< 0 || _iIndex > dMAX_TRAP_SKILL)
		return	NULL;

	if (m_aTrapSkill[_iIndex].m_wSerial	==	0xffff)
		return	NULL;

	return	&m_aTrapSkill[_iIndex];
}

//
//	기술을 초기화 한다.
BOOL
cTrapSkillManager::init(char *fn)
{
#ifdef	_IS_DEV_CLIENT
	CDecoder		reader;

	cTrapSkill		skill;
	DWORD			dwCode		=	0;
	char			*str;
	int				iValue;

	{
		m_iCount		=	0;

		for (int i=0;i<dMAX_TRAP_SKILL;i++)
			m_aTrapSkill[i].reset();
	}

	if (!reader.Upload(fn,NULL))
		return FALSE;

	while(dwCode	!=	dCODE_END)
	{
		char *keyword	=	reader.getWord();
		dwCode			=	reader.analCode(keyword);

		switch(dwCode)
		{
			case	dCODE_CLOSE		:
			{
				addSkill(&skill);
				skill.reset();
				continue;
			}

			case	dCODE_END		:
			case	dCODE_FINISH	:
			case	dCODE_OPEN		:
				continue;
		}

		if (STRICMP("함정",keyword)==0)
		{
			skill.m_wSerial	=	reader.getNumber();
			str				=	reader.getStream();

			strcpy(skill.m_strName,str);
			continue;
		}

		if (STRICMP("효과",keyword)==0)
		{
			str				=	reader.getWord();
			iValue			=	CDecoder::AnalCode(l_aTrapEffect,str);

			if (iValue==dCODE_UNDEFINE)
				EXIT("error in cTrapSkillManager::init(char *fn)","효과가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_wEffect	=	iValue;

			continue;
		}

		if (STRICMP("수치1",keyword)==0)
		{
			skill.m_sValue[0][0]	=	reader.getNumber();

			continue;
		}

		if (STRICMP("수치1per레벨",keyword)==0)
		{
			skill.m_sValue[0][1]	=	reader.getNumber();
			continue;
		}

		if (STRICMP("수치2",keyword)==0)
		{
			skill.m_sValue[1][0]	=	reader.getNumber();
			continue;
		}

		if (STRICMP("수치2per레벨",keyword)==0)
		{
			skill.m_sValue[1][1]	=	reader.getNumber();
			continue;
		}

		if (STRICMP("레벨만큼마법저항력저하",keyword)==0)
		{
			skill.m_bf1IsDeclineMagicResistanceBasedLevel	=	TRUE;
			continue;
		}
		if (STRICMP("레벨만큼명중률회피율저하",keyword)==0)
		{
			skill.m_bf1IsDeclineDodgeHitChanceBasedLevel	=	TRUE;
			continue;
		}
		if (STRICMP("레벨만큼마비",keyword)==0)
		{
			skill.m_bf1IsStunBasedLevel		=	TRUE;
			continue;
		}
		if (STRICMP("레벨만큼얼음결정",keyword)==0)
		{
			skill.m_bf1IsColdBasedLevel		=	TRUE;
			continue;
		}

		if (STRICMP("타격이미지",keyword)==0)
		{
			str						=	reader.getStream();
			skill.m_wHitImage		=	g_im.getEffectIndex(str);
			skill.m_bf4HitImageAnm	=	reader.getNumber();

			if (skill.m_wHitImage	==	0xffff)
				EXIT("error in cTrapSkillManager::init","타격이미지가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf1IsColdBasedLevel		=	TRUE;
			continue;
		}

		if (STRICMP("효과음",keyword)==0)
		{
			str						=	reader.getStream();
			strcpy(skill.m_strHitSound,str);
			continue;
		}

		if (STRICMP("타격이미지위치",keyword)==0)
		{
			str		=	reader.getWord();

			iValue	=	CDecoder::AnalCode(l_aTrapHitEffectOuptuPart,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in cTrapSkillManager::init","타격이미지위치가 이상합니다.\n\n%s-%s",fn,skill.m_strName);

			skill.m_bf4HitImagePos	=	iValue;
			continue;
		}

		EXIT("Undefined KEY WORD","%s's '%s'",skill.m_strName,keyword);

		break;
	}

	cFILE	file;

	file.Open("data/trapSkill.dat","wb");
	file.Write(g_trapSkillManager.m_aTrapSkill,sizeof(g_trapSkillManager.m_aTrapSkill));
	file.Close();
#else
	cFILE	file;

	if (!file.Open("data/trapSkill.dat","rb"))
		return	ERRMSG("Red Stone Loading Error","can not find 'data/trapSkill.dat' file");

	file.Read(g_trapSkillManager.m_aTrapSkill,sizeof(g_trapSkillManager.m_aTrapSkill));
	file.Close();
#endif
	return	TRUE;
}

BOOL
cTrapSkillManager::addSkill(cTrapSkill	*_lpSkill)
{
	for (int i=0;i<m_iCount;i++)
	{
		if (m_aTrapSkill[i].m_wSerial	==	_lpSkill->m_wSerial)
		{
			ERRMSG("error in cTrapSkillManager::addSkill","exist same serial\n\n%s/%s",_lpSkill->m_strName,m_aTrapSkill[i].m_strName);

			return	FALSE;
		}
	}

	memcpy(&m_aTrapSkill[_lpSkill->m_wSerial],_lpSkill,sizeof(cTrapSkill));

	m_iCount++;

	return	TRUE;
}