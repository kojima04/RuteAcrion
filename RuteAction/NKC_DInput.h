#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>     // DirectInputを使用可能にする
#define RELEASE(x) if(x!=NULL){x->Release();x=NULL;}


//-----------------------------------------------------------------------------
// File: NKC_DInput.h
// Desc: NKC_DInput.cppで使用するもののうち、他のソースでも使うものを宣言
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
/* Joypad情報 */

#define BUTTON_UP		0x00000001l	// 方向キー上(dijs.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(dijs.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(dijs.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(dijs.IX>0)
#define BUTTON_A		0x00000010l	// Ａボタン(dijs.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// Ｂボタン(dijs.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// Ｃボタン(dijs.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// Ｘボタン(dijs.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// Ｙボタン(dijs.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// Ｚボタン(dijs.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// Ｌボタン(dijs.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// Ｒボタン(dijs.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(dijs.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// Ｍボタン(dijs.rgbButtons[9]&0x80)




#define JOYPADMAX		4			// 同時に接続するジョイパッドの最大数をセット
//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
/* マウスポインタの情報 */
typedef struct _MSTATE {
    RECT    moveRect;     // 画面上で動ける範囲
    int     x;            // X座標
    int     y;            // Y座標
    bool    lButton;      // 左ボタン
    bool    rButton;      // 右ボタン
    bool    cButton;      // 真ん中ボタン
	bool    exlButton;    // 左ボタン
    bool    exrButton;    // 右ボタン
    bool    excButton;    // 真ん中ボタン	
    int     moveAdd;      // 移動量
    RECT    imgRect;      // マウス用画像矩形
    int     imgWidth;     // マウス画像幅
    int     imgHeight;    // マウス画像高さ
} MSTATE, *LPMSTATE;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
/* キーボード用 */
extern BYTE					g_diKeyState[256];
extern BYTE					g_ExdiKeyState[256];
/* ジョイパッド用 */
extern DWORD				g_diJoyState;
extern DWORD				g_diJoyState2[JOYPADMAX];
extern DWORD				g_ExdiJoyState2[JOYPADMAX];
extern int					g_diJoyCount;
/* マウス用 */
extern DIMOUSESTATE			g_diMouseState;

//-----------------------------------------------------------------------------
// プロトタイプ宣言 function-prototypes
//-----------------------------------------------------------------------------
/* DirectInput共通 */
bool InitDInput(HINSTANCE);
void ReleaseDInput(void);
/* キーボード用 */
bool InitKeyboard(HWND hwnd);
void GetKeyboardState(void);
/* ジョイパッド用 */
bool InitJoypad(void);
void GetJoypadState(void);
bool InitJoypad2(void);
void GetJoypadState2(void);
/* マウス用 */
bool InitMouse(HWND hwnd);
void GetMouseState(void);
POINT& GetMouseLocalPos();
