//*****************************************************************************
//
//     草の処理[grass.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "grass.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define GRASS_COLLISION_RANGE (12.0f)                           // 草との当たり判定を取る範囲
#define GRASS_SWAY_POWER      (D3DXVECTOR3(0.15f, 0.0f, 0.0f))  // 草の揺れる量

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CGrass::CGrass(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// 各種値のクリア
	m_aOffsetVertex[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aOffsetVertex[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BonePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BoneRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_State = STATE_NORMAL;
	m_nStateCounter = 0;
	m_SwayMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SwayMoveCurrent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRivisionSway = 0.0f;
	m_nSwayCounter = 0;
	m_nMaxSwayCounter = 0;
	m_bSway = false;
	m_fCameraLength = 0.0f;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGrass::~CGrass()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGrass *CGrass::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, bool bSway, int nPriority)
{
	CGrass *pGrass = NULL;                 // 草クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGrass == NULL)
		{// メモリが空になっている
			pGrass = new CGrass(nPriority);
			if (pGrass != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGrass->SetPos(pos);         // ポリゴンの座標
				pGrass->SetRot(rot);         // ポリゴンの向き
				pGrass->SetCol(col);         // ポリゴンの色
				pGrass->SetWidth(fWidth);    // ポリゴンの幅
				pGrass->SetHeight(fHeight);  // ポリゴンの高さ
				pGrass->SetSway(bSway);      // 勝手に揺れるかどうか

				if (FAILED(pGrass->Init()))
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

	return pGrass;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGrass::Init(void)
{
	m_nStateCounter = 250;

	m_nMaxSwayCounter = rand() % 150 + 150;
	m_nSwayCounter = m_nMaxSwayCounter / 2;
	m_SwayMove = GRASS_SWAY_POWER;

	if (FAILED(CScene3D::Init()))
	{// 初期化に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGrass::Uninit(void)
{
	// 共通の終了処理
	CScene3D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGrass::Update(void)
{
	if (m_State == STATE_COL_LEFT)
	{// 左に揺れるなら
		m_BonePos.x += (20.0f - m_BonePos.x) * 0.07f;
		m_BonePos.y += (8.0f - m_BonePos.y) * 0.07f;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
		if (pVtxBuff != NULL)
		{// 頂点バッファが生成されている
		    // 頂点情報の設定
			VERTEX_3D *pVtx;

			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			D3DXVECTOR3 Pos;

			for (int nCntVer = 0; nCntVer < 2; nCntVer++)
			{// 頂点数だけ繰り返し
				D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

				// 頂点座標
				pVtx[nCntVer].pos = Pos;
			}

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();
		}
	}
	else if (m_State == STATE_COL_RIGHT)
	{// 右に揺れるなら
		m_BonePos.x += (-20.0f - m_BonePos.x) * 0.07f;
		m_BonePos.y += (8.0f - m_BonePos.y) * 0.07f;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
		if (pVtxBuff != NULL)
		{// 頂点バッファが生成されている
		    // 頂点情報の設定
			VERTEX_3D *pVtx;

			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			D3DXVECTOR3 Pos;

			for (int nCntVer = 0; nCntVer < 2; nCntVer++)
			{// 頂点数だけ繰り返し
				D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

				// 頂点座標
				pVtx[nCntVer].pos = Pos;
			}

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();
		}
	}
	else if (m_State == STATE_NORMAL)
	{// なにもしない
		if (m_bSway == true)
		{// 揺れる草なら
			if (rand() % 20 == 0)
			{
				D3DXVECTOR3 CameraPosV = CManager::GetCamera()->GetPosV();
				m_fCameraLength = sqrtf((GetPos().x - CameraPosV.x) * (GetPos().x - CameraPosV.x) + (GetPos().z - CameraPosV.z) * (GetPos().z - CameraPosV.z));
			}

			if (m_fCameraLength <= 1000.0f)
			{// カメラとの距離が一定である
				m_nSwayCounter++;
				m_fRivisionSway += D3DX_PI / m_nMaxSwayCounter;
				if (m_nSwayCounter % m_nMaxSwayCounter == 0)
				{// 管理カウンターが一定の値に達した
					m_fRivisionSway = 0.0f;
					m_SwayMove *= -1;
				}

				m_SwayMoveCurrent.x = sinf(m_fRivisionSway) * m_SwayMove.x;
				m_SwayMoveCurrent.y = sinf(m_fRivisionSway) * m_SwayMove.y;
				m_SwayMoveCurrent.z = sinf(m_fRivisionSway) * m_SwayMove.z;
				m_BonePos += m_SwayMoveCurrent;

				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
				if (pVtxBuff != NULL)
				{// 頂点バッファが生成されている
					// 頂点情報の設定
					VERTEX_3D *pVtx;

					// 頂点バッファをロックし,頂点データへのポインタを取得
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					D3DXVECTOR3 Pos;

					for (int nCntVer = 0; nCntVer < 2; nCntVer++)
					{// 頂点数だけ繰り返し
						D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

						// 頂点座標
						pVtx[nCntVer].pos = Pos;
					}

					// 頂点バッファをアンロックする
					pVtxBuff->Unlock();
				}
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CGrass::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Culling;

			// カリングしない設定に
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		    // アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
			pDevice->SetRenderState(D3DRS_ALPHAREF, 140);             // 透明度が0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

			// 共通の処理
			CScene3D::Draw();

			// アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// カリングの設定を戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CGrass::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CScene3D::SetMtxWorld(mtxWorld);

	// ボーンのワールドマトリックスを作成
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_BoneMtxWorld);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_BonePos.x, m_BonePos.y, m_BonePos.z);
	D3DXMatrixMultiply(&m_BoneMtxWorld, &m_BoneMtxWorld, &mtxTrans);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGrass::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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

	// ボーンのオフセットを設定
	m_BonePos = D3DXVECTOR3(0.0f, cosf(fAngle) * fLength, 0.0f);
	m_BonePosDef = m_BonePos;

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, cosf(fAngle) * fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * fLength, cosf(fAngle) * fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 0.0f);

	// オフセットを設定
	m_aOffsetVertex[0] = pVtx[0].pos - m_BonePos;
	m_aOffsetVertex[1] = pVtx[1].pos - m_BonePos;

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
//    当たり判定処理
//=============================================================================
bool CGrass::Collision(D3DXVECTOR3 *pPos, float fColRange)
{
	bool bCol = false;       // 当たっているかどうか
	float fHitRange = 0.0f;  // 当たった判定を返す範囲
	float fLength = 0.0f;    // ビルボードの座標と引数の座標との距離

	// 当たり判定を取る長さを計算
	fHitRange = sqrtf((fColRange + GRASS_COLLISION_RANGE) * (fColRange + GRASS_COLLISION_RANGE));

	// ビルボードと引数の座標との距離を計算
	fLength = sqrtf((GetPos().x - pPos->x) * (GetPos().x - pPos->x) + (GetPos().y - pPos->y) * (GetPos().y - pPos->y) + (GetPos().z - pPos->z) * (GetPos().z - pPos->z));

	// 判定開始
	if (fLength <= fHitRange)
	{// ビルボードの座標と引数の座標との距離が当たり判定を取る長さより短い
		bCol = true;   // 当たった判定にする
		float fAngle = atan2f(pPos->x - GetPos().x, pPos->z - GetPos().z) + GetRot().y;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}
		if (fAngle < 0.0f)
		{
			if (m_State != STATE_COL_LEFT)
			{
				m_nStateCounter = 0;
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
				m_State = STATE_COL_LEFT;
			}
		}
		else if(fAngle > 0.0f)
		{
			if (m_State != STATE_COL_RIGHT)
			{
				m_nStateCounter = 0;
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
				m_State = STATE_COL_RIGHT;
			}
		}
	}
	else
	{
		if (m_nStateCounter <= 200)
		{
			m_nStateCounter++;
			if (m_State != STATE_NORMAL)
			{
				m_State = STATE_NORMAL;
				m_nStateCounter = 0;
			}
			m_BonePos += (m_BonePosDef - m_BonePos) * 0.08f;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
			if (pVtxBuff != NULL)
			{// 頂点バッファが生成されている
			    // 頂点情報の設定
				VERTEX_3D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				D3DXVECTOR3 Pos;

				for (int nCntVer = 0; nCntVer < 2; nCntVer++)
				{// 頂点数だけ繰り返し
					D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

					// 頂点座標
					pVtx[nCntVer].pos = Pos;
				}

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();
			}
		}
	}

	return bCol;
}


//=============================================================================
//    勝手に揺れるかどうか設定する処理
//=============================================================================
void CGrass::SetSway(const bool bSway)
{
	m_bSway = bSway;
}

//=============================================================================
//    勝手に揺れるかどうか取得する処理
//=============================================================================
bool CGrass::GetSway(void)
{
	return m_bSway;
}