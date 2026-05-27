#include "CHero.H"
#include "CWindowInterface.H"					// 10.01.07
int
CHero::findLeftAbility()
{
	int	i;

	for (i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill == NULL					)	continue;
		if	(!lpAbility->isEnableJob(m_wJob)	)	continue;
		if	(!lpSkill->isEnableLeftSkill()		)	continue;
		if	(lpSkill->m_wDifficultyLevel == 0 && lpAbility->getSpentCP() == 0) return i;
	}

	for (i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill == NULL					)	continue;
		if	(!lpAbility->isEnableJob(m_wJob)	)	continue;
		if	(!lpSkill->isEnableLeftSkill()		)	continue;
		if (lpSkill->m_wDifficultyLevel	==	0	)	return i;
	}

	for (i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill == NULL					)	continue;
		if	(!lpAbility->isEnableJob(m_wJob)	)	continue;
		if	(!lpSkill->isEnableLeftSkill()		)	continue;

		return i;
	}

	return	0xffff;
}

//
//	스킬 슬롯 데이터를 확인 보정한다.
void
CHero::correctQuickSlotData()
{
	int	i,j,k;

	cAbility	*lpAbility;
	BOOL		bIsIncorrect	=	FALSE;

	if	(m_wCurrentQuickSkillSlot < 0 || m_wCurrentQuickSkillSlot	>=	10)
		m_wCurrentQuickSkillSlot = 0;

	for (i=0;i<2;i++)
		for (j=0;j<dACTIVE_REACTION_COUNT;j++)
			if	(getAbility(m_aActivePassiveSkill[i][j])==	NULL)
				m_aActivePassiveSkill[i][j]	=	0xffff;

	if	(m_wLeftAbility == 0xffff && m_aLeftAbility[m_wJob%2] != 0xffff)
		m_wLeftAbility	=	m_aLeftAbility[m_wJob%2];

	if	(m_wLeftAbility	!=	0xffff)
	{
		lpAbility	=	getLeftAbility();

		if	(!lpAbility	||	lpAbility->getSkill()	==	NULL)
			m_wLeftAbility	=	0xffff;
		else
		if	(!lpAbility->isEnableJob(m_wJob)				)
			m_wLeftAbility	=	0xffff;
		else
		if	(!lpAbility->getSkill()->isEnableLeftSkill()	)
			m_wLeftAbility	=	0xffff;
	}

	if	(m_wLeftAbility	==	0xffff)
	{
		int	iLeftAbility=	findLeftAbility();

		setLeftAbility(iLeftAbility);
	}

	if	(m_wSubAbility == 0xffff && m_aSubAbility[m_wJob%2] != 0xffff)
		m_wSubAbility	=	m_aSubAbility[m_wJob%2];

	if	(m_wSubAbility	!=	0xffff)
	{
		lpAbility	=	getSubAbility();

		if	(!lpAbility	||	lpAbility->getSkill()	==	NULL)
			m_wSubAbility	=	0xffff;
		else
		if	(!lpAbility->isEnableJob(m_wJob)				)
			m_wSubAbility	=	0xffff;
		else
		if	(!lpAbility->getSkill()->isEnableSubSkill()		)
			m_wSubAbility	=	0xffff;
	}

	if	(m_wRightAbility == 0xffff && m_aRightAbility[m_wJob%2] != 0xffff)	
		m_wRightAbility	=	m_aRightAbility[m_wJob%2];

	if	(m_wRightAbility	!=	0xffff)
	{
		lpAbility	=	getRightAbility();

		if	(!lpAbility	||	lpAbility->getSkill()	==	NULL)	
			m_wRightAbility	=	0xffff;
		else
		if	(!lpAbility->isEnableJob(m_wJob)				)	
			m_wRightAbility	=	0xffff;
		else
		if	(!lpAbility->getSkill()->isEnableRightSkill()	)	
			m_wRightAbility	=	0xffff;
	}

	if	(m_wSupportAbility == 0xffff && m_aSupportAbility[m_wJob%2] != 0xffff)	
		m_wSupportAbility	=	m_aSupportAbility[m_wJob%2];

	if	(m_wSupportAbility	!=	0xffff)
	{
		lpAbility	=	getSupportAbility();

		if	(!lpAbility	||	lpAbility->getSkill()	==	NULL)	
			m_wSupportAbility	=	0xffff;
		else
		if	(!lpAbility->isEnableJob(m_wJob)				)	
			m_wSupportAbility	=	0xffff;
		else
		if	(!lpAbility->getSkill()->isSupportSkill()		)	
			m_wSupportAbility	=	0xffff;
	}

	for (i=0;i<2;i++)
	{
		bIsIncorrect=	FALSE;

		if	(m_aLeftAbility[i]	!=	0xffff)
		{
			lpAbility	=	getAbility(m_aLeftAbility[i]);

			if	(!lpAbility || lpAbility->m_wSkill	==	0xffff	||	lpAbility->m_wLevel	==	0	)	
				bIsIncorrect	=	TRUE;
			else
			if	(!lpAbility->getSkill()->isEnableLeftSkill()									)	
				bIsIncorrect	=	TRUE;
			else
			if	(m_wJob%2==i	&&	!lpAbility->isEnableJob(m_wJob)								)	
				bIsIncorrect	=	TRUE;

			if	(bIsIncorrect)
			{
				m_aLeftAbility[i]	=	0xffff;
				sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_LEFT_ABILITY,i,0xffff);
			}
		}

		bIsIncorrect=	FALSE;

		if	(m_aSubAbility[i]	!=	0xffff)
		{
			lpAbility	=	getAbility(m_aSubAbility[i]);

			if	(!lpAbility || lpAbility->m_wSkill	==	0xffff	||	lpAbility->m_wLevel	==	0	)	
				bIsIncorrect	=	TRUE;
			else
			if	(!lpAbility->getSkill()->isEnableSubSkill()										)	
				bIsIncorrect	=	TRUE;
			else
			if	(m_wJob%2==i	&&	!lpAbility->isEnableJob(m_wJob)								)	
				bIsIncorrect	=	TRUE;

			if	(bIsIncorrect)
			{
				m_aSubAbility[i]	=	0xffff;
				sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_SUB_ABILITY,i,0xffff);
			}
		}

		bIsIncorrect=	FALSE;

		if	(m_aRightAbility[i]	!=	0xffff)
		{
			lpAbility	=	getAbility(m_aRightAbility[i]);

			if	(!lpAbility || lpAbility->m_wSkill	==	0xffff	||	lpAbility->m_wLevel	==	0	)	
				bIsIncorrect	=	TRUE;
			else
			if	(!lpAbility->getSkill()->isEnableRightSkill()									)	
				bIsIncorrect	=	TRUE;
			else
			if	(m_wJob%2==i	&&	!lpAbility->isEnableJob(m_wJob)								)	
				bIsIncorrect	=	TRUE;

			if	(bIsIncorrect)
			{
				m_aRightAbility[i]	=	0xffff;
				sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_RIGHT_ABILITY,i,0xffff);
			}
		}

		bIsIncorrect=	FALSE;

		if	(m_aSupportAbility[i]	!=	0xffff)
		{
			lpAbility	=	getAbility(m_aSupportAbility[i]);

			if	(!lpAbility || lpAbility->m_wSkill	==	0xffff	||	lpAbility->m_wLevel	==	0	)	
				bIsIncorrect	=	TRUE;
			else
			if	(!lpAbility->getSkill()->isSupportSkill()										)	
				bIsIncorrect	=	TRUE;
			else
			if	(m_wJob%2==i	&&	!lpAbility->isEnableJob(m_wJob)								)	
				bIsIncorrect	=	TRUE;

			if	(bIsIncorrect)
			{
				m_aSupportAbility[i]=	0xffff;
				sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_SUPPORT_ABILITY,i,0xffff);
			}
		}
	}

	for (i=0;i<10;i++)
		for (j=0;j<10;j++)
			for (k=0;k<2;k++)
			{
				bIsIncorrect	=	FALSE;
				
				if	(m_aQuickSkill[i][j][k]	==	0xffff || m_aQuickSkill[i][j][k] >= dEQUIPMENT_SKILL_SLOT && m_aQuickSkill[i][j][k] <= dEQUIPMENT_SKILL_SLOT * 2)
					continue;
				
				if (m_aQuickSkill[i][j][k]	&	dMASK_QUICK_SLOT_ITEM)
				{
					int	iItemSlot	=	m_aQuickSkill[i][j][k]	-	dMASK_QUICK_SLOT_ITEM;
					
					if (iItemSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
					{
						iItemSlot	-=	dBORDER_OF_ITEM_AND_EQUIPMENT;
						
						if	(iItemSlot >= dEQUIPMENT_PART_COUNT || m_aEquip[iItemSlot].m_wBaseItem	==	0xffff)
							bIsIncorrect	=	TRUE;
					}
					else
					{
						if	(iItemSlot > dOWN_ITEM_COUNT ||  iItemSlot < 0 || m_aItems[iItemSlot].m_wBaseItem	==	0xffff)
							bIsIncorrect	=	TRUE;
					}

					continue;
				}

				lpAbility	=	getAbility(m_aQuickSkill[i][j][k]);

				if	(!lpAbility || lpAbility->m_wSkill == 0xffff || lpAbility->m_wLevel <= 0	)
					bIsIncorrect	=	TRUE;
				else
				if	(m_wJob%2==	k	&&	!lpAbility->isEnableJob(m_wJob)						)
					bIsIncorrect	=	TRUE;

				if	(bIsIncorrect)
				{
					m_aQuickSkill[i][j][k]	=	0xffff;
					sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,i,j,k,0xffff);
				}
			}
	
	// 10.01.07  스킬슬롯(아이템) 초기화
	initQuickSlotItem();			
}

