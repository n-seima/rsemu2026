#include "SFC.h"
#include "miniPet.h"
#include "CMessage.h"
#include "CSKill.h"
#include "cHERO.h"
#include "miniPetWindow.h"

cSTRING	cMiniPet::s_strTooltip;

char				*g_strMiniPetWav[]	=
{
"fyg_in",
"fyg_ot",
"",
"",
"",
"",
"fyg_n1",
"",
"",
"fyg_e1",
"fyg_e2",
"fyg_e3",
"fyg_lv",
"",
"wag_in",
"wag_ot",
"",
"",
"",
"",
"wag_n1",
"",
"",
"wag_e1",
"wag_e2",
"wag_e3",
"wag_lv",
"",
"wig_in",
"wig_ot",
"",
"",
"",
"",
"wig_n1",
"",
"",
"wig_e1",
"wig_e2",
"wig_e3",
"wig_lv",
"",
"eyg_in",
"eyg_ot",
"",
"",
"",
"",
"eyg_n1",
"",
"",
"eyg_e1",
"eyg_e2",
"eyg_e3",
"eyg_lv",
"",
"lyg_in",
"lyg_ot",
"",
"",
"",
"",
"lyg_n1",
"",
"",
"lyg_e1",
"lyg_e2",
"lyg_e3",
"lyg_lv",
"",
"dyg_in",
"dyg_ot",
"",
"",
"",
"",
"dyg_n1",
"",
"",
"dyg_e1",
"dyg_e2",
"dyg_e3",
"dyg_lv",
"",
"sal_in",	//	"잘 부탁해.",	
"sal_ot",	//	"그럼 힘내.",	
"sal_s1",	//	"간다!",	
"sal_s2",	//	"자! 받아!",	
"sal_fi",	//	"해치웠다!",	
"sal_bk",	//	"우리가 해냈어.",	
"sal_n1",	//	"나랑 있으니 춥지 않지?",	
"sal_n2",	//	"넌 내가 책임지고 지켜줄게.",	
"sal_n3",	//	"사냥도 좋지만, 몸도 생각해 가면서 해.",	
"sal_e1",	//	"성의는 고맙지만, 맛이 흐으음.",	
"sal_e2",	//	"이거 꽤 맛있는 걸?",	
"sal_e3",	//	"덕분에 정말 포식한다.",	
"sal_lv",	//	"고마워. 니 덕분이야.",	
"sal_uu",	//	"축하해. 앞으로도 힘내자.",	
"und_in",	//	"부르셨나요?",	
"und_ot",	//	"다음에 뵈요.",	
"und_s1",	//	"자, 가요!",	
"und_s2",	//	"제가 도울게요!",	
"und_fi",	//	"절 물로 보지 마요.",	
"und_bk",	//	"우리들의 승리에요.",	
"und_n1",	//	"모든 생명의 근원은 물이죠.",	
"und_n2",	//	"비가 오면 기분이 좋아지지 않나요?",	
"und_n3",	//	"힘드시죠. 여기 물 한 잔.",	
"und_e1",	//	"뭔가 부족한 맛이네요.",	
"und_e2",	//	"괜찮은 음식이군요.",	
"und_e3",	//	"훌륭한 맛이예요.",	
"und_lv",	//	"당신 덕분이에요. 고마워요.",	
"und_uu",	//	"축하해요. 앞으로도 힘내요.",	
"sil_in",	//	"모시게 되어 영광이옵니다.",	
"sil_ot",	//	"소녀는 물러가겠사옵니다.",	
"sil_s1",	//	"받으시지요.",	
"sil_s2",	//	"각오하시지요.",	
"sil_fi",	//	"이정도론 어림도 없는 게지요.",	
"sil_bk",	//	"역시 대단하옵니다.",	
"sil_n1",	//	"공기의 흐름, 그 것이 바람이옵니다.",	
"sil_n2",	//	"바람이 없으면, 배도 움직일 수 없었을 것이옵니다.",	
"sil_n3",	//	"바람은 날씨에 큰 영향을 주옵니다.",	
"sil_e1",	//	"오늘은 식욕이 별로 없사옵니다.",	
"sil_e2",	//	"잘 먹겠사옵니다.",	
"sil_e3",	//	"소녀에겐 과분한 대접이옵니다.",	
"sil_lv",	//	"앞으로도 더 분발하겠사옵니다.",	
"sil_uu",	//	"감축드리옵니다.",	
"gno_in",	//	"헤이호! 날 불렀는가!",	
"gno_ot",	//	"다음에도 즐겁게 만나세!",	
"gno_s1",	//	"내가 돕겠네.",	
"gno_s2",	//	"으라차차.",	
"gno_fi",	//	"내 실력도 아직 녹슬지 않았군.",	
"gno_bk",	//	"허허허. 월척이로세.",	
"gno_n1",	//	"생명은 땅에서 태어나 땅으로 돌아간다네.",	
"gno_n2",	//	"모험은 준비의 연속이지. 출발 전에 준비하는 습관을 가지게.",	
"gno_n3",	//	"허허허. 웃는 건 좋은 습관이라네.",	
"gno_e1",	//	"허허. 이런 푸대접은 처음일세.",	
"gno_e2",	//	"허허. 정말 맛있군.",	
"gno_e3",	//	"내 생애 먹어본 것 중에 최고일세.",	
"gno_lv",	//	"허허! 정말 축하하네~",	
"gno_uu",	//	"정말 축하하네. ",	
"wis_in",	//	"절 부르셨나요?",	
"wis_ot",	//	"저 삐졌어요.",	
"wis_s1",	//	"괜찮죠?",	
"wis_s2",	//	"난 몰라~",	
"wis_fi",	//	"미안해.",	
"wis_bk",	//	"정말 고생했어요.",	
"wis_n1",	//	"빛은 모든 희망의 상징이죠.",	
"wis_n2",	//	"몬스터들과 평화롭게 살 순 없을까요?",	
"wis_n3",	//	"전 당신을 믿어요.",	
"wis_e1",	//	"으음. 맛있네요.",	
"wis_e2",	//	"괜찮은 맛이네요.",	
"wis_e3",	//	"맛있어요.",	
"wis_lv",	//	"고마워요.",	
"wis_uu",	//	"축하해요.",	
"sha_in",	//	"뵙게되서 영광입니다.",	
"sha_ot",	//	"그럼 이만 물러가겠습니다.",	
"sha_s1",	//	"느립니다.",	
"sha_s2",	//	"이쪽입니다!",	
"sha_fi",	//	"당신의 패배입니다.",	
"sha_bk",	//	"당신의 이름 기억해두겠습니다.",	
"sha_n1",	//	"그건. 비밀입니다.",	
"sha_n2",	//	"어둠은 인간의 본성에 가장 가깝다고 할 수 있죠.",	
"sha_n3",	//	"원하는 걸 얻기위해서는 그만한 희생이 따르죠.",	
"sha_e1",	//	"이건 제 입맛에 맞지 않습니다.",	
"sha_e2",	//	"이건 영양가가 많은 음식이로군요.",	
"sha_e3",	//	"아주 훌륭한 음식입니다.",	
"sha_lv",	//	"이 모든 힘은 다 당신을 위한 겁니다.",	
"sha_uu",	//	"축하합니다. 당신은 계속 강해질 겁니다.",	
"wha_in",	//	"함께 청춘을 불태워보자.",	
"wha_ot",	//	"난 이대로 포기하지 않아!",	
"wha_s1",	//	"자, 간닷!",	
"wha_s2",	//	"아뵤~!",	
"wha_fi",	//	"근성이 그정도 밖에 안되나?",	
"wha_bk",	//	"멋진 승부 잊지 않겠다.",	
"wha_n1",	//	"사나이는 자신의 상대를 감으로 알 수 있지.",	
"wha_n2",	//	"먹이를 노리는 맹수의 눈동자를 본 적 있나?",	
"wha_n3",	//	"수행은 자신을 지키는 무기가 된다.",	
"wha_e1",	//	"맛없는 음식도 근성으로 먹는다.",	
"wha_e2",	//	"덕분에 영양 보충 잘했다.",	
"wha_e3",	//	"최고의 음식을 먹는 것도 수행의 일부지.",	
"wha_lv",	//	"수행을 앞으로 더 게을리하면 안되겠군.",	
"wha_uu",	//	"축하한다. 앞으로 더 정진하자.",	
"soo_in",	//	"어라? 여긴 어디지?",	
"soo_ot",	//	"네? 어디로 들어가죠?",	
"soo_s1",	//	"막아봐!",	
"soo_s2",	//	"아이쿠!",	
"soo_fi",	//	"제가 해치운게 맞나요?",	
"soo_bk",	//	"어? 끝난 건가요?",	
"soo_n1",	//	"이 길이 아닌가 봐요.",	
"soo_n2",	//	"몬스터가 없네요. 헤헤",	
"soo_n3",	//	"무슨 말을 할 지 까먹었어요.",	
"soo_e1",	//	"한 그릇 더 주세요.",	
"soo_e2",	//	"여기 반찬 좀 주세요.",	
"soo_e3",	//	"아줌마 여기 단무지 추가요?",	
"soo_lv",	//	"고마워요. 헤헤.",	
"soo_uu",	//	"축하해요~",	
"poo_in",	//	"만나서 반갑다구.",	
"poo_ot",	//	"너무 아쉽다구.",	
"poo_s1",	//	"간다구!",	
"poo_s2",	//	"막아보라구!",	
"poo_fi",	//	"나는 지지않아!",	
"poo_bk",	//	"우린 무적이라구.",	
"poo_n1",	//	"넌 나의 동료라구. 그 사실을 잊지마.",	
"poo_n2",	//	"동료는 무슨 일이 있어도 내가 지킨다구.",	
"poo_n3",	//	"몬스터들은 인정사정 봐줄 필요 없다구.",	
"poo_e1",	//	"다음부터는 맛있는 거로 달라구.",	
"poo_e2",	//	"다음엔 라면으로 달라구.",	
"poo_e3",	//	"다음에도 이런 걸로 달라구.",	
"poo_lv",	//	"좋았으! 힘이 넘친다구.",	
"poo_uu",	//	"레벨업도 했으니 한턱쏘라구.",	
"zee_in",	//	"무슨 일이에요?",	
"zee_ot",	//	"나야 고맙죠. 혼자 잘해봐요.",	
"zee_s1",	//	"어금니 꽉 깨물어!",	
"zee_s2",	//	"안경 벗어!",	
"zee_fi",	//	"별 것도 아닌게 까불기는.",	
"zee_bk",	//	"이제부터 여긴 내가 접수한다.",	
"zee_n1",	//	"왜 그런 눈으로 보세요.",	
"zee_n2",	//	"껌 좀 드릴까요?",	
"zee_n3",	//	"무슨 일이 있어도 제가 옆에 있을게요.",	
"zee_e1",	//	"다이어트하기도 바쁜 데, 이런 것 까지 먹으라고요? 우웩.",	
"zee_e2",	//	"먹기 싫다니까 정말 귀찮게 하네요.",	
"zee_e3",	//	"아까우니까 그냥 먹을게요.",	
"zee_lv",	//	"고맙긴 한데, 한턱 쏘셔야죠?",	
"zee_uu",	//	"축하해요. 헤헤.",	
"bit_in",	//	"안녕하세용.",	
"bit_ot",	//	"저 자러가용.",	
"bit_s1",	//	"자! 갈게용!",	
"bit_s2",	//	"막아보세요.",	
"bit_fi",	//	"제 도움이 컸나용?",	
"bit_bk",	//	"덕분에 살았네용.",	
"bit_n1",	//	"재미있는 이야기 좀 해주세용.",	
"bit_n2",	//	"심심해용.",	
"bit_n3",	//	"모험이 그렇게 좋은가용?",	
"bit_e1",	//	"맛없어용.",	
"bit_e2",	//	"잘 먹을게용.",	
"bit_e3",	//	"맛있어용.",	
"bit_lv",	//	"칭찬해주세용.",	
"bit_uu",	//	"축하해용.",	
"arm_in",	//	"낄낄낄~ 무슨 일이야?",	
"arm_ot",	//	"낄낄~ 난 자러 갈게.",	
"arm_s1",	//	"여기지롱. 낄낄~",	
"arm_s2",	//	"우히히히~",	
"arm_fi",	//	"히히~ 내가 한건 했어.",	
"arm_bk",	//	"지는 줄 알았다고. 낄낄낄.",	
"arm_n1",	//	"인생 뭐있어? 즐겁게 지내자고. 낄낄.",	
"arm_n2",	//	"자~ 신나는 모험을 시작해보자고. 흐흐.",	
"arm_n3",	//	"심심한데, 뭐 재미있는거 없어? 히히.",	
"arm_e1",	//	"우웨웩~ 놀랐지? 낄낄.",	
"arm_e2",	//	"어거 먹을만 한데? 히히.",	
"arm_e3",	//	"맛있다~ 맛있어. 흐흐.",	
"arm_lv",	//	"어라? 벌써 레벨업인가? 히히히.",	
"arm_uu",	//	"축하해~ 흐흐흐~",	
"hao_in",	//	"만나서 반가워요.",	
"hao_ot",	//	"더 도와드리고 싶은데, 아쉽네요.",	
"hao_s1",	//	"지금이야!",	
"hao_s2",	//	"제가 지원할게요.",	
"hao_fi",	//	"계획대로 되고 있군요.",	
"hao_bk",	//	"우리의 작전이 먹혀들었어요.",	
"hao_n1",	//	"불이 타오르는 모습은 정말 아름답지 않나요?",	
"hao_n2",	//	"불은 온도에 따라 색이 다르다고 해요.",	
"hao_n3",	//	"만약에 불이 없었다면, 인간이 이렇게 잘 살 수 있었을까요?",	
"hao_e1",	//	"덜익었네요. 제가 더 데워먹죠.",	
"hao_e2",	//	"적당히 익었는데, 약간 탄 것 같아요.",	
"hao_e3",	//	"전 태운 음식이 제일 좋아요.",	
"hao_lv",	//	"더 큰 불을 만들 수 있을 것 같아요.",	
"hao_uu",	//	"축하해요. 기념으로 불꽃놀이라도 보여드릴까요?",	
"mio_in",	//	"안녕하십니까.",	
"mio_ot",	//	"가보겠습니다.",	
"mio_s1",	//	"지금입니다.",	
"mio_s2",	//	"처리할게요.",	
"mio_fi",	//	"모든 것이 계산대로 되었군요.",	
"mio_bk",	//	"우린 불가능을 가능으로 만들었어요.",	
"mio_n1",	//	"계획과 실행, 그리고 결과 이것이 모든 일의 전부입니다.",	
"mio_n2",	//	"긴장을 늦추지 마십시오.",	
"mio_n3",	//	"승리의 조건의 정보력의 우위를 점하는 것이죠.",	
"mio_e1",	//	"이런 식단은 스트레스만 유발하지요.",	
"mio_e2",	//	"그냥 먹을 것 그 이상 그 이하도 아니군요.",	
"mio_e3",	//	"음식 그 이상의 무언가가 있는 것 같군요.",	
"mio_lv",	//	"아직도 갈 길이 멀군요.",	
"mio_uu",	//	"축하해요.",	
"suo_in",	//	"만나서 반가워.",	
"suo_ot",	//	"그럼 잘있어.",	
"suo_s1",	//	"도망가면서 때리기!",	
"suo_s2",	//	"노는 척 하면서 보조!",	
"suo_fi",	//	"어때? 정신없지?",	
"suo_bk",	//	"저 친구는 나의 함정에 걸려들어 저리 된거지.",	
"suo_n1",	//	"양치기 소년은 나랑 가장 친한 친구야.",	
"suo_n2",	//	"피노키오라는 친구를 아니?",	
"suo_n3",	//	"거짓말도 가끔은 필요할때가 있지.",	
"suo_e1",	//	"너무 맛있다. 너도 먹어.",	
"suo_e2",	//	"이거 맛이 좀 애매한데?",	
"suo_e3",	//	"이거 맛없어. 내가 대신 먹어줄게.",	
"suo_lv",	//	"기분 정말 좋다. 정말이야.",	
"suo_uu",	//	"축하해! 정말로!",	
"dao_in",	//	"좀 더 자면 안 될까?",	
"dao_ot",	//	"아함. 자러갈게.",	
"dao_s1",	//	"간다?",	
"dao_s2",	//	"각오해.",	
"dao_fi",	//	"어? 무슨 일이 있었나?",	
"dao_bk",	//	"깜짝이야. 이게 무슨 소리야?",	
"dao_n1",	//	"이건 조는 것도 아니고, 자는 것도 아니여~",	
"dao_n2",	//	"대지는 보이진 않지만, 아주 조금씩 움직이고 있어.",	
"dao_n3",	//	"어제 일찍 잤는데도 졸립다. 하암~",	
"dao_e1",	//	"이런 걸 먹느니 잠을 더 자겠어.",	
"dao_e2",	//	"이거 먹을 만한데, 졸음이 가시는 느낌이야.",	
"dao_e3",	//	"오! 잠이 확깨는 맛인걸.",	
"dao_lv",	//	"이제 한숨 자고 하면 안돼?",	
"dao_uu",	//	"축하해. 기념으로 한숨 자자.",	
"cha_in",	//	"어머, 무슨 일이죠?",	
"cha_ot",	//	"전 이만 자야겠군요.",	
"cha_s1",	//	"난 몰라요!",	
"cha_s2",	//	"제가 엄호할게요.",	
"cha_fi",	//	"야호! 제가 해냈어요.",	
"cha_bk",	//	"정말 굉장해요.",	
"cha_n1",	//	"제가 도와드릴 일은 없나요?",	
"cha_n2",	//	"빛이 있기에 어둠도 생기는 법이죠.",	
"cha_n3",	//	"모험은 정말 피곤하네요.",	
"cha_e1",	//	"이게 뭔가요? 맛있는 것 좀 달라구요.",	
"cha_e2",	//	"좀 더 몸에 좋은 음식은 없나요?",	
"cha_e3",	//	"오늘은 웰빙 식단인가요?",	
"cha_lv",	//	"기념으로 선물 좀 사달라구요.",	
"cha_uu",	//	"축하해요.",	
"goo_in",	//	"여긴 무서운 곳이에요. 덜덜덜.",	
"goo_ot",	//	"자다가 가위 눌려요. 덜덜덜.",	
"goo_s1",	//	"공격할까요? 덜덜덜.",	
"goo_s2",	//	"문제없어. 덜덜덜.",	
"goo_fi",	//	"확실히 끝난거죠? 덜덜덜.",	
"goo_bk",	//	"무서워 죽는 줄 알았어요. 덜덜덜.",	
"goo_n1",	//	"저기서 누가 쳐다보고 있는 것 같아요.",	
"goo_n2",	//	"세상에는 불가사의한 일이 많이 일어나죠.",	
"goo_n3",	//	"외계인의 존재를 믿으세요?",	
"goo_e1",	//	"이런 끔찍한 맛은 죽을 때까지 못잊을 거에요.",	
"goo_e2",	//	"평범한 건 정말 싫지만, 음식 불평은 하면 안되죠.",	
"goo_e3",	//	"무언가 굉장한 경험을 한 느낌이 나는 맛이에요.",	
"goo_lv",	//	"앞으로 더 새로운 경험을 하고 싶어요.",	
"goo_uu"	//	"축하해요. 뭔가 특별한 것이 느껴지지 않나요?",	
};

