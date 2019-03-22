//*****************************************************************************
//
//     マップの処理[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "map.h"
#include "sky.h"
#include "mountain.h"
#include "object.h"
#include "meshField.h"
#include "river.h"
#include "meshWall.h"
#include "item.h"
#include "billboardObject.h"
#include "modelManager.h"
#include "textureManager.h"
#include "effectManager.h"
#include "enemy.h"
#include "player.h"
#include "emitter.h"
#include "scene.h"
#include "scene3D.h"
#include "shadow.h"
#include "functionlib.h"
#include "game.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// マップ配置用
#define MAP_SET_ENEMY              (true)                                        // 敵を配置するかどうか
#define MAP_SET_OBJECT             (true)                                        // モデルを配置するかどうか
#define MAP_SET_EFFECT             (true)                                        // エフェクトを配置するかどうか

// セーブデータ用
#define MAP_OBJECT_SAVEFILENAME    "data/TEXT/MAP/OBJECT/save_object.txt"        // 配置物情報を保存するテキストファイル名
#define MAP_ENEMY_SAVEFILENAME     "data/TEXT/MAP/ENEMY/save_set_enemy.txt"      // 敵の配置情報を保存するテキストファイル名
#define MAP_BILLBOARD_SAVEFILENAME "data/TEXT/MAP/BILLBOARD/save_billboard.txt"  // 配置用ビルボード情報を保存するテキストファイル名
#define MAP_EFFECT_SAVEFILENAME    "data/TEXT/MAP/EFFECT/save_effect.txt"        // 配置エフェクト情報を保存するテキストファイル名
#define MAP_WALL_SAVEFILENAME      "data/TEXT/MAP/WALL/save_wall.txt"            // 壁情報を保存するテキストファイル名
#define MAP_ITEM_SAVEFILENAME      "data/TEXT/MAP/ITEM/save_item.txt"            // アイテム情報を保存するテキストファイル名

// スクリプトファイルの読み込み用のパス名
#define MAPSET               "MAPSET"                  // マップセット開始の合図
#define END_MAPSET           "END_MAPSET"              // マップセット終了の合図
#define FIELDSET             "FIELDSET"                // 地面セット開始の合図
#define END_FIELDSET         "END_FIELDSET"            // 地面セット終了の合図
#define RIVERSET             "RIVERSET"                // 川セット開始の合図
#define END_RIVERSET         "END_RIVERSET"            // 川セット終了の合図
#define OBJECTSET            "OBJECTSET"               // 配置物セット開始の合図
#define END_OBJECTSET        "END_OBJECTSET"           // 配置物セット終了の合図
#define OBJECTMOVESET        "OBJECTMOVESET"           // 動く配置物セット開始の合図
#define END_OBJECTMOVESET    "END_OBJECTMOVESET"       // 動く配置物セット終了の合図
#define OBJECTGOALSET        "OBJECTGOALSET"           // ゴール用配置物セット開始の合図
#define END_OBJECTGOALSET    "END_OBJECTGOALSET"       // ゴール用配置物セット終了の合図
#define POLYGONSET           "POLYGONSET"              // ゴール用ポリゴン情報開始の合図
#define END_POLYGONSET       "END_POLYGONSET"          // ゴール用ポリゴン情報終了の合図
#define MOUNTAINSET          "MOUNTAINSET"             // 山セット開始の合図
#define END_MOUNTAINSET      "END_MOUNTAINSET"         // 山セット終了の合図
#define SKYSET               "SKYSET"                  // 空セット開始の合図
#define END_SKYSET           "END_SKYSET"              // 空セット終了の合図
#define ENEMYSET             "ENEMYSET"                // 敵の配置データ読み込み開始の合図
#define END_ENEMYSET         "END_ENEMYSET"            // 敵の配置データ読み込み終了の合図
#define BILLBOARDSET         "BILLBOARDSET"            // 配置用ビルボード読み込み開始の合図
#define END_BILLBOARDSET     "END_BILLBOARDSET"        // 配置用ビルボード読み込み終了の合図
#define EFFECTSET            "EFFECTSET"               // 配置エフェクト読み込み開始の合図
#define END_EFFECTSET        "END_EFFECTSET"           // 配置エフェクト読み込み終了の合図
#define WALLSET              "WALLSET"                 // 壁セット読み込み開始の合図
#define END_WALLSET          "END_WALLSET"             // 壁セット読み込み終了の合図
#define ITEMSET              "ITEMSET"                 // アイテム読み込み開始の合図
#define END_ITEMSET          "END_ITEMSET"             // アイテム読み込み終了の合図
#define AISET                "AISET"                   // 敵のAI情報読み込み開始の合図
#define END_AISET            "END_AISET"               // 敵のAI情報読み込み終了の合図

// ファイル名読み取り用
#define FIELD_FILENAME       "FIELD_FILENAME = "       // 地面のスクリプトファイル名
#define RIVER_FILENAME       "RIVER_FILENAME = "       // 川のスクリプトファイル名
#define OBJECT_FILENAME      "OBJECT_FILENAME = "      // 配置物のスクリプトファイル名
#define MOUNTAIN_FILENAME    "MOUNTAIN_FILENAME = "    // 山のスクリプトファイル名
#define SKY_FILENAME         "SKY_FILENAME = "         // 空のスクリプトファイル名
#define ENEMY_FILENAME       "ENEMY_FILENAME = "       // 敵のスクリプトファイル名
#define BILLBOARD_FILENAME   "BILLBOARD_FILENAME = "   // 配置用ビルボードのスクリプトファイル名
#define EFFECT_FILENAME      "EFFECT_FILENAME = "      // 配置エフェクトのスクリプトファイル名
#define WALL_FILENAME        "WALL_FILENAME = "        // 壁のスクリプトファイル名
#define ITEM_FILENAME        "ITEM_FILENAME = "        // アイテムのスクリプトファイル名
#define PLAYER_DEFAULT_SPAWN "PLAYER_DEFAULT_SPAWN = " // プレイヤーのデフォルトスポーン位置
#define CAMERA_DEFAULT_ROT   "CAMERA_DEFAULT_ROT = "   // カメラのデフォルト向き

// その他
#define NUM_TEXTURE          "NUM_TEXTURE = "          // 読み込むテクスチャの数
#define TEXTURE_FILENAME     "TEXTURE_FILENAME = "     // 読み込むテクスチャのファイル名
#define NUM_MODEL            "NUM_MODEL = "            // 読み込むモデルの数
#define MODEL_FILENAME       "MODEL_FILENAME = "       // 読み込むモデルファイル名
#define ENEMYDATA_FILENAME   "ENEMYDATA_FILENAME = "   // 読み込む敵のデータファイル名

