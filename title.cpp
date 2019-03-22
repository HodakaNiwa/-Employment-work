//*****************************************************************************
//
//     タイトルの処理[title.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "mesh2D.h"
#include "particle2D.h"
#include "textureManager.h"
#include "effectManager.h"
#include "functionlib.h"
#include "map.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// データ入力用マクロ
#define TITLE_SYSTEM_FILENAME       "data/TEXT/MODE/title.ini"                    // タイトルの初期化情報スクリプトファイル名
#define TITLE_MODECHANGE_TIMING     (1555)                                        // 画面遷移を自動で行うタイミング
#define TITLE_ALPHACUTSTART_TIMING  (40)                                          // 黒いポリゴンの透明度を下げ始めるタイミング
#define TITLE_POLYGON_PRIORITY      (11)                                          // タイトルで使用するポリゴンの処理優先順位

// 値を読み取るパス名
#define NUM_TEXTURE                 "NUM_TEXTURE = "                              // 読み込むテクスチャの数
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "                         // 読み込むテクスチャのファイル名
#define EFFECT_FILENAME             "EFFECT_FILENAME = "                          // エフェクトデータのスクリプトファイル名
#define MAP_FILENAME                "MAP_FILENAME = "                             // マップデータのスクリプトファイル名

// タイトルロゴ用１個目用のマクロ
#define TITLE_LOGO_0_POS            (D3DXVECTOR3(SCREEN_WIDTH / 2,260.0f,0.0f))   // タイトルロゴ１個目の座標
#define TITLE_LOGO_0_COL            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // タイトルロゴ１個目の色
#define TITLE_LOGO_0_WIDTH          (640.0f)                                      // タイトルロゴ１個目の幅
#define TITLE_LOGO_0_HEIGHT         (280.0f)                                      // タイトルロゴ１個目の高さ

// タイトルロゴ用２個目用のマクロ
#define TITLE_LOGO_1_TIMING         (45)                                          // タイトルロゴ２個目を出すまでの時間
#define TITLE_LOGO_1_POS            (D3DXVECTOR3(SCREEN_WIDTH / 2,260.0f,0.0f))   // タイトルロゴ２個目の座標
#define TITLE_LOGO_1_COL            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // タイトルロゴ２個目の色
#define TITLE_LOGO_1_WIDTH          (3000.0f)                                     // タイトルロゴ２個目の幅
#define TITLE_LOGO_1_HEIGHT         (1200.0f)                                     // タイトルロゴ２個目の高さ
#define TITLE_LOGO_1_SCALE          (700.0f)                                      // タイトルロゴ２個目の大きさ
#define TITLE_LOGO_1_SCALEDOWN      (1.5f)                                        // タイトルロゴ２個目の大きさの変化量

// 白いポリゴン用のマクロ
#define TITLE_FADEOUT_SPEED         (0.05f)                                       // 透明度を上げていく割合
#define TITLE_FADESTOP_TIME         (100)                                         // フェードアウトしてからフェードインを開始するまでのタイミング
#define TITLE_FADEIN_SPEED          (0.01f)                                       // 透明度を下げていく割合

// 黒いメッシュポリゴン用のマクロ
#define TITLE_BLACK_POS             (D3DXVECTOR3(SCREEN_WIDTH / 2,240.0f,0.0f))   // 黒いポリゴンの座標
#define TITLE_BLACK_WIDTH           (10.0f)                                       // 黒いポリゴンの1ブロック分の幅
#define TITLE_BLACK_HEIGHT          (490.0f)                                      // 黒いポリゴンの1ブロック分の高さ
#define TITLE_BLACK_XBLOCK          (128)                                         // 黒いポリゴンの横の分割数
#define TITLE_BLACK_YBLOCK          (1)                                           // 黒いポリゴンの縦の分割数
#define TITLE_BLACK_ALPHACUT        (0.5f)                                        // 透明度を下げていく割合

