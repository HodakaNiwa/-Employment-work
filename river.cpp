//*****************************************************************************
//
//     川の処理[river.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "river.h"
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
CRiver::CRiver(int nPriority, OBJTYPE objType) : CMeshField(nPriority, objType)
{
	// 各種値のクリア
	m_fAddTexU = 0.0f;   // テクスチャU座標をどれくらい動かすか
	m_fAddTexV = 0.0f;   // テクスチャV座標をどれくらい動かすか
	m_fTexU = 0.0f;      // テクスチャU座標
	m_fTexV = 0.0f;      // テクスチャV座標
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRiver::~CRiver()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRiver *CRiver::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, float fAddTexU, float fAddTexV, char *pFileName, int nPriority)
{
	CRiver *pRiver = NULL;                 // 川クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pRiver == NULL)
		{// メモリが空になっている
			pRiver = new CRiver(nPriority);
			if (pRiver != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pRiver->SetPos(pos);                   // メッシュフィールドの座標
				pRiver->SetRot(rot);                   // メッシュフィールドの向き
				pRiver->SetCol(col);                   // メッシュフィールドの色
				pRiver->SetWidth(fWidth);              // 1ブロック分の幅
				pRiver->SetHeight(fHeight);            // 1ブロック分の高さ
				pRiver->SetXBlock(nXBlock);            // 横の分割数
				pRiver->SetZBlock(nZBlock);            // 奥行の分割数
				pRiver->SetTexSplitU(nTexSplitU);      // テクスチャU座標の分割数
				pRiver->SetTexSplitV(nTexSplitV);      // テクスチャV座標の分割数
				pRiver->SetAddTexU(fAddTexU);          // テクスチャU座標をどれくらい動かすか
				pRiver->SetAddTexV(fAddTexV);          // テクスチャV座標をどれくらい動かすか
				pRiver->SetBinaryFileName(pFileName);  // 頂点情報が格納されたバイナリファイル名

				if (FAILED(pRiver->Init()))
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

	return pRiver;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRiver::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CMeshField::Init()))
	{
		return E_FAIL;
	}

	// 法線ベクトル情報を書き換えておく
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < GetZBlock() + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
				pVtx[nCntH].col = GetCol();
			}
			// ポインタを進める
			pVtx += GetXBlock() + 1;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRiver::Uninit(void)
{
	// 共通の終了処理
	CMeshField::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRiver::Update(void)
{
	// テクスチャ座標を動かす
	m_fTexU += m_fAddTexU;
	m_fTexV += m_fAddTexV;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // テクスチャU座標
		float fTexV = 0.0f;   // テクスチャV座標

		for (int nCntV = 0; nCntV < GetZBlock() + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				// テクスチャ座標を設定
				pVtx[nCntH].tex.x = fTexU + m_fTexU;
				pVtx[nCntH].tex.y = fTexV + m_fTexV;

				// テクスチャU座標をずらす
				fTexU += (1.0f / (GetXBlock())) * GetTexSplitU();
			}
			// テクスチャU座標をずらした分戻す
			fTexU -= ((1.0f / (GetXBlock())) * GetTexSplitU()) * (GetXBlock() + 1);

			// テクスチャV座標を進める
			fTexV += ((1.0f / GetZBlock()) * GetTexSplitV());

			// ポインタを進める
			pVtx += GetXBlock() + 1;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CRiver::Draw(void)
{
	// 共通の描画処理
	CMeshField::Draw();
}

//=============================================================================
//    テクスチャU座標をどれくらい動かすか設定する処理
//=============================================================================
void CRiver::SetAddTexU(const float fAddTexU)
{
	m_fAddTexU = fAddTexU;
}

//=============================================================================
//    テクスチャV座標をどれくらい動かすか設定する処理
//=============================================================================
void CRiver::SetAddTexV(const float fAddTexV)
{
	m_fAddTexV = fAddTexV;
}