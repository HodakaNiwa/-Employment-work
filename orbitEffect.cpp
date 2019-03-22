//*****************************************************************************
//
//     軌跡エフェクトの処理[orbitEffect.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "orbitEffect.h"
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
COrbitEffect::COrbitEffect(int nPriority, OBJTYPE objType) : CMeshOrbit(nPriority, objType)
{
	// 各種値のクリア
	m_fAlphaDecayUp = 0.0f;                          // 頂点の上側の透明度減衰値
 	m_fAlphaDecayDown = 0.0f;                        // 頂点の下側の透明度減衰値
	m_ColUp = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);     // 頂点上側の色
	m_ColDown = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 頂点下側の色
	m_OffsetAmp[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // オフセットの増幅値(１個目)
	m_OffsetAmp[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // オフセットの増幅値(２個目)
}

//=============================================================================
//    デストラクタ
//=============================================================================
COrbitEffect::~COrbitEffect()
{

}

//=============================================================================
//    生成処理
//=============================================================================
COrbitEffect *COrbitEffect::Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, D3DXVECTOR3 OffsetAmp1, D3DXVECTOR3 OffsetAmp2,
	D3DXCOLOR ColUp, D3DXCOLOR ColDown, float fAlphaDecayUp, float fAlphaDecayDown, int nXBlock, int nZBlock, D3DXMATRIX *pMtxParent, int nPriority)
{
	COrbitEffect *pOrbitEffect = NULL;     // 軌跡エフェクトクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pOrbitEffect == NULL)
		{// メモリが空になっている
			pOrbitEffect = new COrbitEffect(nPriority);
			if (pOrbitEffect != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pOrbitEffect->SetOffsetPos(OffsetPos1, 0);
				pOrbitEffect->SetOffsetPos(OffsetPos2, 1);
				pOrbitEffect->SetOffsetAmp1(OffsetAmp1);
				pOrbitEffect->SetOffsetAmp2(OffsetAmp2);
				pOrbitEffect->SetColUp(ColUp);
				pOrbitEffect->SetColDown(ColDown);
				pOrbitEffect->SetAlphaDecayUp(fAlphaDecayUp);
				pOrbitEffect->SetAlphaDecayDown(fAlphaDecayDown);
				pOrbitEffect->SetXBlock(nXBlock);
				pOrbitEffect->SetZBlock(nZBlock);
				pOrbitEffect->SetMtxParent(pMtxParent);

				if (FAILED(pOrbitEffect->Init()))
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

	return pOrbitEffect;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT COrbitEffect::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CMeshOrbit::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void COrbitEffect::Uninit(void)
{
	// 共通の終了処理
	CMeshOrbit::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void COrbitEffect::Update(void)
{
	// 共通の更新処理
	CMeshOrbit::Update();

	D3DXVECTOR3 offset;
	for (int nCntOffset = 0; nCntOffset < 2; nCntOffset++)
	{// 縦の分割数分繰り返し
		offset = GetOffsetPos(nCntOffset);
		offset += m_OffsetAmp[nCntOffset];
		SetOffsetPos(offset, nCntOffset);
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void COrbitEffect::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// 共通の描画処理
			CMeshOrbit::Draw();

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void COrbitEffect::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * GetNumVertex(),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を計算しておく
	D3DXVECTOR3 VertexPos[2] = {};
	for (int nCntVer = 0; nCntVer < 2; nCntVer++)
	{// ついていく頂点座標の分だけ繰り返し
	    // 頂点座標をワールドマトリックスに掛け合わせる
	    //********************************************************************
	    // D3DXVec3TransformCoord
	    // 内容    : ベクトルとマトリックスを掛け合わせ、ベクトルを出力する
	    // 第1引数 : 出力されるベクトル
	    // 第2引数 : 入力ベクトル
	    // 第3引数 : 入力マトリックス
	    //********************************************************************
		D3DXVec3TransformCoord(&VertexPos[nCntVer], &GetOffsetPos(nCntVer), GetMtxParent());
	}

	// 頂点設定
	int nCntUp = 0;
	int nCntDown = 0;
	float fAlpha = 0.0f;
	for (int nCntVer = 0; nCntVer < GetNumVertex(); nCntVer++)
	{// 頂点数分繰り返し
	    // 頂点座標
		SetVertexPos(VertexPos[(nCntVer % 2)], nCntVer);
		pVtx[nCntVer].pos = GetVertexPos()[nCntVer];

		// 頂点カラー
		if (nCntVer % 2 == 0)
		{// 偶数の時
			fAlpha = m_ColUp.a - (nCntUp * m_fAlphaDecayUp);
			if (fAlpha <= 0.0f)
			{
				fAlpha = 0.0f;
			}
			SetVertexCol(D3DXCOLOR(m_ColUp.r, m_ColUp.g, m_ColUp.b, fAlpha), nCntVer);
			nCntUp++;
		}
		else
		{// 奇数の時
			fAlpha = m_ColDown.a - (nCntDown * m_fAlphaDecayDown);
			if (fAlpha <= 0.0f)
			{
				fAlpha = 0.0f;
			}
			SetVertexCol(D3DXCOLOR(m_ColDown.r, m_ColDown.g, m_ColDown.b, fAlpha), nCntVer);
			nCntDown++;
		}
		pVtx[nCntVer].col = GetVertexCol()[nCntVer];

		// テクスチャ座標
		pVtx[nCntVer].tex = D3DXVECTOR2(0.0f + (nCntVer / 2) * (1.0f / (float)GetXBlock()), 0.0f + (nCntVer % 2) * 1.0f);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    頂点上側の色を設定
//=============================================================================
void COrbitEffect::SetColUp(const D3DXCOLOR ColUp)
{
	m_ColUp = ColUp;
}

//=============================================================================
//    頂点下側の色を設定
//=============================================================================
void COrbitEffect::SetColDown(const D3DXCOLOR ColDown)
{
	m_ColDown = ColDown;
}

//=============================================================================
//    頂点上側の透明度の減衰値を設定
//=============================================================================
void COrbitEffect::SetAlphaDecayUp(const float fAlphaDecayUp)
{
	m_fAlphaDecayUp = fAlphaDecayUp;
}

//=============================================================================
//    頂点下側の透明度の減衰値を設定
//=============================================================================
void COrbitEffect::SetAlphaDecayDown(const float fAlphaDecayDown)
{
	m_fAlphaDecayDown = fAlphaDecayDown;
}

//=============================================================================
//    オフセットの増幅値(１個目)を設定
//=============================================================================
void COrbitEffect::SetOffsetAmp1(const D3DXVECTOR3 OffsetAmp)
{
	m_OffsetAmp[0] = OffsetAmp;
}

//=============================================================================
//    オフセットの増幅値(２個目)を設定
//=============================================================================
void COrbitEffect::SetOffsetAmp2(const D3DXVECTOR3 OffsetAmp)
{
	m_OffsetAmp[1] = OffsetAmp;
}

//=============================================================================
//    頂点上側の色を取得
//=============================================================================
D3DXCOLOR COrbitEffect::GetColUp(void)
{
	return m_ColUp;
}

//=============================================================================
//    頂点下側の色を取得
//=============================================================================
D3DXCOLOR COrbitEffect::GetColDown(void)
{
	return m_ColDown;
}

//=============================================================================
//    頂点上側の透明度の減衰値を取得
//=============================================================================
float COrbitEffect::GetAlphaDecayUp(void)
{
	return m_fAlphaDecayUp;
}

//=============================================================================
//    頂点下側の透明度の減衰値を取得
//=============================================================================
float COrbitEffect::GetAlphaDecayDown(void)
{
	return m_fAlphaDecayDown;
}

//=============================================================================
//    オフセットの増幅値(１個目)を取得
//=============================================================================
D3DXVECTOR3 COrbitEffect::GetOffsetAmp1(void)
{
	return m_OffsetAmp[0];
}

//=============================================================================
//    オフセットの増幅値(２個目)を取得
//=============================================================================
D3DXVECTOR3 COrbitEffect::GetOffsetAmp2(void)
{
	return m_OffsetAmp[1];
}