//=============================================================
// StageClass.cpp
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
StageClass::StageClass()
{
	
}

StageClass::StageClass(Renderer *R,Chara *C,CSoundManager * pSoundManager)
{
	m_pSoundManager = pSoundManager;

	for(int i = 0;i < IVENT_STAGE_MAX;i++)
	Iventflg[i] = 0;

	Count[IVENT_STAGE_PINCH] = 60;
	Count[IVENT_STAGE_START] = 30;
	Count[IVENT_STAGE_TIMEUP] = 150;
	Count[IVENT_STAGE_CLEAR] = 150;
	Count[IVENT_STAGE_READY] = 80;

	CardNumtex[0] = new Texture("Picture/NUMBER/0.png",400 - 60,300 - 60,1000,60,60,R);
	CardNumtex[1] = new Texture("Picture/NUMBER/0.png",400 - 100,300 - 60,1000,60,60,R);
	CardNumtex2[0] = new Texture("Picture/NUMBER/0.png",400 - 160,300 - 60,1000,30,30,R);
	CardNumtex2[1] = new Texture("Picture/NUMBER/0.png",400 - 200,300 - 60,1000,30,30,R);
	Tex_Flame = new Texture("Picture/STATUSFLAME.png",400 - 100,300 - 60,1000,60,60,R);
	Tex_Gage = new Texture("Picture/gage_green.png",400 - 100,300 - 60,1000,200,200,R);
	Tex_TimeBack = new Texture("Picture/gage_waku.png",0,0,-100,300,300,R);
	Tex_TimeStatic = new Texture("Picture/gage_black.png",300,240,-100,200,200,R);
	Tex_TimeMove = new Texture("Picture/hari.png",0,0,-100,55,55,R);
	MessageTex =  new Texture("Picture/START_READY.png",0,0,100000,100,600,R);
	for(int i = 0;i < IVENT_STAGE_MAX;i++)
		IventTexTable[i] = new TextureData();

	CardNumtex[0]->SetpTexturefromFile(&IventTexTable[IVENT_STAGE_READY]->pTex,"Picture/START_READY.png");
	CardNumtex[1]->SetpTexturefromFile(&IventTexTable[IVENT_STAGE_START]->pTex,"Picture/START_GO.png");
	CardNumtex[2]->SetpTexturefromFile(&IventTexTable[IVENT_STAGE_PINCH]->pTex,"Picture/HULLY.png");
	CardNumtex[3]->SetpTexturefromFile(&IventTexTable[IVENT_STAGE_TIMEUP]->pTex,"Picture/TIMEUP.png");
	CardNumtex[4]->SetpTexturefromFile(&IventTexTable[IVENT_STAGE_CLEAR]->pTex,"Picture/GAMECLEAR.png");
	CardNumtex[4]->SetpTexturefromFile(&IventTexTable[IVENT_STAGE_PAUSE]->pTex,"Picture/font_pause.png");

	CheckPoint_Font = new Texture("Picture/check_font.png",0,0,-100000,60,60,R);

	CheckPoint_Font->SetMyVertex((float)(11 + 35)/64/6,(float)11/64/6);

	for(int i = 0;i < 10;i++)
		NumberTexTable[i] = new TextureData();

	for(int i = 0;i < 2;i++)
		GageKind[i] = new TextureData();

	CardNumtex[0]->SetpTexturefromFile(&NumberTexTable[0]->pTex,"Picture/NUMBER/0.png");
	CardNumtex[1]->SetpTexturefromFile(&NumberTexTable[1]->pTex,"Picture/NUMBER/1.png");
	CardNumtex[2]->SetpTexturefromFile(&NumberTexTable[2]->pTex,"Picture/NUMBER/2.png");
	CardNumtex[3]->SetpTexturefromFile(&NumberTexTable[3]->pTex,"Picture/NUMBER/3.png");
	CardNumtex[4]->SetpTexturefromFile(&NumberTexTable[4]->pTex,"Picture/NUMBER/4.png");
	CardNumtex[5]->SetpTexturefromFile(&NumberTexTable[5]->pTex,"Picture/NUMBER/5.png");
	CardNumtex[6]->SetpTexturefromFile(&NumberTexTable[6]->pTex,"Picture/NUMBER/6.png");
	CardNumtex[7]->SetpTexturefromFile(&NumberTexTable[7]->pTex,"Picture/NUMBER/7.png");
	CardNumtex[8]->SetpTexturefromFile(&NumberTexTable[8]->pTex,"Picture/NUMBER/8.png");
	CardNumtex[9]->SetpTexturefromFile(&NumberTexTable[9]->pTex,"Picture/NUMBER/9.png");

	CardNumtex[0]->SetpTexturefromFile(&GageKind[0]->pTex,"Picture/gage_green.png");
	CardNumtex[0]->SetpTexturefromFile(&GageKind[1]->pTex,"Picture/gage_red.png");

	LPCTSTR  TexName[BACKTYPEMAX] = {
	"BACKTYPENULL",
	"MORI",
	"UMI",
	"SORA"
	};

	CheckPoint_Count_Stream = 0;
	CheckPoint_Count_Blink = 0;

	CheckPointNow = NULL;

	M_Block = new BlockManager(R,m_pSoundManager);

	Back = new BACK();

	Back->TexClass = new Texture("Picture/NULL.png",0,0,0,1425,800*1425/600,R);

	Back->hCmb = &combo[COMBO_USE_BACK];
	hCmb_Stage = &combo[COMBO_USE_STAGE];
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,1,(LPARAM)"Stage1");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,2,(LPARAM)"Stage2");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,3,(LPARAM)"Stage3");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,4,(LPARAM)"Stage4");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,5,(LPARAM)"Stage5");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,6,(LPARAM)"Stage6");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,7,(LPARAM)"Stage7");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,8,(LPARAM)"Stage8");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,9,(LPARAM)"Stage9");
	SendMessage(*(hCmb_Stage) , CB_INSERTSTRING  ,10,(LPARAM)"Stage10");



	SendMessage(*(hCmb_Stage) , CB_DELETESTRING  ,0,0);
	SendMessage(*(hCmb_Stage) , CB_SETCURSEL  , 0 , 0);

	hCmb_Capter = &combo[COMBO_USE_CAPTER];
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,1,(LPARAM)"Capter1");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,2,(LPARAM)"Capter2");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,3,(LPARAM)"Capter3");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,4,(LPARAM)"Capter4");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,5,(LPARAM)"Capter5");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,6,(LPARAM)"Capter6");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,7,(LPARAM)"Capter7");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,8,(LPARAM)"Capter8");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,9,(LPARAM)"Capter9");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,10,(LPARAM)"Capter10");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,11,(LPARAM)"Capter11");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,12,(LPARAM)"Capter12");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,13,(LPARAM)"Capter13");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,14,(LPARAM)"Capter14");
	SendMessage(*(hCmb_Capter) , CB_INSERTSTRING  ,15,(LPARAM)"Capter15");
	
	SendMessage(*(hCmb_Capter) , CB_DELETESTRING  ,0,0);
	SendMessage(*(hCmb_Capter) , CB_SETCURSEL  , 0 , 0);

	for(int i = 0;i < BACKTYPEMAX;i++)
		Back->TexTable[i] = new TextureData();
	
