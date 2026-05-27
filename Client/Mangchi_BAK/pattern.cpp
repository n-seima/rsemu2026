#include "pattern.h"

char	*g_strPatternExample[]	=
{
"등록된 패턴 호출","패턴호출 10%% \"한놈만 패!\"",
"무적상태 끄기","무적상태 off",
"무적상태 켜기","무적상태 on",
"액션 사용","액션 액션7 북 3회 -1FPS",
"이동","이동 134,294",
"스킬 사용","스킬사용 \"파이어 볼\" 레벨 0",
"스킬 지역에 사용","지역스킬사용 \"파이어 월\" 32,40  42,50",
"시간 지연","시간지연 3000",
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