//=============================================================
// Texture.h
// �e�N�X�`���t���l�p�`�`��N���X�̒�`
//=============================================================
#pragma once

//=============================================================
// include
//=============================================================
#include "DrawObject.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

//=============================================================
// Texture
// �e�N�X�`���t���l�p�`�`��N���X
// ���N���X
//		DrawObject	: �`��I�u�W�F�N�g�̊��N���X
//=============================================================
class F_String :
	public DrawObject
{
public:
	//---------------------------------------------------------
	// �R���X�g���N�^
	//---------------------------------------------------------
	F_String();
	F_String(std::string name,float X,float Y,float Z,float H,float W);

	//---------------------------------------------------------
	// ���z�f�X�g���N�^
	//---------------------------------------------------------
	~F_String();

private:
	//�ʒu�̃p�����[�^
	float x,y,z,width,height;
	//�p�x
	float Xangle,Yangle,Zangle;
	//�e�N�X�`���̃t�@�C����
	std::string FileName;

	// �}�g���b�N�X
	D3DXMATRIX m_proj;	// �ˉe�s��	
	D3DXMATRIX m_world;	// ���[���h�s��

	// ���_�o�b�t�@�̍\����
	struct MY_VERTEX
	{
		D3DXVECTOR3	p;		// �ʒu
		DWORD		color;	// �F
		D3DXVECTOR2	t;		// �e�N�X�`�����W
	};

	// ���_�o�b�t�@�̃t�H�[�}�b�g�̒�`
	const DWORD MY_VERTEX_FVF;

	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;

public:
	//---------------------------------------------------------
	// �e�N�X�`���t���l�p�`�𐶐�
	// ����
	//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
	// �߂�l
	//		����������S_OK
	//---------------------------------------------------------
	HRESULT Create(LPDIRECT3DDEVICE9 pD3DDevice);

	//---------------------------------------------------------
	// �e�N�X�`���t���l�p�`��`��
	// ����
	//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
	//---------------------------------------------------------
	void Draw(LPDIRECT3DDEVICE9 pD3DDevice);
	void Update();
	


};
