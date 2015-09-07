



#include "Common.h"
#include "CheckHit.h"

//---------------------------------------------------------------------------
//	二つの線分が重なっているかどうか判定
//---------------------------------------------------------------------------
bool CrossCheckLine(D3DXVECTOR3 *L1edge1,D3DXVECTOR3 *L1edge2,D3DXVECTOR3 *L2edge1,D3DXVECTOR3 *L2edge2)
{

	//	Line(L1edge1->x,L1edge1->y)-(L1edge2->x,L1edge2->y)とLine(L2edge1->x,L2edge1->y)-(L2edge2->x,L2edge2->y)が重なってるかどうか判定
	//	そして、無事に取得できたならば、(x,y)に代入する
	//
	//	例：if( CrossCheckLine( L1edge1->x, L1edge1->y, L1edge2->x, L1edge2->y, L2edge1->x, L2edge1->y, L2edge2->x, L2edge2->y, &x, &y ) ){ MsgBox("当たった");}

	//	Line(L1edge1->x,L1edge1->y)-(L1edge2->x,L1edge2->y)とLine(L2edge1->x,L2edge1->y)-(L2edge2->x,L2edge2->y)が重なってるかどうか判定
	//	そして、無事に取得できたならば、(x5,y5)に代入する
	//
	//	例：if( CrossCheckLine( L1edge1->x, L1edge1->y, L1edge2->x, L1edge2->y, L2edge1->x, L2edge1->y, L2edge2->x, L2edge2->y, &x, &y ) ){ MsgBox("当たった");}

	float a, b, c, d, e, f;
	float x, y;

	//	L1edge1->xとL1edge2->xが一致しているときは、y軸に平行な直線
	if( L1edge1->x == L1edge2->x ){
		a = 1, b = 0, c = ( float )L1edge1->x;
	}else{
		a = ( ( float )( L1edge2->y - L1edge1->y ) ) / ( ( float )( L1edge2->x - L1edge1->x ) );
		b = -1;
		c = ( ( float )( L1edge2->y - L1edge1->y ) ) / ( ( float )( L1edge2->x - L1edge1->x ) ) * ( float ) L1edge1->x - ( float ) L1edge1->y;
	}

	//	上と同様
	if( L2edge1->x == L2edge2->x ){
		d = 1, e = 0, f = ( float )L2edge1->x;
	}else{
		d = ( ( float )( L2edge2->y - L2edge1->y ) ) / ( ( float )( L2edge2->x - L2edge1->x ) );
		e = -1;
		f = ( ( float )( L2edge2->y - L2edge1->y ) ) / ( ( float )( L2edge2->x - L2edge1->x ) ) * ( float ) L2edge1->x - ( float ) L2edge1->y;
	}

	//	解なしor解が不定
	if( a * e == b * d  ){
		//	３つの座標が同一直線上に存在しない条件（証明は簡単だけど、めんどくさいので略）
		if( L1edge1->x * L1edge1->y + L1edge2->x * L1edge2->y + L2edge1->x * L2edge1->y != L1edge1->x * L1edge2->y + L1edge2->x * L2edge1->y + L2edge1->x * L1edge1->y ){
			return 0;	//	解なし
		}
		//	解が不定の時は、とりあえず全ての座標の中点を交点にする
		x = ( L1edge1->x + L1edge2->x + L2edge1->x + L2edge2->x ) / 4, y = ( L1edge1->y + L1edge2->y + L2edge1->y + L2edge2->y ) / 4;
	}else{
		//	解が一つで、直線同士の交点が求められたが・・・
		x = ( c * e - b * f ) / ( a * e - b * d );
		y = ( c * d - a * f ) / ( b * d - a * e );
	}

	//	↑が線分上で交差しているとは限らないので、以下ではその判定を行う

	//	x座標が正しい位置かどうか判定
	if( L1edge1->x <= L1edge2->x ){
		if( L1edge1->x > x || x > L1edge2->x ) 
			return 0;	//	線分上で一致しなかった（以下同様）
	}else{
		if( L1edge2->x > x || x > L1edge1->x ) 
			return 0;
	}

	if( L2edge1->x <= L2edge2->x ){
		if( L2edge1->x > x || x > L2edge2->x ) 
			return 0;
	}else{
		if( L2edge2->x > x || x > L2edge1->x ) 
			return 0;
	}

	//	y座標が正しい位置かどうか判定
	if( L1edge1->y <= L1edge2->y ){
		if( L1edge1->y > y || y > L1edge2->y ) 
			return 0;
	}else{
		if( L1edge2->y > y || y > L1edge1->y ) 
			return 0;
	}

	if( L2edge1->y <= L2edge2->y ){
		if( L2edge1->y > y || y > L2edge2->y ) 
			return 0;
	}else{
		if( L2edge2->y > y || y > L2edge1->y ) 
			return 0;
	}

	return 1;
}