int	g_aiExpTableForMiniPet[102]	=	
{
	0,	10,	30,	60,	100,	150,	210,	280,	360,	450,	550,	660,	780,	910,	1050,	1200,	1360,	1530,	1710,	1900,	2100,
	2310,	2530,	2760,	3000,	3250,	3510,	3780,	4060,	4350,	4650,	4960,	5280,	5610,	5950,	6300,	6660,	7030,	7410,	7800,	8200,	8610,
	9030,	9460,	9900,	10350,	10810,	11280,	11760,	12250,	12750,	13260,	13780,	14310,	14850,	15400,	15960,	16530,	17110,	17700,	18300,	18910,	19530,
	20160,	20800,	21450,	22110,	22780,	23460,	24150,	24850,	25560,	26280,	27010,	27750,	28500,	29260,	30030,	30810,	31600,	32400,	33210,	34030,	34860,
	35700,	36550,	37410,	38280,	39160,	40050,	40950,	41860,	42780,	43710,	44650,	45600,	46560,	47530,	48510,	49500,	50500
};

cMiniPetTypeInfo	g_aMiniPetTypeInfo[nsMiniPetType::Count]	={
	nsMiniPetType::SpritOfFire,	nsMiniPetStyle::Deault,"불의 영",
	nsMiniPetType::SpritOfWater,nsMiniPetStyle::Deault,"물의 영",
	nsMiniPetType::SpritOfWind,	nsMiniPetStyle::Deault,"바람의 영",
	nsMiniPetType::SpritOfEarth,nsMiniPetStyle::Deault,"대지의 영",
	nsMiniPetType::SpritOfLight,nsMiniPetStyle::Deault,"빛의 영",
	nsMiniPetType::SpritOfDark,	nsMiniPetStyle::Deault,"어둠의 영",

	nsMiniPetType::Salamander,	nsMiniPetStyle::Bio,"사라만다",
	nsMiniPetType::Undine,		nsMiniPetStyle::Bio,"운디네",
	nsMiniPetType::Silf,		nsMiniPetStyle::Bio,"실프",
	nsMiniPetType::Nom,			nsMiniPetStyle::Bio,"놈",
	nsMiniPetType::Wisp,		nsMiniPetStyle::Bio,"위스프",
	nsMiniPetType::Shadow,		nsMiniPetStyle::Bio,"섀도우",

	nsMiniPetType::Hwa,			nsMiniPetStyle::Energy,"화",
	nsMiniPetType::Soo,			nsMiniPetStyle::Energy,"수",
	nsMiniPetType::Poong,		nsMiniPetStyle::Energy,"풍",
	nsMiniPetType::Ji,			nsMiniPetStyle::Energy,"지",
	nsMiniPetType::Vit,			nsMiniPetStyle::Energy,"빛",
	nsMiniPetType::Am,			nsMiniPetStyle::Energy,"암",

	nsMiniPetType::Haoring,		nsMiniPetStyle::Mineral,"하오링",
	nsMiniPetType::Mioring,		nsMiniPetStyle::Mineral,"미오링",
	nsMiniPetType::Sooring,		nsMiniPetStyle::Mineral,"수오링",
	nsMiniPetType::Daoring,		nsMiniPetStyle::Mineral,"다오링",
	nsMiniPetType::Chaoring,	nsMiniPetStyle::Mineral,"차오링",
	nsMiniPetType::Goring,		nsMiniPetStyle::Mineral,"고오링",
};


