#pragma once

#define DEADLINE -2000

typedef enum ___Scearn{
	SCEARN_TYTLE,
	SCEARN_ACTION,
	SCEARN_RANKING,
	SCEARN_RANKING2,
}e_Scearn;

typedef enum ___Sound{
	SOUND1,
	SOUND2,
	SOUND3,
	SOUND4,
	SOUND5,
	SOUND6,
	SOUND_BGM_NORMAL,
	SOUND_BGM_BEGINNER,
	SOUND_BGM_PINCH,
	SOUND_BGM_TYTLE,
	SOUND_TYTLE_CURSOR,
	SOUND_TYTLE_DECIDE,
	SOUND_STAGE_START,
	SOUND_STAGE_TIMEUP,
	SOUND_STAGE_CARDGET,
	SOUND_PLAYER_DEATH,
	SOUND_PLAYER_THROW,
	SOUND_PLAYER_BAUND,
	SOUND_PLAYER_BAUND2,
	SOUND_PLAYER_METAL,
	SOUND_MAX,
}e_Sound;

typedef enum ___INPUTSTATE{
	INPUTSTATE_UP,
	INPUTSTATE_DOWN,
	INPUTSTATE_LEFT,
	INPUTSTATE_RIGHT,
	INPUTSTATE_OK,
	INPUTSTATE_CANCEL,
	INPUTSTATE_START,
	INPUTSTATE_RIDE,
	INPUTSTATE_JUMP,
	INPUTSTATE_MOVE,
	INPUTSTATE_THROW,
	INPUTSTATE_THROW_UPSPIN,
	INPUTSTATE_THROW_DOWNSPIN,
	INPUTSTATE_MAX
}e_INPUTSTATE;

extern BYTE	g_InputState[INPUTSTATE_MAX];
extern BYTE	g_ExInputState[INPUTSTATE_MAX];

class System 
{
	public:
	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	System(bool isFullScreen,HWND H);
	~System();

	HWND hWnd;
	HWND hWndComMain;
	Renderer renderer;
	HRESULT hr;

	int SCEARN;
	int SelectStage;

	DWORD		nowTime ;
	DWORD		beforeTime ;
	void Paint();
	void Paint_Tytle();
	void Paint_Action();
	void Paint_Ranking();
	void Main();
	void SetInputState();
	void Input();
	void Input_Edit();
	void SetCamera();
	void ActiveAction();
	void PassiveAction();
	void EachOtherAction();
	void ScearnSet();

	StageClass *Stage;
	TytleClass *Tytle;
	RankingClass *Ranking;
	Chara *P1;

private:
	//Directサウンドクラス
	CSoundManager * m_pSoundManager;

};

	