int CheckHitLine(Texture *tex,D3DXVECTOR3 *edge1,D3DXVECTOR3 *edge2,int state)
{
	D3DXVECTOR3 here1,here2;
	D3DXVECTOR3 dest1,dest2;

	if(
		(tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[2].y + tex->GetVelo()->y < edge1->y
	&&	 tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[2].y + tex->GetVelo()->y > edge2->y)
	||
		(tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[0].y + tex->GetVelo()->y < edge1->y
	&&	 tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[0].y + tex->GetVelo()->y > edge2->y)
	||
		(tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[2].y + tex->GetVelo()->y < edge2->y
	&&	 tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[0].y + tex->GetVelo()->y > edge1->y)
	)
	{
		if(state == HIT_RIGHT)
		if(tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[1].x + tex->GetVelo()->x > edge2->x
		&& tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[1].x < edge2->x)
			return HIT_RIGHT;
		
		if(state == HIT_LEFT)
		if(tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[0].x + tex->GetVelo()->x < edge2->x
		&& tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[0].x > edge2->x)
			return HIT_LEFT;
	}


	if(state == HIT_HEAD)
	{
		here1.x = tex->GetPos()->x + tex->EdgeScale[0].x*tex->GetSize()->x/2;
		here1.y = tex->GetPos()->y + tex->EdgeScale[0].y*tex->GetSize()->y/2;

		dest1.x = tex->GetPos()->x + tex->EdgeScale[0].x*tex->GetSize()->x/2 
			+ (edge1->y - edge2->y != 0)*tex->GetVelo()->x;
		dest1.y = tex->GetPos()->y + tex->EdgeScale[0].y*tex->GetSize()->y/2 + tex->GetVelo()->y;

		here2.x = tex->GetPos()->x + tex->EdgeScale[1].x*tex->GetSize()->x/2;
		here2.y = tex->GetPos()->y + tex->EdgeScale[1].y*tex->GetSize()->y/2;
		
		dest2.x = tex->GetPos()->x + tex->EdgeScale[1].x*tex->GetSize()->x/2
			+ (edge1->y - edge2->y != 0)*tex->GetVelo()->x;
		dest2.y = tex->GetPos()->y + tex->EdgeScale[1].y*tex->GetSize()->y/2 + tex->GetVelo()->y;

		if(CrossCheckLine(&here1,&dest1,edge1,edge2)
		|| CrossCheckLine(&here2,&dest2,edge1,edge2))
			return HIT_HEAD;
	}

	if(state == HIT_UNDER)
	{
		here1.x = tex->GetPos()->x + tex->EdgeScale[2].x*tex->GetSize()->x/2;
		here1.y = tex->GetPos()->y + tex->EdgeScale[2].y*tex->GetSize()->y/2;

		dest1.x = tex->GetPos()->x + tex->EdgeScale[2].x*tex->GetSize()->x/2
			+ (edge1->y - edge2->y != 0)*tex->GetVelo()->x
			;
		dest1.y = tex->GetPos()->y + tex->EdgeScale[2].y*tex->GetSize()->y/2 + tex->GetVelo()->y;

		here2.x = tex->GetPos()->x + tex->EdgeScale[3].x*tex->GetSize()->x/2;
		here2.y = tex->GetPos()->y + tex->EdgeScale[3].y*tex->GetSize()->y/2;
		
		dest2.x = tex->GetPos()->x + tex->EdgeScale[3].x*tex->GetSize()->x/2
			+ (edge1->y - edge2->y != 0)*tex->GetVelo()->x
			;
		dest2.y = tex->GetPos()->y + tex->EdgeScale[3].y*tex->GetSize()->y/2 + tex->GetVelo()->y;

		if(CrossCheckLine(&here1,&dest1,edge1,edge2)
		|| CrossCheckLine(&here2,&dest2,edge1,edge2))
			return HIT_UNDER;
	}
		
	return 0;
}

