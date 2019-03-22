//*****************************************************************************
//
//     メッシュ2Dポリゴンの処理[mesh2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESH2D_H_
#define _MESH2D_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    メッシュ2Dポリゴンクラスの定義
//*****************************************************************************
class CMesh2D : public CScene
{
public:    // 誰でもアクセス可能
	CMesh2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESH2D);
	~CMesh2D();

	static CMesh2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	              // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;                    // メッシュ2Dポリゴンの座標
	D3DXCOLOR               m_Col;                    // メッシュ2Dポリゴンの色
	float                   m_fWidth;                 // 1ブロック分の幅
	float                   m_fHeight;                // 1ブロック分高さ
	int                     m_nXBlock;                // 横の分割数
	int                     m_nYBlock;                // 縦の分割数
	int                     m_nTexSplitU;             // テクスチャU座標の分割数
	int                     m_nTexSplitV;             // テクスチャV座標の分割数
	int                     m_nNumVertex;             // 頂点数
	int                     m_nNumIndex;              // インデックス数
	int                     m_nNumPolygon;            // ポリゴン数
};

#endif