// データ読み取り用
#define TEX_TYPE             "TEX_TYPE = "             // テクスチャの番号情報
#define TYPE                 "TYPE = "                 // 種類情報
#define POS                  "POS = "                  // 座標情報
#define POS_TARGET           "POS_TARGET = "           // 目的の位置
#define OFFSET               "OFFSET = "               // オフセット位置情報
#define ROT                  "ROT = "                  // 向き情報
#define SCALE                "SCALE = "                // 大きさ情報
#define COL                  "COL = "                  // 色情報
#define MOVE                 "MOVE = "                 // 移動量情報
#define MOVE_MAX             "MOVE_MAX = "             // 移動量を切り替える時間情報
#define PUSH                 "PUSH = "                 // 押せるかどうか
#define BREAK                "BREAK = "                // 壊せるかどうか
#define ROLL_SPEED           "ROLL_SPEED = "           // 回転するスピード量情報
#define CARRY_TIME           "CARRY_TIME = "           // プレイヤーを運ぶのにかける時間
#define COL_RANGE            "COL_RANGE = "            // 当たり判定をする範囲
#define COLLISION            "COLLISION = "            // 当たり判定をするかどうか(または種類)
#define WIDTH                "WIDTH = "                // 1ブロック分の幅情報
#define DEPTH                "DEPTH = "                // 1ブロック分奥行情報
#define RADIUS               "RADIUS = "               // 半径情報
#define HEIGHT               "HEIGHT = "               // 高さ情報
#define XBLOCK               "XBLOCK = "               // 横の分割数情報
#define YBLOCK               "YBLOCK = "               // 縦の分割数情報
#define ZBLOCK               "ZBLOCK = "               // 奥行の分割数情報
#define TEXSPLIT_U           "TEXSPLIT_U = "           // テクスチャU座標の分割数情報
#define TEXSPLIT_V           "TEXSPLIT_V = "           // テクスチャV座標の分割数情報
#define ADDTEX_U             "ADDTEX_U = "             // テクスチャU座標をどれくらい動かすか
#define ADDTEX_V             "ADDTEX_V = "             // テクスチャV座標をどれくらい動かすか
#define CULLING              "CULLING = "              // カリングするかどうか
#define BINARY_FILENAME      "BINARY_FILENAME = "      // 頂点情報が格納されたバイナリファイル名
#define NEXT_MAPNAME         "NEXT_MAPNAME = "         // 次のマップデータのファイル名
#define LIFE                 "LIFE = "                 // 体力
#define SCORE                "SCORE = "                // 倒した時に増えるスコア量
#define LIGHTING             "LIGHTING = "             // ライティングするかしないか
#define DRAW_ADDTIVE         "DRAW_ADDTIVE = "         // 加算合成で描画するかしないか
#define CHEACKDISTANCE       "CHEACKDISTANCE = "       // 警戒できる範囲
#define ATTACKDISTANCE       "ATTACKDISTANCE = "       // 攻撃を仕掛ける範囲
#define ATTACKRANDOM         "ATTACKRANDOM = "         // 攻撃を仕掛けるタイミング

//=============================================================================
//    コンストラクタ
//=============================================================================
CMap::CMap()
{
	// 各種値の初期化
	m_pModelManager = NULL;    // モデル管轄クラスへのポインタ
	m_pTextureManager = NULL;  // テクスチャ管轄クラスへのポインタ
	m_pEffectManager = NULL;   // エフェクト管轄クラスへのポインタ
	m_pEnemyAssembly = NULL;   // 敵管轄まとめクラスへのポインタ
	m_pField = NULL;           // 地面クラスへのポインタ
	m_pMoutain = NULL;         // 円筒クラスへのポインタ
	m_pSky = NULL;             // 空クラスへのポインタ
	m_nCntLoadEnemy = 0;       // 敵データを読み込んだ回数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMap *CMap::Create(char *aFileName)
{
	CMap *pMap = NULL;  // マップクラス型のポインタ
	if (pMap == NULL)
	{// メモリが確保できる状態にある
		pMap = new CMap;
		if (pMap != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pMap->Init(aFileName);
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
	return pMap;
}

//=============================================================================
//    初期化処理
//=============================================================================
void CMap::Init(char *aFileName)
{
	// マップの外部ファイル名を設定
	strcpy(m_aFileName, aFileName);

	// マップ情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                                             // 1行分読み取り用
			char *pStrCur = NULL;                                           // 現在の先頭ポインタ
			char *pStr = NULL;                                              // 先頭ポインタ保存用
			int nNumTex = 0;                                                // テクスチャを読み込む数
			int nNumModel = 0;                                              // モデルを読み込む数
			int nCntTex = 0;                                                // テクスチャを読み込んだ回数
			int nCntModel = 0;                                              // モデルを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;                             // テクスチャ読み取り用
			LPD3DXMESH pMesh = NULL;                                        // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;                                   // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                                              // マテリアル情報の数
			char aTexFileName[256] = "\0";                                  // テクスチャのファイル名
			D3DXVECTOR3 DefaultPlayerSpawn = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // プレイヤーのデフォルトスポーン位置
			D3DXVECTOR3 DefaultCameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // カメラのデフォルト向き

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
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// テクスチャの個数情報があった
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// モデル数が1つ以上ある
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// モデルのファイルパス名情報があった
							    // モデルのファイルパス名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

								// モデルの読み込み
								// xファイルの読み込み
								D3DXLoadMeshFromX(pStr,
									D3DXMESH_SYSTEMMEM,
									CManager::GetRenderer()->GetDevice(),
									NULL,
									&pBuffMat,
									NULL,
									&nNumMat,
									&pMesh);

								// モデル管轄クラスにポインタを設定する
								m_pModelManager->SetMesh(pMesh, nCntModel);
								m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

								// 使用した変数を初期化しておく
								pMesh = NULL;
								pBuffMat = NULL;
								nNumMat = 0;

								nCntModel++;  // モデルを読み込んだ回数を増やす
							}
							else if (CFunctionLib::Memcmp(pStrCur, FIELD_FILENAME) == 0)
							{// 地面のスクリプトファイル名があった
							    // 地面のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, FIELD_FILENAME);
								FieldLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RIVER_FILENAME) == 0)
							{// 川のスクリプトファイル名があった
							    // 川のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, RIVER_FILENAME);
								RiverLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, OBJECT_FILENAME) == 0)
							{// 配置物のスクリプトファイル名があった
								if (MAP_SET_OBJECT)
								{// モデルを配置するなら
									// 配置物のスクリプトファイル名を読み取る
									pStr = CFunctionLib::ReadString(pStrCur, pStr, OBJECT_FILENAME);
									ObjectLoad(pStr, pStrCur, pLine);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOUNTAIN_FILENAME) == 0)
							{// 山のスクリプトファイル名があった
							    // 山のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MOUNTAIN_FILENAME);
								MountainLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, SKY_FILENAME) == 0)
							{// 空のスクリプトファイル名があった
							    // 空のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, SKY_FILENAME);
								SkyLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ENEMY_FILENAME) == 0)
							{// 敵のスクリプトファイル名があった
								if (CManager::GetMode() == CManager::MODE_GAME && MAP_SET_ENEMY
									|| CManager::GetMode() == CManager::MODE_TUTORIAL && MAP_SET_ENEMY)
								{// チュートリアル画面かゲーム画面かつ敵を配置するなら
									// 敵のスクリプトファイル名を読み取る
									pStr = CFunctionLib::ReadString(pStrCur, pStr, ENEMY_FILENAME);
									EnemyLoad(pStr, pStrCur, pLine);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, BILLBOARD_FILENAME) == 0)
							{// 配置用ビルボードのスクリプトファイル名があった
							    // 配置用ビルボードのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, BILLBOARD_FILENAME);
								BillboardLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// 配置エフェクトのスクリプトファイル名があった
								if (MAP_SET_EFFECT)
								{// エフェクトを配置するなら
									// 配置エフェクトのスクリプトファイル名を読み取る
									pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
									EffectLoad(pStr, pStrCur, pLine);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, WALL_FILENAME) == 0)
							{// 壁のスクリプトファイル名があった
							    // 壁のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, WALL_FILENAME);
								WallLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ITEM_FILENAME) == 0)
							{// アイテムのスクリプトファイル名があった
							    // アイテムのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, ITEM_FILENAME);
								ItemLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, PLAYER_DEFAULT_SPAWN) == 0)
							{// プレイヤーのデフォルトスポーン位置があった
								DefaultPlayerSpawn = CFunctionLib::ReadVector3(pStrCur, PLAYER_DEFAULT_SPAWN);
								CScene *pScene = NULL;
								CScene *pSceneNext = NULL;
								CPlayer *pPlayer = NULL;
								for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
								{// 優先順位の数だけ繰り返し
									pScene = CScene::GetTop(nCntPriority);
									while (pScene != NULL)
									{// メモリが空になるまで繰り返し
										pSceneNext = pScene->GetNext();
										if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
										{// プレイヤークラスへのポインタだった
											pPlayer = (CPlayer*)pScene;
											if (pPlayer != NULL)
											{// プレイヤークラスのポインタが取得できた
												pPlayer->SetPos(DefaultPlayerSpawn);
												pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
												pPlayer->SetDestRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
												pPlayer->SetDiffRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
												break;
											}
										}
										pScene = pSceneNext;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, CAMERA_DEFAULT_ROT) == 0)
							{// カメラのデフォルト向きがあった
								DefaultCameraRot = CFunctionLib::ReadVector3(pStrCur, CAMERA_DEFAULT_ROT);

								// 向きをラジアン角に戻す
								DefaultCameraRot.x = D3DXToRadian(DefaultCameraRot.x);
								DefaultCameraRot.y = D3DXToRadian(DefaultCameraRot.y);
								DefaultCameraRot.z = D3DXToRadian(DefaultCameraRot.z);
								// X軸
								if (DefaultCameraRot.x > D3DX_PI)
								{
									DefaultCameraRot.x -= D3DX_PI * 2.0f;
								}
								// Y軸
								if (DefaultCameraRot.y > D3DX_PI)
								{
									DefaultCameraRot.y -= D3DX_PI * 2.0f;
								}
								// Z軸
								if (DefaultCameraRot.z > D3DX_PI)
								{
									DefaultCameraRot.z-= D3DX_PI * 2.0f;
								}

								CManager::GetCamera()->SetRot(DefaultCameraRot);
								CManager::GetCamera()->SetRotDest(DefaultCameraRot);
								CManager::GetCamera()->SetRotDiff(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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
			}
			fclose(pFile);
		}
	}
}

