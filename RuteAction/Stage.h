//=============================================================
// StageClass.h
//=============================================================
#pragma once
#pragma warning ( disable : 4996 )

#define SCREENTOSTATUS (float)1/1.17f
#define STATUS_Y 200
#define STATUS_X -200
#define TIMEMAX 300
#define TIMEMAX1 400
#define TIMEMAX2 800
#define TIMEMAX3 1200

#define RANKINGMAX 10

typedef enum ___Ivent_Stage{
	IVENT_STAGE_READY,
	IVENT_STAGE_START,
	IVENT_STAGE_PINCH,
	IVENT_STAGE_TIMEUP,
	IVENT_STAGE_CLEAR,
	IVENT_STAGE_PAUSE,
	IVENT_STAGE_MAX,
}e_Ivent_Stage;

//=============================================================
// include
//=============================================================
#include "Common.h"

typedef enum ___BackType {
	BACKTYPENULL,
	MORI,
	UMI,
	SORA,
	BACKTYPEMAX,
}e_BackType;


struct BACK
{
	HWND *hCmb;
	int SelectIndex;
	int Type;
	Texture *TexClass;
	TextureData *TexTable[BACKTYPEMAX];
};

//=============================================================
// StageClass
//=============================================================
class StageClass 
{
public:

	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	StageClass();
	StageClass(Renderer *R,Chara *C,CSoundManager * pSoundManager);
	
	//---------------------------------------------------------
	// 仮想デストラクタ
	//---------------------------------------------------------
	~StageClass();

	HWND *hCmb_Stage;
	HWND *hCmb_Capter;
	int SelectIndex;

	void Paint();
	void Main();
	void Input();
	Chara *Player;

	bool EditSwitch;
	BACK *Back;

	Block *CheckPointNow;
	Texture *CheckPoint_Font;
	void SetCheckPoint_Font(Texture *);
	int CheckPoint_Count_Stream;
	int CheckPoint_Count_Blink;

	BlockManager *M_Block;

	void SetStPos(float x, float y, float z);
	void SetStPos(D3DXVECTOR3 &pos){StPos = pos;}
	bool SetPlayerStart();

	int ObjectHit(Texture *,Block *);

	void GettingCard(Block *);
	void SetCardNumtex2();
	void PlayerDeath();

	D3DXVECTOR3	* GetStPos(){return &StPos;}

	bool FileSave();
	bool FileLoad();

	bool CapterSave();
	bool CapterLoad();

	bool SaveScore(int);
	bool ResetScore();

	void Animation();

	TextureData *NumberTexTable[10];
	TextureData *GageKind[2];
	int ColectedCard;
	int TotalCard;
	int TimeCount;
	int TimeCount_MAX;
	int SecondCount;
	int Pinchflg;
	Texture *CardNumtex[2];
	Texture *CardNumtex2[2];
	Texture *Tex_TimeBack;
	Texture *Tex_TimeStatic;
	Texture *Tex_TimeMove;

	Texture *Tex_Flame;
	Texture *Tex_Gage;
	Texture *Tex_PAUSECHOICE[2];
	int PauseSelect;

	void SetStatus();
	void SetWatch();

	int Count[IVENT_STAGE_MAX];
	bool Iventflg[IVENT_STAGE_MAX];
	Texture *MessageTex;
	TextureData *IventTexTable[IVENT_STAGE_MAX];
	void StageStart(int);
	void TimeAlarm(int);
	void TimeUp();
	void StageClear();
	void Pause();

	int WordCord[RANKINGMAX][3];
	int Poseflg;

private:
	D3DXVECTOR3 StPos;
	CSoundManager * m_pSoundManager;
};