//	Back->TexClass->Create();
	Back->TexClass->SetpTexturefromFile(&Back->TexTable[MORI]->pTex,"Picture/BACK/MORI.bmp");	
	Back->TexClass->SetpTexturefromFile(&Back->TexTable[UMI]->pTex,"Picture/BACK/UMI.png");
	Back->TexClass->SetpTexturefromFile(&Back->TexTable[SORA]->pTex,"Picture/BACK/SORA.png");

	for(int i = 1;i < BACKTYPEMAX;i++)
		SendMessage(*(Back->hCmb) , CB_INSERTSTRING  ,i,(LPARAM)TexName[i]);

	EditSwitch = 1;

	Player = C; 
	ColectedCard = 0;
	TotalCard = 0;
	TimeCount_MAX = TIMEMAX;
	TimeCount = TIMEMAX;
	Pinchflg = 0;
	SecondCount = 0;
	for(int i = 0;i < RANKINGMAX;i++)
	{
		for(int p = 0;p < 3;p++)
		{
			WordCord[i][p] = 0;
		}
	}
	Tex_PAUSECHOICE[0] = new Texture("Picture/Pause_Play.png",-100,- 60,-100,400,400,R);
	Tex_PAUSECHOICE[1] = new Texture("Picture/Pause_Return.png",100,- 60,-100,400,400,R);
	Poseflg = 0;
	PauseSelect = 0;
}
//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
StageClass::~StageClass()
{
	delete Tex_TimeBack;
	delete Tex_TimeStatic;
	delete Tex_TimeMove;
	delete Tex_Flame;
	delete Tex_Gage;
	delete CardNumtex[0];
	delete CardNumtex[1];
	delete CardNumtex2[0];
	delete CardNumtex2[1];
	delete MessageTex;
	for(int i = 0;i < IVENT_STAGE_MAX;i++)
		delete IventTexTable[i];
	for(int i = 0;i < 10;i++)
		delete NumberTexTable[i];
	for(int i = 0;i < 2;i++)
		delete GageKind[i];
	delete CheckPoint_Font;

	delete M_Block;
	delete Back->TexClass;
	for(int i = 0;i < BACKTYPEMAX;i++)
		delete Back->TexTable[i];
	delete Back;
	delete Tex_PAUSECHOICE[0];
	delete Tex_PAUSECHOICE[1];
}

