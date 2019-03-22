//*****************************************************************************
//
//     ゲージの処理[gauge.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "gauge.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define GAUGE_LIFE_CUTTIMING     (60)    // 体力ゲージを減らしが始まるタイミング
#define GAUGE_WIDTH_ADD          (42.0f) // ゲージの幅を削る割合
#define SPECIALGAUGE_WIDTH_ADD   (30.0f) // 必殺技ゲージの幅を削る割合

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CGaugeの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGauge::CGauge(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_fGaugeRate = 0.0f;  // ゲージの割合
	m_fLeftWidth = 0.0f;  // ゲージの左側座標
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nPriority)
{
	CGauge *pGauge = NULL;                 // ゲージクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGauge == NULL)
		{// メモリが空になっている
			pGauge = new CGauge(nPriority);
			if (pGauge != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGauge->SetPos(pos);         // ポリゴンの座標
				pGauge->SetCol(col);         // ポリゴンの色
				pGauge->SetRot(fRot);        // ポリゴンの向き
				pGauge->SetWidth(fWidth);    // ポリゴンの幅
				pGauge->SetHeight(fHeight);  // ポリゴンの高さ

				if (FAILED(pGauge->Init()))
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

	return pGauge;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGauge::Init(void)
{
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
void CGauge::Uninit(void)
{
	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGauge::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CGauge::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;

			// ライティングの設定を外す
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CSceneBillboard::Draw();

			// ライティングの設定を元に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGauge::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	m_fLeftWidth = -GetWidth();  // ゲージの左側オフセット
}

//=============================================================================
//    ゲージの割合設定処理
//=============================================================================
void CGauge::SetGaugeRate(const float fGaugeRate)
{
	m_fGaugeRate = fGaugeRate;
}

//=============================================================================
//    ゲージの左側の幅設定処理
//=============================================================================
void CGauge::SetLeftWidth(const float fLeftWidth)
{
	m_fLeftWidth = fLeftWidth;
}

//=============================================================================
//    ゲージの割合取得処理
//=============================================================================
float CGauge::GetGaugeRate(void)
{
	return m_fGaugeRate;
}

//=============================================================================
//    ゲージの左側の幅取得処理
//=============================================================================
float CGauge::GetLeftWidth(void)
{
	return m_fLeftWidth;
}



//*****************************************************************************
//    CGaugeLifeの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGaugeLife::CGaugeLife(int nPriority, OBJTYPE objType) : CGauge(nPriority, objType)
{
	// 各種値のクリア
	m_nCounter = 0;            // ゲージの減らしを管理するカウンター
	m_State = STATE_NONE;      // 状態
	m_nLife = 0;               // 体力
	m_pRedBill = NULL;         // 体力ゲージが減った時に表示する赤いゲージ
	m_pWhiteBill = NULL;       // 体力ゲージのフレーム
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGaugeLife::~CGaugeLife()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGaugeLife *CGaugeLife::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, int nPriority)
{
	CGaugeLife *pGaugeLife = NULL;         // 体力ゲージクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGaugeLife == NULL)
		{// メモリが空になっている
			pGaugeLife = new CGaugeLife(nPriority);
			if (pGaugeLife != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGaugeLife->SetPos(pos);         // ポリゴンの座標
				pGaugeLife->SetCol(col);         // ポリゴンの色
				pGaugeLife->SetRot(fRot);        // ポリゴンの向き
				pGaugeLife->SetWidth(fWidth);    // ポリゴンの幅
				pGaugeLife->SetHeight(fHeight);  // ポリゴンの高さ
				pGaugeLife->SetLife(nLife);      // 体力

				if (FAILED(pGaugeLife->Init()))
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

	return pGaugeLife;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGaugeLife::Init(void)
{
	// 初期化に失敗した
	if (FAILED(CGauge::Init()))
	{
		return E_FAIL;
	}

	// 体力ゲージのフレームを生成する
	if (m_pWhiteBill == NULL)
	{
		m_pWhiteBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (GetWidth() * 1.04f), (GetHeight() * 1.1f), 0.0f,2);
	}

	// 後ろの赤いゲージを生成する
	if (m_pRedBill == NULL)
	{
		m_pRedBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, 2);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGaugeLife::Uninit(void)
{
	// 体力ゲージのフレーム開放処理
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->Uninit();
		m_pWhiteBill = NULL;
	}

	// 赤いゲージの開放処理
	if (m_pRedBill != NULL)
	{
		m_pRedBill->Uninit();
		m_pRedBill = NULL;
	}

	// 共通の描画処理
	CGauge::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGaugeLife::Update(void)
{
	if (m_State == STATE_CUT)
	{// ゲージを減らす状態だったら
		m_nCounter++;  // カウンターを増やす
		if (m_nCounter >= GAUGE_LIFE_CUTTIMING)
		{// ゲージを減らすタイミングになっている
			// 後ろの赤いゲージを減らす
			if (m_pRedBill != NULL)
			{// 赤いゲージが生成されている
				// 赤いゲージの幅を少しずつ減らす
				float fWidth = m_pRedBill->GetWidth();
				fWidth -= 0.5f;
				if (fWidth <= GetWidth())
				{// 赤いゲージの幅が体力ゲージを下回った
					fWidth = GetWidth();   // 幅を戻す
					m_State = STATE_NONE;  // 状態を戻す
					m_nCounter = 0;        // カウンターを戻す
				}
				m_pRedBill->SetWidth(fWidth);

				// ポリゴンの幅を減らす
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRedBill->GetVtxBuff();
				if (pVtxBuff != NULL)
				{// 頂点バッファが生成されている
					// 頂点情報の設定
					VERTEX_3D *pVtx;

					// 頂点バッファをロックし,頂点データへのポインタを取得
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// 頂点座標
					pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(fWidth, GetHeight(), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f);

					// 頂点バッファをアンロックする
					pVtxBuff->Unlock();
				}
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CGaugeLife::Draw(void)
{
	// 共通の描画処理
	CGauge::Draw();
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGaugeLife::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	SetLeftWidth(-GetWidth());                    // ゲージの左側オフセット
	SetGaugeRate((GetWidth() * 2.0f) / m_nLife);  // ゲージ一本分の割合
}

//=============================================================================
//    ゲージを減らす処理
//=============================================================================
void CGaugeLife::CutGauge(const int nCutValue)
{
	// 状態を設定
	m_State = STATE_CUT;

	// ゲージの幅を減らす
	float fWidth = GetWidth();  // 幅
	fWidth -= GetGaugeRate() * nCutValue;
	SetWidth(fWidth);

	// ポリゴンの幅を減らす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
		// 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		if (fWidth == GetLeftWidth())
		{// もし幅が0だったら
		    // 状態を設定
			m_State = STATE_NONE;

			// ポリゴンの幅を減らす
			pVtxBuff = m_pRedBill->GetVtxBuff();
			m_pRedBill->SetWidth(0.0f);
			if (pVtxBuff != NULL)
			{// 頂点バッファが生成されている
			    // 頂点情報の設定
				VERTEX_3D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(fWidth, GetHeight(), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f);

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    ゲージを増やす処理
//=============================================================================
void CGaugeLife::AddGauge(const int nAddValue)
{
	// ゲージの幅を増やす
	float fWidth = GetWidth();  // 幅
	fWidth += GetGaugeRate() * nAddValue;
	SetWidth(fWidth);

	// ポリゴンの幅を増やす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージの座標をずらす処理
//=============================================================================
void CGaugeLife::SetGaugePos(const D3DXVECTOR3 pos)
{
	SetPos(pos);

	// 赤いゲージが生成されている
	if (m_pRedBill != NULL)
	{
		m_pRedBill->SetPos(pos);
	}

	// ゲージのフレームが生成されている
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->SetPos(pos);
	}
}

//=============================================================================
//    状態設定処理
//=============================================================================
void CGaugeLife::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    体力設定処理
//=============================================================================
void CGaugeLife::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    状態取得処理
//=============================================================================
CGaugeLife::STATE CGaugeLife::GetState(void)
{
	return m_State;
}

//=============================================================================
//    体力取得処理
//=============================================================================
int CGaugeLife::GetLife(void)
{
	return m_nLife;
}


//*****************************************************************************
//    CGauge2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGauge2D::CGauge2D(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 各種値のクリア
	m_pFrameTexture = NULL;  // ゲージフレーム用のテクスチャ
	m_fGaugeRate = 0.0f;     // ゲージの割合
	m_fLeftWidth = 0.0f;     // ゲージの左側座標
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGauge2D::~CGauge2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGauge2D *CGauge2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nPriority)
{
	CGauge2D *pGauge2D = NULL;             // ゲージ(2D)クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGauge2D == NULL)
		{// メモリが空になっている
			pGauge2D = new CGauge2D(nPriority);
			if (pGauge2D != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGauge2D->SetPos(pos);         // ポリゴンの座標
				pGauge2D->SetCol(col);         // ポリゴンの色
				pGauge2D->SetRot(fRot);        // ポリゴンの向き
				pGauge2D->SetWidth(fWidth);    // ポリゴンの幅
				pGauge2D->SetHeight(fHeight);  // ポリゴンの高さ

				if (FAILED(pGauge2D->Init()))
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

	return pGauge2D;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGauge2D::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGauge2D::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGauge2D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CGauge2D::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGauge2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f) + GetPos();

	// テクスチャ値
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	m_fLeftWidth = -GetWidth();  // ゲージの左側オフセット
}

//=============================================================================
//    ゲージフレーム用のテクスチャ設定処理
//=============================================================================
void CGauge2D::SetFrameTexture(LPDIRECT3DTEXTURE9 pFrameTexture)
{
	m_pFrameTexture = pFrameTexture;
}

//=============================================================================
//    ゲージの割合設定処理
//=============================================================================
void CGauge2D::SetGaugeRate(const float fGaugeRate)
{
	m_fGaugeRate = fGaugeRate;
}

//=============================================================================
//    ゲージの左側の幅設定処理
//=============================================================================
void CGauge2D::SetLeftWidth(const float fLeftWidth)
{
	m_fLeftWidth = fLeftWidth;
}

//=============================================================================
//    ゲージフレーム用のテクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CGauge2D::GetFrameTexture(void)
{
	return m_pFrameTexture;
}

//=============================================================================
//    ゲージの割合取得処理
//=============================================================================
float CGauge2D::GetGaugeRate(void)
{
	return m_fGaugeRate;
}

//=============================================================================
//    ゲージの左側の幅取得処理
//=============================================================================
float CGauge2D::GetLeftWidth(void)
{
	return m_fLeftWidth;
}


//*****************************************************************************
//    CGaugeLife2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGaugeLife2D::CGaugeLife2D(int nPriority, OBJTYPE objType) : CGauge2D(nPriority, objType)
{
	// 各種値のクリア
	m_nCounter = 0;            // ゲージの減らしを管理するカウンター
	m_State = STATE_NONE;      // 状態
	m_nLife = 0;               // 体力
	m_pBlack2D = NULL;         // 体力ゲージの黒い部分用
	m_pRed2D = NULL;           // 体力ゲージが減った時に表示する赤いゲージ
	m_pFrame2D = NULL;         // 体力ゲージのフレーム
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGaugeLife2D::~CGaugeLife2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGaugeLife2D *CGaugeLife2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, LPDIRECT3DTEXTURE9 pFrameTexture, int nPriority)
{
	CGaugeLife2D *pGaugeLife2D = NULL;     // 体力ゲージクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGaugeLife2D == NULL)
		{// メモリが空になっている
			pGaugeLife2D = new CGaugeLife2D(nPriority);
			if (pGaugeLife2D != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGaugeLife2D->SetPos(pos);                     // ポリゴンの座標
				pGaugeLife2D->SetCol(col);                     // ポリゴンの色
				pGaugeLife2D->SetRot(fRot);                    // ポリゴンの向き
				pGaugeLife2D->SetWidth(fWidth);                // ポリゴンの幅
				pGaugeLife2D->SetHeight(fHeight);              // ポリゴンの高さ
				pGaugeLife2D->SetLife(nLife);                  // 体力
				pGaugeLife2D->SetFrameTexture(pFrameTexture);  // フレーム用のテクスチャ

				if (FAILED(pGaugeLife2D->Init()))
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

	return pGaugeLife2D;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGaugeLife2D::Init(void)
{
	// 初期化に失敗した
	if (FAILED(CGauge2D::Init()))
	{
		return E_FAIL;
	}

	// 後ろの黒背景を生成する
	if (m_pBlack2D == NULL)
	{
		m_pBlack2D = CGauge2D::Create(GetPos(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pBlack2D != NULL)
		{
			// ポリゴンの幅を減らす
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBlack2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// 頂点バッファが生成されている
			    // 頂点情報の設定
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
		}
	}

	// 後ろの赤いゲージを生成する
	if (m_pRed2D == NULL)
	{
		m_pRed2D = CGauge2D::Create(GetPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pRed2D != NULL)
		{
			// ポリゴンの幅を減らす
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRed2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// 頂点バッファが生成されている
			    // 頂点情報の設定
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
		}
	}

	// 体力ゲージのフレームを生成する
	if (m_pFrame2D == NULL)
	{
		m_pFrame2D = CGauge2D::Create(D3DXVECTOR3(1035.0f, 660.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250.0f, 75.0f, 0.0f, GetPriority());
		if (m_pFrame2D != NULL)
		{// 生成できた
			LPDIRECT3DTEXTURE9 pFrameTexture = GetFrameTexture();
			if (pFrameTexture != NULL)
			{// テクスチャへのポインタが取得できた
				m_pFrame2D->BindTexture(pFrameTexture);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGaugeLife2D::Uninit(void)
{
	// 黒いゲージの開放処理
	if (m_pBlack2D != NULL)
	{
		m_pBlack2D = NULL;
	}

	// 体力ゲージのフレーム開放処理
	if (m_pFrame2D != NULL)
	{
		m_pFrame2D = NULL;
	}

	// 赤いゲージの開放処理
	if (m_pRed2D != NULL)
	{
		m_pRed2D = NULL;
	}

	// 共通の描画処理
	CGauge2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGaugeLife2D::Update(void)
{
	if (m_State == STATE_CUT)
	{// ゲージを減らす状態だったら
		m_nCounter++;  // カウンターを増やす
		if (m_nCounter >= GAUGE_LIFE_CUTTIMING)
		{// ゲージを減らすタイミングになっている
		 // 後ろの赤いゲージを減らす
			if (m_pRed2D != NULL)
			{// 赤いゲージが生成されている
			    // 赤いゲージの幅を少しずつ減らす
				float fWidth = m_pRed2D->GetWidth();
				fWidth -= 2.0f;
				if (fWidth <= GetWidth())
				{// 赤いゲージの幅が体力ゲージを下回った
					fWidth = GetWidth();   // 幅を戻す
					m_State = STATE_NONE;  // 状態を戻す
					m_nCounter = 0;        // カウンターを戻す
				}
				m_pRed2D->SetWidth(fWidth);

				// ポリゴンの幅を減らす
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRed2D->GetVtxBuff();
				if (pVtxBuff != NULL)
				{// 頂点バッファが生成されている
				    // 頂点情報の設定
					VERTEX_2D *pVtx;

					// 頂点バッファをロックし,頂点データへのポインタを取得
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// 頂点座標
					pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
					pVtx[1].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f) + GetPos();
					pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
					pVtx[3].pos = D3DXVECTOR3(fWidth + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

					// 頂点バッファをアンロックする
					pVtxBuff->Unlock();
				}
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CGaugeLife2D::Draw(void)
{
	// 共通の描画処理
	CGauge2D::Draw();
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGaugeLife2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

	// テクスチャ値
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	SetLeftWidth(-GetWidth());                    // ゲージの左側オフセット
	SetGaugeRate((GetWidth() * 2.0f) / m_nLife);  // ゲージ一本分の割合
}

//=============================================================================
//    ゲージを減らす処理
//=============================================================================
void CGaugeLife2D::CutGauge(const int nCutValue)
{
	// 状態を設定
	m_State = STATE_CUT;

	// ゲージの幅を減らす
	float fWidth = GetWidth();  // 幅
	fWidth -= GetGaugeRate() * nCutValue;
	SetWidth(fWidth);

	// ポリゴンの幅を減らす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(),-GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		if (fWidth == GetLeftWidth())
		{// もし幅が0だったら
		    // 状態を設定
			m_State = STATE_NONE;

		    // ポリゴンの幅を減らす
			pVtxBuff = m_pRed2D->GetVtxBuff();
			m_pRed2D->SetWidth(0.0f);
			if (pVtxBuff != NULL)
			{// 頂点バッファが生成されている
			    // 頂点情報の設定
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(fWidth + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    ゲージを増やす処理
//=============================================================================
void CGaugeLife2D::AddGauge(const int nAddValue)
{
	// ゲージの幅を増やす
	float fWidth = GetWidth();  // 幅
	fWidth += GetGaugeRate() * nAddValue;
	SetWidth(fWidth);

	// ポリゴンの幅を増やす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    透明度を変更する処理
//=============================================================================
void CGaugeLife2D::ChangeAlpha(const float fAlpha)
{
	D3DXCOLOR col = GetCol();
	col.a = fAlpha;
	SetCol(col);

	// 赤いゲージの透明度も設定する
	if (m_pRed2D != NULL)
	{// 生成されている
		col = m_pRed2D->GetCol();
		col.a = fAlpha;
		m_pRed2D->SetCol(col);
	}

	// ゲージフレームの透明度も設定する
	if (m_pFrame2D != NULL)
	{// 生成されている
		col = m_pFrame2D->GetCol();
		col.a = fAlpha;
		m_pFrame2D->SetCol(col);
	}

	// 黒いポリゴンの透明度も設定する
	if (m_pBlack2D != NULL)
	{// 生成されている
		col = m_pBlack2D->GetCol();
		col.a = fAlpha;
		m_pBlack2D->SetCol(col);
	}
}

//=============================================================================
//    状態設定処理
//=============================================================================
void CGaugeLife2D::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    体力設定処理
//=============================================================================
void CGaugeLife2D::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    状態取得処理
//=============================================================================
CGaugeLife2D::STATE CGaugeLife2D::GetState(void)
{
	return m_State;
}

//=============================================================================
//    体力取得処理
//=============================================================================
int CGaugeLife2D::GetLife(void)
{
	return m_nLife;
}


//*****************************************************************************
//    CGaugeOrbitSlash2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGaugeOrbitSlash2D::CGaugeOrbitSlash2D(int nPriority, OBJTYPE objType) : CGauge2D(nPriority, objType)
{
	// 各種値のクリア
	m_nCounter = 0;            // ゲージの減らしを管理するカウンター
	m_State = STATE_NONE;      // 状態
	m_fOrbitSlash = 0.0f;      // 軌跡ゲージ量
	m_pBlack2D = NULL;         // 黒い背景用ポリゴン
	m_pFrame2D = NULL;         // 軌跡ゲージのフレーム
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGaugeOrbitSlash2D::~CGaugeOrbitSlash2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGaugeOrbitSlash2D *CGaugeOrbitSlash2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fOrbitSlash, LPDIRECT3DTEXTURE9 pFrameTexture, int nPriority)
{
	CGaugeOrbitSlash2D *pGaugeOrbirtSlash2D = NULL;  // 軌跡ゲージクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();             // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGaugeOrbirtSlash2D == NULL)
		{// メモリが空になっている
			pGaugeOrbirtSlash2D = new CGaugeOrbitSlash2D(nPriority);
			if (pGaugeOrbirtSlash2D != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGaugeOrbirtSlash2D->SetPos(pos);                     // ポリゴンの座標
				pGaugeOrbirtSlash2D->SetCol(col);                     // ポリゴンの色
				pGaugeOrbirtSlash2D->SetRot(fRot);                    // ポリゴンの向き
				pGaugeOrbirtSlash2D->SetWidth(fWidth);                // ポリゴンの幅
				pGaugeOrbirtSlash2D->SetHeight(fHeight);              // ポリゴンの高さ
				pGaugeOrbirtSlash2D->SetOrbitSlash(fOrbitSlash);      // 軌跡ゲージ量
				pGaugeOrbirtSlash2D->SetFrameTexture(pFrameTexture);  // フレーム用のテクスチャ

				if (FAILED(pGaugeOrbirtSlash2D->Init()))
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

	return pGaugeOrbirtSlash2D;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGaugeOrbitSlash2D::Init(void)
{
	// 初期化に失敗した
	if (FAILED(CGauge2D::Init()))
	{
		return E_FAIL;
	}

	// 後ろの黒背景を生成する
	if (m_pBlack2D == NULL)
	{
		m_pBlack2D = CGauge2D::Create(GetPos(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pBlack2D != NULL)
		{
			// ポリゴンの幅を減らす
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBlack2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// 頂点バッファが生成されている
			    // 頂点情報の設定
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
		}
	}

	// 軌跡ゲージのフレームを生成する
	if (m_pFrame2D == NULL)
	{
		m_pFrame2D = CGauge2D::Create(D3DXVECTOR3(245.0f, 678.0f, GetPos().z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250.0f, 50.0f, 0.0f, GetPriority());
		if(m_pFrame2D != NULL)
		{// 生成できた
			LPDIRECT3DTEXTURE9 pFrameTexture = GetFrameTexture();
			if (pFrameTexture != NULL)
			{// テクスチャへのポインタが取得できた
				m_pFrame2D->BindTexture(pFrameTexture);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGaugeOrbitSlash2D::Uninit(void)
{
	// 黒いゲージの開放処理
	if (m_pBlack2D != NULL)
	{
		m_pBlack2D = NULL;
	}

	// 軌跡ゲージのフレーム開放処理
	if (m_pFrame2D != NULL)
	{
		m_pFrame2D = NULL;
	}

	// 共通の描画処理
	CGauge2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGaugeOrbitSlash2D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CGaugeOrbitSlash2D::Draw(void)
{
	// 共通の描画処理
	CGauge2D::Draw();
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGaugeOrbitSlash2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

	// テクスチャ値
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	SetLeftWidth(-GetWidth());                          // ゲージの左側オフセット
	SetGaugeRate((GetWidth() * 2.0f) / m_fOrbitSlash);  // ゲージ一本分の割合
}

//=============================================================================
//    ゲージを減らす処理
//=============================================================================
void CGaugeOrbitSlash2D::CutGauge(const float fCutValue)
{
	// 状態を設定
	m_State = STATE_CUT;

	// ゲージの幅を減らす
	float fWidth = GetWidth();  // 幅
	fWidth -= GetGaugeRate() * fCutValue;
	SetWidth(fWidth);

	// ポリゴンの幅を減らす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージを増やす処理
//=============================================================================
void CGaugeOrbitSlash2D::AddGauge(const float fAddValue)
{
	// ゲージの幅を増やす
	float fWidth = GetWidth();  // 幅
	fWidth += GetGaugeRate() * fAddValue;
	SetWidth(fWidth);

	// ポリゴンの幅を増やす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージをなくす処理
//=============================================================================
void CGaugeOrbitSlash2D::DeleteGauge(void)
{
	// ゲージの幅をなくす
	float fWidth = GetWidth();  // 幅
	fWidth = GetLeftWidth();
	SetWidth(fWidth);

	// ポリゴンの幅をなくす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    透明度を変更する処理
//=============================================================================
void CGaugeOrbitSlash2D::ChangeAlpha(const float fAlpha)
{
	D3DXCOLOR col = GetCol();
	col.a = fAlpha;
	SetCol(col);

	// ゲージフレームの透明度も設定する
	if (m_pFrame2D != NULL)
	{// 生成されている
		col = m_pFrame2D->GetCol();
		col.a = fAlpha;
		m_pFrame2D->SetCol(col);
	}

	// 黒いポリゴンの透明度も設定する
	if (m_pBlack2D != NULL)
	{// 生成されている
		col = m_pBlack2D->GetCol();
		col.a = fAlpha;
		m_pBlack2D->SetCol(col);
	}
}

//=============================================================================
//    状態設定処理
//=============================================================================
void CGaugeOrbitSlash2D::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    軌跡ゲージ量設定処理
//=============================================================================
void CGaugeOrbitSlash2D::SetOrbitSlash(const float fOrbitSlash)
{
	m_fOrbitSlash = fOrbitSlash;
}

//=============================================================================
//    状態取得処理
//=============================================================================
CGaugeOrbitSlash2D::STATE CGaugeOrbitSlash2D::GetState(void)
{
	return m_State;
}

//=============================================================================
//    軌跡ゲージ量取得処理
//=============================================================================
float CGaugeOrbitSlash2D::GetOrbitSlash(void)
{
	return m_fOrbitSlash;
}


//*****************************************************************************
//    CGaugeSpecial2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGaugeSpecial2D::CGaugeSpecial2D(int nPriority, OBJTYPE objType) : CGauge2D(nPriority, objType)
{
	// 各種値のクリア
	m_nCounter = 0;            // ゲージの減らしを管理するカウンター
	m_State = STATE_NONE;      // 状態
	m_fSpecial = 0.0f;         // 必殺ゲージ量
	m_fMaxSpecial = 0.0f;      // 必殺ゲージの最大値
	m_pBlack2D = NULL;         // 黒い背景用ポリゴン
	m_pFrame2D = NULL;         // 必殺ゲージのフレーム
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGaugeSpecial2D::~CGaugeSpecial2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGaugeSpecial2D *CGaugeSpecial2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fMaxSpecial, float fSpecial, LPDIRECT3DTEXTURE9 pFrameTexture, int nPriority)
{
	CGaugeSpecial2D *pGaugeSpecial2D = NULL;  // 必殺技ゲージクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();      // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGaugeSpecial2D == NULL)
		{// メモリが空になっている
			pGaugeSpecial2D = new CGaugeSpecial2D(nPriority);
			if (pGaugeSpecial2D != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGaugeSpecial2D->SetPos(pos);                     // ポリゴンの座標
				pGaugeSpecial2D->SetCol(col);                     // ポリゴンの色
				pGaugeSpecial2D->SetRot(fRot);                    // ポリゴンの向き
				pGaugeSpecial2D->SetWidth(fWidth);                // ポリゴンの幅
				pGaugeSpecial2D->SetHeight(fHeight);              // ポリゴンの高さ
				pGaugeSpecial2D->SetSpecial(fSpecial);            // 必殺技ゲージ量
				pGaugeSpecial2D->SetMaxSpecial(fMaxSpecial);      // 必殺ゲージの最大値
				pGaugeSpecial2D->SetFrameTexture(pFrameTexture);  // フレーム用のテクスチャ

				if (FAILED(pGaugeSpecial2D->Init()))
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

	return pGaugeSpecial2D;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGaugeSpecial2D::Init(void)
{
	// 初期化に失敗した
	if (FAILED(CGauge2D::Init()))
	{
		return E_FAIL;
	}

	// 後ろの黒背景を生成する
	if (m_pBlack2D == NULL)
	{
		m_pBlack2D = CGauge2D::Create(GetPos(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pBlack2D != NULL)
		{
			// ポリゴンの幅を減らす
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBlack2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// 頂点バッファが生成されている
			    // 頂点情報の設定
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
		}
	}

	// 必殺ゲージのフレームを生成する
	if (m_pFrame2D == NULL)
	{
		m_pFrame2D = CGauge2D::Create(D3DXVECTOR3(174.0f,610.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180.0f, 50.0f, 0.0f, GetPriority());
		if (m_pFrame2D != NULL)
		{// 生成できた
			LPDIRECT3DTEXTURE9 pFrameTexture = GetFrameTexture();
			if (pFrameTexture != NULL)
			{// テクスチャへのポインタが取得できた
				m_pFrame2D->BindTexture(pFrameTexture);
			}
		}
	}

	// ゲージの幅をリセット
	DeleteGauge();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGaugeSpecial2D::Uninit(void)
{
	// 黒いゲージの開放処理
	if (m_pBlack2D != NULL)
	{
		m_pBlack2D = NULL;
	}

	// 必殺ゲージのフレーム開放処理
	if (m_pFrame2D != NULL)
	{
		m_pFrame2D = NULL;
	}

	// 共通の描画処理
	CGauge2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGaugeSpecial2D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CGaugeSpecial2D::Draw(void)
{
	// 共通の描画処理
	CGauge2D::Draw();
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGaugeSpecial2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(-GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

	// テクスチャ値
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	SetLeftWidth(-GetWidth());                          // ゲージの左側オフセット
	SetGaugeRate((GetWidth() * 2.0f) / m_fMaxSpecial);  // ゲージ一本分の割合
}

//=============================================================================
//    ゲージを減らす処理
//=============================================================================
void CGaugeSpecial2D::CutGauge(const float fCutValue)
{
	// 状態を設定
	m_State = STATE_CUT;

	// ゲージの幅を減らす
	float fWidth = GetWidth();  // 幅
	fWidth -= GetGaugeRate() * fCutValue;
	SetWidth(fWidth);

	// ポリゴンの幅を減らす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージを増やす処理
//=============================================================================
void CGaugeSpecial2D::AddGauge(const float fAddValue)
{
	// ゲージの幅を増やす
	float fWidth = GetWidth();  // 幅
	fWidth += GetGaugeRate() * fAddValue;
	SetWidth(fWidth);

	// ポリゴンの幅を増やす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージをなくす処理
//=============================================================================
void CGaugeSpecial2D::DeleteGauge(void)
{
	// ゲージの幅をなくす
	float fWidth = GetWidth();  // 幅
	fWidth = GetLeftWidth();
	SetWidth(fWidth);

	// ポリゴンの幅をなくす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージを最大にする処理
//=============================================================================
void CGaugeSpecial2D::MaxGauge(void)
{
	// ゲージの幅を増やす
	float fWidth = GetWidth();  // 幅
	fWidth = (m_fMaxSpecial * GetGaugeRate()) / 2;
	SetWidth(fWidth);

	// ポリゴンの幅を増やす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    透明度を変更する処理
//=============================================================================
void CGaugeSpecial2D::ChangeAlpha(const float fAlpha)
{
	D3DXCOLOR col = GetCol();
	col.a = fAlpha;
	SetCol(col);

	// ゲージフレームの透明度も設定する
	if (m_pFrame2D != NULL)
	{// 生成されている
		col = m_pFrame2D->GetCol();
		col.a = fAlpha;
		m_pFrame2D->SetCol(col);
	}

	// 黒いポリゴンの透明度も設定する
	if (m_pBlack2D != NULL)
	{// 生成されている
		col = m_pBlack2D->GetCol();
		col.a = fAlpha;
		m_pBlack2D->SetCol(col);
	}
}

//=============================================================================
//    状態設定処理
//=============================================================================
void CGaugeSpecial2D::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    必殺ゲージ量設定処理
//=============================================================================
void CGaugeSpecial2D::SetSpecial(const float fSpecial)
{
	m_fSpecial = fSpecial;
}

//=============================================================================
//    必殺ゲージの最大値設定処理
//=============================================================================
void CGaugeSpecial2D::SetMaxSpecial(const float fMaxSpecial)
{
	m_fMaxSpecial = fMaxSpecial;
}

//=============================================================================
//    状態取得処理
//=============================================================================
CGaugeSpecial2D::STATE CGaugeSpecial2D::GetState(void)
{
	return m_State;
}

//=============================================================================
//    必殺ゲージ量取得処理
//=============================================================================
float CGaugeSpecial2D::GetSpecial(void)
{
	return m_fSpecial;
}

//=============================================================================
//    必殺ゲージの最大値取得処理
//=============================================================================
float CGaugeSpecial2D::GetMaxSpecial(void)
{
	return m_fMaxSpecial;
}