// PressEnter用のマクロ
#define TITLE_PRESS_POS             (D3DXVECTOR3(SCREEN_WIDTH / 2,645.0f,0.0f))   // PressEnterの座標
#define TITLE_PRESS_COL             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // PressEnterの色
#define TITLE_PRESS_WIDTH           (450.0f)                                      // PressEnterの幅
#define TITLE_PRESS_HEIGHT          (50.0f)                                       // PressEnterの高さ

// パーティクル用のマクロ
#define TITLE_PARTICLE_POS_START    (D3DXVECTOR3(-30.0f,460.0f,0.0f))             // パーティクルを出す位置のスタート場所
#define TITLE_PARTICLE_MOVE         (D3DXVECTOR3(10.3f,0.0f,0.0f))                // パーティクルを出す位置を動かす量
#define TITLE_PARTICLE_APPEAR       (5)                                           // 一回の更新で出すパーティクルの量

// 軌跡エフェクト用のマクロ
#define TITLE_ORBIT_PATTERN_NUM     (4)                                           // 軌跡エフェクトのパターン数
#define TITLE_ORBIT_WIDTH           (40.0f)                                       // 軌跡エフェクトの横幅
#define TITLE_ORBIT_HEIGHT          (40.0f)                                       // 軌跡エフェクトの縦幅
#define TITLE_ORBIT_MOVESPEED       (0.03f)                                       // 軌跡エフェクトの移動スピード
#define TITLE_ORBIT_SWITCH_MIN      (60)                                          // 軌跡エフェクトのパターンを切り替えるタイミングの最小値
#define TITLE_ORBIT_SWITCH_RANDOM   (30)                                          // 軌跡エフェクトのパターンを切り替えるタイミングの振れ幅

// パターン1
#define TITLE_ORBIT_POS_START_0     (D3DXVECTOR3(-30.0f,0.0f,0.0f))               // 軌跡エフェクトを出す位置のスタート位置
#define TITLE_ORBIT_POS_DEST_0      (D3DXVECTOR3(1300.0f,300.0f,0.0f))            // 軌跡エフェクトを出す位置の目標位置

// パターン2
#define TITLE_ORBIT_POS_START_1     (D3DXVECTOR3(1300.0f,0.0f,0.0f))              // 軌跡エフェクトを出す位置のスタート位置
#define TITLE_ORBIT_POS_DEST_1      (D3DXVECTOR3(-30.0f,500.0f,0.0f))             // 軌跡エフェクトを出す位置の目標位置

// パターン3
#define TITLE_ORBIT_POS_START_2     (D3DXVECTOR3(-30.0f,700.0f,0.0f))             // 軌跡エフェクトを出す位置のスタート位置
#define TITLE_ORBIT_POS_DEST_2      (D3DXVECTOR3(1300.0f,0.0f,0.0f))              // 軌跡エフェクトを出す位置の目標位置

// パターン4
#define TITLE_ORBIT_POS_START_3     (D3DXVECTOR3(1300.0f,600.0f,0.0f))            // 軌跡エフェクトを出す位置のスタート位置
#define TITLE_ORBIT_POS_DEST_3      (D3DXVECTOR3(-30.0f,0.0f,0.0f))               // 軌跡エフェクトを出す位置の目標位置

// タイトルカメラ用のマクロ
#define TITLE_CAMERA_PATTERN_NUM    (3)                                           // タイトルカメラのパターン数
#define TITLE_CAMERA_START_POSV_Y   (D3DXVECTOR3(0.0f,350.0f,0.0f))               // タイトルスタート時のカメラの視点の高さ
#define TITLE_CAMERA_START_POSR     (D3DXVECTOR3(450.0f,270.0f,-1760.0f))         // タイトルスタート時のカメラの注視点の位置
#define TITLE_CAMERA_DEST_POSR      (D3DXVECTOR3(450.0f,270.0f,3100.0f))          // タイトルスタート時のカメラの注視点の目的の位置
#define TITLE_CAMERA_MOVESPEED      (0.0015f)                                     // タイトルスタート時のカメラの移動スピード

