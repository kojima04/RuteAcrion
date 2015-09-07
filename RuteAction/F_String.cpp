//=============================================================
// Texture.cpp
// �e�N�X�`���t���l�p�`�`��N���X�̎���
//=============================================================

//=============================================================
// include
//=============================================================
#include "F_String.h"

//=============================================================
// ����
//=============================================================
//-------------------------------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//-------------------------------------------------------------
F_String::~F_String()
{
	// �e�N�X�`���̉��
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// ���_�o�b�t�@�̉��
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

//-------------------------------------------------------------
// �e�N�X�`���t���l�p�`�𐶐�
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
// �߂�l
//		����������S_OK
//-------------------------------------------------------------
HRESULT F_String::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// �e�N�X�`���̏�����
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	// ���_�o�b�t�@�̏�����
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	// �f�o�C�X�̃`�F�b�N
	if(pD3DDevice == 0)
	{
		return E_FAIL;	// �f�o�C�X�ւ̃|�C���^������
	}

	HRESULT hr = 0;

	//-------------------------------------------------------------
	// �I�u�W�F�N�g��VertexBuffer�𐶐�
	// HRESULT IDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage,
	//												DWORD FVF, D3DPOOL Pool,
	//												IDirect3DVertexBuffer9** ppVertexBuffer,
	//												HANDLE* pHandle);
	// ����
	//		Length			: ���_�o�b�t�@�̃T�C�Y
	//		Usage			: ���_�o�b�t�@�̎g����
	//		FVF				: ���_�f�[�^��FVF�t�H�[�}�b�g
	//		Pool			: �V�X�e���̒��_�o�b�t�@������
	//		ppVertexBuffer	: ���_�o�b�t�@���i�[����ϐ�
	//		pHandle			: ���NULL
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

    //VertexBuffer�̒��g�𖄂߂�
    MY_VERTEX* v;
	// �f�[�^�������ݒ��ɃV�X�e�������������ړ����Ȃ��悤�Ƀ��b�N����
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // �e���_�̈ʒu
	v[0].p = D3DXVECTOR3((-1.0f*width)/2, ( 1.0f*height)/2, 0.0f);	// ���_�@
    v[1].p = D3DXVECTOR3((1.0f*width)/2,  (1.0f*height)/2, 0.0f );	// ���_�A
    v[2].p = D3DXVECTOR3((-1.0f*width)/2, (-1.0f*height)/2, 0.0f );	// ���_�B
    v[3].p = D3DXVECTOR3((1.0f*width)/2, (-1.0f*height)/2, 0.0f );	// ���_�C
	// �e�N�X�`�����W�̐ݒ�
    v[0].t = D3DXVECTOR2(0.0f,0.0f);
    v[1].t = D3DXVECTOR2(1.0f,0.0f);
    v[2].t = D3DXVECTOR2(0.0f,1.0f);
    v[3].t = D3DXVECTOR2(1.0f,1.0f);
	// �e���_�̐F(��)
	v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// ���b�N���O��
    m_pVB->Unlock();

	//-------------------------------------------------------------
	// �e�N�X�`���X�e�[�W�̐ݒ�
	// HRESULT IDirect3DDevice9::SetTextureStageState(DWORD Stage,
	//											  	  D3DTEXTURESTAGESTATETYPE Type,
	//												  DWORD Value);
	// ����
	//		Stage	: �e�N�X�`���X�e�[�W�̎��ʎq(0�`7)
	//		Type	: �ݒ肷��e�N�X�`���X�e�[�g
	//		Value	: �ݒ肷��X�e�[�g�l.���̒l�̈Ӗ���Type�p�����[�^�ɂ���Č��܂�.
	// �߂�l
	//		����������D3D_OK, ���s������D3DERR_INVALIDCALL��Ԃ�
	//-------------------------------------------------------------
	// �e�N�X�`���X�e�[�W�̈�ڂ̓��͂Ƀe�N�X�`������͂Ɛݒ�
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	// �e�N�X�`���X�e�[�W�̈�ڂ̓��͂����̂܂܏o�͂��鏈�����s���Ɛݒ�
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//-------------------------------------------------------------
	// �e�N�X�`���摜���t�@�C������ǂݍ���
	// HRESULT D3DXCreateTextureFromFile(LPDIRECT3DDEVICE9 pDevice,
	//									 LPCSTR pSrcFile,
	//									 LPDIRECT3DTEXTURE9 *ppTexture);
	// ����
	//		pDevice		: IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
	//		pSrcFile	: �摜�̃t�@�C����
	//		ppTexture	: �e�N�X�`���ւ̃|�C���^�ւ̃|�C���^
	// �߂�l
	//		����������D3D_OK
	//-------------------------------------------------------------
//	D3DXCreateTextureFromFile(pD3DDevice, "Picture/tex2.dds", &m_pTexture);

	hr = D3DXCreateTextureFromFileEx(
        pD3DDevice,
        FileName.c_str(),              // �t�@�C����
        0,
        0,
        0,
        0,
        D3DFMT_A1R5G5B5,                // �F�������\��
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
        &m_pTexture                    // �e�N�X�`����
    );



	//-------------------------------------------------------------
	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	// HRESULT IDirect3DDevice9::SetRendererState(D3DRENDERSTATETYPE State, DWORD Value)
	// ����
	//		State	: �ύX�Ώۂ̃f�o�C�X �X�e�[�g�ϐ�
	//		Value	: �f�o�C�X�̃����_�����O �X�e�[�g�ɐݒ肷��V�����l
	// �߂�l
	//		����������D3D_OK
	//-------------------------------------------------------------
	// ���ʕ`�惂�[�h�̎w��
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �y��r���s�Ȃ�
    pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// �m�[���C�e�B���O���[�h
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	return S_OK;
}