void StageClass::SetStPos(float x, float y, float z)
{
	StPos.x = x;
	StPos.y = y;
	StPos.z = z;
}

bool StageClass::ResetScore()
{
	FILE *fp;
	
	if((fp = fopen("Score/Ranking.txt","w")) == NULL)
		return 0;
	else
	{
		for(int i = 0;i < RANKINGMAX;i++)
		fprintf(fp,"%d %d\n",0,0);
		fclose(fp);
		return 1;
	}
}
bool StageClass::SaveScore(int stage)
{
	int ranking[RANKINGMAX] = {0};
	int lefttime[RANKINGMAX] = {0};
	int NewRank = -1;

	char TxtName[64];
	sprintf_s(TxtName, 64, "Score/Ranking%d.txt", stage);

	FILE *fp;
	if((fp = fopen(TxtName,"r")) == NULL)
		return 0;
	else
	{
		/*for(int i = 0;i < RANKINGMAX;i++)
		{
			for(int p = 0;p < 3;p++)
			{
				WordCord[i][p] = 0;
			}
		}*/
		//ColectedCard = 5;
		int a1,a2;
		fscanf(fp,"%d %d",&a1,&a2);
		for(int i = 0;fscanf(fp,"%d %d %d %d %d",&WordCord[i][0],&WordCord[i][1],&WordCord[i][2],&ranking[i],&lefttime[i]) == 1||i < RANKINGMAX;i++){}
		for(int i = RANKINGMAX - 1;i >= 0;i--)
		{
			if(ColectedCard < ranking[i]
			|| (ColectedCard == ranking[i]&&TimeCount_MAX - TimeCount >= lefttime[i]))
			{
				if(i < RANKINGMAX - 1)
				{
					if(i < RANKINGMAX - 2)
					{
						for(int p = RANKINGMAX - 3;p >= i;p--)
						{
							ranking[p + 2] = ranking[p + 1];
							lefttime[p + 2] = lefttime[p + 1];
							
							for(int j = 0;j < 3;j++)
							{
									WordCord[p + 2][j] = WordCord[p + 1][j];
							}
							
						}
					}
					NewRank = i + 1;
					ranking[i + 1] = ColectedCard;
					lefttime[i + 1] = TimeCount_MAX - TimeCount;
					for(int j = 0;j < 3;j++)
					{
						WordCord[i + 1][j] = WORDCORD_SPACE;
					}
					WordCord[i + 1][0] = 0;
				}
				break;
			}
		}
		if(ColectedCard > ranking[0]
		   ||(ColectedCard == ranking[0]&&TimeCount_MAX - TimeCount < lefttime[0]))
			{
				for(int p = RANKINGMAX - 2;p >= 0;p--)
					if(p != RANKINGMAX - 1)
					{
						ranking[p + 1] = ranking[p];
						lefttime[p + 1] = lefttime[p];
						
						for(int j = 0;j < 3;j++)
						{
							WordCord[p + 1][j] = WordCord[p][j];
						}
					}
				ranking[0] = ColectedCard;
				lefttime[0] = TimeCount_MAX - TimeCount;
				NewRank = 0;
				for(int j = 0;j < 3;j++)
				{
					WordCord[0][j] = WORDCORD_SPACE;
				}
				WordCord[0][0] = 0;
			}
		if((fp = fopen(TxtName,"w")) == NULL)
		return 0;
		else
		{
			fprintf(fp,"%d\n",TotalCard);
			fprintf(fp,"%d\n",NewRank);
			for(int i = 0;i < RANKINGMAX;i++)
				fprintf(fp,"%d %d %d %d %d \n",WordCord[i][0],WordCord[i][1],WordCord[i][2],ranking[i],lefttime[i]);
				fclose(fp);
		return 1;
	}
	}

}

