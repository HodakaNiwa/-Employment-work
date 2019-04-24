//*****************************************************************************
//
//     ポーズの処理[pause.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "fade.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define PAUSEBLACK_COL_INI       (D3DXCOLOR(0.0f,0.0f,0.0f,0.7f))                 // ポーズ背景(黒いポリゴン)の色(初期値)

#define PAUSEBG_TEXTURE_NAME     "data/TEXTURE/GAME/pause_frame.png"              // ポーズの背景のテクスチャファイスパス名
#define PAUSEBG_POS_INI          (D3DXVECTOR3(SCREEN_WIDTH / 2,290.0f,0.0f))      // ポーズの背景の座標
#define PAUSEBG_WIDTH_INI        (150.0f)                                         // ポーズの背景の幅
#define PAUSEBG_HEIGHT_INI       (120.0f)                                         // ポーズの背景の高さ

#define PAUSESELECT_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH / 2,210.0f,0.0f))      // ポーズ項目の座標(初期値)
#define PAUSESELECT_COL_INI      (D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))                 // ポーズ項目の色(初期値)
#define PAUSESELECT_WIDTH_INI    (130.0f)                                         // 選択されているポーズ選択項目の幅
#define PAUSESELECT_HEIGHT_INI   (32.0f)                                          // 選択されているポーズ選択項目ポーズ選択項目の高さ
#define PAUSESELECT_WIDTH_NONE   (90.0f)                                          // 選択されていないポーズ選択項目の幅
#define PAUSESELECT_HEIGHT_NONE  (20.0f)                                          // 選択されていないポーズ選択項目の高さ
#define PAUSESELECT_COL_NONE     (D3DXCOLOR(0.1f,0.1f,0.1f,1.0f))                 // 選択されていないポーズ選択項目の色(初期値)
#define PAUSESELECT_INTERVAL     (80.0f)                                          // ポーズ選択項目同士の間隔

// 操作方法用ポリゴン1つ目
#define PAUSE_OPERATION_POS_0    (D3DXVECTOR3(330.0f,575.0f,0.0f))                // 操作方法用のポリゴンの座標(1つ目)
#define PAUSE_OPERATION_COL_0    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                 // 操作方法用のポリゴンの色(1つ目)
#define PAUSE_OPERATION_WIDTH_0  (330.0f)                                         // 操作方法用のポリゴンの幅(1つ目)
#define PAUSE_OPERATION_HEIGHT_0 (150.0f)                                         // 操作方法用のポリゴンの高さ(1つ目)

