//*****************************************************************************
//
//     ランキングの処理[ranking.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"
#include "particle2D.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RANKING_POLYGON_PRIORITY     (10)                                        // ランキング画面のポリゴンの処理優先順位
#define RANKING_SYSTEM_FILENAME      "data/TEXT/MODE/ranking.ini"                // ランキングの初期化情報スクリプトファイル名
#define RANKING_SCORE_FILENAME       "data/TEXT/RANKING/ranking_score.txt"       // ランキングスコア情報スクリプトファイル名
#define RANKING_MODECHANGE_TIMING    (600)                                       // 画面遷移を自動で行うタイミング
#define RANKING_NUMBER_INTERVAL      (0.9f)                                      // 数字と数字の間隔

// 値を読み取るパス名
#define NUM_TEXTURE                  "NUM_TEXTURE = "                            // 読み込むテクスチャの数
#define TEXTURE_FILENAME             "TEXTURE_FILENAME = "                       // 読み込むテクスチャのファイル名
#define MAP_FILENAME                 "MAP_FILENAME = "                           // マップデータのスクリプトファイル名
#define RANKINGSCORE_FILENAME        "RANKINGSCORE_FILENAME = "                  // ランキングスコアのスクリプトファイル名
#define RANKING_SCORE                "RANKING_SCORE = "                          // ランキングスコア

// エフェクト専用マクロ
#define RANKING_PARTICLE_APPEAR      (1)                                         // 一回の更新で出すパーティクルの量
#define RANKING_ORBIT_WIDTH          (45.0f)                                     // 軌跡エフェクトの横幅
#define RANKING_ORBIT_HEIGHT         (25.0f)                                     // 軌跡エフェクトの縦幅
#define RANKING_PARTICLE_COL_0       (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))            // ランキングパーティクルの色(1つ目)
#define RANKING_PARTICLE_COL_1       (D3DXCOLOR(0.6f,0.6f,0.6f,1.0f))            // ランキングパーティクルの色(2つ目)
#define RANKING_PARTICLE_COL_2       (D3DXCOLOR(0.6f,0.1f,0.0f,1.0f))            // ランキングパーティクルの色(3つ目)
#define RANKING_PARTICLE_COL_3       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキングパーティクルの色(4つ目)
#define RANKING_PARTICLE_COL_4       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキングパーティクルの色(5つ目)

// ランキングスコア用のマクロ
#define RANKING_SCORE_POS_INI        (D3DXVECTOR3(3000.0f,165.0f,0.0f))          // ランキングスコアの座標(初期値)
#define RANKING_SCORE_POS_MAX        (950.0f)                                    // ランキングスコアの座標の最大値
#define RANKING_SCORE_WIDTH_INI      (45.0f)                                     // ランキングスコアの幅(初期値)
#define RANKING_SCORE_HEIGHT_INI     (40.0f)                                     // ランキングスコアの高さ(初期値)
#define RANKING_SCORE_MOVE_INI       (D3DXVECTOR3(-15.0f,0.0f,0.0f))             // ランキングスコアの移動量(初期値)
#define RANKING_SCORE_COL_0          (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))            // ランキングスコアの色(1つめ)
#define RANKING_SCORE_COL_1          (D3DXCOLOR(0.6f,0.6f,0.6f,1.0f))            // ランキングスコアの色(2つめ)
#define RANKING_SCORE_COL_2          (D3DXCOLOR(0.9f,0.7f,0.0f,1.0f))            // ランキングスコアの色(3つめ)
#define RANKING_SCORE_COL_3          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキングスコアの色(4つめ)
#define RANKING_SCORE_COL_4          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキングスコアの色(5つめ)

// ランキング順位用のマクロ
#define RANKING_RANK_POS_INI         (D3DXVECTOR3(2400.0f,165.0f,0.0f))          // ランキング順位の座標(初期値)
#define RANKING_RANK_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキング順位の色(初期値)
#define RANKING_RANK_WIDTH_INI       (100.0f)                                    // ランキング順位の幅(初期値)
#define RANKING_RANK_HEIGHT_INI      (70.0f)                                     // ランキング順位の高さ(初期値)
#define RANKING_RANK_POS_MAX         (350.0f)                                    // ランキング順位の座標の最大値

