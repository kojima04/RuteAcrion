//=============================================================
// Chara.cpp
//=============================================================

//=============================================================
// include
//=============================================================
#include "Common.h"
#include "Chara.h"

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
Chara::Chara()
{
	
}

Chara::Chara(std::string name,float X,float Y,float Z,float H,float W,Renderer * R,CSoundManager * pSoundManager)
{
	m_pSoundManager = pSoundManager;

	TextureInit(name,X,Y,Z,H,W,Camera);
	for(int i = 0;i < CHARASTATE_MAX;i++)
	{
		TexTable[i] = new TextureData();
		TexTable_Throw[i] = new TextureData();
	}

	for(int i = 0;i < CHARAACTION_MAX;i++)
	{
		TexTable_Action[i] = new TextureData();
	}
	
	CameraWorkCount = 0;
	Bullettest = 0;
	CharaState = 0;
	FricRate = (float)FRICRATE;

	for(int i = 0;i < BULLETMAX;i++)
	{
		Bul[i] = new Bullet(this,i);
	}
	SelectBullet = 0;
	Actionflg = 0;
	Throwflg = 0;
	Muki = -1;
	RideBullet = NULL;
	EdgeNumber = 4;
	Jumpflg = 0;
	GagePoint = 100;
	ThrowAngle = 0;
	CatchandMoveflg = 0;

	AnimInfo[CHARASTATE_STAND].SetUp(2,0,2*8);
	AnimInfo[CHARASTATE_JUMP].SetUp(2,0,2*4);
	AnimInfo[CHARASTATE_FALL].SetUp(2,0,2*4);
	AnimInfo[CHARASTATE_RIDE].SetUp(2,0,2*2);
	AnimInfo[CHARASTATE_DOWN].SetUp(2,0,2*8);

	AnimInfo_Throw[CHARASTATE_STAND].SetUp(2,0,2*8);
	AnimInfo_Throw[CHARASTATE_JUMP].SetUp(2,0,2*8);
	AnimInfo_Throw[CHARASTATE_FALL].SetUp(2,0,2*8);
	AnimInfo_Throw[CHARASTATE_RIDE].SetUp(1,0,0);
	AnimInfo_Throw[CHARASTATE_KO].SetUp(1,0,0);

	AnimInfo_Action[CHARAACTION_THROW].SetUp(6,0,6*3);
	AnimInfo_Action[CHARAACTION_THROW_R].SetUp(3,0,3*3);
	AnimInfo_Action[CHARAACTION_JUMP].SetUp(2,0,2*4);
	AnimInfo_Action[CHARAACTION_WALK].SetUp(4,0,4*8);
	AnimInfo_Action[CHARAACTION_DASH].SetUp(4,0,4*6);
	AnimInfo_Action[CHARAACTION_DOWN].SetUp(2,0,2*4);
	AnimInfo_Action[CHARAACTION_DENGURI].SetUp(5,0,5*10);
	AnimInfo_Action[CHARAACTION_MABATAKI].SetUp(2,0,2*8);
	AnimInfo_Action[CHARAACTION_KO].SetUp(3,0,3*6);

	SetpTexturefromFile(&TexTable[CHARASTATE_STAND]->pTex,"Picture/CHARA/STAND.png");
	TexTable[CHARASTATE_STAND]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable[CHARASTATE_STAND]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable[CHARASTATE_STAND]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable[CHARASTATE_STAND]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable[CHARASTATE_JUMP]->pTex,"Picture/CHARA/JUMP_S.png");
	TexTable[CHARASTATE_JUMP]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable[CHARASTATE_JUMP]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable[CHARASTATE_JUMP]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable[CHARASTATE_JUMP]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable[CHARASTATE_FALL]->pTex,"Picture/CHARA/FALL.png");
	TexTable[CHARASTATE_FALL]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable[CHARASTATE_FALL]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable[CHARASTATE_FALL]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable[CHARASTATE_FALL]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable[CHARASTATE_RIDE]->pTex,"Picture/CHARA/RIDE.png");
	TexTable[CHARASTATE_RIDE]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable[CHARASTATE_RIDE]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable[CHARASTATE_RIDE]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable[CHARASTATE_RIDE]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable[CHARASTATE_DOWN]->pTex,"Picture/CHARA/DOWN_S.png");
	TexTable[CHARASTATE_DOWN]->SetEdgeScale(0,-(float)3/8,(float)-1/8);
	TexTable[CHARASTATE_DOWN]->SetEdgeScale(1,(float)3/8,(float)-1/8);
	TexTable[CHARASTATE_DOWN]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable[CHARASTATE_DOWN]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable[CHARASTATE_KO]->pTex,"Picture/CHARA/KO_A.png");
	TexTable[CHARASTATE_KO]->SetEdgeScale(0,-(float)3/8,(float)5/8/10);
	TexTable[CHARASTATE_KO]->SetEdgeScale(1,(float)3/8,(float)5/8/10);
	TexTable[CHARASTATE_KO]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable[CHARASTATE_KO]->SetEdgeScale(3,(float)3/8,-1);



	SetpTexturefromFile(&TexTable_Throw[CHARASTATE_STAND]->pTex,"Picture/CHARA/WINDUP_S.png");
	TexTable_Throw[CHARASTATE_STAND]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_STAND]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_STAND]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Throw[CHARASTATE_STAND]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Throw[CHARASTATE_JUMP]->pTex,"Picture/CHARA/WINDUP_S.png");
	TexTable_Throw[CHARASTATE_JUMP]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_JUMP]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_JUMP]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Throw[CHARASTATE_JUMP]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Throw[CHARASTATE_FALL]->pTex,"Picture/CHARA/WINDUP_S.png");
	TexTable_Throw[CHARASTATE_FALL]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_FALL]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_FALL]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Throw[CHARASTATE_FALL]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Throw[CHARASTATE_RIDE]->pTex,"Picture/CHARA/WINDUP_R.png");
	TexTable_Throw[CHARASTATE_RIDE]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_RIDE]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Throw[CHARASTATE_RIDE]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Throw[CHARASTATE_RIDE]->SetEdgeScale(3,(float)3/8,-1);


	SetpTexturefromFile(&TexTable_Action[CHARAACTION_THROW]->pTex,"Picture/CHARA/THROW_S.png");
	TexTable_Action[CHARAACTION_THROW]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_THROW]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_THROW]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_THROW]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Action[CHARAACTION_THROW_R]->pTex,"Picture/CHARA/THROW_R.png");
	TexTable_Action[CHARAACTION_THROW_R]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_THROW_R]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_THROW_R]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_THROW_R]->SetEdgeScale(3,(float)3/8,-1);


	SetpTexturefromFile(&TexTable_Action[CHARAACTION_JUMP]->pTex,"Picture/CHARA/JUMP_A.png");
	TexTable_Action[CHARAACTION_JUMP]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_JUMP]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_JUMP]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_JUMP]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Action[CHARAACTION_WALK]->pTex,"Picture/CHARA/WALK.png");
	TexTable_Action[CHARAACTION_WALK]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_WALK]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_WALK]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_WALK]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Action[CHARAACTION_DASH]->pTex,"Picture/CHARA/DASH.png");
	TexTable_Action[CHARAACTION_DASH]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_DASH]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_DASH]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_DASH]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Action[CHARAACTION_DOWN]->pTex,"Picture/CHARA/DOWN_A.png");
	TexTable_Action[CHARAACTION_DOWN]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_DOWN]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_DOWN]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_DOWN]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Action[CHARAACTION_DENGURI]->pTex,"Picture/CHARA/DENGURI.png");
	TexTable_Action[CHARAACTION_DENGURI]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_DENGURI]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_DENGURI]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_DENGURI]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Action[CHARAACTION_MABATAKI]->pTex,"Picture/CHARA/MABATAKI.png");
	TexTable_Action[CHARAACTION_MABATAKI]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_MABATAKI]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_MABATAKI]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_MABATAKI]->SetEdgeScale(3,(float)3/8,-1);

	SetpTexturefromFile(&TexTable_Action[CHARAACTION_KO]->pTex,"Picture/CHARA/KO_A.png");
	TexTable_Action[CHARAACTION_KO]->SetEdgeScale(0,-(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_KO]->SetEdgeScale(1,(float)3/8,(float)5/8);
	TexTable_Action[CHARAACTION_KO]->SetEdgeScale(2,-(float)3/8,-1);
	TexTable_Action[CHARAACTION_KO]->SetEdgeScale(3,(float)3/8,-1);

	StandDashflg = 0;

	SetEdgeScale(TexTable[CHARASTATE_STAND]->EdgeScale);

	Anim_Loop(CharaState,Muki);
	CameraWork = 0;
}
//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Chara::~Chara()
{
	for(int i = 0;i < CHARASTATE_MAX;i++)
	{
		delete TexTable[i];
		delete TexTable_Throw[i];
	}

	for(int i = 0;i < CHARAACTION_MAX;i++)
	{
		delete TexTable_Action[i];
	}
	for(int i = 0;i < BULLETMAX;i++)
	{
		delete Bul[i];
	}
}