//-------------------------------------------------------------
// �e�N�X�`���t���l�p�`��`��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void F_String::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// �f�o�C�X�̃`�F�b�N
	if(pD3DDevice == 0)
	{
		return;	// �f�o�C�X�ւ̃|�C���^������
	}

	// ���_�o�b�t�@�̃`�F�b�N
	if(m_pVB == 0)
	{
		return;	// ���_�o�b�t�@�ւ̃|�C���^������
	}

	// �s��̏�����
	D3DXMatrixIdentity(&m_world);

	// ���W�ϊ�
	D3DXMatrixScaling(&m_world,(FLOAT)1,(FLOAT)1,(FLOAT)1);
	
	


	
   
	// �����_�����O�p�C�v���C���Ƀ}�g���b�N�X��ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);

	//-------------------------------------------------------------
	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	// HRESULT IDirect3DDevice9::SetTexture(DWORD Stage,
	//										IDirect3DBaseTexture9 *pTexture);
	// ����
	//		Stage		: �e�N�X�`������͂���e�N�X�`���X�e�[�W�̔ԍ�
	//		pTexture	: �e�N�X�`���ւ̃|�C���^
	//-------------------------------------------------------------
	pD3DDevice->SetTexture(0, m_pTexture);

	// �e�N�X�`���t���l�p�`�̕`�揈��
	//-------------------------------------------------------------
	// ���_�o�b�t�@�̐ݒ�
	// HRESULT IDirect3DDevice9::SetStreamSource(UINT StreamNumber,
	//											IDirect3DDevice9 *pStreamData,
	//											UINT OffsetInBytes, UINT Stride);
	// ����
	//		StreamNumber	: ���_�X�g���[���ԍ�(0���w��)
	//		pStreamData		: ���_�o�b�t�@�ւ̃|�C���^
	//		OffsetInBytes	: ���Ԗڂ̒��_�f�[�^���痘�p���邩(0���w��)
	//		Stride			: ���_�f�[�^�\���̂̃T�C�Y
	// �߂�l
	//		����������D3D_OK
	//-------------------------------------------------------------
    pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));

	//-------------------------------------------------------------
	// ���_�o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
	// HRESULT IDirect3DDevice9::SetFVF(DWORD FVF);
	// ����
	//		FVF	: ���_�o�b�t�@�̃t�H�[�}�b�g
	// �߂�l
	//		����������D3D_OK
	//-------------------------------------------------------------
    pD3DDevice->SetFVF(MY_VERTEX_FVF);

	//-------------------------------------------------------------
	// ���_�o�b�t�@�̕`��
	// HRESULT IDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,
	//										   UINT StartVertex, UINT PrimitiveCount);
	// ����
	//		PrimitiveType	: �v���~�e�B�u�̃^�C�v
	//		StartVertex		: �ǂ̒��_���痘�p���邩(0���w��)
	//		PrimitiveCount	: �`�悷��O�p�`�̑���
	// �߂�l
	//		����������D3D_OK
	//
	// ��1�����̃v���~�e�B�u�̃^�C�v�ɂ͕`�悷��}�`�ɂ���ē��ӁE�s���ӂ�
	// ����̂ŕ`�悷��}�`�ɍ��킹�Đݒ肷�邱��
	// ����̓g���C�A���O���X�g���b�v�Ƃ���������p���Ă���
	//-------------------------------------------------------------
    pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pD3DDevice->SetTexture(0, 0);
}

void F_String::Update()
{
	MY_VERTEX* v;
	 m_pVB->Lock( 0, 0, (void**)&v, 0 );

	D3DXMatrixRotationYawPitchRoll(&m_world,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100);
   
	// ���b�N���O��
    m_pVB->Unlock();

}