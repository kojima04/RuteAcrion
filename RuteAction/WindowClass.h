#pragma once

#include "Common.h"
#include <string>

/**************************************************************************************************
		name	:	Window
		brief	:	ウィンドウを管理する
***************************************************************************************************/
class Window
{
public:

	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	Window();
	//---------------------------------------------------------
	// デストラクタ
	//---------------------------------------------------------
	~Window();

	HRESULT	Init(HINSTANCE, int);					
	static LRESULT CALLBACK	WindowProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK	WindowProc_Box(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK	WindowProc_BlkInfo(HWND, UINT, WPARAM, LPARAM);


	HRESULT	MutexCreate();							//ミューテックス開始
	void	MutexDestroy();							//ミューテックス終了

	void	Loop();									//メッセージループ
	int		ReturnWinMain(){return msg.wParam;}		//WinMain関数への返り値
	
protected:
	HWND	hWnd;				//ウィンドウハンドル
	MSG		msg;					//メッセージ
	HWND	hWnd_Box;				//ウィンドウハンドル
	MSG		msg_Box;					//メッセージ
	HWND	hWnd_BlkInfo;				//ウィンドウハンドル
	MSG		msg_BlkInfo;					//メッセージ
	HANDLE	hMutex;				//ミューテックス
	System	*Sys;
	bool 	isFullScreen;
public:
	HWND	GetWindowHandle(){return hWnd;}
	MSG		GetMessage(){return msg;}
};



