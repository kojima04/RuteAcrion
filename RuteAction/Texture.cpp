//=============================================================
// Texture.cpp
// テクスチャ付き四角形描画クラスの実装
//=============================================================

//=============================================================
// include
//=============================================================
#include "Common.h"

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------
Texture::Texture(): m_pVB(0), m_pTexture(0), MY_VERTEX_FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)
{
}

Texture::Texture(std::string name,float X,float Y,float Z,float H,float W,Renderer * R,int T)
: m_pVB(0), m_pTexture(0), MY_VERTEX_FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)
{
	RenderType = T;
	Size.y = H;
	Size.x = W;
	Size.z = 0;
	Pos.x = X;
	Pos.y = Y;
	Pos.z = Z;
	alfa = 255;
	FileName = name;
	Rot.x = Rot.y = Rot.z = 0;
	Velo.x = Velo.y = Velo.z = 0;
	Accel.x = Accel.y = Accel.z = 0;
	Camera = R;
	Create();
	SetEdgeScale(0,-1,1);
	SetEdgeScale(1,1,1);
	SetEdgeScale(2,-1,-1);
	SetEdgeScale(3,1,-1);
}

void Texture::TextureInit(std::string name,float X,float Y,float Z,float H,float W,Renderer * R,int T)
{
	RenderType = T;
	Size.y = H;
	Size.x = W;
	Size.z = 0;
	Pos.x = X;
	Pos.y = Y;
	Pos.z = Z;
	alfa = 255;
	FileName = name;
	Rot.x = Rot.y = Rot.z = 0;
	Velo.x = Velo.y = Velo.z = 0;
	Accel.x = Accel.y = Accel.z = 0;
	Camera = R;
	Create();
	SetEdgeScale(0,-1,1);
	SetEdgeScale(1,1,1);
	SetEdgeScale(2,1,-1);
	SetEdgeScale(3,-1,-1);
	EdgeNumber = 4;
}


//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Texture::~Texture()
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
void Texture::SetAlfa(int A)
{
	alfa = A;
	SetMyVertex(0.0f,-1.0f);
}

void Texture::SetPos(float x, float y, float z)
{
	Pos.x = x;
	Pos.y = y;
	Pos.z = z;
}


void Texture::SetRot(float x, float y, float z)
{
	Rot.x = x;
	Rot.y = y;
	Rot.z = z;
}


void Texture::SetSize(float x, float y, float z)
{
	Size.x = x;
	Size.y = y;
	Size.z = z;
}

void Texture::SetVelo(float x, float y, float z)
{
	Velo.x = x;
	Velo.y = y;
	Velo.z = z;
}


void Texture::SetAccel(float x, float y, float z)
{
	Accel.x = x;
	Accel.y = y;
	Accel.z = z;
}

void Texture::Move()
{
	Pos.x += Velo.x;
	Pos.y += Velo.y;
	Pos.z += Velo.z;
}

void Texture::MoveCancel()
{
	Pos.x -= Velo.x;
	Pos.y -= Velo.y;
	Pos.z -= Velo.z;
}


void Texture::Acceleration()
{
	Velo.x += Accel.x/Mass;
	Velo.y += Accel.y/Mass;
	Velo.z += Accel.z/Mass;
}


void Texture::SetWorld(D3DXMATRIX *world)
{
	D3DXMatrixRotationYawPitchRoll(world, Rot.y, Rot.x, Rot.z);

	world->_11 *= Size.x;
	world->_12 *= Size.x;
	world->_13 *= Size.x;

	world->_21 *= Size.y;
	world->_22 *= Size.y;
	world->_23 *= Size.y;

	world->_31 *= Size.z;
	world->_32 *= Size.z;
	world->_33 *= Size.z;

	world->_41 = Pos.x;
	world->_42 = Pos.y;
	world->_43 = Pos.z;
}

void Texture::SetWorld_RHW(D3DXMATRIX *world)
{
	D3DXMATRIX	temp;
	D3DXVECTOR3	Move;

	D3DXMatrixMultiply(&temp, &Camera->m_view, &Camera->m_proj);

	D3DXMatrixMultiply(&temp, &temp, &Camera->m_screen);

	D3DXMatrixInverse(&temp, NULL, &temp);

	D3DXVec3TransformCoord(&Move, &Pos, &temp);

	world->_41 = Move.x;
	world->_42 = Move.y;
	world->_43 = Move.z;

	world->_11 = Size.x * Camera->m_view._11;
	world->_12 = Size.x * Camera->m_view._21;
	world->_13 = Size.x * Camera->m_view._31;

	world->_21 = Size.y * Camera->m_view._12;
	world->_22 = Size.y * Camera->m_view._22;
	world->_23 = Size.y * Camera->m_view._32;

	world->_31 = Size.z * Camera->m_view._13;
	world->_32 = Size.z * Camera->m_view._23;
	world->_33 = Size.z * Camera->m_view._33;

	world->_41 = Pos.x;
	world->_42 = Pos.y;
	world->_43 = Pos.z;
}


