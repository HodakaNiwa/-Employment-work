//*****************************************************************************
//
//     ゲームの処理[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "fade.h"
#include "textureManager.h"
#include "pause.h"
#include "map.h"
#include "functionlib.h"
#include "player.h"
#include "enemy.h"
#include "effectManager.h"
#include "scene2D.h"
#include "ui.h"
#include "score.h"
#include "result.h"
#include "input.h"
#include "shadow.h"
#include "debuglog.h"
#include "river.h"
#include "grass.h"
#include "boxCollider.h"
#include "shadow.h"
#include "object.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define GAME_SET_MINIMAP            (true)                      // ミニマップを表示するかどうか
#define GAME_SET_UI                 (true)                      // UIを画面に表示するかどうか

#define GAME_SYSTEM_FILENAME        "data/TEXT/MODE/game.ini"   // ゲームの初期化情報スクリプトファイル名
#define GAME_MODECHANGE_TIMING      (60)                        // 画面遷移を自動で行うタイミング
#define GAME_MAPCLEAR_CHANGETIMING  (60)                        // マップクリア状態になってからマップを切り替えるまでの時間
#define GAME_POLYGON_PRIORITY       (11)                        // ゲームで使用するポリゴンの処理優先順位

#define GAME_STAGECLEAE_BONUS       (10000)                     // ステージクリア数１つ分のボーナス
#define GAME_COMBOMAX_BONUS         (100)                       // コンボ数１つ分のボーナス
#define GAME_HIDDENITEM_BONUS       (5000)                      // 隠しアイテム数１つ分のボーナス

#define GAME_PLAYER_POSITION_POS    (D3DXVECTOR3(1125.0f,105.0f,0.0f)) // ミニマップに表示するプレイヤーの位置把握用ポリゴンの座標
#define GAME_PLAYER_POSITION_WIDTH  (8.0f)                             // ミニマップに表示するプレイヤーの位置把握用ポリゴンの幅
#define GAME_PLAYER_POSITION_HEIGHT (8.0f)                             // ミニマップに表示するプレイヤーの位置把握用ポリゴンの高さ

#define GAME_DEST_POSITION_POS    (D3DXVECTOR3(1125.0f,10.0f,0.0f))    // ミニマップに表示する目的地把握用ポリゴンの座標
#define GAME_DEST_POSITION_WIDTH  (6.0f)                               // ミニマップに表示する目的地把握用ポリゴンの幅
#define GAME_DEST_POSITION_HEIGHT (6.0f)                               // ミニマップに表示する目的地把握用ポリゴンの高さ

// 値を読み取るパス名
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // 読み込むテクスチャの数
#define NUM_MAP                     "NUM_MAP = "                // 読み込むマップの数
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // 読み込むテクスチャのファイル名
#define EFFECT_FILENAME             "EFFECT_FILENAME = "        // エフェクトデータのスクリプトファイル名
#define MAP_FILENAME                "MAP_FILENAME = "           // マップデータのスクリプトファイル名
#define GAME_TIMER                  "GAME_TIMER = "             // タイマーの秒数

// プレイヤー情報
#define PLAYERSET                   "PLAYERSET"                 // プレイヤー情報読み込み開始の合図
#define END_PLAYERSET               "END_PLAYERSET"             // プレイヤー情報読み込み終了の合図
#define LIFE                        "LIFE = "                   // プレイヤーの体力
#define ORBIT_SLASH                 "ORBIT_SLASH = "            // プレイヤーの軌跡ゲージ量
#define MAX_SPECIAL                 "MAX_SPECIAL = "            // プレイヤーの必殺ゲージの最大値量
#define FILENAME                    "FILENAME = "               // プレイヤーデータのスクリプトファイル名

// 滝の音用マクロ
#define GAME_WATERFALL_SOUND_POS      (D3DXVECTOR3(1764.0f,-150.0f,4219.0f)) // 滝の音の座標
#define GAME_WATERFALL_RANGE          (4000.0f)                              // 滝の音が聞こえる範囲

// ボス出現時の演出用マクロ
#define GAME_BOSSAPPAER_CAMERA_POSV   (D3DXVECTOR3(100.0f,20.0f,-200.0f))    // ボス出現時のカメラ視点の位置
#define GAME_BOSSAPPAER_CAMERA_POSR   (D3DXVECTOR3(0.0f,80.0,0.0f))          // ボス出現時のカメラ注視点の位置
#define GAME_BOSSAPPEAR_LOGO_TIMING_0 (120)                                  // ロゴを出すタイミング(1つ目)
#define GAME_BOSSAPPEAR_LOGO_TIMING_1 (180)                                  // ロゴを出すタイミング(2つ目)
#define GAME_BOSSAPPEAR_EFFECT_TIMING (240)                                  // エフェクトを出すタイミング
#define GAME_BOSSAPPEAR_STATE_TIMING  (380)                                  // 状態を切り替えるタイミング

// ボス撃破時の演出用マクロ
#define GAME_KNOCKDOWN_CAMERA_TIMING  (180)                                  // ボス撃破時のカメラを通常時に戻すタイミング
#define GAME_KNOCKDOWN_CAMERA_LENGTH  (300.0f)                               // ボス撃破時のカメラの長さ
#define GAME_KNOCKDOWN_CAMERA_POSY    (280.0f)                               // ボス撃破時のカメラのY座標