//
//	퀵스킬 슬롯 돌리기
void
CHero::rotateQuickSlot()
{
	int	iCurrent	=	m_wCurrentQuickSkillSlot;
	int	iSlot		=	m_wJob%2;

	if (m_wCurrentQuickSkillSlot	>=	9)
		m_wCurrentQuickSkillSlot	=	0;
	else
		m_wCurrentQuickSkillSlot++;

	while(1)
	{
		if (m_wCurrentQuickSkillSlot	==	iCurrent)
			break;

		for (int i=0;i<10;i++)	
		{
			if (m_aQuickSkill[m_wCurrentQuickSkillSlot][i][iSlot]	!=	0xffff)
				return;
		}

		m_wCurrentQuickSkillSlot++;

		if (m_wCurrentQuickSkillSlot	>=	10)
			m_wCurrentQuickSkillSlot	=	0;
	}

	if (m_wCurrentQuickSkillSlot	==	iCurrent)
	{
		if (m_wCurrentQuickSkillSlot	>=	9)
			m_wCurrentQuickSkillSlot	=	0;
		else
			m_wCurrentQuickSkillSlot++;
	}
}

int
CHero::getQuickSlotSkill(int _iSlot)
{
	switch(_iSlot)
	{
		case	eSKILL_SLOT_LEFT			:
			return	m_wLeftAbility;

		case	eSKILL_SLOT_RIGHT			:
			return	m_wRightAbility;

		case	eSKILL_SLOT_SUB				:
			return	m_wSubAbility;

		case	eSKILL_SLOT_SUPPORT_MAGIC	:
			return	m_wSupportAbility;
	}

	return	m_aQuickSkill[m_wCurrentQuickSkillSlot][_iSlot-4][(m_wJob)%2];
}

