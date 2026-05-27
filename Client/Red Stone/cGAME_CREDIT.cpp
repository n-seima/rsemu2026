#include "cGAME_CREDIT.H"
#include "cJOB.H"
#include "CImageData.H"
#include "CActor.H"


BOOL
(*cGAME_CREDIT::fpUpdate[dSTEP_COUNT])()	=
{
	cGAME_CREDIT::UpdateInit,
	cGAME_CREDIT::UpdateReady,
	cGAME_CREDIT::UpdateMain,
	cGAME_CREDIT::UpdateFinish,
	cGAME_CREDIT::UpdateClose
};

void
(*cGAME_CREDIT::fpDetailDraw[dSTEP_COUNT])()	=
{
	cGAME_CREDIT::DrawInit,
	cGAME_CREDIT::DrawReady,
	cGAME_CREDIT::DrawMain,
	cGAME_CREDIT::DrawFinish,
	cGAME_CREDIT::DrawClose
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_CREDIT::UpdateInit()
{
	BookingStep(dSTEP_READY);

	return	SAFETY;
}

BOOL
cGAME_CREDIT::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

BOOL
cGAME_CREDIT::UpdateMain()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_CREDIT::UpdateFinish()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_CREDIT::UpdateClose()
{
	BookingMode(eGAME_TITLE);

	return	SAFETY;
}


BOOL
cGAME_CREDIT::Run()
{
	return	fpUpdate[s_iStep]();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_CREDIT::DrawInit()
{
	cDRAW::Clear();

}

void
cGAME_CREDIT::DrawReady()
{
	cDRAW::Clear();

}

void
cGAME_CREDIT::DrawMain()
{
	cDRAW::Clear();

}

void
cGAME_CREDIT::DrawFinish()
{
	cDRAW::Clear();
}

void
cGAME_CREDIT::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_CREDIT::Draw()
{
	fpDetailDraw[s_iStep]();

	return	TRUE;
}