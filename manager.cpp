//*****************************************************************************
//
//     マネージャーの処理[manager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "debuglog.h"
#include "scene.h"

#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#ifdef _DEBUG
#define MANAGER_BGM_PLAY                  (false)    // BGMを再生するかどうか

#define MANAGER_DEBUGKEY_DEBUGPROC_CHANGE (DIK_F1)   // デバッグ表示するかしないかを切り替えるキー
#define MANAGER_DEBUGKEY_LIGHTING_CHANGE  (DIK_F2)   // ライティングON/OFFを切り替えるキー
#define MANAGER_DEBUGKEY_WIREFRAME_CHANGE (DIK_F3)   // ワイヤーフレームON/OFFを切り替えるキー
#define MANAGER_DEBUGKEY_UPDATE_STOP      (DIK_F4)   // アップデートを停止するキー
#define MANAGER_DEBUGKEY_MODECHANGE       (DIK_F7)   // 画面遷移をするキー
#define MANAGER_DEBUGKEY_RELEASEALL       (DIK_F11)  // 全てのオブジェクトを破棄するキー
#endif

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CRenderer         *CManager::m_pRenderer = NULL;     // レンダリングクラス型のポインタ
CCamera           *CManager::m_pCamera = NULL;       // カメラクラス型のポインタ
CMapCamera        *CManager::m_pMapCamera = NULL;    // マップカメラクラス型のポインタ
CLight            *CManager::m_pLight = NULL;        // ライトクラス型のポインタ
CInputKeyboard    *CManager::m_pKeyboard = NULL;     // キーボードクラス型のポインタ
CInputMouse       *CManager::m_pMouse = NULL;        // マウスクラス型のポインタ
CInputJoyStick    *CManager::m_pJoyStick = NULL;     // ジョイスティッククラス型のポインタ
CXInput           *CManager::m_pXInput = NULL;       // XInputクラス型のポインタ
CSound            *CManager::m_pSound = NULL;        // サウンドクラス型のポインタ
CDebugProc        *CManager::m_pDebugProc = NULL;    // デバッグ情報クラス型のポインタ

CFade             *CManager::m_pFade = NULL;         // フェードクラス型のポインタ
CTitle            *CManager::m_pTitle = NULL;        // タイトルクラス型のポインタ
CTutorial         *CManager::m_pTutorial = NULL;     // チュートリアルクラス型のポインタ
CGame             *CManager::m_pGame = NULL;         // ゲームクラス型のポインタ
CResult           *CManager::m_pResult = NULL;       // リザルトクラス型のポインタ
CRanking          *CManager::m_pRanking = NULL;      // ランキングクラス型のポインタ

#ifdef _DEBUG
CManager::MODE    CManager::m_Mode = MODE_TITLE;     // 現在のモード
#else
CManager::MODE    CManager::m_Mode = MODE_TITLE;     // 現在のモード
#endif

#ifdef _DEBUG
bool g_bUpdate = true;                               // アップデートするかしないか
bool g_bLighting = true;                             // ライティングON/OFFを切り替えるフラグ
bool g_bWireFrame = false;                           // ワイヤーフレームで描画するかしないか
#endif