void Chara::Init()
{
	SetPos(0,0,0);
	SetVelo(0,0,0);
	SetAccel(0,0,0);
	Bullettest = 0;
	CharaState = 0;
	FricRate = (float)FRICRATE;

	for(int i = 0;i < BULLETMAX;i++)
	{
		Bul[i]->Init();
	}
	SelectBullet = 0;
	Actionflg = 0;
	Throwflg = 0;
	Muki = -1;
	RideBullet = NULL;
	EdgeNumber = 4;
	Jumpflg = 0;
	GagePoint = 100;
	ThrowAngle = 0;
	CatchandMoveflg = 0;

}

void Chara::Input()
{

	if((CharaState == CHARASTATE_STAND||CharaState == CHARASTATE_DOWN)
	&& Actionflg == 0&&Throwflg == 0)
	{
		if(g_InputState[INPUTSTATE_UP])
		{
			if(CameraWorkCount >= 0)
			CameraWorkCount++;

			if(CameraWorkCount > 60)
			SetCameraWork(2);
		}
		else if(g_InputState[INPUTSTATE_DOWN])
		{
			if(CameraWorkCount <= 0)
			CameraWorkCount--;

			if(CameraWorkCount < -60)
			SetCameraWork(-2);
		}
		else
		{
			CameraWorkCount = 0;
			SetCameraWork(0);
		}
	}
	else if(CameraWork)
	{
		SetCameraWork(0);
		CameraWorkCount = 0;
	}


	//SetAccel(0,0,0);
	for(int i = 0;i < BULLETMAX;i++)
	{
		Bul[i]->SetAccel(0,0,0);
	}
	if(DashGrace > 0)
		DashGrace--;
	if(DashGrace < 0)
		DashGrace++;

	if(g_InputState[INPUTSTATE_RIGHT])
	{
		if(Throwflg == 1)
			Dashflg = 0;
		if(CharaState == CHARASTATE_STAND)
		{
			if(((!(g_ExInputState[INPUTSTATE_RIGHT]) || (StandDashflg == 1 && Throwflg != 1))&&DashGrace > 0&&ActionState != CHARAACTION_JUMP)
			 ||(Throwflg != 1&&Dashflg > 0&&ActionState != CHARAACTION_JUMP)||Velo.x > 5)
			{
				ActionState = CHARAACTION_DASH;
				Actionflg = 1;
				Dashflg = 1;
				Throwflg = - 1;
			}
			else
				if(Throwflg != 1&&ActionState != CHARAACTION_THROW&&ActionState != CHARAACTION_JUMP&&CharaState != CHARASTATE_DOWN)
			{
				ActionState = CHARAACTION_WALK;
				Actionflg = 1;
			}
		}
		else if(CharaState != CHARASTATE_DOWN)
			Accel.x = 1;
		Muki = -1;

		if(DashGrace > 0&&CharaState != CHARASTATE_STAND&&(!(g_ExInputState[INPUTSTATE_RIGHT])||StandDashflg == 1))
		{
			DashGrace = DASHGRACETIME;
			StandDashflg = 1;
		}
		else
			StandDashflg = 0;

		if(!(g_ExInputState[INPUTSTATE_RIGHT]))
		{
			DashGrace = DASHGRACETIME;
		}
	}
	else
	if(g_InputState[INPUTSTATE_LEFT])
	{
		if(Throwflg == 1)
			Dashflg = 0;
		if(CharaState == CHARASTATE_STAND)
		{
			if(((!(g_ExInputState[INPUTSTATE_LEFT]) ||( StandDashflg == -1 && Throwflg != 1))&&DashGrace < 0&&ActionState != CHARAACTION_JUMP)
				||(Throwflg != 1&&Dashflg < 0&&ActionState != CHARAACTION_JUMP)||Velo.x < -5)
			{
				ActionState = CHARAACTION_DASH;
				Actionflg = 1;
				Dashflg = -1;
				Throwflg = - 1;
			}
			else
			if(Throwflg != 1&&ActionState != CHARAACTION_THROW&&ActionState != CHARAACTION_JUMP&&CharaState != CHARASTATE_DOWN)
			{
				
				ActionState = CHARAACTION_WALK;
				Actionflg = 1;
			}
			
		}
		else if(CharaState != CHARASTATE_DOWN)
			Accel.x = -1;
		Muki = 1;

		if(DashGrace < 0&&CharaState != CHARASTATE_STAND&&(!(g_ExInputState[INPUTSTATE_LEFT])||StandDashflg == -1))
		{
			DashGrace = -DASHGRACETIME;
			StandDashflg = -1;
		}
		else
			StandDashflg = 0;

		if(!(g_ExInputState[INPUTSTATE_LEFT]))
		{
			DashGrace = -DASHGRACETIME;
		}

	}
	else
	{
		Accel.x = 0;
		if(ActionState == CHARAACTION_WALK||ActionState == CHARAACTION_DASH)
			Actionflg = 0;
	}

	


	/*if(ActionState != CHARAACTION_DASH)
		Dashflg = 0;*/

	if(g_InputState[INPUTSTATE_DOWN])
	{
		if(CharaState == CHARASTATE_STAND
			&&((Throwflg <= 0 && ActionState != CHARAACTION_THROW) 
			/*|| (!(g_InputState[INPUTSTATE_LEFT] )
			&& !(g_InputState[INPUTSTATE_RIGHT]))*/
			)
			)
		{
			ActionState = CHARAACTION_DOWN;
			SetEdgeScale(TexTable[CHARASTATE_DOWN]->EdgeScale);
			Throwflg = - 1;
			Actionflg = 1;
		}
			//CharaState = CHARASTATE_DOWN;
	}

	if(
		//CharaState == CHARASTATE_DOWN
		ActionState == CHARAACTION_DOWN
		&&(!(Velo.x > -0.1f&&Velo.x < 0.1)
		//|| ActionState == CHARAACTION_DASH || ActionState == CHARAACTION_WALK
		)
	//&&!(Velo.x > -0.1f&&Velo.x < 0.1)
	//&& Actionflg != 1
	)
	{
		if(ActionState != CHARAACTION_DENGURI)
		{
			if(Velo.x > -1.0f&&Velo.x < 1.0f)
			{
				if(Velo.x > 0)
					Velo.x = 1.0f;
				else if(Velo.x < 0)
					Velo.x = -1.0f;
				if(Velo.x == 0)
					Velo.x = -1.0f*Muki;
			}
		}
			ActionState = CHARAACTION_DENGURI;
			CharaState = CHARASTATE_DOWN;
			//Throwflg = 0;
			Actionflg = 1;
	}
	if(ActionState == CHARAACTION_DENGURI)
		Muki = -(Velo.x > 0) + (Velo.x < 0);

	if(CharaState == CHARASTATE_RIDE&&g_InputState[INPUTSTATE_DOWN]
		&& g_InputState[INPUTSTATE_JUMP]
		&& !(g_ExInputState[INPUTSTATE_JUMP]))
	{
		SetVelo(GetVelo()->x/SAITOU,GetVelo()->y/SAITOU,0);
		CharaState = CHARASTATE_FALL;
		//ActionState = CHARAACTION_JUMP;]
		Jumpflg = 1;
	}

	if(g_InputState[INPUTSTATE_JUMP]
	&& !(g_ExInputState[INPUTSTATE_JUMP])
	&& CharaState != CHARASTATE_JUMP && CharaState != CHARASTATE_FALL
	&&(HitState != HIT_UNDER||CharaState != CHARASTATE_DOWN)
	)
	{
		if(CharaState == CHARASTATE_RIDE)
			Velo.x = Velo.y = 0;
		if((ActionState == CHARAACTION_DASH || ActionState == CHARAACTION_WALK)&&Actionflg)
		{
			ActionState = -1;
			Actionflg = 0;
		}
		if(Accel.y != 100
		//&& Velo.x < 30
		&&Velo.y < 30)
		Accel.y = 100;
		CharaState = CHARASTATE_JUMP;
		DashGrace = Dashflg = 0;
		Jumpflg = 1;
		//ActionState = CHARAACTION_JUMP;
	}

	if(RideBullet)
	RideBullet->Car->SetMyVertex((Muki > 0),
								 (Muki <= 0));

	if(rand()%100 == 1)
		if(CharaState == CHARASTATE_STAND&&Throwflg != 1&&Actionflg == 0)
		{
			ActionState = CHARAACTION_MABATAKI;
			Actionflg = 1;
		}


	if(g_InputState[INPUTSTATE_JUMP]&&!(g_ExInputState[INPUTSTATE_JUMP])
		&&CharaState != CHARASTATE_DOWN
		//&&ActionState != CHARAACTION_JUMP
		&&Jumpflg != 1
		)
	{
		/*if(CheckTexHit(this,Bul[i]->Car))
		{*/
		if(SerachBullet() != NULL)
		{
			if(RideBullet->GoSignal == 0)
			{
				float ExV = sqrt(sqr(Velo.x) + sqr(Velo.y));
				float temp = sqrt(sqr(Velo.x) + sqr(Velo.y)) + 80;
				float tan = sqrt(sqr(RideBullet->RutePoint[1]->GetPos()->x - RideBullet->RutePoint[0]->GetPos()->x)
							   + sqr(RideBullet->RutePoint[1]->GetPos()->y - RideBullet->RutePoint[0]->GetPos()->y));
				RideBullet->CarDest = 1;
				RideBullet->Car->SetVelo(0,0,0);
				RideBullet->Car->SetAccel(temp*(RideBullet->RutePoint[1]->GetPos()->x - RideBullet->RutePoint[0]->GetPos()->x)/tan,
									  temp*(RideBullet->RutePoint[1]->GetPos()->y - RideBullet->RutePoint[0]->GetPos()->y)/tan,
									  0);
			}
			RideBullet->GoSignal = 1;
			CharaState = CHARASTATE_RIDE;
			D3DXVECTOR3 temp;
			temp.x = RideBullet->Car->GetVelo()->x + RideBullet->Car->GetPos()->x - GetPos()->x;
			temp.y = RideBullet->Car->GetVelo()->y + RideBullet->Car->GetPos()->y - GetPos()->y - 45;
			temp.z = 0;
			CatchandMoveflg = 1;
			SetVelo(temp);
		}
	}
	else if(g_InputState[INPUTSTATE_MOVE]&&!(g_ExInputState[INPUTSTATE_MOVE]))
	{
		for(int i = 0;i < BULLETMAX;i++)
			if(CheckTexinTex(this,Bul[i]->Car))
			{
				if(Bul[i]->GoSignal == 0)
				{
					float ExV = sqrt(sqr(Velo.x) + sqr(Velo.y));
					float temp = sqrt(sqr(Velo.x) + sqr(Velo.y))*3 + 120;
					float tan = sqrt(sqr(Bul[i]->RutePoint[1]->GetPos()->x - Bul[i]->RutePoint[0]->GetPos()->x)
								   + sqr(Bul[i]->RutePoint[1]->GetPos()->y - Bul[i]->RutePoint[0]->GetPos()->y));
					Bul[i]->CarDest = 1;
					Bul[i]->Car->SetVelo(0,0,0);
					Bul[i]->Car->SetAccel(temp*(Bul[i]->RutePoint[1]->GetPos()->x - Bul[i]->RutePoint[0]->GetPos()->x)/tan,
										  temp*(Bul[i]->RutePoint[1]->GetPos()->y - Bul[i]->RutePoint[0]->GetPos()->y)/tan,
										  0);
				}
				Bul[i]->GoSignal = 1;
		
				/*CharaState = CHARASTATE_RIDE;
				D3DXVECTOR3 temp;
				temp.x = RideBullet->Car->GetVelo()->x + RideBullet->Car->GetPos()->x - GetPos()->x;
				temp.y = RideBullet->Car->GetVelo()->y + RideBullet->Car->GetPos()->y - GetPos()->y - 45;
				temp.z = 0;
				SetVelo(temp);*/
			}
	}

	if(ActionState == CHARAACTION_DOWN && Actionflg == 1 )
		Throwflg = -1;

	
	if(Throwflg == 1 && !(g_InputState[INPUTSTATE_THROW]))
	{
		m_pSoundManager->Play("SOUND_PLAYER_THROW");
		ActionState = CHARAACTION_THROW;
		if(CharaState == CHARASTATE_RIDE)
			ActionState = CHARAACTION_THROW_R;
		Actionflg = 1;
		Throwflg = -1;

		if(g_InputState[INPUTSTATE_THROW_UPSPIN])
			ThrowAngle = 1;
		else if(g_InputState[INPUTSTATE_THROW_DOWNSPIN])
			ThrowAngle = -1;
	}

	if(Throwflg == 1 && (g_InputState[INPUTSTATE_THROW_UPSPIN])&&!(g_ExInputState[INPUTSTATE_THROW_UPSPIN]))
	{
		m_pSoundManager->Play("SOUND_PLAYER_THROW");
		ThrowAngle = 1;
		ActionState = CHARAACTION_THROW;
		if(CharaState == CHARASTATE_RIDE)
			ActionState = CHARAACTION_THROW_R;
		Actionflg = 1;
		Throwflg = -1;
	}

	if(Throwflg == 1 && (g_InputState[INPUTSTATE_THROW_DOWNSPIN])&&!(g_ExInputState[INPUTSTATE_THROW_DOWNSPIN]))
	{
		m_pSoundManager->Play("SOUND_PLAYER_THROW");
		ThrowAngle = -1;
		ActionState = CHARAACTION_THROW;
		if(CharaState == CHARASTATE_RIDE)
			ActionState = CHARAACTION_THROW_R;
		Actionflg = 1;
		Throwflg = -1;
	}


	GageRecavaryCount--;
	if(GagePoint < 100)
	{
		if(GageRecavaryCount < 0)
			GagePoint += 3.0f;
		else
			GagePoint += 0.05f;
	}
	else
		GagePoint = 100;

	if(g_InputState[INPUTSTATE_THROW]
	/*&&!(g_InputState[INPUTSTATE_THROW_UPSPIN])
	&&!(g_InputState[INPUTSTATE_THROW_DOWNSPIN])*/
	&&(!(Actionflg)||ActionState == CHARAACTION_WALK)
	&&(CharaState != CHARASTATE_DOWN)
	&&Throwflg != -1
	)
	{
		if(GagePoint > GAGEDEADLINE)
		{
			if(Throwflg != 1)
			{
				GagePoint -= GAGECOST_THROW;
				GageRecavaryCount = GAGERECOVERYCOUNTMAX;
			}
			Throwflg = 1;
			ActionState = -1;
		}
		/*else
			Throwflg = 0;*/

	}
	else 
		if(Throwflg != -1)
			Throwflg = 0;

	if(!(g_InputState[INPUTSTATE_THROW])&&Throwflg == -1)
		Throwflg = 0;

}

