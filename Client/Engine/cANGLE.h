#ifndef __classANGLE_H__
#define __classANGLE_H__

//	재형이가 준것.. 대부분을 그대로 쓰고 함수 이름들을 내 취향으로 바꾸었다.. --;

#include "SFC.H"

#define MAX_ANGLE_DEGREE			( 1024 )

#define ANGLE_000					( 0 )				// 360에서   '0'도에 해당하는 1024단계값
#define ANGLE_090					( 256 )				// 360에서  '90'도에 해당하는 1024단계값
#define ANGLE_180					( 512 )				// 360에서 '180'도에 해당하는 1024단계값
#define ANGLE_270					( 768 )				// 360에서 '270'도에 해당하는 1024단계값
#define ANGLE_360					( 1024 )			// 360에서 '360'도에 해당하는 1024단계값

#define A360TO1024(a)				((a) * 1024 / 360 )
#define A1024TO360(a)				((a) * 360 / 1024 )

#define RADIX_SIZE					( 16 )

#define INTTOFIXED(i)				((i) << RADIX_SIZE )
#define FIXEDTOINT(f)				((f) >> RADIX_SIZE )

extern	const	int	c_aSinTbl[MAX_ANGLE_DEGREE];
extern	const	int	c_aCosTbl[MAX_ANGLE_DEGREE];
extern	const	int	c_aArcTanTbl[MAX_ANGLE_DEGREE * 2 + 1];

class cANGLE
{
public:
	static	int		GetCos(int nAngle);
	static	int		GetSin(int nAngle);
	static	int		GetAngleToTarget(int nX,int nY,int nTargetX,int nTargetY,int oval=1);
	static	int		GetAngleToTarget1024(int nX,int nY,int nTargetX,int nTargetY,int oval=1);
	static	int		GetPosToTarget(CPos *src,CPos *dest,int distance,int oval=1);

	static	int		GetXWithRadius(int nX,int nRadius,int nAngle);
	static	int		GetYWithRadius(int nY,int nRadius,int nAngle);
	static	POINT	GetPtWithRadius(int nX,int nY,int nXRadius,int nYRadius,int nAngle);

	static	int		GetXWithRadiusMirror(int nX,int nRadius,int nAngle);
	static	int		GetYWithRadiusMirror(int nY,int nRadius,int nAngle);
	static	POINT	GetPtWithRadiusMirror(int nX,int nY,int nXRadius,int nYRadius,int nAngle);

	static	int		GetAngleDir(int nAngle,int nTargetAngle);
	static	int		GetAngleMirror(int nAngle);
	static	int		GetAngleVector(int nAngle);

	static	int		ClipAngle(int nAngle);
};

enum
{
	eACCEL_ARRIVE,
	eACCEL_START,
};

class	CAccelValue
{
public:
	int				m_iAccelMethod;	//	시작하는거냐 줄어드는거냐..
	int				m_iBeginValue,m_iEndValue,m_iCurrentValue;
	int				m_iBeginValueForAccelSpeed;

	int				m_iCurrentSpeed,m_iDestSpeed;	//	현재 속도,가속도
	int				m_iAccelStep,m_iAccelTimes;	//	현재 가속도 스텝,몇 번만에 목표 속다가 될껴?
	int				m_iDestStepInTable,m_iDestValueInTable;	//	테이블 안에서 목표 스탭(최고 255),테이블 안에서 목표값

					CAccelValue()
					{
						m_iBeginValueForAccelSpeed	=	0;
						m_iAccelStep				=	0;
						m_iAccelTimes				=	0;
					}

	void			init(int _iBeginValue,int _iEndValue,int _iSteps,int _iDestSpeed,int _iAccelMethod=eACCEL_ARRIVE,int _iAccelTableFactor=1000);	//	초기화
	inline	void	setBeginValueForAccelSpeed(int _iValue)	//	가속을 시작하는 수치
	{
		m_iCurrentSpeed				=	m_iDestSpeed;
		m_iBeginValueForAccelSpeed	=	_iValue;
	}

	inline	int		getValue(){return	m_iCurrentValue;}	//	값 얻기
	BOOL			update(int _iStep=1);	//	업데이트

	void			initAccelSpeed(int _iDestSpeed,int _iTimes,int _iAccelMethod,int _iTableFactor=1000);	//	가속도 초기화
	void			updateSpeed(int _iStep=1);	// 가속도 업데이트
	inline	int		getSpeed(){return	m_iCurrentSpeed;}//	속도 얻기
};

extern	inline	int		GetOvalAngleToTarget(int _iX,int _iY,int _iTX,int _iTY)
{
	return	cANGLE::GetAngleToTarget(_iX,_iY,_iTX,_iTY,2);
}

extern	void	GetLinerRoad(CPos	*_lpRoad,int _iDistance,int _iAngle,int _iFps);
extern	int		GetLinerPath(int _iX1,int _iY1,int _iX2,int _iY2,CPos *_lpRoad,int _iTerm,int _iSize);
extern	int		GetLinerPath(int _iX1,int _iY1,int _iX2,int _iY2,CPos *_lpRoad,int _iSize);


extern	void	GetOvalAnglePos(CPos	*CurPos,int angle,int dist);
extern	void	GetOvalTargetPos(CPos	*_lpCurrentPos,int _iTargetPosX,int _iTargetPosY,int _iDistance);
extern	void	GetTargetPos(CPos	*_lpCurrentPos,int _iTargetPosX,int _iTargetPosY,int _iDistance);
extern	void	GetAnglePos(CPos	*CurPos,int angle,int dist);


