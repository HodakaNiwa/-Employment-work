//*****************************************************************************
//
//     メッシュウォールの処理[meshWall.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshWall.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CMeshWallの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMeshWall::CMeshWall(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_nTexIdx = 0;                                // メッシュウォールのテクスチャ番号
	m_pVtxBuff = NULL;                            // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;                            // インデックスバッファへのポインタ
	m_pTexture = NULL;	                          // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // メッシュフィールドの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // メッシュフィールドの向き
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);    // メッシュフィールドの色
	m_fWidth = 0.0f;                              // 1ブロック分の幅
	m_fHeight = 0.0f;                             // 1ブロック分高さ
	m_nXBlock = 0;                                // 横の分割数
	m_nYBlock = 0;                                // 縦の分割数
	m_nNumVertex = 0;                             // 頂点数
	m_nNumIndex = 0;                              // インデックス数
	m_nNumPolygon = 0;                            // ポリゴン数
	D3DXMatrixIdentity(&m_MtxWorld);              // ワールドマトリックス
	m_WallNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 壁の法線ベクトル
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshWall::~CMeshWall()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshWall *CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority)
{
	CMeshWall *pMeshWall = NULL;           // メッシュウォールクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pMeshWall == NULL)
		{// メモリが空になっている
			pMeshWall = new CMeshWall(nPriority);
			if (pMeshWall != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pMeshWall->SetTexIdx(nTexIdx);            // メッシュウォールのテクスチャ番号
				pMeshWall->SetPos(pos);                   // メッシュウォールの座標
				pMeshWall->SetRot(rot);                   // メッシュウォールの向き
				pMeshWall->SetCol(col);                   // メッシュウォールの色
				pMeshWall->SetWidth(fWidth);              // 1ブロック分の幅
				pMeshWall->SetHeight(fHeight);            // 1ブロック分の高さ
				pMeshWall->SetXBlock(nXBlock);            // 横の分割数
				pMeshWall->SetYBlock(nYBlock);            // 縦の分割数
				pMeshWall->SetTexSplitU(nTexSplitU);      // テクスチャU座標の分割数
				pMeshWall->SetTexSplitV(nTexSplitV);      // テクスチャV座標の分割数

				if (FAILED(pMeshWall->Init()))
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

	return pMeshWall;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshWall::Init(void)
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
void CMeshWall::Uninit(void)
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
void CMeshWall::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshWall::Draw(void)
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
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CMeshWall::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
void CMeshWall::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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

	// 生成に必要な変数を宣言
	D3DXCOLOR col = GetCol();
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
	float YPos = (m_fHeight * m_nYBlock) / 2;  // Y座標を奥に設定
	float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
	float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
		    // 頂点座標
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

			// 法線ベクトル
			pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 頂点カラー
			pVtx[nCntH].col = col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

			XPos += m_fWidth;                              // X座標を横に進める
			fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
		}
		XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
		YPos -= m_fHeight;                                                // Y座標を下に進める
		fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
		fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

		// ポインタを進める
		pVtx += m_nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


	// ワールドマトリックス計算処理
	SetMtxWorld(pDevice);

	// 法線ベクトルを作る
	D3DXVECTOR3 WallPos1, WallPos2, WallPos3;
	// 左上の頂点座標
	WallPos1.x = -((m_fWidth * m_nXBlock) / 2);
	WallPos1.y = ((m_fHeight * m_nYBlock) / 2);
	D3DXVec3TransformCoord(&WallPos1, &WallPos1, &m_MtxWorld);

	// 右上の頂点座標
	WallPos2.x = ((m_fWidth * m_nXBlock) / 2);
	WallPos2.y = ((m_fHeight * m_nYBlock) / 2);
	D3DXVec3TransformCoord(&WallPos2, &WallPos2, &m_MtxWorld);

	// 左下の頂点座標
	WallPos3.x = -((m_fWidth * m_nXBlock) / 2);
	WallPos3.y = -((m_fHeight * m_nYBlock) / 2);
	D3DXVec3TransformCoord(&WallPos3, &WallPos3, &m_MtxWorld);

	// 外積から法線ベクトルを作る
	D3DXVECTOR3 WallVec1, WallVec2;
	WallVec1 = WallPos2 - WallPos1;
	WallVec2 = WallPos3 - WallPos1;
	D3DXVECTOR3 WallCross;
	D3DXVec3Cross(&WallCross, &WallVec2, &WallVec1);

	// 外積から算出したベクトルを正規化して法線ベクトルとする
	D3DXVec3Normalize(&m_WallNormal, &WallCross);
}

//=============================================================================
//    インデックスバッファ生成処理
//=============================================================================
void CMeshWall::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
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
//    当たり判定処理
//=============================================================================
bool CMeshWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Offset, D3DXVECTOR3 *pInterPoint)
{
	bool bCol = false;        // 当たったかどうか
	if (pPos != NULL)
	{
		bool bInside = false;     // 内側にいるかどうか
		D3DXVECTOR3 VecStart;     // ベクトルの始点
		D3DXVECTOR3 VecEnd;       // ベクトルの終点
		D3DXVECTOR3 VerPos[4];    // 内側にいるかどうかの判定用
		D3DXVECTOR3 Pos1, Pos2;   // 計算用

		if (pPos->y + Offset.y >= GetPos().y - ((m_fHeight * m_nYBlock) / 2) && pPos->y <= GetPos().y + ((m_fHeight * m_nYBlock) / 2))
		{// 壁の高さ以内の場所にいる
			// 壁の内側にいるか判定
			VerPos[0] = D3DXVECTOR3(-((m_fWidth * m_nXBlock) / 2) - Offset.x, ((m_fHeight * m_nYBlock) / 2), -Offset.z);
			VerPos[1] = D3DXVECTOR3(((m_fWidth * m_nXBlock) / 2) + Offset.x, ((m_fHeight * m_nYBlock) / 2), -Offset.z);
			VerPos[2] = D3DXVECTOR3(((m_fWidth * m_nXBlock) / 2) + Offset.x, -((m_fHeight * m_nYBlock) / 2), -Offset.z);
			VerPos[3] = D3DXVECTOR3(-((m_fWidth * m_nXBlock) / 2) - Offset.x, -((m_fHeight * m_nYBlock) / 2), -Offset.z);
			for (int nCntVec = 0; nCntVec < 4; nCntVec++)
			{// 4回分繰り返す
				D3DXVec3TransformCoord(&VerPos[nCntVec], &VerPos[nCntVec], &m_MtxWorld);
			}

			int nCntClear = 0;
			for (int nCntVec = 0; nCntVec < 4; nCntVec++)
			{// 4回分判定
				// ベクトルを使って外積で判定する
				Pos1 = *pPos - VerPos[nCntVec];
				Pos2 = VerPos[(nCntVec + 1) % 4] - VerPos[nCntVec];
				D3DXVECTOR3 Cross;
				D3DXVec3Cross(&Cross, &Pos1, &Pos2);

				if (Cross <= 0 || Cross >= 0)
				{// 壁に当たった
					nCntClear++;
					if (nCntClear == 4)
					{// 全ての判定を通った
						bInside = true;
					}
				}
			}

			if (bInside == true)
			{// 壁の内側にいる
				// 壁の端っこ(オフセット)を求める
				float fWallStart = -((m_fWidth * m_nXBlock) / 2) - Offset.x;
				float fWallEnd = ((m_fWidth * m_nXBlock) / 2) + Offset.x;
				VecStart = D3DXVECTOR3(fWallStart, 0.0f, -Offset.z);
				VecEnd = D3DXVECTOR3(fWallEnd, 0.0f, -Offset.z);

				// 壁の向きに合わせてベクトルを作る
				D3DXVec3TransformCoord(&VecStart, &VecStart, &m_MtxWorld);
				D3DXVec3TransformCoord(&VecEnd, &VecEnd, &m_MtxWorld);

				// ベクトルを使って外積で判定する
				Pos1 = *pPos - VecStart;
				Pos2 = VecEnd - VecStart;

				if ((Pos1.z * Pos2.x) - (Pos1.x * Pos2.z) > 0)
				{// 壁に当たった
					// 外積ベクトルを用意するための変数を宣言
					Pos1 = *pPosOld;                // 前回のプレイヤーの座標
					Pos2 = *pPos;                   // 現在のプレイヤーの座標
					D3DXVECTOR3 Pos3 = VecEnd;      // ベクトルの終点
					D3DXVECTOR3 Pos4 = VecStart;    // ベクトルの始点

					// XZ平面上で外積を作る
					D3DXVECTOR3 Vec1 = Pos4 - Pos1;    // ベクトルの始点 - 前回のプレイヤーの座標
					D3DXVECTOR3 Vec2 = Pos2 - Pos1;    // 現在のプレイヤーの座標 - 前回のプレイヤーの座標
					D3DXVECTOR3 Vec3 = Pos3 - Pos4;    // ベクトルの終点 - ベクトルの始点
					float CrossCalc1 = CFunctionLib::D3DXVec2Cross(&D3DXVECTOR2(Vec1.x, Vec1.z), &D3DXVECTOR2(Vec2.x, Vec2.z));
					float CrossCalc2 = CFunctionLib::D3DXVec2Cross(&D3DXVECTOR2(Vec2.x, Vec2.z), &D3DXVECTOR2(Vec3.x, Vec3.z));
					float CrossCalc3 = CFunctionLib::D3DXVec2Cross(&D3DXVECTOR2(Vec1.x, Vec1.z), &D3DXVECTOR2(Vec3.x, Vec3.z));

					if (CrossCalc2 != 0.0f)
					{// 2本目のベクトルと3本目のベクトルの外積が0以外ならば(0だと平行ベクトルなので交差することがない)
						// 交点までの長さの割合を外積から算出
						float Length1 = CrossCalc3 / CrossCalc2;   // 前回のプレイヤーの座標からの距離
						float Length2 = CrossCalc1 / CrossCalc2;   // ベクトルの始点からの距離

						// 交差しているか判定する(交点までの長さの割合が両方の始点から 0以上 1以下 であるなら交差している)
						float ErorrPermission = 0.1f;  // 誤差許容値(浮動小数点の丸め方によって判定がすり抜けるので・・・)
						if (Length1 + ErorrPermission >= 0 && Length1 - ErorrPermission <= 1
							&& Length2 + ErorrPermission >= 0 && Length2 - ErorrPermission <= 1)
						{// 交差していると判定された
							// 交点の座標を算出し壁ずりベクトルを求め交点をずらす
							// 交点 = ベクトルの始点 + 交点までの比率 * ベクトルの終点
							//D3DXVECTOR3 InterPoint = *pPosOld + t1 * Vec2;      // 引数の座標から
							D3DXVECTOR3 InterPoint = VecStart + Length2 * Vec3;   // 壁のベクトルから

							// 壁ずりベクトル = 進行ベクトル - 内積(進行ベクトル * 壁の法線ベクトル) * 壁の法線ベクトル
							D3DXVECTOR3 Move;
							D3DXVECTOR3 ObjMove = *pMove;
							float fDot = D3DXVec3Dot(&ObjMove, &m_WallNormal);
							Move = ObjMove - fDot * m_WallNormal;

							// 交点を壁擦りベクトルでずらしたものを代入
							D3DXVECTOR3 ObjPos;
							ObjPos.x = InterPoint.x + Move.x;
							ObjPos.z = InterPoint.z + Move.z;
							pPos->x = ObjPos.x;
							pPos->z = ObjPos.z;
							if (pInterPoint != NULL)
							{// メモリが確保されている
								*pInterPoint = InterPoint;
							}
							bCol = true;
						}
					}
				}
			}
		}
	}

	return bCol;
}

