//*****************************************************************************
//
//     Xファイルモデルの処理[sceneX.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    Xファイルモデルクラスの定義
//*****************************************************************************
class CSceneX : public CScene
{
public:    // 誰でもアクセス可能
	CSceneX(int nPriority = 3, OBJTYPE objType = OBJTYPE_XFILEMODEL);
	~CSceneX();

	static CSceneX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindModel(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture);
	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void BindTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetScale(const D3DXVECTOR3 scale);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	D3DXVECTOR3 GetVtxMin(void);
	D3DXVECTOR3 GetVtxMax(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetScale(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9    *m_pTexture;  // テクスチャへのポインタ
	LPD3DXMESH            m_pMesh;      // メッシュ情報へのポインタ
	LPD3DXBUFFER          m_pBuffMat;   // マテリアル情報へのポインタ
	DWORD                 m_nNumMat;    // マテリアル情報の数
	D3DXVECTOR3           m_VtxMin;     // 頂点座標の最小値
	D3DXVECTOR3           m_VtxMax;     // 頂点座標の最大値
	D3DXVECTOR3           m_Pos;        // 座標
	D3DXVECTOR3           m_Rot;        // 向き
	D3DXVECTOR3           m_Scale;      // 大きさの倍率
	D3DXMATRIX            m_MtxWorld;   // ワールドマトリックス
};

#endif