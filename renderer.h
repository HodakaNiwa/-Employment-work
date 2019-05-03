//*****************************************************************************
//
//     レンダリングの処理[renderer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_RENDERER_TEXTURE      (2)       // レンダリングに使用するテクスチャの総数

//*****************************************************************************
//    前方宣言(実体がない宣言)
//*****************************************************************************
class CPause;

//*****************************************************************************
//    レンダリングクラスの定義
//*****************************************************************************
class CRenderer
{
public:     // 誰でもアクセス可能
	typedef enum
	{
		TARGET_DEFAULT = 0,   // デフォルトの設定
		TARGET_TEXTURE,       // テクスチャ
		TARGET_MAX
	}TARGET;

	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFeedBack(int nFrame, float fScale = 0.995f, float fAlpha = 0.85f);

	LPDIRECT3DDEVICE9 GetDevice(void);
	void ReleasePause(void);

	void SetPause(bool bPause);
	bool GetPause(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void ReleaseTexture(void);

	void DefaultRender(void);
	void TextureRender(void);
	void FeedBackRender(void);
	void PolygonRender(void);

	void CreateTexture(void);
	void CreateVertexBuff(void);

	CPause                  *m_pPause;                              // ポーズクラス型のポインタ
	bool                    m_bPause;                               // ポーズ中かどうか
	LPDIRECT3D9				m_pD3D;			                        // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	                        // Deviceオブジェクト(描画に必要)
	LPDIRECT3DTEXTURE9      m_apTextureMT[MAX_RENDERER_TEXTURE];    // テクスチャへのポインタ(このテクスチャにオブジェクトを描画する)
	LPDIRECT3DSURFACE9      m_apRenderMT[MAX_RENDERER_TEXTURE];     // テクスチャに描画するためのインターフェイスへのポインタ
	LPDIRECT3DSURFACE9      m_apBuffMT[MAX_RENDERER_TEXTURE];       // 各種バッファへのポインタ(テクスチャにレンダリングする際にZバッファを切り替えるため)
	D3DVIEWPORT9            m_ViewportMT;                           // テクスチャレンダリング用の描画領域
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                             // 頂点バッファへのポインタ(オブジェクトを描画したテクスチャを使用する)
	TARGET                  m_Target;                               // オブジェクトを描画するレンダリングターゲット
	D3DXVECTOR3             m_Rot;                                  // ポリゴンの向き
	float                   m_fAngle;                               // ポリゴンを出す角度
	float                   m_fWidth;                               // ポリゴンの幅
	float                   m_fHeight;                              // ポリゴンの高さ
	float                   m_fLength;                              // ポリゴンの長さ
	bool                    m_bFeedbackEffect;                      // フィードバックエフェクトを出すかどうか
	int                     m_nFeedbackCounter;                     // フィードバックエフェクトをどれくらい出すか
	float                   m_fFeedbackScale;                       // フィードバックエフェクトをかけるポリゴンの大きさ
	float                   m_fFeedbackAlpha;                       // フィードバックエフェクトをかけるポリゴンの透明度
};

#endif