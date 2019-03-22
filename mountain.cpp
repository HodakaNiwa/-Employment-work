//*****************************************************************************
//
//     山の処理[mountain.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mountain.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CMountain::CMountain(int nPriority, OBJTYPE objType) : CMeshCylinder(nPriority, objType)
{
	// 各種値のクリア
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMountain::~CMountain()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMountain *CMountain::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CMountain *pMountain = NULL;           // 山クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pMountain == NULL)
		{// メモリが空になっている
			pMountain = new CMountain(nPriority);
			if (pMountain != NULL)
			{// インスタンスを生成できた
				if (FAILED(pMountain->Init(pos, rot, col, fHeight, fRadius, nXBlock, nYBlock)))
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

	return pMountain;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMountain::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock)
{
	// 各種値の設定
	SetPos(pos);         // 座標
	SetRot(rot);         // 向き
	SetCol(col);         // 色
	SetHeight(fHeight);  // 高さ
	SetRadius(fRadius);  // 半径
	SetXBlock(nXBlock);  // 横の分割数
	SetYBlock(nYBlock);  // 縦の分割数

	if (FAILED(CMeshCylinder::Init()))
	{// 初期化に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMountain::Uninit(void)
{
	// 共通の終了処理
	CMeshCylinder::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMountain::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMountain::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;

			// アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
			pDevice->SetRenderState(D3DRS_ALPHAREF, 200);             // 透明度が200
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

			// ライティングの設定を外す
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CMeshCylinder::Draw();

			// ライティングの設定を元に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // 無効に
		}
	}
}

//=============================================================================
//    法線計算処理
//=============================================================================
void CMountain::MakeNormal(void)
{

}