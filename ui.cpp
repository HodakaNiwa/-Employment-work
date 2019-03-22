//*****************************************************************************
//
//     UIの処理[ui.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ui.h"
#include "timer.h"
#include "score.h"
#include "combo.h"
#include "gauge.h"
#include "textureManager.h"
#include "game.h"
#include "manager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// データ入力用マクロ
#define UI_NUM                 (8)                                          // 表示するUIオブジェクトの数
#define UI_ADD_ALPHA           (1.0f)                                       // UIの透明度を下げる値
#define UI_CUT_ALPHA           (0.08f)                                      // UIの透明度を上げる値

// タイマー用マクロ
#define UI_TIMER_POS           (D3DXVECTOR3(680.0f,32.0f,0.0f))             // タイマーの座標
#define UI_TIMER_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // タイマーの色
#define UI_TIMER_WIDTH         (30.0f)                                      // タイマーの数字ポリゴン１つ分の幅
#define UI_TIMER_HEIGHT        (30.0f)                                      // タイマーの数字ポリゴン１つ分の高さ
#define UI_TIMER_WARNING       (30 * 60)                                    // タイマーが赤くなるタイミング

// スコア用マクロ
#define UI_SCORE_POS           (D3DXVECTOR3(300.0f,22.0f,0.0f))             // スコアの座標
#define UI_SCORE_COL           (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))             // スコアの色
#define UI_SCORE_WIDTH         (20.0f)                                      // スコアの数字ポリゴン１つ分の幅
#define UI_SCORE_HEIGHT        (20.0f)                                      // スコアの数字ポリゴン１つ分の高さ

// 体力ゲージ用マクロ
#define UI_LIFEGAUGE_POS       (D3DXVECTOR3(1020.0f,660.0f,0.0f))           // 体力ゲージの座標
#define UI_LIFEGAUGE_COL       (D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))             // 体力ゲージの色
#define UI_LIFEGAUGE_WIDTH     (200.0f)                                     // 体力ゲージの幅
#define UI_LIFEGAUGE_HEIGHT    (21.0f)                                      // 体力ゲージの高さ

// 軌跡斬ゲージ用マクロ
#define UI_ORBITGAUGE_POS      (D3DXVECTOR3(230.0f,675.0f,0.0f))            // 軌跡ゲージの座標
#define UI_ORBITGAUGE_COL      (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))             // 軌跡ゲージの色
#define UI_ORBITGAUGE_WIDTH    (200.0f)                                     // 軌跡ゲージの幅
#define UI_ORBITGAUGE_HEIGHT   (14.0f)                                      // 軌跡ゲージの高さ

// 必殺ゲージ用マクロ
#define UI_SPECIALGAUGE_POS    (D3DXVECTOR3(165.0f,608.0f,0.0f))            // 必殺ゲージの座標
#define UI_SPECIALGAUGE_COL    (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))             // 必殺ゲージの色
#define UI_SPECIALGAUGE_WIDTH  (142.0f)                                     // 必殺ゲージの幅
#define UI_SPECIALGAUGE_HEIGHT (13.0f)                                      // 必殺ゲージの高さ

// コンボ数用マクロ
#define UI_COMBO_POS           (D3DXVECTOR3(1500.0f,350.0f,0.0f))           // コンボUIの初期座標
#define UI_COMBO_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // コンボUIの色
#define UI_COMBO_WIDTH         (22.0f)                                      // コンボUIの数字ポリゴン１つ分の幅
#define UI_COMBO_HEIGHT        (22.0f)                                      // コンボUIの数字ポリゴン１つ分の高さ

// 現在のステージ数用マクロ
#define UI_NUMSTAGE_POS        (D3DXVECTOR3(SCREEN_WIDTH / 2,40.0f,0.0f))   // ステージ数UIの初期座標
#define UI_NUMSTAGE_COL        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // ステージ数UIの色
#define UI_NUMSTAGE_WIDTH      (90.0f)                                      // ステージ数UIのポリゴンの幅
#define UI_NUMSTAGE_HEIGHT     (30.0f)                                      // ステージ数UIのポリゴンの高さ

// 目的用マクロ
#define UI_PURPOSE_POS         (D3DXVECTOR3(SCREEN_WIDTH / 2,100.0f,0.0f))  // 目的のUIの初期座標
#define UI_PURPOSE_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // 目的のUIの色
#define UI_PURPOSE_WIDTH       (160.0f)                                     // 目的のUIのポリゴンの幅
#define UI_PURPOSE_HEIGHT      (30.0f)                                      // 目的のUIのポリゴンの高さ

