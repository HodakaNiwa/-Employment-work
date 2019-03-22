//*****************************************************************************
//
//     プレイヤーの処理[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "camera.h"
#include "input.h"
#include "effectManager.h"
#include "object.h"
#include "enemy.h"
#include "orbitEffect.h"
#include "meshField.h"
#include "river.h"
#include "meshWall.h"
#include "item.h"
#include "gauge.h"
#include "ui.h"
#include "map.h"
#include "combo.h"
#include "sceneBillboard.h"
#include "textureManager.h"
#include "shadow.h"
#include "functionlib.h"
#include "grass.h"
#include "boxCollider.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// データ入力用マクロ
#define PLAYER_AVATEROFFSET_1        (D3DXVECTOR3(80.0f,0.0f,0.0f))     // １体目の分身のオフセット
#define PLAYER_AVATEROFFSET_2        (D3DXVECTOR3(-80.0f,0.0f,0.0f))    // ２体目の分身のオフセット
#define PLAYER_AVATEROFFSET_3        (D3DXVECTOR3(0.0f,0.0f,80.0f))     // ３体目の分身のオフセット
#define PLAYER_AVATEROFFSET_4        (D3DXVECTOR3(0.0f,0.0f,-80.0f))    // ４体目の分身のオフセット
#define PLAYER_AVATER_CUTTIMING      (1200)                             // 分身を出してから減らすまでの時間
#define PLAYER_MAX_MODELPARTS        (20)                               // プレイヤーのパーツ数
#define PLAYER_SPECIALGAUGE_ORBITADD (1.0f)                             // 必殺ゲージをどれくらい増やすか(軌跡斬をしないとき)
#define PLAYER_SPECIALGAUGE_ADD      (5.0f)                             // 必殺ゲージをどれくらい増やすか
#define PLAYER_AVATER_LIFECUT        (1)                                // 分身を作るときに消費する体力
#define PLAYER_SHADOW_PRIORITY       (9)                                // プレイヤーの影の処理優先順位
#define PLAYER_SHADOW_WIDTH          (18.0f)                            // プレイヤーの影の幅
#define PLAYER_SHADOW_HEIGHT         (18.0f)                            // プレイヤーの影の高さ
#define PLAYER_SHADOW_COLOR          (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // プレイヤーの影の色
#define PLAYER_DAMAGE_COUNTER_MAX    (120)                              // プレイヤーのダメージ管理カウンターの最大値

// ロックオン用マクロ
#define PLAYER_LOCKON_POLY_PRIORITY (10)                                // ロックオンポリゴンの処理優先順位
#define PLAYER_LOCKON_DISTANCE      (500.0f)                            // ロックオンを開始できる距離
#define PLAYER_LOCKONPOLY_WIDTH     (30.0f)                             // ロックオンポリゴンの幅
#define PLAYER_LOCKONPOLY_HEIGHT    (30.0f)                             // ロックオンポリゴンの高さ

// 残像記憶用マクロ
#define PLAYER_AFTERIMAGE_COLOR     (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))    // 残像の色
#define PLAYER_AFTERIMAGE_SCALE     (D3DXVECTOR3(0.95f,0.95f,0.95f))    // 残像の大きさ
#define PLAYER_AFTERIMAGE_SCALECUT  (0.05f)                             // 大きさを削る量
#define PLAYER_AFTERIMAGE_ALPHACUT  (0.01f)                             // 透明度を削る量
#define PLAYER_AFTERIMAGE_REDUP     (0.1f)                              // 赤色を増やす量
#define PLAYER_AFTERIMAGE_GREENUP   (0.1f)                              // 緑を増やす量
#define PLAYER_ORBIT_UPTIMING       (240)                               // 軌跡ゲージが回復するまでの時間
#define PLAYER_ORBIT_UPVALUE        (0.7f)                              // １回の更新で回復する軌跡ゲージ量
#define PLAYER_ORBIT_CUTVALUE       (0.2f)                              // １回の更新で減少する軌跡ゲージ量

// 値読み込みをする際の目印となる文字列
// 共通
#define POS               "POS = "                // 座標を読み取る合図
#define ROT               "ROT = "                // 向きを読み取る合図
#define MOVE              "MOVE = "               // 移動量を読み取る合図

// その他
#define NUM_TEXTURE       "NUM_TEXTURE = "        // 読み込むテクスチャの数
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "   // 読み込むテクスチャのファイル名
#define NUM_MODEL         "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME    "MODEL_FILENAME = "     // 読み込むモデルのファイル名

// キャラクター情報
#define CHARACTERSET      "CHARACTERSET"          // キャラクター情報読み込み開始の合図
#define END_CHARACTERSET  "END_CHARACTERSET"      // キャラクター情報読み込み終了の合図
#define JUMP              "JUMP = "               // ジャンプ量
#define GRAVITY           "GRAVITY = "            // 重力量
#define INERTIA           "INERTIA = "            // 慣性量
#define RIVISION_ANGLE    "RIVISION_ANGLE = "     // 向きを補正する倍率
#define COL_HEIGHT        "COL_HEIGHT = "         // 当たり判定をとる高さ
#define COL_RANGE         "COL_RANGE = "          // 当たり判定をとる範囲
#define NUM_PARTS         "NUM_PARTS = "          // キャラクターのパーツ数

// パーツ情報
#define PARTSSET          "PARTSSET"              // パーツ情報読み込み開始の合図
#define END_PARTSSET      "END_PARTSSET"          // パーツ情報読み込み終了の合図
#define INDEX             "INDEX = "              // 番号を読み取る合図
#define PARENT            "PARENT = "             // 親パーツの番号を読み取る合図

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
#define KEYSET            "KEYSET"                // キーフレーム情報読み込み開始の合図
#define END_KEYSET        "END_KEYSET"            // キーフレーム情報読み込み終了の合図
#define FRAME             "FRAME = "              // キーフレームの再生フレーム数を読み取る合図

// キー情報
#define KEY               "KEY"                   // キー情報読み込み開始の合図
#define END_KEY           "END_KEY"               // キー情報読み込み終了の合図

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
D3DXVECTOR3 CPlayer::m_AvaterOffset[PLAYER_AVATAER_NUM]   // 分身のオフセット
{
	PLAYER_AVATEROFFSET_1,
	PLAYER_AVATEROFFSET_2,
	PLAYER_AVATEROFFSET_3,
	PLAYER_AVATEROFFSET_4
};

