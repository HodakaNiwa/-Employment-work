//*****************************************************************************
//
//     レンダリングの処理[renderer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "pause.h"
#include "camera.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "ui.h"
#include "functionlib.h"
#include "player.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define RENDERER_WINDOW_COLOR (D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0xff))    // 画面クリア時の背景色

#define PAUSE_DRAW            (true)     // ポーズを描画するかどうか
#define PAUSE_PRIORITY        (10)       // ポーズ画面の処理優先順位
#define FEEDBACK_SCALE_INI    (0.995f)   // フィードバックエフェクトの大きさ
#define FEEDBACK_ALPHA_INI    (0.9f)     // フィードバックエフェクトの透明度

//=============================================================================
//    コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	// 各種値のクリア
	m_pPause = NULL;            // ポーズクラスへのポインタ
	m_bPause = false;           // ポーズ中かどうか
	m_pD3DDevice = NULL;        // デバイスへのポインタ
	m_pD3D = NULL;              // Direct3Dオブジェクトへのポインタ
	m_pVtxBuff = NULL;          // 頂点バッファへのポインタ
	m_Target = TARGET_TEXTURE;  // オブジェクトを描画するレンダリングターゲット
	m_fAngle = 0.0f;            // ポリゴン出す角度
	m_fWidth = 0.0f;            // ポリゴンの幅
	m_fHeight = 0.0f;           // ポリゴンの高さ
	m_fLength = 0.0f;           // ポリゴンの長さ
	m_bFeedbackEffect = false;  // フィードバックエフェクトを出すかどうか
	m_nFeedbackCounter = 0;     // フィードバックエフェクトをどれくらい出すか
	m_fFeedbackScale = FEEDBACK_SCALE_INI;
	m_fFeedbackAlpha = FEEDBACK_ALPHA_INI;
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntTex = 0; nCntTex < MAX_RENDERER_TEXTURE; nCntTex++)
	{
		m_apTextureMT[nCntTex] = NULL;   // テクスチャへのポインタ
		m_apRenderMT[nCntTex] = NULL;    // テクスチャに描画するためのインターフェイスへのポインタ
		m_apBuffMT[nCntTex] = NULL;      // 各種バッファへのポインタ
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							            // バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				            // ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				            // ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			            	// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		                // 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						    // デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして24bit、ステンシルバッファとして8bitを使う
	d3dpp.Windowed = bWindow;						                // ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	    // インターバル
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;              // ダブルステンシル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);              // 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                      // Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);             // αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                     // ライティングを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);        // αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // ２番目のアルファ引数

	// オブジェクトレンダリング用のテクスチャを作成
	CreateTexture();

	// テクスチャを張り付けるポリゴンを生成
	CreateVertexBuff();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	// ポーズクラスの破棄
	ReleasePause();

	// レンダリング用のテクスチャを破棄
	ReleaseTexture();

	// 頂点バッファを破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// デバイスの破棄
	DIRECT_RELEASE(m_pD3DDevice);

	// Direct3Dオブジェクトの破棄
	DIRECT_RELEASE(m_pD3D);
}

