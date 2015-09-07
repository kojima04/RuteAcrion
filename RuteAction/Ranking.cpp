//=============================================================
// Ranking.cpp
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
RankingClass::RankingClass()
{
}
RankingClass::RankingClass(Renderer *R)
{

	Tex_RankBack = new Texture("Picture/RANKING/RANKING_BACK.png",0,0,0,600,800,R);
	
	
	for(int i = 0;i < 10;i++)
		NumberTexTable[i] = new TextureData();

	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[0]->pTex,"Picture/NUMBER/0.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[1]->pTex,"Picture/NUMBER/1.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[2]->pTex,"Picture/NUMBER/2.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[3]->pTex,"Picture/NUMBER/3.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[4]->pTex,"Picture/NUMBER/4.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[5]->pTex,"Picture/NUMBER/5.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[6]->pTex,"Picture/NUMBER/6.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[7]->pTex,"Picture/NUMBER/7.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[8]->pTex,"Picture/NUMBER/8.png");
	Tex_RankBack->SetpTexturefromFile(&NumberTexTable[9]->pTex,"Picture/NUMBER/9.png");

	for(int i = 0;i < 10;i++)
		RankTexTable[i] = new TextureData();

	Tex_RankBack->SetpTexturefromFile(&RankTexTable[0]->pTex,"Picture/NUMBER/1st.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[1]->pTex,"Picture/NUMBER/2nd.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[2]->pTex,"Picture/NUMBER/3rd.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[3]->pTex,"Picture/NUMBER/4th.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[4]->pTex,"Picture/NUMBER/5th.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[5]->pTex,"Picture/NUMBER/6th.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[6]->pTex,"Picture/NUMBER/7th.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[7]->pTex,"Picture/NUMBER/8th.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[8]->pTex,"Picture/NUMBER/9th.png");
	Tex_RankBack->SetpTexturefromFile(&RankTexTable[9]->pTex,"Picture/NUMBER/10th.png");

	for(int i = 0;i < RANKTEX_MAX;i++)
		TexTable[i] = new TextureData();

	Tex_RankBack->SetpTexturefromFile(&TexTable[RANK_TEX_CLEAR]->pTex,"Picture/RANKING/stageclear.png");
	Tex_RankBack->SetpTexturefromFile(&TexTable[RANK_TEX_SCORE]->pTex,"Picture/RANKING/font_score.png");
	Tex_RankBack->SetpTexturefromFile(&TexTable[RANK_TEX_BEGINNER]->pTex,"Picture/TYTLE/TYTLE_BEGINNER.png");
	Tex_RankBack->SetpTexturefromFile(&TexTable[RANK_TEX_NORMAL]->pTex,"Picture/TYTLE/TYTLE_NORMAL.png");
	Tex_RankBack->SetpTexturefromFile(&TexTable[RANK_TEX_MASTER]->pTex,"Picture/TYTLE/TYTLE_MASTAR.png");

	Tex_Level = new Texture("Picture/TYTLE/TYTLE_BEGINNER.png",300,50 + 200,0,300,300,R);
	Tex_Score = new Texture("Picture/RANKING/font_score.png",0,50 + 200,0,400,400,R);
	NewEmblem =	new Texture("Picture/NEW.png",0,50 + 200,100,100,100,R);
	NewEmblemLine =	new Texture("Picture/Line.png",0,50 + 200,100,200,1000,R);
	
	Inputflg = 0;
	WordEndCord = 0;
	NameEnd = new Texture("Picture/ALPHABET.png",0,0,110,300,200,R);

	//NameEnd->SetMyVertex((float)(WordCord + 1)/WORDCORDMAX,(float)WordCord/WORDCORDMAX);
	for(int i = 0;i < RANKINGMAX;i++)
	{
		for(int p = 0;p < 3;p++)
		{
			RankingName[i][p] = new Texture("Picture/ALPHABET.png",0,0,0,80,80,R);
			WordCord[i][p] = 0;
		}
		/*if(i < 5)
		{
			Tex_RankNum[i]= new Texture("Picture/NUMBER/0.png",-320,- (float)i * 100 + 200 - 50,0,100,100,R);
			Tex_RankNum[i]->SetpTexture(&RankTexTable[i]->pTex);
		
			Tex_RankScore[i][0] = new Texture("Picture/NUMBER/0.png",-140,- (float)i * 100 + 200 + 20 - 50,0,60,60,R);
			Tex_RankScore[i][1] = new Texture("Picture/NUMBER/0.png",-100,- (float)i * 100 + 200 + 20 - 50,0,60,60,R);

			for(int k = 0;k < 4;k++)
			{
				Tex_Time[i][k] = new Texture("Picture/NUMBER/0.png",-150,- (float)i * 100 + 200 + 20,0,80,80,R);
				Tex_Time[i][k]->SetAlfa(0);
			}
			Tex_Slash[i] = new Texture("Picture/TIMESLASH.png",-150,- (float)i * 100 + 200 + 20,0,80,80,R);;
			Tex_Clear[i] = new Texture("Picture/GAMECLEAR.png",(float)400*(i > 4) - 200,- (float)i * 100 + 200 + 20,0,80,80,R);
			Tex_Slash[i]->SetAlfa(0);		
			Tex_Clear[i]->SetAlfa(0);
			
		}
		else*/

		{
			Tex_RankNum[i] = new Texture("Picture/NUMBER/0.png",80 - 300,- (float)(i % 5) * 100 + 200 - 40,0,100,100,R);
			
			Tex_RankNum[i]->SetpTexture(&RankTexTable[i]->pTex);
			
			Tex_RankScore[i][0] = new Texture("Picture/NUMBER/0.png",220,- (float)(i % 5) * 100 + 200 + 20 - 50,0,60,60,R);
			Tex_RankScore[i][1] = new Texture("Picture/NUMBER/0.png",260,- (float)(i % 5) * 100 + 200 + 20 - 50,0,60,60,R);

			for(int k = 0;k < 4;k++)
			{
				Tex_Time[i][k] = new Texture("Picture/NUMBER/0.png",200,- (float)(i % 5) * 100 + 200 + 20,0,80,80,R);
				Tex_Time[i][k]->SetAlfa(0);
			}
			Tex_Slash[i] = new Texture("Picture/TIMESLASH.png",-70,- (float)i * 100 + 200 + 20,0,80,80,R);;
			Tex_Clear[i] = new Texture("Picture/GAMECLEAR.png",(float)100,- (float)i * 100 + 200 + 20,0,80,80,R);
			Tex_Slash[i]->SetAlfa(0);		
			Tex_Clear[i]->SetAlfa(0);
		}

		Card[i] = new Texture("Picture/CARD.png",0,50 + 200,1000,20,20,R);
		BATSU[i] = new Texture("Picture/BATSU.png",0,50 + 200,1000,45,45,R);
		
	}
	Rankin = new Texture("Picture/font_rankin.png",0,200,1000,400,400,R);
	Count[COUNT_RANK_VIEW] = TIME_RANKSHOW;
	Count[COUNT_NEWLIGHT] = 0;

	for(int i = 0;i < RANKINGMAX;i++)
	{
		CountMove[i] = 0;
		Moveflg[i] = 0;
	}
	Moveflg[0] = 1;
	NewScore = NewTime = ScoreMAX;
	ScoreMAX = 1;
	Process = 0;
	NewRank = 9;
	
	Scearn = 0;
	TypingWord = 0;
	InputNameflg = 0;
}
void RankingClass::Init()
{
	Inputflg = 0;
	TypingWord = 0;
	Count[COUNT_RANK_VIEW] = TIME_RANKSHOW;
	Count[COUNT_NEWLIGHT] = 0;
	for(int i = 0;i < RANKINGMAX;i++)
	{
		Card[i]->SetAlfa(255);
		BATSU[i]->SetAlfa(255);
		Card[i]->SetPos(0,0,100);
		BATSU[i]->SetPos(0,0,100);
		CountMove[i] = 0;
		Moveflg[i] = 0;

		Tex_Clear[i]->SetAlfa(255);
		for(int k = 0;k < 4;k++)
		{
			Tex_Time[i][k]->SetAlfa(255);
		}
		Tex_RankNum[i]->SetAlfa(255);
		Tex_Slash[i]->SetAlfa(255);
		Tex_RankScore[i][1]->SetAlfa(255);
		Tex_RankScore[i][0]->SetAlfa(255);
		for(int p = 0;p < 3;p++)
		{
			RankingName[i][p]->SetPos(0,0,100);
		}
	
	}
	Moveflg[RANKINGMAX - 1] = 1;
	NewScore = NewTime = ScoreMAX;
	ScoreMAX = 1;
	Process = 0;
	NewRank = 9;
	Scearn = 0;
}

