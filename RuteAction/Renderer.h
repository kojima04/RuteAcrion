#pragma once

//=============================================================
// include
//=============================================================
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

//=============================================================
// 依存するクラス
//=============================================================
class DrawObject;

//=============================================================
// Renderer
// DirectGraphicsの初期化及び管理クラス
//=============================================================
class Renderer
{
public:
	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	Renderer();
	//---------------------------------------------------------
	// デストラクタ
	//---------------------------------------------------------
	~Renderer();

private:

	LPDIRECT3D9				m_pD3D;			// IDirect3D9インターフェイスへのポインタ
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// IDirect3DDevice9インターフェイスへのポインタ
	D3DPRESENT_PARAMETERS	m_D3DPP;		// デバイスのプレゼンテーションパラメータ
	D3DXVECTOR3 CPos;
	D3DXVECTOR3 CFocus;

public:
	//---------------------------------------------------------
	// DirectGraphicsの初期化
	//		引数
	//			hWnd			: ウィンドウハンドル
	//			isFullScreen	: フルスクリーンの場合TRUE
	//			clientWidth		: クライアント領域の幅
	//			clientHeight	: クライアント領域の高さ
	//		戻り値
	//			成功したらS_OK
	//---------------------------------------------------------
	HRESULT Initialize(HWND hWnd, BOOL isFullScreen, int clientWidth, int clientHeight);

	//---------------------------------------------------------
	// 描画する
	// 引数
	//		pObject	: 描画するオブジェクト
	//---------------------------------------------------------
	void Render(DrawObject *pObject);
	void RenderBGN();
	void RenderEND();
	void InitRender();

	void SetCPos(float x, float y, float z);
	void SetCPos(D3DXVECTOR3 &cpos){CPos = cpos;}
	void SetCFocus(float x, float y, float z);
	void SetCFocus(D3DXVECTOR3 &cfocus){CFocus = cfocus;}

	D3DXVECTOR3	* GetCPos(){return &CPos;}
	D3DXVECTOR3	* GetCFocus(){return &CFocus;}

	float C_Xangle;
	float C_Yangle;
	float C_Zangle;

	D3DXMATRIX m_proj;	// 射影行列
	D3DXMATRIX m_view;	// ビューイング行列
	D3DXMATRIX m_view_R;//回転行列
	D3DXMATRIX m_view_M;//平行移動行列
	D3DXMATRIX m_screen;
	
	//---------------------------------------------------------
	// デバイスの取得
	// 戻り値
	//		成功したらIDirect3DDevice9インターフェイスへのポインタ
	//		失敗したら0
	//---------------------------------------------------------
	LPDIRECT3DDEVICE9 getDevice() const { return m_pD3DDevice; }

	void Transform(LPDIRECT3DDEVICE9 pD3DDevice);
};

	