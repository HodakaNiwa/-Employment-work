//*****************************************************************************
//
//     コンボUIの処理[combo.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "combo.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "ui.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define COMBO_MOVE_SPEED      (30.0f)   // コンボUIが既定の位置に移動するスピード
#define COMBO_DESTORY_TIMING  (300)     // コンボUIが消えるまでの時間(この秒数でコンボの繋がりやすさが調整できる)7
#define COMBO_MOVESTOP_POS    (1170.0f) // コンボUIの移動を止める位置
#define COMBO_NUMBER_INTERVAL (0.7f)    // 数字と数字の間隔

#define COMBO_LOGO_POS_0     (D3DXVECTOR3(64.0f, 4.0f, 0.0f))    // コンボUIの1つ目のロゴポリゴンのオフセット位置
#define COMBO_LOGO_COL_0     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))    // コンボUIの1つ目のロゴポリゴンの色
#define COMBO_LOGO_WIDTH_0   (40.0f)                             // コンボUIの1つ目のロゴポリゴンの幅
#define COMBO_LOGO_HEIGHT_0  (20.0f)                             // コンボUIの1つ目のロゴポリゴンの高さ

#define COMBO_LOGO_POS_1     (D3DXVECTOR3(63.0f, 43.0f, 0.0f))   // コンボUIの2つ目のロゴポリゴンのオフセット位置
#define COMBO_LOGO_COL_1     (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))    // コンボUIの2つ目のロゴポリゴンの色
#define COMBO_LOGO_WIDTH_1   (180.5f)                            // コンボUIの2つ目のロゴポリゴンの幅
#define COMBO_LOGO_HEIGHT_1  (4.5f)                              // コンボUIの2つ目のロゴポリゴンの高さ

#define COMBO_LOGO_POS_2     (D3DXVECTOR3(60.0f, 40.0f, 0.0f))   // コンボUIの3つ目のロゴポリゴンのオフセット位置
#define COMBO_LOGO_COL_2     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))    // コンボUIの3つ目のロゴポリゴンの色
#define COMBO_LOGO_WIDTH_2   (180.0f)                            // コンボUIの3つ目のロゴポリゴンの幅
#define COMBO_LOGO_HEIGHT_2  (4.0f)                              // コンボUIの3つ目のロゴポリゴンの高さ

