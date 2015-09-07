//=============================================================================
//  DirectX Audio関係の自作関数群
//  Copyright NKC Game Staff(←自分の名前)　
//-----------------------------------------------------------------------------
/*---------- 共通ヘッダファイルcommon.hに、以下を宣言する
#include <dmusici.h>    // DirectAudioを使用可能にする
#include "NKC_DAudio.h"   // NKC_DAudio.cppを利用するためのヘッダ・ファイル
----------*/
#include "NKC_DAudio.h"


// グローバル変数
/* 他のソースからも利用されるもの */
IDirectMusicPerformance8* g_pPerformance = NULL;// パフォーマンス
IDirectMusicLoader8* g_pLoader = NULL;			// ローダー
IDirectMusicAudioPath8* g_pDefAudioPath = NULL;	// デフォルト・オーディオパス
IDirectMusicAudioPath8* g_pAudioPath1;			// オーディオ・パス１（必要に応じて利用）
IDirectMusicAudioPath8* g_pAudioPath2;			// オーディオ・パス２（必要に応じて利用）
IDirectMusicSegment8* g_pSeg[32];				// セグメント
IDirectMusicSegmentState8* g_pSegState[32];		// セグメントの状態
MUSIC_TIME g_pSegLength[32];					// セグメントの長さ
long g_nMasterVolume;							// マスター・ボリューム
long g_nVolume1;								// オーディオ・パス１用のボリューム（必要に応じて利用）
long g_nVolume2;								// オーディオ・パス２用のボリューム（必要に応じて利用）

//-----------------------------------------------------------------------------------------
//DirectX Audioの初期化
//-----------------------------------------------------------------------------------------
bool InitDXAudio(HWND hWnd)
{
	HRESULT hr;

	// パフォーマンスの作成
	hr = CoCreateInstance(
		CLSID_DirectMusicPerformance,
		NULL,
		CLSCTX_INPROC,
		IID_IDirectMusicPerformance8,
		(void**)&g_pPerformance );
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"DirectMusicPerformance8オブジェクトの作成に失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	// パフォーマンスの初期化
	hr = g_pPerformance->InitAudio( 
		NULL,								// IDirectMusicインターフェイスは不要
		NULL,								// IDirectSoundインターフェイスは不要
		hWnd,								// ウィンドウのハンドル
		DMUS_APATH_SHARED_STEREOPLUSREVERB,	// デフォルトのオーディオパス・タイプ
		64,									// パフォーマンス・チャンネルの数
		DMUS_AUDIOF_ALL,					// シンセサイザの機能
		NULL);								// オーディオ・パラメータにはデフォルトを使用
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"DirectMusicPerformance8の初期化に失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	// デフォルト・オーディオパスの取得
	hr = g_pPerformance->GetDefaultAudioPath(&g_pDefAudioPath);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"デフォルト・オーディオパスの取得に失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	// オーディオ・パスを作成（必要に応じて利用する）
	/* １つ目 */
	hr = g_pPerformance->CreateStandardAudioPath( 
		DMUS_APATH_DYNAMIC_STEREO,	// パスの種類。
		64,							// パフォーマンス チャンネルの数。
		TRUE,						// ここでアクティブになる。
		&g_pAudioPath1 );			// オーディオパスを受け取るポインタ。
	if (FAILED(hr))	MessageBox(NULL,NULL,"オーディオ・パス「g_pAudioPath1」の作成に失敗", MB_ICONERROR | MB_OK);
	/* ２つ目 */
	hr = g_pPerformance->CreateStandardAudioPath( 
		DMUS_APATH_DYNAMIC_STEREO,	// パスの種類。
		64,							// パフォーマンス チャンネルの数。
		TRUE,						// ここでアクティブになる。
		&g_pAudioPath2 );			// オーディオパスを受け取るポインタ。
	if (FAILED(hr))	MessageBox(NULL,NULL,"オーディオ・パス「g_pAudioPath2」の作成に失敗", MB_ICONERROR | MB_OK);

	// マスター・ボリュームを取得し、初期値としてセット
	hr = g_pPerformance->SetGlobalParam(GUID_PerfMasterVolume, (void*)&g_nMasterVolume, sizeof(long));
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"マスター・ボリュームの取得に失敗\n", MB_ICONERROR | MB_OK);
		return false;
	}

	long nVolume1 = -200;	// ボリュームを-5.00dbにする

	hr = g_pAudioPath1->SetVolume(nVolume1, 0);  // ボリュームを即時変更
	if (FAILED(hr)) {
		// ボリュームの変更に失敗
	}

	g_nMasterVolume = 0;
	g_nVolume1 = g_nVolume2 = g_nMasterVolume;

	// ローダーの作成
	hr = CoCreateInstance(
		CLSID_DirectMusicLoader,
		NULL, 
		CLSCTX_INPROC,
		IID_IDirectMusicLoader8,
		(void**)&g_pLoader );
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"DirectMusicLoader8オブジェクトの作成に失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	// ローダーの初期化（検索パスをカレント・ディレクトリに設定）
	CHAR strPath[MAX_PATH];
	DWORD cntPath = GetCurrentDirectory(MAX_PATH, strPath);
	if (cntPath==0 || MAX_PATH<cntPath) {
		MessageBox(NULL,NULL,"カレント・ディレクトリの取得に失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	// マルチ・バイト文字をUNICODEに変換
	WCHAR wstrSearchPath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH);

	// ローダーに検索パスを設定
	hr = g_pLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wstrSearchPath, FALSE);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"検索パスの設定に失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
