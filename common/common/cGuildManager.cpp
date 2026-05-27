// cGuildManager.cpp: implementation of the cGuildManager class.
//
//////////////////////////////////////////////////////////////////////
#include "cGuildManager.h"
#include "cSRVUTIL.h"
#include "cPACKET_WORLDSERVER.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int
l_aGuildExpTable[1000]=
{
00,250,420,770,1200,1710,2300,3190,4200,5330,6580,7950,9440,11390,13500,15770,18200,20790,23540,26450,29520,33250,
37180,41310,45640,50170,54900,59830,64960,70290,75820,82250,88920,95830,102980,110370,118000,125870,133980,142330,150920,159750,
168820,179070,189600,200410,211500,222870,234520,246450,258660,271150,283920,296970,310300,323910,337800,353190,368900,384930,401280,417950,
434940,452250,469880,487830,506100,524690,543600,562830,582380,602250,622440,644490,666900,689670,712800,736290,760140,784350,808920,833850,
859140,884790,910800,937170,963900,990990,1018440,1046250,1074420,1104850,1135680,1166910,1198540,1230570,1263000,1295830,1329060,1362690,1396720,1431150,
1465980,1501210,1536840,1572870,1609300,1646130,1683360,1720990,1759020,1799750,1840920,1882530,1924580,1967070,2010000,2053370,2097180,2141430,2186120,2231250,
2276820,2322830,2369280,2416170,2463500,2511270,2559480,2608130,2657220,2706750,2756720,2809870,2863500,2917610,2972200,3027270,3082820,3138850,3195360,3252350,
3309820,3367770,3426200,3485110,3544500,3604370,3664720,3725550,3786860,3848650,3910920,3973670,4036900,4100610,4164800,4232690,4301100,4370030,4439480,4509450,
4579940,4650950,4722480,4794530,4867100,4940190,5013800,5087930,5162580,5237750,5313440,5389650,5466380,5543630,5621400,5699690,5778500,5857830,5937680,6018050,
6098940,6184090,6269800,6356070,6442900,6530290,6618240,6706750,6795820,6885450,6975640,7066390,7157700,7249570,7342000,7434990,7528540,7622650,7717320,7812550,
7908340,8004690,8101600,8199070,8297100,8395690,8494840,8594550,8694820,8799950,8905680,9012010,9118940,9226470,9334600,9443330,9552660,9662590,9773120,9884250,
9995980,10108310,10221240,10334770,10448900,10563630,10678960,10794890,10911420,11028550,11146280,11264610,11383540,11503070,11623200,11743930,11865260,11987190,12109720,12237750,
12366420,12495730,12625680,12756270,12887500,13019370,13151880,13285030,13418820,13553250,13688320,13824030,13960380,14097370,14235000,14373270,14512180,14651730,14791920,14932750,
15074220,15216330,15359080,15502470,15646500,15791170,15936480,16082430,16229020,16376250,16524120,16678170,16832900,16988310,17144400,17301170,17458620,17616750,17775560,17935050,
18095220,18256070,18417600,18579810,18742700,18906270,19070520,19235450,19401060,19567350,19734320,19901970,20070300,20239310,20409000,20579370,20750420,20922150,21094560,21267650,
21441420,21615870,21791000,21966810,22143300,22326690,22510800,22695630,22881180,23067450,23254440,23442150,23630580,23819730,24009600,24200190,24391500,24583530,24776280,24969750,
25163940,25358850,25554480,25750830,25947900,26145690,26344200,26543430,26743380,26944050,27145440,27347550,27550380,27753930,27958200,28163190,28368900,28575330,28782480,28990350,
29198940,29415190,29632200,29849970,30068500,30287790,30507840,30728650,30950220,31172550,31395640,31619490,31844100,32069470,32295600,32522490,32750140,32978550,33207720,33437650,
33668340,33899790,34132000,34364970,34598700,34833190,35068440,35304450,35541220,35778750,36017040,36256090,36495900,36736470,36977800,37219890,37462740,37706350,37950720,38203550,
38457180,38711610,38966840,39222870,39479700,39737330,39995760,40254990,40515020,40775850,41037480,41299910,41563140,41827170,42092000,42357630,42624060,42891290,43159320,43428150,
43697780,43968210,44239440,44511470,44784300,45057930,45332360,45607590,45883620,46160450,46438080,46716510,46995740,47275770,47556600,47838230,48120660,48403890,48687920,48981250,
49275420,49570430,49866280,50162970,50460500,50758870,51058080,51358130,51659020,51960750,52263320,52566730,52870980,53176070,53482000,53788770,54096380,54404830,54714120,55024250,
55335220,55647030,55959680,56273170,56587500,56902670,57218680,57535530,57853220,58171750,58491120,58811330,59132380,59454270,59777000,60100570,60424980,60750230,61076320,61403250,
61731020,62068970,62407800,62747510,63088100,63429570,63771920,64115150,64459260,64804250,65150120,65496870,65844500,66193010,66542400,66892670,67243820,67595850,67948760,68302550,
68657220,69012770,69369200,69726510,70084700,70443770,70803720,71164550,71526260,71888850,72252320,72616670,72981900,73348010,73715000,74082870,74451620,74821250,75191760,75563150,
75935420,76308570,76682600,77057510,77433300,77820190,78208000,78596730,78986380,79376950,79768440,80160850,80554180,80948430,81343600,81739690,82136700,82534630,82933480,83333250,
83733940,84135550,84538080,84941530,85345900,85751190,86157400,86564530,86972580,87381550,87791440,88202250,88613980,89026630,89440200,89854690,90270100,90686430,91103680,91521850,
91940940,92360950,92781880,93203730,93626500,94050190,94474800,94900330,95326780,95754150,96182440,96622790,97064100,97506370,97949600,98393790,98838940,99285050,99732120,100180150,
100629140,101079090,101530000,101981870,102434700,102888490,103343240,103798950,104255620,104713250,105171840,105631390,106091900,106553370,107015800,107479190,107943540,108408850,108875120,109342350,
109810540,110279690,110749800,111220870,111692900,112165890,112639840,113114750,113590620,114067450,114545240,115023990,115503700,115984370,116466000,116948590,117432140,117916650,118402120,118900650,
119400180,119900710,120402240,120904770,121408300,121912830,122418360,122924890,123432420,123940950,124450480,124961010,125472540,125985070,126498600,127013130,127528660,128045190,128562720,129081250,
129600780,130121310,130642840,131165370,131688900,132213430,132738960,133265490,133793020,134321550,134851080,135381610,135913140,136445670,136979200,137513730,138049260,138585790,139123320,139661850,
140201380,140741910,141283440,141825970,142369500,142914030,143459560,144006090,144553620,145115250,145677920,146241630,146806380,147372170,147939000,148506870,149075780,149645730,150216720,150788750,
151361820,151935930,152511080,153087270,153664500,154242770,154822080,155402430,155983820,156566250,157149720,157734230,158319780,158906370,159494000,160082670,160672380,161263130,161854920,162447750,
163041620,163636530,164232480,164829470,165427500,166026570,166626680,167227830,167830020,168433250,169037520,169642830,170249180,170856570,171465000,172074470,172684980,173296530,173909120,174522750,
175137420,175767270,176398200,177030210,177663300,178297470,178932720,179569050,180206460,180844950,181484520,182125170,182766900,183409710,184053600,184698570,185344620,185991750,186639960,187289250,
187939620,188591070,189243600,189897210,190551900,191207670,191864520,192522450,193181460,193841550,194502720,195164970,195828300,196492710,197158200,197824770,198492420,199161150,199830960,200501850,
201173820,201846870,202521000,203196210,203872500,204549870,205228320,205907850,206588460,207270150,207952920,208636770,209321700,210007710,210694800,211398190,212102700,212808330,213515080,214222950,
214931940,215642050,216353280,217065630,217779100,218493690,219209400,219926230,220644180,221363250,222083440,222804750,223527180,224250730,224975400,225701190,226428100,227156130,227885280,228615550,
229346940,230079450,230813080,231547830,232283700,233020690,233758800,234498030,235238380,235979850,236722440,237466150,238210980,238956930,239704000,240452190,241201500,241951930,242703480,243456150,
244209940,244964850,245720880,246478030,247236300,247995690,248756200,249517830,250280580,251044450,251809440,252591890,253375500,254160270,254946200,255733290,256521540,257310950,258101520,258893250,
259686140,260480190,261275400,262071770,262869300,263667990,264467840,265268850,266071020,266874350,267678840,268484490,269291300,270099270,270908400,271718690,272530140,273342750,274156520,274971450,
275787540,276604790,277423200,278242770,279063500,279885390,280708440,281532650,282358020,283184550,284012240,284841090,285671100,286502270,287334600,288168090,289002740,289838550,290675520,291513650,
292352940,293193390,294035000,294877770,295721700,296566790,297413040,298260450,299109020,299976250,300844680,301714310,302585140,303457170,304330400,305204830,306080460,306957290,307835320,308714550,
309594980,310476610,311359440,312243470,313128700,314015130,314902760,315791590,316681620,317572850,318465280,319358910,320253740,321149770,322047000,322945430,323845060,324745890,325647920,326551150,
327455580,328361210,329268040,330176070,331085300,331995730,332907360,333820190,334734220,335649450,336565880,337483510,338402340,339322370,340243600,341166030,342089660,343014490,343940520,344867750,
345796180,346725810,347656640,348588670,349521900,350456330,351391960,352328790,353266820,354224750,355183920,356144330,357105980,358068870,359033000,359998370,360964980,361932830,362901920,363872250,
364843820,365816630,366790680,367765970,368742500,369720270,370699280,371679530,372661020,373643750,374627720,375612930,376599380,377587070,378576000,379566170,380557580,381550230,382544120,383539250,
384535620,385533230,386532080,387532170,388533500,389536070,390539880,391544930,392551220,393558750,394567520,395577530,396588780,397601270,398615000,399629970,400646180,401663630,402682320,403702250,
404723420,405745830,406769480,407794370,408820500,409847870,410876480,411906330,412937420,413969750,415003320,416058070,417114100,418171410,419230000,420289870,421351020,422413450,
};

