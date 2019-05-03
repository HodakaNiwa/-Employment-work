//*****************************************************************************
//
//     �G�l�~�[�̏���[enemy.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "gauge.h"
#include "score.h"
#include "ui.h"
#include "meshField.h"
#include "object.h"
#include "meshWall.h"
#include "player.h"
#include "game.h"
#include "result.h"
#include "map.h"
#include "effectManager.h"
#include "particle.h"
#include "orbitEffect.h"
#include "shadow.h"
#include "textureManager.h"
#include "tornade.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �G�̃f�[�^�ݒ�p�}�N��
#define ENEMY_GAUGE_POS              (D3DXVECTOR3(0.0f,90.0f,0.0f))     // �G�̃Q�[�W�I�t�Z�b�g�ʒu
#define ENEMY_GAUGE_WIDTH            (40.0f)                            // �G�̗̑̓Q�[�W�̕�
#define ENEMY_GAUGE_HEIGHT           (5.0f)                             // �G�̗̑̓Q�[�W�̍���
#define ENEMY_SHADOW_PRIORITY        (9)                                // �G�̉e�̏����D�揇��
#define ENEMY_SHADOW_WIDTH           (13.0f)                            // �G�̉e�̕�
#define ENEMY_SHADOW_HEIGHT          (13.0f)                            // �G�̉e�̍���
#define ENEMY_SHADOW_COLOR           (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // �G�̉e�̐F

// �{�X�̃f�[�^�ݒ�p�}�N��
#define BOSS_SHADOW_WIDTH            (18.0f)                            // �{�X�̉e�̕�
#define BOSS_SHADOW_HEIGHT           (18.0f)                            // �{�X�̉e�̍���
#define BOSS_SHADOW_COLOR            (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // �{�X�̉e�̐F
#define BOSS_INERTIA                 (0.423f)                           // �{�X�̊���
#define BOSS_ATTACK_ORBIT            (420)                              // �O�Ղ��΂��čU���������鎞��
#define BOSS_STEP_MOVE               (6.0f)                             // �X�e�b�v�𓥂񂾎��̈ړ���
#define BOSS_ATTACK_ORBIT_INTERVAL_0 (40)                               // �O�Ղ��΂��Ԋu(�̗͂��܂�����Ƃ�)
#define BOSS_ATTACK_ORBIT_INTERVAL_1 (30)                               // �O�Ղ��΂��Ԋu(�̗͂��܂���������Ƃ�)
#define BOSS_ATTACK_ORBIT_INTERVAL_2 (20)                               // �O�Ղ��΂��Ԋu(�̗͂������Ȃ��Ƃ�)
#define BOSS_RESTART_TIRED           (240)                              // ����Ԃ���N���オ��܂ł̎���
#define BOSS_BONUS                   (10000)                            // ���X�{�X���j�{�[�i�X
#define BOSS_AI_NEAR_DISTANCE        (180.0f)                           // �v���C���[���߂��Ɣ��肳��鋗��
#define BOSS_AI_ATTACK_DISTANCE      (200.0f)                           // �U�����d�|���鋗��
#define BOSS_RED_CHANGETIMING_0      (120)                              // �̗͂��Ⴂ�Ƃ��Ƀ��f���̐F��ς���l�𔽓]������^�C�~���O(�܂��̗͂���)
#define BOSS_RED_CHANGETIMING_1      (40)                               // �̗͂��Ⴂ�Ƃ��Ƀ��f���̐F��ς���l�𔽓]������^�C�~���O(�����̗͂Ȃ�)
#define BOSS_RED_CHANGE_0            (0.008f)                           // �̗͂��Ⴂ�Ƃ��Ƀ��f���̐F��ς���l(�܂��̗͂���)
#define BOSS_RED_CHANGE_1            (0.02f)                            // �̗͂��Ⴂ�Ƃ��Ƀ��f���̐F��ς���l(�����̗͂Ȃ�)

// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// ����
#define POS                "POS = "                // ���W��ǂݎ�鍇�}
#define ROT                "ROT = "                // ������ǂݎ�鍇�}
#define MOVE               "MOVE = "               // �ړ��ʂ�ǂݎ�鍇�}

// ���̑�
#define NUM_TEXTURE        "NUM_TEXTURE = "        // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME   "TEXTURE_FILENAME = "   // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL          "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME     "MODEL_FILENAME = "     // �ǂݍ��ރ��f���̃t�@�C����

// �L�����N�^�[���
#define CHARACTERSET       "CHARACTERSET"          // �L�����N�^�[���ǂݍ��݊J�n�̍��}
#define END_CHARACTERSET   "END_CHARACTERSET"      // �L�����N�^�[���ǂݍ��ݏI���̍��}
#define JUMP               "JUMP = "               // �W�����v��
#define GRAVITY            "GRAVITY = "            // �d�͗�
#define COL_HEIGHT         "COL_HEIGHT = "         // �����蔻����Ƃ鍂��
#define COL_RANGE          "COL_RANGE = "          // �����蔻����Ƃ�͈�
#define NUM_PARTS          "NUM_PARTS = "          // �L�����N�^�[�̃p�[�c��

// �p�[�c���
#define PARTSSET           "PARTSSET"              // �p�[�c���ǂݍ��݊J�n�̍��}
#define END_PARTSSET       "END_PARTSSET"          // �p�[�c���ǂݍ��ݏI���̍��}
#define INDEX              "INDEX = "              // �ԍ���ǂݎ�鍇�}
#define PARENT             "PARENT = "             // �e�p�[�c�̔ԍ���ǂݎ�鍇�}

// ���[�V�������
#define MOTIONSET         "MOTIONSET"             // ���[�V�������ǂݍ��݊J�n�̍��}
#define END_MOTIONSET     "END_MOTIONSET"         // ���[�V�������ǂݍ��ݏI���̍��}
#define COLLISIONSET      "COLLISIONSET"          // ���[�V�����̓����蔻����ǂݍ��݊J�n�̍��}
#define END_COLLISIONSET  "END_COLLISIONSET"      // ���[�V�����̓����蔻����ǂݍ��ݏI���̍��}
#define ORBITSET          "ORBITSET"              // ���[�V�����̋O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBITSET      "END_ORBITSET"          // ���[�V�����̋O�Տ��ǂݍ��ݏI���̍��}
#define RANGE             "RANGE = "              // �͈͂�ǂݎ�鍇�}
#define XBLOCK            "XBLOCK = "             // �O�Ղ̉��̕�������ǂݎ�鍇�}
#define ZBLOCK            "ZBLOCK = "             // �O�Ղ̉��s�̕�������ǂݎ�鍇�}
#define OFFSET            "OFFSET = "             // �I�t�Z�b�g��ǂݎ�鍇�}
#define LOOP              "LOOP = "               // ���[�V���������[�v�Đ����邩�ǂ�����ǂݎ�鍇�}
#define BLEND             "BLEND = "              // ���[�V�����u�����h�̔{����ǂݎ�鍇�}
#define NUM_KEY           "NUM_KEY = "            // ���[�V�����̃L�[�t���[���̐���ǂݎ�鍇�}

// �U�����[�V�������
#define ATTACKSET         "ATTACKSET"             // �U�����[�V�������ǂݍ��݊J�n�̍��}
#define END_ATTACKSET     "END_ATTACKSET"         // �U�����[�V�������ǂݍ��ݏI���̍��}
#define NEXT_MOTION       "NEXT_MOTION = "        // ���̃��[�V�����̔ԍ�

// �����蔻����
#define NUM_COL           "NUM_COL = "            // ���[�V�����̓����蔻����̐���ǂݎ�鍇�}
#define COLLISION         "COLLISION"             // �����蔻����ǂݍ��݊J�n�̍��}
#define DAMAGE            "DAMAGE = "             // ���������ꍇ�̃_���[�W
#define END_COLLISION     "END_COLLISION"         // �����蔻����ǂݍ��ݏI���̍��}

// �O�Տ��
#define NUM_ORBIT         "NUM_ORBIT = "          // ���[�V�����̋O�Տ��̐���ǂݎ�鍇�}
#define ORBIT		      "ORBIT"                 // �O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBIT	      "END_ORBIT"             // �O�Տ��ǂݍ��ݏI���̍��}
#define TEX_INDEX         "TEX_INDEX = "          // �O�Ղɒ���t����e�N�X�`���̔ԍ���ǂݎ�鍇�}
#define OFFSET1           "OFFSET1 = "            // �P�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSET2           "OFFSET2 = "            // �Q�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSETAMP1        "OFFSETAMP1 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�P��)
#define OFFSETAMP2        "OFFSETAMP2 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�Q��)
#define COLUP             "COLUP = "              // �O�Ղ̒��_�㑤�̐F
#define COLDOWN           "COLDOWN = "            // �O�Ղ̒��_�����̐F
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // �O�Ղ̒��_�㑤�̓����x�����l
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // �O�Ղ̒��_�����̓����x�����l
#define START             "START = "              // �O�Ղ��o���n�߂�^�C�~���O��ǂݎ�鍇�}
#define FINISH            "FINISH = "             // �O�Ղ����܂��^�C�~���O��ǂݎ�鍇�}

// �L�[�t���[�����
#define KEYSET             "KEYSET"                // �L�[�t���[�����ǂݍ��݊J�n�̍��}
#define END_KEYSET         "END_KEYSET"            // �L�[�t���[�����ǂݍ��ݏI���̍��}
#define FRAME              "FRAME = "              // �L�[�t���[���̍Đ��t���[������ǂݎ�鍇�}

// �L�[���
#define KEY                "KEY"                   // �L�[���ǂݍ��݊J�n�̍��}
#define END_KEY            "END_KEY"               // �L�[���ǂݍ��ݏI���̍��}