//=============================================================================
//    コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダリングの作成
	if (m_pRenderer == NULL)
	{// メモリを確保できる状態にある
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// メモリを確保できた
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))  // FALSEにするとフルスクリーン
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// カメラの作成
	if (m_pCamera == NULL)
	{// メモリを確保できる状態にある
		m_pCamera = new CCamera;
		if (m_pCamera != NULL)
		{// メモリを確保できた
			if (FAILED(m_pCamera->Init()))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// マップカメラの作成
	if (m_pMapCamera == NULL)
	{// メモリを確保できる状態にある
		m_pMapCamera = new CMapCamera;
		if (m_pMapCamera != NULL)
		{// メモリを確保できた
			if (FAILED(m_pMapCamera->Init()))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// ライトの作成
	if (m_pLight == NULL)
	{// メモリを確保できる状態にある
		m_pLight = new CLight;
		if (m_pLight != NULL)
		{// メモリを確保できた
			if (FAILED(m_pLight->Init()))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// キーボードの作成
	if (m_pKeyboard == NULL)
	{// メモリが確保できる状態にある
		m_pKeyboard = new CInputKeyboard;
		if (m_pKeyboard != NULL)
		{// メモリが確保できた
			if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// マウスの作成
	if (m_pMouse == NULL)
	{// メモリが確保できる状態にある
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// メモリが確保できた
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// ジョイスティックの作成
	if (m_pJoyStick == NULL)
	{// メモリが確保できる状態にある
		m_pJoyStick = new CInputJoyStick(1);
		if (m_pJoyStick != NULL)
		{// メモリが確保できた
			m_pJoyStick->Init(hInstance, hWnd);
		}
	}

	// XInputの作成
	if (m_pXInput == NULL)
	{// メモリが確保できる状態にある
		m_pXInput = new CXInput;
		if (m_pXInput != NULL)
		{// メモリが確保できた
			m_pXInput->Init();
		}
	}

	// サウンドの作成
	if (m_pSound == NULL)
	{// メモリが確保できる状態にある
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{// メモリが確保できた
			if (FAILED(m_pSound->Init(hWnd)))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリが確保できなかった
		return E_FAIL;
	}

	// デバッグ情報クラスの作成
	if (m_pDebugProc == NULL)
	{// メモリが確保できる状態にある
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{// メモリが確保できた
			m_pDebugProc->Init();
		}
	}

	// モードの設定
	SetMode(m_Mode);

	// フェードに使用するテクスチャを読み込み
	CFade::Load();

	// フェードの作成
	if (m_pFade == NULL)
	{// メモリが確保できる状態にある
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{// メモリが確保できた
			if (FAILED(m_pFade->Init()))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリが確保できなかった
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// フェードに使用するテクスチャを開放
	CFade::UnLoad();

	// フェードの破棄
	if (m_pFade != NULL)
	{// メモリが確保されている
	    // フェードの終了処理
		m_pFade->Uninit();

		// フェードの開放
		delete m_pFade;
		m_pFade = NULL;
	}

	// デバッグ情報の破棄
	if (m_pDebugProc != NULL)
	{// メモリが確保されている
	     // デバッグ情報の終了処理
		m_pDebugProc->Uninit();

		// メモリの開放
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// キーボードの破棄
	if (m_pKeyboard != NULL)
	{// メモリが確保されている
	    // キーボードの終了処理
		m_pKeyboard->Uninit();

		// メモリの開放
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	// マウスの破棄
	if (m_pMouse != NULL)
	{// メモリが確保されている
	    // マウスの終了処理
		m_pMouse->Uninit();

		// メモリの開放
		delete m_pMouse;
		m_pMouse = NULL;
	}

	// ジョイスティックの破棄
	if (m_pJoyStick != NULL)
	{// メモリが確保されている
	    // ジョイスティックの終了処理
		m_pJoyStick->Uninit();

		// メモリの開放
		delete m_pJoyStick;
		m_pJoyStick = NULL;
	}

	// XInputの破棄
	if (m_pXInput != NULL)
	{// メモリが確保されている
		// XInputの終了処理
		m_pXInput->Uninit();

		// メモリの開放
		delete m_pXInput;
		m_pXInput = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{// メモリが確保されている
	    // ライトの終了処理
		m_pLight->Uninit();

		// メモリの開放
		delete m_pLight;
		m_pLight = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{// メモリが確保されている
	    // カメラの終了処理
		m_pCamera->Uninit();

		// メモリの開放
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// マップカメラの破棄
	if (m_pMapCamera != NULL)
	{// メモリが確保されている
     	// マップカメラの終了処理
		m_pMapCamera->Uninit();

		// メモリの開放
		delete m_pMapCamera;
		m_pMapCamera = NULL;
	}

	// レンダリングの破棄
	if (m_pRenderer != NULL)
	{// メモリが確保されている
		// レンダリングの終了処理
		m_pRenderer->Uninit();

		// メモリの開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// シーンのインスタンスをすべて破棄する
	CScene::ReleaseAll();

	// サウンドの破棄
	if (m_pSound != NULL)
	{// メモリが確保されている
	 // サウンドの終了処理
		m_pSound->Uninit();

		// メモリの開放
		delete m_pSound;
		m_pSound = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CManager::Update(void)
{
	// 操作方法を描画
#ifdef _DEBUG  // デバッグ時の処理
	CDebugProc::Print("( 操作方法一覧 )\n");
	CDebugProc::Print("デバッグ表示切替           : [ F1 ]\n");
	CDebugProc::Print("ライティング(ON/OFF)       : [ F2 ]\n");
	CDebugProc::Print("ワイヤーフレーム(ON/OFF)   : [ F3 ]\n");
	CDebugProc::Print("更新を停止する/開始する    : [ F4 ]\n");
	CDebugProc::Print("画面遷移                   : [ F7 ]\n");
	CDebugProc::Print("すべてのオブジェクトを削除 : [ F11 ]\n");

	if (m_pKeyboard != NULL)
	{// キーボードが生成されている
		if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_DEBUGPROC_CHANGE) == true)
		{// キーボードのF1キーが押された
			CDebugProc::SetDisp(CDebugProc::GetDisp() ? false : true);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_LIGHTING_CHANGE) == true)
		{// キーボードのF2キーが押された
			g_bLighting = g_bLighting ? false : true;
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_LIGHTING, g_bLighting);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_WIREFRAME_CHANGE) == true)
		{// キーボードのF3キーが押された
			g_bWireFrame = g_bWireFrame ? false : true;
			if (g_bWireFrame == true)
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_UPDATE_STOP) == true)
		{// キーボードのF4キーが押された
			g_bUpdate = g_bUpdate ? false : true;
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_RELEASEALL) == true)
		{// キーボードのF11キーが押された
			CScene::ReleaseAll();
		}

		if (g_bUpdate == false && m_pKeyboard->GetTrigger(DIK_1) == true)
		{
			CScene::UpdateAll();
		}
	}

	if (m_pKeyboard != NULL)
	{// メモリが確保されている
	    // キーボードの更新処理
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// メモリが確保されている
	    // マウスの更新処理
		m_pMouse->Update();
	}

	if (m_pCamera != NULL)
	{// メモリが確保されている
	    // カメラの更新処理
		m_pCamera->Update();
	}

	if (m_pLight != NULL)
	{// メモリが確保されている
	    // ライトの更新処理
		m_pLight->Update();
	}

	if (g_bUpdate == true)
	{// 更新する状態ならば
		if (m_pJoyStick != NULL)
		{// メモリが確保されている
		    // ジョイスティックの更新処理
			m_pJoyStick->Update();
		}

		if (m_pXInput != NULL)
		{// メモリが確保されている
		    // XInputの更新処理
			m_pXInput->Update();
		}

		if (m_pFade != NULL)
		{// メモリが確保されている
		    // フェードの更新処理
			m_pFade->Update();
		}

		if (m_pRenderer != NULL)
		{// メモリが確保されている
		    // レンダリングの更新処理
			m_pRenderer->Update();
		}
	}

#else // リリース時の処理
	if (m_pKeyboard != NULL)
	{// メモリが確保されている
	    // キーボードの更新処理
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// メモリが確保されている
	    // マウスの更新処理
		m_pMouse->Update();
	}

	if (m_pJoyStick != NULL)
	{// メモリが確保されている
	    // ジョイスティックの更新処理
		m_pJoyStick->Update();
	}

	if (m_pXInput != NULL)
	{// メモリが確保されている
	    // XInputの更新処理
		m_pXInput->Update();
	}

	if (m_pFade != NULL)
	{// メモリが確保されている
	    // フェードの更新処理
		m_pFade->Update();
	}

	if (m_pLight != NULL)
	{// メモリが確保されている
	    // ライトの更新処理
		m_pLight->Update();
	}

	if (m_pCamera != NULL)
	{// メモリが確保されている
	    // カメラの更新処理
		m_pCamera->Update();
	}

	if (m_pRenderer != NULL)
	{// メモリが確保されている
	    // レンダリングの更新処理
		m_pRenderer->Update();
	}
#endif
}

//=============================================================================
//    描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// メモリが確保されている
		m_pRenderer->Draw();
	}
}

//=============================================================================
//    次のモードを設定する
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_Mode)
	{// 現在のモードによって処理わけ
	case MODE_TITLE:   // タイトル画面
		if (m_pTitle != NULL)
		{// メモリが確保されている
			m_pTitle = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM000);
		}
		break;
	case MODE_TUTORIAL: // チュートリアル画面
		if (m_pTutorial != NULL)
		{// メモリが確保されている
			m_pTutorial = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM001);
		}
		break;
	case MODE_GAME:    // ゲーム画面
		if (m_pGame != NULL)
		{// メモリが確保されている
			m_pGame = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM002);
			StopBgm(CSound::SOUND_LABEL_BGM003);
		}
		break;
	case MODE_RESULT:  // リザルト画面
		if (m_pResult != NULL)
		{// メモリが確保されている
			m_pResult = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM004);
			StopBgm(CSound::SOUND_LABEL_BGM005);
		}
		break;
	case MODE_RANKING: // ランキング画面
		if (m_pRanking != NULL)
		{// メモリが確保されている
			m_pRanking = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM006);
		}
		break;
	}

	// すべてのオブジェクトを破棄
	CScene::ReleaseAll();

	// 現在のモード更新
	m_Mode = mode;

	switch (mode)
	{// 次のモードによって処理わけ
	case MODE_TITLE:   // タイトル画面
		if (m_pTitle == NULL)
		{// メモリが確保されている
			m_pTitle = CTitle::Create();
		}
		break;
	case MODE_TUTORIAL:  // チュートリアル画面
		if (m_pTutorial == NULL)
		{// メモリが確保されている
			m_pTutorial = CTutorial::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM001);
		}
		break;
	case MODE_GAME:      // ゲーム画面
		if (m_pGame == NULL)
		{// メモリが確保されている
			m_pGame = CGame::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM002);
		}
		break;
	case MODE_RESULT:    // リザルト画面
		if (m_pResult == NULL)
		{// メモリが確保されている
			m_pResult = CResult::Create();
		}
		break;
	case MODE_RANKING:   // ランキング画面
		if (m_pRanking == NULL)
		{// メモリが確保されている
			m_pRanking = CRanking::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM006);
		}
		break;
	}

	// ポーズ状態を解除しておく
	if (m_pRenderer != NULL)
	{
		m_pRenderer->SetPause(false);
	}
}

