//*****************************************************************************
//
//     マップの処理[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "map.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
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
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
//#define MAP_AUTOSAVE                                                           // 宣言時 : オートセーブ

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
	m_nCntLoadEnemy = 0;       // 敵データを読み込んだ回数
	m_PlayerDefaultPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PlayerDefaultRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	// スクリプトを読み込む
	ScriptLoad_Open();
}

//=============================================================================
//    終了処理
//=============================================================================
void CMap::Uninit(void)
{
#ifdef MAP_AUTOSAVE
	// 配置情報を保存する
	Save();
#endif

	// マップ管轄クラスを開放する
	ReleaseModelManager();

	// テクスチャ管轄クラスを開放する
	ReleaseTextureManager();

	// 敵の管轄まとめクラスのメモリを開放する
	ReleaseEnemyAssembley();

	// エフェクト管轄クラスへのポインタを空にする
	m_pEffectManager = NULL;

	// マップの要素であるクラスを削除する
	DeleteMapObject();
}

//=============================================================================
//    スクリプト読み込みのためのファイルオープン処理
//=============================================================================
void CMap::ScriptLoad_Open(void)
{
	char aStrCur[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		strcpy(aStrCur, pFileLoader->GetString(aStrCur));
		if (CFunctionLib::Memcmp(aStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			ScriptLoad(pFileLoader, aStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    スクリプト読み込み処理
//=============================================================================
void CMap::ScriptLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	int nCntTex = 0;    // テクスチャを読み込んだ回数
	int nCntModel = 0;  // モデルを読み込んだ回数

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
		{// テクスチャの個数情報があった
			CreateTextureManager(CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
		{// テクスチャのファイルパス名情報があった
			TextureLoad(pStrCur, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
		{// テクスチャの個数情報があった
			CreateModelManager(CFunctionLib::ReadInt(pStrCur, NUM_MODEL));
		}
		else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
		{// モデルのファイルパス名情報があった
			ModelLoad(pStrCur, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, FIELD_FILENAME) == 0)
		{// 地面のスクリプトファイル名があった
			FieldLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, FIELD_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RIVER_FILENAME) == 0)
		{// 川のスクリプトファイル名があった
			RiverLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, RIVER_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, OBJECT_FILENAME) == 0)
		{// 配置物のスクリプトファイル名があった
			if (MAP_SET_OBJECT)
			{// モデルを配置するなら
				ObjectLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, OBJECT_FILENAME), pStrCur);
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, MOUNTAIN_FILENAME) == 0)
		{// 山のスクリプトファイル名があった
			MountainLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, MOUNTAIN_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, SKY_FILENAME) == 0)
		{// 空のスクリプトファイル名があった
			SkyLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, SKY_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ENEMY_FILENAME) == 0)
		{// 敵のスクリプトファイル名があった
			if (CManager::GetMode() == CManager::MODE_GAME && MAP_SET_ENEMY
				|| CManager::GetMode() == CManager::MODE_TUTORIAL && MAP_SET_ENEMY)
			{// チュートリアル画面かゲーム画面かつ敵を配置するなら
				EnemyLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, ENEMY_FILENAME), pStrCur);
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, BILLBOARD_FILENAME) == 0)
		{// 配置用ビルボードのスクリプトファイル名があった
			BillboardLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, BILLBOARD_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
		{// 配置エフェクトのスクリプトファイル名があった
			if (MAP_SET_EFFECT)
			{// エフェクトを配置するなら
				EffectLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, EFFECT_FILENAME), pStrCur);
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, WALL_FILENAME) == 0)
		{// 壁のスクリプトファイル名があった
			WallLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, WALL_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ITEM_FILENAME) == 0)
		{// アイテムのスクリプトファイル名があった
			ItemLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, ITEM_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PLAYER_DEFAULT_SPAWN) == 0)
		{// プレイヤーのデフォルトスポーン位置があった
			PlayerDefaultSpawnLoad(pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, CAMERA_DEFAULT_ROT) == 0)
		{// カメラのデフォルト向きがあった
			CameraDefaultRotLoad(pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    テクスチャ管轄クラスを生成する処理
//=============================================================================
void CMap::CreateTextureManager(int nNumTexture)
{
	if (nNumTexture >= 1)
	{// モデル数が1つ以上ある
		m_pTextureManager = CTextureManager::Create(nNumTexture);
	}
}

//=============================================================================
//    テクスチャ情報読み込み処理
//=============================================================================
void CMap::TextureLoad(char *pStrCur, int nCntTex)
{
	char aFileName[256] = "\0";
	LPDIRECT3DTEXTURE9 pTexture = NULL;  // テクスチャ読み取り用

	// テクスチャのファイルパス名を読み取る
	strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, TEXTURE_FILENAME));

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aFileName, &pTexture);
	if (pTexture != NULL)
	{// テクスチャを読み込めた
		m_pTextureManager->BindTexture(pTexture, nCntTex);
		m_pTextureManager->SetFileName(aFileName, nCntTex);
		pTexture = NULL;
	}
}

