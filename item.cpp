//*****************************************************************************
//
//     アイテムの処理[item.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "item.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CItemの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CItem::CItem(int nPriority, OBJTYPE objType) : CBillboardObject(nPriority, objType)
{
	// 各種値のクリア
	m_nType = 0;     // アイテムの種類番号
}

//=============================================================================
//    デストラクタ
//=============================================================================
CItem::~CItem()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType, int nPriority)
{
	CItem *pItem = NULL;                   // アイテムクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pItem == NULL)
		{// メモリが空になっている
			pItem = new CItem(nPriority);
			if (pItem != NULL)
			{// インスタンスを生成できた
				if (FAILED(pItem->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
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

	return pItem;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	// 各種値の代入
	m_nType = nType;    // アイテムの種類番号

	// 共通の初期化処理
	if (FAILED(CBillboardObject::Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CItem::Uninit(void)
{
	// 共通の終了処理
	CBillboardObject::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItem::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItem::Draw(void)
{
	// 共通の描画処理
	CBillboardObject::Draw();
}

//=============================================================================
//    種類番号を設定する処理
//=============================================================================
void CItem::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    種類番号を取得する処理
//=============================================================================
int CItem::GetType(void)
{
	return m_nType;
}


//*****************************************************************************
//    CSetItemの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSetItem::CSetItem(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetItem::~CSetItem()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetItem *CSetItem::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType, int nPriority)
{
	CSetItem *pSetItem = NULL;             // 配置用のアイテムクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSetItem == NULL)
		{// メモリが空になっている
			pSetItem = new CSetItem(nPriority);
			if (pSetItem != NULL)
			{// インスタンスを生成できた
				if (FAILED(pSetItem->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
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

	return pSetItem;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetItem::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	// 透明度を上げておく
	col.a = 0.5f;

	// 共通の初期化処理
	if (FAILED(CItem::Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetItem::Uninit(void)
{
	// 共通の終了処理
	CItem::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetItem::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSetItem::Draw(void)
{
	// 共通の描画処理
	CItem::Draw();
}

//=============================================================================
//    頂点情報を書き換える処理
//=============================================================================
void CSetItem::ChangeVertex(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pVtxBuff != NULL)
	{// 頂点バッファが取得できた
		// 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
		SetLength(fLength);

		// オフセットの角度を求める
		float fAngle = atan2f(GetWidth(), GetHeight());
		SetAngle(fAngle);

		// 頂点の回転を考慮して座標を計算
		float XPos[4];
		float YPos[4];
		XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * GetLength(), cosf(GetAngle()) * GetLength());
		XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());
		YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(GetAngle()) * GetLength(), -cosf(GetAngle()) * GetLength());

		// 頂点座標
		for (int nCntVer = 0; nCntVer < 4; nCntVer++)
		{// 頂点の数だけ繰り返し
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
		}

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[1].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[2].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);
		pVtx[3].col = D3DXCOLOR(GetCol().r, GetCol().g, GetCol().b, 0.7f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}