//*****************************************************************************
//
//     ゲージの処理[gauge.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2D.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    ゲージクラスの定義
//*****************************************************************************
class CGauge : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	CGauge(int nPriority = 3, OBJTYPE objType = OBJTYPE_GAUGE);
	~CGauge();

	static CGauge *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetGaugeRate(const float fGaugeRate);
	void SetLeftWidth(const float fLeftWidth);

	float GetGaugeRate(void);
	float GetLeftWidth(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fGaugeRate;    // ゲージの割合
	float m_fLeftWidth;    // ゲージの左側の長さ
};

//*****************************************************************************
//    体力ゲージクラスの定義
//*****************************************************************************
class CGaugeLife : public CGauge
{
public:    // 誰でもアクセス可能
	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,
		STATE_CUT,
		STATE_ADD,
		STATE_MAX
	}STATE;

	CGaugeLife(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE);
	~CGaugeLife();

	static CGaugeLife *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const int nCutValue);
	void AddGauge(const int nAddValue);

	void SetGaugePos(const D3DXVECTOR3 pos);
	void SetState(const STATE State);
	void SetLife(const int nLife);

	STATE GetState(void);
	int GetLife(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int             m_nCounter;      // ゲージの減らしを管理するカウンター
	STATE           m_State;         // 状態
	int             m_nLife;         // 体力
	CGauge         *m_pWhiteBill;    // 体力ゲージのフレーム
	CGauge         *m_pRedBill;      // 体力ゲージが減った時に表示する赤いゲージ
};


//*****************************************************************************
//    ゲージクラス(2D)の定義
//*****************************************************************************
class CGauge2D : public CScene2D
{
public:    // 誰でもアクセス可能
	CGauge2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE);
	~CGauge2D();

	static CGauge2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetFrameTexture(LPDIRECT3DTEXTURE9 pFrameTexture);
	void SetGaugeRate(const float fGaugeRate);
	void SetLeftWidth(const float fLeftWidth);

	LPDIRECT3DTEXTURE9 GetFrameTexture(void);
	float GetGaugeRate(void);
	float GetLeftWidth(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9 m_pFrameTexture;   // ゲージフレーム専用のテクスチャ
	float              m_fGaugeRate;      // ゲージの割合
	float              m_fLeftWidth;      // ゲージの左側の長さ
};

//*****************************************************************************
//    体力ゲージクラス(2D)の定義
//*****************************************************************************
class CGaugeLife2D : public CGauge2D
{
public:    // 誰でもアクセス可能
	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,
		STATE_CUT,
		STATE_ADD,
		STATE_MAX
	}STATE;

	CGaugeLife2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE2D);
	~CGaugeLife2D();

	static CGaugeLife2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, LPDIRECT3DTEXTURE9 pFrameTexture = NULL, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const int nCutValue);
	void AddGauge(const int nAddValue);
	void ChangeAlpha(const float fAlpha);

	void SetState(const STATE State);
	void SetLife(const int nLife);

	STATE GetState(void);
	int GetLife(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int             m_nCounter;      // ゲージの減らしを管理するカウンター
	STATE           m_State;         // 状態
	int             m_nLife;         // 体力
	CGauge2D        *m_pBlack2D;     // 体力ゲージの黒い部分用
	CGauge2D        *m_pFrame2D;     // 体力ゲージのフレーム
	CGauge2D        *m_pRed2D;       // 体力ゲージが減った時に表示する赤いゲージ
};

//*****************************************************************************
//    軌跡ゲージクラス(2D)の定義
//*****************************************************************************
class CGaugeOrbitSlash2D : public CGauge2D
{
public:    // 誰でもアクセス可能
	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,   // 何もしない状態
		STATE_CUT,        // 減っている状態
		STATE_ADD,        // 増えている状態
		STATE_MAX
	}STATE;

	CGaugeOrbitSlash2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_SLASHGAUGE2D);
	~CGaugeOrbitSlash2D();

	static CGaugeOrbitSlash2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fOrbitSlash, LPDIRECT3DTEXTURE9 pFrameTexture = NULL, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const float fCutValue);
	void AddGauge(const float fAddValue);
	void DeleteGauge(void);
	void ChangeAlpha(const float fAlpha);

	void SetState(const STATE State);
	void SetOrbitSlash(const float fOrbitSlash);

	STATE GetState(void);
	float GetOrbitSlash(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int             m_nCounter;      // ゲージの減らしを管理するカウンター
	STATE           m_State;         // 状態
	float           m_fOrbitSlash;   // 軌跡ゲージ量
	CGauge2D        *m_pBlack2D;     // 軌跡ゲージの黒い部分用
	CGauge2D        *m_pFrame2D;     // 軌跡ゲージのフレーム
};

//*****************************************************************************
//    必殺技ゲージクラス(2D)の定義
//*****************************************************************************
class CGaugeSpecial2D : public CGauge2D
{
public:    // 誰でもアクセス可能
	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,   // 何もしない状態
		STATE_CUT,        // 減っている状態
		STATE_ADD,        // 増えている状態
		STATE_MAX
	}STATE;

	CGaugeSpecial2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_SPECIALGAUGE2D);
	~CGaugeSpecial2D();

	static CGaugeSpecial2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fMaxSpecial, float fSpecial, LPDIRECT3DTEXTURE9 pFrameTexture = NULL, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const float fCutValue);
	void AddGauge(const float fAddValue);
	void MaxGauge(void);
	void DeleteGauge(void);
	void ChangeAlpha(const float fAlpha);

	void SetState(const STATE State);
	void SetSpecial(const float fSpecial);
	void SetMaxSpecial(const float fMaxSpecial);

	STATE GetState(void);
	float GetSpecial(void);
	float GetMaxSpecial(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int             m_nCounter;      // ゲージの減らしを管理するカウンター
	STATE           m_State;         // 状態
	float           m_fMaxSpecial;   // 必殺ゲージの最大値
	float           m_fSpecial;      // 必殺技ゲージ量
	CGauge2D        *m_pBlack2D;     // 必殺技ゲージの黒い部分用
	CGauge2D        *m_pFrame2D;     // 必殺技ゲージのフレーム
};


#endif