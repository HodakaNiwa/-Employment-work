//*****************************************************************************
//
//     �v���C���[�̏���[player.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY       (4)  // �v���C���[�̏����D�揇��
#define PLAYER_AFTERIMAGE_NUM (4)  // �c�����o���鐔
#define PLAYER_AVATAER_NUM    (4)  // ���g���o���鐔

//*****************************************************************************
//    �O���錾
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
//    �v���C���[�Ǌ��N���X�̒�`
//*****************************************************************************
class CPlayerManager : CScene
{
public:    // �N�ł��A�N�Z�X�\
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

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char            m_aFileName[256];    // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	int             m_nLife;             // �v���C���[�̗̑�
	float           m_fOrbitSlash;       // �v���C���[�̋O�ՃQ�[�W��
	float           m_fMaxSpecial;       // �v���C���[�̕K�E�Z�Q�[�W�̍ő�l
	CModelManager   *m_pModelManager;    // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
};

//*****************************************************************************
//    �v���C���[�N���X�̒�`
//*****************************************************************************
class CPlayer : public CCharacter
{
public:    // �N�ł��A�N�Z�X�\
	//--------------------
	//  ��Ԃ̎��
	//--------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // �ʏ���
		STATE_MOVE,         // �ړ����
		STATE_ATTACK_0,     // �U�����(1���[�V������)
		STATE_ATTACK_1,     // �U�����(2���[�V������)
		STATE_ATTACK_2,     // �U�����(3���[�V������)
		STATE_ATTACK_3,     // �U�����(4���[�V������)
		STATE_JUMP,         // �W�����v���
		STATE_LANDING,      // ���n���
		STATE_DAMAGE,       // �_���[�W���
		STATE_ORACLEATTACK, // �I���N���A�^�b�N���
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

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
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

	static D3DXVECTOR3 m_AvaterOffset[PLAYER_AVATAER_NUM];           // ���g�̃I�t�Z�b�g�ʒu

	COrbitEffect       *m_pOrbitEffect;                              // �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	CEffectManager     *m_pEffectManager;                            // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CGaugeLife2D       *m_pLifeGauge;                                // �̗̓Q�[�W�ւ̃|�C���^
	CGaugeOrbitSlash2D *m_pOrbitSlashGauge;                          // �O�ՃQ�[�W�ւ̃|�C���^
	CGaugeSpecial2D    *m_pSpecialGauge;                             // �K�E�Q�[�W�ւ̃|�C���^
	CSceneBillboard    *m_pLockOnPoly;                               // ���b�N�I���\���p�|���S���N���X�ւ̃|�C���^
	CEnemy             *m_pLockOnEnemy;                              // ���b�N�I���Ώۂ̓G�N���X�ւ̃|�C���^
	STATE              m_State;                                      // ���
	D3DXVECTOR3        m_Move;                                       // �ړ���
	int                m_nLife;                                      // �̗�
	int                m_nMaxLife;                                   // �̗͂̍ő�l
	int                m_nLifeCounter;                               // �̗͂̉񕜂��Ǘ�����J�E���^�[
	int                m_nEffectCounter;                             // �G�t�F�N�g���o���Ԋu���Ǘ�����J�E���^�[
	int                m_nOrbitCounter;                              // �O�ՃQ�[�W���񕜂���Ԋu���Ǘ�����J�E���^�[
	int                m_nAvaterCounter;                             // ���g���o�Ă���̕b��
	int                m_nAvaterNum;                                 // ���݂̕��g�̐�
	int                m_nDamageCounter;                             // �_���[�W���󂯂Ă���̎��Ԃ��Ǘ�����J�E���^�[
	float              m_fAccel;                                     // �����x
	float              m_fInertia;                                   // ����
	float              m_fJumpPower;                                 // �W�����v��
	float              m_fRivisionRot;                               // ������␳����{��
	float              m_fOrbitSlash;                                // �O�Վa�Ɏg�p����O�ՃQ�[�W��
	float              m_fOrbitSlashDef;                             // �O�Վa�Ɏg�p����O�ՃQ�[�W��
	float              m_fSpecial;                                   // ���݂̕K�E�Z�Q�[�W
	float              m_fMaxSpecial;                                // �K�E�Z�Q�[�W�̍ő�l
	bool               m_bJump;                                      // �W�����v���Ă��邩�ǂ���
	bool               m_bOrbitSlash;                                // �O�Վa�����邩�ǂ���
	bool               m_bAction;                                    // �A�N�V���������Ă��邩�ǂ���
	bool               m_bOrbit;                                     // �O�Ղ��o���Ă��邩�ǂ���
	bool               m_bBooking;                                   // �A�N�V�����̗\�񂪓����Ă��邩�ǂ���
	bool               m_bInRiver;                                   // ��ɓ����Ă��邩�ǂ���
	bool               m_bInGrass;                                   // ���ނ�̒��ɓ����Ă��邩�ǂ���
	bool               m_bLockOn;                                    // ���b�N�I�����Ă��邩�ǂ���
	bool               m_bOracleAttack;                              // �I���N���A�^�b�N�����Ă��邩�ǂ���
	bool               m_bDamage;                                    // �_���[�W���󂯂����ǂ���


	// �c���L���p�ϐ�
	D3DXVECTOR3        m_AfterPos[PLAYER_AFTERIMAGE_NUM];            // �c���L���p�̍��W
	D3DXVECTOR3        m_AfterRot[PLAYER_AFTERIMAGE_NUM];            // �c���L���p�̌���
	D3DXVECTOR3        *m_AfterModelPos[PLAYER_AFTERIMAGE_NUM];      // �c���L���p�̃��f�����W
	D3DXVECTOR3        *m_AfterModelRot[PLAYER_AFTERIMAGE_NUM];      // �c���L���p�̃��f������
	int                m_nCurrentStack;                              // ���݂̎c���L���ԍ�


	// ���g�U���p
	COrbitEffect       *m_pOrbitEffectAvater[PLAYER_AVATAER_NUM];     // ���g�p�̋O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	D3DXMATRIX         m_AvaterModelMtxWorld[PLAYER_AVATAER_NUM];     // ���g�̃��[���h�}�g���b�N�X(����̂�)
};

#endif