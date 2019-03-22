//*****************************************************************************
//
//     リザルトの処理[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"
#include "ranking.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RESULT_SYSTEM_FILENAME         "data/TEXT/MODE/result.ini"                  // リザルトの初期化情報スクリプトファイル名
#define RESULT_MODECHANGE_TIMING       (600)                                        // 画面遷移を自動で行うタイミング
#define RESULT_SCOREAPPEAR_TIMING      (60)                                         // スコアを次々に出していくタイミング
#define RESULT_NUMBER_INTERVAL         (1.0f)                                       // 数字と数字の間隔

// 値を読み取るパス名
#define NUM_TEXTURE                    "NUM_TEXTURE = "                             // 読み込むテクスチャの数
#define TEXTURE_FILENAME               "TEXTURE_FILENAME = "                        // 読み込むテクスチャのファイル名

// PressEnter用のマクロ
#define RESULT_PRESS_POS               (D3DXVECTOR3(SCREEN_WIDTH / 2,690.0f,0.0f))  // PressEnterの座標
#define RESULT_PRESS_COL               (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // PressEnterの色
#define RESULT_PRESS_WIDTH             (280.0f)                                     // PressEnterの幅
#define RESULT_PRESS_HEIGHT            (30.0f)                                      // PressEnterの高さ

// 合計スコア用のマクロ
#define RESULT_ALLSCORE_POS            (D3DXVECTOR3(1050.0f,610.0f,0.0f))           // 合計スコアの座標
#define RESULT_ALLSCORE_COL            (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))             // 合計スコアの色
#define RESULT_ALLSCORE_WIDTH          (40.0f)                                      // 合計スコアの幅
#define RESULT_ALLSCORE_HEIGHT         (40.0f)                                      // 合計スコアの高さ

// ゲームスコア用のマクロ
#define RESULT_GAMESCORE_POS           (D3DXVECTOR3(930.0f,175.0f,0.0f))            // ゲームスコアの座標
#define RESULT_GAMESCORE_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // ゲームスコアの色
#define RESULT_GAMESCORE_WIDTH         (30.0f)                                      // ゲームスコアの幅
#define RESULT_GAMESCORE_HEIGHT        (30.0f)                                      // ゲームスコアの高さ

// ステージクリアボーナス用のマクロ
#define RESULT_STAGECLEAR_BONUS_POS    (D3DXVECTOR3(930.0f,257.0f,0.0f))            // ステージクリアボーナスの座標
#define RESULT_STAGECLEAR_BONUS_COL    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // ステージクリアボーナスの色
#define RESULT_STAGECLEAR_BONUS_WIDTH  (30.0f)                                      // ステージクリアボーナスの幅
#define RESULT_STAGECLEAR_BONUS_HEIGHT (30.0f)                                      // ステージクリアボーナスの高さ

// 最大コンボ数ボーナス用のマクロ
#define RESULT_COMBOMAX_BONUS_POS      (D3DXVECTOR3(930.0f,342.0f,0.0f))            // 最大コンボ数ボーナスの座標
#define RESULT_COMBOMAX_BONUS_COL      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // 最大コンボ数ボーナスの色
#define RESULT_COMBOMAX_BONUS_WIDTH    (30.0f)                                      // 最大コンボ数ボーナスの幅
#define RESULT_COMBOMAX_BONUS_HEIGHT   (30.0f)                                      // 最大コンボ数ボーナスの高さ

// ラスボス撃破ボーナス用のマクロ
#define RESULT_LASTBOSS_BONUS_POS      (D3DXVECTOR3(930.0f,422.0f,0.0f))            // ラスボス撃破ボーナスの座標
#define RESULT_LASTBOSS_BONUS_COL      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // ラスボス撃破ボーナスの色
#define RESULT_LASTBOSS_BONUS_WIDTH    (30.0f)                                      // ラスボス撃破ボーナスの幅
#define RESULT_LASTBOSS_BONUS_HEIGHT   (30.0f)                                      // ラスボス撃破ボーナスの高さ

