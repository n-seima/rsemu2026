#include	"ast_one_bite.h"
#include	"cACTOR.h"

int
cAS_OneBite::fire()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	
	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_OneBite::update()
{
	return	TRUE;
}