RankingClass::~RankingClass()
{
	delete NameEnd;
	for(int i = 0;i < RANKINGMAX;i++)
	{		
		for(int p = 0;p < 3;p++)
		{
			delete RankingName[i][p];
		}
	}
	delete Tex_RankBack;
	delete Tex_Score;
	delete NewEmblem;
	delete NewEmblemLine;
	for(int i = 0;i < RANKINGMAX;i++)
	{
		delete Tex_RankNum[i];
		delete Tex_RankScore[i][0];
		delete Tex_RankScore[i][1];
		for(int k = 0;k < 4;k++)
			delete Tex_Time[i][k];
		delete Tex_Slash[i];
		delete Tex_Clear[i];
		delete Card[i];
		delete BATSU[i];
	}
	for(int i = 0;i < 10;i++)
	{
		delete NumberTexTable[i];
		delete RankTexTable[i];
	}

	for(int i = 0;i < RANKTEX_MAX;i++)
		delete TexTable[i];
	delete Rankin;
	delete Tex_Level;
}

void RankingClass::main()
{
	/*int a = 1;
	for(int i = 0;i < RANKINGMAX;i++)
		if(WordCord[i][0] == 0&&WordCord[i][1] == 0&&WordCord[i][2] == 0)
		{
			a = 0;
			break;
		}
		*/

	if(InputNameflg == 0)
	{
		TexMove();

		if(Inputflg)
			switch(Scearn)
			{
			case 2:
				Count[COUNT_RANK_VIEW] = 0;
				break;
			case 1:
				if(Count[COUNT_RANK_VIEW] > TIME_RANKSHOW/2)
					Count[COUNT_RANK_VIEW] = TIME_RANKSHOW/2;
				else
				for(;Scearn != 2;)
				TexMove();
				break;
			case 0:
				for(;Scearn != 1;)
					TexMove();
				break;
			}


		Count[COUNT_NEWLIGHT]++;

		if(Count[COUNT_NEWLIGHT] > 30)
		{
			if(NewEmblem->GetAlfa())
				NewEmblem->SetAlfa(0);
			else
				NewEmblem->SetAlfa(255);
			Count[COUNT_NEWLIGHT] = 0;
		}
		
	}
	else
	{
		Rankin->SetPos(0,100,0);
		Tex_RankNum[NewRank]->SetAlfa(255);
		for(int p = 0;p < 3;p++)
		{
			RankingName[NewRank][p]->SetPos(- 100 + 100*p
									 ,0,0);
			RankingName[NewRank][p]->SetAlfa(255);
			RankingName[NewRank][p]->SetMyVertex((float)(WordCord[NewRank][p] + 1)/WORDCORDMAX,
													  (float)WordCord[NewRank][p]/WORDCORDMAX);
			if(TypingWord >2)
			{
				InputNameflg = 0;
				for(int p = 0;p < 3;p++)
					RankingName[NewRank][p]->SetPos(0,0,100);
				Rankin->SetPos(0,100,100);
			}	
		}
	}
	
}