// 隠しアイテムボーナス用のマクロ
#define RESULT_HIDDENITEM_BONUS_POS    (D3DXVECTOR3(930.0f,505.0f,0.0f))            // 隠しアイテムボーナスの座標
#define RESULT_HIDDENITEM_BONUS_COL    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // 隠しアイテムボーナスの色
#define RESULT_HIDDENITEM_BONUS_WIDTH  (30.0f)                                      // 隠しアイテムボーナスの幅
#define RESULT_HIDDENITEM_BONUS_HEIGHT (30.0f)                                      // 隠しアイテムボーナスの高さ

// リザルトロゴ用マクロ
// 座標
#define RESULT_LOGO_POS_0              (D3DXVECTOR3(360.0f,173.0f,0.0f))            // リザルトロゴのポリゴン座標(1つ目)
#define RESULT_LOGO_POS_1              (D3DXVECTOR3(360.0f,259.0f,0.0f))            // リザルトロゴのポリゴン座標(2つ目)
#define RESULT_LOGO_POS_2              (D3DXVECTOR3(360.0f,341.0f,0.0f))            // リザルトロゴのポリゴン座標(3つ目)
#define RESULT_LOGO_POS_3              (D3DXVECTOR3(360.0f,427.0f,0.0f))            // リザルトロゴのポリゴン座標(4つ目)
#define RESULT_LOGO_POS_4              (D3DXVECTOR3(360.0f,513.0f,0.0f))            // リザルトロゴのポリゴン座標(5つ目)
#define RESULT_LOGO_POS_5              (D3DXVECTOR3(330.0f,612.0f,0.0f))            // リザルトロゴのポリゴン座標(6つ目)

// 幅
#define RESULT_LOGO_WIDTH_0            (130.0f)                                     // リザルトロゴのポリゴンの幅(1つ目)
#define RESULT_LOGO_WIDTH_1            (130.0f)                                     // リザルトロゴのポリゴンの幅(2つ目)
#define RESULT_LOGO_WIDTH_2            (130.0f)                                     // リザルトロゴのポリゴンの幅(3つ目)
#define RESULT_LOGO_WIDTH_3            (130.0f)                                     // リザルトロゴのポリゴンの幅(4つ目)
#define RESULT_LOGO_WIDTH_4            (130.0f)                                     // リザルトロゴのポリゴンの幅(5つ目)
#define RESULT_LOGO_WIDTH_5            (150.0f)                                     // リザルトロゴのポリゴンの幅(6つ目)

// 高さ
#define RESULT_LOGO_HEIGHT_0           (30.0f)                                      // リザルトロゴのポリゴンの高さ(1つ目)
#define RESULT_LOGO_HEIGHT_1           (30.0f)                                      // リザルトロゴのポリゴンの高さ(2つ目)
#define RESULT_LOGO_HEIGHT_2           (30.0f)                                      // リザルトロゴのポリゴンの高さ(3つ目)
#define RESULT_LOGO_HEIGHT_3           (30.0f)                                      // リザルトロゴのポリゴンの高さ(4つ目)
#define RESULT_LOGO_HEIGHT_4           (30.0f)                                      // リザルトロゴのポリゴンの高さ(5つ目)
#define RESULT_LOGO_HEIGHT_5           (45.0f)                                      // リザルトロゴのポリゴンの高さ(6つ目)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
int CResult::m_nGameScore = 0;         // ゲームのスコア
int CResult::m_nStageClearBonus = 0;   // ステージクリアボーナスのスコア
int CResult::m_nComboMaxBonus = 0;     // 最大コンボ数ボーナスのスコア
int CResult::m_nLastBossBonus = 0;     // ラスボス撃破ボーナスのスコア
int CResult::m_nHiddenItemBonus = 0;   // 隠しアイテムボーナスのスコア

