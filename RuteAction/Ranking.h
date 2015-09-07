//=============================================================
// Ranking.h
//=============================================================
#pragma once

//=============================================================
// include
//=============================================================
#include "Common.h"

#define TIME_RANKSHOW 1000
#define RANKINGMOVE 8
#define RANKINGMOVETIME 120
#define RANKINGMOVERAG 15
#define WORDCORDMAX 30
#define WORDCORD_BACK 28
#define WORDCORD_END 29
#define WORDCORD_SPACE 27

typedef enum ___Count_Rank{
	COUNT_RANK_VIEW,
	COUNT_NEWLIGHT,
	COUNT_RANK_MAX,
}e_Count_Rank;

typedef enum ___Texture_Ranking{
	RANK_TEX_CLEAR,
	RANK_TEX_SCORE,
	RANK_TEX_BEGINNER,
	RANK_TEX_NORMAL,
	RANK_TEX_MASTER,
	RANKTEX_MAX,
}e_Texture_Ranking;


//=============================================================
// RankingClass
//=============================================================
class RankingClass
{
public:

	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	RankingClass();
	RankingClass(Renderer *R);
	
	//---------------------------------------------------------
	// 仮想デストラクタ
	//---------------------------------------------------------
	~RankingClass();

	Texture *Tex_RankBack;
	Texture *Tex_RankNum[RANKINGMAX];
	Texture *Tex_RankScore[RANKINGMAX][2];
	Texture *Tex_Score;
	TextureData *NumberTexTable[10];
	TextureData *RankTexTable[10];
	TextureData *TexTable[RANKTEX_MAX];
	Texture *Card[RANKINGMAX];
	Texture *BATSU[RANKINGMAX];
	Texture *NewEmblem;
	Texture *NewEmblemLine;

	Texture *RankingName[RANKINGMAX][3];
	Texture *NameEnd;
	int WordCord[RANKINGMAX][3];
	int WordEndCord;
	int TypingWord;
	int InputNameflg;

	int Count[COUNT_RANK_MAX];

	Texture *Tex_Time[RANKINGMAX][4];
	Texture *Tex_Slash[RANKINGMAX];
	Texture *Tex_Clear[RANKINGMAX];
	Texture *Tex_Level;
	Texture *Rankin;


	int Moveflg[RANKINGMAX];
	int Process;
	int Scearn;
	int CountMove[RANKINGMAX];
	int Score[RANKINGMAX];
	int Time[RANKINGMAX];
	bool FileLoad(int,bool);
	bool FileSave(int);
	void SetScoreTex(int,int);
	void SetTimeCount(int);
	void SetClearEmblem(int,int);
	int NewScore;
	int NewTime;
	int NewRank;
	int ScoreMAX;

	void CardAnimation(Texture *);
	void TexMove();
	
	void Init();
	void main();
	void Input();
	int Inputflg;

};