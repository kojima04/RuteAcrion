//=============================================================
// Renderer.cpp
// DirectGraphics�̏������y�ъǗ��̎���
//=============================================================

//=============================================================
// include
//=============================================================
#include "Common.h"



//=============================================================
// ����
//=============================================================
//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
Renderer::Renderer()
: m_pD3D(0), m_pD3DDevice(0)
{
 SetCPos(0,0,-720);
 SetCFocus(0,0,0);
 C_Xangle = C_Yangle = C_Zangle = 0;
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
Renderer::~Renderer()
{
	// �f�o�C�X�I�u�W�F�N�g�̉��
	if(m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// DirectGraphics�̉��
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
// DirectGraphics�̏�����
// ����
//		hWnd			: �E�B���h�E�n���h��
//		isFullScreen	: �t���X�N���[���̏ꍇtrue
//		clientWidth		: �N���C�A���g�̈�̕�
//		clientHeight	: �N���C�A���g�̈�̍���
// �߂�l
//		����������S_OK
//-------------------------------------------------------------
HRESULT Renderer::Initialize(HWND hWnd, BOOL isFullScreen, int clientWidth, int clientHeight)
{
	HRESULT hr = 0;



	//-------------------------------------------------------------
	// Direct3D�I�u�W�F�N�g�̍쐬
	// IDrect3D9 *Direct3DCreate9(UINT SDKVersion);
	// ����
	//		SDKVersion	: D3D_SDK_VERSION���w�肷��
	// �߂�l
	//		����������Direct3D9�I�u�W�F�N�g�ւ̃|�C���^,���s�����
	//		NULL��Ԃ�
	//-------------------------------------------------------------
	m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == 0)
	{
		return E_FAIL;	// �擾���s
	}



	// �f�B�X�v���C���[�h�̏����i�[
	D3DDISPLAYMODE d3ddm;
	//-------------------------------------------------------------
	// ���݂̃f�B�X�v���C���[�h���擾
	// HRESULT IDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode);
	// ����
	//		Adapter	: �₢���킹��̃f�B�X�v���C�A�_�v�^�������������w��
	//				  D3DADAPTER_DEFAULT�̓v���C�}���f�B�X�v���C�A�_�v�^
	//		pMode	: �A�_�v�^�̌��݂̃��[�h���L�q�����񂪊i�[�����\
	//				  ���̂ւ̃|�C���^
	// �߂�l
	//		����������D3D_OK, ���s������D3DERR_INVALIDCALL��Ԃ�
	//-------------------------------------------------------------
	hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if(FAILED(hr))
	{
		return E_FAIL;	// �擾���s
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�p�\���̂̏�����
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��ݒ�
	m_D3DPP.BackBufferCount = 1;
	
	if(isFullScreen)	// �t���X�N���[���̏ꍇ
	{
		m_D3DPP.Windowed			= FALSE;			// �t���X�N���[���\���̎w��
		m_D3DPP.BackBufferWidth		= clientWidth;		// �t���X�N���[�����̉���
		m_D3DPP.BackBufferHeight	= clientHeight;		// �t���X�N���[�����̏c��
	}
	else				// �E�B���h�E���[�h�̏ꍇ
	{
		m_D3DPP.Windowed			= TRUE;				// �E�B���h�E���\���̎w��
	}
	m_D3DPP.BackBufferFormat		= d3ddm.Format;				// �J���[���[�h�̎w��
	m_D3DPP.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �摜����ʂɓ]������œK�ȕ��@�������I�ɑI��
	m_D3DPP.EnableAutoDepthStencil	= TRUE;						// �G���[�΍�
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;				// �G���[�΍�



	//-------------------------------------------------------------
	// �f�o�C�X�I�u�W�F�N�g�̐���
	// HRESULT IDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType,
	//									HWND hFocusWindow, DWORD BehaviorFlags,
	//									D3DPRESENT_PARAMETERS *pPresentationParameters,
	//									IDirect3DDevice9** ppReturnedDeviceInterface);
	// ����
	//		Adapter						: �r�f�I�K�[�h�̎w��(D3DADAPTER_DEFAULT���w��)
	//		DeviceType					: �f�o�C�X�̃^�C�v
	//		hFocusWindow				: �E�B���h�E�n���h��
	//		BehaviorFlags				: �f�o�C�X�̓��샂�[�h
	//		pPresentationParameters		: D3DPRESENT_PARAMETERS�\���̂̃A�h���X
	//		ppReturnedDeviceInterface	: "�擾�����f�o�C�X�ւ̃|�C���^"�ւ̃|�C���^
	// �߂�l
	//		����������D3D_OK��Ԃ�
	//-------------------------------------------------------------
	//=============================================================
	// �`��ƒ��_�������n�[�h�E�F�A�ōs��
	//
	// �r�f�I�J�[�h�����_�����n�[�h�E�F�A�𓋍ڂ��Ă���ΐ�������.
	// ������,�m�[�gPC�Ȃǂ̓n�[�h�E�F�A�����_�����ɑΉ����Ă��Ȃ�
	// �ꍇ������,���̏ꍇ���s����
	//=============================================================
	hr = m_pD3D->CreateDevice(
								D3DADAPTER_DEFAULT,
								D3DDEVTYPE_HAL,			// �n�[�h�E�F�A�A�N�Z�����[�V�����@�\���g��
								hWnd,
								D3DCREATE_HARDWARE_VERTEXPROCESSING,	// ���_�������n�[�h�E�F�A�ōs��
								&m_D3DPP,
								&m_pD3DDevice
								);
	if(FAILED(hr))
	{
		// �`��ƒ��_�������n�[�h�E�F�A�ōs���ݒ�Ɏ��s�����ꍇ
		//=============================================================
		// �`����n�[�h�E�F�A�ōs��,���_������CPU�ōs��
		//
		// �r�f�I�K�[�h�����_�����ɑΉ����Ă��Ȃ��ꍇ.�����̊��Ő���
		// ����.
		//=============================================================
		hr = m_pD3D->CreateDevice(
									D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,			// �n�[�h�E�F�A�A�N�Z�����[�V�����@�\���g��
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,	// ���_�������\�t�g�E�F�A�ōs��
									&m_D3DPP,
									&m_pD3DDevice
									);
		if(FAILED(hr))
		{
			// �`����n�[�h�E�F�A,���_������CPU�ōs���ݒ�Ɏ��s�����ꍇ
			//=============================================================
			// �`��ƒ��_������CPU�ōs��
			//
			// ���̐ݒ�ł�����͂��邪,���ɓ��삪�x���Ȃ邽�ߎ����Q�[��
			// �����邱�Ƃ͕s�\�ł���.�����,�ȉ��̏����͔�΂��Ă��܂�
			// ,���������s�Ƃ��Ă��܂��Ă����܂�Ȃ�.
			//=============================================================
			hr = m_pD3D->CreateDevice(
										D3DADAPTER_DEFAULT,
										D3DDEVTYPE_REF,			// �n�[�h�E�F�A�A�N�Z�����[�V�����@�\���g��Ȃ�
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,	// ���_�������\�t�g�E�F�A�ōs��
										&m_D3DPP,
										&m_pD3DDevice
										);
			if(FAILED(hr))
			{
				return E_FAIL;	// ���������s
			}
		}
	}

	D3DXMatrixIdentity(&m_screen);
	
	m_screen._11 = (float)clientWidth/2;
	m_screen._22 = -(float)clientHeight/2;
	m_screen._41 = (float)clientWidth/2;
	m_screen._42 = (float)clientHeight/2;

	// ����������S_OK��Ԃ�
	return S_OK;
}

//-------------------------------------------------------------
// ���W�ϊ����s��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void Renderer::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// �f�o�C�X�̃`�F�b�N
	if(pD3DDevice == 0)
	{
		return;	// �f�o�C�X�ւ̃|�C���^������
	}

	HRESULT hr = 0;

	// �r���[�C���O�s��̏�����
	//-------------------------------------------------------------
	// �������Ȃ��}�g���b�N�X(�P�ʍs��)�𐶐�
	// D3DXMATRIX *D3DXMatrixIdentity(D3DXMATRIX *pOut);
	// ����
	//		pOut	: �P�ʍs����i�[����D3DXMATRIX�\���̂ւ̃|�C���^
	// �߂�l
	//		�P�ʍs��ł��� D3DXMATRIX �\���̂ւ̃|�C���^
	//		���̖߂�l��pOut�̖߂�l�Ɠ����ł���
	//-------------------------------------------------------------
	D3DXMatrixIdentity( &m_view);
	// �J�����̈ʒu�ƕ�����ݒ�
	//-------------------------------------------------------------
	//D3DXMATRIX *D3DXMatrixLookAtLH(D3DXMATRIX *pOut, const D3DXVECTOR3 *pEye,
	//								 const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp);
	// ����
	//		pOut	: ���Z���ʂ̃r���[�C���O�s����i�[����D3DXMATRIX�\���̂ւ̃|�C���^
	//		pEye	: �J�����̈ʒu
	//		pAt		: �J�����̒����Ώۂ̈ʒu
	//		pUp		: �J�����̏�̌���
	// �߂�l
	//		�r���[�C���O�s��ł���D3DXMATRIX�\���̂ւ̃|�C���^
	//		���̖߂�l��pOut�̖߂�l�Ɠ����ł���
	//-------------------------------------------------------------
	D3DXMatrixLookAtLH( &m_view, &D3DXVECTOR3((float)CPos.x, (float)CPos.y, (float)CPos.z), 
								 &D3DXVECTOR3((float)CFocus.x, (float)CFocus.y, (float)CFocus.z), 
								 &D3DXVECTOR3(0.0f, 1.0f, 0.0f));


	/*D3DXMatrixRotationYawPitchRoll(&m_view,(FLOAT)C_Xangle,(FLOAT)C_Yangle,(FLOAT)C_Zangle);
	D3DXMatrixTranslation(&m_view_M,(FLOAT)C_x,(FLOAT)C_y,(FLOAT)C_z);
	D3DXMatrixMultiply(&m_view,&m_view,&m_view_M);*/


	// �J�����̐ݒ�
	//-------------------------------------------------------------
	// �����_�����O�p�C�v���C���Ƀ}�g���b�N�X��ݒ�
	// HRESULT IDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State,
	//										  const D3DMATRIX *pMatrix);
	// ����
	//		State	: �ύX�Ώۂ̃f�o�C�X�X�e�[�g�����g�ϐ�
	//		pMatrix	: �ݒ肷��}�g���b�N�X
	// �߂�l
	//		����������D3D_OK
	//-------------------------------------------------------------
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);


	D3DVIEWPORT9    vp;
	//-------------------------------------------------------------
	// �r���[�|�[�g�̎擾
	// HRESULT IDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport);
	// ����
	//		pViewport	: �r���[�|�[�g�̏����i�[����\���̂ւ̃|�C���^
	// �߂�l
	//		����������D3D_OK
	//-------------------------------------------------------------
	hr = pD3DDevice->GetViewport(&vp);
	if(FAILED(hr))
	{
		return;	// �擾���s
	}

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// �ˉe�s��̏�����
	D3DXMatrixIdentity(&m_proj);
	// �J�����̃p�����[�^�̐ݒ�
	//-------------------------------------------------------------
	//D3DXMATRIX *D3DXMatrixLookAtLH(D3DXMATRIX *pOut, float fovY, 
	//								 float Aspect,float zn, float zf);
	// ����
	//		pOut	: ���Z���ʂ̎ˉe�s����i�[����D3DXMATRIX�\���̂ւ̃|�C���^
	//		fovY	: �J�����̉�p
	//		Aspect	: �A�X�y�N�g��
	//		zn		: Near�v���[��
	//		zf		: Far�v���[��
	// �߂�l
	//		�ˉe�s��ł���D3DXMATRIX�\���̂ւ̃|�C���^
	//		���̖߂�l��pOut�̖߂�l�Ɠ����ł���
	//-------------------------------------------------------------
	D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 5.0f, 10000.0f);

	// �ˉe�s��̐ݒ�
	// �����_�����O�p�C�v���C���Ƀ}�g���b�N�X��ݒ�
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);

}