// パターン1
#define TITLE_CAMERA_SWITCHTIMING_0 (585)                                         // 1つ目のパターンから2つ目のパターンへタイトルカメラを切り替えるタイミング
#define TITLE_CAMERA_0_POSV_Y       (D3DXVECTOR3(0.0f,120.0f,0.0f))               // パターン2つ目のカメラの視点の高さ
#define TITLE_CAMERA_0_POSR         (D3DXVECTOR3(-89.0f,180.0f,1640.0f))          // パターン2つ目のカメラの注視点の位置
#define TITLE_CAMERA_0_POSRDEST     (D3DXVECTOR3(101.0f,180.0f,2740.0f))          // パターン2つ目のカメラの注視点の目的の位置
#define TITLE_CAMERA_0_MOVESPEED    (0.0005f)                                     // パターン2つ目のカメラの移動スピード
#define TITLE_CAMERA_0_ROT          (D3DXVECTOR3(0.0f,-1.1f,0.0f))                // パターン2つ目のカメラ向き

// パターン2
#define TITLE_CAMERA_SWITCHTIMING_1 (580)                                         // 2つ目のパターンから3つ目のパターンへタイトルカメラを切り替えるタイミング
#define TITLE_CAMERA_1_POSV_Y       (D3DXVECTOR3(0.0f,40.0f,0.0f))                // パターン3つ目のカメラの視点の高さ
#define TITLE_CAMERA_1_POSR         (D3DXVECTOR3(-1300.0f,-30.0f,4255.0f))        // パターン3つ目のカメラの注視点の位置
#define TITLE_CAMERA_1_POSRDEST     (D3DXVECTOR3(1700.0f,40.0f,4255.0f))          // パターン3つ目のカメラの注視点の目的の位置
#define TITLE_CAMERA_1_MOVESPEED    (0.002f)                                      // パターン3つ目のカメラの移動スピード
#define TITLE_CAMERA_1_ROT          (D3DXVECTOR3(0.0f,D3DX_PI / 2,0.0f))          // パターン3つ目のカメラ向き

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CTitle::CTitle(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{
	// 各種値のクリア
	m_State = STATE_NONE;          // 状態
	m_pMap = NULL;                 // マップクラスへのポインタ
	m_nCounter = 0;                // カメラの動きを管理するカウンター
	m_pPress = NULL;               // PressEnter用の2Dポリゴンクラスへのポインタ
	m_pTextureManager = NULL;      // テクスチャ管轄クラスへのポインタ
	m_pEffectManager = NULL;       // エフェクト管轄クラスへのポインタ
	m_pWhitePoly = NULL;           // フェードインに使用する白いポリゴン
	strcpy(m_aMapFileName, "\0");  // 生成するマップの名前
	m_fCutScaleValue = 0.0f;       // 大きさを減らす割合
	m_pBlackMesh = NULL;           // 演出用の黒いポリゴンクラスへのポインタ
	m_nPatternCamera = 0;          // 現在のタイトルカメラのパターン番号
	m_nPatternOrbit = 0;           // 現在の軌跡エフェクトパターン番号
	m_nModeCounter = 0;            // 遷移を管理するカウンター
	m_nEffectCounter = 0;          // エフェクトを管理するカウンター
	m_nEffectSwicthTiming = 0;     // エフェクトのパターンを切り替えるタイミング

	for (int nCntTitle = 0; nCntTitle < LOGOTYPE_MAX; nCntTitle++)
	{// タイトルロゴの数だけ繰り返し
		m_pTitleLogo[nCntTitle] = NULL;  // タイトルロゴ用の2Dポリゴンクラスへのポインタ
	}
	m_ParEmitterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // パーティクルを出す位置
	m_ParEmitterDiffPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // パーティクルを出す目的の位置
	m_ParCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);          // パーティクルの色
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;  // タイトルクラス型のポインタ
	if (pTitle == NULL)
	{// メモリが確保できる状態にある
		pTitle = new CTitle;
		if (pTitle != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pTitle->Init();
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
	return pTitle;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	// タイトルの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(TITLE_SYSTEM_FILENAME, "r");
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
								strcpy(m_aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// エフェクト管轄クラス
					m_pEffectManager = CEffectManager::Create(aEffectFileName);

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

	// 最初に必要なロゴやポリゴンを生成する
	StartCreate();

	// パーティクルを出す位置を設定
	m_ParEmitterPos = TITLE_PARTICLE_POS_START;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// エフェクト管轄クラスへのポインタを空にしておく
	m_pEffectManager = NULL;

	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// タイトルロゴの破棄
	for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
	{// ロゴの数だけ繰り返し
		if (m_pTitleLogo[nCntLogo] != NULL)
		{
			m_pTitleLogo[nCntLogo]->Uninit();
			m_pTitleLogo[nCntLogo] = NULL;
		}
	}

	// 白いポリゴンの破棄
	if (m_pWhitePoly != NULL)
	{
		m_pWhitePoly->Uninit();
		m_pWhitePoly = NULL;
	}

	// PressEnterの破棄
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	// マップクラスを開放する
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// タイトルオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTitle::Update(void)
{
	// デバッグ用
	//if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	//{// フェードの状態が使用されていない状態である
	//	CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
	//}

	// カウンターを進める
	m_nCounter++;

	if (m_State == STATE_OK_CHANGEMODE)
	{// 画面遷移ができる状態ならば
		CreateOrbit();
		SwitchCamera();
		CheckModeChange();
	}
	else
	{// 画面遷移ができない状態ならば
		if (m_State == STATE_NONE)
		{// まだなにも出ていない状態ならば
			if (m_nCounter >= TITLE_ALPHACUTSTART_TIMING)
			{// カウンターが既定の値を超えた
				m_nCounter = 0;
				m_State = STATE_ALPHACUT_BLACK;
			}
		}
		else if (m_State == STATE_ALPHACUT_BLACK)
		{// 黒いポリゴンの透明度を下げる状態ならば
			AlphaCutBlack();
		}
		else if (m_State == STATE_STOP_LOGO)
		{// ロゴを出すまで待機する状態だったら
			if (m_nCounter >= TITLE_LOGO_1_TIMING)
			{// ロゴを出す時間になった
				// ロゴを生成する
				if (m_pTitleLogo[1] == NULL)
				{// メモリを確保できる状態である
					m_pTitleLogo[1] = CScene2D::Create(TITLE_LOGO_1_POS, TITLE_LOGO_1_COL, TITLE_LOGO_1_WIDTH, TITLE_LOGO_1_HEIGHT, 0.0f, TITLE_POLYGON_PRIORITY + 1);
					if (m_pTitleLogo[1] != NULL)
					{// ポリゴンが生成できた
						m_pTitleLogo[1]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));
					}
				}
				m_State = STATE_APPEAR_LOGO;
			}
		}
		else if(m_State == STATE_APPEAR_LOGO)
		{// ロゴが出現した状態だったら
			LogoCutScale();
		}
		else if (m_State == STATE_FADE_OUT)
		{// フェードアウトする状態ならば
			FadeOut();
		}
		else if (m_State == STATE_FADE_STOP)
		{// フェードを止める状態ならば
			FadeStop();
		}
		else if (m_State == STATE_FADE_IN)
		{// フェードイン状態ならば
			FadeIn();
		}
	}

	CDebugProc::Print("タイトル画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
//    タイトル画面の開始と同時に生成する処理
//=============================================================================
void CTitle::StartCreate(void)
{
	// 軌跡っぽい柄のポリゴンを生成する
	if (m_pTitleLogo[0] == NULL)
	{// メモリを確保できる状態である
		m_pTitleLogo[0] = CScene2D::Create(TITLE_LOGO_0_POS, TITLE_LOGO_0_COL, TITLE_LOGO_0_WIDTH, TITLE_LOGO_0_HEIGHT, 0.0f, TITLE_POLYGON_PRIORITY);
		if (m_pTitleLogo[0] != NULL)
		{// ポリゴンが生成できた
			m_pTitleLogo[0]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
		}
	}

	// 黒いポリゴンを生成する
	if (m_pBlackMesh == NULL)
	{// メモリを確保できる状態である
		m_pBlackMesh = CMesh2D::Create(TITLE_BLACK_POS,D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), TITLE_BLACK_WIDTH, TITLE_BLACK_HEIGHT, TITLE_BLACK_XBLOCK, TITLE_BLACK_YBLOCK, 1, 1, TITLE_POLYGON_PRIORITY + 1);
	}
}

//=============================================================================
//    黒いポリゴンの透明度を削る処理
//=============================================================================
void CTitle::AlphaCutBlack(void)
{
	float fCutValue = TITLE_BLACK_ALPHACUT;
	int nXBlock = 0;
	int nYBlock = 0;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	D3DXCOLOR col;

	if (m_pBlackMesh != NULL)
	{// 黒いポリゴンが取得できている
		// 頂点バッファを取得
		pVtxBuff = m_pBlackMesh->GetVtxBuff();
		if (pVtxBuff != NULL)
		{// 頂点バッファが取得できた
			// 頂点情報の設定
			VERTEX_2D *pVtx;

			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 各種値を取得
			nXBlock = m_pBlackMesh->GetXBlock();
			nYBlock = m_pBlackMesh->GetYBlock();
			for (int nCntV = 0; nCntV < nYBlock + 1; nCntV++)
			{// 垂直方向の分割数 + 1だけ繰り返し
				for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
				{// 水平方向の分割数 + 1だけ繰り返し
					col = pVtx[nCntH].col;
					col.a -= fCutValue;
					if (col.a <= 0.0f)
					{// 透明になり切っている
						col.a = 0.0f;
					}
					else
					{// まだ透明になり切っていない
						// 透明度を削る割合を減らす
						fCutValue /= 2;
					}
					pVtx[nCntH].col = col;
				}
				// 透明度を削る割合を戻す
				fCutValue = TITLE_BLACK_ALPHACUT;

				// ポインタを進める
				pVtx += nXBlock + 1;
			}

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();

			if (col.a <= 0.0f)
			{// 全ての頂点の透明度が0になった
				// ロゴを出す待機状態にする
				m_State = STATE_STOP_LOGO;
				m_nCounter = 0;
			}

			// パーティクルを出す
			float fAngle = 0.0f;  // 角度
			float fMoveX = 0.0f;  // X方向の移動量
			float fMoveY = 0.0f;  // Y方向の移動量
			float fWidth = 0.0f;  // 横幅
			float fHeight = 0.0f; // 縦幅
			float fPosYDef = m_ParEmitterPos.y;

			for (int nCntPar = 0; nCntPar < TITLE_PARTICLE_APPEAR; nCntPar++)
			{// 出す分だけ繰り返し
				// 事前にランダムで計算
				fAngle = (rand() % 72 * 0.01f) + (-D3DX_PI * 0.5f);
				fMoveX = sinf(fAngle) * (rand() % 5 + 5.0f);
				fMoveY = cosf(fAngle) * (rand() % 5 + 5.0f);
				fWidth = rand() % 10 + 25.0f;
				fHeight = rand() % 10 + 25.0f;
				CParticle2D *pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(fMoveX, fMoveY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fWidth, fHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.03f), -0.4f, 0.0f, 60, 0, true, TITLE_POLYGON_PRIORITY);
				if (pPar2D != NULL)
				{// メモリが確保できた
					pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_3));
				}
			}

			// 生成位置を動かす
			m_ParEmitterPos.y = fPosYDef;
			m_ParEmitterPos += TITLE_PARTICLE_MOVE;
		}
	}
}

