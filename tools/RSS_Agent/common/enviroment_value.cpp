#include	"stdafx.h"
#include	"enviroment_value.h"
#include	"file.h"

cEnviromentValue::cEnviromentValue(int _iMaxCount)
{
	m_pValues	=	NULL;

	init(_iMaxCount);
}

cEnviromentValue::~cEnviromentValue()
{
	pKILL(m_pValues);
}

void
cEnviromentValue::init(int _iSize)
{
	pKILL(m_pValues);
	
	m_iMaxCount	=	_iSize;
	m_iCount	=	0;

	if	(_iSize)
		m_pValues	=	new cValueInfo [_iSize];
}

//	수치 추가 
bool
cEnviromentValue::addValue(void *_lpValue,int _iSize,int _iSlot)
{
	if	(_iSlot	==	-1)
	{
		for	(int i=0;i<m_iMaxCount;i++)
		{
			if	(m_pValues[i].m_wSize	==	0)
			{
				_iSlot	=	i;
				break;
			}
		}
	}

	if	(_iSlot	==	-1)
		return	false;

	if	(m_pValues[_iSlot].m_wSize)
		return	false;

	m_pValues[_iSlot].m_wSize	=	_iSize;
	m_pValues[_iSlot].m_lpBuffer=	(TCHAR*)_lpValue;
	m_iCount++;

	return	TRUE;
}

bool
cEnviromentValue::loadValue(TCHAR *_lpstrName)
{
	cFile	file;

//	if (!file.open(_lpstrName,_T("rb")) )
	if( !file.open( _lpstrName, _T("rb"), FALSE ) )
		return	false;

	int	iCount;

	file.read(&iCount,4);

	for	(int i=0;i<iCount;i++)
	{
		int		iIndex;
		WORD	wSize;
		TCHAR	*pBuffer;

		file.read(&iIndex,4);
		file.read(&wSize,2);

		pBuffer	=	new TCHAR [wSize];

		file.read(pBuffer,wSize);

		if	(m_pValues[iIndex].m_wSize	==	wSize)
			memcpy(m_pValues[iIndex].m_lpBuffer,pBuffer,wSize);

		pKILL(pBuffer);
	}

	file.close();

	return	true;
}

bool
cEnviromentValue::saveValue(TCHAR *_lpstrName)
{
	cFile	file;

	if	(!file.open(_lpstrName,_T("wb"),FALSE) )
		return	false;

	file.write(&m_iCount,4);
	
	for	(int i=0;i<m_iMaxCount;i++)
	{
		if	(m_pValues[i].m_wSize	==	0)
			continue;

		file.write(&i,4);
		file.write(&m_pValues[i].m_wSize,2);
		file.write(m_pValues[i].m_lpBuffer,m_pValues[i].m_wSize);
	}

	file.close();

	return	true;
}