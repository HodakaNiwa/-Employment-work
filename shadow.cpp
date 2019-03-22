//*****************************************************************************
//
//     影(スプライト)の処理[shadow.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SHADOW_TEXUTRE_NAME "data/TEXTURE/COMMON/shadow000.png"   // 影テクスチャへの相対パス名

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CShadow::m_pShadowTexture = NULL;   // テクスチャへのポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CShadow::CShadow(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// 各種値のクリア
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
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CShadow *pShadow = NULL;               // クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pShadow == NULL)
		{// メモリが空になっている
			pShadow = new CShadow(nPriority);
			if (pShadow != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pShadow->SetPos(pos);         // ポリゴンの座標
				pShadow->SetRot(rot);         // ポリゴンの向き
				pShadow->SetCol(col);         // ポリゴンの色
				pShadow->SetWidth(fWidth);    // ポリゴンの幅
				pShadow->SetHeight(fHeight);  // ポリゴンの高さ

				if (FAILED(pShadow->Init()))
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

	return pShadow;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    テクスチャ読み込み処理
//=============================================================================
HRESULT CShadow::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // テクスチャの読み込み
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), SHADOW_TEXUTRE_NAME, &m_pShadowTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャ開放処理
//=============================================================================
void CShadow::UnLoad(void)
{
	DIRECT_RELEASE(m_pShadowTexture)
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CShadow::Init(void)
{
	// 影テクスチャを設定する
	BindTexture(m_pShadowTexture);

	if (FAILED(CScene3D::Init()))
	{// 初期化に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// 共通の終了処理
	CScene3D::Uninit();
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
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 減算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // 透明度が0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

			// 共通の処理
			CScene3D::Draw();

			// アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// 減算合成の設定を戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CShadow::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オフセットの長さを求める
	float fLength = 0.0f;
	float fAngle = 0.0f;
	fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));

	// オフセットの角度を求める
	fAngle = atan2f(GetWidth(), GetHeight());

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, 0.0f, cosf(fAngle) * fLength);
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * fLength, 0.0f, cosf(fAngle) * fLength);
	pVtx[2].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, 0.0f, -cosf(fAngle) * fLength);
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fLength, 0.0f, -cosf(fAngle) * fLength);

	// 法線を代入
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファの設定
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    影の頂点を起伏に合わせる処理
//=============================================================================
void CShadow::SetVertexHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pLandNormal)
{
	// 座標を設定
	SetPos(pos);

	// 頂点情報の設定
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファを取得できた
		if (pLandNormal != NULL)
		{// メモリが確保されている
			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVer = 0; nCntVer < 4; nCntVer++)
			{// 影の頂点数分繰り返し
				float DotX = (pLandNormal->x * pVtx[nCntVer].pos.x);       // X成分の内積を計算
				float DotZ = (pLandNormal->z * pVtx[nCntVer].pos.z);       // Z成分の内積を計算
				float Dot = -DotX - DotZ;                                  // X成分とZ成分の内積同士を引く
				pVtx[nCntVer].pos.y = (Dot / pLandNormal->y);              // 引いたものをY成分の法線で割る
			}

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();
		}
	}
}