// ボス出現時の文字ポリゴン用マクロ(1つ目)
#define GAME_BOSSAPPEAR_POS_0         (D3DXVECTOR3(370.0f,350.0f,0.0f))      // ボス出現時の文字ポリゴンの座標(1つ目)
#define GAME_BOSSAPPEAR_COL_0         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // ボス出現時の文字ポリゴンの色(1つ目)
#define GAME_BOSSAPPEAR_WIDTH_0       (330.0f)                               // ボス出現時の文字ポリゴンの幅(1つ目)
#define GAME_BOSSAPPEAR_HEIGHT_0      (180.0f)                               // ボス出現時の文字ポリゴンの高さ(1つ目)

// ボス出現時の文字ポリゴン用マクロ(1つ目)
#define GAME_BOSSAPPEAR_POS_1         (D3DXVECTOR3(970.0f,350.0f,0.0f))      // ボス出現時の文字ポリゴンの座標(2つ目)
#define GAME_BOSSAPPEAR_COL_1         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // ボス出現時の文字ポリゴンの色(2つ目)
#define GAME_BOSSAPPEAR_WIDTH_1       (330.0f)                               // ボス出現時の文字ポリゴンの幅(2つ目)
#define GAME_BOSSAPPEAR_HEIGHT_1      (180.0f)                               // ボス出現時の文字ポリゴンの高さ(2つ目)

// 草情報読み込み用マクロ
#define GRASSSET                      "GRASSSET"                             // 草情報読み込み開始
#define SWAY                          "SWAY = "                              // 草が揺れるかどうか
#define POS                           "POS = "                               // 座標情報
#define ROT                           "ROT = "                               // 向き情報
#define WIDTH                         "WIDTH = "                             // 幅情報
#define HEIGHT                        "HEIGHT = "                            // 高さ情報
#define END_GRASSSET                  "END_GRASSSET"                         // 草情報読み込み終了