//=============================================================================
//    ロゴの大きさを削る処理
//=============================================================================
void CTitle::LogoCutScale(void)
{
	if (m_pTitleLogo[1] != NULL)
	{// ロゴが生成されている
		m_fCutScaleValue += TITLE_LOGO_1_SCALEDOWN;
		float fLength = m_pTitleLogo[1]->GetLength();

		// ロゴの大きさを小さくする
		fLength -= m_fCutScaleValue;
		m_pTitleLogo[1]->SetLength(fLength);

		if (fLength <= TITLE_LOGO_1_SCALE)
		{// 最終的なロゴの大きさより小さくなっている
			// ロゴの大きさを戻し設定
			fLength = TITLE_LOGO_1_SCALE;
			m_pTitleLogo[1]->SetLength(fLength);

			// フェードアウト状態にする
			m_State = STATE_FADE_OUT;
			if (m_pWhitePoly == NULL)
			{// メモリを確保できる状態である
				m_pWhitePoly = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, TITLE_POLYGON_PRIORITY + 1);
			}
		}
	}
}

//=============================================================================
//    白いポリゴンを使ってフェードアウトする処理
//=============================================================================
void CTitle::FadeOut(void)
{
	if (m_pWhitePoly != NULL)
	{// 白いポリゴンが生成されている
		float fAlpha = m_pWhitePoly->GetCol().a;

		// 白いポリゴンの透明度を増やす
		fAlpha += TITLE_FADEOUT_SPEED;
		if (fAlpha >= 1.0f)
		{// 白色になり切った
			fAlpha = 1.0f;
			m_State = STATE_FADE_STOP;
			m_nCounter = 0;
		}

		// 透明度を設定する
		D3DXCOLOR WhitePolyCol = m_pWhitePoly->GetCol();
		WhitePolyCol.a = fAlpha;
		m_pWhitePoly->SetCol(WhitePolyCol);
	}
}