//=============================================================================
//    レンダリングクラスのポインタを取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
//    カメラクラスのポインタを取得
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//=============================================================================
//    マップカメラクラスのポインタを取得
//=============================================================================
CMapCamera *CManager::GetMapCamera(void)
{
	return m_pMapCamera;
}

//=============================================================================
//    ライトクラスのポインタを取得
//=============================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;
}

//=============================================================================
//    キーボードクラスのポインタを取得
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pKeyboard;
}

//=============================================================================
//    マウスクラスのポインタを取得
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
//    ジョイスティッククラスのポインタを取得
//=============================================================================
CInputJoyStick *CManager::GetJoyStick(void)
{
	return m_pJoyStick;
}

//=============================================================================
//    XInputクラスのポインタを取得
//=============================================================================
CXInput *CManager::GetXInput(void)
{
	return m_pXInput;
}

//=============================================================================
//    サウンドクラスのポインタを取得
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
//    フェードクラスのポインタを取得
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
//    現在のモードを取得する
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
//   タイトルクラスのポインタを取得
//=============================================================================
CTitle *CManager::GetTitle(void)
{
	return m_pTitle;
}

//=============================================================================
//   チュートリアルクラスのポインタを取得
//=============================================================================
CTutorial *CManager::GetTutorial(void)
{
	return m_pTutorial;
}