// 当たり判定用箱モデル情報読み込み用マクロ
#define BOXCOLLIDERSET                "BOXCOLLIDERSET"                       // 当たり判定用箱モデル情報読み込み開始
#define DEPTH                         "DEPTH = "                             // 箱の奥行情報
#define RETURN                        "RETURN = "                            // 当たった際に押し戻すかどうか
#define END_BOXCOLLIDERSET            "END_BOXCOLLIDERSET"                   // 当たり判定用箱モデル情報読み込み終了

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CGame::CGame(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pPlayer = NULL;                           // プレイヤークラスへのポインタ保存用
	m_pUi = NULL;                               // UIクラスへのポインタ保存用
	m_pEffectManager = NULL;                    // エフェクト管轄クラスへのポインタ保存用
	m_pMap = NULL;                              // マップクラスへのポインタ
	m_State = STATE_NORMAL;                     // 状態
	m_nStateCounter = 0;                        // 状態を管理するカウンター
	m_pTextureManager = NULL;                   // テクスチャ管轄クラスへのポインタ
	m_nClearStageNum = 0;                       // クリアしたステージの数
	m_nGetHiddenItem = 0;                       // 手に入れた隠しアイテムの数
	m_nComboMax = 0;                            // ゲームを通した最大コンボ数
	m_nNumMap = 0;                              // マップを読み込む数
	m_nMapIdx = 0;                              // 現在のマップ番号
	m_bMapClear = false;                        // マップをクリアしたかどうか
	m_bMiniMapDisp = false;                     // ミニマップを描画するかどうか
	m_nBossAppearCounter = 0;                   // ボス出現時の演出を管理するカウンター
	m_nKnockDownCounter = 0;                    // ボス撃破時の演出を管理するカウンター
	m_BossPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ボスの座標(保存用)
	m_pDestPosition = NULL;
	m_pPlayerPosition = NULL;
	for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
	{// ロゴを出す数だけ繰り返し
		m_pBossAppearLogo[nCntAppear] = NULL;
	}
	for (int nCntMap = 0; nCntMap < GAME_MAX_MAPLOAD; nCntMap++)
	{// マップを読み込める回数分繰り返し
		strcpy(m_aMapFileName[nCntMap], "\0");
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;  // ゲームクラス型のポインタ
	if (pGame == NULL)
	{// メモリが確保できる状態にある
		pGame = new CGame;
		if (pGame != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pGame->Init();
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
	return pGame;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	// 影モデル読み込み
	CShadow::Load();

	// ゲームの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(GAME_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			int nCntMap = 0;                            // マップを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			char aTexFileName[256] = "\0";              // テクスチャのファイル名
			char aPlayerFileName[256] = "\0";           // プレイヤーデータのスクリプトファイル名
			char aEffectFileName[256] = "\0";           // エフェクトデータのスクリプトファイル名
			int nTimer = 10 * 60;                       // タイマーの秒数
			int nPlayerLife = 0;                        // プレイヤーの体力
			float fPlayerOrbitSlash = 0.0f;             // プレイヤーの軌跡ゲージ量
			float fPlayerMaxSpecial = 0.0f;             // プレイヤーの必殺ゲージの最大値

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
							else if (CFunctionLib::Memcmp(pStrCur, PLAYERSET) == 0)
							{// プレイヤーデータ読み込み開始の合図だった
								while (1)
								{// ループ開始
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
									if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
									{// 体力読み込みの合図だった
										nPlayerLife = CFunctionLib::ReadInt(pStrCur, LIFE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, ORBIT_SLASH) == 0)
									{// 軌跡ゲージ量読み込みの合図だった
										fPlayerOrbitSlash = CFunctionLib::ReadFloat(pStrCur, ORBIT_SLASH);
									}
									else if (CFunctionLib::Memcmp(pStrCur, MAX_SPECIAL) == 0)
									{// 必殺ゲージの最大値量読み込みの合図だった
										fPlayerMaxSpecial = CFunctionLib::ReadFloat(pStrCur, MAX_SPECIAL);
									}
									else if (CFunctionLib::Memcmp(pStrCur, FILENAME) == 0)
									{// プレイヤーデータのスクリプトファイル名だった
										// プレイヤーデータのスクリプトファイル名を読み取る
										pStr = CFunctionLib::ReadString(pStrCur, pStr, FILENAME);
										strcpy(aPlayerFileName, pStr);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_PLAYERSET) == 0)
									{// プレイヤー情報読み込み終了の合図だった
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// エフェクトデータのスクリプトファイル名だった
								// エフェクトデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MAP) == 0)
							{// マップの個数情報があった
								m_nNumMap = CFunctionLib::ReadInt(pStrCur, NUM_MAP);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// マップデータのスクリプトファイル名だった
								// マップデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(m_aMapFileName[nCntMap], pStr);
								nCntMap++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, GAME_TIMER) == 0)
							{// タイマーの秒数だった
								nTimer = CFunctionLib::ReadInt(pStrCur, GAME_TIMER);
								nTimer *= 60;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}


					// 必要なクラスを作成する
					// UI
					if (GAME_SET_UI)
					{
						m_pUi = CUI::Create(nTimer, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial, m_pTextureManager);
					}

					// エフェクト管轄クラス
					m_pEffectManager = CEffectManager::Create(aEffectFileName);

					// マップ
					m_pMap = CMap::Create(m_aMapFileName[0]);

					// プレイヤー
					CPlayerManager::Create(aPlayerFileName, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial);

					if (m_nClearStageNum == 0)
					{// まだ1マップ目だった
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WATER_FALL);
						CManager::GetSound()->SetVoluem(CSound::SOUND_LABEL_SE_WATER_FALL, 0.0f);
					}

					// プレイヤークラスへのポインタを保存しておく
					// プレイヤークラスを取得する
					CScene *pScene = NULL;               // シーンクラスへのポインタ
					CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
					for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
					{// 描画優先順位の数だけ繰り返し
						pScene = CScene::GetTop(nCntPriority);
						while (pScene != NULL)
						{// メモリが空になるまで繰り返し
							pSceneNext = pScene->GetNext();
							if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
							{// プレイヤークラス型のポインタだったら
								m_pPlayer = (CPlayer*)pScene;
								m_pPlayer->SetPos(m_pMap->GetPlayerDefaultPos());
								break;
							}
							pScene = pSceneNext;
						}
					}

					if(m_nMapIdx == 0)
					{// 1マップ目だった
						CRiver *pRiver = NULL;
						pRiver = CRiver::Create(D3DXVECTOR3(426.5f, -59.8f, 4243.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 420.5f, 1196.0f, 2, 1, 1, 1, 0.001f, 0.0f, NULL, RIVER_PRIORITY);
						if (pRiver != NULL)
						{// 川を生成できた
							pRiver->BindTexture(m_pMap->GetTextureManager()->GetTexture(5));
						    // 頂点情報の設定
							VERTEX_3D *pVtx;
							LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pRiver->GetVtxBuff();
							if (pVtxBuff != NULL)
							{// 頂点バッファが生成されている
							    // 頂点バッファをロックし,頂点データへのポインタを取得
								pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

								// 生成に必要な変数を宣言
								D3DXCOLOR col = pRiver->GetCol();
								int nXBlock = pRiver->GetXBlock();
								int nZBlock = pRiver->GetZBlock();
								float fWidth = pRiver->GetWidth();
								float fHeight = pRiver->GetHeight();
								int fTexSplitU = pRiver->GetTexSplitU();
								int fTexSplitV = pRiver->GetTexSplitV();
 								float XPos = -(fWidth * nXBlock) / 2;  // X座標を左に設定
								float ZPos = (fHeight * nZBlock) / 2;  // Z座標を奥に設定
								float fTexU = 0.0f;                    // テクスチャのU座標を右上に設定
								float fTexV = 0.0f;                    // テクスチャのV座標を右上に設定
								float YPos = 0.0f;                     // Y座標

								for (int nCntV = 0; nCntV < nZBlock + 1; nCntV++)
								{// 垂直方向の分割数 + 1だけ繰り返し
									for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
									{// 水平方向の分割数 + 1だけ繰り返し
									    // 頂点座標
										pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

										// 頂点カラー
										pVtx[nCntH].col = col;

										// テクスチャ座標
										pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

										XPos += fWidth;                            // X座標を横に進める
										fTexU += (1.0f / (nXBlock)) * fTexSplitU;  // テクスチャU座標を進める
									}
									XPos -= fWidth * (nXBlock + 1);                               // X座標を進めていた分戻す
									ZPos -= fHeight;                                              // Z座標を手前に進める
									fTexU -= ((1.0f / (nXBlock)) * fTexSplitU) * (nXBlock + 1);   // テクスチャU座標を進めていた分戻す
									fTexV += ((1.0f / nZBlock) * fTexSplitV);                     // テクスチャV座標を進める

									// ポインタを進める
									pVtx += nXBlock + 1;
								}

								// 頂点バッファをアンロックする
								pVtxBuff->Unlock();
							}
						}

						//// 草の配置(ランダム)
						//D3DXVECTOR3 GrassPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						//D3DXVECTOR3 GrassRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						//bool bSway = false;
						//int nSway = 0;
						//float fGrassWidth = 0.0f;
						//float fGrassHeight = 0.0f;
						//for (int nCntGrass = 0; nCntGrass < 500; nCntGrass++)
						//{
						//	nSway = rand() % 2;
						//	if (nSway == 0)
						//	{
						//		bSway = false;
						//	}
						//	else if (nSway == 1)
						//	{
						//		bSway = true;
						//	}
						//	GrassPos.x = (rand() % 530) + 1200.0f + (rand() % 150 - 75);
						//	GrassPos.z = (rand() % 500) - 200.0f + (rand() % 150 - 75);
						//	GrassRot.y = ((rand() % 628) * 0.01f) - D3DX_PI;
						//	fGrassWidth = rand() % 15 + 15.0f;
						//	fGrassHeight = rand() % 10 + 20.0f;
						//	CScene *pScene = NULL;               // シーンクラスへのポインタ
						//	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
						//	CMeshField *pField = NULL;           // 地面クラスへのポインタ
						//	bool bFieldLand = false;             // 地面に乗っているかどうか
						//	float fFieldHeight = 0.0f;           // 地面の高さ

						//	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < FIELD_PRIORITY + 1; nCntPriority++)
						//	{// 描画優先順位の数だけ繰り返し
						//		pScene = CScene::GetTop(nCntPriority);
						//		while (pScene != NULL)
						//		{// メモリが空になるまで繰り返し
						//			pSceneNext = pScene->GetNext();
						//			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
						//			{// 地面クラスへのポインタだったら
						//				if (bFieldLand != true)
						//				{// まだ地面に乗っていない
						//					pField = (CMeshField*)pScene;
						//					if (pField != NULL)
						//					{// 地面が取得できた
						//						fFieldHeight = pField->GetPolyHeight(GrassPos, &bFieldLand, NULL);
						//						if (bFieldLand == true)
						//						{// 地面の中にいる
						//							GrassPos.y = fFieldHeight;
						//						}
						//					}
						//				}
						//			}
						//			pScene = pSceneNext;
						//		}
						//	}
						//	// 草を配置する
						//	CGrass *pGrass = CGrass::Create(D3DXVECTOR3(GrassPos.x, GrassPos.y + 11.0f, GrassPos.z), GrassRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fGrassWidth, fGrassHeight, bSway, 9);
						//	if (pGrass != NULL)
						//	{
						//		pGrass->BindTexture(m_pMap->GetTextureManager()->GetTexture(9));
						//	}
						//}
					}

					// プレイヤーの場所を表示するポリゴンを生成する
					m_pPlayerPosition = CScene2D::Create(GAME_PLAYER_POSITION_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GAME_PLAYER_POSITION_WIDTH, GAME_PLAYER_POSITION_HEIGHT, 0.0f, GAME_POLYGON_PRIORITY);
					if (m_pPlayerPosition != NULL)
					{
						m_pPlayerPosition->BindTexture(m_pTextureManager->GetTexture(6));
					}

					// 目的地を表示するポリゴンを生成する
					m_pDestPosition = CScene2D::Create(GAME_DEST_POSITION_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GAME_DEST_POSITION_WIDTH, GAME_DEST_POSITION_HEIGHT, 0.0f, GAME_POLYGON_PRIORITY);
					if (m_pDestPosition != NULL)
					{
						m_pDestPosition->BindTexture(m_pTextureManager->GetTexture(20));
					}

					// 草の配置
					LoadGrassTransform(pStrCur, pLine);

					// 橋の当たり判定を設定する
					SetBridgeCollider();

					// ミニマップを描画するかどうか設定する
					m_bMiniMapDisp = GAME_SET_MINIMAP;

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

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// 草の配置情報を保存する
	//SaveGrassTransform();

	// 滝の音を止める
	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WATER_FALL);

	// 影モデル開放
	CShadow::UnLoad();

	// プレイヤークラスへのポインタを空にする
	m_pPlayer = NULL;

	// エフェクト管轄クラスへのポインタを空にする
	m_pEffectManager = NULL;

	// UIクラスを開放する
	if (m_pUi != NULL)
	{
		m_pUi->Uninit();

		delete m_pUi;
		m_pUi = NULL;
	}

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

	// ゲームオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGame::Update(void)
{
	// ミニマップカメラを更新する
	CMapCamera *pMapCamera = CManager::GetMapCamera();
	if (pMapCamera != NULL)
	{
		pMapCamera->Update();
	}

	// UIを更新する
	if (m_pUi != NULL)
	{
		m_pUi->Update();
	}

	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NORMAL:        // 通常状態
		if (CManager::GetKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_nMapIdx = 0;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_8) == true)
		{
			m_nMapIdx = 1;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_9) == true)
		{
			m_nMapIdx = 2;
			SetMap();
		}

#ifdef _DEBUG

		if (CManager::GetKeyboard()->GetTrigger(DIK_F9) == true)
		{
			m_State = STATE_END;
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_6) == true)
		{
			if (m_pUi != NULL)
			{
				m_pUi->SetDisp(m_pUi->GetDisp() ? false : true);
			}
			m_bMiniMapDisp = m_bMiniMapDisp ? false : true;
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_nMapIdx = 0;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_8) == true)
		{
			m_nMapIdx = 1;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_9) == true)
		{
			m_nMapIdx = 2;
			SetMap();
		}
