//*****************************************************************************
//
//     エネミーの処理[enemy.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "gauge.h"
#include "score.h"
#include "ui.h"
#include "meshField.h"
#include "object.h"
#include "meshWall.h"
#include "player.h"
#include "game.h"
#include "result.h"
#include "map.h"
#include "effectManager.h"
#include "particle.h"
#include "orbitEffect.h"
#include "shadow.h"
#include "textureManager.h"
#include "tornade.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// 敵のデータ設定用マクロ
#define ENEMY_GAUGE_POS              (D3DXVECTOR3(0.0f,90.0f,0.0f))     // 敵のゲージオフセット位置
#define ENEMY_GAUGE_WIDTH            (40.0f)                            // 敵の体力ゲージの幅
#define ENEMY_GAUGE_HEIGHT           (5.0f)                             // 敵の体力ゲージの高さ
#define ENEMY_SHADOW_PRIORITY        (9)                                // 敵の影の処理優先順位
#define ENEMY_SHADOW_WIDTH           (13.0f)                            // 敵の影の幅
#define ENEMY_SHADOW_HEIGHT          (13.0f)                            // 敵の影の高さ
#define ENEMY_SHADOW_COLOR           (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // 敵の影の色

// ボスのデータ設定用マクロ
#define BOSS_SHADOW_WIDTH            (18.0f)                            // ボスの影の幅
#define BOSS_SHADOW_HEIGHT           (18.0f)                            // ボスの影の高さ
#define BOSS_SHADOW_COLOR            (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // ボスの影の色
#define BOSS_INERTIA                 (0.423f)                           // ボスの慣性
#define BOSS_ATTACK_ORBIT            (420)                              // 軌跡を飛ばして攻撃し続ける時間
#define BOSS_STEP_MOVE               (6.0f)                             // ステップを踏んだ時の移動量
#define BOSS_ATTACK_ORBIT_INTERVAL_0 (40)                               // 軌跡を飛ばす間隔(体力がまだあるとき)
#define BOSS_ATTACK_ORBIT_INTERVAL_1 (30)                               // 軌跡を飛ばす間隔(体力がまだ半分あるとき)
#define BOSS_ATTACK_ORBIT_INTERVAL_2 (20)                               // 軌跡を飛ばす間隔(体力がもうないとき)
#define BOSS_RESTART_TIRED           (240)                              // 疲れ状態から起き上がるまでの時間
#define BOSS_BONUS                   (10000)                            // ラスボス撃破ボーナス
#define BOSS_AI_NEAR_DISTANCE        (180.0f)                           // プレイヤーが近いと判定される距離
#define BOSS_AI_ATTACK_DISTANCE      (200.0f)                           // 攻撃を仕掛ける距離
#define BOSS_RED_CHANGETIMING_0      (120)                              // 体力が低いときにモデルの色を変える値を反転させるタイミング(まだ体力ある)
#define BOSS_RED_CHANGETIMING_1      (40)                               // 体力が低いときにモデルの色を変える値を反転させるタイミング(もう体力ない)
#define BOSS_RED_CHANGE_0            (0.008f)                           // 体力が低いときにモデルの色を変える値(まだ体力ある)
#define BOSS_RED_CHANGE_1            (0.02f)                            // 体力が低いときにモデルの色を変える値(もう体力ない)

// 値読み込みをする際の目印となる文字列
// 共通
#define POS                "POS = "                // 座標を読み取る合図
#define ROT                "ROT = "                // 向きを読み取る合図
#define MOVE               "MOVE = "               // 移動量を読み取る合図

// その他
#define NUM_TEXTURE        "NUM_TEXTURE = "        // 読み込むテクスチャの数
#define TEXTURE_FILENAME   "TEXTURE_FILENAME = "   // 読み込むテクスチャのファイル名
#define NUM_MODEL          "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME     "MODEL_FILENAME = "     // 読み込むモデルのファイル名

// キャラクター情報
#define CHARACTERSET       "CHARACTERSET"          // キャラクター情報読み込み開始の合図
#define END_CHARACTERSET   "END_CHARACTERSET"      // キャラクター情報読み込み終了の合図
#define JUMP               "JUMP = "               // ジャンプ量
#define GRAVITY            "GRAVITY = "            // 重力量
#define COL_HEIGHT         "COL_HEIGHT = "         // 当たり判定をとる高さ
#define COL_RANGE          "COL_RANGE = "          // 当たり判定をとる範囲
#define NUM_PARTS          "NUM_PARTS = "          // キャラクターのパーツ数

// パーツ情報
#define PARTSSET           "PARTSSET"              // パーツ情報読み込み開始の合図
#define END_PARTSSET       "END_PARTSSET"          // パーツ情報読み込み終了の合図
#define INDEX              "INDEX = "              // 番号を読み取る合図
#define PARENT             "PARENT = "             // 親パーツの番号を読み取る合図

// モーション情報
#define MOTIONSET         "MOTIONSET"             // モーション情報読み込み開始の合図
#define END_MOTIONSET     "END_MOTIONSET"         // モーション情報読み込み終了の合図
#define COLLISIONSET      "COLLISIONSET"          // モーションの当たり判定情報読み込み開始の合図
#define END_COLLISIONSET  "END_COLLISIONSET"      // モーションの当たり判定情報読み込み終了の合図
#define ORBITSET          "ORBITSET"              // モーションの軌跡情報読み込み開始の合図
#define END_ORBITSET      "END_ORBITSET"          // モーションの軌跡情報読み込み終了の合図
#define RANGE             "RANGE = "              // 範囲を読み取る合図
#define XBLOCK            "XBLOCK = "             // 軌跡の横の分割数を読み取る合図
#define ZBLOCK            "ZBLOCK = "             // 軌跡の奥行の分割数を読み取る合図
#define OFFSET            "OFFSET = "             // オフセットを読み取る合図
#define LOOP              "LOOP = "               // モーションがループ再生するかどうかを読み取る合図
#define BLEND             "BLEND = "              // モーションブレンドの倍率を読み取る合図
#define NUM_KEY           "NUM_KEY = "            // モーションのキーフレームの数を読み取る合図

// 攻撃モーション情報
#define ATTACKSET         "ATTACKSET"             // 攻撃モーション情報読み込み開始の合図
#define END_ATTACKSET     "END_ATTACKSET"         // 攻撃モーション情報読み込み終了の合図
#define NEXT_MOTION       "NEXT_MOTION = "        // 次のモーションの番号

// 当たり判定情報
#define NUM_COL           "NUM_COL = "            // モーションの当たり判定情報の数を読み取る合図
#define COLLISION         "COLLISION"             // 当たり判定情報読み込み開始の合図
#define DAMAGE            "DAMAGE = "             // 当たった場合のダメージ
#define END_COLLISION     "END_COLLISION"         // 当たり判定情報読み込み終了の合図

// 軌跡情報
#define NUM_ORBIT         "NUM_ORBIT = "          // モーションの軌跡情報の数を読み取る合図
#define ORBIT		      "ORBIT"                 // 軌跡情報読み込み開始の合図
#define END_ORBIT	      "END_ORBIT"             // 軌跡情報読み込み終了の合図
#define TEX_INDEX         "TEX_INDEX = "          // 軌跡に張り付けるテクスチャの番号を読み取る合図
#define OFFSET1           "OFFSET1 = "            // １個目のオフセットを読み取る合図
#define OFFSET2           "OFFSET2 = "            // ２個目のオフセットを読み取る合図
#define OFFSETAMP1        "OFFSETAMP1 = "         // 軌跡のオフセット距離増幅値を読み取る合図(１個目)
#define OFFSETAMP2        "OFFSETAMP2 = "         // 軌跡のオフセット距離増幅値を読み取る合図(２個目)
#define COLUP             "COLUP = "              // 軌跡の頂点上側の色
#define COLDOWN           "COLDOWN = "            // 軌跡の頂点下側の色
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // 軌跡の頂点上側の透明度減衰値
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // 軌跡の頂点下側の透明度減衰値
#define START             "START = "              // 軌跡を出し始めるタイミングを読み取る合図
#define FINISH            "FINISH = "             // 軌跡をしまうタイミングを読み取る合図

// キーフレーム情報
#define KEYSET             "KEYSET"                // キーフレーム情報読み込み開始の合図
#define END_KEYSET         "END_KEYSET"            // キーフレーム情報読み込み終了の合図
#define FRAME              "FRAME = "              // キーフレームの再生フレーム数を読み取る合図

// キー情報
#define KEY                "KEY"                   // キー情報読み込み開始の合図
#define END_KEY            "END_KEY"               // キー情報読み込み終了の合図

