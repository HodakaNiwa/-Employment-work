//*****************************************************************************
//
//     カメラの処理[camera.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CPlayer;
class CScene;

//*****************************************************************************
//    カメラクラスの定義
//*****************************************************************************
class CCamera
{
public:    // 誰でもアクセス可能
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosVDest(const D3DXVECTOR3 posVDest);
	void SetPosVDiff(const D3DXVECTOR3 posVDiff);
	void SetPosVOld(const D3DXVECTOR3 posVOld);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetPosRDest(const D3DXVECTOR3 posRDest);
	void SetPosRDiff(const D3DXVECTOR3 posRDiff);
	void SetMoveV(const D3DXVECTOR3 MoveV);
	void SetMoveR(const D3DXVECTOR3 MoveR);
	void SetRot(const D3DXVECTOR3 Rot);
	void SetRotDest(const D3DXVECTOR3 RotDest);
	void SetRotDiff(const D3DXVECTOR3 RotDiff);
	void SetRotOld(const D3DXVECTOR3 RotOld);
	void SetMtxProjection(const D3DXMATRIX MtxProjection);
	void SetMtxView(const D3DXMATRIX MtxView);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosVDest(void);
	D3DXVECTOR3 GetPosVDiff(void);
	D3DXVECTOR3 GetPosVOld(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetPosRDest(void);
	D3DXVECTOR3 GetPosRDiff(void);
	D3DXVECTOR3 GetMoveV(void);
	D3DXVECTOR3 GetMoveR(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void);
	D3DXVECTOR3 GetRotDiff(void);
	D3DXVECTOR3 GetRotOld(void);
	D3DXMATRIX GetMtxProjection(void);
	D3DXMATRIX GetMtxView(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	bool FieldCollision(void);
	bool WallCollision(void);
	void Title(void);
	void Tutorial(void);
	void Game(void);
	void Result(void);
	void Ranking(void);
	void CalcRot(void);
	void MovieCamera(void);

	CPlayer      *m_pPlayer;        // プレイヤークラスへのポインタ
	CScene       *m_pLandScene;     // 現在カメラが乗っているオブジェクトクラスへのポインタ
	CScene       *m_pLandSceneOld;  // 前回カメラが乗っているオブジェクトクラスへのポインタ
	D3DXVECTOR3  m_PosV;            // 現在の視点
	D3DXVECTOR3  m_PosVDest;        // 目的の視点
	D3DXVECTOR3  m_PosVDiff;        // 現在の視点と目的の視点の差分
	D3DXVECTOR3  m_PosVOld;         // 前回の視点
	D3DXVECTOR3  m_PosR;            // 現在の注視点
	D3DXVECTOR3  m_PosRDest;        // 目的の注視点
	D3DXVECTOR3  m_PosRDiff;        // 現在の注視点と目的の注視点の差分
	D3DXVECTOR3  m_MoveV;           // 視点の移動量
	D3DXVECTOR3  m_MoveR;           // 注視点の移動量
	D3DXVECTOR3  m_Rot;             // 現在の向き
	D3DXVECTOR3  m_RotDest;         // 目的の向き
	D3DXVECTOR3  m_RotDiff;         // 目的の向きと現在の向きの差分
	D3DXVECTOR3  m_RotOld;          // 前回の向き
	D3DXVECTOR3  m_VecU;            // 上方向ベクトル
	D3DXMATRIX   m_MtxProjection;   // プロジェクションマトリックス
	D3DXMATRIX   m_MtxView;         // ビューマトリックス
	D3DVIEWPORT9 m_ViewPort;        // ビューポート(描画領域)
	float        m_fLength;         // 距離
	bool         m_bMoving;         // カメラを動かせるかどうか(動画用)
};

//*****************************************************************************
//    ミニマップカメラクラスの定義
//*****************************************************************************
class CMapCamera
{
public:    // 誰でもアクセス可能
	CMapCamera();
	~CMapCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CPlayer      *m_pPlayer;        // プレイヤークラスへのポインタ
	D3DXVECTOR3  m_PosV;            // 現在の視点
	D3DXVECTOR3  m_PosR;            // 現在の注視点
	D3DXVECTOR3  m_Rot;             // 現在の向き
	D3DXVECTOR3  m_VecU;            // 上方向ベクトル
	D3DXMATRIX   m_MtxProjection;   // プロジェクションマトリックス
	D3DXMATRIX   m_MtxView;         // ビューマトリックス
	D3DVIEWPORT9 m_ViewPort;        // ビューポート(描画領域)
	float        m_fLength;         // 距離
};

#endif