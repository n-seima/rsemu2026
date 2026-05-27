#include "CActor.H"

//
//	잔상 추가
void
CActor::addAfterImage(int _iSlot,CPos *_lpPos,int _iAnm,int _iDirect,int _iFrame,int _iAlphaDepth)
{
	m_aAfterImageList[_iSlot].m_pos.x		=	_lpPos->x;
	m_aAfterImageList[_iSlot].m_pos.y		=	_lpPos->y;
	m_aAfterImageList[_iSlot].m_bAnm		=	_iAnm;
	m_aAfterImageList[_iSlot].m_bDirect		=	_iDirect;
	m_aAfterImageList[_iSlot].m_bFrame		=	_iFrame;
	m_aAfterImageList[_iSlot].m_bAlphaDepth	=	_iAlphaDepth;
}	//	CActor::addAfterImage(int _iSlot,CPos *_lpPos,int _iAnm,int _iDirect,int _iFrame,int _iAlphaDepth)

//
//	캐릭터 잔상의 좌표와 기타 등등을 업데이트 한다.
void
CActor::updateAfterImage()
{
	int		i;

	if	(m_wAwesomeFortressAction	!=	0xffff)
	{
		for (i=0;i<m_wParallelBunshineCount;i++)
		{
			m_aAfterImageList[i].m_bAnm			=	(BYTE)m_awesomeFortressActionInfo.m_wAnm;
			m_aAfterImageList[i].m_bFrame		=	(BYTE)m_awesomeFortressActionInfo.m_wFrame;
			m_aAfterImageList[i].m_bAlphaDepth	=	(BYTE)m_awesomeFortressActionInfo.m_wAlphaDepth;
		}

		return;
	}
	if	(m_wContinuousHitOfDoppelgangerAction[0]!=	0xffff)
	{
		for (i=0;i<m_wDoppelgangerCount;i++)
		{
			
			
			m_aAfterImageList[i].m_bAnm			=	(BYTE)m_continuousHitOfDoppelgangerActionInfo[i].m_wAnm;
			m_aAfterImageList[i].m_bFrame		=	(BYTE)m_continuousHitOfDoppelgangerActionInfo[i].m_wFrame;
			m_aAfterImageList[i].m_bAlphaDepth	=	(BYTE)m_continuousHitOfDoppelgangerActionInfo[i].m_wAlphaDepth;
			
		}
		return;
	}
	if	(m_wParallelBunshineCount)
	{
		int		iRange	=	0;
		CPos	pos;

		iRange	=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
		iRange	=	(int)sqrt((double)iRange);

		CSkill	*lpSkill	=	m_abilityUse.getSkill();

		if	(!lpSkill)
		{
			resetAfterImage();
			return;
		}

		if (lpSkill->m_bf1IsRoundRappedBunshinAttack)
			for (i=0;i<m_wParallelBunshineCount;i++)
			{
				int		iAngle	=	(m_iAngle+180)%360+(i+1)*m_wParallelBunshineAngleGap;

				iAngle	%=	360;

				pos.Set(m_posTarget.x,m_posTarget.y);

				GetOvalAnglePos(&pos,iAngle,iRange);

				m_aAfterImageList[i].m_pos.x		=	pos.x;
				m_aAfterImageList[i].m_pos.y		=	pos.y;
				m_aAfterImageList[i].m_bAnm			=	m_iAnm;
				m_aAfterImageList[i].m_bDirect		=	GetDirect((iAngle+180)%360,m_iDirectCount);;
				m_aAfterImageList[i].m_bFrame		=	m_iFrame;
				m_aAfterImageList[i].m_bAlphaDepth	=	lpSkill->m_bf5BunshineAlphaDepth;
			}
		else
			for (i=0;i<m_wParallelBunshineCount-1;i++)
			{
				int		iAngle	=	m_iAngle+180;

				if (i%2 == 0)	iAngle	=	iAngle-m_wParallelBunshineAngleGap*(i/2+1);
				else			iAngle	=	iAngle+m_wParallelBunshineAngleGap*(i/2+1);

				iAngle	%=	360;

				pos.Set(m_posTarget.x,m_posTarget.y);

				GetOvalAnglePos(&pos,iAngle,iRange);

				m_aAfterImageList[i].m_pos.x		=	pos.x;
				m_aAfterImageList[i].m_pos.y		=	pos.y;
				m_aAfterImageList[i].m_bAnm			=	m_iAnm;
				m_aAfterImageList[i].m_bDirect		=	m_iDirect;
				m_aAfterImageList[i].m_bFrame		=	m_iFrame;
				m_aAfterImageList[i].m_bAlphaDepth	=	lpSkill->m_bf5BunshineAlphaDepth;
			}
	}

	for (i=0;i<dAFTER_IMAGE_COUNT;i++)
	{
		if (m_aAfterImageList[i].m_bAlphaDepth	>	0)
			m_aAfterImageList[i].m_bAlphaDepth--;
	}

	if	(m_cAfterImageStatus	==	dCLOSE_AFTER_IMAGE)
		return;

	if	(m_cAfterImageStatus	==	dFADE_AFTER_IMAGE)			//	잔상 효과 처리
	{
		m_cAfterImageStatus	=	dCLOSE_AFTER_IMAGE;	//	잔상 알파갑싱 전부 0이어야지 닫는다.
		
		for (i=0;i<m_cAfterImageCount;i++)
			if (m_aAfterImageList[i].m_bAlphaDepth	>	0)
			{
				m_aAfterImageList[i].m_bAlphaDepth--;

				if (m_aAfterImageList[i].m_bAlphaDepth	==	0)
					m_aAfterImageList[i].m_bAnm	=	0xff;
				else
					m_cAfterImageStatus			=	dFADE_AFTER_IMAGE;
			}
	}

	if	(m_cAfterImageStatus	==	dPUT_AFTER_IMAGE)		//	잔상효과
	{
		if (m_wAfterImageDelayCounter)
			m_wAfterImageDelayCounter--;

		if (m_wAfterImageDelayCounter	==	0)
		{
			m_wAfterImageDelayCounter	=	m_wAfterImageDelayPeriod;

			for (i=m_cAfterImageCount-1;i>=1;i--)
			{
				m_aAfterImageList[i].m_pos.x		=	m_aAfterImageList[i-1].m_pos.x;
				m_aAfterImageList[i].m_pos.y		=	m_aAfterImageList[i-1].m_pos.y;
				m_aAfterImageList[i].m_bAnm			=	m_aAfterImageList[i-1].m_bAnm;
				m_aAfterImageList[i].m_bDirect		=	m_aAfterImageList[i-1].m_bDirect;
				m_aAfterImageList[i].m_bFrame		=	m_aAfterImageList[i-1].m_bFrame;

				if (m_aAfterImageList[i-1].m_bAlphaDepth	>=	m_cAfterImageDisappearSpeed)
						m_aAfterImageList[i].m_bAlphaDepth	=	m_aAfterImageList[i-1].m_bAlphaDepth-m_cAfterImageDisappearSpeed;
				else	m_aAfterImageList[i].m_bAlphaDepth	=	0;
			}

			m_aAfterImageList[0].m_pos.x		=	m_pos.x;
			m_aAfterImageList[0].m_pos.y		=	m_pos.y - getHeight();
			m_aAfterImageList[0].m_bAlphaDepth	=	m_cAfterImageDepth;
			m_aAfterImageList[0].m_bAnm			=	m_iAnm;
			m_aAfterImageList[0].m_bDirect		=	m_iDirect;
			m_aAfterImageList[0].m_bFrame		=	m_iFrame;
		}
	}
}