int
GetMiniPetTypeByName(char *_lpstrName)
{
	for (int i=0;i<nsMiniPetType::Count;i++)
	{
		if	(STRICMP(g_aMiniPetTypeInfo[i].m_strName,_lpstrName)	==	0)
			return	i;
	}

	return	-1;
}

void
SetMinipetTypeName()
{
	for (int i=0;i<nsMiniPetType::Count;i++)
	{
		strncpy(g_aMiniPetTypeInfo[i].m_strName,g_pstrCommonMessage[dWM_MINIPET_TYPE_NAME_BEGIN+i],30);
	}
}


int
cMiniPet::getGrowBioFactor()
{
	int	iTotal	=	m_iVitalPoint+m_iEnergyPoint+m_iMineralPoint;

	if	(iTotal	==	0)
		return	0;

	return	((LONGLONG)m_iVitalPoint)*100/iTotal;
}

int
cMiniPet::getGrowEnergyFactor()
{
	int	iTotal	=	m_iVitalPoint+m_iEnergyPoint+m_iMineralPoint;

	if	(iTotal	==	0)
		return	0;

	return	((LONGLONG)m_iEnergyPoint)*100/iTotal;
}

int
cMiniPet::getGrowMineralFactor()
{
	int	iTotal	=	m_iVitalPoint+m_iEnergyPoint+m_iMineralPoint;

	if	(iTotal	==	0)
		return	0;

	return	((LONGLONG)m_iMineralPoint)*100/iTotal;
}

