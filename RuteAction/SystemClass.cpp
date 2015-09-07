#include "Common.h"
#define GAMENHASI 200


//#define STAGEEDIT
//#define RANKINGWATCH

LPDIRECT3DDEVICE9 G_Device;
BYTE	g_InputState[INPUTSTATE_MAX];
BYTE	g_ExInputState[INPUTSTATE_MAX];

//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
System::System(bool isFullScreen,HWND H)
{		
	SCEARN = 0;

#ifdef RANKINGWATCH
	SCEARN = 2;
#endif

#ifdef STAGEEDIT
	SCEARN = 1;
#endif
	// Rendererオブジェクトの初期化
	hr = renderer.Initialize(H, isFullScreen, CLIENT_WIDTH, CLIENT_HEIGHT);
	// 描画の設定
	renderer.InitRender();

	::G_Device = renderer.getDevice();

	//サウンドクラスの初期化
	m_pSoundManager = new CSoundManager(H);
//	m_pSoundManager->Init(H);

	m_pSoundManager->AddDatafromFile("SOUND_BGM_NORMAL","SOUND/甲子園っぽい.wav",true,true);
	m_pSoundManager->AddDatafromFile("SOUND_BGM_BEGINNER","SOUND/ちゅ～とりある.wav",true,true);
	m_pSoundManager->AddDatafromFile("SOUND_BGM_PINCH","SOUND/甲子園っぽい時間ピンチ.wav",true,true);
	m_pSoundManager->AddDatafromFile("SOUND_BGM_TYTLE","SOUND/title.wav",true,true);

	m_pSoundManager->AddDatafromFile("SOUND_TYTLE_CURSOR", "SOUND/タイトルカーソル.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_TYTLE_DECIDE", "SOUND/決定.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_STAGE_START", "SOUND/開始.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_STAGE_TIMEUP", "SOUND/TIMEUP.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_STAGE_CARDGET", "SOUND/カードゲット.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_PLAYER_DEATH", "SOUND/断末魔.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_PLAYER_BAUND", "SOUND/バウンド.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_PLAYER_BAUND2", "SOUND/バウンド2.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_PLAYER_METAL", "SOUND/metal04.wav",false,false);
	m_pSoundManager->AddDatafromFile("SOUND_PLAYER_THROW", "SOUND/THROW.wav",false,false);

	nowTime = 0;	// 現在の時刻
	beforeTime = 0;	// 前回TGT_TIMERを送信した時刻
	srand((unsigned)timeGetTime());
	
	P1 = new Chara("Picture/STAND.png",0,0,0,80,80,&renderer,m_pSoundManager);
	
	P1->SetMass(10);
	P1->SetJoyNum(0);

	Stage = new StageClass(&renderer,P1,m_pSoundManager);


	Tytle = new TytleClass(&renderer);
	Ranking = new RankingClass(&renderer);
	SelectStage = 0;

	if(SCEARN == 0){
		m_pSoundManager->Stop("SOUND_BGM_BEGINNER");
		m_pSoundManager->Stop("SOUND_BGM_NORMAL");
		m_pSoundManager->PlayfromHead("SOUND_BGM_TYTLE");
	}
	/*if(SCEARN == 2)
		Ranking->FileLoad();*/

}	

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
System::~System()
{
	//m_pSoundManager->Destroy();
	delete m_pSoundManager;

	delete P1;
	delete Stage;
	delete Tytle;
	delete Ranking;
}

