#include "CNemo.h"
#include "CTitle.h"
#include "CMessage.h"
#include "CHero.h"


//
//	리셋
void
CTitle::reset()
{
	m_wSerial				=	0xffff;
	m_wRequireLevelValue	=	0;
	m_strName[0]			=	NULL;
	m_strExplain[0]			=	NULL;
	m_bf1IsRemoveAble		=	FALSE;
	m_bf1ExceptCH5Reward	=	FALSE;
	
	memset(m_aEffect,0xff,sizeof(m_aEffect));
	m_uiEnableJobMask		=	0;
	m_uiDefaultOwnJobMask	=	0;
}	//	CTitle::reset()

//
//	_iLevel 타이틀을 위해 필요한 레벨
int
CTitle::getRequireLevel(int _iLevel)
{
	if (m_wComputeRequireLevelMethod	==	eCRLM_LEVEL_VALUE	)	return	m_wRequireLevelValue;
	if (m_wComputeRequireLevelMethod	==	eCRLM_MULTIPLE_VALUE)	return	_iLevel*m_wRequireLevelValue;
	if (m_wComputeRequireLevelMethod	==	eCRLM_SQUARE_VALUE	)	return	_iLevel^m_wRequireLevelValue;

	return	0xffff;	
}	//	CTitle::getRequireLevel(int _iLevel)

//
//	타이틀이 적용되는 레벨 구하기
int
CTitle::getEnableLevel(int _iTitleLevel,int _iOwnersLevel)
{
	int	iLevel;

	for (iLevel=_iTitleLevel;iLevel>=1;iLevel--)
		if (_iOwnersLevel >= getRequireLevel(iLevel))	return	iLevel;

	return	0;
}	//	CTitle::getEnableLevel(int _iTitleLevel,int _iOwnersLevel)

//
//	타이틀이 적용되는 레벨 구하기
char*
CTitle::getTooltip(int _iTitleLevel,int _iOwnersLevel)
{
	int		i;

	static	cSTRING	s_strTooltip;

	s_strTooltip.Reset();

	s_strTooltip.Add("%s(<c:LTGREEN>Lv %d<n>)\n\n",m_strName,_iTitleLevel);

	int	iLevel	=	getEnableLevel(_iTitleLevel,_iOwnersLevel);

	if	(iLevel	<=	0)
	{
		s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_MINIMUM_REQUIRE_LEVEL_FOR_TITLE_EFFECT,getRequireLevel(1));
		s_strTooltip.Add("<c:LTRED>- %s<n>\n",dMSG_CAN_NOT_ADAPTIVE_EFFECT_CAUSED_TOO_LOW_LEVEL);

	}
	else
	{
		int	iCount	=	0;
		for (i=0;i<dMAX_TITLE_EFFECT_COUNT;i++)
		{
			CTitleEffectInfo	*lpTitleEffect	=	&m_aEffect[i];

			if	(lpTitleEffect->m_wEffect	==	0xffff)
				continue;

			char	*lpstrEffectComment		=	getEffectComment(lpTitleEffect->m_wEffect,(int *)&lpTitleEffect->m_aiValue[iLevel-1]);

			if	(iCount	==	0)
				s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_TITLE_EFFECT);

			iCount++;

			s_strTooltip.Add("- %s\n",lpstrEffectComment);
		}

		if	(iLevel	!=	_iTitleLevel)
		{
			s_strTooltip.Add("\n<c:LTCYAN><%s><n>\n",dMSG_LIMIT);
			s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_REAL_ADAPTIVE_LEVEL,iLevel);
			s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_REQUIRE_LEVEL,getRequireLevel(_iTitleLevel));
		}
	}

	s_strTooltip.Add("\n");
	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_COMMENT);
	s_strTooltip.Add("- %s",m_strExplain);

	return	s_strTooltip.String;
}

