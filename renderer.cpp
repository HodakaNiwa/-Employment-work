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
#include "functionlib.h"

#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define PAUSE_DRAW     (true)     // ポーズを描画するかどうか
#define PAUSE_PRIORITY (10)       // ポーズ画面の処理優先順位

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
	m_pZBufferSurface = NULL;   // Zバッファへのポインタ
	m_pTexture = NULL;          // テクスチャへのポインタ
	m_pVtxBuff = NULL;          // 頂点バッファへのポインタ
	m_Target = TARGET_DEFAULT;  // オブジェクトを描画するレンダリングターゲット
	m_fRot = 0.0f;              // ポリゴンの向き
	m_fAngle = 0.0f;            // ポリゴン出す角度
	m_fWidth = 0.0f;            // ポリゴンの幅
	m_fHeight = 0.0f;           // ポリゴンの高さ
	m_fLength = 0.0f;           // ポリゴンの長さ
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
	// D3DXCreateTexture関数で作成します
	// 第１引数 : 描画デバイスへのポインタ(基本的に上で作成済みのデバイスを使用)
	// 第２引数 : 作成するテクスチャの幅(画面幅に合わせて固定)
	// 第３引数 : 作成するテクスチャの高さ(画面高さに合わせて固定)
	// 第４引数 : ミップマップレベル(必ず１を指定すること！)
	// 第５引数 : レンダリング可能なテクスチャのフォーマット(基本的に[ D3DFMT_A8R8G8B8 ]でOK)
	// 第６引数 : メモリ管理方法(フォーマットにもよるが基本的に[ D3DPOOL_DEFAULT ]でOK)
	// 第７引数 : テクスチャへのポインタ(空のポインタを用意すること)
	if (FAILED(D3DXCreateTexture(m_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1,
	D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture)))
	{// 作成に失敗した
		return E_FAIL;
	}

	// テクスチャレンダリング専用のZバッファを作成
	// テクスチャのサーフェイス情報を取得する(Zバッファの作成に必要)
	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc(0, &desc);

	// CreateDepthStencilSurface関数で作成します
	// 第１引数 : Z値テクスチャの実質幅(上で作成したテクスチャのサーフェイス情報から抜き取る)
	// 第２引数 : Z値テクスチャの実質高さ(上で作成したテクスチャのサーフェイス情報から抜き取る)
	// 第３引数 : 深度バッファのフォーマット(デバイスのフォーマットと揃えること！)
	// 第４引数 : マルチサンプリングバッファタイプ(基本的には[ D3DMULTISAMPLE_NONE ]でOK)
	// 第５引数 : 品質レベル(基本的に0を指定)
	// 第６引数 : Zバッファの破棄を有効にするかどうか(今回は破棄できるようにしないといけないのでTRUEを指定)
	// 第７引数 : LPDIRECT3DSURFACE9型のポインタ(今回はメンバ変数に格納しておく)
	// 第８引数 : 基本的にNULLでOK
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface(desc.Width, desc.Height,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZBufferSurface, NULL)))
	{// 作成に失敗した
		return E_FAIL;
	}

	// テクスチャ設定用の板ポリゴンを作成する
	// 頂点バッファを生成
	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	m_fWidth = SCREEN_WIDTH / 2;
	m_fHeight = SCREEN_HEIGHT / 2;

	// オフセットの長さを求める
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// オフセットの角度を求める
	m_fAngle = atan2f(m_fWidth, -m_fHeight);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// DirectXラスタライズのルール上左上に0.5fずらすことで描画を補正する
	// X座標
	XPos[0] -= 0.5f;
	XPos[2] -= 0.5f;

	// Y座標
	YPos[0] -= 0.5f;
	YPos[1] -= 0.5f;
	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	}

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	// ポーズクラスの破棄
	ReleasePause();

	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// テクスチャの破棄
	DIRECT_RELEASE(m_pTexture);

	// Zバッファの破棄
	DIRECT_RELEASE(m_pZBufferSurface);

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
	LPDIRECT3DSURFACE9 pBackBufferDef;  // デフォルトのバックバッファへのポインタ(保存用)
	LPDIRECT3DSURFACE9 pZBufferDef;     // デフォルトのZバッファへのポインタ(保存用)

	// 現在のバックバッファ、Zバッファを保存する
	m_pD3DDevice->GetRenderTarget(0, &pBackBufferDef);
	m_pD3DDevice->GetDepthStencilSurface(&pZBufferDef);

	// テクスチャのサーフェイス情報を入手
	LPDIRECT3DSURFACE9 pSurface = NULL;
	m_pTexture->GetSurfaceLevel(0, &pSurface);

	// レンダリングターゲットを自前で作成したテクスチャに設定(その際にZバッファも作成したテクスチャ専用のものに切り替える)
	m_pD3DDevice->SetRenderTarget(0, pSurface);
	m_pD3DDevice->SetDepthStencilSurface(m_pZBufferSurface);
	DIRECT_RELEASE(pSurface);

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
			}
			CManager::GetFade()->Draw();
		}

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// レンダリングターゲットをデフォルトのバックバッファとZバッファに戻す
	m_pD3DDevice->SetRenderTarget(0, pBackBufferDef);
	m_pD3DDevice->SetDepthStencilSurface(pZBufferDef);
	DIRECT_RELEASE(pBackBufferDef);
	DIRECT_RELEASE(pZBufferDef);

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3Dによる描画の開始(レンダリングターゲットはデフォルトのバックバッファ)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		DWORD LightingDef;   // ライティングの設定
		DWORD FillMode;      // 描画方法

		// ライティングを無効にしておく
		m_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &LightingDef);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

		// 通常の描画方法に戻しておく
		m_pD3DDevice->GetRenderState(D3DRS_FILLMODE, &FillMode);
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		// 板ポリゴンにさきほどオブジェクトを描画したテクスチャを設定し板ポリゴンを描画する
		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		m_pD3DDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ライティングを戻しておく
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, LightingDef);

		// 描画方法を戻しておく
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, FillMode);

		// デバック情報表示
		CDebugProc::Draw();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
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