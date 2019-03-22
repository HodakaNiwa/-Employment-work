//*****************************************************************************
//
//     タイトルの処理[title.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2D;
class CScene2DPress;
class CMesh2D;
class CTextureManager;
class CEffectManager;
class CMap;

//*****************************************************************************
//    タイトルクラスの定義
//*****************************************************************************
class CTitle : public CScene
{
public:     // 誰でもアクセス可能
	//--------------------
	//  状態
	//--------------------
	typedef enum
	{
		STATE_NONE = 0,        // 何も出ていない状態
		STATE_ALPHACUT_BLACK,  // 黒いポリゴンの透明度を下げる状態
		STATE_STOP_LOGO,       // ロゴポリゴンが出るまで待機する状態
		STATE_APPEAR_LOGO,     // タイトルロゴを出す状態
		STATE_FADE_OUT,        // フェードアウトを開始した状態
		STATE_FADE_STOP,       // フェードをストップさせるを開始した状態
		STATE_FADE_IN,         // フェードインを開始した状態
		STATE_OK_CHANGEMODE,   // 画面遷移ができる状態状態
		STATE_MAX
	}STATE;

	//--------------------
	//  タイトルロゴの数
	//--------------------
	typedef enum
	{
		LOGOTYPE_0 = 0,
		LOGOTYPE_1,
		LOGOTYPE_MAX
	}LOGOTYPE;

	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // タイトルロゴ１個目用
		TEX_NUMBER_1,      // タイトルロゴ２個目用
		TEX_NUMBER_2,      // PressEnter用
		TEX_NUMBER_3,      // パーティクル用
		TEX_NUMBER_4,      // 軌跡エフェクト用
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTitle(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TITLE);
	~CTitle();

	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	STATE GetState(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void StartCreate(void);
	void AlphaCutBlack(void);
	void LogoCutScale(void);
	void FadeOut(void);
	void FadeStop(void);
	void FadeIn(void);
	void CheckModeChange(void);
	void SwitchCamera(void);
	void CreateOrbit(void);

	STATE           m_State;                        // 状態
	CMap            *m_pMap;                        // マップクラスへのポインタ
	int             m_nCounter;                     // カメラの動きを管理するカウンター
	int             m_nModeCounter;                 // 遷移を管理するカウンター
	int             m_nPatternCamera;               // 現在のタイトルカメラのパターン番号
	int             m_nPatternOrbit;                // 現在の軌跡エフェクトパターン番号
	char            m_aMapFileName[256];            // 生成するマップの名前
	CScene2D        *m_pTitleLogo[LOGOTYPE_MAX];    // タイトルロゴ用の2Dポリゴンクラスへのポインタ
	CScene2DPress   *m_pPress;                      // プレスエンター用の2Dポリゴンクラスへのポインタ
	CEffectManager  *m_pEffectManager;              // エフェクト管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;             // テクスチャ管理クラスへのポインタ
	CScene2D        *m_pWhitePoly;                  // フェードインに使用するポリゴン
	float           m_fCutScaleValue;               // ロゴの大きさを減らしていく倍率
	CMesh2D         *m_pBlackMesh;                  // 演出に使用する黒いポリゴン

	// エフェクト用変数
	int             m_nEffectCounter;               // エフェクトを管理するカウンター
	int             m_nEffectSwicthTiming;          // エフェクトのパターンを切り替えるタイミング
	D3DXVECTOR3     m_ParEmitterPos;                // パーティクルを出す座標(徐々に右にずらしていく)
	D3DXVECTOR3     m_ParEmitterDiffPos;            // パーティクルを出す目的の位置との差分
	D3DXCOLOR       m_ParCol;                       // パーティクルの色
};

#endif