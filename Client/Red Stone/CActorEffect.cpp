#include "CActor.H"
#include "CSound.H"

void
CActor::getHitPointCorrectPos(int _iPart,CPos *_lpPos)
{
	switch(_iPart)
	{
		case	eHEOP_HIT_ZONE		:
			_lpPos->x	=	getBody()->m_rectCrash.x1*m_wHorizonScale/100+random(getBody()->m_rectCrash.getWidth()*m_wHorizonScale/100);
			_lpPos->y	=	getBody()->m_rectCrash.y1*m_wVerticalScale/100+random(getBody()->m_rectCrash.getHeight()*m_wVerticalScale/100);
			break;

		case	eHEOP_FOOT			:
			_lpPos->x	=	0;
			_lpPos->y	=	0;
			break;

		case	eHEOP_SHOULDER		:
			_lpPos->x	=	0;
			_lpPos->y	=	-getBodyHeight()*4/5;
			break;

		case	eHEOP_ON_THE_HEAD	:
			_lpPos->x	=	0;
			_lpPos->y	=	-getBodyHeight();
			break;
	}
}

//
//	타격 이펙트 추가
void
CActor::addHitEffect(int _iDx,int _iDy,int _iEffect,int _iDirect,int _iAnm,int _iShakeValue , BOOL _isHalfFaram)
{
	int		iHitIndex	=	getFreeHitIndex();

	m_aHitEffect[iHitIndex]	=	g_hem.addEffect(_iDx,_iDy,_iEffect,_iDirect,_iAnm,_iShakeValue , 100 , 100, _isHalfFaram);
}

//
//	타격 이펙트 추가
void
CActor::addHitEffectByEvent(int _iDamageType,int _iDamage)
{
	int		iPhysicalDamage	=	0,iMagicDamage	=	0;
	int		iHitImage		=	g_im.m_wNormalHit;
	int		iHitIndex		=	getFreeHitIndex();
	CPos	pos;

	getHitPointCorrectPos(eHEOP_HIT_ZONE,&pos);

	if	(_iDamageType		==	0)	iPhysicalDamage	=	_iDamage;
	if	(_iDamageType		==	1)	iMagicDamage	=	_iDamage;

	m_aHitEffect[iHitIndex]	=	g_hem.addEffectAndInfo(pos.x,pos.y,iHitImage,dHIT_INFO_WHITE_NUMBER,iPhysicalDamage,iMagicDamage,0,isOwnTeam());//	타격효과 추가
}

void
CActor::addHitEffectAndInfo(int _iEffect,int _iPhysicalDamage,int _iMagicDamage,int _iEffectOutputPos,int _iAnm)
{
	int		iHitIndex		=	getFreeHitIndex();
	CPos	pos;

	getHitPointCorrectPos(_iEffectOutputPos,&pos);

	m_aHitEffect[iHitIndex]	=	g_hem.addEffectAndInfo(pos.x,pos.y,_iEffect,dHIT_INFO_WHITE_NUMBER,_iPhysicalDamage,_iMagicDamage,0,isOwnTeam(),_iAnm);//	타격효과 추가
}

//
//	
void
CActor::addHealEffectAndInfo(int _iEffect,int _iHealPoint,int _iEffectOutputPos)
{
	int		iHitIndex		=	getFreeHitIndex();
	CPos	pos;

	getHitPointCorrectPos(_iEffectOutputPos,&pos);
	
	if	(_iHealPoint)
		m_aHitEffect[iHitIndex]	=	g_hem.addEffectAndInfo(pos.x,pos.y,_iEffect,dHIT_INFO_GREEN_NUMBER,_iHealPoint,0,0,isOwnTeam());//	타격효과 추가
	else
		m_aHitEffect[iHitIndex]	=	g_hem.addEffect(pos.x,pos.y,_iEffect,0,0);//	타격효과 추가
}

//
//	
void
CActor::addHitInfo(int _iInfo,int _iEffectOutputPos)
{
	int		iHitIndex		=	getFreeHitIndex();
	CPos	pos;

	getHitPointCorrectPos(_iEffectOutputPos,&pos);

	m_aHitEffect[iHitIndex]	=	g_hem.addInfo(pos.x,pos.x,_iInfo,0,0,isOwnTeam());	//	미스~
}


//
//	오라 이미지 추가
void
CActor::addAuraImage(int _iSkill)
{
	CSkill	*lpSkill =	&g_aSkill[_iSkill];

	//JBC 케스팅 이미지가 초기값이면 이미지 인데스에서 찾아온다. 08-08-28
	//if(lpSkill->m_wCastImage == 0xffff )
	//	lpSkill->m_wCastImage = g_im.getEffectIndex(lpSkill->m_strName);

	if	(lpSkill && lpSkill->m_wCastImage != 0xffff && lpSkill->m_wType	!=	eSKILL_TYPE_IMPACT)
		m_wAuraImage	=	g_hem.addAura(lpSkill->m_wCastImage,lpSkill->m_wCastImageEffect,lpSkill->m_wPaletteIndex,lpSkill->m_wImageScale,lpSkill->m_wImageScale);
}

