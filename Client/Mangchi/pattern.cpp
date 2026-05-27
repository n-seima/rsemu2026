#include "pattern.h"

char	*g_strPatternExample[]	=
{
"Registered pattern call","Pattern call 10%% \"Only one person loses!\"",
"Turn off invulnerability","Turn off invincibility",
"Invincibility on","Invincibility on",
"Use Action","Action Action 7 Book 3 times -1FPS",
"Move","Move 134,294",
"Use skill","Use skill \"Fire Ball\" level 0",
"Use in skill area","Use area skill \"Fire Wall\" 32,40 42,50",
"Time delay","Time delay 3000",
"end",
};

void
cCharacterPattern::reset()
{
	m_wCount	=	0;
	m_wSerial	=	0xffff;
	strcpy(m_strName,"");
}

cCharacterPatternPack::cCharacterPatternPack()
{
	m_wCount	=	0;
}

cCharacterPatternPack::~cCharacterPatternPack()
{
	m_wCount	=	0;
}

void
cCharacterPatternPack::init()
{
	clear();
}

void
cCharacterPatternPack::clear()
{
	m_wCount	=	0;
	
	for (int i=0;i<c_iCharacterPatternCountInField;i++)
		m_aPatterns[i].reset();
}

//	이름으로 패턴 찾기
int
cCharacterPatternPack::getPatternIndexByName(char *_lpstrName,int _iIgnoreSerial)
{
	for (int i=0;i<c_iCharacterPatternCountInField;i++)
	{
		if	(m_aPatterns[i].m_wCount	==	0	||	i	==	_iIgnoreSerial)
			continue;

		if	(stricmp(m_aPatterns[i].m_strName,_lpstrName)	==	0)
			return	i;
	}

	return	0xffff;
}

//	이름으로 패턴 찾기
cCharacterPattern*
cCharacterPatternPack::getPatternByName(char *_lpstrName,int _iIgnoreSerial)
{
	int	iIndex	=	getPatternIndexByName(_lpstrName,_iIgnoreSerial);

	if	(iIndex	==	0xffff)
		return	NULL;

	return	&m_aPatterns[iIndex];
}

BOOL
cCharacterPatternPack::remove(int _iSerial)
{
	if	(get(_iSerial)	==	NULL)
		return	FALSE;

	m_aPatterns[_iSerial].reset();

	m_wCount--;

	return	TRUE;
}

int
cCharacterPatternPack::add(cCharacterPattern *_lpPattern,int _iInsertSerial)
{
	if	(_iInsertSerial	!=	0xffff)
	{
		BOOL	bIsNew	=	FALSE;

		if	(m_aPatterns[_iInsertSerial].m_wSerial	==	0xffff)
			bIsNew	=	TRUE;

		memcpy(&m_aPatterns[_iInsertSerial],_lpPattern,sizeof(cCharacterPattern));

		m_aPatterns[_iInsertSerial].m_wSerial	=	_iInsertSerial;

		if	(bIsNew)
			m_wCount++;

		return	_iInsertSerial;
	}

	for (int i=0;i<c_iCharacterPatternCountInField;i++)
	{
		if	(m_aPatterns[i].m_wCount)
			continue;

		memcpy(&m_aPatterns[i],_lpPattern,sizeof(cCharacterPattern));
		m_aPatterns[i].m_wSerial	=	i;

		m_wCount++;

		return	i;
	}

	return	0xffff;
}

//	패턴 리턴
cCharacterPattern*
cCharacterPatternPack::get(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	c_iCharacterPatternCountInField	||	m_aPatterns[_iIndex].m_wCount	==	0)
		return	NULL;

	return	&m_aPatterns[_iIndex];
}

//	패턴 리턴
int
cCharacterPatternPack::getOrderBySerial(int _iSerial)
{
	int	iOrder	=	0;

	for (int i=0;i<=_iSerial;i++)
	{
		if	(m_aPatterns[i].m_wCount	==	0)
			continue;

		iOrder++;
	}

	return	iOrder;
}

int
cCharacterPatternPack::getFirstPatternSerial()
{
	for (int i=0;i<c_iCharacterPatternCountInField;i++)
	{
		if	(m_aPatterns[i].m_wCount	==	0)
			continue;

		return	i;
	}

	return	0xffff;
}

int
cCharacterPatternPack::getNextPatternSerial(int _iSerial)
{
	for (int i=_iSerial+1;i<c_iCharacterPatternCountInField;i++)
	{
		if	(m_aPatterns[i].m_wCount	==	0)
			continue;

		return	i;
	}

	return	0xffff;
}

int
cCharacterPatternPack::getPrePatternSerial(int _iSerial)
{
	for (int i=_iSerial-1;i>=0;i--)
	{
		if	(m_aPatterns[i].m_wCount	==	0)
			continue;

		return	i;
	}

	return	0xffff;
}