#ifndef _ClassBookingWork_h
#define _ClassBookingWork_h

#include "SFC.H"
#include "definePacketData.H"

#define	dMAX_BOOKED_WORK_COUNT		1000

//
//	Bookedg Work
enum
{
	eBW_REMOVE_PARTY_MEMBER,
};

class	CBookedWorkBaseForm
{
public:
	WORD	m_wWork;
};

//
//	파티멤버 제거
class	CBWRemovePartyMember
{
public:
	WORD	m_wWork;

	WORD	m_wParty;
	char	m_strMemberName[dNAME_LENGTH];
	WORD	m_wIsExceptSendMessageToRemoveMember;
};	//	class	CBWRemovePartyMember


union	UAllBookedWork
{
	CBookedWorkBaseForm				base;	//	기본 패킷

	CBWRemovePartyMember			m_CBWRemovePartyMember;
};

//
//	
class	CBW_Manager
{
public:
	WORD				m_wBookedWorkCount;

	UAllBookedWork		m_aBookedWorkList[dMAX_BOOKED_WORK_COUNT];

						CBW_Manager();
						~CBW_Manager();

	BOOL				init();
	void				reset();
	void				close();

	BOOL				bookingWork(UAllBookedWork *_lpWork);	//	예약
	UAllBookedWork*		popBookedWork();	//	예약된 작업 하나씩 꺼냄
};	//	class	CBW_Manager

#endif