int CheckHitEdge(Texture * tex,Texture * blk,int caltype)//頂点の当たり判定
{
	for(int i = 0;i < blk->EdgeNumber;i++)
	{
		D3DXVECTOR3 dot,dot2,dot3;
		dot.x = blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x;
		dot.y = blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y;
		dot2.x = blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x;
		dot2.y = blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y;

		if(i > 0)
		{
			dot3.x = blk->EdgeScale[i - 1].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x;
			dot3.y = blk->EdgeScale[i - 1].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y;
		}
		else
		{
			dot3.x = blk->EdgeScale[blk->EdgeNumber - 1].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x;
			dot3.y = blk->EdgeScale[blk->EdgeNumber - 1].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y;
		}


		switch(CheckHitPoint(tex,&dot,&dot2,&dot3))
		{
		case HIT_LEFT:
			if(caltype == CALVELO_PARALLEL)
			tex->SetVelo(0 + blk->GetVelo()->x,tex->GetVelo()->y,tex->GetVelo()->z);
			if(caltype == CALVELO_VERTICAL)
			tex->SetVelo(-tex->GetVelo()->x,tex->GetVelo()->y,tex->GetVelo()->z);
			return HIT_LEFT;
		case HIT_RIGHT:
			if(caltype == CALVELO_PARALLEL)
			tex->SetVelo(0 + blk->GetVelo()->x,tex->GetVelo()->y,tex->GetVelo()->z);
			if(caltype == CALVELO_VERTICAL)
			tex->SetVelo(-tex->GetVelo()->x,tex->GetVelo()->y,tex->GetVelo()->z);
			return HIT_RIGHT;
		case HIT_HEAD:
			if(caltype == CALVELO_PARALLEL)
			tex->SetVelo(tex->GetVelo()->x + blk->GetVelo()->x,
						 0 + blk->GetVelo()->y,
						 tex->GetVelo()->z + blk->GetVelo()->z);
			if(caltype == CALVELO_VERTICAL)
			tex->SetVelo(tex->GetVelo()->x,-tex->GetVelo()->y,tex->GetVelo()->z);
			return HIT_HEAD;
		case HIT_UNDER:
			if(caltype == CALVELO_PARALLEL)
			tex->SetVelo(tex->GetVelo()->x + blk->GetVelo()->x,
						 0 + blk->GetVelo()->y,
						 tex->GetVelo()->z + blk->GetVelo()->z);
			if(caltype == CALVELO_VERTICAL)
			tex->SetVelo(tex->GetVelo()->x,-tex->GetVelo()->y,tex->GetVelo()->z);
			return HIT_UNDER;
		}
	}
	return 0;
}

int CheckHitPoint(Texture * tex,D3DXVECTOR3 * dot,D3DXVECTOR3 * dot2,D3DXVECTOR3 * dot3)
{

	if(dot->x - dot2->x <= 0&&dot->x - dot3->x <= 0)
			if(tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[2].y < dot->y
			&& tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[0].y > dot->y)
			{
				if(tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[1].x + tex->GetVelo()->x > dot->x
				&& tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[1].x < dot->x)
				return (HIT_RIGHT);
			}

	if(dot->y - dot2->y >= 0&&dot->y - dot3->y >= 0)
			if(tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[1].x > dot->x
			&& tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[0].x < dot->x)
			{
				if(tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[2].y + tex->GetVelo()->y < dot->y
				&& tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[2].y > dot->y)
				return (HIT_UNDER);
			}
		
	
	if(dot->x - dot2->x >= 0&&dot->x - dot3->x >= 0)
			if(tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[2].y < dot->y
			&& tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[0].y  > dot->y)
			{
				if(tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[0].x + tex->GetVelo()->x < dot->x
				&& tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[0].x > dot->x)
				return (HIT_LEFT);
			}

	if(dot->y - dot2->y <= 0&&dot->y - dot3->y <= 0)
			if(tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[1].x > dot->x
			&& tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[0].x < dot->x)
			{
				if(tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[0].y + tex->GetVelo()->y > dot->y
				&& tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[0].y < dot->y)
				return (HIT_HEAD);
			}
		
	return 0;
}