//=============================================================================
//    テクスチャ番号設定処理
//=============================================================================
void CMeshWall::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMeshWall::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshWall::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    インデックスバッファ設定処理
//=============================================================================
void CMeshWall::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュウォールの座標設定処理
//=============================================================================
void CMeshWall::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュウォールの向き設定処理
//=============================================================================
void CMeshWall::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    メッシュウォールの色設定処理
//=============================================================================
void CMeshWall::SetCol(const D3DXCOLOR col)
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
//    1ブロック分の幅設定処理
//=============================================================================
void CMeshWall::SetWidth(const float fWidth)
{
	// 幅を設定
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float XPos = -(m_fWidth * m_nXBlock) / 2; // X座標を左に設定

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].pos.x = XPos;

				XPos += m_fWidth;   // X座標を横に進める
			}
			XPos -= m_fWidth * (m_nXBlock + 1); // X座標を進めていた分戻す

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1ブロック分の高さ設定処理
//=============================================================================
void CMeshWall::SetHeight(const float fHeight)
{
	// 高さを設定
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float YPos = (m_fHeight * m_nYBlock) / 2; // Y座標を上に設定

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].pos.y = YPos;
			}
			YPos -= m_fHeight;   // Y座標を下に進める

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMeshWall::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pIdxBuff);

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

			// 生成に必要な変数を宣言
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y座標を奥に設定
			float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
			float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// 垂直方向の分割数 + 1だけ繰り返し
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// 水平方向の分割数 + 1だけ繰り返し
				    // 頂点座標
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// 法線ベクトル
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 頂点カラー
					pVtx[nCntH].col = col;

					// テクスチャ座標
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X座標を横に進める
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
				YPos -= m_fHeight;                                                // Y座標を下に進める
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

				// ポインタを進める
				pVtx += m_nXBlock + 1;
			}

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();

			// インデックスバッファの生成
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    縦の分割数設定処理
//=============================================================================
void CMeshWall::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;

	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

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

			// 生成に必要な変数を宣言
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y座標を奥に設定
			float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
			float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// 垂直方向の分割数 + 1だけ繰り返し
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// 水平方向の分割数 + 1だけ繰り返し
				 // 頂点座標
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// 法線ベクトル
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 頂点カラー
					pVtx[nCntH].col = col;

					// テクスチャ座標
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X座標を横に進める
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
				YPos -= m_fHeight;                                                // Y座標を下に進める
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

				// ポインタを進める
				pVtx += m_nXBlock + 1;
			}

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();

			// インデックスバッファの生成
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    テクスチャU座標の分割数処理
//=============================================================================
void CMeshWall::SetTexSplitU(const int nTexSplitU)
{
	m_nTexSplitU = nTexSplitU;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

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
void CMeshWall::SetTexSplitV(const int nTexSplitV)
{
	m_nTexSplitV = nTexSplitV;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

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
//    ワールドマトリックス設定処理
//=============================================================================
void CMeshWall::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    テクスチャ番号取得処理
//=============================================================================
int CMeshWall::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshWall::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    インデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshWall::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshWall::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    メッシュフィールドの座標取得処理
//=============================================================================
D3DXVECTOR3 CMeshWall::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュフィールドの向き取得処理
//=============================================================================
D3DXVECTOR3 CMeshWall::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    メッシュフィールドの色取得処理
//=============================================================================
D3DXCOLOR CMeshWall::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1ブロック分の幅取得処理
//=============================================================================
float CMeshWall::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1ブロック分の高さ取得処理
//=============================================================================
float CMeshWall::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshWall::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshWall::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    テクスチャU座標の分割数取得処理
//=============================================================================
int CMeshWall::GetTexSplitU(void)
{
	return m_nTexSplitU;
}

//=============================================================================
//    テクスチャV座標の分割数取得処理
//=============================================================================
int CMeshWall::GetTexSplitV(void)
{
	return m_nTexSplitV;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshWall::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    インデックス数取得処理
//=============================================================================
int CMeshWall::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMeshWall::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshWall::GetMtxWorld(void)
{
	return m_MtxWorld;
}


//*****************************************************************************
//    CSetMeshWallの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSetMeshWall::CSetMeshWall(int nPriority, OBJTYPE objType) : CMeshWall(nPriority, objType)
{
	// 各種値のクリア
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetMeshWall::~CSetMeshWall()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetMeshWall *CSetMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority)
{
	CSetMeshWall *pSetMeshWall = NULL;     // 配置用メッシュウォールクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSetMeshWall == NULL)
		{// メモリが空になっている
			pSetMeshWall = new CSetMeshWall(nPriority);
			if (pSetMeshWall != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pSetMeshWall->SetTexIdx(nTexIdx);            // 配置用メッシュウォールのテクスチャ番号
				pSetMeshWall->SetPos(pos);                   // 配置用メッシュウォールの座標
				pSetMeshWall->SetRot(rot);                   // 配置用メッシュウォールの向き
				pSetMeshWall->SetCol(col);                   // 配置用メッシュウォールの色
				pSetMeshWall->SetWidth(fWidth);              // 1ブロック分の幅
				pSetMeshWall->SetHeight(fHeight);            // 1ブロック分の高さ
				pSetMeshWall->SetXBlock(nXBlock);            // 横の分割数
				pSetMeshWall->SetYBlock(nYBlock);            // 縦の分割数
				pSetMeshWall->SetTexSplitU(nTexSplitU);      // テクスチャU座標の分割数
				pSetMeshWall->SetTexSplitV(nTexSplitV);      // テクスチャV座標の分割数

				if (FAILED(pSetMeshWall->Init()))
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

	return pSetMeshWall;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetMeshWall::Init(void)
{
	// 色を透明にしておく
	D3DXCOLOR col = GetCol();
	col.a = -0.7f;
	SetCol(col);

	// 共通の初期化処理
	if (FAILED(CMeshWall::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetMeshWall::Uninit(void)
{
	// 共通の終了処理
	CMeshWall::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetMeshWall::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSetMeshWall::Draw(void)
{
	// 共通の描画処理
	CMeshWall::Draw();
}