cGuild::cGuild()
{
	InitializeCriticalSection(&csGuild);

	close();
}

cGuild::~cGuild()
{
	try
	{
		DeleteCriticalSection(&csGuild);
	}catch(...)
	{
	}
}

DWORD
cGuild::getExpSum20040602()
{
	DWORD	dwExpSum	=	m_dwExp;

	for (int iLevel=1;iLevel<m_wLevel;iLevel++)
	{
		DWORD	dwRequireExp=	l_aGuildExpTable[(iLevel-1)*2]+l_aGuildExpTable[(iLevel-1)*2+1];

		dwRequireExp		=	min(dwRequireExp,65535);
		dwExpSum			+=	dwRequireExp;
	}

	return	dwExpSum;
}

BOOL
cGuild::resetGuildExp20040602()
{
	DWORD	dwOldExp	=	m_dwExp;
	WORD	wOldLevel	=	m_wLevel;
	DWORD	dwExpSum	=	getExpSum20040602();

	//	초기화
	{
//		m_dwExp		=	0;
//		m_wLevel	=	1;
	}

//	increaseGuildExp(dwExpSum);
	_log("%18s] %d -> %d , %d -> %d",m_strName,m_wGuildPoint,m_wLevel-1,wOldLevel,m_wLevel);

	memset(m_abGuildSkill,0,sizeof(m_abGuildSkill));
	m_wExpTaxRate	=	getMaximumExpTax();
	m_wGuildPoint	=	m_wLevel-1;
	m_dwExp			=	0;

	return	TRUE;
}

