//=============================================================
// BlockManager.cpp
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
BlockManager::BlockManager(Renderer *R,CSoundManager * pSoundManager)
{
	m_pSoundManager = pSoundManager;
	Camera = R;
	for(int i = 0;i < COMBO_BLKINFMAX;i++)
		hCmb_BlkInf[i] = &combo_Block[i];
	for(int i = 0;i < BLOCKSHAPEMAX;i++)
		for(int p = 0;p < BLOCKTYPEMAX;p++)
			TexTable2[i][p] = new TextureData();
	/*for(int i = 0;i < BLOCKTYPE_MAX;i++)
		TexTable[i]->pTex = new LPDIRECT3DTEXTURE9();
	*/

	B = new Block("Picture/Cursor.png",10000,-10000,0,100,100,Camera);

	MouseCursor = new Texture("Picture/MouseCursor.png",0,0,0,50,50,Camera);
//	MouseCursor->Create();

	Block *temp;
	temp = new Block("Picture/NULL.png",10000,-10000,0,100,100,Camera);
	temp->Type = temp->Shape = 0;
//	temp->Create();
	Blocklist.push_back(temp);	
	SelectBlock = temp;
	//SetCPos(0,0,-720);
	TexTableInit();
	ReserveType = -1;
	Dragflg_Camera = Dragflg_Block =0;

	
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
BlockManager::~BlockManager()
{
	for(int i = 0;i < BLOCKSHAPEMAX;i++)
		for(int p = 0;p < BLOCKTYPEMAX;p++)
			delete TexTable2[i][p];
	delete B;
	delete MouseCursor;
	
	for(list<Block*>::iterator iter = Blocklist.begin(); iter != Blocklist.end(); iter++)
		delete (*iter);
}

void BlockManager::SetEdgeScale(TextureData *tex,int n,float x, float y)
{
	tex->EdgeScale[n].x = x;
	tex->EdgeScale[n].y = y;
}

void BlockManager::SetCPos(float x, float y, float z)
{
	CPos.x = x;
	CPos.y = y;
	CPos.z = z;
}

void BlockManager::Input()
{		
	SetSrideScale();
	SetCombo_BlkInf();
	//イテレーターをセット

		SelectShape = SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE], CB_GETCURSEL , 0 , 0);
		SelectType = SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_GETCURSEL , 0 , 0);
	
	if(SelectType < 0)
		SelectType = 0;
	if(SelectShape < 0)
		SelectShape = 0;

	float tempGRot = (float)G_SelectBlockRot;
	if(SelectBlock->GetRot()->z != tempGRot/180*PAI)
		SelectBlock->SetRot(0,0,(float)(tempGRot/180*PAI));

	

	list<Block*>::iterator iter = Blocklist.end();
	iter--;
	if((*iter)->Type == BLOCKTYPE_NULL&&Blocklist.size() >= 2)
		iter--;

	SelectBlock->Shape = SelectShape;
	SelectBlock->Type = SelectType;

	//画像を大きく
	if(g_diKeyState[DIK_ADD]& 0x80 )
	{
		SelectBlock->SetSize(SelectBlock->GetSize()->x + 1,SelectBlock->GetSize()->y + 1,SelectBlock->GetSize()->z);
		SelectBlock->SetMyVertex(1,0);
		if(SelectType == BLOCKTYPE_NULL)
		{
			B->SetSize(SelectBlock->GetSize()->x,SelectBlock->GetSize()->y,SelectBlock->GetSize()->z);
			B->SetMyVertex(1,0);
		}
	}
	//画像を小さく
	if(g_diKeyState[DIK_SUBTRACT]& 0x80 )
	{
		if(SelectBlock->GetSize()->y > 0)
		{
			SelectBlock->SetSize(SelectBlock->GetSize()->x - 1,SelectBlock->GetSize()->y - 1,SelectBlock->GetSize()->z);
			SelectBlock->SetMyVertex(1,0);
			if(SelectType == BLOCKTYPE_NULL)
			{
				B->SetSize(SelectBlock->GetSize()->x,SelectBlock->GetSize()->y,SelectBlock->GetSize()->z);
				B->SetMyVertex(1,0);
			}
		}
	}
	

	//ブロックの画像を選択時
	/*if(SelectBlock->Type > 0)
	{*/
		SelectBlock->SetpTexture(&TexTable2[SelectShape][SelectType]->pTex);
		SelectBlock->Type = SelectType;
		SelectBlock->Shape = SelectShape;
		SelectBlock->EdgeNumber = TexTable2[SelectShape][SelectType]->EdgeNum;
		SelectBlock->SetEdgeScale(TexTable2[SelectShape][SelectType]->EdgeScale);
		if((SelectBlock->Type == BLOCKTYPE_BRUE || SelectBlock->Type == BLOCKTYPE_RED)
			&& SelectBlock->Shape != BLOCKSHAPE_OBJECT)
		{
			SelectBlock->SetAlfa(SKELTONALFA);		
		}
		else
			SelectBlock->SetAlfa(255);


		list<Block*>::iterator iter2 = Blocklist.end();
		iter2--;
		(*iter2)->SetPos(MouseCursor->GetPos()->x,MouseCursor->GetPos()->y,0);
	//}

	
	if(m_MouseLocalPos.x > 0 && m_MouseLocalPos.x < 800 && m_MouseLocalPos.y > 0 && m_MouseLocalPos.y < 800)
	{
		//コピー
		list<Block*>::iterator iter_copy = Blocklist.end();
		iter_copy--;
		if(g_diKeyState[DIK_C]& 0x80 
		&&!(g_ExdiKeyState[DIK_C]& 0x80)
		&& (*iter_copy)->Type == BLOCKTYPE_NULL
		&& SelectBlock->Type != BLOCKTYPE_NULL
		&& Blocklist.size() >= 2)
		{
			list<Block*>::iterator iter2 = Blocklist.end();
			iter2--;
			(*iter2)->SetPos(*SelectBlock->GetPos());
			(*iter2)->SetRot(0,0,SelectBlock->GetRot()->z);
			(*iter2)->SetSize(*SelectBlock->GetSize());
			(*iter2)->EdgeNumber = SelectBlock->EdgeNumber;
			(*iter2)->SetEdgeScale(SelectBlock->EdgeScale);
			(*iter2)->Shape = SelectBlock->Shape;

			SelectBlock = (*iter2);
			SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE], CB_SETCURSEL ,(*iter)->Shape ,0);
			SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_SETCURSEL ,(*iter)->Type ,0);
			static char text2[16];
			sprintf(text2,"%lf",SelectBlock->GetRot()->z/PAI*180,strlen(text2));
			SetWindowText(edit_Block[0],(LPCSTR)text2);

			if(SrideScale.x != 0||SrideScale.y != 0)
			{
				if((*iter2)->Shape <= BLOCKSHAPE_TRIANGLE)
				{
					(*iter2)->SetPos((*iter2)->GetPos()->x + 0.96f*(*iter2)->GetSize()->x*SrideScale.x,
									 (*iter2)->GetPos()->y + 0.96f*(*iter2)->GetSize()->y*SrideScale.y,
									 (*iter2)->GetPos()->z);
				}
				else
				{
					(*iter2)->SetPos((*iter2)->GetPos()->x + 0.96f*(*iter2)->GetSize()->x*SrideScale.x/2,
									 (*iter2)->GetPos()->y + 0.96f*(*iter2)->GetSize()->y*SrideScale.y,
									 (*iter2)->GetPos()->z);
				}
				SelectBlock->SetVelo(0,0,0);
				Block *temp;
				temp = new Block("Picture/NULL.png",(*iter)->GetPos()->x,(*iter)->GetPos()->y,0,(*iter)->GetSize()->x,(*iter)->GetSize()->y,Camera);
				temp->Type = temp->Shape = 0;
				B->SetPos((*iter)->GetPos()->x,(*iter)->GetPos()->y,0);
				B->SetSize((*iter)->GetSize()->x,(*iter)->GetSize()->y,(*iter)->GetSize()->z);
				B->SetMyVertex(1,0);
				static char text3[16];
				sprintf(text3,"%lf",SelectBlock->GetRot()->z/PAI*180,strlen(text3));
				SetWindowText(edit_Block[0],(LPCSTR)text3);
				
				Blocklist.push_back(temp);
			}

		}

		//移動
		if(g_diKeyState[DIK_RIGHT]& 0x80 && Blocklist.size() >= 2)
		{
			SelectBlock->SetPos(SelectBlock->GetPos()->x + 1,SelectBlock->GetPos()->y,SelectBlock->GetPos()->z);
			B->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y,0);
		}

		if(g_diKeyState[DIK_LEFT]& 0x80 && Blocklist.size() >= 2)
		{
			SelectBlock->SetPos(SelectBlock->GetPos()->x - 1,SelectBlock->GetPos()->y,SelectBlock->GetPos()->z);
			B->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y,0);
		}

		if(g_diKeyState[DIK_UP]& 0x80 && Blocklist.size() >= 2)
		{
			SelectBlock->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y + 1,SelectBlock->GetPos()->z);
			B->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y,0);
		}

		if(g_diKeyState[DIK_DOWN]& 0x80 && Blocklist.size() >= 2)
		{
			SelectBlock->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y - 1,SelectBlock->GetPos()->z);
			B->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y,0);
		}

		//ブロックを削除
		if(g_diKeyState[DIK_BACK]& 0x80 &&!(g_ExdiKeyState[DIK_BACK]& 0x80)&& Blocklist.size() >= 2)
		{
			list<Block*>::iterator iter3 = Blocklist.end();
			iter3--;
			if((*iter3)->Type == BLOCKTYPE_NULL
				&& SelectBlock->Type != BLOCKTYPE_NULL)
			{
				iter3--;
				delete (*iter3);
				Blocklist.erase(iter3);
				
				if(Blocklist.size() >= 2)
				{
					list<Block*>::iterator iter2 = Blocklist.end();
					iter2--;
					iter2--;
					SelectBlock = (*iter2);
					B->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y,0);
					B->SetSize(SelectBlock->GetSize()->x,SelectBlock->GetSize()->y,SelectBlock->GetSize()->z);
					B->SetMyVertex(1,0);
					static char text2[16];
					sprintf(text2,"%lf",SelectBlock->GetRot()->z/PAI*180,strlen(text2));
					SetWindowText(edit_Block[0],(LPCSTR)text2);
					SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE], CB_SETCURSEL ,(*iter2)->Shape ,0);
					SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_SETCURSEL ,(*iter2)->Type ,0);
					ReserveType = (*iter2)->Type;
				}
				else
				{
					list<Block*>::iterator iter2 = Blocklist.end();
					iter2--;
					SelectBlock = (*iter2);
					B->SetPos(10000,-10000,0);
					B->SetSize(0,0,0);
					B->SetMyVertex(1,0);
					static char text2[16];
					sprintf(text2,"%lf",SelectBlock->GetRot()->z/PAI*180,strlen(text2));
					SetWindowText(edit_Block[0],(LPCSTR)text2);
					SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE], CB_SETCURSEL ,(*iter2)->Shape ,0);
					SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_SETCURSEL ,(*iter2)->Type ,0);
					ReserveType = (*iter2)->Type;
				}
			}
			else
			{
				(*iter3)->Type = (*iter3)->Shape = 0;
				if(Blocklist.size() >= 2)
				{
					iter3--;
					SelectBlock = (*iter3);
				}
			}
		}
	//マウスクリック時
		if((MState.lButton&&!(MState.exlButton))||(g_diKeyState[DIK_NUMPADENTER]& 0x80 &&!(g_ExdiKeyState[DIK_NUMPADENTER]& 0x80)))
		{
			list<Block*>::iterator iter2 = Blocklist.end();
			iter2--;
			if((*iter2)->Type!= BLOCKTYPE_NULL)
			{
				SelectBlock = (*iter);
 				SelectBlock->SetVelo(0,0,0);
				Block *temp;
				temp = new Block("Picture/NULL.png",(*iter)->GetPos()->x,(*iter)->GetPos()->y,0,(*iter)->GetSize()->x,(*iter)->GetSize()->y,Camera);
//				temp->Create();
				temp->Type = temp->Shape = 0;
				B->SetPos((*iter)->GetPos()->x,(*iter)->GetPos()->y,0);
				B->SetSize((*iter)->GetSize()->x,(*iter)->GetSize()->y,(*iter)->GetSize()->z);
				B->SetMyVertex(1,0);
				static char text2[16];
				sprintf(text2,"%lf",SelectBlock->GetRot()->z/PAI*180,strlen(text2));
				SetWindowText(edit_Block[0],(LPCSTR)text2);
				
				Blocklist.push_back(temp);
				/*SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_SETCURSEL  , 0 , 0);
				SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE] , CB_SETCURSEL  , 0 , 0);*/
				
			}
			else
			{
				if(Blocklist.size() >= 2)
				{
					
					list<Block*>::iterator iter2 = Blocklist.end();
					iter2--;
					iter2--;
					while(1)
					{
						if(CheckPosinTex(MouseCursor->GetPos(),(*iter2)))
						{
							SelectBlock = (*iter2);
							Blocklist.erase(iter2);
							list<Block*>::iterator iter3 = Blocklist.end();
							iter3--;
							B->SetPos(SelectBlock->GetPos()->x,SelectBlock->GetPos()->y,0);
							B->SetSize(SelectBlock->GetSize()->x,SelectBlock->GetSize()->y,SelectBlock->GetSize()->z);
							B->SetMyVertex(1,0);
							Blocklist.insert(iter3,SelectBlock);
							break;
						}
						if(iter2 == Blocklist.begin())
							break;
						iter2--;
					}
					list<Block*>::iterator iter3 = Blocklist.end();
					iter3--;
					iter3--;
					//SelectBlock = (*iter3);
					static char text2[16];
							sprintf(text2,"%lf",(*iter3)->GetRot()->z/PAI*180,strlen(text2));
							SetWindowText(edit_Block[0],(LPCSTR)text2);

					SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE], CB_SETCURSEL ,(*iter3)->Shape ,0);
					SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_SETCURSEL ,(*iter3)->Type ,0);
					ReserveType = (*iter3)->Type;
					
				}

			}
		}
		//ドラッグ時
		if(MState.lButton&&(MState.exlButton))
		{
			if(CheckPosinTex(MouseCursor->GetPos(),SelectBlock)
				&& Blocklist.size() >= 2)
			{
				if(Dragflg_Block == 0)
				{
					MPostoBlk.x = MouseCursor->GetPos()->x - SelectBlock->GetPos()->x;
					MPostoBlk.y = MouseCursor->GetPos()->y - SelectBlock->GetPos()->y;
					Dragflg_Block = 1;
				}
			}
			if(Dragflg_Block == 1)
				SelectBlock->SetPos(MouseCursor->GetPos()->x - MPostoBlk.x,
									MouseCursor->GetPos()->y - MPostoBlk.y,
									0);		
		}
		else
			Dragflg_Block = 0;
		//Camera->SetCPos(Camera->GetCPos()->x + 1,Camera->GetCPos()->y,Camera->GetCPos()->z);

		if(MState.rButton&&(MState.exrButton))
		{
			if(Dragflg_Camera == 1)
			{
				Camera->SetCPos(Camera->GetCPos()->x
							  - MouseCursor->GetPos()->x + MPostoScreen.x,
								Camera->GetCPos()->y
							  - MouseCursor->GetPos()->y + MPostoScreen.y,
								Camera->GetCPos()->z);	
				Camera->SetCFocus(Camera->GetCPos()->x,
								  Camera->GetCPos()->y,
								  0);
			/*	MouseCursor->SetPos(Camera->GetCPos()->x + MouseCursor->GetPos()->x,
									Camera->GetCPos()->y + MouseCursor->GetPos()->y,					
									0);*/
				MouseCursor->SetPos(
				(float)m_MouseLocalPos.x - CLIENT_WIDTH/2 + Camera->GetCPos()->x,
				(float)-m_MouseLocalPos.y + CLIENT_HEIGHT/2 + Camera->GetCPos()->y,
				0);
				MPostoScreen.x = MouseCursor->GetPos()->x;
				MPostoScreen.y = MouseCursor->GetPos()->y;
			}
			if(Dragflg_Camera == 0)
			{
				MPostoScreen.x = MouseCursor->GetPos()->x
					//- Camera->GetCPos()->x
					;
				MPostoScreen.y = MouseCursor->GetPos()->y
					//- Camera->GetCPos()->y
					;
				Dragflg_Camera = 1;
			}
		}
		else
			Dragflg_Camera = 0;
	}
}
int BlockManager::BlockHit(Texture *tex,Block *blk)
{

	int Hitflg = 0;

	//頂点の当たり判定
	Hitflg = CheckHitEdge(tex,blk,CALVELO_PARALLEL);
	/*if(Hitflg)
		return Hitflg;*/

	//各辺の当たり判定
	D3DXVECTOR3 dest1,dest2,dest3;
	for(int i = 0;i < blk->EdgeNumber;i++)
	{
		dest1.x = blk->GetPos()->x + blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest1.y = blk->GetPos()->y + blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetVelo()->y;

		dest2.x = blk->GetPos()->x + blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest2.y = blk->GetPos()->y + blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetVelo()->y;

		dest3.x = blk->GetPos()->x + blk->EdgeScale[(i + 2)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest3.y = blk->GetPos()->y + blk->EdgeScale[(i + 2)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetVelo()->y;
		if(tex->GetAccel()->y == 0)
		{
			int p = 0;
		}

		if(CheckHitLine(tex,&dest1,&dest2,CheckHitCourse(&dest1,&dest2,&dest3)))
		{
			if( CheckHitCourse(&dest1,&dest2,&dest3)<= HIT_RIGHT)
			{
				if(blk->Type == BLOCKTYPE_BECTLE&&i == 0)
				{
					float V = sqrt(sqr(tex->GetVelo()->x) + sqr(tex->GetVelo()->y));
					if(V > 30)
					{
						V = 30;
					}
					tex->SetVelo(-(blk->GetVelo()->x > 0)*(V*1.2f + 15) + (blk->GetVelo()->x < 0)*(V*1.2f + 15),
								   tex->GetVelo()->y,tex->GetVelo()->z);
				}
				else
					tex->SetVelo(0 + blk->GetVelo()->x,tex->GetVelo()->y,tex->GetVelo()->z);
			}
			else
			{
				//tex->SetVelo(tex->GetVelo()->x,0,tex->GetVelo()->z);
				if(blk->Type == BLOCKTYPE_BECTLE&&i == 0)
					CalVeloHitSide(tex->GetVelo(),&dest1,&dest2,CALVELO_BECTLE,m_pSoundManager);
				else
					CalVeloHitSide(tex->GetVelo(),&dest1,&dest2,CALVELO_PARALLEL,m_pSoundManager);
				tex->SetVelo(tex->GetVelo()->x + blk->GetVelo()->x,
							 tex->GetVelo()->y + blk->GetVelo()->y,
							 tex->GetVelo()->z);

			}
			int a = CheckHitCourse(&dest1,&dest2,&dest3);
			if(a)
				Hitflg = a;
			
			if(blk->Type == BLOCKTYPE_BECTLE&&i == 0&&Hitflg == HIT_UNDER)
				Hitflg = HIT_UNDER_BAUND;
		}
	}

	/*if(Hitflg == 0)
		Hitflg = CheckHitEdge(tex,blk,CALVELO_PARALLEL);*/

	return Hitflg;

}

int BlockManager::BlockHit2(Texture *tex,Block *blk)
{
	int Hitflg = 0;

	//頂点の当たり判定
	/*Hitflg = CheckHitEdge(tex,blk,CALVELO_PARALLEL);
	if(Hitflg)
		return Hitflg;*/

	//各辺の当たり判定
	D3DXVECTOR3 dest1,dest2,dest3;
	for(int i = 0;i < blk->EdgeNumber;i++)
	{
		dest1.x = blk->GetPos()->x + blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest1.y = blk->GetPos()->y + blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetVelo()->y;

		dest2.x = blk->GetPos()->x + blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest2.y = blk->GetPos()->y + blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetVelo()->y;

		dest3.x = blk->GetPos()->x + blk->EdgeScale[(i + 2)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest3.y = blk->GetPos()->y + blk->EdgeScale[(i + 2)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetVelo()->y;
		if(tex->GetAccel()->y == 0)
		{
			int p = 0;
		}

		if(CheckHitLine(tex,&dest1,&dest2,CheckHitCourse(&dest1,&dest2,&dest3)))
		{
			if( CheckHitCourse(&dest1,&dest2,&dest3)<= HIT_RIGHT)
			{
				if(blk->Type == BLOCKTYPE_BECTLE&&i == 0)
				{
					float V = sqrt(sqr(tex->GetVelo()->x) + sqr(tex->GetVelo()->y));
					if(V > 30)
					{
						V = 30;
					}
					tex->SetVelo(-(blk->GetVelo()->x > 0)*(V*1.2f + 15) + (blk->GetVelo()->x < 0)*(V*1.2f + 15),
								   tex->GetVelo()->y,tex->GetVelo()->z);
				}
				else
					tex->SetVelo(0 + blk->GetVelo()->x,tex->GetVelo()->y,tex->GetVelo()->z);
			}
			else
			{
				//tex->SetVelo(tex->GetVelo()->x,0,tex->GetVelo()->z);
				if(blk->Type == BLOCKTYPE_BECTLE&&i == 0)
					CalVeloHitSide(tex->GetVelo(),&dest1,&dest2,CALVELO_BECTLE,m_pSoundManager);
				else
					CalVeloHitSide(tex->GetVelo(),&dest1,&dest2,CALVELO_PARALLEL,m_pSoundManager);
				tex->SetVelo(tex->GetVelo()->x + blk->GetVelo()->x,
							 tex->GetVelo()->y + blk->GetVelo()->y,
							 tex->GetVelo()->z);
			}
			
			Hitflg = CheckHitCourse(&dest1,&dest2,&dest3);
			
			if(blk->Type == BLOCKTYPE_BECTLE&&i == 0&&Hitflg == HIT_UNDER)
				Hitflg = HIT_UNDER_BAUND;
		}
	}

	/*if(Hitflg == 0)
		Hitflg = CheckHitEdge(tex,blk,CALVELO_PARALLEL);*/

	return Hitflg;

}


bool BlockManager::BlockHit_Reflect(Texture *tex,Block *blk)
{
	bool Hitflg = 0;

	CheckHitEdge(tex,blk,CALVELO_VERTICAL);

	D3DXVECTOR3 dest1,dest2,dest3;
	for(int i = 0;i < blk->EdgeNumber;i++)
	{
		dest1.x = blk->GetPos()->x + blk->EdgeScale[i].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest1.y = blk->GetPos()->y + blk->EdgeScale[i].y*blk->GetSize()->y/2 + blk->GetVelo()->y;

		dest2.x = blk->GetPos()->x + blk->EdgeScale[(i + 1)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest2.y = blk->GetPos()->y + blk->EdgeScale[(i + 1)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetVelo()->y;

		dest3.x = blk->GetPos()->x + blk->EdgeScale[(i + 2)%blk->EdgeNumber].x*blk->GetSize()->x/2 + blk->GetVelo()->x;
		dest3.y = blk->GetPos()->y + blk->EdgeScale[(i + 2)%blk->EdgeNumber].y*blk->GetSize()->y/2 + blk->GetVelo()->y;

		if(CheckHitLine(tex,&dest1,&dest2,CheckHitCourse(&dest1,&dest2,&dest3)))
		{
			if( CheckHitCourse(&dest1,&dest2,&dest3)<= HIT_RIGHT)
				tex->SetVelo(-tex->GetVelo()->x,tex->GetVelo()->y,tex->GetVelo()->z);
			else
			{
				//tex->SetVelo(tex->GetVelo()->x,0,tex->GetVelo()->z);
				CalVeloHitSide(tex->GetVelo(),&dest1,&dest2,CALVELO_VERTICAL,m_pSoundManager);
			}
			Hitflg = 1;
		}
	}

	return Hitflg;


	

	return 0;
}

void BlockManager::SetCombo_BlkInf()
{
	//int ReserveType = SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_GETCURSEL  , 0 , 0);
	LPCTSTR  ShapeType[BLOCKSHAPEMAX] = {
	"NULL",
	"SQUARE",
	"TRIANGLE",
	"BYTRIANGLE",
	"BYSQUARE",
	"OBJECT",
	};
	

	LPCTSTR  BlockType[BLOCKTYPEMAX] = {
	"NULL",
	"NORMAL",
	"RED",
	"BRUE",
	"BECTLE",
	"MAGNET",
	};

	LPCTSTR  ObjectType[OBJECTTYPEMAX] = {
	"NULL",
	"START",
	"CHECK",
	"CARD",
	};

	//ブロックの形をコンボボックスに表示//////////////
	if(SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_GETCOUNT,0,0) <= 1)
	for(int i = 1;i < BLOCKSHAPEMAX;i++)
		SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_INSERTSTRING  ,i,(LPARAM)ShapeType[i]);
	//////////////////////////////////////////////////

	//ブロックの種類をコンボボックスに表示//////////////
	if(SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE] , CB_GETCOUNT,0,0)!= UnderShapeNum[SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_GETCURSEL,0,0)])
	{
		SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_RESETCONTENT , 0 , 0);
		if(SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_GETCURSEL,0,0) != -1)
		for(int i = 0;i < UnderShapeNum[SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_GETCURSEL,0,0)];i++)
		{
			if(SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_GETCURSEL,0,0) != BLOCKSHAPE_OBJECT)
				SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE] , CB_INSERTSTRING  ,i,(LPARAM)BlockType[i]);
			else
				SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE] , CB_INSERTSTRING  ,i,(LPARAM)ObjectType[i]);
		}
		else
			SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE] , CB_INSERTSTRING  ,0,(LPARAM)"NULL");

		if(ReserveType > UnderShapeNum[SendMessage(*hCmb_BlkInf[COMBO_BLOCKSHAPE] , CB_GETCURSEL,0,0)])
			SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_SETCURSEL  , 0 , 0);
		else
			SendMessage(*hCmb_BlkInf[COMBO_BLOCKTYPE], CB_SETCURSEL  , ReserveType , 0);
	}

}

