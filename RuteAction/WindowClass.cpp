#include <time.h>
#include <iostream>
#include <crtdbg.h>

#include "WindowClass.h"

MSTATE MState;  // マウス情報
HWND combo[COMBONUM];
HWND button[BUTTONNUM][2];
bool Clickflg[BUTTONNUM][2];
HWND combo_Block[COMBO_BLOCKNUM];
HWND edit_Block[EDIT_BLOCKNUM];
int G_SelectBlockRot;


LRESULT CALLBACK WndProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp);

/*************************************************************************************************
		name	:	Window::Window
		brief	:	デフォルトコンストラクタ
*************************************************************************************************/
Window::Window()
{
	// フルスクリーンにするかどうかの判定
	isFullScreen = false;
}

Window::~Window()
{
	delete Sys;
}

/*************************************************************************************************
		name	:	WinMain
		brief	:	アプリケーションのエントリーポイント
		return	:
**************************************************************************************************/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	static Window wnd_main;

	   // COM の初期化
    if ( FAILED(CoInitialize(NULL)) ) return (FALSE);
	
    // DirectX Audioの初期化
//	if ( FAILED(InitDXAudio(wnd_main.GetWindowHandle())) ) return (FALSE);


	// DirectInputオブジェクトの初期化
	if ( FAILED(InitDInput(hInstance)) ) return (FALSE);


	// Keyboard Object の初期化
//	if ( FAILED(InitDXAudio(wnd_main.GetWindowHandle())) ) return (FALSE);
    if ( FAILED(InitKeyboard(wnd_main.GetWindowHandle())) ) return (FALSE);
	 // Joypad Object の初期化
    if ( FAILED(InitJoypad2()) ) OutputDebugString("Joypad Not Found\n");

	// Mouse Object の初期化
    if ( FAILED(InitMouse(wnd_main.GetWindowHandle())) ) return (FALSE);

	SetRect(&MState.moveRect, 10, 10, 630, 470);	// マウスカーソルの動く範囲
	MState.x = MState.moveRect.left;	// マウスカーソルのＸ座標を初期化
	MState.y = MState.moveRect.top;	// マウスカーソルのＹ座標を初期化
	MState.lButton = false;	// 左ボタンの情報を初期化
	MState.rButton = false;	// 右ボタンの情報を初期化
	MState.cButton = false;	// 中央ボタンの情報を初期化
	MState.moveAdd = 2;	// マウスカーソルの移動量を設定
	SetRect(&MState.imgRect, 400, 0, 420, 20);	// マウスカーソル画像の矩形を設定
	MState.imgWidth = MState.imgRect.right - MState.imgRect.left;	// 画像の幅を計算
	MState.imgHeight = MState.imgRect.bottom - MState.imgRect.top;	// 画像の高さを計算

	//メモリリークを発見する
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if(FAILED(wnd_main.Init(hInstance, nCmdShow)))
	{
		return 0;
	}
	
	wnd_main.Loop();
	
	return static_cast<int>(wnd_main.GetMessage().wParam);
}

/***********************************************************************************************
		name	:	WindowProc
		brief	:	プロシージャ
		return	:
***********************************************************************************************/
HRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		srand((unsigned)timeGetTime());
		break;
	
	case WM_DESTROY:
		ReleaseDInput();
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		// どのキーが押されたか判定
		switch(wParam)
		{
		case VK_ESCAPE:
			// WM_DESTROYメッセージを送信
			SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			break;

		default:
			break;
		}
		break;


	case WM_SETCURSOR:
		while(1)
		{
			if(ShowCursor(false) <= 0)
				break;
		}
		return 0;
		
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

