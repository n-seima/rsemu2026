#ifndef	_process_thread_h
#define	_process_thread_h

#include	"SFC.H"

extern	BOOL RecvPacket(int serial);
extern	BOOL SendPacket(int serial);
extern	void SendPacket_Add(int serial);

#endif