//*****************************************************************************
//     CPlayerManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayerManager::CPlayerManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTextureManager = NULL;     // テクスチャ管轄クラスへのポインタ
	strcpy(m_aFileName, "\0");    // プレイヤーデータのスクリプトファイル名
	m_pModelManager = NULL;       // モデル管轄クラスへのポインタ
	m_fOrbitSlash = 0.0f;         // プレイヤーの軌跡ゲージ量
	m_nLife = 0;                  // プレイヤーの体力
	m_fMaxSpecial = 0.0f;         // プレイヤーの必殺技ゲージの最大値の最大値
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayerManager::~CPlayerManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPlayerManager *CPlayerManager::Create(char *pFileName, int nLife, float fOrbitSlash, float fMaxSpecial, int nPriority)
{
	CPlayerManager *pPlayerManager = NULL;  // プレイヤー管轄クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();    // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pPlayerManager == NULL)
		{// メモリが空になっている
			pPlayerManager = new CPlayerManager(nPriority);
			if (pPlayerManager != NULL)
			{// インスタンスを生成できた
				pPlayerManager->SetFileName(pFileName);
				pPlayerManager->SetLife(nLife);
				pPlayerManager->SetOrbitSlash(fOrbitSlash);
				pPlayerManager->SetMaxSpecial(fMaxSpecial);
				if (FAILED(pPlayerManager->Init()))
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

	return pPlayerManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPlayerManager::Init(void)
{
	// プレイヤーのパーツ情報を抜き出す
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
			int nNumModel = 0;                          // 読み込むモデル数
			int nNumParts = 0;                          // 読み込むパーツ数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数を数える
			int nCntModel = 0;                          // モデルを読み込んだ回数を数える
			int nCntParts = 0;                          // パーツ情報を読み込んだ回数を数える
			int nCntMotion = 0;                         // モーションを読み込んだ回数を数える
			int *pNumParent = NULL;                     // 親モデルの番号
			CMotionManager *pMotionManager = NULL;      // モーション管理クラスへのポインタ
			CMotion *pMotion[CPlayer::STATE_MAX] = {};  // モーションクラスへのポインタ
			CModel **apModel = NULL;                    // モデルクラスへのポインタ
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ情報へのポインタ
			LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                          // マテリアル情報の数
			float fAccel = 0.0f;                        // プレイヤーの加速度情報
			float fInertia = 0.0f;                      // プレイヤーの慣性情報
			float fGravity = 0.0f;                      // プレイヤーの重力情報
			float fJumpPower = 0.0f;                    // プレイヤーのジャンプ量情報
			float fRivisionRot = 0.0f;                  // プレイヤーの向きを補正する倍率情報
			float fColHeight = 0.0f;                    // プレイヤーの当たり判定を取る高さ情報
			float fColRange = 0.0f;                     // プレイヤーの当たり判定を取る範囲情報

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
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// 読み込むモデル数だった
								strcpy(pStr, pStrCur);
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// モデル数が1つ以上ある
									pNumParent = new int[nNumModel];
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// モデルのファイルパス名を読み込んだ
								if (m_pModelManager != NULL && nCntModel < nNumModel)
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
								apModel = ReadCharacterSet(pLine, pStrCur, pFile, &nNumParts, pNumParent, &fAccel, &fInertia, &fJumpPower, &fRivisionRot, &fGravity, &fColHeight, &fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// モーション情報読み込み開始の合図があった
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &nNumParts, pMotion[nCntMotion], nCntMotion);
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
			if (pMotionManager == NULL)
			{// ポインタが確保されている
				pMotionManager = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
				if (pMotionManager != NULL)
				{// モーション管理クラスが作成できた
					for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
					{// 状態の数だけ繰りかえし
						pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// プレイヤークラスを作成する
			CreatePlayer(apModel, pMotionManager, nNumParts, pNumParent, fAccel, fInertia, fJumpPower, fRivisionRot, fGravity, fColHeight, fColRange);

			// 読み取り用のポインタを開放しておく
			if (apModel != NULL)
			{// メモリが確保されている
				for (int nCntModel = 0; nCntModel < nNumParts; nCntModel++)
				{// 使用できるモデル数分繰り返し
					if (apModel[nCntModel] != NULL)
					{// ポインタが確保されている
						apModel[nCntModel]->Uninit();

						// メモリの開放
						delete apModel[nCntModel];
						apModel[nCntModel] = NULL;
					}
				}
				// メモリの開放
				delete[] apModel;
				apModel = NULL;
			}

			// モーションの破棄
			if (pMotionManager != NULL)
			{// ポインタが確保されている
				pMotionManager->Uninit();

				// メモリの開放
				delete pMotionManager;
				pMotionManager = NULL;
			}

			// 親モデルの番号
			if (pNumParent != NULL)
			{// メモリが確保されている
				delete[] pNumParent;
				pNumParent = NULL;
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

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPlayerManager::Uninit(void)
{
	// モデル管轄クラスの破棄
	if (m_pModelManager != NULL)
	{// メモリが確保されている
		// 終了処理
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// 自身のポインタを破棄する
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPlayerManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CPlayerManager::Draw(void)
{

}

//=============================================================================
//    プレイヤークラスを作製する処理
//=============================================================================
void CPlayerManager::CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange)
{
	CModel **apModelCpy = NULL;                     // モデルクラスコピー用
	CMotionAttack *pMotionAttack = NULL;            // 攻撃モーションデータクラスコピー用
	CMotionCollision **pMotionCollision = NULL;     // モーションの当たり判定データクラスコピー用
	CMotionOrbit **pMotionOrbit = NULL;             // モーションの軌跡データクラスコピー用
	CMotion *pMotionCpy[CPlayer::STATE_MAX] = {};   // モージョンクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;       // モーション管轄クラスコピー用
	CPlayer *pPlayer = NULL;                        // プレイヤークラスへのポインタ

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
			pMotionManagerCpy = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
				{// プレイヤーの状態の数だけ繰り返し
					if (pMotionCpy[nCntMotion] == NULL)
					{// メモリを確保できる状態である
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// メモリが確保できた
							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// キーフレームの総数分繰り返し
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// キーフレームの総数分繰り返し
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// 攻撃モーションデータクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// 攻撃モーションデータクラスが生成されている
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// メモリが確保できた
									// 各種値の設定
									pMotionAttack->SetNextMotionIdx(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// 当たり判定データクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionCollision = new CMotionCollision*[pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// メモリが確保できた
									for (int nCntCol = 0; nCntCol < pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// 当たり判定データの数だけ繰り返し
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// メモリが確保できた
											pMotionCollision[nCntCol]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
											pMotionCollision[nCntCol]->SetDamage(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

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
								pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// モーション管轄クラスにポインタを渡す
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// プレイヤークラスを作成する
		if (pPlayer == NULL)
		{// メモリが確保できる状態である
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_nLife, apModelCpy, pMotionManagerCpy, m_pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, m_fOrbitSlash, m_fMaxSpecial, PLAYER_PRIORITY);
		}
	}
}

//=============================================================================
//    プレイヤー情報を読み取る処理
//=============================================================================
CModel **CPlayerManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
		{// 移動量がある
			*pAccel = CFunctionLib::ReadFloat(pStrCur, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, JUMP) == 0)
		{// ジャンプ量がある
			*pJumpPower = CFunctionLib::ReadFloat(pStrCur, JUMP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// 重力量がある
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, INERTIA) == 0)
		{// 慣性量がある
			*pInertia = CFunctionLib::ReadFloat(pStrCur, INERTIA);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RIVISION_ANGLE) == 0)
		{// 向きを補正する倍率量がある
			*pRivisionRot = CFunctionLib::ReadFloat(pStrCur, RIVISION_ANGLE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	当たり判定を取る高さがある
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// 当たり判定を取る範囲がある
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
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
CModel *CPlayerManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // パーツ番号情報
	int nParent = 0;                                   // 親モデルのパーツ番号
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
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx), m_pModelManager->GetTexture(nIdx));
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
CMotion *CPlayerManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
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
CMotionAttack *CPlayerManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
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
CMotionCollision **CPlayerManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
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
CMotionOrbit **CPlayerManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
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
void CPlayerManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
void CPlayerManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
//    プレイヤーデータのスクリプトファイル名を設定する処理
//=============================================================================
void CPlayerManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    プレイヤーの体力を設定する処理
//=============================================================================
void CPlayerManager::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    プレイヤーの軌跡ゲージ量を設定する処理
//=============================================================================
void CPlayerManager::SetOrbitSlash(const float fOrbitSlash)
{
	m_fOrbitSlash = fOrbitSlash;
}

//=============================================================================
//    プレイヤーの必殺技ゲージの最大値を設定する処理
//=============================================================================
void CPlayerManager::SetMaxSpecial(const float fMaxSpecial)
{
	m_fMaxSpecial = fMaxSpecial;
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する処理
//=============================================================================
void CPlayerManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    プレイヤーデータのスクリプトファイル名を取得する処理
//=============================================================================
char *CPlayerManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    プレイヤーの体力を取得する処理
//=============================================================================
int CPlayerManager::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    プレイヤーの軌跡ゲージ量を取得する処理
//=============================================================================
float CPlayerManager::GetOrbitSlash(void)
{
	return m_fOrbitSlash;
}

//=============================================================================
//    プレイヤーの必殺技ゲージの最大値を取得する処理
//=============================================================================
float CPlayerManager::GetMaxSpecial(void)
{
	return m_fMaxSpecial;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelManager *CPlayerManager::GetModelManager(void)
{
	return m_pModelManager;
}


//*****************************************************************************
//     CPlayerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// 各種値の設定
	m_State = STATE_NORMAL;                  // 状態
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 移動量
	m_nLife = 0;                             // 体力
	m_nMaxLife = 0;                          // 体力の最大値
	m_nLifeCounter = 0;                      // 体力の回復を管理するカウンター
	m_nEffectCounter = 0;                    // エフェクトを出す間隔を管理するカウンター
	m_nOrbitCounter = 0;                     // 軌跡ゲージを回復する間隔を管理するカウンター
	m_nAvaterCounter = 0;                    // 分身が出てからの時間
	m_nAvaterNum = 0;                        // 分身を出せる数
	m_nDamageCounter = 0;                    // ダメージをくらってからの時間を管理するカウンター
	m_fAccel = 0.0f;                         // 加速度
	m_fInertia = 0.0f;                       // 慣性
	m_fJumpPower = 0.0f;                     // ジャンプ量
	m_fRivisionRot = 0.0f;                   // 向きを補正する倍率
	m_fOrbitSlash = 0.0f;                    // 軌跡斬に使用する軌跡ゲージ量
	m_fOrbitSlashDef = 0.0f;                 // 軌跡斬に使用する軌跡ゲージ量
	m_fSpecial = 0.0f;                       // 現在の必殺技ゲージ量
	m_fMaxSpecial = 0.0f;                    // 必殺技ゲージの最大値
	m_bJump = false;                         // ジャンプしているかどうか
	m_bAction = false;                       // アクションしているかどうか
	m_bOrbit = false;                        // 軌跡を出しているかどうか
	m_pOrbitEffect = NULL;                   // 軌跡エフェクトクラスへのポインタ
	m_pEffectManager = NULL;                 // エフェクト管轄クラスへのポインタ
	m_pLifeGauge = NULL;                     // 体力ゲージクラスへのポインタ
	m_pOrbitSlashGauge = NULL;               // 軌跡ゲージクラスへのポインタ
	m_pSpecialGauge = NULL;                  // 必殺ゲージクラスへのポインタ
	m_pLockOnPoly = NULL;                    // ロックオン用のポリゴンクラスへのポインタ
	m_bOrbitSlash = false;                   // 軌跡斬をするかどうか
	m_bBooking = false;                      // アクションの予約が入っているかどうか
	m_bInRiver = false;                      // 川に入っているかどうか
	m_bInGrass = false;                      // 草むらの中に入っているかどうか
	m_bLockOn = false;                       // ロックオンしているかどうか
	m_bOracleAttack = false;                 // オラクルアタックをしているかどうか
	m_bDamage = false;                       // ダメージをくらっているかどうか
	m_nCurrentStack = 0;                     // 現在の残像記憶番号
	for (int nCntImage = 0; nCntImage < PLAYER_AFTERIMAGE_NUM; nCntImage++)
	{// 残像記憶量の分だけ繰り返し
		m_AfterPos[nCntImage] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_AfterRot[nCntImage] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_AfterModelPos[nCntImage] = NULL;
		m_AfterModelRot[nCntImage] = NULL;
	}

	for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
	{// 分身を出せる数だけ繰り返し
		D3DXMatrixIdentity(&m_AvaterModelMtxWorld[nCntAvater]);
		m_pOrbitEffectAvater[nCntAvater] = NULL;    // 軌跡エフェクトクラスへのポインタ
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, float fOrbitSlash, float fMaxSpecial, int nPriority)
{
	CPlayer *pPlayer = NULL;               // プレイヤークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pPlayer == NULL)
		{// メモリが空になっている
			pPlayer = new CPlayer(nPriority);
			if (pPlayer != NULL)
			{// インスタンスを生成できた
				if (FAILED(pPlayer->Init(pos, rot, nLife, apModel, pMotionManager, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, fOrbitSlash, fMaxSpecial)))
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

	return pPlayer;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, float fOrbitSlash, float fMaxSpecial)
{
	// 各種値の設定
	SetPos(pos);                        // 現在の座標
	SetRot(rot);                        // 現在の向き
	SetLife(nLife);                     // 体力
	m_nMaxLife = nLife;                 // 体力の最大値
	SetNumParts(nNumParts);             // パーツ数
	SetModel(apModel);                  // モデルクラスへのポインタ
	SetMotionManager(pMotionManager);   // モーション管轄クラスへのポインタ
	SetTextureManager(pTextureManager); // テクスチャ管轄クラスへのポインタ
	SetAccel(fAccel);                   // 加速度
	SetInertia(fInertia);               // 慣性
	SetGravity(fGravity);               // 重力
	SetJumpPower(fJumpPower);           // ジャンプ力
	SetRivisionRot(fRivisionRot);       // 向きを補正する倍率
	SetOrbitSlash(fOrbitSlash);         // 現在の軌跡ゲージ量
	m_fOrbitSlashDef = fOrbitSlash;     // 軌跡ゲージ量
	m_fMaxSpecial = fMaxSpecial;        // 必殺技ゲージの最大値の最大値
	SetColHeight(fColHeight);           // 当たり判定を取る高さ
	SetColRange(fColRange);             // 当たり判定を取る範囲

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
			else if (pScene->GetObjType() == OBJTYPE_LIFEGAUGE2D)
			{// 体力ゲージ(2D)クラスへのポインタだった
				m_pLifeGauge = (CGaugeLife2D*)pScene;
			}
			else if (pScene->GetObjType() == OBJTYPE_SLASHGAUGE2D)
			{// 軌跡ゲージ(2D)クラスへのポインタだった
				m_pOrbitSlashGauge = (CGaugeOrbitSlash2D*)pScene;
			}
			else if (pScene->GetObjType() == OBJTYPE_SPECIALGAUGE2D)
			{// 必殺ゲージ(2D)クラスへのポインタだった
				m_pSpecialGauge = (CGaugeSpecial2D*)pScene;
			}
			pScene = pSceneNext;
		}
	}

	// モデルの位置をニュートラルの位置にする
	SetDefaultPos();

	// モデルの向きをニュートラルの向きにする
	SetDefaultRot();

	// 残像記憶用のメモリを確保する
	for (int nCntAfter = 0; nCntAfter < PLAYER_AFTERIMAGE_NUM; nCntAfter++)
	{// 残像を記憶できる数だけ繰り返し
		m_AfterModelPos[nCntAfter] = new D3DXVECTOR3[GetNumParts()];
		if (m_AfterModelPos[nCntAfter] != NULL)
		{// メモリが確保できた
			for (int nCntModel = 0; nCntModel < GetNumParts(); nCntModel++)
			{// モデルの数だけ繰り返し
				m_AfterModelPos[nCntAfter][nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}

		m_AfterModelRot[nCntAfter] = new D3DXVECTOR3[GetNumParts()];
		if (m_AfterModelRot[nCntAfter] != NULL)
		{// メモリが確保できた
			for (int nCntModel = 0; nCntModel < GetNumParts(); nCntModel++)
			{// モデルの数だけ繰り返し
				m_AfterModelRot[nCntAfter][nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// 分身のオフセットを設定
	m_AvaterOffset[0] = PLAYER_AVATEROFFSET_1;
	m_AvaterOffset[1] = PLAYER_AVATEROFFSET_2;
	m_AvaterOffset[2] = PLAYER_AVATEROFFSET_3;
	m_AvaterOffset[3] = PLAYER_AVATEROFFSET_4;

	// 影を作成する
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_SHADOW_COLOR, PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT, PLAYER_SHADOW_PRIORITY);
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 軌跡エフェクトクラスへのポインタを開放する
	if (m_pOrbitEffect != NULL)
	{// メモリが確保されている
		m_pOrbitEffect->Uninit();
		m_pOrbitEffect = NULL;
	}
	for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
	{// 分身の数だけ繰り返し
		if (m_pOrbitEffectAvater[nCntAvater] != NULL)
		{// メモリが確保されている
			m_pOrbitEffectAvater[nCntAvater]->Uninit();
			m_pOrbitEffectAvater[nCntAvater] = NULL;
		}
	}

	// ロックオンのポリゴンを開放する
	if (m_pLockOnPoly != NULL)
	{
		m_pLockOnPoly->Uninit();
		m_pLockOnPoly = NULL;
	}

	// エフェクト管轄クラスへのポインタを空にしておく
	m_pEffectManager = NULL;

	// 体力ゲージクラスへのポインタを空にしておく
	m_pLifeGauge = NULL;

	// 軌跡ゲージクラスへのポインタを空にしておく
	m_pOrbitSlashGauge = NULL;

	// 必殺ゲージクラスへのポインタを空にしておく
	m_pSpecialGauge = NULL;

	// ロックオン対象の敵ポインタを開放する
	m_pLockOnEnemy = NULL;

	// 共通の終了処理
	CCharacter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// 前回乗っていたオブジェクトへのポインタを保存
	SetLandSceneOld(GetLandScene());

	// 乗っているオブジェクトへのポインタを空にする
	SetLandScene(NULL);

	// 前回の座標を保存
	SetPosOld(GetPos());

	if (m_State == STATE_ATTACK_3 && GetMotionManager()->GetAllCounter() >= 34 && GetMotionManager()->GetAllCounter() <= 47)
	{// 最後の攻撃モーションかつこのモーションのカウンター内である
		if (m_nCurrentStack >= PLAYER_AFTERIMAGE_NUM - 1)
		{// まだスタック量がある
		    // 配列の最後尾から配列の先頭に向かって値をコピー
			for (int nCntAfter = 0; nCntAfter <= m_nCurrentStack - 1; nCntAfter++)
			{// 最後尾アドレスからスタート
				m_AfterPos[nCntAfter] = m_AfterPos[nCntAfter + 1];
				m_AfterRot[nCntAfter] = m_AfterRot[nCntAfter + 1];
				for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
				{// モデルのパーツ数分繰り返し
					m_AfterModelPos[nCntAfter][nCntParts] = m_AfterModelPos[nCntAfter + 1][nCntParts];
					m_AfterModelRot[nCntAfter][nCntParts] = m_AfterModelRot[nCntAfter + 1][nCntParts];
				}
			}
			m_nCurrentStack--;
		}

		// 残像を記憶する
		StackAfterImage();
	}
	else
	{// モーションが変わっている
		if (m_nCurrentStack >= 0)
		{// スタック量が変わるまで
		    // 配列の最後尾から配列の先頭に向かって値をコピー
			for (int nCntAfter = 0; nCntAfter <= m_nCurrentStack - 1; nCntAfter++)
			{// 最後尾アドレスからスタート
				m_AfterPos[nCntAfter] = m_AfterPos[nCntAfter + 1];
				m_AfterRot[nCntAfter] = m_AfterRot[nCntAfter + 1];
				for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
				{// モデルのパーツ数分繰り返し
					m_AfterModelPos[nCntAfter][nCntParts] = m_AfterModelPos[nCntAfter + 1][nCntParts];
					m_AfterModelRot[nCntAfter][nCntParts] = m_AfterModelRot[nCntAfter + 1][nCntParts];
				}
			}
			m_nCurrentStack--;
		}
	}

	if (m_State != STATE_LANDING && m_State != STATE_DAMAGE && m_State != STATE_ORACLEATTACK)
	{// ダメージ状態でないかつ着地状態でないかつステップ状態でないかつ
		if (m_bAction != true)
		{// アクション状態でない
			if (CManager::GetMode() == CManager::MODE_GAME && CManager::GetGame()->GetState() == CGame::STATE_NORMAL
				|| CManager::GetMode() == CManager::MODE_TUTORIAL && CManager::GetTutorial()->GetState() == CTutorial::STATE_NONE)
			{// ゲーム中かチュートリアル画面中だったら
				// 移動量計算処理
				InputMove();
			}
		}
	}

	// 移動処理
	Movement();

	// 当たり判定処理
	Collision();

	if (m_State != STATE_LANDING && m_State != STATE_DAMAGE && m_State != STATE_ORACLEATTACK)
	{// 着地状態でないかつダメージ状態でないかつステップ状態でない
		if (CManager::GetMode() == CManager::MODE_GAME && CManager::GetGame()->GetState() == CGame::STATE_NORMAL
			|| CManager::GetMode() == CManager::MODE_TUTORIAL && CManager::GetTutorial()->GetState() == CTutorial::STATE_NONE)
		{// ゲーム中かチュートリアル画面中だったら
		    // アクション処理
			Action();
		}
	}

	// 状態遷移処理
	Statement();

	// エフェクトを作成する処理
	CreateEffect();

	if (GetMotionManager() != NULL)
	{// モーション管理クラスが生成されている
		GetMotionManager()->Update(GetModel());
	}

	// 軌跡ゲージを回復させる
	if (m_fOrbitSlash < m_fOrbitSlashDef)
	{// まだ軌跡ゲージが回復できる
		m_nOrbitCounter++;
		if (m_nOrbitCounter >= PLAYER_ORBIT_UPTIMING)
		{// 回復するタイミングである
			float fUpValue = PLAYER_ORBIT_UPVALUE;
			m_fOrbitSlash += fUpValue;
			if (m_fOrbitSlash >= m_fOrbitSlashDef)
			{// デフォルトの軌跡ゲージ量を超えた
				fUpValue = m_fOrbitSlash - m_fOrbitSlashDef;
				m_fOrbitSlash = m_fOrbitSlashDef;
			}
			// ゲージを増やす
			if (m_pOrbitSlashGauge != NULL)
			{// 軌跡ゲージクラスが取得できている
				m_pOrbitSlashGauge->AddGauge(fUpValue);
			}
		}
	}

	if (m_nAvaterNum >= 1)
	{// 分身が1体以上出ている
		m_nAvaterCounter++;
		if (m_nAvaterCounter % PLAYER_AVATER_CUTTIMING == 0)
		{// カウンターが既定の値に達した
			m_nAvaterNum--;
			if (m_nAvaterNum < 0)
			{// これ以上数値を減らせない
				m_nAvaterNum = 0;
			}
			else
			{// 分身が減った
				if (m_pEffectManager != NULL)
				{// エフェクト管轄クラスへのポインタが取得できている
					D3DXVECTOR3 EffectPos = GetPos() + m_AvaterOffset[m_nAvaterNum];
					m_pEffectManager->SetEffect(D3DXVECTOR3(EffectPos.x, EffectPos.y + 40.0f, EffectPos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 28);
				}
				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_AVATER_DELETE);

				// カウンターを戻す
				m_nAvaterCounter = 0;
			}
		}
	}

	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// チュートリアル中だったら
		if (m_nLife < m_nMaxLife)
		{// 体力が減っている
			m_nLifeCounter++;
			if (m_nLifeCounter % 5 == 0)
			{// 一定の期間更新された
				m_nLife++;
				if(m_pLifeGauge != NULL)
				{// 体力ゲージが取得できている
					m_pLifeGauge->AddGauge(1);
				}
			}
		}
	}

	if (m_bDamage == true)
	{// ダメージをくらっていたら
		m_nDamageCounter--;
		if (m_nDamageCounter <= 0)
		{// ダメージをくらってからある程度たった
			m_nDamageCounter = 0;
			m_bDamage = false;
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 橋の下の描画時に必要
	CModel **pModel = GetModel();                       // モデルクラスへのポインタ
	LPD3DXBUFFER pBuffMat;                              // マテリアル情報へのポインタ
	DWORD nNumMat;                                      // マテリアル情報の数
	D3DXMATERIAL *pMat;                                 // マテリアルへのポインタ
	D3DXCOLOR ModelColorDef[4][PLAYER_MAX_MODELPARTS];  // モデルの色(保存用)

	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// パーツ数だけ繰り返し
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// ポインタが取得できた
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// マテリアル情報の数だけ繰り返し
				ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}
	}

	// 橋の中にいるかどうか判定
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム中のみ判定
		if (CManager::GetGame()->GetMapIdx() == 0)
		{// 1マップ目のみ判定
			D3DXVECTOR3 pos = GetPos();
			if (pos.z <= 4791.0f && pos.z >= 3695.0f && pos.x >= -2.0f && pos.x <= 855.0f && pos.y < -30.0f)
			{// 橋の中にいる
				for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
				{// パーツ数だけ繰り返し
					pBuffMat = pModel[nCntParts]->GetBuffMat();
					nNumMat = pModel[nCntParts]->GetNumMat();
					if (pBuffMat != NULL)
					{// ポインタが取得できた
						pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
						for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
						{// マテリアル情報の数だけ繰り返し
							ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
							pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(ModelColorDef[nCntMat][nCntParts].r - 0.4f, ModelColorDef[nCntMat][nCntParts].g - 0.4f, ModelColorDef[nCntMat][nCntParts].b - 0.4f, ModelColorDef[nCntMat][nCntParts].a);
						}
					}
				}
			}
		}
	}

	// 共通の描画処理
	if (m_bDamage == true)
	{// ダメージをくらっていたら
		if (m_nDamageCounter % 3 == 0)
		{// 描画するタイミングになった
			CCharacter::Draw();
		}
	}
	else
	{// ダメージをくらっていないなら
		CCharacter::Draw();
	}

	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// パーツ数だけ繰り返し
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// ポインタが取得できた
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// マテリアル情報の数だけ繰り返し
				pMat[nCntMat].MatD3D.Diffuse = ModelColorDef[nCntMat][nCntParts];
			}
		}
	}

	if (m_nCurrentStack > 0)
	{// 残像記憶が始まっている
		DrawAfterImage();
	}

	// 分身を描画する
	if (m_nAvaterNum > 0)
	{// 分身の数が1体でもいる
		DrawAvater();
	}
}

//=============================================================================
//    分身を描画する処理
//=============================================================================
void CPlayer::DrawAvater(void)
{
	// 分身描画に必要
	D3DXVECTOR3 posDef = GetPos();                      // プレイヤーの座標(保存用)
	D3DXVECTOR3 pos = GetPos();                         // 分身の座標
	D3DXCOLOR ModelColorDef[4][PLAYER_MAX_MODELPARTS];  // モデルの色(保存用)
	D3DXCOLOR ModelCol = PLAYER_AFTERIMAGE_COLOR;       // 分身のモデルの色
	CModel **pModel = GetModel();                       // モデルクラスへのポインタ
	LPD3DXBUFFER pBuffMat;                              // マテリアル情報へのポインタ
	DWORD nNumMat;                                      // マテリアル情報の数
	D3DXMATERIAL *pMat;                                 // マテリアルへのポインタ

	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// パーツ数だけ繰り返し
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// ポインタが取得できた
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// マテリアル情報の数だけ繰り返し
				ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Diffuse = ModelCol;
			}
		}
	}

	// 分身を描画する
	for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
	{// 現在の分身の数だけ繰り返し
		pos += m_AvaterOffset[nCntAvater];
		SetPos(pos);
		CCharacter::Draw();
		m_AvaterModelMtxWorld[nCntAvater] = pModel[13]->GetMtxWorld();
		pos = posDef;
	}

	// デフォルトに戻しておく
	SetPos(posDef);
	SetMtxWorld(CManager::GetRenderer()->GetDevice());
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// パーツ数だけ繰り返し
		pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// ポインタが取得できた
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// マテリアル情報の数だけ繰り返し
				pMat[nCntMat].MatD3D.Diffuse = ModelColorDef[nCntMat][nCntParts];
			}
		}
	}

}

//=============================================================================
//    入力による移動量計算処理
//=============================================================================
void CPlayer::InputMove(void)
{
	// 各種値の取得
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き

	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// コントローラーを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// カメラを取得
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// 左方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// 右方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// 上方向の入力がされた
	    // 移動処理
		m_Move.x += sinf(pCamera->GetRot().y) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y) * m_fAccel;

		// 目的の角度変更
		DestRot.y = pCamera->GetRot().y + D3DX_PI;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// 下方向の入力がされた
	    // 移動処理
		m_Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;

		// 目的の角度変更
		DestRot.y = pCamera->GetRot().y;
	}
	else
	{
		if (pJoyStick != NULL)
		{// ジョイスティックが取得できた
			if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
				|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
			{// 左アナログ入力がされている
			    // スティックの倒された長さの比率を計算
				float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

				// コサインカーブで比率を補正
				float fRivision = cosf(sqrtf(2) - fLengthX);
				if (fRivision < 1.0)
				{// 倍率が1を超えていない
					fRivision += 0.09f;  // 足りない分を補正
					if (fRivision >= 1.0f)
					{// 倍率が1を超えてしまった
						fRivision = 1.0f;
					}
				}

				// 移動量計算処理
				m_Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivision);
				m_Move.z -= cosf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivision);

				// 目的の角度変更
				DestRot.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
			}
			else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
				|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
			{// 十字キー入力がされている
			    // 移動量計算処理
				m_Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);

				// 目的の角度変更
				DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			}
		}
	}

	// 各種値の設定
	SetDestRot(DestRot);    // 目的の向き
}

//=============================================================================
//    計算された移動量を座標情報に加える処理
//=============================================================================
void CPlayer::Movement(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();           // 現在の座標
	D3DXVECTOR3 rot = GetRot();           // 現在の向き
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き
	D3DXVECTOR3 DiffRot = GetDiffRot();   // 目的の向きと現在の向きの差分
	float fGravity = GetGravity();        // 重力

	// 現在の座標に移動量を加える
	pos += m_Move;

	// 重力を加える
	m_Move.y += fGravity;

	// 移動量に慣性を加える
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// 角度の修正
	DiffRot.y = DestRot.y - rot.y;   // 現在の向きと目的の向きの差分を計算

	if (DiffRot.y > D3DX_PI)
	{// 差分がD3DX_PIを超えた
		DiffRot.y -= D3DX_PI * 2.0f;
	}
	if (DiffRot.y < -D3DX_PI)
	{// 差分が-D3DX_PIを超えた
		DiffRot.y += D3DX_PI * 2.0f;
	}

	// 現在の向きに目的の向きとの差分を倍率で補正する
	rot.y += DiffRot.y * m_fRivisionRot;

	if (rot.y > D3DX_PI)
	{// 現在の向きがD3DX_PIを超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 現在の向きが-D3DX_PIを超えた
		rot.y += D3DX_PI * 2.0f;
	}

	// 各種値の設定
	SetPos(pos);          // 現在の座標
	SetRot(rot);          // 現在の向き
	SetDiffRot(DiffRot);  // 目的の向きと現在の向きの差分

	CDebugProc::Print("プレイヤーの座標 : %.1f %.1f %.1f\n", pos.x, pos.y, pos.z);
	CDebugProc::Print("プレイヤーの移動量 : %.1f %.1f %.1f\n", m_Move.x, m_Move.y, m_Move.z);
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	bool bLand = false;     // 乗っているかどうか
	bool bInGrass = false;  // 草むらの中にいるかどうか

	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();    // 前回の座標

	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CObjectGoal *pObjectGoal = NULL;     // ゴール用配置物クラスへのポインタ
	CMeshField *pField = NULL;           // 地面クラスへのポインタ
	CMeshWall *pWall = NULL;             // 壁クラスへのポインタ
	CItem *pItem = NULL;                 // アイテムクラスへのポインタ
	CRiver *pRiver = NULL;               // 川クラスへのポインタ
	CEnemy *pEnemy = NULL;               // 敵クラスへのポインタ
	CBillboardObject *pBillObj = NULL;   // 配置ビルボードクラスへのポインタ
	CGrass *pGrass = NULL;               // 草クラスへのポインタ
	CBoxCollider *pBoxCollider = NULL;   // 当たり判定用箱モデルクラスへのポインタ
	CScene *pLandSceneOld = NULL;        // 前回乗っていたオブジェクトへのポインタ
	bool bRiverLand = false;             // 川に乗っているかどうか
	bool bWallCol = false;               // 壁に当たっているかどうか
	float fFieldHeight = 0.0f;           // 地面の高さ

	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// チュートリアル中だったら
		CTutorial *pTutorial = CManager::GetTutorial();
		if (pTutorial != NULL)
		{// チュートリアルクラスが取得できた
			if (pos.x > -100.0f && pos.x <= 100.0f
				&& pos.z >= -25.0f)
			{
				pos.z = -25.0f;
				if (pTutorial->GetGoalCol() == false)
				{// まだ壁に当たっていなかった
					pTutorial->SetState(CTutorial::STATE_MODECHECK);
					pTutorial->SetGoalCol(true);
				}
			}
			else
			{
				pTutorial->SetGoalCol(false);
			}
		}
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム中ならば
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// ゲームクラスが取得できた
			if (pGame->GetMapClear() == false)
			{// マップ未クリア状態である
				pScene = CScene::GetTop(9);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == OBJTYPE_GRASS)
					{// 草クラスだったら
						pGrass = (CGrass*)pScene;
						if (pGrass != NULL)
						{// 草クラスが取得できた
							if (pGrass->Collision(&pos, GetColRange()))
							{// 草に当たっている
								bInGrass = true;
								if (m_State == STATE_MOVE)
								{// 移動状態である
									if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 1
										|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 3)
									{// 音を出すタイミングになった
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
									}
								}
							}
						}
					}
					// 次のオブジェクトへのポインタを取得
					pScene = pSceneNext;
				}
			}
		}
	}


	// 橋の中にいるかどうか判定
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム中のみ判定
		if (CManager::GetGame()->GetMapIdx() == 0)
		{// 1マップ目のみ判定
			if (pos.z <= 4791.0f && pos.z >= 3695.0f && pos.x >= -2.0f && pos.x <= 855.0f && pos.y <= -10.0f)
			{// 橋の中にいる
				if (pos.y + GetColHeight() >= -50.0f)
				{// 橋に当たった
					if (m_bJump == true)
					{// ジャンプしている
						pos.y = -60.0f;
						Landing();
					}
				}
			}
		}
	}

	// 前回乗っていたオブジェクトに合わせて高さをずらす

	bool bFieldLand = false;
	pLandSceneOld = GetLandSceneOld();
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム中だったら
		if (pLandSceneOld != NULL)
		{// ポインタが空ではない
			if (pLandSceneOld->GetObjType() == OBJTYPE_MESHFIELD)
			{// 前回乗っていたものは地面だった
				pField = (CMeshField*)pLandSceneOld;
				D3DXVECTOR3 FieldNormal;
				if (pField != NULL)
				{// 地面のクラスが取得できた
					fFieldHeight = 0.0f;
					fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, &FieldNormal);
					if (m_bJump != true)
					{// ジャンプ状態ではない
						if (bFieldLand == true)
						{// 地面の中ににいると判定された
							SetLandScene((CScene*)pField);
							pos.y = fFieldHeight;
							m_Move.y = 0.0f;
							if (GetShadow() != NULL)
							{
								GetShadow()->SetVertexHeight(D3DXVECTOR3(pos.x, fFieldHeight + 1.0f, pos.z), &FieldNormal);
							}
						}
					}
				}
			}
		}
	}

	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY + 1; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (bLand != true)
			{// まだ何かに乗っていない
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// 地面クラスへのポインタだったら
					if (CManager::GetMode() == CManager::MODE_GAME)
					{// ゲーム中だったら
						if (bFieldLand != true)
						{// まだ地面に乗っていない
							pField = (CMeshField*)pScene;
							if (pField != NULL)
							{// 地面が取得できた
								D3DXVECTOR3 FieldNormal;
								fFieldHeight = 0.0f;
								fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, &FieldNormal);
								if (bFieldLand == true)
								{// 地面の中ににいると判定された
									if (GetShadow() != NULL)
									{
										GetShadow()->SetVertexHeight(D3DXVECTOR3(pos.x, fFieldHeight + 1.0f, pos.z), &FieldNormal);
									}

									if (pField != pLandSceneOld && GetLandScene() != NULL && m_bJump != true)
									{// 乗っている地面が変わった
										pos.y = fFieldHeight;
									}
									if (pos.y < fFieldHeight)
									{// 座標が地面より下になった
										if (m_bJump == true)
										{// ジャンプしていたら
											bFieldLand = false;
											if (GetPosOld().y + 1.0f >= fFieldHeight || m_Move.y < 0.0f && pos.y < fFieldHeight)
											{
												m_Move.y = 0.0f;
												SetLandScene((CScene*)pField);
												pos.y = fFieldHeight;
												Landing();
												bFieldLand = true;
											}
										}
										else
										{// ジャンプしていなかったら
											SetLandScene((CScene*)pField);
											m_Move.y = 0.0f;
											pos.y = fFieldHeight;
										}
									}
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_MESHWALL)
				{// 壁クラスへのポインタだったら
					if (CManager::GetMode() == CManager::MODE_GAME)
					{// ゲーム中だったら
						pWall = (CMeshWall*)pScene;
						if (pWall != NULL)
						{// 壁が取得できた
							D3DXVECTOR3 InterPoint = pos;
							if (pWall->Collision(&pos, &posOld, &m_Move, D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange()), &InterPoint))
							{// 壁に当たっている]
								if (bWallCol == true)
								{// すでに壁に当たっている
									pos.x = InterPoint.x;
									pos.z = InterPoint.z;
								}
								bWallCol = true;
								CDebugProc::Print("壁の座標 : %.1f %.1f %.1f\n", pWall->GetPos().x, pWall->GetPos().y, pWall->GetPos().z);
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// 配置物だったら
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// 配置物が取得できた
						if (pObject->Collision(&pos, &posOld, &m_Move, GetColHeight(), GetColRange(), this) == true)
						{// 配置物の上に乗っている
							if (GetLand() == false)
							{// 着地していなかったら
								SetLand(true);
								Landing();
							}

							if (GetLandScene() == NULL && GetLand() == true && m_bJump == false)
							{// 何かに乗っていた
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECTGOAL)
				{// ゴール用の配置物だったら
					pObjectGoal = (CObjectGoal*)pScene;
					if (pObjectGoal != NULL)
					{// ゴール用配置物が取得できた
						if (pObjectGoal->Collision(&pos, &posOld, &m_Move, GetColHeight(), GetColRange(), this) == true)
						{// ゴール配置物に当たっている
							if (GetLand() == false)
							{// 着地していなかったら
								SetLand(true);
								Landing();
							}
							SetLandScene((CScene*)pObject);
							bLand = true;

							// マップ切り替えの準備に入る
							CGame *pGame = CManager::GetGame();
							if (pGame != NULL)
							{// ゲームクラスが取得できた
								if (pGame->GetMapClear() == false)
								{// マップ未クリア状態である
									pGame->SetMapClear(true);
									if (pGame->GetMapIdx() < (pGame->GetNumMap() - 1))
									{// まだ読み込んだマップがある
										if (pGame->GetState() != CGame::STATE_MAPCLEAR)
										{// マップクリア状態になっていない
											pGame->SetState(CGame::STATE_MAPCLEAR);
										}
									}
									else
									{// これ以上はマップがない
										if (pGame->GetState() != CGame::STATE_END)
										{// 終了状態になっていない
											pGame->SetState(CGame::STATE_END);
										}
									}
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ITEM)
				{// アイテムだったら
					pItem = (CItem*)pScene;
					if (pItem != NULL)
					{// アイテムクラスが取得できた
						if (pItem->Collision(&pos, GetColRange()))
						{// アイテムに当たっている
							// アイテムの効果発動
							switch (pItem->GetType())
							{// アイテムの種類によって処理わけ
							case CItem::TYPE_NORMAL:  // 普通
								break;
							case CItem::TYPE_HIDDEN:  // 隠しアイテム
								if (CManager::GetMode() == CManager::MODE_GAME)
								{// ゲーム中だったら
									CManager::GetGame()->SetHiddenItem(CManager::GetGame()->GetHiddenItem() + 1);
								}
								// SEを再生
								CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GET_HIDDEN_ITEM);
								break;
							}

							// アイテムを消す
							pItem->Uninit();
							pItem = NULL;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_RIVER)
				{// 川クラスへのポインタだった
					if (bRiverLand != true)
					{// 川に乗っている
						pRiver = (CRiver*)pScene;
						if (pRiver != NULL)
						{// 川クラスへのポインタが取得できた
							float PosY = pRiver->GetPos().y;
							if (PosY < 0.0f)
							{// 川の座標が一定の値より低い
								float fRiverHeight = pRiver->GetPolyHeight(pos, NULL, NULL);
								if (pos.y < fRiverHeight && pRiver->GetXBlock() <= 6 && fRiverHeight != 0.0f)
								{// 川より下にいる
									bRiverLand = true;
									m_nEffectCounter++;
									if (m_State == STATE_MOVE)
									{// 移動状態だった
									    // SEを再生
										if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 0
											|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 2)
										{// 再生するタイミングになった
											CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RIVER);
										}
										if (m_nEffectCounter % 10 == 0)
										{//エフェクトを出すタイミングになった
											m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 13);
											m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
											if (m_nAvaterNum >= 1)
											{// 分身が1体以上いる
												D3DXVECTOR3 posDef = pos;
												for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
												{// 現在の分身の数だけ繰り返し
													pos += m_AvaterOffset[nCntAvater];
													m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 13);
													m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
													pos = posDef;
												}
											}
										}
									}
									else
									{// それ以外の状態だった
										if (m_nEffectCounter % 30 == 0)
										{//エフェクトを出すタイミングになった
											m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
											if (m_nAvaterNum >= 1)
											{// 分身が1体以上いる
												D3DXVECTOR3 posDef = pos;
												for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
												{// 現在の分身の数だけ繰り返し
													pos += m_AvaterOffset[nCntAvater];
													m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
													pos = posDef;
												}
											}
										}
									}
									CDebugProc::Print("川に乗っている\n");
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// 敵だったら
					pEnemy = (CEnemy*)pScene;
					if (pEnemy != NULL)
					{// 敵クラスへのポインタが取得できた
						if (pEnemy->Collision(&pos, &posOld, &m_Move, GetColRange()))
						{// 敵に当たっている
							CDebugProc::Print("敵に当たっています\n");
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BILLBOARDOBJ)
				{// 配置ビルボードだったら
					pBillObj = (CBillboardObject*)pScene;
					if (pBillObj != NULL)
					{// 配置ビルボードクラスが取得できた
						if (pBillObj->Collision(&pos, GetColRange()))
						{// 配置ビルボードに当たっている
							if (pBillObj->GetType() == CBillboardObject::TYPE_GRASS)
							{// 草用のビルボードだった
								if (m_State == STATE_MOVE)
								{// 移動状態である
									if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 1
										|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 3)
									{// 音を出すタイミングになった
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
									}
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BOXCOLLIDER)
				{// 当たり判定用箱モデルだったら
					pBoxCollider = (CBoxCollider*)pScene;
					if (pBoxCollider != NULL)
					{// 当たり判定用箱モデルクラスへのポインタが取得できた
						if (pBoxCollider->Collision(&pos, &posOld, &m_Move, D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange()), &bLand))
						{// 箱に当たっている
							if (bLand == true)
							{// 箱に乗っている
								m_Move.y = 0.0f;
								if (m_bJump == true)
								{// ジャンプしていたら
									Landing();
								}
							}
						}
					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	// 川に入っているかどうかを保存する
	m_bInRiver = bRiverLand;

	// 草むらの中にいるかどうか保存する
	m_bInGrass = bInGrass;

	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// チュートリアル画面だったら
		if (m_bJump != true)
		{// ジャンプしていない
			pos.y = 0.0f;
			m_bJump = false;
			SetLandScene(this);
			m_Move.y = 0.0f;
		}
		else
		{// ジャンプしている
			if (m_Move.y <= 0.0f && posOld.y >= 0.0f && pos.y < 0.0f)
			{// 地面に着地した
				m_bJump = false;
				Landing();
				pos.y = 0.0f;
				m_Move.y = 0.0f;
			}
		}
		if (GetShadow() != NULL)
		{
			GetShadow()->SetVertexHeight(D3DXVECTOR3(pos.x, 1.0f, pos.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		}
	}

	// 各種値の設定
	SetPos(pos);         // 現在の座標
	SetLand(bLand);      // オブジェクトに乗っているかどうか
}

//=============================================================================
//    アクション処理
//=============================================================================
void CPlayer::Action(void)
{
	// 各種値の取得
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き

	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// コントローラーを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// カメラを取得
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
	{// ジャンプボタンが押された
		if (GetLandScene() != NULL)
		{// 何かに乗っている
			if (m_bAction != true)
			{// アクションしていない
				if (m_bJump == false)
				{// ジャンプしていない
					// 軌跡を消す
					if (m_pOrbitEffect != NULL)
					{
						m_bOrbit = false;
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
					for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
					{
						if (m_pOrbitEffectAvater[nCntAvater] != NULL)
						{
							m_pOrbitEffectAvater[nCntAvater]->Uninit();
							m_pOrbitEffectAvater[nCntAvater] = NULL;
						}
					}

					// ジャンプ量を加えジャンプしている判定にする
					m_Move.y += m_fJumpPower;
					m_bJump = true;

					// モーション切り替え
					m_State = STATE_JUMP;
					GetMotionManager()->SwitchMotion(GetModel(), m_State);

					// SE再生
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_JUMP);
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_SPACE) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == true)
	{// アクションボタンが押された
		if (m_bAction == false)
		{// アクションをしていない
			if (m_State == STATE_MOVE)
			{// 動いている状態だったら
				if (GetLand() == true)
				{// 何かに乗っている
					CalcOffsetRot();
				}
			}

			// アクションをしている判定に
			m_bAction = true;

			// モーション切り替え
			m_State = STATE_ATTACK_0;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);

			// 軌跡を消しておく
			if (m_pOrbitEffect != NULL)
			{
				m_bOrbit = false;
				m_pOrbitEffect->Uninit();
				m_pOrbitEffect = NULL;
			}
			for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
			{
				if (m_pOrbitEffectAvater[nCntAvater] != NULL)
				{
					m_pOrbitEffectAvater[nCntAvater]->Uninit();
					m_pOrbitEffectAvater[nCntAvater] = NULL;
				}
			}
		}
		else
		{// 攻撃をすでに行っている
			if (GetMotionManager()->GetMotion()[m_State]->GetAttack() != NULL)
			{// 次のモーションに派生できるモーションだった
				// 各種データを用意
				int nNextMotionIdx = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetNextMotionIdx();     // 次のモーション番号
 				int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetStartTiming();         // 次のモーションに派生できる時間が開始するタイミング
				int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetFinishTiming();       // 次のモーションに派生できる時間が終了するタイミング
				if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
				{// 次のモーションに派生できるフレーム内で入力が行われたか予約が入っている
					// 軌跡を消す
					if (m_pOrbitEffect != NULL)
					{
						m_bOrbit = false;
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
					for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
					{
						if (m_pOrbitEffectAvater[nCntAvater] != NULL)
						{
							m_pOrbitEffectAvater[nCntAvater]->Uninit();
							m_pOrbitEffectAvater[nCntAvater] = NULL;
						}
					}

					// モーションを切り替える
					m_State = (STATE)nNextMotionIdx;
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
				else if (m_bBooking == false)
				{// まだ予約が入っていない
					if (GetMotionManager()->GetAllCounter() <= nFinishTiming)
					{// まだ次のモーションに派生できる
						m_bBooking = true;   // 予約を入れておく
					}
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_E) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_4) == true)
	{// ロックオンボタンが押された
		LockOn();
	}
	else if (pKeyboard->GetTrigger(DIK_R) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_5) == true)
	{// 軌跡斬切り替えボタンが押された
		if (m_bAction != true)
		{// アクションをしていない
			if (m_fOrbitSlash >= 0.0f)
			{// 軌跡ゲージがまだある
				m_bOrbitSlash = m_bOrbitSlash ? false : true;
				if (m_bOrbitSlash == true && m_fOrbitSlash > 0.0f)
				{// 軌跡斬を出せる状態になった
					// SEを再生
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ORBITSLASH);
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_N) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_6) == true)
	{// 分身を増やすボタンが押された
		if (m_bAction != true)
		{// アクションをしていない
			if (m_nLife > PLAYER_AVATER_LIFECUT)
			{// 分身を出せる体力である
				m_nAvaterNum++;
				if (m_nAvaterNum > PLAYER_AVATAER_NUM)
				{// これ以上分身は出せない
					m_nAvaterNum = PLAYER_AVATAER_NUM;
				}
				else
				{// 分身を出せた
					// 体力を削る
					m_nLife -= PLAYER_AVATER_LIFECUT;

					// 体力ゲージを減らす
					if (m_pLifeGauge != NULL)
					{
						m_pLifeGauge->CutGauge(PLAYER_AVATER_LIFECUT);
					}
					if (m_pEffectManager != NULL)
					{// エフェクト管轄クラスへのポインタが取得できている
						D3DXVECTOR3 EffectPos = GetPos() + m_AvaterOffset[m_nAvaterNum - 1];
						m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 26);
						m_pEffectManager->SetEffect(D3DXVECTOR3(EffectPos.x, EffectPos.y + 10.0f, EffectPos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 27);
					}

					// SEを再生
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_AVATER_SPAWN);

					// カウンターを戻す
					m_nAvaterCounter = 0;
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_M) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_7) == true)
	{// 分身を減らすボタンが押された
		if (m_bAction != true)
		{// アクションをしていない
			m_nAvaterNum--;
			if (m_nAvaterNum < 0)
			{// これ以上数値を減らせない
				m_nAvaterNum = 0;
			}
			else
			{// 分身が減った
				if (m_pEffectManager != NULL)
				{// エフェクト管轄クラスへのポインタが取得できている
					D3DXVECTOR3 EffectPos = GetPos() + m_AvaterOffset[m_nAvaterNum];
					m_pEffectManager->SetEffect(D3DXVECTOR3(EffectPos.x, EffectPos.y + 40.0f, EffectPos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 28);
				}
				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_AVATER_DELETE);

				// カウンターを戻す
				m_nAvaterCounter = 0;
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_B) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_1) == true)
	{// オラクルアタックボタンが押された
		if (m_fSpecial >= m_fMaxSpecial)
		{// スペシャルゲージが満タンになっている
			if (m_bAction != true && m_bJump != true)
			{// アクションをしていないかつジャンプをしていない
				// オラクルアタックをしている判定に
				m_bOracleAttack = true;

				// 軌跡斬をしていない判定に
				m_bOrbitSlash = false;

				// モーションを切り替える
				m_State = STATE_ORACLEATTACK;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				// 必殺ゲージを削っておく
				m_fSpecial = 0.0f;
				if (m_pSpecialGauge != NULL)
				{// ポインタが取得できている
					m_pSpecialGauge->DeleteGauge();
				}

				// エフェクトを出す
				m_pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ORACLE_START);
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_LSHIFT) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_9) == true)
	{// 視点リセットボタンが押された
		D3DXVECTOR3 PlayerRot = GetRot();     // プレイヤーの向きを取得

		// 向きをカメラ用に補正する
		PlayerRot.y += D3DX_PI;
		if (PlayerRot.y > D3DX_PI)
		{// 円周率を超えた
			PlayerRot.y -= D3DX_PI * 2.0f;
		}

		// カメラの向きをプレイヤーの正面方向に
		pCamera->SetRotDest(D3DXVECTOR3(0.0f, PlayerRot.y, 0.0f));
		pCamera->SetRotDiff(pCamera->GetRotDest() - pCamera->GetRot());
	}

	if (m_bBooking == true)
	{// アクション予約が入っている
		CDebugProc::Print("アクション予約");
		if (GetMotionManager()->GetMotion()[m_State]->GetAttack() != NULL)
		{// 次のモーションに派生できるモーションだった
		    // 各種データを用意
			int nNextMotionIdx = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetNextMotionIdx();   // 次のモーション番号
			int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetStartTiming();       // 次のモーションに派生できる時間が開始するタイミング
			int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetFinishTiming();     // 次のモーションに派生できる時間が終了するタイミング
			if (GetMotionManager()->GetAllCounter() >= nStartTiming)
			{// 次のモーションに派生できるフレームになっている
			    // 軌跡を消す
				if (m_pOrbitEffect != NULL)
				{
					m_bOrbit = false;
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
				for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
				{
					if (m_pOrbitEffectAvater[nCntAvater] != NULL)
					{
						m_pOrbitEffectAvater[nCntAvater]->Uninit();
						m_pOrbitEffectAvater[nCntAvater] = NULL;
					}
				}

				// モーションを切り替える
				m_State = (STATE)nNextMotionIdx;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				// 予約は外しておく
				m_bBooking = false;


				// 向きを入力方向によって直す
				if (pKeyboard->GetPress(DIK_A) == true)
				{// 左方向の入力がされた
					if (pKeyboard->GetPress(DIK_W) == true)
					{// 同時に上方向の入力がされた
					    // 目的の角度変更
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
					}
					else if (pKeyboard->GetPress(DIK_S) == true)
					{// 同時に下方向の入力がされた
					    // 目的の角度変更
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
					}
					else
					{// 何も押されてない
					    // 目的の角度変更
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
					}
				}
				else if (pKeyboard->GetPress(DIK_D) == true)
				{// 右方向の入力がされた
					if (pKeyboard->GetPress(DIK_W) == true)
					{// 同時に上方向の入力がされた
					    // 目的の角度変更
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
					}
					else if (pKeyboard->GetPress(DIK_S) == true)
					{// 同時に下方向の入力がされた
					    // 目的の角度変更
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
					}
					else
					{// 何も押されてない
					    // 目的の角度変更
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
					}
				}
				else if (pKeyboard->GetPress(DIK_W) == true)
				{// 上方向の入力がされた
				    // 目的の角度変更
					DestRot.y = pCamera->GetRot().y + D3DX_PI;
				}
				else if (pKeyboard->GetPress(DIK_S) == true)
				{// 下方向の入力がされた
				    // 目的の角度変更
					DestRot.y = pCamera->GetRot().y;
				}
				else
				{
					if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
						|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
					{// 左アナログ入力がされている
					 // 目的の角度変更
						DestRot.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
					}
					else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
						|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
					{// 十字キー入力がされている
					 // 目的の角度変更
						DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
					}
				}

				// 目的の角度設定
				SetDestRot(DestRot);
			}
		}
	}
}

//=============================================================================
//    状態遷移処理
//=============================================================================
void CPlayer::Statement(void)
{
	if (m_State != STATE_JUMP && m_State != STATE_LANDING && m_bAction == false && m_State != STATE_DAMAGE && m_State != STATE_ORACLEATTACK)
	{// ジャンプしていないかつ着地状態でないかつアクションをしていないかつダメージ状態でないかつオラクルアタック状態ではない
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// 移動していない
			if (m_State != STATE_NORMAL)
			{// 通常状態ではない
				// 状態を通常の状態に
				m_State = STATE_NORMAL;

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				if (GetLand() == true)
				{// 何かに乗っている
					CalcOffsetRot();
				}
			}
		}
		else
		{// 移動している
			if (m_State != STATE_MOVE)
			{// 歩く状態ではない
				m_State = STATE_MOVE;

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
	else
	{// それ以外
		if (m_State == STATE_LANDING || m_State == STATE_DAMAGE || m_State == STATE_ORACLEATTACK)
		{// 着地状態の時かダメージ状態の時かオラクルアタック状態の時
			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// モーション再生が終了した
				if (m_State == STATE_DAMAGE)
				{// ダメージ状態だったら
					m_nDamageCounter = PLAYER_DAMAGE_COUNTER_MAX;
					m_bDamage = true;
				}

				m_State = STATE_NORMAL;

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}

			if (m_State == STATE_ORACLEATTACK)
			{// オラクルアタックの状態だったら
				if (GetMotionManager()->GetAllCounter() == 72)
				{// ある程度の時間になった
					// エフェクトを出す
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 21);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 22);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 23);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 24);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 25);

					if (m_nAvaterNum >= 1)
					{// 分身が1体以上いる
						D3DXVECTOR3 posDef = GetPos();   // プレイヤーの座標(保存用)
						D3DXVECTOR3 pos = GetPos();      // 分身の座標(保存用)
						for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
						{// 現在の分身の数だけ繰り返し
							// 分身の位置にずらす
							pos += m_AvaterOffset[nCntAvater];

							// エフェクトを出す
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 21);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 22);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 23);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 24);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 25);

							// デフォルト値に戻す
							pos = posDef;
						}
					}
				}

				if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
				{// 当たり判定データクラスが生成されている
					for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
					{// 当たり判定データの数だけ繰り返し
					    // 当たり判定に必要なデータを宣言
						int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();            // 当たり判定を行うモデル番号
						float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();                // 当たり判定を取る範囲
						D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();      // 当たり判定を取る位置(モデルからのオフセット)
						int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();      // 当たり判定を開始するタイミング
						int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();    // 当たり判定を終了するタイミング
						int nDamage = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetDamage();                // 当たった際のダメージ量

						// 範囲を拡大しておく
						fRange += (GetMotionManager()->GetAllCounter() - 50) * 10.0f;

						// 判定開始
						if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
						{// 当たり判定を発生させるタイミングならば
						    // 攻撃時の当たり判定を発生させる
							AttackCollision(nModelIdx, fRange, Offset, nDamage);

							if (m_nAvaterNum >= 1)
							{// 分身が1体以上いる
								CModel **pModel = GetModel();    // モデルクラスへのポインタ
								D3DXVECTOR3 posDef = GetPos();   // プレイヤーの位置(保存用)
								D3DXVECTOR3 pos = GetPos();      // 分身の位置
								for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
								{// 現在の分身の数だけ繰り返し
									// 分身の位置を計算
									pos += m_AvaterOffset[nCntAvater];
									SetPos(pos);
									SetMtxWorld(CManager::GetRenderer()->GetDevice());
									for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
									{// パーツ数だけ繰り返し
										pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
									}

									// 分身の当たり判定を開始
									AttackCollision(nModelIdx, fRange, Offset, nDamage);

									// デフォルト値に戻す
									pos = posDef;
								}

								// デフォルトの値に戻す
								SetPos(posDef);
								SetMtxWorld(CManager::GetRenderer()->GetDevice());
								for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
								{// パーツ数だけ繰り返し
									pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
								}
							}
						}

						// SEを再生
						if (GetMotionManager()->GetAllCounter() == nStartTiming)
						{// 音を出すタイミングになった
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ORACLEATTACK);
						}
					}
				}
			}
		}
		if (m_bAction == true)
		{// アクションをしている
			if (m_State == STATE_ATTACK_0)
			{// 1つ目の攻撃モーションだった
				if (GetMotionManager()->GetAllCounter() >= 25 && GetMotionManager()->GetAllCounter() <= 28)
				{// ある程度モーションが進行している
					// オブジェクトとの判定を行う
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// メモリが空になるまで繰り返し
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// 配置物クラスへのポインタだった
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// 配置物クラスのポインタが取得できた
							    // 配置物の当たり判定を取る範囲を取得
								fObjRange = pObject->GetColRange();

								// 当たり判定を取る長さを計算
								fHitRange = sqrtf((fObjRange + 15.0f) * (fObjRange + 15.0f));

								// 当たり判定を取る座標と配置物の座標との距離を測る
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);
								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// 当たり判定を取る座標と配置物の座標との距離が当たり判定を取る長さより短い
									if (pObject->GetHitAttack() == false)
									{// まだ当たっていなかった
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SEを再生
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}

				if (GetMotionManager()->GetAllCounter() == 22)
				{// ある程度のカウンターの値になっている
					if (m_bInGrass == true)
					{// 草むらの中にいる
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// 軌跡斬をする状態である
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
						m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}
						// SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
					}
					else
					{// 軌跡斬をする状態ではない
					    // SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 18)
				{// ある程度モーションが進行している
					m_Move.x -= sinf(GetRot().y) * 20.0f;
					m_Move.z -= cosf(GetRot().y) * 20.0f;
				}
			}
			else if (m_State == STATE_ATTACK_1)
			{// 2つ目の攻撃モーションだった
				if (GetMotionManager()->GetAllCounter() >= 13 && GetMotionManager()->GetAllCounter() <= 15)
				{// ある程度モーションが進行している
					// オブジェクトとの判定を行う
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// メモリが空になるまで繰り返し
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// 配置物クラスへのポインタだった
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// 配置物クラスのポインタが取得できた
							    // 配置物の当たり判定を取る範囲を取得
								fObjRange = pObject->GetColRange();

								// 当たり判定を取る長さを計算
								fHitRange = sqrtf((fObjRange + 25.0f) * (fObjRange + 25.0f));

								// 当たり判定を取る座標と配置物の座標との距離を測る
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);
								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// 当たり判定を取る座標と配置物の座標との距離が当たり判定を取る長さより短い
									if (pObject->GetHitAttack() == false)
									{// まだ当たっていなかった
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SEを再生
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}

				if (GetMotionManager()->GetAllCounter() == 9)
				{// ある程度モーションが進行している
					if (m_bInGrass == true)
					{// 草むらの中にいる
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// 軌跡斬をする状態である
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
						m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);


						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}
						// SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
					}
					else
					{// 軌跡斬をする状態ではない
					    // SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 7)
				{// ある程度モーションが進行している
					m_Move.x -= sinf(GetRot().y) * 10.0f;
					m_Move.z -= cosf(GetRot().y) * 10.0f;
				}
			}
			else if (m_State == STATE_ATTACK_2)
			{// 3つ目の攻撃モーションだった
				if (GetMotionManager()->GetAllCounter() == 30)
				{
					// オブジェクトとの判定を行う
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// メモリが空になるまで繰り返し
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// 配置物クラスへのポインタだった
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// 配置物クラスのポインタが取得できた
							    // 配置物の当たり判定を取る範囲を取得
								fObjRange = pObject->GetColRange();

								// 当たり判定を取る長さを計算
								fHitRange = sqrtf((fObjRange + 15.0f) * (fObjRange + 15.0f));

								// 当たり判定を取る座標と配置物の座標との距離を測る
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);
								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// 当たり判定を取る座標と配置物の座標との距離が当たり判定を取る長さより短い
									if (pObject->GetHitAttack() == false)
									{// まだ当たっていなかった
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SEを再生
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}
				if (GetMotionManager()->GetAllCounter() == 28)
				{// ある程度モーションが進行している
					if (m_bInGrass == true)
					{// 草むらの中にいる
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// 軌跡斬をする状態である
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
						m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}

						// SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
					}
					else
					{// 軌跡斬をする状態ではない
					    // SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 25)
				{// ある程度モーションが進行している
					m_Move.x -= sinf(GetRot().y) * 20.0f;
					m_Move.z -= cosf(GetRot().y) * 20.0f;
				}
			}
			if (m_State == STATE_ATTACK_3)
			{// 最後の攻撃モーションだった
				if (GetMotionManager()->GetAllCounter() >= 38 && GetMotionManager()->GetAllCounter() <= 40)
				{// ある程度モーションが進行している
					// オブジェクトとの判定を行う
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// メモリが空になるまで繰り返し
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// 配置物クラスへのポインタだった
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// 配置物クラスのポインタが取得できた
							    // 配置物の当たり判定を取る範囲を取得
								fObjRange = pObject->GetColRange();

								// 当たり判定を取る長さを計算
								fHitRange = sqrtf((fObjRange + 30.0f) * (fObjRange + 30.0f));

								// 当たり判定を取る座標と配置物の座標との距離を測る
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);

								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// 当たり判定を取る座標と配置物の座標との距離が当たり判定を取る長さより短い
									if (pObject->GetHitAttack() == false)
									{// まだ当たっていなかった
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SEを再生
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}

				// しかるべきタイミングでエフェクトを出す
				if (GetMotionManager()->GetAllCounter() == 35)
				{// ある程度モーションが進行している
					if (m_bInGrass == true)
					{// 草むらの中にいる
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// 軌跡斬をする状態である
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 45.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 75.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 90.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 105.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 120.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);


						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 45.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 75.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 90.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 105.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 120.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}

						// SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK);
					}
					else
					{// 軌跡斬をする状態ではない
						// SEを再生
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 34)
				{
					m_Move.x -= sinf(GetRot().y) * 25.0f;
					m_Move.z -= cosf(GetRot().y) * 25.0f;
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
			{// 当たり判定データクラスが生成されている
				for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
				{// 当たり判定データの数だけ繰り返し
					// 当たり判定に必要なデータを宣言
					int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();          // 当たり判定を取るモデル番号
					float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();              // 当たり判定を取る範囲
					D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();    // 当たり判定を取る位置(モデルからのオフセット)
					int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();    // 当たり判定を開始するタイミング
					int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();  // 当たり判定を終了するタイミング
					int nDamage = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetDamage();              // 当たった際のダメージ量

					if (m_State == STATE_ATTACK_3)
					{// 最後の攻撃モーションだった
						if (m_bOrbitSlash == true)
						{// 軌跡斬をする状態だったら
							fRange = 12.0f * (GetMotionManager()->GetAllCounter() - nStartTiming);
						}
						else
						{// 軌跡斬をしない状態だったら
							fRange = 50.0f;
							nDamage = 1;
						}
					}

					// 判定開始
					if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
					{// 当たり判定を発生させるタイミングならば
						// 攻撃時の当たり判定を発生させる
						AttackCollision(nModelIdx, fRange, Offset, nDamage);

						if (m_nAvaterNum >= 1)
						{// 分身が1体以上いる
							CModel **pModel = GetModel();
							D3DXVECTOR3 posDef = GetPos();
							D3DXVECTOR3 pos = GetPos();
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// 現在の分身の数だけ繰り返し
								// 分身の位置を計算
								pos += m_AvaterOffset[nCntAvater];
								SetPos(pos);
								SetMtxWorld(CManager::GetRenderer()->GetDevice());
								for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
								{// パーツ数だけ繰り返し
									pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
								}

								// 分身の当たり判定開始
								AttackCollision(nModelIdx, fRange, Offset, nDamage);

								// デフォルト値に戻す
								pos = posDef;
							}

							// デフォルトの値に戻す
							SetPos(posDef);
							SetMtxWorld(CManager::GetRenderer()->GetDevice());
							for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
							{// パーツ数だけ繰り返し
								pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
							}
						}
					}
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetOrbit() != NULL)
			{// 軌跡データクラスが生成されている
				if (m_bOrbitSlash == true)
				{// 軌跡斬を出す状態である
					if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetStartTiming() && m_bOrbit == false)
					{// 軌跡を出すタイミングになった
						if (m_pOrbitEffect == NULL)
						{// オービットエフェクトクラスへのポインタが空になっている
							// 軌跡に必要な情報を宣言
							int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetModelIdx();                // 軌跡を出すモデル番号
							int nTexIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetTexIdx();                    // 軌跡に張り付けるテクスチャの番号
							D3DXVECTOR3 OffSetPos1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos1();     // 軌跡のオフセット(下側)
							D3DXVECTOR3 OffSetPos2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos2();     // 軌跡のオフセット(上側)
							D3DXVECTOR3 OffSetAmp1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp1();     // 軌跡のオフセットをどれくらい動かすか(下側)
							D3DXVECTOR3 OffSetAmp2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp2();     // 軌跡のオフセットをどれくらい動かすか(上側)
							D3DXCOLOR ColUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColUp();                 // 軌跡の色(上側)
							D3DXCOLOR ColDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColDown();             // 軌跡の色(下側)
							float fAlphaDecayUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayUp();      // 軌跡の透明度減衰値(上側)
							float fAlphaDecayDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayDown();  // 軌跡の透明度減衰値(下側)
							int nXBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetXBlock();                    // 軌跡の横の分割数
							int nYBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetYBlock();                    // 軌跡の縦の分割数
							D3DXMATRIX *pMtxParent = GetModel(nModelIdx)->GetMtxWorldPointer();                                    // 軌跡を作るワールドマトリックスへのポインタ
							m_pOrbitEffect = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, pMtxParent, EFFECT_PRIORITY);
							if (m_pOrbitEffect != NULL)
							{// 軌跡を生成できた
								m_pOrbitEffect->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
							}
							if (m_nAvaterNum >= 1)
							{// 分身が１体以上出ている
								for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
								{// 現在の分身の数だけ繰り返し
									m_pOrbitEffectAvater[nCntAvater] = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, &m_AvaterModelMtxWorld[nCntAvater], EFFECT_PRIORITY);
									if (m_pOrbitEffectAvater[nCntAvater] != NULL)
									{// 軌跡を生成できた
										m_pOrbitEffectAvater[nCntAvater]->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
									}
								}
							}
							m_bOrbit = true;
						}
					}
				}
				if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetFinishTiming())
				{// 軌跡をしまうタイミングになった
					if (m_pOrbitEffect != NULL)
					{
						m_bOrbit = false;
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
					for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
					{// 分身を出せる数だけ繰り返し
						if (m_pOrbitEffectAvater[nCntAvater] != NULL)
						{
							m_pOrbitEffectAvater[nCntAvater]->Uninit();
							m_pOrbitEffectAvater[nCntAvater] = NULL;
						}
					}
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// モーション再生が終了した
				m_bAction = false;
				m_bOrbit = false;
				m_State = STATE_NORMAL;
				m_bBooking = false;

				// 軌跡を開放しておく
				if (m_pOrbitEffect != NULL)
				{
					m_bOrbit = false;
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
				for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
				{// 分身を出せる数だけ繰り返し
					if (m_pOrbitEffectAvater[nCntAvater] != NULL)
					{
						m_pOrbitEffectAvater[nCntAvater]->Uninit();
						m_pOrbitEffectAvater[nCntAvater] = NULL;
					}
				}

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}

	if (m_bLockOn == true)
	{// ロックオン中だったら
		if (m_pLockOnEnemy != NULL && m_pLockOnPoly != NULL)
		{// ロックオン対象の敵クラスへのポインタとロックオンポリゴンへのポインタが取得できている
			D3DXVECTOR3 LocOnPolyPos = D3DXVECTOR3(m_pLockOnEnemy->GetPos().x, m_pLockOnEnemy->GetPos().y + PLAYER_LOCKONPOLY_HEIGHT, m_pLockOnEnemy->GetPos().z);
			m_pLockOnPoly->SetPos(LocOnPolyPos);
		}
	}

	if (m_bOrbitSlash == true)
	{// 軌跡斬をする状態ならば
		m_fOrbitSlash -= PLAYER_ORBIT_CUTVALUE;
		m_nOrbitCounter = 0;
		if (m_fOrbitSlash <= 0.0f)
		{// 軌跡ゲージ量がなくなった
			m_fOrbitSlash = 0.0f;
			m_bOrbitSlash = false;
			if (m_pOrbitSlashGauge != NULL)
			{// メモリが確保できている
				m_pOrbitSlashGauge->DeleteGauge();
			}
		}
		else
		{// まだ軌跡ゲージ量がある
			if (m_pOrbitSlashGauge != NULL)
			{// メモリが確保できている
				m_pOrbitSlashGauge->CutGauge(PLAYER_ORBIT_CUTVALUE);
			}
		}
	}
}


//=============================================================================
//    着地したときの処理
//=============================================================================
void CPlayer::Landing(void)
{
	// ジャンプしていない状態に
	m_bJump = false;

	if (m_bAction == false)
	{// アクションをしていない
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// 移動していない
			// 着地のモーションに切り替える
			m_State = STATE_LANDING;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);

			// SEを再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_LANDING);
		}
		else
		{// 移動している
			m_State = STATE_MOVE;

			// モーション切り替え処理
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}

	if (GetLand() == true)
	{// オフセット位置をワールドマトリックスを使って割り出す
		CalcOffsetRot();
	}
}

//=============================================================================
//    攻撃時の当たり判定処理
//=============================================================================
bool CPlayer::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage)
{
	bool bHit = false;                 // 攻撃がヒットしたかどうか
	float fEnemyRange = 0.0f;          // 敵の当たり判定を取る範囲
	D3DXVECTOR3 OffSet = Offset;       // 当たり判定をとる場所(親モデルからのオフセット)
	float fHitRange = 0.0f;            // 当たり判定を取る長さ
	float fLength = 0.0f;              // 当たり判定を取る場所と敵との距離
	D3DXMATRIX mtxWorld;
	if (nDamage == 1)
	{// ダメージが小さい
		if (m_bOrbitSlash == true)
		{// 軌跡斬をしていたら
			nDamage += 6;
		}
	}

	if (m_nAvaterNum >= 1)
	{// 分身が1体以上出ている
		if (m_bOrbitSlash == true)
		{// 軌跡斬を出す状態である
			nDamage += m_nAvaterNum;
		}
		else
		{// 軌跡斬を出す状態でない
			nDamage += (m_nAvaterNum / 2) / 2;
		}
	}

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

	// 判定開始
	if (m_bOrbitSlash == true || m_bOrbitSlash != true && Offset.x < 100.0f && Offset.y < 100.0f && Offset.z > -100.0f)
	{// 軌跡斬をしない状態かつオフセットが離れすぎていないまたは軌跡斬をする状態である
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CEnemy *pEnemy = NULL;
		pScene = GetTop(ENEMY_PRIORITY);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// 敵クラスへのポインタだった
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// 敵クラスのポインタが取得できた
					// 敵の当たり判定を取る範囲を取得
					fEnemyRange = pEnemy->GetColRange();

					// 当たり判定を取る長さを計算
					fHitRange = sqrtf((fEnemyRange + fRange) * (fEnemyRange + fRange));

					// 当たり判定を取る座標と敵の座標との距離を測る
					D3DXMATRIX EnemyMtxWorld;
					D3DXVECTOR3 EnemyPos;
					CModel *pModel = pEnemy->GetModel(0);
					if (pModel != NULL)
					{
						EnemyMtxWorld = pModel->GetMtxWorld();
						EnemyPos = D3DXVECTOR3(EnemyMtxWorld._41, EnemyMtxWorld._42 + 10.0f, EnemyMtxWorld._43);
						fLength = sqrtf((EnemyPos.x - OffSet.x) * (EnemyPos.x - OffSet.x) + (EnemyPos.y - OffSet.y) * (EnemyPos.y - OffSet.y) + (EnemyPos.z - OffSet.z) * (EnemyPos.z - OffSet.z));
					}

					if (fLength <= fHitRange)
					{// 当たり判定を取る座標と敵の座標との距離が当たり判定を取る長さより短い
						if (pEnemy->GetDamage() != true && pEnemy->GetState() != CEnemy::STATE_DEATH)
						{// 敵の状態がダメージ状態でない
							bool bDeath = false;
							D3DXVECTOR3 EffectPos = D3DXVECTOR3(pEnemy->GetModel()[0]->GetMtxWorld()._41, pEnemy->GetModel()[0]->GetMtxWorld()._42, pEnemy->GetModel()[0]->GetMtxWorld()._43);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_SLASH);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_SLASHPAR);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9);
							bDeath = pEnemy->Hit(nDamage);

							// SEを再生
							if (m_bOrbitSlash != true)
							{// 軌跡斬をする状態ではない
								if (m_State != STATE_ORACLEATTACK)
								{// オラクルアタック状態ではない
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT);
								}
								else
								{// オラクルアタックをしていたら

								}
							}
							else
							{// 軌跡斬をする状態なら
								if (m_State != STATE_ATTACK_3)
								{// 最後の攻撃以外だったら
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHHIT);
								}
								else
								{// 最後の攻撃だったら
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK);
								}
							}

							if (CManager::GetMode() == CManager::MODE_GAME)
							{// ゲーム中だったら
								CUI *pUi = CManager::GetGame()->GetUI();
								if (pUi != NULL)
								{// メモリが確保できた
									CCombo *pCombo = CManager::GetGame()->GetUI()->GetCombo();
									if (pCombo != NULL)
									{// メモリが確保されている
										pCombo->SetCombo(pCombo->GetCombo() + 1);
									}
									else if (pCombo == NULL)
									{// メモリが確保されていない
										CManager::GetGame()->GetUI()->CreateCombo();
									}
								}
							}

							if (m_bOrbitSlash != true)
							{// 軌跡斬を出す状態でない
								if (m_State != STATE_ORACLEATTACK)
								{// オラクルアタックで死んでいない
									if (m_fSpecial < m_fMaxSpecial)
									{// 最大値に達していない
										float fAddValue = PLAYER_SPECIALGAUGE_ORBITADD;
										m_fSpecial += fAddValue;
										if (m_fSpecial >= m_fMaxSpecial)
										{// 最大値に達した
											m_fSpecial = m_fMaxSpecial;
											if (m_pSpecialGauge != NULL)
											{// 必殺ゲージが取得できている
												m_pSpecialGauge->MaxGauge();
											}
											// SEを再生
											CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_MAXORACLE);
										}
										else
										{// 最大値に達していない
											if (m_pSpecialGauge != NULL)
											{// 必殺ゲージが取得できている
												m_pSpecialGauge->AddGauge(fAddValue);
											}
										}
									}
								}

							}

							if (bDeath == true)
							{// 死んでいる
								if (CManager::GetMode() == CManager::MODE_GAME)
								{// ゲーム中である
									if (CManager::GetGame()->GetState() == CGame::STATE_KNOCKDOWNBOSS)
									{// ボスを倒した状態だったら
									    // ロックオンのポリゴンを開放する
										if (m_pLockOnPoly != NULL)
										{
											m_pLockOnPoly->Uninit();
											m_pLockOnPoly = NULL;
										}
									}
								}

								if (m_State != STATE_ORACLEATTACK)
								{// オラクルアタックで死んでいない
									if (m_fSpecial < m_fMaxSpecial)
									{// 最大値に達していない
										float fAddValue = PLAYER_SPECIALGAUGE_ADD;
										m_fSpecial += fAddValue;
										if (m_fSpecial >= m_fMaxSpecial)
										{// 最大値に達した
											m_fSpecial = m_fMaxSpecial;
											if (m_pSpecialGauge != NULL)
											{// 必殺ゲージが取得できている
												m_pSpecialGauge->MaxGauge();
											}
											// SEを再生
											CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_MAXORACLE);
										}
										else
										{// 最大値に達していない
											if (m_pSpecialGauge != NULL)
											{// 必殺ゲージが取得できている
												m_pSpecialGauge->AddGauge(fAddValue);
											}
										}
									}
								}
							}

							bHit = true;
						}
						CDebugProc::Print("攻撃がヒットしています\n");
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	return bHit;  // ヒットしたかどうかを返す
}

//=============================================================================
//    エフェクトを作る処理
//=============================================================================
void CPlayer::CreateEffect(void)
{
	if (m_pEffectManager != NULL)
	{// エフェクト管轄クラスへのポインタが入手出来ている
		if (m_State == STATE_MOVE)
		{// 状態が動く状態である
			if (m_bInRiver != true)
			{// 川に入っていない
				if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 0
					|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 2)
				{// エフェクトを作るタイミングになった
					// SEを再生
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
					m_pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_DASH);
				}
			}
		}

		if (m_bOrbitSlash == true)
		{// 軌跡斬をする状態である
			m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 5.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		}

		if (m_fSpecial >= m_fMaxSpecial)
		{// 必殺技ゲージが貯まっている
			m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 5.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 29);
		}
	}
}

//=============================================================================
//    乗っているオブジェクトからのオフセット向きを計算する処理
//=============================================================================
void CPlayer::CalcOffsetRot(void)
{
	if (GetLandScene() != NULL)
	{// プレイヤーが乗っているクラスへのポインタが取得できている
		if (GetLandScene()->GetObjType() == OBJTYPE_OBJECT)
		{// 配置物クラスだったら
			CObject *pObject = (CObject*)GetLandScene();
			D3DXVECTOR3 ObjRot = pObject->GetRot();
			D3DXVECTOR3 OffSetRot;
			OffSetRot = GetRot() - ObjRot;
			if (OffSetRot.y > D3DX_PI)
			{// 円周率を超えた
				OffSetRot.y -= D3DX_PI * 2.0f;
			}
			if (OffSetRot.y < -D3DX_PI)
			{// 円周率を超えた
				OffSetRot.y += D3DX_PI * 2.0f;
			}
			SetLandOffsetRot(OffSetRot);
		}
	}
}

//=============================================================================
//    ダメージを受けたときの処理
//=============================================================================
void CPlayer::Damage(int nDamage)
{
	if (m_bDamage == false)
	{// ダメージをくらっていないなら
		bool bDeath = false;   // 死んだかどうか

		if (m_State != STATE_ORACLEATTACK)
		{// オラクルアタック中でなければ
			// 分身の数だけダメージを大きくする
			if (m_nAvaterNum >= 1)
			{// 分身が1体以上出ている
				nDamage += m_nAvaterNum + (m_nAvaterNum / 2);
			}

			// 体力を減らす
			m_nLife -= nDamage;

			// エフェクトを出す
			if (m_pEffectManager != NULL)
			{
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetModel()[0]->GetMtxWorld()._41, GetModel()[0]->GetMtxWorld()._42, GetModel()[0]->GetMtxWorld()._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10);
			}

			// 体力ゲージを減らす
			if (m_pLifeGauge != NULL)
			{
				m_pLifeGauge->CutGauge(nDamage);
			}

			if (m_nLife <= 0)
			{// 0を下回った
				m_nLife = 0;

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DAMAGE000);

				// プレイヤーを消す
				Uninit();

				if (CManager::GetMode() == CManager::MODE_GAME)
				{// ゲーム中だったら
					CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM002);
					CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM003);
					CGame *pGame = CManager::GetGame();
					if (pGame != NULL)
					{// ゲームクラスが取得できた
						pGame->SetPlayer(NULL);
						if (pGame->GetState() != CGame::STATE_END)
						{// 終了状態でない
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM004);
							pGame->SetState(CGame::STATE_END);
						}
					}
				}
			}
			else
			{// 体力がまだある
				// ダメージ状態に
				m_State = STATE_DAMAGE;

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				// 全てのアクションの状態を戻す
				m_bAction = false;
				m_bBooking = false;
				m_bJump = false;
				m_bOrbit = false;

				// 軌跡を消す
				if (m_pOrbitEffect != NULL)
				{
					m_bOrbit = false;
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
				for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
				{// 分身を出せる数だけ繰り返し
					if (m_pOrbitEffectAvater[nCntAvater] != NULL)
					{
						m_pOrbitEffectAvater[nCntAvater]->Uninit();
						m_pOrbitEffectAvater[nCntAvater] = NULL;
					}
				}

				// SEを再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DAMAGE000);
			}
		}
	}
}

