#ifndef	_classNUX_H
#define	_classNUX_H

#include	"SFC.H"

class	cNUX_INFO
{
private:
	BOOL	m_bPause,m_bCanceled;

public:
	DWORD	m_dwTotalSize,m_dwProgressSize,m_dwBeginTime,m_dwProgressTime,m_dwBps;
	DWORD	m_dwBeginCheckTime,m_dwBeginCheckSize;
	DWORD	m_dwOldTime,m_dwRemainTime;
	DWORD	m_dwFileSize,m_dwProgressFileSize;
	int		m_iProgressRate,m_iProgressFileRate,m_iFileCount,m_iCurrentFile;
	char	m_strCurrentFile[256];
	char	m_strDestFolder[512];

			cNUX_INFO()
			{
				Reset();
			}

	void	Reset()
	{
		memset(this,0,sizeof(cNUX_INFO));
		m_dwRemainTime	=	0xffffffff;
	}
	void	Update();

	void	Pause()	{m_bPause	=	TRUE;}
	void	Resume(){m_bPause	=	FALSE;}
	void	Cancel(){m_bCanceled=	TRUE;}

	BOOL	IsPaused()		{return	m_bPause;}
	BOOL	IsCanceled()	{return	m_bCanceled;}
};

#endif