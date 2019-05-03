//*****************************************************************************
//
//     ロックオン用ポリゴンの処理[lockon_poly.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LOCKON_POLY_H_
#define _LOCKON_POLY_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    ロックオン用ポリゴンクラスの定義
//*****************************************************************************
class CLockOnPoly : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	CLockOnPoly(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARD);
	~CLockOnPoly();

	static CLockOnPoly *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting = false, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting = false);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

#endif