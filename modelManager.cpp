//*****************************************************************************
//
//     モデル管轄の処理[modelManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CModelManager::CModelManager()
{
	// 各種値のクリア
	m_nNumModel = 0;    // 読み込んだモデルの数
	m_pMesh = NULL;     // モデルのメッシュ情報
	m_pBuffMat = NULL;  // モデルのマテリアル情報
	m_pNumMat = NULL;   // モデルのマテリアルの数情報
	m_pVtxMax = NULL;   // 頂点座標の最大値へのポインタ
	m_pVtxMin = NULL;   // 頂点座標の最小値へのポインタ
	m_pTexture = NULL;  // テクスチャへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CModelManager::~CModelManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CModelManager *CModelManager::Create(int nNumModel)
{
	CModelManager *pModelManager = NULL;  // モデル管轄クラス型のポインタ
	if (pModelManager == NULL)
	{// メモリが確保できる状態にある
		pModelManager = new CModelManager;
		if (pModelManager != NULL)
		{// メモリが確保できた
			pModelManager->SetNumModel(nNumModel);  // モデルの数を設定
													// 初期化処理を行う
			if (FAILED(pModelManager->Init()))
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
	return pModelManager;
}

//=============================================================================
//    モデルの読み込み処理
//=============================================================================
HRESULT CModelManager::Init(void)
{
	if (m_nNumModel >= 1)
	{// 読み込むモデルの数が1個以上ある
	    // 読み込むモデルの数だけメモリを確保する
	    // メッシュ情報
		if (m_pMesh == NULL)
		{// メモリが確保できる状態にある
			m_pMesh = new LPD3DXMESH[m_nNumModel];
			if (m_pMesh != NULL)
			{// メモリが確保できた
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// 読み込むモデルの数だけ繰り返し
					m_pMesh[nCntModel] = NULL;
				}
			}
		}

		// マテリアル情報
		if (m_pBuffMat == NULL)
		{// メモリが確保できる状態にある
			m_pBuffMat = new LPD3DXBUFFER[m_nNumModel];
			if (m_pBuffMat != NULL)
			{// メモリが確保できた
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// 読み込むモデルの数だけ繰り返し
					m_pBuffMat[nCntModel] = NULL;
				}
			}
		}

		// マテリアルの数情報
		if (m_pNumMat == NULL)
		{// メモリが確保できる状態にある
			m_pNumMat = new DWORD[m_nNumModel];
		}

		// 頂点座標の最大値情報
		if (m_pVtxMax == NULL)
		{// メモリが確保できる状態にある
			m_pVtxMax = new D3DXVECTOR3[m_nNumModel];
		}

		// 頂点座標の最小値情報
		if (m_pVtxMin == NULL)
		{// メモリが確保できる状態にある
			m_pVtxMin = new D3DXVECTOR3[m_nNumModel];
		}

		// テクスチャへのポインタ情報
		if (m_pTexture == NULL)
		{// メモリが確保できる状態である
			m_pTexture = new LPDIRECT3DTEXTURE9*[m_nNumModel];
			if (m_pTexture != NULL)
			{// メモリが確保できた
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// 読み込むモデルの数だけ繰り返し
					m_pTexture[nCntModel] = NULL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    モデルの開放処理
//=============================================================================
void CModelManager::Uninit(void)
{
	// テクスチャへのポインタ情報の開放
	if (m_pTexture != NULL)
	{// メモリが確保されている
		ReleaseTexture();
	}

	// メッシュ情報の開放
	if (m_pMesh != NULL)
	{// メモリが確保されている
		ReleaseMesh();
	}

	// マテリアル情報の開放
	if (m_pBuffMat != NULL)
	{// メモリが確保されている
		ReleaseBuffMat();
	}

	// マテリアルの数情報の開放
	if (m_pNumMat != NULL)
	{// メモリが確保されている
		ReleaseNumMat();
	}

	// 頂点の最大値情報の開放
	if (m_pVtxMax != NULL)
	{// メモリが確保されている
		ReleaseVtxMax();
	}

	// 頂点の最小値情報の開放
	if (m_pVtxMin != NULL)
	{// メモリが確保されている
		ReleaseVtxMin();
	}
}

//=============================================================================
//    読み込むモデルの数を設定する
//=============================================================================
void CModelManager::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//=============================================================================
//    モデルのメッシュ情報を設定する
//=============================================================================
void CModelManager::SetMesh(LPD3DXMESH pMesh, int nIdx)
{
	m_pMesh[nIdx] = pMesh;

	if (m_pMesh[nIdx] != NULL)
	{// モデルが読み込めた
	    // 最小値と最大値を初期化
		m_pVtxMin[nIdx] = D3DXVECTOR3(10000000.0f, 10000000.0f, 10000000.0f);
		m_pVtxMax[nIdx] = D3DXVECTOR3(-10000000.0f, -10000000.0f, -10000000.0f);

		int nNumVtx;     // 頂点数
		DWORD sizeFVF;   // 頂点フォーマットのサイズ
		BYTE *pVtxBuff;  // 頂点バッファへのポインタ

		// 頂点数を獲得
		nNumVtx = m_pMesh[nIdx]->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nIdx]->GetFVF());

		// 頂点バッファをロック
		m_pMesh[nIdx]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

			if (Vtx.x < m_pVtxMin[nIdx].x)
			{// 取り出したX座標の最小値が今までのよりも小さい
				m_pVtxMin[nIdx].x = Vtx.x;  // 最小値更新
			}
			if (Vtx.y < m_pVtxMin[nIdx].y)
			{// 取り出したY座標の最小値が今までのよりも小さい
				m_pVtxMin[nIdx].y = Vtx.y;  // 最小値更新
			}
			if (Vtx.z < m_pVtxMin[nIdx].z)
			{// 取り出したZ座標の最小値が今までのよりも小さい
				m_pVtxMin[nIdx].z = Vtx.z;  // 最小値更新
			}
			if (Vtx.x > m_pVtxMax[nIdx].x)
			{// 取り出したX座標の最大値が今までのよりも大きい
				m_pVtxMax[nIdx].x = Vtx.x;  // 最大値更新
			}
			if (Vtx.y > m_pVtxMax[nIdx].y)
			{// 取り出したY座標の最大値が今までのよりも大きい
				m_pVtxMax[nIdx].y = Vtx.y;  // 最大値更新
			}
			if (Vtx.z > m_pVtxMax[nIdx].z)
			{// 取り出したZ座標の最大値が今までのよりも大きい
				m_pVtxMax[nIdx].z = Vtx.z;  // 最大値更新
			}

			pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
		}
		// 頂点バッファをアンロック
		m_pMesh[nIdx]->UnlockVertexBuffer();
	}
}

//=============================================================================
//    モデルのマテリアル情報を設定する
//=============================================================================
void CModelManager::SetMaterial(LPDIRECT3DDEVICE9 pDevice, LPD3DXBUFFER pBuffMat, DWORD nNumMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;
	m_pNumMat[nIdx] = nNumMat;

	// テクスチャ情報を抜き出す
	// マテリアル情報からテクスチャ情報を引き出す
	if (pBuffMat != NULL)
	{// マテリアル情報が読み込めている
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat[nIdx]->GetBufferPointer();  // マテリアル情報のポインタを取得
		D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[nNumMat];                        // マテリアル情報の数だけポインタを動的確保
		m_pTexture[nIdx] = new LPDIRECT3DTEXTURE9[nNumMat];                        // テクスチャ情報へのポインタをマテリアル情報の数だけ動的確保

		for (DWORD nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
		{// 頂点数の数だけ繰り返し
		    // マテリアル情報を読み込む
			pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

			// 環境光を初期化する
			pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

			// テクスチャへのポインタを空にしておく
			m_pTexture[nIdx][nCntMat] = NULL;

			// テクスチャの情報を読み込む
			if (pMat[nCntMat].pTextureFilename != NULL &&
				lstrlen(pMat[nCntMat].pTextureFilename) > 0)
			{// テクスチャのファイル名がある
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_pTexture[nIdx][nCntMat]);
			}
		}
		// 使用したメモリを開放しておく
		delete[] pMatBuff;
		pMatBuff = NULL;
	}
}

//=============================================================================
//    モデルのマテリアル情報へのポインタを設定する
//=============================================================================
void CModelManager::SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;
}

//=============================================================================
//    モデルのマテリアルの数情報を設定する
//=============================================================================
void CModelManager::SetNumMat(DWORD nNumMat, int nIdx)
{
	m_pNumMat[nIdx] = nNumMat;
}

//=============================================================================
//    モデルの頂点の最大値情報を設定する
//=============================================================================
void CModelManager::SetVtxMax(D3DXVECTOR3 VtxMax, int nIdx)
{
	m_pVtxMax[nIdx] = VtxMax;
}

//=============================================================================
//    モデルの頂点の最小値情報を設定する
//=============================================================================
void CModelManager::SetVtxMin(D3DXVECTOR3 VtxMin, int nIdx)
{
	m_pVtxMin[nIdx] = VtxMin;
}

//=============================================================================
//    モデルのテクスチャへのポインタ情報を設定する
//=============================================================================
void CModelManager::BindTexture(LPDIRECT3DTEXTURE9 *pTexture, int nIdx)
{
	m_pTexture[nIdx] = pTexture;
}

//=============================================================================
//    モデルのメッシュ情報を取得する
//=============================================================================
LPD3DXMESH CModelManager::GetMesh(int nIdx)
{
	LPD3DXMESH pMesh = NULL;  // メッシュ情報

	if (m_pMesh != NULL)
	{// メモリが確保されてる
		pMesh = m_pMesh[nIdx];   // 保存されたメッシュ情報を代入
	}

	return pMesh;  // メッシュ情報を返す
}

//=============================================================================
//    モデルのマテリアル情報を取得する
//=============================================================================
LPD3DXBUFFER CModelManager::GetBuffMat(int nIdx)
{
	LPD3DXBUFFER pBuffMat = NULL;  // マテリアル情報

	if (m_pBuffMat != NULL)
	{// メモリが確保されてる
		pBuffMat = m_pBuffMat[nIdx];   // 保存されたマテリアル情報を代入
	}

	return pBuffMat;  // マテリアル情報を返す
}

//=============================================================================
//    モデルのマテリアルの数情報を取得する
//=============================================================================
DWORD CModelManager::GetNumMat(int nIdx)
{
	DWORD nNumMat = 0;  // マテリアルの数情報

	if (m_pNumMat != NULL)
	{// メモリが確保されてる
		nNumMat = m_pNumMat[nIdx];   // 保存されたマテリアルの数情報を代入
	}

	return nNumMat;  // マテリアルの数情報を返す
}

//=============================================================================
//    モデルの頂点の最大値情報を取得する
//=============================================================================
D3DXVECTOR3 CModelManager::GetVtxMax(int nIdx)
{
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 頂点の最大値情報

	if (m_pVtxMax != NULL)
	{// メモリが確保されてる
		VtxMax = m_pVtxMax[nIdx];   // 保存された頂点の最大値情報を代入
	}

	return VtxMax;  // 頂点の最大値情報を返す
}

//=============================================================================
//    モデルの頂点の最小値情報を取得する
//=============================================================================
D3DXVECTOR3 CModelManager::GetVtxMin(int nIdx)
{
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 頂点の最小値情報

	if (m_pVtxMin != NULL)
	{// メモリが確保されてる
		VtxMin = m_pVtxMin[nIdx];   // 保存された頂点の最小値情報を代入
	}

	return VtxMin;  // 頂点の最小値情報を返す
}

//=============================================================================
//    モデルのテクスチャへのポインタ情報を取得する
//=============================================================================
LPDIRECT3DTEXTURE9 *CModelManager::GetTexture(int nIdx)
{
	return m_pTexture[nIdx];
}

//=============================================================================
//    モデルのメッシュ情報を開放する
//=============================================================================
void CModelManager::ReleaseMesh(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// 読み込んだモデルの数だけ繰り返し
		DIRECT_RELEASE(m_pMesh[nCntModel]);
	}
	delete[] m_pMesh;
	m_pMesh = NULL;
}