// 目的の背景用マクロ
#define UI_PAPER_POS           (D3DXVECTOR3(SCREEN_WIDTH / 2,76.0f,0.0f))   // 紙の初期座標
#define UI_PAPER_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // 紙の色
#define UI_PAPER_WIDTH         (180.0f)                                     // 紙のポリゴンの幅
#define UI_PAPER_HEIGHT        (70.0f)                                      // 紙のポリゴンの高さ

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CUI::CUI()
{
	// 各種値のクリア
	m_State = STATE_NORMAL;    // 状態
	m_pTimer = NULL;           // タイマークラスへのポインタ
	m_pScore = NULL;           // スコアクラスへのポインタ
	m_pGaugeLife = NULL;       // プレイヤーの体力ゲージクラスへのポインタ
	m_pGaugeOrbitSlash = NULL; // プレイヤーの軌跡ゲージクラスへのポインタ
	m_pGaugeSpecial = NULL;    // プレイヤーの必殺技ゲージクラスへのポインタ
	m_pCombo = NULL;           // コンボUIクラスへのポインタ
	m_pPaper = NULL;           // 紙背景ポリゴンクラスへのポインタ
	m_pNumStage = NULL;        // 現在のステージ数ポリゴンクラスへのポインタ
	m_pPurpose = NULL;         // 目的のポリゴンクラスへのポインタ
	m_pTextureManager = NULL;  // テクスチャ管轄クラスへのポインタ
	m_bDisp = true;            // UIを描画するかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CUI *CUI::Create(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager)
{
	CUI *pUi = NULL;  // UIクラス型のポインタ
	if (pUi == NULL)
	{// メモリが確保できる状態にある
		pUi = new CUI;
		if (pUi != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pUi->Init(nTimer, nPlayerLife, fPlayerOrbitSlash, fPlayerSpecial, pTextureManager);
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
	return pUi;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CUI::Init(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager)
{
	// 必要なUIを生成する
	// スコア
	m_pScore = CScore::Create(UI_SCORE_POS, UI_SCORE_COL, UI_SCORE_WIDTH, UI_SCORE_HEIGHT, UI_SCORE_PRIORITY);
	if (m_pScore != NULL)
	{// スコアクラスが生成できた
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲーム中だったら
			m_pScore->SetScoreTexture(pTextureManager->GetTexture(CGame::TEX_NUMBER_4));
		}
		else if(CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// チュートリアル中だったら
			m_pScore->SetScoreTexture(pTextureManager->GetTexture(CGame::TEX_NUMBER_5));
		}
	}

	int nGaugeTexIdx = 0;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム中だったら
		nGaugeTexIdx = 7;
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// チュートリアル中だったら
		nGaugeTexIdx = 9;
	}

	// プレイヤーの体力ゲージ
	m_pGaugeLife = CGaugeLife2D::Create(UI_LIFEGAUGE_POS, UI_LIFEGAUGE_COL, UI_LIFEGAUGE_WIDTH, UI_LIFEGAUGE_HEIGHT, 0.0f, nPlayerLife, pTextureManager->GetTexture(nGaugeTexIdx), UI_LIFEGAUGE_PRIORITY);
	nGaugeTexIdx++;

	// プレイヤーの軌跡ゲージ
	m_pGaugeOrbitSlash = CGaugeOrbitSlash2D::Create(UI_ORBITGAUGE_POS, UI_ORBITGAUGE_COL, UI_ORBITGAUGE_WIDTH, UI_ORBITGAUGE_HEIGHT, 0.0f, fPlayerOrbitSlash, pTextureManager->GetTexture(nGaugeTexIdx), UI_ORBITGAUGE_PRIORITY);
	nGaugeTexIdx++;

	// プレイヤーの必殺ゲージ
	m_pGaugeSpecial = CGaugeSpecial2D::Create(UI_SPECIALGAUGE_POS, UI_SPECIALGAUGE_COL, UI_SPECIALGAUGE_WIDTH, UI_SPECIALGAUGE_HEIGHT, 0.0f, fPlayerSpecial, 0.0f, pTextureManager->GetTexture(nGaugeTexIdx), UI_SPECIALGAUGE_PRIORITY);

	// 紙ポリゴン
	m_pPaper = CScene2D::Create(UI_PAPER_POS, UI_PAPER_COL, UI_PAPER_WIDTH, UI_PAPER_HEIGHT, 0.0f, UI_PAPER_PRIORITY);
	if (m_pPaper != NULL)
	{// 紙ポリゴンクラスが生成できた
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲーム中だったら
			m_pPaper->BindTexture(pTextureManager->GetTexture(15));
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// チュートリアル中だったら
			m_pPaper->BindTexture(pTextureManager->GetTexture(8));
		}
	}

	// 現在のステージ数ポリゴン
	m_pNumStage = CScene2D::Create(UI_NUMSTAGE_POS, UI_NUMSTAGE_COL, UI_NUMSTAGE_WIDTH, UI_NUMSTAGE_HEIGHT, 0.0f, UI_NUMSTAGE_PRIORITY);
	if (m_pNumStage != NULL)
	{// 現在のステージ数クラスが生成できた
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲーム中だったら
			m_pNumStage->BindTexture(pTextureManager->GetTexture(10));
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// チュートリアル中だったら
			m_pNumStage->BindTexture(pTextureManager->GetTexture(6));
		}
	}

	// 目的のポリゴン
	m_pPurpose = CScene2D::Create(UI_PURPOSE_POS, UI_PURPOSE_COL, UI_PURPOSE_WIDTH, UI_PURPOSE_HEIGHT, 0.0f, UI_PURPOSE_PRIORITY);
	if (m_pPurpose != NULL)
	{// 目的のポリゴンクラスが生成できた
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲーム中だったら
			m_pPurpose->BindTexture(pTextureManager->GetTexture(13));
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// チュートリアル中だったら
			m_pPurpose->BindTexture(pTextureManager->GetTexture(7));
		}
	}

	// テクスチャ管轄クラスへのポインタを保存しておく
	m_pTextureManager = pTextureManager;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CUI::Uninit(void)
{
	// タイマークラスへのポインタを空にする
	m_pTimer = NULL;

	// スコアクラスへのポインタを空にする
	m_pScore = NULL;

	// 体力ゲージクラスへのポインタを空にする
	m_pGaugeLife = NULL;

	// 軌跡ゲージクラスへのポインタを空にする
	m_pGaugeOrbitSlash = NULL;

	// 必殺ゲージクラスへのポインタを空にする
	m_pGaugeSpecial = NULL;

	// コンボUIクラスへのポインタを空にする
	m_pCombo = NULL;

	// 現在のステージ数ポリゴンクラスへのポインタを空にする
	m_pNumStage = NULL;

	// 目的のポリゴンクラスへのポインタを空にする
	m_pPurpose = NULL;

	// 紙背景ポリゴンクラスへのポインタを空にする
	m_pPaper = NULL;

	// テクスチャ管轄クラスへのポインタを空にする
	m_pTextureManager = NULL;
}

