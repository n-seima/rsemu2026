#include	"cHero.h"
#include	"cActor.h"

class	cOldFriendInfo 
{
public:
	WORD				m_wGroup;
	WORD				m_wJob;
	char				m_strName[dNAME_LENGTH];

						cOldFriendInfo()
						{
							reset();
						}

	void				reset()
	{
		m_wGroup	=	0xffff;
		m_wJob		=	0xffff;
		m_strName[0]=	0;
	}
};

class	cOLD_SAVE_CLIENT_DATA
{
public:
	WORD				m_aLeftAbility[2],m_aSubAbility[2],m_aRightAbility[2],m_aSupportAbility[2];//선택된 어빌러티
	WORD				m_aQuickSkill[10][10][2];
	int					m_iCurrentQuickSkilSlot;
	WORD				m_aActivePassiveSkill[2][dACTIVE_REACTION_COUNT];
	BOOL				m_isRunning;					//	뛰는중
	WORD				m_wIsOnSubAbility;

	cOldFriendInfo 		m_aFriend[100];	//친구
	char				m_strFriendGroup[10][dNAME_LENGTH];

	void				reset();
	inline	char*		getSaveClientDataPointer(){return	(char *)this;}
};

class	cOldSaveClientData_front
{
public:
	WORD				m_aLeftAbility[2],m_aSubAbility[2],m_aRightAbility[2],m_aSupportAbility[2];//선택된 어빌러티
	WORD				m_aQuickSkill[10][10][2];
	int					m_iCurrentQuickSkilSlot;
};

class	cOldSaveClientData_body
{
public:
	WORD				m_aActivePassiveSkill[2][2];
};

class	cOldSaveClientData_rear
{
public:
	BOOL				m_isRunning;					//	뛰는중
	WORD				m_wIsOnSubAbility;

	cOldFriendInfo		m_aFriend[dOLD_MAX_FRIEND_COUNT];	//친구
	char				m_strFriendGroup[dMAX_FRIEND_GROUP_COUNT][dNAME_LENGTH];
};

class	cOldSaveClientData	:	public cOldSaveClientData_front,public cOldSaveClientData_body,public cOldSaveClientData_rear
{
public:
};



#define	dHERO_INFO_RAND_VALUE			45024865
#define	dCHECK_CODE_OF_HERO_SAVE_DATA	0x12345678

//
//	Custom Hero Save Data
enum
{
	eCHSD_ADD_SUB_SKILL_STATUS	=	2	,
	eCHSD_RENEWEL_FRIEND_LIST			,

	eCURRENT_HERO_SAVE_DATA_VERSION	
};

void
CHero::readOldUserData(cFILE *_lpFile)
{
	char					strName[dNAME_LENGTH];
	cOLD_SAVE_CLIENT_DATA	data;

	_lpFile->Read(strName,sizeof(strName));
	_lpFile->Read(&data,sizeof(cOLD_SAVE_CLIENT_DATA));

	DWORD	dwSeedValue	=	getRandValue();

//	암호화
	{
		DecodeBuffer(dwSeedValue,(BYTE*)strName,dNAME_LENGTH);
		DecodeBuffer(dwSeedValue,(BYTE*)&data,sizeof(cOLD_SAVE_CLIENT_DATA));
	}

	memcpy(m_aLeftAbility,&m_aLeftAbility,sizeof(m_aLeftAbility));
	memcpy(m_aSubAbility,&m_aSubAbility,sizeof(m_aSubAbility));
	memcpy(m_aRightAbility,&m_aRightAbility,sizeof(m_aRightAbility));
	memcpy(m_aSupportAbility,&m_aSupportAbility,sizeof(m_aSupportAbility));
	memcpy(m_aQuickSkill,&m_aQuickSkill,sizeof(m_aQuickSkill));
	memcpy(m_aActivePassiveSkill,&m_aActivePassiveSkill,sizeof(m_aActivePassiveSkill));

	m_wCurrentQuickSkillSlot	=	data.m_iCurrentQuickSkilSlot;
	m_bf1IsRunning				=	data.m_isRunning;
	m_bf1IsSubAttackAbilityOn	=	data.m_wIsOnSubAbility;
	m_bf1IsSaveAllDataByClient	=	TRUE;

	int	iAvailFriendCount		=	0;

	for	(int i=0;i<100;i++)
	{
		if	(data.m_aFriend[i].m_wGroup	==	0)
		{
			m_aFriendList[iAvailFriendCount].m_bf1IsOnline	=	FALSE;
			m_aFriendList[iAvailFriendCount].m_bf4Group		=	0;
			m_aFriendList[iAvailFriendCount].m_bf8Job		=	data.m_aFriend[i].m_wJob;
			strcpy(m_aFriendList[iAvailFriendCount].m_strName,data.m_aFriend[i].m_strName);

			iAvailFriendCount++;

			if	(iAvailFriendCount	>=	dMAX_FRIEND_COUNT)
				break;
		}
	}
}