void System::Main()
{
#ifndef STAGEEDIT
	ScearnSet();
#endif
	switch(SCEARN)
	{
	case SCEARN_TYTLE:
		Tytle->main();
		break;
	case SCEARN_ACTION:

		if(Stage->EditSwitch == 0&&Stage->Poseflg == 0)
		{
			//入力//
			P1->SetAccel(0,0,0);
			if(Stage->TimeCount > 0)
			{
				#ifndef STAGEEDIT
				if(Stage->Iventflg[IVENT_STAGE_START])
				#endif

				{
				P1->Input();
				if(g_diKeyState[DIK_RETURN]& 0x80 &&!(g_ExdiKeyState[DIK_RETURN]& 0x80))
					Stage->PlayerDeath();
				}
				
				#ifndef STAGEEDIT
				else
					Stage->StageStart(Tytle->SelectStage);
				#endif
			}
			else if(P1->CharaState == CHARASTATE_STAND)
			{
				P1->ActionState = CHARAACTION_KO;
				P1->Actionflg = 1;
			}
			
			P1->SetVeloAction();

			//能動的動作//
			ActiveAction();
			//受動的動作//
			PassiveAction();
			//相互に干渉する動作//
			EachOtherAction();
		}

		
		Stage->Animation();

		SetCamera();	
		break;
	case SCEARN_RANKING:
		Ranking->main();
		break;
	case SCEARN_RANKING2:
		Ranking->main();
		break;
	default :
		break;
	}

}

void System::SetInputState(){

	for(int i = 0;i < INPUTSTATE_MAX;i++){
		g_ExInputState[i] = g_InputState[i];
		g_InputState[i] = false;
	}

	if((g_diJoyState2[0] & BUTTON_START)||g_diKeyState[DIK_SPACE])
		g_InputState[INPUTSTATE_START] = true;
	
	if((g_diJoyState2[0] & BUTTON_UP)||g_diKeyState[DIK_UP])
		g_InputState[INPUTSTATE_UP] = true;

	if((g_diJoyState2[0] & BUTTON_DOWN)||g_diKeyState[DIK_DOWN])
		g_InputState[INPUTSTATE_DOWN] = true;

	if((g_diJoyState2[0] & BUTTON_LEFT)||g_diKeyState[DIK_LEFT])
		g_InputState[INPUTSTATE_LEFT] = true;

	if((g_diJoyState2[0] & BUTTON_RIGHT)||g_diKeyState[DIK_RIGHT])
		g_InputState[INPUTSTATE_RIGHT] = true;

	if((g_diJoyState2[0] & BUTTON_B)||g_diKeyState[DIK_C]){
		g_InputState[INPUTSTATE_OK] = true;
		g_InputState[INPUTSTATE_MOVE] = true;
	}

	if((g_diJoyState2[0] & BUTTON_C)||g_diKeyState[DIK_Z]){
		g_InputState[INPUTSTATE_CANCEL] = true;
		g_InputState[INPUTSTATE_RIDE] = true;
		g_InputState[INPUTSTATE_JUMP] = true;
	}

	if((g_diJoyState2[0] & BUTTON_X)||g_diKeyState[DIK_X]){
		g_InputState[INPUTSTATE_OK] = true;
		g_InputState[INPUTSTATE_THROW] = true;
	}

	if((g_diJoyState2[0] & BUTTON_L)||g_diKeyState[DIK_F])
		g_InputState[INPUTSTATE_THROW_UPSPIN] = true;

	if((g_diJoyState2[0] & BUTTON_R)||g_diKeyState[DIK_V])
		g_InputState[INPUTSTATE_THROW_DOWNSPIN] = true;

}