bool StageClass::FileSave()
{
	LPCTSTR  FileName[10] = {
	"Stage/Stage1.txt",
	"Stage/Stage2.txt",
	"Stage/Stage3.txt",
	"Stage/Stage4.txt",
	"Stage/Stage5.txt",
	"Stage/Stage6.txt",
	"Stage/Stage7.txt",
	"Stage/Stage8.txt",
	"Stage/Stage9.txt",
	"Stage/Stage10.txt"
	};
	FILE *fp;
	
	if((fp = fopen(FileName[SendMessage(*hCmb_Stage, CB_GETCURSEL , 0 , 0)],"w")) == NULL)
		return 0;
	else
	{
		fprintf(fp,"%f %f \n",Player->GetPos()->x,Player->GetPos()->y);
		fprintf(fp,"%d \n",Back->SelectIndex);
		for(list<Block*>::iterator iter = M_Block->Blocklist.begin(); iter != M_Block->Blocklist.end(); iter++)
		{
			//if((*iter)->Type != BLOCKTYPE_NULL)
			fprintf(fp,"%d %d %f %f %f %f %f \n",(*iter)->Shape,(*iter)->Type,(*iter)->GetPos()->x,(*iter)->GetPos()->y,(*iter)->GetSize()->x,(*iter)->GetSize()->y,(*iter)->GetRot()->z/PAI*180);
		}
		fclose(fp);
		return 1;
	}
}

bool StageClass::CapterSave()
{
	LPCTSTR  FileName[15] = {
	"Capter/Capter1.txt",
	"Capter/Capter2.txt",
	"Capter/Capter3.txt",
	"Capter/Capter4.txt",
	"Capter/Capter5.txt",
	"Capter/Capter6.txt",
	"Capter/Capter7.txt",
	"Capter/Capter8.txt",
	"Capter/Capter9.txt",
	"Capter/Capter10.txt",
	"Capter/Capter11.txt",
	"Capter/Capter12.txt",
	"Capter/Capter13.txt",
	"Capter/Capter14.txt",
	"Capter/Capter15.txt"
	};

	FILE *fp;
	
	if((fp = fopen(FileName[SendMessage(*hCmb_Capter, CB_GETCURSEL , 0 , 0)],"w")) == NULL)
		return 0;
	else
	{
		fprintf(fp,"%f %f \n",Player->GetPos()->x,Player->GetPos()->y);
		fprintf(fp,"%d \n",Back->SelectIndex);
		for(list<Block*>::iterator iter = M_Block->Blocklist.begin(); iter != M_Block->Blocklist.end(); iter++)
			fprintf(fp,"%d %d %f %f %f %f %f \n",(*iter)->Shape,(*iter)->Type,(*iter)->GetPos()->x,(*iter)->GetPos()->y,(*iter)->GetSize()->x,(*iter)->GetSize()->y,(*iter)->GetRot()->z/PAI*180);
		fclose(fp);
		return 1;
	}
}

