//=============================================================
// Triangle.cpp
// �O�p�`�`��N���X�̎���
//=============================================================

//=============================================================
// include
//=============================================================
#include "Texture2.h"

//=============================================================
// ����
//=============================================================
//-------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------
Triangle::Triangle()
: m_pVB(0), MY_VERTEX_FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
Triangle::~Triangle()
{
	// ���_�o�b�t�@�̉��
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

//-------------------------------------------------------------
// �O�p�`�𐶐�
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
// �߂�l
//		����������S_OK
//-------------------------------------------------------------
HRESULT Triangle::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
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
										3 * sizeof(MY_VERTEX),
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
    v[0].p = D3DXVECTOR3(-1.0f,  1.0f, 0.0f );
    v[1].p = D3DXVECTOR3( 1.0f, -1.0f, 0.0f );
    v[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f );
	// �e���_�̐F
    v[0].color = D3DXCOLOR( 1.0f, 0.0f,  0.0f,  1.0f);
    v[1].color = D3DXCOLOR( 0.0f, 1.0f,  0.0f,  1.0f);
    v[2].color = D3DXCOLOR( 0.0f, 0.0f,  1.0f,  1.0f);
	// ���b�N���O��
    m_pVB->Unlock();

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
	// �m�[���C�e�B���O���[�h
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

//-------------------------------------------------------------
// ���W�ϊ����s��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
/*
void Triangle::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
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
	D3DXMatrixLookAtLH( &m_view, &D3DXVECTOR3(0.0f, 0.0f, -5.0f), 
								 &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
								 &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

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
	D3DXMatrixPerspectiveFovLH(&m_proj, D3DXToRadian(45.0f), aspect, 5.0f, 1000.0f);

	// �ˉe�s��̐ݒ�
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);

}
*/

//-------------------------------------------------------------
// �O�p�`��`��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void Triangle::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
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

	// ���W//�ϊ��i�J�����̐ݒ�Ȃǁj
	//Transform(pD3DDevice);

	// ���W�ϊ��i���f���̔z�u�j
	// �s��̏�����
	D3DXMatrixIdentity(&m_world);
	// �����_�����O�p�C�v���C���Ƀ}�g���b�N�X��ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);


	// �O�p�`�̕`�揈��
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
    pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
}