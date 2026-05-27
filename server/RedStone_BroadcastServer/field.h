#ifndef	_class_community_server_field_h
#define	_class_community_server_field_h

#include	"SFC.H"
#include	"definePacketData.H"

const	int	c_iMaxUserCountInField	=	2048;
const	int	c_iMaxFieldCount		=	1024;

class	cFieldInfo
{
public:
	WORD				m_wSerial;
	WORD				m_wActiveUserCount;
	WORD				m_awUserSerial[c_iMaxUserCountInField];

						cFieldInfo()
						{
							reset();
						}

	BOOL				addUser(int _iSerial);
	void				removeUser(int _iSerial);

	void				reset();
	void				copy(cFieldInfo *_lpInfo);

	void				sendPacketToClient(void *_lpPacket);
};

class	cFieldManager
{
public:
	static	BOOL		s_bIsExist;

	cDen<cFieldInfo>	m_denField;
	CRITICAL_SECTION	m_csFieldWork;

						cFieldManager();
						~cFieldManager();

	cFieldInfo*			get(int _iSerial);
	void				reset();

	void				addUser(int _iField,int _iClient);
	void				removeUser(int _iField,int _iClient);
	void				sendPacketToClient(int _iField,void *_lpPacket);
};

extern	cFieldManager	g_fieldManager;


#endif