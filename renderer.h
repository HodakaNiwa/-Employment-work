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

	LPDIRECT3DDEVICE9 GetDevice(void);
	void ReleasePause(void);

	void SetPause(bool bPause);
	bool GetPause(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void DefaultRender(void);
	void TextureRender(void);

	CPause                  *m_pPause;              // ポーズクラス型のポインタ
	bool                    m_bPause;               // ポーズ中かどうか
	LPDIRECT3D9				m_pD3D;			        // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	        // Deviceオブジェクト(描画に必要)
	LPDIRECT3DSURFACE9      m_pZBufferSurface;      // Zバッファへのポインタ(テクスチャにレンダリングする際にZバッファを切り替えるため)
	LPDIRECT3DTEXTURE9      m_pTexture;             // テクスチャへのポインタ(このテクスチャにオブジェクトを描画する)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;             // 頂点バッファへのポインタ(オブジェクトを描画したテクスチャを使用する)
	TARGET                  m_Target;               // オブジェクトを描画するレンダリングターゲット
	float                   m_fRot;                 // ポリゴンの向き
	float                   m_fAngle;               // ポリゴンを出す角度
	float                   m_fWidth;               // ポリゴンの幅
	float                   m_fHeight;              // ポリゴンの高さ
	float                   m_fLength;              // ポリゴンの長さ
};

#endif