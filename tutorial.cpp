//*****************************************************************************
//
//     チュートリアルの処理[tutorial.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"
#include "ui.h"
#include "player.h"
#include "effectManager.h"
#include "meshWall.h"
#include "shadow.h"


#include "boxCollider.h"
#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define TUTORIAL_SYSTEM_FILENAME   "data/TEXT/MODE/tutorial.ini"                 // チュートリアルの初期化情報スクリプトファイル名
#define TUTORIAL_MODECHANGE_TIMING (600)                                         // 画面遷移を自動で行うタイミング
#define TUTORIAL_POLYGON_PRIORITY  (11)                                          // チュートリアルで使用する2Dポリゴンの処理優先順位

// 値を読み取るパス名
#define NUM_TEXTURE                "NUM_TEXTURE = "                              // 読み込むテクスチャの数
#define TEXTURE_FILENAME           "TEXTURE_FILENAME = "                         // 読み込むテクスチャのファイル名
#define PLAYER_FILENAME            "PLAYER_FILENAME = "                          // プレイヤーデータのスクリプトファイル名
#define EFFECT_FILENAME            "EFFECT_FILENAME = "                          // エフェクトデータのスクリプトファイル名
#define MAP_FILENAME               "MAP_FILENAME = "                             // マップデータのスクリプトファイル名

// プレイヤー情報
#define PLAYERSET                  "PLAYERSET"                                   // プレイヤー情報読み込み開始の合図
#define END_PLAYERSET              "END_PLAYERSET"                               // プレイヤー情報読み込み終了の合図
#define LIFE                       "LIFE = "                                     // プレイヤーの体力
#define ORBIT_SLASH                "ORBIT_SLASH = "                              // プレイヤーの軌跡ゲージ量
#define MAX_SPECIAL                "MAX_SPECIAL = "                              // プレイヤーの必殺ゲージの最大値量
#define FILENAME                   "FILENAME = "                                 // プレイヤーデータのスクリプトファイル名

// PressEnter用のマクロ
#define TUTORIAL_PRESS_POS         (D3DXVECTOR3(SCREEN_WIDTH / 2,600.0f,0.0f))   // PressEnterの座標
#define TUTORIAL_PRESS_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // PressEnterの色
#define TUTORIAL_PRESS_WIDTH       (400.0f)                                      // PressEnterの幅
#define TUTORIAL_PRESS_HEIGHT      (100.0f)                                      // PressEnterの高さ

// 画面遷移チェック用
#define TUTORIAL_MODE_LOGO_MAX        (3)                                           // 説明用の文字ポリゴンの最大数
#define TUTORIAL_MODE_BLACKCOL        (0.7f)                                        // 背景用ポリゴンの透明度最大値(この値に達したらチェック開始)
#define TUTORIAL_MODE_BLACKCOL_UP     (0.02f)                                       // 背景用ポリゴンの透明度を上げる値
#define TUTORIAL_MODE_LOGO_POS_0      (D3DXVECTOR3(426.0f,SCREEN_HEIGHT / 2, 0.0f)) // 1つ目の選択項目用ロゴポリゴンの座標
#define TUTORIAL_MODE_LOGO_POS_1      (D3DXVECTOR3(852.0f,SCREEN_HEIGHT / 2, 0.0f)) // 2つ目の選択項目用ロゴポリゴンの座標
#define TUTORIAL_MODE_LOGO_SCALE_NONE (100.0f)                                      // 選択されていないロゴポリゴンの大きさ
#define TUTORIAL_MODE_LOGO_SCALE      (200.0f)                                      // 選択されているロゴポリゴンの大きさ
#define TUTORIAL_MODE_LOGO_COL_NONE   (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))              // 選択されていないロゴポリゴンの色
#define TUTORIAL_MODE_LOGO_COL_MIN    (0.4f)                                        // 選択されているロゴポリゴンの色下限値
#define TUTORIAL_MODE_LOGO_COL_VALUE  (0.009f)                                      // 選択されているロゴポリゴンの色の変化量

