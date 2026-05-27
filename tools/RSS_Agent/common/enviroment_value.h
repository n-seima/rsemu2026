//
//	제작 일시	2006.07.22
//
//	목적		환경 변수의 일괄적인 관리를 위한 클래스
//
//	제작		이영찬
//

#pragma	once

//	사용 예
//	cEnviromentValue	g_envValue;
//
//	아래 설정은 어플리케이션 초기화시에 해준다.
//	g_envValue.init();
//	g_envValue.addValue(&g_sound.m_iVolume,4,-1);
//	g_envValue.addValue(&g_music.m_iVolume,4,-1);
//	g_envValue.addValue(&g_interface.m_posPos,sizeof(g_interface.m_posPos),-1);
//	이 순서나 사이즈는 변경되면 안된다. 만약 사이즈가 변경된다면 해당 수치는 
//	로딩시 읽어 들이지 않는다. 
//	g_envValue.loadValue("config.dat");	//	
//
//	이후 설정된 수치들은 그냥 사용한다.
//	
//	어플리케이션 종료 직전 혹은 필요 할때마다 저장을 해 준다.
//	g_envValue.saveValue("config.dat");	//	
//	

//
//	환경 변수
class	cEnviromentValue
{
public:
	class	cValueInfo
	{
	public:
		unsigned	short	m_wSize;
		TCHAR				*m_lpBuffer;

		cValueInfo()
		{
			m_lpBuffer	=	NULL;
			m_wSize		=	0;
		}
	};

	enum
	{
		eMaxValueCount	=	256,
	};

	int			m_iCount,m_iMaxCount;
	cValueInfo	*m_pValues;

	cEnviromentValue(int _iMaxCount=eMaxValueCount);
	~cEnviromentValue();

	void		init(int _iSize=eMaxValueCount);
	bool		addValue(void *_lpValue,int _iSize,int _iSlot=-1);	
	//	수치 추가,_iSlot이 -1이면 빈 슬롯을 찾아 추가 시킨다.
	bool		loadValue(TCHAR *_lpstrName);	//	불러오기
	bool		saveValue(TCHAR *_lpstrName);	//	저장하기
};