void Chara::SetVeloAction()
{
	
	if(ActionState == CHARAACTION_KO
	&& Actionflg == 1 
	&& AnimInfo_Action[CHARAACTION_KO].Count == AnimInfo_Action[CHARAACTION_KO].Flame/AnimInfo_Action[CHARAACTION_KO].Num*2)
	{
		CharaState = CHARASTATE_KO;
	}

	if(ActionState == CHARAACTION_DOWN
	&& Actionflg == 1 
	&& AnimInfo_Action[CHARAACTION_DOWN].Count == AnimInfo_Action[CHARAACTION_DOWN].Flame/AnimInfo_Action[CHARAACTION_DOWN].Num*1
	)
	{
		CharaState = CHARASTATE_DOWN;
	}

	if(ActionState == CHARAACTION_WALK
	&& Actionflg == 1 && CharaState == CHARASTATE_STAND
	&& RUNSPEEDLIMIT > Velo.x && -RUNSPEEDLIMIT < Velo.x)
	{
		Accel.x = float(-9*Muki);
	}

	if(ActionState == CHARAACTION_DASH
	&& Actionflg == 1 && CharaState == CHARASTATE_STAND
	&& RUNSPEEDLIMIT > Velo.x && -RUNSPEEDLIMIT < Velo.x)
	{
		Accel.x = float(-18*Muki);
	}

	/*if(ActionState == CHARAACTION_JUMP
		&& AnimInfo_Action[CHARAACTION_JUMP].Count == AnimInfo_Action[CHARAACTION_JUMP].Flame/AnimInfo_Action[CHARAACTION_JUMP].Num*0)
	{
		Accel.y = 100;
		CharaState = CHARASTATE_JUMP;
	}*/
	
	if(ActionState == CHARAACTION_THROW 
	&& AnimInfo_Action[CHARAACTION_THROW].Count == AnimInfo_Action[CHARAACTION_THROW].Flame/AnimInfo_Action[CHARAACTION_THROW].Num*3)
	{
		SetupBullet();
	}

	if(ActionState == CHARAACTION_THROW_R
	&& AnimInfo_Action[CHARAACTION_THROW_R].Count == AnimInfo_Action[CHARAACTION_THROW_R].Flame/AnimInfo_Action[CHARAACTION_THROW_R].Num*2)
	{
		SetupBullet();
	}
}