// 1つ目のロゴポリゴン用
#define TUTORIAL_LOGO_POS_0           (D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f,0.0f))  // 1つ目のロゴポリゴンの座標
#define TUTORIAL_LOGO_WIDTH_0         (550.0f)                                      // 1つ目のロゴポリゴンの幅
#define TUTORIAL_LOGO_HEIGHT_0        (70.0f)                                       // 1つ目のロゴポリゴンの高さ

// 2つ目のロゴポリゴン用
#define TUTORIAL_LOGO_POS_1           (D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f,0.0f))  // 2つ目のロゴポリゴンの座標
#define TUTORIAL_LOGO_WIDTH_1         (250.0f)                                      // 2つ目のロゴポリゴンの幅
#define TUTORIAL_LOGO_HEIGHT_1        (40.0f)                                       // 2つ目のロゴポリゴンの高さ

// 3つ目のロゴポリゴン用
#define TUTORIAL_LOGO_POS_2           (D3DXVECTOR3(SCREEN_WIDTH / 2, 630.0f,0.0f))  // 3つ目のロゴポリゴンの座標
#define TUTORIAL_LOGO_WIDTH_2         (180.0f)                                      // 3つ目のロゴポリゴンの幅
#define TUTORIAL_LOGO_HEIGHT_2        (35.0f)                                       // 3つ目のロゴポリゴンの高さ

// 操作方法用ポリゴン1つ目
#define TUTORIAL_OPERATION_POS_0    (D3DXVECTOR3(330.0f,350.0f,0.0f))               // 操作方法用のポリゴンの座標(1つ目)
#define TUTORIAL_OPERATION_COL_0    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // 操作方法用のポリゴンの色(1つ目)
#define TUTORIAL_OPERATION_WIDTH_0  (330.0f)                                        // 操作方法用のポリゴンの幅(1つ目)
#define TUTORIAL_OPERATION_HEIGHT_0 (180.0f)                                        // 操作方法用のポリゴンの高さ(1つ目)

// 操作方法用ポリゴン2つ目
#define TUTORIAL_OPERATION_POS_1    (D3DXVECTOR3(950.0f,350.0f,0.0f))               // 操作方法用のポリゴンの座標(2つ目)
#define TUTORIAL_OPERATION_COL_1    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // 操作方法用のポリゴンの色(2つ目)
#define TUTORIAL_OPERATION_WIDTH_1  (330.0f)                                        // 操作方法用のポリゴンの幅(2つ目)
#define TUTORIAL_OPERATION_HEIGHT_1 (180.0f)                                        // 操作方法用のポリゴンの高さ(2つ目)

// 操作方法切り替え表示用ポリゴン
#define TUTORIAL_OPERATION_POS      (D3DXVECTOR3(1060.0f,30.0f,0.0f))               // 操作方法切り替え表示用ポリゴンの座標
#define TUTORIAL_OPERATION_COL      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // 操作方法切り替え表示用ポリゴンの色
#define TUTORIAL_OPERATION_WIDTH    (220.0f)                                        // 操作方法切り替え表示用ポリゴンの幅
#define TUTORIAL_OPERATION_HEIGHT   (30.0f)                                         // 操作方法切り替え表示用ポリゴンの高さ