void BlockManager::TexTableInit()
{
	UnderShapeNum[BLOCKSHAPE_NULL] = 1;
	UnderShapeNum[BLOCKSHAPE_SQUARE] = BLOCKTYPE_MAGNET + 1;
	UnderShapeNum[BLOCKSHAPE_TRIANGLE] = BLOCKTYPE_BECTLE + 1;
	UnderShapeNum[BLOCKSHAPE_BYTRIANGLE] = BLOCKTYPE_BRUE + 1;
	UnderShapeNum[BLOCKSHAPE_BYSQUARE] = BLOCKTYPE_BRUE + 1;
	UnderShapeNum[BLOCKSHAPE_OBJECT] = OBJECTTYPEMAX;

	//for(int i = 0)

	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_SQUARE][BLOCKTYPE_NORMAL]->pTex,"Picture/BLOCK/SQUARE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_SQUARE][BLOCKTYPE_RED]->pTex,"Picture/BLOCK/RED_SQUARE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_SQUARE][BLOCKTYPE_BRUE]->pTex,"Picture/BLOCK/BRUE_SQUARE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_SQUARE][BLOCKTYPE_BECTLE]->pTex,"Picture/BLOCK/BECTLE_SQUARE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_SQUARE][BLOCKTYPE_MAGNET]->pTex,"Picture/BLOCK/MAGNET_SQUARE.png");

	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_TRIANGLE][BLOCKTYPE_NORMAL]->pTex,"Picture/BLOCK/TRIANGLE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_TRIANGLE][BLOCKTYPE_RED]->pTex,"Picture/BLOCK/RED_TRIANGLE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_TRIANGLE][BLOCKTYPE_BRUE]->pTex,"Picture/BLOCK/BRUE_TRIANGLE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_TRIANGLE][BLOCKTYPE_BECTLE]->pTex,"Picture/BLOCK/BECTLE_TRIANGLE.png");

	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_BYTRIANGLE][BLOCKTYPE_NORMAL]->pTex,"Picture/BLOCK/HALFTRIANGLE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_BYTRIANGLE][BLOCKTYPE_RED]->pTex,"Picture/BLOCK/RED_HALFTRIANGLE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_BYTRIANGLE][BLOCKTYPE_BRUE]->pTex,"Picture/BLOCK/BRUE_HALFTRIANGLE.png");
	
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_BYSQUARE][BLOCKTYPE_NORMAL]->pTex,"Picture/BLOCK/HALF_SQUARE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_BYSQUARE][BLOCKTYPE_RED]->pTex,"Picture/BLOCK/RED_HALF_SQUARE.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_BYSQUARE][BLOCKTYPE_BRUE]->pTex,"Picture/BLOCK/BRUE_HALF_SQUARE.png");

	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_OBJECT][OBJECTTYPE_START]->pTex,"Picture/STARTPOINT.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_OBJECT][OBJECTTYPE_CHECK]->pTex,"Picture/check.png");
	B->SetpTexturefromFile(&TexTable2[BLOCKSHAPE_OBJECT][OBJECTTYPE_CARD]->pTex,"Picture/CARD.png");
	
	for(int i = 0;i < UnderShapeNum[BLOCKSHAPE_TRIANGLE];i++)
	{
		SetEdgeScale(TexTable2[BLOCKSHAPE_TRIANGLE][i],0,-1,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_TRIANGLE][i],1,1,-1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_TRIANGLE][i],2,-1,-1);
		TexTable2[BLOCKSHAPE_TRIANGLE][i]->EdgeNum = 3;
	}
	
	for(int i = 0;i < UnderShapeNum[BLOCKSHAPE_SQUARE];i++)
	{
		SetEdgeScale(TexTable2[BLOCKSHAPE_SQUARE][i],0,-1,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_SQUARE][i],1,1,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_SQUARE][i],2,1,-1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_SQUARE][i],3,-1,-1);
		TexTable2[BLOCKSHAPE_SQUARE][i]->EdgeNum = 4;
	}
	
	for(int i = 0;i < UnderShapeNum[BLOCKSHAPE_BYTRIANGLE];i++)
	{
		SetEdgeScale(TexTable2[BLOCKSHAPE_BYTRIANGLE][i],0,-0.5f,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_BYTRIANGLE][i],1,0.5f,-1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_BYTRIANGLE][i],2,-0.5f,-1);
		TexTable2[BLOCKSHAPE_BYTRIANGLE][i]->EdgeNum = 3;
	}

	for(int i = 0;i < UnderShapeNum[BLOCKSHAPE_BYSQUARE];i++)
	{
		SetEdgeScale(TexTable2[BLOCKSHAPE_BYSQUARE][i],0,-0.5f,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_BYSQUARE][i],1,0.5f,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_BYSQUARE][i],2,0.5f,-1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_BYSQUARE][i],3,-0.5f,-1);
		TexTable2[BLOCKSHAPE_BYSQUARE][i]->EdgeNum = 4;
	}

	for(int i = 0;i < UnderShapeNum[BLOCKSHAPE_OBJECT];i++)
	{
		SetEdgeScale(TexTable2[BLOCKSHAPE_OBJECT][i],0,-1,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_OBJECT][i],1,1,1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_OBJECT][i],2,1,-1);
		SetEdgeScale(TexTable2[BLOCKSHAPE_OBJECT][i],3,-1,-1);
		TexTable2[BLOCKSHAPE_OBJECT][i]->EdgeNum = 4;
	}

}


