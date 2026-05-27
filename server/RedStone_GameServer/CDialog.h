#ifndef _ClassDIALOG_H
#define _ClassDIALOG_H

#include "cKARMA.H"

#define	dMAX_CONVERSATION_OPENNER_COUNT		50
#define	dMAX_CONVERSATION_COUNT				100
#define	dMAX_CONVERSATION_CONTENTS_COUNT	100
#define	dMAX_DIALOG_TRIGGER_COUNT			100
#define	dMAX_SPEECH_LENGTH					2048

class cACTOR;

class	CSpeech
{
public:
	WORD	m_wConversationIndex;
	WORD	m_wContentsCount;
	WORD	m_wLength;
	WORD	m_wDialogBoxSize;
	char	m_strText[dMAX_SPEECH_LENGTH];

			CSpeech()
			{
				m_wConversationIndex	=	0xffff;
			}
};

class	CConversationBasicInfo
{
public:
	WORD				m_wSerial;			//	시리얼
	WORD				m_wSpeechLength;	//	대사 길이
	WORD				m_wContentsCount;	//	선택 항목 수
	WORD				m_wTriggerCount;	//	이 대화가 처음에 시작될 조건 수

	WORD				m_isWantAndCondition: 1;
	WORD				m_isWantNotComplete	: 1;
	WORD				m_bf7ActivateTriggerCount	:	7;
	WORD				m_spareValue		: 7;

	WORD				m_wIsNoSpeech;		//	대화 없음
};

//
//	대화
class	CConversation : public CConversationBasicInfo
{
public:
	char				*m_pstrSpeech;		//	대사
	CKarmaItem			*m_pTrigger;		//	이 대화가 처음에 선택될 조건
	CKarmaContainer		*m_pContents;		//	선택 항목


						CConversation()
						{
							m_pstrSpeech	=	NULL;	//	대사
							m_pContents		=	NULL;	//	선택 항목
							m_pTrigger		=	NULL;
							reset();
						}

						~CConversation()	{destroy();}

	BOOL				init();		//	초기화
	BOOL				init(int _iTriggerCount,int _iContentsCount);		//	초기화
	void				reset();	//	리셋
	void				destroy();	//	자폭 -o-

	char*				getTitle();						//	제목 얻어오기(대사에서 일정길이 잘라옴)
	void				setSpeech(char *_lpstrSpeech);	//	대사 설정
	BOOL				isCompleteTrigger(cACTOR *_lpAsker,cACTOR *_lpOwner);
	BOOL				operateReaction(int _iSpeech);//	리액션
	BOOL				getSpeech(CSpeech* _lpReturnBuffer);//	대화 리턴
	BOOL				isExistShop(int _iSerial);	//	이런 상점을 언급하냐?
};

//
//	대화 컨테이너
class	CConversationContainer
{
public:
	WORD				m_wSerial;
	WORD				m_wConversationCount;
	WORD				m_wDialogBoxSize;
	WORD				m_wIsRandomChoiceDialg;
	CConversation		*m_pConversation;
	WORD				m_wIsEventArea;

						CConversationContainer();
						~CConversationContainer();

	BOOL				init();		//	초기화
	BOOL				init(int _iConversationCount);		//	초기화
	void				reset();	//	리셋
	void				destroy();	//	자폭 -o-

	BOOL				operateFirstSpeech(cACTOR *_lpAsker,cACTOR *_lpOwner,WORD _wEventArea=0xffff);
	BOOL				operateSelectSpeechReaction(cACTOR *_lpAsker,cACTOR *_lpOwner,int _iSpeechIndex,int _iSelectSpeech);//선택한 문장의 액션 처리

	BOOL				isExistShop(int _iSerial);	//	이런 상점을 언급하냐?
};

#endif