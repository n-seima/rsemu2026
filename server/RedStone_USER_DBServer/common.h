#pragma once

#include "SFC.H"

class	CClient;

extern	void	SendPacket(CClient * _cpUser,void* _lpPacket);
extern	void	InitWeeklyPvPInfo();	//	주간 랭킹/WCG 랭킹을 산정 하고 모든 계정의 주간 전적을 초기화 시킨다.
extern	void	InitDailyPvPInfo();		//	일간 랭킹 정보를 초기화 시킨다.
extern	void	SendDailyDuelRankingInfo(CClient * _cpUser=NULL);		//	일간/WCG 랭킹 정보 전송
extern	void	SendWeeklyDuelRankingInfo(CClient * _cpUser=NULL);	//	주간 랭킹 정보 전송
extern	void	MakeDailyDeulRankingInfo();	//	일간 랭킹/WCG 랭킹 산정