#endif

		break;
	case STATE_MAPCHANGE:       // マップ切り替え状態
		CDebugProc::Print("マップ切り替え中\n");
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CFade::TYPE_SWITCHMAP);
		}
		break;
	case STATE_MAPCLEAR:        // マップクリア状態
		CDebugProc::Print("マップクリアー\n");
		ClearMapEvent();
		break;
	case STATE_BOSSAPPEAR:      // ボスが出現した状態
		CDebugProc::Print("ボス出現中\n");
		BossAppearEvent();
		break;
	case STATE_KNOCKDOWNBOSS:   // ボスが死んだ状態状態
		CDebugProc::Print("ボスが死にましたn");
		KonckDownBossEvent();
		break;
	case STATE_END:             // 終了状態
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_MODECHANGE_TIMING)
		{// カウンターが既定の値に達した
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードの状態が使用されていない状態である
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);

				// リザルトに必要な数値を渡しておく
				if (m_pUi != NULL)
				{
					CResult::SetHiddenItemBonus(m_nGetHiddenItem * GAME_HIDDENITEM_BONUS);
					CResult::SetComboMaxBonus(m_nComboMax * GAME_COMBOMAX_BONUS);
					CResult::SetStageClearBonus(m_nClearStageNum * GAME_STAGECLEAE_BONUS);
					CResult::SetGameScore(m_pUi->GetScore()->GetScore());
				}
			}
		}
		break;
	}

	CDebugProc::Print("ゲーム画面\n");
	CDebugProc::Print("\n");

	if (m_nClearStageNum == 0)
	{// まだ1マップ目だった
		if(m_pPlayer != NULL)
		{// プレイヤーが取得できている
			D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
			D3DXVECTOR3 WaterPos = GAME_WATERFALL_SOUND_POS;
			float fLength = sqrtf((PlayerPos.x - WaterPos.x) * (PlayerPos.x - WaterPos.x) + (PlayerPos.y - WaterPos.y) * (PlayerPos.y - WaterPos.y) + (PlayerPos.z - WaterPos.z) * (PlayerPos.z - WaterPos.z));
			if (fLength < GAME_WATERFALL_RANGE)
			{// 滝の音の範囲内である
				float fRivision = fLength / GAME_WATERFALL_RANGE;
				float fValue = cosf((D3DX_PI * 0.5f) * fRivision);
				CManager::GetSound()->SetVoluem(CSound::SOUND_LABEL_SE_WATER_FALL, fValue);
			}
			else
			{// 滝の音の範囲外である
				CManager::GetSound()->SetVoluem(CSound::SOUND_LABEL_SE_WATER_FALL, 0.0f);
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    ポーズクラスにテクスチャを設定する処理
//=============================================================================
void CGame::BindPauseTexture(CPause *pPause)
{
	if (pPause != NULL)
	{// メモリが確保されている
		// ポーズ背景用のテクスチャを設定する
		pPause->GetPauseBg()->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));

		// 選択項目用のテクスチャを設定する
		int nCntScene2DFlash = 0;
		for (int nCntTex = TEX_NUMBER_1; nCntTex < TEX_NUMBER_1 + 3; nCntTex++, nCntScene2DFlash++)
		{// テクスチャを読み込む回数繰り返し
			pPause->GetScene2DFlash(nCntScene2DFlash)->BindTexture(m_pTextureManager->GetTexture(nCntTex));
		}

		// 操作方法用のテクスチャを設定する
		int nCntOpeInfo = 0;
		for (int nCntTex = 16; nCntTex < 18; nCntTex++, nCntOpeInfo++)
		{// テクスチャを読み込む数だけ繰り返し
			pPause->GetOpeInfo(nCntOpeInfo)->BindTexture(m_pTextureManager->GetTexture(nCntTex));
		}
	}
}

//=============================================================================
//    マップを切り替える処理
//=============================================================================
void CGame::SetMap(void)
{
	m_State = STATE_NORMAL;  // 状態を戻しておく
	m_nStateCounter = 0;     // カウンターを戻す
	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WATER_FALL);

	if (m_nMapIdx == 0)
	{// 1マップ目に切り替えだったら
		if (m_pUi != NULL)
		{
			if (m_pUi->GetNumStage() != NULL)
			{
				m_pUi->GetNumStage()->BindTexture(m_pTextureManager->GetTexture(10));
			}
		}
	}
	else if (m_nMapIdx == 1)
	{// 2マップ目に切り替えだったら
		if (m_pUi != NULL)
		{
			if (m_pUi->GetNumStage() != NULL)
			{
				m_pUi->GetNumStage()->BindTexture(m_pTextureManager->GetTexture(11));
			}
		}
	}
	else if (m_nMapIdx == 2)
	{// 3マップ目に切り替えだったら
		if (m_pUi != NULL)
		{
			if (m_pUi->GetPurpose() != NULL)
			{
				m_pUi->GetPurpose()->BindTexture(m_pTextureManager->GetTexture(14));
			}
			if (m_pUi->GetNumStage() != NULL)
			{
				m_pUi->GetNumStage()->BindTexture(m_pTextureManager->GetTexture(12));
			}
		}
		// BGMを停止
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM002);

		// ボス出現状態にする
		m_State = STATE_BOSSAPPEAR;

		// UIの表示を消す
		if (m_pUi != NULL)
		{
			m_pUi->SetDisp(false);
		}
		m_bMiniMapDisp = false;
		if (m_pPlayerPosition != NULL)
		{// メモリが確保されている
			m_pPlayerPosition->Uninit();
			m_pPlayerPosition = NULL;
		}
		if (m_pDestPosition != NULL)
		{// メモリが確保されている
			m_pDestPosition->Uninit();
			m_pDestPosition = NULL;
		}
	}

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}

	// マップの生成
	if (m_pMap == NULL)
	{// メモリが確保できる状態にある
		m_pMap = CMap::Create(m_aMapFileName[m_nMapIdx]);
		if (m_pMap != NULL && m_pPlayer != NULL)
		{
			m_pPlayer->SetPos(m_pMap->GetPlayerDefaultPos());
		}
	}

	if (m_nMapIdx == 0)
	{// 1マップ目だったら
		// 橋の当たり判定を設定する
		SetBridgeCollider();
	}

	// 目的のUIを再表示
	if (m_pUi != NULL)
	{
		m_pUi->ResetPurpose();
	}

	// カメラの位置を再設定
	CManager::GetCamera()->SetDefaultPos(m_pMap->GetPlayerDefaultPos());

	m_bMapClear = false;    // マップを未クリア状態にする
}

