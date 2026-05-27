#pragma	once

#include	"SFC.h"

class	cFileBuffer
{
public:
	FILE	*m_pFP;
	char	*m_pBuffer;
	int		m_iBP;
	int		m_iLength,m_iReadLength;

			cFileBuffer()
			{
				m_pBuffer	=	NULL;
			}
			~cFileBuffer()
			{
				if	(m_pFP)
					fclose(m_pFP);

				pKILL(m_pBuffer);
			}

	BOOL	read(char *_lpData,int _iSize)
	{
		if	(m_iBP+_iSize	>	m_iLength)
			return	FALSE;

		memcpy(_lpData,m_pBuffer+m_iBP,_iSize);
		m_iBP	+=	_iSize;

		return	TRUE;
	}

	BOOL	uploadDataToBuffer(int _iSize);

	void	addBP(int _iAddBP)
	{
		m_iBP	+=	_iAddBP;
	}
	void	setBP(int _iBP)
	{
		m_iBP	=	_iBP;
	}

	BOOL	open(char *_lpFn);

	void	close()
	{
		if	(m_pBuffer)
			delete [] m_pBuffer;

		m_pBuffer	=	NULL;
	}

	void	saveFile(char *_lpstrPath);
};