void
cGuild::close()
{
	CCritical	CS(&csGuild);

	memset(this,0xff,sizeof(CGuildAdvanceInfo));
	m_strName[0] = 0;
	wPre	= 1;
	wSerial = 0xffff;
	m_bf3GuildGoodwillForRoen = 1;
//	m_bf3GuildDungeonPlayTime = 0;
	m_bf1IsUseGuildMasterGrace = 0;
//	Sleep(1000);
	dwUpdateTime = timeGetTime();

	wPre = 0xffff;		//	0xffff - 초기상태 , 1 - 임시 등록된 상태. 0 - 정식등록된 상태
	dwHashValue = 0;

	for(int i = 0;i < dGUILD_MAX_PLAYER;i++)
		m_stack[i].close();
	m_wNext = 0;
	m_wCount = 0;
}

int
cGuild::getCheckSum()
{
	int	iCheckSum	=	CGuildAdvanceInfo::getCheckSum(),i,iCount;

	for (i=0,iCount=0;i<dGUILD_MAX_PLAYER,iCount<m_wMemberCount;i++)
	{
		if	(m_stack[i].wSerial	>= 0xcccc)
			continue;

		iCount++;
		iCheckSum	+=	GetHashCode(m_stack[i].m_strName);
		iCheckSum	+=	m_stack[i].m_wRank;
		iCheckSum	+=	m_stack[i].wSerial;
	}

	return	iCheckSum;
}

void
cGuild::updateGuildInfo()
{
	m_wSubMasterCount	=	0;
	m_wMemberCount		=	0;
	m_wCongressManCount	=	0;

	for (int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if	(m_stack[i].wSerial	==	0xffff)
			continue;

		m_wMemberCount++;

		if	(m_stack[i].m_wRank	==	dGUILD_CLASS_SUBMASTER)
			m_wSubMasterCount++;
		if	(m_stack[i].m_wRank	==	dGUILD_CLASS_PARLIAMENT)
			m_wCongressManCount++;
	}
}

//
//	길드 경험치 증가. 증가 가능한 경험치를 증가 시키고 길드 레벨과 길드 스킬 포인트를 갱신한다.
int
cGuild::increaseGuildExp(DWORD _dwIncreaseExp)
{
	CCritical	CS(&csGuild);

	int	iCurrentGuildPointSum	=	getCurrentGuildPointSum();

	if	(iCurrentGuildPointSum	!=	getFullSkillPoint())
	{
		memset(m_abGuildSkill,0,c_iNormalGuildSkillCount);

		m_wGuildPoint			=	getFullSkillPoint();

		_log("%s 길드 경험치 처리 도중 길드 포인트가 이상해서 초기화 시킴",m_strName);

//	로그필요
	}

	if	(m_wLevel	>=	dMAX_GUILD_LEVEL)
		return	FALSE;

	WORD	iLevel	=	m_wLevel,iIncreaseLevel=0;

	while(_dwIncreaseExp)
	{
		DWORD	dwAddExp=	0xffffffff-m_dwExp;	//	증가 가능한 경험치(실제로 더해줄 경험치이다.)

		if (_dwIncreaseExp	<=	dwAddExp)	//	증가 가능한 범위 안에 있다.
		{
			dwAddExp		=	_dwIncreaseExp;
			_dwIncreaseExp	-=	dwAddExp;	//	더하기로 했던 경험치에서 증가 가능한 수치 만큼 빼준다.
			m_dwExp			+=	dwAddExp;	//	증가 가능한 수치 만큼 더해준다.
		}
		else	//	초과 했다. 경험치를 한계 수치로 설정한다.
		{
			_dwIncreaseExp	-=	dwAddExp;	//	더하기로 했던 경험치에서 증가 가능한 수치 만큼 빼준다.
			m_dwExp			=	0xffffffff;
		}


		while(1)
		{
			DWORD	dwRequireExp=	l_aGuildExpTable[(iLevel-1)*2]+l_aGuildExpTable[(iLevel-1)*2+1];

			if	(m_dwExp	<	dwRequireExp)
				break;

			m_dwExp		-=	dwRequireExp;

			iIncreaseLevel++;
			iLevel++;

			if	(iLevel	>=	dMAX_GUILD_LEVEL)
				break;
		}
	}

	m_wLevel			=	iLevel;
	m_wGuildPoint		+=	iIncreaseLevel;

	m_wLevel			=	min(m_wLevel,dMAX_GUILD_LEVEL);
	m_wExpTaxRate		=	min(getMaximumExpTax(),m_wExpTaxRate);
//	길드 경험치 적산 수치는 플레이어 경험치의 0.05%~(10/길드레벨)%임 (적산 시 버림 처리)

	return	iIncreaseLevel;
}