//=============================================================================
//    マップをクリアしたときの処理
//=============================================================================
void CGame::ClearMapEvent(void)
{
	m_nStateCounter++;
	if (m_nStateCounter >= GAME_MAPCLEAR_CHANGETIMING == 0)
	{// マップ切り替えのタイミングになった
		SetState(CGame::STATE_MAPCHANGE);
		m_nClearStageNum++;
		m_nMapIdx++;
	}
}

//=============================================================================
//    ミニマップを描画する処理
//=============================================================================
void CGame::MiniMapDraw(void)
{
	if (m_bMiniMapDisp && m_bMiniMapDisp == GAME_SET_MINIMAP)
	{// ミニマップを描画するならば
		CRenderer *pRenderer = CManager::GetRenderer();
		if (pRenderer != NULL)
		{// レンダリングクラスが取得できた
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// デバイスが取得できた
				// ミニマップにも描画する
				D3DVIEWPORT9 ViewportDef;                           // 描画領域保存用
				CMapCamera *pMapCamera = CManager::GetMapCamera();  // ミニマップ用のカメラ

				// 描画領域を保存しておく
				pDevice->GetViewport(&ViewportDef);

				// マップカメラを設定する
				pMapCamera->SetCamera();

				// デバイスをクリアする
				pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

				// マップを構成する要素を描画する
				D3DXVECTOR3 GoalPos;
				CScene *pScene = NULL;               // シーンクラスへのポインタ
				CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
				CObject *pObjcet = NULL;
				CObjectGoal *pObjGoal = NULL;
				for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
				{// 描画優先順位の数だけ繰り返し
					pScene = CScene::GetTop(nCntPriority);
					while (pScene != NULL)
					{// メモリが空になるまで繰り返し
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT || pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE || pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL
							|| pScene->GetObjType() == CScene::OBJTYPE_BILLBOARDOBJ || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER
							|| pScene->GetObjType() == CScene::OBJTYPE_ENEMYMANAGER || pScene->GetObjType() == CScene::OBJTYPE_ENEMY || pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD
							|| pScene->GetObjType() == CScene::OBJTYPE_MOUNTAIN || pScene->GetObjType() == CScene::OBJTYPE_SKY || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE
							|| pScene->GetObjType() == CScene::OBJTYPE_RINGEFFECT || pScene->GetObjType() == CScene::OBJTYPE_MESHWALL || pScene->GetObjType() == CScene::OBJTYPE_RIVER)
						{// マップを構成するオブジェクトだったら
							if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
							{
								pObjcet = (CObject*)pScene;
								pObjcet->SetThin(false);
								pObjcet->SetAlpha(1.0f);
							}
							else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
							{
								pObjGoal = (CObjectGoal*)pScene;
								GoalPos = pObjGoal->GetPos();
							}

							pScene->Draw();
						}
						pScene = pSceneNext;
					}
				}

				if (m_pPlayerPosition != NULL)
				{// メモリが確保されている
					if (m_pPlayer != NULL)
					{// プレイヤーが生成されている
						m_pPlayerPosition->SetRot(m_pPlayer->GetRot().y);
						m_pPlayerPosition->Draw();
					}
				}

				if (m_pDestPosition != NULL)
				{// メモリが確保されている
					if (m_pPlayer != NULL)
					{// プレイヤーが生成されている
						float fRot = atan2f(m_pPlayer->GetPos().x - GoalPos.x, m_pPlayer->GetPos().z- GoalPos.z);
						m_pDestPosition->SetRot(fRot);
						m_pDestPosition->Draw();
					}
				}

				if (CManager::GetFade()->GetState() != CFade::STATE_NONE)
				{// フェードが使用されている
					if (CManager::GetFade()->GetScene2D() != NULL)
					{// フェードに使用するポリゴンが生成されている
						CManager::GetFade()->GetScene2D()->Draw();
					}
				}

				// 描画領域を元に戻す
				pDevice->SetViewport(&ViewportDef);
			}
		}
	}
}

