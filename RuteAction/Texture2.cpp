//=============================================================
// Triangle.cpp
// 三角形描画クラスの実装
//=============================================================

//=============================================================
// include
//=============================================================
#include "Texture2.h"

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
Triangle::Triangle()
: m_pVB(0), MY_VERTEX_FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Triangle::~Triangle()
{
	// 頂点バッファの解放
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

//-------------------------------------------------------------
// 三角形を生成
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
// 戻り値
//		成功したらS_OK
//-------------------------------------------------------------
HRESULT Triangle::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// 頂点バッファの初期化
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	// デバイスのチェック
	if(pD3DDevice == 0)
	{
		return E_FAIL;	// デバイスへのポインタが無い
	}

	HRESULT hr = 0;

	//-------------------------------------------------------------
	// オブジェクトのVertexBufferを生成
	// HRESULT IDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage,
	//												DWORD FVF, D3DPOOL Pool,
	//												IDirect3DVertexBuffer9** ppVertexBuffer,
	//												HANDLE* pHandle);
	// 引数
	//		Length			: 頂点バッファのサイズ
	//		Usage			: 頂点バッファの使われ方
	//		FVF				: 頂点データのFVFフォーマット
	//		Pool			: システムの頂点バッファ扱い方
	//		ppVertexBuffer	: 頂点バッファを格納する変数
	//		pHandle			: 常にNULL
	//-------------------------------------------------------------
	hr = pD3DDevice->CreateVertexBuffer(
										3 * sizeof(MY_VERTEX),
										D3DUSAGE_WRITEONLY,
										MY_VERTEX_FVF,
										D3DPOOL_MANAGED, &m_pVB, NULL
										);
    if( FAILED(hr))
	{
        return E_FAIL;
	}

    //VertexBufferの中身を埋める
    MY_VERTEX* v;
	// データ書き込み中にシステムがメモリを移動しないようにロックする
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // 各頂点の位置
    v[0].p = D3DXVECTOR3(-1.0f,  1.0f, 0.0f );
    v[1].p = D3DXVECTOR3( 1.0f, -1.0f, 0.0f );
    v[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f );
	// 各頂点の色
    v[0].color = D3DXCOLOR( 1.0f, 0.0f,  0.0f,  1.0f);
    v[1].color = D3DXCOLOR( 0.0f, 1.0f,  0.0f,  1.0f);
    v[2].color = D3DXCOLOR( 0.0f, 0.0f,  1.0f,  1.0f);
	// ロックを外す
    m_pVB->Unlock();

	//-------------------------------------------------------------
	// レンダリングステートパラメータの設定
	// HRESULT IDirect3DDevice9::SetRendererState(D3DRENDERSTATETYPE State, DWORD Value)
	// 引数
	//		State	: 変更対象のデバイス ステート変数
	//		Value	: デバイスのレンダリング ステートに設定する新しい値
	// 戻り値
	//		成功したらD3D_OK
	//-------------------------------------------------------------
	// 両面描画モードの指定
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Ｚ比較を行なう
    pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ノーライティングモード
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

//-------------------------------------------------------------
// 座標変換を行う
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
/*
void Triangle::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// デバイスのチェック
	if(pD3DDevice == 0)
	{
		return;	// デバイスへのポインタが無い
	}

	HRESULT hr = 0;

	// ビューイング行列の初期化
	//-------------------------------------------------------------
	// 何もしないマトリックス(単位行列)を生成
	// D3DXMATRIX *D3DXMatrixIdentity(D3DXMATRIX *pOut);
	// 引数
	//		pOut	: 単位行列を格納するD3DXMATRIX構造体へのポインタ
	// 戻り値
	//		単位行列である D3DXMATRIX 構造体へのポインタ
	//		この戻り値はpOutの戻り値と同じである
	//-------------------------------------------------------------
	D3DXMatrixIdentity( &m_view);
	// カメラの位置と方向を設定
	//-------------------------------------------------------------
	//D3DXMATRIX *D3DXMatrixLookAtLH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye,
	//								 const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp);
	// 引数
	//		pOut	: 演算結果のビューイング行列を格納するD3DXMATRIX構造体へのポインタ
	//		pEye	: カメラの位置
	//		pAt		: カメラの注視対象の位置
	//		pUp		: カメラの上の向き
	// 戻り値
	//		ビューイング行列であるD3DXMATRIX構造体へのポインタ
	//		この戻り値はpOutの戻り値と同じである
	//-------------------------------------------------------------
	D3DXMatrixLookAtLH( &m_view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f), 
								 &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
								 &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// カメラの設定
	//-------------------------------------------------------------
	// レンダリングパイプラインにマトリックスを設定
	// HRESULT IDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State,
	//										  const D3DMATRIX *pMatrix);
	// 引数
	//		State	: 変更対象のデバイスステートメント変数
	//		pMatrix	: 設定するマトリックス
	// 戻り値
	//		成功したらD3D_OK
	//-------------------------------------------------------------
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);


	D3DVIEWPORT9    vp;
	//-------------------------------------------------------------
	// ビューポートの取得
	// HRESULT IDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport);
	// 引数
	//		pViewport	: ビューポートの情報を格納する構造体へのポインタ
	// 戻り値
	//		成功したらD3D_OK
	//-------------------------------------------------------------
	hr = pD3DDevice->GetViewport(&vp);
	if(FAILED(hr))
	{
		return;	// 取得失敗
	}

	// アスペクト比の計算
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// 射影行列の初期化
	D3DXMatrixIdentity(&m_proj);
	// カメラのパラメータの設定
	//-------------------------------------------------------------
	//D3DXMATRIX *D3DXMatrixLookAtLH(D3DXMATRIX *pOut, float fovY, 
	//								 float Aspect,float zn, float zf);
	// 引数
	//		pOut	: 演算結果の射影行列を格納するD3DXMATRIX構造体へのポインタ
	//		fovY	: カメラの画角
	//		Aspect	: アスペクト比
	//		zn		: Nearプレーン
	//		zf		: Farプレーン
	// 戻り値
	//		射影行列であるD3DXMATRIX構造体へのポインタ
	//		この戻り値はpOutの戻り値と同じである
	//-------------------------------------------------------------
	D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 5.0f, 1000.0f);

	// 射影行列の設定
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);

}
*/

//-------------------------------------------------------------
// 三角形を描画
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void Triangle::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// デバイスのチェック
	if(pD3DDevice == 0)
	{
		return;	// デバイスへのポインタが無い
	}

	// 頂点バッファのチェック
	if(m_pVB == 0)
	{
		return;	// 頂点バッファへのポインタが無い
	}

	// 座標//変換（カメラの設定など）
	//Transform(pD3DDevice);

	// 座標変換（モデルの配置）
	// 行列の初期化
	D3DXMatrixIdentity(&m_world);
	// レンダリングパイプラインにマトリックスを設定
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);


	// 三角形の描画処理
	//-------------------------------------------------------------
	// 頂点バッファの設定
	// HRESULT IDirect3DDevice9::SetStreamSource(UINT StreamNumber,
	//											IDirect3DDevice9 *pStreamData,
	//											UINT OffsetInBytes, UINT Stride);
	// 引数
	//		StreamNumber	: 頂点ストリーム番号(0を指定)
	//		pStreamData		: 頂点バッファへのポインタ
	//		OffsetInBytes	: 何番目の頂点データから利用するか(0を指定)
	//		Stride			: 頂点データ構造体のサイズ
	// 戻り値
	//		成功したらD3D_OK
	//-------------------------------------------------------------
    pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));

	//-------------------------------------------------------------
	// 頂点バッファのフォーマットの設定
	// HRESULT IDirect3DDevice9::SetFVF(DWORD FVF);
	// 引数
	//		FVF	: 頂点バッファのフォーマット
	// 戻り値
	//		成功したらD3D_OK
	//-------------------------------------------------------------
    pD3DDevice->SetFVF(MY_VERTEX_FVF);

	//-------------------------------------------------------------
	// 頂点バッファの描画
	// HRESULT IDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,
	//										   UINT StartVertex, UINT PrimitiveCount);
	// 引数
	//		PrimitiveType	: プリミティブのタイプ
	//		StartVertex		: どの頂点から利用するか(0を指定)
	//		PrimitiveCount	: 描画する三角形の総数
	// 戻り値
	//		成功したらD3D_OK
	//
	// 第1引数のプリミティブのタイプには描画する図形によって得意・不得意が
	// あるので描画する図形に合わせて設定すること
	// 今回はトライアングルストリップという方式を用いている
	//-------------------------------------------------------------
    pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
}