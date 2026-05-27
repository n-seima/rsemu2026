#ifndef _classAI_H
#define	_classAI_H

#include "SFC.H"

#define	dBASE_AI_NONE			0		//	없다. 때리면... 맞고.. --;; 치료해주면... 헤~~ 웃고..
#define	dBASE_AI_DEFAULT		1		//	가만히 있는다. 단지 맞으면 때린다.
#define	dBASE_AI_WANDER			2		//	이리저리 방황한다.
#define	dBASE_AI_TRACER			3		//	적을 찾아 헤멘다.
#define	dBASE_AI_GUARD_AREA		4		//	특정 지역을 방어
#define	dBASE_AI_GUARD_PERSON	5		//	특정 캐릭터를 보호한다.
#define	dBASE_AI_ASSASIN		6		//	짱박혀 있다가 누가 오며 스윽!
#define	dBASE_AI_STRANGER		7		//	이상한 짓을 함.. --;;
#define	dBASE_AI_PLAYER			8		//	플레이어의 AI
#define	dBASE_AI_CONFUSE		9		//	혼란 상태
#define	dBASE_AI_BERSERKER		10		//	혼란 상태


extern	char	*g_strBattleAI[];

#endif