void WalkStair(Texture *tex,Block *blk)
{
	for(int i = 0;i < blk->EdgeNumber;i++)
	{
		if(blk->EdgeScale[i].x - blk->EdgeScale[(i + 1)%blk->EdgeNumber].x > -1
		|| blk->EdgeScale[i].x - blk->EdgeScale[(i + 1)%blk->EdgeNumber].x < 1)
		{
			int p = (i + 1)%blk->EdgeNumber;
			if(blk->EdgeScale[i].y >= blk->EdgeScale[(i + 1)%blk->EdgeNumber].y)
			{
			if(tex->EdgeScale[2].y*tex->GetSize()->y/2 + tex->GetPos()->y + tex->GetVelo()->y < blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y
			&& tex->EdgeScale[2].y*tex->GetSize()->y/2 + tex->GetPos()->y + tex->GetVelo()->y > blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y - STAIR_HEIGHT)
			{
				if(tex->EdgeScale[2].x*tex->GetSize()->x/2 + tex->GetPos()->x + tex->GetVelo()->x < blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x
				&& tex->EdgeScale[2].x*tex->GetSize()->x/2 + tex->GetPos()->x > blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x)
				tex->SetPos(tex->GetPos()->x,blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetPos()->y - blk->GetVelo()->y - tex->EdgeScale[2].y*tex->GetSize()->y/2 + 1,0);

				if(tex->EdgeScale[3].x*tex->GetSize()->x/2 + tex->GetPos()->x + tex->GetVelo()->x > blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x
				&& tex->EdgeScale[3].x*tex->GetSize()->x/2 + tex->GetPos()->x < blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x)
				tex->SetPos(tex->GetPos()->x,blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetPos()->y - blk->GetVelo()->y - tex->EdgeScale[2].y*tex->GetSize()->y/2 + 1,0);
			}
			}
			else
			if(tex->EdgeScale[2].y*tex->GetSize()->y/2 + tex->GetPos()->y + tex->GetVelo()->y < blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y
			&& tex->EdgeScale[2].y*tex->GetSize()->y/2 + tex->GetPos()->y + tex->GetVelo()->y > blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetPos()->y + blk->GetVelo()->y - STAIR_HEIGHT)
			{
				if(tex->EdgeScale[2].x*tex->GetSize()->x/2 + tex->GetPos()->x + tex->GetVelo()->x < blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x
				&& tex->EdgeScale[2].x*tex->GetSize()->x/2 + tex->GetPos()->x > blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x)
				tex->SetPos(tex->GetPos()->x,blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetPos()->y - blk->GetVelo()->y - tex->EdgeScale[2].y*tex->GetSize()->y/2 + 1,0);

				if(tex->EdgeScale[3].x*tex->GetSize()->x/2 + tex->GetPos()->x + tex->GetVelo()->x > blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x
				&& tex->EdgeScale[3].x*tex->GetSize()->x/2 + tex->GetPos()->x < blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetPos()->x + blk->GetVelo()->x)
				tex->SetPos(tex->GetPos()->x,blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetPos()->y - blk->GetVelo()->y - tex->EdgeScale[2].y*tex->GetSize()->y/2 + 1,0);
			}
			
			
		}
	}

	
}

