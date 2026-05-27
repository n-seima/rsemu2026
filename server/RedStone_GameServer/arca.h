#ifndef	_classARCA_H
#define	_classARCA_H

#include "cItem.H"
#include "arcaDefine.H"
#include "CAbility.H"

#define	dMAX_ACTIVE_ARCA_COUNT	4096

class	cArcaInfo	:	public cArcaDefine
{
public:
};

class	cACTOR;

//
//	º¸¹°
class	cArca
{
public:
	WORD		m_wSerial;
	WORD		m_wFieldSerial;
	WORD		m_bf8InstanceField		:	8;
	WORD		m_bf4InstanceFieldFloor	:	4;
	int			m_iUniqueFieldSerial;
	cPOINT		m_pos;//¾ð¿¡?

	int			m_iDurability,m_iMaxDurability;
	int			m_iResistance;

	WORD		m_bf1IsDestroyAble		:	1;
	WORD		m_bf10DestroyAbleLevel	:	10;
	WORD		m_bf2TrapTarget			:	2;

	BYTE		m_bSecretLevel,m_bLockLevel;

	DWORD		m_dwOwnGold;

	WORD		m_wTrapLevel;
	WORD		m_wDetectTrapLevel;
	WORD		m_wDisarmTrapLevel;
	WORD		m_awTrapActiveChance[2];
	WORD		m_awTrapSkill[2];

	WORD		m_wOwner;//
	WORD		m_wOwnerParty;// ÆÆ¼
	WORD		m_wSealedTime,m_wLifeTime;

	cItem		m_aItem[dMAX_ITEM_COUNT_IN_ARCA];

				cArca()	{reset();}

	void		copy(cArca *_lpArca)
	{
		memcpy(this,_lpArca,sizeof(cArca));
	}
	void		reset()
	{
		memset(this,0,sizeof(cArca));

		m_wSerial		=	0xffff;
		m_wFieldSerial	=	0xffff;

		for (int i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
			m_aItem[i].reset();

		m_wOwner	=	0xffff;//
		m_wOwnerParty=	0xffff;// ÆÆ¼
	}

	void		resetForTreasureMap(int _iX,int _iY);

	cFIELD*		getField();

	BOOL		checkOwner(cACTOR	*_lpActor);

	void		addItem(cItem *_lpItem);
	int			getItemCount();
	void		addDropGold(int _iDropGold,cACTOR *_lpMonster);
	BOOL		generateForMonsterDrop(cACTOR *_lpMonster,int _iOwner);
	BOOL		generate(cArcaInfo *_lpArcaInfo,cFIELD *_lpField,int _iX,int _iY);

	BOOL		open(cACTOR	*_lpActor);

	void		tryDisarmTrap(cACTOR *_lpActor);
	void		disarmTrap(cACTOR *_lpActor);
	void		tryUnlock(cACTOR *_lpActor,BOOL _bIsWantUseKey);
	void		unlock(cACTOR *_lpActor);
	
	void		attack(cACTOR	*_lpActor);
	void		explosionTrap(cACTOR *_lpActor);

	void		getDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo);
	void		hitDamage(cACTOR *_lpAttacker,int _iDamage);
	void		destroyItemByDestroyLock(cACTOR *_lpAttacker);
	
	void		operateKarmaWork(int _iWork);
};

class	cArcaData
{
public:
	int					m_iArcaDataCount;
	cArcaInfo			*m_pArcaList;

	cArcaInfo*			getArcaData(int _iIndex)
	{
		if (_iIndex	>=	m_iArcaDataCount||	_iIndex< 0)
			return	NULL;

		return	&m_pArcaList[_iIndex];
	}
};

class	cArcaManager	:	public cArcaData 
{
public:
	cDen<cArca>			m_den;

						cArcaManager()
						{
							m_pArcaList	=	NULL;
							m_den.init(dMAX_ACTIVE_ARCA_COUNT);
						}

						~cArcaManager()
						{
							pKILL(m_pArcaList);
						}

	void				alloc(int _iCount)
	{
		m_pArcaList		=	new cArcaInfo [_iCount];
		m_iArcaDataCount=	_iCount;
	}

	cArca*				get(int _iIndex);
	int					add(cArca *_lpArca)	{return	m_den.add(_lpArca);}
	BOOL				remove(int _iIndex);

	int					generate(int _iSourceArcaIndex,cFIELD *_lpField,int _iX,int _iY);
	void				update();
	void				operateOncePer10SecondWork();

	void				open(int _iArcaIndex,cACTOR *_lpActor);
	void				disarmTrap(int _iArcaIndex,cACTOR	*_lpActor);
	void				tryUnlock(int _iArcaIndex,cACTOR	*_lpActor,BOOL _bIsWantUseKey);
	void				attack(int _iArcaIndex,cACTOR	*_lpActor);
};

extern	cArcaManager	g_arcaManager;

#endif