// 関数名：LoadSeg(int seg, char* FileName, bool flgMIDI)
// 引　数：int seg 読み込ませたいセグメント番号
//         char FileName 読み込ませたいファイルの名前
// 機　能：ファイル名とセグメント番号を指定してファイルから読み込ませる。
//-----------------------------------------------------------------------------------------
bool LoadSeg(int seg, char* FileName, bool flgMIDI,bool flgLOOP) 
{
	HRESULT	hr;

	// セグメントの初期化
	if(g_pSeg[seg] != NULL) g_pSeg[seg] = NULL;

	// セグメントにファイル名を指定してロードする
	WCHAR wstrFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, FileName, -1, wstrFileName, MAX_PATH);
	hr = g_pLoader->LoadObjectFromFile(
		CLSID_DirectMusicSegment,
		IID_IDirectMusicSegment8,
		wstrFileName,
		(LPVOID*)&g_pSeg[seg]);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"MIDIファイルのロードに失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	// MIDIファイルの場合、パラメータを設定する
	// （一度スタートすると、停止が実行されるまでループさせる）
	if (flgMIDI) {
		hr = g_pSeg[seg]->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
		if (FAILED(hr)) {
			MessageBox(NULL,NULL,"セグメントのパラメータ設定に失敗", MB_ICONERROR | MB_OK);
			return false;
		}
	}
	if(flgLOOP){
		// セグメントの長さを取得
		hr = g_pSeg[seg]->GetLength(&g_pSegLength[seg]);
		if (FAILED(hr))	MessageBox(NULL,NULL,"セグメントの長さの取得に失敗", MB_ICONERROR | MB_OK);

		// セグメントのループ回数設定
		//（０：ループなし　DMUS_SEG_REPEAT_INFINITE：明示的に止められるまでループ）
		hr = g_pSeg[seg]->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
		if (FAILED(hr))	MessageBox(NULL,NULL,"セグメントのループ回数の設定に失敗", MB_ICONERROR | MB_OK);
	}

	// バンドのダウンロード
	hr = g_pSeg[seg]->Download(g_pPerformance);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"セグメントのバンドダウンロードに失敗", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//	セグメントの終了処理
//-----------------------------------------------------------------------------------------
bool ReleaseDMSegment(void)
{
	HRESULT hr;

	// 演奏停止
	if (g_pPerformance) {
		hr = g_pPerformance->Stop(NULL, NULL, 0, 0);
		if (FAILED(hr))	MessageBox(NULL,NULL,"全セグメントの演奏停止に失敗", MB_ICONERROR | MB_OK);
	}

	// 音色データのアンロード・セグメントの開放
	for (int i=0 ; i<32 ; i++) {
		if (g_pSeg[i]) {
			hr = g_pSeg[i]->Unload(g_pPerformance);
			if (FAILED(hr))	MessageBox(NULL,NULL,"セグメントの音色データのアンロードに失敗", MB_ICONERROR | MB_OK);
		}
		RELEASE(g_pSegState[i]);
		RELEASE(g_pSeg[i]);
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//	DirectX Audioの終了処理
//-----------------------------------------------------------------------------------------
bool ReleaseDXAudio(void)
{
	HRESULT hr;

	// ローダを開放
	RELEASE(g_pLoader);

	// オーディオパスを開放
	RELEASE(g_pDefAudioPath);
	RELEASE(g_pAudioPath1);
	RELEASE(g_pAudioPath2);

	// パフォーマンスを開放
	if (g_pPerformance) {
		hr = g_pPerformance->CloseDown();
		if (FAILED(hr))	MessageBox(NULL,NULL,"IDirectMusicPerformance8::CloseDownに失敗", MB_ICONERROR | MB_OK);
	}
	RELEASE(g_pPerformance);

	return true;
}