//=============================================================================
//    終了処理
//=============================================================================
void CMap::Uninit(void)
{
	// 配置情報を保存する
	//Save();

	// マップ管轄クラスを開放する
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// 敵の管轄まとめクラスのメモリを開放する
	if (m_pEnemyAssembly != NULL)
	{
		// メモリの開放
		delete m_pEnemyAssembly;
		m_pEnemyAssembly = NULL;
	}

	// エフェクト管轄クラスへのポインタを空にする
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager = NULL;
	}

	// 地面の破棄
	if (m_pField != NULL)
	{// メモリが確保されている
		m_pField = NULL;
	}

	// 山の破棄
	if (m_pMoutain != NULL)
	{// メモリが確保されている
		m_pMoutain = NULL;
	}

	// 空の破棄
	if (m_pSky != NULL)
	{// メモリが確保されている
		m_pSky = NULL;
	}


	// マップの要素であるクラスを削除する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT || pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE || pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL
				|| pScene->GetObjType() == CScene::OBJTYPE_BILLBOARDOBJ || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER
				|| pScene->GetObjType() == CScene::OBJTYPE_ENEMYMANAGER || pScene->GetObjType() == CScene::OBJTYPE_ENEMY || pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD
				|| pScene->GetObjType() == CScene::OBJTYPE_MOUNTAIN || pScene->GetObjType() == CScene::OBJTYPE_SKY || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE
				|| pScene->GetObjType() == CScene::OBJTYPE_RINGEFFECT || pScene->GetObjType() == CScene::OBJTYPE_MESHWALL || pScene->GetObjType() == CScene::OBJTYPE_RIVER
				|| pScene->GetObjType() == CScene::OBJTYPE_ITEM || pScene->GetObjType() == CScene::OBJTYPE_GRASS || pScene->GetObjType() == CScene::OBJTYPE_BOXCOLLIDER)
			{// マップの要素を形成するクラスへのポインタだった
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}
	CScene::DeathCheck();
}