void System::Input()
{
	/* キーボードの状態を取得 */
	GetKeyboardState();
	// ジョイパッドの状態を取得
	GetJoypadState2();
	// マウスの状態を取得
    GetMouseState();

	MState.exlButton = MState.lButton;
	MState.exrButton = MState.rButton;
	MState.excButton = MState.cButton;
	MState.x += (g_diMouseState.lX * MState.moveAdd);
	MState.y += (g_diMouseState.lY * MState.moveAdd);
	if ( MState.x < MState.moveRect.left ) MState.x = MState.moveRect.left;
	if ( MState.x > MState.moveRect.right - MState.imgWidth ) MState.x = MState.moveRect.right - MState.imgWidth;
	if ( MState.y < MState.moveRect.top ) MState.y = MState.moveRect.top;
	if ( MState.y > MState.moveRect.bottom - MState.imgHeight ) MState.y = MState.moveRect.bottom - MState.imgHeight;
	(g_diMouseState.rgbButtons[0] & 0x80) ? MState.lButton = true : MState.lButton = false;
	(g_diMouseState.rgbButtons[1] & 0x80) ? MState.rButton = true : MState.rButton = false;
	(g_diMouseState.rgbButtons[2] & 0x80) ? MState.cButton = true : MState.cButton = false;

	
	switch(SCEARN)
	{
	case SCEARN_TYTLE:
		Tytle->Input();break;
	case SCEARN_ACTION:
		#ifndef STAGEEDIT
		if(g_InputState[INPUTSTATE_START]&&!(g_ExInputState[INPUTSTATE_START])
			&&Stage->Count[IVENT_STAGE_CLEAR] == 150&&Stage->Count[IVENT_STAGE_TIMEUP] == 150)
		{
			if(Stage->Poseflg)
			{
				Stage->Poseflg = 0;
				Stage->MessageTex->SetPos(0,0,100000);
			}
			else
				Stage->Poseflg = 1;
		}
		if(Stage->Poseflg)
		{
			if((g_InputState[INPUTSTATE_RIGHT]&&!(g_ExInputState[INPUTSTATE_RIGHT]))
			||(g_InputState[INPUTSTATE_LEFT]&&!(g_ExInputState[INPUTSTATE_LEFT])))
			{
				if(Stage->PauseSelect)
					Stage->PauseSelect = 0;
				else
					Stage->PauseSelect = 1;
			}

			if(g_InputState[INPUTSTATE_OK]&&!(g_ExInputState[INPUTSTATE_OK]))
			{
				if(Stage->PauseSelect == 0)
				{
					Stage->Poseflg = 0;
					Stage->MessageTex->SetPos(0,0,100000);
				}
				/*else
					Stage->PauseSelect = 1;*/
			}
		}
		else
			Stage->PauseSelect = 0;
		#endif
		
		#ifdef STAGEEDIT
			Input_Edit();break;
		#endif
		break;
	case SCEARN_RANKING:
		Ranking->Input();
		break;
	case SCEARN_RANKING2:
		Ranking->Input();
		break;
	default :
		break;
	}
	
}

void System::Paint()
{
	switch(SCEARN)
	{
	case SCEARN_TYTLE:
		Paint_Tytle();break;
	case SCEARN_ACTION:
		Paint_Action();break;
	case SCEARN_RANKING:
		Paint_Ranking();break;
	case SCEARN_RANKING2:
		Paint_Ranking();break;
	default :
		break;
	}
}

