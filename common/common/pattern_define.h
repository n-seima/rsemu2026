#ifndef _class_character_pattern_define_h
#define _class_character_pattern_define_h

#include "SFC.H"

enum
{
	eCP_move,
	eCP_delay_time,
	eCP_use_action,
	eCP_use_skill,
	eCP_set_invincible,
	eCP_call_pattern,
	eCP_use_local_skill,
};

const	int	c_iCharacterPatternNameLength	=	20;
const	int	c_iCharacterPatternValueCount	=	10;
const	int	c_iCharacterPatternPieceCount	=	256;
const	int	c_iCharacterPatternCountInField	=	256;

class	cCharacterPatternPieceDefine
{
public:
	WORD	m_wPattern;
	int		m_aiValue[c_iCharacterPatternValueCount];
};

class	cCharacterPatternDefine
{
public:
	WORD							m_wCount;
	char							m_strName[c_iCharacterPatternNameLength];
	cCharacterPatternPieceDefine	*m_pPattern;
};

class	cCharacterPatternPackDefine
{
public:
	WORD							m_wCount;
	cCharacterPatternDefine			*m_pPatterns[c_iCharacterPatternCountInField];
};

#endif