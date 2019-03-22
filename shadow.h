//*****************************************************************************
//
//     影の処理[shadow.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    影(スプライト)クラスの定義
//*****************************************************************************
class CShadow : public CScene3D
{
public:    // 誰でもアクセス可能
	CShadow(int nPriority = 3, OBJTYPE objType = OBJTYPE_3DPOLYGON);
	~CShadow();

	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetVertexHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pLandNormal);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_pShadowTexture;     // 影用のテクスチャ
};

#endif