bool RankingClass::FileLoad(int stage,bool newswitch)
{
	int csf = 2;
	char TxtName[64];
		sprintf_s(TxtName, 64, "Score/Ranking%d.txt", stage);
	int ranking[RANKINGMAX] = {0};
	FILE *fp;
	if((fp = fopen(TxtName,"r")) == NULL)
		return 0;
	else
	{
		fscanf(fp,"%d",&ScoreMAX);
		fscanf(fp,"%d",&NewRank);
		//ColectedCard = 5;
		for(int i = 0;fscanf(fp,"%d %d %d %d %d",&WordCord[i][0],&WordCord[i][1],&WordCord[i][2],&Score[i],&Time[i]) == 2||i < RANKINGMAX;i++)
		{
			SetScoreTex(i,Score[i]);
			/*if(Time[i] == 0)
				Tex_RankScore[i][0]->SetPos(Tex_RankScore[i][0]->GetPos()->x,Tex_RankScore[i][0]->GetPos()->y,0);*/
		}
		Tex_Level->SetpTexture(&TexTable[RANK_TEX_BEGINNER + stage]->pTex);
		if(newswitch)
		{
			if(NewRank >= 0)
				InputNameflg = 1;
			else
				InputNameflg = 0;
		}
		else
			InputNameflg = 0;
	return 1;
	}
}

