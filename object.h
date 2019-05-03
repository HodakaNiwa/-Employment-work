//*****************************************************************************
//
//     配置物の処理[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CGoalCylinder;

//*****************************************************************************
//    オブジェクトクラスの定義
//*****************************************************************************
class CObject : public CSceneX
{
public:    // 誰でもアクセス可能
	//------------------------
	//  コリジョンの種類
	//------------------------
	typedef enum
	{
		COL_TYPE_SQUARE = 0,  // 四角で判定
		COL_TYPE_CIRCLE,      // 円形で判定
		COL_TYPE_NONE,        // 判定しない
		COL_TYPE_MAX
	}COL_TYPE;

	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);
	void SetTransParent(float fAlpha);

	void SetCollision(const COL_TYPE Collision);
	void SetRollSpeed(const D3DXVECTOR3 RollSpeed);
	void SetPosOld(const D3DXVECTOR3 PosOld);
	void SetModelIdx(const int ModelIdx) { m_ModelIdx = ModelIdx; };
	void SetRotStd(const D3DXVECTOR3 RotStd);
	void SetColRange(const float fColRange);
	void SetHitAttack(const bool bHitAttack);
	void SetHitCounter(const int nHitCounter);
	void SetThin(const bool bThin);

	COL_TYPE GetCollision(void);
	int GetModelIdx(void) { return m_ModelIdx; };
	D3DXVECTOR3 GetRollSpeed(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRotStd(void);
	float GetColRange(void);
	bool GetHitAttack(void);
	bool GetThin(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	bool SquareCol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);
	bool CircleCol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

	COL_TYPE    m_Collision;    // コリジョンの種類
	int         m_ModelIdx;     // 使用しているモデルの番号
	D3DXVECTOR3 m_PosOld;       // 前回の座標
	D3DXVECTOR3 m_RollSpeed;    // 向きを回転させるスピード
	D3DXVECTOR3 m_PosStd;       // モデルの座標(生成時)
	D3DXVECTOR3 m_RotStd;       // モデルの向き(生成時)
	float       m_fColRange;    // 当たり判定の種類(円形の場合に使用)
	bool        m_bHitAttack;   // 攻撃がヒットしたかどうか
	int         m_nHitCounter;  // 攻撃がヒットしてからの時間
	bool        m_bThin;        // 薄く描画するかどうか
};

//*****************************************************************************
//    動くオブジェクトクラスの定義
//*****************************************************************************
class CObjectMove : public CObject
{
public:    // 誰でもアクセス可能
	CObjectMove(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTMOVE);
	~CObjectMove();

	static CObjectMove *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision = COL_TYPE_SQUARE, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(const D3DXVECTOR3 Move);
	void SetMoveCurrent(const D3DXVECTOR3 MoveCurrent);
	void SetCounter(const int nCounter);
	void SetMaxMoveCounter(const int nMaxMoveCounter);
	void SetRivisionMove(const float fRivisionMove);
	void SetPosStd(const D3DXVECTOR3 PosStd);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetMoveCurrent(void);
	int GetCounter(void);
	int GetMaxMoveCounter(void);
	float GetRivisionMove(void);
	D3DXVECTOR3 GetPosStd(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Move;             // 移動量
	D3DXVECTOR3 m_MoveCurrent;      // 現在の移動量
	int         m_nCounter;         // 移動量を管理するカウンター
	int         m_nMaxMoveCounter;  // 移動量が最大値になる時間
	float       m_fRivisionMove;    // 移動の倍率
	D3DXVECTOR3 m_PosStd;           // モデルの座標(生成時)
};

//*****************************************************************************
//    ゴール用オブジェクトクラスの定義
//*****************************************************************************
class CObjectGoal : public CObject
{
public:    // 誰でもアクセス可能
	CObjectGoal(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTGOAL);
	~CObjectGoal();

	static CObjectGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

	void SetDoorOpen(const bool bDoorOpen);
	bool GetDoorOpen(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CGoalCylinder *m_pGoalCylinder;    // ゴール用円筒クラスへのポインタ
	bool          m_bDoorOpen;         // ドアを開く状態かどうか
};

#endif