char*
CTitle::getEffectComment(int _iTitle,int *_lpiValues)
{
	int					i;
	CTitleEffectText	*lpHyperText	=	NULL;

	for (i=0;;i++)
	{
		if	(g_aTitleHyperText[i].m_iSerial	==	0xffff	)
			break;
		if	(g_aTitleHyperText[i].m_iSerial	==	_iTitle	)
		{
			lpHyperText	=	&g_aTitleHyperText[i];
			break;
		}
	}

	if	(!lpHyperText)
		return	NULL;

	static	cSTRING	s_strComment;

	s_strComment.Reset();

	if	(_iTitle	==	eTE_BARD)
	{
		int	iUpkeepTime	=	m_aEffect[0].m_aiValue[0][3];
		int	iValue		=	m_aEffect[0].m_aiValue[0][4];

		s_strComment.Add(lpHyperText->m_strReferenceText,iUpkeepTime,iValue);
		return	s_strComment.String;
	}

	if	(_iTitle	==	eTE_INCLINE_POWER)
	{
		if	(_lpiValues[0])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_STRENGTH,_lpiValues[0]);
		if	(_lpiValues[1])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_AGILITY,_lpiValues[1]);
		if	(_lpiValues[2])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_INTELLIGENCE,_lpiValues[2]);
		if	(_lpiValues[3])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_WISDOM,_lpiValues[3]);
		if	(_lpiValues[4])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_CHARISMA,_lpiValues[4]);
		if	(_lpiValues[5])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_LUCK,_lpiValues[5]);
		if	(_lpiValues[6])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_HEALTH,_lpiValues[6]);
		if	(_lpiValues[7])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_HP,_lpiValues[7]);
		if	(_lpiValues[8])
			s_strComment.Add("%s <c:LTYELLOW>+%d<n> ",dMSG_CP,_lpiValues[8]);

		return	s_strComment.String;
	}

	CDecoder		decoder;

	decoder.UploadBuffer(lpHyperText->m_strReferenceText,NULL);

	BOOL	bFirst	=	TRUE;
	BOOL	bIsAcquireSymbol=	FALSE;
	int		iLimitSkillLevel=	0;
	
	while(1)
	{
		bIsAcquireSymbol	=	FALSE;
		char	*lpStream	=	decoder.getStream('[');

		if (!lpStream && !bFirst)
			break;

		bFirst	=	FALSE;

		if	(lpStream)	
			s_strComment.Add(lpStream);

		if (decoder.m_isDone	)
			break;		//	다 읽어 들였다.

		if (decoder.getFirstChar() == '+')
		{
			decoder.increaseBP();
			bIsAcquireSymbol	=	TRUE;
		}
		
		int	iIndex		=	decoder.getNumber();
		int	iValue		=	_lpiValues[iIndex];

		lpStream		=	decoder.getStream(']');

		if	(bIsAcquireSymbol && iValue > 0 )
			s_strComment.Add("<c:LTYELLOW>+");
		else
			s_strComment.Add("<c:LTYELLOW>");

		switch(_iTitle)
		{
			case	eTE_DISTINGUISHER	:
			{
				iValue	=	g_hero.getLevel()*iValue/100;
				s_strComment.Add("%d<n>",iValue);
				break;
			}

			case	eTE_BANK_CLIENT	:	// 09.08.28 은행 칭호 수수료~~ 보이기 툴팁~
				switch(iIndex)
				{
				case 1:
					s_strComment.Add("%d.%.2d<n>",iValue/100,iValue%100);
					break;
				case 2:
					s_strComment.Add("%.1lf<n>",iValue/10.0);
					break;
				default:
					s_strComment.Add("%d<n>",iValue);
					break;
				}
					
				break;

			case	eTE_NATURAL_ENEMY	:
			{
				if	(iIndex	== 0)
					s_strComment.Add("%d.%.2d<n>",iValue/100,iValue%100);
				else
					s_strComment.Add("%d<n>",iValue);
				break;
			}

			case	eTE_COURTESY		:
			{
				s_strComment.Add("%d.%.1d<n>",iValue/10/10,(iValue/10)%10);
				break;
			}

			case	eTE_MONSTER_DISCERNER	:
			{
//	"감별 가능 레벨 [0]\n- 감별 가능 분류 [1]\n- 감별 가능 등급 [2]",
				int	iLimitLevel,iEnableKind,iLimitGrade;

				g_hero.getDiscermentMonsterInfo(&iLimitLevel,&iEnableKind,&iLimitGrade);

				if	(iIndex	== 0)
					s_strComment.Add("%d<n>",iLimitLevel);

				if	(iIndex	== 1)
				{
					if (iEnableKind & 1<<eJT_UNDEAD)
						s_strComment.Add("%s ",dMSG_UNDEAD_TYPE);
					if (iEnableKind & 1<<eJT_DEVIL)
						s_strComment.Add("%s ",dMSG_DEVIL_TYPE);
					if (iEnableKind & 1<<eJT_ANIMAL)
						s_strComment.Add("%s ",dMSG_ANIMAL_TYPE);
					if (iEnableKind & 1<<eJT_HOLY_BEAST)
						s_strComment.Add("%s ",dMSG_HOLY_BEAST_TYPE);
					if (iEnableKind & 1<<eJT_HUMAN)
						s_strComment.Add("%s ",dMSG_HUMAN_TYPE);

					if (iEnableKind	==	0)
						s_strComment.Add("%s ",dMSG_NOTHING);

					s_strComment.Add("<n>");
				}
				if	(iIndex	== 2)
				{
					if (iLimitGrade	==	-1)
						s_strComment.Add("%s <n>",dMSG_NOTHING);
					else
						s_strComment.Add("%s <n>",g_pstrMonsterGrade[iLimitGrade]);
				}
				break;
			}

			case	eTE_SKILL_MASTER:
			{
				if	(iIndex	== 2)
					s_strComment.Add("%d<n>",iLimitSkillLevel);

				if	(iIndex	== 1)
				{
					for (i=0;i<dPLAYER_JOB_COUNT;i++)
					{
						if	(isEnableJob(i))
						{
							s_strComment.Add("%s<n>",g_pstrHeroJobName[i]);
							break;
						}
					}
				}
				if	(iIndex	== 0)
				{
					int	iMasterSkillDifficultySum	=	0;

					for (i=0;i<dPLAYER_JOB_COUNT;i++)
					{
						if (isEnableJob(i))
						{
							iMasterSkillDifficultySum	=	g_hero.getMasterSkillDifficultySum(i);
							break;
						}
					}

					iMasterSkillDifficultySum	=	min(iMasterSkillDifficultySum,iValue);
					iLimitSkillLevel			=	iValue;

					s_strComment.Add("%d<n>",iMasterSkillDifficultySum);
				}
				break;
			}

			default					:
				s_strComment.Add("%d<n>",iValue);
				break;

		}

		if (decoder.m_isDone	)	break;		//	다 읽어 들였다.
	}
	
	return	s_strComment.String;
}