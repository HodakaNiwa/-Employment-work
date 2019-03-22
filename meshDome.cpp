//*****************************************************************************
//
//     半球の処理[meshDome.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshDome.h"
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
CMeshDome::CMeshDome(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;                          // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;                          // インデックスバッファへのポインタ
	m_pTexture = NULL;	                        // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュドームの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュドームの向き
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // メッシュドームの色
	m_fRadius = 0.0f;                           // メッシュドームの半径
	m_nXBlock = 0;                              // 横の分割数
	m_nYBlock = 0;                              // 奥行の分割数
	m_nNumVertex = 0;                           // 頂点数
	m_nNumIndex = 0;                            // インデックス数
	m_nNumPolygon = 0;                          // ポリゴン数
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshDome::~CMeshDome()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshDome *CMeshDome::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CMeshDome *pMeshDome = NULL;           // メッシュドームクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pMeshDome == NULL)
		{// メモリが空になっている
			pMeshDome = new CMeshDome(nPriority);
			if (pMeshDome != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pMeshDome->SetPos(pos);         // メッシュドームの座標
				pMeshDome->SetRot(rot);         // メッシュドームの向き
				pMeshDome->SetCol(col);         // メッシュドームの色
				pMeshDome->SetRadius(fRadius);  // メッシュドームの半径
				pMeshDome->SetXBlock(nXBlock);  // 横の分割数
				pMeshDome->SetYBlock(nYBlock);  // 縦の分割数

				if (FAILED(pMeshDome->Init()))
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

	return pMeshDome;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshDome::Init(void)
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
void CMeshDome::Uninit(void)
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
void CMeshDome::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshDome::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデバイスのデータストリームにバインド
			pDevice->SetIndices(m_pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

			// 蓋の描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (m_nXBlock + 1), GetNumIndex(), (m_nXBlock));
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CMeshDome::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CMeshDome::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成(蓋の頂点数も考慮する)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nNumVertex + (m_nXBlock + 2)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // 角度
	float fRadius = 0;  // 半径
	float fDome = 0.0f; // ドーム計算用(半径を縮める倍率)
	D3DXVECTOR3 pos;    // 座標
	float Utex;         // テクスチャU座標
	float Vtex;         // テクスチャV座標

	fAngle = 0.0f;                       // 角度は0に
	fRadius = cosf(fDome) * m_fRadius;   // 半径を設定
	fDome = 0.0f;                        // 倍率を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 座標を設定
	Utex = 0.0f;                         // テクスチャU座標を設定
	Vtex = -1.0f;                        // テクスチャV座標を設定

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(fAngle) * fRadius;
			pos.y = sinf(fDome) * m_fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 法線ベクトル
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

			// 頂点カラー
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (m_nXBlock / 2);  // 角度を進める
			Utex += 1.0f / (m_nXBlock / 2);       // テクスチャU座標を進める

			if (fAngle <= -D3DX_PI)
			{// 倍率が円周率を超えた
				fAngle += D3DX_PI * 2.0f;
			}
		}
		fDome += (D3DX_PI * 0.5f) / (m_nYBlock + 1); // 半径を縮める倍率を上げる
		fRadius = cosf(fDome) * m_fRadius;           // 半径をコサインカーブで縮めていく
		fAngle = 0.0f;                               // 角度は0に
		Utex = 0.0f;                                 // テクスチャU座標を設定
		Vtex -= 1.0f / (m_nYBlock);                  // テクスチャV座標を進める
		pVtx += m_nXBlock + 1;                       // 頂点の先頭の番号を進める
	}

	// 蓋用の頂点情報を作成
	fDome = D3DX_PI * 0.5f;   // 半径を縮める倍率を上げる
	// 頂点座標
	pos = D3DXVECTOR3(0.0f, sinf(fDome) * m_fRadius, 0.0f);
	pVtx[0].pos = pos;

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, -0.99f);
	pVtx++;

	fDome -= (D3DX_PI * 0.5f) / (m_nYBlock + 1);  // 半径を縮める倍率を戻しておく
	fRadius = cosf(fDome) * m_fRadius;            // 半径計算を戻しておく
	for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
	{// 水平方向の分割数だけ繰り返し
		pos.x = sinf(fAngle) * fRadius;
		pos.y = sinf(fDome) * m_fRadius;
		pos.z = cosf(fAngle) * fRadius;

		// 頂点座標
		pVtx[nCntH].pos = pos;

		// 法線ベクトル
		pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

		// 頂点カラー
		pVtx[nCntH].col = m_Col;

		// テクスチャ座標
		pVtx[nCntH].tex = D3DXVECTOR2(0.0f, -0.98f);

		fAngle += -D3DX_PI / (m_nXBlock / 2);  // 角度を進める
		Utex -= 1.0f / (m_nXBlock / 2);        // テクスチャU座標を進める

		if (fAngle <= -D3DX_PI)
		{// 倍率が円周率を超えた
			fAngle += D3DX_PI * 2.0f;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 法線計算処理
	MakeNormal();
}

//=============================================================================
//    インデックスバッファ生成処理
//=============================================================================
void CMeshDome::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nNumIndex + (m_nXBlock + 2)),
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

	// 蓋のインデックスを設定
	nCntIdx += m_nXBlock + 1;
	// 蓋の先頭番号を設定
	for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 2; nCntIdxX++, nCntIdx++)
	{// 横の分割数+2繰り返し
		pIdx[0] = nCntIdx;
		pIdx++;  // 1つ分進める
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    法線計算処理
//=============================================================================
void CMeshDome::MakeNormal(void)
{
	VERTEX_3D *pVtx;	                              // 頂点データへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファ

	D3DXVECTOR3 VecVer[3];	    // 法線計算用ベクトル
	D3DXVECTOR3 *pNor = NULL;   // 頂点の法線

	// 必要な数だけ動的に確保する
	pNor = new D3DXVECTOR3[(m_nXBlock * m_nYBlock) * 2];

	// 外積用にベクトルを作る
	if (pNor != NULL)
	{	// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVec1 = 0;                // 番号計算用(左上の頂点番号)
		int nCntVec2 = (m_nXBlock + 1);  // 番号計算用(左下の頂点番号)
		int nCntVec3 = (m_nXBlock + 2);  // 番号計算用(右下の頂点番号)
		int nCntVec4 = 1;                // 番号計算用(右上の頂点番号)
		int nCntNor = 0;                 // 番号計算用(法線の番号)
		for (int nCntV = 0; nCntV < m_nYBlock; nCntV++)
		{// 垂直方向の分割数だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// 水平方向の分割数だけ繰り返し
				VecVer[0] = pVtx[nCntVec2].pos - pVtx[nCntVec1].pos;
				VecVer[1] = pVtx[nCntVec3].pos - pVtx[nCntVec1].pos;
				VecVer[2] = pVtx[nCntVec4].pos - pVtx[nCntVec1].pos;

				// 外積を使ってベクトルにする
				D3DXVec3Cross(&pNor[nCntNor], &VecVer[1], &VecVer[0]);
				D3DXVec3Cross(&pNor[nCntNor + 1], &VecVer[2], &VecVer[1]);

				// 出てきたベクトルを正規化して法線にする
				D3DXVec3Normalize(&pNor[nCntNor], &pNor[nCntNor]);
				D3DXVec3Normalize(&pNor[nCntNor + 1], &pNor[nCntNor + 1]);

				// 番号をずらす
				nCntNor += 2;
				nCntVec1++;
				nCntVec2++;
				nCntVec3++;
				nCntVec4++;
			}
			// 右端はもう終わっているので左端になるように番号をそろえる
			nCntVec1++;
			nCntVec2++;
			nCntVec3++;
			nCntVec4++;
		}

		// 法線を代入
		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				if (nCntH == 0)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番左上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[0] + pNor[1]) / 2;  // OK
					}
					else if (nCntV == m_nYBlock)
					{// 一番左下の頂点番号の場合
						pVtx[nCntH].nor = pNor[((m_nXBlock * 2) * (m_nYBlock - 1))];  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2))] + pNor[nCntV * (m_nXBlock * 2)] + pNor[nCntV * (m_nXBlock * 2) + 1]) / 3;   // OK
					}
				}
				else if (nCntH == m_nXBlock)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番右上の頂点番号の場合
						pVtx[nCntH].nor = pNor[(m_nXBlock * 2) - 1];  // OK
					}
					else if (nCntV == m_nYBlock)
					{// 一番右下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((m_nXBlock * 2) * m_nYBlock) - 1] + pNor[((m_nXBlock * 2) * m_nYBlock) - 2]) / 2;  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 2]) / 3;  // OK
					}
				}
				else
				{// 真ん中の場合
					if (nCntV == 0)
					{// 一番上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[(nCntH * 2) - 1] + pNor[(nCntH * 2)] + pNor[(nCntH * 2) + 1]) / 3;   // OK
					}
					else if (nCntV == m_nYBlock)
					{// 一番下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1))] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1)) + 1] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1)) + 2]) / 3; // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 2] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2)] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 1] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 2]) / 6;  // OK
					}
				}
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		if (pNor != NULL)
		{// メモリが確保されている
			delete[] pNor;
			pNor = NULL;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMeshDome::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshDome::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    インデックスバッファ設定処理
//=============================================================================
void CMeshDome::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュドームの座標設定処理
//=============================================================================
void CMeshDome::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュドームの向き設定処理
//=============================================================================
void CMeshDome::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    メッシュドームの色設定処理
//=============================================================================
void CMeshDome::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

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
//    メッシュドームの半径設定処理
//=============================================================================
void CMeshDome::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMeshDome::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    奥行の分割数設定処理
//=============================================================================
void CMeshDome::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CMeshDome::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshDome::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    インデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshDome::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshDome::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    メッシュドームの座標取得処理
//=============================================================================
D3DXVECTOR3 CMeshDome::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュドームの向き取得処理
//=============================================================================
D3DXVECTOR3 CMeshDome::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    メッシュドームの色取得処理
//=============================================================================
D3DXCOLOR CMeshDome::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    メッシュドームの半径取得処理
//=============================================================================
float CMeshDome::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshDome::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshDome::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshDome::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    インデックス数取得処理
//=============================================================================
int CMeshDome::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMeshDome::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshDome::GetMtxWorld(void)
{
	return m_MtxWorld;
}