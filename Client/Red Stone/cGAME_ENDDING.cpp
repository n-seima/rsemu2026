#include "cGAME_ENDDING.H"
#include "cJOB.H"
#include "CImageData.H"
#include "CActor.H"


BOOL
(*cGAME_ENDDING::fpUpdate[dSTEP_COUNT])()	=
{
	cGAME_ENDDING::UpdateInit,
	cGAME_ENDDING::UpdateReady,
	cGAME_ENDDING::UpdateMain,
	cGAME_ENDDING::UpdateFinish,
	cGAME_ENDDING::UpdateClose
};

void
(*cGAME_ENDDING::fpDetailDraw[dSTEP_COUNT])()	=
{
	cGAME_ENDDING::DrawInit,
	cGAME_ENDDING::DrawReady,
	cGAME_ENDDING::DrawMain,
	cGAME_ENDDING::DrawFinish,
	cGAME_ENDDING::DrawClose
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_ENDDING::UpdateInit()
{
	BookingStep(dSTEP_READY);

	return	SAFETY;
}

BOOL
cGAME_ENDDING::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

BOOL
cGAME_ENDDING::UpdateMain()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_ENDDING::UpdateFinish()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_ENDDING::UpdateClose()
{
	BookingMode(eGAME_TITLE);

	return	SAFETY;
}


BOOL
cGAME_ENDDING::Run()
{
	return	fpUpdate[s_iStep]();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_ENDDING::DrawInit()
{
	cDRAW::Clear();

}

void
cGAME_ENDDING::DrawReady()
{
	cDRAW::Clear();

}

void
cGAME_ENDDING::DrawMain()
{
	cDRAW::Clear();

}

void
cGAME_ENDDING::DrawFinish()
{
	cDRAW::Clear();
}

void
cGAME_ENDDING::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_ENDDING::Draw()
{
	fpDetailDraw[s_iStep]();

	return	TRUE;
}