//
//	레벨 변경
BOOL
cGuild::makeLevel(int _iLevel)
{
	CCritical	CS(&csGuild);

	if (_iLevel		>=	dMAX_GUILD_LEVEL)	return	FALSE;

	int	iIncreaseLevel	=	_iLevel-m_wLevel;

	m_wLevel			=	_iLevel;
	m_dwExp				=	0;
	m_wGuildPoint		+=	iIncreaseLevel;
	m_wLevel			=	min(m_wLevel,dMAX_GUILD_LEVEL);
	m_wExpTaxRate		=	min(getMaximumExpTax(),m_wExpTaxRate);
//	길드 경험치 적산 수치는 플레이어 경험치의 0.05%~(10/길드레벨)%임 (적산 시 버림 처리)

	return	TRUE;
}

//
//	현재 길드 포인트의 합
int
cGuild::getCurrentGuildPointSum()
{
	int	iSum	=	0;

	for (int i=0;i<100;i++)
		iSum	+=	m_abGuildSkill[i];

	iSum	+=	m_wGuildPoint;

	return	iSum;
}
//
//	길드 경험치 증가. 증가 가능한 경험치를 증가 시키고 길드 레벨과 길드 스킬 포인트를 갱신한다.
//	그리고 결과 리턴
int
cGuild::increaseGuildSkillLevel(int _iSkill,CGuildSkillInfo *_lpSkillInfo)
{
	CCritical	CS(&csGuild);

	if	(_iSkill	==	0xffff)	//	완전 초기화(길드 석상 제외)
	{
		memset(m_abGuildSkill,0,sizeof(m_abGuildSkill));
		m_wGuildPoint			=	getFullSkillPoint();

		_lpSkillInfo->m_wSkill	=	_iSkill;

		return	eRIGSL_OK;
	}

	if	(_iSkill	==	0xfffe)	//	길드 스킬만 초기화
	{
		memset(m_abGuildSkill,0,c_iNormalGuildSkillCount);

		m_wGuildPoint			=	getFullSkillPoint();

		_lpSkillInfo->m_wSkill	=	_iSkill;

		for (int i=m_wMemberCount;i;)
		{
			--i;

			if	(m_stack[i].m_wRank	==	dGUILD_CLASS_SUBMASTER	||	m_stack[i].m_wRank	==	dGUILD_CLASS_PARLIAMENT)
				m_stack[i].m_wRank	=	dGUILD_CLASS_MEMBER;
		}

		return	eRIGSL_OK;
	}

	if	(_iSkill	==	0xfffd)	//	CH5용 초기화
	{
		memset(m_abGuildSkill,0,c_iNormalGuildSkillCount);

		m_bf6PeneltyBonusGuildPoint	=	m_bf6BonusGuildPoint;
		m_wGuildPoint				=	getFullSkillPoint();
		_lpSkillInfo->m_wSkill		=	_iSkill;

		for (int i=m_wMemberCount;i;)
		{
			--i;

			if	(m_stack[i].m_wRank	==	dGUILD_CLASS_SUBMASTER	||	m_stack[i].m_wRank	==	dGUILD_CLASS_PARLIAMENT)
				m_stack[i].m_wRank	=	dGUILD_CLASS_MEMBER;
		}

		return	eRIGSL_OK;
	}

	if	(m_wGuildPoint			<=	0	&&	_iSkill	<	c_iGuildStatueSkillBeginIndex)
		return	eRIGSL_SKILL_POINT_LACKS;
	if	(m_abGuildSkill[_iSkill]>	dMAX_GUILD_SKILL_LEVEL	)
		m_abGuildSkill[_iSkill]	=	0;
	if	(m_abGuildSkill[_iSkill]>=	dMAX_GUILD_SKILL_LEVEL	)
		return	eRIGSL_LEVEL_SKILL_IS_MAX;

	if	(_iSkill	>=	c_iGuildStatueSkillBeginIndex)
		if	(m_abGuildSkill[_iSkill]	>=	5)
			return	eRIGSL_LEVEL_SKILL_IS_MAX;
	
	m_abGuildSkill[_iSkill]++;

	if	(_iSkill	<	c_iGuildStatueSkillBeginIndex)
		m_wGuildPoint--;

	_lpSkillInfo->m_wLevel	=	m_abGuildSkill[_iSkill];
	_lpSkillInfo->m_wSkill	=	_iSkill;

	return	eRIGSL_OK;
}

void
cGuild::GetNextEmpty()
{
	for(int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if	(m_stack[i].wSerial == 0xffff)
		{
			m_wNext = i;
			return ;
		}
	}
	m_wNext = 0xffff;
}

WORD
cGuild::Add(CGuildMan * _p)
{	
	CCritical	CS(&csGuild);
	GetNextEmpty();

	WORD	wResult = m_wNext;
	if(wResult == 0xffff)		return 0xffff;

	memcpy(&m_stack[wResult],_p,sizeof(CGuildMan));
	m_stack[wResult].wSerial = wResult;
	m_wCount++;
	m_wMemberCount = m_wCount;
	return wResult;
}

