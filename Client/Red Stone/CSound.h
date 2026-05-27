#ifndef _classEFFECT_SOUND_H
#define _classEFFECT_SOUND_H

#include "CNemo.h"
#include "CGame.h"

enum
{
eES_BLOCKING,
eES_DODGE,
eES_SWORD_HIT,
eES_SPEAR_HIT,
eES_BLUNT_HIT,
eES_ARROW_SHOT,
eES_ARROW_HIT,
};

#define	dEFFECT_SOUND_FILE_LENGTH	32
#define	dEFFECT_SOUND_NAME_LENGTH	32
#define	dMAX_EFFECT_SOUND_COUNT		100

class	CSoundEffectInfo
{
public:
	WORD	m_wSerial;
	WORD	m_isStatic;
	char	m_strFileName[dEFFECT_SOUND_FILE_LENGTH];
	char	m_strName[dEFFECT_SOUND_NAME_LENGTH];
	WORD	m_wStaticIndex;
};

extern	CSoundEffectInfo	g_aEffectSound[dMAX_EFFECT_SOUND_COUNT];

class	CSoundManager	:	public CGame
{
public:
	int		m_iCount,m_iStaticSoundCount;
	BOOL	m_bIsActive;
	WORD	m_awBgm[1024];

	void	setActive(BOOL _bIsActive)	{m_bIsActive	=	_bIsActive;}
	BOOL	init();
	BOOL	initForDev();
	void	play(int _iSerial);
	void	play(char *_lpstrFileName);

	void	playActionSound(CSkill *_lpSkill);
	void	playHitSound(CSkill *_lpSkill);
	void	playCastingSound(CSkill *_lpSkill);
	void	playCreateSound(CSkill *_lpSkill);
	void	playExplosionSound(CSkill *_lpSkill);
	void	playMissSound(CSkill *_lpSkill);

	char*	getSkillSound(CSkill *_lpSkill,int _iSound);
	void	playSEE_SuccessSound(CSkill *_lpSkill);
	void	playSEE_MissSound(CSkill *_lpSkill);

	int		findSound(int _iSerial);
	int		findSound(char *_lpstrName);

	void	playMonsterAttackSound(int _iIndex);
	void	playMonsterDeathSound(int _iIndex);
};

extern	CSoundManager	g_esm;
extern	char	*g_strBgmFile[];
extern	char	*g_strOldBgmFile[];

#endif