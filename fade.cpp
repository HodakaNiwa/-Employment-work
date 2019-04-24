//*****************************************************************************
//
//     フェードの処理[fade.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "scene2D.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define FADE_LOGO_CREATE    (true)                               // フェードのロゴポリゴンを生成するかどうか

#define FADE_LOGO_TEXNAME   "data/TEXTURE/COMMON/fade_logo.png"  // フェードに使用する文字テクスチャの相対パス名
#define FADE_STOP_PERIOD    (60)                                 // フェードをどれだけ止めるか
#define FADE_PRIORITY       (MAX_PRIORITY_NUM - 1)               // フェードの描画優先順位
#define FADE_OUT_ALPHA      (0.02f)                              // フェードアウトするときのα値の減算値
#define FADE_IN_ALPHA       (-0.02f)                             // フェードインするときのα値の加算値
#define FADE_COLOR_INI      (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))     // フェードポリゴンの色(初期値)
#define FADE_COLOR_START    (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))     // フェードポリゴンの色(スタート時)

#define FADE_LOGO_POS       (D3DXVECTOR3(960.0f,630.0f,0.0f))    // ロゴポリゴンの座標
#define FADE_LOGO_WIDTH     (300.0f)                             // ロゴポリゴンの幅
#define FADE_LOGO_HEIGHT    (60.0f)                              // ロゴポリゴンの高さ

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture = NULL;    // 文字テクスチャへのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CFade::CFade()
{
	// 各種値のクリア
	m_pScene2D = NULL;                 // フェードに使用するポリゴン
	m_pLogo2D = NULL;                  // 文字ポリゴン
	m_State = STATE_IN;                // 状態はフェードイン状態に
	m_Type = TYPE_MODECHANGE;          // 種類は画面遷移切り替えに
	m_NextMode = CManager::MODE_TITLE; // モードはタイトル状態に
	m_nCounter = 0;                    // 状態を管理するカウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CFade::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // テクスチャの読み込み
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), FADE_LOGO_TEXNAME, &m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    初期化処理
//=============================================================================
void CFade::UnLoad(void)
{
	DIRECT_RELEASE(m_pTexture)
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CFade::Init(void)
{
	// フェードに使用するポリゴンを生成
	if (m_pScene2D == NULL)
	{// メモリが確保できる状態にある
		m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);

		if (FADE_LOGO_CREATE == true)
		{// フェードに使用する文字ポリゴンを生成するならば
			m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
			if (m_pLogo2D != NULL)
			{// 生成できた
				m_pLogo2D->BindTexture(m_pTexture);
			}
		}
	}
	else
	{// メモリが確保できる状態にない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CFade::Uninit(void)
{
	if (m_pScene2D != NULL)
	{// メモリが確保されている
	    // 終了処理
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	if (m_pLogo2D != NULL)
	{// メモリが確保されている
	    // 終了処理
		m_pLogo2D->Uninit();
		m_pLogo2D = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CFade::Update(void)
{
	if (m_State != STATE_NONE)
	{// フェードが使われる状態なら
		if (m_pScene2D != NULL)
		{// フェードに使用するポリゴンが確保されている
			if (m_State == STATE_IN)
			{// フェードイン状態ならば
				FadeIn();
			}
			else if (m_State == STATE_STOP)
			{// フェードを止める状態だったら
				FadeStop();
			}
			else if(m_State == STATE_OUT)
			{// フェードアウト状態ならば
				FadeOut();
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CFade::Draw(void)
{

}

//=============================================================================
//    フェードの設定
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	if (m_State == STATE_NONE)
	{// フェードの状態が使用されていない状態である
		m_nCounter = 0;            // カウンターを戻しておく
		m_NextMode = modeNext;     // 次のモードを設定
		m_State = STATE_OUT;       // 状態をフェードアウト状態に
		m_Type = TYPE_MODECHANGE;  // 種類を画面遷移に設定

		// フェードに使用するポリゴンを生成
		if (m_pScene2D == NULL)
		{// メモリが確保できる状態にある
			D3DXCOLOR col = FADE_COLOR_START;
			col.a = 0.0f;
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_START, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);

			if (FADE_LOGO_CREATE == true)
			{// フェードに使用する文字ポリゴンを生成するならば
				m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
				if (m_pLogo2D != NULL)
				{// 生成できた
					m_pLogo2D->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    フェードの設定(画面切り替え以外の場合)
//=============================================================================
void CFade::SetFade(TYPE type)
{
	if (m_State == STATE_NONE)
	{// フェードの状態が使用されていない状態である
		m_nCounter = 0;           // カウンターを戻しておく
		m_State = STATE_OUT;      // 状態をフェードアウト状態に
		m_Type = type;            // 種類を設定

		// フェードに使用するポリゴンを生成
		if (m_pScene2D == NULL)
		{// メモリが確保できる状態にある
			D3DXCOLOR col = FADE_COLOR_START;
			col.a = 0.0f;
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_START, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);

			if (FADE_LOGO_CREATE == true)
			{// フェードに使用する文字ポリゴンを生成するならば
				m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
				if (m_pLogo2D != NULL)
				{// 生成できた
					m_pLogo2D->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    フェードイン状態の更新処理
//=============================================================================
void CFade::FadeIn(void)
{
	if (m_pScene2D != NULL)
	{// メモリが確保されている
	     // 現在の色を取得
		D3DXCOLOR col = m_pScene2D->GetCol();

		// 透明度を減算する
		col.a += FADE_IN_ALPHA;

		// 色の設定
		m_pScene2D->SetCol(col);

		if (m_pLogo2D != NULL)
		{// 文字が生成されている
		    // 現在の色を取得
			D3DXCOLOR LogoCol = m_pLogo2D->GetCol();

			// 透明度を減算する
			LogoCol.a += FADE_IN_ALPHA;

			// 色の設定
			m_pLogo2D->SetCol(LogoCol);
		}

		if (col.a <= 0.0f)
		{// 透明になり切った
			m_State = STATE_NONE;  // 状態を戻す
			// 使用したポリゴンを開放する
			if (m_pScene2D != NULL)
			{// メモリが確保されている
				// 終了処理
				m_pScene2D->Uninit();
				m_pScene2D = NULL;
			}
			if (m_pLogo2D != NULL)
			{// メモリが確保されている
			    // 終了処理
				m_pLogo2D->Uninit();
				m_pLogo2D = NULL;
			}
		}
	}
}

//=============================================================================
//    フェードを止める状態の更新処理
//=============================================================================
void CFade::FadeStop(void)
{
	m_nCounter++;
	if (m_nCounter >= FADE_STOP_PERIOD)
	{// フェード終了時
	    // 使用したポリゴンを開放する
		if (m_pScene2D != NULL)
		{// メモリが確保されている
		    // 終了処理
			m_pScene2D->Uninit();
			m_pScene2D = NULL;
		}
		if (m_pLogo2D != NULL)
		{// メモリが確保されている
		    // 終了処理
			m_pLogo2D->Uninit();
			m_pLogo2D = NULL;
		}

		if (m_Type == TYPE_MODECHANGE)
		{// 画面遷移をするフェードだったら
		    // 次のモードに遷移
			CManager::SetMode(m_NextMode);
		}
		else if (m_Type == TYPE_SWITCHMAP)
		{// マップ切り替え状態ならば
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// ゲームが取得できた
				pGame->SetMap();
			}
		}

		// フェードに使用するポリゴンを確保しなおす
		if (m_pScene2D == NULL)
		{// メモリが確保できる状態にある
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);
		}

		if (FADE_LOGO_CREATE == true)
		{// フェードに使用する文字ポリゴンを生成するならば
			m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
			if (m_pLogo2D != NULL)
			{// 生成できた
				m_pLogo2D->BindTexture(m_pTexture);
			}
		}

		// フェードイン状態に
		m_State = STATE_IN;
	}
}

//=============================================================================
//    フェードアウト状態の更新処理
//=============================================================================
void CFade::FadeOut(void)
{
	if (m_pScene2D != NULL)
	{// メモリが確保されている
	    // 現在の色を取得
		D3DXCOLOR col = m_pScene2D->GetCol();

		// 透明度を加算する
		col.a += FADE_OUT_ALPHA;

		if (col.a >= 1.0f)
		{// 透明になり切った
			m_State = STATE_STOP;      // 状態をフェードを止める状態に
			col.a = 1.0f;              // 色を戻す
		}

		// 色の設定
		m_pScene2D->SetCol(col);

		if (m_pLogo2D != NULL)
		{// 文字が生成されている
		    // 現在の色を取得
			D3DXCOLOR LogoCol = m_pLogo2D->GetCol();

			// 透明度を加算する
			LogoCol.a += FADE_OUT_ALPHA;

			// 色の設定
			m_pLogo2D->SetCol(LogoCol);
		}
	}
}

//=============================================================================
//    フェードの状態を取得
//=============================================================================
CFade::STATE CFade::GetState(void)
{
	return m_State;
}

//=============================================================================
//    フェードに使用するポリゴンクラスを取得するクラス
//=============================================================================
CScene2D *CFade::GetScene2D(void)
{
	return m_pScene2D;
}