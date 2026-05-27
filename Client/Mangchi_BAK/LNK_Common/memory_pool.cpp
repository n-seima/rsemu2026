#include	"mem.h"
#include	<string.h>

//
//	생성자
cMemoryPool::cMemoryPool()
{
	m_u4FullSize		=	0;
	m_u4FrontPointer	=	0;
	m_pPool				=	NULL;
	m_u4TempAlloc		=	0xffffffff;
	m_lpReference		=	NULL;

	m_s4StackMethod		=	eStackFront;
}

//
//	디스트럭터
cMemoryPool::~cMemoryPool()
{
	close();
}

//
//	메모리 설정
bool
cMemoryPool::init(u4 _u4Size,char *_lpstrName)
{
	if	(_lpstrName	==	NULL)
		_lpstrName	=	"noname";

	strncpy(m_strName,_lpstrName,eNameLength-1);

	if	((_u4Size%eMemoryAllign)	!=	0)	//	메모리 정렬
		_u4Size		+=	(eMemoryAllign-(_u4Size%eMemoryAllign));

	m_u4FullSize	=	_u4Size;

	reset();

	mFree(m_pPool);

	m_pPool		=	mNew(char,m_u4FullSize,NULL,_lpstrName);

	if	(!m_pPool)
	{
		g_ecm.addError("cMemoryPool::init - Memory allocation Error [%s]",_lpstrName);

		return false;
	}

	if	(((u4)m_pPool%eMemoryAllign)	!= 0)	//	메모리 정렬
		m_u4FrontPointer	+=	eMemoryAllign	-	((u4)m_pPool%eMemoryAllign);

	m_u4RearPointer	=	m_u4FullSize;

	return	true;
}

//
//	레퍼런스 메모리 설정
bool
cMemoryPool::init(cMemoryPool *_lpMcu,int _s4StackMethod,char *_lpstrName)
{
	if	(_lpstrName	==	NULL)
		_lpstrName	=	"noname";

	if	(_lpMcu->m_lpReference)
	{
		g_ecm.addError("cMemoryPool::init - _lpMcu->m_lpReference == NULL [%s]",_lpstrName);

		return	false;
	}

	m_lpReference	=	_lpMcu;
	m_s4StackMethod	=	_s4StackMethod;

	strncpy(m_strName,_lpstrName,eNameLength-1);

	return	true;
}

//
//	작살-o- 낸다.
void
cMemoryPool::close()
{
	reset();

	if	(!m_lpReference)
		mFree(m_pPool);

	m_lpReference		=	NULL;
}

//
//	초기화 하고, 상처 입은 메모리가 있는지 확인한다.
void
cMemoryPool::reset()
{
	if	(m_lpReference)
	{
		if	(m_s4StackMethod	==	eStackRear)
			m_lpReference->m_u4RearPointer	=	m_lpReference->m_u4FullSize;
		else
			m_lpReference->m_u4FrontPointer	=	0;
	}
	else
	{
		m_u4FrontPointer	=	0;
		m_u4RearPointer		=	m_u4FullSize;
		m_u4TempAlloc		=	0xffffffff;
	}
}

//
//	남은 메모리
u4
cMemoryPool::getRemainMemorySize()
{
	if	(m_lpReference)
		return	m_lpReference->getRemainMemorySize();

	return	m_u4RearPointer - m_u4FrontPointer;
}

//
//	사용된 메모리
u4
cMemoryPool::getUsedMemorySize()
{
	if	(m_lpReference)
		return	m_lpReference->getUsedMemorySize();

	return	m_u4FrontPointer+(m_u4FullSize-m_u4RearPointer);
}