void Chara::VeloCal()
{
	for(int i = 0;i < BULLETMAX;i++)
	{
		Bul[i]->VeloCal();
		if(Bul[i]->GoSignal)
		{
			/*D3DXVECTOR3 temp;
			temp.x = -Bul[i]->Car->GetVelo()->y/2;
			temp.y = Bul[i]->Car->GetVelo()->x/2;
			float tan;
			tan = sqrt(sqr(temp.x) + sqr(temp.y));
			Bul[i]->Car->SetAccel(Bul[i]->Car->GetAccel()->x + temp.x,Bul[i]->Car->GetAccel()->y + temp.y,0);*/
			Bul[i]->Car->Acceleration();
			Bul[i]->CarVeloCal();
		}
		
	}
	
	SetAccel(Accel.x,Accel.y - GRAV,Accel.z);
	Anim_Loop(CharaState,Muki);
	if(Actionflg)
	{
		Anim_Action(ActionState,Muki);
	}
	Acceleration();
	
	Friction(GetFricRate());
	Riding();
	for(int i = 0;i < BULLETMAX;i++)
	{
		if(Bul[i]->GoSignal)
		{
			if(Bul[i]->NotMoveflg)
				Bul[i]->NotMoveflg = 0;
			else
			{
				float V = sqrt(sqr(Bul[i]->Car->GetVelo()->x ) + sqr(Bul[i]->Car->GetVelo()->y));
				/*if(V > 30)
				{
					Bul[i]->Car->SetVelo(30*Bul[i]->Car->GetVelo()->x/V,30*Bul[i]->Car->GetVelo()->y/V,0);
					V = sqrt(sqr(Bul[i]->Car->GetVelo()->x ) + sqr(Bul[i]->Car->GetVelo()->y));
				}*/
				Bul[i]->Car->Move();
			}
			//Bul[i]->ShotCount[CAR] ++;
			Bul[i]->Car->SetAccel(0,0,0);
			if(Bul[i]->ShotCount[CAR] > RUTEPOINTMAX)
			{
				/*if(Bul[i]->CarDest < RUTEPOINTMAX - 1||Bul[i]->CarDest >= 0)
				Bul[i]->Car->SetAccel(Bul[i]->Car->GetAccel()->x,Bul[i]->Car->GetAccel()->y - GRAV,Bul[i]->Car->GetAccel()->z);*/
				Bul[i]->ReflectCount[CAR] = 10;
			}
		}
	}
	
	
	/*if(ActionState == CHARAACTION_JUMP)
		ActionState = -1;*/
	if(Jumpflg == 1)
		Jumpflg = 0;
	
}