void System::SetCamera()
{
	//カメラ位置の設定///////////////////////////////////////////////////////////////
	if(Stage->EditSwitch == 0)
	{
		/*if(Stage->TimeCount == 0)
			Stage->SaveScore();*/
		if(Stage->MessageTex->GetPos()->z >= 0)
		Stage->SecondCount++;
		if(Stage->SecondCount > 60&&Stage->TimeCount > 0)
		{
			Stage->SecondCount = 0;
			Stage->TimeCount--;
		}
		if(Stage->TimeCount < Stage->TimeCount_MAX/5&&Stage->Iventflg[IVENT_STAGE_PINCH] == 0)
		{
			m_pSoundManager->Stop("SOUND_BGM_NORMAL");
			Stage->TimeAlarm(Tytle->SelectStage);
		}
		if(Stage->TimeCount < Stage->TimeCount_MAX/5&&Stage->Pinchflg == 0)
		{
			m_pSoundManager->Stop("SOUND_BGM_NORMAL");
			m_pSoundManager->PlayfromHead("SOUND_BGM_PINCH");
			Stage->Pinchflg = 1;
		}

		#ifdef STAGEEDIT
			Stage->TimeCount = TIMEMAX;
		#endif

		renderer.SetCPos(P1->GetPos()->x,P1->CameraWork,renderer.GetCPos()->z);
		renderer.SetCFocus(P1->GetPos()->x,renderer.GetCPos()->y,0);

		if(P1->GetPos()->y > 0)
		{
			renderer.SetCPos(P1->GetPos()->x,P1->GetPos()->y + P1->CameraWork,renderer.GetCPos()->z);
			renderer.SetCFocus(P1->GetPos()->x,P1->GetPos()->y + P1->CameraWork,0);
		}
	}

	Stage->Back->TexClass->SetPos(renderer.GetCPos()->x,renderer.GetCPos()->y,1000);
	Stage->SetStatus();	
	Stage->CardNumtex[0]->SetPos(renderer.GetCPos()->x + 350 - 100,renderer.GetCPos()->y + 250 - 30,-100);
	Stage->CardNumtex[1]->SetPos(renderer.GetCPos()->x + 350 - 140,renderer.GetCPos()->y + 250 - 30,-100);

	Stage->CardNumtex2[0]->SetPos(renderer.GetCPos()->x + 350 - 40,renderer.GetCPos()->y + 250 - 80,-100);
	Stage->CardNumtex2[1]->SetPos(renderer.GetCPos()->x + 350 - 60,renderer.GetCPos()->y + 250 - 80,-100);
	/////////////////////////////////////////////////////////////////////////////////
}
void System::ActiveAction()
{
	P1->VeloCal();
}
void System::PassiveAction()
{
	if(P1->GetPos()->y + P1->GetVelo()->y< -400
	&& P1->GetPos()->y > -400)
	m_pSoundManager->Play("SOUND_PLAYER_DEATH");
		
	if(P1->GetPos()->y < DEADLINE)
		Stage->PlayerDeath();
}
void System::EachOtherAction()
{
	int testswitch = 0;
	bool Hitflg = 0;
	for(list<Block*>::iterator iter = Stage->M_Block->Blocklist.begin(); iter != Stage->M_Block->Blocklist.end(); iter++)
		{
			if((*iter)->Type != NULL)
			{
				if(P1->Bullettest)
				{
					int i = 0;
				
					if((*iter)->Shape != BLOCKSHAPE_OBJECT
						&& (*iter)->Type != BLOCKTYPE_RED)
					if(CheckTexinTex(P1->Bul[P1->SelectBullet],(*iter)))
					{
						P1->SetdownBullet();
						testswitch = 1;
					}
				
				}
				if((*iter)->Type != BLOCKTYPE_NULL)
				{
					if(
						((*iter)->Type != BLOCKTYPE_BRUE || (*iter)->Shape == BLOCKSHAPE_OBJECT)
						&&CheckTexinScrean((*iter),P1->GetPos())
						)
					{
						if(P1->CharaState == CHARASTATE_STAND||P1->CharaState == CHARASTATE_DOWN)
							WalkStair(P1,(*iter));
						int a = 0;
						if((*iter)->Shape != BLOCKSHAPE_OBJECT)//ブロックがオブジェクトでないならば
							a = Stage->M_Block->BlockHit(P1,(*iter));//当たり判定
						else
							Stage->ObjectHit(P1,(*iter));
						if(a)
						{
							if(P1->CatchandMoveflg)
							{
								P1->SetVelo(0,0,0);
							}
							Hitflg = 1;
							P1->HitState = a;
							if(P1->CharaState == CHARASTATE_RIDE)
							{
								P1->SetVelo(P1->GetVelo()->x/SAITOU,P1->GetVelo()->y/SAITOU,0);
								P1->CharaState = CHARASTATE_FALL;
								//Stage->M_Block->BlockHit(P1,(*iter));
							}

							if(P1->HitState == HIT_UNDER)
							{
								if(P1->CharaState == CHARASTATE_DOWN&&(g_InputState[INPUTSTATE_DOWN]))
									P1->CharaState = CHARASTATE_DOWN;
								/*else if(P1->CharaState == CHARASTATE_DENGURI&&(g_InputState[INPUTSTATE_DOWN]))
									P1->CharaState = CHARASTATE_DENGURI;*/
								else if(P1->CharaState != CHARASTATE_KO)
								{
									P1->CharaState = CHARASTATE_STAND;
									P1->SetEdgeScale(P1->TexTable[CHARASTATE_STAND]->EdgeScale);
									for(list<Block*>::iterator iter2 = Stage->M_Block->Blocklist.begin(); iter2 != Stage->M_Block->Blocklist.end(); iter2++)
										if(CheckTexinScrean((*iter2),P1->GetPos())
										&&((*iter2)->Type != BLOCKTYPE_BRUE && (*iter2)->Shape != BLOCKSHAPE_OBJECT)&&(*iter2)->Type != BLOCKTYPE_NULL
												&&(*iter)!=(*iter2))
									{
										if((CheckTexinTex(P1,(*iter2)) || CheckTexinTex((*iter2),P1)))
										{
											P1->CharaState = CHARASTATE_DOWN;
											P1->SetEdgeScale(P1->TexTable[CHARASTATE_DOWN]->EdgeScale);
										}
									}
								}
							}

							if(P1->HitState == HIT_UNDER_BAUND)
								P1->CharaState = CHARASTATE_JUMP;

						}
					}
					if((*iter)->Type != BLOCKTYPE_RED
						&&(*iter)->Shape != BLOCKSHAPE_OBJECT)
					{
						for(int i = 0;i < BULLETMAX;i++)
						{
							if(CheckTexinScrean((*iter),P1->Bul[i]->GetPos())
							//ブロックがオブジェクトでないならば
							&&P1->Bul[i]->ReflectCount[BULLET] < 5
							&&(P1->Bul[i]->GetVelo()->x != 0||P1->Bul[i]->GetVelo()->y != 0))
							{
								Stage->M_Block->BlockHit_Reflect(P1->Bul[i],(*iter));
							}
//								P1->Bul[i]->ReflectCount[BULLET]++;
							
						/*	if(P1->Bul[i]->GoSignal != 0)
								if(P1->Bul[i]->ReflectCount[CAR] < 5)
									if(Stage->M_Block->BlockHit_Reflect(P1->Bul[i]->Car,(*iter)))
										P1->Bul[i]->ReflectCount[CAR]++;*/
						}
					}
				}
			}
		}
	if(P1->CatchandMoveflg)
							P1->CatchandMoveflg = 0;
	if(P1->Bullettest)
	{
		P1->Bullettest = 0;
		if(testswitch == 0)
		if(P1->SelectBullet == BULLETMAX - 1)
			P1->SelectBullet = 0;
		else
			P1->SelectBullet ++;
	}

	for(list<Block*>::iterator iter = Stage->M_Block->Blocklist.begin(); iter != Stage->M_Block->Blocklist.end(); iter++)
	{
		if((*iter)->Type != BLOCKTYPE_NULL
		&&(*iter)->Type != BLOCKTYPE_BRUE 
		&& (*iter)->Shape != BLOCKSHAPE_OBJECT
		&&CheckTexinScrean((*iter),P1->GetPos())
		)
		Stage->M_Block->BlockHit2(P1,(*iter));
	}
	
		if(Hitflg == 0)
			P1->HitState = 0;
		P1->SetCharaState();
		if(P1->RideBullet != NULL)
		{
				P1->Move();
		}
		else
			P1->Move();
		for(int i = 0;i < BULLETMAX;i++)
		{
			P1->Bul[i]->Move();
		}
}