//=============================================================================
//    白いポリゴンを使ってフェードを止めている時の処理
//=============================================================================
void CTitle::FadeStop(void)
{
	if (m_nCounter >= TITLE_FADESTOP_TIME)
	{// フェードアウトしてからある程度時間が経過した
		// フェードイン状態にする
		m_State = STATE_FADE_IN;

		// カメラの注視点の位置を設定
		CManager::GetCamera()->SetPosR(TITLE_CAMERA_START_POSR);

		// カメラの目的の注視点の位置と現在の位置との差分を設定
		CManager::GetCamera()->SetPosRDiff((TITLE_CAMERA_DEST_POSR - TITLE_CAMERA_START_POSR) * TITLE_CAMERA_MOVESPEED);

		// カメラの視点の高さを設定
		CManager::GetCamera()->SetPosVDest(TITLE_CAMERA_START_POSV_Y);

		// カウンターを戻す
		m_nCounter = 0;

		// BGMを再生する
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM000);

		// マップを生成する
		if (m_pMap == NULL)
		{
			m_pMap = CMap::Create(m_aMapFileName);
		}
	}
}

//=============================================================================
//    白いポリゴンを使ってフェードインする処理
//=============================================================================
void CTitle::FadeIn(void)
{
	if (m_pWhitePoly != NULL)
	{// 白いポリゴンが生成されている
		float fAlpha = m_pWhitePoly->GetCol().a;

		// 白いポリゴンの透明度を増やす
		fAlpha -= TITLE_FADEIN_SPEED;
		if (fAlpha <= 0.0f)
		{// 透明になり切った
		    // 白いポリゴンの破棄
			if (m_pWhitePoly != NULL)
			{
				m_pWhitePoly->Uninit();
				m_pWhitePoly = NULL;
			}

		    // PressEnterの作成
			if (m_pPress == NULL)
			{
				m_pPress = CScene2DPress::Create(TITLE_PRESS_POS, TITLE_PRESS_COL, TITLE_PRESS_WIDTH, TITLE_PRESS_HEIGHT, 0.0f, 80, 100, 4, TITLE_POLYGON_PRIORITY);
				if (m_pPress != NULL)
				{// メモリを確保できた
					m_pPress->BindTexture(m_pTextureManager->GetTexture(LOGOTYPE_MAX));
				}
			}

			// 状態を設定する
			m_State = STATE_OK_CHANGEMODE;

			// 軌跡エフェクトの座標を設定
			m_ParEmitterPos = TITLE_ORBIT_POS_START_0;
			m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_0 - TITLE_ORBIT_POS_START_0) * TITLE_ORBIT_MOVESPEED;

			// 軌跡エフェクトの色をランダムで出力
			m_ParCol.r = rand() % 10 * 0.1f;
			m_ParCol.g = rand() % 10 * 0.1f;
			m_ParCol.b = rand() % 10 * 0.1f;
			m_ParCol.a = 1.0f;

			// 切り替えるタイミングをランダムで出力
			m_nEffectSwicthTiming = (rand() % TITLE_ORBIT_SWITCH_RANDOM) + TITLE_ORBIT_SWITCH_MIN;
		}
		else
		{// まだ透明になり切っていない
			D3DXCOLOR WhitePolyCol = m_pWhitePoly->GetCol();
			WhitePolyCol.a = fAlpha;
			m_pWhitePoly->SetCol(WhitePolyCol);
		}
	}
}

