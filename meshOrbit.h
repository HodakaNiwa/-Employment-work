//*****************************************************************************
//
//     軌跡の処理[meshOrbit.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    軌跡クラスの定義
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:    // 誰でもアクセス可能
	CMeshOrbit(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHORBIT);
	~CMeshOrbit();

	static CMeshOrbit *Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, int nXBlock, int nZBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetOffsetPos(const D3DXVECTOR3 OffsetPos, int nIdx);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetVertexPos(D3DXVECTOR3 pos, int nIdx);
	void SetVertexCol(D3DXCOLOR col, int nIdx);
	void SetVertexPos(D3DXVECTOR3 *pPos);
	void SetVertexCol(D3DXCOLOR *pCol);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetMtxParent(D3DXMATRIX *pMtxParent);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetOffsetPos(int nIdx);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	int GetNumPolygon(void);
	D3DXVECTOR3 *GetVertexPos(void);
	D3DXCOLOR *GetVertexCol(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX *GetMtxParent(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;      // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	     // テクスチャへのポインタ
	int                     m_nXBlock;       // 横の分割数
	int                     m_nZBlock;       // 奥行の分割数
	int                     m_nNumVertex;    // 頂点数
	int                     m_nNumPolygon;   // ポリゴン数
	D3DXVECTOR3             *m_pVertexPos;   // 頂点の座標(頂点数分動的に確保)
	D3DXVECTOR3             m_aOffsetPos[2]; // 頂点のオフセット
	D3DXCOLOR               *m_pVertexCol;   // 頂点の色(頂点数分動的に確保)
	D3DXMATRIX              m_MtxWorld;      // ワールドマトリックス
	D3DXMATRIX              *m_pMtxParent;   // 親のマトリックスへのポインタ
};

#endif