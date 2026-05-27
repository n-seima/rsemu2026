#ifndef	_classStressTest_h
#define	_classStressTest_h

#include "SFC.H"

class	cStressTestReportClient
{
public:
	WORD	m_wSerial;
};

class	cStressTestManager
{
public:
	int		m_aiRepoterList[1024];
	int		m_aiClient[1024*10][2];

			cStressTestManager();
			
	void	reset();
	BOOL	addReporter(int	_iSerial);
	BOOL	addClient(int _iSerial);
	void	sendReport();
	void	updateJoin();
};


extern	cStressTestManager	*g_pStressTestManager;

#endif