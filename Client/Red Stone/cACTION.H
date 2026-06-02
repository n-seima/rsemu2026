#ifndef _classACTION_H
#define _classACTION_H

#include "cNEMO.H"

#define	dMINIPET_ACT_READY		0
#define	dMINIPET_ACT_MOVE		1
#define	dMINIPET_ACT_ACTION		2
#define	dMINIPET_ACT_SUMMON		3
#define	dMINIPET_ACT_RELEASE	4

//	공통 에니메이션 분류
#define	dACT_WALK				0		//	걷는다.
#define	dACT_RUN				1		//	뛴다.
#define	dACT_READY				2		//	정지(전투중)
#define	dACT_HIT				3		//	맞았다.
#define	dACT_CHANGE1			4		//	무기 바꿈
#define	dACT_CHANGE2			5		//	무기 바꿈
#define	dACT_DEAD				6		//	죽음
#define	dACT_SITDOWN			7		//	앉기
#define	dACT_ACTION_1			8		//	액션 1
#define	dACT_ACTION_2			9		//	액션 2
#define	dACT_ACTION_3			10		//	액션 3
#define	dACT_ACTION_4			11		//	액션 4
#define	dACT_ACTION_5			12		//	액션 5
#define	dACT_ACTION_6			13		//	액션 6
#define	dACT_ACTION_7			14		//	액션 7
#define	dACT_ACTION_8			15		//	액션 8
#define	dACT_ACTION_9			16		//	액션 9
#define	dACT_ACTION_10			17		//	액션 10
#define	dACT_ACTION_11			18		//	액션 11
#define	dACT_ACTION_12			19		//	액션 12
#define	dACT_ACTION_13			20		//	액션 13
#define	dACT_ACTION_14			21		//	액션 14
#define	dACT_ACTION_15			22		//	액션 15
#define	dACT_ACTION_16			23		//	액션 16

#define	dACT_RIDE_DOG_MOVE		12		//	개타고 이동
#define	dACT_RIDE_DOG_READY		13		//	개타고 정지
#define	dACT_RIDE_DOG_ATTACK	14		//	개타고 공격

#define	dACT_MONSTER_BLOCKING	16		//	액션 9

#define	dACT_RIDE_DOG_ACTION_COUNT		3	//	개타고 하는 액션 수
#define	dACT_PRINCE_JUMPPING_SKIPPING	18	//	개타고 하는 액션 수


const	int	ct_aBlockingAction[16]	=
{
	13,17,
	dACT_HIT,dACT_HIT,
	12,dACT_HIT,
	23,23,
	14,dACT_HIT,
	dACT_HIT,dACT_HIT,
	dACT_HIT,dACT_HIT,
	dACT_HIT,dACT_HIT,
};

#define	dACT_DEFAULT_ATTACK		100		//	기본 공격 액션
#define	dACT_DEFAULT_MAGIC		101		//	기본 마법 액션

#define	dACT_BLOCKER			19

#define	dDIRECT_N	0
#define	dDIRECT_NE	1
#define	dDIRECT_E	2
#define	dDIRECT_SE	3
#define	dDIRECT_S	4
#define	dDIRECT_SW	5
#define	dDIRECT_W	6
#define	dDIRECT_NW	7

inline	int	GetDirect(int _iAngle,int _iDirectCount)
{
	if	(_iDirectCount	==	0)
		return	0;

	_iAngle		=	_iAngle + 360/_iDirectCount/2;
	
	if (_iAngle	>= 360) _iAngle-= 360;
	_iAngle		=	_iAngle*_iDirectCount/360;

	if (_iAngle	<=	_iDirectCount/4)	return	_iDirectCount/4-_iAngle;
	
	return	_iDirectCount-(_iAngle-_iDirectCount/4);
}

#endif