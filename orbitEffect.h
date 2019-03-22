//*****************************************************************************
//
//     軌跡エフェクトの処理[orbitEffect.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ORBITEFFECT_H_
#define _ORBITEFFECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshOrbit.h"

//*****************************************************************************
//    軌跡クラスの定義
//*****************************************************************************
class COrbitEffect : public CMeshOrbit
{
public:    // 誰でもアクセス可能
	COrbitEffect(int nPriority = 3, OBJTYPE objType = OBJTYPE_ORBITEFFECT);
	~COrbitEffect();

	static COrbitEffect *Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, D3DXVECTOR3 OffsetAmp1, D3DXVECTOR3 OffsetAmp2,
		D3DXCOLOR ColUp, D3DXCOLOR ColDown,float fAlphaDecayUp, float fAlphaDecayDown, int nXBlock, int nZBlock, D3DXMATRIX *pMtxParent, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetColUp(const D3DXCOLOR ColUp);
	void SetColDown(const D3DXCOLOR ColDown);
	void SetAlphaDecayUp(const float fAlphaDecayUp);
	void SetAlphaDecayDown(const float fAlphaDecayDown);
	void SetOffsetAmp1(const D3DXVECTOR3 OffsetAmp);
	void SetOffsetAmp2(const D3DXVECTOR3 OffsetAmp);

	D3DXCOLOR GetColUp(void);
	D3DXCOLOR GetColDown(void);
	float GetAlphaDecayUp(void);
	float GetAlphaDecayDown(void);
	D3DXVECTOR3 GetOffsetAmp1(void);
	D3DXVECTOR3 GetOffsetAmp2(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXCOLOR   m_ColUp;            // 頂点上側の色
	D3DXCOLOR   m_ColDown;          // 頂点下側の色
	float       m_fAlphaDecayUp;    // 頂点上側透明度の減衰値
	float       m_fAlphaDecayDown;  // 頂点下側透明度の減衰値
	D3DXVECTOR3 m_OffsetAmp[2];     // 頂点オフセットの増幅値
};

#endif