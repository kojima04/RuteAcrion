//=============================================================
// Chara.h
//=============================================================
#pragma once

//=============================================================
// include
//=============================================================
#include "Texture.h"
#include "Bullet.h"

#define FRICRATE 0.98
#define GRAV 3
#define BULLETMAX 7
#define DASHGRACETIME 15
#define CAMERALANGE 200
#define RUNSPEEDLIMIT 50
#define SAITOU 4
#define DELTAVELO 0.01f
#define GAGERECOVERYCOUNTMAX 150
#define GAGECOST_THROW 30
#define GAGEDEADLINE 40

typedef enum ___CharaState {
	CHARASTATE_STAND,
	CHARASTATE_JUMP,
	CHARASTATE_FALL,
	CHARASTATE_DOWN,
	CHARASTATE_KO,
	CHARASTATE_RIDE,
	CHARASTATE_MAX,
}e_CharaState;

typedef enum ___CharaActionState {
	CHARAACTION_THROW,
	CHARAACTION_THROW_R,
	CHARAACTION_JUMP,
	CHARAACTION_WALK,
	CHARAACTION_DASH,
	CHARAACTION_DOWN,
	CHARAACTION_DENGURI,
	CHARAACTION_MABATAKI,
	CHARAACTION_KO,
//	CHARAACTION_DENGURI,
	//CHARAACTION_LANDING,
	CHARAACTION_MAX,
}e_CharaActionState;

struct AnimInfomation
{
	int	Num;
	int	Count;
	int	CountMax;
	int	Flame;
	void SetUp(int num,int countmax,int flame)
	{
		Num = num;
		CountMax = countmax;
		Flame = flame;
		Count = 0;
	}
};

class Bullet;

//=============================================================
// Chara
//=============================================================
class Chara :
	public Texture
{
public:
	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	Chara();
	Chara(std::string name,float X,float Y,float Z,float H,float W,Renderer *R,CSoundManager * m_pSoundManager);
	
	//---------------------------------------------------------
	// 仮想デストラクタ
	//---------------------------------------------------------
	~Chara();

	void Init();

public:

	Bullet *Bul[BULLETMAX];
	int SelectBullet;
	TextureData *TexTable[CHARASTATE_MAX];
	TextureData *TexTable_Throw[CHARASTATE_MAX];
	TextureData *TexTable_Action[CHARAACTION_MAX];

	float CameraWork;
	int CameraWorkCount;
	void SetCameraWork(int);

	void Input();
	void SetVeloAction();
	void InputandSetState();

	void SetJoyNum(int num){JoyNum = num;}
	void SetFricRate(float rate){FricRate = rate;} 
	float GetFricRate(){return FricRate;}
	int GetCharaState(){return CharaState;}

	void VeloCal();
	void Friction(float);
	void Gravitation();


	bool SetCharaState();
	bool Animation();
	bool Anim_Loop(int,int);
	bool Anim_Action(int,int);
	void Walk_Action();
	int Muki;
	Bullet *RideBullet;
	void Riding();
	int CharaState;//状態
	int ActionState;//状態
	int HitState;//状態
	bool Bullettest;
	void SetdownBullet();
	int JoyNum;//コントローラーのID
	int Jumpflg;
	
	bool Actionflg;
	int StandDashflg;

	float GagePoint;
	int GageRecavaryCount;
	int CatchandMoveflg;
	
private:
	CSoundManager * m_pSoundManager;
	int DashGrace;
	int Dashflg;
	int Throwflg;
	int ThrowAngle;
	Bullet * SerachBullet();

	AnimInfomation AnimInfo[CHARASTATE_MAX];
	AnimInfomation AnimInfo_Throw[CHARASTATE_MAX];
	AnimInfomation AnimInfo_Action[CHARAACTION_MAX];
	
	float FricRate;//摩擦係数

	void SetupBullet();

};
