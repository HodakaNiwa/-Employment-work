//*****************************************************************************
//
//     Xファイルモデルの処理[sceneX.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                        // テクスチャへのポインタ
	m_pMesh = NULL;                           // メッシュ情報へのポインタ
	m_pBuffMat = NULL;                        // マテリアル情報へのポインタ
	m_nNumMat = 0;                            // マテリアル情報の数
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 向き
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 大きさの倍率
	D3DXMatrixIdentity(&m_MtxWorld);          // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CSceneX *pSceneX = NULL;               // Xファイルモデルクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSceneX == NULL)
		{// メモリが空になっている
			pSceneX = new CSceneX(nPriority);
			if (pSceneX != NULL)
			{// インスタンスを生成できた
				// 各種値の設定
				pSceneX->SetPos(pos);      // 座標
				pSceneX->SetRot(rot);      // 向き
				pSceneX->SetScale(scale);  // 大きさの倍率

				// モデル情報を設定
				pSceneX->BindModel(pMesh, pBuffMat, nNumMat, VtxMax, VtxMin, pTexture);

				if (FAILED(pSceneX->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pSceneX;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSceneX::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	// ポインタを空にしておく
	// メッシュ情報
	if (m_pMesh != NULL)
	{
		m_pMesh = NULL;
	}
	// マテリアル情報へのポインタ
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat = NULL;
	}
	// テクスチャへのポインタ
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	Release();  // 自身のオブジェクトを破棄
}

//=============================================================================
//    更新処理
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	D3DMATERIAL9 matDef;   // 現在のマテリアル保存用
	D3DXMATERIAL *pMat;    // マテリアルデータへのポインタ

	// レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ワールドマトリックス設定処理
			SetMtxWorld(pDevice);

			// 法線情報を正規化する(スケーリング時に法線情報がいじられるので)
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			if (m_pBuffMat != NULL && m_pMesh != NULL)
			{// Xファイルからモデルデータが読み込めている
				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
				{// 設定されていたマテリアルの数だけ繰り返し
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					if (m_pTexture != NULL)
					{// テクスチャ情報が引き出せている
						// テクスチャの設定
						pDevice->SetTexture(0, m_pTexture[nCntMat]);
					}
					else
					{// テクスチャ情報が引き出せていない
						// テクスチャの設定
						pDevice->SetTexture(0, NULL);
					}

					// モデル(パーツ)の描画
					m_pMesh->DrawSubset(nCntMat);
				}
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);

			// 法線を正規化しない
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		}
	}
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CSceneX::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxScale, mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    モデル情報設定処理
//=============================================================================
void CSceneX::BindModel(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture)
{
	// モデル情報の割り当て
	if (pMesh != NULL)
	{// メッシュ情報が読み込めた
		m_pMesh = pMesh;          // メッシュ情報へのポインタ
	}
	if (pBuffMat != NULL)
	{// マテリアル情報へのポインタを読み込めた
		m_pBuffMat = pBuffMat;    // マテリアル情報へのポインタ
	}
	if (nNumMat != -1)
	{// マテリアルの数情報が読み込めた
		m_nNumMat = nNumMat;      // マテリアル情報の数
	}
	if (pTexture != NULL)
	{// テクスチャ情報へのポインタが読み込めた
		m_pTexture = pTexture;    // テクスチャ情報へのポインタ
	}
	m_VtxMax = VtxMax;            // 頂点の最大値情報
	m_VtxMin = VtxMin;            // 頂点の最小値情報

	// 大きさの倍率を頂点情報にも反映させる
	// 頂点の最大値
	m_VtxMax.x *= m_Scale.x;
	m_VtxMax.y *= m_Scale.y;
	m_VtxMax.z *= m_Scale.z;

	// 頂点の最小値
	m_VtxMin.x *= m_Scale.x;
	m_VtxMin.y *= m_Scale.y;
	m_VtxMin.z *= m_Scale.z;
}

//=============================================================================
//    メッシュの頂点座標最小値設定処理
//=============================================================================
void CSceneX::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    マテリアル情報へのポインタ設定処理
//=============================================================================
void CSceneX::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    マテリアル情報の数設定処理
//=============================================================================
void CSceneX::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    メッシュの頂点座標最小値設定処理
//=============================================================================
void CSceneX::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    メッシュの頂点座標最大値設定処理
//=============================================================================
void CSceneX::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    テクスチャへのポインタ設定処理
//=============================================================================
void CSceneX::BindTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    座標設定処理
//=============================================================================
void CSceneX::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向き設定処理
//=============================================================================
void CSceneX::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    大きさの倍率設定処理
//=============================================================================
void CSceneX::SetScale(const D3DXVECTOR3 scale)
{
	m_Scale = scale;

	// 大きさの倍率を頂点情報にも反映させる
	// 頂点の最大値
	m_VtxMax.x *= m_Scale.x;
	m_VtxMax.y *= m_Scale.y;
	m_VtxMax.z *= m_Scale.z;

	// 頂点の最小値
	m_VtxMin.x *= m_Scale.x;
	m_VtxMin.y *= m_Scale.y;
	m_VtxMin.z *= m_Scale.z;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CSceneX::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//   メッシュの頂点座標最小値取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    メッシュの頂点座標最大値取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    テクスチャ情報へのポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 *CSceneX::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    座標取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向き取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    大きさの倍率取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetScale(void)
{
	return m_Scale;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CSceneX::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    メッシュ情報取得処理
//=============================================================================
LPD3DXMESH CSceneX::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    マテリアル情報取得処理
//=============================================================================
LPD3DXBUFFER CSceneX::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    マテリアルの数取得処理
//=============================================================================
DWORD CSceneX::GetNumMat(void)
{
	return m_nNumMat;
}