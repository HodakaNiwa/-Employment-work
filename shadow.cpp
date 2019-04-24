//*****************************************************************************
//
//     影の処理[shadow.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SHADOW_PRIORITY       (3)                        // 影の処理優先順位
#define SHADOW_MODEL_FILENAME "data/MODEL/shadow000.x"   // 影モデルのファイル名

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************
LPD3DXMESH CShadow::m_pMesh = NULL;       // メッシュへのポインタ
LPD3DXBUFFER CShadow::m_pBuffMat = NULL;  // マテリアル情報へのポインタ
DWORD CShadow::m_nNumMat = 0;             // マテリアル情報の数

//=============================================================================
//    コンストラクタ
//=============================================================================
CShadow::CShadow(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CShadow *pShadow = NULL;  // モデル(シーン派生)クラス型のポインタ
	if (pShadow == NULL)
	{// メモリが空になっている
		pShadow = new CShadow(SHADOW_PRIORITY);
		if (pShadow != NULL)
		{// インスタンスを生成できた
			if (FAILED(pShadow->Init(pos, rot, scale)))
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

	return pShadow;
}

//=============================================================================
//    影モデルの読み込み処理
//=============================================================================
HRESULT CShadow::Load(void)
{
	// xファイルの読み込み
	D3DXLoadMeshFromX(SHADOW_MODEL_FILENAME,
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=============================================================================
//    影モデルの開放処理
//=============================================================================
void CShadow::UnLoad(void)
{
	// メッシュの開放
	DIRECT_RELEASE(m_pMesh);

	// マテリアル情報の開放
	DIRECT_RELEASE(m_pBuffMat);
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// 各種値の設定
	SetPos(pos);      // 座標
	SetRot(rot);      // 向き
	SetScale(scale);  // 大きさの倍率

	 // モデルの設定
	BindModel(m_pMesh, m_pBuffMat, m_nNumMat, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// 共通の終了処理
	CSceneX::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CShadow::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CShadow::Draw(void)
{
	// レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ステンシルバッファに書き込む設定に
			pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

			// Zバッファに書き込まない設定に
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			// 色を書き込まない設定に
			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

			// ステンシルマスク値の設定
			pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

			// ステンシルテストの設定
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);        // 必ずステンシルテストに合格する
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);    // 両方のテストに合格した際
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);   // Zテストにのみ失敗した場合
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);    // 両方失敗した場合

			// 表面をステンシルバッファに描画
			CSceneX::Draw();

			// カリングの設定を反対にする(表面カリング)
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

			// 参照値を設定する
			pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);                   // ステンシルテストのための参照値
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);     // 参照値より低いものをステンシルテストを合格とする
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);    // 両方のテストに合格した際
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);   // Zテストにのみ失敗した場合
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);    // ステンシルテストに失敗した場合

			// 裏面をステンシルバッファに描画
			CSceneX::Draw();

			// カリングの設定を戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);             // 裏面カリング

			// 色を書き込む設定に
			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);

			// 半透明板ポリゴンを画面の最前面に描画しステンシルバッファの値でテストし切り取る(切り取った部分が影となる)
			pDevice->SetRenderState(D3DRS_STENCILREF, 0x2);                   // ステンシルテストのための参照値
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);     // 参照値以下を描画しない
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);    // 両方合格した場合
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);   // Zテストに失敗した場合
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);    // ステンシルテストに失敗した場合

			// ポリゴン描画用
			struct SHADOW_POLYGON
			{
				D3DXVECTOR4 VertexPos;
				D3DCOLOR VertexCol;
			};
			SHADOW_POLYGON ShadowPolygon[4];

			// 頂点座標の設定
			ShadowPolygon[0].VertexPos = D3DXVECTOR4(0, 0, 0, 1);
			ShadowPolygon[1].VertexPos = D3DXVECTOR4(SCREEN_WIDTH, 0, 0, 1);
			ShadowPolygon[2].VertexPos = D3DXVECTOR4(0, SCREEN_HEIGHT, 0, 1);
			ShadowPolygon[3].VertexPos = D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1);

			// 頂点カラーの設定
			ShadowPolygon[0].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			ShadowPolygon[1].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			ShadowPolygon[2].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			ShadowPolygon[3].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

			// ポリゴン描画
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, ShadowPolygon, sizeof(SHADOW_POLYGON));

			// レンダリングステートを元に戻す
			pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);   // ステンシルテストをしない設定に
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);          // Zバッファを使用する設定に
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	   // Zバッファに書き込む設定に

			// ステンシルバッファをクリアしておく
			pDevice->Clear(0, NULL, (D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);
		}
	}
}