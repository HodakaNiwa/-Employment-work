//*****************************************************************************
//
//     パーティクルの処理[particle.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "effectManager.h"
#include "player.h"
#include "score.h"
#include "game.h"
#include "ui.h"
#include "map.h"
#include "sound.h"
#include "functionlib.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define SCOREPAR_OFFSETPOSYMAX   (40.0f)     // スコアパーティクルのオフセット高さの限界値
#define SCOREPAR_SEARCH_RADIAN   (240.0f)    // スコアパーティクルが反応する範囲
#define SCOREPAR_SEARCHSTART     (100)       // スコアパーティクルが反応を開始するタイミング
#define SCOREPAR_DESTROYTIMING   (240)       // スコアパーティクルが消えるまでの秒数
#define SCOREPAR_GRAVITY         (0.8f)      // スコアパーティクルにかける重力量
#define SCOREPAR_INERTIA         (0.04f)     // スコアパーティクルにかける慣性
#define SCOREPAR_CHANGELANGTH    (0.15f)     // スコアパーティクルをどれくらい小さくするか
#define SCOREPAR_ROTATIONSPEED   (0.1f)      // オフセットを回転させるスピード
#define SCOREPAR_RADIANSPEED     (3.3f)      // 半径の変化量

//*****************************************************************************
//    CParDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParData::CParData()
{
	m_Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 生成する範囲
	m_fRangeMag = 0.0f;                              // 生成する範囲の倍率
	m_fMaxRot = 0.0f;                                // 生成時の向き(最大値)
	m_fMinRot = 0.0f;                                // 生成時の向き(最小値)
	m_MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量の最大値
	m_MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量の最小値
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量の変化量
	m_InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 生成時の色
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // フレーム毎の色の変化量
	m_fMaxWidth = 0.0f;                              // 幅の最大値
	m_fMinWidth = 0.0f;                              // 幅の最小値
	m_fMaxHeight = 0.0f;                             // 高さの最大値
	m_fMinHeight = 0.0f;                             // 高さの最小値
	m_fSpread = 0.0f;                                // 大きさの変化量
	m_nMaxLife = 0;                                  // 寿命の最大値
	m_nMinLife = 0;                                  // 寿命の最小値
	m_fGravity = 0.0f;                               // 毎フレームかける重力
	m_fBouncing = 0.0f;                              // バウンド量
	m_nRotPattern = 0;                               // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
	m_fRotSpeed = 0.0f;                              // 回転するスピード
	m_bCollision = false;                            // 当たり判定をするかどうか
	m_bDrawAddtive = true;                           // 加算合成するかしないか[ 0:なし 1:あり ]
	m_bLighting = false;                             // ライティングするかしないか[ 0:なし 1:あり ]
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParData::~CParData()
{

}

//*****************************************************************************
//    CParticleの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParticle::CParticle(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // どれくらい色が変化するか
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 移動量
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量の変化量
	m_fGravity = 0.0f;                               // 重力
	m_fSpread = 0.0f;                                // どれくらい変形するか
	m_fChangeRot = 0.0f;                             // どれくらい回転するか
	m_fBouncing = 0.0f;                              // 地面についた際にバウンドする値
	m_nLife = 0;                                     // 寿命
	m_nRotPattern = 0;                               // 回転の種類
	m_bCollision = false;                            // バウンドするかどうか
	m_bDrawAddtive = false;                          // 加算合成で描画するかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
	float fChangeRot, float fBouncing, int nLife, int nRotPattern,
	bool bCol, bool bDrawAddtive, bool bLighting, int nPriority)
{
	CParticle *pParticle = NULL;  // パーティクルクラス型のポインタ
	if (pParticle == NULL)
	{// メモリが空になっている
		pParticle = new CParticle(nPriority);
		if (pParticle != NULL)
		{// インスタンスを生成できた
			if (FAILED(pParticle->Init(pos, move, ChangeMove, col, fWidth, fHeight, fRot, ChangeCol, fGravity, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCol, bDrawAddtive, bLighting)))
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

	return pParticle;   // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
	float fChangeRot, float fBouncing, int nLife, int nRotPattern,
	bool bCol, bool bDrawAddtive, bool bLighting)
{
	// 各種値の設定
	SetPos(pos);                       // 座標
	SetCol(col);                       // 色
	SetWidth(fWidth);                  // 幅
	SetHeight(fHeight);                // 高さ
	SetRot(fRot);                      // 向き
	SetLighting(bLighting);            // ライティングするかしないか
	m_Move = move;                     // 移動量
	m_ChangeMove = ChangeMove;         // 移動量の変化量
	m_ChangeCol = ChangeCol;           // 色
	m_fGravity = fGravity;             // 半径
	m_fSpread = fSpread;               // 半径の変化量
	m_fChangeRot = fChangeRot;         // 向きの変化量
	m_fBouncing = fBouncing;           // バウンド量
	m_nLife = nLife;                   // 寿命
	m_nRotPattern = nRotPattern;       // 回転のパターン
	m_bCollision = bCol;               // バウンドするかしないか
	m_bDrawAddtive = bDrawAddtive;     // 加算合成で描画するかしないか

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
void CParticle::Uninit(void)
{
	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CParticle::Update(void)
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
		m_Move.y -= m_fGravity;		 // 重力を加える

		// 移動量を変化させる
		m_Move.x += (m_Move.x * m_ChangeMove.x);
		m_Move.y += (m_Move.y * m_ChangeMove.y);
		m_Move.z += (m_Move.z * m_ChangeMove.z);

		// 当たり判定を取るかチェック
		if (pos.y <= 0 && m_bCollision == true)
		{// バウンド
			m_Move.y += (m_fBouncing - pos.y) * 2.0f;
			m_fBouncing = pos.y;   // 値を記憶
		}

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
		ChangeVertex(pos, col, fRot, fRadius);
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CParticle::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // 透明度が0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

			if (m_bDrawAddtive == true)
			{// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// 共通の描画処理
			CSceneBillboard::Draw();

			// アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Zテストを元の値に戻す
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CParticle::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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

	// オフセットの長さを求める
	float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
	SetLength(fLength);

	// オフセットの角度を求める
	float fAngle = atan2f(GetWidth(), GetHeight());
	SetAngle(fAngle);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	}

	// 法線
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

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

	// 頂点バッファの設定
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    頂点情報設定処理
//=============================================================================
void CParticle::ChangeVertex(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fLength)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pVtxBuff != NULL)
	{// 頂点バッファが取得できている
		// 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    移動量設定処理
//=============================================================================
void CParticle::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    移動量の変化量設定処理
//=============================================================================
void CParticle::SetChangeMove(const D3DXVECTOR3 ChangeMove)
{
	m_ChangeMove = ChangeMove;
}

//=============================================================================
//    色の変化量設定処理
//=============================================================================
void CParticle::SetChangeCol(const D3DXCOLOR ChangeCol)
{
	m_ChangeCol = ChangeCol;
}

//=============================================================================
//    寿命設定処理
//=============================================================================
void CParticle::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    回転パターン設定処理
//=============================================================================
void CParticle::SetRotPattern(const int nRotPattern)
{
	m_nRotPattern = nRotPattern;
}

//=============================================================================
//    重力量設定処理
//=============================================================================
void CParticle::SetGravity(const float fGravity)
{
	m_fGravity = fGravity;
}

//=============================================================================
//    大きさの変化量設定処理
//=============================================================================
void CParticle::SetSpread(const float fSpread)
{
	m_fSpread = fSpread;
}

//=============================================================================
//    向きの変化量設定処理
//=============================================================================
void CParticle::SetChangeRot(const float fChangeRot)
{
	m_fChangeRot = fChangeRot;
}

//=============================================================================
//    バウンド量設定処理
//=============================================================================
void CParticle::SetBouncing(const float fBouncing)
{
	m_fBouncing = fBouncing;
}

//=============================================================================
//    当たり判定を取るか設定処理
//=============================================================================
void CParticle::SetCollision(const bool bCollision)
{
	m_bCollision = bCollision;
}

//=============================================================================
//    加算合成で描画するかしないか設定処理
//=============================================================================
void CParticle::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    移動量取得処理
//=============================================================================
D3DXVECTOR3 CParticle::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    移動量の変化量取得処理
//=============================================================================
D3DXVECTOR3 CParticle::GetChangeMove(void)
{
	return m_ChangeMove;
}

//=============================================================================
//    色の変化量取得処理
//=============================================================================
D3DXCOLOR CParticle::GetChangeCol(void)
{
	return m_ChangeCol;
}

//=============================================================================
//    寿命取得処理
//=============================================================================
int CParticle::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    回転パターン取得処理
//=============================================================================
int CParticle::GetRotPattern(void)
{
	return m_nRotPattern;
}

//=============================================================================
//    重力量取得処理
//=============================================================================
float CParticle::GetGravity(void)
{
	return m_fGravity;
}

//=============================================================================
//    大きさの変化量取得処理
//=============================================================================
float CParticle::GetSpread(void)
{
	return m_fSpread;
}

//=============================================================================
//    向きの変化量取得処理
//=============================================================================
float CParticle::GetChangeRot(void)
{
	return m_fChangeRot;
}

//=============================================================================
//    バウンド量取得処理
//=============================================================================
float CParticle::GetBouncing(void)
{
	return m_fBouncing;
}

//=============================================================================
//    当たり判定を取るか取得処理
//=============================================================================
bool CParticle::GetCollision(void)
{
	return m_bCollision;
}

//=============================================================================
//    加算合成で描画するかしないか取得処理
//=============================================================================
bool CParticle::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}


//*****************************************************************************
//    CScoreParの処理
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CScorePar::CScorePar(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_pLandScene = NULL;                  // 今乗っているオブジェクトへのポインタ
	m_State = STATE_NONE;                 // 状態
	m_nCounter = 0;                       // 状態を管理するカウンター
	m_nAddScore = 0;                      // 取得時に加算するスコア量
	m_Move = D3DXVECTOR3(0.0f,0.0f,0.0f); // 移動量
	m_pPlayer = NULL;                     // プレイヤークラスへのポインタ
	m_fRotation = 0.0f;                   // 回転の向き
	m_fTargetLength = 0.0f;               // 対象のオブジェクトとの距離(半径)
	m_fDestPosY = 0.0f;                   // 目的地の高さ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScorePar::~CScorePar()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScorePar *CScorePar::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, D3DXVECTOR3 move, CPlayer *pPlayer, int nAddScore, int nPriority)
{
	CScorePar *pScorePar = NULL;           // スコアパーティクルクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pScorePar == NULL)
		{// メモリが空になっている
			pScorePar = new CScorePar(nPriority);
			if (pScorePar != NULL)
			{// インスタンスを生成できた
				if (FAILED(pScorePar->Init(pos, col, fRot, fWidth, fHeight, move, pPlayer, nAddScore)))
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

	return pScorePar;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScorePar::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, D3DXVECTOR3 move, CPlayer *pPlayer, int nAddScore)
{
	// 各種値の設定
	SetPos(pos);             // 座標
	SetCol(col);             // 色
	SetRot(fRot);            // ポリゴンの向き
	SetWidth(fWidth);        // ポリゴンの幅
	SetHeight(fHeight);      // ポリゴンの高さ
	m_Move = move;           // 移動量
	m_pPlayer = pPlayer;     // プレイヤークラスへのポインタ
	m_nAddScore = nAddScore; // 取得時に加算するスコア量

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
void CScorePar::Uninit(void)
{
	// 乗っている地面へのポインタを空にする
	m_pLandScene = NULL;

	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScorePar::Update(void)
{
	m_nCounter++;

	if (m_State == STATE_NONE)
	{// 何もしない状態だったら
		// 各種値の取得
		D3DXVECTOR3 pos = GetPos();

		// 移動させる
		pos += m_Move;

		// 移動量を慣性によって減衰させる
		m_Move.x += (0.0f - m_Move.x) * SCOREPAR_INERTIA;
		m_Move.z += (0.0f - m_Move.z) * SCOREPAR_INERTIA;

		// 値の設定
		SetPos(pos);

		if (m_nCounter >= 20)
		{// 出現してからある程度たった
			if (m_pLandScene == NULL)
			{// 乗っている地面が確定していない
				Collision();
				// 重力を加える
				m_Move.y -= SCOREPAR_GRAVITY;
			}
			else
			{// 乗っている地面が確定している
				CMeshField *pField = NULL;
				float fFieldHeight = 0.0f;
				bool bFieldLand = false;
				if (m_pLandScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
				{// 地面クラスへのポインタだったら
					if (bFieldLand == false)
					{// 地面にまだ乗っていない
						pField = (CMeshField*)m_pLandScene;
						if (pField != NULL)
						{// 地面が取得できた
							fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, NULL);
							if (pos.y - GetHeight() <= fFieldHeight)
							{// 地面の高さより低くなった
								m_Move.y = 0.0f;
								pos.y = fFieldHeight + GetHeight();
								SetPos(pos);
							}
						}
					}
				}
			}
		}

		if (m_nCounter >= SCOREPAR_SEARCHSTART)
		{// 出現してからある程度たった
			// 目的対象の範囲チェック
			SearchTarget();
		}

		if (m_nCounter >= SCOREPAR_DESTROYTIMING)
		{// スコアパーティクルが消える時間になった
			Uninit();
		}
	}
	else if (m_State == STATE_TRACKING)
	{// 目的対象についていく状態ならば
		TrackingTarget();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CScorePar::Draw(void)
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

			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ライティングの設定
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CSceneBillboard::Draw();

			// ライティングを戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Zテストを元の値に戻す
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
}

//=============================================================================
//    大きさを変更する処理
//=============================================================================
void CScorePar::ChangeLength(void)
{
	// 大きさの取得
	float fLength = GetLength();

	// 大きさを小さくする
	fLength -= SCOREPAR_CHANGELANGTH;
	if (fLength <= 1.5f)
	{
		fLength = 1.5f;
	}
	SetLength(fLength);

	// ポリゴンの大きさを設定
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点の回転を考慮して座標を計算
		float XPos[4];
		float YPos[4];
		XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * fLength, cosf(GetAngle()) * fLength);
		YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * fLength, cosf(GetAngle()) * fLength);
		XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * fLength, cosf(GetAngle()) * fLength);
		YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * fLength, cosf(GetAngle()) * fLength);
		XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * fLength, -cosf(GetAngle()) * fLength);
		YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * fLength, -cosf(GetAngle()) * fLength);
		XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * fLength, -cosf(GetAngle()) * fLength);
		YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * fLength, -cosf(GetAngle()) * fLength);

		// 頂点座標
		for (int nCntVer = 0; nCntVer < 4; nCntVer++)
		{// 頂点の数だけ繰り返し
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CScorePar::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CMeshField *pField = NULL;           // 地面クラスへのポインタ
	bool bFieldLand = false;             // 地面に乗っているかどうか
	float fFieldHeight = 0.0f;           // 地面の高さ

	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < FIELD_PRIORITY + 1; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// 地面クラスへのポインタだったら
				if (bFieldLand != true)
				{// まだ地面に乗っていない
					pField = (CMeshField*)pScene;
					if (pField != NULL)
					{// 地面が取得できた
						fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, NULL);
						if (bFieldLand == true)
						{// 地面の中にいる
							if (pos.y - GetHeight() <= fFieldHeight)
							{// 地面の高さより低くなった
								pos.y = fFieldHeight + GetHeight();
								m_pLandScene = pField;
								m_Move.y = 0.0f;
							}
						}
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	// 値の設定
	SetPos(pos);
}

//=============================================================================
//    目的のターゲットが範囲内に入ったかどうかチェックする処理
//=============================================================================
void CScorePar::SearchTarget(void)
{
	float fDistance = 0.0f;   // 距離
	D3DXVECTOR3 PlayerPos;

	if (m_pPlayer != NULL)
	{// ポインタが取得できている
		PlayerPos = m_pPlayer->GetPos();

		// 距離を計算
		fDistance = sqrtf((PlayerPos.x - GetPos().x) * (PlayerPos.x - GetPos().x) + (PlayerPos.y - GetPos().y) * (PlayerPos.y - GetPos().y) + (PlayerPos.z - GetPos().z) * (PlayerPos.z - GetPos().z));

		if (fDistance <= SCOREPAR_SEARCH_RADIAN)
		{// 反応する範囲より距離が短い
			// 状態を変える
			m_State = STATE_TRACKING;

			// 角度を計算しておく
			m_fRotation = atan2f((PlayerPos.x - GetPos().x), (PlayerPos.z - GetPos().z));

			// 距離を保存
			m_fTargetLength = fDistance;
		}
	}
}

//=============================================================================
//    目的の位置についていく処理
//=============================================================================
void CScorePar::TrackingTarget(void)
{
	// 大きさを変更させる
	ChangeLength();

	D3DXVECTOR3 OffsetPos = D3DXVECTOR3(0.0f, m_fDestPosY, 0.0f);   // 目的の位置からのオフセット位置

	// 目的のオフセットの高さを上げる
	m_fDestPosY += 1.5f;
	if (m_fDestPosY >= SCOREPAR_OFFSETPOSYMAX)
	{// ある程度の高さまで到達した
		m_fDestPosY = SCOREPAR_OFFSETPOSYMAX;
	}

	// 角度を回転させる
	m_fRotation += SCOREPAR_ROTATIONSPEED;
	if(m_fRotation > D3DX_PI)
	{// 円周率を超えた
		m_fRotation -= D3DX_PI * 2.0f;
	}

	// 半径を減らす
	m_fTargetLength -= SCOREPAR_RADIANSPEED;

	if (m_fTargetLength <= 0.0f)
	{// 半径が0以下になった
	    // スコアを加算する
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲームモード中だったら
			CUI *pUi = CManager::GetGame()->GetUI();
			if (pUi != NULL)
			{// UIが生成されている
				CScore *pScore = pUi->GetScore();
				if (pScore != NULL)
				{// スコアクラスが取得できた
					pScore->AddScore(m_nAddScore);
				}
			}
		}

		// SEを再生
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GET_SCOREPAR);

		// 自身を消す
		Uninit();
	}
	else
	{// 半径がまだ0以上である
	    // オフセット位置を計算
		OffsetPos.x = sinf(m_fRotation) * m_fTargetLength;
		OffsetPos.z = cosf(m_fRotation) * m_fTargetLength;

		// 位置を補正する
		if (m_pPlayer != NULL)
		{
			D3DXVec3TransformCoord(&OffsetPos, &OffsetPos, &m_pPlayer->GetMtxWorld());
		}

		// 位置の設定
		SetPos(OffsetPos);
	}
}