void Texture::SetWorld_Relative(D3DXMATRIX *world)
{

	world->_11 = Size.x * Camera->m_view._11;
	world->_12 = Size.x * Camera->m_view._21;
	world->_13 = Size.x * Camera->m_view._31;

	world->_21 = Size.y * Camera->m_view._12;
	world->_22 = Size.y * Camera->m_view._22;
	world->_23 = Size.y * Camera->m_view._32;

	world->_31 = Size.z * Camera->m_view._13;
	world->_32 = Size.z * Camera->m_view._23;
	world->_33 = Size.z * Camera->m_view._33;

	world->_41 = Pos.x;
	world->_42 = Pos.y;
	world->_43 = Pos.z;
}



//-------------------------------------------------------------
// テクスチャ付き四角形を生成
// 引数
//		G_Device : IDirect3DDevice9 インターフェイスへのポインタ
// 戻り値
//		成功したらS_OK
//-------------------------------------------------------------
HRESULT Texture::Create()
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
	if(::G_Device == 0)
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
	hr = ::G_Device->CreateVertexBuffer(
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
	//v[0].p = D3DXVECTOR3((-1.0f*Size.x)/2, ( 1.0f*Size.y)/2, 0.0f);	// 頂点①
 //   v[1].p = D3DXVECTOR3((1.0f*Size.x)/2,  (1.0f*Size.y)/2, 0.0f );	// 頂点②
 //   v[2].p = D3DXVECTOR3((-1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// 頂点③
 //   v[3].p = D3DXVECTOR3((1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// 頂点④

	v[0].p = D3DXVECTOR3(-1.0f/2, 1.0f/2, 0.0f);	// 頂点①
    v[1].p = D3DXVECTOR3(1.0f/2,  1.0f/2, 0.0f );	// 頂点②
    v[2].p = D3DXVECTOR3(-1.0f/2,-1.0f/2, 0.0f );	// 頂点③
    v[3].p = D3DXVECTOR3(1.0f/2, -1.0f/2, 0.0f );	// 頂点④
	// テクスチャ座標の設定
    v[0].t = D3DXVECTOR2(0.0f,0.0f);
    v[1].t = D3DXVECTOR2(1.0f,0.0f);
    v[2].t = D3DXVECTOR2(0.0f,1.0f);
    v[3].t = D3DXVECTOR2(1.0f,1.0f);
	// 各頂点の色(白)
	v[0].color = v[1].color = v[2].color = v[3].color = D3DCOLOR_RGBA(255,255,255,alfa);
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
	G_Device ->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	// テクスチャステージの一つ目の入力をそのまま出力する処理を行うと設定
	G_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

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
//	D3DXCreateTextureFromFile(G_Device, "Picture/tex2.dds", &m_pTexture);

	hr = D3DXCreateTextureFromFileEx(
        G_Device,
        FileName.c_str(),              // ファイル名
        0,
        0,
        0,
        0,
        D3DFMT_A8B8G8R8,                // 色抜きを可能に
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
        &m_pTexture                    // テクスチャ名
    );


	m_pTexture2[0] = &m_pTexture;



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
	G_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Ｚ比較を行なう
	G_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ディザリングを行なう（高品質描画）
	G_Device->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// ノーライティングモード
    G_Device->SetRenderState(D3DRS_LIGHTING, FALSE);


	return S_OK;
}



