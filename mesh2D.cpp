//*****************************************************************************
//
//     メッシュ2Dポリゴンの処理[mesh2D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mesh2D.h"
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
CMesh2D::CMesh2D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;                          // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;                          // インデックスバッファへのポインタ
	m_pTexture = NULL;	                        // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュ2Dポリゴンの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // メッシュ2Dポリゴンの色
	m_fWidth = 0.0f;                            // 1ブロック分の幅
	m_fHeight = 0.0f;                           // 1ブロック分高さ
	m_nXBlock = 0;                              // 横の分割数
	m_nYBlock = 0;                              // 縦の分割数
	m_nNumVertex = 0;                           // 頂点数
	m_nNumIndex = 0;                            // インデックス数
	m_nNumPolygon = 0;                          // ポリゴン数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMesh2D::~CMesh2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMesh2D *CMesh2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nPriority)
{
	CMesh2D *pMesh2D = NULL;               // メッシュ2Dポリゴンクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pMesh2D == NULL)
		{// メモリが空になっている
			pMesh2D = new CMesh2D(nPriority);
			if (pMesh2D != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pMesh2D->SetPos(pos);                   // メッシュ2Dポリゴンの座標
				pMesh2D->SetCol(col);                   // メッシュ2Dポリゴンの色
				pMesh2D->SetWidth(fWidth);              // 1ブロック分の幅
				pMesh2D->SetHeight(fHeight);            // 1ブロック分の高さ
				pMesh2D->SetXBlock(nXBlock);            // 横の分割数
				pMesh2D->SetYBlock(nYBlock);            // 縦の分割数
				pMesh2D->SetTexSplitU(nTexSplitU);      // テクスチャU座標の分割数
				pMesh2D->SetTexSplitV(nTexSplitV);      // テクスチャV座標の分割数

				if (FAILED(pMesh2D->Init()))
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

	return pMesh2D;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMesh2D::Init(void)
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
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // 頂点数を計算
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // インデックス数を設定
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // 描画するポリゴン数を設定

			// 頂点バッファの生成
			MakeVertex(pDevice);

			// インデックスバッファの生成
			MakeIndex(pDevice);
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
void CMesh2D::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pIdxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMesh2D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMesh2D::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

			// インデックスバッファをデバイスのデータストリームにバインド
			pDevice->SetIndices(m_pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			if (m_pTexture != NULL)
			{
				pDevice->SetTexture(0, m_pTexture);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CMesh2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 生成に必要な変数を宣言
	D3DXCOLOR col = GetCol();
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
	float YPos = -(m_fHeight * m_nYBlock) / 2; // Y座標を上に設定
	float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
	float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
		    // 頂点座標
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f) + m_Pos;

			// テクスチャ値
			pVtx[nCntH].rhw = 1.0f;

			// 頂点カラー
			pVtx[nCntH].col = col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

			XPos += m_fWidth;                              // X座標を横に進める
			fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
		}
		XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
		YPos += m_fHeight;                                                // Y座標を下に進める
		fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
		fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

		// ポインタを進める
		pVtx += m_nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    インデックスバッファ生成処理
//=============================================================================
void CMesh2D::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		    // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_nXBlock + 1; // 下側
			pIdx[1] = nCntIdx;                 // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                       // 上側
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMesh2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMesh2D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    インデックスバッファ設定処理
//=============================================================================
void CMesh2D::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュ2Dポリゴンの座標設定処理
//=============================================================================
void CMesh2D::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュ2Dポリゴンの色設定処理
//=============================================================================
void CMesh2D::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
			    // 頂点カラー
				pVtx[nCntH].col = m_Col;
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1ブロック分の幅設定処理
//=============================================================================
void CMesh2D::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    1ブロック分の高さ設定処理
//=============================================================================
void CMesh2D::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMesh2D::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    縦の分割数設定処理
//=============================================================================
void CMesh2D::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;
}

//=============================================================================
//    テクスチャU座標の分割数処理
//=============================================================================
void CMesh2D::SetTexSplitU(const int nTexSplitU)
{
	m_nTexSplitU = nTexSplitU;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // テクスチャU座標

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].tex.x = fTexU;
				fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;
			}
			fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1);

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    テクスチャV座標の分割数設定処理
//=============================================================================
void CMesh2D::SetTexSplitV(const int nTexSplitV)
{
	m_nTexSplitV = nTexSplitV;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexV = 0.0f;   // テクスチャV座標

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].tex.y = fTexV;
			}
			fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMesh2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    インデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMesh2D::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMesh2D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    メッシュ2Dポリゴンの座標取得処理
//=============================================================================
D3DXVECTOR3 CMesh2D::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュ2Dポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CMesh2D::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1ブロック分の幅取得処理
//=============================================================================
float CMesh2D::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1ブロック分の高さ取得処理
//=============================================================================
float CMesh2D::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMesh2D::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    縦の分割数取得処理
//=============================================================================
int CMesh2D::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    テクスチャU座標の分割数取得処理
//=============================================================================
int CMesh2D::GetTexSplitU(void)
{
	return m_nTexSplitU;
}

//=============================================================================
//    テクスチャV座標の分割数取得処理
//=============================================================================
int CMesh2D::GetTexSplitV(void)
{
	return m_nTexSplitV;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMesh2D::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    インデックス数取得処理
//=============================================================================
int CMesh2D::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMesh2D::GetNumPolygon(void)
{
	return m_nNumPolygon;
}