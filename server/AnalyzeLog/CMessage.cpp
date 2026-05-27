#include "CMessage.h"

char	g_strGameFolder[512];

#ifdef	_FOR_KOREA
char	*gstrMiniPetName[]	=
{
"불의 영",
"물의 영",
"바람의 영",
"대지의 영",
"빛의 영",
"어둠의 영",
"사라만다",
"운디네",
"실프",
"놈",
"위스프",
"섀도우",
"화",
"수",
"풍",
"지",
"빛",
"암",
"하오링",
"미오링",
"수오링",
"다오링",
"차오링",
"고오링",
};


char	*g_strMessage[]	=
{
//	0~5
	"부숴진 ",
	"개량할 수 없는",
	"저주받은",
	"'%d'번 인덱스를 가지는 아이템은 없습니다. 다시 입력해 주세요.",
	"길드전예약",
	"길드전취소",

//	6~10
	"길드전결정",
	"길드전결과",
	"OX 퀴즈 승자",
	"OX 퀴즈 참가자",
	"OX 퀴즈 탈락자",

//	11~15
	"OX 퀴즈 질문",
	"길드가입",
	"길드탈퇴",
	"길드전시작",
	"길드전결과정보",

//	16~20
	"길드전죽이기에러",
	"길드전결과에러",
	"길드전_적죽임",
	"아이템수리 비용",
	"PickBy소환수",
//	21~25
	"ReceiveFrom소환수",
	"길드전멸",
	"성공",
	"부숴짐",
	"파괴됨",
//	26~30
	"부서진 상태 수리",
	"저주 수리",
	"접두사를 붙일 수 없는 상태 수리",
	"수리는 했지만 접두사가 사라짐",
	"인챈트 정보가 이상함",
//	31~35
	"Item 전부",
	"Item 드랍",
	"Item 줍기",
	"Item 구입/판매",
	"Item 삭제",
//	36~40
	"Item 교환",
	"Item 은행",
	"Item 구매",
	"Item 파기",
	"Item 복사품",
//	41~45
	"Item 인챈트",
	"Item 노점상",
	"Item 잘못된 접두사",
	"캐릭터 전부",
	"캐릭터 생성",
//	46~50
	"캐릭터 삭제",
	"캐릭터 인",
	"캐릭터 아웃",
	"이벤트 관련 로그",
	"길드 관련 로그",
//	51~55
	"모든 데이타 검색",	
	"캐릭터 저장",
	"저장",
	"아이템기간연장",
	"아이템 생성",
//	56~60
	"아이템 사용",
	"유료 아이템",
	"캐릭터 생성",
	"캐릭터 제거",
	"로그인",

//	61~65
	"로그 아웃",
	"프리미엄 아이템 꺼냄",
	"프리미엄 아이템 생성",
	"복권",
	"신비한 아이템 상자",

//	66~70
	"사육 기록",
	"검색결과",
	"총 %d건이 검색되었습니다.",
	"성공적으로 다운 받았다. -0-",
	"잘못된 아이템 제거",

//	71~75
	"사용자가 아이템 파기",
	"퀘스트에서 아이템 제거",
	"죽음 페널티 회복 비용",
	"사육 기록서 사용",
	"길드전 필드 아이템",

//	76~80
	"복권 꽝",
	"당근 꺼내기",
	"당근으로 구매",
	"남은 당근",
	"꺼낸 당근",

//	80~85
	"캐릭터 보유 당근",
	"보물상자에서 얻음",
	"몬스터에게서 훔침",
	"DX 아이템 수리",
	"접두사 제거",

//	86~90
	"아이템 조각 합침",
	"아이템 감별",
	"퀘스트 관련",
	"퀘스트 보상 경험치",
	"퀘스트 완료",

//	91~95
	"해킹툴 사용",
	"운영자 로그",
	"필드 이동 하면서 파기됨",
	"보물지도(서버 경험치 200%)",
	"보물지도(서버 골드 400%)",
	
//	96~100
	"보물지도(개인 경험치 200%)",
	"보물지도(개인 골드 400%)",
	"보물지도",
	"백섭",
	"펫 관련 정보",

//	101~105
	"펫 조련",
	"펫 제거",
	"펫 사육 기록서 제작",
	"펫 사육 기록서에서 꺼냄",
	"제거(바닥없는상자)",

//	106~110
	"생성(바닥없는상자)",
	"바닥 없는 상자",
	"낱말 퀴즈 승자",
	"낱말 퀴즈 참가자",
	"낱말 퀴즈 탈락자",

//	111~115
	"낱말 퀴즈 질문",
	"채팅",
	"일반 대화",
	"귓말",
	"파티 대화",

//	116~120
	"길드 대화",
	"외치기",
	"관리자 메시지",
	"길드 펫에게 먹임",
	"연고지 추가",

//	121~125
	"길드 창고에 보관",
	"길드 창고에서 꺼냄",
	"연고지 제거",
	"미니펫 관련",
	"미니펫 생성",

//	126~130
	"미니펫 제거",
	"미니펫 시간 연장",
	"미니펫이 주워 옴",
	"아이템 제거[미니펫 봉인]",
	"아이템 제거[미니펫 봉인 해제]",

//	131~135
	"아이템 제거[미니펫 시간 연장]",
	"아이템 생성[미니펫 봉인]",
	"아이템 제거[미니펫 먹이]",
	"길드 명예 포인트",
	"길드 던젼 탐험 횟수",

//	136~140
	"길드 마스터 은총",
	"길드 로엔의 호감도 변화",
	"미니펫 봉인",
	"미니펫 봉인 해제",
	"전생",

//	141~145
	"미니펫 제거(미니펫 섞기)",
	"미니펫 구속",
	"보물상자 이벤트로 얻음",
	"당근 상점",
	"결투",

//	146~150
	"노점상",
	"Item 판매",
	"Item 유효기간 종료",
	"Item 줏음",
	"상점에서 구매",

//	151~155
	"비둘기 상점에서 구매",
	"길드 상점에서 구매",
	"은행에서 금괴 구매",
	"아이템 강화",		// 09.09.04
	"강화 성공",		// 09.09.04
//	156~160

	"강화 실패",		// 09.09.04
	"미사용",		// 09.09.04
	"재료",		// 09.09.04
	"보조제",		// 09.09.04
	"일반",		// 09.09.04
//	161~165

	"프리미엄",		// 09.09.04
	"신비석",		// 09.09.04
	"완성된 탄틸리스 유물",		// 09.09.04
	"결정석",		// 09.09.04
	"→",		// 09.09.04

//	166~170

	"토큰상점",		// 09.10.27
	"일반상점",		// 09.10.27
	"개",			// 09.10.27
	"사랑의전선구",
	"길드상점",

//	170~175
	"은행금괴",
	"배지 상점",
	"접두사 잘라내기",
	"접두사 복사 성공",
	"접두사 복사 실패",

//	176~180
	"접두사 붙이기 성공",
	"접두사 붙이기 실패",
	"각인 레벨 증가 성공",
	"각인 레벨 증가 실패",
	"각인 레벨 설정 성공",
	
	//	181~185
	"각인 레벨 설정 실패",
	"아이템 분해",
	"GVG길드 전송",
	"GVG아바타 전송",
	"길드 삭제",

	//	186~190
	"길드원 탈퇴",
	"생성 실패",
	"접속 시간 초과",
	"신입회원",
	"정회원",

	//	191~195
	"의원",
	"부 마스터",
	"원로",
	"마스터",
	"임명",

	//	196~200
	"해임",
	"캐릭터 삭제로 임명",
};