//=============================================================================
//    更新処理
//=============================================================================
void CUI::Update(void)
{
	if (m_State == STATE_ALPHACHANGE)
	{// 透明度を変更する状態ならば
	    // 透明度を変更する
		int nCntOk = 0;
		D3DXCOLOR col;
		float fValue;

		if (m_bDisp != true)
		{// 描画しない状態ならば
			// 透明度を0にしていく
			fValue = -UI_ADD_ALPHA;

			// スコア
			if (m_pScore != NULL)
			{// 生成されている
				col = m_pScore->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pScore->SetCol(col);
			}

			// 体力ゲージ
			if (m_pGaugeLife != NULL)
			{// 生成されている
				col = m_pGaugeLife->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pGaugeLife->ChangeAlpha((float)col.a);
			}

			// 軌跡ゲージ
			if (m_pGaugeOrbitSlash != NULL)
			{// 生成されている
				col = m_pGaugeOrbitSlash->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pGaugeOrbitSlash->ChangeAlpha((float)col.a);
			}

			// 必殺ゲージ
			if (m_pGaugeSpecial != NULL)
			{// 生成されている
				col = m_pGaugeSpecial->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pGaugeSpecial->ChangeAlpha((float)col.a);
			}

			// コンボUI
			if (m_pCombo != NULL)
			{// 生成されている
				col = m_pCombo->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pCombo->SetCol(col);
			}

			// 現在のステージ数
			if (m_pNumStage != NULL)
			{// 生成されている
				col = m_pNumStage->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pNumStage->SetCol(col);
			}

			// 目的
			if (m_pPurpose != NULL)
			{// 生成されている
				col = m_pPurpose->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pPurpose->SetCol(col);
			}

			// 紙背景
			if (m_pPaper != NULL)
			{// 生成されている
				col = m_pPaper->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// 透明になり切っている
					col.a = 0.0f;
					nCntOk++;
				}
				m_pPaper->SetCol(col);
			}
		}
		else if (m_bDisp == true)
		{// 描画する状態ならば
			// 透明度を1にしていく
			fValue = UI_CUT_ALPHA;

			// スコア
			if (m_pScore != NULL)
			{// 生成されている
				col = m_pScore->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pScore->SetCol(col);
			}

			// 体力ゲージ
			if (m_pGaugeLife != NULL)
			{// 生成されている
				col = m_pGaugeLife->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pGaugeLife->ChangeAlpha((float)col.a);
			}

			// 軌跡ゲージ
			if (m_pGaugeOrbitSlash != NULL)
			{// 生成されている
				col = m_pGaugeOrbitSlash->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pGaugeOrbitSlash->ChangeAlpha((float)col.a);
			}

			// 必殺ゲージ
			if (m_pGaugeSpecial != NULL)
			{// 生成されている
				col = m_pGaugeSpecial->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pGaugeSpecial->ChangeAlpha((float)col.a);
			}

			// コンボUI
			if (m_pCombo != NULL)
			{// 生成されている
				col = m_pCombo->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pCombo->SetCol(col);
			}

			// 現在のステージ数
			if (m_pNumStage != NULL)
			{// 生成されている
				col = m_pNumStage->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pNumStage->SetCol(col);
			}

			// 目的
			if (m_pPurpose != NULL)
			{// 生成されている
				col = m_pPurpose->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pPurpose->SetCol(col);
			}

			// 紙背景
			if (m_pPaper != NULL)
			{// 生成されている
				col = m_pPaper->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// 透明度がこれ以上下がらない
					col.a = 1.0f;
					nCntOk++;
				}
				m_pPaper->SetCol(col);
			}
		}

		if (nCntOk == UI_NUM)
		{// 全ての透明度が変更されている
			m_State = STATE_NORMAL;
		}
	}
}