BOOL
cMiniPet::resetSkill(int _iMiniPetIndex)
{
	if	(_iMiniPetIndex	<	0	||	_iMiniPetIndex	>=	c_iMiniPetCount	||	g_hero.m_aMiniPet[_iMiniPetIndex].m_bf7Level == 0)
		return FALSE;


	cMiniPet	*lpPet			=	&g_hero.m_aMiniPet[_iMiniPetIndex];
	if(!lpPet)
		return FALSE;

	int iSkillPoint = 0;
	for (int i=0;i<c_iMiniPetSkillCount;i++)
	{
		if(lpPet->m_aAbility[i].m_bf11Skill  == 0)
			continue;
		if(lpPet->m_aAbility[i].m_bf4Level == 0)
			continue;
		
		CSkill*	lpSkill	=	&g_aSkill[lpPet->m_aAbility[i].m_bf11Skill];
		
		if(!lpSkill)
			continue;
		if(lpPet->m_aAbility[i].m_bf4Level)
		{
			int iMax = lpPet->m_aAbility[i].m_bf4Level;
			
			lpPet->m_aAbility[i].m_bf4Level =	0;
			lpPet->m_aAbility[i].m_bf1IsOFF	=	FALSE;
			if(lpSkill->m_bf1IsDefaultMiniPetSkill)
			{
				lpPet->m_aAbility[i].m_bf4Level = 1;
			}
		}
	}
	
	lpPet->m_aAbility[c_iMiniPetSkillCount-1].m_bf11Skill	=	0;
	lpPet->m_aAbility[c_iMiniPetSkillCount-1].m_bf4Level	=	0;
	lpPet->m_aAbility[c_iMiniPetSkillCount-1].m_bf1IsOFF	=	FALSE;
	lpPet->m_bf7SkillPoint									=	m_bf7Level - 1;
	return TRUE;
}


