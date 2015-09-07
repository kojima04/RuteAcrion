//=============================================================
// Bullet.h
//=============================================================
#pragma once

//=============================================================
// include
//=============================================================
#include "Texture.h"
#include "Chara.h"

#define RUTEPOINTMAX 30
#define BULLET 0
#define CAR 1

class Chara;
//=============================================================
// Bullet
//=============================================================
class Bullet :
	public Texture
{
public:

	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	Bullet();
	Bullet(Chara *,int);
	
	//---------------------------------------------------------
	// 仮想デストラクタ
	//---------------------------------------------------------
	~Bullet();

	void Init();

	Texture *Car;
	Texture *Bectle;
	int CarDest;
	int RuteDirection;
	int CheckPoint;
	void SetRuteDirection();
	
	//D3DXVECTOR3
	Texture *RutePoint[RUTEPOINTMAX];
	Texture *RuteRibon[RUTEPOINTMAX - 1];
	void SetRuteRibon(int);
	int ReflectCount[2];
	int ShotCount[2];
	int spin;
	bool GoSignal;
	void SetRute();
	Chara *Player;
	void VeloCal();
	bool CarVeloCal();
	void TraceRute();
	bool NotMoveflg;
	float ExV;
	int Muki_Car;
	void SetAngle();
	float GetAngle(float,float);
	float CreanCount;
	void CreanRute();
private:
};
 