//=============================================================================
//    UIを描画するかどうかを設定する処理
//=============================================================================
void CUI::SetDisp(const bool bDisp)
{
	m_bDisp = bDisp;
	m_State = STATE_ALPHACHANGE;
}

//=============================================================================
//    UIを描画するかどうかを取得する処理
//=============================================================================
bool CUI::GetDisp(void)
{
	return m_bDisp;
}

//=============================================================================
//    コンボUIクラスを生成する処理
//=============================================================================
void CUI::CreateCombo(void)
{
	if (m_pCombo == NULL)
	{// メモリが確保されていない
		m_pCombo = CCombo::Create(UI_COMBO_POS, UI_COMBO_COL, UI_COMBO_WIDTH, UI_COMBO_HEIGHT, UI_COMBO_PRIORITY);
		if(m_pCombo != NULL)
		{// メモリが確保できた
			m_pCombo->SetNumberTexture(m_pTextureManager->GetTexture(CGame::TEX_NUMBER_4));
			m_pCombo->SetLogoTexture(m_pTextureManager->GetTexture(CGame::TEX_NUMBER_5), 0);
		}
	}
}

//=============================================================================
//    コンボUIクラスへのポインタを空にする処理
//=============================================================================
void CUI::ReleaseCombo(void)
{
	if (m_pCombo != NULL)
	{// メモリが確保されている
		m_pCombo = NULL;
	}
}

//=============================================================================
//    スコアクラスへのポインタを取得する処理
//=============================================================================
CScore *CUI::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
//    タイマークラスへのポインタを取得する処理
//=============================================================================
CTimer *CUI::GetTimer(void)
{
	return m_pTimer;
}

//=============================================================================
//    コンボUIクラスへのポインタを取得する処理
//=============================================================================
CCombo *CUI::GetCombo(void)
{
	return m_pCombo;
}

//=============================================================================
//    現在のステージ数ポリゴンクラスへのポインタを取得する処理
//=============================================================================
CScene2D *CUI::GetNumStage(void)
{
	return m_pNumStage;
}

//=============================================================================
//    目的のポリゴンクラスへのポインタを取得する処理
//=============================================================================
CScene2D *CUI::GetPurpose(void)
{
	return m_pPurpose;
}