//=============================================================================
//    コンストラクタ
//=============================================================================
CResult::CResult(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_State = STATE_NONE;      // 状態
	m_nAllScore = 0;           // 合計スコア
	m_nCounter = 0;            // 状態を管理するカウンター
	m_pPress = NULL;           // プレスエンター用の2Dポリゴンクラスへのポインタ
	m_pTextureManager = NULL;  // テクスチャ管理クラスへのポインタ
	m_pResultBg = NULL;        // 背景用のポリゴン

	for (int nCntResult = 0; nCntResult < TYPE_SCORE_MAX; nCntResult++)
	{// スコアの種類だけ繰り返し
		m_apNumber[nCntResult] = NULL;     // それぞれのスコア表示用のポリゴン
		m_aDigit[nCntResult] = NULL;       // それぞれのスコアの桁数
		m_apScoreLogo[nCntResult] = NULL;  // それぞれのスコアのロゴ表示用ポリゴン
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pResult = NULL;  // リザルトクラス型のポインタ
	if (pResult == NULL)
	{// メモリが確保できる状態にある
		pResult = new CResult;
		if (pResult != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pResult->Init();
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
	return pResult;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// リザルトの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(RESULT_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			char aTexFileName[256] = "\0";              // テクスチャのファイル名

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// メモリを確保できた
				    // 文字列を初期化
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// テクスチャの個数情報があった
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// モデル数が1つ以上ある
									m_pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名情報があった
							    // テクスチャのファイルパス名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

								// テクスチャの読み込み
								D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
								if (pTexture != NULL)
								{// テクスチャを読み込めた
									m_pTextureManager->BindTexture(pTexture, nCntTex);
									m_pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// メモリの開放
					if (pStr != NULL)
					{// メモリが確保されている
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// メモリが確保されている
						delete[] pLine;
						pLine = NULL;
					}
				}
				else
				{// メモリを確保できなかった
					return E_FAIL;
				}
			}
			else
			{// メモリを確保できる状態でない
				return E_FAIL;
			}

			fclose(pFile);
		}
		else
		{// ファイルポインタを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// ファイルポインタを確保できる状態でない
		return E_FAIL;
	}

	// 背景の作成
	if (m_pResultBg == NULL)
	{
		m_pResultBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (m_pResultBg != NULL)
		{// メモリを確保できた
			m_pResultBg->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_6));
		}
	}

	// 合計スコアの計算をしておく
	m_nAllScore = m_nGameScore + m_nStageClearBonus + m_nComboMaxBonus + m_nLastBossBonus + m_nHiddenItemBonus;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// PressEnterの破棄
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	// 数字ポリゴンの破棄
	for (int nCntResult = 0; nCntResult < TYPE_SCORE_MAX; nCntResult++)
	{// スコアの種類だけ繰り返し
		if (m_apNumber[nCntResult] != NULL)
		{// メモリが確保されている
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntResult]; nCntDigit++)
			{// ランキングスコアの桁数分繰り返し
				if (m_apNumber[nCntResult][nCntDigit] != NULL)
				{// メモリが確保されている
					m_apNumber[nCntResult][nCntDigit]->Uninit();
					m_apNumber[nCntResult][nCntDigit] = NULL;
				}
			}
			delete m_apNumber[nCntResult];
			m_apNumber[nCntResult] = NULL;
		}
	}

	// ランキングに合計スコアを設定
	CRanking::SetGameScore(m_nAllScore);

	// スコアを初期化しておく
	m_nGameScore = 0;           // ゲームのスコア
	m_nStageClearBonus = 0;     // ステージクリアボーナス
	m_nComboMaxBonus = 0;       // 最大コンボボーナス
	m_nLastBossBonus = 0;       // ラスボス撃破ボーナス
	m_nHiddenItemBonus = 0;     // ステージクリアボーナス

	// リザルトオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CResult::Update(void)
{
	// デバッグ用
	//if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	//{// フェードの状態が使用されていない状態である
	//	CManager::GetFade()->SetFade(CManager::MODE_RANKING);
	//}

	m_nCounter++;  // カウンターを進める

	if (m_State == STATE_APPEAR_ALLSCORE)
	{// 全てのスコアが出きっている状態ならば
		ModeChangeCheck();
	}
	else
	{// それ以外の状態ならば
		if (m_State == STATE_NONE)
		{// まだ何も出ていない状態だったら
			if (m_nCounter >= RESULT_SCOREAPPEAR_TIMING + 60)
			{// カウンターが既定の値を超えた
				CreateNumber();
				m_nCounter = 0;
			}
		}
		else
		{// スコアが徐々に出ている状態だったら
			if (m_nCounter >= RESULT_SCOREAPPEAR_TIMING)
			{// カウンターが既定の値を超えた
				CreateNumber();
				m_nCounter = 0;
			}
		}
	}

	CDebugProc::Print("リザルト画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
//    スコアを表示させる処理
//=============================================================================
void CResult::CreateNumber(void)
{
	// 数字ポリゴン生成に必要な変数を宣言
	D3DXVECTOR3 NumberPos;   // 座標
	D3DXCOLOR NumberCol;     // 色
	float fNumberWidth;      // 幅
	float fNumberHeight;     // 高さ
	int nNumber = 0;         // 表示する数字の計算用
	int nTexNumber = 0;      // 使用するテクスチャの番号
	int nScore = 0;          // 表示するスコア(桁数計算用)

	// ロゴポリゴン生成に必要な変数を宣言
	D3DXVECTOR3 LogoPos;
	float fLogoWidth;
	float fLogoHeight;

	// 変数に値を代入(状態によって処理わけ)
	switch (m_State)
	{
	case STATE_NONE:                     // まだ何も出ていない状態
		NumberPos = RESULT_GAMESCORE_POS;               // 座標
		NumberCol = RESULT_GAMESCORE_COL;               // 色
		fNumberWidth = RESULT_GAMESCORE_WIDTH;          // 幅
		fNumberHeight = RESULT_GAMESCORE_HEIGHT;        // 高さ
		nTexNumber = TEX_NUMBER_0;                      // 使用するテクスチャの番号
		nScore = m_nGameScore;                          // 表示するスコア
		m_State = STATE_APPEAR_GAME_SCORE;              // 状態
		LogoPos = RESULT_LOGO_POS_0;                    // ロゴの座標
		fLogoWidth = RESULT_LOGO_WIDTH_0;               // ロゴの幅
		fLogoHeight = RESULT_LOGO_HEIGHT_0;             // ロゴの高さ
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_GAME_SCORE:        // ゲームのスコアが出ている状態
		NumberPos = RESULT_STAGECLEAR_BONUS_POS;        // 座標
		NumberCol = RESULT_STAGECLEAR_BONUS_COL;        // 色
		fNumberWidth = RESULT_STAGECLEAR_BONUS_WIDTH;   // 幅
		fNumberHeight = RESULT_STAGECLEAR_BONUS_HEIGHT; // 高さ
		nTexNumber = TEX_NUMBER_1;                      // 使用するテクスチャの番号
		nScore = m_nStageClearBonus;                    // 表示するスコア
		m_State = STATE_APPEAR_STAGECLEAR_BONUS;        // 状態
		LogoPos = RESULT_LOGO_POS_1;                    // ロゴの座標
		fLogoWidth = RESULT_LOGO_WIDTH_1;               // ロゴの幅
		fLogoHeight = RESULT_LOGO_HEIGHT_1;             // ロゴの高さ
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_STAGECLEAR_BONUS:  // ステージクリアボーナスが出ている状態
		NumberPos = RESULT_COMBOMAX_BONUS_POS;          // 座標
		NumberCol = RESULT_COMBOMAX_BONUS_COL;          // 色
		fNumberWidth = RESULT_COMBOMAX_BONUS_WIDTH;     // 幅
		fNumberHeight = RESULT_COMBOMAX_BONUS_HEIGHT;   // 高さ
		nTexNumber = TEX_NUMBER_2;                      // 使用するテクスチャの番号
		nScore = m_nComboMaxBonus;                      // 表示するスコア
		m_State = STATE_APPEAR_COMBOMAX_BONUS;          // 状態
		LogoPos = RESULT_LOGO_POS_2;                    // ロゴの座標
		fLogoWidth = RESULT_LOGO_WIDTH_2;               // ロゴの幅
		fLogoHeight = RESULT_LOGO_HEIGHT_2;             // ロゴの高さ
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_COMBOMAX_BONUS:    // 最大コンボボーナスが出ている状態
		NumberPos = RESULT_LASTBOSS_BONUS_POS;          // 座標
		NumberCol = RESULT_LASTBOSS_BONUS_COL;          // 色
		fNumberWidth = RESULT_LASTBOSS_BONUS_WIDTH;     // 幅
		fNumberHeight = RESULT_LASTBOSS_BONUS_HEIGHT;   // 高さ
		nTexNumber = TEX_NUMBER_3;                      // 使用するテクスチャの番号
		nScore = m_nLastBossBonus;                      // 表示するスコア
		m_State = STATE_APPEAR_LASTBOSS_BONUS;          // 状態
		LogoPos = RESULT_LOGO_POS_3;                    // ロゴの座標
		fLogoWidth = RESULT_LOGO_WIDTH_3;               // ロゴの幅
		fLogoHeight = RESULT_LOGO_HEIGHT_3;             // ロゴの高さ
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_LASTBOSS_BONUS:    // ラスボス撃破ボーナスが出ている状態
		NumberPos = RESULT_HIDDENITEM_BONUS_POS;        // 座標
		NumberCol = RESULT_HIDDENITEM_BONUS_COL;        // 色
		fNumberWidth = RESULT_HIDDENITEM_BONUS_WIDTH;   // 幅
		fNumberHeight = RESULT_HIDDENITEM_BONUS_HEIGHT; // 高さ
		nTexNumber = TEX_NUMBER_4;                      // 使用するテクスチャの番号
		nScore = m_nHiddenItemBonus;                    // 表示するスコア
		m_State = STATE_APPEAR_HIDDENITEM_BONUS;        // 状態
		LogoPos = RESULT_LOGO_POS_4;                    // ロゴの座標
		fLogoWidth = RESULT_LOGO_WIDTH_4;               // ロゴの幅
		fLogoHeight = RESULT_LOGO_HEIGHT_4;             // ロゴの高さ
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_HIDDENITEM_BONUS:  // 隠しアイテムボーナスが出ている状態
		NumberPos = RESULT_ALLSCORE_POS;                // 座標
		NumberCol = RESULT_ALLSCORE_COL;                // 色
		fNumberWidth = RESULT_ALLSCORE_WIDTH;           // 幅
		fNumberHeight = RESULT_ALLSCORE_HEIGHT;         // 高さ
		nTexNumber = TEX_NUMBER_5;                      // 使用するテクスチャの番号
		nScore = m_nAllScore;                           // 表示するスコア
		m_State = STATE_APPEAR_ALLSCORE;                // 状態
		LogoPos = RESULT_LOGO_POS_5;                    // ロゴの座標
		fLogoWidth = RESULT_LOGO_WIDTH_5;               // ロゴの幅
		fLogoHeight = RESULT_LOGO_HEIGHT_5;             // ロゴの高さ
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT001);
		break;
	}

	// 表示するスコアの桁数を計算
	m_aDigit[nTexNumber] = (int)log10f((float)nScore) + 1;  // 桁数を求める
	if (m_aDigit[nTexNumber] <= 0)
	{// 桁数が0以下ならば
		m_aDigit[nTexNumber] = 1; // 0を表示する分を追加
	}

	// 数字の生成
	m_apNumber[nTexNumber] = new CNumber*[m_aDigit[nTexNumber]];   // 桁数分メモリ確保
	if (m_apNumber[nTexNumber] != NULL)
	{// メモリが確保されている
		for (int nCntScore = 0; nCntScore < m_aDigit[nTexNumber]; nCntScore++)
		{// スコアの桁数分繰り返し
		    // 表示する数字の計算
			nNumber = nScore % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

			// 数字ポリゴンのインスタンス生成処理
			m_apNumber[nTexNumber][nCntScore] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber);
			if (m_apNumber[nTexNumber][nCntScore] != NULL)
			{// メモリを確保できた
				m_apNumber[nTexNumber][nCntScore]->BindTexture(m_pTextureManager->GetTexture(8));
			}

			// 横に進める
			NumberPos.x -= fNumberWidth + (fNumberWidth * RESULT_NUMBER_INTERVAL);
		}
	}

	// ロゴポリゴンの生成
	if (m_apScoreLogo[nTexNumber] == NULL)
	{// メモリが確保できる状態である
		m_apScoreLogo[nTexNumber] = CScene2D::Create(LogoPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fLogoWidth, fLogoHeight);
		if (m_apScoreLogo[nTexNumber] != NULL)
		{// 生成できた
			m_apScoreLogo[nTexNumber]->BindTexture(m_pTextureManager->GetTexture(nTexNumber));
		}
	}

	if (m_State == STATE_APPEAR_ALLSCORE)
	{// 合計スコアが出ている状態になった
	    // PressEnterの作成
		if (m_pPress == NULL)
		{
			m_pPress = CScene2DPress::Create(RESULT_PRESS_POS, RESULT_PRESS_COL, RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT);
			if (m_pPress != NULL)
			{// メモリを確保できた
				m_pPress->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_7));
			}
		}
	}
}

