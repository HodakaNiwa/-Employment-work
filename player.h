//*****************************************************************************
//
//     プレイヤーの処理[player.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY       (4)  // プレイヤーの処理優先順位
#define PLAYER_AFTERIMAGE_NUM (4)  // 残像を出せる数
#define PLAYER_AVATAER_NUM    (4)  // 分身を出せる数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class CMotion;
class CModelManager;
class COrbitEffect;
class CEffectManager;
class CGaugeLife2D;
class CGaugeOrbitSlash2D;
class CGaugeSpecial2D;
class CSceneBillboard;
class CEnemy;

//*****************************************************************************
//    プレイヤー管轄クラスの定義
//*****************************************************************************
class CPlayerManager : CScene
{
public:    // 誰でもアクセス可能
	CPlayerManager(int nPriority = 1, OBJTYPE objType = OBJTYPE_PLAYERMANAGER);
	~CPlayerManager();

	static CPlayerManager *Create(char *pFileName, int nLife, float fOrbitSlash, float fMaxSpecial, int nPriority = 1);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFileName(char *pFileName);
	void SetLife(const int nLife);
	void SetOrbitSlash(const float fOrbitSlash);
	void SetMaxSpecial(const float fMaxSpecial);
	void SetModelManager(CModelManager *pModelManager);

