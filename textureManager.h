//*****************************************************************************
//
//     テクスチャマネージメントの処理[texturManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    前方宣言
//*****************************************************************************

//*****************************************************************************
//    テクスチャ管轄クラスの定義
//*****************************************************************************
class CTextureManager
{
public:     // 誰でもアクセス可能
	CTextureManager();
	~CTextureManager();

	static CTextureManager *Create(int nNumTex);
	HRESULT Init(void);
	void Uninit(void);

	void SetNumTexture(int nNumTex);
	void SetFileName(char *pFileName, int nIdx);

	int GetNumTexture(void);
	char *GetFileName(int nIdx);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx);

	LPDIRECT3DTEXTURE9 GetTexture(int nIdx);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void ReleaseTexture(void);

	char               **m_pFileName;   // テクスチャのファイルパス名
	int                m_nNumTexture;   // 読み込んだテクスチャの数
	LPDIRECT3DTEXTURE9 *m_apTexture;    // テクスチャへのポインタ
};

#endif