// 操作方法切り替え表示用ポリゴン
#define TUTORIAL_SKIP_POS           (D3DXVECTOR3(1080.0f,80.0f,0.0f))               // スキップ方法表示用ポリゴンの座標
#define TUTORIAL_SKIP_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // スキップ方法表示用ポリゴンの色
#define TUTORIAL_SKIP_WIDTH         (200.0f)                                        // スキップ方法表示用ポリゴンの幅
#define TUTORIAL_SKIP_HEIGHT        (28.0f)                                         // スキップ方法表示用ポリゴンの高さ

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CTutorial::CTutorial(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pPlayer = NULL;           // プレイヤークラスへのポインタ(保存用)
	m_pUi = NULL;               // UIクラスへのポインタ
	m_pMap = NULL;              // マップクラスへのポインタ
	m_nCounter = 0;             // 遷移を管理するカウンター
	m_pTextureManager = NULL;   // テクスチャ管轄クラスへのポインタ
	m_bGoalCol = false;         // ゴールに触れているかどうか
	m_pGoalWall = NULL;         // ゴール用の壁クラスへのポインタ
	m_State = STATE_NONE;       // 状態
	m_pSkip = NULL;             // スキップ方法表示ポリゴンクラスへのポインタ
	m_pCheck = NULL;            // 操作方法表示切替用ポリゴンクラスへのポインタ
	m_bOperationInfo = false;   // 操作方法を表示しているかどうか
	for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
	{// 操作方法用のポリゴンの数だけ繰り返し
		m_pOperationInfo[nCntOpe] = NULL;
	}

	// 画面遷移チェック用
	m_nSelectMode = 0;          // 画面遷移するかどうかを選べる番号
	m_pBlackPoly = NULL;        // チェックするときに表示する黒いポリゴン
	m_fModeChangeCol = 0.0f;    // ロゴポリゴンの色の変化量
	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// 選択項目の数だけ繰り返し
		m_pSelectMode[nCntSelect] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;  // チュートリアルクラス型のポインタ
	if (pTutorial == NULL)
	{// メモリが確保できる状態にある
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pTutorial->Init();
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
	return pTutorial;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// 影モデル読み込み
	CShadow::Load();

	// チュートリアルの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(TUTORIAL_SYSTEM_FILENAME, "r");
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
			char aEffectFileName[256] = "\0";           // エフェクトデータのスクリプトファイル名
			char aPlayerFileName[256] = "\0";           // プレイヤーデータのスクリプトファイル名
			char aMapFileName[256] = "\0";              // マップデータのスクリプトファイル名
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
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// マップデータのスクリプトファイル名だった
							    // マップデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// 必要なクラスを作成する
					m_pUi = CUI::Create(999 * 60, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial, m_pTextureManager);

					// エフェクト管轄クラス
					m_pEffectManager = CEffectManager::Create(aEffectFileName);

					// マップの生成
					m_pMap = CMap::Create(aMapFileName);

					// プレイヤー
					CPlayerManager::Create(aPlayerFileName, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial);

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

					// 色の変化量を設定
					m_fModeChangeCol = TUTORIAL_MODE_LOGO_COL_VALUE;    // ロゴポリゴンの色の変化量

					// エフェクトを出す
					if (m_pEffectManager != NULL)
					{
						m_pEffectManager->SetEffect(D3DXVECTOR3(0.0f, 0.0f, 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 30);
					}

					// 操作方法用のポリゴンの生成
					// 1つ目
					if (m_pOperationInfo[0] == NULL)
					{
						m_pOperationInfo[0] = CScene2D::Create(TUTORIAL_OPERATION_POS_0, TUTORIAL_OPERATION_COL_0, TUTORIAL_OPERATION_WIDTH_0, TUTORIAL_OPERATION_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pOperationInfo[0] != NULL)
						{
							m_pOperationInfo[0]->BindTexture(m_pTextureManager->GetTexture(12));
						}
					}

					// 2つ目
					if (m_pOperationInfo[1] == NULL)
					{
						m_pOperationInfo[1] = CScene2D::Create(TUTORIAL_OPERATION_POS_1, TUTORIAL_OPERATION_COL_1, TUTORIAL_OPERATION_WIDTH_1, TUTORIAL_OPERATION_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pOperationInfo[1] != NULL)
						{
							m_pOperationInfo[1]->BindTexture(m_pTextureManager->GetTexture(13));
						}
					}

					// 操作方法表示切替用ポリゴンの生成
					if (m_pCheck == NULL)
					{
						m_pCheck = CScene2D::Create(TUTORIAL_OPERATION_POS, TUTORIAL_OPERATION_COL, TUTORIAL_OPERATION_WIDTH, TUTORIAL_OPERATION_HEIGHT, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pCheck != NULL)
						{
							m_pCheck->BindTexture(m_pTextureManager->GetTexture(14));
						}
					}
					m_bOperationInfo = true;

					// スキップ方法表示用ポリゴンの生成
					if (m_pSkip == NULL)
					{
						m_pSkip = CScene2D::Create(TUTORIAL_SKIP_POS, TUTORIAL_SKIP_COL, TUTORIAL_SKIP_WIDTH, TUTORIAL_SKIP_HEIGHT, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pSkip != NULL)
						{
							m_pSkip->BindTexture(m_pTextureManager->GetTexture(15));
						}
					}

					// 当たり判定用箱モデルの配置
					SetBoxCollider();

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

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
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

	// チュートリアルオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTutorial::Update(void)
{
	if (m_State == STATE_MODECHECK)
	{// 画面遷移するかチェックする状態だったら
		ModeChangeCheck();
	}
	else if(m_State == STATE_MODENONE)
	{// 画面遷移しなかったら
		if (m_pBlackPoly != NULL)
		{// 黒いポリゴンが生成されている
			D3DXCOLOR BlackCol = m_pBlackPoly->GetCol();
			if (BlackCol.a > 0.0f)
			{// まだ透明度を下げれる
				BlackCol.a -= TUTORIAL_MODE_BLACKCOL_UP;
				// 色を設定
				m_pBlackPoly->SetCol(BlackCol);
				if (BlackCol.a <= 0.0f)
				{// 透明度が下がり切った
					m_pBlackPoly->Uninit();
					m_pBlackPoly = NULL;
					m_State = STATE_NONE;
					m_nSelectMode = 0;
				}
			}
		}
	}
	else if (m_State == STATE_NONE)
	{// 通常状態だったら
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
		    // キーボードを取得
			CInputKeyboard *pKeyboard = CManager::GetKeyboard();

			// コントローラーを取得
			CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);
			if (pKeyboard != NULL)
			{// キーボードが取得できた
				if (pKeyboard->GetTrigger(DIK_P) == true)
				{// Pキーが押された
					m_bOperationInfo = m_bOperationInfo ? false : true;
					if (m_bOperationInfo == true)
					{// 表示するなら
					    // 操作方法用のポリゴンの生成
					    // 1つ目
						if (m_pOperationInfo[0] == NULL)
						{
							m_pOperationInfo[0] = CScene2D::Create(TUTORIAL_OPERATION_POS_0, TUTORIAL_OPERATION_COL_0, TUTORIAL_OPERATION_WIDTH_0, TUTORIAL_OPERATION_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
							if (m_pOperationInfo[0] != NULL)
							{
								m_pOperationInfo[0]->BindTexture(m_pTextureManager->GetTexture(12));
							}
						}

						// 2つ目
						if (m_pOperationInfo[1] == NULL)
						{
							m_pOperationInfo[1] = CScene2D::Create(TUTORIAL_OPERATION_POS_1, TUTORIAL_OPERATION_COL_1, TUTORIAL_OPERATION_WIDTH_1, TUTORIAL_OPERATION_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
							if (m_pOperationInfo[1] != NULL)
							{
								m_pOperationInfo[1]->BindTexture(m_pTextureManager->GetTexture(13));
							}
						}
					}
					else if (m_bOperationInfo == false)
					{// 表示しないなら
					    // 操作方法用のポリゴンの開放
						for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
						{// 操作方法用のポリゴンの数だけ繰り返し
							if (m_pOperationInfo[nCntOpe] != NULL)
							{// メモリが確保されている
								m_pOperationInfo[nCntOpe]->Uninit();
								m_pOperationInfo[nCntOpe] = NULL;
							}
						}
					}
				}
				else if (pKeyboard->GetTrigger(DIK_RSHIFT) == true)
				{// RSHIFTキーが押された
					CManager::GetFade()->SetFade(CManager::MODE_GAME);
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
				}
				else
				{// キーボードのボタンが押されていない
					if (pJoyStick != NULL)
					{// コントローラーが取得できた
						if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_11) == true)
						{// STARTボタンが押された
							m_bOperationInfo = m_bOperationInfo ? false : true;
							if (m_bOperationInfo == true)
							{// 表示するなら
							    // 操作方法用のポリゴンの生成
							    // 1つ目
								if (m_pOperationInfo[0] == NULL)
								{
									m_pOperationInfo[0] = CScene2D::Create(TUTORIAL_OPERATION_POS_0, TUTORIAL_OPERATION_COL_0, TUTORIAL_OPERATION_WIDTH_0, TUTORIAL_OPERATION_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
									if (m_pOperationInfo[0] != NULL)
									{
										m_pOperationInfo[0]->BindTexture(m_pTextureManager->GetTexture(12));
									}
								}

								// 2つ目
								if (m_pOperationInfo[1] == NULL)
								{
									m_pOperationInfo[1] = CScene2D::Create(TUTORIAL_OPERATION_POS_1, TUTORIAL_OPERATION_COL_1, TUTORIAL_OPERATION_WIDTH_1, TUTORIAL_OPERATION_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
									if (m_pOperationInfo[1] != NULL)
									{
										m_pOperationInfo[1]->BindTexture(m_pTextureManager->GetTexture(13));
									}
								}
							}
							else if (m_bOperationInfo == false)
							{// 表示しないなら
							    // 操作方法用のポリゴンの開放
								for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
								{// 操作方法用のポリゴンの数だけ繰り返し
									if (m_pOperationInfo[nCntOpe] != NULL)
									{// メモリが確保されている
										m_pOperationInfo[nCntOpe]->Uninit();
										m_pOperationInfo[nCntOpe] = NULL;
									}
								}
							}
						}
						else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_10) == true)
						{// BACKボタンが押された
							CManager::GetFade()->SetFade(CManager::MODE_GAME);
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
						}
					}
				}
			}
		}
	}

	CDebugProc::Print("チュートリアル画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
//    画面遷移するかチェックする処理
//=============================================================================
void CTutorial::ModeChangeCheck(void)
{
	if (m_pBlackPoly != NULL)
	{// 黒いポリゴンが生成されている
		D3DXCOLOR BlackCol = m_pBlackPoly->GetCol();
		if(BlackCol.a < TUTORIAL_MODE_BLACKCOL)
		{// まだ透明度を上げれる
			BlackCol.a += TUTORIAL_MODE_BLACKCOL_UP;
			if (BlackCol.a >= TUTORIAL_MODE_BLACKCOL)
			{// 透明度が上がり切った
				BlackCol.a = TUTORIAL_MODE_BLACKCOL;
				CreateModeLogo();
			}
			// 色を設定
			m_pBlackPoly->SetCol(BlackCol);
		}
		else
		{// もう上げきれない
			// ロゴの色を変化させる
			if (m_pSelectMode[m_nSelectMode] != NULL)
			{// ポリゴンが生成されている
				m_pSelectMode[m_nSelectMode]->Update();
			}

			// 画面遷移チェック開始
			// キーボードを取得
			CInputKeyboard *pKeyboard = CManager::GetKeyboard();

			// コントローラーを取得
			CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

			if (pKeyboard != NULL)
			{// キーボードが取得できた
				if (pKeyboard->GetTrigger(DIK_RETURN) == true)
				{// ENTERキーが押された
					if (m_nSelectMode == SELECT_CHANGE)
					{// 画面遷移する番号だった
						if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
						{// フェードの状態が使用されていない状態である
							CManager::GetFade()->SetFade(CManager::MODE_GAME);
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
							m_State = STATE_MODECHANGE;
						}
					}
					else if(m_nSelectMode == SELECT_NONE)
					{// 画面遷移しない番号だった
						m_State = STATE_MODENONE;
						for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
						{// 選択項目の数だけ繰り返し
							if (m_pSelectMode[nCntSelect] != NULL)
							{// メモリが確保されている
								m_pSelectMode[nCntSelect]->Uninit();
								m_pSelectMode[nCntSelect] = NULL;
							}
						}
						for (int nCntLogo = 0; nCntLogo < TUTORIAL_MODE_LOGO_MAX; nCntLogo++)
						{// ロゴの数だけ繰り返し
							if (m_pLogo[nCntLogo] != NULL)
							{// メモリが確保されている
								m_pLogo[nCntLogo]->Uninit();
								m_pLogo[nCntLogo] = NULL;
							}
						}
					}
				}
				else if (pKeyboard->GetTrigger(DIK_A) == true || pKeyboard->GetRepeat(DIK_A) == true)
				{// Aキーが押された
					// 選択されていたロゴの設定
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
					m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

					// 番号の設定
					m_nSelectMode = (m_nSelectMode + (SELECT_MAX - 1)) % SELECT_MAX;

					// これから選択されるロゴの設定
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
					m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

					// 音を鳴らす
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				}
				else if (pKeyboard->GetTrigger(DIK_D) == true || pKeyboard->GetRepeat(DIK_D) == true)
				{// Dキーが押された
				    // 選択されていたロゴの設定
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
					m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

					// 番号の設定
					m_nSelectMode = (m_nSelectMode + 1) % SELECT_MAX;

					// これから選択されるロゴの設定
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
					m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

					// 音を鳴らす
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				}
				else
				{// ENTERキーが押されていない
					if (pJoyStick != NULL)
					{// コントローラーが取得できた
						if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == true)
						{// Bボタンが押された
							if (m_nSelectMode == SELECT_CHANGE)
							{// 画面遷移する番号だった
								if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
								{// フェードの状態が使用されていない状態である
									CManager::GetFade()->SetFade(CManager::MODE_GAME);
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
									m_State = STATE_MODECHANGE;
								}
							}
							else if (m_nSelectMode == SELECT_NONE)
							{// 画面遷移しない番号だった
								m_State = STATE_MODENONE;
								for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
								{// 選択項目の数だけ繰り返し
									if (m_pSelectMode[nCntSelect] != NULL)
									{// メモリが確保されている
										m_pSelectMode[nCntSelect]->Uninit();
										m_pSelectMode[nCntSelect] = NULL;
									}
								}
								for (int nCntLogo = 0; nCntLogo < TUTORIAL_MODE_LOGO_MAX; nCntLogo++)
								{// ロゴの数だけ繰り返し
									if (m_pLogo[nCntLogo] != NULL)
									{// メモリが確保されている
										m_pLogo[nCntLogo]->Uninit();
										m_pLogo[nCntLogo] = NULL;
									}
								}
							}
						}
						else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_22) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_22) == true)
						{// 左スティックが右に動かされた
						    // 選択されていたロゴの設定
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
							m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

							// 番号の設定
							m_nSelectMode = (m_nSelectMode + 1) % SELECT_MAX;

							// これから選択されるロゴの設定
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
							m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

							// 音を鳴らす
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
						}
						else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_23) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_23) == true)
						{// 左スティックが左に動かされた
						    // 選択されていたロゴの設定
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
							m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

							// 番号の設定
							m_nSelectMode = (m_nSelectMode + (SELECT_MAX - 1)) % SELECT_MAX;

							// これから選択されるロゴの設定
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
							m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

							// 音を鳴らす
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
						}
					}
				}
			}
			CDebugProc::Print("画面遷移しますか\n");
			CDebugProc::Print("選択番号 %d\n", m_nSelectMode);
		}
	}
}

