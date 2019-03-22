//*****************************************************************************
//
//     フェードの処理[fade.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "manager.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
//    フェードクラスの定義
//*****************************************************************************
class CFade
{
public:     // 誰でもアクセス可能
	//------------------------
	//  フェードの種類
	//------------------------
	typedef enum
	{
		TYPE_MODECHANGE = 0,  // 画面遷移に使用する
		TYPE_SWITCHMAP,       // マップ切り替えに使用する
		TYPE_BOSSAPPEAR,      // ボス出現に使用する
		TYPE_MAX
	}TYPE;

	//------------------------
	//  フェードの状態
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // 何もしていない状態
		STATE_IN,        // フェードイン状態
		STATE_STOP,      // フェードしたまま止める状態
		STATE_OUT,       // フェードアウト状態
		STATE_MAX
	}STATE;

	CFade();
	~CFade();

	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CManager::MODE modeNext = CManager::MODE_TITLE);
	void SetFade(TYPE type = TYPE_SWITCHMAP);

	STATE GetState(void);
	CScene2D *GetScene2D(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void FadeIn(void);
	void FadeStop(void);
	void FadeOut(void);

	static LPDIRECT3DTEXTURE9 m_pTexture;        // 文字テクスチャへのポインタ

	CScene2D           *m_pScene2D;       // フェードに使用するポリゴン
	CScene2D           *m_pLogo2D;        // 文字ポリゴン
	CManager::MODE     m_NextMode;        // 次に遷移するモード
	STATE              m_State;           // 状態
	TYPE               m_Type;            // 種類
	int                m_nCounter;        // フェードをどれだけ止めるか
};

#endif