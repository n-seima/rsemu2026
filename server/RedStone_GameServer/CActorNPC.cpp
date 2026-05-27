#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.H"

//
//	업데이트
void
cACTOR::updateNpc()
{
	if (!m_lpField->checkLocalPartPlayerForMonster(m_iLocalPart))	//	주변에 플레이어도 없는체 1분이 지났다.
		return;

	operateAi();

	if	(isDeath())
		m_lpField->addBookedRemoveActor(m_wSerialInField);

	return;
}