//=============================================================================
//    遷移するかどうかチェックする処理
//=============================================================================
void CResult::ModeChangeCheck(void)
{
	if (m_nCounter >= RESULT_MODECHANGE_TIMING)
	{// カウンターが既定の値を超えた
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	else
	{// カウンターがまだ既定の値を超えていない
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				CManager::GetFade()->SetFade(CManager::MODE_RANKING);
				if (m_pPress != NULL)
				{// メモリが確保されている
					m_pPress->FlashStart();
				}
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						CManager::GetFade()->SetFade(CManager::MODE_RANKING);
						if (m_pPress != NULL)
						{// メモリが確保されている
							m_pPress->FlashStart();
						}
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
					}
				}
			}
		}
	}
}

//=============================================================================
//    ゲームスコアを設定する処理
//=============================================================================
void CResult::SetGameScore(int nGameScore)
{
	m_nGameScore = nGameScore;
}

//=============================================================================
//    ステージクリアボーナスを設定する処理
//=============================================================================
void CResult::SetStageClearBonus(int nStageBonus)
{
	m_nStageClearBonus = nStageBonus;
}

//=============================================================================
//    最大コンボ数ボーナスを設定する処理
//=============================================================================
void CResult::SetComboMaxBonus(int nComboMaxBonus)
{
	m_nComboMaxBonus = nComboMaxBonus;
}

//=============================================================================
//    ラスボス撃破ボーナスを設定する処理
//=============================================================================
void CResult::SetLastBossBonus(int nLastBossBonus)
{
	m_nLastBossBonus = nLastBossBonus;
}

//=============================================================================
//    隠しアイテムボーナスを設定する処理
//=============================================================================
void CResult::SetHiddenItemBonus(int nHiddenItemBonus)
{
	m_nHiddenItemBonus = nHiddenItemBonus;
}