const	int	dCOS_TABLE[360]	=
{  0,   4,   8,  13,  17,  22,  26,  31,  35,  40,  44,  49,  53,  57,  62,  66,  70,  75,  79,  83,
  87,  92,  96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 139, 143, 147, 151, 154, 158, 161,
 165, 168, 171, 175, 178, 181, 184, 187, 190, 193, 196, 199, 202, 205, 207, 210, 213, 215, 217, 220,
 222, 224, 226, 228, 230, 232, 234, 236, 238, 239, 241, 242, 244, 245, 247, 248, 249, 250, 251, 252,
 253, 253, 254, 255, 255, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 255, 255, 254, 253,
 253, 252, 251, 250, 249, 248, 247, 245, 244, 242, 241, 239, 238, 236, 234, 232, 230, 228, 226, 224,
 222, 220, 217, 215, 213, 210, 207, 205, 202, 199, 196, 193, 190, 187, 184, 181, 178, 175, 171, 168,
 165, 161, 158, 154, 151, 147, 143, 139, 136, 132, 128, 124, 120, 116, 112, 108, 104, 100,  96,  92,
  87,  83,  79,  75,  70,  66,  62,  57,  53,  49,  44,  40,  35,  31,  26,  22,  17,  13,   8,   4,
   0,  -4,  -8, -13, -17, -22, -26, -31, -35, -40, -44, -49, -53, -57, -62, -66, -70, -75, -79, -83,
 -87, -92, -96,-100,-104,-108,-112,-116,-120,-124,-128,-132,-136,-139,-143,-147,-151,-154,-158,-161,
-165,-168,-171,-175,-178,-181,-184,-187,-190,-193,-196,-199,-202,-205,-207,-210,-213,-215,-217,-220,
-222,-224,-226,-228,-230,-232,-234,-236,-238,-239,-241,-242,-244,-245,-247,-248,-249,-250,-251,-252,
-253,-253,-254,-255,-255,-256,-256,-256,-256,-256,-256,-256,-256,-256,-256,-256,-255,-255,-254,-253,
-253,-252,-251,-250,-249,-248,-247,-245,-244,-242,-241,-239,-238,-236,-234,-232,-230,-228,-226,-224,
-222,-220,-217,-215,-213,-210,-207,-205,-202,-199,-196,-193,-190,-187,-184,-181,-178,-175,-171,-168,
-165,-161,-158,-154,-151,-147,-143,-139,-136,-132,-128,-124,-120,-116,-112,-108,-104,-100, -96, -92,
 -87, -83, -79, -75, -70, -66, -62, -57, -53, -49, -44, -40, -35, -31, -26, -22, -17, -13,  -8,  -4
};

const	int	dSIN_TABLE[360]	=
{256, 256, 256, 256, 256, 256, 255, 255, 254, 253, 253, 252, 251, 250, 249, 248, 247, 245, 244, 242,
 241, 239, 238, 236, 234, 232, 230, 228, 226, 224, 222, 220, 217, 215, 213, 210, 207, 205, 202, 199,
 196, 193, 190, 187, 184, 181, 178, 175, 171, 168, 165, 161, 158, 154, 151, 147, 143, 139, 136, 132,
 128, 124, 120, 116, 112, 108, 104, 100,  96,  92,  87,  83,  79,  75,  70,  66,  62,  57,  53,  49,
  44,  40,  35,  31,  26,  22,  17,  13,   8,   4,   0,  -4,  -8, -13, -17, -22, -26, -31, -35, -40,
 -44, -49, -53, -57, -62, -66, -70, -75, -79, -83, -87, -92, -96,-100,-104,-108,-112,-116,-120,-124,
-128,-132,-136,-139,-143,-147,-151,-154,-158,-161,-165,-168,-171,-175,-178,-181,-184,-187,-190,-193,
-196,-199,-202,-205,-207,-210,-213,-215,-217,-220,-222,-224,-226,-228,-230,-232,-234,-236,-238,-239,
-241,-242,-244,-245,-247,-248,-249,-250,-251,-252,-253,-253,-254,-255,-255,-256,-256,-256,-256,-256,
-256,-256,-256,-256,-256,-256,-255,-255,-254,-253,-253,-252,-251,-250,-249,-248,-247,-245,-244,-242,
-241,-239,-238,-236,-234,-232,-230,-228,-226,-224,-222,-220,-217,-215,-213,-210,-207,-205,-202,-199,
-196,-193,-190,-187,-184,-181,-178,-175,-171,-168,-165,-161,-158,-154,-151,-147,-143,-139,-136,-132,
-128,-124,-120,-116,-112,-108,-104,-100, -96, -92, -87, -83, -79, -75, -70, -66, -62, -57, -53, -49,
 -44, -40, -35, -31, -26, -22, -17, -13,  -8,  -4,   0,   4,   8,  13,  17,  22,  26,  31,  35,  40,
  44,  49,  53,  57,  62,  66,  70,  75,  79,  83,  87,  92,  96, 100, 104, 108, 112, 116, 120, 124,
 128, 132, 136, 139, 143, 147, 151, 154, 158, 161, 165, 168, 171, 175, 178, 181, 184, 187, 190, 193,
 196, 199, 202, 205, 207, 210, 213, 215, 217, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238, 239,
 241, 242, 244, 245, 247, 248, 249, 250, 251, 252, 253, 253, 254, 255, 255, 256, 256, 256, 256, 256

};


#endif