//=============================================================================
//    ボスが出た時のイベント処理
//=============================================================================
void CGame::BossAppearEvent(void)
{
	// カメラの設定
	CManager::GetCamera()->SetPosV(GAME_BOSSAPPAER_CAMERA_POSV);
	CManager::GetCamera()->SetPosR(GAME_BOSSAPPAER_CAMERA_POSR);

	m_nBossAppearCounter++;  // カウンターを増やす

	if (m_nBossAppearCounter >= GAME_BOSSAPPEAR_STATE_TIMING)
	{// 状態を変えるタイミングなった
		for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
		{// ロゴを出す数だけ繰り返し
			if (m_pBossAppearLogo[nCntAppear] != NULL)
			{
				m_pBossAppearLogo[nCntAppear]->Uninit();
				m_pBossAppearLogo[nCntAppear] = NULL;
			}
		}

		// BGMを再生
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM003);

		// 状態を戻す
		m_State = STATE_NORMAL;

		// UIの表示を戻す
		if (m_pUi != NULL)
		{
			m_pUi->SetDisp(true);
		}
		m_bMiniMapDisp = true;

		// プレイヤーの場所を表示するポリゴンを再生成する
		m_pPlayerPosition = CScene2D::Create(GAME_PLAYER_POSITION_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GAME_PLAYER_POSITION_WIDTH, GAME_PLAYER_POSITION_HEIGHT, 0.0f, GAME_POLYGON_PRIORITY);
		if (m_pPlayerPosition != NULL)
		{
			m_pPlayerPosition->BindTexture(m_pTextureManager->GetTexture(6));
		}
	}
	if (m_nBossAppearCounter == GAME_BOSSAPPEAR_LOGO_TIMING_0)
	{// 1つ目のロゴを出すタイミングなった
		if (m_pBossAppearLogo[0] == NULL)
		{
			m_pBossAppearLogo[0] = CScene2D::Create(GAME_BOSSAPPEAR_POS_0, GAME_BOSSAPPEAR_COL_0, GAME_BOSSAPPEAR_WIDTH_0, GAME_BOSSAPPEAR_HEIGHT_0, 0.0f, GAME_POLYGON_PRIORITY);
			if (m_pBossAppearLogo[0] != NULL)
			{
				m_pBossAppearLogo[0]->BindTexture(m_pTextureManager->GetTexture(18));
			}

			// SEを再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSSAPPEAR);
		}
	}
	if (m_nBossAppearCounter == GAME_BOSSAPPEAR_LOGO_TIMING_1)
	{// 2つ目のロゴを出すタイミングなった
		if (m_pBossAppearLogo[1] == NULL)
		{
			m_pBossAppearLogo[1] = CScene2D::Create(GAME_BOSSAPPEAR_POS_1, GAME_BOSSAPPEAR_COL_1, GAME_BOSSAPPEAR_WIDTH_1, GAME_BOSSAPPEAR_HEIGHT_1, 0.0f, GAME_POLYGON_PRIORITY);
			if (m_pBossAppearLogo[1] != NULL)
			{
				m_pBossAppearLogo[1]->BindTexture(m_pTextureManager->GetTexture(19));
			}

			// SEを再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSSAPPEAR);
		}
	}
	if (m_nBossAppearCounter == GAME_BOSSAPPEAR_EFFECT_TIMING)
	{// エフェクトを出すタイミングなった
		// エフェクトを出す
		m_pEffectManager->SetEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);

		// フィードバックエフェクトを出す
		CManager::GetRenderer()->SetFeedBack(60, 0.994f);

		// SEを再生
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSSAPPEAR_EXPLOSION);
	}
}