void
CHero::setQuickSlotSkill(int _iSlot,int _iSkill)
{
	switch(_iSlot)
	{
		case	eSKILL_SLOT_LEFT			:
			setLeftAbility(_iSkill);
			return;

		case	eSKILL_SLOT_RIGHT			:
			setRightAbility(_iSkill);
			return;

		case	eSKILL_SLOT_SUB				:
			setSubAbility(_iSkill);
			return;

		case	eSKILL_SLOT_SUPPORT_MAGIC	:
			setSupportAbility(_iSkill);
			return;
	}

	setQuickSlotAbility(_iSkill,_iSlot-4);
}

void
CHero::setLeftAbility(int _iAbility)
{
	CheckBCS(TRUE);

	if	(_iAbility < dABILITY_COUNT && _iAbility >= 0 && m_aAbility[_iAbility].m_wLevel	<=	0)
		return;
	if	(_iAbility>=dABILITY_COUNT	||	_iAbility<0)
		return;
	if	(m_aAbility[_iAbility].m_wSkill	==	0xffff)
		return;
	if	(!m_aAbility[_iAbility].getSkill()->isEnableJob(m_wJob))
		return;

	m_wLeftAbility				=	_iAbility;
	m_aLeftAbility[(m_wJob)%2]	=	_iAbility;

	getRangeOfDefaultAttack();

	sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_LEFT_ABILITY,m_wJob%2,_iAbility);
}

