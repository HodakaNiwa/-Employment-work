//*****************************************************************************
//
//     タイマーの処理[timer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "timer.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define TIMER_NUMBER_INTERVAL (1.0f)  // 数字と数字の間隔

//=============================================================================
//    コンストラクタ
//=============================================================================
CTimer::CTimer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_apNumber = NULL;                           // 数字ポリゴンクラスへのポインタ
	m_nDigit = 0;                                // タイマーの桁数
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // タイマーの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // タイマーの色
	m_fWidth = 0.0f;                             // タイマーの数字ポリゴン１つ分の幅
	 m_fHeight = 0.0f;                           // タイマーの数字ポリゴン１つ分の幅
	m_nTimer = 0;                                // タイマー
	m_nWarningTime = 0;                          // タイマーが赤くなるタイミング
	m_pTexture = NULL;                           // テクスチャへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTimer, int nWarningTime, int nPriority)
{
	CTimer *pTimer = NULL;                 // タイマークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pTimer == NULL)
		{// メモリが空になっている
			pTimer = new CTimer(nPriority);
			if (pTimer != NULL)
			{// メモリを確保できた
				// 各種値の設定
				pTimer->SetPos(pos);                    // タイマーの座標
				pTimer->SetCol(col);                    // タイマーの色
				pTimer->SetWidth(fWidth);               // タイマーの数字ポリゴン１つ分の幅
				pTimer->SetHeight(fHeight);             // タイマーの数字ポリゴン１つ分の高さ
				pTimer->SetTimer(nTimer);               // タイマー
				pTimer->SetWarningTime(nWarningTime);   // タイマーが赤くなるタイミング

				if (FAILED(pTimer->Init()))
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

	return pTimer;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTimer::Init(void)
{
	// 各種値の初期化
	D3DXVECTOR3 NumberPos = m_Pos;   // 座標
	D3DXCOLOR NumberCol = m_Col;     // 色
	float fNumberWidth = m_fWidth;   // 幅
	float fNumberHeight = m_fHeight; // 高さ
	int nNumber = 0;                 // 表示する数字の計算用

	// タイマーの桁数を計算し、桁数分メモリを確保
	m_nDigit = (int)log10f((float)(m_nTimer / 60)) + 1;   // 桁数を求める
	if (m_nDigit <= 0) { m_nDigit = 1; }	              // 0以下のとき1にする

	if (m_apNumber == NULL)
	{// メモリが確保できる状態である
		m_apNumber = new CNumber*[m_nDigit];  // 桁数分メモリ確保
		if (m_apNumber != NULL)
		{// メモリを確保できた
			for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
			{// タイマーの桁数分繰り返し
				m_apNumber[nCntTimer] = NULL;   // いったんメモリを空にする
				if (m_apNumber[nCntTimer] == NULL)
				{// メモリが確保できる状態にある
					// 表示する数字の計算
					nNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

					// 数字ポリゴンのインスタンス生成処理
					m_apNumber[nCntTimer] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());

					// 横に進める
					NumberPos.x -= m_fWidth + (m_fWidth * TIMER_NUMBER_INTERVAL);
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	// 数字ポリゴンの開放処理
	if(m_apNumber != NULL)
	{// メモリが確保されている
		for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
		{// タイマーの桁数分繰り返し
			if (m_apNumber[nCntTimer] != NULL)
			{// メモリが確保されている
			    // 数字ポリゴンの終了処理
				m_apNumber[nCntTimer]->Uninit();
				m_apNumber[nCntTimer] = NULL;
			}
		}
		delete[] m_apNumber;
		m_apNumber = NULL;
	}

	// タイマーの消去処理
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTimer::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム中だったら
		if (CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
		{// 通常のゲームだったら
			if (m_nTimer > 0)
			{// タイマーがまだある
			    // タイマーを減らす
				m_nTimer--;
				if (m_nTimer % 60 == 0)
				{// アップデートが60回まわった
					if (m_apNumber != NULL)
					{// 数字ポリゴンが生成されている
						int nSetNumber = 0;   // 表示する数字の計算用
						for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
						{// タイマーの桁数分繰り返し
							if (m_nTimer == m_nWarningTime)
							{// タイマーがなくなりそうならば
								m_apNumber[nCntTimer]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
								//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WARNING);
							}

							// 表示する数字の計算
							nSetNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

							// 数字ポリゴンの数字設定処理
							m_apNumber[nCntTimer]->SetNumber(nSetNumber);
						}
					}
				}
			}
			else
			{// タイマーがなくなった
				if (CManager::GetMode() == CManager::MODE_GAME)
				{// ゲームモード中だったら
					CGame *pGame = CManager::GetGame();
					if (pGame->GetState() != CGame::STATE_END)
					{// ゲーム終了状態でない
						pGame->SetState(CGame::STATE_END);
					}
				}
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CTimer::Draw(void)
{

}

//=============================================================================
//    数字テクスチャを設定する処理
//=============================================================================
void CTimer::SetTimerTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// テクスチャへのポインタが取得できた
		if (m_apNumber != NULL)
		{// 数字ポリゴンが生成されている
			for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
			{// タイマーの桁数分繰り返し
				if (m_apNumber[nCntTimer] != NULL)
				{// 数字ポリゴンが生成されている
					m_apNumber[nCntTimer]->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    タイマーの座標を設定する処理
//=============================================================================
void CTimer::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    タイマーの色を設定する処理
//=============================================================================
void CTimer::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
	if (m_apNumber != NULL)
	{// 数字ポリゴンが生成されている
		for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
		{// タイマーの桁数分繰り返し
			if (m_apNumber[nCntTimer] != NULL)
			{// 数字ポリゴンが生成されている
				m_apNumber[nCntTimer]->SetCol(col);
			}
		}
	}
}

//=============================================================================
//    タイマーの数字ポリゴン１つ分の幅を設定する処理
//=============================================================================
void CTimer::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    タイマーの数字ポリゴン１つ分の高さを設定する処理
//=============================================================================
void CTimer::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    タイマーを設定する処理
//=============================================================================
void CTimer::SetTimer(const int nTimer)
{
	m_nTimer = nTimer;
}

//=============================================================================
//    タイマーが赤くなるタイミングを設定する処理
//=============================================================================
void CTimer::SetWarningTime(const int nWarningTime)
{
	m_nWarningTime = nWarningTime;
}

//=============================================================================
//    タイマーの座標を取得する処理
//=============================================================================
D3DXVECTOR3 CTimer::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    タイマーの色を取得する処理
//=============================================================================
D3DXCOLOR CTimer::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    タイマーの数字ポリゴン１つ分の幅を取得する処理
//=============================================================================
float CTimer::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    タイマーの数字ポリゴン１つ分の高さを取得する処理
//=============================================================================
float CTimer::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    タイマーを取得する処理
//=============================================================================
int CTimer::GetTimer(void)
{
	return m_nTimer;
}

//=============================================================================
//    タイマーの赤くなるタイミングを取得する処理
//=============================================================================
int CTimer::GetWarningTime(void)
{
	return m_nWarningTime;
}