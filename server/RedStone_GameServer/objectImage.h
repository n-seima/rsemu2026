#ifndef	_classObjectImage_h
#define	_classObjectImage_h

#include	"SFC.H"

class	cObjectCrashInfo
{
public:
	BYTE	*m_pCrashInfo;

			cObjectCrashInfo()
			{
				m_pCrashInfo	=	NULL;
			}
			
			~cObjectCrashInfo()
			{
				pKILL(m_pCrashInfo);
			}

};

class	cObjectCrashInfoManager
{
public:
	cObjectCrashInfo	m_aData[40][1024];

	BOOL				init();
};

extern	cObjectCrashInfoManager	g_ociManager;

#endif