//=============================================================================
//    更新処理
//=============================================================================
void CRenderer::Update(void)
{
	CManager::MODE mode = CManager::GetMode();  // 現在のモードを取得

	if (mode == CManager::MODE_GAME)
	{// ゲーム中だったら
		if (CManager::GetGame()->GetState() != CGame::STATE_END)
		{// ゲーム中である
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードしていなかったら
				if (CManager::GetKeyboard()->GetTrigger(DIK_P) == true || CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger(CJoyStick::DIJS_BUTTON_11) == TRUE)
				{// ポーズON/OFF
					m_bPause = m_bPause ? false : true;
					if (m_pPause == NULL && m_bPause == true)
					{// ポーズ中にメモリが確保されていない
						m_pPause = CPause::Create(PAUSE_PRIORITY);
						CManager::GetGame()->BindPauseTexture(m_pPause);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
					}
					else if (m_pPause != NULL)
					{// ポーズ解除時にメモリが確保されている
					    // ポーズの開放
						ReleasePause();
					}
				}
			}
		}
	}

	if (m_bPause == false)
	{// ポーズ中でない
	    // シーンクラスの更新処理
		CScene::UpdateAll();
	}
	else
	{// ポーズ中である
		if (m_pPause != NULL)
		{// メモリが確保されている
		    // ポーズの更新処理
			m_pPause->Update();
			CScene::DeathCheck();
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	if (m_Target == TARGET_DEFAULT)
	{// 通常の描画形式なら
		DefaultRender();
	}
	else if (m_Target == TARGET_TEXTURE)
	{// ターゲットがテクスチャならば
		TextureRender();
	}
}

//=============================================================================
//    通常の描画形式
//=============================================================================
void CRenderer::DefaultRender(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3Dによる描画の開始(レンダリングターゲットはテクスチャ)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// カメラの設定
		CManager::GetCamera()->SetCamera();

		// シーンクラスの描画処理
		CScene::DrawAll();

		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲームモード中だったら
			CManager::GetGame()->MiniMapDraw();
			if (m_pPause != NULL)
			{// ポーズの描画
				m_pPause->Draw();

				// 目的のUIだけを表示する
				CUI *pUi = CManager::GetGame()->GetUI();
				if (pUi != NULL)
				{
					pUi->DrawUI_Purpose();
				}
			}
			CManager::GetFade()->Draw();
		}

		// デバック情報表示
		CDebugProc::Draw();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    テクスチャをレンダリングターゲットにした場合の描画形式
//=============================================================================
void CRenderer::TextureRender(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3Dによる描画の開始(レンダリングターゲットはテクスチャ)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		DWORD Lighting;
		D3DXMATRIX mtxViewOrg, mtxProjOrg;
		D3DVIEWPORT9 ViewportOrg;
		LPDIRECT3DSURFACE9 pRenderOrg = NULL;
		LPDIRECT3DSURFACE9 pBuffOrg = NULL;

		// 現在の情報を保存する
		m_pD3DDevice->GetRenderTarget(0, &pRenderOrg);
		m_pD3DDevice->GetDepthStencilSurface(&pBuffOrg);
		m_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);

		// レンダリング先を1枚目のテクスチャに設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);

		// 1枚目のテクスチャをクリア
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), RENDERER_WINDOW_COLOR, 1.0f, 0);

		// カメラの設定
		CManager::GetCamera()->SetCamera();

		m_pD3DDevice->GetTransform(D3DTS_VIEW, &mtxViewOrg);
		m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &mtxProjOrg);
		m_pD3DDevice->GetViewport(&ViewportOrg);

		// シーンクラスの描画処理
		CScene::DrawAll();

		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲームモード中だったら
			CManager::GetGame()->MiniMapDraw();
			if (m_pPause != NULL)
			{// ポーズの描画
				m_pPause->Draw();

				// 目的のUIだけを表示する
				CUI *pUi = CManager::GetGame()->GetUI();
				if (pUi != NULL)
				{
					pUi->DrawUI_Purpose();
				}
			}
			CManager::GetFade()->Draw();
		}

		// ライティングの設定を外す
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// フィードバックエフェクトの描画
		if (m_bFeedbackEffect == true)
		{
			m_nFeedbackCounter--;
			FeedBackRender();

			if (m_nFeedbackCounter <= 0)
			{// 出すフレームがなくなった
				m_nFeedbackCounter = 0;
				m_bFeedbackEffect = false;
			}
		}

		// レンダリング先をバックバッファに設定
		m_pD3DDevice->SetRenderTarget(0, pRenderOrg);
		m_pD3DDevice->SetDepthStencilSurface(pBuffOrg);

		// 1枚目のテクスチャをポリゴンに設定して描画
		PolygonRender();

		// マトリックスの設定を戻す
		m_pD3DDevice->SetViewport(&ViewportOrg);
		m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxViewOrg);
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjOrg);

		// ライティングの設定を戻す
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

		// デバッグ情報の描画
		CDebugProc::Draw();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    フィードバックエフェクトを描画する処理
//=============================================================================
void CRenderer::FeedBackRender(void)
{
	VERTEX_3D *pVtx;
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
	D3DXMATRIX mtxView, mtxProj;

	// 2枚目のテクスチャを1枚目に加算する形で描画
	// ビューポート(描画領域)の設定
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProj);

	// プロジェクションマトリックスを作成
	D3DXMatrixOrthoLH(&mtxProj,
		(float)m_ViewportMT.Width,
		(float)m_ViewportMT.Height,
		1.0f,
		25000.0f);

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&mtxView,
		&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// 3Dポリゴンを描画
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScale, m_fFeedbackScale, m_fFeedbackScale, m_fFeedbackScale);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	m_pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);

	// Zバッファをクリアする
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), RENDERER_WINDOW_COLOR, 1.0f, 0);

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// ポリゴンの描画
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
//    レンダリング用のテクスチャを表示する処理
//=============================================================================
void CRenderer::PolygonRender(void)
{
	VERTEX_3D *pVtx;
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
	D3DXMATRIX mtxView, mtxProj;

	// 2枚目のテクスチャを1枚目に加算する形で描画
	// ビューポート(描画領域)の設定
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProj);

	// プロジェクションマトリックスを作成
	D3DXMatrixOrthoLH(&mtxProj,
		(float)m_ViewportMT.Width,
		(float)m_ViewportMT.Height,
		1.0f,
		25000.0f);

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&mtxView,
		&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// 3Dポリゴンを描画
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	m_pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);

	// Zバッファを使用しない
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ライティングの設定を外す
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// ポリゴンの描画
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// Zバッファを使用する
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// テクスチャのポインタを入れ替える
	LPDIRECT3DTEXTURE9 pTextureMTWk = m_apTextureMT[1];
	LPDIRECT3DSURFACE9 pRenderMTWk = m_apRenderMT[1];
	LPDIRECT3DSURFACE9 pBuffMTWk = m_apBuffMT[1];
	m_apTextureMT[1] = m_apTextureMT[0];
	m_apRenderMT[1] = m_apRenderMT[0];
	m_apBuffMT[1] = m_apBuffMT[0];
	m_apTextureMT[0] = pTextureMTWk;
	m_apRenderMT[0] = pRenderMTWk;
	m_apBuffMT[0] = pBuffMTWk;
}

