//*****************************************************************************
//
//     メッシュウォールの処理[meshWall.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    メッシュウォールクラスの定義
//*****************************************************************************
class CMeshWall : public CScene
{
public:    // 誰でもアクセス可能
	CMeshWall(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHWALL);
	~CMeshWall();

	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Offset, D3DXVECTOR3 *pInterPoint = NULL);

	void SetTexIdx(const int nTexIdx);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	int GetTexIdx(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
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
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int                     m_nTexIdx;                // テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	              // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;                    // メッシュウォールの座標
	D3DXVECTOR3             m_Rot;                    // メッシュウォールの向き
	D3DXCOLOR               m_Col;                    // メッシュウォールの色
	float                   m_fWidth;                 // 1ブロック分の幅
	float                   m_fHeight;                // 1ブロック分高さ
	int                     m_nXBlock;                // 横の分割数
	int                     m_nYBlock;                // 縦の分割数
	int                     m_nTexSplitU;             // テクスチャU座標の分割数
	int                     m_nTexSplitV;             // テクスチャV座標の分割数
	int                     m_nNumVertex;             // 頂点数
	int                     m_nNumIndex;              // インデックス数
	int                     m_nNumPolygon;            // ポリゴン数
	D3DXMATRIX              m_MtxWorld;               // ワールドマトリックス
	D3DXVECTOR3             m_WallNormal;             // 壁の法線
};

//*****************************************************************************
//    配置用メッシュウォールクラスの定義
//*****************************************************************************
class CSetMeshWall : public CMeshWall
{
public:    // 誰でもアクセス可能
	CSetMeshWall(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETMESHWALL);
	~CSetMeshWall();

	static CSetMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

#endif