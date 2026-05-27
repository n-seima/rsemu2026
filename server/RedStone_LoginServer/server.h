#pragma	once

class	cSOCKET;

extern	int			SendToAllWorld(void *_lpPacket);
extern	int			SendToWorld(void *_lpPacket,cSOCKET *_lpSocket);
extern	cSOCKET*	GetGVGServerSocket();	//	GVG 서버 소켓 구하기
extern	cSOCKET*	GetWorldServerSocket(int _iWorldServerIndex);	//	월드 서버 인덱스로 월드 서버 소켓 구하기

extern	int			g_bIsIgnoreSMS_Alarm;
extern	int			g_bIsMainTestServer;	//	본서버랑 설정이 똑같은 테섭. 결재나 몇가지 게임 외적인것만 테스트다.