bool StageClass::FileLoad()
{
	LPCTSTR  FileName[10] = {
	"Stage/Stage1.txt",
	"Stage/Stage2.txt",
	"Stage/Stage3.txt",
	"Stage/Stage4.txt",
	"Stage/Stage5.txt",
	"Stage/Stage6.txt",
	"Stage/Stage7.txt",
	"Stage/Stage8.txt",
	"Stage/Stage9.txt",
	"Stage/Stage10.txt"
	};
	FILE *fp;
	D3DXVECTOR3 Vectemp;
	D3DXVECTOR3 Vectemp2;
	float Rotz;
	Vectemp.z = 0;
	int Inttemp;
	int Inttemp2;
	TotalCard = 0;

	for(list<Block*>::iterator iter = M_Block->Blocklist.begin(); iter != M_Block->Blocklist.end(); iter++)
		delete (*iter);
	M_Block->Blocklist.clear();

	if((fp = fopen(FileName[SendMessage(*hCmb_Stage, CB_GETCURSEL , 0 , 0)],"r")) == NULL)
		return 0;
	else
	{
		fscanf(fp,"%f%f",&Vectemp.x,&Vectemp.y);
		SetStPos(Vectemp);
		fscanf(fp,"%d",&Inttemp);
		Back->SelectIndex = Inttemp;
		SendMessage(*(Back->hCmb), CB_SETCURSEL  , Inttemp , Inttemp);
		while(fscanf(fp,"%d%d%f%f%f%f%f",&Inttemp,&Inttemp2,&Vectemp.x,&Vectemp.y,&Vectemp2.x,&Vectemp2.y,&Rotz) == 7)
		{
			Block *temp;
			temp = new Block("Picture/NULL.png",Vectemp.x,Vectemp.y,0,Vectemp2.x,Vectemp2.y,M_Block->Camera);
//			temp->Create();
			temp->Shape = Inttemp;
			temp->Type = Inttemp2;
			if(temp->Shape == BLOCKSHAPE_OBJECT && temp->Type == OBJECTTYPE_CARD)
				TotalCard++;
			temp->SetpTexture(&M_Block->TexTable2[Inttemp][Inttemp2]->pTex);
			temp->EdgeNumber = M_Block->TexTable2[Inttemp][Inttemp2]->EdgeNum;
			temp->SetRot(0,0,(float)(Rotz/180*PAI));

			if(temp->Shape == BLOCKSHAPE_TRIANGLE&&temp->GetRot()->z!= 0)
			{
				int p = 0;
			}
			temp->SetEdgeScale(M_Block->TexTable2[Inttemp][Inttemp2]->EdgeScale);
			if((temp->Type == BLOCKTYPE_BRUE || temp->Type == BLOCKTYPE_RED)
			&&	temp->Shape != BLOCKSHAPE_OBJECT)
			{
				temp->SetAlfa(SKELTONALFA);		
			}
			else
				temp->SetAlfa(255);
			M_Block->Blocklist.push_back(temp);	
		}
			list<Block*>::iterator iter = M_Block->Blocklist.end();
			iter--;
			if((*iter)->Type == BLOCKTYPE_NULL&&M_Block->Blocklist.size() >= 2)
			{
				iter--;
				SendMessage(*(M_Block->hCmb_BlkInf[COMBO_BLOCKSHAPE]), CB_SETCURSEL ,(*iter)->Shape ,0);
				SendMessage(*(M_Block->hCmb_BlkInf[COMBO_BLOCKTYPE]), CB_SETCURSEL ,(*iter)->Type ,0);
			}
			M_Block->SelectBlock = (*iter);

		fclose(fp);
		SetCardNumtex2();
		return 1;
	}
}