int 
cMiniPet::getMaxGrowFactor()
{
	if(m_iVitalPoint > m_iEnergyPoint && m_iVitalPoint > m_iMineralPoint)
		return nsMiniPetStyle::Bio;

	if(m_iEnergyPoint > m_iVitalPoint && m_iEnergyPoint > m_iMineralPoint)
		return nsMiniPetStyle::Energy;

	if(m_iMineralPoint > m_iVitalPoint && m_iMineralPoint > m_iEnergyPoint)
		return nsMiniPetStyle::Mineral;

	return nsMiniPetStyle::Deault;
}

char*
cMiniPet::getName()
{
	static	char	strTempName[c_iMiniPetNameLength+1];

	memcpy(strTempName,m_strName,c_iMiniPetNameLength);

	strTempName[c_iMiniPetNameLength]	=	NULL;

	return	strTempName;
}

cMiniPetAbilityInfo*
cMiniPet::getAbilityBySkill(int _iSkill)
{
	for (int i=0;i<c_iMiniPetSkillCount;i++)
	{
		if	(m_aAbility[i].m_bf11Skill	==	_iSkill)
			return	&m_aAbility[i];
	}

	return	NULL;
}

BOOL
cMiniPet::isLevelUpAbleAbility(cMiniPetAbilityInfo	*_lpAbility)
{
	if	(!_lpAbility	||	_lpAbility->m_bf11Skill	==	0	||	_lpAbility->m_bf4Level	>=	c_iMiniPetSkillMaxLevel)
		return	FALSE;

	cAbility	ability;

	ability.set(_lpAbility->m_bf11Skill,_lpAbility->m_bf4Level);
	
	CSkill	*lpSkill	=	ability.getSkill();

	if	(!lpSkill	)
		return	FALSE;	//	이건 문제인데.. --
	if	(m_bf7SkillPoint	<	_lpAbility->m_bf4Level+1)
		return	FALSE;

	for (int i=0;i<dMAX_REQUIRE_SKILL_COUNT;i++)
	{
		if	(lpSkill->m_aRequireSkill[i].m_wSkill	==	0xffff)
			break;
		
		int	iSkill	=	lpSkill->m_aRequireSkill[i].m_wSkill;

		_lpAbility	=	getAbilityBySkill(iSkill);

		if	(!_lpAbility)
			return	FALSE;
		if	(_lpAbility->m_bf4Level	<	lpSkill->m_aRequireSkill[i].m_wLevel)
			return	FALSE;
	}

	return	TRUE;
}

