//*****************************************************************************
//
//     敵の処理[enemy.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define ENEMY_PRIORITY  (4)     // 敵の処理優先順位
#define MAX_ENEMY_TYPE  (10)    // 読み込める敵の種類の数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMotion;
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class COrbitEffect;
class CEffectManager;
class CTextureManager;
class CGaugeLife;
class CPlayer;

//*****************************************************************************
//    エネミークラスの定義
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // 誰でもアクセス可能
	//----------------------
	// 種類
	//----------------------
	typedef enum
	{
		TYPE_FIRE = 0,   // 火の敵
		TYPE_ICE,        // 氷の敵
		TYPE_BOSS,       // ボスの敵
		TYPE_MAX
	}TYPE;

	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_NORMAL = 0,      // 通常状態
		STATE_MOVE,            // 移動状態
		STATE_ACTION,          // アクション状態
		STATE_DAMAGE,          // ダメージ状態
		STATE_DEATH,           // 死亡状態
		STATE_BOSS_ATTACK_0,   // ボスの攻撃モーション(1つ目)
		STATE_BOSS_ATTACK_1,   // ボスの攻撃モーション(2つ目)
		STATE_BOSS_ATTACK_2,   // ボスの攻撃モーション(3つ目)
		STATE_BOSS_ATTACK_3,   // ボスの攻撃モーション(4つ目)
		STATE_BOSS_SKIPOVER,   // ボスの軌跡飛ばしモーション
		STATE_BOSS_STEP,       // ボスのステップモーション
		STATE_BOSS_BACKSTEP,   // ボスのバックステップモーション
		STATE_BOSS_TIRED,      // ボスの疲れモーション
		STATE_MAX
	}STATE;

	//----------------------
	// AIの状態
	//----------------------
	typedef enum
	{
		AI_TYPE_CHEAK = 0,     // 巡回状態
		AI_TYPE_ACCESS,        // 対象のオブジェクトに近づく状態
		AI_TYPE_ATTACK,        // 対象のオブジェクトに攻撃を仕掛ける状態
		AI_TYPE_RETURN_CHEAK,  // 巡回状態に戻る状態
		AI_TYPE_MAX
	}AI_TYPE;

	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife,int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual bool Hit(int nDamage);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRange);

	void SetType(const TYPE type);
	void SetState(const STATE State);
	void SetMove(const D3DXVECTOR3 Move);
	void SetLife(const int nLife);
	void SetAddScore(const int nAddScore);
	void SetAI_Type(const AI_TYPE AI_Type);
	void SetAI_CheackDistance(const float fDistance);
	void SetAI_AttackDistance(const float fDistance);
	void SetAI_AttackTiming(const int nTiming);
	void SetAI_DestPos(const D3DXVECTOR3 AI_DestPos);
	void SetDispCounter(const int nDispCounter);
	void SetGaugeLife(CGaugeLife *pGaugeLife);
	void SetPosStd(const D3DXVECTOR3 posStd);
	void SetRotStd(const D3DXVECTOR3 rotStd);
	void SetDamage(const bool bDamage);

	TYPE GetType(void);
	STATE GetState(void);
	D3DXVECTOR3 GetMove(void);
	int GetLife(void);
	int GetAddScore(void);
	AI_TYPE GetAI_Type(void);
	float GetAI_CheackDistance(void);
	float GetAI_AttackDistance(void);
	int GetAI_AttackTiming(void);
	D3DXVECTOR3 GetAI_DestPos(void);
	int GetDispCounter(void);
	CGaugeLife *GetGaugeLife(void);
	D3DXVECTOR3 GetPosStd(void);
	D3DXVECTOR3 GetRotStd(void);
	bool GetDamage(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Movement(void);
	void Collision(void);
	void Statement(void);
	bool AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage);
	void Death(void);

	void AI(void);
	void AI_CheakDistance(void);
	void AI_ApproachObject(void);
	void AI_AttackObject(void);
	void AI_ReturnPatrol(void);

	STATE       m_State;               // 状態
	AI_TYPE     m_AIType;              // AIの状態
	TYPE        m_Type;                // 種類
	D3DXVECTOR3 m_Move;                // 移動量
	int         m_nLife;               // 体力
	int         m_nAddScore;           // 倒した時に増えるスコア量
	CGaugeLife  *m_pGaugeLife;         // 体力ゲージ
	float       m_fAI_CheackDistance;  // 警戒できる範囲
	float       m_fAI_AttackDistance;  // 攻撃を仕掛ける距離
	int         m_nAI_AttackTiming;    // 攻撃を仕掛けるタイミング(ランダム値)
	D3DXVECTOR3 m_AI_DestPos;          // 目的の位置
	int         m_nAI_Counter;         // AIを管理するカウンター
	bool        m_bAI_Patrol;          // パトロール行動をしているかどうか
	int         m_nDispCounter;        // 描画を管理するカウンター
	D3DXVECTOR3 m_PosStd;              // モデルの座標(生成時)
	D3DXVECTOR3 m_RotStd;              // モデルの向き(生成時)
	bool        m_bDamage;             // ダメージをくらっているかどうか
};

