//*****************************************************************************
//
//     チュートリアルの処理[tutorial.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CTextureManager;
class CMap;
class CUI;
class CPlayer;
class CEffectManager;
class CMeshWall;
class CScene2DFlash;
class CScene2D;

//*****************************************************************************
//    チュートリアルクラスの定義
//*****************************************************************************
class CTutorial : public CScene
{
public:     // 誰でもアクセス可能
	//--------------------
	//  状態
	//--------------------
	typedef enum
	{
		STATE_NONE = 0,    // 何もしていない状態
		STATE_MODECHECK,   // 画面遷移するかどうかをチェックする
		STATE_MODENONE,    // 画面遷移しない状態
		STATE_MODECHANGE,  // 画面遷移する状態
		STATE_MAX
	}STATE;

	//--------------------
	//  画面遷移チェック
	//--------------------
	typedef enum
	{
		SELECT_CHANGE = 0,
		SELECT_NONE,
		SELECT_MAX
	}SELECT;

	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter用
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTutorial(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TUTORIAL);
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	void SetGoalCol(const bool bGoalCol);

	STATE GetState(void);
	CPlayer *GetPlayer(void);
	CMeshWall *GetGoalWall(void);
	bool GetGoalCol(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void CreateModeLogo(void);
	void ModeChangeCheck(void);
	void SetBoxCollider(void);

	STATE           m_State;                     // 状態
	CPlayer         *m_pPlayer;                  // プレイヤークラスへのポインタ(保存用)
	CUI             *m_pUi;                      // UIクラスへのポインタ
	CMap            *m_pMap;                     // マップクラスへのポインタ
	int             m_nCounter;                  // 遷移を管理するカウンター
	CEffectManager  *m_pEffectManager;           // エフェクト管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;          // テクスチャ管理クラスへのポインタ
	CMeshWall       *m_pGoalWall;                // ゴール用の壁(プレイヤーがここに当たったら開始するかどうか選ばせる)
	bool            m_bGoalCol;                  // ゴールにプレイヤーが触れているかどうか
	CScene2D        *m_pCheck;                   // 操作方法表示切替表示用ポリゴン
	CScene2D        *m_pSkip;                    // スキップ説明表示用ポリゴン
	CScene2D        *m_pOperationInfo[2];        // 操作方法用のポリゴン
	bool            m_bOperationInfo;            // 操作方法を表示するかどうか

	// 画面遷移チェック用
	CScene2DFlash   *m_pSelectMode[SELECT_MAX];  // 画面遷移するかどうかの項目表示用ポリゴン
	int             m_nSelectMode;               // 画面遷移するかどうかを選べる番号
	CScene2D        *m_pLogo[3];                 // 説明用の文字ポリゴン
	CScene2D        *m_pBlackPoly;               // チェックするときに表示する黒いポリゴン
	float           m_fModeChangeCol;            // ロゴポリゴンの色の変化量
};

#endif