//=============================================================================
//    ボスが死んだときのイベント処理
//=============================================================================
void CGame::KonckDownBossEvent(void)
{
	// カメラの設定
	D3DXVECTOR3 PosV;
	PosV.x = m_BossPos.x + sinf(0.0f) * GAME_KNOCKDOWN_CAMERA_LENGTH;
	PosV.y = GAME_KNOCKDOWN_CAMERA_POSY;
	PosV.z = m_BossPos.z + cosf(0.0f) * GAME_KNOCKDOWN_CAMERA_LENGTH;
	CManager::GetCamera()->SetPosV(PosV);
	CManager::GetCamera()->SetPosR(m_BossPos);

	m_nKnockDownCounter++;   // カウンターを進める
	if (m_nKnockDownCounter >= GAME_KNOCKDOWN_CAMERA_TIMING)
	{// カメラを通常のカメラに戻すタイミングになった
		// 状態を変更
		m_State = STATE_END;
	}
}

//=============================================================================
//    プレイヤークラスへのポインタを設定する処理
//=============================================================================
void CGame::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CGame::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    マップをクリアしたかどうかを設定する
//=============================================================================
void CGame::SetMapClear(bool bMapClear)
{
	m_bMapClear = bMapClear;
}

//=============================================================================
//    取得した隠しアイテムの数を設定する処理
//=============================================================================
void CGame::SetHiddenItem(const int nGetHiddenItem)
{
	m_nGetHiddenItem = nGetHiddenItem;
}

//=============================================================================
//    コンボ数の最大値を設定する処理
//=============================================================================
void CGame::SetComboMax(const int nComboMax)
{
	m_nComboMax = nComboMax;
}

//=============================================================================
//    ボスの座標を設定する処理
//=============================================================================
void CGame::SetBossPos(const D3DXVECTOR3 BossPos)
{
	m_BossPos = BossPos;
}

//=============================================================================
//    UIクラスへのポインタを取得する処理
//=============================================================================
CUI *CGame::GetUI(void)
{
	return m_pUi;
}