//=============================================================================
//    近くの敵をロックオンする処理
//=============================================================================
void CPlayer::LockOn(void)
{
	if (m_bLockOn == false)
	{// まだロックオンしていない
	    // 判定開始
		float fDistance = 0.0f;         // 敵との距離
		float fDistanceMin = 10000.0f;  // 一番近い敵との距離
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CEnemy *pEnemy = NULL;
		pScene = GetTop(ENEMY_PRIORITY);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// 敵クラスへのポインタだった
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// 敵クラスのポインタが取得できた
					D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
					D3DXVECTOR3 PlayerPos = GetPos();

					// 敵との距離を計算
					fDistance = sqrtf((EnemyPos.x - PlayerPos.x) * (EnemyPos.x - PlayerPos.x) + (EnemyPos.y - PlayerPos.y) * (EnemyPos.y - PlayerPos.y) + (EnemyPos.z - PlayerPos.z) * (EnemyPos.z - PlayerPos.z));

					if (fDistance <= fDistanceMin)
					{// 一番近い敵との距離が更新された
						fDistanceMin = fDistance;
						m_pLockOnEnemy = pEnemy;
					}
				}
			}
			pScene = pSceneNext;
		}

		if (fDistanceMin <= PLAYER_LOCKON_DISTANCE)
		{// 距離がある程度の範囲内だったら
			if (m_pLockOnPoly == NULL && m_pLockOnEnemy != NULL)
			{// メモリが取得できる状態である
				if (m_pLockOnEnemy->GetState() != CEnemy::STATE_DEATH)
				{// 敵が死んでいなかったら
				    // ロックオンを開始する
					m_bLockOn = true;
					D3DXVECTOR3 LocOnPolyPos = D3DXVECTOR3(m_pLockOnEnemy->GetPos().x, m_pLockOnEnemy->GetPos().y + PLAYER_LOCKONPOLY_HEIGHT, m_pLockOnEnemy->GetPos().z);
					m_pLockOnPoly = CSceneBillboard::Create(LocOnPolyPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, PLAYER_LOCKONPOLY_WIDTH, PLAYER_LOCKONPOLY_HEIGHT, false, PLAYER_LOCKON_POLY_PRIORITY);
					if (m_pLockOnPoly != NULL)
					{// メモリが確保できた
						m_pLockOnPoly->BindTexture(GetTextureManager()->GetTexture(2));
					}

					// SEを再生
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_LOCKON);
				}
				else
				{// 敵が死んでいる状態ならば
					m_pLockOnEnemy = NULL;
				}
			}
		}
		else
		{// 距離が離れすぎている
			// ロックオン対象の敵クラスへのポインタを空にしておく
			m_pLockOnEnemy = NULL;
		}
	}
	else if (m_bLockOn == true)
	{// ロックオンしているならば
		LockLost();
	}
}

