#include "cGAME_OPTION.H"
#include "cJOB.H"
#include "CActor.H"


BOOL
(*cGAME_OPTION::fpUpdate[dSTEP_COUNT])()	=
{
	cGAME_OPTION::UpdateInit,
	cGAME_OPTION::UpdateReady,
	cGAME_OPTION::UpdateMain,
	cGAME_OPTION::UpdateFinish,
	cGAME_OPTION::UpdateClose
};

void
(*cGAME_OPTION::fpDetailDraw[dSTEP_COUNT])()	=
{
	cGAME_OPTION::DrawInit,
	cGAME_OPTION::DrawReady,
	cGAME_OPTION::DrawMain,
	cGAME_OPTION::DrawFinish,
	cGAME_OPTION::DrawClose
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_OPTION::UpdateInit()
{
	BookingStep(dSTEP_READY);

	return	SAFETY;
}

BOOL
cGAME_OPTION::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

BOOL
cGAME_OPTION::UpdateMain()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_OPTION::UpdateFinish()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_OPTION::UpdateClose()
{
	BookingMode(eGAME_TITLE);

	return	SAFETY;
}


BOOL
cGAME_OPTION::Run()
{
	return	fpUpdate[s_iStep]();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_OPTION::DrawInit()
{
	cDRAW::Clear();

}

void
cGAME_OPTION::DrawReady()
{
	cDRAW::Clear();

}

void
cGAME_OPTION::DrawMain()
{
	cDRAW::Clear();

}

void
cGAME_OPTION::DrawFinish()
{
	cDRAW::Clear();
}

void
cGAME_OPTION::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_OPTION::Draw()
{
	fpDetailDraw[s_iStep]();

	return	TRUE;
}