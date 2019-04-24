//*****************************************************************************
//
//     空の処理[sky.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sky.h"
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
CSky::CSky(int nPriority, OBJTYPE objType) : CMeshDome(nPriority, objType)
{
	// 各種値のクリア
	m_fRotSpeed = 0.0f;    // 回転するスピード
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSky::~CSky()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSky *CSky::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed, int nPriority)
{
	CSky *pSky = NULL;                     // 空クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSky == NULL)
		{// メモリが空になっている
			pSky = new CSky(nPriority);
			if (pSky != NULL)
			{// インスタンスを生成できた
				if (FAILED(pSky->Init(pos, rot, col, fRadius, nXBlock, nYBlock, fRotSpeed)))
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

	return pSky;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSky::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed)
{
	// 各種値の設定
	SetPos(pos);              // 座標
	SetRot(rot);              // 向き
	SetCol(col);              // 色
	SetRadius(fRadius);       // 半径
	SetXBlock(nXBlock);       // 横の分割数
	SetYBlock(nYBlock);       // 縦の分割数
	m_fRotSpeed = fRotSpeed;  // 回転するスピード

	if (FAILED(CMeshDome::Init()))
	{// 初期化に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSky::Uninit(void)
{
	// 共通の終了処理
	CMeshDome::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSky::Update(void)
{
	D3DXVECTOR3 rot = GetRot();

	rot.y += m_fRotSpeed;
	if (rot.y > D3DX_PI)
	{// 向きが円周率を超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.y += D3DX_PI * 2.0f;
	}

	SetRot(rot);
}

//=============================================================================
//    描画処理
//=============================================================================
void CSky::Draw(void)
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

			// ライティングの設定を外す
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CMeshDome::Draw();

			// ライティングの設定を元に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    法線計算処理
//=============================================================================
void CSky::MakeNormal(void)
{

}