//*****************************************************************************
//     CEnemyManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemyManager::CEnemyManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	strcpy(m_aFileName, "\0");      // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	m_pModelManager = NULL;         // ���f���Ǌ��N���X�ւ̃|�C���^

	m_nNumModel = 0;                // �ǂݍ��ރ��f����
	m_nType = 0;                    // ���
	m_nNumParts = 0;                // �ǂݍ��ރp�[�c��
	m_pNumParent = NULL;            // �e���f���̔ԍ�
	m_pMotionManager = NULL;        // ���[�V�����Ǘ��N���X�ւ̃|�C���^
	m_apModel = NULL;               // ���f���N���X�ւ̃|�C���^
	m_fGravity = 0.0f;              // �G�̏d�͏��
	m_fColHeight = 0.0f;            // �����蔻�����鍂��
	m_fColRange = 0.0f;             // �����蔻������͈�
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemyManager::~CEnemyManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEnemyManager *CEnemyManager::Create(int nType, char *pFileName, int nPriority)
{
	CEnemyManager *pEnemyManager = NULL;   // �G�l�~�[�Ǌ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEnemyManager == NULL)
		{// ����������ɂȂ��Ă���
			pEnemyManager = new CEnemyManager(nPriority);
			if (pEnemyManager != NULL)
			{// �C���X�^���X�𐶐��ł���
				pEnemyManager->SetFileName(pFileName);
				pEnemyManager->SetType(nType);
				if (FAILED(pEnemyManager->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pEnemyManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEnemyManager::Init(void)
{
	// �G�l�~�[�̃p�[�c���𔲂��o��
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTexture = 0;                        // �ǂݍ��ރe�N�X�`���̐�
			int nCntModel = 0;                          // ���f����ǂݍ��񂾉񐔂𐔂���
			int nCntParts = 0;                          // �p�[�c����ǂݍ��񂾉񐔂𐔂���
			int nCntMotion = 0;                         // ���[�V������ǂݍ��񂾉񐔂𐔂���
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉񐔂𐔂���
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`�����ւ̃|�C���^
			LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                          // �}�e���A�����̐�
			CMotion *pMotion[CEnemy::STATE_MAX] = {};   // ���[�V�����N���X�ւ̃|�C���^

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂ���Ă���
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// �ǂݍ��ރe�N�X�`����������
								strcpy(pStr, pStrCur);
								nNumTexture = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTexture >= 1)
								{// �ǂݍ��ރe�N�X�`������1�ȏ゠��
									m_pTextureManager = CTextureManager::Create(nNumTexture);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// �e�N�X�`���̃t�@�C���p�X����ǂݍ���
								if (m_pTextureManager != NULL && nCntTex < nNumTexture)
								{// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���m�ۂ���Ă���
								    // �e�N�X�`���̃t�@�C���p�X����ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

									// �e�N�X�`���̓ǂݍ���
									D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
									if (pTexture != NULL)
									{// �e�N�X�`����ǂݍ��߂�
										m_pTextureManager->BindTexture(pTexture, nCntTex);
										m_pTextureManager->SetFileName(pStr, nCntTex);
										pTexture = NULL;
										nCntTex++;
									}
								}
							}
							if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// ���f������ǂݍ��ނȂ�
								strcpy(pStr, pStrCur);
								m_nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (m_nNumModel >= 1)
								{// ���f������1�ȏ゠��
									m_pNumParent = new int[m_nNumModel];
									m_pModelManager = CModelManager::Create(m_nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// ���f���̃t�@�C���p�X����ǂݍ���
								if (m_pModelManager != NULL && nCntModel < m_nNumModel)
								{// ���f���Ǌ��N���X�ւ̃|�C���^���m�ۂ���Ă���
									// ���f���̃t�@�C���p�X����ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

									// x�t�@�C���̓ǂݍ���
									D3DXLoadMeshFromX(pStr,
										D3DXMESH_SYSTEMMEM,
										CManager::GetRenderer()->GetDevice(),
										NULL,
										&pBuffMat,
										NULL,
										&nNumMat,
										&pMesh);

									// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
									m_pModelManager->SetMesh(pMesh, nCntModel);
									m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

									// �g�p�����ϐ������������Ă���
									pMesh = NULL;
									pBuffMat = NULL;
									nNumMat = 0;

									nCntModel++;  // ���f����ǂݍ��񂾉񐔂𑝂₷
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, CHARACTERSET) == 0)
							{// �L�����N�^�[���ǂݍ��݊J�n�̍��}��������
								m_apModel = ReadCharacterSet(pLine, pStrCur, pFile, &m_nNumParts, m_pNumParent, &m_fGravity, &m_fColHeight, &m_fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// ���[�V�������ǂݍ��݊J�n�̍��}��������
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &m_nNumParts, pMotion[nCntMotion], nCntMotion);
								nCntMotion++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}
				}
			}

			// �������̊J��
			if (pStr != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pStr;
				pStr = NULL;
			}
			if (pLine != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pLine;
				pLine = NULL;
			}

			// �K�v�ȃN���X���쐬���Ă���
			// ���[�V�����Ǌ��N���X
			if (m_pMotionManager == NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_pMotionManager = CMotionManager::Create(CEnemy::STATE_MAX, m_nNumParts);
				if (m_pMotionManager != NULL)
				{// ���[�V�����Ǘ��N���X���쐬�ł���
					for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
					{// ��Ԃ̐������J�肩����
						m_pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// �v���C���[�N���X���擾���ĕۑ����Ă���
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CPlayer *pPlayer = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// �D�揇�ʂ̐������J��Ԃ�
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
					{// �v���C���[�N���X�ւ̃|�C���^������
						pPlayer = (CPlayer*)pScene;
						if (pPlayer != NULL)
						{// �v���C���[�N���X�̃|�C���^���擾�ł���
							m_pPlayer = pPlayer;
							break;
						}
					}
					pScene = pSceneNext;
				}
			}

			// �t�@�C�������
			fclose(pFile);
		}
		else
		{// �t�@�C���|�C���^���m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �t�@�C���|�C���^���m�ۂł����ԂłȂ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEnemyManager::Uninit(void)
{
	// �v���C���[�N���X�ւ̃|�C���^����ɂ��Ă���
	m_pPlayer = NULL;

	// �ǂݎ��p�̃|�C���^���J�����Ă���
	if (m_apModel != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �g�p�ł��郂�f�������J��Ԃ�
			if (m_apModel[nCntModel] != NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_apModel[nCntModel]->Uninit();

				// �������̊J��
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// �������̊J��
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// ���[�V�����̔j��
	if (m_pMotionManager != NULL)
	{// �|�C���^���m�ۂ���Ă���
		m_pMotionManager->Uninit();

		// �������̊J��
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// �e���f���̔ԍ�
	if (m_pNumParent != NULL)
	{// ���������m�ۂ���Ă���
		delete[] m_pNumParent;
		m_pNumParent = NULL;
	}

	// ���f���Ǌ��N���X�̔j��
	if (m_pModelManager != NULL)
	{// ���������m�ۂ���Ă���
	    // �I������
		m_pModelManager->Uninit();

		// �������̊J��
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// ���g�̃|�C���^��j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEnemyManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEnemyManager::Draw(void)
{

}

//=============================================================================
//    �G��z�u���鏈��
//=============================================================================
void CEnemyManager::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// �G�l�~�[�N���X���쐬����
	CreateEnemy(m_apModel, m_pMotionManager, m_nNumParts, m_pNumParent, m_fGravity, m_fColHeight, m_fColRange, pos, rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore);
}

//=============================================================================
//    �}�b�v�G�f�B�b�g�p�̓G��z�u���鏈��
//=============================================================================
CSetEnemy *CEnemyManager::SetMapEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// �G�l�~�[�N���X���쐬����
	CSetEnemy *pSetEnemy = NULL;
	return pSetEnemy = CreateSetEnemy(m_apModel, m_pMotionManager, m_nNumParts, m_pNumParent, m_fGravity, m_fColHeight, m_fColRange, pos, rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore);
}

//=============================================================================
//    �G�l�~�[�N���X���쐻���鏈��
//=============================================================================
CEnemy *CEnemyManager::CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const float fAI_CheackDistance, const float fAI_AttackDistance, const int nAI_AttackTiming, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // ���f���N���X�R�s�[�p
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionCollision **pMotionCollision = NULL;    // �����蔻��f�[�^�N���X�R�s�[�p
	CMotionOrbit **pMotionOrbit = NULL;            // ���[�V�����̋O�Ճf�[�^�N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;      // ���[�V�����Ǌ��N���X�R�s�[�p
	CEnemy *pEnemy = NULL;                         // �G�l�~�[�N���X�ւ̃|�C���^

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	    // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// �G�̏�Ԃ̐������J��Ԃ�
					if (pMotionManager->GetMotion()[nCntMotion] != NULL)
					{// ���[�V��������������Ă���
						if (pMotionCpy[nCntMotion] == NULL)
						{// ���������m�ۂł����Ԃł���
							pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
							if (pMotionCpy[nCntMotion] != NULL)
							{// ���������m�ۂł���
								// �����蔻��f�[�^�N���X��ݒ肷��
								int nNumColData = pMotionManager->GetMotion()[nCntMotion]->GetNumColData();
								if (nNumColData >= 1)
								{// 1�ȏ㓖���蔻��f�[�^�N���X�����݂���
									pMotionCollision = new CMotionCollision*[nNumColData];
									if (pMotionCollision != NULL)
									{// ���������m�ۂł���
										for (int nCntNumCol = 0; nCntNumCol < nNumColData; nCntNumCol++)
										{// �����蔻��f�[�^�N���X�̐������J��Ԃ�
											pMotionCollision[nCntNumCol] = new CMotionCollision;
											if (pMotionCollision[nCntNumCol] != NULL)
											{// ���������m�ۂł���
												// �f�[�^���R�s�[����
												pMotionCollision[nCntNumCol]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetModelIdx());
												pMotionCollision[nCntNumCol]->SetOffsetPos(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetOffsetPos());
												pMotionCollision[nCntNumCol]->SetRange(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetRange());
												pMotionCollision[nCntNumCol]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetStartTiming());
												pMotionCollision[nCntNumCol]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetFinishTiming());
												pMotionCollision[nCntNumCol]->SetDamage(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntNumCol]->GetDamage());
											}
										}
										pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
									}
								}
								pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());

								// �O�Ճf�[�^�N���X��ݒ肷��
								// �O�Ճf�[�^�N���X�̃R�s�[
								if (pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
								{// �����蔻��f�[�^���P�ȏ゠��
									pMotionOrbit = new CMotionOrbit*[pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
									if (pMotionOrbit != NULL)
									{// ���������m�ۂł���
										for (int nCntOrbit = 0; nCntOrbit < pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
										{// �����蔻��f�[�^�̐������J��Ԃ�
											pMotionOrbit[nCntOrbit] = new CMotionOrbit;
											if (pMotionOrbit[nCntOrbit] != NULL)
											{// ���������m�ۂł���
												pMotionOrbit[nCntOrbit]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
												pMotionOrbit[nCntOrbit]->SetTexIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
												pMotionOrbit[nCntOrbit]->SetOffsetPos1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
												pMotionOrbit[nCntOrbit]->SetOffsetPos2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
												pMotionOrbit[nCntOrbit]->SetOffsetAmp1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
												pMotionOrbit[nCntOrbit]->SetOffsetAmp2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
												pMotionOrbit[nCntOrbit]->SetColUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
												pMotionOrbit[nCntOrbit]->SetColDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
												pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
												pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
												pMotionOrbit[nCntOrbit]->SetXBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
												pMotionOrbit[nCntOrbit]->SetYBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
												pMotionOrbit[nCntOrbit]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
												pMotionOrbit[nCntOrbit]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
											}
										}
									}
									pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								}
								pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());

								for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
								{// �L�[�t���[���̑������J��Ԃ�
									for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
									{// �L�[�t���[���̑������J��Ԃ�
										pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
										pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
									}
								}

								// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
								pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
							}
						}
					}
				}
			}
		}

		// �G�l�~�[�N���X���쐬����
		if (pEnemy == NULL)
		{// ���������m�ۂł����Ԃł���
			if (nType == CEnemy::TYPE_BOSS)
			{// �{�X�G������
				pEnemy = CBoss::Create(pos, rot, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore, ENEMY_PRIORITY);
				if (pEnemy != NULL)
				{// �����ɐ�������
					if (m_pTextureManager != NULL)
					{// �e�N�X�`���Ǌ��N���X����������Ă���
						pEnemy->SetTextureManager(m_pTextureManager);
					}
				}
			}
			else
			{// �{�X�̓G�ł͂Ȃ�
				pEnemy = CEnemy::Create(pos, rot, (CEnemy::TYPE)nType, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore, ENEMY_PRIORITY);
			}
		}
	}

	return pEnemy;
}

//=============================================================================
//    �z�u�p�̃G�l�~�[�N���X���쐻���鏈��
//=============================================================================
CSetEnemy *CEnemyManager::CreateSetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nType, const float fAI_CheackDistance, const float fAI_AttackDistance, const int nAI_AttackTiming, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // ���f���N���X�R�s�[�p
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;      // ���[�V�����Ǌ��N���X�R�s�[�p
	CSetEnemy *pSetEnemy = NULL;                   // �z�u�p�G�l�~�[�N���X�ւ̃|�C���^

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	    // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// �G�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
						    // �U�����[�V�����f�[�^�N���X��ݒ肷��
							pMotionCpy[nCntMotion]->SetAttack(pMotionManager->GetMotion()[nCntMotion]->GetAttack());

							// �����蔻��f�[�^�N���X��ݒ肷��
							pMotionCpy[nCntMotion]->SetCollision(pMotionManager->GetMotion()[nCntMotion]->GetCollision());
							pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());

							// �O�Ճf�[�^�N���X��ݒ肷��
							pMotionCpy[nCntMotion]->SetOrbit(pMotionManager->GetMotion()[nCntMotion]->GetOrbit());
							pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());

							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �z�u�p�G�l�~�[�N���X���쐬����
		if (pSetEnemy == NULL)
		{// ���������m�ۂł����Ԃł���
			pSetEnemy = CSetEnemy::Create(pos, rot, (CEnemy::TYPE)nType, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore);
		}
	}

	return pSetEnemy;
}

//=============================================================================
//    �G�l�~�[����ǂݎ�鏈��
//=============================================================================
CModel **CEnemyManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// �d�͗ʂ�����
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	�����蔻�����鍂��������
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͂�����
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// �p�[�c����񂪂���
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			if (apModel == NULL)
			{// ���������m�ۂł����Ԃł���
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// �p�[�c���ǂݍ��݊J�n�̍��}������
			if (nCntParts < *pNumParts)
			{// �܂��p�[�c����ǂݍ��߂�
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_CHARACTERSET) == 0)
		{// �L�����N�^�[���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return apModel;
}

//=============================================================================
//    �p�[�c����ǂݎ�鏈��
//=============================================================================
CModel *CEnemyManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // �p�[�c�ԍ����
	int nParent = -1;                                   // �e���f���̃p�[�c�ԍ�
	CModel *pModel = NULL;                             // ���f���N���X�ւ̃|�C���^
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̍��W���
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̌������
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// �p�[�c�ԍ���񂪂���
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// �e���f���̃p�[�c�ԍ���񂪂���
			pNumParent[nIdx] = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���f���̍��W��񂪂���
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ���f���̌�����񂪂���
			Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_PARTSSET) == 0)
		{// �p�[�c���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pModel == NULL)
	{// ���������m�ۂł����Ԃł���
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx),m_pModelManager->GetTexture(nIdx));
		if (pModel != NULL)
		{// ���f���|�C���^���m�ۂ��ꂽ
			if (nParent != -1)
			{// �e���f�������݂���
				pModel->SetParent(apModel[nParent]);
			}
		}
	}

	return pModel;
}

