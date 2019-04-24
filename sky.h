//*****************************************************************************
//
//     空の処理[sky.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshDome.h"

//*****************************************************************************
//    空クラスの定義
//*****************************************************************************
class CSky : public CMeshDome
{
public:    // 誰でもアクセス可能
	CSky(int nPriority = 3, OBJTYPE objType = OBJTYPE_SKY);
	~CSky();
	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fRotSpeed;     // 回転するスピード
};

#endif