void RankingClass::SetScoreTex(int rank,int score)
{
	if(score >= 10)
		Tex_RankScore[rank][0]->SetpTexture(&NumberTexTable[score/10]->pTex);
	else
		Tex_RankScore[rank][0]->SetpTexture(&NumberTexTable[0]->pTex);
	Tex_RankScore[rank][1]->SetpTexture(&NumberTexTable[score%10]->pTex);

	Tex_RankNum[rank]->SetPos(Tex_RankNum[rank]->GetPos()->x + RANKINGMOVE*RANKINGMOVETIME,Tex_RankNum[rank]->GetPos()->y,0);
	Tex_RankScore[rank][1]->SetPos(Tex_RankScore[rank][1]->GetPos()->x + RANKINGMOVE*RANKINGMOVETIME,Tex_RankNum[rank]->GetPos()->y - 5,0);
	Tex_RankScore[rank][0]->SetPos(Tex_RankScore[rank][0]->GetPos()->x + RANKINGMOVE*RANKINGMOVETIME,Tex_RankNum[rank]->GetPos()->y - 5,0);
	
		Tex_Clear[rank]->SetAlfa(0);
		Tex_Slash[rank]->SetAlfa(0);
		for(int k = 0;k < 4;k++)
		{
			Tex_Time[rank][k]->SetAlfa(0);
		}
		Tex_RankScore[rank][1]->SetAlfa(255);
		Tex_RankScore[rank][0]->SetAlfa(255);

		for(int p = 0;p < 3;p++)
		{
			RankingName[rank][p]->SetMyVertex((float)(WordCord[rank][p] + 1)/WORDCORDMAX,(float)WordCord[rank][p]/WORDCORDMAX);
			RankingName[rank][p]->SetPos(Tex_RankNum[rank]->GetPos()->x + RANKINGMOVE*RANKINGMOVETIME + 100 + 50*p
										,Tex_RankNum[rank]->GetPos()->y,0);
		}
	
}

void RankingClass::SetClearEmblem(int rank,int score)
{
	if(score >= ScoreMAX&&Tex_RankNum[rank]->GetAlfa())
	{
		//Tex_RankScore[rank][0]->SetpTexture(&TexTable[RANK_TEX_CLEAR]->pTex);
		Tex_Clear[rank]->SetSize(300,300,0);
		Tex_Clear[rank]->SetPos((float)200,Tex_RankScore[rank][0]->GetPos()->y + 30,0);
		Tex_Clear[rank]->SetAlfa(255);
		for(int k = 0;k < 4;k++)
		{
			Tex_Time[rank][k]->SetAlfa(255);
			Tex_Time[rank][k]->SetPos((float)400- 160 + k * 30 + (k > 1)*10,Tex_RankScore[rank][0]->GetPos()->y - 10,0);
			Tex_Time[rank][k]->SetSize(50,50,0);
		}
		Tex_Slash[rank]->SetAlfa(255);
		Tex_Slash[rank]->SetPos((float)400 - 160 + 1 * 30 + 30,Tex_RankScore[rank][0]->GetPos()->y - 5,0);
		Tex_Slash[rank]->SetSize(50,50,0);
		SetTimeCount(rank);


		Tex_RankScore[rank][1]->SetAlfa(50);
		Tex_RankScore[rank][0]->SetAlfa(50);
		BATSU[rank]->SetAlfa(0);
		Card[rank]->SetAlfa(0);
	}
	else
	{
		Tex_Clear[rank]->SetAlfa(0);
		Tex_Slash[rank]->SetAlfa(0);
		for(int k = 0;k < 4;k++)
		{
			Tex_Time[rank][k]->SetAlfa(0);
		}
		Tex_RankScore[rank][1]->SetAlfa(255);
		Tex_RankScore[rank][0]->SetAlfa(255);
	}
}