void
cGuild::Del(WORD _serial)
{
	CCritical	CS(&csGuild);

	m_stack[_serial].close();
	if(m_wCount>0)	m_wCount--;
	m_wMemberCount = m_wCount;
}

void
cGuild::Del(char	*_strName)
{
	CCritical	CS(&csGuild);

	for(int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if(STRICMP(m_stack[i].m_strName,_strName)==0)
		{
			m_stack[i].close();
			if(m_wCount>0)	m_wCount--;
			break;
		}
	}
	m_wMemberCount = m_wCount;
}

CGuildMan*
cGuild::getMember(WORD _wSerial)
{
	CCritical	CS(&csGuild);

	if	(_wSerial	<	dGUILD_MAX_PLAYER && m_stack[_wSerial].wSerial	<	dGUILD_MAX_PLAYER)
		return &m_stack[_wSerial];

	return NULL;
}

CGuildMan *
cGuild::GetGuilderPnt(WORD	_wSerial)
{
	CCritical	CS(&csGuild);

	if	(_wSerial<dGUILD_MAX_PLAYER && m_stack[_wSerial].wSerial<0xcccc)	
		return &m_stack[_wSerial];

	return NULL;
}

CGuildMan *
cGuild::GetGuilderPnt(char * _strName)
{
	CCritical	CS(&csGuild);
	DWORD dwHashCode = cSRVUTIL::GetHashCode((BYTE *)_strName);

	for(int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if(STRICMP(m_stack[i].m_strName,_strName)==0)
			return &m_stack[i];
	}
	return NULL;
}

CGuildMan *
cGuild::GetGuilderPntByID(char * _strID)
{
	CCritical	CS(&csGuild);

	for(int i=0;i<dGUILD_MAX_PLAYER;i++)
		if	(STRICMP(m_stack[i].m_strId,_strID)==0)
			return &m_stack[i];

	return NULL;
}

CGuildMan*
cGuild::GetNextMaster()
{
	CCritical	CS(&csGuild);
	
	if	(m_wSubMasterCount>0)
	{
		for(int i=0;i<dGUILD_MAX_PLAYER;i++)
		{
			if(m_stack[i].wSerial>=0xcccc)	continue;
			if(m_stack[i].m_wRank==dGUILD_CLASS_SUBMASTER)
				return &m_stack[i];
		}
	}
	if(m_wCongressManCount>0)
	{
		for(int i=0;i<dGUILD_MAX_PLAYER;i++)
		{
			if(m_stack[i].wSerial>=0xcccc)	continue;
			if(m_stack[i].m_wRank==dGUILD_CLASS_PARLIAMENT)
				return &m_stack[i];
		}
	}
	for(int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if(m_stack[i].wSerial>=0xcccc)	continue;
		if(m_stack[i].m_wRank==dGUILD_CLASS_MEMBER)
			return &m_stack[i];
	}
	for(i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if(m_stack[i].wSerial>=0xcccc)	continue;
		if(m_stack[i].m_wRank==dGUILD_CLASS_ELDER)
			return &m_stack[i];
	}
	for(i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if(m_stack[i].wSerial>=0xcccc)	continue;
		if(m_stack[i].m_wRank==dGUILD_CLASS_ROOKIE)
			return &m_stack[i];
	}
	return NULL;
}

BOOL	cGuild::isMaster(char * _strMember)
{
	CCritical	CS(&csGuild);

	CGuildMan * man = GetGuilderPnt(_strMember);
	if(!man || man->wSerial>=0xcccc)		return FALSE;
	if(man->m_wRank!=dGUILD_CLASS_MASTER)	return FALSE;
	return TRUE;
}

BOOL	cGuild::isSubMaster(char * _strMember)
{
	CCritical	CS(&csGuild);

	CGuildMan * man = GetGuilderPnt(_strMember);
	if(!man || man->wSerial>=0xcccc)		return FALSE;
	if(man->m_wRank!=dGUILD_CLASS_SUBMASTER)	return FALSE;
	return TRUE;
}

BOOL	cGuild::isSubOrMaster(char * _strMember)
{
	CCritical	CS(&csGuild);

	CGuildMan * man = GetGuilderPnt(_strMember);
	if(!man || man->wSerial>=0xcccc)	return FALSE;
	if(man->m_wRank!=dGUILD_CLASS_MASTER && man->m_wRank!=dGUILD_CLASS_SUBMASTER)	return FALSE;
	return TRUE;
}

WORD	cGuild::GetRank(char * _strMember)
{
	CCritical	CS(&csGuild);

	for(int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if(m_stack[i].wSerial>=0xcccc)	continue;
		return m_stack[i].m_wRank;
	}
	return dGUILD_CLASS_ROOKIE;
}

WORD	cGuild::CheckJoinMember(char * _strMaster, char * _strMember)
{
	CCritical	CS(&csGuild);

	CGuildMan * man = GetGuilderPnt(_strMaster);
	CGuildMan * man2 = GetGuilderPnt(_strMember);
	//	_strMaster가 길마 혹은 부길마인가.
	if(!man || man->wSerial==0xffff || (man->m_wRank!=dGUILD_CLASS_MASTER && man->m_wRank!=dGUILD_CLASS_SUBMASTER))
		return dJOINGUILD_RESULT_NEEDRANK;
	//	최대 인원을 초과 했는가.. _strMember가 이미 있는가...
	if(man2 && man2->wSerial!=0xffff)
		return dJOINGUILD_RESULT_JOINED;
	
	return 0;
}