// 操作方法用ポリゴン2つ目
#define PAUSE_OPERATION_POS_1    (D3DXVECTOR3(950.0f,575.0f,0.0f))                // 操作方法用のポリゴンの座標(2つ目)
#define PAUSE_OPERATION_COL_1    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                 // 操作方法用のポリゴンの色(2つ目)
#define PAUSE_OPERATION_WIDTH_1  (330.0f)                                         // 操作方法用のポリゴンの幅(2つ目)
#define PAUSE_OPERATION_HEIGHT_1 (150.0f)                                         // 操作方法用のポリゴンの高さ(2つ目)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CPause::CPause()
{
	// 各種値の初期化
	m_pBlackBg = NULL;                     // ポーズ背景用ポリゴン(黒いポリゴン)
	m_pPauseBg = NULL;                     // ポーズの背景用ポリゴン
	m_nSelect = SELECT_CONTINUE;           // 現在選択している項目
	m_nCounter = 0;                        // 描画管理カウンター
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		m_apLogoPause[nCntPause] = NULL;  // ポーズロゴクラス型のポインタ
	}
	for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
	{// 操作方法用のポリゴンの数だけ繰り返し
		m_pOperationInfo[nCntOpe] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPause *CPause::Create(int nPriority)
{
	CPause *pPause = NULL;  // ポーズクラス型のポインタ
	if (pPause == NULL)
	{// メモリが確保できる状態にある
		pPause = new CPause;
		if (pPause != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pPause->Init(nPriority);
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
	return pPause;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPause::Init(int nPriority)
{
	// ポーズ背景用黒いポリゴンの作成
	if (m_pBlackBg == NULL)
	{
		m_pBlackBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), PAUSEBLACK_COL_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, nPriority);
	}

	// ポーズの背景用のポリゴン生成処理
	if (m_pPauseBg == NULL)
	{// メモリが確保できる状態にある
		m_pPauseBg = CScene2D::Create(PAUSEBG_POS_INI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSEBG_WIDTH_INI, PAUSEBG_HEIGHT_INI, 0.0f, nPriority);
	}

	// ポーズ選択項目のポリゴン生成処理
	D3DXVECTOR3 pos = PAUSESELECT_POS_INI;                    // 座標
	D3DXCOLOR col = PAUSESELECT_COL_INI;                      // 色
	float fWidth = PAUSESELECT_WIDTH_INI;                     // 幅
	float fHeight = PAUSESELECT_HEIGHT_INI;                   // 高さ
	CScene2DFlash::STATE state = CScene2DFlash::STATE_SELECT; // 状態
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		if (m_apLogoPause[nCntPause] == NULL)
		{// メモリが確保できる状態にある
			m_apLogoPause[nCntPause] = CScene2DFlash::Create(pos, col, fWidth, fHeight, 0.0f, state, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), 0.01f, 4, nPriority);
		}

		if (nCntPause == 0)
		{// 1回目の繰り返しが終わった
			fWidth = PAUSESELECT_WIDTH_NONE;
			fHeight = PAUSESELECT_HEIGHT_NONE;
			col = PAUSESELECT_COL_NONE;
			state = CScene2DFlash::STATE_NONE;
		}
		pos.y += PAUSESELECT_INTERVAL;
	}

	// 操作方法用のポリゴンの生成
	// 1つ目
	if (m_pOperationInfo[0] == NULL)
	{
		m_pOperationInfo[0] = CScene2D::Create(PAUSE_OPERATION_POS_0, PAUSE_OPERATION_COL_0, PAUSE_OPERATION_WIDTH_0, PAUSE_OPERATION_HEIGHT_0, 0.0f, nPriority);
	}

	// 2つ目
	if (m_pOperationInfo[1] == NULL)
	{
		m_pOperationInfo[1] = CScene2D::Create(PAUSE_OPERATION_POS_1, PAUSE_OPERATION_COL_1, PAUSE_OPERATION_WIDTH_1, PAUSE_OPERATION_HEIGHT_1, 0.0f, nPriority);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		if (m_apLogoPause[nCntPause] != NULL)
		{// メモリが確保されている
			// ポーズロゴの終了処理
			m_apLogoPause[nCntPause]->Uninit();
			m_apLogoPause[nCntPause] = NULL;
		}
	}

	// 背景ポリゴンの開放
	if (m_pPauseBg != NULL)
	{// メモリが確保されている
		m_pPauseBg->Uninit();
		m_pPauseBg = NULL;
	}

	// 黒いポリゴンの開放
	if (m_pBlackBg != NULL)
	{// メモリが確保されている
		m_pBlackBg->Uninit();
		m_pBlackBg = NULL;
	}

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

//=============================================================================
//    更新処理
//=============================================================================
void CPause::Update(void)
{
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		if (m_apLogoPause[nCntPause] != NULL)
		{// メモリが確保されている
		    // ポーズロゴの更新処理
			m_apLogoPause[nCntPause]->Update();
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetKeyboard();                   // キーボードの取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice();   // ジョイスティックの取得

	// 選択項目の処理
	if (pKeyboard->GetTrigger(DIK_W) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_28) == TRUE
		|| pKeyboard->GetRepeat(DIK_W) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_28) == TRUE)
	{// 上方向の入力がされた
		// 選択されていた項目の設定
		m_apLogoPause[m_nSelect]->SetState(CScene2DFlash::STATE_NONE);   // 状態
		m_apLogoPause[m_nSelect]->SetCol(PAUSESELECT_COL_NONE);          // 色
		m_apLogoPause[m_nSelect]->SetWidth(PAUSESELECT_WIDTH_NONE);      // 幅
		m_apLogoPause[m_nSelect]->SetHeight(PAUSESELECT_HEIGHT_NONE);    // 高さ

		// 次に選択される項目の設定
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // 状態
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetCol(PAUSESELECT_COL_INI);            // 色
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetWidth(PAUSESELECT_WIDTH_INI);        // 幅
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetHeight(PAUSESELECT_HEIGHT_INI);      // 高さ

		// 番号を設定
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// 音を鳴らす
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if (pKeyboard->GetTrigger(DIK_S) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_21) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_29) == TRUE
		|| pKeyboard->GetRepeat(DIK_S) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_21) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_29) == TRUE)
	{// 下方向の入力がされた
	    // 選択されていた項目の設定
		m_apLogoPause[m_nSelect]->SetState(CScene2DFlash::STATE_NONE);   // 状態
		m_apLogoPause[m_nSelect]->SetCol(PAUSESELECT_COL_NONE);          // 色
		m_apLogoPause[m_nSelect]->SetWidth(PAUSESELECT_WIDTH_NONE);      // 幅
		m_apLogoPause[m_nSelect]->SetHeight(PAUSESELECT_HEIGHT_NONE);    // 高さ

		// 次に選択される項目の設定
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // 状態
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetCol(PAUSESELECT_COL_INI);            // 色
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetWidth(PAUSESELECT_WIDTH_INI);        // 幅
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetHeight(PAUSESELECT_HEIGHT_INI);      // 高さ

		// 番号を設定
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		// 音を鳴らす
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == TRUE)
	{// 決定ボタンが押された
		// レンダリングクラスの取得
		CRenderer *pRenderer = CManager::GetRenderer();

		switch (m_nSelect)
		{// 現在選択されていた項目によって処理わけ
		case CPause::SELECT_CONTINUE:  // 再開
			pRenderer->SetPause(false);
			break;
		case CPause::SELECT_RETRY:     // やり直し
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			break;
		case CPause::SELECT_QUIT:      // 終了
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			break;
		}

		// ポーズの開放
		pRenderer->ReleasePause();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CPause::Draw(void)
{
	// 黒いポリゴンの描画
	if (m_pBlackBg != NULL)
	{// メモリが確保されている
		m_pBlackBg->Draw();
	}

	// 背景ポリゴンの描画
	if (m_pPauseBg != NULL)
	{// メモリが確保されている
		m_pPauseBg->Draw();
	}

	// 操作方法用のポリゴンの開放
	for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
	{// 操作方法用のポリゴンの数だけ繰り返し
		if (m_pOperationInfo[nCntOpe] != NULL)
		{// メモリが確保されている
			m_pOperationInfo[nCntOpe]->Draw();
		}
	}


	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// ポーズ画面の選択項目の数だけ繰り返し
		if (m_apLogoPause[nCntPause] != NULL)
		{// メモリが確保されている
		    // ポーズロゴの描画
			m_apLogoPause[nCntPause]->Draw();
		}
	}
}

//=============================================================================
//    選択項目用のポリゴンクラスへのポインタ取得処理
//=============================================================================
CScene2DFlash *CPause::GetScene2DFlash(int nIdx)
{
	return m_apLogoPause[nIdx];
}

//=============================================================================
//    ポーズ背景用のポリゴンクラスへのポインタ取得処理
//=============================================================================
CScene2D *CPause::GetPauseBg(void)
{
	return m_pPauseBg;
}

//=============================================================================
//    操作法方法表示用ポリゴンクラスへのポインタ取得処理
//=============================================================================
CScene2D *CPause::GetOpeInfo(int nIdx)
{
	return m_pOperationInfo[nIdx];
}

//=============================================================================
//    選択項目の取得
//=============================================================================
int CPause::GetSelect(void)
{
	return m_nSelect;
}