//*****************************************************************************
//     CEnemyManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEnemyManager::CEnemyManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	strcpy(m_aFileName, "\0");      // プレイヤーデータのスクリプトファイル名
	m_pModelManager = NULL;         // モデル管轄クラスへのポインタ

	m_nNumModel = 0;                // 読み込むモデル数
	m_nType = 0;                    // 種類
	m_nNumParts = 0;                // 読み込むパーツ数
	m_pNumParent = NULL;            // 親モデルの番号
	m_pMotionManager = NULL;        // モーション管理クラスへのポインタ
	m_apModel = NULL;               // モデルクラスへのポインタ
	m_fGravity = 0.0f;              // 敵の重力情報
	m_fColHeight = 0.0f;            // 当たり判定を取る高さ
	m_fColRange = 0.0f;             // 当たり判定を取る範囲
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEnemyManager::~CEnemyManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEnemyManager *CEnemyManager::Create(int nType, char *pFileName, int nPriority)
{
	CEnemyManager *pEnemyManager = NULL;   // エネミー管轄クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pEnemyManager == NULL)
		{// メモリが空になっている
			pEnemyManager = new CEnemyManager(nPriority);
			if (pEnemyManager != NULL)
			{// インスタンスを生成できた
				pEnemyManager->SetFileName(pFileName);
				pEnemyManager->SetType(nType);
				if (FAILED(pEnemyManager->Init()))
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

	return pEnemyManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEnemyManager::Init(void)
{
	// エネミーのパーツ情報を抜き出す
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTexture = 0;                        // 読み込むテクスチャの数
			int nCntModel = 0;                          // モデルを読み込んだ回数を数える
			int nCntParts = 0;                          // パーツ情報を読み込んだ回数を数える
			int nCntMotion = 0;                         // モーションを読み込んだ回数を数える
			int nCntTex = 0;                            // テクスチャを読み込んだ回数を数える
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ情報へのポインタ
			LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                          // マテリアル情報の数
			CMotion *pMotion[CEnemy::STATE_MAX] = {};   // モーションクラスへのポインタ

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// メモリが確保されている
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// 読み込むテクスチャ数だった
								strcpy(pStr, pStrCur);
								nNumTexture = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTexture >= 1)
								{// 読み込むテクスチャ数が1つ以上ある
									m_pTextureManager = CTextureManager::Create(nNumTexture);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名を読み込んだ
								if (m_pTextureManager != NULL && nCntTex < nNumTexture)
								{// テクスチャ管轄クラスへのポインタが確保されている
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
							}
							if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// モデル数を読み込むなら
								strcpy(pStr, pStrCur);
								m_nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (m_nNumModel >= 1)
								{// モデル数が1つ以上ある
									m_pNumParent = new int[m_nNumModel];
									m_pModelManager = CModelManager::Create(m_nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// モデルのファイルパス名を読み込んだ
								if (m_pModelManager != NULL && nCntModel < m_nNumModel)
								{// モデル管轄クラスへのポインタが確保されている
									// モデルのファイルパス名を読み取る
									pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

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
							}
							else if (CFunctionLib::Memcmp(pStrCur, CHARACTERSET) == 0)
							{// キャラクター情報読み込み開始の合図があった
								m_apModel = ReadCharacterSet(pLine, pStrCur, pFile, &m_nNumParts, m_pNumParent, &m_fGravity, &m_fColHeight, &m_fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// モーション情報読み込み開始の合図があった
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &m_nNumParts, pMotion[nCntMotion], nCntMotion);
								nCntMotion++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
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

			// 必要なクラスを作成しておく
			// モーション管轄クラス
			if (m_pMotionManager == NULL)
			{// ポインタが確保されている
				m_pMotionManager = CMotionManager::Create(CEnemy::STATE_MAX, m_nNumParts);
				if (m_pMotionManager != NULL)
				{// モーション管理クラスが作成できた
					for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
					{// 状態の数だけ繰りかえし
						m_pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// プレイヤークラスを取得して保存しておく
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
							m_pPlayer = pPlayer;
							break;
						}
					}
					pScene = pSceneNext;
				}
			}

			// ファイルを閉じる
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

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEnemyManager::Uninit(void)
{
	// プレイヤークラスへのポインタを空にしておく
	m_pPlayer = NULL;

	// 読み取り用のポインタを開放しておく
	if (m_apModel != NULL)
	{// メモリが確保されている
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// 使用できるモデル数分繰り返し
			if (m_apModel[nCntModel] != NULL)
			{// ポインタが確保されている
				m_apModel[nCntModel]->Uninit();

				// メモリの開放
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// メモリの開放
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// モーションの破棄
	if (m_pMotionManager != NULL)
	{// ポインタが確保されている
		m_pMotionManager->Uninit();

		// メモリの開放
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// 親モデルの番号
	if (m_pNumParent != NULL)
	{// メモリが確保されている
		delete[] m_pNumParent;
		m_pNumParent = NULL;
	}

	// モデル管轄クラスの破棄
	if (m_pModelManager != NULL)
	{// メモリが確保されている
	    // 終了処理
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// 自身のポインタを破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEnemyManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CEnemyManager::Draw(void)
{

}

//=============================================================================
//    敵を配置する処理
//=============================================================================
void CEnemyManager::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// エネミークラスを作成する
	CreateEnemy(m_apModel, m_pMotionManager, m_nNumParts, m_pNumParent, m_fGravity, m_fColHeight, m_fColRange, pos, rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore);
}

//=============================================================================
//    マップエディット用の敵を配置する処理
//=============================================================================
CSetEnemy *CEnemyManager::SetMapEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// エネミークラスを作成する
	CSetEnemy *pSetEnemy = NULL;
	return pSetEnemy = CreateSetEnemy(m_apModel, m_pMotionManager, m_nNumParts, m_pNumParent, m_fGravity, m_fColHeight, m_fColRange, pos, rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore);
}

//=============================================================================
//    エネミークラスを作製する処理
//=============================================================================
CEnemy *CEnemyManager::CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const float fAI_CheackDistance, const float fAI_AttackDistance, const int nAI_AttackTiming, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // モデルクラスコピー用
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // モージョンクラスコピー用
	CMotionCollision **pMotionCollision = NULL;    // 当たり判定データクラスコピー用
	CMotionOrbit **pMotionOrbit = NULL;            // モーションの軌跡データクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;      // モーション管轄クラスコピー用
	CEnemy *pEnemy = NULL;                         // エネミークラスへのポインタ

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// モデルへのポインタとモーションクラスへのポインタとモデル管轄クラスへのポインタが作成されている
	    // モデル情報をコピーする
		if (apModelCpy == NULL)
		{// メモリが確保されている
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// メモリが確保できた
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// パーツ数分繰り返し
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// メモリが確保できる状態である
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// メモリが確保できた
							if (pNumParent[nCntParts] != -1)
							{// 親モデルが存在する
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// モーション情報をコピーする
		if (pMotionManagerCpy == NULL)
		{// メモリが確保できる状態である
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// 敵の状態の数だけ繰り返し
					if (pMotionManager->GetMotion()[nCntMotion] != NULL)
					{// モーションが生成されている
						if (pMotionCpy[nCntMotion] == NULL)
						{// メモリを確保できる状態である
							pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
							if (pMotionCpy[nCntMotion] != NULL)
							{// メモリが確保できた
								// 当たり判定データクラスを設定する
								int nNumColData = pMotionManager->GetMotion()[nCntMotion]->GetNumColData();
								if (nNumColData >= 1)
								{// 1つ以上当たり判定データクラスが存在する
									pMotionCollision = new CMotionCollision*[nNumColData];
									if (pMotionCollision != NULL)
									{// メモリを確保できた
										for (int nCntNumCol = 0; nCntNumCol < nNumColData; nCntNumCol++)
										{// 当たり判定データクラスの数だけ繰り返し
											pMotionCollision[nCntNumCol] = new CMotionCollision;
											if (pMotionCollision[nCntNumCol] != NULL)
											{// メモリを確保できた
												// データをコピーする
												pMotionCollision[nCntNumCol]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetModelIdx());
												pMotionCollision[nCntNumCol]->SetOffsetPos(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetOffsetPos());
												pMotionCollision[nCntNumCol]->SetRange(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetRange());
												pMotionCollision[nCntNumCol]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetStartTiming());
												pMotionCollision[nCntNumCol]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetFinishTiming());
												pMotionCollision[nCntNumCol]->SetDamage(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetDamage());
											}
										}
										pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
									}
								}
								pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());

								// 軌跡データクラスを設定する
								// 軌跡データクラスのコピー
								if (pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
								{// 当たり判定データが１個以上ある
									pMotionOrbit = new CMotionOrbit*[pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
									if (pMotionOrbit != NULL)
									{// メモリが確保できた
										for (int nCntOrbit = 0; nCntOrbit < pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
										{// 当たり判定データの数だけ繰り返し
											pMotionOrbit[nCntOrbit] = new CMotionOrbit;
											if (pMotionOrbit[nCntOrbit] != NULL)
											{// メモリが確保できた
												pMotionOrbit[nCntOrbit]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
												pMotionOrbit[nCntOrbit]->SetTexIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
												pMotionOrbit[nCntOrbit]->SetOffsetPos1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
												pMotionOrbit[nCntOrbit]->SetOffsetPos2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
												pMotionOrbit[nCntOrbit]->SetOffsetAmp1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
												pMotionOrbit[nCntOrbit]->SetOffsetAmp2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
												pMotionOrbit[nCntOrbit]->SetColUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
												pMotionOrbit[nCntOrbit]->SetColDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
												pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
												pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
												pMotionOrbit[nCntOrbit]->SetXBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
												pMotionOrbit[nCntOrbit]->SetYBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
												pMotionOrbit[nCntOrbit]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
												pMotionOrbit[nCntOrbit]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
											}
										}
									}
									pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								}
								pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());

								for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
								{// キーフレームの総数分繰り返し
									for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
									{// キーフレームの総数分繰り返し
										pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
										pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
									}
								}

								// モーション管轄クラスにポインタを渡す
								pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
							}
						}
					}
				}
			}
		}

		// エネミークラスを作成する
		if (pEnemy == NULL)
		{// メモリが確保できる状態である
			if (nType == CEnemy::TYPE_BOSS)
			{// ボス敵だった
				pEnemy = CBoss::Create(pos, rot, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore, ENEMY_PRIORITY);
				if (pEnemy != NULL)
				{// 生成に成功した
					if (m_pTextureManager != NULL)
					{// テクスチャ管轄クラスが生成されている
						pEnemy->SetTextureManager(m_pTextureManager);
					}
				}
			}
			else
			{// ボスの敵ではない
				pEnemy = CEnemy::Create(pos, rot, (CEnemy::TYPE)nType, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore, ENEMY_PRIORITY);
			}
		}
	}

	return pEnemy;
}

//=============================================================================
//    配置用のエネミークラスを作製する処理
//=============================================================================
CSetEnemy *CEnemyManager::CreateSetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const float fAI_CheackDistance, const float fAI_AttackDistance, const int nAI_AttackTiming, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // モデルクラスコピー用
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // モージョンクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;      // モーション管轄クラスコピー用
	CSetEnemy *pSetEnemy = NULL;                   // 配置用エネミークラスへのポインタ

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// モデルへのポインタとモーションクラスへのポインタとモデル管轄クラスへのポインタが作成されている
	    // モデル情報をコピーする
		if (apModelCpy == NULL)
		{// メモリが確保されている
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// メモリが確保できた
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// パーツ数分繰り返し
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// メモリが確保できる状態である
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// メモリが確保できた
							if (pNumParent[nCntParts] != -1)
							{// 親モデルが存在する
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// モーション情報をコピーする
		if (pMotionManagerCpy == NULL)
		{// メモリが確保できる状態である
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// 敵の状態の数だけ繰り返し
					if (pMotionCpy[nCntMotion] == NULL)
					{// メモリを確保できる状態である
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// メモリが確保できた
						    // 攻撃モーションデータクラスを設定する
							pMotionCpy[nCntMotion]->SetAttack(pMotionManager->GetMotion()[nCntMotion]->GetAttack());

							// 当たり判定データクラスを設定する
							pMotionCpy[nCntMotion]->SetCollision(pMotionManager->GetMotion()[nCntMotion]->GetCollision());
							pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());

							// 軌跡データクラスを設定する
							pMotionCpy[nCntMotion]->SetOrbit(pMotionManager->GetMotion()[nCntMotion]->GetOrbit());
							pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());

							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// キーフレームの総数分繰り返し
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// キーフレームの総数分繰り返し
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// モーション管轄クラスにポインタを渡す
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// 配置用エネミークラスを作成する
		if (pSetEnemy == NULL)
		{// メモリが確保できる状態である
			pSetEnemy = CSetEnemy::Create(pos, rot, (CEnemy::TYPE)nType, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore);
		}
	}

	return pSetEnemy;
}

//=============================================================================
//    エネミー情報を読み取る処理
//=============================================================================
CModel **CEnemyManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// 重力量がある
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	当たり判定を取る高さがある
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// 当たり判定を取る範囲がある
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// パーツ数情報がある
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			if (apModel == NULL)
			{// メモリが確保できる状態である
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// パーツ情報読み込み開始の合図だった
			if (nCntParts < *pNumParts)
			{// まだパーツ情報を読み込める
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_CHARACTERSET) == 0)
		{// キャラクター情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return apModel;
}

//=============================================================================
//    パーツ情報を読み取る処理
//=============================================================================
CModel *CEnemyManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // パーツ番号情報
	int nParent = -1;                                   // 親モデルのパーツ番号
	CModel *pModel = NULL;                             // モデルクラスへのポインタ
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // モデルの座標情報
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // モデルの向き情報
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// パーツ番号情報がある
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// 親モデルのパーツ番号情報がある
			pNumParent[nIdx] = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// モデルの座標情報がある
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// モデルの向き情報がある
			Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_PARTSSET) == 0)
		{// パーツ情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	if (pModel == NULL)
	{// メモリを確保できる状態である
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx),m_pModelManager->GetTexture(nIdx));
		if (pModel != NULL)
		{// モデルポインタが確保された
			if (nParent != -1)
			{// 親モデルが存在する
				pModel->SetParent(apModel[nParent]);
			}
		}
	}

	return pModel;
}

//=============================================================================
//    モーション情報を読み取る処理
//=============================================================================
CMotion *CEnemyManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;                           // キーフレームフレーム情報を読み込んだ回数
	CMotionAttack *pMotionAttack = NULL;          // 攻撃モーションデータクラスへのポインタ
	CMotionCollision **pMotionCollision = NULL;   // モーションの当たり判定データクラスへのポインタ
	int nNumColData = 0;                          // 当たり判定データの数
	CMotionOrbit **pMotionOrbit = NULL;           // モーションの軌跡データクラスへのポインタ
	int nNumOrbitData = 0;                        // 軌跡データの数
	bool bLoop = false;                           // ループするかしないか
	float fBlendMag = 0.0f;                       // ブレンド倍率
	int nNumKey = 0;                              // キーフレームの総数
	int *pPlaybackKey = NULL;                     // 再生フレーム数(複数あるので動的確保)
	D3DXVECTOR3 **pAddPos = NULL;                 // モデルの座標
	D3DXVECTOR3 **pDestRot = NULL;                // モデルの向き
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
		{// ループするかしないかの情報がある
			bLoop = CFunctionLib::ReadBool(pStrCur, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BLEND) == 0)
		{// ブレンド倍率の情報がある
			fBlendMag = CFunctionLib::ReadFloat(pStrCur, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_KEY) == 0)
		{// キーフレームの総数の情報がある
			nNumKey = (CFunctionLib::ReadInt(pStrCur, NUM_KEY));
			if (nNumKey >= 1)
			{// 1個以上キーフレームがある
			    // 必要な変数を動的に確保する
			    // 再生フレーム数
				pPlaybackKey = new int[nNumKey];

				// 座標
				if (pAddPos == NULL)
				{// メモリが確保できる状態である
					pAddPos = new D3DXVECTOR3*[nNumKey];
					if (pAddPos != NULL)
					{// メモリが確保できた
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// キーフレームの総数分繰り返し
							pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}

				if (pDestRot == NULL)
				{// メモリが確保できる状態である
				 // 向き
					pDestRot = new D3DXVECTOR3*[nNumKey];
					if (pDestRot != NULL)
					{// メモリが確保できた
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// キーフレームの総数分繰り返し
							pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionAttack = ReadMotionAttackSet(pLine, pStrCur, pFile);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionCollision = ReadMotionCollisionSet(pLine, pStrCur, pFile, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBITSET) == 0)
		{// 軌跡情報読み込み開始の合図だった
			pMotionOrbit = ReadMotionOrbitSet(pLine, pStrCur, pFile, &nNumOrbitData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, KEYSET) == 0)
		{// キーフレーム情報読み込み開始の合図だった
			ReadKeySet(pLine, pStrCur, pFile, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOTIONSET) == 0)
		{// モーション情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	if (pMotion == NULL)
	{// メモリが確保できる状態である
		pMotion = CMotion::Create(*pNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
		if (pMotion != NULL)
		{// メモリが確保できた
	    	// 攻撃モーションデータクラスを設定する
			pMotion->SetAttack(pMotionAttack);

			// 当たり判定データクラスを設定する
			pMotion->SetCollision(pMotionCollision);
			pMotion->SetNumColData(nNumColData);

			// 軌跡データクラスを設定する
			pMotion->SetOrbit(pMotionOrbit);
			pMotion->SetNumOrbitData(nNumOrbitData);

			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// キーフレームの総数分繰り返し
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// キーフレームの総数分繰り返し
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	// メモリを確保した分開放する
	// 再生フレーム数
	if (pPlaybackKey != NULL)
	{// メモリが確保されている
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// 座標
	if (pAddPos != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// 向き
	if (pDestRot != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
//    モーションの攻撃モーション情報を読み取る処理
//=============================================================================
CMotionAttack *CEnemyManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
{
	CMotionAttack *pMotionAttack = NULL;   // 攻撃モーションデータクラスへのポインタ
	pMotionAttack = new CMotionAttack;     // メモリを確保する
	if (pMotionAttack != NULL)
	{// メモリを確保できた
		while (1)
		{// ループ開始
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, NEXT_MOTION) == 0)
			{// 次のモーションの番号情報だった
				pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStrCur, NEXT_MOTION));
			}
			else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
			{// 軌跡を出し始めるタイミング情報だった
				pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
			}
			else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
			{// 軌跡をしまうタイミング情報だった
				pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
			}
			else if (CFunctionLib::Memcmp(pStrCur, END_ATTACKSET) == 0)
			{// 攻撃モーションデータ読み込み終了の合図だった
				break;  // ループ終了
			}
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    モーションの当たり判定情報を読み取る処理
//=============================================================================
CMotionCollision **CEnemyManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
{
	int nCntCollision = 0;                       // 当たり判定データを読み込んだ回数
	CMotionCollision **pMotionCollision = NULL;  // モーションの当たり判定データクラスへのポインタ

	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_COL) == 0)
		{// 当たり判定データの数がある
			*pNumColData = CFunctionLib::ReadInt(pStrCur, NUM_COL);
			if (*pNumColData >= 1)
			{// 当たり判定データが１個以上ある
				pMotionCollision = new CMotionCollision*[*pNumColData];
				if (pMotionCollision != NULL)
				{// メモリが確保できた
					for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
					{// 当たり判定データの数だけ繰り返し
						pMotionCollision[nCntCol] = new CMotionCollision;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// 当たり判定データ読み込み開始の合図だった
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// 当たり判定を取るモデルの番号情報だった
					pMotionCollision[nCntCollision]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET) == 0)
				{// 当たり判定を取るモデルからのオフセット距離情報だった
					pMotionCollision[nCntCollision]->SetOffsetPos(CFunctionLib::ReadVector3(pStrCur, OFFSET));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// 当たり判定を取る範囲情報だった
					pMotionCollision[nCntCollision]->SetRange(CFunctionLib::ReadFloat(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// 当たり判定をし始めるタイミング情報だった
					pMotionCollision[nCntCollision]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// 当たり判定を終了するタイミング情報だった
					pMotionCollision[nCntCollision]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DAMAGE) == 0)
				{// 当たった時のダメージ量情報だった
					pMotionCollision[nCntCollision]->SetDamage(CFunctionLib::ReadInt(pStrCur, DAMAGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
				{// 当たり判定データ読み込み終了の合図だった
					nCntCollision++;
					break;  // ループ終了
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// 当たり判定情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pMotionCollision;
}

//=============================================================================
//    モーションの軌跡情報を読み取る処理
//=============================================================================
CMotionOrbit **CEnemyManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
{
	int nCntOrbit = 0;                   // 軌跡データを読み込んだ回数
	CMotionOrbit **pMotionOrbit = NULL;  // モーションの軌跡データクラスへのポインタ

	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_ORBIT) == 0)
		{// 軌跡データの数がある
			*pNumOrbitData = CFunctionLib::ReadInt(pStrCur, NUM_ORBIT);
			if (*pNumOrbitData >= 1)
			{// 当たり判定データが１個以上ある
				pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
				if (pMotionOrbit != NULL)
				{// メモリが確保できた
					for (int nCntOr = 0; nCntOr < *pNumOrbitData; nCntOr++)
					{// 当たり判定データの数だけ繰り返し
						pMotionOrbit[nCntOr] = new CMotionOrbit;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBIT) == 0)
		{// 軌跡データ読み込み開始の合図だった
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// 軌跡を出すモデルの番号情報だった
					pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TEX_INDEX) == 0)
				{// 軌跡に張り付けるテクスチャの番号情報だった
					pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET1) == 0)
				{// 軌跡を出すモデルからのオフセット距離情報(１個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStrCur, OFFSET1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET2) == 0)
				{// 軌跡を出すモデルからのオフセット距離情報(２個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStrCur, OFFSET2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP1) == 0)
				{// 軌跡のオフセット距離増幅値(1個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP2) == 0)
				{// 軌跡のオフセット距離増幅値(2個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLUP) == 0)
				{// 軌跡の頂点上側の色だった
					pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStrCur, COLUP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLDOWN) == 0)
				{// 軌跡の頂点下側の色だった
					pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStrCur, COLDOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_UP) == 0)
				{// 軌跡の頂点上側の透明度減衰値だった
					pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_UP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_DOWN) == 0)
				{// 軌跡の頂点下側の透明度減衰値だった
					pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_DOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
				{// 軌跡の横の分割数情報だった
					pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStrCur, XBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
				{// 軌跡の縦の分割数情報だった
					pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStrCur, ZBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// 軌跡を出し始めるタイミング情報だった
					pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// 軌跡をしまうタイミング情報だった
					pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_ORBIT) == 0)
				{// 軌跡データ読み込み終了の合図だった
					nCntOrbit++;
					break;  // ループ終了
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ORBITSET) == 0)
		{// 軌跡情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pMotionOrbit;
}

//=============================================================================
//    キーフレーム情報を読み取る処理
//=============================================================================
void CEnemyManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, FRAME) == 0)
		{// 再生フレーム数がある
			pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStrCur, FRAME);
		}
		if (CFunctionLib::Memcmp(pStrCur, KEY) == 0)
		{// キー情報読み込み開始の合図だった
			ReadKey(pLine, pStrCur, pFile, nCntKey, AddPos, DestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEYSET) == 0)
		{// キーフレーム情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    キー情報を読み取る処理
//=============================================================================
void CEnemyManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// 座標がある
			AddPos[nCntKey] = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// 向きがある
			DestRot[nCntKey] = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEY) == 0)
		{// キー情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    スクリプトファイル名を設定する処理
//=============================================================================
void CEnemyManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    敵の種類を設定する処理
//=============================================================================
void CEnemyManager::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する処理
//=============================================================================
void CEnemyManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    スクリプトファイル名を取得する処理
//=============================================================================
char *CEnemyManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    敵の種類を取得する処理
//=============================================================================
int CEnemyManager::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelManager *CEnemyManager::GetModelManager(void)
{
	return m_pModelManager;
}


//*****************************************************************************
//     CEnemyの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// 各種値の設定
	m_Type = TYPE_FIRE;                           // 種類
	m_AIType = AI_TYPE_CHEAK;                     // AIの状態
	m_State = STATE_NORMAL;                       // 状態
	m_nLife = 0;                                  // 体力
	m_nAddScore = 0;                              // 倒した時に増えるスコア量
	m_fAI_CheackDistance = 0.0f;                  // 警戒できる範囲
	m_fAI_AttackDistance = 0.0f;                  // 攻撃を仕掛ける範囲
	m_nAI_AttackTiming = 1;                       // 攻撃を仕掛けるタイミング
	m_AI_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 目的の位置
	m_nAI_Counter = 0;                            // AIを管理するカウンター
	m_bAI_Patrol = false;                         // パトロール行動をしているかどうか
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量
	m_pGaugeLife = NULL;                          // 体力ゲージ
	m_PosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // デフォルトの位置
	m_RotStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // デフォルトの向き
	m_bDamage = false;                            // ダメージをくらっているかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore, int nPriority)
{
	CEnemy *pEnemy = NULL;                 // エネミークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pEnemy == NULL)
		{// メモリが空になっている
			pEnemy = new CEnemy(nPriority);
			if (pEnemy != NULL)
			{// インスタンスを生成できた
				if (FAILED(pEnemy->Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore)))
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

	return pEnemy;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// 各種値の設定
	SetPos(pos);                               // 現在の座標
	SetRot(rot);                               // 現在の向き
	SetType(type);                             // 敵の種類
	SetNumParts(nNumParts);                    // パーツ数
	SetModel(apModel);                         // モデルクラスへのポインタ
	SetMotionManager(pMotionManager);          // モーション管轄クラスへのポインタ
	SetGravity(fGravity);                      // 重力
	SetColHeight(fColHeight);                  // 当たり判定を取る高さ
	SetColRange(fColRange);                    // 当たり判定を取る範囲
	SetAI_CheackDistance(fAI_CheackDistance);  // 警戒できる範囲
	SetAI_AttackDistance(fAI_AttackDistance);  // 攻撃できる範囲
	SetAI_AttackTiming(nAI_AttackTiming);      // 攻撃を仕掛けるタイミング
	SetLife(nLife);                            // 体力
	SetAddScore(nAddScore);                    // 倒した時に増えるスコア量
	m_PosStd = pos;                            // 生成時の座標
	m_RotStd = rot;                            // 生成時の向き

	// 体力ゲージを作る
	if (m_pGaugeLife == NULL)
	{
		m_pGaugeLife = CGaugeLife::Create(ENEMY_GAUGE_POS + pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), ENEMY_GAUGE_WIDTH, ENEMY_GAUGE_HEIGHT, 0.0f, m_nLife);
	}

	// モデルの位置をニュートラルの位置にする
	SetDefaultPos();

	// モデルの向きをニュートラルの向きにする
	SetDefaultRot();

	// 影を作成する
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// 体力ゲージの開放
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}

	// 共通の終了処理
	CCharacter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// 前回の座標を保存
	SetPosOld(GetPos());

	// 前回乗っていたオブジェクトへのポインタを保存
	SetLandSceneOld(GetLandScene());

	// 乗っているオブジェクトへのポインタを空にする
	SetLandScene(NULL);


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
	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
		// 必要な変数を宣言
		float fDistance = 0.0f;                     // 対象のオブジェクトとの距離
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // 対象のオブジェクトの座標
		D3DXVECTOR3 EnemyPos = GetPos();

		// 距離を計算
		fDistance = sqrtf((ObjectPos.x - EnemyPos.x) * (ObjectPos.x - EnemyPos.x) + (ObjectPos.y - EnemyPos.y) * (ObjectPos.y - EnemyPos.y) + (ObjectPos.z - EnemyPos.z) * (ObjectPos.z - EnemyPos.z));

		if (fDistance <= 2000.0f && m_State != STATE_DEATH && m_State != STATE_DAMAGE)
		{// 死亡状態でないかつダメージ状態でない
			// AIの処理
			AI();

			// 移動計算処理
			Movement();

			// 当たり判定処理
			Collision();
		}
	}

	// ゲージの座標をずらしておく
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}

	if (GetMotionManager() != NULL)
	{// モーション管理クラスが生成されている
		GetMotionManager()->Update(GetModel());
	}

	// 影の座標をずらしておく
	if (GetShadow() != NULL)
	{// 影が生成されている
		GetShadow()->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, GetPos().z));
	}

	// 状態遷移処理
	Statement();
}

//=============================================================================
//    描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// 共通の描画処理
	if (m_bDamage == true && m_nDispCounter % 6 == 0 || m_bDamage != true)
	{// ダメージ状態でない または ダメージ状態の時に描画するタイミングである
		CCharacter::Draw();
	}
}

//=============================================================================
//    移動計算処理
//=============================================================================
void CEnemy::Movement(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();           // 現在の座標
	D3DXVECTOR3 rot = GetRot();           // 現在の向き
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き
	D3DXVECTOR3 DiffRot = GetDiffRot();   // 目的の向きと現在の向きの差分
	float fGravity = GetGravity();        // 重力

	// 重力を加える
	pos.y += fGravity;

	// 各種値の設定
	SetPos(pos);
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CEnemy::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRange)
{
	bool bHit = false;

	// 必要な値を取得
	D3DXVECTOR3 pos = GetPos();
	float fRange = GetColRange();
	float fDistance = 0.0f;
	float fHitRange = sqrtf((fColRange + fRange) * (fColRange + fRange));

	// 距離を計算
	fDistance = sqrtf((pPos->x - pos.x) * (pPos->x - pos.x) + (pPos->z - pos.z) * (pPos->z - pos.z));

	if (fDistance < fHitRange)
	{// 半径内に入っている
		// ベクトルで押し返す
		D3DXVECTOR3 VecA = pos - *pPos;
		D3DXVec3Normalize(&VecA, &VecA);
		pPos->x -= VecA.x * (fHitRange - fDistance);
		pPos->z -= VecA.z * (fHitRange - fDistance);

		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    敵のAI処理
//=============================================================================
void CEnemy::AI(void)
{
	switch (m_AIType)
	{// 現在のAIの状態によって処理わけ
	case AI_TYPE_CHEAK:        // 対象のオブジェクトとの距離を測る状態
		AI_CheakDistance();
		break;
	case AI_TYPE_ACCESS:       // 対象のオブジェクトに近づく状態
		AI_ApproachObject();
		break;
	case AI_TYPE_ATTACK:       // 対象のオブジェクトに攻撃を仕掛ける状態
		AI_AttackObject();
		break;
	case AI_TYPE_RETURN_CHEAK: // 敵のAI巡回状態に戻る処理
		AI_ReturnPatrol();
		break;
	}
}

//=============================================================================
//    敵のAI対象のオブジェクトとの距離を測る処理
//=============================================================================
void CEnemy::AI_CheakDistance(void)
{
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

	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
		// 必要な変数を宣言
		float fDistance = 0.0f;                     // 対象のオブジェクトとの距離
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // 対象のオブジェクトの座標

		// 距離を計算
		fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

		if (fDistance <= m_fAI_CheackDistance)
		{// 距離が一定の値になった
			// 近づいていく状態に
			m_AIType = AI_TYPE_ACCESS;

			// 目的の位置を算出
			float fAngle = ((rand() % 314) * 0.01f) - D3DX_PI;
			m_AI_DestPos = D3DXVECTOR3(ObjectPos.x + sinf(fAngle) * 60.0f, ObjectPos.y, ObjectPos.z + cosf(fAngle) * 60.0f) - GetPos();

			// 移動状態にする
			m_State = STATE_MOVE;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
		else
		{// まだプレイヤーが近くにいない
			if (m_bAI_Patrol == true)
			{// パトロール行動をするなら
				if (m_nAI_Counter <= 200)
				{ // カウンターが既定の値に達していない
					m_nAI_Counter++;
					if (m_nAI_Counter > 200)
					{// カウンターが既定の値に達した
						m_nAI_Counter = 0;
						m_bAI_Patrol = false;
						// 通常の状態にする
						m_State = STATE_NORMAL;
						GetMotionManager()->SwitchMotion(GetModel(), m_State);
					}
					else
					{// カウンターが既定の値に達していない
						D3DXVECTOR3 EnemyPos = GetPos();
						D3DXVECTOR3 DestPos = m_AI_DestPos - EnemyPos;
						// 移動させる
						D3DXVec3Normalize(&DestPos, &DestPos);
						EnemyPos.x += DestPos.x * 2.0f;
						EnemyPos.z += DestPos.z * 2.0f;

						// 値の設定
						SetPos(EnemyPos);

						// 角度を計算
						float fAngle = atan2f((m_AI_DestPos.x - GetPos().x), (m_AI_DestPos.z - GetPos().z));
						fAngle += D3DX_PI;
						if (fAngle > D3DX_PI)
						{
							fAngle -= D3DX_PI * 2.0f;
						}

						// 向きを直す
						SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

						if (EnemyPos.x <= m_AI_DestPos.x + 5.0f && EnemyPos.x >= m_AI_DestPos.x - 5.0f
							&& EnemyPos.z <= m_AI_DestPos.z + 5.0f && EnemyPos.z >= m_AI_DestPos.z - 5.0f)
						{// 目的の位置に達した
							m_nAI_Counter = 0;
							m_bAI_Patrol = false;
							// 通常の状態にする
							m_State = STATE_NORMAL;
							GetMotionManager()->SwitchMotion(GetModel(), m_State);
						}
					}
				}
			}
			else
			{// パトロール行動をしていないなら
				if (rand() % 150 == 0)
				{// 向かうべき場所を計算する
					float fAngle = ((rand() % 628) * 0.01f) - D3DX_PI;
					float fMoveX = rand() % 150 + 100.0f;
					float fMoveZ = rand() % 150 + 100.0f;
					m_AI_DestPos = D3DXVECTOR3(m_PosStd.x + sinf(fAngle) * fMoveX, 0.0f, m_PosStd.z + cosf(fAngle) * fMoveZ);
					m_nAI_Counter = 0;
					m_bAI_Patrol = true;

					// 角度を計算
					fAngle = atan2f((m_AI_DestPos.x - GetPos().x), (m_AI_DestPos.z - GetPos().z));
					fAngle += D3DX_PI;
					if (fAngle > D3DX_PI)
					{
						fAngle -= D3DX_PI * 2.0f;
					}

					// 向きを直す
					SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));
					// 移動状態にする
					m_State = STATE_MOVE;
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
			}
		}
	}
}

//=============================================================================
//    敵のAI対象のオブジェクトに近づく処理
//=============================================================================
void CEnemy::AI_ApproachObject(void)
{
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

	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
	    // 必要な変数を宣言
		float fDistance = 0.0f;                       // 対象のオブジェクトとの距離
		float fAngle = 0.0f;                          // 対象のオブジェクトとの角度
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();    // 対象のオブジェクトの座標
		D3DXVECTOR3 EnemyPos = GetPos();              // 自身の座標
		D3DXVECTOR3 DestPos = ObjectPos;              // 敵が向かうべき目的の位置

		// 向かうべき場所へのベクトルを算出
		if (rand() % 10 == 0)
		{// 向かうべき場所を計算する
			fAngle = ((rand() % 314) * 0.01f) - D3DX_PI;
			m_AI_DestPos = D3DXVECTOR3(ObjectPos.x + sinf(fAngle) * 20.0f, ObjectPos.y, ObjectPos.z + cosf(fAngle) * 20.0f) - GetPos();
		}

		// 距離を計算
		fDistance = sqrtf((ObjectPos.x - EnemyPos.x) * (ObjectPos.x - EnemyPos.x) + (ObjectPos.y - EnemyPos.y) * (ObjectPos.y - EnemyPos.y) + (ObjectPos.z - EnemyPos.z) * (ObjectPos.z - EnemyPos.z));

		// 角度を計算
		fAngle = atan2f((EnemyPos.x - ObjectPos.x), (EnemyPos.z - ObjectPos.z));

		// 向きを直す
		SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

		if (fDistance <= m_fAI_AttackDistance)
		{// 距離が一定の値になった
			if (m_State != STATE_NORMAL)
			{// 通常状態でない
				// 通常状態にする
				m_State = STATE_NORMAL;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}

			if (rand() % m_nAI_AttackTiming == 0)
			{// 攻撃を仕掛けるタイミングが算出された
				// 攻撃を仕掛ける状態に
				m_AIType = AI_TYPE_ATTACK;

				// 攻撃状態にする
				m_State = STATE_ACTION;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
		else
		{// 攻撃を仕掛けるタイミングではない
			// 移動させる
			D3DXVec3Normalize(&m_AI_DestPos, &m_AI_DestPos);
			EnemyPos.x += m_AI_DestPos.x * 3.0f;
			EnemyPos.z += m_AI_DestPos.z * 3.0f;

			// 値の設定
			SetPos(EnemyPos);

			if (fDistance >= m_fAI_AttackDistance && fDistance <= m_fAI_CheackDistance)
			{// 攻撃範囲から逃げられた
				m_AIType = AI_TYPE_ACCESS;	// 近づいていく状態に
				if (m_State != STATE_MOVE)
				{// 移動状態でない
					m_State = STATE_MOVE;   // 移動状態にする
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
			}
			else if (fDistance >= m_fAI_CheackDistance)
			{// 距離が一定の値を超えた
				// パトロール行動に戻る状態に
				m_AIType = AI_TYPE_RETURN_CHEAK;

				// 移動状態にする
				m_State = STATE_MOVE;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
}

//=============================================================================
//    敵のAI対象のオブジェクトに攻撃を仕掛ける処理
//=============================================================================
void CEnemy::AI_AttackObject(void)
{
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

	if (pPlayer != NULL)
	{// プレイヤークラスが取得できた
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// モーション再生が終了した
			// 必要な変数を宣言
			float fDistance = 0.0f;                       // 対象のオブジェクトとの距離
			D3DXVECTOR3 ObjectPos = pPlayer->GetPos();    // 対象のオブジェクトの座標

			// 距離を計算
			fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));
			if (fDistance <= m_fAI_AttackDistance)
			{// 距離が攻撃を仕掛ける距離だった
				if (rand() % m_nAI_AttackTiming == 0)
				{// 攻撃を仕掛けるタイミングが算出された
					m_AIType = AI_TYPE_ATTACK;  // 攻撃を仕掛ける状態に
					m_State = STATE_ACTION;	    // 攻撃状態にする
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
				else
				{// 攻撃を仕掛けるタイミングが算出されなかった
					m_AIType = AI_TYPE_CHEAK;	// 巡回状態に
					m_State = STATE_NORMAL;     // 通常状態にする
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
			}
			else if (fDistance <= m_fAI_CheackDistance)
			{// 距離が一定の値になった
				m_AIType = AI_TYPE_ACCESS;	// 近づいていく状態に
				m_State = STATE_MOVE;       // 移動状態にする
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
			else
			{// 反応する距離から離れている
			    // パトロール行動に戻る状態に
				m_AIType = AI_TYPE_RETURN_CHEAK;
				m_State = STATE_MOVE;     // 通常状態にする
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}

			// モーションを切り替える
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}
}

//=============================================================================
//    敵のAI巡回状態に戻る処理
//=============================================================================
void CEnemy::AI_ReturnPatrol(void)
{
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

	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
	    // 必要な変数を宣言
		float fDistance = 0.0f;                     // 対象のオブジェクトとの距離
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // 対象のオブジェクトの座標

		// 距離を計算
		fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

		if (fDistance <= m_fAI_CheackDistance)
		{// 距離が一定の値になった
		    // 近づいていく状態に
			m_AIType = AI_TYPE_ACCESS;

			// 目的の位置を算出
			float fAngle = ((rand() % 314) * 0.01f) - D3DX_PI;
			m_AI_DestPos = D3DXVECTOR3(ObjectPos.x + sinf(fAngle) * 60.0f, ObjectPos.y, ObjectPos.z + cosf(fAngle) * 60.0f) - GetPos();

			// 移動状態にする
			m_State = STATE_MOVE;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
		else
		{// プレイヤーが近くにいない
			D3DXVECTOR3 EnemyPos = GetPos();

			// 目標位置を計算
			m_AI_DestPos = m_PosStd - EnemyPos;

			// 角度を計算
			float fAngle = atan2f((EnemyPos.x - m_PosStd.x), (EnemyPos.z - m_PosStd.z));

			// 向きを直す
			SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

			// 移動させる
			D3DXVec3Normalize(&m_AI_DestPos, &m_AI_DestPos);
			EnemyPos.x += m_AI_DestPos.x * 2.0f;
			EnemyPos.z += m_AI_DestPos.z * 2.0f;

			// 値の設定
			SetPos(EnemyPos);

			if (EnemyPos.x <= m_PosStd.x + 5.0f && EnemyPos.x >= m_PosStd.x - 5.0f
				&& EnemyPos.z <= m_PosStd.z + 5.0f && EnemyPos.z >= m_PosStd.z - 5.0f)
			{
				m_AIType = AI_TYPE_CHEAK;	// 巡回状態に
				m_State = STATE_NORMAL;     // 通常状態にする
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
}

//=============================================================================
//    敵のダメージ処理
//=============================================================================
bool CEnemy::Hit(int nDamage)
{
	bool bDeath = false;  // 死んだかどうか

	// 体力を減らす
	m_nLife -= nDamage;

	// 攻撃が当たった判定にする
	m_bDamage = true;

	// 体力ゲージを減らす
	if (m_pGaugeLife != NULL)
	{// メモリが確保されている
		m_pGaugeLife->CutGauge(nDamage);
	}

	if (m_nLife <= 0)
	{// 体力が0以下になった
		Death();

		// スコアクラスを取得し、スコアを加算する
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CScore *pScore = NULL;
		pScene = GetTop(UI_SCORE_PRIORITY);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_SCORE)
			{// エフェクト管轄クラスへのポインタだった
				pScore = (CScore*)pScene;
				if (pScore != NULL)
				{// スコアクラスが取得できた
					pScore->AddScore(m_nAddScore);
				}
			}
			pScene = pSceneNext;
		}

		// 死んだ判定に
		bDeath = true;
	}
	else
	{// まだ体力がある
		m_State = STATE_DAMAGE;
		GetMotionManager()->SwitchMotion(GetModel(), m_State);
	}

	return bDeath;
}

//=============================================================================
//    敵が死んだときの処理
//=============================================================================
void CEnemy::Death(void)
{
	// 状態とモーションを変える
	m_State = STATE_DEATH;
	GetMotionManager()->SwitchMotion(GetModel(), m_State);

	// プレイヤーのロックオン対象になっていないかチェック
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面だったら
		CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
		if (pPlayer != NULL)
		{// プレイヤークラスが取得できた
			if (pPlayer->GetLockOnEnemy() == this)
			{// ロックオン対象が自分だった
			    // ロックオンを解除する
				pPlayer->LockLost();
			}
		}
	}
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CEnemy::Collision(void)
{
	bool bLand = false;   // 乗っているかどうか

	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標

	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CMeshField *pField = NULL;           // 地面クラスへのポインタ
	CMeshWall *pWall = NULL;             // 壁クラスへのポインタ
	CEnemy *pEnemy = NULL;               // 敵クラスへのポインタ
	bool bFieldLand = false;              // 地面の中にいるかどうか
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (bLand != true)
			{// まだオブジェクトに乗っていない
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// 地面クラスへのポインタだったら
					if (bFieldLand == false)
					{// まだ地面に乗っていない
						pField = (CMeshField*)pScene;
						if (pField != NULL)
						{// 地面が取得できた
							D3DXVECTOR3 FieldNormal;
							float fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, &FieldNormal);
							if (bFieldLand == true)
							{// 地面に乗っている
								if (pos.y < fFieldHeight)
								{// 座標が地面より下になった
									m_Move.y = 0.0f;
									pos.y = fFieldHeight;
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// 配置物だったら
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// 配置物が取得できた
						if (pObject->Collision(&pos, &GetPosOld(), &m_Move, GetColHeight(), GetColRange(), this) == true)
						{// 配置物の上に乗っている
							if (GetLand() == false)
							{// 着地していなかったら
								SetLand(true);
							}

							if (GetLandScene() == NULL && GetLand() == true)
							{// 何かに乗っていた
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_MESHWALL)
				{// 壁クラスへのポインタだったら
					pWall = (CMeshWall*)pScene;
					if (pWall != NULL)
					{// 壁が取得できた
						if (pWall->Collision(&pos, &GetPosOld(), &D3DXVECTOR3(), D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange())))
						{// 壁に当たっている

						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// 敵だったら
					pEnemy = (CEnemy*)pScene;
					if (pEnemy != NULL)
					{// 敵クラスへのポインタが取得できた
						if (pEnemy->Collision(&pos, &GetPosOld(), &m_Move, GetColRange()))
						{// 敵に当たっている
						}
					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	// 各種値の設定
	SetPos(pos);         // 現在の座標
	SetLand(bLand);      // オブジェクトに乗っているかどうか
}

//=============================================================================
//    攻撃の当たり判定処理
//=============================================================================
bool CEnemy::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage)
{
	bool bHit = false;                 // 攻撃がヒットしたかどうか
	float fPlayerRange = 0.0f;         // プレイヤーの当たり判定を取る範囲
	D3DXVECTOR3 OffSet = Offset;       // 当たり判定をとる場所(親モデルからのオフセット)
	float fHitRange = 0.0f;            // 当たり判定を取る長さ
	float fLength = 0.0f;              // 当たり判定を取る場所と敵との距離
	D3DXMATRIX mtxWorld;

	// 当たり判定を取る座標を確定させる
	if (nModelIdx != -1)
	{// モデルから当たり判定を発生させる
		CModel *pModel = GetModel(nModelIdx);
		if (pModel != NULL)
		{
			mtxWorld = pModel->GetMtxWorld();
		}
	}
	else
	{// モデルから当たり判定を発生させない
		mtxWorld = GetMtxWorld();
	}
	D3DXVec3TransformCoord(&OffSet, &OffSet, &mtxWorld);

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

	// 判定開始
	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
	    // プレイヤーの当たり判定を取る範囲を取得
		fPlayerRange = pPlayer->GetColRange();

		// 当たり判定を取る長さを計算
		fHitRange = sqrtf((fPlayerRange + fRange) * (fPlayerRange + fRange));

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
				pPlayer->Damage(nDamage);
				bHit = true;
			}
			CDebugProc::Print("攻撃がヒットしました\n");
		}
	}

	return bHit;  // ヒットしたかどうかを返す
}

//=============================================================================
//    状態遷移処理
//=============================================================================
void CEnemy::Statement(void)
{
	if (m_State == STATE_DAMAGE)
	{// ダメージ状態だったら
		m_nDispCounter++;
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// モーション再生が終了した
			m_State = STATE_NORMAL;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);

			m_AIType = AI_TYPE_CHEAK;
			m_nDispCounter = 0;

			// 攻撃が当たった判定を戻す
			m_bDamage = false;
		}
	}
	else if (m_State == STATE_DEATH)
	{// 死亡状態だったら
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// モーション再生が終了した
			// エフェクトを出す
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// ゲーム中だったら
				CEffectManager *pEffectManager = CManager::GetGame()->GetEffectManager();
				if (pEffectManager != NULL)
				{// ポインタが取得できた
					pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17);
					pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 18);
					pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 19);
				}

				// スコアパーティクルを出す
				CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
				if (pPlayer != NULL)
				{// プレイヤーが取得できた
					float fAngle = 0.0f;
					D3DXVECTOR3 ParPos;
					D3DXVECTOR3 Move;
					CScorePar *pScorePar = NULL;
					ParPos = D3DXVECTOR3(GetPos().x, GetPos().y + 40.0f, GetPos().z);
					for (int nCntPar = 0; nCntPar < 10; nCntPar++)
					{// スコアパーティクルを出す数分繰り返し
						fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
						Move.x = sinf(fAngle) * ((rand() % 5) + 3.0f);
						Move.y = ((rand() % 10) + 10.0f);
						Move.z = cosf(fAngle) * ((rand() % 5) + 3.0f);
						pScorePar = CScorePar::Create(GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 8.0f, 8.0f, Move, pPlayer, 100, EFFECT_PRIORITY);
						if (pScorePar != NULL)
						{
							pScorePar->BindTexture(pEffectManager->GetTexManager()->GetTexture(5));
						}
					}
				}
			}
			// SEを再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DEATH);

			// 敵を消す
			Uninit();
		}
	}
	else if (m_State == STATE_ACTION)
	{// 攻撃状態だったら
		if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
		{// 当たり判定データクラスが生成されている
			for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
			{// 当たり判定データの数だけ繰り返し
				// 当たり判定に必要なデータを宣言
				int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();
				float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();
				D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();
				int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();
				int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();
				int nDamage = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetDamage();

				// 判定開始
				if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
				{// 当たり判定を発生させるタイミングならば
					// 攻撃時の当たり判定を発生させる
					AttackCollision(nModelIdx, fRange, Offset, nDamage);
				}

				// SEを再生
				if (GetMotionManager()->GetAllCounter() == nStartTiming)
				{// 再生するタイミングになった
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_SWING);
				}
			}
		}
	}
}

//=============================================================================
//    種類を設定する処理
//=============================================================================
void CEnemy::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CEnemy::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    移動量を設定する処理
//=============================================================================
void CEnemy::SetMove(const D3DXVECTOR3 Move)
{
	m_Move = Move;
}

//=============================================================================
//    体力を設定する処理
//=============================================================================
void CEnemy::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    倒した時に増えるスコア量を設定する処理
//=============================================================================
void CEnemy::SetAddScore(const int nAddScore)
{
	m_nAddScore = nAddScore;
}

//=============================================================================
//    AIの種類を設定する処理
//=============================================================================
void CEnemy::SetAI_Type(const AI_TYPE AI_Type)
{
	m_AIType = AI_Type;
}

//=============================================================================
//    警戒できる範囲を設定する処理
//=============================================================================
void CEnemy::SetAI_CheackDistance(const float fDistance)
{
	m_fAI_CheackDistance = fDistance;
}

//=============================================================================
//    攻撃を仕掛ける範囲を設定する処理
//=============================================================================
void CEnemy::SetAI_AttackDistance(const float fDistance)
{
	m_fAI_AttackDistance = fDistance;
}

//=============================================================================
//    攻撃を仕掛けるタイミングを設定する処理
//=============================================================================
void CEnemy::SetAI_AttackTiming(const int nTiming)
{
	m_nAI_AttackTiming = nTiming;
}

//=============================================================================
//    目的の位置を設定する処理
//=============================================================================
void CEnemy::SetAI_DestPos(const D3DXVECTOR3 AI_DestPos)
{
	m_AI_DestPos = AI_DestPos;
}

//=============================================================================
//    描画を管理するカウンターを設定する処理
//=============================================================================
void CEnemy::SetDispCounter(const int nDispCounter)
{
	m_nDispCounter = nDispCounter;
}

//=============================================================================
//    体力ゲージクラスへのポインタを設定する処理
//=============================================================================
void CEnemy::SetGaugeLife(CGaugeLife *pGaugeLife)
{
	m_pGaugeLife = pGaugeLife;
}

//=============================================================================
//    生成時の座標を設定する処理
//=============================================================================
void CEnemy::SetPosStd(const D3DXVECTOR3 posStd)
{
	m_PosStd = posStd;
}

//=============================================================================
//    生成時の向きを設定する処理
//=============================================================================
void CEnemy::SetRotStd(const D3DXVECTOR3 rotStd)
{
	m_RotStd = rotStd;
}

//=============================================================================
//    ダメージをくらっているかどうかを設定する処理
//=============================================================================
void CEnemy::SetDamage(const bool bDamage)
{
	m_bDamage = bDamage;
}

//=============================================================================
//    種類を取得する処理
//=============================================================================
CEnemy::TYPE CEnemy::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CEnemy::STATE CEnemy::GetState(void)
{
	return m_State;
}

//=============================================================================
//    移動量を取得する処理
//=============================================================================
D3DXVECTOR3 CEnemy::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    体力を取得する処理
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    倒した時に増えるスコア量を取得する処理
//=============================================================================
int CEnemy::GetAddScore(void)
{
	return m_nAddScore;
}

//=============================================================================
//    AIの種類を取得する処理
//=============================================================================
CEnemy::AI_TYPE CEnemy::GetAI_Type(void)
{
	return m_AIType;
}

//=============================================================================
//    警戒する範囲を取得する処理
//=============================================================================
float CEnemy::GetAI_CheackDistance(void)
{
	return m_fAI_CheackDistance;
}

//=============================================================================
//    攻撃を仕掛ける範囲を取得する処理
//=============================================================================
float CEnemy::GetAI_AttackDistance(void)
{
	return m_fAI_AttackDistance;
}

//=============================================================================
//    攻撃を仕掛けるタイミングを取得する処理
//=============================================================================
int CEnemy::GetAI_AttackTiming(void)
{
	return m_nAI_AttackTiming;
}

//=============================================================================
//    目的の位置を取得する処理
//=============================================================================
D3DXVECTOR3 CEnemy::GetAI_DestPos(void)
{
	return m_AI_DestPos;
}

//=============================================================================
//    描画を管理するカウンターを取得する処理
//=============================================================================
int CEnemy::GetDispCounter(void)
{
	return m_nDispCounter;
}

//=============================================================================
//    体力ゲージクラスへのポインタを取得する処理
//=============================================================================
CGaugeLife *CEnemy::GetGaugeLife(void)
{
	return m_pGaugeLife;
}

//=============================================================================
//    生成時の座標を取得する処理
//=============================================================================
D3DXVECTOR3 CEnemy::GetPosStd(void)
{
	return m_PosStd;
}

//=============================================================================
//    生成時の向きを取得する処理
//=============================================================================
D3DXVECTOR3 CEnemy::GetRotStd(void)
{
	return m_RotStd;
}

//=============================================================================
//    ダメージをくらっているかどうかを取得する処理
//=============================================================================
bool CEnemy::GetDamage(void)
{
	return m_bDamage;
}

//*****************************************************************************
//     CSetEnemyの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSetEnemy::CSetEnemy(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetEnemy::~CSetEnemy()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetEnemy *CSetEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore, int nPriority)
{
	CSetEnemy *pSetEnemy = NULL;           // 配置用エネミークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSetEnemy == NULL)
		{// メモリが空になっている
			pSetEnemy = new CSetEnemy(nPriority);
			if (pSetEnemy != NULL)
			{// インスタンスを生成できた
				if (FAILED(pSetEnemy->Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nLife, nAddScore)))
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

	return pSetEnemy;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// 共通の初期化処理
	if (FAILED(CEnemy::Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetEnemy::Uninit(void)
{
	// 共通の終了処理
	CEnemy::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetEnemy::Update(void)
{
	// ゲージの座標をずらしておく
	CGaugeLife *pGaugeLife = GetGaugeLife();
	if (pGaugeLife != NULL)
	{
		pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CSetEnemy::Draw(void)
{
	// 透明度を下げる
	SetModelAlpha(0.7f);

	// 共通の描画処理
	CEnemy::Draw();

	// 透明度を元に戻しておく
	SetModelAlpha(1.0f);
}

//*****************************************************************************
//    CEnemyAssemblyの処理
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CEnemyAssembly::CEnemyAssembly()
{
	// 各種値のクリア
	for (int nCntEnemyType = 0; nCntEnemyType < MAX_ENEMY_TYPE; nCntEnemyType++)
	{// 読み込める敵の種類の数だけ繰り返し
		m_pEnemyManager[nCntEnemyType] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEnemyAssembly::~CEnemyAssembly()
{

}


//=============================================================================
//    敵管轄クラスへのポインタを設定する処理
//=============================================================================
void CEnemyAssembly::SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx)
{
	m_pEnemyManager[nIdx] = pEnemyManager;
}

//=============================================================================
//    敵管轄クラスへのポインタを取得する処理
//=============================================================================
CEnemyManager *CEnemyAssembly::GetEnemyManager(int nIdx)
{
	return m_pEnemyManager[nIdx];
}

//*****************************************************************************
//     CBossの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBoss::CBoss(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{
	// 各種値のクリア
	m_pOrbitEffect = NULL;              // 軌跡エフェクトクラスへのポインタ
	m_pEffectManager = NULL;            // エフェクト管轄クラスへのポインタ
	m_AIType_Boss = AI_TYPE_BOSS_CHECK; // ボスのAIの状態
	m_nBackStep = 0;                    // バックステップを踏んだ回数(攻撃行動をしたら0にリセット)
	m_nAttack = 0;                      // 攻撃を仕掛けた回数(バックステップ状態に移ったらリセット)
	m_nStep = 0;                        // ステップを踏んだ回数(軌跡飛ばしをしたらリセット)
	m_nSkipOver = 0;                    // 軌跡飛ばしをした回数(ステップを踏んだらリセット)
	m_nBackStepCounter = 0;             // バックステップを踏んだ回数(バックステップは必ず2回踏ませるため)
	m_nTiredCounter = 0;                // 疲れ状態になってからの時間を数えるカウンター
	m_nSkipOverOrbitCounter = 0;        // 軌跡飛ばし状態になってからの時間を数えるカウンター
	m_nDamageCounter = 0;               // ダメージをくらってからの時間
	m_fRedAdd[0] = 0.0f;                // モデルの色を変える値
	m_fRedAdd[1] = 0.0f;                // モデルの色を変える値
	m_nRedCounter = 0;                  // モデルの色変更を管理するカウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBoss::~CBoss()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBoss *CBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore, int nPriority)
{
	CBoss *pEnemy = NULL;                 // エネミークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pEnemy == NULL)
		{// メモリが空になっている
			pEnemy = new CBoss(nPriority);
			if (pEnemy != NULL)
			{// インスタンスを生成できた
				if (FAILED(pEnemy->Init(pos, rot, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore)))
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

	return pEnemy;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBoss::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// 各種値の設定
	SetPos(pos);                               // 現在の座標
	SetRot(rot);                               // 現在の向き
	SetNumParts(nNumParts);                    // パーツ数
	SetType(TYPE_BOSS);                        // 種類
	SetModel(apModel);                         // モデルクラスへのポインタ
	SetMotionManager(pMotionManager);          // モーション管轄クラスへのポインタ
	SetGravity(fGravity);                      // 重力
	SetColHeight(fColHeight);                  // 当たり判定を取る高さ
	SetColRange(fColRange);                    // 当たり判定を取る範囲
	SetAI_CheackDistance(fAI_CheackDistance);  // 警戒できる範囲
	SetAI_AttackDistance(fAI_AttackDistance);  // 攻撃できる範囲
	SetAI_AttackTiming(nAI_AttackTiming);      // 攻撃を仕掛けるタイミング
	SetLife(nLife);                            // 体力
	SetAddScore(nAddScore);                    // 倒した時に増えるスコア量
	SetPosStd(pos);                            // 生成時の座標
	SetRotStd(rot);                            // 生成時の向き

	// 各クラスへのポインタを入手しておく
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_EFFECTMANAGER)
			{// エフェクト管轄クラスへのポインタだった
				m_pEffectManager = (CEffectManager*)pScene;
			}
			pScene = pSceneNext;
		}
	}

	// 体力ゲージを作る
	CGaugeLife *pGaugeLife = NULL;
	if (pGaugeLife == NULL)
	{
		pGaugeLife = CGaugeLife::Create(ENEMY_GAUGE_POS + pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), ENEMY_GAUGE_WIDTH, ENEMY_GAUGE_HEIGHT, 0.0f, nLife);
		SetGaugeLife(pGaugeLife);
	}

	// モデルの位置をニュートラルの位置にする
	SetDefaultPos();

	// モデルの向きをニュートラルの向きにする
	SetDefaultRot();

	// デフォルトの色を取得する
	CModel **pModel = GetModel();
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATERIAL *pMat;
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// パーツ数だけ繰り返し
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// ポインタが取得できた
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// マテリアル情報の数だけ繰り返し
				m_ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}
	}

	m_fRedAdd[0] = BOSS_RED_CHANGE_0;                // モデルの色を変える値
	m_fRedAdd[1] = BOSS_RED_CHANGE_1;                // モデルの色を変える値

	// 影を作成する
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.2f, 1.2f, 1.2f));
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	// 軌跡エフェクトクラスへのポインタを開放する
	if (m_pOrbitEffect != NULL)
	{// メモリが確保されている
		m_pOrbitEffect->Uninit();
		m_pOrbitEffect = NULL;
	}

	// 共通の終了処理
	CEnemy::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBoss::Update(void)
{
	// 前回の座標を保存
	SetPosOld(GetPos());

	// 前回乗っていたオブジェクトへのポインタを保存
	SetLandSceneOld(GetLandScene());

	// 乗っているオブジェクトへのポインタを空にする
	SetLandScene(NULL);

	if (GetState() != STATE_DEATH && GetState() != STATE_DAMAGE && CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
	{// 死亡状態でないかつダメージ状態でないかつゲームが通常状態である
	    // AIの処理
		AI();
	}

	// 移動計算処理
	Movement();

	// 当たり判定処理
	Collision();

	// 状態遷移処理
	Statement();

	if (m_AIType_Boss == AI_TYPE_BOSS_TIRED)
	{// 疲れるAIの状態だったら
		m_nTiredCounter++;
	}

	if (GetDamage() == true)
	{// ダメージをくらっていたら
		m_nDamageCounter++;
		if (m_nDamageCounter >= 80)
		{// ダメージ状態になってから一定時間がたった
			m_nDamageCounter = 0;
			SetDamage(false);
		}
	}

	// ゲージの座標をずらしておく
	CGaugeLife *pGaugeLife = GetGaugeLife();
	if (pGaugeLife != NULL)
	{
		pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}

	if (GetMotionManager() != NULL)
	{// モーション管理クラスが生成されている
		GetMotionManager()->Update(GetModel());
	}

	// 影の座標をずらしておく
	if (GetShadow() != NULL)
	{// 影が生成されている
		GetShadow()->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, GetPos().z));
	}

	if (GetState() == STATE_DEATH)
	{// 死亡状態だったら
		DeathState();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CBoss::Draw(void)
{
	// 体力が低いときは赤く点滅させる
	DeathWarning();

	// 共通の描画処理
	if (GetState() == STATE_DAMAGE && GetDispCounter() % 6 == 0 || GetState() != STATE_DAMAGE)
	{// ダメージ状態でない または ダメージ状態の時に描画するタイミングである
		CCharacter::Draw();
	}
}

//=============================================================================
//    移動計算処理
//=============================================================================
void CBoss::Movement(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();           // 現在の座標
	D3DXVECTOR3 Move = GetMove();         // 現在の移動量
	D3DXVECTOR3 rot = GetRot();           // 現在の向き
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き
	D3DXVECTOR3 DiffRot = GetDiffRot();   // 目的の向きと現在の向きの差分
	float fGravity = GetGravity();        // 重力

	// 移動量を加える
	pos += Move;

	// 重力を加える
	pos.y += fGravity;

	// 移動量に慣性を加える
	Move.x += (0.0f - Move.x) * BOSS_INERTIA;
	Move.z += (0.0f - Move.z) * BOSS_INERTIA;

	// 各種値の設定
	SetPos(pos);
	SetMove(Move);
}

//=============================================================================
//    ボスのAI処理
//=============================================================================
void CBoss::AI(void)
{
	switch (m_AIType_Boss)
	{// 現在のAIの状態によって処理わけ
	case AI_TYPE_BOSS_CHECK:              // 対象のオブジェクトとの距離を測る状態
		AI_CheakDistance();
		break;
	case AI_TYPE_BOSS_ATTACK:             // 攻撃を仕掛ける状態
		AI_Attack();
		break;
	case AI_TYPE_BOSS_SKIPOVER_ORBIT:     // 軌跡を飛ばして攻撃する状態
		AI_SkipOverOrbit();
		break;
	case AI_TYPE_BOSS_TIRED:              // 軌跡を飛ばして疲れてしまった状態
		AI_Tired();
		break;
	case AI_TYPE_BOSS_STEP:               // ステップを踏んで近づく状態
		AI_Step();
		break;
	case AI_TYPE_BOSS_BACKSTEP:           // バックステップで逃げる状態
		AI_BackStep();
		break;
	}
}

//=============================================================================
//    ボスのプレイヤーとの距離を測る処理
//=============================================================================
void CBoss::AI_CheakDistance(void)
{
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

	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
	    // 必要な変数を宣言
		float fDistance = 0.0f;                     // 対象のオブジェクトとの距離
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // 対象のオブジェクトの座標

		// 距離を計算
		fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

		// 行動のために向きを補正
		float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
		fAngle += D3DX_PI;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI *2.0f;
		}
		SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

		if (fDistance <= BOSS_AI_NEAR_DISTANCE)
		{// 距離が一定の値になった
			int nBehavior = rand() % 3 + (-m_nAttack + m_nBackStep) - 1;
			if (nBehavior >= 0)
			{// 攻撃する行動番号だった
				// 攻撃した回数を増やす
				m_nAttack++;

				// バックステップを連続で踏んだ回数をリセット
				m_nBackStep = 0;

			    // AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_ATTACK;

				// 通常状態にする
				SetState(STATE_BOSS_ATTACK_0);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (nBehavior < 0)
			{// バックステップを踏む番号だった
			    // バックステップを踏んだ回数を増やす
				m_nBackStep++;

				// 攻撃を連続で仕掛けた回数をリセット
				m_nAttack = 0;

				// AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_BACKSTEP;

				// バックステップ状態にする
				SetState(STATE_BOSS_BACKSTEP);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
			}
		}
		else
		{// まだプレイヤーが近くにいない
			int nBehavior = rand() % 2 + (-m_nSkipOver + m_nStep) - 1;
			if (nBehavior >= 0)
			{// 軌跡を飛ばす行動番号だった
			    // 軌跡を飛ばした回数を増やす
				m_nSkipOver++;

				// ステップを連続で踏んだ回数をリセット
				m_nStep = 0;

				// AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_SKIPOVER_ORBIT;

				// 軌跡を飛ばす状態にする
				SetState(STATE_BOSS_SKIPOVER);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (nBehavior < 0)
			{// ステップを踏む番号だった
			    // ステップを踏んだ回数を増やす
				m_nStep++;

				// 軌跡を連続で飛ばした回数をリセット
				m_nSkipOver = 0;

				// AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_STEP;

				// バックステップ状態にする
				SetState(STATE_BOSS_STEP);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
			}
		}
	}
}

//=============================================================================
//    ボスの攻撃を仕掛ける処理処理
//=============================================================================
void CBoss::AI_Attack(void)
{
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

	if (GetState() != STATE_BOSS_ATTACK_3)
	{// 最後の攻撃モーションじゃなかった
		if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
		{// モーション再生が終了した
			// 攻撃する方向をプレイヤーに向ける
			if (pPlayer != NULL)
			{// プレイヤークラスが取得できている
				float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
				fAngle += D3DX_PI;
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI *2.0f;
				}
				SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));
			}

			// モーションを切り替える
			if (GetState() == STATE_BOSS_ATTACK_0)
			{// 1つ目の攻撃モーションだったら
				SetState(STATE_BOSS_ATTACK_1);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (GetState() == STATE_BOSS_ATTACK_1)
			{// 2つ目の攻撃モーションだったら
				SetState(STATE_BOSS_ATTACK_2);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (GetState() == STATE_BOSS_ATTACK_2)
			{// 3つ目の攻撃モーションだったら
				SetState(STATE_BOSS_ATTACK_3);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
		}
	}
	else
	{// 最後の攻撃モーションだったら
		if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
		{// モーション再生が終了した
			int nBehavior = rand() % 2;
			if (nBehavior == 0)
			{
				// AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_TIRED;

				// 疲れた状態にする
				SetState(STATE_BOSS_TIRED);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else
			{
				// AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_CHECK;

				// 通常状態にする
				SetState(STATE_NORMAL);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
		}
	}
}

//=============================================================================
//    ボスのバックステップで逃げる処理
//=============================================================================
void CBoss::AI_BackStep(void)
{
	// 各種値の取得
	D3DXVECTOR3 Move = GetMove();

	// 計算しておいた向きとは逆向きに移動量を加える
	Move.x += sinf(GetRot().y) * BOSS_STEP_MOVE;
	Move.z += cosf(GetRot().y) * BOSS_STEP_MOVE;

	// 各種値の設定
	SetMove(Move);

	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// モーション再生が終了した
		if (m_nBackStepCounter == 0)
		{// まだバックステップを一回も踏んでいなかった
			m_nBackStepCounter++;

			// バックステップ状態にする
			SetState(STATE_BOSS_BACKSTEP);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());

			// SEを再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
		}
		else
		{// すでにバックステップを1回踏んでいた
			// カウンターを戻す
			m_nBackStepCounter = 0;

			// AIの種類を変える
			m_AIType_Boss = AI_TYPE_BOSS_SKIPOVER_ORBIT;

		    // 軌跡飛ばし状態にする
			SetState(STATE_BOSS_SKIPOVER);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}
	}
}

//=============================================================================
//    ボスのステップを踏んで近づく処理
//=============================================================================
void CBoss::AI_Step(void)
{
	// 各種値の取得
	D3DXVECTOR3 Move = GetMove();

	// 計算しておいた向きに合わせて移動量を加える
	Move.x += sinf(GetRot().y - D3DX_PI) * BOSS_STEP_MOVE;
	Move.z += cosf(GetRot().y - D3DX_PI) * BOSS_STEP_MOVE;

	// 各種値の設定
	SetMove(Move);

	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// モーション再生が終了した
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

		if (pPlayer != NULL)
		{// プレイヤーが取得できた
			float fDistance = 0.0f;                     // 対象のオブジェクトとの距離
			D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // 対象のオブジェクトの座標

			// 距離を計算
			fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

			if (fDistance <= BOSS_AI_ATTACK_DISTANCE)
			{// 攻撃を仕掛けられる距離にいる
			    // 攻撃した回数を増やす
				m_nAttack++;

				// バックステップを連続で踏んだ回数をリセット
				m_nBackStep = 0;

				// AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_ATTACK;

				// 通常状態にする
				SetState(STATE_BOSS_ATTACK_0);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else
			{// 攻撃を仕掛けられる距離にいない
			    // AIの種類を入れ替え
				m_AIType_Boss = AI_TYPE_BOSS_CHECK;

				// 通常状態にする
				SetState(STATE_NORMAL);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
		}
		else
		{// プレイヤーが死んでいる
		    // AIの種類を入れ替え
			m_AIType_Boss = AI_TYPE_BOSS_CHECK;

			// 通常状態にする
			SetState(STATE_NORMAL);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}
	}
}

//=============================================================================
//    ボスの軌跡を飛ばして攻撃する処理
//=============================================================================
void CBoss::AI_SkipOverOrbit(void)
{
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

	// 攻撃する方向をプレイヤーに向ける
	if (pPlayer != NULL)
	{// プレイヤークラスが取得できている
		float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
		fAngle += D3DX_PI;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI *2.0f;
		}
		SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));
	}

	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// モーション再生が終了した
		int nTiming = BOSS_ATTACK_ORBIT_INTERVAL_0;
		if (GetLife() <= 50)
		{// 体力がもうない
			nTiming = BOSS_ATTACK_ORBIT_INTERVAL_2;
		}
		else if (GetLife() <= 150)
		{// 体力が半分以下である
			nTiming = BOSS_ATTACK_ORBIT_INTERVAL_1;
		}

		if (nTiming != 0)
		{// 0になっていない
			if (m_nSkipOverOrbitCounter % nTiming == 0)
			{// 軌跡を飛ばすタイミングなった
				if (pPlayer != NULL)
				{// プレイヤークラスが取得できている
					// 竜巻を飛ばす
					float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
					CTornade::Create(GetPos(), m_pEffectManager, fAngle, 0);

					// SEを再生
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSS_TORNADE);
				}
			}
		}

		if (m_nSkipOverOrbitCounter >= BOSS_ATTACK_ORBIT)
		{// 軌跡を飛ばす状態になってからしばらくたった
		    // カウンターを戻す
			m_nSkipOverOrbitCounter = 0;

			// AIの種類を入れ替え
			m_AIType_Boss = AI_TYPE_BOSS_TIRED;

			// 疲れた状態にする
			SetState(STATE_BOSS_TIRED);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}

	    // カウンターを増やす
		m_nSkipOverOrbitCounter++;
	}
}

//=============================================================================
//    ボスの軌跡を飛ばして疲れてしまった処理
//=============================================================================
void CBoss::AI_Tired(void)
{
	if (m_nTiredCounter % BOSS_RESTART_TIRED == 0)
	{// 疲れる状態になってからしばらくたっている
		// カウンターを戻す
		m_nTiredCounter = 0;

	    // AIの種類を入れ替え
		m_AIType_Boss = AI_TYPE_BOSS_CHECK;

		// 通常状態にする
		SetState(STATE_NORMAL);
		GetMotionManager()->SwitchMotion(GetModel(), GetState());
	}
}

//=============================================================================
//    ボスのダメージ処理
//=============================================================================
bool CBoss::Hit(int nDamage)
{
	bool bDeath = false;    // 死んだかどうか
	int nLife = GetLife();  // 現在の体力

	// 体力を減らす
	nLife -= nDamage;

	if (GetLife() <= 50)
	{// 体力がもうない
		// デフォルトの色を設定する
		CModel **pModel = GetModel();
		LPD3DXBUFFER pBuffMat;
		DWORD nNumMat;
		D3DXMATERIAL *pMat;
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// パーツ数だけ繰り返し
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// ポインタが取得できた
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// マテリアル情報の数だけ繰り返し
					pMat[nCntMat].MatD3D.Diffuse = m_ModelColorDef[nCntMat][nCntParts];
				}
			}
		}
	}
	else if (GetLife() <= 150)
	{// 体力が半分以下である
		// デフォルトの色を設定する
		CModel **pModel = GetModel();
		LPD3DXBUFFER pBuffMat;
		DWORD nNumMat;
		D3DXMATERIAL *pMat;
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// パーツ数だけ繰り返し
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// ポインタが取得できた
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// マテリアル情報の数だけ繰り返し
					pMat[nCntMat].MatD3D.Diffuse = m_ModelColorDef[nCntMat][nCntParts];
				}
			}
		}
	}

	// 攻撃が当たった判定にする
	SetDamage(true);

	// 体力ゲージを減らす
	CGaugeLife *pGaugeLife = GetGaugeLife();
	if (pGaugeLife != NULL)
	{
		pGaugeLife->CutGauge(nDamage);
	}

	if (nLife <= 0)
	{// 体力が0以下になった
		Death();

		// 死んだ判定に
		bDeath = true;
	}
	else
	{// まだ体力がある
		if (m_AIType_Boss != AI_TYPE_BOSS_ATTACK && m_AIType_Boss != AI_TYPE_BOSS_STEP
			&& m_AIType_Boss != AI_TYPE_BOSS_BACKSTEP && m_AIType_Boss != AI_TYPE_BOSS_SKIPOVER_ORBIT)
		{
			SetState(STATE_DAMAGE);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}

		// 体力の設定
		SetLife(nLife);
	}

	return bDeath;
}

//=============================================================================
//    ボスが 死んだときの処理
//=============================================================================
void CBoss::Death(void)
{
	// 状態とモーションを変える
	SetState(STATE_DEATH);
	GetMotionManager()->SwitchMotion(GetModel(), GetState());

	// プレイヤーのロックオン対象になっていないかチェック
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面だったら
		CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
		if (pPlayer != NULL)
		{// プレイヤークラスが取得できた
			if (pPlayer->GetLockOnEnemy() == this)
			{// ロックオン対象が自分だった
			    // ロックオンを解除する
				pPlayer->LockLost();
			}
		}
	}

	// ラスボス撃破ボーナスを付与
	CResult::SetLastBossBonus(BOSS_BONUS);

	// ボスが倒れた時のイベントを発生させる
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲームモード中だったら
		CGame *pGame = CManager::GetGame();
		if (pGame->GetState() != CGame::STATE_KNOCKDOWNBOSS)
		{// ボスが倒れた状態でない
			pGame->SetState(CGame::STATE_KNOCKDOWNBOSS);
			pGame->SetBossPos(GetPos());
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM003);
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM005);
		}
	}
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CBoss::Collision(void)
{
	bool bLand = false;   // 乗っているかどうか

	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標
	D3DXVECTOR3 Move = GetMove();        // 現在の移動量

	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CMeshField *pField = NULL;           // 地面クラスへのポインタ
	CMeshWall *pWall = NULL;             // 壁クラスへのポインタ
	CEnemy *pEnemy = NULL;               // 敵クラスへのポインタ
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (bLand != true)
			{// まだオブジェクトに乗っていない
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// 地面クラスへのポインタだったら
					pField = (CMeshField*)pScene;
					if (pField != NULL)
					{// 地面が取得できた
						float fFieldHeight = pField->GetPolyHeight(pos, NULL, NULL);
						if (pos.y < fFieldHeight)
						{// 座標が地面より下になった
							Move.y = 0.0f;
							pos.y = fFieldHeight;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// 配置物だったら
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// 配置物が取得できた
						if (pObject->Collision(&pos, &GetPosOld(), &Move, GetColHeight(), GetColRange(), this) == true)
						{// 配置物の上に乗っている
							if (GetLand() == false)
							{// 着地していなかったら
								SetLand(true);
							}

							if (GetLandScene() == NULL && GetLand() == true)
							{// 何かに乗っていた
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_MESHWALL)
				{// 壁クラスへのポインタだったら
					pWall = (CMeshWall*)pScene;
					if (pWall != NULL)
					{// 壁が取得できた
						if (pWall->Collision(&pos, &GetPosOld(), &D3DXVECTOR3(), D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange())))
						{// 壁に当たっている

						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// 敵だったら
					pEnemy = (CEnemy*)pScene;
					if (pEnemy != NULL)
					{// 敵クラスへのポインタが取得できた
						if (pEnemy->Collision(&pos, &GetPosOld(), &Move, GetColRange()))
						{// 敵に当たっている
						}
					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	// 各種値の設定
	SetPos(pos);         // 現在の座標
	SetMove(Move);       // 現在の移動量
	SetLand(bLand);      // オブジェクトに乗っているかどうか
}

//=============================================================================
//    攻撃の当たり判定処理
//=============================================================================
bool CBoss::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage)
{
	bool bHit = false;                 // 攻撃がヒットしたかどうか
	bool bDeath = false;               // プレイヤーが死んだかどうか
	float fPlayerRange = 0.0f;         // プレイヤーの当たり判定を取る範囲
	D3DXVECTOR3 OffSet = Offset;       // 当たり判定をとる場所(親モデルからのオフセット)
	float fHitRange = 0.0f;            // 当たり判定を取る長さ
	float fLength = 0.0f;              // 当たり判定を取る場所とプレイヤーとの距離
	D3DXMATRIX mtxWorld;

	// 当たり判定を取る座標を確定させる
	if (nModelIdx != -1)
	{// モデルから当たり判定を発生させる
		CModel *pModel = GetModel(nModelIdx);
		if (pModel != NULL)
		{
			mtxWorld = pModel->GetMtxWorld();
		}
	}
	else
	{// モデルから当たり判定を発生させない
		mtxWorld = GetMtxWorld();
	}
	D3DXVec3TransformCoord(&OffSet, &OffSet, &mtxWorld);

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

	// 判定開始
	if (pPlayer != NULL)
	{// プレイヤークラスへのポインタが取得できている
	    // プレイヤーの当たり判定を取る範囲を取得
		fPlayerRange = pPlayer->GetColRange();

		// 当たり判定を取る長さを計算
		fHitRange = sqrtf((fPlayerRange + fRange) * (fPlayerRange + fRange));

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
				pPlayer->Damage(nDamage);
				bHit = true;
			}
			CDebugProc::Print("攻撃がヒットしました\n");
		}
	}

	return bHit;  // ヒットしたかどうかを返す
}

//=============================================================================
//    状態遷移処理
//=============================================================================
void CBoss::Statement(void)
{
	if (m_AIType_Boss == AI_TYPE_BOSS_ATTACK)
	{// 攻撃しているAIの状態だったら
		D3DXVECTOR3 Move = GetMove();

		if (GetState() == STATE_BOSS_ATTACK_0)
		{// 1つ目の攻撃モーションだった
			if (GetMotionManager()->GetAllCounter() == 22)
			{// ある程度のカウンターの値になっている

				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
			}
			if (GetMotionManager()->GetAllCounter() == 18)
			{
				Move.x -= sinf(GetRot().y) * 20.0f;
				Move.z -= cosf(GetRot().y) * 20.0f;
			}
		}
		else if (GetState() == STATE_BOSS_ATTACK_1)
		{// 2つ目の攻撃モーションだった
			if (GetMotionManager()->GetAllCounter() == 9)
			{
				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
			}
			if (GetMotionManager()->GetAllCounter() == 7)
			{
				Move.x -= sinf(GetRot().y) * 10.0f;
				Move.z -= cosf(GetRot().y) * 10.0f;
			}
		}
		else if (GetState() == STATE_BOSS_ATTACK_2)
		{// 3つ目の攻撃モーションだった
			if (GetMotionManager()->GetAllCounter() == 28)
			{
				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
			}
			if (GetMotionManager()->GetAllCounter() == 25)
			{
				Move.x -= sinf(GetRot().y) * 20.0f;
				Move.z -= cosf(GetRot().y) * 20.0f;
			}
		}
		if (GetState() == STATE_BOSS_ATTACK_3)
		{// 最後の攻撃モーションだった
			// しかるべきタイミングでエフェクトを出す
			if (GetMotionManager()->GetAllCounter() == 35)
			{
				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 45.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 75.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 90.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 105.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 120.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK);
			}
			if (GetMotionManager()->GetAllCounter() == 34)
			{
				Move.x -= sinf(GetRot().y) * 25.0f;
				Move.z -= cosf(GetRot().y) * 25.0f;
			}
		}

		if (GetMotionManager()->GetMotion()[GetState()]->GetCollision() != NULL)
		{// 当たり判定データクラスが生成されている
			for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[GetState()]->GetNumColData(); nCntCol++)
			{// 当たり判定データの数だけ繰り返し
				// 当たり判定に必要なデータを宣言
				int nModelIdx = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetModelIdx();
				float fRange = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetRange();
				D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetOffsetPos();
				int nStartTiming = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetStartTiming();
				int nFinishTiming = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetFinishTiming();
				int nDamage = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetDamage();

				if (GetState() == STATE_BOSS_ATTACK_3)
				{// 最後の攻撃モーションだった
					fRange += 12.0f * (GetMotionManager()->GetAllCounter() - nStartTiming);
				}

				// 判定開始
				if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
				{// 当たり判定を発生させるタイミングならば
				 // 攻撃時の当たり判定を発生させる
					AttackCollision(nModelIdx, fRange, Offset, nDamage);
				}
			}
		}

		if (GetMotionManager()->GetMotion()[GetState()]->GetOrbit() != NULL)
		{// 軌跡データクラスが生成されている
			if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetStartTiming())
			{// 軌跡を出すタイミングになった
				if (m_pOrbitEffect == NULL)
				{// オービットエフェクトクラスへのポインタが空になっている
					// 軌跡に必要な情報を宣言
					int nModelIdx = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetModelIdx();
					int nTexIdx = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetTexIdx();
					D3DXVECTOR3 OffSetPos1 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetPos1();
					D3DXVECTOR3 OffSetPos2 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetPos2();
					D3DXVECTOR3 OffSetAmp1 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetAmp1();
					D3DXVECTOR3 OffSetAmp2 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetAmp2();
					D3DXCOLOR ColUp = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetColUp();
					D3DXCOLOR ColDown = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetColDown();
					float fAlphaDecayUp = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetAlphaDecayUp();
					float fAlphaDecayDown = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetAlphaDecayDown();
					int nXBlock = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetXBlock();
					int nYBlock = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetYBlock();
					D3DXMATRIX *pMtxParent = GetModel(nModelIdx)->GetMtxWorldPointer();
					m_pOrbitEffect = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, pMtxParent, EFFECT_PRIORITY);
					if (m_pOrbitEffect != NULL)
					{// 軌跡を生成できた
						m_pOrbitEffect->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
					}
				}
			}
			if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetFinishTiming())
			{// 軌跡をしまうタイミングになった
				if (m_pOrbitEffect != NULL)
				{
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
			}
		}

		// 各種値の設定
		SetMove(Move);
	}

	if (GetState() == STATE_DAMAGE)
	{// ダメージ状態だったら
		int nDispCounter = GetDispCounter();
		nDispCounter++;
		if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
		{// モーション再生が終了した
			if (m_AIType_Boss == AI_TYPE_BOSS_TIRED)
			{// 疲れているモーションだったら
				SetState(STATE_BOSS_TIRED);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else
			{// 疲れていないモーションだったら
				SetState(STATE_NORMAL);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			nDispCounter = 0;
			SetDamage(false);
			m_nDamageCounter = 0;
		}
		SetDispCounter(nDispCounter);
	}
}

//=============================================================================
//    体力が低くなっているときの処理
//=============================================================================
void CBoss::DeathWarning(void)
{
	if (GetLife() <= 150)
	{// 体力が半分以下である
		int nIdx = 0;
		m_nRedCounter++;
		int nChangeTiming = 0;
		if (GetLife() <= 50)
		{// 体力がもうない
			nIdx = 1;
			nChangeTiming = BOSS_ATTACK_ORBIT_INTERVAL_1;
		}
		else if (GetLife() <= 150)
		{// 体力が半分以下である
			nChangeTiming = BOSS_ATTACK_ORBIT_INTERVAL_2;
		}
		if (nChangeTiming != 0)
		{// 変えるタイミングが設定で来た
			if (m_nRedCounter % nChangeTiming == 0)
			{// 色を変えるタイミングになった
				m_fRedAdd[nIdx] *= -1;
			}
		}

		 // モデルの色を変更する
		CModel **pModel = GetModel();
		LPD3DXBUFFER pBuffMat;
		DWORD nNumMat;
		D3DXMATERIAL *pMat;
		D3DXCOLOR ModelColor[4];
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// パーツ数だけ繰り返し
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// ポインタが取得できた
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// マテリアル情報の数だけ繰り返し
					ModelColor[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
					ModelColor[nCntMat].r += m_fRedAdd[nIdx];
					if (ModelColor[nCntMat].r >= 1.0f)
					{
						ModelColor[nCntMat].r = 1.0f;
					}
					ModelColor[nCntMat].g -= m_fRedAdd[nIdx];
					if (ModelColor[nCntMat].g >= 0.0f)
					{
						ModelColor[nCntMat].g = 0.0f;
					}
					ModelColor[nCntMat].b -= m_fRedAdd[nIdx];
					if (ModelColor[nCntMat].b <= 0.0f)
					{
						ModelColor[nCntMat].b = 0.0f;
					}
					pMat[nCntMat].MatD3D.Diffuse = ModelColor[nCntMat];
				}
			}
		}
	}
}

//=============================================================================
//    ボスが死んだときの処理
//=============================================================================
void CBoss::DeathState(void)
{
	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// モーション再生が終了した
	    // エフェクトを出す
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲーム中だったら
			CEffectManager *pEffectManager = CManager::GetGame()->GetEffectManager();
			if (pEffectManager != NULL)
			{// ポインタが取得できた
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 21);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 22);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 23);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 24);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 25);
			}
		}

		// SEを再生
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DEATH);

		// ボスを消す
		Uninit();
	}
}