void
CHero::setRightAbility(int _iAbility)
{
	CheckBCS(TRUE);

	if	(_iAbility>=dABILITY_COUNT	||	_iAbility<0)
	{
		_iAbility		=	0xffff;
	}
	else
	{
		if	(_iAbility < dABILITY_COUNT && _iAbility >= 0 && m_aAbility[_iAbility].m_wLevel	<=	0)
			return;
		if	(m_aAbility[_iAbility].m_wSkill	==	0xffff)
			return;
		if	(!m_aAbility[_iAbility].getSkill()->isEnableJob(m_wJob))
			return;
	}

	m_wRightAbility				=	_iAbility;
	m_aRightAbility[(m_wJob)%2]	=	_iAbility;

	sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_RIGHT_ABILITY,m_wJob%2,_iAbility);
}

void
CHero::setSupportAbility(int _iAbility)
{
	CheckBCS(TRUE);

	if	(_iAbility>=dABILITY_COUNT	||	_iAbility<0)
		_iAbility	=	0xffff;
	else
	{
		if	(_iAbility < dABILITY_COUNT && _iAbility >= 0 && m_aAbility[_iAbility].m_wLevel	<=	0)
			return;
		if	(m_aAbility[_iAbility].m_wSkill	==	0xffff)
			return;
		if	(!m_aAbility[_iAbility].getSkill()->isEnableJob(m_wJob))
			return;
	}

	m_wSupportAbility				=	_iAbility;
	m_aSupportAbility[(m_wJob)%2]	=	_iAbility;

	sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_SUPPORT_ABILITY,m_wJob%2,_iAbility);
}
void
CHero::setSubAbility(int _iAbility)
{
	CheckBCS(TRUE);

	if	(_iAbility	>=	dABILITY_COUNT	||	_iAbility	<	0)
		_iAbility	=	0xffff;
	else
	{
		if	(_iAbility < dABILITY_COUNT && _iAbility >= 0 && m_aAbility[_iAbility].m_wLevel	<=	0)
			return;
		if	(m_aAbility[_iAbility].m_wSkill	==	0xffff)
			return;
		if	(!m_aAbility[_iAbility].getSkill()->isEnableJob(m_wJob))
			return;
	}

	m_wSubAbility					=	_iAbility;
	m_aSubAbility[(m_wJob)%2]		=	_iAbility;

	sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_SUB_ABILITY,m_wJob%2,_iAbility);
}

void
CHero::setQuickSlotAbility(int _iAbility,int _iSlot)
{
	CheckBCS(TRUE);

	if	(_iAbility	>=	dABILITY_COUNT	||	_iAbility	<	0)
		_iAbility	=	0xffff;
	else
	{
		if	(_iAbility < dABILITY_COUNT && _iAbility >= 0 && m_aAbility[_iAbility].m_wLevel	<=	0)
			return;
		if	(m_aAbility[_iAbility].m_wSkill	==	0xffff)
			return;
		if	(!m_aAbility[_iAbility].getSkill()->isEnableJob(m_wJob))
			return;
	}

	m_aQuickSkill[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2]	=	_iAbility;

	sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,m_wCurrentQuickSkillSlot,_iSlot,m_wJob%2,_iAbility);
	// 10.01.07 추가
	m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].IconShape	= _iAbility;
	m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].NotUseItem	= FALSE;
	m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].DxItem		= FALSE;
	m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].UmItem		= FALSE;
}