void Chara::Friction(float rate)
{
	if(CharaState != CHARASTATE_DOWN)
	//if(ActionState != CHARAACTION_DENGURI)
	{
		if(CharaState == CHARASTATE_STAND)
			Velo.x *= 0.8f;
		else
			Velo.x *= rate;
		Velo.y *= rate;
		Velo.z *= rate;
		if(Velo.y < DELTAVELO && Velo.y > -DELTAVELO)
			Velo.y = 0;
		if(Velo.x < DELTAVELO && Velo.x > -DELTAVELO)
			Velo.x = 0;
	}
}



bool Chara::Anim_Loop(int state,int Muki)
{
	if(Throwflg != 1)
	{
		SetpTexture(&TexTable[state]->pTex);
		if(state == CHARASTATE_DOWN)
		SetEdgeScale(TexTable[state]->EdgeScale);

		if(AnimInfo[state].Count < AnimInfo[state].Flame)
			AnimInfo[state].Count++;
		else
			AnimInfo[state].Count = 0;

		for(int i = 0;i < AnimInfo[state].Num;i++)
		{
			if(AnimInfo[state].Count <= AnimInfo[state].Flame/AnimInfo[state].Num*(i + 1))
			{
				SetMyVertex((Muki > 0) + (float)1/AnimInfo[state].Num*(i + 1)*(Muki > 0) - (float)1/AnimInfo[state].Num*(i + 1)*(Muki <= 0),
							(Muki > 0) + (float)1/AnimInfo[state].Num*(i)*(Muki > 0) - (float)1/AnimInfo[state].Num*(i)*(Muki <= 0));
				return 0;
			}
		}
	}
	else
	{
		SetpTexture(&TexTable_Throw[state]->pTex);

		if(AnimInfo_Throw[state].Count < AnimInfo_Throw[state].Flame)
			AnimInfo_Throw[state].Count++;
		else
			AnimInfo_Throw[state].Count = 0;


		for(int i = 0;i < AnimInfo_Throw[state].Num;i++)
		{
			if(AnimInfo_Throw[state].Count <= AnimInfo_Throw[state].Flame/AnimInfo_Throw[state].Num*(i + 1))
			{
				SetMyVertex((Muki > 0) + (float)1/AnimInfo_Throw[state].Num*(i + 1)*(Muki > 0) - (float)1/AnimInfo_Throw[state].Num*(i + 1)*(Muki <= 0),
							(Muki > 0) + (float)1/AnimInfo_Throw[state].Num*(i)*(Muki > 0) - (float)1/AnimInfo_Throw[state].Num*(i)*(Muki <= 0));
				return 0;
			}
		}
	}

	return 0;
}