void System::Input_Edit()
{
	if(g_diKeyState[DIK_RETURN]& 0x80 &&!(g_ExdiKeyState[DIK_RETURN]& 0x80))
	{
		if(Stage->EditSwitch == 0)
		{
			Stage->EditSwitch = 1;
		}
		else
		{
			if(g_diKeyState[DIK_LCONTROL]& 0x80||g_diKeyState[DIK_RCONTROL]& 0x80)
				Stage->SetPlayerStart();
			Stage->EditSwitch = 0;
		}
	}

	if(Clickflg[0][0])
	{
		Stage->FileSave();
	}

	if(Clickflg[0][1])
	{
		Stage->FileLoad();
	}

	if(Clickflg[1][0])
	{
		Stage->CapterSave();
	}

	if((Clickflg[1][1])
	|| (g_diKeyState[DIK_L]& 0x80&&!(g_ExdiKeyState[DIK_L]& 0x80)))
	{
		Stage->CapterLoad();
	}

	if(g_diKeyState[DIK_R]& 0x80 &&!(g_ExdiKeyState[DIK_R]& 0x80))
	{
		Stage->ResetScore();
	}

	/*if(g_diKeyState[DIK_S]& 0x80 &&!(g_ExdiKeyState[DIK_S]& 0x80))
	{
		Stage->SaveScore();
	}*/


	for(int i = 0;i < BUTTONNUM;i++)
			for(int j = 0;j < 2;j++)
				Clickflg[i][j] = false;
	
	Stage->M_Block->MouseCursor->SetPos(
		(float)m_MouseLocalPos.x - CLIENT_WIDTH/2 + renderer.GetCPos()->x,
		(float)-m_MouseLocalPos.y + CLIENT_HEIGHT/2 + renderer.GetCPos()->y,
		0);
	if(Stage->EditSwitch == 1)
	Stage->M_Block->Input();

	Stage->Back->SelectIndex = SendMessage(*Stage->Back->hCmb, CB_GETCURSEL , 0 , 0);
	//ブロックの画像を選択時
	if(Stage->Back->SelectIndex > 0)
	{
		Stage->Back->TexClass->SetpTexture(&Stage->Back->TexTable[Stage->Back->SelectIndex]->pTex);
	}

}

