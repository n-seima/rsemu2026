#include "cGAME_LOGO.H"
#include "cJOB.H"
#include "CActor.H"


BOOL
(*cGAME_LOGO::fpUpdate[dSTEP_COUNT])()	=
{
	cGAME_LOGO::UpdateInit,
	cGAME_LOGO::UpdateReady,
	cGAME_LOGO::UpdateMain,
	cGAME_LOGO::UpdateFinish,
	cGAME_LOGO::UpdateClose
};

void
(*cGAME_LOGO::fpDetailDraw[dSTEP_COUNT])()	=
{
	cGAME_LOGO::DrawInit,
	cGAME_LOGO::DrawReady,
	cGAME_LOGO::DrawMain,
	cGAME_LOGO::DrawFinish,
	cGAME_LOGO::DrawClose
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_LOGO::UpdateInit()
{
	BookingStep(dSTEP_READY);

	return	SAFETY;
}

BOOL
cGAME_LOGO::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

BOOL
cGAME_LOGO::UpdateMain()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_LOGO::UpdateFinish()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_LOGO::UpdateClose()
{
	BookingMode(eGAME_TITLE);

	return	SAFETY;
}


BOOL
cGAME_LOGO::Run()
{
	return	fpUpdate[s_iStep]();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_LOGO::DrawInit()
{
	cDRAW::Clear();

}

void
cGAME_LOGO::DrawReady()
{
	cDRAW::Clear();

}

void
cGAME_LOGO::DrawMain()
{
	cDRAW::Clear();

}

void
cGAME_LOGO::DrawFinish()
{
	cDRAW::Clear();
}

void
cGAME_LOGO::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_LOGO::Draw()
{
	fpDetailDraw[s_iStep]();

	return	TRUE;
}