//*****************************************************************************
//
//     当たり判定用箱モデルの処理[boxCollider.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BOXCOLLIDER_H_
#define _BOXCOLLIDER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    当たり判定用箱モデルクラスの定義
//*****************************************************************************
class CBoxCollider : public CScene
{
public:    // 誰でもアクセス可能
	CBoxCollider(int nPriority = 3, OBJTYPE objType = OBJTYPE_BOXCOLLIDER);
	~CBoxCollider();

	static CBoxCollider *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetDepth(const float fDepth);
	void SetReturnFlag(const bool bReturnFlag);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetDepth(void);
	bool GetReturnFlag(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeVertex(void);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	}BOX_VERTEX;

	BOX_VERTEX              m_Vertex[8];    // 箱の頂点情報
	WORD                    m_wIndex[24];   // 箱の描画に使用するインデックス情報
	D3DXVECTOR3             m_Pos;          // 座標
	D3DXVECTOR3             m_Rot;          // 向き
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
	float                   m_fWidth;       // 箱の横幅
	float                   m_fHeight;      // 箱の高さ
	float                   m_fDepth;       // 箱の奥行
	bool                    m_bReturnFlag;  // 当たった際に戻すかどうか
};

#endif