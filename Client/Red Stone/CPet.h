#ifndef	_classPet_H
#define	_classPet_H

#include "CPetDefine.h"
#include "SFC.h"
#include "cJOB.h"

class	cPet	:	public cPetDefine
{
public:
	WORD		m_wBodySerial;
	WORD		m_wCommand;
	WORD		m_wMinAttackPower,m_wMaxAttackPower;
	WORD		m_wProtectActor;
	CPos		m_posProtect;
	WORD		m_wEngageTarget,m_wNameBarSizeAtField;
	int			m_iDefensePower;

	CJobInfo*	getJob()
	{
		if	(m_bf10Level	==	0)
			return	NULL;

		return	&g_jm.m_aJob[getJobIndex()+100];
	};

	char*		getName()
	{
		static	char	strName[dPET_NAME_LENGTH+1];

		memcpy(strName,m_strName,dPET_NAME_LENGTH);

		strName[dPET_NAME_LENGTH]	=	NULL;

		return	strName;
	}

	void		reset()
	{
		cPetDefine::reset();

		m_wNameBarSizeAtField	=	0;
	}

	BOOL		isNoName()
	{
		if	(strcmp(getName(),"noname")==0)
			return	TRUE;

		return	FALSE;
	}
};

#endif