#else

char	*gstrMiniPetName[]	=
{
	"SpritOfFire",
	"SpritOfWater",
	"SpritOfWind",
	"SpritOfEarth",
	"SpritOfLight",
	"SpritOfDark",
	"Salamander",
	"Undine",
	"Silf",
	"Nom",
	"Wisp",
	"Shadow",
	"Hwa",
	"Soo",
	"Poong",
	"Ji",
	"Vit",
	"Am",
	"Haoring",
	"Mioring",
	"Sooring",
	"Daoring",
	"Chaoring",
	"Goring",
};

char	*g_strMessage[]	=
{
//	0~5
	"broken",
	"can not improve",
	"cursed",
	"not exist number '%d' index item.",
	"BookingGuildBattle",
	"CancelGuildBattle",

//	6~10
	"EngageGuildBattle",
	"ResultOfGuildBattle",
	"OX Quiz Winner",
	"OX Quiz Entry",
	"OX Quiz Loser",

//	11~15
	"OX Quiz Question",
	"JoinGuild",
	"LeaveGuild",
	"BeginGuildBattle",
	"ResultOfGuildBattle",

//	16~20
	"GuildBattleError",
	"ResultOfGuildBattleError",
	"KillEnemy",
	"Repair Item Price",
	"PickBySummonedBeast",
//	21~25
	"ReceiveFromSummonedBeast",
	"ExterminateGuild",
	"Success",
	"broken",
	"destroy",
//	26~30
	"repair Broken",
	"repair curse",
	"repair can not attact prefix status",
	"repair but remove prefix",
	"incorrect enchant info",
//	31~35
	"All Item",
	"Drop Item",
	"Pick Item",
	"Sell/Buy Item",
	"Destroy Item",
//	36~40
	"Trade Item",
	"Bank Item",
	"Buy Item",
	"Expired Item",
	"Copy Item",
//	41~45
	"Enchant Item",
	"Street Stall",
	"Incorrect Prefix",
	"All Character",
	"Create Character",
//	46~50
	"Remove Character",
	"Log In",
	"Log Out",
	"About Event",
	"About Guild",
//	51~55
	"Search All Data",	
	"Save Log",	
	"Save",	
	"Renew Term",
	"Create Item",
//	56~60
	"Use Item",
	"Premium Item",
	"Create Character",
	"Remove Character",
	"Log in",
//	61~65
	"Log out",
	"Withdraw Premium Item",
	"Create Premium Item",
	"Lottery Item",
	"Mystery Item Box",
//	66~70
	"Breeding Record Book",
	"Serch result",
	"total %d item(s) found",
	"download complete",
	"remove Incorrect Item",

//	71~75
	"destroy item by player",
	"remove by quest",
	"cost of recover death penelty",
	"unseal breeading record book",
	"guild battle field item",

//	76~80
	"Lottery ticket Failed",
	"Withdraw Carrot",
	"Buy by Carrot",
	"Remain Carrot",
	"Withdraw Carrot",

//	81~85
	"Player Own Carrot",
	"Pick in Treasure Box",
	"Steal Monster Item",
	"Repair DX Item",
	"Remove Prefix",

//	86~90
	"Merge Item Piece",
	"Judge to Item",
	"About Quest",
	"Quest reward Exp",
	"Complete Qeust",

//	91~95
	"Use Hacking Tool",
	"Operator Log",
	"Remove Item by Move Field",
	"Treasure map(Server Exp 200%)",
	"Treasure map(Server Gold 400%)",
	
//	96~100
	"Treasure map(Personal Exp 200%)",
	"Treasure map(Personal Gold 400%)",
	"Treasure map",
	"Back Server",
	"About Pet",

//	101~105
	"Tame Pet",
	"Remove PEt",
	"Seal Pet",
	"Unseal Pet",
	"Mixed by Bottomless box(remove)",

//	106~110
	"Create by Bottomless box",
	"Bottomless box",
	"Word Quiz Winner",
	"Word Quiz Comer",
	"Word Quiz Loser",

//	111~115
	"Word Quiz Question",
	"Chatting",
	"Normal Chatting",
	"Whisper",
	"Party Chatting",

//	116~120
	"Guild Chatting",
	"Shout",
	"Admin Message",
	"Feed To Guild Pet",
	"Add a place reated to guild",

//	121~125
	"Storing In Guild Storehouse",
	"Withdraw From Guild Storehouse",
	"Remove a place reated to guild",
	"About Mini Pet",
	"Create Mini Pet",

//	126~130
	"Remove Mini Pet",
	"Extend Mini Pet Time",
	"MiniPet Pick a Item",
	"Remove Item[Seal MiniPet]",
	"Remove Item[Unseal MiniPet]",

//	131~135
	"Remove Item[Extend MiniPet Time]",
	"Create Item[Seal MiniPet]",
	"Remove Item[MiniPet feed]",
	"Guild Honor Point",
	"Explore Guild Dungeon",

//	136~140
	"Glory of Guild Master",
	"GoodWill of Guild Roen",
	"Seal Mini pet",
	"Unseal Mini pet",
	"Rebirth",

//	141~145
	"Release by Mix Minipet",
	"Restraint Minipet",
	"Get Gold By Treasure Event",
	"Carrot Shop",
	"Duel",

//	146~150
	"street stall",
	"Sell Item",
	"Expired Item",
	"Pick Item",
	"in Shop",

//	151~155
	"in Pigeon Shop",
	"in Guild Shop",
	"in Gold Bar Shop",
	"Improve Item ",		// 09.09.04
	"Success to Improve Item",		// 09.09.04
//	156~160

	"Faield to Improve Item",		// 09.09.04
	"not used",		// 09.09.04
	"stuff",		// 09.09.04
	"supprot stuff",		// 09.09.04
	"Normal",		// 09.09.04
//	161~165

	"Premiun",		// 09.09.04
	"Mystic Stone",		// 09.09.04
	"Relic of Tantaliss",		// 09.09.04
	"Cristal Stone",		// 09.09.04
	"→",		// 09.09.04

//	166~170

	"Token Shop",		// 09.10.27
	"Normal Shop",		// 09.10.27
	"Count",			// 09.10.27
	"Pigeon Shop",
	"Guild Shop",

//	170~175
	"Bank Goldbar",
	"BadgeOfTrader Shop",
	"Cut Prefix",
	"Success Copy Prefix",
	"Failed Copy Prefix",

//	176~180
	"Success Attach Prefix",
	"Failed Attach Prefix",
	"Success Increase Carving Level",
	"Failed Increase Carving Level",
	"Success Set Carving Level",

//	181~185
	"Failed Set Carving Level",
	"Disjointing Item",
	"Trans Guild",
	"Trans Avatar",
	"delete guild",

	//	186~190
	"withdraw guild",
	"create fail",
	"time over",
	"rookie",
	"member",

	//	191~195
	"parliament",
	"submaster",
	"elder",
	"master",
	"appoint",

	//	196~200
	"dismissal",
	"changerank_master_appoint_special",
};

#endif