//=============================================================================
//    ���[�V��������ǂݎ�鏈��
//=============================================================================
CMotion *CEnemyManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;                           // �L�[�t���[���t���[������ǂݍ��񂾉�
	CMotionAttack *pMotionAttack = NULL;          // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	CMotionCollision **pMotionCollision = NULL;   // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^
	int nNumColData = 0;                          // �����蔻��f�[�^�̐�
	CMotionOrbit **pMotionOrbit = NULL;           // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^
	int nNumOrbitData = 0;                        // �O�Ճf�[�^�̐�
	bool bLoop = false;                           // ���[�v���邩���Ȃ���
	float fBlendMag = 0.0f;                       // �u�����h�{��
	int nNumKey = 0;                              // �L�[�t���[���̑���
	int *pPlaybackKey = NULL;                     // �Đ��t���[����(��������̂œ��I�m��)
	D3DXVECTOR3 **pAddPos = NULL;                 // ���f���̍��W
	D3DXVECTOR3 **pDestRot = NULL;                // ���f���̌���
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
		{// ���[�v���邩���Ȃ����̏�񂪂���
			bLoop = CFunctionLib::ReadBool(pStrCur, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BLEND) == 0)
		{// �u�����h�{���̏�񂪂���
			fBlendMag = CFunctionLib::ReadFloat(pStrCur, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_KEY) == 0)
		{// �L�[�t���[���̑����̏�񂪂���
			nNumKey = (CFunctionLib::ReadInt(pStrCur, NUM_KEY));
			if (nNumKey >= 1)
			{// 1�ȏ�L�[�t���[��������
			    // �K�v�ȕϐ��𓮓I�Ɋm�ۂ���
			    // �Đ��t���[����
				pPlaybackKey = new int[nNumKey];

				// ���W
				if (pAddPos == NULL)
				{// ���������m�ۂł����Ԃł���
					pAddPos = new D3DXVECTOR3*[nNumKey];
					if (pAddPos != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}

				if (pDestRot == NULL)
				{// ���������m�ۂł����Ԃł���
				 // ����
					pDestRot = new D3DXVECTOR3*[nNumKey];
					if (pDestRot != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionAttack = ReadMotionAttackSet(pLine, pStrCur, pFile);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionCollision = ReadMotionCollisionSet(pLine, pStrCur, pFile, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBITSET) == 0)
		{// �O�Տ��ǂݍ��݊J�n�̍��}������
			pMotionOrbit = ReadMotionOrbitSet(pLine, pStrCur, pFile, &nNumOrbitData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��݊J�n�̍��}������
			ReadKeySet(pLine, pStrCur, pFile, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOTIONSET) == 0)
		{// ���[�V�������ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pMotion == NULL)
	{// ���������m�ۂł����Ԃł���
		pMotion = CMotion::Create(*pNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
		if (pMotion != NULL)
		{// ���������m�ۂł���
	    	// �U�����[�V�����f�[�^�N���X��ݒ肷��
			pMotion->SetAttack(pMotionAttack);

			// �����蔻��f�[�^�N���X��ݒ肷��
			pMotion->SetCollision(pMotionCollision);
			pMotion->SetNumColData(nNumColData);

			// �O�Ճf�[�^�N���X��ݒ肷��
			pMotion->SetOrbit(pMotionOrbit);
			pMotion->SetNumOrbitData(nNumOrbitData);

			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// �L�[�t���[���̑������J��Ԃ�
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// �L�[�t���[���̑������J��Ԃ�
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	// ���������m�ۂ������J������
	// �Đ��t���[����
	if (pPlaybackKey != NULL)
	{// ���������m�ۂ���Ă���
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// ���W
	if (pAddPos != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// ����
	if (pDestRot != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
//    ���[�V�����̍U�����[�V��������ǂݎ�鏈��
//=============================================================================
CMotionAttack *CEnemyManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
{
	CMotionAttack *pMotionAttack = NULL;   // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	pMotionAttack = new CMotionAttack;     // ���������m�ۂ���
	if (pMotionAttack != NULL)
	{// ���������m�ۂł���
		while (1)
		{// ���[�v�J�n
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, NEXT_MOTION) == 0)
			{// ���̃��[�V�����̔ԍ���񂾂���
				pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStrCur, NEXT_MOTION));
			}
			else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
			{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
				pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
			}
			else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
			{// �O�Ղ����܂��^�C�~���O��񂾂���
				pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
			}
			else if (CFunctionLib::Memcmp(pStrCur, END_ATTACKSET) == 0)
			{// �U�����[�V�����f�[�^�ǂݍ��ݏI���̍��}������
				break;  // ���[�v�I��
			}
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    ���[�V�����̓����蔻�����ǂݎ�鏈��
//=============================================================================
CMotionCollision **CEnemyManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
{
	int nCntCollision = 0;                       // �����蔻��f�[�^��ǂݍ��񂾉�
	CMotionCollision **pMotionCollision = NULL;  // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^

	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_COL) == 0)
		{// �����蔻��f�[�^�̐�������
			*pNumColData = CFunctionLib::ReadInt(pStrCur, NUM_COL);
			if (*pNumColData >= 1)
			{// �����蔻��f�[�^���P�ȏ゠��
				pMotionCollision = new CMotionCollision*[*pNumColData];
				if (pMotionCollision != NULL)
				{// ���������m�ۂł���
					for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
					{// �����蔻��f�[�^�̐������J��Ԃ�
						pMotionCollision[nCntCol] = new CMotionCollision;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// �����蔻��f�[�^�ǂݍ��݊J�n�̍��}������
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// �����蔻�����郂�f���̔ԍ���񂾂���
					pMotionCollision[nCntCollision]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET) == 0)
				{// �����蔻�����郂�f������̃I�t�Z�b�g������񂾂���
					pMotionCollision[nCntCollision]->SetOffsetPos(CFunctionLib::ReadVector3(pStrCur, OFFSET));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// �����蔻������͈͏�񂾂���
					pMotionCollision[nCntCollision]->SetRange(CFunctionLib::ReadFloat(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// �����蔻������n�߂�^�C�~���O��񂾂���
					pMotionCollision[nCntCollision]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// �����蔻����I������^�C�~���O��񂾂���
					pMotionCollision[nCntCollision]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DAMAGE) == 0)
				{// �����������̃_���[�W�ʏ�񂾂���
					pMotionCollision[nCntCollision]->SetDamage(CFunctionLib::ReadInt(pStrCur, DAMAGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
				{// �����蔻��f�[�^�ǂݍ��ݏI���̍��}������
					nCntCollision++;
					break;  // ���[�v�I��
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// �����蔻����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pMotionCollision;
}

//=============================================================================
//    ���[�V�����̋O�Տ���ǂݎ�鏈��
//=============================================================================
CMotionOrbit **CEnemyManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
{
	int nCntOrbit = 0;                   // �O�Ճf�[�^��ǂݍ��񂾉�
	CMotionOrbit **pMotionOrbit = NULL;  // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^

	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_ORBIT) == 0)
		{// �O�Ճf�[�^�̐�������
			*pNumOrbitData = CFunctionLib::ReadInt(pStrCur, NUM_ORBIT);
			if (*pNumOrbitData >= 1)
			{// �����蔻��f�[�^���P�ȏ゠��
				pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
				if (pMotionOrbit != NULL)
				{// ���������m�ۂł���
					for (int nCntOr = 0; nCntOr < *pNumOrbitData; nCntOr++)
					{// �����蔻��f�[�^�̐������J��Ԃ�
						pMotionOrbit[nCntOr] = new CMotionOrbit;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBIT) == 0)
		{// �O�Ճf�[�^�ǂݍ��݊J�n�̍��}������
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// �O�Ղ��o�����f���̔ԍ���񂾂���
					pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TEX_INDEX) == 0)
				{// �O�Ղɒ���t����e�N�X�`���̔ԍ���񂾂���
					pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET1) == 0)
				{// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�P��)������
					pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStrCur, OFFSET1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET2) == 0)
				{// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�Q��)������
					pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStrCur, OFFSET2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP1) == 0)
				{// �O�Ղ̃I�t�Z�b�g���������l(1��)������
					pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP2) == 0)
				{// �O�Ղ̃I�t�Z�b�g���������l(2��)������
					pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLUP) == 0)
				{// �O�Ղ̒��_�㑤�̐F������
					pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStrCur, COLUP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLDOWN) == 0)
				{// �O�Ղ̒��_�����̐F������
					pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStrCur, COLDOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_UP) == 0)
				{// �O�Ղ̒��_�㑤�̓����x�����l������
					pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_UP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_DOWN) == 0)
				{// �O�Ղ̒��_�����̓����x�����l������
					pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_DOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
				{// �O�Ղ̉��̕�������񂾂���
					pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStrCur, XBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
				{// �O�Ղ̏c�̕�������񂾂���
					pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStrCur, ZBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
					pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// �O�Ղ����܂��^�C�~���O��񂾂���
					pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_ORBIT) == 0)
				{// �O�Ճf�[�^�ǂݍ��ݏI���̍��}������
					nCntOrbit++;
					break;  // ���[�v�I��
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ORBITSET) == 0)
		{// �O�Տ��ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pMotionOrbit;
}

//=============================================================================
//    �L�[�t���[������ǂݎ�鏈��
//=============================================================================
void CEnemyManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, FRAME) == 0)
		{// �Đ��t���[����������
			pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStrCur, FRAME);
		}
		if (CFunctionLib::Memcmp(pStrCur, KEY) == 0)
		{// �L�[���ǂݍ��݊J�n�̍��}������
			ReadKey(pLine, pStrCur, pFile, nCntKey, AddPos, DestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �L�[����ǂݎ�鏈��
//=============================================================================
void CEnemyManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W������
			AddPos[nCntKey] = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ����������
			DestRot[nCntKey] = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEY) == 0)
		{// �L�[���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    �G�̎�ނ�ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    �X�N���v�g�t�@�C�������擾���鏈��
//=============================================================================
char *CEnemyManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    �G�̎�ނ��擾���鏈��
//=============================================================================
int CEnemyManager::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CEnemyManager::GetModelManager(void)
{
	return m_pModelManager;
}


//*****************************************************************************
//     CEnemy�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_Type = TYPE_FIRE;                           // ���
	m_AIType = AI_TYPE_CHEAK;                     // AI�̏��
	m_State = STATE_NORMAL;                       // ���
	m_nLife = 0;                                  // �̗�
	m_nAddScore = 0;                              // �|�������ɑ�����X�R�A��
	m_fAI_CheackDistance = 0.0f;                  // �x���ł���͈�
	m_fAI_AttackDistance = 0.0f;                  // �U�����d�|����͈�
	m_nAI_AttackTiming = 1;                       // �U�����d�|����^�C�~���O
	m_AI_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړI�̈ʒu
	m_nAI_Counter = 0;                            // AI���Ǘ�����J�E���^�[
	m_bAI_Patrol = false;                         // �p�g���[���s�������Ă��邩�ǂ���
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �ړ���
	m_pGaugeLife = NULL;                          // �̗̓Q�[�W
	m_PosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �f�t�H���g�̈ʒu
	m_RotStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �f�t�H���g�̌���
	m_bDamage = false;                            // �_���[�W��������Ă��邩�ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore, int nPriority)
{
	CEnemy *pEnemy = NULL;                 // �G�l�~�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEnemy == NULL)
		{// ����������ɂȂ��Ă���
			pEnemy = new CEnemy(nPriority);
			if (pEnemy != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pEnemy->Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore)))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pEnemy;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// �e��l�̐ݒ�
	SetPos(pos);                               // ���݂̍��W
	SetRot(rot);                               // ���݂̌���
	SetType(type);                             // �G�̎��
	SetNumParts(nNumParts);                    // �p�[�c��
	SetModel(apModel);                         // ���f���N���X�ւ̃|�C���^
	SetMotionManager(pMotionManager);          // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	SetGravity(fGravity);                      // �d��
	SetColHeight(fColHeight);                  // �����蔻�����鍂��
	SetColRange(fColRange);                    // �����蔻������͈�
	SetAI_CheackDistance(fAI_CheackDistance);  // �x���ł���͈�
	SetAI_AttackDistance(fAI_AttackDistance);  // �U���ł���͈�
	SetAI_AttackTiming(nAI_AttackTiming);      // �U�����d�|����^�C�~���O
	SetLife(nLife);                            // �̗�
	SetAddScore(nAddScore);                    // �|�������ɑ�����X�R�A��
	m_PosStd = pos;                            // �������̍��W
	m_RotStd = rot;                            // �������̌���

	// �̗̓Q�[�W�����
	if (m_pGaugeLife == NULL)
	{
		m_pGaugeLife = CGaugeLife::Create(ENEMY_GAUGE_POS + pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), ENEMY_GAUGE_WIDTH, ENEMY_GAUGE_HEIGHT, 0.0f, m_nLife);
	}

	// ���f���̈ʒu���j���[�g�����̈ʒu�ɂ���
	SetDefaultPos();

	// ���f���̌������j���[�g�����̌����ɂ���
	SetDefaultRot();

	// �e���쐬����
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 1.5f));
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// �̗̓Q�[�W�̊J��
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}

	// ���ʂ̏I������
	CCharacter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// �O��̍��W��ۑ�
	SetPosOld(GetPos());

	// �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^��ۑ�
	SetLandSceneOld(GetLandScene());

	// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
	SetLandScene(NULL);


	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}
	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
		// �K�v�ȕϐ���錾
		float fDistance = 0.0f;                     // �Ώۂ̃I�u�W�F�N�g�Ƃ̋���
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // �Ώۂ̃I�u�W�F�N�g�̍��W
		D3DXVECTOR3 EnemyPos = GetPos();

		// �������v�Z
		fDistance = sqrtf((ObjectPos.x - EnemyPos.x) * (ObjectPos.x - EnemyPos.x) + (ObjectPos.y - EnemyPos.y) * (ObjectPos.y - EnemyPos.y) + (ObjectPos.z - EnemyPos.z) * (ObjectPos.z - EnemyPos.z));

		if (fDistance <= 2000.0f && m_State != STATE_DEATH && m_State != STATE_DAMAGE)
		{// ���S��ԂłȂ����_���[�W��ԂłȂ�
			// AI�̏���
			AI();

			// �ړ��v�Z����
			Movement();

			// �����蔻�菈��
			Collision();
		}
	}

	// �Q�[�W�̍��W�����炵�Ă���
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}

	if (GetMotionManager() != NULL)
	{// ���[�V�����Ǘ��N���X����������Ă���
		GetMotionManager()->Update(GetModel());
	}

	// �e�̍��W�����炵�Ă���
	if (GetShadow() != NULL)
	{// �e����������Ă���
		GetShadow()->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, GetPos().z));
	}

	// ��ԑJ�ڏ���
	Statement();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// ���ʂ̕`�揈��
	if (m_bDamage == true && m_nDispCounter % 6 == 0 || m_bDamage != true)
	{// �_���[�W��ԂłȂ� �܂��� �_���[�W��Ԃ̎��ɕ`�悷��^�C�~���O�ł���
		CCharacter::Draw();
	}
}

