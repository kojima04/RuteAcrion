//=============================================================
// Texture.cpp
// �e�N�X�`���t���l�p�`�`��N���X�̎���
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
// �f�X�g���N�^
//-------------------------------------------------------------
Texture::~Texture()
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
// �e�N�X�`���t���l�p�`�𐶐�
// ����
//		G_Device : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
// �߂�l
//		����������S_OK
//-------------------------------------------------------------
HRESULT Texture::Create()
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
	if(::G_Device == 0)
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

    //VertexBuffer�̒��g�𖄂߂�
    MY_VERTEX* v;
	// �f�[�^�������ݒ��ɃV�X�e�������������ړ����Ȃ��悤�Ƀ��b�N����
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // �e���_�̈ʒu
	//v[0].p = D3DXVECTOR3((-1.0f*Size.x)/2, ( 1.0f*Size.y)/2, 0.0f);	// ���_�@
 //   v[1].p = D3DXVECTOR3((1.0f*Size.x)/2,  (1.0f*Size.y)/2, 0.0f );	// ���_�A
 //   v[2].p = D3DXVECTOR3((-1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// ���_�B
 //   v[3].p = D3DXVECTOR3((1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// ���_�C

	v[0].p = D3DXVECTOR3(-1.0f/2, 1.0f/2, 0.0f);	// ���_�@
    v[1].p = D3DXVECTOR3(1.0f/2,  1.0f/2, 0.0f );	// ���_�A
    v[2].p = D3DXVECTOR3(-1.0f/2,-1.0f/2, 0.0f );	// ���_�B
    v[3].p = D3DXVECTOR3(1.0f/2, -1.0f/2, 0.0f );	// ���_�C
	// �e�N�X�`�����W�̐ݒ�
    v[0].t = D3DXVECTOR2(0.0f,0.0f);
    v[1].t = D3DXVECTOR2(1.0f,0.0f);
    v[2].t = D3DXVECTOR2(0.0f,1.0f);
    v[3].t = D3DXVECTOR2(1.0f,1.0f);
	// �e���_�̐F(��)
	v[0].color = v[1].color = v[2].color = v[3].color = D3DCOLOR_RGBA(255,255,255,alfa);
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
	G_Device ->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	// �e�N�X�`���X�e�[�W�̈�ڂ̓��͂����̂܂܏o�͂��鏈�����s���Ɛݒ�
	G_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

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
//	D3DXCreateTextureFromFile(G_Device, "Picture/tex2.dds", &m_pTexture);

	hr = D3DXCreateTextureFromFileEx(
        G_Device,
        FileName.c_str(),              // �t�@�C����
        0,
        0,
        0,
        0,
        D3DFMT_A8B8G8R8,                // �F�������\��
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
        &m_pTexture                    // �e�N�X�`����
    );


	m_pTexture2[0] = &m_pTexture;



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
	G_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �y��r���s�Ȃ�
	G_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	G_Device->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// �m�[���C�e�B���O���[�h
    G_Device->SetRenderState(D3DRS_LIGHTING, FALSE);


	return S_OK;
}



//-------------------------------------------------------------
// �e�N�X�`���t���l�p�`��`��
// ����
//		G_Device : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void Texture::Draw()
{
	// �f�o�C�X�̃`�F�b�N
	if(G_Device == 0)
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

	//// ���W�ϊ�
	//D3DXMatrixScaling(&m_world,(FLOAT)1,(FLOAT)1,(FLOAT)1);
	//D3DXMatrixRotationYawPitchRoll(&m_world_R,(FLOAT)Rot.x,(FLOAT)Rot.y,(FLOAT)Rot.z);
	//D3DXMatrixTranslation(&m_world_M,(FLOAT)Pos.x,(FLOAT)Pos.y,(FLOAT)Pos.z);

	//D3DXMatrixMultiply(&m_world,&m_world,&m_world_R);
	//D3DXMatrixMultiply(&m_world,&m_world,&m_world_M);

	


	
   
	// �����_�����O�p�C�v���C���Ƀ}�g���b�N�X��ݒ�
	G_Device->SetTransform(D3DTS_WORLD, &m_world);

	//-------------------------------------------------------------
	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	// HRESULT IDirect3DDevice9::SetTexture(DWORD Stage,
	//										IDirect3DBaseTexture9 *pTexture);
	// ����
	//		Stage		: �e�N�X�`������͂���e�N�X�`���X�e�[�W�̔ԍ�
	//		pTexture	: �e�N�X�`���ւ̃|�C���^
	//-------------------------------------------------------------
	G_Device->SetTexture(0, *m_pTexture2[0]);

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
    G_Device->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));

	//-------------------------------------------------------------
	// ���_�o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
	// HRESULT IDirect3DDevice9::SetFVF(DWORD FVF);
	// ����
	//		FVF	: ���_�o�b�t�@�̃t�H�[�}�b�g
	// �߂�l
	//		����������D3D_OK
	//-------------------------------------------------------------
    G_Device->SetFVF(MY_VERTEX_FVF);

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
    G_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	G_Device->SetTexture(0, 0);
}

void Texture::Update()
{
	MY_VERTEX* v;
	 m_pVB->Lock( 0, 0, (void**)&v, 0 );

	//D3DXMatrixRotationYawPitchRoll(&m_world,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100,(FLOAT)timeGetTime()/100);
   
	// ���b�N���O��
    m_pVB->Unlock();

}


HRESULT Texture::SetpTexturefromFile(LPDIRECT3DTEXTURE9 *tex,std::string FileName)
{
	
	HRESULT hr = 0;


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

	hr = D3DXCreateTextureFromFileEx(
        G_Device,
        FileName.c_str(),              // �t�@�C����
        0,
        0,
        0,
        0,
        D3DFMT_A8B8G8R8,                // �F�������\��
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
        tex                   // �e�N�X�`����
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
	//VertexBuffer�̒��g�𖄂߂�
    MY_VERTEX* v;
	// �f�[�^�������ݒ��ɃV�X�e�������������ړ����Ȃ��悤�Ƀ��b�N����
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // �e���_�̈ʒu
	//v[0].p = D3DXVECTOR3((-1.0f*Size.x)/2, ( 1.0f*Size.y)/2, 0.0f);	// ���_�@
 //   v[1].p = D3DXVECTOR3((1.0f*Size.x)/2,  (1.0f*Size.y)/2, 0.0f );	// ���_�A
 //   v[2].p = D3DXVECTOR3((-1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// ���_�B
 //   v[3].p = D3DXVECTOR3((1.0f*Size.x)/2, (-1.0f*Size.y)/2, 0.0f );	// ���_�C
	v[0].p = D3DXVECTOR3(-1.0f/2, 1.0f/2, 0.0f);	// ���_�@
    v[1].p = D3DXVECTOR3(1.0f/2,  1.0f/2, 0.0f );	// ���_�A
    v[2].p = D3DXVECTOR3(-1.0f/2,-1.0f/2, 0.0f );	// ���_�B
    v[3].p = D3DXVECTOR3(1.0f/2, -1.0f/2, 0.0f );	// ���_�C
	// �e�N�X�`�����W�̐ݒ�
    v[0].t = D3DXVECTOR2(leftedge,0.0f);
    v[1].t = D3DXVECTOR2(rightedge,0.0f);
    v[2].t = D3DXVECTOR2(leftedge,1.0f);
    v[3].t = D3DXVECTOR2(rightedge,1.0f);
	// �e���_�̐F(��)
	v[0].color = v[1].color = v[2].color = v[3].color = D3DCOLOR_RGBA(255,255,255,alfa);
	// ���b�N���O��
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