// ランキングロゴ用のマクロ
#define RANKING_LOGO_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 2,65.0f,0.0f))  // ランキングロゴの座標(初期値)
#define RANKING_LOGO_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキングロゴの色(初期値)
#define RANKING_LOGO_WIDTH_INI       (300.0f)                                    // ランキングロゴの幅(初期値)
#define RANKING_LOGO_HEIGHT_INI      (60.0f)                                     // ランキングロゴの高さ(初期値)

// ランキングカメラ用のマクロ
#define RANKING_CAMERA_PATTERN_NUM    (3)                                        // ランキングカメラのパターン数
#define RANKING_CAMERA_START_POSV_Y   (D3DXVECTOR3(0.0f,575.0f,0.0f))            // ランキングスタート時のカメラの視点の高さ
#define RANKING_CAMERA_START_POSR     (D3DXVECTOR3(-4200.0f,450.0f,-4450.0f))    // ランキングスタート時のカメラの注視点の位置
#define RANKING_CAMERA_DEST_POSR      (D3DXVECTOR3(-4200.0f,450.0f,100.0f))      // ランキングスタート時のカメラの注視点の目的の位置
#define RANKING_CAMERA_MOVESPEED      (0.002f)                                   // ランキングスタート時のカメラの移動スピード

// パターン1
#define RANKING_CAMERA_SWITCHTIMING_0 (300)                                      // 1つ目のパターンから2つ目のパターンへタイトルカメラを切り替えるタイミング
#define RANKING_CAMERA_0_POSV_Y       (D3DXVECTOR3(0.0f,50.0f,0.0f))             // パターン2つ目のカメラの視点の高さ
#define RANKING_CAMERA_0_POSR         (D3DXVECTOR3(1200.0f,110.0f,-3780.0f))     // パターン2つ目のカメラの注視点の位置
#define RANKING_CAMERA_0_POSRDEST     (D3DXVECTOR3(-1500.0f,110.0f,-3780.0f))    // パターン2つ目のカメラの注視点の目的の位置
#define RANKING_CAMERA_0_MOVESPEED    (0.0025f)                                  // パターン2つ目のカメラの移動スピード
#define RANKING_CAMERA_0_ROT          (D3DXVECTOR3(0.0f,-D3DX_PI / 2,0.0f))      // パターン2つ目のカメラ向き

// パターン2
#define RANKING_CAMERA_SWITCHTIMING_1 (480)                                       // 2つ目のパターンから3つ目のパターンへタイトルカメラを切り替えるタイミング
#define RANKING_CAMERA_1_POSV_Y       (D3DXVECTOR3(0.0f,40.0f,0.0f))              // パターン3つ目のカメラの視点の高さ
#define RANKING_CAMERA_1_POSR         (D3DXVECTOR3(1200.0f,100.0f,-3780.0f))      // パターン3つ目のカメラの注視点の位置
#define RANKING_CAMERA_1_POSRDEST     (D3DXVECTOR3(1700.0f,100.0f,-3780.0f))      // パターン3つ目のカメラの注視点の目的の位置
#define RANKING_CAMERA_1_MOVESPEED    (0.002f)                                    // パターン3つ目のカメラの移動スピード
#define RANKING_CAMERA_1_ROT          (D3DXVECTOR3(0.0f,D3DX_PI / 2,0.0f))        // パターン3つ目のカメラ向き

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
int CRanking::m_nGameScore = 0;       // ゲームのスコア

