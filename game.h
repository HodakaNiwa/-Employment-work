//*****************************************************************************
//
//     ゲームの処理[game.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define GAME_MAX_MAPLOAD     (10)    // ゲームのマップを読み込める数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2D;
class CPause;
class CTextureManager;
class CMap;
class CUI;
class CPlayer;
class CEffectManager;

//*****************************************************************************
//    ゲームクラスの定義
//*****************************************************************************
class CGame : public CScene
{
public:     // 誰でもアクセス可能
	//-----------------
	//  状態
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,    // 通常状態
		STATE_MAPCHANGE,     // マップ切り替え状態
		STATE_MAPCLEAR,      // マップクリア状態
		STATE_BOSSAPPEAR,    // ボス出現状態
		STATE_KNOCKDOWNBOSS, // ボスを倒した状態
		STATE_END,           // 終了状態
		STATE_MAX
	}STATE;

	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // ポーズ背景用
		TEX_NUMBER_1,      // ポーズ選択項目用(CONTINUE)
		TEX_NUMBER_2,      // ポーズ選択項目用(RETRY)
		TEX_NUMBER_3,      // ポーズ選択項目用(QUIT)
		TEX_NUMBER_4,      // 数字
		TEX_NUMBER_5,      // コンボ数UI文字
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CGame(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_GAME);
	~CGame();

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MiniMapDraw(void);
	void ClearMapEvent(void);

	void SetPlayer(CPlayer *pPlayer);
	void BindPauseTexture(CPause *pPause);
	void SetMapClear(bool bMapClear);
	void SetMap(void);
	void SetState(STATE state);
	void SetHiddenItem(const int nGetHiddenItem);
	void SetComboMax(const int nComboMax);
	void SetBossPos(const D3DXVECTOR3 BossPos);

	CUI *GetUI(void);
	CEffectManager *GetEffectManager(void);
	CPlayer *GetPlayer(void);
	STATE GetState(void);
	bool GetMapClear(void);
	int GetMapIdx(void);
	int GetNumMap(void);
	int GetHiddenItem(void);
	int GetComboMax(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void BossAppearEvent(void);
	void KonckDownBossEvent(void);
	void SetBridgeCollider(void);

	void SaveGrassTransform(void);
	void LoadGrassTransform(char *pStrCur, char *pLine);

	CPlayer         *m_pPlayer;                               // プレイヤークラスへのポインタ(保存用)
	CUI             *m_pUi;                                   // UIクラスへのポインタ
	CMap            *m_pMap;                                  // マップクラスへのポインタ
	char            m_aMapFileName[GAME_MAX_MAPLOAD][256];    // 読み込むマップデータのスクリプトファイル名
	STATE           m_State;                                  // 状態
	int             m_nStateCounter;                          // 状態を管理するカウンター
	CEffectManager  *m_pEffectManager;                        // エフェクト管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;                       // テクスチャ管理クラスへのポインタ
	int             m_nClearStageNum;                         // クリアしたステージの数
	int             m_nGetHiddenItem;                         // 手に入れた隠しアイテムの数
	int             m_nComboMax;                              // ゲームを通した最大コンボ数
	int             m_nNumMap;                                // 読み込むマップの数
	int             m_nMapIdx;                                // 現在のマップ番号
	bool            m_bMapClear;                              // マップをクリアしたかどうか
	bool            m_bMiniMapDisp;                           // ミニマップを描画するかどうか
	CScene2D        *m_pPlayerPosition;                       // プレイヤーの座標を示す2Dポリゴン

	// ボス出現演出用変数
	CScene2D        *m_pBossAppearLogo[2];                    // ボス出現時に出す文字ポリゴン
	int             m_nBossAppearCounter;                     // ボス出現時の演出を管理するカウンター

	// ボス撃破演出用変数
	D3DXVECTOR3     m_BossPos;                                // ボスの座標(保存用)
	int             m_nKnockDownCounter;                      // ボス撃破時の演出を管理するカウンター
};

#endif