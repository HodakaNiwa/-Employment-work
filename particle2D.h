//*****************************************************************************
//
//     パーティクル(2D)の処理[particle2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    パーティクル(2D)クラスの定義
//*****************************************************************************
class CParticle2D : public CScene2D
{
public:     // 誰でもアクセス可能
	CParticle2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLE2D);
	~CParticle2D();

	static CParticle2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col, float fWidth,
		float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
		float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col, float fWidth,
		float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
		float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(const D3DXVECTOR3 move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetLife(const int nLife);
	void SetRotPattern(const int nRotPattern);
	void SetSpread(const float fSpread);
	void SetChangeRot(const float fChangeRot);
	void SetDrawAddtive(const bool bDrawAddtive);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	int GetLife(void);
	int GetRotPattern(void);
	float GetSpread(void);
	float GetChangeRot(void);
	bool GetDrawAddtive(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Move;           // 移動量
	D3DXVECTOR3 m_ChangeMove;     // 移動量の変化量
	D3DXCOLOR   m_ChangeCol;      // 色の変化量
	int         m_nLife;          // 寿命
	int         m_nRotPattern;    // 回転の種類
	float       m_fSpread;        // 大きさの変化量
	float       m_fChangeRot;     // 回転の変化量
	bool        m_bDrawAddtive;   // 加算合成で描画するかしないか
};

#endif