WORD	cGuild::CheckExitMember(char * _strMaster, char * _strMember)
{
	CCritical	CS(&csGuild);

	CGuildMan	*member			=	GetGuilderPnt(_strMaster);
	CGuildMan	*targetMember	=	GetGuilderPnt(_strMember);

	if	(!member || member->wSerial	==	0xffff)
		return dEXITGUILD_RESULT_FAIL;

	if	(!targetMember	|| targetMember->wSerial	==	0xffff)
		return dEXITGUILD_RESULT_FAIL;

	if	(STRICMP(_strMaster,_strMember)	!=	0)	//	타인을 쫒으려면 부길마나 길마여야 한다.
	{
		if	(member->m_wRank	!=	dGUILD_CLASS_MASTER &&	member->m_wRank	!=	dGUILD_CLASS_SUBMASTER)
			return dEXITGUILD_RESULT_NEEDRANK;

//		if	(targetMember->m_wRank	==	dGUILD_CLASS_ELDER)
//			return dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_ELDER;

		if	(targetMember->m_wRank	==	dGUILD_CLASS_PARLIAMENT	&&	member->m_wRank	!=	dGUILD_CLASS_MASTER)
			return dEXITGUILD_RESULT_FAILED_BY_IT_IS_ABLE_ONLY_MASTER;
	}

	if	(targetMember->m_wRank	==	dGUILD_CLASS_MASTER)
		return dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_GUILD_MASTER;

	return	0;
}

//****************************************************************************************
//	Class	Guild	Manager
//****************************************************************************************
cGuildManager::cGuildManager()
{	
	InitializeCriticalSection(&csDeque);

	for(int i=0;i<dGUILD_MAX_COUNT;i++)
		m_stack[i].close();

	m_wNext					=	0;
	m_wCount				=	0;
	m_bIsLoadGuildHallInfo	=	FALSE;

	m_timeLastUpdate.update();

	resetGuildHallInfo();
}

cGuildManager::~cGuildManager()
{
	DeleteCriticalSection(&csDeque);
}

cGuildManager & cGuildManager::GetInstance()
{
	static cGuildManager cmanager;

	return cmanager;
}

void
cGuildManager::GetNextEmpty()
{
	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if(m_stack[i].wSerial == 0xffff)
		{
			m_wNext = i;
			return ;
		}
	}
	m_wNext = 0xffff;
}

WORD
cGuildManager::Add(cGuild * _p)
{
	CCritical	CS(&csDeque);

	WORD	wResult = _p->m_wSerial;

	if	(wResult	>=	dGUILD_MAX_COUNT || m_stack[wResult].wSerial != 0xffff)
		return 0xffff;
	
	memcpy(&m_stack[wResult],_p,sizeof(cGuild));
	m_stack[wResult].wSerial = wResult;
	m_wCount++;
	m_stack[wResult].dwHashValue = cSRVUTIL::GetHashCode((BYTE *)_p->m_strName);

	cGuild	*lpGuild	=	&m_stack[wResult];

	for	(int iPet=0;iPet<dGUILD_PET_TYPE_COUNT;iPet++)
	{
		if	(lpGuild->m_aGuildPet[iPet].m_dwExperience	>=	c_iMaxGuildPetExp*2)
		{
			memset(lpGuild->m_aGuildPet,0,sizeof(lpGuild->m_aGuildPet));
			memset(lpGuild->m_aGuildGuardian,0,sizeof(lpGuild->m_aGuildGuardian));
		}
	}
	for	(int iGuardian=0;iGuardian<dGUILD_GUARDIAN_TYPE_COUNT;iGuardian++)
	{
		if	(lpGuild->m_aGuildGuardian[iGuardian].m_dwExperience	>=	c_iMaxGuildPetExp*2)
		{
			memset(lpGuild->m_aGuildPet,0,sizeof(lpGuild->m_aGuildPet));
			memset(lpGuild->m_aGuildGuardian,0,sizeof(lpGuild->m_aGuildGuardian));
		}
	}

	for	(int iItem=0;iItem<c_iGuildInventorySize;iItem++)
	{
		if	(lpGuild->m_aInventory[iItem].m_wBaseItem	==	0xffff)
			continue;
		if	(lpGuild->m_aInventory[iItem].m_wBaseItem	==	0	||	lpGuild->m_aInventory[iItem].m_wBaseItem	>=	10000)
			lpGuild->m_aInventory[iItem].reset();
	}
	
	if	(lpGuild->m_bf2TryGuildPointBattleCountPerWeek	>	c_iLimitOfTryGuildPointBattlePerWeek)
		lpGuild->m_bf2TryGuildPointBattleCountPerWeek	=	0;

	return wResult;
}

cGuild	*	
cGuildManager::GetGuildPnt(char *_strName)
{
	CCritical	CS(&csDeque);
	DWORD	dwHashValue = cSRVUTIL::GetHashCode((BYTE *)_strName);

	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if	(m_stack[i].m_wSerial==0xffff)
			continue;
		if	(m_stack[i].dwHashValue == dwHashValue)
			return &m_stack[i];
	}
	return NULL;
}