//-------------------------------------------------------------
// �`�悷��
// ����
//		pObject	: �`�悷��I�u�W�F�N�g
//-------------------------------------------------------------
void Renderer::RenderBGN()
{

	HRESULT hr = 0;
	//-------------------------------------------------------------
	// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
	// HRESULT LPDIRECT3DDEVICE9::Clear(DWORD Count, const D3DRECT *pRects,
	//									DWORD Flags, D3DCOLOR Color,
	//									float Z, DWORD Stencil);
	// ����
	//		Count	: pRects�z��ɂ����`�̐�.pRects��NULL�Ȃ�0���w��
	//		pRects	: �N���A�����`���w��.NULL���w�肷��ƃr���[�|�[��`
	//				  �S�̂��N���A����
	//		Flags	: �N���A����T�[�t�F�C�X���w�肷��t���O
	//		Color	: �N���A����F�i�f�o�b�O���͐��w�肷��Ɨǂ��j
	//		Z		: �N���A����Z�o�b�t�@�̒l
	//		Stencil	: �N���A����X�e���V���o�b�t�@�̒l
	//-------------------------------------------------------------
	hr = m_pD3DDevice->Clear(
								0,
								NULL,						// �S�̂��N���A
								D3DCLEAR_TARGET |			// �o�b�N�o�b�t�@���w��
								D3DCLEAR_ZBUFFER,			// Z�o�b�t�@���w��
								D3DCOLOR_XRGB(0, 0, 255),	// �N���A����F�͐�
								1.0f,						// �N���A����Z�o�b�t�@�̒l
								0							// �N���A����X�e���V���o�b�t�@�̒l
								);
	if(FAILED(hr))
	{
		return;	// �o�b�N�o�b�t�@�̃N���A�Ɏ��s
	}

	// �`��J�n�錾
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

	/* �����Ń��f���Ȃǂ̕`�� */	
	pObject->Draw();
		

}