//=============================================================================
//    コンストラクタ
//=============================================================================
CRanking::CRanking(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		m_aDigit[nCntRanking] = 0;                            // ランキングスコアの桁数
		m_apNumber[nCntRanking] = NULL;                       // ランキングスコア表示用のポリゴン
		m_Move[nCntRanking] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ランキングスコアの移動量
		m_apRankingRank[nCntRanking] = NULL;                  // ランキング順位表示用のポリゴン
		m_aRankingScore[nCntRanking] = 0;                     // ランキングスコア
		m_aScoreState[nCntRanking] = STATE_NONE;              // ランキングスコアの状態
	}
	m_pMap = NULL;              // マップクラスへのポインタ
	m_pTextureManager = NULL;   // テクスチャ管轄クラスへのポインタ
	m_pRankingLogo = NULL;      // ランキングロゴ用表示用のポリゴン
	m_nModeCounter = 0;         // 遷移を管理するカウンター
	m_nCameraCounter = 0;       // カメラの動きを管理するカウンター
	m_nPatternCamera = 0;       // 現在のランキングカメラのパターン番号
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRanking *CRanking::Create(void)
{
	CRanking *pRanking = NULL;  // ランキングクラス型のポインタ
	if (pRanking == NULL)
	{// メモリが確保できる状態にある
		pRanking = new CRanking;
		if (pRanking != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pRanking ->Init();
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
	return pRanking;
}

//=============================================================================
//    ゲームスコアの設定
//=============================================================================
void CRanking::SetGameScore(int nScore)
{
	m_nGameScore = nScore;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	// ランキングの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(RANKING_SYSTEM_FILENAME, "r");
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
			char aMapFileName[256] = "\0";              // マップデータのスクリプトファイル名
			char aRankingScoreFileName[256] = "\0";     // ランキングスコアのスクリプトファイル名

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
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// マップデータのスクリプトファイル名だった
							    // マップデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RANKINGSCORE_FILENAME) == 0)
							{// ランキングスコアのスクリプトファイル名だった
							    // ランキングスコアのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, RANKINGSCORE_FILENAME);
								strcpy(aRankingScoreFileName, pStr);

								// ランキングスコアを外部ファイルから読み込む
								FILE *pRankingScoreFile = NULL;  // ファイルポインタ
								int nCntRankingScore = 0;        // ランキングスコアを読み取った回数

								if (pRankingScoreFile == NULL)
								{// ファイルポインタが確保できる状態である
									pRankingScoreFile = fopen(aRankingScoreFileName, "r");
									if (pRankingScoreFile != NULL)
									{// ファイルが開けた
										pStrCur = CFunctionLib::ReadLine(pRankingScoreFile, pLine);  // 有効な文字列を見つける
										if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
										{// スクリプト読み込み開始の合図だった
											while (1)
											{// ループ開始
												pStrCur = CFunctionLib::ReadLine(pRankingScoreFile, pLine);  // 有効な文字列を見つける
												if (CFunctionLib::Memcmp(pStrCur, RANKING_SCORE) == 0)
												{// ランキングスコアだった
													m_aRankingScore[nCntRankingScore] = CFunctionLib::ReadInt(pStrCur, RANKING_SCORE);
													nCntRankingScore++;
												}
												else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
												{// スクリプト読み込み終了の合図だった
													break;  // ループ終了
												}
											}
										}

										// ファイルを閉じる
										fclose(pRankingScoreFile);
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// マップの生成
					m_pMap = CMap::Create(aMapFileName);

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

	// ランキングスコアが入れ替わっていないかチェックする
	int nScoreChange = m_nGameScore; // 入れ替えるスコア
	int nScoreOld = 0;               // ランキングスコアの一時的な格納場所
	bool bSet = false;               // ランキングが更新されたかどうか

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_aRankingScore[nCntRanking] < nScoreChange)
		{// 登録されているスコアより入れ替えるスコアが大きい
			nScoreOld = m_aRankingScore[nCntRanking];    // 元のスコアを格納
			m_aRankingScore[nCntRanking] = nScoreChange; // スコア更新
			nScoreChange = nScoreOld;                    // 元のスコアを入れ替える対象に
			if (bSet == false)
			{// 更新されていなかったら
				m_aScoreState[nCntRanking] = STATE_UPDATE;     // 更新された状態に
				bSet = true;                                   // 更新した状態に
			}
		}
	}


	// ランキングスコアの生成
	D3DXVECTOR3 NumberPos = RANKING_SCORE_POS_INI;     // 座標
	float fNumberWidth = RANKING_SCORE_WIDTH_INI;      // 幅
	float fNumberHeight = RANKING_SCORE_HEIGHT_INI;    // 高さ
	int nNumber = 0;                                   // 表示する数字の計算用
	CNumber::STATE NumberState = CNumber::STATE_NONE;  // 状態
	D3DXCOLOR NumberCol[MAX_RANKING] =                 // 色
	{
		RANKING_SCORE_COL_0,
		RANKING_SCORE_COL_1,
		RANKING_SCORE_COL_2,
		RANKING_SCORE_COL_3,
		RANKING_SCORE_COL_4,
	};

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		// 状態の設定
		if (m_aScoreState[nCntRanking] == STATE_UPDATE)
		{// 更新された状態だったら
			NumberState = CNumber::STATE_FLASH;
		}

		// 桁数の計算
		m_aDigit[nCntRanking] = (int)log10f((float)m_aRankingScore[nCntRanking]) + 1;  // 桁数を求める
		if (m_aDigit[nCntRanking] <= 0) { m_aDigit[nCntRanking] = 1; }	               // 0以下のとき1にする
		m_apNumber[nCntRanking] = new CNumber*[m_aDigit[nCntRanking]];		           // 桁数分メモリ確保
		if (m_apNumber[nCntRanking] != NULL)
		{// メモリが確保されている
			for (int nCntScore = 0; nCntScore < m_aDigit[nCntRanking]; nCntScore++)
			{// ランキングスコアの桁数分繰り返し
				// 表示する数字の計算
				nNumber = m_aRankingScore[nCntRanking] % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

				// 数字ポリゴンのインスタンス生成処理
				m_apNumber[nCntRanking][nCntScore] = CNumber::Create(NumberPos, NumberCol[nCntRanking], fNumberWidth, fNumberHeight, 0.0f, NumberState, nNumber, 3, RANKING_POLYGON_PRIORITY);
				if (m_apNumber[nCntRanking][nCntScore] != NULL)
				{// メモリを確保できた
					m_apNumber[nCntRanking][nCntScore]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
				}

				// 横に進める
				NumberPos.x -= fNumberWidth + (fNumberWidth * RANKING_NUMBER_INTERVAL);
			}
		}
		// 下に進める
		NumberPos = RANKING_SCORE_POS_INI;
		NumberPos.x += 500.0f * (nCntRanking + 1);
		NumberPos.y += (RANKING_SCORE_HEIGHT_INI + (RANKING_SCORE_HEIGHT_INI * 2.0f)) * (nCntRanking + 1);

		if (nCntRanking == 2)
		{// ランキングが4つめになった
			fNumberWidth *= 0.75f;
			fNumberHeight *= 0.75f;
		}
		// 状態を戻しておく
		NumberState = CNumber::STATE_NONE;
	}

	// ランキング順位ポリゴンのインスタンス生成
	D3DXVECTOR3 RankPos = RANKING_RANK_POS_INI;   // 座標
	float fRankWidth = RANKING_RANK_WIDTH_INI;    // 横幅
	float fRankHeight = RANKING_RANK_HEIGHT_INI;  // 縦幅
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
	    // 移動量の設定
		m_Move[nCntRanking] = RANKING_SCORE_MOVE_INI;

		if (m_apRankingRank[nCntRanking] == NULL)
		{// メモリが確保できる状態にある
			m_apRankingRank[nCntRanking] = CScene2D::Create(RankPos, RANKING_RANK_COL_INI, fRankWidth, fRankHeight, 0.0f, RANKING_POLYGON_PRIORITY);
			if (m_apRankingRank[nCntRanking] != NULL)
			{// メモリを確保できた
				m_apRankingRank[nCntRanking]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));

				// テクスチャ座標をずらす
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apRankingRank[nCntRanking]->GetVtxBuff();
				if(pVtxBuff != NULL)
				{// 頂点バッファを取得できた
				    // 頂点情報
					VERTEX_2D *pVtx;

					// 頂点バッファをロックし,頂点データへのポインタを取得
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// テクスチャ座標
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f + (0.2f * nCntRanking));
					pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f + (0.2f * nCntRanking));

					// 頂点バッファをアンロックする
					pVtxBuff->Unlock();
				}
			}
			RankPos = RANKING_RANK_POS_INI;
			RankPos.x += 500.0f * (nCntRanking + 1);
			RankPos.y += (RANKING_RANK_HEIGHT_INI + (RANKING_RANK_HEIGHT_INI * 0.75f)) * (nCntRanking + 1);

			if (nCntRanking == 2)
			{// ランキングが4つめになった
				fRankWidth *= 0.75f;
				fRankHeight *= 0.75f;
			}
		}
	}

	// ランキングロゴの生成
	if (m_pRankingLogo == NULL)
	{
		m_pRankingLogo = CScene2D::Create(RANKING_LOGO_POS_INI, RANKING_LOGO_COL_INI, RANKING_LOGO_WIDTH_INI, RANKING_LOGO_HEIGHT_INI, 0.0f, RANKING_POLYGON_PRIORITY);
		if (m_pRankingLogo != NULL)
		{// メモリを確保できた
			m_pRankingLogo->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_2));
		}
	}

	// カメラの注視点の位置を設定
	CManager::GetCamera()->SetPosR(RANKING_CAMERA_START_POSR);

	// カメラの目的の注視点の位置と現在の位置との差分を設定
	CManager::GetCamera()->SetPosRDiff((RANKING_CAMERA_DEST_POSR - RANKING_CAMERA_START_POSR) * RANKING_CAMERA_MOVESPEED);

	// カメラの視点の高さを設定
	CManager::GetCamera()->SetPosVDest(RANKING_CAMERA_START_POSV_Y);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	// ゲームのスコアを初期化する
	m_nGameScore = 0;

	// マップクラスを開放する
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// ランキングスコアの破棄
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		if (m_apNumber[nCntRanking] != NULL)
		{// メモリが確保されている
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
			{// ランキングスコアの桁数分繰り返し
				if (m_apNumber[nCntRanking][nCntDigit] != NULL)
				{// メモリが確保されている
					m_apNumber[nCntRanking][nCntDigit]->Uninit();
					m_apNumber[nCntRanking][nCntDigit] = NULL;
				}
			}
			delete m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}
	}

	// ランキングロゴの破棄
	if (m_pRankingLogo != NULL)
	{
		m_pRankingLogo->Uninit();
		m_pRankingLogo = NULL;
	}

	// ランキング順位の破棄
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_apRankingRank[nCntRanking] != NULL)
		{// メモリが確保されている
			m_apRankingRank[nCntRanking]->Uninit();
			m_apRankingRank[nCntRanking] = NULL;
		}
	}

	// ランキングスコアの保存
	Save();

	// ランキングオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRanking::Update(void)
{
	// デバッグ用
	//if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	//{// フェードの状態が使用されていない状態である
	//	CManager::GetFade()->SetFade(CManager::MODE_TITLE);
	//}

	// カメラを動かす
	SwitchCamera();

	// ランキングをスライドさせる
	SlideRanking();

	m_nModeCounter++;  // カウンターを進める

	if (m_nModeCounter >= RANKING_MODECHANGE_TIMING)
	{// カウンターが既定の値を超えた
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	else
	{// カウンターがまだ既定の値を超えていない
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						CManager::GetFade()->SetFade(CManager::MODE_TITLE);
					}
				}
			}
		}
	}

	CDebugProc::Print("ランキング画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
//    カメラのパターンを切り替える処理
//=============================================================================
void CRanking::SwitchCamera(void)
{
	// カウンターを増やす
	m_nCameraCounter++;

	int nSwitchTiming = 0;   // カメラのパターンを切り替えるタイミング
	switch (m_nPatternCamera)
	{// 現在のランキングカメラのパターンによって処理わけ
	case 0:   // 最初のパターン
		nSwitchTiming = RANKING_CAMERA_SWITCHTIMING_0;
		break;
	case 1:   // 最初のパターン
		nSwitchTiming = RANKING_CAMERA_SWITCHTIMING_1;
		break;
	}

	if (nSwitchTiming > 0)
	{// 切り替えるタイミングが設定された
		if (m_nCameraCounter % nSwitchTiming == 0)
		{// 切り替えるタイミングになっている
		    // カウンターを戻す
			m_nCameraCounter = 0;

			// パターン番号によって設定する値を変える
			D3DXVECTOR3 PosVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fMoveSpeed = 0.0f;
			D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (m_nPatternCamera)
			{// 現在のタイトルカメラのパターンによって処理わけ
			case 0:   // 最初のパターン
				PosVDest = RANKING_CAMERA_0_POSV_Y;
				PosR = RANKING_CAMERA_0_POSR;
				PosRDiff = (RANKING_CAMERA_0_POSRDEST - RANKING_CAMERA_0_POSR);
				fMoveSpeed = RANKING_CAMERA_0_MOVESPEED;
				Rot = RANKING_CAMERA_0_ROT;
				break;
			case 1:   // 最初のパターン
				PosVDest = RANKING_CAMERA_1_POSV_Y;
				PosR = RANKING_CAMERA_1_POSR;
				PosRDiff = (RANKING_CAMERA_1_POSRDEST - RANKING_CAMERA_1_POSR);
				fMoveSpeed = RANKING_CAMERA_1_MOVESPEED;
				Rot = RANKING_CAMERA_1_ROT;
				break;
			}

			// カメラの視点の高さを設定
			CManager::GetCamera()->SetPosVDest(PosVDest);

			// カメラの注視点の位置を設定
			CManager::GetCamera()->SetPosR(PosR);

			// カメラの目的の注視点の位置と現在の位置との差分を設定
			CManager::GetCamera()->SetPosRDiff(PosRDiff * fMoveSpeed);

			// カメラの向きを設定
			CManager::GetCamera()->SetRot(Rot);

			// パターン番号を進める
			m_nPatternCamera++;
		}
	}
}

//=============================================================================
//    ランキングを横からスライドさせる処理
//=============================================================================
void CRanking::SlideRanking(void)
{
	D3DXVECTOR3 RankPos;
	D3DXVECTOR3 NumberPos;

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの数分繰り返し
		// 順位ポリゴンを動かす
		if (m_apRankingRank[nCntRanking] != NULL)
		{// メモリが確保されている
			RankPos = m_apRankingRank[nCntRanking]->GetPos();
			RankPos += m_Move[nCntRanking];
			m_apRankingRank[nCntRanking]->SetPos(RankPos);
		}
		// 数字ポリゴンを動かす
		if (m_apNumber[nCntRanking] != NULL)
		{// メモリが確保されている
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
			{// 桁数分繰り返し
				if (m_apNumber[nCntRanking][nCntDigit] != NULL)
				{// メモリが確保されている
					NumberPos = m_apNumber[nCntRanking][nCntDigit]->GetPos();
					NumberPos += m_Move[nCntRanking];
					m_apNumber[nCntRanking][nCntDigit]->SetPos(NumberPos);
				}
			}
			if (RankPos.x <= RANKING_RANK_POS_MAX)
			{// これ以上横に行けない
				if (m_Move[nCntRanking].x != 0.0f)
				{// これ以上横に行けない
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RANKING);
				}

				m_Move[nCntRanking].x = 0.0f;

				// 位置の再設定
				RankPos.x = RANKING_RANK_POS_MAX;
				NumberPos.x = RANKING_SCORE_POS_MAX;

				// 順位ポリゴン
				m_apRankingRank[nCntRanking]->SetPos(RankPos);

				// 数字ポリゴン
				for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
				{// 桁数分繰り返し
					if (m_apNumber[nCntRanking][nCntDigit] != NULL)
					{// メモリが確保されている
						m_apNumber[nCntRanking][nCntDigit]->SetPos(NumberPos);
				        // 横に進める
						NumberPos.x -= m_apNumber[nCntRanking][nCntDigit]->GetWidth() + (m_apNumber[nCntRanking][nCntDigit]->GetWidth() * RANKING_NUMBER_INTERVAL);
					}
				}
			}
			else
			{// まだ動かせる
				D3DXVECTOR3 ParEmitterPos = D3DXVECTOR3(m_apRankingRank[nCntRanking]->GetPos().x - m_apRankingRank[nCntRanking]->GetWidth() + 10.0f, m_apRankingRank[nCntRanking]->GetPos().y + m_apRankingRank[nCntRanking]->GetHeight() - 10.0f, 0.0f);
				CreateOrbit(ParEmitterPos, nCntRanking);
			}
		}
	}
}

