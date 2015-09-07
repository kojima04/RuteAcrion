//=============================================================
// Tytle.cpp
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
TytleClass::TytleClass()
{
	
}

TytleClass::TytleClass(Renderer *R)
{
	TitleBack = new Texture("Picture/TYTLE/title.png",0,0,0,600,800,R);
	TitleChoices[CHOICEMEM_START] = new Texture("Picture/TYTLE/TYTLE_START.png",0,-180,0,250,250,R);
	TitleChoices[CHOICEMEM_PLAY] = new Texture("Picture/TYTLE/TYTLE_PLAY.png",-200,-180,0,250,250,R);
	TitleChoices[CHOICEMEM_RANKING] = new Texture("Picture/TYTLE/TYTLE_RANKING.png",0,-180,0,250,250,R);
	TitleChoices[CHOICEMEM_EXIT] = new Texture("Picture/TYTLE/TYTLE_EXIT.png",200,-180,0,250,250,R);
	TitleChoices[CHOICEMEM_BEGINNER] = new Texture("Picture/TYTLE/TYTLE_BEGINNER.png",-150,-180,0,250,250,R);
	TitleChoices[CHOICEMEM_NORMAL] = new Texture("Picture/TYTLE/TYTLE_NORMAL.png",0,-180,0,250,250,R);
	TitleChoices[CHOICEMEM_MASTER] = new Texture("Picture/TYTLE/TYTLE_MASTAR.png",150,-180,0,250,250,R);

	SelectChoice = TitleChoices[CHOICEMEM_START];
	UpperChoice = NULL;
	Inputflg = 0;
	ChoiceNumMAX = 0;
	ChoiceNum = 0;
	SignalCount = 0;
	SelectStage = -1;
	Rankingflg = 0;
}

TytleClass::~TytleClass()
{
	delete TitleBack;
	for(int i = 0;i < CHOICEMEM_MAX;i++)
		delete TitleChoices[i];
}

void TytleClass::main()
{
	for(int i = 0;i < CHOICEMEM_MAX;i++)
		TitleChoices[i]->SetAlfa(0);

	if(UpperChoice == TitleChoices[CHOICEMEM_PLAY])
	{
		ChoiceNumMAX = 2;
		TitleChoices[CHOICEMEM_BEGINNER]->SetAlfa(255);
		TitleChoices[CHOICEMEM_NORMAL]->SetAlfa(255);
		TitleChoices[CHOICEMEM_MASTER]->SetAlfa(255);
		//TitleChoices[CHOICEMEM_MASTER]->SetAlfa(50);
		switch(ChoiceNum)
		{
		case 0:SelectChoice = TitleChoices[CHOICEMEM_BEGINNER];break;
		case 1:SelectChoice = TitleChoices[CHOICEMEM_NORMAL];break;
		case 2:SelectChoice = TitleChoices[CHOICEMEM_MASTER];break;
		}
		if(Inputflg == 1)
		{
			SelectStage = ChoiceNum;
	//		g_pPerformance->PlaySegmentEx(g_pSeg[SOUND_TYTLE_DECIDE], NULL, NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL);
		}

		if(Inputflg == -1)
		{
			ChoiceNumMAX = 2;
			ChoiceNum = 0;
			SelectChoice = TitleChoices[CHOICEMEM_PLAY];
			UpperChoice = TitleChoices[CHOICEMEM_START];
		}
	}

	if(UpperChoice == NULL)
	{	
		ChoiceNumMAX = 0;
		TitleChoices[CHOICEMEM_START]->SetAlfa(255);
	}
	if(UpperChoice == TitleChoices[CHOICEMEM_START])
	{
		ChoiceNumMAX = 2;
		TitleChoices[CHOICEMEM_PLAY]->SetAlfa(255);
		TitleChoices[CHOICEMEM_RANKING]->SetAlfa(255);
		TitleChoices[CHOICEMEM_EXIT]->SetAlfa(255);
		switch(ChoiceNum)
		{
		case 0:SelectChoice = TitleChoices[CHOICEMEM_PLAY];break;
		case 1:SelectChoice = TitleChoices[CHOICEMEM_RANKING];break;
		case 2:SelectChoice = TitleChoices[CHOICEMEM_EXIT];break;
		}
		if(Inputflg == 1)
		{
		//	g_pPerformance->PlaySegmentEx(g_pSeg[SOUND_TYTLE_DECIDE], NULL, NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL);
			if(SelectChoice == TitleChoices[CHOICEMEM_PLAY])
				UpperChoice = TitleChoices[CHOICEMEM_PLAY];
			if(SelectChoice == TitleChoices[CHOICEMEM_RANKING])
				Rankingflg = 1;
			if(SelectChoice == TitleChoices[CHOICEMEM_EXIT])
				PostQuitMessage(0);
		}
		if(Inputflg == -1)
		{
			ChoiceNumMAX = 0;
			ChoiceNum = 0;
			SelectChoice = TitleChoices[CHOICEMEM_START];
			UpperChoice = NULL;
		}
	}
	
	SignalCount++;
	if(SignalCount < 20)
		SelectChoice->SetAlfa(255);
	else
		SelectChoice->SetAlfa(0);
	if(SignalCount > 40)
		SignalCount = 0;

	if(g_InputState[INPUTSTATE_LEFT]&&!(g_ExInputState[INPUTSTATE_LEFT])&&ChoiceNum > 0)
	{
			ChoiceNum--;
//		g_pPerformance->PlaySegmentEx(g_pSeg[SOUND_TYTLE_CURSOR], NULL, NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL);
	}
	if(g_InputState[INPUTSTATE_RIGHT]&&!(g_ExInputState[INPUTSTATE_RIGHT])&&ChoiceNum < ChoiceNumMAX)
	{
			ChoiceNum++;
//		g_pPerformance->PlaySegmentEx(g_pSeg[SOUND_TYTLE_CURSOR], NULL, NULL, DMUS_SEGF_SECONDARY, 0, NULL, NULL, NULL);
	}
}

void TytleClass::Input()
{
	Inputflg = 0;

	if(UpperChoice == NULL&&g_InputState[INPUTSTATE_START]&&!(g_ExInputState[INPUTSTATE_START]))
	{
		ChoiceNum = 0;
		UpperChoice = TitleChoices[CHOICEMEM_START];
		SelectChoice = TitleChoices[CHOICEMEM_PLAY];
	}
	else
	if((g_InputState[INPUTSTATE_OK]&&!(g_ExInputState[INPUTSTATE_OK]))
	|| (g_InputState[INPUTSTATE_START]&&!(g_ExInputState[INPUTSTATE_START]))
	)
		Inputflg = 1;
	if(g_InputState[INPUTSTATE_CANCEL]&&!(g_ExInputState[INPUTSTATE_CANCEL])
	)
		Inputflg = -1;

	
}