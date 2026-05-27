#ifndef _class_server_character_pattern_h
#define _class_server_character_pattern_h

#include "SFC.H"
#include "pattern_define.H"

#pragma	pack (4)
class	cCharacterPatternPiece
{
public:
	WORD	m_wPattern;
	int		m_aiValue[c_iCharacterPatternValueCount];
};

class	cCharacterPattern
{
public:
	WORD					m_wSerial;
	WORD					m_wCount;

	cCharacterPatternPiece	*m_pPattern;

							cCharacterPattern();
							~cCharacterPattern();

	void					init();
	void					close();
};

class	cCharacterPatternPack
{
public:
	WORD				m_wCount;
	cCharacterPattern	*m_apPatterns[c_iCharacterPatternCountInField];

						cCharacterPatternPack();
						~cCharacterPatternPack();

	void				init();
	void				close();
};

#pragma	pack ()

#endif