//=============================================================================
//    地面情報読み込み処理
//=============================================================================
void CMap::FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pFieldFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // 地面用の格納変数
			int nFieldTexIdx = 0;                                    // 地面のテクスチャ番号
			D3DXVECTOR3 FieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 地面の座標
			D3DXVECTOR3 FieldRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 地面の向き
			D3DXCOLOR FieldCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 地面の色
			float fFieldWidth = 0.0f;                                // 地面の1ブロック分の幅
			float fFieldDepth = 0.0f;                                // 地面の1ブロック分の奥行
			int nFieldXblock = 0;                                    // 地面の横のブロック数
			int nFieldZblock = 0;                                    // 地面の奥行のブロック数
			int nFieldTexSplitU = 0;                                 // 地面のテクスチャU座標の分割数
			int nFieldTexSplitV = 0;                                 // 地面のテクスチャV座標の分割数
			bool bFieldCulling = true;                               // カリングするかどうか
			char aFileName[256] = "\0";                              // 地面の頂点情報が格納されたバイナリファイル名

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, FIELDSET) == 0)
					{// 地面情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nFieldTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								FieldPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								FieldRot.x = D3DXToRadian(Rot.x);
								FieldRot.y = D3DXToRadian(Rot.y);
								FieldRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								FieldCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 1ブロック分の幅情報だった
								fFieldWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
							{// 1ブロック分の奥行情報だった
								fFieldDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								nFieldXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
							{// 奥行の分割数だった
								nFieldZblock = CFunctionLib::ReadInt(pStrCur, ZBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
							{// テクスチャU座標の分割数だった
								nFieldTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
							{// テクスチャV座標の分割数だった
								nFieldTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
							}
							else if (CFunctionLib::Memcmp(pStrCur, CULLING) == 0)
							{// カリングするかどうかだった
								bFieldCulling = CFunctionLib::ReadBool(pStrCur, CULLING);
							}
							else if (CFunctionLib::Memcmp(pStrCur, BINARY_FILENAME) == 0)
							{// 頂点情報が格納されたバイナリファイル名だった
								pStrCur = CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME);
								strcpy(aFileName, pStrCur);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_FIELDSET) == 0)
							{// 地面情報読み込み終了の合図だった
							    // 地面を生成する
								m_pField = CMeshField::Create(FieldPos, FieldRot, FieldCol, fFieldWidth, fFieldDepth, nFieldXblock, nFieldZblock, nFieldTexSplitU, nFieldTexSplitV, aFileName, bFieldCulling, FIELD_PRIORITY);
								if (m_pField != NULL)
								{// 地面が生成できた
									m_pField->BindTexture(m_pTextureManager->GetTexture(nFieldTexIdx));
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

//=============================================================================
//    川情報読み込み処理
//=============================================================================
void CMap::RiverLoad(char *pRiverFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pRiverFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 川用の格納変数
			CRiver *pRiver = NULL;                                   // 川クラスへのポインタ
			int nRiverTexIdx = 0;                                    // 川のテクスチャ番号
			D3DXVECTOR3 RiverPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 川の座標
			D3DXVECTOR3 RiverRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 川の向き
			D3DXCOLOR RiverCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 川の色
			float fRiverWidth = 0.0f;                                // 川の1ブロック分の幅
			float fRiverDepth = 0.0f;                                // 川の1ブロック分の奥行
			int nRiverXblock = 0;                                    // 川の横のブロック数
			int nRiverZblock = 0;                                    // 川の奥行のブロック数
			int nRiverTexSplitU = 0;                                 // 川のテクスチャU座標の分割数
			int nRiverTexSplitV = 0;                                 // 川のテクスチャV座標の分割数
			float fRiverAddTexU = 0;                                 // 川のテクスチャU座標をどれくらい動かすか
			float fRiverAddTexV = 0;                                 // 川のテクスチャV座標をどれくらい動かすか
			char aFileName[256] = "\0";                              // 地面の頂点情報が格納されたバイナリファイル名

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, RIVERSET) == 0)
					{// 川情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nRiverTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								RiverPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								RiverRot.x = D3DXToRadian(Rot.x);
								RiverRot.y = D3DXToRadian(Rot.y);
								RiverRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								RiverCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 1ブロック分の幅情報だった
								fRiverWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
							{// 1ブロック分の奥行情報だった
								fRiverDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								nRiverXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
							{// 奥行の分割数だった
								nRiverZblock = CFunctionLib::ReadInt(pStrCur, ZBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
							{// テクスチャU座標の分割数だった
								nRiverTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
							{// テクスチャV座標の分割数だった
								nRiverTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ADDTEX_U) == 0)
							{// テクスチャU座標をどれくらい動かすか情報だった
								fRiverAddTexU = CFunctionLib::ReadFloat(pStrCur, ADDTEX_U);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ADDTEX_V) == 0)
							{// テクスチャV座標をどれくらい動かすか情報だった
								fRiverAddTexV = CFunctionLib::ReadFloat(pStrCur, ADDTEX_V);
							}
							else if (CFunctionLib::Memcmp(pStrCur, BINARY_FILENAME) == 0)
							{// 頂点情報が格納されたバイナリファイル名だった
								pStrCur = CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME);
								strcpy(aFileName, pStrCur);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_RIVERSET) == 0)
							{// 川情報読み込み終了の合図だった
							    // 川を生成する
								pRiver = CRiver::Create(RiverPos, RiverRot, RiverCol, fRiverWidth, fRiverDepth, nRiverXblock, nRiverZblock, nRiverTexSplitU, nRiverTexSplitV, fRiverAddTexU, fRiverAddTexV, aFileName, RIVER_PRIORITY);
								if (pRiver != NULL)
								{// 地面が生成できた
									pRiver->BindTexture(m_pTextureManager->GetTexture(nRiverTexIdx));
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

//=============================================================================
//    配置物情報読み込み処理
//=============================================================================
void CMap::ObjectLoad(char *pObjectFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pObjectFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 配置物用の格納変数
			CObject *pObject = NULL;                                        // 配置物クラスへのポインタ
			CObjectMove *pObjectMove = NULL;                                // 動く配置物クラスへのポインタ
			CObjectGoal *pObjectGoal = NULL;                                // ゴール用配置物クラスへのポインタ
			int nObjectType = 0;                                            // 配置物のモデル番号
			int nObjectColType = 0;                                         // 配置物の当たり判定の種類
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の座標
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の向き
			D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 配置物の大きさの倍率
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 配置物の回転スピード
			float fObjectColRange = 0.0f;                             // 配置物の当たり判定を取る範囲(円形に当たり判定する際に使用する)
			D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 配置物の移動量
			int nObjectMaxCounter = 0;                                      // 配置物の移動量を切り替える時間

			int nGoalPolygonTexType = 0;                                    // ゴール用ドアポリゴンの使用するテクスチャ番号
			D3DXVECTOR3 GoalPolygonOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ゴール用ドアポリゴンのオフセット位置
			float fGoalPolygonWidth = 0.0f;                                 // ゴール用ドアポリゴンの幅
			float fGoalPolygonHeight = 0.0f;                                // ゴール用ドアポリゴンの高さ

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, OBJECTSET) == 0)
					{// 配置物情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 使用するモデル番号情報だった
								nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
							{// 当たり判定の種類情報だった
								nObjectColType = CFunctionLib::ReadInt(pStrCur, COLLISION);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
							{// 当たり判定を取る範囲情報だった
								fObjectColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								ObjectRot.x = D3DXToRadian(Rot.x);
								ObjectRot.y = D3DXToRadian(Rot.y);
								ObjectRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
							{// スケール情報だった
								ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// 回転するスピード情報だった
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTSET) == 0)
							{// 配置物情報読み込み終了の合図だった
							    // 配置物を生成する
								pObject = CObject::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, (CObject::COL_TYPE)nObjectColType, fObjectColRange, OBJECT_PRIORITY);
								if (pObject != NULL)
								{
									pObject->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType),m_pModelManager->GetVtxMax(nObjectType),m_pModelManager->GetVtxMin(nObjectType),m_pModelManager->GetTexture(nObjectType));
									pObject->SetModelIdx(nObjectType);
								}
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, OBJECTMOVESET) == 0)
					{// 動く配置物情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 使用するモデル番号情報だった
								nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
							{// 当たり判定の種類情報だった
								nObjectColType = CFunctionLib::ReadInt(pStrCur, COLLISION);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
							{// 当たり判定を取る範囲情報だった
								fObjectColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								ObjectRot.x = D3DXToRadian(Rot.x);
								ObjectRot.y = D3DXToRadian(Rot.y);
								ObjectRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
							{// スケール情報だった
								ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// 回転するスピード情報だった
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
							{// 移動量情報だった
								ObjectMove = CFunctionLib::ReadVector3(pStrCur, MOVE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOVE_MAX) == 0)
							{// 移動量を切り替える時間情報だった
								nObjectMaxCounter = CFunctionLib::ReadInt(pStrCur, MOVE_MAX);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTMOVESET) == 0)
							{// 動く配置物情報読み込み終了の合図だった
							    // 動く配置物を生成する
								pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, ObjectMove, nObjectMaxCounter, (CObject::COL_TYPE)nObjectColType, fObjectColRange, OBJECT_PRIORITY);
								if (pObjectMove != NULL)
								{
									pObjectMove->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
									pObjectMove->SetModelIdx(nObjectType);
								}
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, OBJECTGOALSET) == 0)
					{// ゴール用配置物情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 使用するモデル番号情報だった
								nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								ObjectRot.x = D3DXToRadian(Rot.x);
								ObjectRot.y = D3DXToRadian(Rot.y);
								ObjectRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
							{// スケール情報だった
								ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// 回転するスピード情報だった
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POLYGONSET) == 0)
							{// ゴール用のドアポリゴン情報読み込み開始の合図だった
								while (1)
								{// ループ開始
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
									if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
									{// 使用するテクスチャ番号だった
										nGoalPolygonTexType = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, OFFSET) == 0)
									{// オフセット位置情報だった
										GoalPolygonOffset = CFunctionLib::ReadVector3(pStrCur, OFFSET);
									}
									else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
									{// ポリゴンの横幅情報だった
										fGoalPolygonWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
									}
									else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
									{// ポリゴンの縦幅情報だった
										fGoalPolygonHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_POLYGONSET) == 0)
									{// ゴール用のドアポリゴン情報読み込み終了の合図だった
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTGOALSET) == 0)
							{// ゴール配置物情報読み込み終了の合図だった
							    // ゴール配置物を生成する
								pObjectGoal = CObjectGoal::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, OBJECT_PRIORITY);
								if (pObjectGoal != NULL)
								{
									pObjectGoal->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
									pObjectGoal->SetModelIdx(nObjectType);
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

//=============================================================================
//    山情報読み込み処理
//=============================================================================
void CMap::MountainLoad(char *pMoutainFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pMoutainFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // 山用の格納変数
			int nMountainTexIdx = 0;                                    // 山のテクスチャ番号
			D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 山の座標
			D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 山の向き
			D3DXCOLOR MountainCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 山の色
			float fMountainRadius = 0.0f;                               // 山の半径
			float fMountainHeight = 0.0f;                               // 山の高さ
			int nMountainXblock = 0;                                    // 山の横のブロック数
			int nMountainYblock = 0;                                    // 山の縦のブロック数

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, MOUNTAINSET) == 0)
					{// 山情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nMountainTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								MountainPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								MountainRot.x = D3DXToRadian(Rot.x);
								MountainRot.y = D3DXToRadian(Rot.y);
								MountainRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								MountainCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
							{// 山の半径情報だった
								fMountainRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// 山の高さ情報だった
								fMountainHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								nMountainXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// 縦の分割数だった
								nMountainYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_MOUNTAINSET) == 0)
							{// 山情報読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						break;  // ループ終了
					}
				}
				// 山を生成する
				m_pMoutain = CMountain::Create(MountainPos, MountainRot, MountainCol, fMountainHeight, fMountainRadius, nMountainXblock, nMountainYblock, MOUNTAIN_PRIORITY);
				if (m_pMoutain != NULL)
				{// 山が生成できた
					m_pMoutain->BindTexture(m_pTextureManager->GetTexture(nMountainTexIdx));
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    空情報読み込み処理
//=============================================================================
void CMap::SkyLoad(char *pSkyFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pSkyFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 空用の格納変数
		 	int nSkyTexIdx = 0;                                    // 空のテクスチャ番号
			D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 空の座標
			D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 空の向き
			D3DXCOLOR SkyCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 空の色
			float fSkyRadius = 0.0f;                               // 空の半径
			int nSkyXblock = 0;                                    // 空の横のブロック数
			int nSkyYblock = 0;                                    // 空の縦のブロック数
			float fRotSpeed = 0.0f;                                // 空の回転するスピード

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SKYSET) == 0)
					{// 空情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nSkyTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								SkyPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								SkyRot.x = D3DXToRadian(Rot.x);
								SkyRot.y = D3DXToRadian(Rot.y);
								SkyRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								SkyCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
							{// 空の半径情報だった
								fSkyRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								nSkyXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// 縦の分割数だった
								nSkyYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// 縦の分割数だった
								fRotSpeed = CFunctionLib::ReadFloat(pStrCur, ROLL_SPEED);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SKYSET) == 0)
							{// 空情報読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						break;  // ループ終了
					}
				}
				// 空を生成する
				m_pSky = CSky::Create(SkyPos, SkyRot, SkyCol, fSkyRadius, nSkyXblock, nSkyYblock, fRotSpeed, SKY_PRIORITY);
				if (m_pSky != NULL)
				{// 空が生成できた
					m_pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    敵情報読み込み処理
//=============================================================================
void CMap::EnemyLoad(char *pEnemyFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pEnemyFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			int nCntEnemyLoad = 0;                              // エネミーデータを読み込んだ回数
			char aEnemyDataFileName[256] = "\0";                // エネミーデータのスクリプトファイル名
			CEnemyManager *apEnemyManager[MAX_ENEMY_TYPE] = {}; // 敵管轄クラスへのポインタ

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, ENEMYDATA_FILENAME) == 0)
					{// 敵のデータ情報読み込み開始の合図だった
						pStrCur = CFunctionLib::ReadString(pStrCur, aEnemyDataFileName, ENEMYDATA_FILENAME);
						strcpy(aEnemyDataFileName, pStrCur);
						if (apEnemyManager[nCntEnemyLoad] == NULL)
						{// ポインタが確保できる状態である
							apEnemyManager[nCntEnemyLoad] = CEnemyManager::Create(nCntEnemyLoad, aEnemyDataFileName);
							nCntEnemyLoad++;
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, ENEMYSET) == 0)
					{// 敵の配置情報読み込み開始の合図だった
						int nEnemyType = 0;
						D3DXVECTOR3 EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXVECTOR3 EnemyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						float fEnemyAI_CheackDistance = 0.0f;
						float fEnemyAI_AttackDistance = 0.0f;
						int nEnemyAI_AttackTiming = 1;
						int nEnemyLife = 0;
						int nEnemyAddScore = 0;
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, AISET) == 0)
							{// AIの情報読み込み開始の合図だった
								while (1)
								{// ループ開始
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
									if (CFunctionLib::Memcmp(pStrCur, CHEACKDISTANCE) == 0)
									{// 警戒できる範囲情報がある
										fEnemyAI_CheackDistance = CFunctionLib::ReadFloat(pStrCur, CHEACKDISTANCE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, ATTACKDISTANCE) == 0)
									{// 攻撃を仕掛ける範囲情報がある
										fEnemyAI_AttackDistance = CFunctionLib::ReadFloat(pStrCur, ATTACKDISTANCE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, ATTACKRANDOM) == 0)
									{// 攻撃を仕掛けるタイミング情報がある
										nEnemyAI_AttackTiming = CFunctionLib::ReadInt(pStrCur, ATTACKRANDOM);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_AISET) == 0)
									{// AIの情報読み込み終了の合図だった
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 敵の種類がある
								nEnemyType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
							{// 体力がある
								nEnemyLife = CFunctionLib::ReadInt(pStrCur, LIFE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, SCORE) == 0)
							{// 敵の座標情報がある
								nEnemyAddScore = CFunctionLib::ReadInt(pStrCur, SCORE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 敵の座標情報がある
								EnemyPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 敵の向き情報がある
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								EnemyRot.x = D3DXToRadian(Rot.x);
								EnemyRot.y = D3DXToRadian(Rot.y);
								EnemyRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_ENEMYSET) == 0)
							{// 敵の配置情報読み込み終了の合図だった
								if (apEnemyManager[nEnemyType] != NULL)
								{// 敵の管轄クラスへのポインタが取得できている
									apEnemyManager[nEnemyType]->SetEnemy(EnemyPos, EnemyRot, nEnemyType, fEnemyAI_CheackDistance, fEnemyAI_AttackDistance, nEnemyAI_AttackTiming, nEnemyLife, nEnemyAddScore);
								}
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						// 敵管轄まとめクラスへポインタを設定する
						if (m_pEnemyAssembly == NULL)
						{// メモリが確保できる状態である
							m_pEnemyAssembly = new CEnemyAssembly;
							if (m_pEnemyAssembly != NULL)
							{// メモリが確保できた
								for (int nCntEnemy = 0; nCntEnemy < nCntEnemyLoad; nCntEnemy++)
								{// 敵のデータを読み込んだ回数分繰り返し
									m_pEnemyAssembly->SetEnemyManager(apEnemyManager[nCntEnemy], nCntEnemy);
								}
								// 読み込んだ回数を保存する
								m_nCntLoadEnemy = nCntEnemyLoad;
							}
						}

						break;  // ループ終了
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置用ビルボード情報読み込み処理
//=============================================================================
void CMap::BillboardLoad(char *pBillboardFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pBillboardFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, BILLBOARDSET) == 0)
					{// 配置用ビルボード情報読み込み開始の合図だった
						int nBillboardType = 0;
						int nBillboardTexIdx = 0;
						D3DXVECTOR3 BillboardPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXCOLOR BillboardCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
						float fBillboardColRange = 0.0f;
						float fBillboardRot = 0.0f;
						float fBillboardWidth = 0.0f;
						float fBillboardHeight = 0.0f;
						bool bBillboardLighting = false;
						bool bBillboardDrawAddtive = false;
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 種類番号情報がある
								nBillboardType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャ番号情報がある
								nBillboardTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
							{// 当たり判定を取る範囲情報がある
								fBillboardColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報がある
								BillboardPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報がある
								BillboardCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報がある
								float fRot = CFunctionLib::ReadFloat(pStrCur, ROT);
								fBillboardRot = D3DXToRadian(fRot);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 幅情報がある
								fBillboardWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// 高さ情報がある
								fBillboardHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, LIGHTING) == 0)
							{// ライティングするかしないか情報がある
								bBillboardLighting = CFunctionLib::ReadBool(pStrCur, LIGHTING);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
							{// 加算合成で描画するかしないか情報がある
								bBillboardDrawAddtive = CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_BILLBOARDSET) == 0)
							{// 配置用ビルボード情報読み込み終了の合図だった
								CBillboardObject *pBillboardObj = CBillboardObject::Create(BillboardPos, BillboardCol, fBillboardWidth, fBillboardHeight, fBillboardRot, bBillboardLighting, bBillboardDrawAddtive, nBillboardTexIdx, fBillboardColRange, nBillboardType, BILLBOARD_PRIORITY);
								if (pBillboardObj != NULL)
								{// 生成できた
									pBillboardObj->BindTexture(m_pTextureManager->GetTexture(nBillboardTexIdx));
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

//=============================================================================
//    配置エフェクト情報読み込み処理
//=============================================================================
void CMap::EffectLoad(char *pEffectFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pEffectFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			// エフェクト管轄クラスへのポインタを取得する
			CEffectManager *pEffectManager = NULL;
			if (pEffectManager == NULL)
			{// ポインタが確保できる状態である
				CScene *pScene = NULL;
				CScene *pSceneNext = NULL;
				for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
				{// 優先順位の数だけ繰り返し
					pScene = CScene::GetTop(nCntPriority);
					while (pScene != NULL)
					{// メモリが空になるまで繰り返し
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == CScene::OBJTYPE_EFFECTMANAGER)
						{// エフェクト管轄クラスへのポインタだった
							pEffectManager = (CEffectManager*)pScene;
							break;
						}
						pScene = pSceneNext;
					}
				}
			}
			if (pEffectManager != NULL)
			{// エフェクト管轄クラスへのポインタを取得できた
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
				{// スクリプト読み込み開始の合図だった
					while (1)
					{// ループ開始
						pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
						if (CFunctionLib::Memcmp(pStrCur, EFFECTSET) == 0)
						{// 配置エフェクト情報読み込み開始の合図だった
							int nEffectType = 0;
							D3DXVECTOR3 EffectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							D3DXVECTOR3 EffectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							while (1)
							{// ループ開始
								pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
								if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
								{// 種類情報がある
									nEffectType = CFunctionLib::ReadInt(pStrCur, TYPE);
								}
								else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
								{// 座標情報がある
									EffectPos = CFunctionLib::ReadVector3(pStrCur, POS);
								}
								else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
								{// 向き情報がある
									D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
									EffectRot.x = D3DXToRadian(Rot.x);
									EffectRot.y = D3DXToRadian(Rot.y);
									EffectRot.z = D3DXToRadian(Rot.z);
								}
								else if (CFunctionLib::Memcmp(pStrCur, END_EFFECTSET) == 0)
								{// 配置エフェクト情報読み込み終了の合図だった
									pEffectManager->SetEffect(EffectPos, EffectRot, nEffectType, EFFECT_PRIORITY);
									break;  // ループ終了
								}
							}
						}
						else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
						{// スクリプト読み込み終了の合図だった
							m_pEffectManager = pEffectManager;
							break;  // ループ終了
						}
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    壁情報読み込み処理
//=============================================================================
void CMap::WallLoad(char *pWallFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pWallFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 壁用の格納変数
			CMeshWall *pMeshWall = NULL;                            // 壁クラスのポインタ
			int nWallTexIdx = 0;                                    // 壁のテクスチャ番号
			D3DXVECTOR3 WallPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 壁の座標
			D3DXVECTOR3 WallRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 壁の向き
			D3DXCOLOR WallCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 壁の色
			float fWallWidth = 0.0f;                                // 壁の1ブロック分の幅
			float fWallHeight = 0.0f;                               // 壁の1ブロック分の奥行
			int nWallXblock = 0;                                    // 壁の横のブロック数
			int nWallYblock = 0;                                    // 壁の縦のブロック数
			int nWallTexSplitU = 0;                                 // 壁のテクスチャU座標の分割数
			int nWallTexSplitV = 0;                                 // 壁のテクスチャV座標の分割数

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, WALLSET) == 0)
					{// 壁情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nWallTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								WallPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								WallRot.x = D3DXToRadian(Rot.x);
								WallRot.y = D3DXToRadian(Rot.y);
								WallRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								WallCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 1ブロック分の幅情報だった
								fWallWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// 1ブロック分の高さ情報だった
								fWallHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								nWallXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// 縦の分割数だった
								nWallYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
							{// テクスチャU座標の分割数だった
								nWallTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
							{// テクスチャV座標の分割数だった
								nWallTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_WALLSET) == 0)
							{// 壁情報読み込み終了の合図だった
							    // 壁を生成する
								pMeshWall = CMeshWall::Create(WallPos, WallRot, WallCol, fWallWidth, fWallHeight, nWallXblock, nWallYblock, nWallTexSplitU, nWallTexSplitV, nWallTexIdx, WALL_PRIORITY);
								if (pMeshWall != NULL)
								{// 壁が生成できた
									pMeshWall->BindTexture(m_pTextureManager->GetTexture(nWallTexIdx));
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

//=============================================================================
//    アイテム情報読み込み処理
//=============================================================================
void CMap::ItemLoad(char *pItemFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pItemFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, ITEMSET) == 0)
					{// アイテム情報読み込み開始の合図だった
						int nItemTexIdx = 0;
						int nItemType = 0;
						float fItemColRange = 0.0f;
						D3DXVECTOR3 ItemPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXCOLOR ItemCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
						float fItemRot = 0.0f;
						float fItemWidth = 0.0f;
						float fItemHeight = 0.0f;
						bool bItemLighting = false;
						bool bItemDrawAddtive = false;
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャ番号情報がある
								nItemTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 種類情報がある
								nItemType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
							{// 当たり判定を取る範囲情報がある
								fItemColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報がある
								ItemPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報がある
								ItemCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報がある
								float fRot = CFunctionLib::ReadFloat(pStrCur, ROT);
								fItemRot = D3DXToRadian(fRot);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 幅情報がある
								fItemWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// 高さ情報がある
								fItemHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, LIGHTING) == 0)
							{// ライティングするかしないか情報がある
								bItemLighting = CFunctionLib::ReadBool(pStrCur, LIGHTING);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
							{// 加算合成で描画するかしないか情報がある
								bItemDrawAddtive = CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_ITEMSET) == 0)
							{// アイテム情報読み込み終了の合図だった
								CItem *pItem = CItem::Create(ItemPos, ItemCol, fItemWidth, fItemHeight, fItemRot, bItemLighting, bItemDrawAddtive, nItemTexIdx, fItemColRange, nItemType, BILLBOARD_PRIORITY);
								if (pItem != NULL)
								{// 生成できた
									pItem->BindTexture(m_pTextureManager->GetTexture(nItemTexIdx));
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

//=============================================================================
//    各種配置情報を保存する処理
//=============================================================================
void CMap::Save(void)
{
	// モデルの配置情報
	SaveObject();

	// 敵の配置情報
	SaveEnemy();

	// ビルボードの配置情報
	SaveBillboard();

	// エフェクトの配置情報
	SaveEffect();

	// 壁の配置情報
	SaveWall();

	// アイテムの配置情報
	SaveItem();
}

//=============================================================================
//    モデルの配置情報を保存する処理
//=============================================================================
void CMap::SaveObject(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(MAP_OBJECT_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       配置物情報スクリプトファイル[save_object.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  モデル情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// オブジェクト情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CObject *pObject = NULL;
			CObjectMove *pObjectMove = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
					{// 配置物クラスへのポインタだった
						pObject = (CObject*)pScene;
						if (pObject != NULL)
						{// オブジェクトへのポインタが取得できた
						    // 事前に計算をすませておく
							D3DXVECTOR3 ModelRot = pObject->GetRotStd();
							D3DXVECTOR3 ModelRollSpeed = pObject->GetRollSpeed();
							// 向き
							ModelRot.x = D3DXToDegree(ModelRot.x);
							ModelRot.y = D3DXToDegree(ModelRot.y);
							ModelRot.z = D3DXToDegree(ModelRot.z);

							// 回転するスピード
							ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
							ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
							ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

							// 配置物の情報を書き込み
							fprintf(pFile, "%s\n", OBJECTSET);
							fprintf(pFile, "	%s%d						# 使用するモデルの番号\n", TYPE, pObject->GetModelIdx());
							fprintf(pFile, "	%s%d					# 当たり判定の種類\n", COLLISION, pObject->GetCollision());
							fprintf(pFile, "	%s%.1f					# 当たり判定を取る範囲\n", COL_RANGE, pObject->GetColRange());
							fprintf(pFile, "	%s%.1f %.1f %.1f			# モデルの座標\n", POS, pObject->GetPos().x, pObject->GetPos().y, pObject->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの向き\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの大きさ\n", SCALE, pObject->GetScale().x, pObject->GetScale().y, pObject->GetScale().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# モデルの回転するスピード\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
							fprintf(pFile, "%s\n", END_OBJECTSET);
							fprintf(pFile, "\n");
						}
					}
					else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
					{// 動く配置物クラスへのポインタだった
						pObjectMove = (CObjectMove*)pScene;
						if (pObjectMove != NULL)
						{// オブジェクトへのポインタが取得できた
						 // 事前に計算をすませておく
							D3DXVECTOR3 ModelRot = pObjectMove->GetRotStd();
							D3DXVECTOR3 ModelRollSpeed = pObjectMove->GetRollSpeed();
							// 向き
							ModelRot.x = D3DXToDegree(ModelRot.x);
							ModelRot.y = D3DXToDegree(ModelRot.y);
							ModelRot.z = D3DXToDegree(ModelRot.z);

							// 回転するスピード
							ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
							ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
							ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

							// 配置物の情報を書き込み
							fprintf(pFile, "%s\n", OBJECTMOVESET);
							fprintf(pFile, "	%s%d						# 使用するモデルの番号\n", TYPE, pObjectMove->GetModelIdx());
							fprintf(pFile, "	%s%d					# 当たり判定の種類\n", COLLISION, pObject->GetCollision());
							fprintf(pFile, "	%s%.1f					# 当たり判定を取る範囲\n", COL_RANGE, pObject->GetColRange());
							fprintf(pFile, "	%s%.1f %.1f %.1f			# モデルの座標\n", POS, pObjectMove->GetPosStd().x, pObjectMove->GetPosStd().y, pObjectMove->GetPosStd().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの向き\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの大きさ\n", SCALE, pObjectMove->GetScale().x, pObjectMove->GetScale().y, pObjectMove->GetScale().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# モデルの回転するスピード\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの移動量\n", MOVE, pObjectMove->GetMove().x, pObjectMove->GetMove().y, pObjectMove->GetMove().z);
							fprintf(pFile, "	%s%d					# 移動量を切り替える時間\n", MOVE_MAX, pObjectMove->GetMaxMoveCounter());
							fprintf(pFile, "%s\n", END_OBJECTMOVESET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}


			// コメント部分を書き込み
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    敵の配置情報を保存する処理
//=============================================================================
void CMap::SaveEnemy(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(MAP_ENEMY_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       敵の配置情報スクリプトファイル[save_set_enemy.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  使用する敵のデータ\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// 敵のデータファイル名を保存
			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_TYPE; nCntEnemy++)
			{// 敵のデータを読み込める回数分繰り返し
				if (m_pEnemyAssembly != NULL)
				{
					CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nCntEnemy);
					if (pEnemyManager != NULL)
					{
						fprintf(pFile, "%s%s		# 敵%d体目\n", ENEMYDATA_FILENAME, pEnemyManager->GetFileName(), nCntEnemy + 1);
					}
				}
			}
			fprintf(pFile, "\n");

			// コメント部分を書き込み
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  敵の配置情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// 敵の配置情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CEnemy *pEnemy = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
					{// 敵クラスへのポインタだった
						pEnemy = (CEnemy*)pScene;
						if (pEnemy != NULL)
						{// 敵へのポインタが取得できた
						    // 事前に計算をすませておく
							D3DXVECTOR3 EnemyRot = pEnemy->GetRotStd();
							// 向き
							EnemyRot.x = D3DXToDegree(EnemyRot.x);
							EnemyRot.y = D3DXToDegree(EnemyRot.y);
							EnemyRot.z = D3DXToDegree(EnemyRot.z);

							// 敵の配置情報を書き込み
							fprintf(pFile, "%s\n", ENEMYSET);
							fprintf(pFile, "	%s						# AIの情報\n", AISET);
							fprintf(pFile, "		%s%f	# 警戒できる範囲", CHEACKDISTANCE, pEnemy->GetAI_CheackDistance());
							fprintf(pFile, "		%s%f	# 攻撃を仕掛ける範囲", ATTACKDISTANCE, pEnemy->GetAI_AttackDistance());
							fprintf(pFile, "		%s%d		# 攻撃を仕掛けるタイミング", ATTACKRANDOM, pEnemy->GetAI_AttackTiming());
							fprintf(pFile, "	%s\n\n", END_AISET);
							fprintf(pFile, "	%s%d					# 種類\n", TYPE, pEnemy->GetType());
							fprintf(pFile, "	%s%d					# 体力\n", LIFE, pEnemy->GetLife());
							fprintf(pFile, "	%s%d				# 倒した時に増えるスコア量\n", SCORE, pEnemy->GetAddScore());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# 座標\n", POS, pEnemy->GetPosStd().x, pEnemy->GetPosStd().y, pEnemy->GetPosStd().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f			# 向き\n", ROT, EnemyRot.x, EnemyRot.y, EnemyRot.z);
							fprintf(pFile, "%s\n", END_ENEMYSET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}

			// コメント部分を書き込み
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置用ビルボード情報を保存する処理
//=============================================================================
void CMap::SaveBillboard(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(MAP_BILLBOARD_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       配置用ビルボード情報スクリプトファイル[save_billboard.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  ビルボード情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// ビルボード情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CBillboardObject *pBillboardObject = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_BILLBOARDOBJ)
					{// 配置用ビルボードクラスへのポインタだった
						pBillboardObject = (CBillboardObject*)pScene;
						if (pBillboardObject != NULL)
						{// 配置用ビルボードへのポインタが取得できた
						    // 事前に計算をすませておく
							float fBillboardRot = pBillboardObject->GetRot();
							// 向き
							fBillboardRot = D3DXToDegree(fBillboardRot);

							// 配置物の情報を書き込み
							fprintf(pFile, "%s\n", BILLBOARDSET);
							fprintf(pFile, "	%s%d				# 種類番号\n", TYPE, pBillboardObject->GetType());
							fprintf(pFile, "	%s%d				# 使用するテクスチャの番号\n", TEX_TYPE, pBillboardObject->GetTexIdx());
							fprintf(pFile, "	%s%.1f				# 当たり判定を取る範囲\n", COL_RANGE, pBillboardObject->GetColRange());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# 座標\n", POS, pBillboardObject->GetPos().x, pBillboardObject->GetPos().y, pBillboardObject->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f %.1f		# 色\n", COL, pBillboardObject->GetCol().r, pBillboardObject->GetCol().g, pBillboardObject->GetCol().b, pBillboardObject->GetCol().a);
							fprintf(pFile, "	%s%.1f					# 向き\n", ROT, fBillboardRot);
							fprintf(pFile, "	%s%.1f				# 幅\n", WIDTH, pBillboardObject->GetWidth());
							fprintf(pFile, "	%s%.1f				# 高さ\n", HEIGHT, pBillboardObject->GetHeight());
							fprintf(pFile, "	%s%d				# ライティングするかしないか\n", LIGHTING, pBillboardObject->GetLighting());
							fprintf(pFile, "	%s%d			# 加算合成で描画するかしないか\n", DRAW_ADDTIVE, pBillboardObject->GetDrawAddtive());
							fprintf(pFile, "%s\n", END_BILLBOARDSET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}


			// コメント部分を書き込み
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置エフェクト情報を保存する処理
//=============================================================================
void CMap::SaveEffect(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(MAP_EFFECT_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       配置エフェクト情報スクリプトファイル[save_effect.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  配置エフェクト情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// エミッタ情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CEmitter *pEmitter = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
					{// エミッタクラスへのポインタだった
						pEmitter = (CEmitter*)pScene;
						if (pEmitter != NULL)
						{// エミッタへのポインタが取得できた
						    // 事前に計算をすませておく
							D3DXVECTOR3 EffectRot = pEmitter->GetRot();
							// 向き
							EffectRot.x = D3DXToDegree(EffectRot.x);
							EffectRot.y = D3DXToDegree(EffectRot.y);
							EffectRot.z = D3DXToDegree(EffectRot.z);

							// 配置エフェクトの情報を書き込み
							fprintf(pFile, "%s\n", EFFECTSET);
							fprintf(pFile, "	%s%d				# 使用するエフェクトの種類番号\n", TYPE, pEmitter->GetType());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# 座標\n", POS, pEmitter->GetPos().x, pEmitter->GetPos().y, pEmitter->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# 向き\n", ROT, EffectRot.x, EffectRot.y, EffectRot.z);
							fprintf(pFile, "%s\n", END_EFFECTSET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}

			// コメント部分を書き込み
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    壁情報を保存する処理
//=============================================================================
void CMap::SaveWall(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(MAP_WALL_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       壁情報スクリプトファイル[save_wall.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  壁情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// 壁情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CMeshWall *pMeshWall = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_MESHWALL)
					{// 壁クラスへのポインタだった
						pMeshWall = (CMeshWall*)pScene;
						if (pMeshWall != NULL)
						{// 配置用ビルボードへのポインタが取得できた
						    // 事前に計算をすませておく
							D3DXVECTOR3 WallRot = pMeshWall->GetRot();
							// 向き
							WallRot.x = D3DXToDegree(WallRot.x);
							WallRot.y = D3DXToDegree(WallRot.y);
							WallRot.z = D3DXToDegree(WallRot.z);

							// 壁の情報を書き込み
							fprintf(pFile, "%s\n", WALLSET);
							fprintf(pFile, "	%s%d				# 使用するテクスチャの番号\n", TEX_TYPE, pMeshWall->GetTexIdx());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# 座標\n", POS, pMeshWall->GetPos().x, pMeshWall->GetPos().y, pMeshWall->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f			# 向き\n", ROT, WallRot.x, WallRot.y, WallRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f %.1f		# 色\n", COL, pMeshWall->GetCol().r, pMeshWall->GetCol().g, pMeshWall->GetCol().b, pMeshWall->GetCol().a);
							fprintf(pFile, "	%s%.1f				# 1ブロック分の幅\n", WIDTH, pMeshWall->GetWidth());
							fprintf(pFile, "	%s%.1f				# 1ブロック分の高さ\n", HEIGHT, pMeshWall->GetHeight());
							fprintf(pFile, "	%s%d					# 横の分割数\n", XBLOCK, pMeshWall->GetXBlock());
							fprintf(pFile, "	%s%d					# 縦の分割数\n", YBLOCK, pMeshWall->GetYBlock());
							fprintf(pFile, "	%s%d				# テクスチャU座標の分割数\n", TEXSPLIT_U, pMeshWall->GetTexSplitU());
							fprintf(pFile, "	%s%d				# テクスチャV座標の分割数\n", TEXSPLIT_V, pMeshWall->GetTexSplitV());
							fprintf(pFile, "%s\n", END_WALLSET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}


			// コメント部分を書き込み
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    アイテム情報を保存する処理
//=============================================================================
void CMap::SaveItem(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(MAP_ITEM_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       アイテム情報スクリプトファイル[save_item.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  アイテム情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// アイテム情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CItem *pItem = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_ITEM)
					{// アイテムクラスへのポインタだった
						pItem = (CItem*)pScene;
						if (pItem != NULL)
						{// アイテムクラスへのポインタが取得できた
						    // 事前に計算をすませておく
							float fItemRot = pItem->GetRot();
							// 向き
							fItemRot = D3DXToDegree(fItemRot);

							// アイテムの情報を書き込み
							fprintf(pFile, "%s\n", ITEMSET);
							fprintf(pFile, "	%s%d				# 使用するテクスチャの番号\n", TEX_TYPE, pItem->GetTexIdx());
							fprintf(pFile, "	%s%d					# 種類\n", TYPE, pItem->GetType());
							fprintf(pFile, "	%s%.1f				# 当たり判定を取る範囲\n", COL_RANGE, pItem->GetColRange());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# 座標\n", POS, pItem->GetPos().x, pItem->GetPos().y, pItem->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f %.1f		# 色\n", COL, pItem->GetCol().r, pItem->GetCol().g, pItem->GetCol().b, pItem->GetCol().a);
							fprintf(pFile, "	%s%.1f					# 向き\n", ROT, fItemRot);
							fprintf(pFile, "	%s%.1f				# 幅\n", WIDTH, pItem->GetWidth());
							fprintf(pFile, "	%s%.1f				# 高さ\n", HEIGHT, pItem->GetHeight());
							fprintf(pFile, "	%s%d				# ライティングするかしないか\n", LIGHTING, pItem->GetLighting());
							fprintf(pFile, "	%s%d			# 加算合成で描画するかしないか\n", DRAW_ADDTIVE, pItem->GetDrawAddtive());
							fprintf(pFile, "%s\n", END_ITEMSET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}


			// コメント部分を書き込み
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置物を配置する処理
//=============================================================================
void CMap::SetObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, D3DXVECTOR3 RollSpeed, int nModelIdx)
{
	// 配置物を生成する
	CObject *pObject = CObject::Create(Pos, Rot, Scale, RollSpeed);
	if (pObject != NULL)
	{
		pObject->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObject->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    動く配置物を配置する処理
//=============================================================================
void CMap::SetObjectMove(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, D3DXVECTOR3 RollSpeed, int nModelIdx, D3DXVECTOR3 Move, int nMaxMoveCounter)
{
	// 動く配置物を生成する
	CObjectMove *pObjectMove = CObjectMove::Create(Pos, Rot, Scale, RollSpeed, Move, nMaxMoveCounter);
	if (pObjectMove != NULL)
	{
		pObjectMove->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObjectMove->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    ビルボードを配置する処理
//=============================================================================
void CMap::SetBillboard(D3DXVECTOR3 Pos, D3DXCOLOR Col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	CBillboardObject *pBillObj = CBillboardObject::Create(Pos, Col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType);
	if (pBillObj != NULL)
	{
		pBillObj->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    エフェクトを配置する処理
//=============================================================================
void CMap::SetEffect(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType)
{
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->SetEffect(Pos, Rot, nType);
	}
}

//=============================================================================
//    敵を配置する処理
//=============================================================================
void CMap::SetEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nScore)
{
	if (m_pEnemyAssembly != NULL)
	{
		CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nType);
		if (pEnemyManager != NULL)
		{
			pEnemyManager->SetEnemy(Pos, Rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nScore);
		}
	}
}

//=============================================================================
//    壁を配置する処理
//=============================================================================
void CMap::SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx)
{
	// 壁を生成する
	CMeshWall *pMeshWall = CMeshWall::Create(pos, rot, col, fWidth, fHeight, nXBlock, nYBlock, nTexSplitU, nTexSplitV, nTexIdx);
	if (pMeshWall != NULL)
	{// 壁が生成できた
		pMeshWall->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    アイテムを配置する処理
//=============================================================================
void CMap::SetItem(D3DXVECTOR3 Pos, D3DXCOLOR Col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	CItem *pItem = CItem::Create(Pos, Col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType);
	if (pItem != NULL)
	{
		pItem->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    読み込んだマップのファイル名を取得する処理
//=============================================================================
char *CMap::GetMapName(void)
{
	return m_aFileName;
}

//=============================================================================
//    敵データを読み込んだ回数を取得する処理
//=============================================================================
int CMap::GetCntEnemyLoad(void)
{
	return m_nCntLoadEnemy;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelManager *CMap::GetModelManager(void)
{
	return m_pModelManager;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する処理
//=============================================================================
CTextureManager *CMap::GetTextureManager(void)
{
	return m_pTextureManager;
}