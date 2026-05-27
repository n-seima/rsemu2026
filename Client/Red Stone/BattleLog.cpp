#include	"BattleLog.h"
#include	"CSkill.h"
#include	"cACTOR.H"

BattleLog	g_BattleLog;

void
BattleLog::Init()
{
	char szFolder[] = {"C:\\BattleLog.txt"};

	m_hFile = CreateFile(szFolder,GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);

}

void
BattleLog::WriteLog(char* _strAttackerName , char* _strDefenderName , CSkill* _lpSkill , int _iPhysicalDamage , int _iMagicDamage, int _iAttackClass)
{//마지막 수정일 : 09.09.23
	if(!m_bIsActive)
		return;

	char			szWriteBuffer[1024*4];
	DWORD			dwRead;
	SYSTEMTIME		timeinfo;	
	char			strHitClass[1024];

	ZeroMemory(&strHitClass , sizeof(strHitClass));
	ZeroMemory(&timeinfo , sizeof(timeinfo));
	ZeroMemory(szWriteBuffer , sizeof(szWriteBuffer));
	GetLocalTime(&timeinfo);
	
	switch(_iAttackClass)
	{
	case BattleLog::eATTACK_NORMAL :
		strcpy(strHitClass, "일반");
		break;
	case BattleLog::eATTACK_MISS :
		strcpy(strHitClass, "빗나감");
		break;
	case BattleLog::eATTACK_DODGE :
		strcpy(strHitClass, "피함");
		break;
	case BattleLog::eATTACK_BLOCK :
		strcpy(strHitClass, "막음");
		break;
	case BattleLog::eATTACK_CRUSH :
		strcpy(strHitClass, "결정타");
		break;
	case BattleLog::eATTACK_CRITICAL :
		strcpy(strHitClass, "치명타");
		break;
	case BattleLog::eATTACK_DOUBLE_CRITICAL :
		strcpy(strHitClass, "(운)치명타");
		break;
	case BattleLog::eATTACK_LUCKY_HIT_OR_DAMAGE:
		strcpy(strHitClass, "(운)일반");
		break;
	case BattleLog::eATTACK_LUCKY_DODGE:
		strcpy(strHitClass, "(운)피함");
		break;
	case BattleLog::eATTACK_LUCKY_DODGE_CRITICAL:
		strcpy(strHitClass, "(운)치명타피함");
		break;
	case BattleLog::eATTACK_HARD_BLOW:
		strcpy(strHitClass, "강타");
		break;	
	default:
		strcpy(strHitClass, "???");
		break;
	}

	sprintf(szWriteBuffer, "공격 : %s\t 수비 : %s\t 스킬 : %s\t 공격상태 : %s\t 물리대미지 : %d\t 마법대미지 : %d\t 시간 : %d-%d-%d %d:%d:%d",
		_strAttackerName, _strDefenderName,	_lpSkill->m_strName, strHitClass, _iPhysicalDamage, _iMagicDamage, 
		timeinfo.wYear, timeinfo.wMonth , timeinfo.wDay , timeinfo.wHour , timeinfo.wMinute, timeinfo.wSecond);

	char	szEnter[2];

	szEnter[0] = 13;
	szEnter[1] = 10;

	WriteFile(m_hFile, szWriteBuffer, strlen(szWriteBuffer),&dwRead, NULL);
	WriteFile(m_hFile, szEnter, 2,&dwRead, NULL);
}

void
BattleLog::showDamage(char* _strAttackerName , char* _strDefenderName , CSkill* _lpSkill , int _iPhysicalDamage , int _iMagicDamage, int _iAttackClass, BOOL _bIsMonster)
{ 
	if(!m_bIsShowDamage)
		return;

	if(_bIsMonster && !m_bIsShowMonsterDamage)
	{
		return;
	}
	char	strHitClass[256] = "";
		switch(_iAttackClass)
		{
		case BattleLog::eATTACK_NORMAL :
			strcpy(strHitClass, "일반");
			break;
		case BattleLog::eATTACK_MISS :
			strcpy(strHitClass, "빗나감");
			break;
		case BattleLog::eATTACK_DODGE :
			strcpy(strHitClass, "피함");
			break;
		case BattleLog::eATTACK_BLOCK :
			strcpy(strHitClass, "막음");
			break;
		case BattleLog::eATTACK_CRUSH :
			strcpy(strHitClass, "결정타");
			break;
		case BattleLog::eATTACK_CRITICAL :
			strcpy(strHitClass, "치명타");
			break;
		case BattleLog::eATTACK_DOUBLE_CRITICAL :
			strcpy(strHitClass, "(운)치명타");
			break;
		case BattleLog::eATTACK_LUCKY_HIT_OR_DAMAGE:
			strcpy(strHitClass, "(운)일반");
			break;
		case BattleLog::eATTACK_LUCKY_DODGE:
			strcpy(strHitClass, "(운)피함");
			break;
		case BattleLog::eATTACK_LUCKY_DODGE_CRITICAL:
			strcpy(strHitClass, "(운)치명타피함");
			break;
		case BattleLog::eATTACK_HARD_BLOW:
			strcpy(strHitClass, "강타");
			break;
		default:
			strcpy(strHitClass, "???");
			break;
		}
		
		CGamePlay::AddSystemMessage(LTGREEN,"[%-10s -> %-10s] [스킬 : %-12s][%s] [피해 : %d / %d] ", _strAttackerName, _strDefenderName,_lpSkill->m_strName,strHitClass,_iPhysicalDamage,_iMagicDamage);
}
void
BattleLog::WriteExpLog(int _iExp)
{//마지막 수정일 : 09.09.23
	if(!m_bIsActive)
		return;
	
	char	szWriteBuffer[1024*4];
	DWORD	dwRead;
	SYSTEMTIME		timeinfo;	
	
	ZeroMemory(&timeinfo , sizeof(timeinfo));
	ZeroMemory(szWriteBuffer , sizeof(szWriteBuffer));
	GetLocalTime(&timeinfo);
	
	sprintf(szWriteBuffer , "경험치 획득:%d\t시간:%d-%d-%d\t%d:%d:%d" , _iExp,
		timeinfo.wYear,timeinfo.wMonth , timeinfo.wDay , timeinfo.wHour , timeinfo.wMinute, timeinfo.wSecond);
	
	char	szEnter[2];
	
	szEnter[0] = 13;
	szEnter[1] = 10;
	
	WriteFile(m_hFile, szWriteBuffer, strlen(szWriteBuffer),&dwRead, NULL);
	WriteFile(m_hFile, szEnter, 2,&dwRead, NULL);

}
