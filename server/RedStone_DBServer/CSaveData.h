#ifndef _classDataSave_H
#define _classDataSave_H

#include "cPLAYER_DATA.H"
#include "cADO_AVATAR.h"
#include "cPACKET_DBSERVER.h"

#define	dMAX_SAVE_DATA_COUNT	4096

/*
>>	초기화 
	void					init(char *_lpstrPath,int _iQueSize=dMAX_SAVE_DATA_COUNT);	//	초기화
	패스와 큐 사이즈를 입력한다.

>>	메인루프
	void					update();
	이자체가 무한루프 이기 때문에 그냥 쓰레드에 넣어 두면 된다.

>>	세이브 데이터 추가
	BOOL					add(cPLAYER_DATA *_lpSaveData,BOOL _bIsSaveToDB);	//	패킷 추가
	캐릭터 데이터를 넘겨주고 _bIsSaveToDB를 설정하면 저장하면서 DB에도 저장한다.

>>	세이브 데이터 확인
	BOOL					checkSaveData(cPLAYER_DATA *_lpData);
	데이터 체크 - 만약, 더 최신 데이터가 있으면 최신 정보를 담은후에 TRUE를 돌려준다.
	
>>	종료
	void					end();	//	메니저 종료
	메니저가 종료할때까지 루프를 돈다.
*/

enum{
	eGETGUILDINFO_SKILL,
	eGETGUILDINFO_PET,
	eGETGUILDINFO_GUARDIAN,
	eGETGUILDINFO_OBJECT,
	eGETGUILDINFO_ITEM,
	eGETGUILDINFO_SELL,
	eGETGUILDINFO_POLL,
};

class	CDataSaveManager
{
	char					m_strPath[512],m_strListPath[512],m_strBankPath[512];

	void					close();									//	종료
	void					reset();									//	리셋
public:
							CDataSaveManager();
							~CDataSaveManager();

	void					init(char *_lpstrPath,char *_lpstrListPath,char *_lpstrBankPath);	//	초기화

	int						save(cPLAYER_DATA *_lpData);
	inline	BOOL			isRequireResaveResult(int _iResult)
	{
		if (_iResult == eSAVE_DB_UNKNOWN_ERROR || _iResult == eSAVE_DB_FAILED)
			return	TRUE;

		return	FALSE;
	}

//	은행 관련
	BOOL					openBank(char * _lpstrId,CBankInfo * bank);
	WORD					closeBank(CBankInfo * bank);
	void					delBank(char * _lpstrId);
	void					updateBank(char * _lpstrId);

//	테스트 서버 ...
	BOOL					copyData(cAVATAR_INFO *_lpData,char *_lpstrId);
	void					makeDuelPlayer(cPLAYER_DATA	*_lpPlayer);
};

extern	CDataSaveManager	g_dsManager;
extern	cADO_AVATAR			g_adoAvatar;

#endif