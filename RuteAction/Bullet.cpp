//=============================================================
// Bullet.cpp
//=============================================================

//=============================================================
// include
//=============================================================
#include "Common.h"

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
Bullet::Bullet()
{
	
}

Bullet::Bullet(Chara *P,int ID)
{
	NotMoveflg = 0;
	TextureInit("Picture/BULLET/TAMA.png",1000,-1000,P->GetPos()->z,60,60,P->Camera);
	/*for(int i = 0;i < EdgeNumber;i++)
	{
		EdgeScale[i].x = P->EdgeScale[i].x/P->GetSize()->x*Size.x;
		EdgeScale[i].y = P->EdgeScale[i].x/P->GetSize()->x*Size.x;
		if(EdgeScale[i].y*P->EdgeScale[i].x < 0)
			EdgeScale[i].y = - EdgeScale[i].y;
	}*/
	for(int i = 0;i < EdgeNumber;i++)
	EdgeScale[i] /= 2;
	SetPos(10000,-10000,0);
	SetVelo(0,0,0);
	SetAccel(0,0,0);
	Player = P;
	Mass = 10;
	CarDest = 1;
	CheckPoint = 0;
	RuteDirection = 1;

	ReflectCount[0] = ReflectCount[1] = 0;
	ShotCount[0] = ShotCount[1] = 0;
	GoSignal = 0;
	//Car = new Texture("Picture/BULLET/kassha.png",10000,-10000,P->GetPos()->z,35,35,P->Camera);
	Car = new Texture("Picture/BULLET/kassha.png",10000,-10000,P->GetPos()->z,120,120,P->Camera);
	for(int i = 0;i < Car->EdgeNumber;i++)
	{
		Car->EdgeScale[i].x /= 50/35;
		Car->EdgeScale[i].y /= 50/35;
	}
	Car->SetMass(10);
	Car->EdgeNumber = 4;
	ExV = -1;
	CreanCount = 0;
	for(int i = 0;i < RUTEPOINTMAX;i++)
	{
		RutePoint[i] = new Texture("Picture/NULL.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);
	}

	switch(ID)
	{
			
		case 0:Bectle = new Texture("Picture/BULLET/yajirushi_r.png",10000,-10000,P->GetPos()->z,100,100,P->Camera);break;
		case 1:Bectle = new Texture("Picture/BULLET/yajirushi_blown.png",10000,-10000,P->GetPos()->z,100,100,P->Camera);break;
		case 2:Bectle = new Texture("Picture/BULLET/yajirushi_y.png",10000,-10000,P->GetPos()->z,100,100,P->Camera);break;
		case 3:Bectle = new Texture("Picture/BULLET/yajirushi_g.png",10000,-10000,P->GetPos()->z,100,100,P->Camera);break;
		case 4:Bectle = new Texture("Picture/BULLET/yajirushi_blue.png",10000,-10000,P->GetPos()->z,100,100,P->Camera);break;
		case 5:Bectle = new Texture("Picture/BULLET/yajirushi_navy.png",10000,-10000,P->GetPos()->z,100,100,P->Camera);break;
		case 6:Bectle = new Texture("Picture/BULLET/yajirushi_v.png",10000,-10000,P->GetPos()->z,100,100,P->Camera);break;
	}
	for(int i = 0;i < RUTEPOINTMAX - 1;i++)
	{
		switch(ID)
		{
			case 0:RuteRibon[i] = new Texture("Picture/BULLET/RuteRibon1.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);break;
			case 1:RuteRibon[i] = new Texture("Picture/BULLET/RuteRibon2.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);break;
			case 2:RuteRibon[i] = new Texture("Picture/BULLET/RuteRibon3.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);break;
			case 3:RuteRibon[i] = new Texture("Picture/BULLET/RuteRibon4.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);break;
			case 4:RuteRibon[i] = new Texture("Picture/BULLET/RuteRibon5.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);break;
			case 5:RuteRibon[i] = new Texture("Picture/BULLET/RuteRibon6.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);break;
			case 6:RuteRibon[i] = new Texture("Picture/BULLET/RuteRibon7.png",10000,-10000,P->GetPos()->z,5,5,P->Camera);break;
		}
		RuteRibon[i]->SetAlfa(100);
		RuteRibon[i]->SetMyVertex(1,0);
	}
	Muki_Car = 1;
}
//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Bullet::~Bullet()
{
	delete Bectle;
	delete Car;
	for(int i = 0;i < RUTEPOINTMAX;i++)
	{
		delete RutePoint[i];
	}
	for(int i = 0;i < RUTEPOINTMAX - 1;i++)
	{
		delete RuteRibon[i];
	}
}

void Bullet::Init()
{
	NotMoveflg = 0;
	SetPos(10000,-10000,0);
	SetVelo(0,0,0);
	SetAccel(0,0,0);
	Mass = 10;
	CarDest = 1;
	CheckPoint = 0;
	RuteDirection = 1;

	ReflectCount[0] = ReflectCount[1] = 0;
	ShotCount[0] = ShotCount[1] = 0;
	GoSignal = 0;
	Car->SetPos(10000,-10000,0);
	Car->SetVelo(0,0,0);
	Car->SetAccel(0,0,0);
	Car->SetMass(10);
	ExV = -1;
	for(int i = 0;i < RUTEPOINTMAX;i++)
	{
		RutePoint[i]->SetPos(10000,-10000,0);
		RutePoint[i]->SetVelo(0,0,0);
		RutePoint[i]->SetAccel(0,0,0);
	}
	for(int i = 0;i < RUTEPOINTMAX - 1;i++)
	{
		RuteRibon[i]->SetPos(10000,-10000,0);
		RuteRibon[i]->SetVelo(0,0,0);
		RuteRibon[i]->SetAccel(0,0,0);
	}
	
	Muki_Car = 1;
}

void Bullet::VeloCal()
{
	D3DXVECTOR3 vec1,vec2,temp;
	temp.x = temp.y = 0;
	if(ShotCount[BULLET] < RUTEPOINTMAX&&spin)
	{
		if(spin == 1)
		{
			temp.x = Velo.y;
			temp.y = -Velo.x;
		}
		if(spin == -1)
		{
			temp.x = -Velo.y;
			temp.y = Velo.x;
		}
		SetAccel(Accel.x + temp.x,Accel.y + temp.y,0);
	}
	
	Acceleration();
	if(Velo.x != 0||Velo.y != 0)
	{
		SetAngle();
		SetRute();
	}
	
	//Bectle->SetPos(Car->GetPos()->x,Car->GetPos()->y,-1);
	//if(Car->GetVelo()->x != 0&&Car->GetVelo()->y != 0)
	//Bectle->SetRot(0,0,
	//						GetAngle(Car->GetVelo()->x,Car->GetVelo()->y) - PAI/2);
}
bool Bullet::CarVeloCal()
{
	if(CarDest < RUTEPOINTMAX - 1&&CarDest >= 0)
	{

	D3DXVECTOR3 vec1,vec2,v;
	
	vec1.x = (RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x);
	vec1.y = (RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y);
	vec2.x = vec1.y;
	vec2.y = -1*vec1.x;
	float s,t;
	if(vec1.y > 0)
		s = vec1.y;
	else
		s = -vec1.y;

	if(vec2.y > 0)
		t = vec2.y;
	else
		t = -vec2.y;
	v.y = -GRAV/Car->GetMass()*s/(s + t);
	v.x = v.y * vec1.x /vec1.y;

	//v.y = v.x = 0;

	if(vec1.y == 0)
	{
		v.y = v.x = 0;
	}
	
	float temp = sqrt(sqr(v.x) + sqr(v.y));
	float V = sqrt(sqr(Car->GetVelo()->x ) + sqr(Car->GetVelo()->y));
	/*if(V > 30)
		V = 30;*/

	float DistoDest = sqrt(sqr(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x) + sqr(RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y));
	
		if(
		Car->GetVelo()->x*(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x) < 0
		&& 
		Car->GetVelo()->y*(RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y) < 0
		&&
		ExV == -1)
		{
			Car->SetVelo(-V*(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x)/DistoDest,
					 -V*(RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y)/DistoDest,
					 0);
		}
		else
		{
			if(ExV != -1)
			{
				V = ExV;
				ExV = -1;
			}


			Car->SetVelo(V*(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x)/DistoDest,
						 V*(RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y)/DistoDest,
						 0);
		}
		Car->SetVelo(Car->GetVelo()->x + v.x,Car->GetVelo()->y + v.y,0);
		
		V = sqrt(sqr(Car->GetVelo()->x ) + sqr(Car->GetVelo()->y));
		/*if(V > 30)
		V = 30;*/

		if(V > DistoDest)
		{
			if(CarDest + 1*RuteDirection >= RUTEPOINTMAX||CarDest + 1*RuteDirection < 0)
			{
				CarDest += 1*RuteDirection;
				return 1;
			}
			/*Car->SetVelo(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x,
						 RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y,
						 0);*/
			//SetRuteDirection();
			CheckPoint = CarDest;
		
			ExV = V;
			/*Car->SetPos(RutePoint[CarDest]->GetPos()->x - Car->GetVelo()->x,
						RutePoint[CarDest]->GetPos()->y - Car->GetVelo()->y,
						0);*/
			if(CarDest + 1*RuteDirection >= 0&&CarDest + 1*RuteDirection < RUTEPOINTMAX - 1)
			Car->SetVelo(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x,
						RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y,
						0);

		//	NotMoveflg = 1;
			if(CarDest < RUTEPOINTMAX)
			{
				CarDest += 1*RuteDirection;
			}
			return 1;
		}
		if(CarDest < RUTEPOINTMAX - 1&&CarDest >= 0)
		{
			//目標の点とその前の点との距離
			float p = sqrt(sqr(RutePoint[CarDest]->GetPos()->x - RutePoint[CarDest - 1*RuteDirection]->GetPos()->x) 
						 + sqr(RutePoint[CarDest]->GetPos()->y - RutePoint[CarDest - 1*RuteDirection]->GetPos()->y));
			/*DistoDest = sqrt(sqr(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x + Car->GetVelo()->x)
						   + sqr(RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y + Car->GetVelo()->y));*/
			//現在地と目標とする点との距離
			DistoDest = sqrt(sqr(RutePoint[CarDest]->GetPos()->x - Car->GetPos()->x - Car->GetVelo()->x)
						   + sqr(RutePoint[CarDest]->GetPos()->y - Car->GetPos()->y - Car->GetVelo()->y));

			if(DistoDest > p)
			{
				CheckPoint = CarDest - 1*RuteDirection;
				ExV = V;
				if(CarDest - 2*RuteDirection >= 0&&CarDest - 2*RuteDirection < RUTEPOINTMAX - 1)
				Car->SetVelo(RutePoint[CarDest - 1*RuteDirection]->GetPos()->x - Car->GetPos()->x,
							RutePoint[CarDest - 1*RuteDirection]->GetPos()->y - Car->GetPos()->y,
							0);

				/*Car->SetPos(RutePoint[CarDest - 1*RuteDirection]->GetPos()->x - Car->GetVelo()->x,
							RutePoint[CarDest - 1*RuteDirection]->GetPos()->y - Car->GetVelo()->y,
							0);*/
				//NotMoveflg = 1;
				CarDest -= 2*RuteDirection;
				RuteDirection = -RuteDirection;
				return 1;
		
			}
		}
	}
	else
	{
		Car->SetVelo(Car->GetVelo()->x,Car->GetVelo()->y  - GRAV/Car->GetMass(),0);
		if(ExV != -1)
		{
			float V = ExV;
			ExV = -1;
			Car->SetVelo(V*Car->GetVelo()->x/sqrt(sqr(Car->GetVelo()->x) + sqr(Car->GetVelo()->y)),
						 V*Car->GetVelo()->y/sqrt(sqr(Car->GetVelo()->x) + sqr(Car->GetVelo()->y)),
						0);
		}
		//Car->SetVelo(Car->GetVelo()->x,Car->GetVelo()->y  - GRAV/Car->GetMass(),0);
	}	
	return 0;
}

void Bullet::SetRuteDirection()
{

	if(CarDest == CheckPoint)
		RuteDirection = -RuteDirection;
}

void Bullet::SetRute()
{
	if(ShotCount[BULLET] == 0)
	{
		Car->SetMyVertex((GetVelo()->x <= 0),
						 (GetVelo()->x > 0));
		Car->SetPos(*GetPos());
		RutePoint[0]->SetPos(*GetPos());
	}
	for(int i = 1;i < RUTEPOINTMAX;i++)
		if(ShotCount[BULLET] == i)
		{
			RutePoint[i]->SetPos(*GetPos());
			SetRuteRibon(i);
		}
		ShotCount[BULLET] ++;
		if(ShotCount[BULLET] == RUTEPOINTMAX)
			ReflectCount[BULLET] = 100;

		if(CarDest > RUTEPOINTMAX - 2||CarDest < 0)
			CreanRute();

}

void Bullet::SetRuteRibon(int i)
{
	float L = sqrt(sqr(RutePoint[i]->GetPos()->x - RutePoint[i - 1]->GetPos()->x) 
				 + sqr(RutePoint[i]->GetPos()->y - RutePoint[i - 1]->GetPos()->y) );
	RuteRibon[i - 1]->SetPos((RutePoint[i]->GetPos()->x + RutePoint[i - 1]->GetPos()->x)/2,
						 (RutePoint[i]->GetPos()->y + RutePoint[i - 1]->GetPos()->y)/2,
						  0);
	RuteRibon[i - 1]->SetSize(L,RuteRibon[i - 1]->GetSize()->y,RuteRibon[i - 1]->GetSize()->z);
	RuteRibon[i - 1]->SetAlfa(100);
	float angle = acos((RutePoint[i]->GetPos()->x - RutePoint[i - 1]->GetPos()->x)/L);
	if(RutePoint[i]->GetPos()->y - RutePoint[i - 1]->GetPos()->y < 0)
		angle = -angle;
	RuteRibon[i - 1]->SetRot(0,0,angle);
	//RuteRibon[i]->SetMyVertex(0,1);
}

void Bullet::SetAngle()
{
	float L = sqrt(sqr(GetVelo()->x) + sqr(GetVelo()->y) );

	float angle = acos((GetVelo()->x)/L);
	if(GetVelo()->x < 0)
	{
		angle = -acos(-(GetVelo()->x)/L);
		SetMyVertex(0,1);
	}
	else
		SetMyVertex(1,0);

	if(GetVelo()->y < 0)
		angle = -angle;
	/*if(GetVelo()->x < 0)
	{
		
	}*/
	SetRot(0,0,angle);
}
float Bullet::GetAngle(float X,float Y)
{
	float L = sqrt(sqr(X) + sqr(Y) );

	float angle = acos((X)/L);
	/*if(X< 0)
	{
		angle = -acos(-(X)/L);
	}*/

	if(Y < 0)
		angle = -angle;
	if(X == 0)
		angle = (float)(((Y > 0) - (Y < 0))*PAI/2);

	return angle;
}
void Bullet::CreanRute()
{
	for(int i = 0;i < RUTEPOINTMAX - 1;i++)
		if(RuteRibon[i]->GetAlfa() > 0)
		RuteRibon[i]->SetAlfa(RuteRibon[i]->GetAlfa() - 1);
}