void Renderer::RenderEND()
{
		// �`��I���錾
		m_pD3DDevice->EndScene();


	//-------------------------------------------------------------
	// �o�b�N�o�b�t�@�̓��e���f�B�X�v���C�ɕ`��
	// HRESULT LPDIRECT3DDEVICE9::Present(const RECT *pSourceRect,
	//									  const RECT *pDestRect,
	//									  HWND hDestWindowOverride,
	//									  const RGNDATA *pDirtyRegion);
	// ����
	//		pSourceRect			: �]�����̋�`�i0�Ȃ�S�́j
	//		pDestRect			: �]����̋�`�i0�Ȃ�S�́j
	//		hDestWindowOverride	: �v���[���e�[�V�����̑ΏۂƂ��Ĉ�����]����E�B���h�E�ւ̃|�C���^
	//		pDirtyRegion		: NULL���w��
	//-------------------------------------------------------------
	HRESULT hr = m_pD3DDevice->Present(0, 0, 0, 0);
	if(FAILED(hr))
	{
		//-------------------------------------------------------------
		// �f�o�C�X�������畜�A
		// HRESULT LPDIRECT3DDEVICE9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters);
		// ����
		//		pPresentationParameters	: D3DPRESENT_PARAMETERS�\���̂̃A�h���X
		// �߂�l
		//		����������D3D_OK
		//-------------------------------------------------------------
		m_pD3DDevice->Reset(&m_D3DPP);
	}
}

void Renderer::InitRender(void)
{
    //RGB�J���[�v�f�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//�A���t�@�v�f�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	
	 // �A���t�@�E�u�����f�B���O���s��
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//�������������s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    // ���ߏ������s��
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	
	//�e�N�X�`���t�B���^
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

	