//-------------------------------------------------------------
// テクスチャ付き四角形を描画
// 引数
//		G_Device : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void Texture::Draw()
{
	// デバイスのチェック
	if(G_Device == 0)
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

	switch(RenderType)
	{

		case RENDERTYPE_ABSOLUTE:
			SetWorld(&m_world);
			break;
		case RENDERTYPE_RELATIVE:
			SetWorld_Relative(&m_world);
			break;
		case RENDERTYPE_RHW:
			SetWorld_RHW(&m_world);
			break;
	}

	//// 座標変換
	//D3DXMatrixScaling(&m_world,(FLOAT)1,(FLOAT)1,(FLOAT)1);
	//D3DXMatrixRotationYawPitchRoll(&m_world_R,(FLOAT)Rot.x,(FLOAT)Rot.y,(FLOAT)Rot.z);
	//D3DXMatrixTranslation(&m_world_M,(FLOAT)Pos.x,(FLOAT)Pos.y,(FLOAT)Pos.z);

	//D3DXMatrixMultiply(&m_world,&m_world,&m_world_R);
	//D3DXMatrixMultiply(&m_world,&m_world,&m_world_M);

	


	
   
	// レンダリングパイプラインにマトリックスを設定
	G_Device->SetTransform(D3DTS_WORLD, &m_world);

	//-------------------------------------------------------------
	// テクスチャをパイプラインにセット
	// HRESULT IDirect3DDevice9::SetTexture(DWORD Stage,
	//										IDirect3DBaseTexture9 *pTexture);
	// 引数
	//		Stage		: テクスチャを入力するテクスチャステージの番号
	//		pTexture	: テクスチャへのポインタ
	//-------------------------------------------------------------
	G_Device->SetTexture(0, *m_pTexture2[0]);

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
    G_Device->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));

	//-------------------------------------------------------------
	// 頂点バッファのフォーマットの設定
	// HRESULT IDirect3DDevice9::SetFVF(DWORD FVF);
	// 引数
	//		FVF	: 頂点バッファのフォーマット
	// 戻り値
	//		成功したらD3D_OK
	//-------------------------------------------------------------
    G_Device->SetFVF(MY_VERTEX_FVF);

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
    G_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	G_Device->SetTexture(0, 0);
}

void Texture::Update()
{
	MY_VERTEX* v;
	 m_pVB->Lock( 0, 0, (void**)&v, 0 );

	//D3DXMatrixRotationYawPitchRoll(&m_world,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100);
   
	// ロックを外す
    m_pVB->Unlock();

}


HRESULT Texture::SetpTexturefromFile(LPDIRECT3DTEXTURE9 *tex,std::string FileName)
{
	
	HRESULT hr = 0;


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

	hr = D3DXCreateTextureFromFileEx(
        G_Device,
        FileName.c_str(),              // ファイル名
        0,
        0,
        0,
        0,
        D3DFMT_A8B8G8R8,                // 色抜きを可能に
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
        tex                   // テクスチャ名
    );

	
    if( FAILED(hr))
	{
        return E_FAIL;
	}


	//tex = &temp;


	return S_OK;
}

void Texture::SetpTexture(LPDIRECT3DTEXTURE9 *tex)
{
	m_pTexture2[0] = tex;
}

void Texture::SetMyVertex(float rightedge,float leftedge)
{
	//VertexBufferの中身を埋める
    MY_VERTEX* v;
	// データ書き込み中にシステムがメモリを移動しないようにロックする
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // 各頂点の位置
	//v[0].p = D3DXVECTOR3((-1.0f*Size.x)/2, ( 1.0f*Size.y)/2, 0.0f);	// 頂点①
 //   v[1].p = D3DXVECTOR3((1.0f*Size.x)/2,  (1.0f*Size.y)/2, 0.0f );	// 頂点②
 //   v[2].p = D3DXVECTOR3((-1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// 頂点③
 //   v[3].p = D3DXVECTOR3((1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// 頂点④
	v[0].p = D3DXVECTOR3(-1.0f/2, 1.0f/2, 0.0f);	// 頂点①
    v[1].p = D3DXVECTOR3(1.0f/2,  1.0f/2, 0.0f );	// 頂点②
    v[2].p = D3DXVECTOR3(-1.0f/2,-1.0f/2, 0.0f );	// 頂点③
    v[3].p = D3DXVECTOR3(1.0f/2, -1.0f/2, 0.0f );	// 頂点④
	// テクスチャ座標の設定
    v[0].t = D3DXVECTOR2(leftedge,0.0f);
    v[1].t = D3DXVECTOR2(rightedge,0.0f);
    v[2].t = D3DXVECTOR2(leftedge,1.0f);
    v[3].t = D3DXVECTOR2(rightedge,1.0f);
	// 各頂点の色(白)
	v[0].color = v[1].color = v[2].color = v[3].color = D3DCOLOR_RGBA(255,255,255,alfa);
	// ロックを外す
    m_pVB->Unlock();
}