//=============================================================================
//    画面遷移するかどうかチェックする処理
//=============================================================================
void CTitle::CheckModeChange(void)
{
	// カウンターを進める
	m_nModeCounter++;

	if (m_nModeCounter >= TITLE_MODECHANGE_TIMING)
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
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
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
						CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
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
//    カメラのパターンを切り替える処理
//=============================================================================
void CTitle::SwitchCamera(void)
{
	int nSwitchTiming = 0;   // カメラのパターンを切り替えるタイミング
	switch (m_nPatternCamera)
	{// 現在のタイトルカメラのパターンによって処理わけ
	case 0:   // パターン1
		nSwitchTiming = TITLE_CAMERA_SWITCHTIMING_0;
		break;
	case 1:   // パターン2
		nSwitchTiming = TITLE_CAMERA_SWITCHTIMING_1;
		break;
	}

	if (nSwitchTiming > 0)
	{// 切り替えるタイミングが設定された
		if (m_nCounter % nSwitchTiming  == 0)
		{// 切り替えるタイミングになっている
			// カウンターを戻す
			m_nCounter = 0;

			// パターン番号によって設定する値を変える
			D3DXVECTOR3 PosVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fMoveSpeed = 0.0f;
			D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (m_nPatternCamera)
			{// 現在のタイトルカメラのパターンによって処理わけ
			case 0:   // パターン1
				PosVDest = TITLE_CAMERA_0_POSV_Y;
				PosR = TITLE_CAMERA_0_POSR;
				PosRDiff = (TITLE_CAMERA_0_POSRDEST - TITLE_CAMERA_0_POSR);
				fMoveSpeed = TITLE_CAMERA_0_MOVESPEED;
				Rot = TITLE_CAMERA_0_ROT;
				break;
			case 1:   // パターン2
				PosVDest = TITLE_CAMERA_1_POSV_Y;
				PosR = TITLE_CAMERA_1_POSR;
				PosRDiff = (TITLE_CAMERA_1_POSRDEST - TITLE_CAMERA_1_POSR);
				fMoveSpeed = TITLE_CAMERA_1_MOVESPEED;
				Rot = TITLE_CAMERA_1_ROT;
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
//   　軌跡のようなエフェクトを生成する処理
//=============================================================================
void CTitle::CreateOrbit(void)
{
	// カウンターを進める
	m_nEffectCounter++;

	// パーティクルを出す
	CParticle2D *pPar2D = NULL;
	float fAngle = 0.0f;  // 角度
	float fMoveX = 0.0f;  // X方向の移動量
	float fMoveY = 0.0f;  // Y方向の移動量
	float fWidth = 0.0f;  // 横幅
	float fHeight = 0.0f; // 縦幅
	float fPosYDef = m_ParEmitterPos.y;
	D3DXVECTOR3 ParEmitterDestPos;
	switch (m_nPatternOrbit)
	{// 現在の軌跡エフェクトのパターンによって処理わけ
	case 0:   // パターン1
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_0;
		break;
	case 1:   // パターン2
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_1;
		break;
	case 2:   // パターン3
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_2;
		break;
	case 3:   // パターン4
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_3;
		break;
	}

	for (int nCntPar = 0; nCntPar < TITLE_PARTICLE_APPEAR - 3; nCntPar++)
	{// 出す分だけ繰り返し
	    // 事前にランダムで計算
		fAngle = atan2f(ParEmitterDestPos.x - m_ParEmitterPos.x, ParEmitterDestPos.y - m_ParEmitterPos.y) + ((rand() % 72 * 0.01f) - (rand() % 72 * 0.01f));
		fMoveX = sinf(fAngle) * (rand() % 5 + 5.0f);
		fMoveY = cosf(fAngle) * (rand() % 5 + 5.0f);
		fWidth = rand() % 10 + 25.0f;
		fHeight = rand() % 10 + 25.0f;
		pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(fMoveX, fMoveY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fWidth, fHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.03f), -0.4f, 0.0f, 60, 0, true, TITLE_POLYGON_PRIORITY - 1);
		if (pPar2D != NULL)
		{// メモリが確保できた
			pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_3));
		}
	}

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// メモリが確保できた
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// 出す位置を動かす
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// メモリが確保できた
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// 出す位置を動かす
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// メモリが確保できた
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// 出す位置を動かす
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// メモリが確保できた
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// 出す位置を動かす
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	if (m_nEffectSwicthTiming > 0)
	{// タイミングが設定できている
		if (m_nEffectCounter % m_nEffectSwicthTiming == 0)
		{// カウンターが既定の値を超えた
			// カウンターを戻す
			m_nEffectCounter = 0;

			// パターン番号を進める
			m_nPatternOrbit = (m_nPatternOrbit + 1) % TITLE_ORBIT_PATTERN_NUM;

			switch (m_nPatternOrbit)
			{// 現在の軌跡エフェクトのパターンによって処理わけ
			case 0:   // パターン1
				m_ParEmitterPos = TITLE_ORBIT_POS_START_0;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_0 - TITLE_ORBIT_POS_START_0) * TITLE_ORBIT_MOVESPEED;
				break;
			case 1:   // パターン2
				m_ParEmitterPos = TITLE_ORBIT_POS_START_1;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_1 - TITLE_ORBIT_POS_START_1) * TITLE_ORBIT_MOVESPEED;
				break;
			case 2:   // パターン3
				m_ParEmitterPos = TITLE_ORBIT_POS_START_2;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_2 - TITLE_ORBIT_POS_START_2) * TITLE_ORBIT_MOVESPEED;
				break;
			case 3:   // パターン4
				m_ParEmitterPos = TITLE_ORBIT_POS_START_3;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_3 - TITLE_ORBIT_POS_START_3) * TITLE_ORBIT_MOVESPEED;
				break;
			}

			// 軌跡エフェクトの色をランダムで出力
			m_ParCol.r = rand() % 10 * 0.1f;
			m_ParCol.g = rand() % 10 * 0.1f;
			m_ParCol.b = rand() % 10 * 0.1f;

			// 切り替えるタイミングをランダムで出力
			m_nEffectSwicthTiming = (rand() % TITLE_ORBIT_SWITCH_RANDOM) + TITLE_ORBIT_SWITCH_MIN;
		}
	}
	else
	{// 設定されていない
	    // 切り替えるタイミングをランダムで出力
		m_nEffectSwicthTiming = (rand() % TITLE_ORBIT_SWITCH_RANDOM) + TITLE_ORBIT_SWITCH_MIN;
	}
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CTitle::STATE CTitle::GetState(void)
{
	return m_State;
}