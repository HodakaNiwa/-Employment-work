//*****************************************************************************
//
//     軌跡の処理[meshOrbit.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshOrbit.h"
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
CMeshOrbit::CMeshOrbit(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;                          // 頂点バッファへのポインタ
	m_pTexture = NULL;	                        // テクスチャへのポインタ
	m_nXBlock = 0;                              // 横の分割数
	m_nZBlock = 0;                              // 奥行の分割数
	m_nNumVertex = 0;                           // 頂点数
	m_nNumPolygon = 0;                          // ポリゴン数
	m_pMtxParent = NULL;                        // 親のマトリックスのポインタ
	m_pVertexPos = NULL;                        // 頂点座標(頂点数分動的に確保)
	m_pVertexCol = NULL;                        // 頂点カラー(頂点数分動的に確保)
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshOrbit *CMeshOrbit::Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, int nXBlock, int nZBlock, int nPriority)
{
	CMeshOrbit *pMeshOrbit= NULL;          // 軌跡クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pMeshOrbit == NULL)
		{// メモリが空になっている
			pMeshOrbit = new CMeshOrbit(nPriority);
			if (pMeshOrbit != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pMeshOrbit->SetOffsetPos(OffsetPos1,0);
				pMeshOrbit->SetOffsetPos(OffsetPos2,1);
				pMeshOrbit->SetXBlock(nXBlock);
				pMeshOrbit->SetZBlock(nZBlock);

				if (FAILED(pMeshOrbit->Init()))
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

	return pMeshOrbit;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                        // 頂点数を計算
			m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);   // 描画するポリゴン数を設定

			// 頂点座標分のメモリを動的に確保
			// 頂点座標
			if (m_pVertexPos == NULL)
			{// メモリが確保できる状態である
				m_pVertexPos = new D3DXVECTOR3[m_nNumVertex];
			}

			// 頂点カラー
			if (m_pVertexCol == NULL)
			{// メモリが確保できる状態である
				m_pVertexCol = new D3DXCOLOR[m_nNumVertex];
			}

			// 頂点バッファの生成
			MakeVertex(pDevice);
		}
		else
		{// デバイスを取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが生成されていない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 確保した頂点データのメモリを開放する
	// 頂点座標
	if (m_pVertexPos != NULL)
	{// メモリが確保できる状態である
		delete m_pVertexPos;
		m_pVertexPos = NULL;
	}

	// 頂点カラー
	if (m_pVertexCol != NULL)
	{// メモリが確保できる状態である
		delete m_pVertexCol;
		m_pVertexCol = NULL;
	}

	// 自身のポインタを破棄する
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMeshOrbit::Update(void)
{
	// 頂点座標を計算する
	if (m_pVertexPos != NULL)
	{// メモリが確保されている
		for (int nCntVer = m_nNumVertex - 1; nCntVer > 1; nCntVer--)
		{// 頂点座標分繰り返し
			m_pVertexPos[nCntVer] = m_pVertexPos[nCntVer - 2];
		}

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
			D3DXVec3TransformCoord(&m_pVertexPos[nCntVer], &m_aOffsetPos[nCntVer], m_pMtxParent);
		}
	}

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
		// 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVer = 0; nCntVer < m_nNumVertex; nCntVer++)
		{// 頂点数分繰り返し
			pVtx[nCntVer].pos = m_pVertexPos[nCntVer];
			pVtx[nCntVer].col = m_pVertexCol[nCntVer];
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshOrbit::Draw(void)
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
			DWORD CullMode;

			// ライティングの設定を外す
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// カリングの設定を外す
			pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumPolygon);

			// カリングの設定を戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);

			// ライティングの設定を戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CMeshOrbit::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CMeshOrbit::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVer = 0; nCntVer < m_nNumVertex; nCntVer++)
	{// 頂点数分繰り返し
		// 頂点座標
		m_pVertexPos[nCntVer] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCntVer].pos = m_pVertexPos[nCntVer];

		// 頂点カラー
		m_pVertexCol[nCntVer] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntVer].col = m_pVertexCol[nCntVer];

		// テクスチャ座標
		pVtx[nCntVer].tex = D3DXVECTOR2(0.0f + (nCntVer / 2) * (1.0f / (float)m_nXBlock), 0.0f + (nCntVer % 2) * 1.0f);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMeshOrbit::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshOrbit::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    オフセットの座標を設定する処理
//=============================================================================
void CMeshOrbit::SetOffsetPos(const D3DXVECTOR3 OffsetPos, int nIdx)
{
	m_aOffsetPos[nIdx] = OffsetPos;
}

//=============================================================================
//   頂点座標を設定する処理
//=============================================================================
void CMeshOrbit::SetVertexPos(D3DXVECTOR3 pos, int nIdx)
{
	m_pVertexPos[nIdx] = pos;
}

//=============================================================================
//   頂点カラーを設定する処理
//=============================================================================
void CMeshOrbit::SetVertexCol(D3DXCOLOR col, int nIdx)
{
	m_pVertexCol[nIdx] = col;
}

//=============================================================================
//   頂点座標を設定する処理
//=============================================================================
void CMeshOrbit::SetVertexPos(D3DXVECTOR3 *pPos)
{
	m_pVertexPos = pPos;
}

//=============================================================================
//   頂点カラーを設定する処理
//=============================================================================
void CMeshOrbit::SetVertexCol(D3DXCOLOR *pCol)
{
	m_pVertexCol = pCol;
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMeshOrbit::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    奥行の分割数設定処理
//=============================================================================
void CMeshOrbit::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CMeshOrbit::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    親のマトリックスのポインタ設定処理
//=============================================================================
void CMeshOrbit::SetMtxParent(D3DXMATRIX *pMtxParent)
{
	m_pMtxParent = pMtxParent;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshOrbit::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshOrbit::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    オフセットの距離取得処理
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetOffsetPos(int nIdx)
{
	return m_aOffsetPos[nIdx];
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshOrbit::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshOrbit::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshOrbit::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMeshOrbit::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    頂点座標取得処理
//=============================================================================
D3DXVECTOR3 *CMeshOrbit::GetVertexPos(void)
{
	return m_pVertexPos;
}

//=============================================================================
//    頂点カラー取得処理
//=============================================================================
D3DXCOLOR *CMeshOrbit::GetVertexCol(void)
{
	return m_pVertexCol;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshOrbit::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    親のマトリックスのポインタ取得処理
//=============================================================================
D3DXMATRIX *CMeshOrbit::GetMtxParent(void)
{
	return m_pMtxParent;
}