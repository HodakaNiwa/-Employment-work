//*****************************************************************************
//
//		川の処理[river.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RIVER_H_
#define _RIVER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshField.h"

//*****************************************************************************
//    川クラスの定義
//*****************************************************************************
class CRiver : public CMeshField
{
public:    // 誰でもアクセス可能
	CRiver(int nPriority = 3, OBJTYPE objType = OBJTYPE_RIVER);
	~CRiver();

	static CRiver *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, float fAddTexU, float fAddTexV, char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAddTexU(const float fAddTexU);
	void SetAddTexV(const float fAddTexV);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fAddTexU;  // テクスチャU座標をどれくらい動かすか
	float m_fAddTexV;  // テクスチャV座標をどれくらい動かすか
	float m_fTexU;     // テクスチャのU座標
	float m_fTexV;     // テクスチャのV座標

};

#endif