bool StageClass::CapterLoad()
{
	LPCTSTR  FileName[15] = {
	"Capter/Capter1.txt",
	"Capter/Capter2.txt",
	"Capter/Capter3.txt",
	"Capter/Capter4.txt",
	"Capter/Capter5.txt",
	"Capter/Capter6.txt",
	"Capter/Capter7.txt",
	"Capter/Capter8.txt",
	"Capter/Capter9.txt",
	"Capter/Capter10.txt",
	"Capter/Capter11.txt",
	"Capter/Capter12.txt",
	"Capter/Capter13.txt",
	"Capter/Capter14.txt",
	"Capter/Capter15.txt"
	};

	FILE *fp;
	D3DXVECTOR3 Vectemp;
	D3DXVECTOR3 Vectemp2;
	float Rotz;
	Vectemp.z = 0;
	int Inttemp;
	int Inttemp2;
	list<Block*>::iterator iter = M_Block->Blocklist.end();
	iter--;
	
	delete (*iter);
	M_Block->Blocklist.erase(iter);

	if((fp = fopen(FileName[SendMessage(*hCmb_Capter, CB_GETCURSEL , 0 , 0)],"r")) == NULL)
		return 0;
	else
	{
		
		fscanf(fp,"%f%f",&Vectemp.x,&Vectemp.y);
		SetStPos(Vectemp);
		fscanf(fp,"%d",&Inttemp);
		//Back->SelectIndex = Inttemp;
		//SendMessage(*(Back->hCmb), CB_SETCURSEL  , Inttemp , Inttemp);
		while(fscanf(fp,"%d%d%f%f%f%f%f",&Inttemp,&Inttemp2,&Vectemp.x,&Vectemp.y,&Vectemp2.x,&Vectemp2.y,&Rotz) == 7)
		{
			Block *temp;
 			temp = new Block("Picture/NULL.png",Vectemp.x + M_Block->Camera->GetCPos()->x,Vectemp.y + M_Block->Camera->GetCPos()->y,0,Vectemp2.x,Vectemp2.y,M_Block->Camera);
//			temp->Create();
			temp->Shape = Inttemp;
			temp->Type = Inttemp2;
			temp->SetpTexture(&M_Block->TexTable2[Inttemp][Inttemp2]->pTex);
			temp->EdgeNumber = M_Block->TexTable2[Inttemp][Inttemp2]->EdgeNum;
			temp->SetRot(0,0,(float)(Rotz/180*PAI));
			if(temp->GetRot()->z != 0)
			{

				int p = 0;
				p = M_Block->TexTable2[Inttemp][Inttemp2]->EdgeScale[0].x; 
			}
			temp->SetEdgeScale(M_Block->TexTable2[Inttemp][Inttemp2]->EdgeScale);
			if((temp->Type == BLOCKTYPE_BRUE || temp->Type == BLOCKTYPE_RED)
			&&	temp->Shape != BLOCKSHAPE_OBJECT)
			{
				temp->SetAlfa(SKELTONALFA);		
			}
			else
				temp->SetAlfa(255);
			M_Block->Blocklist.push_back(temp);	
		}
			list<Block*>::iterator iter = M_Block->Blocklist.end();
			iter--;
			if((*iter)->Type == BLOCKTYPE_NULL&&M_Block->Blocklist.size() >= 2)
			{
				iter--;
				SendMessage(*(M_Block->hCmb_BlkInf[COMBO_BLOCKSHAPE]), CB_SETCURSEL ,(*iter)->Shape ,0);
				SendMessage(*(M_Block->hCmb_BlkInf[COMBO_BLOCKTYPE]), CB_SETCURSEL ,(*iter)->Type ,0);
			}
			M_Block->SelectBlock = (*iter);

		fclose(fp);
		return 1;
	}

}

bool StageClass::SetPlayerStart()
{
	list<Block*>::iterator iter = M_Block->Blocklist.end();
	iter--;
	while((*iter)->Shape != BLOCKSHAPE_OBJECT||(*iter)->Type != OBJECTTYPE_START)
	{
		if(iter == M_Block->Blocklist.begin())
		{
			Player->SetPos(0,0,0);
			return 0;
		}	
		iter--;		
	}
	Player->SetPos(*(*iter)->GetPos());
	return 1;
}

int StageClass::ObjectHit(Texture *tex,Block *blk)
{
	if(CheckTexinTex(tex,blk))
	{
		switch(blk->Type)
		{
		case OBJECTTYPE_CHECK:
			CheckPointNow = blk;
			Player->GagePoint = 100;
			return 1;
		case OBJECTTYPE_CARD:
			if(blk->GetAlfa() != 0)
				GettingCard(blk);
			return 1;
		}
	}
	return 0;
}

void StageClass::PlayerDeath()
{
	Player->Init();
	
	if(CheckPointNow)
		Player->SetPos(*CheckPointNow->GetPos());
	else
		SetPlayerStart();
}

void StageClass::GettingCard(Block * blk)
{
	m_pSoundManager->Play("SOUND_STAGE_CARDGET");
	ColectedCard ++;
	CardNumtex[0]->SetpTexture(&NumberTexTable[ColectedCard%10]->pTex);
	CardNumtex[1]->SetpTexture(&NumberTexTable[ColectedCard/10]->pTex);
	blk->SetAlfa(0);
}

void StageClass::SetCardNumtex2()
{
	CardNumtex2[0]->SetpTexture(&NumberTexTable[TotalCard%10]->pTex);
	CardNumtex2[1]->SetpTexture(&NumberTexTable[TotalCard/10]->pTex);
}