//=============================================================================
//    コンストラクタ
//=============================================================================
CCombo::CCombo(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	for (int nCntNumber = 0; nCntNumber < MAX_COMBO_NUMBER; nCntNumber++)
	{// コンボUIの桁数分繰り返し
		m_apNumber[nCntNumber] = NULL;           // 数字ポリゴンクラスへのポインタ
	}
	for (int nCntLogo = 0; nCntLogo < 3; nCntLogo++)
	{// ロゴポリゴンの数だけ繰り返し
		m_pComboLogo[nCntLogo] = NULL;           // ロゴポリゴンクラスへのポインタ
	}
	m_State = STATE_APPEAR;                      // 状態
	m_nCounter = 0;                              // 状態管理カウンター
	m_nDigit = 0;                                // コンボUIの桁数
	m_nCombo = 0;                                // コンボ数
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // コンボUIの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // コンボUIの色
	m_fWidth = 0.0f;                             // コンボUIの数字ポリゴン１つ分の幅
	m_fHeight = 0.0f;                            // コンボUIの数字ポリゴン１つ分の幅
	m_pTexture = NULL;                           // テクスチャへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCombo::~CCombo()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CCombo *CCombo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CCombo *pCombo = NULL;                 // コンボUIクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pCombo == NULL)
		{// メモリが空になっている
			pCombo = new CCombo(nPriority);
			if (pCombo != NULL)
			{// メモリを確保できた
			    // 各種値の設定
				pCombo->SetPos(pos);         // コンボUIの座標
				pCombo->SetCol(col);         // コンボUIの色
				pCombo->SetWidth(fWidth);    // コンボUIの数字ポリゴン１つ分の幅
				pCombo->SetHeight(fHeight);  // コンボUIの数字ポリゴン１つ分の高さ

				if (FAILED(pCombo->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// メモリを確保できなかった
				return NULL;
			}
		}
		else
		{// メモリが空ではない
			return NULL;
		}
	}
	else
	{// これ以上生成できない
		return NULL;
	}

	return pCombo;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCombo::Init(void)
{
	D3DXVECTOR3 LogoPos = m_Pos + COMBO_LOGO_POS_1;
	D3DXCOLOR LogoCol = COMBO_LOGO_COL_1;
	float fLogoWidth = COMBO_LOGO_WIDTH_1;
	float fLogoHeight = COMBO_LOGO_HEIGHT_1;
	if (m_pComboLogo[1] == NULL)
	{// メモリが確保できる状態である
		m_pComboLogo[1] = CScene2D::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, 0.0f, GetPriority());
	}

	LogoPos = m_Pos + COMBO_LOGO_POS_2;
	LogoCol = COMBO_LOGO_COL_2;
	fLogoWidth = COMBO_LOGO_WIDTH_2;
	fLogoHeight = COMBO_LOGO_HEIGHT_2;
	if (m_pComboLogo[2] == NULL)
	{// メモリが確保できる状態である
		m_pComboLogo[2] = CScene2D::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, 0.0f, GetPriority());
	}

	// 各種値の初期化
	D3DXVECTOR3 NumberPos = m_Pos;   // 座標
	D3DXCOLOR NumberCol = m_Col;     // 色
	float fNumberWidth = m_fWidth;   // 幅
	float fNumberHeight = m_fHeight; // 高さ
	int nNumber = m_nCombo = 1;      // 表示する数字
	m_nDigit = 1;                    // 桁数は1にしておく

	// 数字ポリゴンを1つだけ生成する
	if (m_apNumber[0] == NULL)
	{// メモリが確保できる状態である
		m_apNumber[0] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());
	}

	// ロゴポリゴンを生成する
	LogoPos = m_Pos + COMBO_LOGO_POS_0;
	LogoCol = COMBO_LOGO_COL_0;
	fLogoWidth = COMBO_LOGO_WIDTH_0;
	fLogoHeight = COMBO_LOGO_HEIGHT_0;
	if (m_pComboLogo[0] == NULL)
	{// メモリが確保できる状態である
		m_pComboLogo[0] = CScene2D::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, 0.0f, GetPriority());
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCombo::Uninit(void)
{
	// 数字ポリゴンの開放処理
	for (int nCntCombo = 0; nCntCombo < MAX_COMBO_NUMBER; nCntCombo++)
	{// コンボの桁数分繰り返し
		if (m_apNumber[nCntCombo] != NULL)
		{// メモリが確保されている
			// 数字ポリゴンの終了処理
			m_apNumber[nCntCombo]->Uninit();
			m_apNumber[nCntCombo] = NULL;
		}
	}

	// ロゴポリゴンの開放処理
	for (int nCntLogo = 0; nCntLogo < 3; nCntLogo++)
	{// ロゴポリゴンの数だけ繰り返し
		if (m_pComboLogo[nCntLogo] != NULL)
		{// メモリが確保されている
			m_pComboLogo[nCntLogo]->Uninit();
			m_pComboLogo[nCntLogo] = NULL;
		}
	}

	if(CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面だったら
		// 最大コンボ数の更新
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// ゲームクラスが取得できた
			int nComboMax = pGame->GetComboMax();
			if (nComboMax < m_nCombo)
			{// 最大コンボ数が更新された
				pGame->SetComboMax(m_nCombo);
			}
			// UIクラスのポインタを空にする
			CUI *pUi = pGame->GetUI();
			if (pUi != NULL)
			{// ポインタが取得できた
				pUi->ReleaseCombo();
			}
		}
	}

	// コンボUIの消去処理
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CCombo::Update(void)
{
	m_nCounter++;   // カウンターを増やす
	if (m_State == STATE_APPEAR)
	{// 出現状態だったら
		// 位置をずらす
		m_Pos.x -= COMBO_MOVE_SPEED;
		if (m_Pos.x <= COMBO_MOVESTOP_POS)
		{// 既定の位置に到達した
			m_Pos.x = COMBO_MOVESTOP_POS;   // 位置を戻す
			m_State = STATE_NORMAL;         // 状態を通常状態に
		}

		// 数字の位置をずらす
		D3DXVECTOR3 NumberPos = m_Pos;   // 座標
		for (int nCntCombo = 0; nCntCombo < m_nDigit; nCntCombo++)
		{// コンボの桁数分繰り返し
			if (m_apNumber[nCntCombo] != NULL)
			{// メモリが確保されている
				m_apNumber[nCntCombo]->SetPos(NumberPos);
			}
			// 横に進める
			NumberPos.x -= m_fWidth + (m_fWidth * 0.5f);
		}


		// ロゴの位置をずらす
		D3DXVECTOR3 LogoPos = m_Pos + COMBO_LOGO_POS_0;
		if (m_pComboLogo[0] != NULL)
		{// メモリが確保されている
			m_pComboLogo[0]->SetPos(LogoPos);
		}

		LogoPos = m_Pos + COMBO_LOGO_POS_1;
		if (m_pComboLogo[1] != NULL)
		{// メモリが確保されている
			m_pComboLogo[1]->SetPos(LogoPos);
		}

		LogoPos = m_Pos + COMBO_LOGO_POS_2;
		if (m_pComboLogo[2] != NULL)
		{// メモリが確保されている
			m_pComboLogo[2]->SetPos(LogoPos);
		}
	}
	else if (m_State == STATE_NORMAL)
	{// 通常状態ならば
		if(m_nCounter % COMBO_DESTORY_TIMING == 0)
		{// 最後のコンボからある程度時間がたった
			Uninit();
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CCombo::Draw(void)
{

}

//=============================================================================
//    数字テクスチャを設定する処理
//=============================================================================
void CCombo::SetNumberTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// テクスチャへのポインタが取得できた
		for (int nCntCombo = 0; nCntCombo < m_nDigit; nCntCombo++)
		{// コンボUIの桁数分繰り返し
			if (m_apNumber[nCntCombo] != NULL)
			{// 数字ポリゴンが生成されている
				m_apNumber[nCntCombo]->BindTexture(m_pTexture);
			}
		}
	}
}

//=============================================================================
//    ロゴテクスチャを設定する処理
//=============================================================================
void CCombo::SetLogoTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx)
{
	if (m_pComboLogo[nIdx] != NULL)
	{// メモリが確保されている
		m_pComboLogo[nIdx]->BindTexture(pTexture);
	}
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
void CCombo::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    色を設定する処理
//=============================================================================
void CCombo::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    数字ポリゴン１つ分の幅を設定する処理
//=============================================================================
void CCombo::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    数字ポリゴン１つ分の高さを設定する処理
//=============================================================================
void CCombo::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    コンボ数を設定する処理
//=============================================================================
void CCombo::SetCombo(const int nCombo)
{
	m_nCombo = nCombo;
	m_nCounter = 0;

	int nNumber = 0;    // 表示する数字
	for (int nCntCombo = 0; nCntCombo < m_nDigit; nCntCombo++)
	{// コンボの桁数分繰り返し
		if (m_apNumber[nCntCombo] != NULL)
		{// メモリが確保されている
		    // 表示する数字の計算
 			nNumber = (m_nCombo) % ((int)powf(10.0f, (float)nCntCombo) * 10) / (int)powf(10.0f, (float)nCntCombo);
			m_apNumber[nCntCombo]->SetNumber(nNumber);
		}
	}

	// コンボの桁数を計算する
	int nDigit = (int)log10f((float)(m_nCombo)) + 1;   // 桁数を求める
	if (nDigit <= 0) { nDigit = 1; }	               // 0以下のとき1にする
	if (nDigit != m_nDigit)
	{// 現在生成されている数字の桁数と異なる
		D3DXVECTOR3 NumberPos = m_Pos;   // 座標
		D3DXCOLOR NumberCol = m_Col;     // 色
		float fNumberWidth = m_fWidth;   // 幅
		float fNumberHeight = m_fHeight; // 高さ

		if (nDigit > m_nDigit)
		{// 桁数が増えている
			for (int nCntCombo = 0; nCntCombo < nDigit; nCntCombo++)
			{// コンボの桁数分繰り返し
			    // 表示する数字の計算
				nNumber = (m_nCombo) % ((int)powf(10.0f, (float)nCntCombo) * 10) / (int)powf(10.0f, (float)nCntCombo);

				if (m_apNumber[nCntCombo] == NULL)
				{// メモリが確保されていない
					// 数字ポリゴンのインスタンス生成処理
					m_apNumber[nCntCombo] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());
					m_apNumber[nCntCombo]->BindTexture(m_pTexture);
				}
				// 横に進める
				NumberPos.x -= m_fWidth + (m_fWidth * COMBO_NUMBER_INTERVAL);
			}
		}
		else if (nDigit < m_nDigit)
		{// 桁数が減っている
			for (int nCntCombo = m_nDigit - 1; nCntCombo > nDigit - 1; nCntCombo--)
			{// コンボの桁数分繰り返し
				if (m_apNumber[nCntCombo] != NULL)
				{// メモリが確保されている
					m_apNumber[nCntCombo]->Uninit();
					m_apNumber[nCntCombo] = NULL;
				}
			}
		}

		// 桁数を設定しなおす
		m_nDigit = nDigit;
	}
}

//=============================================================================
//    座標を取得する処理
//=============================================================================
D3DXVECTOR3 CCombo::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    タイマーの色を取得する処理
//=============================================================================
D3DXCOLOR CCombo::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    数字ポリゴン１つ分の幅を取得する処理
//=============================================================================
float CCombo::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    数字ポリゴン１つ分の高さを取得する処理
//=============================================================================
float CCombo::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    コンボ数を取得する処理
//=============================================================================
int CCombo::GetCombo(void)
{
	return m_nCombo;
}