bool Chara::Anim_Action(int state,int Muki)
{
	SetpTexture(&TexTable_Action[state]->pTex);

	if(AnimInfo_Action[state].Count < AnimInfo_Action[state].Flame)
		AnimInfo_Action[state].Count++;
	else
		AnimInfo_Action[state].Count = 0;

	if(AnimInfo_Action[state].Count + 1 > AnimInfo_Action[state].Flame)
	{
		Actionflg = 0;
		ActionState = -1;
	}

	for(int i = 0;i < AnimInfo_Action[state].Num;i++)
	{
		if(AnimInfo_Action[state].Count <= AnimInfo_Action[state].Flame/AnimInfo_Action[state].Num*(i + 1))
		{
			if(Muki > 0)
				SetMyVertex((float)1/AnimInfo_Action[state].Num*(i + 1),
							(float)1/AnimInfo_Action[state].Num*(i));
			else
				SetMyVertex((float)1/AnimInfo_Action[state].Num*(i),
							(float)1/AnimInfo_Action[state].Num*(i + 1));
							

			return 0;
		}
	}

	return 0;
}

bool Chara::SetCharaState()
{
	if(CharaState == CHARASTATE_FALL)
	{
		if(GetVelo()->y == 0)
		{
			CharaState = CHARASTATE_STAND;
		}
	}

	if(CharaState == CHARASTATE_JUMP)
	{
		if(GetVelo()->y <= 0)
			CharaState = CHARASTATE_FALL;
	}
	/*if(CharaState == CHARASTATE_STAND)
	{
		if(GetVelo()->y < -0.1f)
		{
			if(ActionState == CHARAACTION_DASH || ActionState == CHARAACTION_WALK)
			{
				ActionState = -1;
				Actionflg = 0;
			}
			CharaState = CHARASTATE_FALL;
		}
	}*/
	
	/*if(g_InputState[INPUTSTATE_JUMP]
	&& !(g_ExInputState[INPUTSTATE_JUMP])
	&& CharaState == CHARASTATE_STAND)
	{
		ActionState = CHARAACTION_JUMP;
		Actionflg = 1;
		DashGrace = Dashflg = 0;
	}*/

	if(HitState == HIT_UNDER&&CharaState != CHARASTATE_DOWN&&CharaState != CHARASTATE_KO)
	{
		CharaState = CHARASTATE_STAND;
	}
	else if(HitState != HIT_UNDER&&CharaState != CHARASTATE_DOWN&&CharaState != CHARASTATE_KO&&CharaState != CHARASTATE_RIDE)
	{
		if(Velo.y < -0.6f)
		{
			CharaState = CHARASTATE_FALL;
			if(ActionState == CHARAACTION_DASH || ActionState == CHARAACTION_WALK)
			{
				ActionState = -1;
				Actionflg = 0;
			}
		}
		/*else if(Velo.y > 3.4f)
		{
			CharaState = CHARASTATE_JUMP;
			if(ActionState == CHARAACTION_DASH || ActionState == CHARAACTION_WALK)
			{
				ActionState = -1;
				Actionflg = 0;
			}
		}*/
	}
		/*if(CharaState == CHARASTATE_STAND&&Actionflg == 0)
	{

		Velo.y = 0;
	}*/

	return 0;
}