//	Serial과 고유 index를 동일시 처리 해버렸다. 문제 없겠지??
cGuild	*	
cGuildManager::GetGuildPnt(WORD	_wIndex)
{
	CCritical	CS(&csDeque);

	if	(_wIndex	>= dGUILD_MAX_COUNT	||	m_stack[_wIndex].wSerial >= dGUILD_MAX_COUNT)
		return NULL;
	else
		return &m_stack[_wIndex];
}

cGuild	*	
cGuildManager::GetGuildPntBySerial(WORD _wIndex)
{
	CCritical	CS(&csDeque);

	if	(_wIndex	>= dGUILD_MAX_COUNT	||	m_stack[_wIndex].wSerial >= dGUILD_MAX_COUNT)
		return NULL;
	else
		return &m_stack[_wIndex];
}

void		
cGuildManager::Del(char *	_strName)
{
	CCritical	CS(&csDeque);

	DWORD		dwHashValue = cSRVUTIL::GetHashCode((BYTE *)_strName);

	for(int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		if	(m_stack[i].dwHashValue == dwHashValue	&&	STRICMP(m_stack[i].m_strName,_strName)	==	0)
		{
			rebuildHallInfoByRemoveGuild(i);

			m_stack[i].close();

			if	(m_wCount	>	0)
				m_wCount--;

			break;
		}
	}
}

WORD		
cGuildManager::GetBlankGuildSerial()
{
	CCritical	CS(&csDeque);

	GetNextEmpty();

	return m_wNext;
}

CGuildMan *	
cGuildManager::GetGuildManPnt(char * _strName)
{
	CCritical	CS(&csDeque);
//	DWORD	dwHashValue = cSRVUTIL::GetHashCode((BYTE *)_strName);
//	CGuildAdvanceInfo정보가 변경되는 이유 때문에 _strName에 대한 hashcode검색이 불가능한 상태.

	CGuildMan * _man = NULL;

	for(int i = dGUILD_MAX_COUNT;i>0;)
	{
		--i;
		_man = m_stack[i].GetGuilderPnt(_strName);

		if	(_man)
			return _man;
	}
	return NULL;
}

void
cGuildManager::resetGuildHallInfo()
{
	int	aiHallCountByHallLevel[]	=	{0,0xffff,27,9,3,1};

	for	(int iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		for	(int iHall=0;iHall<aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			cGuildHallInfo	*lpHallInfo	=	&m_aHallList[iHallLevel][iHall];

			lpHallInfo->m_wOwnGuild			=	0xffff;
			lpHallInfo->m_wHallSerial		=	getGuildHallFieldSerial(iHallLevel,iHall,0);
			lpHallInfo->m_dwHashCode		=	0xffffffff;
			lpHallInfo->m_bf3HallLevel		=	iHallLevel;
			lpHallInfo->m_bf1IsOpen			=	TRUE;
			lpHallInfo->m_bf3HallShape		=	0;
			lpHallInfo->m_bf5Slot			=	iHall;
			lpHallInfo->m_bf12BattleField	=	getBattleFieldByHall(iHallLevel,iHall);
			lpHallInfo->m_wInvadeGuild		=	0xffff;
		}
	}
}

//	전투 필드 시리얼 리턴
int
cGuildManager::getBattleFieldByHall(int _iHallLevel,int _iHallSlot)
{
	int	aiHallCountByHallLevel[]	=	{0,0xffff,27,9,3,1};
	int	aiBattleFieldSerial[6]		=	{0,0,931,938,940,941};

	return	aiBattleFieldSerial[_iHallLevel]-aiHallCountByHallLevel[_iHallLevel]+_iHallSlot+1;
}

//
//	길드 정보에 따라 길드 홀 정보를 빌드 한다.
void
cGuildManager::buildHallInfoByGuild()
{
	int	iGuild,iHallLevel,iHallCount=0,iGuildCount;

	int	aiHallCountByHallLevel[]	=	{0,0xffff,27,9,3,1};
	int	iBattleFieldSerial[6]		=	{0,0,940,941};

	resetGuildHallInfo();

	cIndexForQSort	aGuildList[dGUILD_MAX_COUNT];

	for	(iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		iHallCount	=	0;
		iGuildCount	=	0;

		for	(iGuild=0;iGuild<dGUILD_MAX_COUNT;iGuild++)
		{
			cGuild	*lpGuild	=	GetGuildPnt(iGuild);

			if	(!lpGuild	||	lpGuild->m_wHallLevel	!=	iHallLevel)
				continue;

			aGuildList[iGuildCount].m_iIndex	=	iGuild;
			aGuildList[iGuildCount].m_iValue	=	lpGuild->m_iVictoryPoint;
			iGuildCount++;
		}

		qsort((void *)aGuildList,iGuildCount,sizeof(cIndexForQSort),(int(*)(const void *,const void *))DesCompare);

		for	(int i=0;i<iGuildCount;i++)
		{
			cIndexForQSort	*lpIndex	=	&aGuildList[i];
			iGuild						=	lpIndex->m_iIndex;
			cGuild			*lpGuild	=	GetGuildPnt(iGuild);

			if	(iHallCount	>=	aiHallCountByHallLevel[iHallLevel])
			{
				ERRMSG("error in initialize guild hall info","over guildhall count!!");
				break;
			}

			cGuildHallInfo	*lpHallInfo	=	&m_aHallList[iHallLevel][iHallCount];

			lpHallInfo->m_wOwnGuild			=	iGuild;
			lpHallInfo->m_wHallSerial		=	getGuildHallFieldSerial(iHallLevel,iHallCount,lpGuild->m_wGuildHallShape);
			lpHallInfo->m_dwHashCode		=	GetHashCode(lpGuild->m_strName);
			lpHallInfo->m_bf3HallLevel		=	iHallLevel;
			lpHallInfo->m_bf1IsOpen			=	lpGuild->m_bf1IsPermitVisitor;
			lpHallInfo->m_bf3HallShape		=	lpGuild->m_wGuildHallShape;
			lpHallInfo->m_bf5Slot			=	iHallCount;
			lpHallInfo->m_bf12BattleField	=	getBattleFieldByHall(iHallLevel,iHallCount);
			lpHallInfo->m_wInvadeGuild		=	0xffff;

			iHallCount++;
		}
	}
}