bool Texture::CheckTexHit(Texture *tex1,Texture *tex2)
{
	if(
		(tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[2].y + tex1->GetVelo()->y < tex2->GetPos()->y + tex2->GetSize()->y/2*tex2->EdgeScale[0].y + tex2->GetVelo()->y
	&&	 tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[2].y + tex1->GetVelo()->y > tex2->GetPos()->y + tex2->GetSize()->y/2*tex2->EdgeScale[2].y + tex2->GetVelo()->y)
	||
		(tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[0].y + tex1->GetVelo()->y < tex2->GetPos()->y + tex2->GetSize()->y/2*tex2->EdgeScale[0].y + tex2->GetVelo()->y
	&&	 tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[0].y + tex1->GetVelo()->y > tex2->GetPos()->y + tex2->GetSize()->y/2*tex2->EdgeScale[2].y + tex2->GetVelo()->y)
	||
		(tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[2].y + tex1->GetVelo()->y < tex2->GetPos()->y + tex2->GetSize()->y/2*tex2->EdgeScale[2].y + tex2->GetVelo()->y
	&&	 tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[0].y + tex1->GetVelo()->y > tex2->GetPos()->y + tex2->GetSize()->y/2*tex2->EdgeScale[0].y + tex2->GetVelo()->y)
	)
		if(
			(tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[1].x + tex1->GetVelo()->x > tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[0].x + tex2->GetVelo()->x
		&&	 tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[1].x < tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[0].x)
		||
			(tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[0].x + tex1->GetVelo()->x < tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[1].x + tex2->GetVelo()->x
		&&	 tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[0].x > tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[1].x)
		)
		{
			return 1;
		}


	if(
		(tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[0].x + tex1->GetVelo()->x < tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[1].x + tex2->GetVelo()->x
	&&   tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[0].x + tex1->GetVelo()->x > tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[0].x + tex2->GetVelo()->x)
	||
		(tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[1].x + tex1->GetVelo()->x < tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[1].x + tex2->GetVelo()->x
	&&   tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[1].x + tex1->GetVelo()->x > tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[0].x + tex2->GetVelo()->x)
	||
		(tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[0].x + tex1->GetVelo()->x < tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[0].x + tex2->GetVelo()->x
	&&   tex1->GetPos()->x + tex1->GetSize()->x/2*tex1->EdgeScale[1].x + tex1->GetVelo()->x > tex2->GetPos()->x + tex2->GetSize()->x/2*tex2->EdgeScale[1].x + tex2->GetVelo()->x)
	)
		if(
			(tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[0].y + tex1->GetVelo()->y > tex2->GetPos()->y + tex2->GetSize()->y/2*tex1->EdgeScale[2].y + tex2->GetVelo()->y
		&&	 tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[0].y < tex2->GetPos()->y + tex2->GetSize()->y/2*tex1->EdgeScale[2].y)
		||
			(tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[2].y + tex1->GetVelo()->y < tex2->GetPos()->y + tex2->GetSize()->y/2*tex1->EdgeScale[0].y + tex2->GetVelo()->y
		&&	 tex1->GetPos()->y + tex1->GetSize()->y/2*tex1->EdgeScale[2].y > tex2->GetPos()->y + tex2->GetSize()->y/2*tex1->EdgeScale[0].y)
		)
		{
			return 1;
		}
	return 0;
}


void Texture::SetEdgeScale(int Num,float x,float y)
{
	EdgeScale[Num].x = x; 
	EdgeScale[Num].y = y; 
}

void Texture::SetEdgeScale(D3DXVECTOR3 *epos)
{
	D3DXVECTOR3 temp;
	for(int i = 0;i < EdgeNumber;i++)
	{
		if(Rot.z != 0)
		{
  			temp.x = (float)(epos[i].x*cos(Rot.z)-epos[i].y*sin(Rot.z));
			temp.y = (float)(epos[i].x*sin(Rot.z)+epos[i].y*cos(Rot.z));
			int temp1 = temp.x*10000;
			if(temp1%10 > 4||temp1%10 < -4)
			{
				temp1 += (temp1 > 0)*10 -(temp1 < 0)*10 - temp1%10;
			}

			temp.x = (float)temp1 / 10000;

			temp1 = temp.y*10000;
			if(temp1%10 > 4||temp1%10 < -4)
			{
				temp1 += (temp1 > 0)*10 -(temp1 < 0)*10 - temp1%10;
			}
			temp.y = (float)temp1 / 10000;
			
			SetEdgeScale(i,temp.x,temp.y);
		}
		else
		SetEdgeScale(i,epos[i].x,epos[i].y);
	}
}