//=============================================================================
//    �ړ��v�Z����
//=============================================================================
void CEnemy::Movement(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();           // ���݂̍��W
	D3DXVECTOR3 rot = GetRot();           // ���݂̌���
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���
	D3DXVECTOR3 DiffRot = GetDiffRot();   // �ړI�̌����ƌ��݂̌����̍���
	float fGravity = GetGravity();        // �d��

	// �d�͂�������
	pos.y += fGravity;

	// �e��l�̐ݒ�
	SetPos(pos);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CEnemy::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRange)
{
	bool bHit = false;

	// �K�v�Ȓl���擾
	D3DXVECTOR3 pos = GetPos();
	float fRange = GetColRange();
	float fDistance = 0.0f;
	float fHitRange = sqrtf((fColRange + fRange) * (fColRange + fRange));

	// �������v�Z
	fDistance = sqrtf((pPos->x - pos.x) * (pPos->x - pos.x) + (pPos->z - pos.z) * (pPos->z - pos.z));

	if (fDistance < fHitRange)
	{// ���a���ɓ����Ă���
		// �x�N�g���ŉ����Ԃ�
		D3DXVECTOR3 VecA = pos - *pPos;
		D3DXVec3Normalize(&VecA, &VecA);
		pPos->x -= VecA.x * (fHitRange - fDistance);
		pPos->z -= VecA.z * (fHitRange - fDistance);

		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    �G��AI����
//=============================================================================
void CEnemy::AI(void)
{
	switch (m_AIType)
	{// ���݂�AI�̏�Ԃɂ���ď����킯
	case AI_TYPE_CHEAK:        // �Ώۂ̃I�u�W�F�N�g�Ƃ̋����𑪂���
		AI_CheakDistance();
		break;
	case AI_TYPE_ACCESS:       // �Ώۂ̃I�u�W�F�N�g�ɋ߂Â����
		AI_ApproachObject();
		break;
	case AI_TYPE_ATTACK:       // �Ώۂ̃I�u�W�F�N�g�ɍU�����d�|������
		AI_AttackObject();
		break;
	case AI_TYPE_RETURN_CHEAK: // �G��AI�����Ԃɖ߂鏈��
		AI_ReturnPatrol();
		break;
	}
}

//=============================================================================
//    �G��AI�Ώۂ̃I�u�W�F�N�g�Ƃ̋����𑪂鏈��
//=============================================================================
void CEnemy::AI_CheakDistance(void)
{
	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
		// �K�v�ȕϐ���錾
		float fDistance = 0.0f;                     // �Ώۂ̃I�u�W�F�N�g�Ƃ̋���
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // �Ώۂ̃I�u�W�F�N�g�̍��W

		// �������v�Z
		fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

		if (fDistance <= m_fAI_CheackDistance)
		{// ���������̒l�ɂȂ���
			// �߂Â��Ă�����Ԃ�
			m_AIType = AI_TYPE_ACCESS;

			// �ړI�̈ʒu���Z�o
			float fAngle = ((rand() % 314) * 0.01f) - D3DX_PI;
			m_AI_DestPos = D3DXVECTOR3(ObjectPos.x + sinf(fAngle) * 60.0f, ObjectPos.y, ObjectPos.z + cosf(fAngle) * 60.0f) - GetPos();

			// �ړ���Ԃɂ���
			m_State = STATE_MOVE;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
		else
		{// �܂��v���C���[���߂��ɂ��Ȃ�
			if (m_bAI_Patrol == true)
			{// �p�g���[���s��������Ȃ�
				if (m_nAI_Counter <= 200)
				{ // �J�E���^�[������̒l�ɒB���Ă��Ȃ�
					m_nAI_Counter++;
					if (m_nAI_Counter > 200)
					{// �J�E���^�[������̒l�ɒB����
						m_nAI_Counter = 0;
						m_bAI_Patrol = false;
						// �ʏ�̏�Ԃɂ���
						m_State = STATE_NORMAL;
						GetMotionManager()->SwitchMotion(GetModel(), m_State);
					}
					else
					{// �J�E���^�[������̒l�ɒB���Ă��Ȃ�
						D3DXVECTOR3 EnemyPos = GetPos();
						D3DXVECTOR3 DestPos = m_AI_DestPos - EnemyPos;
						// �ړ�������
						D3DXVec3Normalize(&DestPos, &DestPos);
						EnemyPos.x += DestPos.x * 2.0f;
						EnemyPos.z += DestPos.z * 2.0f;

						// �l�̐ݒ�
						SetPos(EnemyPos);

						// �p�x���v�Z
						float fAngle = atan2f((m_AI_DestPos.x - GetPos().x), (m_AI_DestPos.z - GetPos().z));
						fAngle += D3DX_PI;
						if (fAngle > D3DX_PI)
						{
							fAngle -= D3DX_PI * 2.0f;
						}

						// �����𒼂�
						SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

						if (EnemyPos.x <= m_AI_DestPos.x + 5.0f && EnemyPos.x >= m_AI_DestPos.x - 5.0f
							&& EnemyPos.z <= m_AI_DestPos.z + 5.0f && EnemyPos.z >= m_AI_DestPos.z - 5.0f)
						{// �ړI�̈ʒu�ɒB����
							m_nAI_Counter = 0;
							m_bAI_Patrol = false;
							// �ʏ�̏�Ԃɂ���
							m_State = STATE_NORMAL;
							GetMotionManager()->SwitchMotion(GetModel(), m_State);
						}
					}
				}
			}
			else
			{// �p�g���[���s�������Ă��Ȃ��Ȃ�
				if (rand() % 150 == 0)
				{// �������ׂ��ꏊ���v�Z����
					float fAngle = ((rand() % 628) * 0.01f) - D3DX_PI;
					float fMoveX = rand() % 150 + 100.0f;
					float fMoveZ = rand() % 150 + 100.0f;
					m_AI_DestPos = D3DXVECTOR3(m_PosStd.x + sinf(fAngle) * fMoveX, 0.0f, m_PosStd.z + cosf(fAngle) * fMoveZ);
					m_nAI_Counter = 0;
					m_bAI_Patrol = true;

					// �p�x���v�Z
					fAngle = atan2f((m_AI_DestPos.x - GetPos().x), (m_AI_DestPos.z - GetPos().z));
					fAngle += D3DX_PI;
					if (fAngle > D3DX_PI)
					{
						fAngle -= D3DX_PI * 2.0f;
					}

					// �����𒼂�
					SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));
					// �ړ���Ԃɂ���
					m_State = STATE_MOVE;
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
			}
		}
	}
}

//=============================================================================
//    �G��AI�Ώۂ̃I�u�W�F�N�g�ɋ߂Â�����
//=============================================================================
void CEnemy::AI_ApproachObject(void)
{
	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
	    // �K�v�ȕϐ���錾
		float fDistance = 0.0f;                       // �Ώۂ̃I�u�W�F�N�g�Ƃ̋���
		float fAngle = 0.0f;                          // �Ώۂ̃I�u�W�F�N�g�Ƃ̊p�x
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();    // �Ώۂ̃I�u�W�F�N�g�̍��W
		D3DXVECTOR3 EnemyPos = GetPos();              // ���g�̍��W
		D3DXVECTOR3 DestPos = ObjectPos;              // �G���������ׂ��ړI�̈ʒu

		// �������ׂ��ꏊ�ւ̃x�N�g�����Z�o
		if (rand() % 10 == 0)
		{// �������ׂ��ꏊ���v�Z����
			fAngle = ((rand() % 314) * 0.01f) - D3DX_PI;
			m_AI_DestPos = D3DXVECTOR3(ObjectPos.x + sinf(fAngle) * 20.0f, ObjectPos.y, ObjectPos.z + cosf(fAngle) * 20.0f) - GetPos();
		}

		// �������v�Z
		fDistance = sqrtf((ObjectPos.x - EnemyPos.x) * (ObjectPos.x - EnemyPos.x) + (ObjectPos.y - EnemyPos.y) * (ObjectPos.y - EnemyPos.y) + (ObjectPos.z - EnemyPos.z) * (ObjectPos.z - EnemyPos.z));

		// �p�x���v�Z
		fAngle = atan2f((EnemyPos.x - ObjectPos.x), (EnemyPos.z - ObjectPos.z));

		// �����𒼂�
		SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

		if (fDistance <= m_fAI_AttackDistance)
		{// ���������̒l�ɂȂ���
			if (m_State != STATE_NORMAL)
			{// �ʏ��ԂłȂ�
				// �ʏ��Ԃɂ���
				m_State = STATE_NORMAL;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}

			if (rand() % m_nAI_AttackTiming == 0)
			{// �U�����d�|����^�C�~���O���Z�o���ꂽ
				// �U�����d�|�����Ԃ�
				m_AIType = AI_TYPE_ATTACK;

				// �U����Ԃɂ���
				m_State = STATE_ACTION;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
		else
		{// �U�����d�|����^�C�~���O�ł͂Ȃ�
			// �ړ�������
			D3DXVec3Normalize(&m_AI_DestPos, &m_AI_DestPos);
			EnemyPos.x += m_AI_DestPos.x * 3.0f;
			EnemyPos.z += m_AI_DestPos.z * 3.0f;

			// �l�̐ݒ�
			SetPos(EnemyPos);

			if (fDistance >= m_fAI_AttackDistance && fDistance <= m_fAI_CheackDistance)
			{// �U���͈͂��瓦����ꂽ
				m_AIType = AI_TYPE_ACCESS;	// �߂Â��Ă�����Ԃ�
				if (m_State != STATE_MOVE)
				{// �ړ���ԂłȂ�
					m_State = STATE_MOVE;   // �ړ���Ԃɂ���
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
			}
			else if (fDistance >= m_fAI_CheackDistance)
			{// ���������̒l�𒴂���
				// �p�g���[���s���ɖ߂��Ԃ�
				m_AIType = AI_TYPE_RETURN_CHEAK;

				// �ړ���Ԃɂ���
				m_State = STATE_MOVE;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
}

//=============================================================================
//    �G��AI�Ώۂ̃I�u�W�F�N�g�ɍU�����d�|���鏈��
//=============================================================================
void CEnemy::AI_AttackObject(void)
{
	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	if (pPlayer != NULL)
	{// �v���C���[�N���X���擾�ł���
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			// �K�v�ȕϐ���錾
			float fDistance = 0.0f;                       // �Ώۂ̃I�u�W�F�N�g�Ƃ̋���
			D3DXVECTOR3 ObjectPos = pPlayer->GetPos();    // �Ώۂ̃I�u�W�F�N�g�̍��W

			// �������v�Z
			fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));
			if (fDistance <= m_fAI_AttackDistance)
			{// �������U�����d�|���鋗��������
				if (rand() % m_nAI_AttackTiming == 0)
				{// �U�����d�|����^�C�~���O���Z�o���ꂽ
					m_AIType = AI_TYPE_ATTACK;  // �U�����d�|�����Ԃ�
					m_State = STATE_ACTION;	    // �U����Ԃɂ���
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
				else
				{// �U�����d�|����^�C�~���O���Z�o����Ȃ�����
					m_AIType = AI_TYPE_CHEAK;	// �����Ԃ�
					m_State = STATE_NORMAL;     // �ʏ��Ԃɂ���
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
			}
			else if (fDistance <= m_fAI_CheackDistance)
			{// ���������̒l�ɂȂ���
				m_AIType = AI_TYPE_ACCESS;	// �߂Â��Ă�����Ԃ�
				m_State = STATE_MOVE;       // �ړ���Ԃɂ���
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
			else
			{// �������鋗�����痣��Ă���
			    // �p�g���[���s���ɖ߂��Ԃ�
				m_AIType = AI_TYPE_RETURN_CHEAK;
				m_State = STATE_MOVE;     // �ʏ��Ԃɂ���
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}

			// ���[�V������؂�ւ���
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}
}

//=============================================================================
//    �G��AI�����Ԃɖ߂鏈��
//=============================================================================
void CEnemy::AI_ReturnPatrol(void)
{
	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
	    // �K�v�ȕϐ���錾
		float fDistance = 0.0f;                     // �Ώۂ̃I�u�W�F�N�g�Ƃ̋���
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // �Ώۂ̃I�u�W�F�N�g�̍��W

		// �������v�Z
		fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

		if (fDistance <= m_fAI_CheackDistance)
		{// ���������̒l�ɂȂ���
		    // �߂Â��Ă�����Ԃ�
			m_AIType = AI_TYPE_ACCESS;

			// �ړI�̈ʒu���Z�o
			float fAngle = ((rand() % 314) * 0.01f) - D3DX_PI;
			m_AI_DestPos = D3DXVECTOR3(ObjectPos.x + sinf(fAngle) * 60.0f, ObjectPos.y, ObjectPos.z + cosf(fAngle) * 60.0f) - GetPos();

			// �ړ���Ԃɂ���
			m_State = STATE_MOVE;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
		else
		{// �v���C���[���߂��ɂ��Ȃ�
			D3DXVECTOR3 EnemyPos = GetPos();

			// �ڕW�ʒu���v�Z
			m_AI_DestPos = m_PosStd - EnemyPos;

			// �p�x���v�Z
			float fAngle = atan2f((EnemyPos.x - m_PosStd.x), (EnemyPos.z - m_PosStd.z));

			// �����𒼂�
			SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

			// �ړ�������
			D3DXVec3Normalize(&m_AI_DestPos, &m_AI_DestPos);
			EnemyPos.x += m_AI_DestPos.x * 2.0f;
			EnemyPos.z += m_AI_DestPos.z * 2.0f;

			// �l�̐ݒ�
			SetPos(EnemyPos);

			if (EnemyPos.x <= m_PosStd.x + 5.0f && EnemyPos.x >= m_PosStd.x - 5.0f
				&& EnemyPos.z <= m_PosStd.z + 5.0f && EnemyPos.z >= m_PosStd.z - 5.0f)
			{
				m_AIType = AI_TYPE_CHEAK;	// �����Ԃ�
				m_State = STATE_NORMAL;     // �ʏ��Ԃɂ���
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
}

//=============================================================================
//    �G�̃_���[�W����
//=============================================================================
bool CEnemy::Hit(int nDamage)
{
	bool bDeath = false;  // ���񂾂��ǂ���

	// �̗͂����炷
	m_nLife -= nDamage;

	// �U����������������ɂ���
	m_bDamage = true;

	// �̗̓Q�[�W�����炷
	if (m_pGaugeLife != NULL)
	{// ���������m�ۂ���Ă���
		m_pGaugeLife->CutGauge(nDamage);
	}

	if (m_nLife <= 0)
	{// �̗͂�0�ȉ��ɂȂ���
		Death();

		// �X�R�A�N���X���擾���A�X�R�A�����Z����
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CScore *pScore = NULL;
		pScene = GetTop(UI_SCORE_PRIORITY);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_SCORE)
			{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������
				pScore = (CScore*)pScene;
				if (pScore != NULL)
				{// �X�R�A�N���X���擾�ł���
					pScore->AddScore(m_nAddScore);
				}
			}
			pScene = pSceneNext;
		}

		// ���񂾔����
		bDeath = true;
	}
	else
	{// �܂��̗͂�����
		m_State = STATE_DAMAGE;
		GetMotionManager()->SwitchMotion(GetModel(), m_State);
	}

	return bDeath;
}

//=============================================================================
//    �G�����񂾂Ƃ��̏���
//=============================================================================
void CEnemy::Death(void)
{
	// ��Ԃƃ��[�V������ς���
	m_State = STATE_DEATH;
	GetMotionManager()->SwitchMotion(GetModel(), m_State);

	// �v���C���[�̃��b�N�I���ΏۂɂȂ��Ă��Ȃ����`�F�b�N
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
		if (pPlayer != NULL)
		{// �v���C���[�N���X���擾�ł���
			if (pPlayer->GetLockOnEnemy() == this)
			{// ���b�N�I���Ώۂ�����������
			    // ���b�N�I������������
				pPlayer->LockLost();
			}
		}
	}
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
void CEnemy::Collision(void)
{
	bool bLand = false;   // ����Ă��邩�ǂ���

	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();          // ���݂̍��W

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	CMeshWall *pWall = NULL;             // �ǃN���X�ւ̃|�C���^
	CEnemy *pEnemy = NULL;               // �G�N���X�ւ̃|�C���^
	bool bFieldLand = false;              // �n�ʂ̒��ɂ��邩�ǂ���
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (bLand != true)
			{// �܂��I�u�W�F�N�g�ɏ���Ă��Ȃ�
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// �n�ʃN���X�ւ̃|�C���^��������
					if (bFieldLand == false)
					{// �܂��n�ʂɏ���Ă��Ȃ�
						pField = (CMeshField*)pScene;
						if (pField != NULL)
						{// �n�ʂ��擾�ł���
							D3DXVECTOR3 FieldNormal;
							float fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, &FieldNormal);
							if (bFieldLand == true)
							{// �n�ʂɏ���Ă���
								if (pos.y < fFieldHeight)
								{// ���W���n�ʂ�艺�ɂȂ���
									m_Move.y = 0.0f;
									pos.y = fFieldHeight;
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// �z�u����������
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// �z�u�����擾�ł���
						if (pObject->Collision(&pos, &GetPosOld(), &m_Move, GetColHeight(), GetColRange(), this) == true)
						{// �z�u���̏�ɏ���Ă���
							if (GetLand() == false)
							{// ���n���Ă��Ȃ�������
								SetLand(true);
							}

							if (GetLandScene() == NULL && GetLand() == true)
							{// �����ɏ���Ă���
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_MESHWALL)
				{// �ǃN���X�ւ̃|�C���^��������
					pWall = (CMeshWall*)pScene;
					if (pWall != NULL)
					{// �ǂ��擾�ł���
						if (pWall->Collision(&pos, &GetPosOld(), &D3DXVECTOR3(), D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange())))
						{// �ǂɓ������Ă���

						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// �G��������
					pEnemy = (CEnemy*)pScene;
					if (pEnemy != NULL)
					{// �G�N���X�ւ̃|�C���^���擾�ł���
						if (pEnemy->Collision(&pos, &GetPosOld(), &m_Move, GetColRange()))
						{// �G�ɓ������Ă���
						}
					}
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);         // ���݂̍��W
	SetLand(bLand);      // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
}

//=============================================================================
//    �U���̓����蔻�菈��
//=============================================================================
bool CEnemy::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage)
{
	bool bHit = false;                 // �U�����q�b�g�������ǂ���
	float fPlayerRange = 0.0f;         // �v���C���[�̓����蔻������͈�
	D3DXVECTOR3 OffSet = Offset;       // �����蔻����Ƃ�ꏊ(�e���f������̃I�t�Z�b�g)
	float fHitRange = 0.0f;            // �����蔻�����钷��
	float fLength = 0.0f;              // �����蔻������ꏊ�ƓG�Ƃ̋���
	D3DXMATRIX mtxWorld;

	// �����蔻��������W���m�肳����
	if (nModelIdx != -1)
	{// ���f�����瓖���蔻��𔭐�������
		CModel *pModel = GetModel(nModelIdx);
		if (pModel != NULL)
		{
			mtxWorld = pModel->GetMtxWorld();
		}
	}
	else
	{// ���f�����瓖���蔻��𔭐������Ȃ�
		mtxWorld = GetMtxWorld();
	}
	D3DXVec3TransformCoord(&OffSet, &OffSet, &mtxWorld);

	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	// ����J�n
	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
	    // �v���C���[�̓����蔻������͈͂��擾
		fPlayerRange = pPlayer->GetColRange();

		// �����蔻�����钷�����v�Z
		fHitRange = sqrtf((fPlayerRange + fRange) * (fPlayerRange + fRange));

		// �����蔻��������W�ƓG�̍��W�Ƃ̋����𑪂�
		D3DXMATRIX PlayerMtxWorld;
		D3DXVECTOR3 PlayerPos;
		CModel *pModel = pPlayer->GetModel(0);
		if (pModel != NULL)
		{
			PlayerMtxWorld = pModel->GetMtxWorld();
			PlayerPos = D3DXVECTOR3(PlayerMtxWorld._41, PlayerMtxWorld._42, PlayerMtxWorld._43);
			fLength = sqrtf((PlayerPos.x - OffSet.x) * (PlayerPos.x - OffSet.x) + (PlayerPos.y - OffSet.y) * (PlayerPos.y - OffSet.y) + (PlayerPos.z - OffSet.z) * (PlayerPos.z - OffSet.z));
		}

		if (fLength <= fHitRange)
		{// �����蔻��������W�ƓG�̍��W�Ƃ̋����������蔻�����钷�����Z��
			if (pPlayer->GetState() != CPlayer::STATE_DAMAGE)
			{// �v���C���[�̏�Ԃ��_���[�W��ԂłȂ�
				pPlayer->Damage(nDamage);
				bHit = true;
			}
			CDebugProc::Print("�U�����q�b�g���܂���\n");
		}
	}

	return bHit;  // �q�b�g�������ǂ�����Ԃ�
}

//=============================================================================
//    ��ԑJ�ڏ���
//=============================================================================
void CEnemy::Statement(void)
{
	if (m_State == STATE_DAMAGE)
	{// �_���[�W��Ԃ�������
		m_nDispCounter++;
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			m_State = STATE_NORMAL;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);

			m_AIType = AI_TYPE_CHEAK;
			m_nDispCounter = 0;

			// �U�����������������߂�
			m_bDamage = false;
		}
	}
	else if (m_State == STATE_DEATH)
	{// ���S��Ԃ�������
		if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			// �G�t�F�N�g���o��
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// �Q�[������������
				CEffectManager *pEffectManager = CManager::GetGame()->GetEffectManager();
				if (pEffectManager != NULL)
				{// �|�C���^���擾�ł���
					pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17);
					pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 18);
					pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 19);
				}

				// �X�R�A�p�[�e�B�N�����o��
				CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
				if (pPlayer != NULL)
				{// �v���C���[���擾�ł���
					float fAngle = 0.0f;
					D3DXVECTOR3 ParPos;
					D3DXVECTOR3 Move;
					CScorePar *pScorePar = NULL;
					ParPos = D3DXVECTOR3(GetPos().x, GetPos().y + 40.0f, GetPos().z);
					for (int nCntPar = 0; nCntPar < 10; nCntPar++)
					{// �X�R�A�p�[�e�B�N�����o�������J��Ԃ�
						fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
						Move.x = sinf(fAngle) * ((rand() % 5) + 3.0f);
						Move.y = ((rand() % 10) + 10.0f);
						Move.z = cosf(fAngle) * ((rand() % 5) + 3.0f);
						pScorePar = CScorePar::Create(GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 8.0f, 8.0f, Move, pPlayer, 100, EFFECT_PRIORITY);
						if (pScorePar != NULL)
						{
							pScorePar->BindTexture(pEffectManager->GetTexManager()->GetTexture(5));
						}
					}
				}
			}
			// SE���Đ�
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DEATH);

			// �G������
			Uninit();
		}
	}
	else if (m_State == STATE_ACTION)
	{// �U����Ԃ�������
		if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
		{// �����蔻��f�[�^�N���X����������Ă���
			for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
			{// �����蔻��f�[�^�̐������J��Ԃ�
				// �����蔻��ɕK�v�ȃf�[�^��錾
				int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();
				float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();
				D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();
				int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();
				int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();
				int nDamage = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetDamage();

				// ����J�n
				if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
				{// �����蔻��𔭐�������^�C�~���O�Ȃ��
					// �U�����̓����蔻��𔭐�������
					AttackCollision(nModelIdx, fRange, Offset, nDamage);
				}

				// SE���Đ�
				if (GetMotionManager()->GetAllCounter() == nStartTiming)
				{// �Đ�����^�C�~���O�ɂȂ���
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_SWING);
				}
			}
		}
	}
}

//=============================================================================
//    ��ނ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �ړ��ʂ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetMove(const D3DXVECTOR3 Move)
{
	m_Move = Move;
}

//=============================================================================
//    �̗͂�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �|�������ɑ�����X�R�A�ʂ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAddScore(const int nAddScore)
{
	m_nAddScore = nAddScore;
}

//=============================================================================
//    AI�̎�ނ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAI_Type(const AI_TYPE AI_Type)
{
	m_AIType = AI_Type;
}

//=============================================================================
//    �x���ł���͈͂�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAI_CheackDistance(const float fDistance)
{
	m_fAI_CheackDistance = fDistance;
}

//=============================================================================
//    �U�����d�|����͈͂�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAI_AttackDistance(const float fDistance)
{
	m_fAI_AttackDistance = fDistance;
}

//=============================================================================
//    �U�����d�|����^�C�~���O��ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAI_AttackTiming(const int nTiming)
{
	m_nAI_AttackTiming = nTiming;
}

//=============================================================================
//    �ړI�̈ʒu��ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAI_DestPos(const D3DXVECTOR3 AI_DestPos)
{
	m_AI_DestPos = AI_DestPos;
}

//=============================================================================
//    �`����Ǘ�����J�E���^�[��ݒ肷�鏈��
//=============================================================================
void CEnemy::SetDispCounter(const int nDispCounter)
{
	m_nDispCounter = nDispCounter;
}

//=============================================================================
//    �̗̓Q�[�W�N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEnemy::SetGaugeLife(CGaugeLife *pGaugeLife)
{
	m_pGaugeLife = pGaugeLife;
}

//=============================================================================
//    �������̍��W��ݒ肷�鏈��
//=============================================================================
void CEnemy::SetPosStd(const D3DXVECTOR3 posStd)
{
	m_PosStd = posStd;
}

//=============================================================================
//    �������̌�����ݒ肷�鏈��
//=============================================================================
void CEnemy::SetRotStd(const D3DXVECTOR3 rotStd)
{
	m_RotStd = rotStd;
}

//=============================================================================
//    �_���[�W��������Ă��邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CEnemy::SetDamage(const bool bDamage)
{
	m_bDamage = bDamage;
}

//=============================================================================
//    ��ނ��擾���鏈��
//=============================================================================
CEnemy::TYPE CEnemy::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CEnemy::STATE CEnemy::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �ړ��ʂ��擾���鏈��
//=============================================================================
D3DXVECTOR3 CEnemy::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �̗͂��擾���鏈��
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    �|�������ɑ�����X�R�A�ʂ��擾���鏈��
//=============================================================================
int CEnemy::GetAddScore(void)
{
	return m_nAddScore;
}

//=============================================================================
//    AI�̎�ނ��擾���鏈��
//=============================================================================
CEnemy::AI_TYPE CEnemy::GetAI_Type(void)
{
	return m_AIType;
}

//=============================================================================
//    �x������͈͂��擾���鏈��
//=============================================================================
float CEnemy::GetAI_CheackDistance(void)
{
	return m_fAI_CheackDistance;
}

//=============================================================================
//    �U�����d�|����͈͂��擾���鏈��
//=============================================================================
float CEnemy::GetAI_AttackDistance(void)
{
	return m_fAI_AttackDistance;
}

//=============================================================================
//    �U�����d�|����^�C�~���O���擾���鏈��
//=============================================================================
int CEnemy::GetAI_AttackTiming(void)
{
	return m_nAI_AttackTiming;
}

//=============================================================================
//    �ړI�̈ʒu���擾���鏈��
//=============================================================================
D3DXVECTOR3 CEnemy::GetAI_DestPos(void)
{
	return m_AI_DestPos;
}

//=============================================================================
//    �`����Ǘ�����J�E���^�[���擾���鏈��
//=============================================================================
int CEnemy::GetDispCounter(void)
{
	return m_nDispCounter;
}

//=============================================================================
//    �̗̓Q�[�W�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CGaugeLife *CEnemy::GetGaugeLife(void)
{
	return m_pGaugeLife;
}

//=============================================================================
//    �������̍��W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CEnemy::GetPosStd(void)
{
	return m_PosStd;
}

//=============================================================================
//    �������̌������擾���鏈��
//=============================================================================
D3DXVECTOR3 CEnemy::GetRotStd(void)
{
	return m_RotStd;
}

//=============================================================================
//    �_���[�W��������Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CEnemy::GetDamage(void)
{
	return m_bDamage;
}

//*****************************************************************************
//     CSetEnemy�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSetEnemy::CSetEnemy(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSetEnemy::~CSetEnemy()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSetEnemy *CSetEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore, int nPriority)
{
	CSetEnemy *pSetEnemy = NULL;           // �z�u�p�G�l�~�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pSetEnemy == NULL)
		{// ����������ɂȂ��Ă���
			pSetEnemy = new CSetEnemy(nPriority);
			if (pSetEnemy != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pSetEnemy->Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nLife, nAddScore)))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pSetEnemy;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSetEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// ���ʂ̏���������
	if (FAILED(CEnemy::Init(pos, rot, type, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSetEnemy::Uninit(void)
{
	// ���ʂ̏I������
	CEnemy::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSetEnemy::Update(void)
{
	// �Q�[�W�̍��W�����炵�Ă���
	CGaugeLife *pGaugeLife = GetGaugeLife();
	if (pGaugeLife != NULL)
	{
		pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSetEnemy::Draw(void)
{
	// �����x��������
	SetModelAlpha(0.7f);

	// ���ʂ̕`�揈��
	CEnemy::Draw();

	// �����x�����ɖ߂��Ă���
	SetModelAlpha(1.0f);
}

//*****************************************************************************
//    CEnemyAssembly�̏���
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemyAssembly::CEnemyAssembly()
{
	// �e��l�̃N���A
	for (int nCntEnemyType = 0; nCntEnemyType < MAX_ENEMY_TYPE; nCntEnemyType++)
	{// �ǂݍ��߂�G�̎�ނ̐������J��Ԃ�
		m_pEnemyManager[nCntEnemyType] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemyAssembly::~CEnemyAssembly()
{

}


//=============================================================================
//    �G�Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEnemyAssembly::SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx)
{
	m_pEnemyManager[nIdx] = pEnemyManager;
}

//=============================================================================
//    �G�Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEnemyManager *CEnemyAssembly::GetEnemyManager(int nIdx)
{
	return m_pEnemyManager[nIdx];
}

//*****************************************************************************
//     CBoss�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBoss::CBoss(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{
	// �e��l�̃N���A
	m_pOrbitEffect = NULL;              // �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	m_pEffectManager = NULL;            // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_AIType_Boss = AI_TYPE_BOSS_CHECK; // �{�X��AI�̏��
	m_nBackStep = 0;                    // �o�b�N�X�e�b�v�𓥂񂾉�(�U���s����������0�Ƀ��Z�b�g)
	m_nAttack = 0;                      // �U�����d�|������(�o�b�N�X�e�b�v��ԂɈڂ����烊�Z�b�g)
	m_nStep = 0;                        // �X�e�b�v�𓥂񂾉�(�O�Ք�΂��������烊�Z�b�g)
	m_nSkipOver = 0;                    // �O�Ք�΂���������(�X�e�b�v�𓥂񂾂烊�Z�b�g)
	m_nBackStepCounter = 0;             // �o�b�N�X�e�b�v�𓥂񂾉�(�o�b�N�X�e�b�v�͕K��2�񓥂܂��邽��)
	m_nTiredCounter = 0;                // ����ԂɂȂ��Ă���̎��Ԃ𐔂���J�E���^�[
	m_nSkipOverOrbitCounter = 0;        // �O�Ք�΂���ԂɂȂ��Ă���̎��Ԃ𐔂���J�E���^�[
	m_nDamageCounter = 0;               // �_���[�W��������Ă���̎���
	m_fRedAdd[0] = 0.0f;                // ���f���̐F��ς���l
	m_fRedAdd[1] = 0.0f;                // ���f���̐F��ς���l
	m_nRedCounter = 0;                  // ���f���̐F�ύX���Ǘ�����J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBoss::~CBoss()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBoss *CBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore, int nPriority)
{
	CBoss *pEnemy = NULL;                 // �G�l�~�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEnemy == NULL)
		{// ����������ɂȂ��Ă���
			pEnemy = new CBoss(nPriority);
			if (pEnemy != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pEnemy->Init(pos, rot, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nAddScore)))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pEnemy;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBoss::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nAddScore)
{
	// �e��l�̐ݒ�
	SetPos(pos);                               // ���݂̍��W
	SetRot(rot);                               // ���݂̌���
	SetNumParts(nNumParts);                    // �p�[�c��
	SetType(TYPE_BOSS);                        // ���
	SetModel(apModel);                         // ���f���N���X�ւ̃|�C���^
	SetMotionManager(pMotionManager);          // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	SetGravity(fGravity);                      // �d��
	SetColHeight(fColHeight);                  // �����蔻�����鍂��
	SetColRange(fColRange);                    // �����蔻������͈�
	SetAI_CheackDistance(fAI_CheackDistance);  // �x���ł���͈�
	SetAI_AttackDistance(fAI_AttackDistance);  // �U���ł���͈�
	SetAI_AttackTiming(nAI_AttackTiming);      // �U�����d�|����^�C�~���O
	SetLife(nLife);                            // �̗�
	SetAddScore(nAddScore);                    // �|�������ɑ�����X�R�A��
	SetPosStd(pos);                            // �������̍��W
	SetRotStd(rot);                            // �������̌���

	// �e�N���X�ւ̃|�C���^����肵�Ă���
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_EFFECTMANAGER)
			{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������
				m_pEffectManager = (CEffectManager*)pScene;
			}
			pScene = pSceneNext;
		}
	}

	// �̗̓Q�[�W�����
	CGaugeLife *pGaugeLife = NULL;
	if (pGaugeLife == NULL)
	{
		pGaugeLife = CGaugeLife::Create(ENEMY_GAUGE_POS + pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), ENEMY_GAUGE_WIDTH, ENEMY_GAUGE_HEIGHT, 0.0f, nLife);
		SetGaugeLife(pGaugeLife);
	}

	// ���f���̈ʒu���j���[�g�����̈ʒu�ɂ���
	SetDefaultPos();

	// ���f���̌������j���[�g�����̌����ɂ���
	SetDefaultRot();

	// �f�t�H���g�̐F���擾����
	CModel **pModel = GetModel();
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATERIAL *pMat;
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// �p�[�c�������J��Ԃ�
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// �|�C���^���擾�ł���
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// �}�e���A�����̐������J��Ԃ�
				m_ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}
	}

	m_fRedAdd[0] = BOSS_RED_CHANGE_0;                // ���f���̐F��ς���l
	m_fRedAdd[1] = BOSS_RED_CHANGE_1;                // ���f���̐F��ς���l

	// �e���쐬����
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.2f, 1.2f, 1.2f));
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBoss::Uninit(void)
{
	// �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^���J������
	if (m_pOrbitEffect != NULL)
	{// ���������m�ۂ���Ă���
		m_pOrbitEffect->Uninit();
		m_pOrbitEffect = NULL;
	}

	// ���ʂ̏I������
	CEnemy::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBoss::Update(void)
{
	// �O��̍��W��ۑ�
	SetPosOld(GetPos());

	// �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^��ۑ�
	SetLandSceneOld(GetLandScene());

	// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
	SetLandScene(NULL);

	if (GetState() != STATE_DEATH && GetState() != STATE_DAMAGE && CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
	{// ���S��ԂłȂ����_���[�W��ԂłȂ����Q�[�����ʏ��Ԃł���
	    // AI�̏���
		AI();
	}

	// �ړ��v�Z����
	Movement();

	// �����蔻�菈��
	Collision();

	// ��ԑJ�ڏ���
	Statement();

	if (m_AIType_Boss == AI_TYPE_BOSS_TIRED)
	{// ����AI�̏�Ԃ�������
		m_nTiredCounter++;
	}

	if (GetDamage() == true)
	{// �_���[�W��������Ă�����
		m_nDamageCounter++;
		if (m_nDamageCounter >= 80)
		{// �_���[�W��ԂɂȂ��Ă����莞�Ԃ�������
			m_nDamageCounter = 0;
			SetDamage(false);
		}
	}

	// �Q�[�W�̍��W�����炵�Ă���
	CGaugeLife *pGaugeLife = GetGaugeLife();
	if (pGaugeLife != NULL)
	{
		pGaugeLife->SetGaugePos(ENEMY_GAUGE_POS + GetPos());
	}

	if (GetMotionManager() != NULL)
	{// ���[�V�����Ǘ��N���X����������Ă���
		GetMotionManager()->Update(GetModel());
	}

	// �e�̍��W�����炵�Ă���
	if (GetShadow() != NULL)
	{// �e����������Ă���
		GetShadow()->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, GetPos().z));
	}

	if (GetState() == STATE_DEATH)
	{// ���S��Ԃ�������
		DeathState();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBoss::Draw(void)
{
	// �̗͂��Ⴂ�Ƃ��͐Ԃ��_�ł�����
	DeathWarning();

	// ���ʂ̕`�揈��
	if (GetState() == STATE_DAMAGE && GetDispCounter() % 6 == 0 || GetState() != STATE_DAMAGE)
	{// �_���[�W��ԂłȂ� �܂��� �_���[�W��Ԃ̎��ɕ`�悷��^�C�~���O�ł���
		CCharacter::Draw();
	}
}

//=============================================================================
//    �ړ��v�Z����
//=============================================================================
void CBoss::Movement(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();           // ���݂̍��W
	D3DXVECTOR3 Move = GetMove();         // ���݂̈ړ���
	D3DXVECTOR3 rot = GetRot();           // ���݂̌���
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���
	D3DXVECTOR3 DiffRot = GetDiffRot();   // �ړI�̌����ƌ��݂̌����̍���
	float fGravity = GetGravity();        // �d��

	// �ړ��ʂ�������
	pos += Move;

	// �d�͂�������
	pos.y += fGravity;

	// �ړ��ʂɊ�����������
	Move.x += (0.0f - Move.x) * BOSS_INERTIA;
	Move.z += (0.0f - Move.z) * BOSS_INERTIA;

	// �e��l�̐ݒ�
	SetPos(pos);
	SetMove(Move);
}

//=============================================================================
//    �{�X��AI����
//=============================================================================
void CBoss::AI(void)
{
	switch (m_AIType_Boss)
	{// ���݂�AI�̏�Ԃɂ���ď����킯
	case AI_TYPE_BOSS_CHECK:              // �Ώۂ̃I�u�W�F�N�g�Ƃ̋����𑪂���
		AI_CheakDistance();
		break;
	case AI_TYPE_BOSS_ATTACK:             // �U�����d�|������
		AI_Attack();
		break;
	case AI_TYPE_BOSS_SKIPOVER_ORBIT:     // �O�Ղ��΂��čU��������
		AI_SkipOverOrbit();
		break;
	case AI_TYPE_BOSS_TIRED:              // �O�Ղ��΂��Ĕ��Ă��܂������
		AI_Tired();
		break;
	case AI_TYPE_BOSS_STEP:               // �X�e�b�v�𓥂�ŋ߂Â����
		AI_Step();
		break;
	case AI_TYPE_BOSS_BACKSTEP:           // �o�b�N�X�e�b�v�œ�������
		AI_BackStep();
		break;
	}
}

//=============================================================================
//    �{�X�̃v���C���[�Ƃ̋����𑪂鏈��
//=============================================================================
void CBoss::AI_CheakDistance(void)
{
	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
	    // �K�v�ȕϐ���錾
		float fDistance = 0.0f;                     // �Ώۂ̃I�u�W�F�N�g�Ƃ̋���
		D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // �Ώۂ̃I�u�W�F�N�g�̍��W

		// �������v�Z
		fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

		// �s���̂��߂Ɍ�����␳
		float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
		fAngle += D3DX_PI;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI *2.0f;
		}
		SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));

		if (fDistance <= BOSS_AI_NEAR_DISTANCE)
		{// ���������̒l�ɂȂ���
			int nBehavior = rand() % 3 + (-m_nAttack + m_nBackStep) - 1;
			if (nBehavior >= 0)
			{// �U������s���ԍ�������
				// �U�������񐔂𑝂₷
				m_nAttack++;

				// �o�b�N�X�e�b�v��A���œ��񂾉񐔂����Z�b�g
				m_nBackStep = 0;

			    // AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_ATTACK;

				// �ʏ��Ԃɂ���
				SetState(STATE_BOSS_ATTACK_0);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (nBehavior < 0)
			{// �o�b�N�X�e�b�v�𓥂ޔԍ�������
			    // �o�b�N�X�e�b�v�𓥂񂾉񐔂𑝂₷
				m_nBackStep++;

				// �U����A���Ŏd�|�����񐔂����Z�b�g
				m_nAttack = 0;

				// AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_BACKSTEP;

				// �o�b�N�X�e�b�v��Ԃɂ���
				SetState(STATE_BOSS_BACKSTEP);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
			}
		}
		else
		{// �܂��v���C���[���߂��ɂ��Ȃ�
			int nBehavior = rand() % 2 + (-m_nSkipOver + m_nStep) - 1;
			if (nBehavior >= 0)
			{// �O�Ղ��΂��s���ԍ�������
			    // �O�Ղ��΂����񐔂𑝂₷
				m_nSkipOver++;

				// �X�e�b�v��A���œ��񂾉񐔂����Z�b�g
				m_nStep = 0;

				// AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_SKIPOVER_ORBIT;

				// �O�Ղ��΂���Ԃɂ���
				SetState(STATE_BOSS_SKIPOVER);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (nBehavior < 0)
			{// �X�e�b�v�𓥂ޔԍ�������
			    // �X�e�b�v�𓥂񂾉񐔂𑝂₷
				m_nStep++;

				// �O�Ղ�A���Ŕ�΂����񐔂����Z�b�g
				m_nSkipOver = 0;

				// AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_STEP;

				// �o�b�N�X�e�b�v��Ԃɂ���
				SetState(STATE_BOSS_STEP);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
			}
		}
	}
}

//=============================================================================
//    �{�X�̍U�����d�|���鏈������
//=============================================================================
void CBoss::AI_Attack(void)
{
	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	if (GetState() != STATE_BOSS_ATTACK_3)
	{// �Ō�̍U�����[�V��������Ȃ�����
		if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			// �U������������v���C���[�Ɍ�����
			if (pPlayer != NULL)
			{// �v���C���[�N���X���擾�ł��Ă���
				float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
				fAngle += D3DX_PI;
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI *2.0f;
				}
				SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));
			}

			// ���[�V������؂�ւ���
			if (GetState() == STATE_BOSS_ATTACK_0)
			{// 1�ڂ̍U�����[�V������������
				SetState(STATE_BOSS_ATTACK_1);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (GetState() == STATE_BOSS_ATTACK_1)
			{// 2�ڂ̍U�����[�V������������
				SetState(STATE_BOSS_ATTACK_2);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else if (GetState() == STATE_BOSS_ATTACK_2)
			{// 3�ڂ̍U�����[�V������������
				SetState(STATE_BOSS_ATTACK_3);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
		}
	}
	else
	{// �Ō�̍U�����[�V������������
		if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			int nBehavior = rand() % 2;
			if (nBehavior == 0)
			{
				// AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_TIRED;

				// ��ꂽ��Ԃɂ���
				SetState(STATE_BOSS_TIRED);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else
			{
				// AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_CHECK;

				// �ʏ��Ԃɂ���
				SetState(STATE_NORMAL);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
		}
	}
}

//=============================================================================
//    �{�X�̃o�b�N�X�e�b�v�œ����鏈��
//=============================================================================
void CBoss::AI_BackStep(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 Move = GetMove();

	// �v�Z���Ă����������Ƃ͋t�����Ɉړ��ʂ�������
	Move.x += sinf(GetRot().y) * BOSS_STEP_MOVE;
	Move.z += cosf(GetRot().y) * BOSS_STEP_MOVE;

	// �e��l�̐ݒ�
	SetMove(Move);

	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// ���[�V�����Đ����I������
		if (m_nBackStepCounter == 0)
		{// �܂��o�b�N�X�e�b�v����������ł��Ȃ�����
			m_nBackStepCounter++;

			// �o�b�N�X�e�b�v��Ԃɂ���
			SetState(STATE_BOSS_BACKSTEP);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());

			// SE���Đ�
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
		}
		else
		{// ���łɃo�b�N�X�e�b�v��1�񓥂�ł���
			// �J�E���^�[��߂�
			m_nBackStepCounter = 0;

			// AI�̎�ނ�ς���
			m_AIType_Boss = AI_TYPE_BOSS_SKIPOVER_ORBIT;

		    // �O�Ք�΂���Ԃɂ���
			SetState(STATE_BOSS_SKIPOVER);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}
	}
}

//=============================================================================
//    �{�X�̃X�e�b�v�𓥂�ŋ߂Â�����
//=============================================================================
void CBoss::AI_Step(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 Move = GetMove();

	// �v�Z���Ă����������ɍ��킹�Ĉړ��ʂ�������
	Move.x += sinf(GetRot().y - D3DX_PI) * BOSS_STEP_MOVE;
	Move.z += cosf(GetRot().y - D3DX_PI) * BOSS_STEP_MOVE;

	// �e��l�̐ݒ�
	SetMove(Move);

	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// ���[�V�����Đ����I������
	    // �v���C���[�N���X���擾����
		CPlayer *pPlayer = NULL;
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[����ʂ�������
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// �Q�[���N���X���擾�ł���
				pPlayer = pGame->GetPlayer();
			}
		}

		if (pPlayer != NULL)
		{// �v���C���[���擾�ł���
			float fDistance = 0.0f;                     // �Ώۂ̃I�u�W�F�N�g�Ƃ̋���
			D3DXVECTOR3 ObjectPos = pPlayer->GetPos();  // �Ώۂ̃I�u�W�F�N�g�̍��W

			// �������v�Z
			fDistance = sqrtf((ObjectPos.x - GetPos().x) * (ObjectPos.x - GetPos().x) + (ObjectPos.y - GetPos().y) * (ObjectPos.y - GetPos().y) + (ObjectPos.z - GetPos().z) * (ObjectPos.z - GetPos().z));

			if (fDistance <= BOSS_AI_ATTACK_DISTANCE)
			{// �U�����d�|�����鋗���ɂ���
			    // �U�������񐔂𑝂₷
				m_nAttack++;

				// �o�b�N�X�e�b�v��A���œ��񂾉񐔂����Z�b�g
				m_nBackStep = 0;

				// AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_ATTACK;

				// �ʏ��Ԃɂ���
				SetState(STATE_BOSS_ATTACK_0);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else
			{// �U�����d�|�����鋗���ɂ��Ȃ�
			    // AI�̎�ނ����ւ�
				m_AIType_Boss = AI_TYPE_BOSS_CHECK;

				// �ʏ��Ԃɂ���
				SetState(STATE_NORMAL);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
		}
		else
		{// �v���C���[������ł���
		    // AI�̎�ނ����ւ�
			m_AIType_Boss = AI_TYPE_BOSS_CHECK;

			// �ʏ��Ԃɂ���
			SetState(STATE_NORMAL);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}
	}
}

//=============================================================================
//    �{�X�̋O�Ղ��΂��čU�����鏈��
//=============================================================================
void CBoss::AI_SkipOverOrbit(void)
{
	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	// �U������������v���C���[�Ɍ�����
	if (pPlayer != NULL)
	{// �v���C���[�N���X���擾�ł��Ă���
		float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
		fAngle += D3DX_PI;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI *2.0f;
		}
		SetRot(D3DXVECTOR3(GetRot().x, fAngle, GetRot().z));
	}

	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// ���[�V�����Đ����I������
		int nTiming = BOSS_ATTACK_ORBIT_INTERVAL_0;
		if (GetLife() <= 50)
		{// �̗͂������Ȃ�
			nTiming = BOSS_ATTACK_ORBIT_INTERVAL_2;
		}
		else if (GetLife() <= 150)
		{// �̗͂������ȉ��ł���
			nTiming = BOSS_ATTACK_ORBIT_INTERVAL_1;
		}

		if (nTiming != 0)
		{// 0�ɂȂ��Ă��Ȃ�
			if (m_nSkipOverOrbitCounter % nTiming == 0)
			{// �O�Ղ��΂��^�C�~���O�Ȃ���
				if (pPlayer != NULL)
				{// �v���C���[�N���X���擾�ł��Ă���
					// �������΂�
					float fAngle = atan2f(pPlayer->GetPos().x - GetPos().x, pPlayer->GetPos().z - GetPos().z);
					CTornade::Create(GetPos(), m_pEffectManager, fAngle, 0);

					// SE���Đ�
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSS_TORNADE);
				}
			}
		}

		if (m_nSkipOverOrbitCounter >= BOSS_ATTACK_ORBIT)
		{// �O�Ղ��΂���ԂɂȂ��Ă��炵�΂炭������
		    // �J�E���^�[��߂�
			m_nSkipOverOrbitCounter = 0;

			// AI�̎�ނ����ւ�
			m_AIType_Boss = AI_TYPE_BOSS_TIRED;

			// ��ꂽ��Ԃɂ���
			SetState(STATE_BOSS_TIRED);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}

	    // �J�E���^�[�𑝂₷
		m_nSkipOverOrbitCounter++;
	}
}