int CheckInsideAngle(D3DXVECTOR3 *point1,D3DXVECTOR3 *fulcrum,D3DXVECTOR3 *point2)
{
	D3DXVECTOR3 side1,side2;

	side1.x = fulcrum->x - point1->x;
	side1.y = fulcrum->y - point1->y;

	side2.x = -fulcrum->x + point2->x;
	side2.y = -fulcrum->y + point2->y;

	float angle1,angle2;
	if(side1.y > 0)
		angle1 = acos(side1.x/sqrt(sqr(side1.x) + sqr(side1.y)));
	else
		angle1 = -acos(side1.x/sqrt(sqr(side1.x) + sqr(side1.y)));

	if(side2.y > 0)
		angle2 = acos(side2.x/sqrt(sqr(side2.x) + sqr(side2.y)));
	else
		angle2 = -acos(side2.x/sqrt(sqr(side2.x) + sqr(side2.y)));

	if(angle2 - angle1 < PAI && angle2 - angle1 > -PAI)
	{
		if(angle2 - angle1 > 0)
			return -1;
		else
		if(angle2 - angle1 < 0)
			return 1;
		else
			return 0;
	}
	else
	{
		if(angle1 - angle2 > 0)
			return -1;
		else
		if(angle1 - angle2 < 0)
			return 1;
		else
			return 0;
	}

}

int CheckHitCourse(D3DXVECTOR3 *point1,D3DXVECTOR3 *fulcrum,D3DXVECTOR3 *point2)
{
	D3DXVECTOR3 side1,side2;

	side1.x = fulcrum->x - point1->x;
	side1.y = fulcrum->y - point1->y;

	side2.x = -fulcrum->x + point2->x;
	side2.y = -fulcrum->y + point2->y;
	
	if(point1->x - fulcrum->x < 0.0001f&&point1->x - fulcrum->x > -0.0001f)
	{
		if(side1.y > 0)
		{
			if(CheckInsideAngle(point1,fulcrum,point2) == 1)
			{
				return HIT_RIGHT;
			}
			else
				return HIT_LEFT;
		}
		else
		{
			if(CheckInsideAngle(point1,fulcrum,point2) == 1)
			{
				return HIT_LEFT;
			}
			else
				return HIT_RIGHT;
		}
	}
	else
	{
		if(side1.x > 0)
		{
			if(CheckInsideAngle(point1,fulcrum,point2) == 1)
			{
				return HIT_UNDER;
			}
			else
				return HIT_HEAD;
		}
		else
		{
			if(CheckInsideAngle(point1,fulcrum,point2) == 1)
			{
				return HIT_HEAD;
			}
			else
				return HIT_UNDER;
		}
	}

	return 0;
}

D3DXVECTOR3 *CalVeloHitSide(D3DXVECTOR3 *velo,D3DXVECTOR3 *point1,D3DXVECTOR3 *point2,int caltype,CSoundManager * pSoundManager)
{
	D3DXVECTOR3 vec1,vec2,temp;
	
	vec1.x = (point1->x - point2->x);
	vec1.y = (point1->y - point2->y);
	vec2.x = vec1.y;
	vec2.y = -1*vec1.x;
	float s,t;
	if((vec1.x* velo->y - velo->x *vec1.y) != 0&&(vec1.x*vec2.y - vec2.x* vec1.y) != 0
	&& vec1.x!= 0)
	{
		t = (vec1.x * velo->y - velo->x *vec1.y)/(vec1.x*vec2.y - vec2.x* vec1.y);

		s = (velo->x - t*vec2.x)/vec1.x;

		switch(caltype)
		{
		case CALVELO_PARALLEL:
			velo->x += -t*vec2.x;
			velo->y += -t*vec2.y;
			/*if(velo->x*vec1.x < 0)
			{
				vec1.x = -vec1.x;
			}
			if(velo->y*vec1.y < 0)
			{
				vec1.y = -vec1.y;
			}*/
			if(velo->x/sqrt(sqr(velo->x) + sqr(velo->y))
			!= vec1.x/sqrt(sqr(vec1.x) + sqr(vec1.y))
				)
			{
				float b = vec1.x/sqrt(sqr(vec1.x) + sqr(vec1.y));
				//float b2 = vec1.y/sqrt(sqr(vec1.x) + sqr(vec1.y));
			
				if(velo->y * velo->x > 0)
					velo->x -= 0.00001f;
				else if(velo->y * velo->x < 0)
					velo->x += 0.00001f;
				
				float a = velo->x/sqrt(sqr(velo->x) + sqr(velo->y));

				if((velo->y*velo->x)*(b - a) < 0)
				int p = 0;
			}
			break;
		case CALVELO_VERTICAL:
			velo->x += -t*vec2.x*2;
			velo->y += -t*vec2.y*2;
			break;
		case CALVELO_BECTLE:
			//g_pPerformance->PlaySegmentEx(g_pSeg[SOUND_PLAYER_BAUND + rand()%2], NULL, NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL);
			pSoundManager->Play("SOUND_PLAYER_BAUND");
			/*velo->x += -t*vec2.x*4;
			velo->y += -t*vec2.y*4;*/
			float V = sqrt(sqr(velo->x) + sqr(velo->y));
			if(V > 30)
			{
				V = 30;
			}
			/*velo->x += -t*vec2.x*2;
			velo->y += -t*vec2.y*2;
			velo->x = (V*1.2f + 8)*(velo->x)/sqrt(sqr(velo->x) + sqr(velo->y));
			velo->y = (V*1.2f + 8)*(velo->y)/sqrt(sqr(velo->x) + sqr(velo->y));*/
			velo->x = (V*1.2f + 8)*(-t*vec2.x*2)/sqrt(sqr(t*vec2.x*2) + sqr(t*vec2.y*2));
			velo->y = (V*1.2f + 8)*(-t*vec2.y*2)/sqrt(sqr(t*vec2.x*2) + sqr(t*vec2.y*2));
			
			
			break;
		}


		return (velo);
	}
	else
	{
		return velo;
	}
}

