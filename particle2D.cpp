//*****************************************************************************
//
//     パーティクル(2D)の処理[particle2D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "particle2D.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CParticle2D::CParticle2D(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 各種値のクリア
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // どれくらい色が変化するか
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 移動量
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量の変化量
	m_fSpread = 0.0f;                                // どれくらい変形するか
	m_fChangeRot = 0.0f;                             // どれくらい回転するか
	m_nLife = 0;                                     // 寿命
	m_nRotPattern = 0;                               // 回転の種類
	m_bDrawAddtive = false;                          // 加算合成で描画するかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParticle2D::~CParticle2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CParticle2D *CParticle2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
	float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive, int nPriority)
{
	CParticle2D *pParticle2D = NULL;  // パーティクル(2D)クラス型のポインタ
	if (pParticle2D == NULL)
	{// メモリが空になっている
		pParticle2D = new CParticle2D(nPriority);
		if (pParticle2D != NULL)
		{// インスタンスを生成できた
			if (FAILED(pParticle2D->Init(pos, move, ChangeMove, col, fWidth, fHeight, fRot, ChangeCol, fSpread, fChangeRot, nLife, nRotPattern, bDrawAddtive)))
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

	return pParticle2D;   // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CParticle2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
	float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive)
{
	// 各種値の設定
	SetPos(pos);                       // 座標
	SetCol(col);                       // 色
	SetWidth(fWidth);                  // 幅
	SetHeight(fHeight);                // 高さ
	SetRot(fRot);                      // 向き
	m_Move = move;                     // 移動量
	m_ChangeMove = ChangeMove;         // 移動量の変化量
	m_ChangeCol = ChangeCol;           // 色
	m_fSpread = fSpread;               // 半径の変化量
	m_fChangeRot = fChangeRot;         // 向きの変化量
	m_nLife = nLife;                   // 寿命
	m_nRotPattern = nRotPattern;       // 回転のパターン
	m_bDrawAddtive = bDrawAddtive;     // 加算合成で描画するかしないか

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
void CParticle2D::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CParticle2D::Update(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();  // 座標
	D3DXCOLOR col = GetCol();    // 色
	float fRot = GetRot();       // 向き
	float fRadius = GetLength(); // 半径

	// 各種値の更新
	pos += m_Move;           // 位置の更新
	col += m_ChangeCol;      // 色の更新
	m_nLife--;               // 寿命を減らす
	fRadius += m_fSpread;    // 大きさの更新

	if (m_nLife <= 0 || fRadius < 0.0f || col.a <= 0.0f)
	{// 寿命が0以下になった
		Uninit();	// 終了処理
	}
	else
	{// まだ表示時間がある
		// 移動量を変化させる
		m_Move.x += (m_Move.x * m_ChangeMove.x);
		m_Move.y += (m_Move.y * m_ChangeMove.y);
		m_Move.z += (m_Move.z * m_ChangeMove.z);

		// 向きを回転させる
		if (m_nRotPattern == 0)
		{// 時計回り
			fRot -= m_fChangeRot;
		}
		else if (m_nRotPattern == 1)
		{// 反時計回り
			fRot += m_fChangeRot;
		}
		else if (m_nRotPattern == 2)
		{// ランダム
			if (rand() % 2 == 0)
			{// 時計回り
				fRot += m_fChangeRot;
			}
			else
			{// 反時計回り
				fRot -= m_fChangeRot;
			}
		}
		if (fRot > D3DX_PI)
		{// 向きが円周率を超えた
			fRot -= D3DX_PI * 2.0f;
		}
		if (fRot > D3DX_PI)
		{// 向きが円周率を超えた
			fRot += D3DX_PI * 2.0f;
		}

		// 各種値の設定
		SetPos(pos);          // 座標
		SetCol(col);          // 色
		SetRot(fRot);         // 向き
		SetLength(fRadius);   // 大きさ
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CParticle2D::Draw(void)
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
			CScene2D::Draw();

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
//    移動量設定処理
//=============================================================================
void CParticle2D::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    移動量の変化量設定処理
//=============================================================================
void CParticle2D::SetChangeMove(const D3DXVECTOR3 ChangeMove)
{
	m_ChangeMove = ChangeMove;
}

//=============================================================================
//    色の変化量設定処理
//=============================================================================
void CParticle2D::SetChangeCol(const D3DXCOLOR ChangeCol)
{
	m_ChangeCol = ChangeCol;
}

//=============================================================================
//    寿命設定処理
//=============================================================================
void CParticle2D::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    回転パターン設定処理
//=============================================================================
void CParticle2D::SetRotPattern(const int nRotPattern)
{
	m_nRotPattern = nRotPattern;
}

//=============================================================================
//    大きさの変化量設定処理
//=============================================================================
void CParticle2D::SetSpread(const float fSpread)
{
	m_fSpread = fSpread;
}

//=============================================================================
//    向きの変化量設定処理
//=============================================================================
void CParticle2D::SetChangeRot(const float fChangeRot)
{
	m_fChangeRot = fChangeRot;
}

//=============================================================================
//    加算合成で描画するかしないか設定処理
//=============================================================================
void CParticle2D::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    移動量取得処理
//=============================================================================
D3DXVECTOR3 CParticle2D::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    移動量の変化量取得処理
//=============================================================================
D3DXVECTOR3 CParticle2D::GetChangeMove(void)
{
	return m_ChangeMove;
}

//=============================================================================
//    色の変化量取得処理
//=============================================================================
D3DXCOLOR CParticle2D::GetChangeCol(void)
{
	return m_ChangeCol;
}

//=============================================================================
//    寿命取得処理
//=============================================================================
int CParticle2D::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    回転パターン取得処理
//=============================================================================
int CParticle2D::GetRotPattern(void)
{
	return m_nRotPattern;
}

//=============================================================================
//    大きさの変化量取得処理
//=============================================================================
float CParticle2D::GetSpread(void)
{
	return m_fSpread;
}

//=============================================================================
//    向きの変化量取得処理
//=============================================================================
float CParticle2D::GetChangeRot(void)
{
	return m_fChangeRot;
}

//=============================================================================
//    加算合成で描画するかしないか取得処理
//=============================================================================
bool CParticle2D::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}