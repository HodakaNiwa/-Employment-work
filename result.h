//*****************************************************************************
//
//     リザルトの処理[result.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMap;
class CScene2D;
class CScene2DPress;
class CTextureManager;
class CNumber;

//*****************************************************************************
//    リザルトクラスの定義
//*****************************************************************************
class CResult : public CScene
{
public:     // 誰でもアクセス可能
	//--------------------
	//  スコアの種類
	//--------------------
	typedef enum
	{
		TYPE_SCORE_GAMESCORE = 0,       // ゲームのスコア
		TYPE_SCORE_STAGECLEAR_BONUS,    // ステージクリアボーナス
		TYPE_SCORE_COMBOMAX_BONUS,      // 最大コンボ数ボーナス
		TYPE_SCORE_LASTBOSS_BONUS,      // ラスボス撃破ボーナス
		TYPE_SCORE_HIDDENITEM_BONUS,    // 隠しアイテム取得ボーナス
		TYPE_SCORE_ALLSCORE,            // 合計スコア
		TYPE_SCORE_MAX
	}TYPE_SCORE;

	//--------------------
	//  状態
	//--------------------
	typedef enum
	{
		STATE_NONE = 0,                   // 何もない状態
		STATE_APPEAR_GAME_SCORE,          // ゲームのスコアまでを表示している状態
		STATE_APPEAR_STAGECLEAR_BONUS,    // ステージクリアボーナスまでを表示している状態
		STATE_APPEAR_COMBOMAX_BONUS,      // 最大コンボボーナスまでを表示している状態
		STATE_APPEAR_LASTBOSS_BONUS,      // ラスボス撃破ボーナスまでを表示している状態
		STATE_APPEAR_HIDDENITEM_BONUS,    // 隠しアイテムボーナスまでを表示している状態
		STATE_APPEAR_ALLSCORE,            // 合計スコアまでを表示している状態(遷移できる状態でもある)
		STATE_MAX
	}STATE;

	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // ゲームのスコア用
		TEX_NUMBER_1,      // ステージクリアボーナス用
		TEX_NUMBER_2,      // 最大コンボボーナス用
		TEX_NUMBER_3,      // ラスボス撃破ボーナス用
		TEX_NUMBER_4,      // 隠しアイテムボーナス用
		TEX_NUMBER_5,      // 合計スコア用
		TEX_NUMBER_6,      // 背景用
		TEX_NUMBER_7,      // PressEnter用
		TEX_NUMBER_8,      // 数字用
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CResult(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_RESULT);
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetGameScore(int nGameScore);
	static void SetStageClearBonus(int nStageBonus);
	static void SetComboMaxBonus(int nComboMaxBonus);
	static void SetLastBossBonus(int nLastBossBonus);
	static void SetHiddenItemBonus(int nHiddenItemBonus);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void ModeChangeCheck(void);
	void CreateNumber(void);

	static int      m_nGameScore;           // ゲームのスコア
	static int      m_nStageClearBonus;     // ステージクリアボーナス
	static int      m_nComboMaxBonus;       // 最大コンボボーナス
	static int      m_nLastBossBonus;       // ラスボス撃破ボーナス
	static int      m_nHiddenItemBonus;     // ステージクリアボーナス

	STATE           m_State;                         // 状態
	int             m_nAllScore;                     // 合計スコア
	int             m_nCounter;                      // 状態を管理するカウンター
	CNumber         **m_apNumber[TYPE_SCORE_MAX];    // それぞれのスコア表示用のポリゴン
	int             m_aDigit[TYPE_SCORE_MAX];        // それぞれのスコアの桁数
	CScene2D        *m_apScoreLogo[TYPE_SCORE_MAX];  // それぞれのスコアのロゴ表示用ポリゴン
	CScene2DPress   *m_pPress;                       // プレスエンター用の2Dポリゴンクラスへのポインタ
	CTextureManager *m_pTextureManager;              // テクスチャ管理クラスへのポインタ
	CScene2D        *m_pResultBg;                    // 背景用のポリゴン
};

#endif