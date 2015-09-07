//=============================================================
// Window.cpp
// WinMain関数および,ウインドウプロシージャの実装
//=============================================================

//=============================================================
// include
//=============================================================
#include <crtdbg.h>		// Memoryリーク発見関数用
#include <windows.h>

#include "Common.h"


//=============================================================
// マクロ定義
//=============================================================

//=============================================================
// 列挙体定義
//=============================================================

// シーン切り替え用
enum eSceneObject
{
	NONE,
	TRIANGLE,
	SQUARE,
	TEXTURE,
	ROBOT,
};

//=============================================================
// 関数プロトタイプ宣言
//=============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp);

//=============================================================
// グローバル変数定義
//=============================================================
eSceneObject g_key = NONE;
MSTATE MState;  // マウス情報
HWND combo[6];

LPCTSTR strBlock[] = {
	TEXT("NULL") ,
};




// 例外クラス
class KeyNumberException
{
private:
	eSceneObject m_key;
public:
	KeyNumberException(eSceneObject key)
		: m_key(key) {}
	eSceneObject getErrorKey() const { return m_key; }
};

//-------------------------------------------------------------
// WinMain関数
// 引数
//		hInstance     : 現在のインスタンスのハンドル
//		hPrevInstance : 以前のインスタンスのハンドル
//		lpCmdLine	  : コマンドラインパラメータ
//		nCmdShow	  : ウィンドウの表示状態
// 戻り値
//		成功したら0以外の値
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	


	// ウィンドウ作成
	// フルスクリーンにするかどうかの判定
	bool isFullScreen = false;
	HWND hWnd = NULL;
	char clsName[] = "RuteAction";	// ウィンドウクラス名
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// 多重起動の防止
	HANDLE hMutex = NULL;
	const char mutexName[] = "RuteAction";
	hMutex = CreateMutex(NULL, TRUE, mutexName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "多重起動を確認しました.\nプログラムを終了します.", "エラー：多重起動", MB_OK);
		return 0;
	}


    // COM の初期化
    if ( FAILED(CoInitialize(NULL)) ) return (FALSE);
	
    // DirectX Audioの初期化
    if ( FAILED(InitDXAudio(hWnd)) ) return (FALSE);


	// DirectInputオブジェクトの初期化
	if ( FAILED(InitDInput(hInstance)) ) return (FALSE);
	// Keyboard Object の初期化
    if ( FAILED(InitKeyboard(hWnd)) ) return (FALSE);
	 // Joypad Object の初期化
    if ( FAILED(InitJoypad2()) ) OutputDebugString("Joypad Not Found\n");

	// Mouse Object の初期化
    if ( FAILED(InitMouse(hWnd)) ) return (FALSE);

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

	HWND hwnd;
	
	WNDCLASS winc;

	winc.style		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc_Box;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInstance;
	winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= TEXT("KITTY");

	if (!RegisterClass(&winc)) return -1;

	hwnd = CreateWindow(
			TEXT("KITTY") , TEXT("Kitty on your lap") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			1000, 						// ウインドウの横方向の位置
			300, 						// ウインドウの縦方向の位置
			300, 			// ウインドウの幅
			200,			// ウインドウの高さ
			NULL , NULL , hInstance , NULL
	);


	// Memoryリーク発見関数
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

		// マウスカーソルを非表示にする
		//ShowCursor(FALSE);

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

	// ウィンドウの表示
    ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);

	System *Sys = new System(isFullScreen,hWnd,hwnd);

	eSceneObject prev_key = NONE;


	



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
				
				
				Sys->Paint();
			}
		}

		// OSに制御を明け渡すためのスリープ
		Sleep(1);
	}

	


	//============//
	//  解放処理  //
	//============//
	
	delete Sys;

	 // セグメントの開放処理
    ReleaseDMSegment();

    // DirectX Audioの開放処理
    ReleaseDXAudio();


	// ミューテックスオブジェクトの解放(多重起動の防止用)
	CloseHandle(hMutex);
	hMutex = NULL;

	return static_cast<int>(msg.wParam);
}

//-------------------------------------------------------------
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
	{
	case WM_DESTROY:
		// アプリケーションの終了を通知
		// DirectInputオブジェクトの開放
	    ReleaseDInput();
		PostQuitMessage(0);
		break;

	// キーが押された
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
		ShowCursor(false);
		return 0;

	//case WM_LBUTTONDOWN:

	
	//case WM_MOUSEMOVE:	// マウスカーソルが移動したときに送られてくる
	//	// 移動先の座標を取得
	//	m_MouseLocalPos.x = LOWORD( lParam );
	//	m_MouseLocalPos.y = HIWORD( lParam );

	//	return 0;


	case WM_KILLFOCUS:
		
		m_MouseLocalPos.x = -10000;
		m_MouseLocalPos.y = -10000;
		MState.cButton = MState.rButton = MState.lButton = false;
		//SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			
		return 0;

	// デフォルトの処理
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}




LRESULT CALLBACK WndProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	
	int i;
	PSTR strText;

	switch (msg) {
	case WM_DESTROY:
		// アプリケーションの終了を通知
		// DirectInputオブジェクトの開放
	    ReleaseDInput();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		for(int x = 0;x < 6;x++)
		{
		combo[x] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
			100 + 140*(x > 2), 66*x - 198 * (x>2), 50 , 300 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		for (i = 0 ; i < 5 ; i++)
			SendMessage(combo[x] , CB_ADDSTRING , 0 , (LPARAM)strBlock[i]);
			SendMessage(combo[x] , CB_SETCURSEL  , 0 , 0);
		}
		strText = (PSTR)malloc(
				SendMessage(combo[1] , CB_GETLBTEXTLEN ,
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) , 0
				) + 1
			);
				SendMessage(combo[1] , CB_GETLBTEXT , 
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) ,
				(LPARAM)strText
			);
			SetWindowText(hwnd , (LPCTSTR)strText);
			free(strText);

		return 0;		
	case WM_COMMAND:

		if (HIWORD(wp) == CBN_SELCHANGE) {

			strText = (PSTR)malloc(
				SendMessage(combo[1] , CB_GETLBTEXTLEN ,
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) , 0
				) + 1
			);
				SendMessage(combo[1] , CB_GETLBTEXT , 
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) ,
				(LPARAM)strText
			);
			SetWindowText(hwnd , (LPCTSTR)strText);
			free(strText);
		}

		return 0;


	
	case WM_SETCURSOR:
		ShowCursor(true);
		return 0;
	
	}
	return DefWindowProc(hwnd , msg , wp , lp);

}
