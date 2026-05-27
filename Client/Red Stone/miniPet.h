#ifndef _class_mini_pet_h
#define _class_mini_pet_h

#include "miniPetCommon.h"

class	cItem;

class	cMiniPet	: public cMiniPetCommon
{
	static	cSTRING	s_strTooltip;
public:
	int		getGrowBioFactor();
	int		getGrowEnergyFactor();
	int		getGrowMineralFactor();
	int		getMaxGrowFactor();

	char*	getName();
	char*	getToolTip();

	cMiniPetAbilityInfo*	getAbilityBySkill(int _iSkill);
	BOOL					isLevelUpAbleAbility(cMiniPetAbilityInfo	*_lpAbility);

	void					unseal(cItem *_lpItem);
	int						addSpareSkill();
	
	int		getDecreaseChargeCount();
	bool	isExpired();
	int		getPowerUpPercentage(int _iTargetSkill);
	cMiniPetAbilityInfo*	getPowerUpSkill(int _iSkill);
	BOOL	resetSkill(int _iMiniPetIndex);
};

class	cMiniPetTypeInfo
{
public:
	WORD	m_wSerial;
	WORD	m_wStyle;
	char	m_strName[32];
};

extern	void				SetMinipetTypeName();
extern	int					GetMiniPetTypeByName(char *_lpstrName);
extern	cMiniPetTypeInfo	g_aMiniPetTypeInfo[nsMiniPetType::Count];
extern	int					g_aiExpTableForMiniPet[102];
extern	char				*g_strMiniPetWav[];

const	int					c_iMiniPetVoiceCount	=	14;
#endif