bool CheckContained(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR3 p){
        int a = getSignedArea(p1, p2, p3);
        if(a == 0) return false;//3点が直線に並んでる
		if(
			(getSignedArea(p1, p2, p) > 0&&getSignedArea(p2, p3, p) > 0&&getSignedArea(p3, p1, p) > 0)
			||
			(getSignedArea(p1, p2, p) < 0&&getSignedArea(p2, p3, p) < 0&&getSignedArea(p3, p1, p) < 0)
			)
			return true;
		return false;
    }

static int getSignedArea(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3)
{
	return (int)((p2.x - p1.x) * (p3.y - p1.y) 
		 - (p3.x - p1.x) * (p2.y - p1.y));
}

bool CheckPosinTex(D3DXVECTOR3 *pos,Texture *tex)
{
	D3DXVECTOR3 TexEdge[10];
	for(int i = 0;i < tex->EdgeNumber; i++)
	{
		TexEdge[i].x = tex->GetPos()->x + tex->EdgeScale[i].x*tex->GetSize()->x/2;
		TexEdge[i].y = tex->GetPos()->y + tex->EdgeScale[i].y*tex->GetSize()->y/2;
	}
        // 凸包上の点を得る
	for(int i = 0; i < tex->EdgeNumber - 2; ++i)
	{
		for(int j = i + 1; j < tex->EdgeNumber - 1; ++j)
		{
			for(int k = j + 1; k < tex->EdgeNumber; ++k)
			{
                        //点lが三角形ijkに含まれていたら、凸包の点ではない
				{
					if((CheckContained(
					TexEdge[i], TexEdge[j],TexEdge[k],*pos)))
					{
					   return 1;
					}
                }
                
            }
		}
	}
	return 0;
}

bool CheckTexinTex(Texture *tex1,Texture *tex2)
{
	D3DXVECTOR3 TexEdge;
	for(int i = 0;i < tex1->EdgeNumber;i++)
	{
		TexEdge.x = tex1->GetPos()->x + tex1->EdgeScale[i].x*tex1->GetSize()->x/2;
		TexEdge.y = tex1->GetPos()->y + tex1->EdgeScale[i].y*tex1->GetSize()->y/2;
		if(CheckPosinTex(&TexEdge,tex2))
			return 1;
	}
	return 0;
}

bool CheckTexinScrean(Texture *tex,D3DXVECTOR3 * CPos)
{
	for(int i = 0;i < tex->EdgeNumber;i++)
		if(tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[i].x >= CPos->x - 400
		&& tex->GetPos()->x + tex->GetSize()->x/2*tex->EdgeScale[i].x <= CPos->x + 400
		&& tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[i].y >= CPos->y - 300
		&& tex->GetPos()->y + tex->GetSize()->y/2*tex->EdgeScale[i].y <= CPos->y + 300)
			return 1;
	return 0;
}