//=============================================================================
//    �{�X�̋O�Ղ��΂��Ĕ��Ă��܂�������
//=============================================================================
void CBoss::AI_Tired(void)
{
	if (m_nTiredCounter % BOSS_RESTART_TIRED == 0)
	{// �����ԂɂȂ��Ă��炵�΂炭�����Ă���
		// �J�E���^�[��߂�
		m_nTiredCounter = 0;

	    // AI�̎�ނ����ւ�
		m_AIType_Boss = AI_TYPE_BOSS_CHECK;

		// �ʏ��Ԃɂ���
		SetState(STATE_NORMAL);
		GetMotionManager()->SwitchMotion(GetModel(), GetState());
	}
}

//=============================================================================
//    �{�X�̃_���[�W����
//=============================================================================
bool CBoss::Hit(int nDamage)
{
	bool bDeath = false;    // ���񂾂��ǂ���
	int nLife = GetLife();  // ���݂̗̑�

	// �̗͂����炷
	nLife -= nDamage;

	if (GetLife() <= 50)
	{// �̗͂������Ȃ�
		// �f�t�H���g�̐F��ݒ肷��
		CModel **pModel = GetModel();
		LPD3DXBUFFER pBuffMat;
		DWORD nNumMat;
		D3DXMATERIAL *pMat;
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// �p�[�c�������J��Ԃ�
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// �|�C���^���擾�ł���
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// �}�e���A�����̐������J��Ԃ�
					pMat[nCntMat].MatD3D.Diffuse = m_ModelColorDef[nCntMat][nCntParts];
				}
			}
		}
	}
	else if (GetLife() <= 150)
	{// �̗͂������ȉ��ł���
		// �f�t�H���g�̐F��ݒ肷��
		CModel **pModel = GetModel();
		LPD3DXBUFFER pBuffMat;
		DWORD nNumMat;
		D3DXMATERIAL *pMat;
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// �p�[�c�������J��Ԃ�
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// �|�C���^���擾�ł���
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// �}�e���A�����̐������J��Ԃ�
					pMat[nCntMat].MatD3D.Diffuse = m_ModelColorDef[nCntMat][nCntParts];
				}
			}
		}
	}

	// �U����������������ɂ���
	SetDamage(true);

	// �̗̓Q�[�W�����炷
	CGaugeLife *pGaugeLife = GetGaugeLife();
	if (pGaugeLife != NULL)
	{
		pGaugeLife->CutGauge(nDamage);
	}

	if (nLife <= 0)
	{// �̗͂�0�ȉ��ɂȂ���
		Death();

		// ���񂾔����
		bDeath = true;
	}
	else
	{// �܂��̗͂�����
		if (m_AIType_Boss != AI_TYPE_BOSS_ATTACK && m_AIType_Boss != AI_TYPE_BOSS_STEP
			&& m_AIType_Boss != AI_TYPE_BOSS_BACKSTEP && m_AIType_Boss != AI_TYPE_BOSS_SKIPOVER_ORBIT)
		{
			SetState(STATE_DAMAGE);
			GetMotionManager()->SwitchMotion(GetModel(), GetState());
		}

		// �̗͂̐ݒ�
		SetLife(nLife);
	}

	return bDeath;
}

