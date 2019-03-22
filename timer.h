//*****************************************************************************
//
//     タイマーの処理[timer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
//     タイマークラスの定義
//*****************************************************************************
class CTimer : public CScene
{
public:    // 誰でもアクセス可能
	CTimer(int nPriority = 3, OBJTYPE objType = OBJTYPE_TIMER);
	~CTimer();

	static CTimer *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTimer, int nWarningTime, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTimerTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetTimer(const int nTimer);
	void SetWarningTime(const int nWarningTime);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetTimer(void);
	int GetWarningTime(void);

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9 m_pTexture;         // 数字テクスチャへのポインタ
	D3DXVECTOR3        m_Pos;              // タイマーの座標
	D3DXCOLOR          m_Col;              // タイマーの色
	float              m_fWidth;           // タイマーの数字ポリゴン１つ分の幅
	float              m_fHeight;          // タイマーの数字ポリゴン１つ分の高さ
	CNumber            **m_apNumber;       // 数字ポリゴンクラス型のポインタ
	int                m_nDigit;           // タイマーの桁数
	int                m_nTimer;           // タイマー
	int                m_nWarningTime;     // タイマーが赤くなるタイミング
};

#endif