void BlockManager::SetSrideScale()
{
	SrideScale.x = 0;
	SrideScale.y = 0;
	if(g_diKeyState[DIK_NUMPAD1]& 0x80 
	|| g_diKeyState[DIK_NUMPAD2]& 0x80 
	|| g_diKeyState[DIK_NUMPAD3]& 0x80
	|| g_diKeyState[DIK_DOWN]& 0x80 
	)
		SrideScale.y = -1;

	if(g_diKeyState[DIK_NUMPAD7]& 0x80 
	|| g_diKeyState[DIK_NUMPAD8]& 0x80 
	|| g_diKeyState[DIK_NUMPAD9]& 0x80
	|| g_diKeyState[DIK_UP]& 0x80 )
		SrideScale.y = 1;

	if(g_diKeyState[DIK_NUMPAD7]& 0x80 
	|| g_diKeyState[DIK_NUMPAD4]& 0x80 
	|| g_diKeyState[DIK_NUMPAD1]& 0x80
	|| g_diKeyState[DIK_LEFT]& 0x80 )
		SrideScale.x = -1;

	if(g_diKeyState[DIK_NUMPAD9]& 0x80 
	|| g_diKeyState[DIK_NUMPAD6]& 0x80 
	|| g_diKeyState[DIK_NUMPAD3]& 0x80
	|| g_diKeyState[DIK_RIGHT]& 0x80 )
		SrideScale.x = 1;


	
}