//=============================================================================
//    �{�X�� ���񂾂Ƃ��̏���
//=============================================================================
void CBoss::Death(void)
{
	// ��Ԃƃ��[�V������ς���
	SetState(STATE_DEATH);
	GetMotionManager()->SwitchMotion(GetModel(), GetState());

	// �v���C���[�̃��b�N�I���ΏۂɂȂ��Ă��Ȃ����`�F�b�N
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
		if (pPlayer != NULL)
		{// �v���C���[�N���X���擾�ł���
			if (pPlayer->GetLockOnEnemy() == this)
			{// ���b�N�I���Ώۂ�����������
			    // ���b�N�I������������
				pPlayer->LockLost();
			}
		}
	}

	// ���X�{�X���j�{�[�i�X��t�^
	CResult::SetLastBossBonus(BOSS_BONUS);

	// �{�X���|�ꂽ���̃C�x���g�𔭐�������
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[�����[�h����������
		CGame *pGame = CManager::GetGame();
		if (pGame->GetState() != CGame::STATE_KNOCKDOWNBOSS)
		{// �{�X���|�ꂽ��ԂłȂ�
			pGame->SetState(CGame::STATE_KNOCKDOWNBOSS);
			pGame->SetBossPos(GetPos());
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM003);
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM005);
		}
	}
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
void CBoss::Collision(void)
{
	bool bLand = false;   // ����Ă��邩�ǂ���

	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();          // ���݂̍��W
	D3DXVECTOR3 Move = GetMove();        // ���݂̈ړ���

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	CMeshWall *pWall = NULL;             // �ǃN���X�ւ̃|�C���^
	CEnemy *pEnemy = NULL;               // �G�N���X�ւ̃|�C���^
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (bLand != true)
			{// �܂��I�u�W�F�N�g�ɏ���Ă��Ȃ�
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// �n�ʃN���X�ւ̃|�C���^��������
					pField = (CMeshField*)pScene;
					if (pField != NULL)
					{// �n�ʂ��擾�ł���
						float fFieldHeight = pField->GetPolyHeight(pos, NULL, NULL);
						if (pos.y < fFieldHeight)
						{// ���W���n�ʂ�艺�ɂȂ���
							Move.y = 0.0f;
							pos.y = fFieldHeight;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// �z�u����������
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// �z�u�����擾�ł���
						if (pObject->Collision(&pos, &GetPosOld(), &Move, GetColHeight(), GetColRange(), this) == true)
						{// �z�u���̏�ɏ���Ă���
							if (GetLand() == false)
							{// ���n���Ă��Ȃ�������
								SetLand(true);
							}

							if (GetLandScene() == NULL && GetLand() == true)
							{// �����ɏ���Ă���
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_MESHWALL)
				{// �ǃN���X�ւ̃|�C���^��������
					pWall = (CMeshWall*)pScene;
					if (pWall != NULL)
					{// �ǂ��擾�ł���
						if (pWall->Collision(&pos, &GetPosOld(), &D3DXVECTOR3(), D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange())))
						{// �ǂɓ������Ă���

						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// �G��������
					pEnemy = (CEnemy*)pScene;
					if (pEnemy != NULL)
					{// �G�N���X�ւ̃|�C���^���擾�ł���
						if (pEnemy->Collision(&pos, &GetPosOld(), &Move, GetColRange()))
						{// �G�ɓ������Ă���
						}
					}
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);         // ���݂̍��W
	SetMove(Move);       // ���݂̈ړ���
	SetLand(bLand);      // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
}

//=============================================================================
//    �U���̓����蔻�菈��
//=============================================================================
bool CBoss::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage)
{
	bool bHit = false;                 // �U�����q�b�g�������ǂ���
	bool bDeath = false;               // �v���C���[�����񂾂��ǂ���
	float fPlayerRange = 0.0f;         // �v���C���[�̓����蔻������͈�
	D3DXVECTOR3 OffSet = Offset;       // �����蔻����Ƃ�ꏊ(�e���f������̃I�t�Z�b�g)
	float fHitRange = 0.0f;            // �����蔻�����钷��
	float fLength = 0.0f;              // �����蔻������ꏊ�ƃv���C���[�Ƃ̋���
	D3DXMATRIX mtxWorld;

	// �����蔻��������W���m�肳����
	if (nModelIdx != -1)
	{// ���f�����瓖���蔻��𔭐�������
		CModel *pModel = GetModel(nModelIdx);
		if (pModel != NULL)
		{
			mtxWorld = pModel->GetMtxWorld();
		}
	}
	else
	{// ���f�����瓖���蔻��𔭐������Ȃ�
		mtxWorld = GetMtxWorld();
	}
	D3DXVec3TransformCoord(&OffSet, &OffSet, &mtxWorld);

	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}

	// ����J�n
	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
	    // �v���C���[�̓����蔻������͈͂��擾
		fPlayerRange = pPlayer->GetColRange();

		// �����蔻�����钷�����v�Z
		fHitRange = sqrtf((fPlayerRange + fRange) * (fPlayerRange + fRange));

		// �����蔻��������W�ƓG�̍��W�Ƃ̋����𑪂�
		D3DXMATRIX PlayerMtxWorld;
		D3DXVECTOR3 PlayerPos;
		CModel *pModel = pPlayer->GetModel(0);
		if (pModel != NULL)
		{
			PlayerMtxWorld = pModel->GetMtxWorld();
			PlayerPos = D3DXVECTOR3(PlayerMtxWorld._41, PlayerMtxWorld._42, PlayerMtxWorld._43);
			fLength = sqrtf((PlayerPos.x - OffSet.x) * (PlayerPos.x - OffSet.x) + (PlayerPos.y - OffSet.y) * (PlayerPos.y - OffSet.y) + (PlayerPos.z - OffSet.z) * (PlayerPos.z - OffSet.z));
		}

		if (fLength <= fHitRange)
		{// �����蔻��������W�ƓG�̍��W�Ƃ̋����������蔻�����钷�����Z��
			if (pPlayer->GetState() != CPlayer::STATE_DAMAGE)
			{// �v���C���[�̏�Ԃ��_���[�W��ԂłȂ�
				pPlayer->Damage(nDamage);
				bHit = true;
			}
			CDebugProc::Print("�U�����q�b�g���܂���\n");
		}
	}

	return bHit;  // �q�b�g�������ǂ�����Ԃ�
}

//=============================================================================
//    ��ԑJ�ڏ���
//=============================================================================
void CBoss::Statement(void)
{
	if (m_AIType_Boss == AI_TYPE_BOSS_ATTACK)
	{// �U�����Ă���AI�̏�Ԃ�������
		D3DXVECTOR3 Move = GetMove();

		if (GetState() == STATE_BOSS_ATTACK_0)
		{// 1�ڂ̍U�����[�V����������
			if (GetMotionManager()->GetAllCounter() == 22)
			{// ������x�̃J�E���^�[�̒l�ɂȂ��Ă���

				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
			}
			if (GetMotionManager()->GetAllCounter() == 18)
			{
				Move.x -= sinf(GetRot().y) * 20.0f;
				Move.z -= cosf(GetRot().y) * 20.0f;
			}
		}
		else if (GetState() == STATE_BOSS_ATTACK_1)
		{// 2�ڂ̍U�����[�V����������
			if (GetMotionManager()->GetAllCounter() == 9)
			{
				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
			}
			if (GetMotionManager()->GetAllCounter() == 7)
			{
				Move.x -= sinf(GetRot().y) * 10.0f;
				Move.z -= cosf(GetRot().y) * 10.0f;
			}
		}
		else if (GetState() == STATE_BOSS_ATTACK_2)
		{// 3�ڂ̍U�����[�V����������
			if (GetMotionManager()->GetAllCounter() == 28)
			{
				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
			}
			if (GetMotionManager()->GetAllCounter() == 25)
			{
				Move.x -= sinf(GetRot().y) * 20.0f;
				Move.z -= cosf(GetRot().y) * 20.0f;
			}
		}
		if (GetState() == STATE_BOSS_ATTACK_3)
		{// �Ō�̍U�����[�V����������
			// ������ׂ��^�C�~���O�ŃG�t�F�N�g���o��
			if (GetMotionManager()->GetAllCounter() == 35)
			{
				D3DXVECTOR3 posDef = GetPos();
				D3DXVECTOR3 pos = GetPos();
				m_pEffectManager->SetEffect(pos + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 45.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 75.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 90.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 105.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
				m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 120.0f, pos.z) + (Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK);
			}
			if (GetMotionManager()->GetAllCounter() == 34)
			{
				Move.x -= sinf(GetRot().y) * 25.0f;
				Move.z -= cosf(GetRot().y) * 25.0f;
			}
		}

		if (GetMotionManager()->GetMotion()[GetState()]->GetCollision() != NULL)
		{// �����蔻��f�[�^�N���X����������Ă���
			for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[GetState()]->GetNumColData(); nCntCol++)
			{// �����蔻��f�[�^�̐������J��Ԃ�
				// �����蔻��ɕK�v�ȃf�[�^��錾
				int nModelIdx = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetModelIdx();
				float fRange = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetRange();
				D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetOffsetPos();
				int nStartTiming = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetStartTiming();
				int nFinishTiming = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetFinishTiming();
				int nDamage = GetMotionManager()->GetMotion()[GetState()]->GetCollision()[nCntCol]->GetDamage();

				if (GetState() == STATE_BOSS_ATTACK_3)
				{// �Ō�̍U�����[�V����������
					fRange += 12.0f * (GetMotionManager()->GetAllCounter() - nStartTiming);
				}

				// ����J�n
				if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
				{// �����蔻��𔭐�������^�C�~���O�Ȃ��
				 // �U�����̓����蔻��𔭐�������
					AttackCollision(nModelIdx, fRange, Offset, nDamage);
				}
			}
		}

		if (GetMotionManager()->GetMotion()[GetState()]->GetOrbit() != NULL)
		{// �O�Ճf�[�^�N���X����������Ă���
			if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetStartTiming())
			{// �O�Ղ��o���^�C�~���O�ɂȂ���
				if (m_pOrbitEffect == NULL)
				{// �I�[�r�b�g�G�t�F�N�g�N���X�ւ̃|�C���^����ɂȂ��Ă���
					// �O�ՂɕK�v�ȏ���錾
					int nModelIdx = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetModelIdx();
					int nTexIdx = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetTexIdx();
					D3DXVECTOR3 OffSetPos1 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetPos1();
					D3DXVECTOR3 OffSetPos2 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetPos2();
					D3DXVECTOR3 OffSetAmp1 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetAmp1();
					D3DXVECTOR3 OffSetAmp2 = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetOffsetAmp2();
					D3DXCOLOR ColUp = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetColUp();
					D3DXCOLOR ColDown = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetColDown();
					float fAlphaDecayUp = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetAlphaDecayUp();
					float fAlphaDecayDown = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetAlphaDecayDown();
					int nXBlock = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetXBlock();
					int nYBlock = GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetYBlock();
					D3DXMATRIX *pMtxParent = GetModel(nModelIdx)->GetMtxWorldPointer();
					m_pOrbitEffect = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, pMtxParent, EFFECT_PRIORITY);
					if (m_pOrbitEffect != NULL)
					{// �O�Ղ𐶐��ł���
						m_pOrbitEffect->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
					}
				}
			}
			if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[GetState()]->GetOrbit()[0]->GetFinishTiming())
			{// �O�Ղ����܂��^�C�~���O�ɂȂ���
				if (m_pOrbitEffect != NULL)
				{
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
			}
		}

		// �e��l�̐ݒ�
		SetMove(Move);
	}

	if (GetState() == STATE_DAMAGE)
	{// �_���[�W��Ԃ�������
		int nDispCounter = GetDispCounter();
		nDispCounter++;
		if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
		{// ���[�V�����Đ����I������
			if (m_AIType_Boss == AI_TYPE_BOSS_TIRED)
			{// ���Ă��郂�[�V������������
				SetState(STATE_BOSS_TIRED);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			else
			{// ���Ă��Ȃ����[�V������������
				SetState(STATE_NORMAL);
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			nDispCounter = 0;
			SetDamage(false);
			m_nDamageCounter = 0;
		}
		SetDispCounter(nDispCounter);
	}
}

//=============================================================================
//    �̗͂��Ⴍ�Ȃ��Ă���Ƃ��̏���
//=============================================================================
void CBoss::DeathWarning(void)
{
	if (GetLife() <= 150)
	{// �̗͂������ȉ��ł���
		int nIdx = 0;
		m_nRedCounter++;
		int nChangeTiming = 0;
		if (GetLife() <= 50)
		{// �̗͂������Ȃ�
			nIdx = 1;
			nChangeTiming = BOSS_ATTACK_ORBIT_INTERVAL_1;
		}
		else if (GetLife() <= 150)
		{// �̗͂������ȉ��ł���
			nChangeTiming = BOSS_ATTACK_ORBIT_INTERVAL_2;
		}
		if (nChangeTiming != 0)
		{// �ς���^�C�~���O���ݒ�ŗ���
			if (m_nRedCounter % nChangeTiming == 0)
			{// �F��ς���^�C�~���O�ɂȂ���
				m_fRedAdd[nIdx] *= -1;
			}
		}

		 // ���f���̐F��ύX����
		CModel **pModel = GetModel();
		LPD3DXBUFFER pBuffMat;
		DWORD nNumMat;
		D3DXMATERIAL *pMat;
		D3DXCOLOR ModelColor[4];
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// �p�[�c�������J��Ԃ�
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// �|�C���^���擾�ł���
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// �}�e���A�����̐������J��Ԃ�
					ModelColor[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
					ModelColor[nCntMat].r += m_fRedAdd[nIdx];
					if (ModelColor[nCntMat].r >= 1.0f)
					{
						ModelColor[nCntMat].r = 1.0f;
					}
					ModelColor[nCntMat].g -= m_fRedAdd[nIdx];
					if (ModelColor[nCntMat].g >= 0.0f)
					{
						ModelColor[nCntMat].g = 0.0f;
					}
					ModelColor[nCntMat].b -= m_fRedAdd[nIdx];
					if (ModelColor[nCntMat].b <= 0.0f)
					{
						ModelColor[nCntMat].b = 0.0f;
					}
					pMat[nCntMat].MatD3D.Diffuse = ModelColor[nCntMat];
				}
			}
		}
	}
}

//=============================================================================
//    �{�X�����񂾂Ƃ��̏���
//=============================================================================
void CBoss::DeathState(void)
{
	if (GetMotionManager()->GetMotion()[GetState()]->GetState() == CMotion::STATE_STOP)
	{// ���[�V�����Đ����I������
	    // �G�t�F�N�g���o��
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[������������
			CEffectManager *pEffectManager = CManager::GetGame()->GetEffectManager();
			if (pEffectManager != NULL)
			{// �|�C���^���擾�ł���
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 21);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 22);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 23);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 24);
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 25);
			}
		}

		// SE���Đ�
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DEATH);

		// �{�X������
		Uninit();
	}
}