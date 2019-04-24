//*****************************************************************************
//
//     草の処理[grass.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GRASS_H_
#define _GRASS_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    草クラスの定義
//*****************************************************************************
class CGrass : public CScene3D
{
public:    // 誰でもアクセス可能
	//-----------------
	//  状態
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,    // 通常状態
		STATE_COL_LEFT,      // 左から当たっている状態
		STATE_COL_RIGHT,     // 右から当たっている状態
		STATE_MAX
	}STATE;

	CGrass(int nPriority = 3, OBJTYPE objType = OBJTYPE_GRASS);
	~CGrass();

	static CGrass *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, bool bSway, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, float fRange);

	void SetSway(const bool bSway);
	bool GetSway(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	STATE       m_State;             // 状態
	D3DXVECTOR3 m_aOffsetVertex[2];  // ボーンに対するオフセット
	D3DXVECTOR3 m_BonePos;           // ボーンの座標
	D3DXVECTOR3 m_BonePosDef;        // ボーンの座標(デフォルト値)
	D3DXVECTOR3 m_BoneRot;           // ボーンの向き
	D3DXMATRIX  m_BoneMtxWorld;      // ボーンのワールドマトリックス
	int         m_nStateCounter;     // 状態を管理するカウンター
	D3DXVECTOR3 m_SwayMove;          // 草の揺れる量
	D3DXVECTOR3 m_SwayMoveCurrent;   // 現在の草の揺れる量
	float       m_fRivisionSway;     // 草の揺れる量計算用
	int         m_nSwayCounter;      // 草の揺れを管理するカウンター
	int         m_nMaxSwayCounter;   // 草の揺れを管理するカウンターの最大値
	bool        m_bSway;             // 勝手に揺れるかどうか
	float       m_fCameraLength;     // カメラとの距離保存用
};

#endif