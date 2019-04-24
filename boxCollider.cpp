//*****************************************************************************
//
//     当たり判定用箱モデルの処理[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "boxCollider.h"
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
CBoxCollider::CBoxCollider(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{// 箱の頂点の数だけ繰り返し
		m_Vertex[nCntVer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vertex[nCntVer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	for (int nCntIndex = 0; nCntIndex < 24; nCntIndex++)
	{// 描画に使用するインデックス情報の数だけ繰り返し
		m_wIndex[nCntIndex] = 0;
	}
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 向き
	D3DXMatrixIdentity(&m_MtxWorld);          // ワールドマトリックス
	m_fWidth = 0.0f;                          // 箱の横幅
	m_fHeight = 0.0f;                         // 箱の高さ
	m_fDepth = 0.0f;                          // 箱の奥行
	m_bReturnFlag = false;                    // 当たった際に戻すかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag, int nPriority)
{
	CBoxCollider *pBoxCollider = NULL;      // 当たり判定用箱モデルクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pBoxCollider == NULL)
		{// メモリが空になっている
			pBoxCollider = new CBoxCollider(nPriority);
			if (pBoxCollider != NULL)
			{// インスタンスを生成できた
			    // 各種値の設定
				pBoxCollider->SetPos(pos);                 // 座標
				pBoxCollider->SetWidth(fWidth);            // 箱の横幅
				pBoxCollider->SetHeight(fHeight);          // 箱の高さ
				pBoxCollider->SetDepth(fDepth);            // 箱の奥行
				pBoxCollider->SetReturnFlag(bReturnFlag);  // 当たった際に戻すかどうか

				if (FAILED(pBoxCollider->Init()))
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

	return pBoxCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBoxCollider::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 頂点バッファの生成
			MakeVertex();
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBoxCollider::Uninit(void)
{
	Release();  // 自身のオブジェクトを破棄
}

//=============================================================================
//    更新処理
//=============================================================================
void CBoxCollider::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBoxCollider::Draw(void)
{
#ifdef _DEBUG
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Culling;
			DWORD Lighting;

			// ライティングしない設定に
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// カリングを外しておく
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// 箱の描画
			pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(BOX_VERTEX));

			// カリングの設定を戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ライティングの設定を戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}

#endif
}

//=============================================================================
//    頂点情報生成処理
//=============================================================================
void CBoxCollider::MakeVertex(void)
{
	// 頂点座標を設定(順番は 上側の 左奥 -> 右奥 -> -> 左前 -> 右奥
	//                       下側の 左奥 -> 右奥 -> -> 左前 -> 右奥)
	m_Vertex[0].pos = D3DXVECTOR3(0.0f, m_fHeight, m_fDepth);
	m_Vertex[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	m_Vertex[2].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	m_Vertex[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	m_Vertex[4].pos = D3DXVECTOR3(0.0f, 0.0f, m_fDepth);
	m_Vertex[5].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	m_Vertex[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Vertex[7].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);

	// 頂点カラーを設定(全て赤色に設定)
	m_Vertex[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// インデックス情報を設定
	m_wIndex[0] = 0;
	m_wIndex[1] = 1;
	m_wIndex[2] = 1;
	m_wIndex[3] = 3;
	m_wIndex[4] = 3;
	m_wIndex[5] = 2;
	m_wIndex[6] = 2;
	m_wIndex[7] = 0;
	m_wIndex[8] = 0;
	m_wIndex[9] = 4;
	m_wIndex[10] = 1;
	m_wIndex[11] = 5;
	m_wIndex[12] = 2;
	m_wIndex[13] = 6;
	m_wIndex[14] = 3;
	m_wIndex[15] = 7;
	m_wIndex[16] = 4;
	m_wIndex[17] = 5;
	m_wIndex[18] = 5;
	m_wIndex[19] = 7;
	m_wIndex[20] = 7;
	m_wIndex[21] = 6;
	m_wIndex[22] = 6;
	m_wIndex[23] = 4;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CBoxCollider::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxScale, mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

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
//    当たり判定処理
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // 当たったかどうか

	// Y軸上の判定
	if (pPos->y < m_Pos.y + m_fHeight && pPos->y + ColRange.y > m_Pos.y)
	{// 現在の高さが箱の中にいる
		// X軸上の判定
		if (pPos->z + ColRange.z > m_Pos.z && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
		{// Z軸上にいるかどうか判定
			if (pPosOld->x + ColRange.x <= m_Pos.x && pPos->x + ColRange.x > m_Pos.x)
			{// 箱の左側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = m_Pos.x - ColRange.x;
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= m_Pos.x + m_fWidth && pPos->x - ColRange.x < m_Pos.x + m_fWidth)
			{// 箱の右側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = m_Pos.x + m_fWidth + ColRange.x;
				}
				bHit = true;
			}
		}
		// Z軸上の判定
		if (pPos->x + ColRange.x > m_Pos.x && pPos->x - ColRange.x < m_Pos.x + m_fWidth)
		{// X軸上にいるかどうか判定
			if (pPosOld->z - ColRange.z >= m_Pos.z + m_fDepth && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
			{// 箱の奥側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = m_Pos.z + m_fDepth + ColRange.z;
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= m_Pos.z && pPos->z + ColRange.z > m_Pos.z)
			{// 箱の手前側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = m_Pos.z - ColRange.z;
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= m_Pos.y + m_fHeight && pPos->y < m_Pos.y + m_fHeight)
	{// 高さが上から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > m_Pos.x && pPos->x - ColRange.x < m_Pos.x + m_fWidth
			&& pPos->z + ColRange.z > m_Pos.z && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = m_Pos.y + m_fHeight;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= m_Pos.y && pPos->y + ColRange.y > m_Pos.y)
	{// 高さが下から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > m_Pos.x && pPos->x - ColRange.x < m_Pos.x + m_fWidth
			&& pPos->z + ColRange.z > m_Pos.z && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = m_Pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    座標設定処理
//=============================================================================
void CBoxCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向き設定処理
//=============================================================================
void CBoxCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CBoxCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    箱の横幅設定処理
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    箱の高さ設定処理
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    箱の奥行設定処理
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか設定処理
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    座標取得処理
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向き取得処理
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CBoxCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    箱の横幅取得処理
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    箱の高さ取得処理
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    箱の奥行取得処理
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか取得処理
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}