//=============================================================================
//    ロックオンを解除する処理
//=============================================================================
void CPlayer::LockLost(void)
{
	// ロックオンを解除する
	m_bLockOn = false;

	// ロックオン対象の敵クラスへのポインタを空にしておく
	m_pLockOnEnemy = NULL;

	// ロックオンポリゴンの開放
	if (m_pLockOnPoly != NULL)
	{// メモリが確保されている
		m_pLockOnPoly->Uninit();
		m_pLockOnPoly = NULL;
	}
}

//=============================================================================
//    残像描画用に座標や向きを保存する処理
//=============================================================================
void CPlayer::StackAfterImage(void)
{
	// 配列の最後尾番号を増やす
	m_nCurrentStack++;
	if (m_nCurrentStack >= PLAYER_AFTERIMAGE_NUM)
	{// これ以上スタックできない
		m_nCurrentStack = PLAYER_AFTERIMAGE_NUM - 1;
	}

	// 配列の最後尾に値を保存
	CModel **pModel = GetModel();
	m_AfterPos[m_nCurrentStack] = GetPos();
	m_AfterRot[m_nCurrentStack] = GetRot();
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// モデルのパーツ数分繰り返し
		m_AfterModelPos[m_nCurrentStack][nCntParts] = pModel[nCntParts]->GetAddPos();
		m_AfterModelRot[m_nCurrentStack][nCntParts] = pModel[nCntParts]->GetRot();
	}
}

