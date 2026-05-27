#include "cGAME_OPENING.H"
#include "cJOB.H"
#include "CActor.H"


BOOL
(*cGAME_OPENING::fpUpdate[dSTEP_COUNT])()	=
{
	cGAME_OPENING::UpdateInit,
	cGAME_OPENING::UpdateReady,
	cGAME_OPENING::UpdateMain,
	cGAME_OPENING::UpdateFinish,
	cGAME_OPENING::UpdateClose
};

void
(*cGAME_OPENING::fpDetailDraw[dSTEP_COUNT])()	=
{
	cGAME_OPENING::DrawInit,
	cGAME_OPENING::DrawReady,
	cGAME_OPENING::DrawMain,
	cGAME_OPENING::DrawFinish,
	cGAME_OPENING::DrawClose
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_OPENING::UpdateInit()
{
	BookingStep(dSTEP_READY);

	return	SAFETY;
}

BOOL
cGAME_OPENING::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

BOOL
cGAME_OPENING::UpdateMain()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_OPENING::UpdateFinish()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_OPENING::UpdateClose()
{
	BookingMode(eGAME_LOGO);

	return	SAFETY;
}


BOOL
cGAME_OPENING::Run()
{
	return	fpUpdate[s_iStep]();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_OPENING::DrawInit()
{
	cDRAW::Clear();

}

void
cGAME_OPENING::DrawReady()
{
	cDRAW::Clear();

}

void
cGAME_OPENING::DrawMain()
{
	cDRAW::Clear();

}

void
cGAME_OPENING::DrawFinish()
{
	cDRAW::Clear();
}

void
cGAME_OPENING::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_OPENING::Draw()
{
	fpDetailDraw[s_iStep]();

	return	TRUE;
}