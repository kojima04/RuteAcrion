#include <math.h>
#include "Common.h"

#define HIT_LEFT 1
#define HIT_RIGHT 2
#define HIT_HEAD 3
#define HIT_UNDER 4
#define HIT_UNDER_BAUND 5
#define STAIR_HEIGHT 20
#define CALVELO_PARALLEL 0
#define CALVELO_VERTICAL 1
#define CALVELO_BECTLE 2


bool CrossCheckLine(D3DXVECTOR3 *,D3DXVECTOR3 *,D3DXVECTOR3 *,D3DXVECTOR3 *);

int CheckHitLine(Texture *,D3DXVECTOR3 *,D3DXVECTOR3 *,int);

int CheckHitEdge(Texture *,Texture *,int);

int CheckHitPoint(Texture *,D3DXVECTOR3 *,D3DXVECTOR3 *,D3DXVECTOR3 *);

void WalkStair(Texture *,Block *);

int CheckInsideAngle(D3DXVECTOR3 *,D3DXVECTOR3 *,D3DXVECTOR3 *);

int CheckHitCourse(D3DXVECTOR3 *,D3DXVECTOR3 *,D3DXVECTOR3 *);

D3DXVECTOR3 *CalVeloHitSide(D3DXVECTOR3 *,D3DXVECTOR3 *,D3DXVECTOR3 *,int,CSoundManager *);

bool CheckContained(D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3);

static int getSignedArea(D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3);

bool CheckPosinTex(D3DXVECTOR3 *,Texture *);

bool CheckTexinTex(Texture *,Texture *);

bool CheckTexinScrean(Texture *,D3DXVECTOR3 *);