#include "cGAME_CLOSE.H"
#include "cJOB.H"
#include "CImageData.H"
#include "CActor.H"


BOOL
(*cGAME_CLOSE::fpUpdate[dSTEP_COUNT])()	=
{
	cGAME_CLOSE::UpdateInit,
	cGAME_CLOSE::UpdateReady,
	cGAME_CLOSE::UpdateMain,
	cGAME_CLOSE::UpdateFinish,
	cGAME_CLOSE::UpdateClose
};

void
(*cGAME_CLOSE::fpDetailDraw[dSTEP_COUNT])()	=
{
	cGAME_CLOSE::DrawInit,
	cGAME_CLOSE::DrawReady,
	cGAME_CLOSE::DrawMain,
	cGAME_CLOSE::DrawFinish,
	cGAME_CLOSE::DrawClose
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_CLOSE::UpdateInit()
{
	BookingStep(dSTEP_READY);

	return	SAFETY;
}

BOOL
cGAME_CLOSE::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

BOOL
cGAME_CLOSE::UpdateMain()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_CLOSE::UpdateFinish()
{
	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_CLOSE::UpdateClose()
{
	Exit();

	return	SAFETY;
}


BOOL
cGAME_CLOSE::Run()
{
	return	fpUpdate[s_iStep]();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_CLOSE::DrawInit()
{
	cDRAW::Clear();

}

void
cGAME_CLOSE::DrawReady()
{
	cDRAW::Clear();

}

void
cGAME_CLOSE::DrawMain()
{
	cDRAW::Clear();

}

void
cGAME_CLOSE::DrawFinish()
{
	cDRAW::Clear();
}

void
cGAME_CLOSE::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_CLOSE::Draw()
{
	fpDetailDraw[s_iStep]();

	return	TRUE;
}