void System::Paint_Action()
{
	///////描画//////////

	if(Stage->Back->SelectIndex > BACKTYPENULL)
		renderer.Render(Stage->Back->TexClass);


	for(list<Block*>::iterator iter = Stage->M_Block->Blocklist.begin(); iter != Stage->M_Block->Blocklist.end(); iter++)
	{
		if((*iter)->Type != BLOCKTYPE_NULL
			&&(((*iter)->Type != OBJECTTYPE_START||(*iter)->Shape != BLOCKSHAPE_OBJECT)||Stage->EditSwitch == 1))
		{
			renderer.Render(*iter);
		}
	}

	renderer.Render(Stage->CheckPoint_Font);
	
	for(int i = 0;i < BULLETMAX;i++)
		for(int j = 0;j < RUTEPOINTMAX -1 ;j++)
			renderer.Render(P1->Bul[i]->RuteRibon[j]);
	for(int i = 0;i < BULLETMAX;i++)
		renderer.Render(P1->Bul[i]);
	for(int i = 0;i < BULLETMAX;i++)
	{
		renderer.Render(P1->Bul[i]->Car);
		P1->Bul[i]->Bectle->SetPos(P1->Bul[i]->Car->GetPos()->x,P1->Bul[i]->Car->GetPos()->y,0);
		if(P1->Bul[i]->Car->GetVelo()->x != 0||P1->Bul[i]->Car->GetVelo()->y != 0)
		P1->Bul[i]->Bectle->SetRot(0,0,
								P1->Bul[i]->GetAngle(P1->Bul[i]->Car->GetVelo()->x,P1->Bul[i]->Car->GetVelo()->y) - (float)PAI/2);
		renderer.Render(P1->Bul[i]->Bectle);
	}
	
	renderer.Render(P1);

	if(P1->RideBullet)
	{
	renderer.Render(P1->RideBullet->Car);
	renderer.Render(P1->RideBullet->Bectle);
	}
//	if(P1->)
	/*renderer.Render(Stage->CardNumtex[0]);
	renderer.Render(Stage->CardNumtex[1]);*/

	if(Stage->EditSwitch != 1)
	{
		//renderer.Render(Stage->Tex_Flame);
		renderer.Render(Stage->CardNumtex[0]);
		renderer.Render(Stage->CardNumtex[1]);
		renderer.Render(Stage->CardNumtex2[0]);
		renderer.Render(Stage->CardNumtex2[1]);
		renderer.Render(Stage->Tex_TimeStatic);
		renderer.Render(Stage->Tex_Gage);
		renderer.Render(Stage->Tex_TimeBack);
		renderer.Render(Stage->Tex_TimeMove);
	}

	if(Stage->Poseflg)
	{
		renderer.Render(Stage->Tex_PAUSECHOICE[0]);
		renderer.Render(Stage->Tex_PAUSECHOICE[1]);
	}

	renderer.Render(Stage->MessageTex);

	#ifdef STAGEEDIT
	renderer.Render(Stage->M_Block->MouseCursor);
	#endif
	/////////////////////
}

