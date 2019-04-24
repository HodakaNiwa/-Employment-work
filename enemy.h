//*****************************************************************************
//
//     �G�̏���[enemy.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define ENEMY_PRIORITY  (4)     // �G�̏����D�揇��
#define MAX_ENEMY_TYPE  (10)    // �ǂݍ��߂�G�̎�ނ̐�

//*****************************************************************************
//    �O���錾
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
//    �G�l�~�[�N���X�̒�`
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		TYPE_FIRE = 0,   // �΂̓G
		TYPE_ICE,        // �X�̓G
		TYPE_BOSS,       // �{�X�̓G
		TYPE_MAX
	}TYPE;

	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_NORMAL = 0,      // �ʏ���
		STATE_MOVE,            // �ړ����
		STATE_ACTION,          // �A�N�V�������
		STATE_DAMAGE,          // �_���[�W���
		STATE_DEATH,           // ���S���
		STATE_BOSS_ATTACK_0,   // �{�X�̍U�����[�V����(1��)
		STATE_BOSS_ATTACK_1,   // �{�X�̍U�����[�V����(2��)
		STATE_BOSS_ATTACK_2,   // �{�X�̍U�����[�V����(3��)
		STATE_BOSS_ATTACK_3,   // �{�X�̍U�����[�V����(4��)
		STATE_BOSS_SKIPOVER,   // �{�X�̋O�Ք�΂����[�V����
		STATE_BOSS_STEP,       // �{�X�̃X�e�b�v���[�V����
		STATE_BOSS_BACKSTEP,   // �{�X�̃o�b�N�X�e�b�v���[�V����
		STATE_BOSS_TIRED,      // �{�X�̔�ꃂ�[�V����
		STATE_MAX
	}STATE;

	//----------------------
	// AI�̏��
	//----------------------
	typedef enum
	{
		AI_TYPE_CHEAK = 0,     // ������
		AI_TYPE_ACCESS,        // �Ώۂ̃I�u�W�F�N�g�ɋ߂Â����
		AI_TYPE_ATTACK,        // �Ώۂ̃I�u�W�F�N�g�ɍU�����d�|������
		AI_TYPE_RETURN_CHEAK,  // �����Ԃɖ߂���
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

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
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

	STATE       m_State;               // ���
	AI_TYPE     m_AIType;              // AI�̏��
	TYPE        m_Type;                // ���
	D3DXVECTOR3 m_Move;                // �ړ���
	int         m_nLife;               // �̗�
	int         m_nAddScore;           // �|�������ɑ�����X�R�A��
	CGaugeLife  *m_pGaugeLife;         // �̗̓Q�[�W
	float       m_fAI_CheackDistance;  // �x���ł���͈�
	float       m_fAI_AttackDistance;  // �U�����d�|���鋗��
	int         m_nAI_AttackTiming;    // �U�����d�|����^�C�~���O(�����_���l)
	D3DXVECTOR3 m_AI_DestPos;          // �ړI�̈ʒu
	int         m_nAI_Counter;         // AI���Ǘ�����J�E���^�[
	bool        m_bAI_Patrol;          // �p�g���[���s�������Ă��邩�ǂ���
	int         m_nDispCounter;        // �`����Ǘ�����J�E���^�[
	D3DXVECTOR3 m_PosStd;              // ���f���̍��W(������)
	D3DXVECTOR3 m_RotStd;              // ���f���̌���(������)
	bool        m_bDamage;             // �_���[�W��������Ă��邩�ǂ���
};