	char *GetFileName(void);
	int GetLife(void);
	float GetOrbitSlash(void);
	float GetMaxSpecial(void);
	CModelManager *GetModelManager(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char            m_aFileName[256];    // プレイヤーデータのスクリプトファイル名
	int             m_nLife;             // プレイヤーの体力
	float           m_fOrbitSlash;       // プレイヤーの軌跡ゲージ量
	float           m_fMaxSpecial;       // プレイヤーの必殺技ゲージの最大値
	CModelManager   *m_pModelManager;    // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;  // テクスチャ管轄クラスへのポインタ
};

//*****************************************************************************
//    プレイヤークラスの定義
//*****************************************************************************
class CPlayer : public CCharacter
{
public:    // 誰でもアクセス可能
	//--------------------
	//  状態の種類
	//--------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常状態
		STATE_MOVE,         // 移動状態
		STATE_ATTACK_0,     // 攻撃状態(1モーション目)
		STATE_ATTACK_1,     // 攻撃状態(2モーション目)
		STATE_ATTACK_2,     // 攻撃状態(3モーション目)
		STATE_ATTACK_3,     // 攻撃状態(4モーション目)
		STATE_JUMP,         // ジャンプ状態
		STATE_LANDING,      // 着地状態
		STATE_DAMAGE,       // ダメージ状態
		STATE_ORACLEATTACK, // オラクルアタック状態
		STATE_MAX
	}STATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, float fOrbitSlash, float fMaxSpecial, int nPriority = 3);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, float fOrbitSlash, float fMaxSpecial);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Damage(int nDamage);
	void LockLost(void);

	void SetState(const STATE State);
	void SetMove(const D3DXVECTOR3 move);
	void SetLife(const int nLife);
	void SetAccel(const float fAccel);
	void SetInertia(const float fInertia);
	void SetJumpPower(const float fJumpPower);
	void SetRivisionRot(const float fRivisionRot);
	void SetOrbitSlash(const float fOrbitSlash);
	void SetSpecial(const float fSpecial);
	void SetJump(const bool bJump);
	void SetAction(const bool bAction);
	void SetLockOn(const bool bLockOn);
	void SetDamage(const bool bDamage);

	CEnemy *GetLockOnEnemy(void);
	STATE GetState(void);
	D3DXVECTOR3 GetMove(void);
	int GetLife(void);
	float GetAccel(void);
	float GetInertia(void);
	float GetJumpPower(void);
	float GetRivisionRot(void);
	float GetOrbitSlash(void);
	float GetSpecial(void);
	bool GetJump(void);
	bool GetAction(void);
	bool GetLockOn(void);
	bool GetDamage(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void InputMove(void);
	void Movement(void);
	void Collision(void);
	void Action(void);
	void Statement(void);
	void Landing(void);
	bool AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage);
	void CreateEffect(void);
	void CalcOffsetRot(void);
	void LockOn(void);
	void DrawAfterImage(void);
	void DrawAvater(void);
	void StackAfterImage(void);

	static D3DXVECTOR3 m_AvaterOffset[PLAYER_AVATAER_NUM];           // 分身のオフセット位置

	COrbitEffect       *m_pOrbitEffect;                              // 軌跡エフェクトクラスへのポインタ
	CEffectManager     *m_pEffectManager;                            // エフェクト管轄クラスへのポインタ
	CGaugeLife2D       *m_pLifeGauge;                                // 体力ゲージへのポインタ
	CGaugeOrbitSlash2D *m_pOrbitSlashGauge;                          // 軌跡ゲージへのポインタ
	CGaugeSpecial2D    *m_pSpecialGauge;                             // 必殺ゲージへのポインタ
	CSceneBillboard    *m_pLockOnPoly;                               // ロックオン表示用ポリゴンクラスへのポインタ
	CEnemy             *m_pLockOnEnemy;                              // ロックオン対象の敵クラスへのポインタ
	STATE              m_State;                                      // 状態
	D3DXVECTOR3        m_Move;                                       // 移動量
	int                m_nLife;                                      // 体力
	int                m_nMaxLife;                                   // 体力の最大値
	int                m_nLifeCounter;                               // 体力の回復を管理するカウンター
	int                m_nEffectCounter;                             // エフェクトを出す間隔を管理するカウンター
	int                m_nOrbitCounter;                              // 軌跡ゲージを回復する間隔を管理するカウンター
	int                m_nAvaterCounter;                             // 分身が出てからの秒数
	int                m_nAvaterNum;                                 // 現在の分身の数
	int                m_nDamageCounter;                             // ダメージを受けてからの時間を管理するカウンター
	float              m_fAccel;                                     // 加速度
	float              m_fInertia;                                   // 慣性
	float              m_fJumpPower;                                 // ジャンプ量
	float              m_fRivisionRot;                               // 向きを補正する倍率
	float              m_fOrbitSlash;                                // 軌跡斬に使用する軌跡ゲージ量
	float              m_fOrbitSlashDef;                             // 軌跡斬に使用する軌跡ゲージ量
	float              m_fSpecial;                                   // 現在の必殺技ゲージ
	float              m_fMaxSpecial;                                // 必殺技ゲージの最大値
	bool               m_bJump;                                      // ジャンプしているかどうか
	bool               m_bOrbitSlash;                                // 軌跡斬をするかどうか
	bool               m_bAction;                                    // アクションをしているかどうか
	bool               m_bOrbit;                                     // 軌跡を出しているかどうか
	bool               m_bBooking;                                   // アクションの予約が入っているかどうか
	bool               m_bInRiver;                                   // 川に入っているかどうか
	bool               m_bInGrass;                                   // 草むらの中に入っているかどうか
	bool               m_bLockOn;                                    // ロックオンしているかどうか
	bool               m_bOracleAttack;                              // オラクルアタックをしているかどうか
	bool               m_bDamage;                                    // ダメージを受けたかどうか


	// 残像記憶用変数
	D3DXVECTOR3        m_AfterPos[PLAYER_AFTERIMAGE_NUM];            // 残像記憶用の座標
	D3DXVECTOR3        m_AfterRot[PLAYER_AFTERIMAGE_NUM];            // 残像記憶用の向き
	D3DXVECTOR3        *m_AfterModelPos[PLAYER_AFTERIMAGE_NUM];      // 残像記憶用のモデル座標
	D3DXVECTOR3        *m_AfterModelRot[PLAYER_AFTERIMAGE_NUM];      // 残像記憶用のモデル向き
	int                m_nCurrentStack;                              // 現在の残像記憶番号


	// 分身攻撃用
	COrbitEffect       *m_pOrbitEffectAvater[PLAYER_AVATAER_NUM];     // 分身用の軌跡エフェクトクラスへのポインタ
	D3DXMATRIX         m_AvaterModelMtxWorld[PLAYER_AVATAER_NUM];     // 分身のワールドマトリックス(武器のみ)
};

#endif