//
//	길드의 길드홀 정보를 길드홀 정보에 따라 설정한다.
void
cGuildManager::buildGuildHallInfoByHall()
{
	int	iGuild,iHallLevel,iHallCount=0;

	int	aiHallCountByHallLevel[]	=	{0,0xffff,27,9,3,1};

	for	(iGuild=0;iGuild<dGUILD_MAX_COUNT;iGuild++)
	{
		cGuild	*lpGuild	=	GetGuildPnt(iGuild);

		if	(!lpGuild	||	lpGuild->m_wHallLevel	<=	1)
			continue;

		lpGuild->m_wHallLevel	=	0;
	}

	for	(iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		for	(int iHall=0;iHall<aiHallCountByHallLevel[iHallLevel];iHall++)
		{
			int	iGuild	=	m_aHallList[iHallLevel][iHall].m_wOwnGuild;

			if	(iGuild	==	0xffff)
				break;

			cGuild	*lpGuild	=	GetGuildPnt(iGuild);

			if	(!lpGuild)
				continue;

			lpGuild->m_wHallLevel	=	iHallLevel;
		}
	}
}

int
cGuildManager::getInvalidHallCount(int _iHallLevel)
{
	int	aiHallCountByHallLevel[]	=	{0,0xffff,27,9,3,1};

	for	(int iHall=0;iHall<aiHallCountByHallLevel[_iHallLevel];iHall++)
	{
		int	iGuild	=	m_aHallList[_iHallLevel][iHall].m_wOwnGuild;

		if	(iGuild	==	0xffff)
			return	aiHallCountByHallLevel[_iHallLevel]-iHall;
	}

	return	0;
}

void
cGuildManager::resetInvadeGuildInfo()
{
	for	(int i=0;i<=c_iMaxGuildHallLevel;i++)
		for	(int j=0;j<c_iGuildHallCount;j++)
			m_aHallList[i][j].m_wInvadeGuild	=	0xffff;
}

//	공성전 할 길드 설정
void
cGuildManager::setInvadeGuild(int _iHallLevel,int _iOrder,int _iGuild)
{
	m_aHallList[_iHallLevel][_iOrder].m_wInvadeGuild	=	_iGuild;
}

//	_iGuild가 제거 되어 _iHallLevel레벨 홀의 순서를 정리 하고 공성전 정보를 정리한다.
void
cGuildManager::rebuildHallInfoByRemoveGuild(int _iGuild)
{
	int	aiHallCountByHallLevel[]	=	{0,0,27,9,3,1},iHall;
	int	iHallLevel					=	0;
	int	iCheckHallLevel				=	-1;
	int	iHallCount					=	0;

	for	(iHallLevel=2;iHallLevel<=c_iMaxGuildHallLevel;iHallLevel++)
	{
		iHallCount					=	aiHallCountByHallLevel[iHallLevel];

		for	(iHall=0;iHall<iHallCount;iHall++)
		{
			if	(m_aHallList[iHallLevel][iHall].m_wOwnGuild	==	_iGuild)
			{
				for	(int i=iHall;i<iHallCount-1;i++)	//	길드정보를 앞으로 댕긴다
				{
					int	iFieldSerial	=	m_aHallList[iHallLevel][i].m_wHallSerial;
					int	iBattleField	=	m_aHallList[iHallLevel][i].m_bf12BattleField;

					memcpy(&m_aHallList[iHallLevel][i],&m_aHallList[iHallLevel][i+1],sizeof(cGuildHallInfo));

					m_aHallList[iHallLevel][i].m_bf5Slot			=	i;
					m_aHallList[iHallLevel][i].m_bf12BattleField	=	iBattleField;
					m_aHallList[iHallLevel][i].m_wHallSerial		=	iFieldSerial;
				}

				m_aHallList[iHallLevel][iHallCount-1].reset();	//	마지막건 리셋
				iCheckHallLevel	=	iHallLevel;
				break;
			}
		}
	}

	if	(iCheckHallLevel	==	-1	||	iCheckHallLevel	>=	5)
		return;

	iCheckHallLevel				-=	1;
	iHallCount					=	aiHallCountByHallLevel[iCheckHallLevel];

	for	(iHall=0;iHall<iHallCount;iHall++)
	{
		if	(m_aHallList[iCheckHallLevel][iHall].m_wInvadeGuild	==	_iGuild)
		{
			m_aHallList[iCheckHallLevel][iHall].m_wInvadeGuild	=	0xffff;
			break;
		}
	}
}
