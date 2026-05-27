#ifndef _classKARMA_DEFINE_H
#define	_classKARMA_DEFINE_H

#include "cNEMO.H"

//////////////////////////////////////////////////////////////////////////////////////////
//	트리거들
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
////	시스템 관련
////	게임시스템에 관련된 트리거
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//	시나리오
#define	dTRIGGER_START					1

//	변수
#define	dTRIGGER_VALUE					2

//	반복
#define	dTRIGGER_REPEAT					3

//	특정 지역안에 캐릭터 수
#define	dTRIGGER_CHRACTER_COUNT			4

//	날짜
#define	dTRIGGER_DATE					5

//	카운트 다운
#define	dTRIGGER_COUNT_DOWN				6

//	게임 타이머
#define	dTRIGGER_TIME					7

//	퀘스트 상태
#define	dTRIGGER_QUEST					8


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
////	캐릭터의 상태
////	캐릭터의 상태와 관련된 카르마들
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//	캐릭터의 상태(퍼센티지)
#define	dTRIGGER_STATUS_PERCENTAGE		51

//	캐릭터의 상태(수치)
#define	dTRIGGER_STATUS_VALUE			52

//	캐릭터가 죽었다
#define	dTRIGGER_DEATH					53

//	캐릭터간의 거리
#define	dTRIGGER_RANGE					54

//	캐릭터의 직업
#define	dTRIGGER_JOB					55

//	어빌러티 유무
#define	dTRIGGER_ABILITY				56

//	어빌러티 레벨
#define	dTRIGGER_ABILITY_LEVEL			57


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
////	기타
////	기타 카르마
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//	아이템
#define	dTRIGGER_ITEM					101

//	무기/장비
#define	dTRIGGER_ARMOR					102

//	돈
#define	dTRIGGER_MONEY					103

//	마법 사용
#define	dTRIGGER_USE_SKILL				104

//	게임의 주인공
#define	dTRIGGER_HERO					105

//	현재 선택한 캐릭터
#define	dTRIGGER_SELECT_HERO			106

//	파티의 인원수
#define	dTRIGGER_PARTY_COUNT			107

//	파티원
#define	dTRIGGER_IS_PARTY				108



//////////////////////////////////////////////////////////////////////////////////////////
//	반응들
//////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//	시스템설정 관련
//	게임 시스템에 관련된 각종설정
///////////////////////////////////////////////////////////////////////////////

//	게임 종료(엔딩을 보여준다.)		
#define	dREACTION_GAME_END					1

//	게임 오버
#define	dREACTION_GAME_OVER					2

//	게임 정지							
#define	dREACTION_PAUSE_GAME				3

//	이벤트							
#define	dREACTION_EVENT						4

//	시나리오 불러오기					
#define	dREACTION_LOAD_SCENARIO				5

//	게임 저장하기 가능/불가능			
#define	dREACTION_DISABLE_SAVE_LOAD			6

//	변수 상태 변경					
#define	dREACTION_SET_VALUE					7

//	카르마 설정						
#define	dREACTION_SET_KARMA					8

//	조건에 관계 없이 카르마 실행		
#define	dREACTION_LAUNCH_KARMA				9

//	액션간 시간 지연					
#define	dREACTION_DELAY_TIME				10

//	여행 일지 추가					
#define	dREACTION_ADD_JOURNAL				11

//	퀘스트							
#define	dREACTION_QUEST						12

//	카운트 다운 타이머 시간 설정		
#define	dREACTION_COUNTDOWN					13

//	카운트 다운 타이머 표시
#define	dREACTION_DISPLAY_COUNTDOWN			14

//	날짜 설정
#define	dREACTION_SET_DATE					15

//	날짜 조절
#define	dREACTION_ADD_DATE					16

//	시간이 흐르는 속도 설정
#define	dREACTION_SET_TIME_SPEED			17

//	시간 설정
#define	dREACTION_SET_TIME					18

//	시간 설정
#define	dREACTION_ADD_TIME					19

//	문 열기/닫기						
#define	dREACTION_DOOR						20

//	상자 열기/닫기					
#define	dREACTION_BOX						21

//	게임 타이머 리셋
#define	dREACTION_RESET_GAME_TIMER			22

//	이벤트 경계						
#define	dREACTION_BORDER					23

///////////////////////////////////////////////////////////////////////////////
//	캐릭터 데이타 설정
//	캐릭터에 관련된 각종 데이터를 설정합니다.
///////////////////////////////////////////////////////////////////////////////

//	캐릭터 이름 변경					
#define	dREACTION_CHANGE_NAME				51

//	캐릭터의 팀 변경					
#define	dREACTION_CHANGE_TEAM				52

//	캐릭터의 능력치 설정				
#define	dREACTION_SET_POWER					53