/**************************************************************************************************
		name	:	MessageLoop
		brief	:	メッセージループ
		return	:	HRESULT
***************************************************************************************************/
void Window::Loop()
{	
	// メッセージ処理および描画ループ
	while(true)
	{
		// 処理すべきメッセージがある
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// PostQuitMessageが呼ばれた
			if(msg.message == WM_QUIT)
			{
				// ループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// 処理すべきメッセージがない場合
		else
		{
				//現在の時間を取得
			Sys->nowTime = timeGetTime();

			//経過時間が16msより大きいなら
			if(Sys->nowTime - Sys->beforeTime > 8){
				//過去の時刻の更新
				Sys->beforeTime = Sys->nowTime;
				if(FAILED(g_diJoyState2[0]))
					g_diJoyCount = 1;
				if(FAILED(g_diJoyState2[1]))
					g_diJoyCount = 2;
				
				GetCursorPos(&m_MouseLocalPos);
				ScreenToClient(hWnd,&m_MouseLocalPos);
				Sys->SetInputState();
				Sys->Input();
				Sys->Main();

			}
			// ウィンドウが見えている時だけ描画するための処理
			WINDOWPLACEMENT wndpl;
			GetWindowPlacement(hWnd, &wndpl);	// ウィンドウの状態を取得
			if((wndpl.showCmd != SW_HIDE) &&
				(wndpl.showCmd != SW_MINIMIZE) &&
				(wndpl.showCmd != SW_SHOWMINIMIZED) &&
				(wndpl.showCmd != SW_SHOWMINNOACTIVE))
			{//--- ここで描画 ---//	
				Sys->renderer.RenderBGN();
				Sys->Paint();
				Sys->renderer.RenderEND();
			}
		}
		Sleep(1);
	}
}

/*************************************************************************************************
		name	:	InitApp
		brief	:	ウィンドウの生成
		return	:	E_FAIL	失敗
************************************************************************************************/	
HRESULT Window::Init(HINSTANCE hInstance, INT nCmdShow)
{	
	#ifdef STAGEEDIT

	WNDCLASS wc_box;

	wc_box.style		= CS_HREDRAW | CS_VREDRAW;
	wc_box.lpfnWndProc	= WindowProc_Box;
	wc_box.cbClsExtra	= wc_box.cbWndExtra	= 0;
	wc_box.hInstance		= hInstance;
	wc_box.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	wc_box.hCursor		= LoadCursor(NULL , IDC_ARROW);
	wc_box.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc_box.lpszMenuName	= NULL;
	wc_box.lpszClassName	= TEXT("STAGE Window");

	if (!RegisterClass(&wc_box)) 
		return -1;

	hWnd_Box = CreateWindow(
			TEXT("STAGE Window") , TEXT("STAGE Window") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			1000, 						// ウインドウの横方向の位置
			500, 						// ウインドウの縦方向の位置
			BOX_WIDTH, 			// ウインドウの幅
			BOX_HEIGHT,			// ウインドウの高さ
			NULL , NULL , hInstance , NULL
	);

	WNDCLASS wc_blkinf;

	wc_blkinf.style		= CS_HREDRAW | CS_VREDRAW;
	wc_blkinf.lpfnWndProc	= WindowProc_BlkInfo;
	wc_blkinf.cbClsExtra	= wc_blkinf.cbWndExtra	= 0;
	wc_blkinf.hInstance		= hInstance;
	wc_blkinf.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	wc_blkinf.hCursor		= LoadCursor(NULL , IDC_ARROW);
	wc_blkinf.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc_blkinf.lpszMenuName	= NULL;
	wc_blkinf.lpszClassName	= TEXT("Box Window");

	if (!RegisterClass(&wc_blkinf)) 
		return -1;

	hWnd_BlkInfo = CreateWindow(
			TEXT("Box Window") , TEXT("Box Window") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			1000, 						// ウインドウの横方向の位置
			200, 						// ウインドウの縦方向の位置
			BLKINF_WIDTH, 			// ウインドウの幅
			BLKINF_HEIGHT,			// ウインドウの高さ
			NULL , NULL , hInstance , NULL
	);

	#endif
	
	char clsName[] = "RuteAction";	// ウィンドウクラス名

	// ウィンドウクラスの初期化
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),					// この構造体のサイズ
		NULL,								// ウィンドウのスタイル
		WindowProc, 						// メッセージ処理関数の登録
		0,									// 通常は使わないので常に0
		0,									// 通常は使わないので常に0
		hInstance,							// インスタンスへのハンドル
		LoadIcon(NULL, IDI_APPLICATION),	// アイコン
		LoadCursor(NULL, IDC_ARROW),		// カーソルの形
		(HBRUSH)COLOR_BACKGROUND+1,			// 背景
		NULL,								// メニューなし
		clsName,							// クラス名の指定
		NULL								// 小アイコン（なし）
	};

	// ウィンドウクラスの登録
	if(RegisterClassEx(&wcex) == 0)
	{
		MessageBox(NULL, "ウィンドウクラスの作成に失敗しました.\nプログラムを終了します.", NULL, MB_OK | MB_ICONERROR);
		return 0;	// 登録失敗
	}

	// ウィンドウの作成
	if(isFullScreen)	// フルスクリーン
	{
		hWnd = CreateWindow(
					clsName, 				// 登録されているクラス名
					WINDOW_NAME, 			// ウインドウ名
					WS_POPUP,				// ウインドウスタイル（ポップアップウインドウを作成）
					0, 						// ウインドウの横方向の位置
					0, 						// ウインドウの縦方向の位置
					CLIENT_WIDTH, 			// ウインドウの幅
					CLIENT_HEIGHT,			// ウインドウの高さ
					NULL,					// 親ウインドウのハンドル（省略）
					NULL,					// メニューや子ウインドウのハンドル
					hInstance, 				// アプリケーションインスタンスのハンドル
					NULL					// ウインドウの作成データ
				);

		if(hWnd == NULL)
		{
			MessageBox(NULL, "ウィンドウの生成に失敗しました.\nプログラムを終了します.", NULL, MB_OK | MB_ICONERROR);
			return 0;	// ウィンドウの生成に失敗
		}
	}
	else				// ウィンドウ
	{
		hWnd = CreateWindow(
					clsName,					// 登録されているクラス名
					WINDOW_NAME,				// ウインドウ名
					WS_OVERLAPPED | WS_SYSMENU,	// ウインドウスタイル（オーバーラップウィンドウを作成）
					CW_USEDEFAULT,				// ウインドウの横方向の位置（デフォルトの指定を使う）
					CW_USEDEFAULT,				// ウインドウの縦方向の位置（デフォルトの指定を使う）
					CW_USEDEFAULT,				// ウインドウの幅（デフォルトの指定を使う）
					CW_USEDEFAULT,				// ウインドウの高さ（デフォルトの指定を使う）
					NULL,						// 親ウインドウのハンドル（省略）
					NULL,						// メニューや子ウインドウのハンドル
					hInstance,					// アプリケーションインスタンスのハンドル
					NULL						// ウインドウの作成データ
					);

		if(hWnd == NULL)
		{
			MessageBox(NULL, "ウィンドウの生成に失敗しました.\nプログラムを終了します.", NULL, MB_OK | MB_ICONERROR);
			return 0;	// ウィンドウの生成に失敗
		}
	// ウィンドウサイズを再設定する
		RECT rect;
		int ww, wh;
		int cw, ch;
		// クライアント領域の外の幅を計算
		GetClientRect(hWnd, &rect);		// クライアント部分のサイズの取得
		cw = rect.right - rect.left;	// クライアント領域外の横幅を計算
		ch = rect.bottom - rect.top;	// クライアント領域外の縦幅を計算

		// ウインドウ全体の横幅の幅を計算
		GetWindowRect(hWnd, &rect);		// ウインドウ全体のサイズ取得
		ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
		wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算
		ww = ww - cw;					// クライアント領域以外に必要な幅
		wh = wh - ch;					// クライアント領域以外に必要な高さ

		// ウィンドウサイズの再計算
		ww = CLIENT_WIDTH + ww;			// 必要なウインドウの幅
		wh = CLIENT_HEIGHT + wh;		// 必要なウインドウの高さ

		// ウインドウサイズの再設定
		SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	}

	Sys = new System(isFullScreen,hWnd);


	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ValidateRect(hWnd, 0);				//WM_PAINTを無効に

	HWND hWnd2 = NULL;

	return S_OK;
}