//=============================================================================
//    画面遷移チェック用のロゴを生成する処理
//=============================================================================
void CTutorial::CreateModeLogo(void)
{
	// 選択項目用のロゴポリゴンを生成
	m_pSelectMode[0] = CScene2DFlash::Create(TUTORIAL_MODE_LOGO_POS_0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f, 0.0f, CScene2DFlash::STATE_SELECT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.4f), TUTORIAL_MODE_LOGO_COL_VALUE, 4, TUTORIAL_POLYGON_PRIORITY);
	if (m_pSelectMode[0] != NULL)
	{// 生成できた
		m_pSelectMode[0]->BindTexture(m_pTextureManager->GetTexture(3));
		m_pSelectMode[0]->SetLength(TUTORIAL_MODE_LOGO_SCALE);
	}

	m_pSelectMode[1] = CScene2DFlash::Create(TUTORIAL_MODE_LOGO_POS_1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f, 0.0f, CScene2DFlash::STATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.4f), TUTORIAL_MODE_LOGO_COL_VALUE, 4, TUTORIAL_POLYGON_PRIORITY);
	if (m_pSelectMode[1] != NULL)
	{// 生成できた
		m_pSelectMode[1]->BindTexture(m_pTextureManager->GetTexture(4));
		m_pSelectMode[1]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);
	}

	// 説明用のロゴポリゴンを生成
	m_pLogo[0] = CScene2D::Create(TUTORIAL_LOGO_POS_0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIAL_LOGO_WIDTH_0, TUTORIAL_LOGO_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	if (m_pLogo[0] != NULL)
	{// 生成できた
		m_pLogo[0]->BindTexture(m_pTextureManager->GetTexture(0));
	}

	m_pLogo[1] = CScene2D::Create(TUTORIAL_LOGO_POS_1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIAL_LOGO_WIDTH_1, TUTORIAL_LOGO_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	if (m_pLogo[1] != NULL)
	{// 生成できた
		m_pLogo[1]->BindTexture(m_pTextureManager->GetTexture(1));
	}

	m_pLogo[2] = CScene2D::Create(TUTORIAL_LOGO_POS_2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIAL_LOGO_WIDTH_2, TUTORIAL_LOGO_HEIGHT_2, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	if (m_pLogo[2] != NULL)
	{// 生成できた
		m_pLogo[2]->BindTexture(m_pTextureManager->GetTexture(2));
	}
}

//=============================================================================
//    チュートリアル用の当たり判定用箱モデルを配置する処理
//=============================================================================
void CTutorial::SetBoxCollider(void)
{
	// 壁の右側
	CBoxCollider::Create(D3DXVECTOR3(495.0f,0.0f,-1000.0f), 10.0f, 300.0f, 1000.0f, true, 5);

	// 壁の左側
	CBoxCollider::Create(D3DXVECTOR3(-505.0f, 0.0f, -1000.0f), 10.0f, 300.0f, 1000.0f, true, 5);

	// 壁の後ろ側
	CBoxCollider::Create(D3DXVECTOR3(-500.0f, 0.0f, -1005.0f), 1000.0f, 300.0f, 10.0f, true, 5);

	// 壁の前の右側
	CBoxCollider::Create(D3DXVECTOR3(112.0f, 0.0f, -5.0f), 500.0f, 300.0f, 10.0f, true, 5);

	// 壁の前の左側
	CBoxCollider::Create(D3DXVECTOR3(-512.0f, 0.0f, -5.0f), 400.0f, 300.0f, 10.0f, true, 5);
}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CTutorial::SetState(const STATE state)
{
	m_State = state;
	if (m_State == STATE_MODECHECK)
	{// 画面遷移するかどうかチェックする状態になった
		m_pBlackPoly = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	}
}

//=============================================================================
//    ゴールに触れているかどうかを設定する処理
//=============================================================================
void CTutorial::SetGoalCol(const bool bGoalCol)
{
	m_bGoalCol = bGoalCol;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CTutorial::STATE CTutorial::GetState(void)
{
	return m_State;
}

//=============================================================================
//    プレイヤークラスへのポインタを取得する処理
//=============================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
//    ゴール用の壁クラスを取得する処理
//=============================================================================
CMeshWall *CTutorial::GetGoalWall(void)
{
	return m_pGoalWall;
}

//=============================================================================
//    ゴールに触れているかどうかを取得する処理
//=============================================================================
bool CTutorial::GetGoalCol(void)
{
	return m_bGoalCol;
}