//	잔여 스킬과 레벨을 확인해 스킬 추가
int
cMiniPet::addSpareSkill()
{
	if	(m_bf7Level			<	100)
		return	-1;	
	if	(m_bf7SkillPoint	==	0)
		return	-1;

	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];

		if	(lpAbility->m_bf11Skill	!=	0	&&lpAbility->m_bf4Level	<	5	&&	lpAbility->m_bf4Level	<	m_bf7SkillPoint)
			return	-1;
	}

	int	iType	=	m_bf5Type%6;

	for (int i=0;i<dMAX_SKILL;i++)
	{
		CSkill				*lpSkill	=	&g_aSkill[i];

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	c_iMiniPetSpareSkillType)
			continue;

		if	(lpSkill->m_aValue[0]	!=	(short)m_bf7SkillPoint)
			continue;

		if	(lpSkill->m_aValue[1]	!=	iType	&&	lpSkill->m_aValue[2]	!=	iType	&&	lpSkill->m_aValue[3]	!=	iType)
			continue;

		m_aAbility[c_iMiniPetSkillCount-1].m_bf11Skill	=	i;
		m_aAbility[c_iMiniPetSkillCount-1].m_bf4Level	=	1;
		m_aAbility[c_iMiniPetSkillCount-1].m_bf1IsOFF	=	FALSE;
		m_bf7SkillPoint	=	0;

		return	i;
	}

	return	-1;
}	
					


