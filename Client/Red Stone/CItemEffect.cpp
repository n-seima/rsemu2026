#include "CItemEffect.H"
#include "CHero.H"

cITEM_GENERATE_DATA_DECODER	g_itemGenerateDataDecoder;

CDecoder	cITEM_GENERATE_DATA_DECODER::s_decoder;
cSTRING		cITEM_GENERATE_DATA_DECODER::s_string;

//
//	아이템 효과들의 인덱스를 얻어 온다.(시리얼 참조)
int
cITEM_GENERATE_DATA_DECODER::getIndex(int serial)
{
	int	iIndex	=	0;

	while(1)
	{
		if	(g_aItemEffectHyperText[iIndex].m_iSerial	==	0xffff)
			break;
		if	(g_aItemEffectHyperText[iIndex].m_iSerial	==	serial)
			return	iIndex;

		iIndex++;
	}

	return	0xffff;
}

void
cITEM_GENERATE_DATA_DECODER::applyFormulaValue(int _iEffect,int _iOrder,  int* _iValue)
{

	switch(_iEffect)
	{
	case eIE_ACL_SHIELD:
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_SHIELD_FORMULA1(*_iValue);
			else if(_iOrder == 1)
				*_iValue	=	dACL_OPTION_SHIELD_FORMULA2(*_iValue);
			break;
		}
	case eIE_ACL_CHARGE:
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_CHARGE_FORMULA1(*_iValue);
			else if(_iOrder == 1)
				*_iValue	=	dACL_OPTION_CHARGE_FORMULA2(*_iValue);
			break;
		}
	case eIE_ACL_INCREASE_EXP :
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_INCREASE_EXP_FORMULA(*_iValue);
			break;
		}
	case eIE_ACL_ADD_FIRE_DAMAGE :
		{
			if(_iOrder == 1)
				*_iValue	=	dACL_OPTION_ADD_FIRE_DAMAGE(*_iValue);
			break;
		}
	case eIE_ACL_INCREASE_ALL_STATUS :
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_INCREASE_ALL_STATUS(*_iValue);
			break;
		}
	case eIE_ACL_SOUL_GUARD :
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_SOUL_GUARD(*_iValue);
			break;
		}
	case eIE_ACL_TRANSFORMATION_REPTILE:
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA1(*_iValue);
			else if(_iOrder == 1)
				*_iValue	=	dACL_OPTION_TRANSFORMAION_REPTILE_FORMULA2(*_iValue);
			break;
		}
	case eIE_ACL_INCREASE_MAX_HP:
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_INCREASE_MAX_HP_FORMULA1(*_iValue);

			break;
		}
	case eIE_ACL_INCREASE_ATTACK_POWER:
		{
			if(_iOrder == 0)
				*_iValue	=	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA1(*_iValue);
			else if(_iOrder == 1)
				*_iValue	=	dACL_OPTION_INCREASE_ATTACK_POWER_FORMULA2(*_iValue);
			break;
		}
	case eIE_ADD_MAX_HPx100:
		{
			if(_iOrder == 0)
				*_iValue	=	*_iValue * 100;
			else if(_iOrder == 1)
				*_iValue	=	*_iValue * 100;
			break;
		}
	case eIE_ADD_MAX_CPx100:
		{
			if(_iOrder == 0)
				*_iValue	=	*_iValue * 100;
			else if(_iOrder == 1)
				*_iValue	=	*_iValue * 100;
			break;
		}
	}
}
//
//	아이템 부가 효과 설명
char*
cITEM_GENERATE_DATA_DECODER::getComment(cITEM_GENERATE_DATA* _lpData,cItem *_lpItem , cBasicItem* _lpBasicItem)
{
	if	(_lpData->m_wEffect	== eIE_FREE_RARE_OPTION	)
		return	NULL;
	if	(_lpData->m_wEffect	== eIE_TELEPORT_STONE	)
		return	"isWayPoint";
	if	(_lpData->m_wEffect	== eIE_ITEM_PIECE	)
		return	"isItemPiece";

	int		iIndex	=	getIndex(_lpData->m_wEffect);
	char	*lpStream;

	CRY(iIndex	==	0xffff,"등록되지 않은 생성 데이터야!!");

	s_string.Reset();
	s_decoder.UploadBuffer(g_aItemEffectHyperText[iIndex].m_strReferenceText,NULL);

	BOOL	bFirst					=	TRUE;
	BOOL	bIsAcquireSymbol		=	FALSE;
	BOOL	bIsAcquireMinusSymbol	=	FALSE;
	BOOL	bIsRequireString		=	FALSE;
	LONG	lStreamCount			=	0;

	while(1)
	{
		bIsAcquireSymbol	=	FALSE;
		bIsRequireString	=	FALSE;
		lpStream			=	s_decoder.getStream('[');

		if	(!lpStream && !bFirst)
			break;

		bFirst	=	FALSE;

		s_string.Add(lpStream);

		if	(s_decoder.m_isDone	)
			break;		//	다 읽어 들였다.

		if	(s_decoder.getFirstChar() == '+')
		{
			s_decoder.increaseBP();
			bIsAcquireSymbol	=	TRUE;
		}

		if	(s_decoder.getFirstChar() == '-')
		{
			s_decoder.increaseBP();
			bIsAcquireMinusSymbol	=	TRUE;
		}

		if	(s_decoder.getFirstChar() == '*')
		{
			s_decoder.increaseBP();
			bIsRequireString	=	TRUE;
		}

		int iValue		=	0;
		
		if	(s_decoder.getFirstChar() == '(')
		{
			int	iValueIndex	=	s_decoder.getNumber();
			char strSymbol	=	s_decoder.getFirstChar();	
			int iValue2		=	0;
			
			iValueIndex		=	_lpData->m_aValue[iValueIndex];
			iValue			=	_lpItem->m_aOption[iValueIndex];
			
			
			iValueIndex		=	s_decoder.getNumber();
			iValueIndex		=	_lpData->m_aValue[iValueIndex];
			iValue2			+= _lpItem->m_aOption[iValueIndex];
			
			switch(strSymbol)
			{
			case '+':
				iValue		+= iValue2;
				break;
			case '-':
				iValue		+= iValue2;
				break;
			case '/':
				if(iValue2)
					iValue		/= iValue2;
				break;
			case '*':
				iValue		*= iValue2;
				break;
			}
			s_decoder.getStream(')');
		}
		else
		{
			int	iValueIndex	=	s_decoder.getNumber();
			iValueIndex		=	_lpData->m_aValue[iValueIndex];
			iValue			=	_lpItem->m_aOption[iValueIndex];
		}

		if	(bIsAcquireMinusSymbol)
			iValue		=	-iValue;

//	억지 코드 -_- 복권의 경우 드랍레벨이 0이면 드랍레벨을 유저의 레벨로 한다.
		if	(_lpData->m_wEffect	==	eIE_RARE_ITEM_LOTTERY_TICKET	||
			_lpData->m_wEffect	==	eIE_UNIQUE_ITEM_LOTTERY_TICKET	||
			_lpData->m_wEffect	==	eIE_SUPER_ITEM_LOTTERY_TICKET	)
			if	(iValue			==	0)
				iValue			=	g_hero.getLevel();

		applyFormulaValue(_lpData->m_wEffect, lStreamCount, &iValue);

		lpStream	=	s_decoder.getStream(']');
		++lStreamCount;
		if	(bIsAcquireSymbol && iValue > 0 )
			s_string.Add(_ms("<c:LTYELLOW>+%d<n>",iValue));
		else
		if	(bIsRequireString && _lpBasicItem)
			s_string.Add(_ms("<c:LTYELLOW>%s - %d Lv<n>" , g_aSkill[_lpBasicItem->m_aValue[0][0]].m_strName , _lpBasicItem->m_aValue[0][1]));
		else
			s_string.Add(_ms("<c:LTYELLOW>%d<n>",iValue));

		if	(s_decoder.m_isDone	)
			break;		//	다 읽어 들였다.
	}

	return	s_string.String;
}