//=============================================================================
//    プレイヤークラスへのポインタを取得する処理
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを取得する処理
//=============================================================================
CEffectManager *CGame::GetEffectManager(void)
{
	return m_pEffectManager;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//    マップをクリアしたかどうかを取得する
//=============================================================================
bool CGame::GetMapClear(void)
{
	return m_bMapClear;
}

//=============================================================================
//    現在のマップ番号を取得する処理
//=============================================================================
int CGame::GetMapIdx(void)
{
	return m_nMapIdx;
}

//=============================================================================
//    マップの読み込み数を取得する処理
//=============================================================================
int CGame::GetNumMap(void)
{
	return m_nNumMap;
}

//=============================================================================
//    取得した隠しアイテムの数を取得する処理
//=============================================================================
int CGame::GetHiddenItem(void)
{
	return m_nGetHiddenItem;
}

//=============================================================================
//    コンボ数の最大値を取得する処理
//=============================================================================
int CGame::GetComboMax(void)
{
	return m_nComboMax;
}

//=============================================================================
//    橋の当たり判定を設定する処理
//=============================================================================
void CGame::SetBridgeCollider(void)
{
	FILE *pFile = NULL;      // ファイルポインタ
	char *pLine = NULL;      // 1行分読み取り用
	char *pStrCur = NULL;    // 現在の先頭ポインタ
	D3DXVECTOR3 ColliderPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fColliderWidth = 0.0f;
	float fColliderHeight = 0.0f;
	float fColliderDepth = 0.0f;
	bool bColliderReturnFlag = false;

	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen("data/TEXT/MAP/COLLIDER/boxcollider000.txt", "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			if (pLine == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				if (pLine != NULL)
				{// メモリを確保できた
				    // 文字列を初期化
					strcpy(pLine, "\0");
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, BOXCOLLIDERSET) == 0)
							{// 当たり判定用箱モデルの配置情報読み込み開始の合図だった
								while (1)
								{// ループ開始
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
									if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
									{// 座標情報がある
										ColliderPos = CFunctionLib::ReadVector3(pStrCur, POS);
									}
									else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
									{// 幅情報がある
										fColliderWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
									}
									else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
									{// 高さ情報がある
										fColliderHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
									}
									else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
									{// 奥行情報がある
										fColliderDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
									}
									else if (CFunctionLib::Memcmp(pStrCur, RETURN) == 0)
									{// 当たった際に押し戻すかどうか情報がある
										bColliderReturnFlag = CFunctionLib::ReadBool(pStrCur, RETURN);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_BOXCOLLIDERSET) == 0)
									{// 当たり判定用箱モデルの配置情報読み込み終了の合図だった
										CBoxCollider::Create(ColliderPos, fColliderWidth, fColliderHeight, fColliderDepth, bColliderReturnFlag, 5);
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					if (pLine != NULL)
					{// メモリが確保されている
						delete[] pLine;
						pLine = NULL;
					}
				}
			}
		}
	}
}

//=============================================================================
//    草の位置や向きを保存する処理
//=============================================================================
void CGame::SaveGrassTransform(void)
{
	FILE *pFile = NULL;
	pFile = fopen("data/TEXT/save_grass.txt", "w");
	if (pFile != NULL)
	{
		// コメント部分を書き込み
		fprintf(pFile, "#******************************************************************************\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#       草の配置情報スクリプトファイル[save_grass.txt]\n");
		fprintf(pFile, "#       Auther : Hodaka Niwa\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#******************************************************************************\n");
		fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#  草の配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CGrass *pGrass = NULL;
		pScene = CScene::GetTop(9);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_GRASS)
			{// 草クラスだったら
				pGrass = (CGrass*)pScene;
				if (pGrass != NULL)
				{// 草クラスが取得できた
					D3DXVECTOR3 GrassRot = pGrass->GetRot();
					// 向き
					GrassRot.x = D3DXToDegree(GrassRot.x);
					GrassRot.y = D3DXToDegree(GrassRot.y);
					GrassRot.z = D3DXToDegree(GrassRot.z);

					// 配置物の情報を書き込み
					fprintf(pFile, "%s\n", GRASSSET);
					fprintf(pFile, "	%s%d						# 草が揺れるかどうか\n", SWAY, pGrass->GetSway());
					fprintf(pFile, "	%s%.1f %.1f %.1f			# 草の座標\n", POS, pGrass->GetPos().x, pGrass->GetPos().y, pGrass->GetPos().z);
					fprintf(pFile, "	%s%.1f %.1f %.1f				# 草の向き\n", ROT, GrassRot.x, GrassRot.y, GrassRot.z);
					fprintf(pFile, "	%s%.1f					# 草の幅\n", WIDTH, pGrass->GetWidth());
					fprintf(pFile, "	%s%.1f					# 草の高さ\n", HEIGHT, pGrass->GetHeight());
					fprintf(pFile, "%s\n", END_GRASSSET);
					fprintf(pFile, "\n");
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}

		// コメント部分を書き込み
		fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);
	}
}

//=============================================================================
//    草の位置や向きを読み込む処理
//=============================================================================
void CGame::LoadGrassTransform(char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen("data/TEXT/MAP/GRASS/grass.txt", "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, GRASSSET) == 0)
					{// 草の配置情報読み込み開始の合図だった
						D3DXVECTOR3 GrassPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXVECTOR3 GrassRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						bool bGrassSway = false;
						float fGrassWidth = 0.0f;
						float fGrassHeight = 0.0f;
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, SWAY) == 0)
							{// 座標情報がある
								bGrassSway = CFunctionLib::ReadBool(pStrCur, SWAY);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報がある
								GrassPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報がある
								GrassRot = CFunctionLib::ReadVector3(pStrCur, ROT);
								GrassRot.x = D3DXToRadian(GrassRot.x);
								GrassRot.y = D3DXToRadian(GrassRot.y);
								GrassRot.z = D3DXToRadian(GrassRot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 幅情報がある
								fGrassWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// 高さ情報がある
								fGrassHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_GRASSSET) == 0)
							{// 草の配置情報読み込み終了の合図だった
							 	// 草を配置する
							 	CGrass *pGrass = CGrass::Create(D3DXVECTOR3(GrassPos.x, GrassPos.y + 11.0f, GrassPos.z), GrassRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fGrassWidth, fGrassHeight, bGrassSway, 9);
							 	if (pGrass != NULL)
							 	{
							 		pGrass->BindTexture(m_pMap->GetTextureManager()->GetTexture(9));
							 	}
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						break;  // ループ終了
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}