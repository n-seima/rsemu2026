#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"

//
//	연사 스킬
int
CActiveSkill::fireContinuousShoot()
{
	return	fireContinuousAttack();
}

//
//	연사 스킬 업데이트
BOOL
CActiveSkill::updateContinuousShoot()
{
	return	updateContinuousAttack();
}