//=============================================================================
//    モデル管轄クラスを生成する処理
//=============================================================================
void CMap::CreateModelManager(int nNumModel)
{
	if (nNumModel >= 1)
	{// モデル数が1つ以上ある
		m_pModelManager = CModelManager::Create(nNumModel);
	}
}

//=============================================================================
//    モデル情報読み込み処理
//=============================================================================
void CMap::ModelLoad(char *pStrCur, int nCntModel)
{
	char aFileName[256] = "\0";
	LPD3DXMESH pMesh = NULL;         // メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;    // マテリアル情報へのポインタ
	DWORD nNumMat = 0;               // マテリアル情報の数

	// モデルのファイルパス名を読み取る
	strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, MODEL_FILENAME));

	// モデルの読み込み
	// xファイルの読み込み
	D3DXLoadMeshFromX(aFileName,
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
}

//=============================================================================
//    プレイヤーのスポーン位置読み込み処理
//=============================================================================
void CMap::PlayerDefaultSpawnLoad(char *pStrCur)
{
	m_PlayerDefaultPos = CFunctionLib::ReadVector3(pStrCur, PLAYER_DEFAULT_SPAWN);
}

//=============================================================================
//    カメラの向き初期情報読み込み処理
//=============================================================================
void CMap::CameraDefaultRotLoad(char *pStrCur)
{
	D3DXVECTOR3 DefaultCameraRot = CFunctionLib::ReadVector3(pStrCur, CAMERA_DEFAULT_ROT);

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
		DefaultCameraRot.z -= D3DX_PI * 2.0f;
	}

	CManager::GetCamera()->SetRot(DefaultCameraRot);
	CManager::GetCamera()->SetRotDest(DefaultCameraRot);
	CManager::GetCamera()->SetRotDiff(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//    地面読み込みのためのファイルオープン処理
//=============================================================================
void CMap::FieldLoad_Open(char *pFieldFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pFieldFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			FieldLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    地面読み込み処理
//=============================================================================
void CMap::FieldLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, FIELDSET) == 0)
		{// 地面情報読み込み開始の合図だった
			FieldLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    地面情報読み込み処理
//=============================================================================
void CMap::FieldLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
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

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
			strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_FIELDSET) == 0)
		{// 地面情報読み込み終了の合図だった
		    // 地面を生成する
			CMeshField *pField = CMeshField::Create(FieldPos, FieldRot, FieldCol, fFieldWidth, fFieldDepth, nFieldXblock, nFieldZblock, nFieldTexSplitU, nFieldTexSplitV, aFileName, bFieldCulling, FIELD_PRIORITY);
			if (pField != NULL)
			{// 地面が生成できた
				pField->BindTexture(m_pTextureManager->GetTexture(nFieldTexIdx));
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    川読み込みのためのファイルオープン処理
//=============================================================================
void CMap::RiverLoad_Open(char *pRiverFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pRiverFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			RiverLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    川読み込み処理
//=============================================================================
void CMap::RiverLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, RIVERSET) == 0)
		{// 川情報読み込み開始の合図だった
			RiverLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    川情報読み込み処理
//=============================================================================
void CMap::RiverLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// 川用の格納変数
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

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
			strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_RIVERSET) == 0)
		{// 川情報読み込み終了の合図だった
		    // 川を生成する
			CRiver *pRiver = CRiver::Create(RiverPos, RiverRot, RiverCol, fRiverWidth, fRiverDepth, nRiverXblock, nRiverZblock, nRiverTexSplitU, nRiverTexSplitV, fRiverAddTexU, fRiverAddTexV, aFileName, RIVER_PRIORITY);
			if (pRiver != NULL)
			{// 地面が生成できた
				pRiver->BindTexture(m_pTextureManager->GetTexture(nRiverTexIdx));
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    配置物読み込みのためのファイルオープン処理
//=============================================================================
void CMap::ObjectLoad_Open(char *pObjectFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pObjectFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			ObjectLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    配置物読み込み処理
//=============================================================================
void CMap::ObjectLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, OBJECTSET) == 0)
		{// 配置物情報読み込み開始の合図だった
			ObjectLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, OBJECTMOVESET) == 0)
		{// 動く配置物情報読み込み開始の合図だった
			ObjectMoveLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, OBJECTGOALSET) == 0)
		{// ゴール用配置物情報読み込み開始の合図だった
			ObjectGoalLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    配置物情報読み込み処理
//=============================================================================
void CMap::ObjectLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// 配置物用の格納変数
	int nObjectType = 0;                                            // 配置物のモデル番号
	int nObjectColType = 0;                                         // 配置物の当たり判定の種類
	D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の座標
	D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の向き
	D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 配置物の大きさの倍率
	D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 配置物の回転スピード
	float fObjectColRange = 0.0f;                                   // 配置物の当たり判定を取る範囲(円形に当たり判定する際に使用する)
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
			CObject *pObject = CObject::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, (CObject::COL_TYPE)nObjectColType, fObjectColRange, OBJECT_PRIORITY);
			if (pObject != NULL)
			{
				pObject->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
				pObject->SetModelIdx(nObjectType);
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    動く配置物情報読み込み処理
//=============================================================================
void CMap::ObjectMoveLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// 動く配置物用の格納変数
	int nObjectType = 0;                                            // 配置物のモデル番号
	int nObjectColType = 0;                                         // 配置物の当たり判定の種類
	D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の座標
	D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の向き
	D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 配置物の大きさの倍率
	D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 配置物の回転スピード
	float fObjectColRange = 0.0f;                                   // 配置物の当たり判定を取る範囲(円形に当たり判定する際に使用する)
	D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 配置物の移動量
	int nObjectMaxCounter = 0;                                      // 配置物の移動量を切り替える時間

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
			CObjectMove *pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, ObjectMove, nObjectMaxCounter, (CObject::COL_TYPE)nObjectColType, fObjectColRange, OBJECT_PRIORITY);
			if (pObjectMove != NULL)
			{
				pObjectMove->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
				pObjectMove->SetModelIdx(nObjectType);
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    ゴール用配置物情報読み込み処理
//=============================================================================
void CMap::ObjectGoalLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// ゴール配置物用の格納変数
	int nObjectType = 0;                                            // 配置物のモデル番号
	int nObjectColType = 0;                                         // 配置物の当たり判定の種類
	D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の座標
	D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の向き
	D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 配置物の大きさの倍率
	D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 配置物の回転スピード
	float fObjectColRange = 0.0f;                                   // 配置物の当たり判定を取る範囲(円形に当たり判定する際に使用する)

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
		else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTGOALSET) == 0)
		{// ゴール配置物情報読み込み終了の合図だった
		    // ゴール配置物を生成する
			CObjectGoal *pObjectGoal = CObjectGoal::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, OBJECT_PRIORITY);
			if (pObjectGoal != NULL)
			{
				pObjectGoal->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
				pObjectGoal->SetModelIdx(nObjectType);
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    山読み込みのためのファイルオープン処理
//=============================================================================
void CMap::MountainLoad_Open(char *pMountainFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pMountainFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			MountainLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    山読み込み処理
//=============================================================================
void CMap::MountainLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, MOUNTAINSET) == 0)
		{// 山情報読み込み開始の合図だった
			MountainLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    配置物情報読み込み処理
//=============================================================================
void CMap::MountainLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// 山用の格納変数
	int nMountainTexIdx = 0;                                    // 山のテクスチャ番号
	D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 山の座標
	D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 山の向き
	D3DXCOLOR MountainCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 山の色
	float fMountainRadius = 0.0f;                               // 山の半径
	float fMountainHeight = 0.0f;                               // 山の高さ
	int nMountainXblock = 0;                                    // 山の横のブロック数
	int nMountainYblock = 0;                                    // 山の縦のブロック数

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
		    // 山を生成する
			CMountain *pMountain = CMountain::Create(MountainPos, MountainRot, MountainCol, fMountainHeight, fMountainRadius, nMountainXblock, nMountainYblock, MOUNTAIN_PRIORITY);
			if (pMountain != NULL)
			{// 山が生成できた
				pMountain->BindTexture(m_pTextureManager->GetTexture(nMountainTexIdx));
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    空読み込みのためのファイルオープン処理
//=============================================================================
void CMap::SkyLoad_Open(char *pSkyFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pSkyFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			SkyLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    空読み込み処理
//=============================================================================
void CMap::SkyLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SKYSET) == 0)
		{// 空情報読み込み開始の合図だった
			SkyLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    空情報読み込み処理
//=============================================================================
void CMap::SkyLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// 空用の格納変数
	int nSkyTexIdx = 0;                                    // 空のテクスチャ番号
	D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 空の座標
	D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 空の向き
	D3DXCOLOR SkyCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 空の色
	float fSkyRadius = 0.0f;                               // 空の半径
	int nSkyXblock = 0;                                    // 空の横のブロック数
	int nSkyYblock = 0;                                    // 空の縦のブロック数
	float fRotSpeed = 0.0f;                                // 空の回転するスピード

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
		    // 空を生成する
			CSky *pSky = CSky::Create(SkyPos, SkyRot, SkyCol, fSkyRadius, nSkyXblock, nSkyYblock, fRotSpeed, SKY_PRIORITY);
			if (pSky != NULL)
			{// 空が生成できた
				pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    敵読み込みのためのファイルオープン処理
//=============================================================================
void CMap::EnemyLoad_Open(char *pEnemyFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pEnemyFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			EnemyLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    敵読み込み処理
//=============================================================================
void CMap::EnemyLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	int nCntEnemyLoad = 0;                              // エネミーデータを読み込んだ回数
	char aEnemyDataFileName[256] = "\0";                // エネミーデータのスクリプトファイル名
	CEnemyManager *apEnemyManager[MAX_ENEMY_TYPE] = {}; // 敵管轄クラスへのポインタ

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, ENEMYDATA_FILENAME) == 0)
		{// 敵のデータ情報読み込み開始の合図だった
			strcpy(aEnemyDataFileName, CFunctionLib::ReadString(pStrCur, aEnemyDataFileName, ENEMYDATA_FILENAME));
			apEnemyManager[nCntEnemyLoad] = EnemyLoad_Manager(pFileLoader, aEnemyDataFileName, nCntEnemyLoad);
			nCntEnemyLoad++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, ENEMYSET) == 0)
		{// 敵情報読み込み開始の合図だった
			EnemyLoad_Info(pFileLoader, pStrCur, apEnemyManager);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			SetEnemyManagerToAssembley(apEnemyManager, nCntEnemyLoad);
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    敵管轄クラス情報を読み込む処理
//=============================================================================
CEnemyManager *CMap::EnemyLoad_Manager(CFileLoader *pFileLoader, char *pFileName, int nCntEnemyLoad)
{
	// 敵管轄クラスのポインタを生成
	CEnemyManager *pEnemyManager = CEnemyManager::Create(nCntEnemyLoad, pFileName);
	return pEnemyManager;
}

//=============================================================================
//    敵情報読み込み処理
//=============================================================================
void CMap::EnemyLoad_Info(CFileLoader *pFileLoader, char *pStrCur, CEnemyManager **apEnemyManager)
{
	// 敵情報格納用変数
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
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, AISET) == 0)
		{// AIの情報読み込み開始の合図だった
			EnemyLoad_Info_AI(pFileLoader, pStrCur, &fEnemyAI_CheackDistance, &fEnemyAI_AttackDistance, &nEnemyAI_AttackTiming);
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

//=============================================================================
//    敵のAI情報読み込み処理
//=============================================================================
void CMap::EnemyLoad_Info_AI(CFileLoader *pFileLoader, char *pStrCur, float *pCheackDistance, float *pAttackDistance, int *pAttackTiming)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, CHEACKDISTANCE) == 0)
		{// 警戒できる範囲情報がある
			*pCheackDistance = CFunctionLib::ReadFloat(pStrCur, CHEACKDISTANCE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKDISTANCE) == 0)
		{// 攻撃を仕掛ける範囲情報がある
			*pAttackDistance = CFunctionLib::ReadFloat(pStrCur, ATTACKDISTANCE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKRANDOM) == 0)
		{// 攻撃を仕掛けるタイミング情報がある
			*pAttackTiming = CFunctionLib::ReadInt(pStrCur, ATTACKRANDOM);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_AISET) == 0)
		{// AIの情報読み込み終了の合図だった
			break;
		}
	}
}

