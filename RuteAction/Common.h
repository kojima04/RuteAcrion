#pragma once
#include "SoundManager.h"
#include "Renderer.h"
#include "Texture.h"
#include "Chara.h"
#include "Bullet.h"
#include "Block.h"
#include "BlockManager.h"
#include "Tytle.h"
#include "Stage.h"
#include "Ranking.h"
#include "CheckHit.h"
#include "NKC_DInput.h"
#include "SystemClass.h"
#include "WindowClass.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "d3dxof.lib ")
#pragma comment(lib, "dsound.lib ")
#pragma comment(lib, "winmm.lib")

//#define STAGEEDIT
#define CLIENT_WIDTH	800			// クライアント領域のデフォルトの幅, フルスクリーンの場合は水平方向解像度
#define	CLIENT_HEIGHT	600			// クライアント領域のデフォルトの高さ, フルスクリーンの場合は垂直方向解像度
#define BOX_WIDTH	400			
#define	BOX_HEIGHT	200	
#define BLKINF_WIDTH	300			
#define	BLKINF_HEIGHT	300		
#define WINDOW_NAME		"RuteAction"	// ウィンドウ名
#define COMBONUM 3
#define BUTTONNUM 2
#define COMBO_BLOCKNUM 2
#define EDIT_BLOCKNUM 2

#define PAI    3.1415927f
#define sqr(x) ( (x)*(x) )

typedef enum ___ComboUse {
	COMBO_USE_STAGE,
	COMBO_USE_CAPTER,
	COMBO_USE_BACK,
	COMBO_USEMAX,
}e_ComboUse;

extern HWND combo[COMBONUM];
extern HWND button[BUTTONNUM][2];
extern bool Clickflg[BUTTONNUM][2];
extern HWND combo_Block[COMBO_BLOCKNUM];

extern HWND edit_Block[EDIT_BLOCKNUM];
extern int G_SelectBlockRot;
extern LPDIRECT3DDEVICE9 G_Device;
extern MSTATE MState;  // マウス情報
extern POINT m_MouseLocalPos;






