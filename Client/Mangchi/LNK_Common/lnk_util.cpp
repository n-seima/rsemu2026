#include	"lnk_util.h"

#ifndef	dIS_REDSTONE_ONE

const	u4	c_u4LargeRandomValue			=	0x8000;
const	u4	c_u4LargeRandomLimitValue		=	0x40000000;
const	u4	c_u4LargeRandomIncorrectRange	=	0xffffffff;

//	·´¼
inline u4	
largeRandom(u4 _u4range)
{
	if	(_u4range	>=	c_u4LargeRandomLimitValue)
		return	c_u4LargeRandomIncorrectRange;

	u4	u4Value;

	if	(_u4range	<=	c_u4LargeRandomValue)	
		return	random(_u4range);

	u4Value		=	_u4range/c_u4LargeRandomValue+1;

	if	(_u4range%c_u4LargeRandomValue)
	{
		u4		u4MaximumValue;
		u8		u8Value;

		u4MaximumValue	=	u4Value*c_u4LargeRandomValue-1;
		u4Value			=	random(u4Value)*c_u4LargeRandomValue;
		u4Value			+=	random(c_u4LargeRandomValue);

		u8Value			=	u4Value;
		u8Value			*=	_u4range;
		u8Value			/=	u4MaximumValue;
		u4Value			=	(u4)u8Value;
	}
	else
	{
		u4Value--;

		u4Value			=	random(u4Value)*c_u4LargeRandomValue;
		u4Value			+=	random(c_u4LargeRandomValue);
	}

	return	u4Value;
}

inline u4	
random(u4 _u4range)
{
	if	(_u4range	==	0) 
		return 0;

	if	(_u4range	>	c_u4LargeRandomValue)
		return	largeRandom(_u4range);

	return (rand()%_u4range);
}
#endif