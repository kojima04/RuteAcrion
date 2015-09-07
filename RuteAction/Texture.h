//=============================================================
// Texture.h
// �e�N�X�`���t���l�p�`�`��N���X�̒�`
//=============================================================
#pragma once

//=============================================================
// include
//=============================================================
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "DrawObject.h"

typedef enum ___RenderType {
	RENDERTYPE_ABSOLUTE,
	RENDERTYPE_RHW,
	RENDERTYPE_RELATIVE,
	RENDERTYPEMAX,
}e_RenderType;

struct TextureData
{
	std::string filename;
	LPDIRECT3DTEXTURE9 pTex;
	D3DXVECTOR3 EdgeScale[6];
	int EdgeNum;
	TextureData()
	{
		EdgeNum = 4;
	}
	void SetEdgeScale(int Num,float x,float y)
	{
	EdgeScale[Num].x = x; 
	EdgeScale[Num].y = y; 
	};
	void SetEdgeScale(D3DXVECTOR3 *epos)
	{
		for(int i = 0;i < 4;i++)
			SetEdgeScale(i,epos[i].x,epos[i].y);
	}
	int	ShapeType;
};



//=============================================================
// Texture
// �e�N�X�`���t���l�p�`�`��N���X
// ���N���X
//		DrawObject	: �`��I�u�W�F�N�g�̊��N���X
//=============================================================
class Texture :
	public DrawObject
{
public:
	//---------------------------------------------------------
	// �R���X�g���N�^
	//---------------------------------------------------------
	Texture();
	Texture(std::string name,float X,float Y,float Z,float H,float W,Renderer *R,int T = 0);

	void TextureInit(std::string name,float X,float Y,float Z,float H,float W,Renderer *R,int T = 0);

	//---------------------------------------------------------
	// ���z�f�X�g���N�^
	//---------------------------------------------------------
	~Texture();

	void SetPos(float x, float y, float z);	//�ʒu
	void SetPos(D3DXVECTOR3 &pos){Pos = pos;}

	void SetRot(float x, float y, float z);	//��]
	void SetRot(D3DXVECTOR3 &rot){Rot = rot;}

	void SetSize(float x, float y, float z);//�T�C�Y
	void SetSize(D3DXVECTOR3 &size){Size = size;}

	D3DXVECTOR3	* GetPos(){return &Pos;}
	D3DXVECTOR3 * GetRot(){return &Rot;}
	D3DXVECTOR3 * GetSize(){return &Size;}

	void SetVelo(float x, float y, float z);	//���x
	void SetVelo(D3DXVECTOR3 &velo){Velo = velo;}

	void SetAccel(float x, float y, float z);	//�����x
	void SetAccel(D3DXVECTOR3 &accel){Accel = accel;}

	D3DXVECTOR3	* GetVelo(){return &Velo;}
	D3DXVECTOR3 * GetAccel(){return &Accel;}

	void SetMass(float m){Mass = m;}

	float GetMass(){return Mass;}

	int  GetAlfa(){return alfa;}

	void SetAlfa(int A);

	void Move();
	void MoveCancel();
	void Acceleration();

	Renderer * Camera; 

	void SetEdgeScale(int Num,float x,float y);
	void SetEdgeScale(D3DXVECTOR3 *epos);

	

	bool CheckTexHit(Texture *tex1,Texture *tex2);
	D3DXVECTOR3 EdgeScale[6];	
	int EdgeNumber;

protected:
	
	D3DXVECTOR3 Pos;//�ʒu
	D3DXVECTOR3 Size;//�T�C�Y
	D3DXVECTOR3 Rot;//��]
	D3DXVECTOR3 Velo;//���x
	D3DXVECTOR3 Accel;//�����x
	float Mass;//����
	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

	// ���_�o�b�t�@�̍\����
	struct MY_VERTEX
	{
		D3DXVECTOR3	p;		// �ʒu
		DWORD		color;	// �F
		D3DXVECTOR2	t;		// �e�N�X�`�����W
	};


	int alfa;//���ߗ�

private:
	int RenderType;

	// �}�g���b�N�X
	D3DXMATRIX m_proj;	// �ˉe�s��	
	D3DXMATRIX m_world;	// ���[���h�s��
	D3DXMATRIX m_world_R;//��]�s��
	D3DXMATRIX m_world_M;//���s�ړ��s��

	
	// ���_�o�b�t�@�̃t�H�[�}�b�g�̒�`
	const DWORD MY_VERTEX_FVF;

	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;

	LPDIRECT3DTEXTURE9		*m_pTexture2[10];

	VOID				SetWorld(D3DXMATRIX *);
	VOID				SetWorld_RHW(D3DXMATRIX *);
	VOID				SetWorld_Relative(D3DXMATRIX *);
	


public:
	//�e�N�X�`���̃t�@�C����
	std::string FileName;
	//---------------------------------------------------------
	// �e�N�X�`���t���l�p�`�𐶐�
	// ����
	//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
	// �߂�l
	//		����������S_OK
	//---------------------------------------------------------
	HRESULT Create();
	
	//---------------------------------------------------------
	// �e�N�X�`���t���l�p�`��`��
	// ����
	//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
	//---------------------------------------------------------
	void SetMyVertex(float,float);
	void Draw();
	void Update();
	void SetpTexture(LPDIRECT3DTEXTURE9 *);
	HRESULT SetpTexturefromFile(LPDIRECT3DTEXTURE9 *,std::string);

};
