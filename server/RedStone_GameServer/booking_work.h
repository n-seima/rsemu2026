#ifndef	_classBookingWork_h_
#define	_classBookingWork_h_

#include	"SFC.H"
#include	"cFIELD.H"
#include	"cArea.H"
#include	"CProfiler.h"
#include	"debugCode.h"
#include	"Client.h"

#define	dMAX_BOOKING_JOIN_PLAYER_COUNT			512
#define	dMAX_BOOKING_MOVE_FIELD_USER_COUNT		2048

class	cBookedJoinPlayerData
{
public:
	int					m_iClientSerial;
	int					m_iFieldSerial;
	BOOL				m_bIsFirstConnect;
	CPlayerData			data;
	cActorStatusUpkeep	m_upkeepStatus;

	void				reset()	{}
	void				copy(cBookedJoinPlayerData *_lpData){}
};

class	cJoinPlayerManager
{
public:
	CRITICAL_SECTION		m_cs;

	cBookedJoinPlayerData	m_aData[dMAX_BOOKING_JOIN_PLAYER_COUNT];
	int						m_iCount;

							cJoinPlayerManager();
							~cJoinPlayerManager();

	int						booking(CPlayerSaveData *_lpPlayerData,CUpkeepPlayerData *_lpUpkeepData,
									cActorStatusUpkeep *_lpUpkeepStatus,int _iClientSerial,BOOL _bIsFirstConnect);
	void					operate();
};

class	cBookedMoveFieldUserData
{
public:
	char				m_strName[dNAME_LENGTH];
	char				m_strFieldFileName[64];
	int					m_iUserSerial;
	int					m_iFieldSerial;
	int					m_iGateIndex;
	int					m_iX,m_iY;

	void				reset()	{}
	void				copy(cBookedMoveFieldUserData *_lpData){}
};

class	cMoveFieldUserManager
{
public:
	CRITICAL_SECTION			m_cs;

	cBookedMoveFieldUserData	m_aData[dMAX_BOOKING_MOVE_FIELD_USER_COUNT];
	int							m_iTail,m_iHead;

								cMoveFieldUserManager();
								~cMoveFieldUserManager();

	BOOL						booking(cACTOR *_lpActor,char *_lpstrFieldFileName,int _iFieldIndex,int _iGate=0xffff,int _iX=-1,int _iY=-1);
	void						operate();
};

extern	cJoinPlayerManager*		g_pJoinPlayerManager;
extern	cMoveFieldUserManager*	g_pMoveFieldUserManager;

#endif