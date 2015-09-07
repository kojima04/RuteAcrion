#pragma once

#include <dmusici.h>
#define RELEASE(x) if(x!=NULL){x->Release();x=NULL;}
//-----------------------------------------------------------------------------
// File: NKC_DAudio.h
// Desc: NKC_DAudio.cppで使用するもののうち、他のソースでも使うものを宣言
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define VOLUME_MAX 1000
#define VOLUME_MIN -10000

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
extern IDirectMusicPerformance8* g_pPerformance;// パフォーマンス
extern IDirectMusicLoader8* g_pLoader;			// ローダー
extern IDirectMusicAudioPath8* g_pDefAudioPath;	// デフォルト・オーディオパス
extern IDirectMusicAudioPath8* g_pAudioPath1;	// オーディオ・パス１（必要に応じて作成）
extern IDirectMusicAudioPath8* g_pAudioPath2;	// オーディオ・パス２（必要に応じて作成）
extern IDirectMusicSegment8* g_pSeg[32];		// セグメント
extern IDirectMusicSegmentState8* g_pSegState[32]; // セグメントの状態
extern MUSIC_TIME g_pSegLength[32];				// セグメントの長さ
extern long g_nMasterVolume;					// マスター・ボリューム
extern long g_nVolume1;							// オーディオ・パス１用のボリューム（必要に応じて利用）
extern long g_nVolume2;							// オーディオ・パス２用のボリューム（必要に応じて利用）

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
bool InitDXAudio(HWND hWnd);				// DirectAudioの初期化
bool ReleaseDMSegment(void);		// セグメントの削除
bool ReleaseDXAudio(void);			// DirectAudioオブジェクトの削除
bool LoadSeg(int, char*, bool,bool);		// 音ファイルの読み込み