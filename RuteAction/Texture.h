//=============================================================
// Texture.h
// テクスチャ付き四角形描画クラスの定義
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
// テクスチャ付き四角形描画クラス
// 基底クラス
//		DrawObject	: 描画オブジェクトの基底クラス
//=============================================================
class Texture :
	public DrawObject
{
public:
	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	Texture();
	Texture(std::string name,float X,float Y,float Z,float H,float W,Renderer *R,int T = 0);

	void TextureInit(std::string name,float X,float Y,float Z,float H,float W,Renderer *R,int T = 0);

	//---------------------------------------------------------
	// 仮想デストラクタ
	//---------------------------------------------------------
	~Texture();

	void SetPos(float x, float y, float z);	//位置
	void SetPos(D3DXVECTOR3 &pos){Pos = pos;}

	void SetRot(float x, float y, float z);	//回転
	void SetRot(D3DXVECTOR3 &rot){Rot = rot;}

	void SetSize(float x, float y, float z);//サイズ
	void SetSize(D3DXVECTOR3 &size){Size = size;}

	D3DXVECTOR3	* GetPos(){return &Pos;}
	D3DXVECTOR3 * GetRot(){return &Rot;}
	D3DXVECTOR3 * GetSize(){return &Size;}

	void SetVelo(float x, float y, float z);	//速度
	void SetVelo(D3DXVECTOR3 &velo){Velo = velo;}

	void SetAccel(float x, float y, float z);	//加速度
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
	
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 Size;//サイズ
	D3DXVECTOR3 Rot;//回転
	D3DXVECTOR3 Velo;//速度
	D3DXVECTOR3 Accel;//加速度
	float Mass;//質量
	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

	// 頂点バッファの構造体
	struct MY_VERTEX
	{
		D3DXVECTOR3	p;		// 位置
		DWORD		color;	// 色
		D3DXVECTOR2	t;		// テクスチャ座標
	};


	int alfa;//透過率

private:
	int RenderType;

	// マトリックス
	D3DXMATRIX m_proj;	// 射影行列	
	D3DXMATRIX m_world;	// ワールド行列
	D3DXMATRIX m_world_R;//回転行列
	D3DXMATRIX m_world_M;//平行移動行列

	
	// 頂点バッファのフォーマットの定義
	const DWORD MY_VERTEX_FVF;

	// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;

	LPDIRECT3DTEXTURE9		*m_pTexture2[10];

	VOID				SetWorld(D3DXMATRIX *);
	VOID				SetWorld_RHW(D3DXMATRIX *);
	VOID				SetWorld_Relative(D3DXMATRIX *);
	


public:
	//テクスチャのファイル名
	std::string FileName;
	//---------------------------------------------------------
	// テクスチャ付き四角形を生成
	// 引数
	//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
	// 戻り値
	//		成功したらS_OK
	//---------------------------------------------------------
	HRESULT Create();
	
	//---------------------------------------------------------
	// テクスチャ付き四角形を描画
	// 引数
	//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
	//---------------------------------------------------------
	void SetMyVertex(float,float);
	void Draw();
	void Update();
	void SetpTexture(LPDIRECT3DTEXTURE9 *);
	HRESULT SetpTexturefromFile(LPDIRECT3DTEXTURE9 *,std::string);

};
