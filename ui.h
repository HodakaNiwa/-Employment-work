//*****************************************************************************
//
//     UIの処理[ui.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define UI_TIMER_PRIORITY        (11)    // タイマーの処理優先順位
#define UI_SCORE_PRIORITY        (11)    // スコアの処理優先順位
#define UI_LIFEGAUGE_PRIORITY    (11)    // 体力ゲージの処理優先順位
#define UI_ORBITGAUGE_PRIORITY   (11)    // 軌跡ゲージの処理優先順位
#define UI_SPECIALGAUGE_PRIORITY (11)    // 必殺ゲージの処理優先順位
#define UI_PAPER_PRIORITY        (11)    // 紙の処理優先順位
#define UI_NUMSTAGE_PRIORITY     (11)    // ステージ数の処理優先順位
#define UI_PURPOSE_PRIORITY      (11)    // 目的の処理優先順位
#define UI_COMBO_PRIORITY        (11)    // コンボUIの処理優先順位

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CTimer;
class CScore;
class CCombo;
class CGaugeLife2D;
class CGaugeOrbitSlash2D;
class CGaugeSpecial2D;
class CScene2D;
class CTextureManager;

//*****************************************************************************
//    UIクラスの定義
//*****************************************************************************
class CUI
{
public:    // 誰でもアクセス可能
	//-----------------
	//  状態
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,    // 通常状態
		STATE_ALPHACHANGE,   // 透明度を下げている状態
		STATE_MAX
	}STATE;

	CUI();
	~CUI();

	static CUI *Create(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager);

	HRESULT Init(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager);
	void Uninit(void);
	void Update(void);
	void SetDisp(const bool bDisp);
	bool GetDisp(void);
	void CreateCombo(void);
	void ReleaseCombo(void);

	CScore *GetScore(void);
	CTimer *GetTimer(void);
	CCombo *GetCombo(void);
	CScene2D *GetNumStage(void);
	CScene2D *GetPurpose(void);

private:   // このクラスだけがアクセス可能
	STATE              m_State;             // 状態
	CScore             *m_pScore;           // スコアクラスへのポインタ
	CTimer             *m_pTimer;           // タイマークラスへのポインタ
	CCombo             *m_pCombo;           // コンボクラスへのポインタ
	CGaugeLife2D       *m_pGaugeLife;       // 体力ゲージクラスへのポインタ
	CGaugeOrbitSlash2D *m_pGaugeOrbitSlash; // 軌跡ゲージクラスへのポインタ
	CGaugeSpecial2D    *m_pGaugeSpecial;    // 必殺ゲージクラスへのポインタ
	CScene2D           *m_pPaper;           // 紙背景ポリゴンクラスへのポインタ
	CScene2D           *m_pNumStage;        // 現在のステージ数ポリゴンクラスへのポインタ
	CScene2D           *m_pPurpose;         // 目的のポリゴンクラスへのポインタ
	CTextureManager    *m_pTextureManager;  // テクスチャ管轄クラスへのポインタ
	bool               m_bDisp;             // UIを描画するかどうか
};

#endif