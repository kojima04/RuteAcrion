//=============================================================
// BlockManager.h
//=============================================================
#pragma once

//=============================================================
// include
//=============================================================
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include "Block.h"


using namespace std;

#define SKELTONALFA 150

typedef enum ___ComboBlkinf{
	COMBO_BLOCKSHAPE,
	COMBO_BLOCKTYPE,
	COMBO_POSX,
	COMBO_BLKINFMAX,
}e_ComboBlkinf;

typedef enum ___BlockShape {
	BLOCKSHAPE_NULL,
	BLOCKSHAPE_SQUARE,
	BLOCKSHAPE_TRIANGLE,
	BLOCKSHAPE_BYTRIANGLE,
	BLOCKSHAPE_BYSQUARE,
	BLOCKSHAPE_OBJECT,
	BLOCKSHAPEMAX,
}e_BlockShape;

typedef enum ___BlockType{
	BLOCKTYPE_NULL,
	BLOCKTYPE_NORMAL,
	BLOCKTYPE_RED,
	BLOCKTYPE_BRUE,
	BLOCKTYPE_BECTLE,
	BLOCKTYPE_MAGNET,
	BLOCKTYPEMAX,
}e_BlockType2;

typedef enum ___ObjectType{
	OBJECTTYPE_NULL,
	OBJECTTYPE_START,
	OBJECTTYPE_CHECK,
	OBJECTTYPE_CARD,
	OBJECTTYPEMAX,
}e_ObjectType;


//=============================================================
// BlockManager
//=============================================================
class BlockManager 
{
public:

	//---------------------------------------------------------
	// コンストラクタ
	//---------------------------------------------------------
	BlockManager(Renderer *R,CSoundManager * pSoundManager);

	//---------------------------------------------------------
	// 仮想デストラクタ
	//---------------------------------------------------------
	~BlockManager();

	HWND *hCmb_BlkInf[COMBO_BLKINFMAX];
	int UnderShapeNum[BLOCKSHAPEMAX];
	TextureData *TexTable2[BLOCKSHAPEMAX][BLOCKTYPEMAX];
	int SelectType;
	int SelectShape;
	Block *B;
	Texture *MouseCursor;
	std::list<Block*> Blocklist;  // Block型の双方向リスト
	void Input();
	void SetEdgeScale(TextureData *tex,int n,float x, float y);
	D3DXVECTOR3 EdgeScaleTable[BLOCKSHAPEMAX][8];
	int EdgeNumberTable[BLOCKSHAPEMAX];

	bool BlockHit_Reflect(Texture *,Block *);
	int BlockHit(Texture *,Block *);
	int BlockHit2(Texture *,Block *);
	void SetCPos(D3DXVECTOR3 &cpos){CPos = cpos;}
	void SetCPos(float x, float y, float z);
	D3DXVECTOR3 *GetCPos(){return &CPos;}
	Renderer * Camera; 
	Block *SelectBlock;
	Block *MouseBlock;
	void SetCombo_BlkInf();
	void ExpandCombo();
	int ReserveType;
	D3DXVECTOR3 MPostoBlk;
	D3DXVECTOR3 MPostoScreen;
	bool Dragflg_Block;
	bool Dragflg_Camera;

	
	POINT SrideScale;
	void SetSrideScale();
	
	/*int Send_Type;
	int Send_Shape;*/
private:
	CSoundManager * m_pSoundManager;
	D3DXVECTOR3 CPos;
	
	void TexTableInit();
	
};
