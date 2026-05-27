#include <MATH.H>
#include "CActor.H"
#include "cMAP.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "CFieldItem.H"
#include "cSHAKE.H"
#include "cMESSAGE.H"
#include "CSound.H"

//
//	체력 회복
void
CActor::operateRecoverHP()
{
}

void
CActor::increaseHP(int _iHP)
{
	m_iHP	+=	_iHP;
	m_iHP	=	min(m_iHP,m_iMaxHP);
}