void System::Paint_Tytle()
{
	renderer.Render(Tytle->TitleBack);
	for(int i = 0;i < CHOICEMEM_MAX;i++)
		renderer.Render(Tytle->TitleChoices[i]);
}

void System::Paint_Ranking()
{
	renderer.Render(Ranking->Tex_RankBack);
	renderer.Render(Ranking->NewEmblemLine);
	renderer.Render(Ranking->NewEmblem);
	for(int i = 0;i < RANKINGMAX;i++)
	{
		renderer.Render(Ranking->Tex_RankNum[i]);
		renderer.Render(Ranking->Tex_RankScore[i][0]);
		renderer.Render(Ranking->Tex_RankScore[i][1]);
		renderer.Render(Ranking->Tex_Clear[i]);
		for(int k = 0;k < 4;k++)
		renderer.Render(Ranking->Tex_Time[i][k]);
		renderer.Render(Ranking->Tex_Slash[i]);
		renderer.Render(Ranking->Card[i]);
		renderer.Render(Ranking->BATSU[i]);
	}
	renderer.Render(Ranking->Tex_Score);
	renderer.Render(Ranking->Tex_Level);
	renderer.Render(Ranking->Rankin);
	renderer.Render(Ranking->NameEnd);
	for(int i = 0;i < RANKINGMAX;i++)
	{
		for(int p = 0;p < 3;p++)
		{
			renderer.Render(Ranking->RankingName[i][p]);
		}
	}
}

