//=============================================================
// Tytle.h
//=============================================================
#pragma once

//=============================================================
// include
//=============================================================
#include "Common.h"

typedef enum ___ChoiceMember {
	CHOICEMEM_START,
	CHOICEMEM_PLAY,
	CHOICEMEM_RANKING,
	CHOICEMEM_BEGINNER,
	CHOICEMEM_NORMAL,
	CHOICEMEM_MASTER,
	CHOICEMEM_EXIT,
	CHOICEMEM_MAX,
}e_ChoiceMember;

typedef enum ___Count_Tytle{
	COUNT_TYTLE_VIEW,
	COUNT_TYTLE_MAX,
}e_Count_Tytle;

//=============================================================
// TytleClass
//=============================================================
class TytleClass
{
public:

	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	TytleClass();
	TytleClass(Renderer *R);
	
	//---------------------------------------------------------
	// 仮想デストラクタ
	//---------------------------------------------------------
	~TytleClass();

	void main();
	void Input();

	int Inputflg;
	int SignalCount;
	int ChoiceNum;
	int ChoiceNumMAX;
	int SelectStage;
	int Rankingflg;
	Texture *SelectChoice;
	Texture *UpperChoice;
	Texture *TitleBack;
	Texture *TitleChoices[CHOICEMEM_MAX];

	int Count[COUNT_TYTLE_MAX];
};