void StageClass::Animation()
{
	for(list<Block*>::iterator iter = M_Block->Blocklist.begin(); iter != M_Block->Blocklist.end(); iter++)
	{
		if(CheckTexinScrean((*iter),M_Block->Camera->GetCPos()))
		{
			if((*iter)->Shape == BLOCKSHAPE_OBJECT
				&& (*iter)->Type == OBJECTTYPE_CARD)
			{
				(*iter)->SetSize(40,40,0);
				if(((*iter)->GetRot()->y >= PAI/2&&(*iter)->GetRot()->y <= PAI*3/2))
					(*iter)->SetMyVertex(0.5f,0);
				else
					(*iter)->SetMyVertex(1,0.5f);
				if((*iter)->GetRot()->y > PAI*2)
					(*iter)->SetRot(0,0,(float)(PAI/6));
				else
					(*iter)->SetRot(0,(*iter)->GetRot()->y + (float)(PAI/180*3),(float)(PAI/6));
			}
			if((*iter)->Shape == BLOCKSHAPE_OBJECT
				&& (*iter)->Type == OBJECTTYPE_CHECK)
			{
				if(CheckPointNow == (*iter))
					(*iter)->SetMyVertex(0.5f,0);
				else
					(*iter)->SetMyVertex(1,0.5f);
				SetCheckPoint_Font((*iter));
			}
		}
	}

}

void StageClass::SetCheckPoint_Font(Texture * tex)
{
	CheckPoint_Font->SetPos(tex->GetPos()->x - tex->GetSize()->x*3/64,tex->GetPos()->y,tex->GetPos()->z);
	CheckPoint_Font->SetSize(tex->GetSize()->x*35/64,tex->GetSize()->y,tex->GetSize()->z);
	if(CheckTexinTex(Player,tex))
	{
		if(CheckPoint_Count_Blink %8 != 0)
			CheckPoint_Font->SetAlfa(255);
		else
			CheckPoint_Font->SetAlfa(0);
		CheckPoint_Font->SetMyVertex((float)(11 + 35)/448,(float)(11)/448);
		CheckPoint_Count_Stream = 0;
		CheckPoint_Count_Blink++;
	}
	else if(CheckPointNow == tex)
	{
	CheckPoint_Font->SetAlfa(255);
	CheckPoint_Font->SetMyVertex((float)(64 + 11 + 35 + (CheckPoint_Count_Stream)%(340*3)/3)/448,
		(float)(64 + 11+ (CheckPoint_Count_Stream%(340*3))/3)/448);
	CheckPoint_Count_Stream ++;
	CheckPoint_Count_Blink = 0;

	}
	else
	{
		CheckPoint_Font->SetAlfa(0);
	}
	if(CheckPoint_Count_Stream > 340*3)
		CheckPoint_Count_Stream = 0;
	//CheckPoint_Font->SetMyVertex((float)(11 + 35)/64/6,(float)11/64/6);
		//CheckPoint_Count
}

void StageClass::SetStatus()
{
//	Tex_Flame->SetPos(M_Block->Camera->GetCPos()->x + );
	Tex_Flame->SetSize(800*SCREENTOSTATUS,100,0);
	Tex_Flame->SetPos(M_Block->Camera->GetCPos()->x,
					  M_Block->Camera->GetCPos()->y + STATUS_Y*SCREENTOSTATUS + Tex_Flame->GetSize()->y/2,-100);
	/*Tex_Gage->SetSize((float)Player->GagePoint*SCREENTOSTATUS*2,
						(float)40*SCREENTOSTATUS,0);*/
	Tex_Gage->SetSize((float)Player->GagePoint/100*Tex_TimeBack->GetSize()->x,
						(float)Tex_TimeBack->GetSize()->y + 50,0);
	Tex_Gage->SetPos(M_Block->Camera->GetCPos()->x
				- (float)192*SCREENTOSTATUS 
				- (float)(100 - Player->GagePoint)/100*Tex_TimeBack->GetSize()->x/3,
					 M_Block->Camera->GetCPos()->y + 249*SCREENTOSTATUS,-100);

	Tex_TimeStatic->SetPos(M_Block->Camera->GetCPos()->x
						- (float)194*SCREENTOSTATUS ,
					 M_Block->Camera->GetCPos()->y + 249*SCREENTOSTATUS,-100);
	Tex_TimeStatic->SetSize((float)Tex_TimeBack->GetSize()->x,
							(float)Tex_TimeBack->GetSize()->y,0);

	Tex_TimeMove->SetPos(M_Block->Camera->GetCPos()->x - (float)306*SCREENTOSTATUS,
						 M_Block->Camera->GetCPos()->y + 220*SCREENTOSTATUS,-100);
	Tex_TimeBack->SetPos(M_Block->Camera->GetCPos()->x - (float)(200)*SCREENTOSTATUS,
						 M_Block->Camera->GetCPos()->y + (250)*SCREENTOSTATUS,-100);
	SetWatch();
	if(Player->GagePoint < GAGEDEADLINE)
		Tex_Gage->SetpTexture(&GageKind[1]->pTex);
	else
		Tex_Gage->SetpTexture(&GageKind[0]->pTex);
}