void RankingClass::SetTimeCount(int rank)
{
	if(Time[rank] > 600)
		Tex_Time[rank][0]->SetpTexture(&NumberTexTable[Time[rank]/600]->pTex);
	else
		Tex_Time[rank][0]->SetpTexture(&NumberTexTable[0]->pTex);
	if(Time[rank] > 60)
		Tex_Time[rank][1]->SetpTexture(&NumberTexTable[(Time[rank]%600)/60]->pTex);
	else
		Tex_Time[rank][1]->SetpTexture(&NumberTexTable[0]->pTex);
	if(Time[rank] > 10)
		Tex_Time[rank][2]->SetpTexture(&NumberTexTable[(Time[rank]%60) /10]->pTex);
	else
		Tex_Time[rank][2]->SetpTexture(&NumberTexTable[0]->pTex);

	Tex_Time[rank][3]->SetpTexture(&NumberTexTable[Time[rank]%10]->pTex);

}

void RankingClass::CardAnimation(Texture * tex)
{
	tex->SetSize(40,40,0);
	if((tex->GetRot()->y >= PAI/2&&tex->GetRot()->y <= PAI*3/2))
		tex->SetMyVertex(0.5f,0);
	else
		tex->SetMyVertex(1,0.5f);
	if(tex->GetRot()->y > PAI*2)
		tex->SetRot(0,0,(float)(PAI/6));
	else
		tex->SetRot(0,tex->GetRot()->y + (float)(PAI/180*3),(float)(PAI/6));
}

void RankingClass::Input()
{
	Inputflg = 0;

	if((g_InputState[INPUTSTATE_OK]&&!(g_ExInputState[INPUTSTATE_OK]))
	|| (g_InputState[INPUTSTATE_START]&&!(g_ExInputState[INPUTSTATE_START]))
	)
		Inputflg = 1;
	if(g_InputState[INPUTSTATE_CANCEL]&&!(g_ExInputState[INPUTSTATE_CANCEL])
	)
		Inputflg = -1;
	
	if(InputNameflg)
	{
		if(g_InputState[INPUTSTATE_DOWN]&&!(g_ExInputState[INPUTSTATE_DOWN]))
		{
			WordCord[NewRank][TypingWord] ++;
			if(WordCord[NewRank][TypingWord] >= WORDCORDMAX)
				WordCord[NewRank][TypingWord] = 0;
		}

		if(g_InputState[INPUTSTATE_UP]&&!(g_ExInputState[INPUTSTATE_UP]))
		{
			WordCord[NewRank][TypingWord] --;
			if(WordCord[NewRank][TypingWord] < 0)
				WordCord[NewRank][TypingWord] = WORDCORDMAX - 1;
		}
		if(Inputflg == 1)
		{
			if(WordCord[NewRank][TypingWord] != WORDCORD_BACK&&WordCord[NewRank][TypingWord] != WORDCORD_END)
			{
				TypingWord ++;
				if(TypingWord < 3)
				WordCord[NewRank][TypingWord] = 0;
			}
			else
			{
				if(TypingWord > 0&&WordCord[NewRank][TypingWord] != WORDCORD_END)
				{
					WordCord[NewRank][TypingWord] = WORDCORD_SPACE;
					TypingWord--;
				}
				if(WordCord[NewRank][TypingWord] == WORDCORD_END)
				{
					if(TypingWord > 0)
					{
						for(;TypingWord < 3;TypingWord++)
						WordCord[NewRank][TypingWord] = WORDCORD_SPACE;
					}
					else
					{
						WordCord[NewRank][0] = WordCord[NewRank][1] = WordCord[NewRank][2] = 0;
						TypingWord = 4;
					}
				}
			}
		}
		if(Inputflg == -1)
		{
			if(TypingWord > 0)
			{
				WordCord[NewRank][TypingWord] = WORDCORD_SPACE;
				TypingWord --;
			}
		}

	}

}

bool RankingClass::FileSave(int stage)
{
	char TxtName[64];
	sprintf_s(TxtName, 64, "Score/Ranking%d.txt", stage);

	FILE *fp;

		if((fp = fopen(TxtName,"w")) == NULL)
		return 0;
		else
		{
			fprintf(fp,"%d\n",ScoreMAX);
			fprintf(fp,"%d\n",NewRank);
			for(int i = 0;i < RANKINGMAX;i++)
				fprintf(fp,"%d %d %d %d %d \n",WordCord[i][0],WordCord[i][1],WordCord[i][2],Score[i],Time[i]);
				fclose(fp);
		return 1;
	}

}