//*****************************************************************************
//    �{�X�N���X�̒�`
//*****************************************************************************
class CBoss : public CEnemy
{
public:    // �N�ł��A�N�Z�X�\
	CBoss(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CBoss();

	//----------------------
	// AI�̏��(�{�X�o�[�W����)
	//----------------------
	typedef enum
	{
		AI_TYPE_BOSS_CHECK = 0,       // �v���C���[�Ƃ̋����𑪂���
		AI_TYPE_BOSS_ATTACK,          // �U�����d�|������
		AI_TYPE_BOSS_SKIPOVER_ORBIT,  // �O�Ղ��΂��čU��������
		AI_TYPE_BOSS_TIRED,           // �O�Ղ��΂��Ĕ��Ă��܂������
		AI_TYPE_BOSS_STEP,            // �v���C���[�ɃX�e�b�v�ŋ߂Â����
		AI_TYPE_BOSS_BACKSTEP,        // �Ώۂ̃I�u�W�F�N�g�ɍU�����d�|������
		AI_TYPE_BOSS_MAX
	}AI_TYPE_BOSS;

	static CBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Hit(int nDamage);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
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


	COrbitEffect    *m_pOrbitEffect;          // �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	CEffectManager  *m_pEffectManager;        // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	AI_TYPE_BOSS    m_AIType_Boss;            // �{�X��AI�̏��
	int             m_nBackStep;              // �o�b�N�X�e�b�v�𓥂񂾉�(�U���s����������0�Ƀ��Z�b�g)
	int             m_nAttack;                // �U�����d�|������(�o�b�N�X�e�b�v��ԂɈڂ����烊�Z�b�g)
	int             m_nStep;                  // �X�e�b�v�𓥂񂾉�(�O�Ք�΂��������烊�Z�b�g)
	int             m_nSkipOver;              // �O�Ք�΂���������(�X�e�b�v�𓥂񂾂烊�Z�b�g)
	int             m_nBackStepCounter;       // �o�b�N�X�e�b�v�𓥂񂾉�(�o�b�N�X�e�b�v�͕K��2�񓥂܂��邽��)
	int             m_nTiredCounter;          // ����ԂɂȂ��Ă���̎��Ԃ𐔂���J�E���^�[
	int             m_nSkipOverOrbitCounter;  // �O�Ք�΂���ԂɂȂ��Ă���̎��Ԃ𐔂���J�E���^�[
	int             m_nDamageCounter;         // �_���[�W��������Ă���̎���
	float           m_fRedAdd[2];             // �ǂꂭ�炢�ԐF�ɂ��邩
	int             m_nRedCounter;            // ���f���̐F�ύX���Ǘ�����J�E���^�[
	D3DXCOLOR       m_ModelColorDef[4][20];   // ���f���̐F(�f�t�H���g�l)
};

//*****************************************************************************
//    �z�u�p�G�l�~�[�N���X�̒�`
//*****************************************************************************
class CSetEnemy : public CEnemy
{
public:    // �N�ł��A�N�Z�X�\
	CSetEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETENEMY);
	~CSetEnemy();

	static CSetEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

//*****************************************************************************
//    �G�l�~�[�Ǌ��N���X�̒�`
//*****************************************************************************
class CEnemyManager : CScene
{
public:    // �N�ł��A�N�Z�X�\
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

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
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

	char            m_aFileName[256];    // �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C����
	CModelManager   *m_pModelManager;    // ���f���Ǌ��N���X�ւ̃|�C���^

	CPlayer         *m_pPlayer;          // AI��ω�������Ώۂ̃I�u�W�F�N�g(����̓v���C���[)
	int             m_nNumModel;         // �ǂݍ��ރ��f����
	int             m_nType;             // �ǂݍ��񂾓G�̎��
	int             m_nNumParts;         // �ǂݍ��ރp�[�c��
	int             *m_pNumParent;       // �e���f���̔ԍ�
	CTextureManager *m_pTextureManager;  // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	CMotionManager  *m_pMotionManager;   // ���[�V�����Ǘ��N���X�ւ̃|�C���^
	CModel          **m_apModel;         // ���f���N���X�ւ̃|�C���^
	float           m_fGravity;          // �G�̏d�͏��
	float           m_fColHeight;        // �����蔻�����鍂��
	float           m_fColRange;         // �����蔻������͈�
};

//*****************************************************************************
//    �G�l�~�[�Ǌ��܂Ƃ߃N���X�̒�`
//*****************************************************************************
class CEnemyAssembly
{
public:    // �N�ł��A�N�Z�X�\
	CEnemyAssembly();
	~CEnemyAssembly();

	void SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx);
	CEnemyManager *GetEnemyManager(int nIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CEnemyManager *m_pEnemyManager[MAX_ENEMY_TYPE];   // �G�Ǌ��N���X�ւ̃|�C���^
};

#endif