//*****************************************************************************
//
//     ロックオン用ポリゴンの処理[lockon_poly.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "lockon_poly.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CLockOnPoly::CLockOnPoly(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CLockOnPoly::~CLockOnPoly()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CLockOnPoly *CLockOnPoly::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, int nPriority)
{
	CLockOnPoly *pLockOnPoly = NULL;        // ロックオン用ポリゴンクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();    // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pLockOnPoly == NULL)
		{// メモリが空になっている
			pLockOnPoly = new CLockOnPoly(nPriority);
			if (pLockOnPoly != NULL)
			{// インスタンスを生成できた
				if (FAILED(pLockOnPoly->Init(pos, col, fWidth, fHeight, fRot, bLighting)))
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

	return pLockOnPoly;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLockOnPoly::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting)
{
	// 各種値の代入
	SetPos(pos);                    // ポリゴンの座標
	SetCol(col);                    // ポリゴンの色
	SetRot(fRot);                   // ポリゴンの向き
	SetWidth(fWidth);               // ポリゴンの幅
	SetHeight(fHeight);             // ポリゴンの高さ
	SetLighting(bLighting);         // ライティングするかしないか

	// 共通の初期化処理
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLockOnPoly::Uninit(void)
{
	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CLockOnPoly::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CLockOnPoly::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

			// 共通の描画処理
			CSceneBillboard::Draw();

			// Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			if (CManager::GetGame()->GetPlayer() != NULL)
			{
				CManager::GetGame()->GetPlayer()->Draw();
			}
		}
	}
}