void Chara::Riding()
{
	if(CharaState == CHARASTATE_RIDE)
	{
		//SetVelo(*RideBullet->Car->GetVelo());
		D3DXVECTOR3 temp;
		temp.x = RideBullet->Car->GetVelo()->x + RideBullet->Car->GetPos()->x - GetPos()->x;
		temp.y = RideBullet->Car->GetVelo()->y + RideBullet->Car->GetPos()->y - GetPos()->y - 58;
		temp.z = 0;
		SetVelo(temp);
	}
}

void Chara::SetupBullet()
{
	Bullettest = 1;
	if(Bul[SelectBullet] == RideBullet)
	if(SelectBullet == BULLETMAX - 1)
		SelectBullet = 0;
	else
		SelectBullet ++;

	Bul[SelectBullet]->SetPos(GetPos()->x,GetPos()->y + GetSize()->y*EdgeScale[0].y*5/4,GetPos()->z);
	Bul[SelectBullet]->SetVelo(0,0,0);
	//Bul[SelectBullet]->SetVelo(*GetVelo());
	Bul[SelectBullet]->SetVelo(GetVelo()->x*0.8f,GetVelo()->y*0.8f,0);
	
	Bul[SelectBullet]->SetAccel(0,0,0);
	Bul[SelectBullet]->ReflectCount[BULLET] = 0;
	Bul[SelectBullet]->ReflectCount[CAR] = 0;
	Bul[SelectBullet]->spin = 0;
	if(ThrowAngle > 0)
	{
		if(Muki > 0)
			Bul[SelectBullet]->spin = 1;
		else
			Bul[SelectBullet]->spin = -1;
	}
	if(ThrowAngle < 0)
	{
		if(Muki > 0)
			Bul[SelectBullet]->spin = -1;
		else
			Bul[SelectBullet]->spin = 1;
	}
		ThrowAngle = 0;
	
	if(g_InputState[INPUTSTATE_LEFT])
		Bul[SelectBullet]->SetAccel(-100,0,0);
	else
	if(g_InputState[INPUTSTATE_RIGHT])
		Bul[SelectBullet]->SetAccel(100,0,0);

	if(g_InputState[INPUTSTATE_UP])
		Bul[SelectBullet]->SetAccel(Bul[SelectBullet]->GetAccel()->x,100,0);
	else
	if(g_InputState[INPUTSTATE_DOWN])
		Bul[SelectBullet]->SetAccel(Bul[SelectBullet]->GetAccel()->x,-100,0);

	if(Bul[SelectBullet]->GetAccel()->x != 0&&Bul[SelectBullet]->GetAccel()->y != 0)
	{
		Bul[SelectBullet]->GetAccel()->x /= 1.41421356f;
		Bul[SelectBullet]->GetAccel()->y /= 1.41421356f;
	}
	
	if(Bul[SelectBullet]->GetAccel()->x == 0&&Bul[SelectBullet]->GetAccel()->y == 0)
		Bul[SelectBullet]->SetAccel(-(float)100*Muki,0,0);

	Bul[SelectBullet]->GoSignal = 0;
	/*Bul[SelectBullet]->Car->SetAccel(*Bul[SelectBullet]->GetAccel());
	Bul[SelectBullet]->Car->SetVelo(*GetVelo());*/

	Bul[SelectBullet]->Car->SetAccel(0,0,0);
	Bul[SelectBullet]->Car->SetVelo(0,0,0);
	Bul[SelectBullet]->ExV = -1;
	Bul[SelectBullet]->CarDest = 1;
	Bul[SelectBullet]->CheckPoint = 0;
	Bul[SelectBullet]->RuteDirection = 1;


	Bul[SelectBullet]->ShotCount[BULLET] = -10;
	Bul[SelectBullet]->ShotCount[CAR] = 0;

	Bul[SelectBullet]->Bectle->SetPos(Bul[SelectBullet]->Car->GetPos()->x,Bul[SelectBullet]->Car->GetPos()->y,0);
	Bul[SelectBullet]->Bectle->SetRot(0,0,
								Bul[SelectBullet]->GetAngle(
								Bul[SelectBullet]->GetAccel()->x/Bul[SelectBullet]->GetMass() + Bul[SelectBullet]->GetVelo()->x*0.8f ,
								Bul[SelectBullet]->GetAccel()->y/Bul[SelectBullet]->GetMass() + Bul[SelectBullet]->GetVelo()->y*0.8f) - (float)PAI/2);

}