void System::ScearnSet()
{
	switch(SCEARN)
	{
	case SCEARN_TYTLE:
		if(Tytle->SelectStage != -1)
		{
			/*delete P1;
			P1 = new Chara("Picture/STAND.png",0,0,0,80,80,&renderer);*/
			P1->Init();
			/*P1->SetMass(10);
			P1->SetJoyNum(0);*/
			delete Stage;
			Stage = new StageClass(&renderer,P1,m_pSoundManager);
			SCEARN = SCEARN_ACTION;
			SendMessage(*Stage->hCmb_Stage, CB_SETCURSEL,Tytle->SelectStage,0);
			Stage->FileLoad();
			Stage->SetPlayerStart();
			Stage->Back->TexClass->SetpTexture(&Stage->Back->TexTable[Stage->Back->SelectIndex]->pTex);
			Stage->EditSwitch = 0;
			Stage->TimeCount_MAX = (int)((Tytle->SelectStage == 0)*TIMEMAX1
								 + (Tytle->SelectStage == 1)*TIMEMAX2
								 + (Tytle->SelectStage == 2)*TIMEMAX3);

			Stage->TimeCount = Stage->TimeCount_MAX;
			m_pSoundManager->Stop("SOUND_BGM_TYTLE");
		}
		if(Tytle->Rankingflg)
		{
			Ranking->Init();
			Ranking->Count[COUNT_RANK_VIEW] = TIME_RANKSHOW;
			Ranking->FileLoad(SelectStage,0);
			Ranking->NewEmblem->SetPos(0,0,100);
			Ranking->NewEmblemLine->SetPos(0,0,100);
			Ranking->NewRank = - 1;
			SCEARN = SCEARN_RANKING2;
		}
		break;
	case SCEARN_ACTION:
		if(Stage->Poseflg)
		{
			Stage->Pause();
		}

		if((g_InputState[INPUTSTATE_OK]&&!(g_ExInputState[INPUTSTATE_OK]))
			&& Stage->Poseflg &&Stage->PauseSelect)
		{
			renderer.SetCPos(0,0,renderer.GetCPos()->z);
			renderer.SetCFocus(0,0,0);
			delete Tytle;
			Tytle = new TytleClass(&renderer);
			SCEARN = SCEARN_TYTLE;
			m_pSoundManager->Stop("SOUND_BGM_BEGINNER");
			m_pSoundManager->Stop("SOUND_BGM_NORMAL");
			m_pSoundManager->PlayfromHead("SOUND_BGM_TYTLE");
		}

		if(Stage->ColectedCard >= Stage->TotalCard)
		{
			if(Stage->Iventflg[IVENT_STAGE_CLEAR])
			{
				
					Ranking->Init();
					Ranking->Count[COUNT_RANK_VIEW] = TIME_RANKSHOW;
					SCEARN = SCEARN_RANKING;
					renderer.SetCPos(0,0,renderer.GetCPos()->z);
					renderer.SetCFocus(0,0,0);
					Stage->SaveScore(Tytle->SelectStage);
					Ranking->FileLoad(Tytle->SelectStage,1);
//					g_pPerformance->Stop(NULL, NULL, 0, 0);
					Ranking->NewEmblem->SetPos(0,0,100);
					Ranking->NewEmblemLine->SetPos(0,0,100);
				
			}
			else
				Stage->StageClear();
		}	

		if(Stage->TimeCount <= 0)
		{
			if(Stage->Iventflg[IVENT_STAGE_TIMEUP])
			{
				
					Ranking->Init();
					Ranking->Count[COUNT_RANK_VIEW] = TIME_RANKSHOW;
					SCEARN = SCEARN_RANKING;

					Ranking->NewScore = Stage->ColectedCard;
					Ranking->NewTime = Stage->TimeCount_MAX - Stage->TimeCount;
					Ranking->ScoreMAX = Stage->TotalCard;

					renderer.SetCPos(0,0,renderer.GetCPos()->z);
					renderer.SetCFocus(0,0,0);
					Stage->SaveScore(Tytle->SelectStage);
					Ranking->FileLoad(Tytle->SelectStage,1);
//					g_pPerformance->Stop(NULL, NULL, 0, 0);
					Ranking->NewEmblem->SetPos(0,0,100);
					Ranking->NewEmblemLine->SetPos(0,0,100);
			
			}
			else
				Stage->TimeUp();
		}
		break;
	case SCEARN_RANKING:
		if(Ranking->Count[COUNT_RANK_VIEW] <= 0)
		{
			Ranking->FileSave(Tytle->SelectStage);
			delete Tytle;
			Tytle = new TytleClass(&renderer);
			SCEARN = SCEARN_TYTLE;
			m_pSoundManager->Stop("SOUND_BGM_BEGINNER");
			m_pSoundManager->Stop("SOUND_BGM_NORMAL");
			m_pSoundManager->PlayfromHead("SOUND_BGM_TYTLE");
		}
		break;
	case SCEARN_RANKING2:
		if(Ranking->Count[COUNT_RANK_VIEW] <= 0)
		{
			switch(SelectStage)
			{
			case 0:
					SelectStage++;
					Ranking->Init();
					Ranking->Count[COUNT_RANK_VIEW] = TIME_RANKSHOW;
					Ranking->FileLoad(SelectStage,0);
					Ranking->NewEmblem->SetPos(0,0,100);
					Ranking->NewEmblemLine->SetPos(0,0,100);
					Ranking->NewRank = - 1;
					break;
			case 1:
					SelectStage++;
					Ranking->Init();
					Ranking->Count[COUNT_RANK_VIEW] = TIME_RANKSHOW;
					Ranking->FileLoad(SelectStage,0);
					Ranking->NewEmblem->SetPos(0,0,100);
					Ranking->NewEmblemLine->SetPos(0,0,100);
					Ranking->NewRank = - 1;
					break;
			case 2:
				SelectStage = 0;
				delete Tytle;
				Tytle = new TytleClass(&renderer);
				SCEARN = SCEARN_TYTLE;
				break;
			}
		}
		break;
	}
}