/************************************************************************************************
		name	:	InitMutex
		brief	:	ミューテックス開始処理。多重起動の防止
		return	:	HRESULT
************************************************************************************************/
HRESULT Window::MutexCreate()
{
	hMutex = CreateMutex(NULL, TRUE, TEXT("Mutex"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, TEXT("多重起動を確認しました"), 0, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	
	return S_OK;
}

/***********************************************************************************************
		name	:	FinaMutex
		brief	:	ミューテックス終了処理
**************************************************************************************************/
VOID Window::MutexDestroy()
{
	CloseHandle(hMutex);
}


HRESULT CALLBACK Window::WindowProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	
	switch (msg) {
	case WM_DESTROY:
		// アプリケーションの終了を通知
		// DirectInputオブジェクトの開放
	    ReleaseDInput();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		for(int i = 0;i < COMBONUM;i++)
		{
		combo[i] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
	//		100 + 140*(x > 2), 66*x - 198 * (x>2), 50 , 300 , hwnd , (HMENU)1 ,
			BOX_WIDTH/4*(1 + 2*(i > 2)),
			BOX_HEIGHT/6*(2*(i % 3)), BOX_WIDTH/4 , 300 , hwnd , (HMENU)i ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		for (int x = 0 ; x < 1 ; x++)
			SendMessage(combo[i] , CB_ADDSTRING , 0 , (LPARAM)"NOTHING");
			SendMessage(combo[i] , CB_SETCURSEL  , 0 , 0);
		}

		for(int i = 0;i < BUTTONNUM;i++)
		{
			button[i][0] = CreateWindow( 
			"button",                           // スタティックコントロールのクラス名
			"SAVE",                               // 表示する文字列
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
												// ウインドウスタイル
			BOX_WIDTH/4*(2),
			BOX_HEIGHT/6*(2*(i % 3)),                               // 左上の座標
			BOX_WIDTH/4 , 25 ,
			hwnd,                            // 親ウインドウのウインドウハンドル
			(HMENU)i,                               // メニューハンドル。NULLでよい。
			((LPCREATESTRUCT)(lp))->hInstance,                          // アプリケーションのインスタンスハンドル。
			NULL                                // ウインドウ作成データ。NULLでよい
			) ;

			button[i][1] = CreateWindow( 
			"button",                           // スタティックコントロールのクラス名
			"LOAD",                               // 表示する文字列
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
												// ウインドウスタイル
			BOX_WIDTH/4*(3),
			BOX_HEIGHT/6*(2*(i % 3)),                               // 左上の座標
			BOX_WIDTH/4 , 25 ,
			hwnd,                            // 親ウインドウのウインドウハンドル
			(HMENU)(i + BUTTONNUM),                               // メニューハンドル。NULLでよい。
			((LPCREATESTRUCT)(lp))->hInstance,                          // アプリケーションのインスタンスハンドル。
			NULL                                // ウインドウ作成データ。NULLでよい
			) ;
		}


		return 0;

	case WM_COMMAND:
		for(int i = 0;i < BUTTONNUM;i++)
		{
			if( LOWORD(wp) == i)
			{
				if(HIWORD( wp) == BN_CLICKED)
				{
					// クリックされたときの処理
					Clickflg[i][0] = true;
				}
			}
			if( LOWORD(wp) == i + BUTTONNUM)
			{
				if(HIWORD( wp) == BN_CLICKED)
				{
					// クリックされたときの処理
					Clickflg[i][1] = true;
				}
			}
		}


	case WM_SETCURSOR:
		while(1)
		{
			if(ShowCursor(true) > 0)
				break;
		}
		return 0;
	
	}
	return DefWindowProc(hwnd , msg , wp , lp);

}

HRESULT CALLBACK Window::WindowProc_BlkInfo(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	char strText[64]={0};

	switch (msg) {
	case WM_DESTROY:
		// アプリケーションの終了を通知
		// DirectInputオブジェクトの開放
	    ReleaseDInput();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		combo_Block[0] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
			0,
			0, BLKINF_WIDTH/4 , 300 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		combo_Block[1] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
			BLKINF_WIDTH/4,
			0, BLKINF_WIDTH/4 , 300 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);

			for (int x = 0 ; x < 1 ; x++)
			{
				SendMessage(combo_Block[x] , CB_ADDSTRING , 0 , (LPARAM)"NULL");
				SendMessage(combo_Block[x] , CB_SETCURSEL  , 0 , 0);
			}
		
		edit_Block[0] = CreateWindow(
			TEXT("EDIT") ,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT ,
			0,
			40, BLKINF_WIDTH/4 , 25 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		SetWindowText(edit_Block[0],(LPCTSTR)"0") ;

		return 0;		

	case WM_SETCURSOR:
		while(1)
		{
			if(ShowCursor(true) > 0)
				break;
		}
		return 0;
	case WM_COMMAND:
		if (LOWORD(wp) == 1 && HIWORD(wp) == EN_UPDATE) {
			//strText = new char;
			GetWindowText(edit_Block[0] , strText,64);
			G_SelectBlockRot = atoi(strText); 
			//delete strText;
		}
		return 0;

		
	}
	return DefWindowProc(hwnd , msg , wp , lp);

}