void StageClass::SetWatch()
{
	Tex_TimeMove->SetRot(0,0,-PAI*2*TimeCount/TimeCount_MAX);
}

void StageClass::StageStart(int stage)
{
	MessageTex->SetSize(512,512,0);
	MessageTex->SetPos(M_Block->Camera->GetCPos()->x,M_Block->Camera->GetCPos()->y,-100);
	if(Count[IVENT_STAGE_READY])
	{
		MessageTex->SetpTexture(&IventTexTable[IVENT_STAGE_READY]->pTex);
		Count[IVENT_STAGE_READY]--;
	}
	else if(Count[IVENT_STAGE_START])
	{
		MessageTex->SetpTexture(&IventTexTable[IVENT_STAGE_START]->pTex);
		Count[IVENT_STAGE_START]--;
	}	
	else
	{
		MessageTex->SetPos(0,0,100000);
		Iventflg[IVENT_STAGE_START] = 1;
		m_pSoundManager->Play("SOUND_STAGE_START");
		if(stage)
		{
			m_pSoundManager->PlayfromHead("SOUND_BGM_NORMAL");
		}
		else
		{
			m_pSoundManager->PlayfromHead("SOUND_BGM_BEGINNER");
		}
	}
}
void StageClass::TimeAlarm(int stage)
{
	MessageTex->SetSize(512,512,0);
	MessageTex->SetPos(M_Block->Camera->GetCPos()->x,M_Block->Camera->GetCPos()->y,-100);
	if(Count[IVENT_STAGE_PINCH])
	{
		MessageTex->SetpTexture(&IventTexTable[IVENT_STAGE_PINCH]->pTex);
		Count[IVENT_STAGE_PINCH]--;
	}
	else
	{
		MessageTex->SetPos(0,0,100000);
		Iventflg[IVENT_STAGE_PINCH] = 1;
	if(stage)
		m_pSoundManager->PlayfromHead("SOUND_BGM_PINCH");
	}
}
void StageClass::TimeUp()
{
	MessageTex->SetSize(512,512,0);
	MessageTex->SetPos(M_Block->Camera->GetCPos()->x,M_Block->Camera->GetCPos()->y,-100);
	if(Count[IVENT_STAGE_TIMEUP])
	{
		MessageTex->SetpTexture(&IventTexTable[IVENT_STAGE_TIMEUP]->pTex);
		Count[IVENT_STAGE_TIMEUP]--;
	}
	else
	{
		m_pSoundManager->Play("SOUND_STAGE_TIMEUP");
		MessageTex->SetPos(0,0,100000);
		Iventflg[IVENT_STAGE_TIMEUP] = 1;
	}
}

void StageClass::StageClear()
{
	MessageTex->SetSize(512,512,0);
	MessageTex->SetPos(M_Block->Camera->GetCPos()->x,M_Block->Camera->GetCPos()->y,-100);
	if(Count[IVENT_STAGE_CLEAR])
	{
		MessageTex->SetpTexture(&IventTexTable[IVENT_STAGE_CLEAR]->pTex);
		Count[IVENT_STAGE_CLEAR]--;
	}
	else
	{
		MessageTex->SetPos(0,0,100000);
		Iventflg[IVENT_STAGE_CLEAR] = 1;
	}
}

void StageClass::Pause()
{
	MessageTex->SetSize(512,512,0);
	MessageTex->SetPos(M_Block->Camera->GetCPos()->x,M_Block->Camera->GetCPos()->y,-100);
	Tex_PAUSECHOICE[0]->SetPos(M_Block->Camera->GetCPos()->x - 100,M_Block->Camera->GetCPos()->y - 100,-100);
	Tex_PAUSECHOICE[1]->SetPos(M_Block->Camera->GetCPos()->x + 100,M_Block->Camera->GetCPos()->y - 100,-100);
	Tex_PAUSECHOICE[0]->SetAlfa(100);
	Tex_PAUSECHOICE[1]->SetAlfa(100);

	Tex_PAUSECHOICE[PauseSelect]->SetAlfa(255);
	
	MessageTex->SetpTexture(&IventTexTable[IVENT_STAGE_PAUSE]->pTex);
}