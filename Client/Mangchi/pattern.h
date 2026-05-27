#ifndef _class_mangchi_character_pattern_h
#define _class_mangchi_character_pattern_h

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
	char					m_strName[c_iCharacterPatternNameLength];

	cCharacterPatternPiece	m_aPattern[c_iCharacterPatternPieceCount];

	void					reset();
};

class	cCharacterPatternPack
{
public:
	WORD				m_wCount;
	cCharacterPattern	m_aPatterns[c_iCharacterPatternCountInField];

						cCharacterPatternPack();
						~cCharacterPatternPack();

	void				init();
	void				clear();

	int					getPatternIndexByName(char *_lpstrName,int _iIgnoreSerial=0xffff);	//	이름으로 패턴 찾기
	cCharacterPattern*	getPatternByName(char *_lpstrName,int _iIgnoreSerial=0xffff);	//	이름으로 패턴 찾기
	int					add(cCharacterPattern *_lpPattern,int _iInsertSerial=0xffff);	//	패턴 추가
	BOOL				remove(int _iSerial);
	cCharacterPattern*	get(int _iIndex);	//	패턴 리턴
	int					getOrderBySerial(int _iSerial);
	int					getFirstPatternSerial();
	int					getNextPatternSerial(int _iSerial);
	int					getPrePatternSerial(int _iSerial);
};

extern	char	*g_strPatternExample[];

#pragma	pack ()

#endif