//=============================================================================
//    敵管轄まとめクラスへ敵管轄クラスへのポインタを設定する処理
//=============================================================================
void CMap::SetEnemyManagerToAssembley(CEnemyManager **apEnemyManager, int nCntEnemyLoad)
{
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
}

//=============================================================================
//    配置用ビルボード読み込みのためのファイルオープン処理
//=============================================================================
void CMap::BillboardLoad_Open(char *pBillboardFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pBillboardFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			BillboardLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    配置用ビルボード読み込み処理
//=============================================================================
void CMap::BillboardLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, BILLBOARDSET) == 0)
		{// 配置用ビルボード情報読み込み開始の合図だった
			BillboardLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    配置用ビルボード情報読み込み処理
//=============================================================================
void CMap::BillboardLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// 配置用ビルボード情報格納用変数
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
		pStrCur = pFileLoader->GetString(pStrCur);
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

//=============================================================================
//    配置用エフェクト読み込みのためのファイルオープン処理
//=============================================================================
void CMap::EffectLoad_Open(char *pEffectFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pEffectFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			EffectLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    配置用エフェクト読み込み処理
//=============================================================================
void CMap::EffectLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	// エフェクト管轄クラスへのポインタを取得
	SearchEffectManager();
	if (m_pEffectManager == NULL) return;

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, EFFECTSET) == 0)
		{// 配置用エフェクト情報読み込み開始の合図だった
			EffectLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを取得する
//=============================================================================
void CMap::SearchEffectManager(void)
{
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
				m_pEffectManager = (CEffectManager*)pScene;
				break;
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置用エフェクト情報読み込み処理
//=============================================================================
void CMap::EffectLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	//　配置用エフェクト情報格納用変数
	int nEffectType = 0;
	D3DXVECTOR3 EffectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 EffectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
			m_pEffectManager->SetEffect(EffectPos, EffectRot, nEffectType, EFFECT_PRIORITY);
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    壁読み込みのためのファイルオープン処理
//=============================================================================
void CMap::WallLoad_Open(char *pWallFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pWallFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			WallLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    壁読み込み処理
//=============================================================================
void CMap::WallLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, WALLSET) == 0)
		{// 壁情報読み込み開始の合図だった
			WallLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    壁情報読み込み処理
//=============================================================================
void CMap::WallLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// 壁情報格納用変数
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

	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
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
			CMeshWall *pMeshWall = CMeshWall::Create(WallPos, WallRot, WallCol, fWallWidth, fWallHeight, nWallXblock, nWallYblock, nWallTexSplitU, nWallTexSplitV, nWallTexIdx, WALL_PRIORITY);
			if (pMeshWall != NULL)
			{// 壁が生成できた
				pMeshWall->BindTexture(m_pTextureManager->GetTexture(nWallTexIdx));
			}
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    アイテム読み込みのためのファイルオープン処理
//=============================================================================
void CMap::ItemLoad_Open(char *pItemFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pItemFileName);
	if (pFileLoader != NULL)
	{// ファイルオープンに成功
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図だった
			ItemLoad(pFileLoader, pStrCur);
		}

		// ファイルクローズ
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    アイテム読み込み処理
//=============================================================================
void CMap::ItemLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ループ開始
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, ITEMSET) == 0)
		{// アイテム情報読み込み開始の合図だった
			ItemLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    アイテム情報読み込み処理
//=============================================================================
void CMap::ItemLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// アイテム情報格納用変数
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
		pStrCur = pFileLoader->GetString(pStrCur);
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
			SetItem(ItemPos, ItemCol, fItemWidth, fItemHeight, fItemRot, bItemLighting, bItemDrawAddtive,
				nItemTexIdx, fItemColRange, nItemType);
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    各種配置情報を保存する処理
//=============================================================================
void CMap::Save(void)
{
	// 配置物の配置情報
	SaveObject_Open();

	// 敵の配置情報
	SaveEnemy_Open();

	// ビルボードの配置情報
	SaveBillboard_Open();

	// エフェクトの配置情報
	SaveEffect_Open();

	// 壁の配置情報
	SaveWall_Open();

	// アイテムの配置情報
	SaveItem_Open();
}

//=============================================================================
//    配置物を保存するためのファイルオープン処理
//=============================================================================
void CMap::SaveObject_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_OBJECT_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// ファイルオープンに成功
	    // 冒頭部分を書き込み
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       配置物情報スクリプトファイル[save_object.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n\n", SCRIPT);

		// 配置物を保存する
		SaveObject(pFileSaver);

		// スクリプト読み込み終了の合図を書き込み
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

		// ファイルクローズ
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    配置物保存処理
//=============================================================================
void CMap::SaveObject(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  モデル情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// 配置物クラスへのポインタだった
				SaveObject_Info(pFileSaver, (CObject*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
			{// 動く配置物クラスへのポインタだった
				SaveObjectMove_Info(pFileSaver, (CObjectMove*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
			{// ゴール用配置物クラスへのポインタだった
				SaveObjectGoal_Info(pFileSaver, (CObjectGoal*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置物情報保存処理
//=============================================================================
void CMap::SaveObject_Info(CFileSaver *pFileSaver, CObject *pObject)
{
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
	pFileSaver->Print("%s\n", OBJECTSET);
	pFileSaver->Print("	%s%d						# 使用するモデルの番号\n", TYPE, pObject->GetModelIdx());
	pFileSaver->Print("	%s%d					# 当たり判定の種類\n", COLLISION, pObject->GetCollision());
	pFileSaver->Print("	%s%.1f					# 当たり判定を取る範囲\n", COL_RANGE, pObject->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f			# モデルの座標\n", POS, pObject->GetPos().x, pObject->GetPos().y, pObject->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# モデルの向き\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# モデルの大きさ\n", SCALE, pObject->GetScale().x, pObject->GetScale().y, pObject->GetScale().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# モデルの回転するスピード\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
	pFileSaver->Print("%s\n", END_OBJECTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    動く配置物情報保存処理
//=============================================================================
void CMap::SaveObjectMove_Info(CFileSaver *pFileSaver, CObjectMove *pObjectMove)
{
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

	// 動く配置物の情報を書き込み
	pFileSaver->Print("%s\n", OBJECTMOVESET);
	pFileSaver->Print("	%s%d						# 使用するモデルの番号\n", TYPE, pObjectMove->GetModelIdx());
	pFileSaver->Print("	%s%d					# 当たり判定の種類\n", COLLISION, pObjectMove->GetCollision());
	pFileSaver->Print("	%s%.1f					# 当たり判定を取る範囲\n", COL_RANGE, pObjectMove->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f			# モデルの座標\n", POS, pObjectMove->GetPosStd().x, pObjectMove->GetPosStd().y, pObjectMove->GetPosStd().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# モデルの向き\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# モデルの大きさ\n", SCALE, pObjectMove->GetScale().x, pObjectMove->GetScale().y, pObjectMove->GetScale().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# モデルの回転するスピード\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# モデルの移動量\n", MOVE, pObjectMove->GetMove().x, pObjectMove->GetMove().y, pObjectMove->GetMove().z);
	pFileSaver->Print("	%s%d					# 移動量を切り替える時間\n", MOVE_MAX, pObjectMove->GetMaxMoveCounter());
	pFileSaver->Print("%s\n", END_OBJECTMOVESET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    ゴール用配置物情報保存処理
//=============================================================================
void CMap::SaveObjectGoal_Info(CFileSaver *pFileSaver, CObjectGoal *pObjectGoal)
{
	// 事前に計算をすませておく
	D3DXVECTOR3 ModelRot = pObjectGoal->GetRotStd();
	D3DXVECTOR3 ModelRollSpeed = pObjectGoal->GetRollSpeed();
	// 向き
	ModelRot.x = D3DXToDegree(ModelRot.x);
	ModelRot.y = D3DXToDegree(ModelRot.y);
	ModelRot.z = D3DXToDegree(ModelRot.z);

	// 回転するスピード
	ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
	ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
	ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

	// ゴール用配置物の情報を書き込み
	pFileSaver->Print("%s\n", OBJECTGOALSET);
	pFileSaver->Print("	%s%d						# 使用するモデルの番号\n", TYPE, pObjectGoal->GetModelIdx());
	pFileSaver->Print("	%s%d					# 当たり判定の種類\n", COLLISION, pObjectGoal->GetCollision());
	pFileSaver->Print("	%s%.1f					# 当たり判定を取る範囲\n", COL_RANGE, pObjectGoal->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f			# モデルの座標\n", POS, pObjectGoal->GetPos().x, pObjectGoal->GetPos().y, pObjectGoal->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# モデルの向き\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# モデルの大きさ\n", SCALE, pObjectGoal->GetScale().x, pObjectGoal->GetScale().y, pObjectGoal->GetScale().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# モデルの回転するスピード\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
	pFileSaver->Print("%s\n", END_OBJECTGOALSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    敵情報保存のためのファイルオープン処理
//=============================================================================
void CMap::SaveEnemy_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_ENEMY_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// ファイルオープンに成功
	    // 冒頭部分を書き込み
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       敵の配置情報スクリプトファイル[save_set_enemy.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n\n", SCRIPT);

		// 敵管轄情報保存処理
		SaveEnemy_Manager(pFileSaver);

		// 敵保存処理
		SaveEnemy(pFileSaver);

		// スクリプト読み込み終了の合図を書き込み
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

		// ファイルクローズ
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    敵管轄情報保存処理
//=============================================================================
void CMap::SaveEnemy_Manager(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  使用する敵のデータ\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_TYPE; nCntEnemy++)
	{// 敵のデータを読み込める回数分繰り返し
		if (m_pEnemyAssembly != NULL)
		{
			CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nCntEnemy);
			if (pEnemyManager != NULL)
			{
				pFileSaver->Print("%s%s		# 敵%d体目\n", ENEMYDATA_FILENAME, pEnemyManager->GetFileName(), nCntEnemy + 1);
			}
		}
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    敵保存処理
//=============================================================================
void CMap::SaveEnemy(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  敵の配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 敵の配置情報を保存
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// 敵クラスへのポインタだった
				SaveEnemy_Info(pFileSaver, (CEnemy*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    敵情報保存処理
//=============================================================================
void CMap::SaveEnemy_Info(CFileSaver *pFileSaver, CEnemy *pEnemy)
{
	// 事前に計算をすませておく
	D3DXVECTOR3 EnemyRot = pEnemy->GetRotStd();
	// 向き
	EnemyRot.x = D3DXToDegree(EnemyRot.x);
	EnemyRot.y = D3DXToDegree(EnemyRot.y);
	EnemyRot.z = D3DXToDegree(EnemyRot.z);

	// 敵の配置情報を書き込み
	pFileSaver->Print("%s\n", ENEMYSET);
	pFileSaver->Print("	%s						# AIの情報\n", AISET);
	pFileSaver->Print("		%s%f	# 警戒できる範囲", CHEACKDISTANCE, pEnemy->GetAI_CheackDistance());
	pFileSaver->Print("		%s%f	# 攻撃を仕掛ける範囲", ATTACKDISTANCE, pEnemy->GetAI_AttackDistance());
	pFileSaver->Print("		%s%d		# 攻撃を仕掛けるタイミング", ATTACKRANDOM, pEnemy->GetAI_AttackTiming());
	pFileSaver->Print("	%s\n\n", END_AISET);
	pFileSaver->Print("	%s%d					# 種類\n", TYPE, pEnemy->GetType());
	pFileSaver->Print("	%s%d					# 体力\n", LIFE, pEnemy->GetLife());
	pFileSaver->Print("	%s%d				# 倒した時に増えるスコア量\n", SCORE, pEnemy->GetAddScore());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, pEnemy->GetPosStd().x, pEnemy->GetPosStd().y, pEnemy->GetPosStd().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f			# 向き\n", ROT, EnemyRot.x, EnemyRot.y, EnemyRot.z);
	pFileSaver->Print("%s\n", END_ENEMYSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    配置用ビルボード情報保存のためのファイルオープン処理
//=============================================================================
void CMap::SaveBillboard_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_BILLBOARD_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// ファイルオープンに成功
	    // 冒頭部分を書き込み
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       配置用ビルボード情報スクリプトファイル[save_billboard.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n\n", SCRIPT);

		// 配置用ビルボードを保存する
		SaveBillboard(pFileSaver);

		// スクリプト読み込み終了の合図を書き込み
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

		// ファイルクローズ
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    配置用ビルボード保存処理
//=============================================================================
void CMap::SaveBillboard(CFileSaver *pFileSaver)
{
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
				SaveBillboard_Info(pFileSaver, (CBillboardObject*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置用ビルボード情報保存処理
//=============================================================================
void CMap::SaveBillboard_Info(CFileSaver *pFileSaver, CBillboardObject *pBillObj)
{
	// 事前に計算をすませておく
	float fBillboardRot = pBillObj->GetRot();
	// 向き
	fBillboardRot = D3DXToDegree(fBillboardRot);

	// 配置物の情報を書き込み
	pFileSaver->Print("%s\n", BILLBOARDSET);
	pFileSaver->Print("	%s%d				# 種類番号\n", TYPE, pBillObj->GetType());
	pFileSaver->Print("	%s%d				# 使用するテクスチャの番号\n", TEX_TYPE, pBillObj->GetTexIdx());
	pFileSaver->Print("	%s%.1f				# 当たり判定を取る範囲\n", COL_RANGE, pBillObj->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, pBillObj->GetPos().x, pBillObj->GetPos().y, pBillObj->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# 色\n", COL, pBillObj->GetCol().r, pBillObj->GetCol().g, pBillObj->GetCol().b, pBillObj->GetCol().a);
	pFileSaver->Print("	%s%.1f					# 向き\n", ROT, fBillboardRot);
	pFileSaver->Print("	%s%.1f				# 幅\n", WIDTH, pBillObj->GetWidth());
	pFileSaver->Print("	%s%.1f				# 高さ\n", HEIGHT, pBillObj->GetHeight());
	pFileSaver->Print("	%s%d				# ライティングするかしないか\n", LIGHTING, pBillObj->GetLighting());
	pFileSaver->Print("	%s%d			# 加算合成で描画するかしないか\n", DRAW_ADDTIVE, pBillObj->GetDrawAddtive());
	pFileSaver->Print("%s\n", END_BILLBOARDSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    配置用エフェクト情報保存のためのファイルオープン処理
//=============================================================================
void CMap::SaveEffect_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_EFFECT_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// ファイルオープンに成功
	    // 冒頭部分を書き込み
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       配置エフェクト情報スクリプトファイル[save_effect.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n\n", SCRIPT);

		// 配置用エフェクトを保存する
		SaveEffect(pFileSaver);

		// スクリプト読み込み終了の合図を書き込み
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

		// ファイルクローズ
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    配置用エフェクト保存処理
//=============================================================================
void CMap::SaveEffect(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  配置エフェクト情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
			{// エミッタクラスへのポインタだった
				SaveEffect_Info(pFileSaver, (CEmitter*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置用エフェクト情報保存処理
//=============================================================================
void CMap::SaveEffect_Info(CFileSaver *pFileSaver, CEmitter *pEmitter)
{
	// 事前に計算をすませておく
	D3DXVECTOR3 EffectRot = pEmitter->GetRot();
	// 向き
	EffectRot.x = D3DXToDegree(EffectRot.x);
	EffectRot.y = D3DXToDegree(EffectRot.y);
	EffectRot.z = D3DXToDegree(EffectRot.z);

	// 配置エフェクトの情報を書き込み
	pFileSaver->Print("%s\n", EFFECTSET);
	pFileSaver->Print("	%s%d				# 使用するエフェクトの種類番号\n", TYPE, pEmitter->GetType());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, pEmitter->GetPos().x, pEmitter->GetPos().y, pEmitter->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 向き\n", ROT, EffectRot.x, EffectRot.y, EffectRot.z);
	pFileSaver->Print("%s\n", END_EFFECTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    壁情報保存のためのファイルオープン処理
//=============================================================================
void CMap::SaveWall_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_WALL_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// ファイルオープンに成功
	    // 冒頭部分を書き込み
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       壁情報スクリプトファイル[save_wall.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n\n", SCRIPT);

		// 壁を保存する
		SaveWall(pFileSaver);

		// スクリプト読み込み終了の合図を書き込み
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

		// ファイルクローズ
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    壁保存処理
//=============================================================================
void CMap::SaveWall(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  壁情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_MESHWALL)
			{// 壁クラスへのポインタだった
				SaveWall_Info(pFileSaver, (CMeshWall*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    壁情報保存処理
//=============================================================================
void CMap::SaveWall_Info(CFileSaver *pFileSaver, CMeshWall *pMeshWall)
{
	// 事前に計算をすませておく
	D3DXVECTOR3 WallRot = pMeshWall->GetRot();
	// 向き
	WallRot.x = D3DXToDegree(WallRot.x);
	WallRot.y = D3DXToDegree(WallRot.y);
	WallRot.z = D3DXToDegree(WallRot.z);

	// 壁の情報を書き込み
	pFileSaver->Print("%s\n", WALLSET);
	pFileSaver->Print("	%s%d				# 使用するテクスチャの番号\n", TEX_TYPE, pMeshWall->GetTexIdx());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, pMeshWall->GetPos().x, pMeshWall->GetPos().y, pMeshWall->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f			# 向き\n", ROT, WallRot.x, WallRot.y, WallRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# 色\n", COL, pMeshWall->GetCol().r, pMeshWall->GetCol().g, pMeshWall->GetCol().b, pMeshWall->GetCol().a);
	pFileSaver->Print("	%s%.1f				# 1ブロック分の幅\n", WIDTH, pMeshWall->GetWidth());
	pFileSaver->Print("	%s%.1f				# 1ブロック分の高さ\n", HEIGHT, pMeshWall->GetHeight());
	pFileSaver->Print("	%s%d					# 横の分割数\n", XBLOCK, pMeshWall->GetXBlock());
	pFileSaver->Print("	%s%d					# 縦の分割数\n", YBLOCK, pMeshWall->GetYBlock());
	pFileSaver->Print("	%s%d				# テクスチャU座標の分割数\n", TEXSPLIT_U, pMeshWall->GetTexSplitU());
	pFileSaver->Print("	%s%d				# テクスチャV座標の分割数\n", TEXSPLIT_V, pMeshWall->GetTexSplitV());
	pFileSaver->Print("%s\n", END_WALLSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    アイテム情報保存のためのファイルオープン処理
//=============================================================================
void CMap::SaveItem_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_WALL_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// ファイルオープンに成功
	    // 冒頭部分を書き込み
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       アイテム情報スクリプトファイル[save_item.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n\n", SCRIPT);

		// アイテムを保存する
		SaveItem(pFileSaver);

		// スクリプト読み込み終了の合図を書き込み
		pFileSaver->Print("%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

		// ファイルクローズ
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    アイテム保存処理
//=============================================================================
void CMap::SaveItem(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  アイテム情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM)
			{// アイテムクラスへのポインタだった
				SaveItem_Info(pFileSaver, (CItem*)pScene);
			}
			pScene = pSceneNext;
		}
	}

}

//=============================================================================
//    アイテム情報保存処理
//=============================================================================
void CMap::SaveItem_Info(CFileSaver *pFileSaver, CItem *pItem)
{
	// 事前に計算をすませておく
	float fItemRot = pItem->GetRot();
	// 向き
	fItemRot = D3DXToDegree(fItemRot);

	// アイテムの情報を書き込み
	pFileSaver->Print("%s\n", ITEMSET);
	pFileSaver->Print("	%s%d				# 使用するテクスチャの番号\n", TEX_TYPE, pItem->GetTexIdx());
	pFileSaver->Print("	%s%d					# 種類\n", TYPE, pItem->GetType());
	pFileSaver->Print("	%s%.1f				# 当たり判定を取る範囲\n", COL_RANGE, pItem->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, pItem->GetPos().x, pItem->GetPos().y, pItem->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# 色\n", COL, pItem->GetCol().r, pItem->GetCol().g, pItem->GetCol().b, pItem->GetCol().a);
	pFileSaver->Print("	%s%.1f					# 向き\n", ROT, fItemRot);
	pFileSaver->Print("	%s%.1f				# 幅\n", WIDTH, pItem->GetWidth());
	pFileSaver->Print("	%s%.1f				# 高さ\n", HEIGHT, pItem->GetHeight());
	pFileSaver->Print("	%s%d				# ライティングするかしないか\n", LIGHTING, pItem->GetLighting());
	pFileSaver->Print("	%s%d			# 加算合成で描画するかしないか\n", DRAW_ADDTIVE, pItem->GetDrawAddtive());
	pFileSaver->Print("%s\n", END_ITEMSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    配置物を配置する処理
//=============================================================================
void CMap::SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 rollSpeed, int nModelIdx)
{
	// 配置物を生成する
	CObject *pObject = CObject::Create(pos, rot, scale, rollSpeed);
	if (pObject != NULL)
	{
		pObject->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObject->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    動く配置物を配置する処理
//=============================================================================
void CMap::SetObjectMove(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 rollSpeed, int nModelIdx, D3DXVECTOR3 move, int nMaxMoveCounter)
{
	// 動く配置物を生成する
	CObjectMove *pObjectMove = CObjectMove::Create(pos, rot, scale, rollSpeed, move, nMaxMoveCounter);
	if (pObjectMove != NULL)
	{
		pObjectMove->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObjectMove->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    ビルボードを配置する処理
//=============================================================================
void CMap::SetBillboard(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	CBillboardObject *pBillObj = CBillboardObject::Create(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType);
	if (pBillObj != NULL)
	{
		pBillObj->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    エフェクトを配置する処理
//=============================================================================
void CMap::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->SetEffect(pos, rot, nType);
	}
}

//=============================================================================
//    敵を配置する処理
//=============================================================================
void CMap::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nScore)
{
	if (m_pEnemyAssembly != NULL)
	{
		CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nType);
		if (pEnemyManager != NULL)
		{
			pEnemyManager->SetEnemy(pos, rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nScore);
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
void CMap::SetItem(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	CItem *pItem = CItem::Create(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType, BILLBOARD_PRIORITY);
	if (pItem != NULL)
	{
		pItem->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    モデル管轄クラスを破棄する処理
//=============================================================================
void CMap::ReleaseModelManager(void)
{
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();
		delete m_pModelManager;
		m_pModelManager = NULL;
	}
}

//=============================================================================
//    テクスチャ管轄クラスを破棄する処理
//=============================================================================
void CMap::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    敵管轄クラスを破棄する処理
//=============================================================================
void CMap::ReleaseEnemyAssembley(void)
{
	if (m_pEnemyAssembly != NULL)
	{
		delete m_pEnemyAssembly;
		m_pEnemyAssembly = NULL;
	}
}

//=============================================================================
//    マップのオブジェクトを破棄する処理
//=============================================================================
void CMap::DeleteMapObject(void)
{
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

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する処理
//=============================================================================
D3DXVECTOR3 CMap::GetPlayerDefaultPos(void)
{
	return m_PlayerDefaultPos;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する処理
//=============================================================================
D3DXVECTOR3 CMap::GetPlayerDefaultRot(void)
{
	return m_PlayerDefaultRot;
}