void
cMiniPet::unseal(cItem *_lpItem)
{
	memset(this,0,sizeof(cMiniPet));

	cBlockLightPocket	*lpPocket	=	(cBlockLightPocket	*)_lpItem;

	m_bf7Level		=	lpPocket->m_bf7Level	;
	m_bf5Type		=	lpPocket->m_bf5Type		;
	m_bf16Exp		=	lpPocket->m_bf16Exp		;
	m_bf7SkillPoint	=	lpPocket->m_bf7SkillPoint;
	m_iEnergyPoint	=	lpPocket->m_iEnergyPoint;
	m_iMineralPoint	=	lpPocket->m_iMineralPoint;
	m_iVitalPoint	=	lpPocket->m_iVitalPoint	;

	m_bf4ExpireMonth=	lpPocket->m_bf4ExpireMonth;
	m_bf5ExpireYear	=	lpPocket->m_bf5ExpireYear	;
	m_bf5ExpireDay	=	lpPocket->m_bf5ExpireDay	;
	m_bf5ExpireHour	=	lpPocket->m_bf5ExpireHour	;
	m_bf6ExpireMinute=	lpPocket->m_bf6ExpireMinute;

	int	i,iSkillCount=0;

	for (i=0;i<dMAX_SKILL && iSkillCount < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkillCount];
		CSkill				*lpSkill	=	&g_aSkill[i];

		if	(lpSkill->m_wSerial	==	0xffff)
			continue;

		if	(lpSkill->m_bf5MiniPetType	!=	m_bf5Type	&&	lpSkill->m_bf5MiniPetType	!=	(m_bf5Type	%	6))
			continue;

		lpAbility->m_bf11Skill	=	i;
		lpAbility->m_bf4Level	=	0;
		lpAbility->m_bf1IsOFF	=	FALSE;
		if	(lpSkill->m_bf1IsDefaultMiniPetSkill)
			lpAbility->m_bf4Level	=	1;

		iSkillCount++;
	}
	
	m_aAbility[0].m_bf4Level	=	lpPocket->m_bf3SkillLevel1;
	m_aAbility[1].m_bf4Level	=	lpPocket->m_bf3SkillLevel2;
	m_aAbility[2].m_bf4Level	=	lpPocket->m_bf3SkillLevel3;
	m_aAbility[3].m_bf4Level	=	lpPocket->m_bf3SkillLevel4;
	m_aAbility[4].m_bf4Level	=	lpPocket->m_bf3SkillLevel5;
	m_aAbility[5].m_bf4Level	=	lpPocket->m_bf3SkillLevel6;
	m_aAbility[6].m_bf4Level	=	lpPocket->m_bf3SkillLevel7;
	m_aAbility[7].m_bf4Level	=	lpPocket->m_bf3SkillLevel8;
	m_aAbility[8].m_bf4Level	=	lpPocket->m_bf3SkillLevel9;
	m_aAbility[9].m_bf4Level	=	lpPocket->m_bf3SkillLevel10;

	int	iSkillPoint	=	m_bf7SkillPoint;

	m_bf7SkillPoint	=	m_bf7Level-1;

	for (i=0;i<iSkillCount;i++)
	{
		if	(m_aAbility[i].m_bf11Skill	==	0)
			continue;

		for (int j=0;j<m_aAbility[i].m_bf4Level;j++)
		{
			if	(j	==	0)
			{
				CSkill	*lpSkill	=	&g_aSkill[m_aAbility[i].m_bf11Skill];

				if	(lpSkill->m_bf1IsDefaultMiniPetSkill)
					continue;
			}

			if	(m_bf7SkillPoint>=	(UINT)(j+1))
				m_bf7SkillPoint	-=	(j+1);
			else
				m_bf7SkillPoint	=	m_bf7SkillPoint;
		}
	}

	addSpareSkill();
}


bool
cMiniPet::isExpired()
{
	CTimeInfo	currentTime,expireTime;

	currentTime.update();

	expireTime.m_wYear	=	m_bf5ExpireYear+2000;
	expireTime.m_wMonth	=	m_bf4ExpireMonth	;
	expireTime.m_wDay	=	m_bf5ExpireDay		;
	expireTime.m_wHour	=	m_bf5ExpireHour		;
	expireTime.m_wMinute=	m_bf6ExpireMinute	;

	if	(currentTime.getMinuteValue(2000)	>	expireTime.getMinuteValue(2000))
		return	TRUE;

	return	FALSE;
}

cMiniPetAbilityInfo*	
cMiniPet::getPowerUpSkill(int _iSkill)
{
	if	(m_bf7Level	==	0)
		return	NULL;
	
	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];
		
		if	(lpAbility->m_bf4Level	==	0)
			continue;
		if	(lpAbility->m_bf1IsOFF)
			continue;		
		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];
		
		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::MiniPetSkillPowerUp)
			continue;
		
		if	(lpSkill->m_aValue[0]	==	_iSkill)
			return	lpAbility;
	}
	
	return	NULL;
}
int
cMiniPet::getPowerUpPercentage(int _iTargetSkill)
{
	int iPercent =	0;
	
	if	(m_bf7Level	==	0)
		return	NULL;
	
	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[iSkill];
		
		if	(lpAbility->m_bf4Level	==	0)
			continue;
		if	(lpAbility->m_bf1IsOFF)
			continue;		
		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];
		
		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::MiniPetSkillPowerUp)
			continue;

		if(lpSkill->m_aValue[0]  == _iTargetSkill)
		{
			iPercent += lpSkill->m_sFireDamagePercentagePerLevel * lpAbility->m_bf4Level;
			iPercent += lpSkill->m_sWaterDamagePercentagePerLevel * lpAbility->m_bf4Level;
			iPercent += lpSkill->m_sWindDamagePercentagePerLevel * lpAbility->m_bf4Level;
			iPercent += lpSkill->m_sEarthDamagePercentagePerLevel * lpAbility->m_bf4Level;
			iPercent += lpSkill->m_sLightDamagePercentagePerLevel * lpAbility->m_bf4Level;
			iPercent += lpSkill->m_sDarkDamagePercentagePerLevel * lpAbility->m_bf4Level;
		}
	}
	
	return	iPercent;
}