void RankingClass::TexMove()
{
	if(Count[COUNT_RANK_VIEW] > TIME_RANKSHOW/2 - 1&&Count[COUNT_RANK_VIEW] < TIME_RANKSHOW/2 + 1)
	{
		for(int i = RANKINGMAX - 1;i > 4;i--)
		{
			Tex_Clear[i]->SetAlfa(0);
			for(int k = 0;k < 4;k++)
			{
				Tex_Time[i][k]->SetAlfa(0);
			}
			Tex_RankNum[i]->SetAlfa(0);
			Tex_Slash[i]->SetAlfa(0);
			Tex_RankScore[i][1]->SetAlfa(0);
			Tex_RankScore[i][0]->SetAlfa(0);
			BATSU[i]->SetAlfa(0);
			Card[i]->SetAlfa(0);
			for(int p = 0;p < 3;p++)
			{
				RankingName[i][p]->SetPos(0,0,100);
			}
		}
		NewEmblem->SetPos(0,0,100);
		NewEmblemLine->SetPos(0,0,100);
		Moveflg[4] = 1;
	}

	
	for(int i = RANKINGMAX - 1;i >= 0;i--)
	{
		CardAnimation(Card[i]);
		if(CountMove[i] < RANKINGMOVETIME&&Moveflg[i])
		{
			CountMove[i]++;
			if(CountMove[i] == RANKINGMOVERAG &&(i > 5||(i > 0&&i <= 4)) )
			{
				Moveflg[i - 1] = 1;
			}
			Tex_RankNum[i]->SetPos(Tex_RankNum[i]->GetPos()->x - RANKINGMOVE,Tex_RankNum[i]->GetPos()->y,0);
			Tex_RankScore[i][1]->SetPos(Tex_RankScore[i][1]->GetPos()->x - RANKINGMOVE,Tex_RankScore[i][1]->GetPos()->y,0);
			Tex_RankScore[i][0]->SetPos(Tex_RankScore[i][0]->GetPos()->x - RANKINGMOVE,Tex_RankScore[i][0]->GetPos()->y,0);
			Card[i]->SetPos(Tex_RankScore[i][1]->GetPos()->x*SCREENTOSTATUS - 130,Tex_RankScore[i][1]->GetPos()->y*SCREENTOSTATUS,-100);
			BATSU[i]->SetPos(Tex_RankScore[i][1]->GetPos()->x*SCREENTOSTATUS - 80,Tex_RankScore[i][1]->GetPos()->y*SCREENTOSTATUS,-100);

			for(int p = 0;p < 3;p++)
			{
			RankingName[i][p]->SetPos(Tex_RankNum[i]->GetPos()->x + 100 + 50*p
										,Tex_RankNum[i]->GetPos()->y,0);
			}
		}
		else
		{
			Moveflg[i] = 0;
		}
	}

		if(Moveflg[1] == 0)
			int p = 0;
	
		if(CountMove[5] >= RANKINGMOVETIME&&Moveflg[5] == 0&&Scearn == 0)
		{
			for(int i = 5;i < RANKINGMAX;i++)
				SetClearEmblem(i,Score[i]);
			Scearn = 1;

			NewEmblem->SetPos(0,0,100);
			if(NewRank >= 5)
			{
				NewEmblem->SetPos(Tex_RankNum[NewRank]->GetPos()->x - 100,Tex_RankNum[NewRank]->GetPos()->y,0);
				NewEmblemLine->SetPos(0,Tex_RankNum[NewRank]->GetPos()->y+10,0);
			}
		}
		if(CountMove[0] >= RANKINGMOVETIME&&Moveflg[0] == 0&&Scearn == 1)
		{
			for(int i = 0;i < 5;i++)
				SetClearEmblem(i,Score[i]);
			Scearn = 2;
			NewEmblem->SetPos(0,0,100);
			if(NewRank >= 0&&NewRank < 5)
			{
				NewEmblem->SetPos(Tex_RankNum[NewRank]->GetPos()->x - 100,Tex_RankNum[NewRank]->GetPos()->y + 5,0);
				NewEmblemLine->SetPos(0,Tex_RankNum[NewRank]->GetPos()->y+10,0);
			}
		}

	Count[COUNT_RANK_VIEW]--;

}