#ifndef _classArca_h
#define	_classArca_h

#include "cNemo.h"
#include "arcaDefine.h"
#include "CGamePlay.h"
#include "CHitEffect.h"

#define	dSORT_OBJECT_BORDER_ARCA	70000

class	cArcaInfo	:	public	cArcaSimpleInfo
{
public:
	WORD				m_bf1IsOpen			:	1;
	WORD				m_bf1IsArmedTrap	:	1;

	WORD				m_wLinkArea;
	WORD				m_wRemoveTimer;
	WORD				m_wRemainDurability;
	cHitInfoManager		m_hitEffect;

						cArcaInfo();
	
	void				reset();
	void				hit(cAbility *_lpAbility,CHitInfo *_lpHitInfo);
};

class	cArcaManager : public CGamePlay
{
public:
	WORD		m_wWorkArca;
	int			m_iSelectArca;
	int			m_iFocusArca;
	int			m_iCount;
	cArcaInfo	m_aArca[dMAX_ARCA_COUNT_IN_FIELD+1];

	cArcaInfo*	getArca(int _iSerial);	
	void		reset();
	void		addInArea(cArcaInfo *_lpInfo);
	void		receiveExistArcaList(cArcaSimpleInfo *_lpList,int _iCount,BOOL _bIsFirst);
	int			getViewArcas(cINDEX *_lpIndexes,cRECT *_lpRectViewport);
	BOOL		isVisible(int _iSerial);
	void		drawArca(int _iIndex,int _iDX,int _iDY);

	void		addArca(cArcaSimpleInfo *_lpInfo);
	void		removeArca(int _iSerial,BOOL _bIsFirst=FALSE);
	void		openArca(int _iSerial);
	void		unlokcByAttack(int _iSerial);
	void		update();
	void		operatePopupMenu(int _iArcaIndex);

	void		operateAW_DisarmTrap(int _iSerial);
	void		operateAW_IsHideArca(int _iSerial);
	void		operateAW_IsTrapArca(int _iSerial);
	void		operateAW_IsLockedArca(int _iSerial);
	void		operateAW_IsFailedWork(int _iSerial);
	void		operateAW_UnlockByKey(int _iSerial,char *_lpstrName,int _iKey);
	void		operateAW_UnlockBySkill(int _iSerial,char *_lpstrName);


};

extern	cArcaManager	g_arcaManager;

#endif