//*****************************************************************************
//
//     モデル管轄の処理[modelManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************

//*****************************************************************************
//    モデル管轄クラスの定義
//*****************************************************************************
class CModelManager
{
public:    // 誰でもアクセス可能
	CModelManager();
	~CModelManager();
	static CModelManager *Create(int nNumModel);
	HRESULT Init(void);
	void Uninit(void);

	void SetNumModel(int nNumModel);
	void SetMesh(LPD3DXMESH pMesh, int nIdx);
	void SetMaterial(LPDIRECT3DDEVICE9 pDevice, LPD3DXBUFFER pBuffMat, DWORD nNumMat, int nIdx);
	void SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx);
	void SetNumMat(DWORD nNumMat, int nIdx);
	void SetVtxMax(D3DXVECTOR3 nNumMat, int nIdx);
	void SetVtxMin(D3DXVECTOR3 nNumMat, int nIdx);
	void BindTexture(LPDIRECT3DTEXTURE9 *pTexture, int nIdx);

	LPD3DXMESH GetMesh(int nIdx);
	LPD3DXBUFFER GetBuffMat(int nIdx);
	DWORD GetNumMat(int nIdx);
	D3DXVECTOR3 GetVtxMax(int nIdx);
	D3DXVECTOR3 GetVtxMin(int nIdx);
	LPDIRECT3DTEXTURE9 *GetTexture(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void ReleaseMesh(void);
	void ReleaseBuffMat(void);
	void ReleaseNumMat(void);
	void ReleaseVtxMax(void);
	void ReleaseVtxMin(void);
	void ReleaseTexture(void);

	int                   m_nNumModel;    // 読み込んだモデルの数
	LPD3DXMESH            *m_pMesh;       // メッシュ情報へのポインタ
	LPD3DXBUFFER          *m_pBuffMat;    // マテリアル情報へのポインタ
	DWORD                 *m_pNumMat;     // マテリアル情報の数
	D3DXVECTOR3           *m_pVtxMax;     // 頂点座標の最大値へのポインタ
	D3DXVECTOR3           *m_pVtxMin;     // 頂点座標の最小値へのポインタ
	LPDIRECT3DTEXTURE9    **m_pTexture;   // テクスチャへのポインタ(マテリアル情報から抜き出す)
};

#endif