// 10.01.07 함수 추가
// 스킬슬롯 아이템 초기화
void
CHero::initQuickSlotItem()
{
	for(int i=0; i<10; i++)
	{
		for(int j=0; j<10; j++)
		{
			for(int k=0; k<2; k++)
			{
				if(m_aQuickSkill[i][j][k] >= 200 && m_aQuickSkill[i][j][k] <= 400)
				{
					cItem	*lpItem					= getInventoryItem(m_aQuickSkill[i][j][k]-dEQUIPMENT_SKILL_SLOT);
					cBasicItem	*lpBasicItem		= NULL;
					
					if(lpItem)
					{
						lpBasicItem							= lpItem->getBasicItem(TRUE);
						m_sQuickSlotItem[i][j][k].IconShape	= lpBasicItem->m_wIconShape;
			
						if(!g_hero.isUseAbleItem(lpItem))
							m_sQuickSlotItem[i][j][k].NotUseItem = TRUE;
						else
							m_sQuickSlotItem[i][j][k].NotUseItem = FALSE;

						if	(lpItem->isUltimateItem())
							m_sQuickSlotItem[i][j][k].UmItem = TRUE;
						else
							m_sQuickSlotItem[i][j][k].UmItem = FALSE;
						
						if	(lpItem->isDXItem())
							m_sQuickSlotItem[i][j][k].DxItem = TRUE;
						else
							m_sQuickSlotItem[i][j][k].DxItem = FALSE;
					}
				}
				else
				{
					m_sQuickSlotItem[i][j][k].IconShape		= m_aQuickSkill[i][j][k];
					m_sQuickSlotItem[i][j][k].NotUseItem	= FALSE;
					m_sQuickSlotItem[i][j][k].UmItem		= FALSE;
					m_sQuickSlotItem[i][j][k].DxItem		= FALSE;
				}
			}
		}
	}
}

// 10.01.07 함수 추가
// 스킬 슬롯에 아이템을 설정
void
CHero::setQuickSlotItem(int _iItemSlot,int _iSlot)
{
	CheckBCS(TRUE);

	for(int i=0; i<10; i++)
		for(int j=0; j<10; j++)
			for(int k=0; k<2; k++)
			{
				if(m_aQuickSkill[i][j][k] == _iItemSlot + dEQUIPMENT_SKILL_SLOT)
				{
					m_aQuickSkill[i][j][k]					= 0xffff;
					
					m_sQuickSlotItem[i][j][k].IconShape		= 0xffff;
					m_sQuickSlotItem[i][j][k].NotUseItem	= FALSE;
					m_sQuickSlotItem[i][j][k].DxItem		= FALSE;
					m_sQuickSlotItem[i][j][k].UmItem		= FALSE;
					
					sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,i,j,k,0xffff);
				}
			}

	cItem	*lpItem					= getInventoryItem(_iItemSlot);
	cBasicItem	*lpBasicItem		= NULL;
	lpBasicItem						= lpItem->getBasicItem(TRUE);
	
	// 아이템 설정
	m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].IconShape	= lpBasicItem->m_wIconShape;
				
	if(!g_hero.isUseAbleItem(lpItem))
		m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].NotUseItem = TRUE;
	else
		m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].NotUseItem = FALSE;
				
	if	(lpItem->isUltimateItem())
		m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].UmItem = TRUE;
	else
		m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].UmItem = FALSE;
				
	if	(lpItem->isDXItem())
		m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].DxItem = TRUE;
	else
		m_sQuickSlotItem[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2].DxItem = FALSE;
				
	m_aQuickSkill[m_wCurrentQuickSkillSlot][_iSlot][m_wJob%2]			=	_iItemSlot + dEQUIPMENT_SKILL_SLOT;
	sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,m_wCurrentQuickSkillSlot,_iSlot,m_wJob%2,_iItemSlot + dEQUIPMENT_SKILL_SLOT);
}

// 10.01.07 함수 추가
// 스킬슬롯에 아이템이 설정되어 있다면 삭제
void
CHero::deleteQuickSlotItem(int _iSlot)
{
	int skillslot = g_hero.getEquipItemBySlotIndex(_iSlot);
								
	if(skillslot != 0xffff)
	{
		int EquiItemSlot1 = (skillslot / 2) / 10;
		int EquiItemSlot2 = (skillslot / 2) % 10;
		int EquiItemSlot3 = skillslot % 2;
					
		m_aQuickSkill[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3]	= 0xffff;
		sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,EquiItemSlot1,EquiItemSlot2,EquiItemSlot3,0xffff);
								
		m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].IconShape		= 0xffff;
		m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].NotUseItem	= FALSE;
		m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].DxItem		= FALSE;
		m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].UmItem		= FALSE;
	}
}