//*****************************************************************************
//    ボスクラスの定義
//*****************************************************************************
class CBoss : public CEnemy
{
public:    // 誰でもアクセス可能
	CBoss(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CBoss();

	//----------------------
	// AIの状態(ボスバージョン)
	//----------------------
	typedef enum
	{
		AI_TYPE_BOSS_CHECK = 0,       // プレイヤーとの距離を測る状態
		AI_TYPE_BOSS_ATTACK,          // 攻撃を仕掛ける状態
		AI_TYPE_BOSS_SKIPOVER_ORBIT,  // 軌跡を飛ばして攻撃する状態
		AI_TYPE_BOSS_TIRED,           // 軌跡を飛ばして疲れてしまった状態
		AI_TYPE_BOSS_STEP,            // プレイヤーにステップで近づく状態
		AI_TYPE_BOSS_BACKSTEP,        // 対象のオブジェクトに攻撃を仕掛ける状態
		AI_TYPE_BOSS_MAX
	}AI_TYPE_BOSS;

	static CBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Hit(int nDamage);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Movement(void);
	void Collision(void);
	void Statement(void);
	bool AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage);
	void Death(void);
	void DeathWarning(void);
	void DeathState(void);

	void AI(void);
	void AI_CheakDistance(void);
	void AI_Attack(void);
	void AI_BackStep(void);
	void AI_Step(void);
	void AI_SkipOverOrbit(void);
	void AI_Tired(void);


	COrbitEffect    *m_pOrbitEffect;          // 軌跡エフェクトクラスへのポインタ
	CEffectManager  *m_pEffectManager;        // エフェクト管轄クラスへのポインタ
	AI_TYPE_BOSS    m_AIType_Boss;            // ボスのAIの状態
	int             m_nBackStep;              // バックステップを踏んだ回数(攻撃行動をしたら0にリセット)
	int             m_nAttack;                // 攻撃を仕掛けた回数(バックステップ状態に移ったらリセット)
	int             m_nStep;                  // ステップを踏んだ回数(軌跡飛ばしをしたらリセット)
	int             m_nSkipOver;              // 軌跡飛ばしをした回数(ステップを踏んだらリセット)
	int             m_nBackStepCounter;       // バックステップを踏んだ回数(バックステップは必ず2回踏ませるため)
	int             m_nTiredCounter;          // 疲れ状態になってからの時間を数えるカウンター
	int             m_nSkipOverOrbitCounter;  // 軌跡飛ばし状態になってからの時間を数えるカウンター
	int             m_nDamageCounter;         // ダメージをくらってからの時間
	float           m_fRedAdd[2];             // どれくらい赤色にするか
	int             m_nRedCounter;            // モデルの色変更を管理するカウンター
	D3DXCOLOR       m_ModelColorDef[4][20];   // モデルの色(デフォルト値)
};

//*****************************************************************************
//    配置用エネミークラスの定義
//*****************************************************************************
class CSetEnemy : public CEnemy
{
public:    // 誰でもアクセス可能
	CSetEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETENEMY);
	~CSetEnemy();

	static CSetEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

//*****************************************************************************
//    エネミー管轄クラスの定義
//*****************************************************************************
class CEnemyManager : CScene
{
public:    // 誰でもアクセス可能
	CEnemyManager(int nPriority = 1, OBJTYPE objType = OBJTYPE_ENEMYMANAGER);
	~CEnemyManager();

	static CEnemyManager *Create(int nType, char *pFileName, int nPriority = 1);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore);
	CSetEnemy *SetMapEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore);

	void SetFileName(char *pFileName);
	void SetType(const int nType);
	void SetModelManager(CModelManager *pModelManager);

	char *GetFileName(void);
	int GetType(void);
	CModelManager *GetModelManager(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CEnemy *CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const float fAI_CheackDistance, const float fAI_AttackDistance, const int nAI_AttackTiming, const int nLife, const int nAddScore = 1000);
	CSetEnemy *CreateSetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const float fAI_CheackDistance, const float fAI_AttackDistance, const int nAI_AttackTiming, const int nLife, const int nAddScore = 1000);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char            m_aFileName[256];    // エネミーデータのスクリプトファイル名
	CModelManager   *m_pModelManager;    // モデル管轄クラスへのポインタ

	CPlayer         *m_pPlayer;          // AIを変化させる対象のオブジェクト(今回はプレイヤー)
	int             m_nNumModel;         // 読み込むモデル数
	int             m_nType;             // 読み込んだ敵の種類
	int             m_nNumParts;         // 読み込むパーツ数
	int             *m_pNumParent;       // 親モデルの番号
	CTextureManager *m_pTextureManager;  // テクスチャ管理クラスへのポインタ
	CMotionManager  *m_pMotionManager;   // モーション管理クラスへのポインタ
	CModel          **m_apModel;         // モデルクラスへのポインタ
	float           m_fGravity;          // 敵の重力情報
	float           m_fColHeight;        // 当たり判定を取る高さ
	float           m_fColRange;         // 当たり判定を取る範囲
};

//*****************************************************************************
//    エネミー管轄まとめクラスの定義
//*****************************************************************************
class CEnemyAssembly
{
public:    // 誰でもアクセス可能
	CEnemyAssembly();
	~CEnemyAssembly();

	void SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx);
	CEnemyManager *GetEnemyManager(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CEnemyManager *m_pEnemyManager[MAX_ENEMY_TYPE];   // 敵管轄クラスへのポインタ
};

#endif