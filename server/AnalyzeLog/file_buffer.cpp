#include "file_buffer.h"
#include <io.h>
#include <process.h>

BOOL
cFileBuffer::open(char *_lpFn)
{
	m_iReadLength	=	0;
	m_pFP			=	fopen(_lpFn,"rb");

	if	(!m_pFP)
		return	FALSE;

	m_iLength	=	filelength(fileno(m_pFP));

	if	(m_pBuffer)
		delete [] m_pBuffer;

	m_iBP		=	0;
	m_pBuffer 	=	new char [m_iLength+1]; 

	return	TRUE;
}

BOOL
cFileBuffer::uploadDataToBuffer(int _iSize)
{
	if	(m_iReadLength+_iSize	>	m_iLength)
		_iSize	=	m_iLength-m_iReadLength;

	fread(m_pBuffer+m_iReadLength,_iSize,1,m_pFP);
	m_iReadLength	+=	_iSize;

	if	(m_iReadLength	>=	m_iLength)
	{
		fclose(m_pFP);
		m_pFP	=	NULL;

		return	FALSE;
	}

	return	TRUE;
}

void
cFileBuffer::saveFile(char *_lpstrPath)
{
	cFILE::Burn(_lpstrPath,m_pBuffer,m_iLength);
}