void Chara::SetdownBullet()
{
	Bul[SelectBullet]->SetPos(1000,1000,100);
	Bul[SelectBullet]->SetVelo(1000,1000,100);
	m_pSoundManager->Play("SOUND_PLAYER_METAL");
	GagePoint += GAGECOST_THROW/3*2;

}

void Chara::SetCameraWork(int state)
{
	if(CameraWork + state < CAMERALANGE&&CameraWork + state > - CAMERALANGE)
		CameraWork += state;
	if(state == 0)
	{
		if(CameraWork > 0)
		{
			CameraWork -= 3;
			if(CameraWork + 3 > 0&&CameraWork < 0)
				CameraWork = 0;
		}
		if(CameraWork < 0)
		{
			CameraWork += 3;
			if(CameraWork - 3 < 0&&CameraWork > 0)
				CameraWork = 0;
		}
	}
		
}

Bullet * Chara::SerachBullet()
{
	RideBullet = NULL;
	if(Velo.y > 0)
	Move();
	for(int i = 0;i < BULLETMAX;i++)
	{
		if(CheckTexinTex(this,Bul[i]->Car))
		{
			if(RideBullet == NULL)
				RideBullet = Bul[i];
			else
			{
				bool LV,LV2;
				if(RideBullet->Car->GetPos()->y + RideBullet->Car->GetPos()->y - GetPos()->y - GetVelo()->y - 45 >= 0)
					LV = 1;
				else
					LV = 0;

				if(Bul[i]->Car->GetPos()->y + Bul[i]->Car->GetPos()->y - GetPos()->y - GetVelo()->y - 45 >= 0)
					LV2 = 1;
				else
					LV2 = 0;


				//RideBullet->Car->GetVelo()->y + RideBullet->Car->GetPos()->y - GetPos()->y - 45

				float L1,L2;

				if(GetVelo()->y < 0)
				{
					L1 = sqrt(sqr(RideBullet->Car->GetPos()->x - GetPos()->x - GetVelo()->x)
								  + sqr(RideBullet->Car->GetPos()->y + RideBullet->Car->GetVelo()->y - Pos.y -Velo.y -45));
					L2 = sqrt(sqr(Bul[i]->Car->GetPos()->x - GetPos()->x - GetVelo()->x)
								  + sqr(Bul[i]->Car->GetPos()->y + Bul[i]->Car->GetVelo()->y - Pos.y -Velo.y -45));
					if(RideBullet->Car->GetPos()->y > Bul[i]->Car->GetPos()->y)
						int p = 0;
					else
						int o = 0;
				}	
				else
				{
					L1 = sqrt(sqr(RideBullet->Car->GetPos()->x - GetPos()->x - GetVelo()->x)
								  + sqr(RideBullet->Car->GetPos()->y + RideBullet->Car->GetVelo()->y - Pos.y - Velo.y*100 -45));
					L2 = sqrt(sqr(Bul[i]->Car->GetPos()->x - GetPos()->x - GetVelo()->x)
								  + sqr(Bul[i]->Car->GetPos()->y + Bul[i]->Car->GetVelo()->y - Pos.y - Velo.y*100 -45));
					if(RideBullet->Car->GetPos()->y > Bul[i]->Car->GetPos()->y)
						int p = 0;
					else
						int o = 0;
				}
			/*	if(LV2 > LV)
					RideBullet = Bul[i];

				if(L1 > L2&&LV == LV2)
					RideBullet = Bul[i];*/

				if(L1 > L2)
					RideBullet = Bul[i];
			}
		}
	}
	if(Velo.y > 0)
	MoveCancel();
		/*{
			//RideBullet = Bul[i];

			
			
		}*/
	return RideBullet;
}