//=============================================================================
//    デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

//=============================================================================
//    ポーズの開放
//=============================================================================
void CRenderer::ReleasePause(void)
{
	if (m_pPause != NULL)
	{// ポーズ解除時にメモリが確保されている
	    // ポーズの終了処理
		m_pPause->Uninit();

		// メモリの開放
		delete m_pPause;
		m_pPause = NULL;
	}
}

//=============================================================================
//    レンダリング用のテクスチャを開放する処理
//=============================================================================
void CRenderer::ReleaseTexture(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RENDERER_TEXTURE; nCntTex++)
	{
		DIRECT_RELEASE(m_apTextureMT[nCntTex]);
		DIRECT_RELEASE(m_apRenderMT[nCntTex]);
		DIRECT_RELEASE(m_apBuffMT[nCntTex]);
	}
}

//=============================================================================
//    レンダリング用のテクスチャを作成する処理
//=============================================================================
void CRenderer::CreateTexture(void)
{
	// レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			LPDIRECT3DSURFACE9 pRenderOrg = NULL;
			LPDIRECT3DSURFACE9 pBuffOrg = NULL;

			// デフォルト値を保存
			pDevice->GetRenderTarget(0, &pRenderOrg);
			pDevice->GetDepthStencilSurface(&pBuffOrg);

			// 描画領域を設定
			m_ViewportMT.X = 0;
			m_ViewportMT.Y = 0;
			m_ViewportMT.Width = SCREEN_WIDTH;
			m_ViewportMT.Height = SCREEN_HEIGHT;
			m_ViewportMT.MinZ = 0.0f;
			m_ViewportMT.MaxZ = 1.0f;

			for (int nCntTex = 0; nCntTex < MAX_RENDERER_TEXTURE; nCntTex++)
			{
				// レンダリング用のテクスチャを作成
				pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1,
					D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_apTextureMT[nCntTex], NULL);

				// テクスチャのサーフェイス情報を取得
				m_apTextureMT[nCntTex]->GetSurfaceLevel(0, &m_apRenderMT[nCntTex]);

				// テクスチャにレンダリングするときに使うバッファを生成
				pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT,
					D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_apBuffMT[nCntTex], NULL);

				// テクスチャのサーフェイス情報を設定
				pDevice->SetRenderTarget(0, m_apRenderMT[nCntTex]);
				pDevice->SetDepthStencilSurface(m_apBuffMT[nCntTex]);

				// サーフェイス情報をクリアする
				pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

			}

			// デフォルト値に戻す
			pDevice->SetRenderTarget(0, pRenderOrg);
			pDevice->SetDepthStencilSurface(pBuffOrg);
		}
	}
}

//=============================================================================
//    レンダリング用の3Dポリゴンを作成する処理
//=============================================================================
void CRenderer::CreateVertexBuff(void)
{
	// 頂点バッファを生成
	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 6.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 6.0f);
	pVtx[2].pos = D3DXVECTOR3(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 6.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 6.0f);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f - 0.00128f, 0.0f - 0.00072f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f - 0.00072f);
	pVtx[2].tex = D3DXVECTOR2(0.0f - 0.00128f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ポーズの設定
//=============================================================================
void CRenderer::SetPause(bool bPause)
{
	m_bPause = bPause;
}

//=============================================================================
//    ポーズの取得
//=============================================================================
bool CRenderer::GetPause(void)
{
	return m_bPause;
}

//=============================================================================
//    フィードバックエフェクトを出すように設定する
//=============================================================================
void CRenderer::SetFeedBack(int nFrame, float fScale, float fAlpha)
{
	m_bFeedbackEffect = true;
	m_nFeedbackCounter = nFrame;
	m_fFeedbackScale = fScale;
	m_fFeedbackAlpha = fAlpha;
}