//=============================================================================
//    残像を描画する処理
//=============================================================================
void CPlayer::DrawAfterImage(void)
{
	// 現在の値を格納しておく
	D3DXVECTOR3 PosDef = GetPos();
	D3DXVECTOR3 RotDef = GetRot();
	D3DXVECTOR3 ModelAddPosDef[PLAYER_MAX_MODELPARTS];
	D3DXVECTOR3 ModelRotDef[PLAYER_MAX_MODELPARTS];
	CModel **pModel = GetModel();
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATERIAL *pMat;
	D3DXCOLOR ModelColorDef[4][PLAYER_MAX_MODELPARTS];
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// パーツ数だけ繰り返し
		ModelAddPosDef[nCntParts] = pModel[nCntParts]->GetAddPos();
		ModelRotDef[nCntParts] = pModel[nCntParts]->GetRot();
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// ポインタが取得できた
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// マテリアル情報の数だけ繰り返し
				ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}
	}

	// 残像を描画する(記憶した分だけ)
	D3DXCOLOR ModelCol = PLAYER_AFTERIMAGE_COLOR;
	D3DXVECTOR3 ModelScale = PLAYER_AFTERIMAGE_SCALE;
	for (int nCntAfter = m_nCurrentStack; nCntAfter >= 0; nCntAfter--)
	{// 残像を記憶した数だけ繰り返し
		SetPos(m_AfterPos[nCntAfter]);
		SetRot(m_AfterRot[nCntAfter]);
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// パーツ数だけ繰り返し
			pModel[nCntParts]->SetScale(ModelScale);
			pModel[nCntParts]->SetAddPos(m_AfterModelPos[nCntAfter][nCntParts]);
			pModel[nCntParts]->SetRot(m_AfterModelRot[nCntAfter][nCntParts]);
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// ポインタが取得できた
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// マテリアル情報の数だけ繰り返し
					pMat[nCntMat].MatD3D.Diffuse = ModelCol;
				}
			}
		}

		// キャラクターの描画処理
		CCharacter::Draw();

		// マテリアルの色を変更する
		ModelCol.r += PLAYER_AFTERIMAGE_REDUP;
		ModelCol.g +=PLAYER_AFTERIMAGE_GREENUP;
		ModelCol.a -= PLAYER_AFTERIMAGE_ALPHACUT;

		// 大きさを変更する
		ModelScale.x -= PLAYER_AFTERIMAGE_SCALECUT;
		ModelScale.y -= PLAYER_AFTERIMAGE_SCALECUT;
		ModelScale.z -= PLAYER_AFTERIMAGE_SCALECUT;
	}

	// 現在の値に戻す
	SetPos(PosDef);
	SetRot(RotDef);
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// パーツ数だけ繰り返し
		pModel[nCntParts]->SetAddPos(ModelAddPosDef[nCntParts]);
		pModel[nCntParts]->SetRot(ModelRotDef[nCntParts]);
		pModel[nCntParts]->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// ポインタが取得できた
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// マテリアル情報の数だけ繰り返し
				pMat[nCntMat].MatD3D.Diffuse = ModelColorDef[nCntMat][nCntParts];
			}
		}
	}
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
void CPlayer::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    移動量を設定する処理
//=============================================================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    体力を設定する処理
//=============================================================================
void CPlayer::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    加速度を設定する処理
//=============================================================================
void CPlayer::SetAccel(const float fAccel)
{
	m_fAccel = fAccel;
}