//=============================================================================
//    モデルのマテリアル情報を開放する
//=============================================================================
void CModelManager::ReleaseBuffMat(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// 読み込んだモデルの数だけ繰り返し
		DIRECT_RELEASE(m_pBuffMat[nCntModel]);
	}
	delete[] m_pBuffMat;
	m_pBuffMat = NULL;
}

//=============================================================================
//    モデルのマテリアルの数情報を開放する
//=============================================================================
void CModelManager::ReleaseNumMat(void)
{
	delete[] m_pNumMat;
	m_pNumMat = NULL;
}

//=============================================================================
//    モデルの頂点の最大値情報を開放する
//=============================================================================
void CModelManager::ReleaseVtxMax(void)
{
	delete[] m_pVtxMax;
	m_pVtxMax = NULL;
}

//=============================================================================
//    モデルの頂点の最小値情報情報を開放する
//=============================================================================
void CModelManager::ReleaseVtxMin(void)
{
	delete[] m_pVtxMin;
	m_pVtxMin = NULL;
}

//=============================================================================
//    モデルのテクスチャへのポインタ情報を開放する
//=============================================================================
void CModelManager::ReleaseTexture(void)
{
	if (m_pTexture != NULL)
	{// メモリが確保されている
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{// 読み込んだモデル数分繰り返し
			if (m_pTexture[nCntModel] != NULL)
			{// メモリが確保されている
				int nNumMat = (int)m_pNumMat[nCntModel];
				for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
				{// マテリアル情報の数だけ繰りかえし
					if (m_pTexture[nCntModel][nCntMat] != NULL)
					{// メモリが確保されている
						m_pTexture[nCntModel][nCntMat]->Release();
						m_pTexture[nCntModel][nCntMat] = NULL;
					}
				}
				delete[] m_pTexture[nCntModel];
				m_pTexture[nCntModel] = NULL;
			}
		}
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}