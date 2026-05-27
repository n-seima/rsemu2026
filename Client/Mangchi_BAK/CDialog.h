#ifndef _ClassDIALOG_H
#define _ClassDIALOG_H

#include "cKARMA.H"

#define	dMAX_CONVERSATION_OPENNER_COUNT		50
#define	dMAX_CONVERSATION_COUNT				1024
#define	dMAX_CONVERSATION_CONTENTS_COUNT	1024
#define	dMAX_DIALOG_TRIGGER_COUNT			100

class	cACTOR;

class	CConversationBasicInfo
{
public:
	WORD					m_wSerial;			//	시리얼
	WORD					m_wSpeechLength;	//	대사 길이
	WORD					m_wContentsCount;	//	선택 항목 수
	WORD					m_wTriggerCount;	//	이 대화가 처음에 시작될 조건 수

	WORD					m_isWantAndCondition: 1;
	WORD					m_isWantNotComplete	: 1;
	WORD					m_bf7ActivateTriggerCount	:	7;
	WORD					m_spareValue		: 7;

	WORD					m_wIsNoSpeech;		//	대화 없음
	BOOL					isSame(CConversationBasicInfo* _lpConversationBasicInfo)
	{

		if(m_wSerial != _lpConversationBasicInfo->m_wSerial)
			return FALSE;

		if(m_wSpeechLength != _lpConversationBasicInfo->m_wSpeechLength)
			return FALSE;

		if(m_wContentsCount != _lpConversationBasicInfo->m_wContentsCount)
			return FALSE;

		if(m_wTriggerCount != _lpConversationBasicInfo->m_wTriggerCount)
			return FALSE;
		
		if(m_isWantAndCondition != _lpConversationBasicInfo->m_isWantAndCondition)
			return FALSE;
		if(m_isWantNotComplete != _lpConversationBasicInfo->m_isWantNotComplete)
			return FALSE;
		if(m_bf7ActivateTriggerCount != _lpConversationBasicInfo->m_bf7ActivateTriggerCount)
			return FALSE;
		if(m_spareValue != _lpConversationBasicInfo->m_spareValue)
			return FALSE;
		
		if(m_wIsNoSpeech != _lpConversationBasicInfo->m_wIsNoSpeech)
			return FALSE;

		return TRUE;
	}
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
	void				allocSpeechMemory();	//	메모리 할당
	void				allocTriggerMemory();	//	메모리 할당
	void				allocContentsMemory();	//	메모리 할당

	BOOL				init(int _iTriggerCount,int _iContentsCount);		//	초기화
	void				reset();	//	리셋
	void				destroy();	//	자폭 -o-

	char*				getTitle();						//	제목 얻어오기(대사에서 일정길이 잘라옴)
	void				setSpeech(char *_lpstrSpeech);	//	대사 설정
	void				copy(CConversation *_lpConversation,BOOL _isForEdit=FALSE);	//	복사

	BOOL				addContent(CKarmaContainer *_lpKc);//	항목 추가
	BOOL				removeContent(int _iIndex);//	항목 제거

	BOOL				addTrigger(CKarmaItem *_lpKarma);
	BOOL				removeTrigger(int _iIndex);//	항목 제거
	BOOL				isSame(CConversation* _lpConversation)
	{
		if(!((CConversationBasicInfo*)this)->isSame((CConversationBasicInfo*)_lpConversation))
			return FALSE;


		if(m_wSpeechLength)
		{
			if(strcmp(m_pstrSpeech,_lpConversation->m_pstrSpeech) != 0)
				return FALSE;
		}

		for(int iTrigger = 0;iTrigger<m_wTriggerCount;++iTrigger)
		{
			if(!m_pTrigger[iTrigger].isSame(&_lpConversation->m_pTrigger[iTrigger]))
				return FALSE;

		}

		for(int iContents = 0;iContents<m_wContentsCount;++iContents)
		{
			if(!m_pContents[iContents].isSame(&_lpConversation->m_pContents[iContents]))
				return FALSE;

		}
		return TRUE;
	}
};	//	class	CConversation

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

						CConversationContainer();
						~CConversationContainer();

	BOOL				init();		//	초기화
	BOOL				init(int _iConversationCount,BOOL _bIsAllocBuffer=FALSE);		//	초기화
	void				reset();	//	리셋
	void				destroy();	//	자폭 -o-

	BOOL				addSpeech(CConversation *_lpcNewSpeech);	//	대화 추가
	BOOL				removeSpeech(int _iIndex);	//	대화 추가
	CKarmaItem*			findContents(int _iShopIndex=0xffff);//상점과 관련된 컨텐츠 구하기
	CKarmaItem*			findShopContents(int _iShopIndex=0xffff);//상점과 관련된 컨텐츠 구하기
	CKarmaItem*			findSpeech(int _iIndex=0xffff);// 특정 대화를 사용하는가?
	int					checkIncorrectKarma(cACTOR *_lpActor);

	void				copy(CConversationContainer *_lpConversationContanier,BOOL _isForEdit=FALSE);
	BOOL				isSame(CConversationContainer* _lpConversationContainer)
	{
		if(m_wSerial != _lpConversationContainer->m_wSerial)
			return FALSE;
		if(m_wConversationCount != _lpConversationContainer->m_wConversationCount)
			return FALSE;
		if(m_wDialogBoxSize != _lpConversationContainer->m_wDialogBoxSize)
			return FALSE;
		if(m_wIsRandomChoiceDialg != _lpConversationContainer->m_wIsRandomChoiceDialg)
			return FALSE;

		if(!m_pConversation || !_lpConversationContainer->m_pConversation)
		{
			if((m_pConversation && !_lpConversationContainer->m_pConversation) || (!m_pConversation && _lpConversationContainer->m_pConversation))
				return FALSE;
		}
		else
		{
			for(int i=0;i<m_wConversationCount ;++i)
			{
				if(!m_pConversation[i].isSame(&_lpConversationContainer->m_pConversation[i]))
					return FALSE;
			}
		}

		return TRUE;
	}
};	//	class	CConversationContainer

extern	char					g_strCurrentSpeechList[dMAX_CONVERSATION_COUNT][64];
extern	CConversationContainer	g_clipConversationContainer;
extern	CConversationContainer	g_clipConversation;

#endif