//=============================================================================
//    慣性を設定する処理
//=============================================================================
void CPlayer::SetInertia(const float fInertia)
{
	m_fInertia = fInertia;
}

//=============================================================================
//    ジャンプ量を設定する処理
//=============================================================================
void CPlayer::SetJumpPower(const float fJumpPower)
{
	m_fJumpPower = fJumpPower;
}

//=============================================================================
//    向きを補正する倍率を設定する処理
//=============================================================================
void CPlayer::SetRivisionRot(const float fRivisionRot)
{
	m_fRivisionRot = fRivisionRot;
}

//=============================================================================
//    現在の軌跡ゲージ量を設定する処理
//=============================================================================
void CPlayer::SetOrbitSlash(const float fOrbitSlash)
{
	m_fOrbitSlash = fOrbitSlash;
}

//=============================================================================
//    現在の必殺技ゲージ量を設定する処理
//=============================================================================
void CPlayer::SetSpecial(const float fSpecial)
{
	m_fSpecial = fSpecial;
}

//=============================================================================
//    ジャンプしているかどうかを設定する処理
//=============================================================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//=============================================================================
//    アクションしているかどうかを設定する処理
//=============================================================================
void CPlayer::SetAction(const bool bAction)
{
	m_bAction = bAction;
}

//=============================================================================
//    ロックオンしているかどうかを設定する処理
//=============================================================================
void CPlayer::SetLockOn(const bool bLockOn)
{
	m_bLockOn = bLockOn;
}

