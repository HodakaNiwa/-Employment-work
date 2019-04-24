//*****************************************************************************
//
//     コンボ数UIの処理[combo.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _COMBO_H_
#define _COMBO_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_COMBO_NUMBER   (5)      // コンボ数の数字の最大値

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CNumber;
class CScene2D;

//*****************************************************************************
//     コンボ数UIクラスの定義
//*****************************************************************************
class CCombo : public CScene
{
public:    // 誰でもアクセス可能
	//--------------------
	//  状態
	//--------------------
	typedef enum
	{
		STATE_APPEAR = 0,   // 出現状態
		STATE_NORMAL,       // 通常状態
		STAET_MAX
	}STATE;

	CCombo(int nPriority = 3, OBJTYPE objType = OBJTYPE_COMBOUI);
	~CCombo();

	static CCombo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumberTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetLogoTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetCombo(const int nCombo);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetCombo(void);

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9 m_pTexture;                     // 数字テクスチャへのポインタ
	STATE              m_State;                        // 状態
	int                m_nCounter;                     // 状態管理カウンター
	D3DXVECTOR3        m_Pos;                          // コンボ数の座標
	D3DXCOLOR          m_Col;                          // コンボ数の色
	float              m_fWidth;                       // 数字ポリゴン１つ分の幅
	float              m_fHeight;                      // 数字ポリゴン１つ分の高さ
	int                m_nCombo;                       // 現在のコンボ数
	int                m_nDigit;                       // 現在のコンボ数の最大値
	CScene2D           *m_pComboLogo[3];               // UI用の文字ポリゴンクラスへのポインタ
	CNumber            *m_apNumber[MAX_COMBO_NUMBER];  // 数字ポリゴンクラス型のポインタ
};

#endif