//=============================================================================
//    ゲームクラスのポインタを取得
//=============================================================================
CGame *CManager::GetGame(void)
{
	return m_pGame;
}

//=============================================================================
//    リザルトクラスのポインタを取得
//=============================================================================
CResult *CManager::GetResult(void)
{
	return m_pResult;
}

//=============================================================================
//    ランキングクラスのポインタを取得
//=============================================================================
CRanking *CManager::GetRanking(void)
{
	return m_pRanking;
}

//=============================================================================
//    BGMの再生
//=============================================================================
void CManager::PlayBgm(CSound::SOUND_LABEL soundLabel)
{
#ifdef _DEBUG
	if (MANAGER_BGM_PLAY == true)
	{// BGMを再生する状態である
		if (m_pSound != NULL)
		{// サウンドクラスが生成されている
			m_pSound->PlaySound(soundLabel);

		}
	}

#else
	if (m_pSound != NULL)
	{// サウンドクラスが生成されている
		m_pSound->PlaySound(soundLabel);
	}
#endif
}

//=============================================================================
//    BGMの停止
//=============================================================================
void CManager::StopBgm(CSound::SOUND_LABEL soundLabel)
{
#ifdef _DEBUG
	if (MANAGER_BGM_PLAY == true)
	{// BGMを再生する状態である
		if (m_pSound != NULL)
		{// サウンドクラスが生成されている
			m_pSound->StopSound(soundLabel);

		}
	}

#else
	if (m_pSound != NULL)
	{// サウンドクラスが生成されている
		m_pSound->StopSound(soundLabel);
	}
#endif
}

#ifdef _DEBUG  // デバッグ時の処理

//=============================================================================
//    更新しているかどうかを返す処理
//=============================================================================
bool GetUpdate(void)
{
	return g_bUpdate;
}

#endif