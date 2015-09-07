//=============================================================
// Texture.cpp
// テクスチャ付き四角形描画クラスの実装
//=============================================================

//=============================================================
// include
//=============================================================
#include "F_String.h"

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
F_String::F_String(): m_pVB(0), m_pTexture(0), MY_VERTEX_FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)
{
}

F_String::F_String(std::string name,float X,float Y,float Z,float H,float W)
: m_pVB(0), m_pTexture(0), MY_VERTEX_FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)
{
	
	width = W;
	height = H;
	x = X;
	y = Y;
	z = Z;
	FileName = name;
	Xangle = Yangle = Zangle = 0;
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
F_String::~F_String()
{
	// テクスチャの解放
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの解放
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

//-------------------------------------------------------------
// テクスチャ付き四角形を生成
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
// 戻り値
//		成功したらS_OK
//-------------------------------------------------------------
HRESULT F_String::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// テクスチャの初期化
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
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
										4 * sizeof(MY_VERTEX),
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
	v[0].p = D3DXVECTOR3((-1.0f*width)/2, ( 1.0f*height)/2, 0.0f);	// 頂点①
    v[1].p = D3DXVECTOR3((1.0f*width)/2,  (1.0f*height)/2, 0.0f );	// 頂点②
    v[2].p = D3DXVECTOR3((-1.0f*width)/2, (-1.0f*height)/2, 0.0f );	// 頂点③
    v[3].p = D3DXVECTOR3((1.0f*width)/2, (-1.0f*height)/2, 0.0f );	// 頂点④
	// テクスチャ座標の設定
    v[0].t = D3DXVECTOR2(0.0f,0.0f);
    v[1].t = D3DXVECTOR2(1.0f,0.0f);
    v[2].t = D3DXVECTOR2(0.0f,1.0f);
    v[3].t = D3DXVECTOR2(1.0f,1.0f);
	// 各頂点の色(白)
	v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// ロックを外す
    m_pVB->Unlock();

	//-------------------------------------------------------------
	// テクスチャステージの設定
	// HRESULT IDirect3DDevice9::SetTextureStageState(DWORD Stage,
	//											  	  D3DTEXTURESTAGESTATETYPE Type,
	//												  DWORD Value);
	// 引数
	//		Stage	: テクスチャステージの識別子(0～7)
	//		Type	: 設定するテクスチャステート
	//		Value	: 設定するステート値.この値の意味はTypeパラメータによって決まる.
	// 戻り値
	//		成功したらD3D_OK, 失敗したらD3DERR_INVALIDCALLを返す
	//-------------------------------------------------------------
	// テクスチャステージの一つ目の入力にテクスチャを入力と設定
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	// テクスチャステージの一つ目の入力をそのまま出力する処理を行うと設定
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//-------------------------------------------------------------
	// テクスチャ画像をファイルから読み込む
	// HRESULT D3DXCreateTextureFromFile(LPDIRECT3DDEVICE9 pDevice,
	//									 LPCSTR pSrcFile,
	//									 LPDIRECT3DTEXTURE9 *ppTexture);
	// 引数
	//		pDevice		: IDirect3DDevice9 インターフェイスへのポインタ
	//		pSrcFile	: 画像のファイル名
	//		ppTexture	: テクスチャへのポインタへのポインタ
	// 戻り値
	//		成功したらD3D_OK
	//-------------------------------------------------------------
//	D3DXCreateTextureFromFile(pD3DDevice, "Picture/tex2.dds", &m_pTexture);

	hr = D3DXCreateTextureFromFileEx(
        pD3DDevice,
        FileName.c_str(),              // ファイル名
        0,
        0,
        0,
        0,
        D3DFMT_A1R5G5B5,                // 色抜きを可能に
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
        &m_pTexture                    // テクスチャ名
    );



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
	// ディザリングを行なう（高品質描画）
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// ノーライティングモード
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	return S_OK;
}



//-------------------------------------------------------------
// テクスチャ付き四角形を描画
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void F_String::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
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

	// 行列の初期化
	D3DXMatrixIdentity(&m_world);

	// 座標変換
	D3DXMatrixScaling(&m_world,(FLOAT)1,(FLOAT)1,(FLOAT)1);
	
	


	
   
	// レンダリングパイプラインにマトリックスを設定
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);

	//-------------------------------------------------------------
	// テクスチャをパイプラインにセット
	// HRESULT IDirect3DDevice9::SetTexture(DWORD Stage,
	//										IDirect3DBaseTexture9 *pTexture);
	// 引数
	//		Stage		: テクスチャを入力するテクスチャステージの番号
	//		pTexture	: テクスチャへのポインタ
	//-------------------------------------------------------------
	pD3DDevice->SetTexture(0, m_pTexture);

	// テクスチャ付き四角形の描画処理
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
    pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pD3DDevice->SetTexture(0, 0);
}

void F_String::Update()
{
	MY_VERTEX* v;
	 m_pVB->Lock( 0, 0, (void**)&v, 0 );

	D3DXMatrixRotationYawPitchRoll(&m_world,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100);
   
	// ロックを外す
    m_pVB->Unlock();

}