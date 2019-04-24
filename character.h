//*****************************************************************************
//
//     キャラクターの処理[character.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;
class CMotionManager;
class CModelManager;
class CTextureManager;
class CShadow;

//*****************************************************************************
//    キャラクタークラスの定義
//*****************************************************************************
class CCharacter : public CScene
{
public:    // 誰でもアクセス可能
	CCharacter(int nPriority = 3,OBJTYPE objType = OBJTYPE_CHARACTER);
	~CCharacter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);
	void SetDefaultPos(void);
	void SetDefaultRot(void);
	void SetModelAlpha(float fAlpha);

	void SetNumParts(const int nNumParts);
	void SetModel(CModel **pModel);
	void SetModel(CModel *pModel, const int nIdx);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);
	void SetDestRot(const D3DXVECTOR3 Destrot);
	void SetDiffRot(const D3DXVECTOR3 Diffrot);
	void SetGravity(const float fGravity);
	void SetColHeight(const float fColHeight);
	void SetColRange(const float fColRange);
	void SetLand(const bool bLand);
	void SetLandScene(CScene *pLandScene);
	void SetLandSceneOld(CScene *pLandSceneOld);
	void SetLandOffsetPos(const D3DXVECTOR3 LandOffsetPos);
	void SetLandOffsetAdd(const D3DXVECTOR3 LandOffsetAdd);
	void SetLandOffsetRot(const D3DXVECTOR3 LandOffsetRot);
	void SetLandOffsetRotStart(const D3DXVECTOR3 LandOffsetRotStart);
	void SetShadow(CShadow *pShadow);

	int GetNumParts(void);
	CModel **GetModel(void);
	CModel *GetModel(const int nIdx);
	CMotionManager *GetMotionManager(void);
	CTextureManager *GetTextureManager(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetDestRot(void);
	D3DXVECTOR3 GetDiffRot(void);
	float GetGravity(void);
	float GetColHeight(void);
	float GetColRange(void);
	bool GetLand(void);
	CScene *GetLandScene(void);
	CScene *GetLandSceneOld(void);
	D3DXVECTOR3 GetLandOffsetPos(void);
	D3DXVECTOR3 GetLandOffsetAdd(void);
	D3DXVECTOR3 GetLandOffsetRot(void);
	D3DXVECTOR3 GetLandOffsetRotStart(void);
	CShadow *GetShadow(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int             m_nNumParts;             // 使用するパーツ数
	CModel          **m_apModel;             // モデルクラスへのポインタ
	CMotionManager  *m_pMotionManager;       // モーションクラスへのポインタ
	CTextureManager *m_pTextureManager;      // テクスチャ管轄クラスへのポインタ
	D3DXMATRIX      m_MtxWorld;              // ワールドマトリックス
	D3DXVECTOR3     m_Scale;                 // スケールの倍率
	D3DXVECTOR3     m_Pos;                   // 現在の座標
	D3DXVECTOR3     m_PosOld;                // 前回の座標
	D3DXVECTOR3     m_Rot;                   // 現在の向き
	D3DXVECTOR3     m_DestRot;               // 目的の向き
	D3DXVECTOR3     m_DiffRot;               // 目的の向きと現在の向きの差分
	float           m_fGravity;              // 重力
	float           m_fColHeight;            // 当たり判定をとる高さ
	float           m_fColRange;             // 当たり判定を取る範囲
	CScene          *m_pLandScene;           // 乗っているオブジェクトへのポインタ
	CScene          *m_pLandSceneOld;        // 前回乗っていたオブジェクトへのポインタ
	bool            m_bLand;                 // 前回オブジェクトに乗っていたかどうか
	D3DXVECTOR3     m_LandOffsetPos;         // オブジェクトからのオフセット距離
	D3DXVECTOR3     m_LandOffsetAdd;         // オフセット距離に加える値
	D3DXVECTOR3     m_LandOffsetRot;         // オブジェクトからのオフセット向き
	D3DXVECTOR3     m_LandOffsetRotStart;    // 乗ってからの向き
	CShadow         *m_pShadow;              // 影クラスへのポインタ
};

#endif