//	캐릭터의 능력치 조절				
#define	dREACTION_ADD_POWER					54

//	캐릭터의 상태 설정				
#define	dREACTION_SET_STATUS				55

//	캐릭터의 상태 해제				
#define	dREACTION_OFF_STATUS				56

//	캐릭터 위치 변경					
#define	dREACTION_CHARACTER_POS				57

///////////////////////////////////////////////////////////////////////////////
//	마법/어빌러티 관련
//	마법/어빌러티 설정이나 마법을 이용한 연출에 관련된 카르마들
///////////////////////////////////////////////////////////////////////////////

//	어빌러티 추가						
#define	dREACTION_ADD_ABILITY				101

//	어빌러티 제거						
#define	dREACTION_REMOVE_ABILITY			102

//	어빌러티 레벨 설정				
#define	dREACTION_SET_ABILITY_LEVEL			103

//	어빌러티 레벨 조절				
#define	dREACTION_ADD_ABILITY_LEVEL			104

//	마법 효과 발생(캐릭터)			
#define	dREACTION_OCCUR_MAGIC_IN_AREA		105

//	마법 효과 발생(특정 위치)			
#define	dREACTION_OCCUR_MAGIC_IN_CHARACTER	106

//	특정 지역에 마법 사용				
#define	dREACTION_MAGIC_AT_AREA				107

//	특정 유닛에게 마법 사용			
#define	dREACTION_MAGIC_AT_CHARACTER		108


///////////////////////////////////////////////////////////////////////////////
//	캐릭터 행동관련
//	캐릭터의 행동에 관련된 카르마
///////////////////////////////////////////////////////////////////////////////

//	캐릭터 이동(위치)					
#define	dREACTION_MOVE						151

//	캐릭터 따라다님					
#define	dREACTION_FOLLOW					152

//	캐릭터 생성(지역)					
#define	dREACTION_CREATE_IN_AREA			153

//	캐릭터 제거						
#define	dREACTION_REMOVE_CHARACTER			154

//	캐릭터를 죽임						
#define	dREACTION_KILL						155

//	정지								
#define	dREACTION_STOP						156

//	방향 바꿈(방향)					
#define	dREACTION_SET_DIRECT				157

//	바라보기							
#define	dREACTION_GAZE_HERO					158

//	특정 지역에 마법 사용				
#define	dREACTION_USE_MAGIC_AT_AREA			159

//	특정 유닛에게 마법 사용			
#define	dREACTION_USE_MAGIC_AT_CHARACTER	160

//	공격한다.
#define	dREACTION_ATTACK					161


///////////////////////////////////////////////////////////////////////////////
//	파티 관련
//	파티원에 관련된 카르마들
///////////////////////////////////////////////////////////////////////////////

//	파티원 추가						
#define	dREACTION_ADD_PARTY					201

//	파티원 제거						
#define	dREACTION_REMOVE_PARTY				202

//	파티의 리더 설정					
#define	dREACTION_SET_LEADER				203

///////////////////////////////////////////////////////////////////////////////
//	무기/아이템 관련
//	무기나 아이템에 관련된 카르마
///////////////////////////////////////////////////////////////////////////////

//	아이템 추가						
#define	dREACTION_ADD_ITEM_CHARACTER		251

//	아이템 제거						
#define	dREACTION_REMOVE_ITEM_HERO			252

//	아이템 추가/제거(필드)			
#define	dREACTION_ITEM_FIELD				253

//	장비								
#define	dREACTION_ARMOR						254

///////////////////////////////////////////////////////////////////////////////
//	게임 연출
//	각종 연출들
///////////////////////////////////////////////////////////////////////////////

//	말하기							
#define	dREACTION_TALK						301

//	화면에 메시지 출력				
#define	dREACTION_DISPLAY_TEXT				302

//	배경음악 연주(.wav/.mid/.mp3)		
#define	dREACTION_PLAY_MUSIC				303

//	배경음 끄기/켜기					
#define	dREACTION_SET_MUSIC					304

//	효과음(.wav)						
#define	dREACTION_PLAY_SOUND				305

//	이미지 출력(pcx)					
#define	dREACTION_VIEW_IMAGE				306

//	포커스 이동(위치)					
#define	dREACTION_FOCUS_AREA				307

//	포커스 이동(유닛)					
#define	dREACTION_FOCUS_CHARACTER			308

//	날씨 효과(비/눈)					
#define	dREACTION_WHETHER					309

//	화면 효과(페이드 인/아웃)			
#define	dREACTION_FX_FADE					310

//	화면 효과(번쩍 거림)				
#define	dREACTION_FX_SPLASH					311

//	화면 효과(흔들림)					
#define	dREACTION_FX_SHAKE					312

#endif