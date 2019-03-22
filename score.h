//*****************************************************************************
//
//     スコアの処理[score.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define  SCORE_NUMBER_MAX  (8)    // スコアの桁数

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
//     スコアクラスの定義
//*****************************************************************************
class CScore : public CScene
{
public:    // 誰でもアクセス可能
	CScore(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCORE);
	~CScore();

	static CScore *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScoreTexture(LPDIRECT3DTEXTURE9 pTexture);
	void AddScore(int nScore);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetScore(const int nScore);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetScore(void);

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9 m_pTexture;                     // 数字テクスチャへのポインタ
	CNumber            *m_apNumber[SCORE_NUMBER_MAX];  // 数字ポリゴンクラス型のポインタ
	D3DXVECTOR3        m_Pos;                          // スコアの座標
	D3DXCOLOR          m_Col;                          // スコアの色
	float              m_fWidth;                       // スコアの数字ポリゴン１個分の幅
	float              m_fHeight;                      // スコアの数字ポリゴン１個分の高さ
	int                m_nScore;                       // スコア
};

#endif