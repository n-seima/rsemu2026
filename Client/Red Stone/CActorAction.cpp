#include "CActor.H"

//
//	회피 액션
void
CActor::dodgeAction(CActor *_lpAttacker,int _iSpentCP,int _iDodgeAbility)
{
	if	(!_lpAttacker)
		return;

	CSkill	*lpSkill	=	&g_aSkill[_iDodgeAbility];

	if (lpSkill->m_wSerial	==	0xffff)	return;

	int	iAngle	=	cANGLE::GetAngleToTarget(_lpAttacker->m_pos.x,_lpAttacker->m_pos.y,m_pos.x,m_pos.y,2);

	iAngle		+=	90;
	iAngle		%=	360;

	CPos	pos;

	pos.Set(m_pos.x,m_pos.y);
	GetOvalAnglePos(&pos,iAngle,lpSkill->m_wDodgeDistance);

	if (!g_map.isBlank(pos.x/dTILE_XS,pos.y/dTILE_YS))
	{
		iAngle		+=	180;
		iAngle		%=	360;

		pos.Set(m_pos.x,m_pos.y);
		GetOvalAnglePos(&pos,iAngle,20);

		if (!g_map.isBlank(pos.x/dTILE_XS,pos.y/dTILE_YS))
			return;
	}

	if (m_cAfterImageStatus	==	dCLOSE_AFTER_IMAGE)
	{
		m_cAfterImageDisappearSpeed	=	1;
		m_cAfterImageStatus			=	dFADE_AFTER_IMAGE;
		m_cAfterImageCount			=	1;
		addAfterImage(0,(CPos *)&m_pos,m_iAnm,m_iDirect,m_iFrame,32);
	}

//	막힌 지형 아닌지 확인
	m_pos.Set(pos.x,pos.y);
}