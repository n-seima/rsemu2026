// cGuildManager.h: interface for the cGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGUILDMANAGER_H__82C6A693_5364_4B7B_8A03_D78431020EFB__INCLUDED_)
#define AFX_CGUILDMANAGER_H__82C6A693_5364_4B7B_8A03_D78431020EFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CGuildDefine.h"
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include <mmsystem.h>
#include "STACK.H"
#include "RSComm.h"

class cGuild : public CGuildAdvanceInfo
{
public:
	WORD				wSerial;
	WORD				wPre;		//	0xffff - 초기상태 , 1 - 임시 등록된 상태. 0 - 정식등록된 상태
	DWORD				dwUpdateTime;
	DWORD				dwHashValue;

	CGuildMan			m_stack[dGUILD_MAX_PLAYER];
	WORD				m_wNext,m_wCount;
	CRITICAL_SECTION	csGuild;

						cGuild();
	virtual				~cGuild();

	DWORD				getExpSum20040602();
	BOOL				resetGuildExp20040602();

	void				close();
	inline	int			GetCount(){	return m_wCount;	}
	void				GetNextEmpty();
	
	int					getCheckSum();
	void				updateGuildInfo();
	int					increaseGuildExp(DWORD _dwExp);			//	길드 경험치를 증가 시키고 증가한 레벨을 반환한다.
	BOOL				makeLevel(int _iLevel);	//	레벨 변경
	int					increaseGuildSkillLevel(int _iSkill,CGuildSkillInfo *_lpSkillInfo);
	int					getCurrentGuildPointSum();
	
	//	길드 스킬레벨 증가 시키고 결과를 리턴, 증가한 스킬에 대한 정보는 _lpSkillInfo에 담는다.
	WORD				Add(CGuildMan * _p);
	void				Del(char *);
	void				Del(WORD _serial);
	CGuildMan	*		getMember(WORD _wSerial);
	CGuildMan	*		GetGuilderPnt(WORD);
	CGuildMan	*		GetGuilderPnt(char *);
	CGuildMan	*		GetGuilderPntByID(char * _strID);

	CGuildMan	*		GetNextMaster();

	BOOL				isMaster(char * _strMember);
	BOOL				isSubMaster(char * _strMember);
	BOOL				isSubOrMaster(char * _strMember);
	WORD				GetRank(char * _strMember);

	WORD				CheckJoinMember(char * _strMaster, char * _strMember);
	WORD				CheckExitMember(char * _strMaster, char * _strMember);

	void				captureGuildHall(int _iHallLevel,int _iHallSlot);

	inline void			writeNotice(WORD _wType,char *_strNotice)		//	공지사항 쓰기
	{
		m_wNoticeType = _wType;
		strcpy(m_strNotice,_strNotice);
	}

	//	계급 이동 : 길드 이름 , 길드원 이름 , 이동할 계급
	int				moveClass(char *ava_name, WORD c);
	//	마스터 물려주기 : 새로운 마스터될 유저이름 (기존 마스터는 계급이동-> 원로)
	int				moveMaster(char *ava_name);
	//	길드 인벤토리 사용(넣기 , 꺼내기)
//	int				useInven(int type , 아이템 정보 );
	//	길드 인벤토리 공간 늘리기
	int				addInven();
	//	접속중인 길드원들에게 길드 정보 보내기
	//				체팅 메세지 보내기

	//	길드 경험치 더하기 , 레벨 처리 , 스킬 포인트 부여 
	int				addExp(int e);
	//	포인터 사용하기
//	int				usePoint(사용하려는 곳 );
	//	기부금 내기
	int				sendGift(int g);
	//	기부금 사용
	int				useGift(int g);

	int				getPetLevel(int _iExp);

};

class cGuildManager
{
private:
	CRITICAL_SECTION	csDeque;

public:
	cGuildManager();
	virtual				 ~cGuildManager();
	static cGuildManager &GetInstance();	//	항상 동일한 인스턴스를 준다.

	cGuild				m_stack[dGUILD_MAX_COUNT];
	WORD				m_wNext,m_wCount;
	BOOL				m_bIsLoadGuildHallInfo;
	cGuildHallInfo		m_aHallList[6][c_iGuildHallCount];
	CTimeInfo			m_timeLastUpdate;