void
CActor::addSkillHitEffect(int _iSkill)
{
	if	(_iSkill	==	0xffff	||	_iSkill	>=	dMAX_SKILL	||	_iSkill	<	0)
		return;

	int		iHitIndex	=	getFreeHitIndex();
	CSkill	*lpSkill	=	&g_aSkill[_iSkill];
	CPos	pos;

	if	(lpSkill->m_wSerial	==	0xffff)
		return;

	int		iHitEffect	=	getFreeHitIndex();

	getHitPointCorrectPos(lpSkill->m_wHitImageOutputPart,&pos);

	m_aHitEffect[iHitIndex]	=	g_hem.addEffect(pos.x,pos.y,lpSkill->m_wHitImage,m_iDirect);
}

//
//	스킬 효과 추가
void
CActor::addSkillEffect(int _iSkill,int _iImage)
{
	if	(_iSkill	==	0xffff)
		return;

	CSkill	*lpSkill	=	&g_aSkill[_iSkill];

	if	(_iImage	==	0xffff)
		_iImage	=	eSE_HIT;

	switch(_iImage)
	{
		case	eSE_HIT		:
			addEffect(lpSkill->m_wHitImageOutputPart,lpSkill->m_wHitImage);
			break;
		case	eSE_SKILL	:
			addEffect(lpSkill->m_wSkillImageOutputPart,lpSkill->m_wSkillImage);
			break;
		case	eSE_HEAL	:
			addEffect(lpSkill->m_wHealImageOutputPart,lpSkill->m_wHealImage);
			break;
		case	eSE_CAST	:
			addEffect(lpSkill->m_wCastImageOutputPart,lpSkill->m_wCastImage);
			break;
		case	eSE_AID_SKILL	:
			addEffect(lpSkill->m_wAidSkillCastingImageOutputPart,lpSkill->m_wAidSkillCastingImage);
			break;
		case	eSE_CASTER_HIT	:
			addEffect(lpSkill->m_wCasterHitImageOutputPart,lpSkill->m_wCasterHitImage);
			break;
		case	eSE_EXPLOSION	:
			addEffect(eHEOP_FOOT,lpSkill->m_wExplosionImage);
			break;
	}
}

void
CActor::addEffect(int _iEffectOutputPos,int _iImage,int _iAnm,int _iXScale,int _iYScale)
{
	int		iHitIndex	=	getFreeHitIndex();
	CPos	pos;

	getHitPointCorrectPos(_iEffectOutputPos,&pos);

	m_aHitEffect[iHitIndex]	=	g_hem.addEffect(pos.x,pos.y,_iImage,m_iDirect,_iAnm,0,_iXScale,_iYScale);
}

//
//	레벨업 효과 붙이기
void
CActor::attatchLevelUpEffect()
{
	addHitEffect(0,0,g_im.m_wImageLevelUp);

	if (g_rectViewPort.isIn(m_pos.x,m_pos.y))
		g_esm.play("level_up.wav");
}

//
//	레벨업 효과 붙이기
void
CActor::attatchSkillLevelUpEffect()
{
	addHitEffect(0,0,g_im.m_wImageSkillLevelUp);

	if (g_rectViewPort.isIn(m_pos.x,m_pos.y))
		g_esm.play("skill_level_up.wav");
}


//
//	충전효과 설정
void
CActor::setChargeEffect(int _iImage)
{
	m_wChargeImage			=	_iImage;
	m_wChargeFrameCounter	=	0;
	m_bChargeAnm			=	0;
	m_bChargeFrame			=	0;
}

//
//	충전효과 에니메이션 증가
void
CActor::increaseChargeEffectAnimation()
{
	m_wChargeFrameCounter	=	0;
	m_bChargeFrame			=	0;

	if (m_bChargeAnm		!=	eCAE_MAIN_LOOP)
		m_bChargeAnm++;

	if (m_bChargeAnm		>	eCAE_END)
	{
		m_bChargeAnm	=	0;
		m_wChargeImage	=	0xffff;
	}
}

//
//	충전효과 초기화
void
CActor::resetChargeEffect()
{
	m_wChargeImage			=	0xffff;
	m_wChargeFrameCounter	=	0;
	m_bChargeAnm			=	0;
	m_bChargeFrame			=	0;
}

//
//	충전 효과 취소
void
CActor::cancelChargeEffect()
{
	if (m_bChargeAnm	==	eCAE_END)
		return;

	cANM	*lpEffect	=	g_im.getEffect(m_wChargeImage);
	int		iFrameCount	=	lpEffect->GetFrameCount(m_bChargeAnm);

	m_bChargeAnm		=	eCAE_END;

	if (lpEffect->m_iAnmCount==	1)
		return;

	m_wChargeFrameCounter=	0;

	if (m_bChargeAnm	==	eCAE_BEGIN)
	{
		m_bChargeFrame	=	iFrameCount-m_bChargeFrame-1;
		if (iFrameCount	!=	lpEffect->GetFrameCount(m_bChargeAnm))	m_bChargeFrame	=	0;
	}
	else
	if (m_bChargeAnm	==	eCAE_MAIN_LOOP)
		m_bChargeFrame	=	0;
}

void
CActor::changeSageBookColor(int _iBookColor)
{
	m_wSageBookColor	=	_iBookColor;
}
