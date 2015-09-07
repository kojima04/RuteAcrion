//=============================================================
// Renderer.cpp
// DirectGraphicsの初期化及び管理の実装
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
Renderer::Renderer()
: m_pD3D(0), m_pD3DDevice(0)
{
 SetCPos(0,0,-720);
 SetCFocus(0,0,0);
 C_Xangle = C_Yangle = C_Zangle = 0;
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Renderer::~Renderer()
{
	// デバイスオブジェクトの解放
	if(m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// DirectGraphicsの解放
	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

void Renderer::SetCPos(float x, float y, float z)
{
	CPos.x = x;
	CPos.y = y;
	CPos.z = z;
}

void Renderer::SetCFocus(float x, float y, float z)
{
	CFocus.x = x;
	CFocus.y = y;
	CFocus.z = z;
}


//-------------------------------------------------------------
// DirectGraphicsの初期化
// 引数
//		hWnd			: ウィンドウハンドル
//		isFullScreen	: フルスクリーンの場合true
//		clientWidth		: クライアント領域の幅
//		clientHeight	: クライアント領域の高さ
// 戻り値
//		成功したらS_OK
//-------------------------------------------------------------
HRESULT Renderer::Initialize(HWND hWnd, BOOL isFullScreen, int clientWidth, int clientHeight)
{
	HRESULT hr = 0;



	//-------------------------------------------------------------
	// Direct3Dオブジェクトの作成
	// IDrect3D9 *Direct3DCreate9(UINT SDKVersion);
	// 引数
	//		SDKVersion	: D3D_SDK_VERSIONを指定する
	// 戻り値
	//		成功したらDirect3D9オブジェクトへのポインタ,失敗すると
	//		NULLを返す
	//-------------------------------------------------------------
	m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == 0)
	{
		return E_FAIL;	// 取得失敗
	}



	// ディスプレイモードの情報を格納
	D3DDISPLAYMODE d3ddm;
	//-------------------------------------------------------------
	// 現在のディスプレイモードを取得
	// HRESULT IDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode);
	// 引数
	//		Adapter	: 問い合わせ先のディスプレイアダプタを示す序数を指定
	//				  D3DADAPTER_DEFAULTはプライマリディスプレイアダプタ
	//		pMode	: アダプタの現在のモードを記述する情報が格納される構
	//				  造体へのポインタ
	// 戻り値
	//		成功したらD3D_OK, 失敗したらD3DERR_INVALIDCALLを返す
	//-------------------------------------------------------------
	hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if(FAILED(hr))
	{
		return E_FAIL;	// 取得失敗
	}

	// デバイスのプレゼンテーションパラメータ用構造体の初期化
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));

	// デバイスのプレゼンテーションパラメータを設定
	m_D3DPP.BackBufferCount = 1;
	
	if(isFullScreen)	// フルスクリーンの場合
	{
		m_D3DPP.Windowed			= FALSE;			// フルスクリーン表示の指定
		m_D3DPP.BackBufferWidth		= clientWidth;		// フルスクリーン時の横幅
		m_D3DPP.BackBufferHeight	= clientHeight;		// フルスクリーン時の縦幅
	}
	else				// ウィンドウモードの場合
	{
		m_D3DPP.Windowed			= TRUE;				// ウィンドウ内表示の指定
	}
	m_D3DPP.BackBufferFormat		= d3ddm.Format;				// カラーモードの指定
	m_D3DPP.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 画像を画面に転送する最適な方法を自動的に選択
	m_D3DPP.EnableAutoDepthStencil	= TRUE;						// エラー対策
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;				// エラー対策



	//-------------------------------------------------------------
	// デバイスオブジェクトの生成
	// HRESULT IDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType,
	//									HWND hFocusWindow, DWORD BehaviorFlags,
	//									D3DPRESENT_PARAMETERS *pPresentationParameters,
	//									IDirect3DDevice9** ppReturnedDeviceInterface);
	// 引数
	//		Adapter						: ビデオガードの指定(D3DADAPTER_DEFAULTを指定)
	//		DeviceType					: デバイスのタイプ
	//		hFocusWindow				: ウィンドウハンドル
	//		BehaviorFlags				: デバイスの動作モード
	//		pPresentationParameters		: D3DPRESENT_PARAMETERS構造体のアドレス
	//		ppReturnedDeviceInterface	: "取得したデバイスへのポインタ"へのポインタ
	// 戻り値
	//		成功したらD3D_OKを返す
	//-------------------------------------------------------------
	//=============================================================
	// 描画と頂点処理をハードウェアで行う
	//
	// ビデオカードが頂点処理ハードウェアを搭載していれば成功する.
	// しかし,ノートPCなどはハードウェアが頂点処理に対応していない
	// 場合が多く,その場合失敗する
	//=============================================================
	hr = m_pD3D->CreateDevice(
								D3DADAPTER_DEFAULT,
								D3DDEVTYPE_HAL,			// ハードウェアアクセラレーション機能を使う
								hWnd,
								D3DCREATE_HARDWARE_VERTEXPROCESSING,	// 頂点処理をハードウェアで行う
								&m_D3DPP,
								&m_pD3DDevice
								);
	if(FAILED(hr))
	{
		// 描画と頂点処理をハードウェアで行う設定に失敗した場合
		//=============================================================
		// 描画をハードウェアで行い,頂点処理はCPUで行う
		//
		// ビデオガードが頂点処理に対応していない場合.多くの環境で成功
		// する.
		//=============================================================
		hr = m_pD3D->CreateDevice(
									D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,			// ハードウェアアクセラレーション機能を使う
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,	// 頂点処理をソフトウェアで行う
									&m_D3DPP,
									&m_pD3DDevice
									);
		if(FAILED(hr))
		{
			// 描画をハードウェア,頂点処理をCPUで行う設定に失敗した場合
			//=============================================================
			// 描画と頂点処理をCPUで行う
			//
			// この設定でも動作はするが,非常に動作が遅くなるため実質ゲーム
			// をすることは不可能である.よって,以下の処理は飛ばしてしまい
			// ,初期化失敗としてしまってもかまわない.
			//=============================================================
			hr = m_pD3D->CreateDevice(
										D3DADAPTER_DEFAULT,
										D3DDEVTYPE_REF,			// ハードウェアアクセラレーション機能を使わない
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,	// 頂点処理をソフトウェアで行う
										&m_D3DPP,
										&m_pD3DDevice
										);
			if(FAILED(hr))
			{
				return E_FAIL;	// 初期化失敗
			}
		}
	}

	D3DXMatrixIdentity(&m_screen);
	
	m_screen._11 = (float)clientWidth/2;
	m_screen._22 = -(float)clientHeight/2;
	m_screen._41 = (float)clientWidth/2;
	m_screen._42 = (float)clientHeight/2;

	// 成功したらS_OKを返す
	return S_OK;
}