	inline WORD			Size()
	{	
		return m_wCount;
	}
	void				GetNextEmpty();
	WORD				Add(cGuild * _p);
	CGuildMan		*	GetGuildManPnt(char * _strName);	//	주어진 이름의 캐릭터가 길드에 가입되었다면 포인터를 돌려준다.
	cGuild			*	GetGuildPnt(char *);		//	길드 이름으로 찾아본다. 
	cGuild			*	GetGuildPnt(WORD);			//	월드에 등록된 인덱스로 찾아본다.
	cGuild			*	GetGuildPntBySerial(WORD);	//	고유 시리얼로 찾아본다.
	void				Del(char *);				//	길드 삭제
	WORD				GetBlankGuildSerial();		//	비어있는 고유 시리얼을 찾아본다. 

	void				buildHallInfoByGuild();					//	길드 정보로 길드 홀 정보 세팅
	void				checkHallInfoByGuild();					//	길드 홀 정보에 문제가 없나 확인한다.
	void				buildGuildHallInfoByHall();				//	길드 홀 정보로 길드의 홀 정보를 세팅
	void				buildGuildHallInfoByEndGuildProlog(BOOL _bIsOnlyTestGuild = FALSE);	//	프롤로그가 끝났다.

	int					getBattleFieldByHall(int _iHallLevel,int _iHallSlot);	//	전투 필드 시리얼 리턴
	int					getInvalidHallCount(int _iHallLevel);	//	비어 있는 길드 홀 수 체크
	void				resetGuildHallInfo();					//	길드 홀 정보 초기화
	void				setInvadeGuild(int _iHallLevel,int _iOrder,int _iGuild);	//	공성전 할 길드 설정
	void				resetInvadeGuildInfo();	//	공성전 할 길드 정보 초기화
	void				decreasePetAndGuardianExp();
	void				rebuildHallInfoByRemoveGuild(int _iGuild);	//	_iGuild가 제거 되어 _iHallLevel레벨 홀의 순서를 정리 한다.

	void				captureGuildHall(int _iHallLevel,int _iHallSlot);	//	공성 성공
	void				defenseGuildHall(int _iHallLevel,int _iHallSlot);	//	수성 성공

	void				increasePetExp(int _iGuild,int _iPetType,int _iExp);
	void				changePetExp(int _iGuild,int _iPetType,int _iExp); //  펫경험치 변경
	void				toggleGuildHallOpenStatus(int _iGuild);
	void				activeGuildMasterGrace(int _iGuild);
	void				resetGuildHonorPoint();	//	길드 프롤로그 포인트 초기화

	void				setGuildHonorPoint(int _iGuild,int _iPoint);
	void				resetGuildPrologPoint();	//	길드 프롤로그 포인트 초기화
	void				changeGuildHallLevel(int _iGuild,int _iHallLevel);
	void				changeGuildVillageInfo(int _iGuild,WORD _wValue1 ,WORD _wValue2 , WORD _wValue3 , WORD _wValue4 , 
									  WORD _wValue5 , WORD _wValue6, WORD _wValue7);
	void				changeGuildVillageInfo(int _iGuild,WORD _wZeroCheckVillage ,WORD _wMaxCheckVillage);
	void				changeGuildMasterGrace(int _iGuild,WORD _wValue1);
	void				changeGuildETCInfo(int _iGuild, WORD _wRoenGoodWill , WORD _wGuildMasterGrace);
	void				changeGuildMarkTwinkleStatus(int _iGuild,BOOL _bIsTwinkle);
	void				setGuildPrologPoint(int _iGuild,int _iPoint);

	void				endGuildProlog();
	void				rentGuildPet(int _iGuild,int _iPetType,int _iMember);	//	길드 펫 대여
	void				retrievalRentedGuildPet(int _iGuild,int _iMember);		//	길드 펫 회수
	int					getGuildHallFieldSerial(int _iHallLevel,int _iHallSlot,int _iHallShape);	//	길드 홀 필드 시리얼 구하기

	cGuildHallInfo*		getHallByGuild(cGuild *_lpGuild);	//	길드 홀 정보 리턴
	DWORD				getGuildHallCheckSum();	//	길드홀 정보 체크섬
};

extern	cGuild	*GetGuild(int _iIndex);

#endif // !defined(AFX_CGUILDMANAGER_H__82C6A693_5364_4B7B_8A03_D78431020EFB__INCLUDED_)