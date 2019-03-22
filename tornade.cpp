//*****************************************************************************
//
//     竜巻の処理[tornade.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "tornade.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"
#include "model.h"
#include "emitter.h"
#include "effectManager.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define TORNADE_MOVE            (13.0f)           // 竜巻の移動量
#define TORNADE_DIFF_ANGLE      (0.2f)            // 竜巻の向きを補正できる倍率
#define TORNADE_RIVISION_ANGLE  (D3DX_PI * 0.2f)  // 竜巻がどれくらい向きを補正できるか
#define TORNADE_MAX_LIFE        (70)              // 竜巻の寿命最大値
#define TORNADE_MIN_LIFE        (50)              // 竜巻の寿命最小値
#define TORNADE_COLLISION_RANGE (30.0f)           // 竜巻の当たり判定を取れる範囲
#define TORNADE_DAMAGE          (3)               // 竜巻のダメージ
#define TORNADE_UP              (15.0f)           // 竜巻のエフェクトをどれだけ上げるか

//=============================================================================
//    コンストラクタ
//=============================================================================
CTornade::CTornade(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_nEffectCounter = 0;                          // エフェクトを出すタイミングを管理するカウンター
	m_pEffectManager = NULL;                       // エフェクト管轄クラスへのポインタ
	m_State = STATE_HORMING;                       // 状態
	m_nLife = 0;                                   // 寿命(追尾が終了したら減らし始める)
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);           // 座標
	m_fAngle = 0.0f;                               // 現在の向き
	m_fDestAngle = 0.0f;                           // 目的の向き
	m_fDiffAngle = 0.0f;                           // 目的の向きと現在の向きの差分
	m_fRivisionAngle = 0.0f;                       // どれだけ向きを補正したかどうか

	for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
	{// エフェクトを生成する数だけ繰り返し
		m_pTornadeEmitter[nCntEffect] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTornade::~CTornade()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTornade *CTornade::Create(D3DXVECTOR3 pos, CEffectManager *pEffectManager, float fDestAngle, int nPriority)
{
	CTornade *pTornade = NULL;             // 竜巻クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pTornade == NULL)
		{// メモリが空になっている
			pTornade = new CTornade(nPriority);
			if (pTornade != NULL)
			{// メモリを確保できた
			    // 各種値の設定
				pTornade->SetPos(pos);                       // 竜巻の座標
				pTornade->SetAngle(fDestAngle);              // 竜巻の向き
				pTornade->SetEffectManager(pEffectManager);  // 竜巻のエフェクト管轄クラスへのポインタ

				if (FAILED(pTornade->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// メモリを確保できなかった
				return NULL;
			}
		}
		else
		{// メモリが空ではない
			return NULL;
		}
	}
	else
	{// これ以上生成できない
		return NULL;
	}

	return pTornade;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTornade::Init(void)
{
	// 竜巻のエフェクトを生成する
	if (m_pEffectManager != NULL)
	{// エフェクト管轄クラスへのポインタが取得できている
		D3DXVECTOR3 EffectPos = D3DXVECTOR3(m_Pos.x, m_Pos.y + 10.0f, m_Pos.z);
		for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
		{// エフェクトを生成する数だけ繰り返し
			m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33 + nCntEffect);
			EffectPos.y += TORNADE_UP;
		}
	}

	// 寿命を計算で出力
	m_nLife = rand() % (TORNADE_MAX_LIFE - TORNADE_MIN_LIFE) + TORNADE_MIN_LIFE;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTornade::Uninit(void)
{
	// エミッタの開放
	for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
	{// エフェクトを生成する数だけ繰り返し
		if (m_pTornadeEmitter[nCntEffect] != NULL)
		{// エミッタが生成されている
			m_pTornadeEmitter[nCntEffect]->Uninit();
			m_pTornadeEmitter[nCntEffect] = NULL;
		}
	}

	// 竜巻の消去処理
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTornade::Update(void)
{
	m_nEffectCounter++;
	if (m_nEffectCounter % 1 == 0)
	{// エフェクトを出すタイミングになった
	    // 竜巻のエフェクトを生成する
		if (m_pEffectManager != NULL)
		{// エフェクト管轄クラスへのポインタが取得できている
			D3DXVECTOR3 EffectPos = D3DXVECTOR3(m_Pos.x, m_Pos.y + 10.0f, m_Pos.z);
			for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
			{// エフェクトを生成する数だけ繰り返し
				m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33 + nCntEffect);
				EffectPos.y += TORNADE_UP;
			}
		}
	}

	// プレイヤークラスを取得する
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面だったら
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// ゲームクラスが取得できた
			pPlayer = pGame->GetPlayer();
		}
	}
	if (m_State == STATE_HORMING)
	{// 追尾する状態ならば
		if (pPlayer != NULL)
		{// プレイヤーが取得できた
			// 向きの計算を行う
			m_fDestAngle = atan2f(pPlayer->GetPos().x - m_Pos.x, pPlayer->GetPos().z - m_Pos.z);
		}

		// 竜巻の現在の向きと目的の向きの差分を計算
		m_fDiffAngle = m_fDestAngle - m_fAngle;
		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI * 2.0f;
		}

		// 竜巻の向きを補正する
		m_fAngle += m_fDiffAngle * TORNADE_DIFF_ANGLE;
		if (m_fAngle > D3DX_PI)
		{
			m_fAngle -= D3DX_PI * 2.0f;
		}
		if (m_fAngle < -D3DX_PI)
		{
			m_fAngle += D3DX_PI * 2.0f;
		}

		// 竜巻を移動させる
		m_Pos.x += sinf(m_fAngle) * TORNADE_MOVE;
		m_Pos.z += cosf(m_fAngle) * TORNADE_MOVE;

		float fRivision = sqrtf((m_fDiffAngle * TORNADE_DIFF_ANGLE) * (m_fDiffAngle * TORNADE_DIFF_ANGLE));
		m_fRivisionAngle += fRivision;
		if (m_fRivisionAngle >= TORNADE_RIVISION_ANGLE)
		{// 補正した向きの量が多くなりすぎた
			m_State = STATE_NONE_HORMING;
		}

		if (pPlayer != NULL)
		{// プレイヤーが取得できた
			// 当たり判定を行う
			Collision(pPlayer);
		}
	}
	else if(m_State == STATE_NONE_HORMING)
	{// 追尾しない状態ならば
	    // 竜巻の現在の向きと目的の向きの差分を計算
		m_fDiffAngle = m_fDestAngle - m_fAngle;
		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI * 2.0f;
		}

	    // 竜巻の向きを補正する
		m_fAngle += m_fDiffAngle * TORNADE_DIFF_ANGLE;
		if (m_fAngle > D3DX_PI)
		{
			m_fAngle -= D3DX_PI * 2.0f;
		}
		if (m_fAngle < -D3DX_PI)
		{
			m_fAngle += D3DX_PI * 2.0f;
		}

	    // 竜巻を移動させる
		m_Pos.x += sinf(m_fAngle) * TORNADE_MOVE;
		m_Pos.z += cosf(m_fAngle) * TORNADE_MOVE;

	    // 当たり判定を行う
		Collision(pPlayer);

		// 寿命をへらす
		m_nLife--;
		if (m_nLife <= 0.0f)
		{// 寿命がなくなった
			Uninit();
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CTornade::Draw(void)
{

}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CTornade::Collision(CPlayer *pPlayer)
{
	bool bHit = false;                 // 攻撃がヒットしたかどうか
	float fPlayerRange = 0.0f;         // プレイヤーの当たり判定を取る範囲
	D3DXVECTOR3 OffSet = m_Pos;        // 当たり判定をとる場所
	float fHitRange = 0.0f;            // 当たり判定を取る長さ
	float fLength = 0.0f;              // 当たり判定を取る場所と敵との距離

	// 判定開始
	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
	    // プレイヤーの当たり判定を取る範囲を取得
		fPlayerRange = pPlayer->GetColRange();

		// 当たり判定を取る長さを計算
		fHitRange = sqrtf((fPlayerRange + TORNADE_COLLISION_RANGE) * (fPlayerRange + TORNADE_COLLISION_RANGE));

		// 当たり判定を取る座標と敵の座標との距離を測る
		D3DXMATRIX PlayerMtxWorld;
		D3DXVECTOR3 PlayerPos;
		CModel *pModel = pPlayer->GetModel(0);
		if (pModel != NULL)
		{
			PlayerMtxWorld = pModel->GetMtxWorld();
			PlayerPos = D3DXVECTOR3(PlayerMtxWorld._41, PlayerMtxWorld._42, PlayerMtxWorld._43);
			fLength = sqrtf((PlayerPos.x - OffSet.x) * (PlayerPos.x - OffSet.x) + (PlayerPos.y - OffSet.y) * (PlayerPos.y - OffSet.y) + (PlayerPos.z - OffSet.z) * (PlayerPos.z - OffSet.z));
		}

		if (fLength <= fHitRange)
		{// 当たり判定を取る座標と敵の座標との距離が当たり判定を取る長さより短い
			if (pPlayer->GetState() != CPlayer::STATE_DAMAGE)
			{// プレイヤーの状態がダメージ状態でない
				pPlayer->Damage(TORNADE_DAMAGE);
				m_State = STATE_NONE_HORMING;
				bHit = true;
			}
		}
	}

	return bHit;
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
void CTornade::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向きを設定する処理
//=============================================================================
void CTornade::SetAngle(const float fAngle)
{
	m_fAngle = fAngle;
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを設定する処理
//=============================================================================
void CTornade::SetEffectManager(CEffectManager *pEffectManager)
{
	m_pEffectManager = pEffectManager;
}

//=============================================================================
//    座標を取得する処理
//=============================================================================
D3DXVECTOR3 CTornade::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向きを取得する処理
//=============================================================================
float CTornade::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを取得する処理
//=============================================================================
CEffectManager *CTornade::GetEffectManager(void)
{
	return m_pEffectManager;
}