#include "cBARMENU.H"
#include "cDRAW.H"
#include "cTEXT.H"


cBAR::cBAR()
{	bCHECK	=	FALSE;
	bCLICK	=	FALSE;
	bFOCUS	=	FALSE;
	bENABLE	=	FALSE;
	bVIRGIN	=	TRUE;

	Normal	=	0xffff;
	Active	=	0xffff;
	Select	=	0xffff;
	Check	=	0xffff;

	Menu	=	0xffffffff;
}

cBAR::~cBAR()
{
}
//	이미지 바
void
cBAR::Init(cSPRITE *sprite,int x,int y,DWORD menu,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{	int image	=	normal;
	
	Normal	=	normal;
	Active	=	active;
	Select	=	select;
	Check	=	check;
	Menu	=	menu;
	Disable	=	disable;

	bCHECK	=	FALSE;
	bCLICK	=	FALSE;
	bFOCUS	=	FALSE;
	bENABLE	=	TRUE;
	bVIRGIN	=	TRUE;
	m_isDrawParents	=	FALSE;

	if (image==0xffff)	image	=	Active;
	if (image==0xffff)	image	=	Select;
	if (image==0xffff)	image	=	Check;

	if (image!=0xffff && sprite)	sprite->GetRect(image,&CrashBox);

	CrashBox.Move(x,y);
	Pos.Set(x,y);
}

//	텍스트 바
void
cBAR::Init(char *str,DWORD menu,int x,int y,int xs,int ys)
{	Menu	=	menu;

	bCHECK	=	FALSE;
	bCLICK	=	FALSE;
	bFOCUS	=	FALSE;
	bENABLE	=	TRUE;

	strcpy(string,str);

	CrashBox.Set(x,y,x+xs-1,y+ys-1);
	Pos.Set(x,y);
}

//	텍스트 바
void
cBAR::Reset(char *str,DWORD menu,int x,int y,int xs,int ys)
{	Menu	=	menu;

	bCHECK	=	FALSE;
	bCLICK	=	FALSE;
	bFOCUS	=	FALSE;
	bENABLE	=	TRUE;

	strcpy(string,str);

	CrashBox.Set(x,y,x+xs-1,y+ys-1);
	Pos.Set(x,y);
}


void
cBAR::SetSize(int xs,int ys)
{	CrashBox.x2	=	CrashBox.x1	+	xs;	
	CrashBox.y2	=	CrashBox.y1	+	ys;	
}

void
cBAR::SetSize(int x,int y,int xs,int ys)
{	CrashBox.x1	=	x;	
	CrashBox.y1	=	y;	
	CrashBox.x2	=	CrashBox.x1	+	xs;	
	CrashBox.y2	=	CrashBox.y1	+	ys;	
}

//	이미지 바
void
cBAR::Draw(cSPRITE *sprite,int alpha)
{	if (sprite)
	{	int	image	=	0xffff;

		if (bENABLE)
		{	image	=	Normal;

			if (bFOCUS	&&	Active!=0xffff)	image=Active;
			if (bCLICK	&&	Select!=0xffff)	image=Select;
		}
		else	if (Disable!=0xffff)	image=Disable;


		if (image!=0xffff				)
		{
			if (bCHECK)
			{
				if (m_isDrawParents)	sprite->Put(Pos.x,Pos.y,image,100,100,dPUT_ALPHA_BLENDING,alpha);
			}	else					sprite->Put(Pos.x,Pos.y,image,100,100,dPUT_ALPHA_BLENDING,alpha);
		}

		if (bCHECK	&&	Check !=0xffff	)	sprite->Put(Pos.x,Pos.y,Check,100,100,dPUT_ALPHA_BLENDING,alpha);
//		cDRAW::Box(0xffff,CrashBox.x1,CrashBox.y1,CrashBox.x2,CrashBox.y2);
	}
}

//	텍스트 바
void
cBAR::Draw(cBARCOLOR	*Color)
{
	DWORD	TextColor;
	WORD	BoxColor,FillColor;

	if (bENABLE)
	{	if (bCHECK)
		{	TextColor	=	Color->CheckText;
			BoxColor	=	Color->CheckBox;
			FillColor	=	Color->CheckColor;
		}
		else
		if (bFOCUS)
		{	TextColor	=	Color->FocusText;
			BoxColor	=	Color->FocusBox;
			FillColor	=	Color->FocusColor;
		}
		else
		{	TextColor	=	Color->NormalText;
			BoxColor	=	Color->NormalBox;
			FillColor	=	Color->NormalColor;
		}
	}
	else
	{	TextColor	=	Color->DisableText;
		BoxColor	=	Color->DisableBox;
		FillColor	=	Color->DisableColor;
	}

	char	str[256];

	if (!bVIRGIN)	strcpy(str,_ms("%s *",string));
	else			strcpy(str,string);


	if (!bCHECK)	cDRAW::FillHB(0				,CrashBox.x1+1	,CrashBox.y1+1,CrashBox.x2+1,CrashBox.y2+1);

	if (bCHECK)	
	{
		cDRAW::FillHB(FillColor		,CrashBox.x1+1	,CrashBox.y1+1,CrashBox.x2+2,CrashBox.y2+1);
		cDRAW::Box	 (BoxColor		,CrashBox.x1+1	,CrashBox.y1+1,CrashBox.x2+2,CrashBox.y2+1);
		cTEXT::Put   (CrashBox.x1+5	,CrashBox.y1+5	,TextColor	,str);
	}
	else
	{
		cDRAW::FillHB(FillColor		,CrashBox.x1  	,CrashBox.y1  ,CrashBox.x2  ,CrashBox.y2  );
		cDRAW::Box	 (BoxColor		,CrashBox.x1  	,CrashBox.y1  ,CrashBox.x2  ,CrashBox.y2  );
		cTEXT::Put   (CrashBox.x1+4	,CrashBox.y1+4	,TextColor	,str);
	}
}

BOOL
cBAR::isSELECT(int x,int y)
{	if (!bENABLE) return FALSE;

	return CrashBox.isIN(x,y);
}

void
cBAR::Update(BOOL focus,BOOL click)
{	bCLICK	=	click;
	bFOCUS	=	focus;
}

void
cBAR::Move(int x,int y)
{	if (x==0xffff) x = CrashBox.x1;
	if (y==0xffff) y = CrashBox.y1;

	CrashBox.x2	=	(CrashBox.x2	-	CrashBox.x1)	+	x;
	CrashBox.y2	=	(CrashBox.y2	-	CrashBox.y1)	+	y;
	CrashBox.x1	=	x;
	CrashBox.y1	=	y;
}

void
cBAR::Move(int x,int y,int xs,int ys)
{	if (x==0xffff) x = CrashBox.x1;
	if (y==0xffff) y = CrashBox.y1;

	CrashBox.x2	=	x+xs;;
	CrashBox.y2	=	y+ys;
	CrashBox.x1	=	x;
	CrashBox.y1	=	y;
}

void
cBAR::Resize(int xs,int ys)
{	CrashBox.x2	=	CrashBox.x1+xs;
	CrashBox.y2	=	CrashBox.y1+ys;
}


/****************************************************************
	BarMenu class
****************************************************************/

cBARMENU::cBARMENU()
{	Count	=	0;
	FOCUS	=	0xffff;
	MaxBar	=	0;
	pIMAGE	=	NULL;
	pMENU	=	NULL;

}

cBARMENU::~cBARMENU()
{	pKILL(pMENU);
}

void
cBARMENU::Close()
{	pKILL(pMENU);
	Reset();
}

void
cBARMENU::Init(cSPRITE *image,int maxbar,WORD checkmode,BOOL drawParents)
{	pIMAGE		=	image;
	MaxBar		=	maxbar;
	CheckMode	=	checkmode;
	Count		=	0;
	m_isDrawParents	=	drawParents;

	pKILL(pMENU);
	pMENU	=	new cBAR [maxbar];
}

void
cBARMENU::Init(int maxbar,WORD checkmode,BOOL drawParents)
{	MaxBar		=	maxbar;
	CheckMode	=	checkmode;
	Count		=	0;
	m_isDrawParents	=	drawParents;

	pKILL(pMENU);
	pMENU	=	new cBAR [maxbar];

//	         (normalText      ,focusText       ,checkText       ,disableText     ,noramlColor,focusColor,checkColor,disableColor)
	InitColor(RGB(200,200,200),RGB(200,200,200),RGB(255,255,0  ),RGB(128,128,128),0          ,0			,_LTBLUE	,_DEEPGRAY);
}

void
cBARMENU::InitColor(DWORD normalText,DWORD focusText,DWORD checkText,DWORD disableText,WORD noramlColor,WORD focusColor,WORD checkColor,WORD disableColor)
{	NormalText		=	normalText;		
	FocusText		=	focusText;
	CheckText		=	checkText;
	DisableText		=	disableText;
	NormalColor		=	noramlColor;
	FocusColor		=	focusColor;
	CheckColor		=	checkColor;
	DisableColor	=	disableColor;

	NormalBox		=	RGB24To16(normalText);
	FocusBox		=	RGB24To16(focusText);
	CheckBox		=	RGB24To16(checkText);
	DisableBox		=	RGB24To16(disableText);
}

void
cBARMENU::Reset()
{	Count	=	0;
	FOCUS	=	0xffff;
}

BOOL
cBARMENU::Add(int x,int y,DWORD menu,WORD normal,WORD active,WORD select,WORD check,WORD disable)
{	if (Count >= MaxBar) return FALSE;

	pMENU[Count].Init(pIMAGE,x,y,menu,normal,active,select,check,disable);
	pMENU[Count].m_isDrawParents	=	m_isDrawParents;

	Count++;

	return TRUE;
}

BOOL
cBARMENU::Add(char *str,DWORD menu,int x,int y,int xs,int ys)
{	if (Count >= MaxBar) return FALSE;

	pMENU[Count].Init(str,menu,x,y,xs,ys);

	Count++;

	return TRUE;
}

BOOL
cBARMENU::Add(char *str,DWORD menu,int x,int y)
{	if (Count >= MaxBar) return FALSE;

	int	xs	=	strlen(str)*6+10+12;

	pMENU[Count].Init(str,menu,x,y,xs,20);

	Count++;

	return TRUE;
}

void
cBARMENU::SetSize(DWORD menu,int xs,int ys)
{	int index	=	GetIndex(menu);

	if (index == 0xffff)	return;

	pMENU[index].SetSize(xs,ys);
}

void
cBARMENU::SetSize(int xs,int ys)
{	pMENU[Count-1].SetSize(xs,ys);
}

void
cBARMENU::SetSize(int x,int y,int xs,int ys)
{	pMENU[Count-1].SetSize(x,y,xs,ys);
}

void
cBARMENU::SetSize(DWORD menu,int x,int y,int xs,int ys)
{
	int index	=	GetIndex(menu);

	if (index == 0xffff)	return;

	pMENU[index].SetSize(x,y,xs,ys);
}

void
cBARMENU::Check(DWORD menu,WORD check)
{	int i,index	=	0xffff;

	for (i=0;i<Count;i++)
		if (pMENU[i].Menu	==	menu)
		{	index	=	i;
			break;
		}

	if (index == 0xffff) return;

	pMENU[index].bCLICK	=	(BYTE)check;
	pMENU[index].bFOCUS	=	(BYTE)check;

	switch(CheckMode)
	{	case	dBAR_RADIO	:
			for (i=0;i<Count;i++)	pMENU[i].bCHECK	=	FALSE;
			pMENU[index].bCHECK	=	TRUE;
			break;

		case	dBAR_CHECK	:
			if (check==0xffff)	pMENU[index].bCHECK	=	1	-	pMENU[index].bCHECK;
			else				pMENU[index].bCHECK	=	(BOOL)check;
			break;
	}

}

DWORD
cBARMENU::Update(int x,int y,BOOL click,BOOL checkNow)
{	int		i;
	DWORD	result	=	0xffffffff;

	for (i=0;i<Count;i++)
	{	if (pMENU[i].isSELECT(x,y))
		{	FOCUS	=	i;
			pMENU[i].Update(TRUE,click);

			if (click)
			{	result	=	pMENU[i].Menu;
				if (checkNow)	Check(result);
			}
		}
		else	pMENU[i].Update(FALSE,FALSE);
	}

	return result;
}

DWORD
cBARMENU::GetMenu(int x,int y)
{	int		i;

	for (i=0;i<Count;i++)
		if (pMENU[i].isSELECT(x,y))
			return pMENU[i].Menu;

	return 0xffffffff;
}

int
cBARMENU::GetIndex(DWORD menu)
{	int i;

	for (i=0;i<Count;i++)
		if (pMENU[i].Menu	==	menu)	return i;

	return 0xffff;
}

void
cBARMENU::Draw(int alpha)
{
	if (pIMAGE)	for (int i=0;i<Count;i++)	pMENU[i].Draw(pIMAGE,alpha);
	else		for (int i=0;i<Count;i++)	pMENU[i].Draw(this);
}

BOOL
cBARMENU::IsCheck(DWORD menu)
{	int i;

	for (i=0;i<Count;i++)
		if (pMENU[i].Menu==menu)
			if (pMENU[i].bCHECK)	return TRUE;

	return FALSE;
}

BOOL
cBARMENU::IsActive(DWORD menu)
{	int i;

	for (i=0;i<Count;i++)
		if (pMENU[i].Menu==menu)
			if (pMENU[i].bFOCUS)	return TRUE;

	return FALSE;
}

BOOL
cBARMENU::Move(DWORD menu,int x,int y)
{	int index = GetIndex(menu);

	if (index == 0xffff) return FALSE;

	pMENU[index].Move(x,y);

	return TRUE;
}

BOOL
cBARMENU::Move(DWORD menu,int x,int y,int xs,int ys)
{	int index = GetIndex(menu);

	if (index == 0xffff) return FALSE;

	pMENU[index].Move(x,y,xs,ys);

	return TRUE;	
}

BOOL
cBARMENU::Resize(DWORD menu,int xs,int ys)
{	int index = GetIndex(menu);

	if (index == 0xffff) return FALSE;

	pMENU[index].Resize(xs,ys);

	return TRUE;
}

void
cBARMENU::Enable(DWORD menu)
{	int index = GetIndex(menu);
	if (index==0xffff) return;

	pMENU[index].bENABLE	=	TRUE;
}

void
cBARMENU::Disable(DWORD menu)
{	int index = GetIndex(menu);
	if (index==0xffff) return;

	pMENU[index].bENABLE	=	FALSE;
}

BOOL
cBARMENU::IsEnable(DWORD menu)
{	int index = GetIndex(menu);
	if (index==0xffff) return FALSE;

	return pMENU[index].bENABLE;
}

int
cBARMENU::GetRightPos()
{
	int		i,x2=0;

	for (i=0;i<Count;i++)
		if (pMENU[i].CrashBox.x2 > x2)	x2	=	pMENU[i].CrashBox.x2;

	return x2;
}

void
cBARMENU::SetVirgin(DWORD menu,BOOL virgin)
{
	int index = GetIndex(menu);
	if (index==0xffff) return;

	pMENU[index].SetVirgin(virgin);
}

BOOL
cBARMENU::Set(DWORD menu,char *str)
{	int index = GetIndex(menu);
	if (index==0xffff) return FALSE;

	int	i;

	pMENU[index].SetStr(str);

	int	xs	=	strlen(str)*6+10+12;
	int	x2	=	pMENU[index].CrashBox.x2;

	pMENU[index].Reset(str,menu,pMENU[index].CrashBox.x1,pMENU[index].CrashBox.y1,xs,20);

	xs	=	pMENU[index].CrashBox.x2 - x2;

	for (i=index+1;i<Count;i++)
	{
		pMENU[i].CrashBox.x2 +=	xs;
		pMENU[i].CrashBox.x1 +=	xs;
		pMENU[i].Pos.x		=	pMENU[i].CrashBox.x1;
	}

	return TRUE;
}

BOOL
cBARMENU::Remove(DWORD menu)
{	int index = GetIndex(menu);
	if (index==0xffff) return FALSE;

	int	i;

	if (index == Count -1 )
	{
		pMENU[index].Menu	=	0xffffffff;
		Count--;
		return TRUE;
	}

	int	xs	=	pMENU[index+1].CrashBox.x1 - pMENU[index].CrashBox.x1;

	for (i=index+1;i<Count;i++)
	{
		memcpy(&pMENU[i-1],&pMENU[i],sizeof(cBAR));

		pMENU[i-1].CrashBox.x1	=	pMENU[i].CrashBox.x1 -xs;
		pMENU[i-1].CrashBox.x2	=	pMENU[i].CrashBox.x2 -xs;
		pMENU[i-1].CrashBox.y1	=	pMENU[i].CrashBox.y1 ;
		pMENU[i-1].CrashBox.y2	=	pMENU[i].CrashBox.y2 ;

		pMENU[i-1].Pos.x		=	pMENU[i-1].CrashBox.x1;
		pMENU[i-1].Pos.y		=	pMENU[i-1].CrashBox.y1;
	}

	pMENU[Count-1].Menu	=	0xffffffff;

	Count--;

	return TRUE;
}