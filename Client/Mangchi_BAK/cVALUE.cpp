#include "cVALUE.H"
#include "cMESSAGE.H"

cVALUE		g_ProjectValue;
cVALUE		*g_lpValue;

cVALUE::cVALUE()
{
	Reset();
}

//
//	카르마 값들을 초기화
//
void
cVALUE::Reset()
{
	for (int i=0;i<dVALUE_COUNT;i++)
	{
		m_aValue[i].m_iValue	=	dVALID_VALUE;

		strcpy(m_aValue[i].m_strName,"Brain Shaker!!");
	}

	m_iCount	=	0;
}	//	cVALUE::Reset()

//
//	strName에 iDefaultValue를 가지는 카르마 값 추가
//
int
cVALUE::Add(char *strName,int iDefaultValue)
{
	int	i,iCount=0;

	if	(m_iCount	>=	dVALUE_COUNT)
	{
		ERRMSG(dMSG_ERROR_ADD_VALUE,dMSG_TOO_MANY_VALUE);

		return	0xffff;
	}

	for (i=0;i<dVALUE_COUNT && iCount < m_iCount;i++)
	{
		if (m_aValue[i].m_iValue	!=	dVALID_VALUE)
		{
			if (stricmp(m_aValue[i].m_strName,strName)==0)
			{
				ERRMSG(dMSG_ERROR_ADD_VALUE,dMSG_EXIT_SAME_VALUE);

				return	0xffff;
			}

			iCount++;
		}
	}

	for (i=0;i<dVALUE_COUNT;i++)
		if (m_aValue[i].m_iValue	==	dVALID_VALUE)
		{
			m_aValue[i].m_iValue	=	iDefaultValue;
			strcpy(m_aValue[i].m_strName,strName);

			m_iCount++;

			return	i;
		}

	return	0xffff;
}	//	cVALUE::Add(char *strName,int iDefaultValue)

//
//	index에 카르마값 제거
//
BOOL
cVALUE::Remove(int index)
{
	if (m_aValue[index].m_iValue	==	dVALID_VALUE)	return	FALSE;

	m_iCount--;
	m_aValue[index].m_iValue		=	dVALID_VALUE;
	strcpy(m_aValue[index].m_strName,"It's Removed Value!!");

	return	TRUE;
}	//	cVALUE::Remove(int index)

//
//	strName을 가진 카르마값 삭제
//
BOOL
cVALUE::Remove(char *strName)
{
	int	i,iCount	=	0;

	for (i=0;i<dVALUE_COUNT && iCount < m_iCount;i++)
	{
		if (m_aValue[i].m_iValue	!=	dVALID_VALUE)
		{
			if (stricmp(m_aValue[i].m_strName,strName)==0)
			{
				m_aValue[i].m_iValue		=	dVALID_VALUE;
				strcpy(m_aValue[i].m_strName,"Brain Shaker!!");

				m_iCount--;

				return	TRUE;
			}

			iCount++;
		}
	}

	return	FALSE;

}	//	cVALUE::Remove(char *strName)

//
//	값이 등록 되어 있는 마지막 인덱스를 구한다.
//
int
cVALUE::GetLastIndex()
{
	for (int i=dVALUE_COUNT-1 ;i>=0;i--)
		if (m_aValue[i].m_iValue	!=	dVALID_VALUE)
			return	i;

	return	-1;
}	//	cVALUE::GetLastIndex()