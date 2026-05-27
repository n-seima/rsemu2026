#ifndef CSOLIDCHECK_H
#define CSOLIDCHECK_H

typedef struct LineNode {
	int			iD;
	float		fNX, fNY;
	LineNode	*pFront, *pBack;
}*LPLineNode;

class CSolidCheck{
public:
	CSolidCheck();
	~CSolidCheck();

	BOOL IsCheckIn(POINT pos);

	BOOL SetSolid(POINT *pos, int iCount);

	LPLineNode	m_pLines;

	WORD		m_wNumLine;
};

#endif