//=============================================================================
//    ダメージを受けているかどうか設定する処理
//=============================================================================
void CPlayer::SetDamage(const bool bDamage)
{
	m_bDamage = bDamage;
}

//=============================================================================
//    移動量を取得する処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    体力を取得する処理
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
//    加速度を取得する処理
//=============================================================================
float CPlayer::GetAccel(void)
{
	return m_fAccel;
}

//=============================================================================
//    慣性を取得する処理
//=============================================================================
float CPlayer::GetInertia(void)
{
	return m_fInertia;
}

//=============================================================================
//    ジャンプ量を取得する処理
//=============================================================================
float CPlayer::GetJumpPower(void)
{
	return m_fJumpPower;
}

//=============================================================================
//    向きを補正する倍率を取得する処理
//=============================================================================
float CPlayer::GetRivisionRot(void)
{
	return m_fRivisionRot;
}

//=============================================================================
//    現在の軌跡ゲージ量を取得する処理
//=============================================================================
float CPlayer::GetOrbitSlash(void)
{
	return m_fOrbitSlash;
}

//=============================================================================
//    現在の必殺技ゲージ量を取得する処理
//=============================================================================
float CPlayer::GetSpecial(void)
{
	return m_fSpecial;
}

//=============================================================================
//    ジャンプしているかどうかを取得する処理
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    アクションしているかどうかを取得する処理
//=============================================================================
bool CPlayer::GetAction(void)
{
	return m_bAction;
}
//=============================================================================
//    ロックオンしているかどうかを取得する処理
//=============================================================================
bool CPlayer::GetLockOn(void)
{
	return m_bLockOn;
}

//=============================================================================
//    ロックオン対象の敵クラスへのポインタを取得する処理
//=============================================================================
CEnemy *CPlayer::GetLockOnEnemy(void)
{
	return m_pLockOnEnemy;
}

//=============================================================================
//    ダメージを受けているかどうかを取得する処理
//=============================================================================
bool CPlayer::GetDamage(void)
{
	return m_bDamage;
}