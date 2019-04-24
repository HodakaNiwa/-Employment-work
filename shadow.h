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
#include "sceneX.h"

//*****************************************************************************
//    モデルクラス(シーンクラス派生)の定義
//*****************************************************************************
class CShadow : public CSceneX
{
public:    // 誰でもアクセス可能
	CShadow(int nPriority = 3, OBJTYPE objType = OBJTYPE_SHADOW);
	~CShadow();

	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXMESH   m_pMesh;     // メッシュへのポインタ
	static LPD3DXBUFFER m_pBuffMat;  // マテリアル情報へのポインタ
	static DWORD        m_nNumMat;   // マテリアル情報の数
};

#endif