//-------------------------------------------------------------
// 座標変換を行う
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void Renderer::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
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
	D3DXMatrixLookAtLH( &m_view, &D3DXVECTOR3((float)CPos.x, (float)CPos.y, (float)CPos.z), 
								 &D3DXVECTOR3((float)CFocus.x, (float)CFocus.y, (float)CFocus.z), 
								 &D3DXVECTOR3(0.0f, 1.0f, 0.0f));


	/*D3DXMatrixRotationYawPitchRoll(&m_view,(FLOAT)C_Xangle,(FLOAT)C_Yangle,(FLOAT)C_Zangle);
	D3DXMatrixTranslation(&m_view_M,(FLOAT)C_x,(FLOAT)C_y,(FLOAT)C_z);
	D3DXMatrixMultiply(&m_view,&m_view,&m_view_M);*/


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
	D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 5.0f, 10000.0f);

	// 射影行列の設定
	// レンダリングパイプラインにマトリックスを設定
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);

}

//-------------------------------------------------------------
// 描画する
// 引数
//		pObject	: 描画するオブジェクト
//-------------------------------------------------------------
void Renderer::RenderBGN()
{

	HRESULT hr = 0;
	//-------------------------------------------------------------
	// ビューポートと深度バッファのクリアとステンシルバッファの削除
	// HRESULT LPDIRECT3DDEVICE9::Clear(DWORD Count, const D3DRECT *pRects,
	//									DWORD Flags, D3DCOLOR Color,
	//									float Z, DWORD Stencil);
	// 引数
	//		Count	: pRects配列にある矩形の数.pRectsがNULLなら0を指定
	//		pRects	: クリアする矩形を指定.NULLを指定するとビューポー矩形
	//				  全体をクリアする
	//		Flags	: クリアするサーフェイスを指定するフラグ
	//		Color	: クリアする色（デバッグ時は青を指定すると良い）
	//		Z		: クリアするZバッファの値
	//		Stencil	: クリアするステンシルバッファの値
	//-------------------------------------------------------------
	hr = m_pD3DDevice->Clear(
								0,
								NULL,						// 全体をクリア
								D3DCLEAR_TARGET |			// バックバッファを指定
								D3DCLEAR_ZBUFFER,			// Zバッファを指定
								D3DCOLOR_XRGB(0, 0, 255),	// クリアする色は青
								1.0f,						// クリアするZバッファの値
								0							// クリアするステンシルバッファの値
								);
	if(FAILED(hr))
	{
		return;	// バックバッファのクリアに失敗
	}

	// 描画開始宣言
	hr = m_pD3DDevice->BeginScene();
	Transform(m_pD3DDevice);

	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000001);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	/*m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,true);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);*/
}

void Renderer::Render(DrawObject *pObject)
{

	if(pObject == 0)
	{
		return;
	}

	/* ここでモデルなどの描画 */	
	pObject->Draw();
		

}

void Renderer::RenderEND()
{
		// 描画終了宣言
		m_pD3DDevice->EndScene();


	//-------------------------------------------------------------
	// バックバッファの内容をディスプレイに描画
	// HRESULT LPDIRECT3DDEVICE9::Present(const RECT *pSourceRect,
	//									  const RECT *pDestRect,
	//									  HWND hDestWindowOverride,
	//									  const RGNDATA *pDirtyRegion);
	// 引数
	//		pSourceRect			: 転送元の矩形（0なら全体）
	//		pDestRect			: 転送先の矩形（0なら全体）
	//		hDestWindowOverride	: プレゼンテーションの対象として扱われる転送先ウィンドウへのポインタ
	//		pDirtyRegion		: NULLを指定
	//-------------------------------------------------------------
	HRESULT hr = m_pD3DDevice->Present(0, 0, 0, 0);
	if(FAILED(hr))
	{
		//-------------------------------------------------------------
		// デバイス消失から復帰
		// HRESULT LPDIRECT3DDEVICE9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters);
		// 引数
		//		pPresentationParameters	: D3DPRESENT_PARAMETERS構造体のアドレス
		// 戻り値
		//		成功したらD3D_OK
		//-------------------------------------------------------------
		m_pD3DDevice->Reset(&m_D3DPP);
	}
}

void Renderer::InitRender(void)
{
    //RGBカラー要素の設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//アルファ要素の設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	
	 // アルファ・ブレンディングを行う
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//半透明処理を行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    // 透過処理を行う
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	
	//テクスチャフィルタ
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

	