//
//	유저 데이터 읽기
void
CHero::readOldUserData()
{
	DWORD	dwCheckData;
	DWORD	dwDataVersion;
	char	strFileName[64];
	int		i,j;

	strcpy(strFileName,_ms("%s.ud7",m_strId));

	SetLogFolder();
	SetCurrentDirectory("data/user");

	if	(!SetCurrentDirectory(s_strSelectServerName))
	{
		SetLogFolder();
		SetCurrentDirectory("data");
		cFOLDER::Create("user",TRUE);
		cFOLDER::Create(s_strSelectServerName,TRUE);
	}

	cFILE	file;

	while(file.Open(strFileName,"rb"))
	{
		SetLogFolder();

		file.Read(&dwCheckData,4);

		if	(dwCheckData	!=	dCHECK_CODE_OF_HERO_SAVE_DATA)
		{
			file.Close();
			DeleteFile(strFileName);
			break;
		}

		file.Read(&dwDataVersion,4);

		if	(dwDataVersion	<	eCHSD_RENEWEL_FRIEND_LIST)
		{
			file.Close();
			break;
		}
/*
		if	(file.Length	!=	11394)
		{
			char				astrName[4][dNAME_LENGTH];
			cOLD_SAVE_CLIENT_DATA	aData[4];
			char				strEncodeName[dNAME_LENGTH];
			DWORD				dwLocate	=	file.Locate();
			DWORD				dwSeedValue	=	getRandValue();

			file.Read(strEncodeName,sizeof(strEncodeName));	//	마지막으로 플레이한 아바타 이름

			for	(i=0;i<4;i++)
			{
				cOldSaveClientData	data;

				file.Read(astrName[i],sizeof(astrName[i]));
				file.Read(&data,sizeof(cOldSaveClientData));

				DecodeBuffer(dwSeedValue,(BYTE*)astrName[i],dNAME_LENGTH);
				DecodeBuffer(dwSeedValue,(BYTE*)&data,sizeof(cOldSaveClientData));

				memcpy(&aData[i],data.m_aLeftAbility,sizeof(cOldSaveClientData_front));
				memcpy(&aData[i].m_isRunning,&data.m_isRunning,sizeof(cOldSaveClientData_rear));
				memset(aData[i].m_aActivePassiveSkill,0xff,sizeof(aData[i].m_aActivePassiveSkill));
			}

			file.Close();

			SetLogFolder();
			SetCurrentDirectory("data/user");
			SetCurrentDirectory(s_strSelectServerName);

			SetFileAttributes(strFileName,FILE_ATTRIBUTE_ARCHIVE);
			file.Open(strFileName,"wb");
			file.Write(&dwCheckData,4);
			file.Write(&dwDataVersion,4);
			file.Write(strEncodeName,sizeof(strEncodeName));

			for	(i=0;i<4;i++)
			{
				strcpy(m_strName,astrName[i]);
				saveUserData(&file,astrName[i],&aData[i]);
			}

			file.Close();

			file.Open(strFileName,"rb");
			file.Seek(dwLocate,SEEK_SET);

			SetLogFolder();
		}
*/
		file.Skip(sizeof(m_strName));	//	마지막으로 플레이한 아바타 이름
		file.Skip(s_iSelectAvatar*(sizeof(cOLD_SAVE_CLIENT_DATA)+sizeof(m_strName)));

		readOldUserData(&file);

		file.Close();
	}

	if	(m_wCurrentQuickSkillSlot < 0 || m_wCurrentQuickSkillSlot>=10)
		m_wCurrentQuickSkillSlot = 0;

	for (i=0;i<2;i++)
		for	(j=0;j<dACTIVE_REACTION_COUNT;j++)
			if	(getAbility(m_aActivePassiveSkill[i][j])==	NULL)
				m_aActivePassiveSkill[i][j]	=	0xffff;

	SetLogFolder();
}
