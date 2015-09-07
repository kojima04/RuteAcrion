#pragma once

//=============================================================
// include
//=============================================================
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

//=============================================================
// �ˑ�����N���X
//=============================================================
class DrawObject;

//=============================================================
// Renderer
// DirectGraphics�̏������y�ъǗ��N���X
//=============================================================
class Renderer
{
public:
	//---------------------------------------------------------
	// �R���X�g���N�^
	//---------------------------------------------------------
	Renderer();
	//---------------------------------------------------------
	// �f�X�g���N�^
	//---------------------------------------------------------
	~Renderer();

private:

	LPDIRECT3D9				m_pD3D;			// IDirect3D9�C���^�[�t�F�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	D3DPRESENT_PARAMETERS	m_D3DPP;		// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
	D3DXVECTOR3 CPos;
	D3DXVECTOR3 CFocus;

public:
	//---------------------------------------------------------
	// DirectGraphics�̏�����
	//		����
	//			hWnd			: �E�B���h�E�n���h��
	//			isFullScreen	: �t���X�N���[���̏ꍇTRUE
	//			clientWidth		: �N���C�A���g�̈�̕�
	//			clientHeight	: �N���C�A���g�̈�̍���
	//		�߂�l
	//			����������S_OK
	//---------------------------------------------------------
	HRESULT Initialize(HWND hWnd, BOOL isFullScreen, int clientWidth, int clientHeight);

	//---------------------------------------------------------
	// �`�悷��
	// ����
	//		pObject	: �`�悷��I�u�W�F�N�g
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

	D3DXMATRIX m_proj;	// �ˉe�s��
	D3DXMATRIX m_view;	// �r���[�C���O�s��
	D3DXMATRIX m_view_R;//��]�s��
	D3DXMATRIX m_view_M;//���s�ړ��s��
	D3DXMATRIX m_screen;
	
	//---------------------------------------------------------
	// �f�o�C�X�̎擾
	// �߂�l
	//		����������IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	//		���s������0
	//---------------------------------------------------------
	LPDIRECT3DDEVICE9 getDevice() const { return m_pD3DDevice; }

	void Transform(LPDIRECT3DDEVICE9 pD3DDevice);
};

	