// 10.01.07 함수 추가
// 장비장착시 스킬슬롯 검색 및 설정
BOOL
CHero::setEquipItem(int _iItem, int _iSlot, BOOL _bSlotCheck)
{

	m_bUseSkillSlotItem	= TRUE;
	cItem	*lpItem	= g_hero.getInventoryItem(_iItem);
	
	if(!lpItem)
		return FALSE;

	cBasicItem* lpBasicItem = lpItem->getBasicItem();

	if(!lpBasicItem)
		return FALSE;
	

	int iEquipIndex	= g_gwSkill.getSkillItemtoEquipmentIndex(lpItem->getItemType());

	if(lpBasicItem->m_bf1IsUseAfterWear)
	{
		if	(_iItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		{
			if(g_hero.isUseAbleItem(lpItem))
			{
					g_hero.askUseItem(_iItem);
				
				return TRUE;
			}
		}	

		if(iEquipIndex == 0xffff)
			return FALSE;
	}
	
	if( iEquipIndex == 0xffff)
	{
		m_iEquipItemSlot	= 0xffff;
		m_iStripItemSlot	= 0xffff;
		g_hero.sendEquipItem(_iItem,GetFocusEquipmentInInventoryWindow(FALSE));
		return FALSE;
	}
	
	// 10.01.27 수정
	m_iEquipItemSlot		= g_hero.getEquipItemBySlotIndex(_iItem);
	int iEquipFinalIndex	= 0xffff;
	if(g_hero.isUseAbleItem(lpItem))
	{
		if(iEquipIndex == dEQUIP_WEAPON)
		{
			int iJobCheck = g_hero.m_wJob%2;
			iEquipIndex = dEQUIP_WEAPON + iJobCheck * 17;
			m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(iEquipIndex + 100);
		}
		else
		{
			m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(iEquipIndex + 100);
		}
		iEquipFinalIndex	= iEquipIndex + dEQUIPMENT_SKILL_SLOT;
		
	}
/*
	// 주무기가 양손무기 캐릭터
	if(g_hero.m_wJob==dJOB_WARRIOR||g_hero.m_wJob==dJOB_WEREWOLF||g_hero.m_wJob==dJOB_FALLEN_ANGEL||g_hero.m_wJob==dJOB_FIGHTER
		||g_hero.m_wJob==dJOB_MAGIC_ARCHER||g_hero.m_wJob==dJOB_MAGICAL_GIRL||g_hero.m_wJob==dJOB_DEVIL)
	{
		// 한손 무기가 들어왔을때
		if(iEquipIndex == dEQUIP_WEAPON)
		{
			if(g_hero.isUseAbleItem(lpItem))
					m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(117);
			else
				m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(iEquipIndex + 100);
		}
		else
			m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(iEquipIndex + 100);
		
		iEquipFinalIndex	= iEquipIndex + 100 + dEQUIPMENT_SKILL_SLOT;
	}
	else	// 주무기가 한손무기 캐릭터
	{
		// 양손무기가 들어왔을때
		if(iEquipIndex == dEQUIP_WEAPON_2)
		{
			if(g_hero.isUseAbleItem(lpItem))
			{
				m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(100);
				iEquipFinalIndex	= 100 + dEQUIPMENT_SKILL_SLOT;
			}
			else
			{
				m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(iEquipIndex + 100);
				iEquipFinalIndex	= iEquipIndex + 100 + dEQUIPMENT_SKILL_SLOT;
			}
		}
		else
		{
			m_iStripItemSlot	= g_hero.getEquipItemBySlotIndex(iEquipIndex + 100);
			iEquipFinalIndex	= iEquipIndex + 100 + dEQUIPMENT_SKILL_SLOT;
		}
	}
*/
	// 착용하려는 장비 아이템이 스킬창에 등록되어 있다.
	if(m_iEquipItemSlot != 0xffff)
	{
		// 같은 장비
		if(_bSlotCheck)
			if(m_aQuickSkill[m_wCurrentQuickSkillSlot][_iSlot][(m_wJob)%2] == iEquipFinalIndex )
			{
				// 10.01.14
				m_bUseSkillSlotItem	= FALSE;
				return FALSE;
			}
	}

	if(_bSlotCheck)
		g_hero.sendEquipItem(_iItem,iEquipIndex);
	else
		g_hero.sendEquipItem(_iItem,GetFocusEquipmentInInventoryWindow(FALSE));

	return TRUE;
}
// 10.01.07 함수 추가
// 장비해제시 스킬슬롯 검색 및 설정
void
CHero::stripEquipItem(int _iItem)
{

	// 10.01.14
	m_bUseSkillSlotItem = TRUE;
	m_iStripItemSlot = g_hero.getEquipItemBySlotIndex(_iItem);
	s_agent.sendStripEquipmentMessage(_iItem - dBORDER_OF_ITEM_AND_EQUIPMENT);
			
	m_iStripIndex = 0xfffff;

	int i = 0;
	m_iStripIndex = 0;	

	if(m_iStripItemSlot != 0xffff)
	{
		for (i=0;i<m_wItemSlotCount;i++, m_iStripIndex++)
			if	(m_aItems[i].m_wBaseItem	==	0xffff)
				return;
	}
	m_iStripIndex = 42;

	for (i=0;i<m_bf6ExtraInventorySize;i++, m_iStripIndex++)
		if(m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			break;
}


// 10.01.11 함수 추가
// 장비 착용시 서버 완료메시지후  변경
void
CHero::changeQuickSlotEquipItem(int _iEquip, int _iStrip)
{
	if(m_iEquipItemSlot != 0xffff)
	{
		int iEquipItemSlot1 = (m_iEquipItemSlot / 2) / 10;
		int iEquipItemSlot2 = (m_iEquipItemSlot / 2) % 10;
		int iEquipItemSlot3 = m_iEquipItemSlot % 2;
		
		m_aQuickSkill[iEquipItemSlot1][iEquipItemSlot2][iEquipItemSlot3]	= _iEquip + dEQUIPMENT_SKILL_SLOT + 100;
		sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,iEquipItemSlot1,iEquipItemSlot2,iEquipItemSlot3,_iEquip  + dEQUIPMENT_SKILL_SLOT + 100);			
	}
		
	if(m_iStripItemSlot != 0xffff)
	{
		int iStripItemSlot1 = (m_iStripItemSlot / 2) / 10;
		int iStripItemSlot2 = (m_iStripItemSlot / 2) % 10;
		int iStripItemSlot3 = m_iStripItemSlot % 2;		
		
		m_aQuickSkill[iStripItemSlot1][iStripItemSlot2][iStripItemSlot3]	= _iStrip + dEQUIPMENT_SKILL_SLOT;
		sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,iStripItemSlot1,iStripItemSlot2,iStripItemSlot3, _iStrip + dEQUIPMENT_SKILL_SLOT);
	}

	m_iStripIndex	 	= 0xffff;
	m_iEquipItemSlot	= 0xffff;
	m_iStripItemSlot	= 0xffff;
}

// 10.01.11 함수 추가
// 장비 해제시 서버 완료메시지후  변경
void
CHero::changeQuickSlotStripItem()
{
	if(m_iStripItemSlot != 0xffff)
	{
		int iStriItemSlot1 = (m_iStripItemSlot / 2) / 10;
		int iStriItemSlot2 = (m_iStripItemSlot / 2) % 10;
		int iStriItemSlot3 = m_iStripItemSlot  % 2;
		
		m_aQuickSkill[iStriItemSlot1][iStriItemSlot2][iStriItemSlot3] =	m_iStripIndex + dEQUIPMENT_SKILL_SLOT;
		sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,iStriItemSlot1,iStriItemSlot2,iStriItemSlot3,m_iStripIndex + dEQUIPMENT_SKILL_SLOT);
	}
	m_iEquipItemSlot = 0xffff;
	m_iStripItemSlot = 0xffff;
	m_iStripIndex	 = 0xffff;
}