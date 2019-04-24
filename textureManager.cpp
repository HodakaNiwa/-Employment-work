//*****************************************************************************
//
//     テクスチャ管轄の処理[textureManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "textureManager.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CTextureManager::CTextureManager()
{
	// 各種値のクリア
	m_pFileName = NULL;  // テクスチャのファイル名
	m_nNumTexture = 0;   // 読み込んだテクスチャの数
	m_apTexture = NULL;  // テクスチャへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTextureManager::~CTextureManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTextureManager *CTextureManager::Create(int nNumTex)
{
	CTextureManager *pTextureManager = NULL;  // テクスチャ管轄クラス型のポインタ
	if (pTextureManager == NULL)
	{// メモリが確保できる状態にある
		pTextureManager = new CTextureManager;
		if (pTextureManager != NULL)
		{// メモリが確保できた
			pTextureManager->SetNumTexture(nNumTex);  // 読み込むテクスチャの数を設定
			// 初期化処理を行う
			if (FAILED(pTextureManager->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	// インスタンスを返す
	return pTextureManager;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTextureManager::Init(void)
{
	if (m_nNumTexture >= 1)
	{// 読み込むテクスチャの数が1個以上ある
	    // 読み込むテクスチャの数だけメモリを確保する
		if (m_apTexture == NULL)
		{// メモリが確保できる状態にある
			m_apTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
		}
		if (m_pFileName == NULL)
		{// メモリが確保できる状態にある
			m_pFileName = new char*[m_nNumTexture];
			if (m_pFileName != NULL)
			{// メモリが確保できた
				for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
				{// 読み込むテクスチャの数だけ繰り返し
					m_pFileName[nCntTex] = new char[256];
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTextureManager::Uninit(void)
{
	// テクスチャ情報の開放
	if (m_apTexture != NULL)
	{// メモリが確保されている
		ReleaseTexture();
	}

	// ファイル名の開放
	if (m_pFileName != NULL)
	{// メモリが確保できる状態にある
		for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
		{// 読み込むテクスチャの数だけ繰り返し
			delete[] m_pFileName[nCntTex];
			m_pFileName[nCntTex] = NULL;
		}
		delete[] m_pFileName;
		m_pFileName = NULL;
	}
}

//=============================================================================
//    読み込むテクスチャの数設定処理
//=============================================================================
void CTextureManager::SetNumTexture(int nNumTex)
{
	m_nNumTexture = nNumTex;
}

//=============================================================================
//    テクスチャのファイル名設定処理
//=============================================================================
void CTextureManager::SetFileName(char *pFileName, int nIdx)
{
	strcpy(m_pFileName[nIdx], pFileName);
}

//=============================================================================
//    読み込むテクスチャの数設定処理
//=============================================================================
int CTextureManager::GetNumTexture(void)
{
	return m_nNumTexture;
}

//=============================================================================
//    テクスチャのファイル名取得処理
//=============================================================================
char *CTextureManager::GetFileName(int nIdx)
{
	return m_pFileName[nIdx];
}

//=============================================================================
//    テクスチャの取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(int nIdx)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;  // テクスチャへのポインタ
	if (m_apTexture != NULL)
	{// メモリが確保されている
		pTexture = m_apTexture[nIdx];
	}
	return pTexture;
}

//=============================================================================
//    テクスチャの割り当て処理
//=============================================================================
void CTextureManager::BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx)
{
	m_apTexture[nIdx] = pTexture;
}

//=============================================================================
//    テクスチャの開放処理
//=============================================================================
void CTextureManager::ReleaseTexture(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{// 読み込んだテクスチャの数だけ繰り返し
		if (m_apTexture[nCntTex] != NULL)
		{// メモリが確保されている
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}

	delete[] m_apTexture;
	m_apTexture = NULL;
}