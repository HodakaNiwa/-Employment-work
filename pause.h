//*****************************************************************************
//
//     ポーズの処理[pause.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2D;
class CScene2DFlash;

//*****************************************************************************
//    ポーズクラスの定義
//*****************************************************************************
class CPause
{
public:    // 誰でもアクセス可能
	//------------------------
	//  ポーズ項目の種類
	//------------------------
	typedef enum
	{
		SELECT_CONTINUE = 0, // 再開
		SELECT_RETRY,        // やり直し
		SELECT_QUIT,         // 終了
		SELECT_MAX
	}SELECT;

	CPause();
	~CPause();

	static CPause *Create(int nPriority = 3);

	HRESULT Init(int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CScene2DFlash *GetScene2DFlash(int nIdx);
	CScene2D *GetPauseBg(void);
	CScene2D *GetOpeInfo(int nIdx);
	int GetSelect(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CScene2DFlash             *m_apLogoPause[SELECT_MAX];     // ポーズ項目の種類
	CScene2D                  *m_pBlackBg;                    // ポーズ背景用(黒いポリゴン)
	CScene2D                  *m_pPauseBg;                    // ポーズの背景
	CScene2D                  *m_pOperationInfo[2];           // 操作方法表示用のポリゴン
	int                       m_nSelect;                      // 現在選択している項目
	int                       m_nCounter;                     // 描画を管理するカウンター
};

#endif