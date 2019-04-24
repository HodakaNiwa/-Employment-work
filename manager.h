//*****************************************************************************
//
//     マネージャーの処理[manager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sound.h"

//*****************************************************************************
//    前方宣言(実体がない宣言)
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CXInput;
class CSound;
class CLight;
class CCamera;
class CMapCamera;
class CDebugProc;

class CFade;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;

//*****************************************************************************
//    マネージャクラスの定義
//*****************************************************************************
class CManager
{
public:    // 誰でもアクセス可能
	//------------------------
	//  モードの種類
	//------------------------
	typedef enum
	{
		MODE_TITLE = 0,  // タイトル
		MODE_TUTORIAL,   // チュートリアル
		MODE_GAME,       // ゲーム
		MODE_RESULT,     // リザルト
		MODE_RANKING,    // ランキング
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetMode(MODE mode);
	static MODE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CLight *GetLight(void);
	static CCamera *GetCamera(void);
	static CMapCamera *GetMapCamera(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CXInput *GetXInput(void);
	static CSound *GetSound(void);

	static CFade *GetFade(void);
	static CTitle *GetTitle(void);
	static CTutorial *GetTutorial(void);
	static CGame *GetGame(void);
	static CResult *GetResult(void);
	static CRanking *GetRanking(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static void PlayBgm(CSound::SOUND_LABEL soundLabel);
	static void StopBgm(CSound::SOUND_LABEL soundLabel);

	static CRenderer         *m_pRenderer;  // レンダリングクラス型のポインタ
	static CLight            *m_pLight;     // ライトクラス型のポインタ
	static CCamera           *m_pCamera;    // カメラクラス型のポインタ
	static CMapCamera        *m_pMapCamera; // マップカメラクラス型のポインタ
	static CInputKeyboard    *m_pKeyboard;  // キーボードクラス型のポインタ
	static CInputMouse       *m_pMouse;     // マウスクラス型のポインタ
	static CInputJoyStick    *m_pJoyStick;  // ジョイスティッククラス型のポインタ
	static CXInput           *m_pXInput;    // XInputクラス型のポインタ
	static CSound            *m_pSound;     // サウンドクラス型のポインタ
	static CDebugProc        *m_pDebugProc; // デバッグ情報クラス型のポインタ

	static CFade             *m_pFade;      // フェードクラス型のポインタ
	static CTitle            *m_pTitle;     // タイトルクラス型のポインタ
	static CTutorial         *m_pTutorial;  // チュートリアルクラス型のポインタ
	static CGame             *m_pGame;      // ゲームクラス型のポインタ
	static CResult           *m_pResult;    // リザルトクラス型のポインタ
	static CRanking          *m_pRanking;   // ランキングクラス型のポインタ
	static MODE              m_Mode;        // 現在のモード
};

#if _DEBUG
//*****************************************************************************
//    プロトタイプ宣言
//*****************************************************************************
bool GetUpdate(void);

#endif

#endif