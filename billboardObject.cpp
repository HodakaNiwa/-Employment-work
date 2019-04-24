//*****************************************************************************
//
//     配置用ビルボードの処理[billboardObject.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "billboardObject.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CBillboardObjectの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBillboardObject::CBillboardObject(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_nType = TYPE_NONE;     // 種類番号
	m_nTexIdx = 0;           // テクスチャ番号
	m_bDrawAddtive = false;  // 加算合成するかしないか
	m_fColRange = 0.0f;      // 当たり判定を取る範囲
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBillboardObject::~CBillboardObject()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBillboardObject *CBillboardObject::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType, int nPriority)
{
	CBillboardObject *pBillboardObject = NULL;      // 配置ビルボードクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();            // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pBillboardObject == NULL)
		{// メモリが空になっている
			pBillboardObject = new CBillboardObject(nPriority);
			if (pBillboardObject != NULL)
			{// インスタンスを生成できた
				if (FAILED(pBillboardObject->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
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

	return pBillboardObject;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBillboardObject::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	// 各種値の代入
	SetPos(pos);                    // ポリゴンの座標
	SetCol(col);                    // ポリゴンの色
	SetRot(fRot);                   // ポリゴンの向き
	SetWidth(fWidth);               // ポリゴンの幅
	SetHeight(fHeight);             // ポリゴンの高さ
	SetLighting(bLighting);         // ライティングするかしないか
	SetDrawAddtive(bDrawAddtive);   // 加算合成で描画するかしないか
	SetTexIdx(nTexIdx);             // 使用しているテクスチャの番号
	SetColRange(fColRange);         // 当たり判定を取る範囲
	SetType(nType);                 // 種類番号

	// 共通の初期化処理
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBillboardObject::Uninit(void)
{
	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBillboardObject::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBillboardObject::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			if (m_bDrawAddtive == true)
			{// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // 透明度が0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

			// 共通の描画処理
			CSceneBillboard::Draw();

			// アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CBillboardObject::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxWorld, mtxView, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// カメラの向きを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ワールドマトリックスに向きを設定(カメラの逆行列を入れることでカメラの正面に向きを補正)
	mtxWorld._11 = mtxView._11;
	mtxWorld._12 = mtxView._21;
	mtxWorld._13 = mtxView._31;
	//mtxWorld._21 = mtxView._12;
	//mtxWorld._22 = mtxView._22;
	//mtxWorld._23 = mtxView._32;
	mtxWorld._31 = mtxView._13;
	mtxWorld._32 = mtxView._23;
	mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CSceneBillboard::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CBillboardObject::Collision(D3DXVECTOR3 *pPos, float fColRange)
{
	bool bCol = false;       // 当たっているかどうか
	float fHitRange = 0.0f;  // 当たった判定を返す範囲
	float fLength = 0.0f;    // ビルボードの座標と引数の座標との距離

	// 当たり判定を取る長さを計算
	fHitRange = sqrtf((fColRange + m_fColRange) * (fColRange + m_fColRange));

	// ビルボードと引数の座標との距離を計算
	fLength = sqrtf((GetPos().x - pPos->x) * (GetPos().x - pPos->x) + (GetPos().y - pPos->y) * (GetPos().y - pPos->y) + (GetPos().z - pPos->z) * (GetPos().z - pPos->z));

	// 判定開始
	if (fLength <= fHitRange)
	{// ビルボードの座標と引数の座標との距離が当たり判定を取る長さより短い
		bCol = true;   // 当たった判定にする
	}

	return bCol;
}

//=============================================================================
//    草の表現処理
//=============================================================================
void CBillboardObject::GrassChange(void)
{

}

//=============================================================================
//    種類番号設定処理
//=============================================================================
void CBillboardObject::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    使用しているテクスチャの番号設定処理
//=============================================================================
void CBillboardObject::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    加算合成で描画するかしないか設定処理
//=============================================================================
void CBillboardObject::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    当たり判定を取る範囲設定処理
//=============================================================================
void CBillboardObject::SetColRange(const float fColRange)
{
	m_fColRange = fColRange;
}

//=============================================================================
//    種類番号取得処理
//=============================================================================
int CBillboardObject::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    使用しているテクスチャの番号取得処理
//=============================================================================
int CBillboardObject::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    加算合成で描画するかしないか取得処理
//=============================================================================
bool CBillboardObject::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}

//=============================================================================
//    当たり判定を取る範囲取得処理
//=============================================================================
float CBillboardObject::GetColRange(void)
{
	return m_fColRange;
}


//*****************************************************************************
//    CSetBillboardObjectの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSetBillboardObject::CSetBillboardObject(int nPriority, OBJTYPE objType) : CBillboardObject(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetBillboardObject::~CSetBillboardObject()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetBillboardObject *CSetBillboardObject::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType, int nPriority)
{
	CSetBillboardObject *pSetBillboardObject = NULL;   // 配置用の配置ビルボードクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();               // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSetBillboardObject == NULL)
		{// メモリが空になっている
			pSetBillboardObject = new CSetBillboardObject(nPriority);
			if (pSetBillboardObject != NULL)
			{// インスタンスを生成できた
				if (FAILED(pSetBillboardObject->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
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

	return pSetBillboardObject;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetBillboardObject::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	// 透明度を上げておく
	col.a = 0.5f;

	// 共通の初期化処理
	if (FAILED(CBillboardObject::Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetBillboardObject::Uninit(void)
{
	// 共通の終了処理
	CBillboardObject::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetBillboardObject::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSetBillboardObject::Draw(void)
{
	// 共通の描画処理
	CBillboardObject::Draw();
}

//=============================================================================
//    頂点情報を書き換える処理
//=============================================================================
void CSetBillboardObject::ChangeVertex(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pVtxBuff != NULL)
	{// 頂点バッファが取得できた
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
		SetLength(fLength);

		// オフセットの角度を求める
		float fAngle = atan2f(GetWidth(), GetHeight());
		SetAngle(fAngle);

		// 頂点の回転を考慮して座標を計算
		float XPos[4];
		float YPos[4];
		XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());

		// 頂点座標
		for (int nCntVer = 0; nCntVer < 4; nCntVer++)
		{// 頂点の数だけ繰り返し
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
		}

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[1].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[2].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[3].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}