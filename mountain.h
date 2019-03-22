//*****************************************************************************
//
//     山の処理[mountain.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MOUNTAIN_H_
#define _MOUNTAIN_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
//    山クラスの定義
//*****************************************************************************
class CMountain : public CMeshCylinder
{
public:    // 誰でもアクセス可能
	CMountain(int nPriority = 3, OBJTYPE objType = OBJTYPE_MOUNTAIN);
	~CMountain();
	static CMountain *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

#endif