int
cMiniPet::getDecreaseChargeCount()
{
	cMiniPet*	lpMiniPet	=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];

	if	(lpMiniPet->m_bf7Level	==	0)
		return	0;
	
	for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&(lpMiniPet->m_aAbility[iSkill]);
		
		if	(lpAbility->m_bf4Level	==	0)
			continue;
		if	(lpAbility->m_bf1IsOFF)
			continue;		
		CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];
		
		if	(lpSkill->m_bf5MiniPetSkillType	!=	nsMiniPetST::DecreaseChargeCount)
			continue;
		
		int	iLevel	=	lpAbility->m_bf4Level;
		
		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			iLevel		+=	g_hero.m_bIncreaseMiniPetBasicSkillLevel;
		iLevel		+=	g_hero.m_bIncreaseMiniPetSkillLevel;
		iLevel		+=	g_hero.m_bCorrectMiniPetSkillLevel;
		
		return	(lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*iLevel)/100;
	}
	
	return	0;
}


char*
cMiniPet::getToolTip()
{
	s_strTooltip.Reset();
	s_strTooltip.Add("%s\n",getName());

	s_strTooltip.Add("\n");
	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_ITEM_BASIC_INFO);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_LEVEL,m_bf7Level);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_EXP,m_bf16Exp);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_SKILL_POINT,m_bf7SkillPoint);
	s_strTooltip.Add("- %s <c:LTYELLOW>%s<n>\n",dMSG_MINIPET_EVOLUTION_GRADE,g_aMiniPetTypeInfo[m_bf5Type].m_strName);

	int		iEatCount;
	char	*lpstrState;

	switch(getAttr())
	{
		case	nsMiniPetType::SpritOfFire		:
			iEatCount	=	g_hero.m_bf4EatFireMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_STRENGTH	];
			break;
		case	nsMiniPetType::SpritOfWater		:
			iEatCount	=	g_hero.m_bf4EatWaterMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_WISDOM	];
			break;
		case	nsMiniPetType::SpritOfWind		:
			iEatCount	=	g_hero.m_bf4EatWindMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_AGILITY	];
			break;
		case	nsMiniPetType::SpritOfEarth		:
			iEatCount	=	g_hero.m_bf4EatEarthMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_CONSTITUTION	];
			break;
		case	nsMiniPetType::SpritOfLight		:
			iEatCount	=	g_hero.m_bf4EatLightMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_INTELLIGENCE	];
			break;
		case	nsMiniPetType::SpritOfDark		:
			iEatCount	=	g_hero.m_bf4EatDarkMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_CHARISMA	];
			break;

	}

	s_strTooltip.Add(dMSG_MINIPET_RESTRAINT_COUNT_FOR_TT_FORM,iEatCount,lpstrState,iEatCount*dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET);

	//JBC	차광 주머니 씨앗 열매 툴팁	09-01-05
	if	(m_bf1MinipetAwaken100)
		s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MINI_PET_AWAKEN_ITEM100);
	else
	if	(m_bf1MinipetAwaken50)
		s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_MINI_PET_AWAKEN_ITEM50);

	s_strTooltip.Add("\n");

	{
		s_strTooltip.Add("<c:LTCYAN><%s><n>\n- ",dMSG_SKILL_INFO);

		int	iCount	=	0;

		for(int i=0;i<c_iMiniPetSkillCount;i++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&m_aAbility[i];

			if	(lpAbility->m_bf4Level	==	0)
				continue;

			if	(iCount)
				s_strTooltip.Add(",");

			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			s_strTooltip.Add("[%s:Lv %d]",lpSkill->m_strName,lpAbility->m_bf4Level);

			iCount++;
		}

		s_strTooltip.Add("\n\n");
	}
	
	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_MINIPET_GROW_RATE);

	int	iTotal	=	m_iVitalPoint+m_iEnergyPoint+m_iMineralPoint;
	int	iVital	=	0,iEnergy=0,iMineral=0;

	if	(iTotal)
	{
		iVital	=	((LONGLONG)m_iVitalPoint)*100/iTotal;
		iEnergy	=	((LONGLONG)m_iEnergyPoint)*100/iTotal;
		iMineral=	((LONGLONG)m_iMineralPoint)*100/iTotal;
	}

	s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_MINIPET_BIO,iVital,dMSG_PERCENTAGE);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_MINIPET_ENERGY,iEnergy,dMSG_PERCENTAGE);
	s_strTooltip.Add("- %s <c:LTYELLOW>%d%s<n>\n",dMSG_MINIPET_MINERAL,iMineral,dMSG_PERCENTAGE);

	return	s_strTooltip.String;
}