//=============================================================================
//    軌跡のようなエフェクトを生成する処理
//=============================================================================
void CRanking::CreateOrbit(D3DXVECTOR3 pos, int nCntRanking)
{
	// パーティクルを出す
	CParticle2D *pPar2D = NULL;
	float fAngle = 0.0f;  // 角度
	float fMoveX = 0.0f;  // X方向の移動量
	float fMoveY = 0.0f;  // Y方向の移動量
	float fWidth = 0.0f;  // 横幅
	float fHeight = 0.0f; // 縦幅
	float fPosYDef = m_ParEmitterPos.y;
	float fParWidth = RANKING_ORBIT_WIDTH;
	float fParHeight = RANKING_ORBIT_HEIGHT;
	D3DXVECTOR3 ParEmitterPosDef = m_ParEmitterPos;
	D3DXCOLOR ParCol[MAX_RANKING] =
	{
		RANKING_PARTICLE_COL_0,
		RANKING_PARTICLE_COL_1,
		RANKING_PARTICLE_COL_2,
		RANKING_PARTICLE_COL_3,
		RANKING_PARTICLE_COL_4
	};

	if (nCntRanking >= 3)
	{
		fParHeight *= 0.5f;
	}

	// 放出場所を設定
	m_ParEmitterPos = pos;

	if (m_nModeCounter % 2 == 0)
	{
		for (int nCntPar = 0; nCntPar < RANKING_PARTICLE_APPEAR; nCntPar++)
		{// 出す分だけ繰り返し
		    // 事前にランダムで計算
			fAngle = (D3DX_PI * 0.5f) + ((rand() % 72 * 0.01f) - (rand() % 72 * 0.01f));
			fMoveX = sinf(fAngle) * (rand() % 5 + 5.0f);
			fMoveY = cosf(fAngle) * (rand() % 5 + 5.0f);
			fWidth = rand() % 10 + 25.0f;
			fHeight = rand() % 10 + 25.0f;
			pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(fMoveX, fMoveY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fWidth, fHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.03f), -0.4f, 0.0f, 60, 0, true, RANKING_POLYGON_PRIORITY - 1);
			if (pPar2D != NULL)
			{// メモリが確保できた
				pPar2D->BindTexture(m_pTextureManager->GetTexture(3));
			}
		}

		pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ParCol[nCntRanking], fParWidth, fParHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, RANKING_POLYGON_PRIORITY - 1);
		if (pPar2D != NULL)
		{// メモリが確保できた
			pPar2D->BindTexture(m_pTextureManager->GetTexture(4));
		}

		// 出す位置を動かす
		m_ParEmitterPos -= (m_Move[nCntRanking] * 0.3f);

		pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ParCol[nCntRanking], fParWidth, fParHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, RANKING_POLYGON_PRIORITY - 1);
		if (pPar2D != NULL)
		{// メモリが確保できた
			pPar2D->BindTexture(m_pTextureManager->GetTexture(4));
		}
	}
}

//=============================================================================
//    ランキングスコアの保存処理
//=============================================================================
void CRanking::Save(void)
{
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(RANKING_SCORE_FILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			// コメント部分を作成
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "# ランキングスコアファイル [ranking_score.txt]\n");
			fprintf(pFile, "# Author : HODAKA NIWA\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n",SCRIPT);


			// ランキングスコアの保存
			for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
			{// ランキングスコアの桁数分繰り返し
				fprintf(pFile, "%s%d		# %d位のランキングスコア\n", RANKING_SCORE, m_aRankingScore[nCntRanking], nCntRanking + 1);
			}

			// コメント部分を作成
			fprintf(pFile, "\n%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}