//*****************************************************************************
//
//     �v���C���[�̏���[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "camera.h"
#include "input.h"
#include "effectManager.h"
#include "object.h"
#include "enemy.h"
#include "orbitEffect.h"
#include "meshField.h"
#include "river.h"
#include "meshWall.h"
#include "item.h"
#include "gauge.h"
#include "ui.h"
#include "map.h"
#include "combo.h"
#include "sceneBillboard.h"
#include "textureManager.h"
#include "shadow.h"
#include "functionlib.h"
#include "grass.h"
#include "boxCollider.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �f�[�^���͗p�}�N��
#define PLAYER_AVATEROFFSET_1        (D3DXVECTOR3(80.0f,0.0f,0.0f))     // �P�̖ڂ̕��g�̃I�t�Z�b�g
#define PLAYER_AVATEROFFSET_2        (D3DXVECTOR3(-80.0f,0.0f,0.0f))    // �Q�̖ڂ̕��g�̃I�t�Z�b�g
#define PLAYER_AVATEROFFSET_3        (D3DXVECTOR3(0.0f,0.0f,80.0f))     // �R�̖ڂ̕��g�̃I�t�Z�b�g
#define PLAYER_AVATEROFFSET_4        (D3DXVECTOR3(0.0f,0.0f,-80.0f))    // �S�̖ڂ̕��g�̃I�t�Z�b�g
#define PLAYER_AVATER_CUTTIMING      (1200)                             // ���g���o���Ă��猸�炷�܂ł̎���
#define PLAYER_MAX_MODELPARTS        (20)                               // �v���C���[�̃p�[�c��
#define PLAYER_SPECIALGAUGE_ORBITADD (1.0f)                             // �K�E�Q�[�W���ǂꂭ�炢���₷��(�O�Վa�����Ȃ��Ƃ�)
#define PLAYER_SPECIALGAUGE_ADD      (5.0f)                             // �K�E�Q�[�W���ǂꂭ�炢���₷��
#define PLAYER_AVATER_LIFECUT        (1)                                // ���g�����Ƃ��ɏ����̗�
#define PLAYER_SHADOW_PRIORITY       (9)                                // �v���C���[�̉e�̏����D�揇��
#define PLAYER_SHADOW_WIDTH          (18.0f)                            // �v���C���[�̉e�̕�
#define PLAYER_SHADOW_HEIGHT         (18.0f)                            // �v���C���[�̉e�̍���
#define PLAYER_SHADOW_COLOR          (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // �v���C���[�̉e�̐F
#define PLAYER_DAMAGE_COUNTER_MAX    (120)                              // �v���C���[�̃_���[�W�Ǘ��J�E���^�[�̍ő�l

// ���b�N�I���p�}�N��
#define PLAYER_LOCKON_POLY_PRIORITY (10)                                // ���b�N�I���|���S���̏����D�揇��
#define PLAYER_LOCKON_DISTANCE      (500.0f)                            // ���b�N�I�����J�n�ł��鋗��
#define PLAYER_LOCKONPOLY_WIDTH     (30.0f)                             // ���b�N�I���|���S���̕�
#define PLAYER_LOCKONPOLY_HEIGHT    (30.0f)                             // ���b�N�I���|���S���̍���

// �c���L���p�}�N��
#define PLAYER_AFTERIMAGE_COLOR     (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))    // �c���̐F
#define PLAYER_AFTERIMAGE_SCALE     (D3DXVECTOR3(0.95f,0.95f,0.95f))    // �c���̑傫��
#define PLAYER_AFTERIMAGE_SCALECUT  (0.05f)                             // �傫��������
#define PLAYER_AFTERIMAGE_ALPHACUT  (0.01f)                             // �����x������
#define PLAYER_AFTERIMAGE_REDUP     (0.1f)                              // �ԐF�𑝂₷��
#define PLAYER_AFTERIMAGE_GREENUP   (0.1f)                              // �΂𑝂₷��
#define PLAYER_ORBIT_UPTIMING       (240)                               // �O�ՃQ�[�W���񕜂���܂ł̎���
#define PLAYER_ORBIT_UPVALUE        (0.7f)                              // �P��̍X�V�ŉ񕜂���O�ՃQ�[�W��
#define PLAYER_ORBIT_CUTVALUE       (0.2f)                              // �P��̍X�V�Ō�������O�ՃQ�[�W��

// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// ����
#define POS               "POS = "                // ���W��ǂݎ�鍇�}
#define ROT               "ROT = "                // ������ǂݎ�鍇�}
#define MOVE              "MOVE = "               // �ړ��ʂ�ǂݎ�鍇�}

// ���̑�
#define NUM_TEXTURE       "NUM_TEXTURE = "        // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "   // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL         "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME    "MODEL_FILENAME = "     // �ǂݍ��ރ��f���̃t�@�C����

// �L�����N�^�[���
#define CHARACTERSET      "CHARACTERSET"          // �L�����N�^�[���ǂݍ��݊J�n�̍��}
#define END_CHARACTERSET  "END_CHARACTERSET"      // �L�����N�^�[���ǂݍ��ݏI���̍��}
#define JUMP              "JUMP = "               // �W�����v��
#define GRAVITY           "GRAVITY = "            // �d�͗�
#define INERTIA           "INERTIA = "            // ������
#define RIVISION_ANGLE    "RIVISION_ANGLE = "     // ������␳����{��
#define COL_HEIGHT        "COL_HEIGHT = "         // �����蔻����Ƃ鍂��
#define COL_RANGE         "COL_RANGE = "          // �����蔻����Ƃ�͈�
#define NUM_PARTS         "NUM_PARTS = "          // �L�����N�^�[�̃p�[�c��

// �p�[�c���
#define PARTSSET          "PARTSSET"              // �p�[�c���ǂݍ��݊J�n�̍��}
#define END_PARTSSET      "END_PARTSSET"          // �p�[�c���ǂݍ��ݏI���̍��}
#define INDEX             "INDEX = "              // �ԍ���ǂݎ�鍇�}
#define PARENT            "PARENT = "             // �e�p�[�c�̔ԍ���ǂݎ�鍇�}

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
#define KEYSET            "KEYSET"                // �L�[�t���[�����ǂݍ��݊J�n�̍��}
#define END_KEYSET        "END_KEYSET"            // �L�[�t���[�����ǂݍ��ݏI���̍��}
#define FRAME             "FRAME = "              // �L�[�t���[���̍Đ��t���[������ǂݎ�鍇�}

// �L�[���
#define KEY               "KEY"                   // �L�[���ǂݍ��݊J�n�̍��}
#define END_KEY           "END_KEY"               // �L�[���ǂݍ��ݏI���̍��}

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
D3DXVECTOR3 CPlayer::m_AvaterOffset[PLAYER_AVATAER_NUM]   // ���g�̃I�t�Z�b�g
{
	PLAYER_AVATEROFFSET_1,
	PLAYER_AVATEROFFSET_2,
	PLAYER_AVATEROFFSET_3,
	PLAYER_AVATEROFFSET_4
};

//*****************************************************************************
//     CPlayerManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayerManager::CPlayerManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pTextureManager = NULL;     // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	strcpy(m_aFileName, "\0");    // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	m_pModelManager = NULL;       // ���f���Ǌ��N���X�ւ̃|�C���^
	m_fOrbitSlash = 0.0f;         // �v���C���[�̋O�ՃQ�[�W��
	m_nLife = 0;                  // �v���C���[�̗̑�
	m_fMaxSpecial = 0.0f;         // �v���C���[�̕K�E�Z�Q�[�W�̍ő�l�̍ő�l
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPlayerManager::~CPlayerManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPlayerManager *CPlayerManager::Create(char *pFileName, int nLife, float fOrbitSlash, float fMaxSpecial, int nPriority)
{
	CPlayerManager *pPlayerManager = NULL;  // �v���C���[�Ǌ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();    // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pPlayerManager == NULL)
		{// ����������ɂȂ��Ă���
			pPlayerManager = new CPlayerManager(nPriority);
			if (pPlayerManager != NULL)
			{// �C���X�^���X�𐶐��ł���
				pPlayerManager->SetFileName(pFileName);
				pPlayerManager->SetLife(nLife);
				pPlayerManager->SetOrbitSlash(fOrbitSlash);
				pPlayerManager->SetMaxSpecial(fMaxSpecial);
				if (FAILED(pPlayerManager->Init()))
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

	return pPlayerManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPlayerManager::Init(void)
{
	// �v���C���[�̃p�[�c���𔲂��o��
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
			int nNumModel = 0;                          // �ǂݍ��ރ��f����
			int nNumParts = 0;                          // �ǂݍ��ރp�[�c��
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉񐔂𐔂���
			int nCntModel = 0;                          // ���f����ǂݍ��񂾉񐔂𐔂���
			int nCntParts = 0;                          // �p�[�c����ǂݍ��񂾉񐔂𐔂���
			int nCntMotion = 0;                         // ���[�V������ǂݍ��񂾉񐔂𐔂���
			int *pNumParent = NULL;                     // �e���f���̔ԍ�
			CMotionManager *pMotionManager = NULL;      // ���[�V�����Ǘ��N���X�ւ̃|�C���^
			CMotion *pMotion[CPlayer::STATE_MAX] = {};  // ���[�V�����N���X�ւ̃|�C���^
			CModel **apModel = NULL;                    // ���f���N���X�ւ̃|�C���^
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`�����ւ̃|�C���^
			LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                          // �}�e���A�����̐�
			float fAccel = 0.0f;                        // �v���C���[�̉����x���
			float fInertia = 0.0f;                      // �v���C���[�̊������
			float fGravity = 0.0f;                      // �v���C���[�̏d�͏��
			float fJumpPower = 0.0f;                    // �v���C���[�̃W�����v�ʏ��
			float fRivisionRot = 0.0f;                  // �v���C���[�̌�����␳����{�����
			float fColHeight = 0.0f;                    // �v���C���[�̓����蔻�����鍂�����
			float fColRange = 0.0f;                     // �v���C���[�̓����蔻������͈͏��

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
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// �ǂݍ��ރ��f����������
								strcpy(pStr, pStrCur);
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// ���f������1�ȏ゠��
									pNumParent = new int[nNumModel];
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// ���f���̃t�@�C���p�X����ǂݍ���
								if (m_pModelManager != NULL && nCntModel < nNumModel)
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
								apModel = ReadCharacterSet(pLine, pStrCur, pFile, &nNumParts, pNumParent, &fAccel, &fInertia, &fJumpPower, &fRivisionRot, &fGravity, &fColHeight, &fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// ���[�V�������ǂݍ��݊J�n�̍��}��������
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &nNumParts, pMotion[nCntMotion], nCntMotion);
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
			if (pMotionManager == NULL)
			{// �|�C���^���m�ۂ���Ă���
				pMotionManager = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
				if (pMotionManager != NULL)
				{// ���[�V�����Ǘ��N���X���쐬�ł���
					for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
					{// ��Ԃ̐������J�肩����
						pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// �v���C���[�N���X���쐬����
			CreatePlayer(apModel, pMotionManager, nNumParts, pNumParent, fAccel, fInertia, fJumpPower, fRivisionRot, fGravity, fColHeight, fColRange);

			// �ǂݎ��p�̃|�C���^���J�����Ă���
			if (apModel != NULL)
			{// ���������m�ۂ���Ă���
				for (int nCntModel = 0; nCntModel < nNumParts; nCntModel++)
				{// �g�p�ł��郂�f�������J��Ԃ�
					if (apModel[nCntModel] != NULL)
					{// �|�C���^���m�ۂ���Ă���
						apModel[nCntModel]->Uninit();

						// �������̊J��
						delete apModel[nCntModel];
						apModel[nCntModel] = NULL;
					}
				}
				// �������̊J��
				delete[] apModel;
				apModel = NULL;
			}

			// ���[�V�����̔j��
			if (pMotionManager != NULL)
			{// �|�C���^���m�ۂ���Ă���
				pMotionManager->Uninit();

				// �������̊J��
				delete pMotionManager;
				pMotionManager = NULL;
			}

			// �e���f���̔ԍ�
			if (pNumParent != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pNumParent;
				pNumParent = NULL;
			}
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
void CPlayerManager::Uninit(void)
{
	// ���f���Ǌ��N���X�̔j��
	if (m_pModelManager != NULL)
	{// ���������m�ۂ���Ă���
		// �I������
		m_pModelManager->Uninit();

		// �������̊J��
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// ���g�̃|�C���^��j������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPlayerManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPlayerManager::Draw(void)
{

}

//=============================================================================
//    �v���C���[�N���X���쐻���鏈��
//=============================================================================
void CPlayerManager::CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange)
{
	CModel **apModelCpy = NULL;                     // ���f���N���X�R�s�[�p
	CMotionAttack *pMotionAttack = NULL;            // �U�����[�V�����f�[�^�N���X�R�s�[�p
	CMotionCollision **pMotionCollision = NULL;     // ���[�V�����̓����蔻��f�[�^�N���X�R�s�[�p
	CMotionOrbit **pMotionOrbit = NULL;             // ���[�V�����̋O�Ճf�[�^�N���X�R�s�[�p
	CMotion *pMotionCpy[CPlayer::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;       // ���[�V�����Ǌ��N���X�R�s�[�p
	CPlayer *pPlayer = NULL;                        // �v���C���[�N���X�ւ̃|�C���^

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
			pMotionManagerCpy = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
				{// �v���C���[�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// �U�����[�V�����f�[�^�N���X�̃R�s�[
							if (pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// �U�����[�V�����f�[�^�N���X����������Ă���
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// ���������m�ۂł���
									// �e��l�̐ݒ�
									pMotionAttack->SetNextMotionIdx(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// �����蔻��f�[�^�N���X�̃R�s�[
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionCollision = new CMotionCollision*[pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// ���������m�ۂł���
									for (int nCntCol = 0; nCntCol < pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// ���������m�ۂł���
											pMotionCollision[nCntCol]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
											pMotionCollision[nCntCol]->SetDamage(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

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
								pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �v���C���[�N���X���쐬����
		if (pPlayer == NULL)
		{// ���������m�ۂł����Ԃł���
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_nLife, apModelCpy, pMotionManagerCpy, m_pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, m_fOrbitSlash, m_fMaxSpecial, PLAYER_PRIORITY);
		}
	}
}

//=============================================================================
//    �v���C���[����ǂݎ�鏈��
//=============================================================================
CModel **CPlayerManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
		{// �ړ��ʂ�����
			*pAccel = CFunctionLib::ReadFloat(pStrCur, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, JUMP) == 0)
		{// �W�����v�ʂ�����
			*pJumpPower = CFunctionLib::ReadFloat(pStrCur, JUMP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// �d�͗ʂ�����
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, INERTIA) == 0)
		{// �����ʂ�����
			*pInertia = CFunctionLib::ReadFloat(pStrCur, INERTIA);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RIVISION_ANGLE) == 0)
		{// ������␳����{���ʂ�����
			*pRivisionRot = CFunctionLib::ReadFloat(pStrCur, RIVISION_ANGLE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	�����蔻�����鍂��������
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͂�����
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
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
CModel *CPlayerManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // �p�[�c�ԍ����
	int nParent = 0;                                   // �e���f���̃p�[�c�ԍ�
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
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx), m_pModelManager->GetTexture(nIdx));
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
CMotion *CPlayerManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
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
CMotionAttack *CPlayerManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
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
CMotionCollision **CPlayerManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
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
CMotionOrbit **CPlayerManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
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
void CPlayerManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
void CPlayerManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
//    �v���C���[�f�[�^�̃X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CPlayerManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    �v���C���[�̗̑͂�ݒ肷�鏈��
//=============================================================================
void CPlayerManager::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �v���C���[�̋O�ՃQ�[�W�ʂ�ݒ肷�鏈��
//=============================================================================
void CPlayerManager::SetOrbitSlash(const float fOrbitSlash)
{
	m_fOrbitSlash = fOrbitSlash;
}

//=============================================================================
//    �v���C���[�̕K�E�Z�Q�[�W�̍ő�l��ݒ肷�鏈��
//=============================================================================
void CPlayerManager::SetMaxSpecial(const float fMaxSpecial)
{
	m_fMaxSpecial = fMaxSpecial;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CPlayerManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    �v���C���[�f�[�^�̃X�N���v�g�t�@�C�������擾���鏈��
//=============================================================================
char *CPlayerManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    �v���C���[�̗̑͂��擾���鏈��
//=============================================================================
int CPlayerManager::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    �v���C���[�̋O�ՃQ�[�W�ʂ��擾���鏈��
//=============================================================================
float CPlayerManager::GetOrbitSlash(void)
{
	return m_fOrbitSlash;
}

//=============================================================================
//    �v���C���[�̕K�E�Z�Q�[�W�̍ő�l���擾���鏈��
//=============================================================================
float CPlayerManager::GetMaxSpecial(void)
{
	return m_fMaxSpecial;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CPlayerManager::GetModelManager(void)
{
	return m_pModelManager;
}


//*****************************************************************************
//     CPlayer�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_State = STATE_NORMAL;                  // ���
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړ���
	m_nLife = 0;                             // �̗�
	m_nMaxLife = 0;                          // �̗͂̍ő�l
	m_nLifeCounter = 0;                      // �̗͂̉񕜂��Ǘ�����J�E���^�[
	m_nEffectCounter = 0;                    // �G�t�F�N�g���o���Ԋu���Ǘ�����J�E���^�[
	m_nOrbitCounter = 0;                     // �O�ՃQ�[�W���񕜂���Ԋu���Ǘ�����J�E���^�[
	m_nAvaterCounter = 0;                    // ���g���o�Ă���̎���
	m_nAvaterNum = 0;                        // ���g���o���鐔
	m_nDamageCounter = 0;                    // �_���[�W��������Ă���̎��Ԃ��Ǘ�����J�E���^�[
	m_fAccel = 0.0f;                         // �����x
	m_fInertia = 0.0f;                       // ����
	m_fJumpPower = 0.0f;                     // �W�����v��
	m_fRivisionRot = 0.0f;                   // ������␳����{��
	m_fOrbitSlash = 0.0f;                    // �O�Վa�Ɏg�p����O�ՃQ�[�W��
	m_fOrbitSlashDef = 0.0f;                 // �O�Վa�Ɏg�p����O�ՃQ�[�W��
	m_fSpecial = 0.0f;                       // ���݂̕K�E�Z�Q�[�W��
	m_fMaxSpecial = 0.0f;                    // �K�E�Z�Q�[�W�̍ő�l
	m_bJump = false;                         // �W�����v���Ă��邩�ǂ���
	m_bAction = false;                       // �A�N�V�������Ă��邩�ǂ���
	m_bOrbit = false;                        // �O�Ղ��o���Ă��邩�ǂ���
	m_pOrbitEffect = NULL;                   // �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	m_pEffectManager = NULL;                 // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_pLifeGauge = NULL;                     // �̗̓Q�[�W�N���X�ւ̃|�C���^
	m_pOrbitSlashGauge = NULL;               // �O�ՃQ�[�W�N���X�ւ̃|�C���^
	m_pSpecialGauge = NULL;                  // �K�E�Q�[�W�N���X�ւ̃|�C���^
	m_pLockOnPoly = NULL;                    // ���b�N�I���p�̃|���S���N���X�ւ̃|�C���^
	m_bOrbitSlash = false;                   // �O�Վa�����邩�ǂ���
	m_bBooking = false;                      // �A�N�V�����̗\�񂪓����Ă��邩�ǂ���
	m_bInRiver = false;                      // ��ɓ����Ă��邩�ǂ���
	m_bInGrass = false;                      // ���ނ�̒��ɓ����Ă��邩�ǂ���
	m_bLockOn = false;                       // ���b�N�I�����Ă��邩�ǂ���
	m_bOracleAttack = false;                 // �I���N���A�^�b�N�����Ă��邩�ǂ���
	m_bDamage = false;                       // �_���[�W��������Ă��邩�ǂ���
	m_nCurrentStack = 0;                     // ���݂̎c���L���ԍ�
	for (int nCntImage = 0; nCntImage < PLAYER_AFTERIMAGE_NUM; nCntImage++)
	{// �c���L���ʂ̕������J��Ԃ�
		m_AfterPos[nCntImage] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_AfterRot[nCntImage] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_AfterModelPos[nCntImage] = NULL;
		m_AfterModelRot[nCntImage] = NULL;
	}

	for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
	{// ���g���o���鐔�����J��Ԃ�
		D3DXMatrixIdentity(&m_AvaterModelMtxWorld[nCntAvater]);
		m_pOrbitEffectAvater[nCntAvater] = NULL;    // �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, float fOrbitSlash, float fMaxSpecial, int nPriority)
{
	CPlayer *pPlayer = NULL;               // �v���C���[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pPlayer == NULL)
		{// ����������ɂȂ��Ă���
			pPlayer = new CPlayer(nPriority);
			if (pPlayer != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pPlayer->Init(pos, rot, nLife, apModel, pMotionManager, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, fOrbitSlash, fMaxSpecial)))
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

	return pPlayer;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, float fOrbitSlash, float fMaxSpecial)
{
	// �e��l�̐ݒ�
	SetPos(pos);                        // ���݂̍��W
	SetRot(rot);                        // ���݂̌���
	SetLife(nLife);                     // �̗�
	m_nMaxLife = nLife;                 // �̗͂̍ő�l
	SetNumParts(nNumParts);             // �p�[�c��
	SetModel(apModel);                  // ���f���N���X�ւ̃|�C���^
	SetMotionManager(pMotionManager);   // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	SetTextureManager(pTextureManager); // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	SetAccel(fAccel);                   // �����x
	SetInertia(fInertia);               // ����
	SetGravity(fGravity);               // �d��
	SetJumpPower(fJumpPower);           // �W�����v��
	SetRivisionRot(fRivisionRot);       // ������␳����{��
	SetOrbitSlash(fOrbitSlash);         // ���݂̋O�ՃQ�[�W��
	m_fOrbitSlashDef = fOrbitSlash;     // �O�ՃQ�[�W��
	m_fMaxSpecial = fMaxSpecial;        // �K�E�Z�Q�[�W�̍ő�l�̍ő�l
	SetColHeight(fColHeight);           // �����蔻�����鍂��
	SetColRange(fColRange);             // �����蔻������͈�

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
			else if (pScene->GetObjType() == OBJTYPE_LIFEGAUGE2D)
			{// �̗̓Q�[�W(2D)�N���X�ւ̃|�C���^������
				m_pLifeGauge = (CGaugeLife2D*)pScene;
			}
			else if (pScene->GetObjType() == OBJTYPE_SLASHGAUGE2D)
			{// �O�ՃQ�[�W(2D)�N���X�ւ̃|�C���^������
				m_pOrbitSlashGauge = (CGaugeOrbitSlash2D*)pScene;
			}
			else if (pScene->GetObjType() == OBJTYPE_SPECIALGAUGE2D)
			{// �K�E�Q�[�W(2D)�N���X�ւ̃|�C���^������
				m_pSpecialGauge = (CGaugeSpecial2D*)pScene;
			}
			pScene = pSceneNext;
		}
	}

	// ���f���̈ʒu���j���[�g�����̈ʒu�ɂ���
	SetDefaultPos();

	// ���f���̌������j���[�g�����̌����ɂ���
	SetDefaultRot();

	// �c���L���p�̃��������m�ۂ���
	for (int nCntAfter = 0; nCntAfter < PLAYER_AFTERIMAGE_NUM; nCntAfter++)
	{// �c�����L���ł��鐔�����J��Ԃ�
		m_AfterModelPos[nCntAfter] = new D3DXVECTOR3[GetNumParts()];
		if (m_AfterModelPos[nCntAfter] != NULL)
		{// ���������m�ۂł���
			for (int nCntModel = 0; nCntModel < GetNumParts(); nCntModel++)
			{// ���f���̐������J��Ԃ�
				m_AfterModelPos[nCntAfter][nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}

		m_AfterModelRot[nCntAfter] = new D3DXVECTOR3[GetNumParts()];
		if (m_AfterModelRot[nCntAfter] != NULL)
		{// ���������m�ۂł���
			for (int nCntModel = 0; nCntModel < GetNumParts(); nCntModel++)
			{// ���f���̐������J��Ԃ�
				m_AfterModelRot[nCntAfter][nCntModel] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// ���g�̃I�t�Z�b�g��ݒ�
	m_AvaterOffset[0] = PLAYER_AVATEROFFSET_1;
	m_AvaterOffset[1] = PLAYER_AVATEROFFSET_2;
	m_AvaterOffset[2] = PLAYER_AVATEROFFSET_3;
	m_AvaterOffset[3] = PLAYER_AVATEROFFSET_4;

	// �e���쐬����
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_SHADOW_COLOR, PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT, PLAYER_SHADOW_PRIORITY);
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^���J������
	if (m_pOrbitEffect != NULL)
	{// ���������m�ۂ���Ă���
		m_pOrbitEffect->Uninit();
		m_pOrbitEffect = NULL;
	}
	for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
	{// ���g�̐������J��Ԃ�
		if (m_pOrbitEffectAvater[nCntAvater] != NULL)
		{// ���������m�ۂ���Ă���
			m_pOrbitEffectAvater[nCntAvater]->Uninit();
			m_pOrbitEffectAvater[nCntAvater] = NULL;
		}
	}

	// ���b�N�I���̃|���S�����J������
	if (m_pLockOnPoly != NULL)
	{
		m_pLockOnPoly->Uninit();
		m_pLockOnPoly = NULL;
	}

	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����ɂ��Ă���
	m_pEffectManager = NULL;

	// �̗̓Q�[�W�N���X�ւ̃|�C���^����ɂ��Ă���
	m_pLifeGauge = NULL;

	// �O�ՃQ�[�W�N���X�ւ̃|�C���^����ɂ��Ă���
	m_pOrbitSlashGauge = NULL;

	// �K�E�Q�[�W�N���X�ւ̃|�C���^����ɂ��Ă���
	m_pSpecialGauge = NULL;

	// ���b�N�I���Ώۂ̓G�|�C���^���J������
	m_pLockOnEnemy = NULL;

	// ���ʂ̏I������
	CCharacter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^��ۑ�
	SetLandSceneOld(GetLandScene());

	// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
	SetLandScene(NULL);

	// �O��̍��W��ۑ�
	SetPosOld(GetPos());

	if (m_State == STATE_ATTACK_3 && GetMotionManager()->GetAllCounter() >= 34 && GetMotionManager()->GetAllCounter() <= 47)
	{// �Ō�̍U�����[�V���������̃��[�V�����̃J�E���^�[���ł���
		if (m_nCurrentStack >= PLAYER_AFTERIMAGE_NUM - 1)
		{// �܂��X�^�b�N�ʂ�����
		    // �z��̍Ō������z��̐擪�Ɍ������Ēl���R�s�[
			for (int nCntAfter = 0; nCntAfter <= m_nCurrentStack - 1; nCntAfter++)
			{// �Ō���A�h���X����X�^�[�g
				m_AfterPos[nCntAfter] = m_AfterPos[nCntAfter + 1];
				m_AfterRot[nCntAfter] = m_AfterRot[nCntAfter + 1];
				for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
				{// ���f���̃p�[�c�����J��Ԃ�
					m_AfterModelPos[nCntAfter][nCntParts] = m_AfterModelPos[nCntAfter + 1][nCntParts];
					m_AfterModelRot[nCntAfter][nCntParts] = m_AfterModelRot[nCntAfter + 1][nCntParts];
				}
			}
			m_nCurrentStack--;
		}

		// �c�����L������
		StackAfterImage();
	}
	else
	{// ���[�V�������ς���Ă���
		if (m_nCurrentStack >= 0)
		{// �X�^�b�N�ʂ��ς��܂�
		    // �z��̍Ō������z��̐擪�Ɍ������Ēl���R�s�[
			for (int nCntAfter = 0; nCntAfter <= m_nCurrentStack - 1; nCntAfter++)
			{// �Ō���A�h���X����X�^�[�g
				m_AfterPos[nCntAfter] = m_AfterPos[nCntAfter + 1];
				m_AfterRot[nCntAfter] = m_AfterRot[nCntAfter + 1];
				for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
				{// ���f���̃p�[�c�����J��Ԃ�
					m_AfterModelPos[nCntAfter][nCntParts] = m_AfterModelPos[nCntAfter + 1][nCntParts];
					m_AfterModelRot[nCntAfter][nCntParts] = m_AfterModelRot[nCntAfter + 1][nCntParts];
				}
			}
			m_nCurrentStack--;
		}
	}

	if (m_State != STATE_LANDING && m_State != STATE_DAMAGE && m_State != STATE_ORACLEATTACK)
	{// �_���[�W��ԂłȂ������n��ԂłȂ����X�e�b�v��ԂłȂ�����
		if (m_bAction != true)
		{// �A�N�V������ԂłȂ�
			if (CManager::GetMode() == CManager::MODE_GAME && CManager::GetGame()->GetState() == CGame::STATE_NORMAL
				|| CManager::GetMode() == CManager::MODE_TUTORIAL && CManager::GetTutorial()->GetState() == CTutorial::STATE_NONE)
			{// �Q�[�������`���[�g���A����ʒ���������
				// �ړ��ʌv�Z����
				InputMove();
			}
		}
	}

	// �ړ�����
	Movement();

	// �����蔻�菈��
	Collision();

	if (m_State != STATE_LANDING && m_State != STATE_DAMAGE && m_State != STATE_ORACLEATTACK)
	{// ���n��ԂłȂ����_���[�W��ԂłȂ����X�e�b�v��ԂłȂ�
		if (CManager::GetMode() == CManager::MODE_GAME && CManager::GetGame()->GetState() == CGame::STATE_NORMAL
			|| CManager::GetMode() == CManager::MODE_TUTORIAL && CManager::GetTutorial()->GetState() == CTutorial::STATE_NONE)
		{// �Q�[�������`���[�g���A����ʒ���������
		    // �A�N�V��������
			Action();
		}
	}

	// ��ԑJ�ڏ���
	Statement();

	// �G�t�F�N�g���쐬���鏈��
	CreateEffect();

	if (GetMotionManager() != NULL)
	{// ���[�V�����Ǘ��N���X����������Ă���
		GetMotionManager()->Update(GetModel());
	}

	// �O�ՃQ�[�W���񕜂�����
	if (m_fOrbitSlash < m_fOrbitSlashDef)
	{// �܂��O�ՃQ�[�W���񕜂ł���
		m_nOrbitCounter++;
		if (m_nOrbitCounter >= PLAYER_ORBIT_UPTIMING)
		{// �񕜂���^�C�~���O�ł���
			float fUpValue = PLAYER_ORBIT_UPVALUE;
			m_fOrbitSlash += fUpValue;
			if (m_fOrbitSlash >= m_fOrbitSlashDef)
			{// �f�t�H���g�̋O�ՃQ�[�W�ʂ𒴂���
				fUpValue = m_fOrbitSlash - m_fOrbitSlashDef;
				m_fOrbitSlash = m_fOrbitSlashDef;
			}
			// �Q�[�W�𑝂₷
			if (m_pOrbitSlashGauge != NULL)
			{// �O�ՃQ�[�W�N���X���擾�ł��Ă���
				m_pOrbitSlashGauge->AddGauge(fUpValue);
			}
		}
	}

	if (m_nAvaterNum >= 1)
	{// ���g��1�̈ȏ�o�Ă���
		m_nAvaterCounter++;
		if (m_nAvaterCounter % PLAYER_AVATER_CUTTIMING == 0)
		{// �J�E���^�[������̒l�ɒB����
			m_nAvaterNum--;
			if (m_nAvaterNum < 0)
			{// ����ȏ㐔�l�����点�Ȃ�
				m_nAvaterNum = 0;
			}
			else
			{// ���g��������
				if (m_pEffectManager != NULL)
				{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾�ł��Ă���
					D3DXVECTOR3 EffectPos = GetPos() + m_AvaterOffset[m_nAvaterNum];
					m_pEffectManager->SetEffect(D3DXVECTOR3(EffectPos.x, EffectPos.y + 40.0f, EffectPos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 28);
				}
				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_AVATER_DELETE);

				// �J�E���^�[��߂�
				m_nAvaterCounter = 0;
			}
		}
	}

	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// �`���[�g���A������������
		if (m_nLife < m_nMaxLife)
		{// �̗͂������Ă���
			m_nLifeCounter++;
			if (m_nLifeCounter % 5 == 0)
			{// ���̊��ԍX�V���ꂽ
				m_nLife++;
				if(m_pLifeGauge != NULL)
				{// �̗̓Q�[�W���擾�ł��Ă���
					m_pLifeGauge->AddGauge(1);
				}
			}
		}
	}

	if (m_bDamage == true)
	{// �_���[�W��������Ă�����
		m_nDamageCounter--;
		if (m_nDamageCounter <= 0)
		{// �_���[�W��������Ă��炠����x������
			m_nDamageCounter = 0;
			m_bDamage = false;
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// ���̉��̕`�掞�ɕK�v
	CModel **pModel = GetModel();                       // ���f���N���X�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;                              // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat;                                      // �}�e���A�����̐�
	D3DXMATERIAL *pMat;                                 // �}�e���A���ւ̃|�C���^
	D3DXCOLOR ModelColorDef[4][PLAYER_MAX_MODELPARTS];  // ���f���̐F(�ۑ��p)

	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// �p�[�c�������J��Ԃ�
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// �|�C���^���擾�ł���
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// �}�e���A�����̐������J��Ԃ�
				ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}
	}

	// ���̒��ɂ��邩�ǂ�������
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[�����̂ݔ���
		if (CManager::GetGame()->GetMapIdx() == 0)
		{// 1�}�b�v�ڂ̂ݔ���
			D3DXVECTOR3 pos = GetPos();
			if (pos.z <= 4791.0f && pos.z >= 3695.0f && pos.x >= -2.0f && pos.x <= 855.0f && pos.y < -30.0f)
			{// ���̒��ɂ���
				for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
				{// �p�[�c�������J��Ԃ�
					pBuffMat = pModel[nCntParts]->GetBuffMat();
					nNumMat = pModel[nCntParts]->GetNumMat();
					if (pBuffMat != NULL)
					{// �|�C���^���擾�ł���
						pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
						for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
						{// �}�e���A�����̐������J��Ԃ�
							ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
							pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(ModelColorDef[nCntMat][nCntParts].r - 0.4f, ModelColorDef[nCntMat][nCntParts].g - 0.4f, ModelColorDef[nCntMat][nCntParts].b - 0.4f, ModelColorDef[nCntMat][nCntParts].a);
						}
					}
				}
			}
		}
	}

	// ���ʂ̕`�揈��
	if (m_bDamage == true)
	{// �_���[�W��������Ă�����
		if (m_nDamageCounter % 3 == 0)
		{// �`�悷��^�C�~���O�ɂȂ���
			CCharacter::Draw();
		}
	}
	else
	{// �_���[�W��������Ă��Ȃ��Ȃ�
		CCharacter::Draw();
	}

	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// �p�[�c�������J��Ԃ�
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// �|�C���^���擾�ł���
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// �}�e���A�����̐������J��Ԃ�
				pMat[nCntMat].MatD3D.Diffuse = ModelColorDef[nCntMat][nCntParts];
			}
		}
	}

	if (m_nCurrentStack > 0)
	{// �c���L�����n�܂��Ă���
		DrawAfterImage();
	}

	// ���g��`�悷��
	if (m_nAvaterNum > 0)
	{// ���g�̐���1�̂ł�����
		DrawAvater();
	}
}

//=============================================================================
//    ���g��`�悷�鏈��
//=============================================================================
void CPlayer::DrawAvater(void)
{
	// ���g�`��ɕK�v
	D3DXVECTOR3 posDef = GetPos();                      // �v���C���[�̍��W(�ۑ��p)
	D3DXVECTOR3 pos = GetPos();                         // ���g�̍��W
	D3DXCOLOR ModelColorDef[4][PLAYER_MAX_MODELPARTS];  // ���f���̐F(�ۑ��p)
	D3DXCOLOR ModelCol = PLAYER_AFTERIMAGE_COLOR;       // ���g�̃��f���̐F
	CModel **pModel = GetModel();                       // ���f���N���X�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;                              // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat;                                      // �}�e���A�����̐�
	D3DXMATERIAL *pMat;                                 // �}�e���A���ւ̃|�C���^

	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// �p�[�c�������J��Ԃ�
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// �|�C���^���擾�ł���
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// �}�e���A�����̐������J��Ԃ�
				ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Diffuse = ModelCol;
			}
		}
	}

	// ���g��`�悷��
	for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
	{// ���݂̕��g�̐������J��Ԃ�
		pos += m_AvaterOffset[nCntAvater];
		SetPos(pos);
		CCharacter::Draw();
		m_AvaterModelMtxWorld[nCntAvater] = pModel[13]->GetMtxWorld();
		pos = posDef;
	}

	// �f�t�H���g�ɖ߂��Ă���
	SetPos(posDef);
	SetMtxWorld(CManager::GetRenderer()->GetDevice());
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// �p�[�c�������J��Ԃ�
		pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// �|�C���^���擾�ł���
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// �}�e���A�����̐������J��Ԃ�
				pMat[nCntMat].MatD3D.Diffuse = ModelColorDef[nCntMat][nCntParts];
			}
		}
	}

}

//=============================================================================
//    ���͂ɂ��ړ��ʌv�Z����
//=============================================================================
void CPlayer::InputMove(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���

	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �R���g���[���[���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// �J�������擾
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// �������̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_W) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// �E�����̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_W) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// ������̓��͂����ꂽ
	    // �ړ�����
		m_Move.x += sinf(pCamera->GetRot().y) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y) * m_fAccel;

		// �ړI�̊p�x�ύX
		DestRot.y = pCamera->GetRot().y + D3DX_PI;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// �������̓��͂����ꂽ
	    // �ړ�����
		m_Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;

		// �ړI�̊p�x�ύX
		DestRot.y = pCamera->GetRot().y;
	}
	else
	{
		if (pJoyStick != NULL)
		{// �W���C�X�e�B�b�N���擾�ł���
			if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
				|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
			{// ���A�i���O���͂�����Ă���
			    // �X�e�B�b�N�̓|���ꂽ�����̔䗦���v�Z
				float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

				// �R�T�C���J�[�u�Ŕ䗦��␳
				float fRivision = cosf(sqrtf(2) - fLengthX);
				if (fRivision < 1.0)
				{// �{����1�𒴂��Ă��Ȃ�
					fRivision += 0.09f;  // ����Ȃ�����␳
					if (fRivision >= 1.0f)
					{// �{����1�𒴂��Ă��܂���
						fRivision = 1.0f;
					}
				}

				// �ړ��ʌv�Z����
				m_Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivision);
				m_Move.z -= cosf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivision);

				// �ړI�̊p�x�ύX
				DestRot.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
			}
			else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
				|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
			{// �\���L�[���͂�����Ă���
			    // �ړ��ʌv�Z����
				m_Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);

				// �ړI�̊p�x�ύX
				DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			}
		}
	}

	// �e��l�̐ݒ�
	SetDestRot(DestRot);    // �ړI�̌���
}

//=============================================================================
//    �v�Z���ꂽ�ړ��ʂ����W���ɉ����鏈��
//=============================================================================
void CPlayer::Movement(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();           // ���݂̍��W
	D3DXVECTOR3 rot = GetRot();           // ���݂̌���
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���
	D3DXVECTOR3 DiffRot = GetDiffRot();   // �ړI�̌����ƌ��݂̌����̍���
	float fGravity = GetGravity();        // �d��

	// ���݂̍��W�Ɉړ��ʂ�������
	pos += m_Move;

	// �d�͂�������
	m_Move.y += fGravity;

	// �ړ��ʂɊ�����������
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// �p�x�̏C��
	DiffRot.y = DestRot.y - rot.y;   // ���݂̌����ƖړI�̌����̍������v�Z

	if (DiffRot.y > D3DX_PI)
	{// ������D3DX_PI�𒴂���
		DiffRot.y -= D3DX_PI * 2.0f;
	}
	if (DiffRot.y < -D3DX_PI)
	{// ������-D3DX_PI�𒴂���
		DiffRot.y += D3DX_PI * 2.0f;
	}

	// ���݂̌����ɖړI�̌����Ƃ̍�����{���ŕ␳����
	rot.y += DiffRot.y * m_fRivisionRot;

	if (rot.y > D3DX_PI)
	{// ���݂̌�����D3DX_PI�𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// ���݂̌�����-D3DX_PI�𒴂���
		rot.y += D3DX_PI * 2.0f;
	}

	// �e��l�̐ݒ�
	SetPos(pos);          // ���݂̍��W
	SetRot(rot);          // ���݂̌���
	SetDiffRot(DiffRot);  // �ړI�̌����ƌ��݂̌����̍���

	CDebugProc::Print("�v���C���[�̍��W : %.1f %.1f %.1f\n", pos.x, pos.y, pos.z);
	CDebugProc::Print("�v���C���[�̈ړ��� : %.1f %.1f %.1f\n", m_Move.x, m_Move.y, m_Move.z);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	bool bLand = false;     // ����Ă��邩�ǂ���
	bool bInGrass = false;  // ���ނ�̒��ɂ��邩�ǂ���

	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();          // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();    // �O��̍��W

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CObjectGoal *pObjectGoal = NULL;     // �S�[���p�z�u���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	CMeshWall *pWall = NULL;             // �ǃN���X�ւ̃|�C���^
	CItem *pItem = NULL;                 // �A�C�e���N���X�ւ̃|�C���^
	CRiver *pRiver = NULL;               // ��N���X�ւ̃|�C���^
	CEnemy *pEnemy = NULL;               // �G�N���X�ւ̃|�C���^
	CBillboardObject *pBillObj = NULL;   // �z�u�r���{�[�h�N���X�ւ̃|�C���^
	CGrass *pGrass = NULL;               // ���N���X�ւ̃|�C���^
	CBoxCollider *pBoxCollider = NULL;   // �����蔻��p�����f���N���X�ւ̃|�C���^
	CScene *pLandSceneOld = NULL;        // �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^
	bool bRiverLand = false;             // ��ɏ���Ă��邩�ǂ���
	bool bWallCol = false;               // �ǂɓ������Ă��邩�ǂ���
	float fFieldHeight = 0.0f;           // �n�ʂ̍���

	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// �`���[�g���A������������
		CTutorial *pTutorial = CManager::GetTutorial();
		if (pTutorial != NULL)
		{// �`���[�g���A���N���X���擾�ł���
			if (pos.x > -100.0f && pos.x <= 100.0f
				&& pos.z >= -25.0f)
			{
				pos.z = -25.0f;
				if (pTutorial->GetGoalCol() == false)
				{// �܂��ǂɓ������Ă��Ȃ�����
					pTutorial->SetState(CTutorial::STATE_MODECHECK);
					pTutorial->SetGoalCol(true);
				}
			}
			else
			{
				pTutorial->SetGoalCol(false);
			}
		}
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[�����Ȃ��
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			if (pGame->GetMapClear() == false)
			{// �}�b�v���N���A��Ԃł���
				pScene = CScene::GetTop(9);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == OBJTYPE_GRASS)
					{// ���N���X��������
						pGrass = (CGrass*)pScene;
						if (pGrass != NULL)
						{// ���N���X���擾�ł���
							if (pGrass->Collision(&pos, GetColRange()))
							{// ���ɓ������Ă���
								bInGrass = true;
								if (m_State == STATE_MOVE)
								{// �ړ���Ԃł���
									if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 1
										|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 3)
									{// �����o���^�C�~���O�ɂȂ���
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
									}
								}
							}
						}
					}
					// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
					pScene = pSceneNext;
				}
			}
		}
	}


	// ���̒��ɂ��邩�ǂ�������
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[�����̂ݔ���
		if (CManager::GetGame()->GetMapIdx() == 0)
		{// 1�}�b�v�ڂ̂ݔ���
			if (pos.z <= 4791.0f && pos.z >= 3695.0f && pos.x >= -2.0f && pos.x <= 855.0f && pos.y <= -10.0f)
			{// ���̒��ɂ���
				if (pos.y + GetColHeight() >= -50.0f)
				{// ���ɓ�������
					if (m_bJump == true)
					{// �W�����v���Ă���
						pos.y = -60.0f;
						Landing();
					}
				}
			}
		}
	}

	// �O�����Ă����I�u�W�F�N�g�ɍ��킹�č��������炷

	bool bFieldLand = false;
	pLandSceneOld = GetLandSceneOld();
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[������������
		if (pLandSceneOld != NULL)
		{// �|�C���^����ł͂Ȃ�
			if (pLandSceneOld->GetObjType() == OBJTYPE_MESHFIELD)
			{// �O�����Ă������̂͒n�ʂ�����
				pField = (CMeshField*)pLandSceneOld;
				D3DXVECTOR3 FieldNormal;
				if (pField != NULL)
				{// �n�ʂ̃N���X���擾�ł���
					fFieldHeight = 0.0f;
					fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, &FieldNormal);
					if (m_bJump != true)
					{// �W�����v��Ԃł͂Ȃ�
						if (bFieldLand == true)
						{// �n�ʂ̒��ɂɂ���Ɣ��肳�ꂽ
							SetLandScene((CScene*)pField);
							pos.y = fFieldHeight;
							m_Move.y = 0.0f;
							if (GetShadow() != NULL)
							{
								GetShadow()->SetVertexHeight(D3DXVECTOR3(pos.x, fFieldHeight + 1.0f, pos.z), &FieldNormal);
							}
						}
					}
				}
			}
		}
	}

	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY + 1; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (bLand != true)
			{// �܂������ɏ���Ă��Ȃ�
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// �n�ʃN���X�ւ̃|�C���^��������
					if (CManager::GetMode() == CManager::MODE_GAME)
					{// �Q�[������������
						if (bFieldLand != true)
						{// �܂��n�ʂɏ���Ă��Ȃ�
							pField = (CMeshField*)pScene;
							if (pField != NULL)
							{// �n�ʂ��擾�ł���
								D3DXVECTOR3 FieldNormal;
								fFieldHeight = 0.0f;
								fFieldHeight = pField->GetPolyHeight(pos, &bFieldLand, &FieldNormal);
								if (bFieldLand == true)
								{// �n�ʂ̒��ɂɂ���Ɣ��肳�ꂽ
									if (GetShadow() != NULL)
									{
										GetShadow()->SetVertexHeight(D3DXVECTOR3(pos.x, fFieldHeight + 1.0f, pos.z), &FieldNormal);
									}

									if (pField != pLandSceneOld && GetLandScene() != NULL && m_bJump != true)
									{// ����Ă���n�ʂ��ς����
										pos.y = fFieldHeight;
									}
									if (pos.y < fFieldHeight)
									{// ���W���n�ʂ�艺�ɂȂ���
										if (m_bJump == true)
										{// �W�����v���Ă�����
											bFieldLand = false;
											if (GetPosOld().y + 1.0f >= fFieldHeight || m_Move.y < 0.0f && pos.y < fFieldHeight)
											{
												m_Move.y = 0.0f;
												SetLandScene((CScene*)pField);
												pos.y = fFieldHeight;
												Landing();
												bFieldLand = true;
											}
										}
										else
										{// �W�����v���Ă��Ȃ�������
											SetLandScene((CScene*)pField);
											m_Move.y = 0.0f;
											pos.y = fFieldHeight;
										}
									}
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_MESHWALL)
				{// �ǃN���X�ւ̃|�C���^��������
					if (CManager::GetMode() == CManager::MODE_GAME)
					{// �Q�[������������
						pWall = (CMeshWall*)pScene;
						if (pWall != NULL)
						{// �ǂ��擾�ł���
							D3DXVECTOR3 InterPoint = pos;
							if (pWall->Collision(&pos, &posOld, &m_Move, D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange()), &InterPoint))
							{// �ǂɓ������Ă���]
								if (bWallCol == true)
								{// ���łɕǂɓ������Ă���
									pos.x = InterPoint.x;
									pos.z = InterPoint.z;
								}
								bWallCol = true;
								CDebugProc::Print("�ǂ̍��W : %.1f %.1f %.1f\n", pWall->GetPos().x, pWall->GetPos().y, pWall->GetPos().z);
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// �z�u����������
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// �z�u�����擾�ł���
						if (pObject->Collision(&pos, &posOld, &m_Move, GetColHeight(), GetColRange(), this) == true)
						{// �z�u���̏�ɏ���Ă���
							if (GetLand() == false)
							{// ���n���Ă��Ȃ�������
								SetLand(true);
								Landing();
							}

							if (GetLandScene() == NULL && GetLand() == true && m_bJump == false)
							{// �����ɏ���Ă���
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECTGOAL)
				{// �S�[���p�̔z�u����������
					pObjectGoal = (CObjectGoal*)pScene;
					if (pObjectGoal != NULL)
					{// �S�[���p�z�u�����擾�ł���
						if (pObjectGoal->Collision(&pos, &posOld, &m_Move, GetColHeight(), GetColRange(), this) == true)
						{// �S�[���z�u���ɓ������Ă���
							if (GetLand() == false)
							{// ���n���Ă��Ȃ�������
								SetLand(true);
								Landing();
							}
							SetLandScene((CScene*)pObject);
							bLand = true;

							// �}�b�v�؂�ւ��̏����ɓ���
							CGame *pGame = CManager::GetGame();
							if (pGame != NULL)
							{// �Q�[���N���X���擾�ł���
								if (pGame->GetMapClear() == false)
								{// �}�b�v���N���A��Ԃł���
									pGame->SetMapClear(true);
									if (pGame->GetMapIdx() < (pGame->GetNumMap() - 1))
									{// �܂��ǂݍ��񂾃}�b�v������
										if (pGame->GetState() != CGame::STATE_MAPCLEAR)
										{// �}�b�v�N���A��ԂɂȂ��Ă��Ȃ�
											pGame->SetState(CGame::STATE_MAPCLEAR);
										}
									}
									else
									{// ����ȏ�̓}�b�v���Ȃ�
										if (pGame->GetState() != CGame::STATE_END)
										{// �I����ԂɂȂ��Ă��Ȃ�
											pGame->SetState(CGame::STATE_END);
										}
									}
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ITEM)
				{// �A�C�e����������
					pItem = (CItem*)pScene;
					if (pItem != NULL)
					{// �A�C�e���N���X���擾�ł���
						if (pItem->Collision(&pos, GetColRange()))
						{// �A�C�e���ɓ������Ă���
							// �A�C�e���̌��ʔ���
							switch (pItem->GetType())
							{// �A�C�e���̎�ނɂ���ď����킯
							case CItem::TYPE_NORMAL:  // ����
								break;
							case CItem::TYPE_HIDDEN:  // �B���A�C�e��
								if (CManager::GetMode() == CManager::MODE_GAME)
								{// �Q�[������������
									CManager::GetGame()->SetHiddenItem(CManager::GetGame()->GetHiddenItem() + 1);
								}
								// SE���Đ�
								CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GET_HIDDEN_ITEM);
								break;
							}

							// �A�C�e��������
							pItem->Uninit();
							pItem = NULL;
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_RIVER)
				{// ��N���X�ւ̃|�C���^������
					if (bRiverLand != true)
					{// ��ɏ���Ă���
						pRiver = (CRiver*)pScene;
						if (pRiver != NULL)
						{// ��N���X�ւ̃|�C���^���擾�ł���
							float PosY = pRiver->GetPos().y;
							if (PosY < 0.0f)
							{// ��̍��W�����̒l���Ⴂ
								float fRiverHeight = pRiver->GetPolyHeight(pos, NULL, NULL);
								if (pos.y < fRiverHeight && pRiver->GetXBlock() <= 6 && fRiverHeight != 0.0f)
								{// ���艺�ɂ���
									bRiverLand = true;
									m_nEffectCounter++;
									if (m_State == STATE_MOVE)
									{// �ړ���Ԃ�����
									    // SE���Đ�
										if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 0
											|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 2)
										{// �Đ�����^�C�~���O�ɂȂ���
											CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RIVER);
										}
										if (m_nEffectCounter % 10 == 0)
										{//�G�t�F�N�g���o���^�C�~���O�ɂȂ���
											m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 13);
											m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
											if (m_nAvaterNum >= 1)
											{// ���g��1�̈ȏア��
												D3DXVECTOR3 posDef = pos;
												for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
												{// ���݂̕��g�̐������J��Ԃ�
													pos += m_AvaterOffset[nCntAvater];
													m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 13);
													m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
													pos = posDef;
												}
											}
										}
									}
									else
									{// ����ȊO�̏�Ԃ�����
										if (m_nEffectCounter % 30 == 0)
										{//�G�t�F�N�g���o���^�C�~���O�ɂȂ���
											m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
											if (m_nAvaterNum >= 1)
											{// ���g��1�̈ȏア��
												D3DXVECTOR3 posDef = pos;
												for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
												{// ���݂̕��g�̐������J��Ԃ�
													pos += m_AvaterOffset[nCntAvater];
													m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, fRiverHeight + 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12);
													pos = posDef;
												}
											}
										}
									}
									CDebugProc::Print("��ɏ���Ă���\n");
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// �G��������
					pEnemy = (CEnemy*)pScene;
					if (pEnemy != NULL)
					{// �G�N���X�ւ̃|�C���^���擾�ł���
						if (pEnemy->Collision(&pos, &posOld, &m_Move, GetColRange()))
						{// �G�ɓ������Ă���
							CDebugProc::Print("�G�ɓ������Ă��܂�\n");
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BILLBOARDOBJ)
				{// �z�u�r���{�[�h��������
					pBillObj = (CBillboardObject*)pScene;
					if (pBillObj != NULL)
					{// �z�u�r���{�[�h�N���X���擾�ł���
						if (pBillObj->Collision(&pos, GetColRange()))
						{// �z�u�r���{�[�h�ɓ������Ă���
							if (pBillObj->GetType() == CBillboardObject::TYPE_GRASS)
							{// ���p�̃r���{�[�h������
								if (m_State == STATE_MOVE)
								{// �ړ���Ԃł���
									if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 1
										|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 3)
									{// �����o���^�C�~���O�ɂȂ���
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
									}
								}
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BOXCOLLIDER)
				{// �����蔻��p�����f����������
					pBoxCollider = (CBoxCollider*)pScene;
					if (pBoxCollider != NULL)
					{// �����蔻��p�����f���N���X�ւ̃|�C���^���擾�ł���
						if (pBoxCollider->Collision(&pos, &posOld, &m_Move, D3DXVECTOR3(GetColRange(), GetColHeight(), GetColRange()), &bLand))
						{// ���ɓ������Ă���
							if (bLand == true)
							{// ���ɏ���Ă���
								m_Move.y = 0.0f;
								if (m_bJump == true)
								{// �W�����v���Ă�����
									Landing();
								}
							}
						}
					}
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	// ��ɓ����Ă��邩�ǂ�����ۑ�����
	m_bInRiver = bRiverLand;

	// ���ނ�̒��ɂ��邩�ǂ����ۑ�����
	m_bInGrass = bInGrass;

	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// �`���[�g���A����ʂ�������
		if (m_bJump != true)
		{// �W�����v���Ă��Ȃ�
			pos.y = 0.0f;
			m_bJump = false;
			SetLandScene(this);
			m_Move.y = 0.0f;
		}
		else
		{// �W�����v���Ă���
			if (m_Move.y <= 0.0f && posOld.y >= 0.0f && pos.y < 0.0f)
			{// �n�ʂɒ��n����
				m_bJump = false;
				Landing();
				pos.y = 0.0f;
				m_Move.y = 0.0f;
			}
		}
		if (GetShadow() != NULL)
		{
			GetShadow()->SetVertexHeight(D3DXVECTOR3(pos.x, 1.0f, pos.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);         // ���݂̍��W
	SetLand(bLand);      // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
}

//=============================================================================
//    �A�N�V��������
//=============================================================================
void CPlayer::Action(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���

	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �R���g���[���[���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// �J�������擾
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
	{// �W�����v�{�^���������ꂽ
		if (GetLandScene() != NULL)
		{// �����ɏ���Ă���
			if (m_bAction != true)
			{// �A�N�V�������Ă��Ȃ�
				if (m_bJump == false)
				{// �W�����v���Ă��Ȃ�
					// �O�Ղ�����
					if (m_pOrbitEffect != NULL)
					{
						m_bOrbit = false;
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
					for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
					{
						if (m_pOrbitEffectAvater[nCntAvater] != NULL)
						{
							m_pOrbitEffectAvater[nCntAvater]->Uninit();
							m_pOrbitEffectAvater[nCntAvater] = NULL;
						}
					}

					// �W�����v�ʂ������W�����v���Ă��锻��ɂ���
					m_Move.y += m_fJumpPower;
					m_bJump = true;

					// ���[�V�����؂�ւ�
					m_State = STATE_JUMP;
					GetMotionManager()->SwitchMotion(GetModel(), m_State);

					// SE�Đ�
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_JUMP);
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_SPACE) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == true)
	{// �A�N�V�����{�^���������ꂽ
		if (m_bAction == false)
		{// �A�N�V���������Ă��Ȃ�
			if (m_State == STATE_MOVE)
			{// �����Ă����Ԃ�������
				if (GetLand() == true)
				{// �����ɏ���Ă���
					CalcOffsetRot();
				}
			}

			// �A�N�V���������Ă��锻���
			m_bAction = true;

			// ���[�V�����؂�ւ�
			m_State = STATE_ATTACK_0;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);

			// �O�Ղ������Ă���
			if (m_pOrbitEffect != NULL)
			{
				m_bOrbit = false;
				m_pOrbitEffect->Uninit();
				m_pOrbitEffect = NULL;
			}
			for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
			{
				if (m_pOrbitEffectAvater[nCntAvater] != NULL)
				{
					m_pOrbitEffectAvater[nCntAvater]->Uninit();
					m_pOrbitEffectAvater[nCntAvater] = NULL;
				}
			}
		}
		else
		{// �U�������łɍs���Ă���
			if (GetMotionManager()->GetMotion()[m_State]->GetAttack() != NULL)
			{// ���̃��[�V�����ɔh���ł��郂�[�V����������
				// �e��f�[�^��p��
				int nNextMotionIdx = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetNextMotionIdx();     // ���̃��[�V�����ԍ�
 				int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetStartTiming();         // ���̃��[�V�����ɔh���ł��鎞�Ԃ��J�n����^�C�~���O
				int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetFinishTiming();       // ���̃��[�V�����ɔh���ł��鎞�Ԃ��I������^�C�~���O
				if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
				{// ���̃��[�V�����ɔh���ł���t���[�����œ��͂��s��ꂽ���\�񂪓����Ă���
					// �O�Ղ�����
					if (m_pOrbitEffect != NULL)
					{
						m_bOrbit = false;
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
					for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
					{
						if (m_pOrbitEffectAvater[nCntAvater] != NULL)
						{
							m_pOrbitEffectAvater[nCntAvater]->Uninit();
							m_pOrbitEffectAvater[nCntAvater] = NULL;
						}
					}

					// ���[�V������؂�ւ���
					m_State = (STATE)nNextMotionIdx;
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
				else if (m_bBooking == false)
				{// �܂��\�񂪓����Ă��Ȃ�
					if (GetMotionManager()->GetAllCounter() <= nFinishTiming)
					{// �܂����̃��[�V�����ɔh���ł���
						m_bBooking = true;   // �\������Ă���
					}
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_E) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_4) == true)
	{// ���b�N�I���{�^���������ꂽ
		LockOn();
	}
	else if (pKeyboard->GetTrigger(DIK_R) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_5) == true)
	{// �O�Վa�؂�ւ��{�^���������ꂽ
		if (m_bAction != true)
		{// �A�N�V���������Ă��Ȃ�
			if (m_fOrbitSlash >= 0.0f)
			{// �O�ՃQ�[�W���܂�����
				m_bOrbitSlash = m_bOrbitSlash ? false : true;
				if (m_bOrbitSlash == true && m_fOrbitSlash > 0.0f)
				{// �O�Վa���o�����ԂɂȂ���
					// SE���Đ�
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ORBITSLASH);
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_N) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_6) == true)
	{// ���g�𑝂₷�{�^���������ꂽ
		if (m_bAction != true)
		{// �A�N�V���������Ă��Ȃ�
			if (m_nLife > PLAYER_AVATER_LIFECUT)
			{// ���g���o����̗͂ł���
				m_nAvaterNum++;
				if (m_nAvaterNum > PLAYER_AVATAER_NUM)
				{// ����ȏ㕪�g�͏o���Ȃ�
					m_nAvaterNum = PLAYER_AVATAER_NUM;
				}
				else
				{// ���g���o����
					// �̗͂����
					m_nLife -= PLAYER_AVATER_LIFECUT;

					// �̗̓Q�[�W�����炷
					if (m_pLifeGauge != NULL)
					{
						m_pLifeGauge->CutGauge(PLAYER_AVATER_LIFECUT);
					}
					if (m_pEffectManager != NULL)
					{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾�ł��Ă���
						D3DXVECTOR3 EffectPos = GetPos() + m_AvaterOffset[m_nAvaterNum - 1];
						m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 26);
						m_pEffectManager->SetEffect(D3DXVECTOR3(EffectPos.x, EffectPos.y + 10.0f, EffectPos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 27);
					}

					// SE���Đ�
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_AVATER_SPAWN);

					// �J�E���^�[��߂�
					m_nAvaterCounter = 0;
				}
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_M) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_7) == true)
	{// ���g�����炷�{�^���������ꂽ
		if (m_bAction != true)
		{// �A�N�V���������Ă��Ȃ�
			m_nAvaterNum--;
			if (m_nAvaterNum < 0)
			{// ����ȏ㐔�l�����点�Ȃ�
				m_nAvaterNum = 0;
			}
			else
			{// ���g��������
				if (m_pEffectManager != NULL)
				{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾�ł��Ă���
					D3DXVECTOR3 EffectPos = GetPos() + m_AvaterOffset[m_nAvaterNum];
					m_pEffectManager->SetEffect(D3DXVECTOR3(EffectPos.x, EffectPos.y + 40.0f, EffectPos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 28);
				}
				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_AVATER_DELETE);

				// �J�E���^�[��߂�
				m_nAvaterCounter = 0;
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_B) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_1) == true)
	{// �I���N���A�^�b�N�{�^���������ꂽ
		if (m_fSpecial >= m_fMaxSpecial)
		{// �X�y�V�����Q�[�W�����^���ɂȂ��Ă���
			if (m_bAction != true && m_bJump != true)
			{// �A�N�V���������Ă��Ȃ����W�����v�����Ă��Ȃ�
				// �I���N���A�^�b�N�����Ă��锻���
				m_bOracleAttack = true;

				// �O�Վa�����Ă��Ȃ������
				m_bOrbitSlash = false;

				// ���[�V������؂�ւ���
				m_State = STATE_ORACLEATTACK;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				// �K�E�Q�[�W������Ă���
				m_fSpecial = 0.0f;
				if (m_pSpecialGauge != NULL)
				{// �|�C���^���擾�ł��Ă���
					m_pSpecialGauge->DeleteGauge();
				}

				// �G�t�F�N�g���o��
				m_pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ORACLE_START);
			}
		}
	}
	else if (pKeyboard->GetTrigger(DIK_LSHIFT) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_9) == true)
	{// ���_���Z�b�g�{�^���������ꂽ
		D3DXVECTOR3 PlayerRot = GetRot();     // �v���C���[�̌������擾

		// �������J�����p�ɕ␳����
		PlayerRot.y += D3DX_PI;
		if (PlayerRot.y > D3DX_PI)
		{// �~�����𒴂���
			PlayerRot.y -= D3DX_PI * 2.0f;
		}

		// �J�����̌������v���C���[�̐��ʕ�����
		pCamera->SetRotDest(D3DXVECTOR3(0.0f, PlayerRot.y, 0.0f));
		pCamera->SetRotDiff(pCamera->GetRotDest() - pCamera->GetRot());
	}

	if (m_bBooking == true)
	{// �A�N�V�����\�񂪓����Ă���
		CDebugProc::Print("�A�N�V�����\��");
		if (GetMotionManager()->GetMotion()[m_State]->GetAttack() != NULL)
		{// ���̃��[�V�����ɔh���ł��郂�[�V����������
		    // �e��f�[�^��p��
			int nNextMotionIdx = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetNextMotionIdx();   // ���̃��[�V�����ԍ�
			int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetStartTiming();       // ���̃��[�V�����ɔh���ł��鎞�Ԃ��J�n����^�C�~���O
			int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetFinishTiming();     // ���̃��[�V�����ɔh���ł��鎞�Ԃ��I������^�C�~���O
			if (GetMotionManager()->GetAllCounter() >= nStartTiming)
			{// ���̃��[�V�����ɔh���ł���t���[���ɂȂ��Ă���
			    // �O�Ղ�����
				if (m_pOrbitEffect != NULL)
				{
					m_bOrbit = false;
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
				for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
				{
					if (m_pOrbitEffectAvater[nCntAvater] != NULL)
					{
						m_pOrbitEffectAvater[nCntAvater]->Uninit();
						m_pOrbitEffectAvater[nCntAvater] = NULL;
					}
				}

				// ���[�V������؂�ւ���
				m_State = (STATE)nNextMotionIdx;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				// �\��͊O���Ă���
				m_bBooking = false;


				// ��������͕����ɂ���Ē���
				if (pKeyboard->GetPress(DIK_A) == true)
				{// �������̓��͂����ꂽ
					if (pKeyboard->GetPress(DIK_W) == true)
					{// �����ɏ�����̓��͂����ꂽ
					    // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
					}
					else if (pKeyboard->GetPress(DIK_S) == true)
					{// �����ɉ������̓��͂����ꂽ
					    // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
					}
					else
					{// ����������ĂȂ�
					    // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
					}
				}
				else if (pKeyboard->GetPress(DIK_D) == true)
				{// �E�����̓��͂����ꂽ
					if (pKeyboard->GetPress(DIK_W) == true)
					{// �����ɏ�����̓��͂����ꂽ
					    // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
					}
					else if (pKeyboard->GetPress(DIK_S) == true)
					{// �����ɉ������̓��͂����ꂽ
					    // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
					}
					else
					{// ����������ĂȂ�
					    // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
					}
				}
				else if (pKeyboard->GetPress(DIK_W) == true)
				{// ������̓��͂����ꂽ
				    // �ړI�̊p�x�ύX
					DestRot.y = pCamera->GetRot().y + D3DX_PI;
				}
				else if (pKeyboard->GetPress(DIK_S) == true)
				{// �������̓��͂����ꂽ
				    // �ړI�̊p�x�ύX
					DestRot.y = pCamera->GetRot().y;
				}
				else
				{
					if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
						|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
					{// ���A�i���O���͂�����Ă���
					 // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
					}
					else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
						|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
					{// �\���L�[���͂�����Ă���
					 // �ړI�̊p�x�ύX
						DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
					}
				}

				// �ړI�̊p�x�ݒ�
				SetDestRot(DestRot);
			}
		}
	}
}

//=============================================================================
//    ��ԑJ�ڏ���
//=============================================================================
void CPlayer::Statement(void)
{
	if (m_State != STATE_JUMP && m_State != STATE_LANDING && m_bAction == false && m_State != STATE_DAMAGE && m_State != STATE_ORACLEATTACK)
	{// �W�����v���Ă��Ȃ������n��ԂłȂ����A�N�V���������Ă��Ȃ����_���[�W��ԂłȂ����I���N���A�^�b�N��Ԃł͂Ȃ�
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// �ړ����Ă��Ȃ�
			if (m_State != STATE_NORMAL)
			{// �ʏ��Ԃł͂Ȃ�
				// ��Ԃ�ʏ�̏�Ԃ�
				m_State = STATE_NORMAL;

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				if (GetLand() == true)
				{// �����ɏ���Ă���
					CalcOffsetRot();
				}
			}
		}
		else
		{// �ړ����Ă���
			if (m_State != STATE_MOVE)
			{// ������Ԃł͂Ȃ�
				m_State = STATE_MOVE;

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
	else
	{// ����ȊO
		if (m_State == STATE_LANDING || m_State == STATE_DAMAGE || m_State == STATE_ORACLEATTACK)
		{// ���n��Ԃ̎����_���[�W��Ԃ̎����I���N���A�^�b�N��Ԃ̎�
			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// ���[�V�����Đ����I������
				if (m_State == STATE_DAMAGE)
				{// �_���[�W��Ԃ�������
					m_nDamageCounter = PLAYER_DAMAGE_COUNTER_MAX;
					m_bDamage = true;
				}

				m_State = STATE_NORMAL;

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}

			if (m_State == STATE_ORACLEATTACK)
			{// �I���N���A�^�b�N�̏�Ԃ�������
				if (GetMotionManager()->GetAllCounter() == 72)
				{// ������x�̎��ԂɂȂ���
					// �G�t�F�N�g���o��
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 21);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 22);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 23);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 24);
					m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 25);

					if (m_nAvaterNum >= 1)
					{// ���g��1�̈ȏア��
						D3DXVECTOR3 posDef = GetPos();   // �v���C���[�̍��W(�ۑ��p)
						D3DXVECTOR3 pos = GetPos();      // ���g�̍��W(�ۑ��p)
						for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
						{// ���݂̕��g�̐������J��Ԃ�
							// ���g�̈ʒu�ɂ��炷
							pos += m_AvaterOffset[nCntAvater];

							// �G�t�F�N�g���o��
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 21);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 22);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 23);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 24);
							m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 25);

							// �f�t�H���g�l�ɖ߂�
							pos = posDef;
						}
					}
				}

				if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
				{// �����蔻��f�[�^�N���X����������Ă���
					for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
					{// �����蔻��f�[�^�̐������J��Ԃ�
					    // �����蔻��ɕK�v�ȃf�[�^��錾
						int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();            // �����蔻����s�����f���ԍ�
						float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();                // �����蔻������͈�
						D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();      // �����蔻������ʒu(���f������̃I�t�Z�b�g)
						int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();      // �����蔻����J�n����^�C�~���O
						int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();    // �����蔻����I������^�C�~���O
						int nDamage = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetDamage();                // ���������ۂ̃_���[�W��

						// �͈͂��g�債�Ă���
						fRange += (GetMotionManager()->GetAllCounter() - 50) * 10.0f;

						// ����J�n
						if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
						{// �����蔻��𔭐�������^�C�~���O�Ȃ��
						    // �U�����̓����蔻��𔭐�������
							AttackCollision(nModelIdx, fRange, Offset, nDamage);

							if (m_nAvaterNum >= 1)
							{// ���g��1�̈ȏア��
								CModel **pModel = GetModel();    // ���f���N���X�ւ̃|�C���^
								D3DXVECTOR3 posDef = GetPos();   // �v���C���[�̈ʒu(�ۑ��p)
								D3DXVECTOR3 pos = GetPos();      // ���g�̈ʒu
								for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
								{// ���݂̕��g�̐������J��Ԃ�
									// ���g�̈ʒu���v�Z
									pos += m_AvaterOffset[nCntAvater];
									SetPos(pos);
									SetMtxWorld(CManager::GetRenderer()->GetDevice());
									for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
									{// �p�[�c�������J��Ԃ�
										pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
									}

									// ���g�̓����蔻����J�n
									AttackCollision(nModelIdx, fRange, Offset, nDamage);

									// �f�t�H���g�l�ɖ߂�
									pos = posDef;
								}

								// �f�t�H���g�̒l�ɖ߂�
								SetPos(posDef);
								SetMtxWorld(CManager::GetRenderer()->GetDevice());
								for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
								{// �p�[�c�������J��Ԃ�
									pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
								}
							}
						}

						// SE���Đ�
						if (GetMotionManager()->GetAllCounter() == nStartTiming)
						{// �����o���^�C�~���O�ɂȂ���
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ORACLEATTACK);
						}
					}
				}
			}
		}
		if (m_bAction == true)
		{// �A�N�V���������Ă���
			if (m_State == STATE_ATTACK_0)
			{// 1�ڂ̍U�����[�V����������
				if (GetMotionManager()->GetAllCounter() >= 25 && GetMotionManager()->GetAllCounter() <= 28)
				{// ������x���[�V�������i�s���Ă���
					// �I�u�W�F�N�g�Ƃ̔�����s��
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// ����������ɂȂ�܂ŌJ��Ԃ�
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// �z�u���N���X�ւ̃|�C���^������
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// �z�u���N���X�̃|�C���^���擾�ł���
							    // �z�u���̓����蔻������͈͂��擾
								fObjRange = pObject->GetColRange();

								// �����蔻�����钷�����v�Z
								fHitRange = sqrtf((fObjRange + 15.0f) * (fObjRange + 15.0f));

								// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����𑪂�
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);
								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����������蔻�����钷�����Z��
									if (pObject->GetHitAttack() == false)
									{// �܂��������Ă��Ȃ�����
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SE���Đ�
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}

				if (GetMotionManager()->GetAllCounter() == 22)
				{// ������x�̃J�E���^�[�̒l�ɂȂ��Ă���
					if (m_bInGrass == true)
					{// ���ނ�̒��ɂ���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// �O�Վa�������Ԃł���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
						m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}
						// SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
					}
					else
					{// �O�Վa�������Ԃł͂Ȃ�
					    // SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 18)
				{// ������x���[�V�������i�s���Ă���
					m_Move.x -= sinf(GetRot().y) * 20.0f;
					m_Move.z -= cosf(GetRot().y) * 20.0f;
				}
			}
			else if (m_State == STATE_ATTACK_1)
			{// 2�ڂ̍U�����[�V����������
				if (GetMotionManager()->GetAllCounter() >= 13 && GetMotionManager()->GetAllCounter() <= 15)
				{// ������x���[�V�������i�s���Ă���
					// �I�u�W�F�N�g�Ƃ̔�����s��
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// ����������ɂȂ�܂ŌJ��Ԃ�
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// �z�u���N���X�ւ̃|�C���^������
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// �z�u���N���X�̃|�C���^���擾�ł���
							    // �z�u���̓����蔻������͈͂��擾
								fObjRange = pObject->GetColRange();

								// �����蔻�����钷�����v�Z
								fHitRange = sqrtf((fObjRange + 25.0f) * (fObjRange + 25.0f));

								// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����𑪂�
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);
								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����������蔻�����钷�����Z��
									if (pObject->GetHitAttack() == false)
									{// �܂��������Ă��Ȃ�����
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SE���Đ�
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}

				if (GetMotionManager()->GetAllCounter() == 9)
				{// ������x���[�V�������i�s���Ă���
					if (m_bInGrass == true)
					{// ���ނ�̒��ɂ���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// �O�Վa�������Ԃł���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
						m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);


						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}
						// SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
					}
					else
					{// �O�Վa�������Ԃł͂Ȃ�
					    // SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 7)
				{// ������x���[�V�������i�s���Ă���
					m_Move.x -= sinf(GetRot().y) * 10.0f;
					m_Move.z -= cosf(GetRot().y) * 10.0f;
				}
			}
			else if (m_State == STATE_ATTACK_2)
			{// 3�ڂ̍U�����[�V����������
				if (GetMotionManager()->GetAllCounter() == 30)
				{
					// �I�u�W�F�N�g�Ƃ̔�����s��
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// ����������ɂȂ�܂ŌJ��Ԃ�
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// �z�u���N���X�ւ̃|�C���^������
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// �z�u���N���X�̃|�C���^���擾�ł���
							    // �z�u���̓����蔻������͈͂��擾
								fObjRange = pObject->GetColRange();

								// �����蔻�����钷�����v�Z
								fHitRange = sqrtf((fObjRange + 15.0f) * (fObjRange + 15.0f));

								// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����𑪂�
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);
								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����������蔻�����钷�����Z��
									if (pObject->GetHitAttack() == false)
									{// �܂��������Ă��Ȃ�����
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SE���Đ�
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}
				if (GetMotionManager()->GetAllCounter() == 28)
				{// ������x���[�V�������i�s���Ă���
					if (m_bInGrass == true)
					{// ���ނ�̒��ɂ���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// �O�Վa�������Ԃł���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
						m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 30.0f, GetPos().z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);

						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 14);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}

						// SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASH_SWING);
					}
					else
					{// �O�Վa�������Ԃł͂Ȃ�
					    // SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 25)
				{// ������x���[�V�������i�s���Ă���
					m_Move.x -= sinf(GetRot().y) * 20.0f;
					m_Move.z -= cosf(GetRot().y) * 20.0f;
				}
			}
			if (m_State == STATE_ATTACK_3)
			{// �Ō�̍U�����[�V����������
				if (GetMotionManager()->GetAllCounter() >= 38 && GetMotionManager()->GetAllCounter() <= 40)
				{// ������x���[�V�������i�s���Ă���
					// �I�u�W�F�N�g�Ƃ̔�����s��
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					CObject *pObject = NULL;
					pScene = GetTop(OBJECT_PRIORITY);
					float fObjRange = 0.0f;
					float fHitRange = 0.0f;
					float fLength = 0.0f;
					D3DXMATRIX mtxWorld;
					D3DXVECTOR3 OffsetObj = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CModel *pModel = GetModel(13);
					if (pModel != NULL)
					{
						mtxWorld = pModel->GetMtxWorld();
					}
					D3DXVec3TransformCoord(&OffsetObj, &OffsetObj, &mtxWorld);
					while (pScene != NULL)
					{// ����������ɂȂ�܂ŌJ��Ԃ�
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == OBJTYPE_OBJECT)
						{// �z�u���N���X�ւ̃|�C���^������
							pObject = (CObject*)pScene;
							if (pObject != NULL)
							{// �z�u���N���X�̃|�C���^���擾�ł���
							    // �z�u���̓����蔻������͈͂��擾
								fObjRange = pObject->GetColRange();

								// �����蔻�����钷�����v�Z
								fHitRange = sqrtf((fObjRange + 30.0f) * (fObjRange + 30.0f));

								// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����𑪂�
								D3DXVECTOR3 ObjectPos = D3DXVECTOR3(pObject->GetPos().x, pObject->GetPos().y + 30.0f, pObject->GetPos().z);

								fLength = sqrtf((ObjectPos.x - OffsetObj.x) * (ObjectPos.x - OffsetObj.x) + (ObjectPos.y - OffsetObj.y) * (ObjectPos.y - OffsetObj.y) + (ObjectPos.z - OffsetObj.z) * (ObjectPos.z - OffsetObj.z));

								if (fLength <= fHitRange)
								{// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����������蔻�����钷�����Z��
									if (pObject->GetHitAttack() == false)
									{// �܂��������Ă��Ȃ�����
										pObject->SetHitAttack(true);
										pObject->SetHitCounter(20);
										if (m_pEffectManager != NULL)
										{
											m_pEffectManager->SetEffect(OffsetObj, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32);
										}
										// SE���Đ�
										CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT_OBJECT);
									}
								}
							}
						}
						pScene = pSceneNext;
					}
				}

				// ������ׂ��^�C�~���O�ŃG�t�F�N�g���o��
				if (GetMotionManager()->GetAllCounter() == 35)
				{// ������x���[�V�������i�s���Ă���
					if (m_bInGrass == true)
					{// ���ނ�̒��ɂ���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 31);
								pos = posDef;
							}
						}
					}

					if (m_bOrbitSlash == true)
					{// �O�Վa�������Ԃł���
						D3DXVECTOR3 posDef = GetPos();
						D3DXVECTOR3 pos = GetPos();
						m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 45.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 75.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 90.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 105.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
						m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 120.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);


						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								pos += m_AvaterOffset[nCntAvater];
								m_pEffectManager->SetEffect(pos + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 11);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 15.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 45.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 75.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 90.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 105.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								m_pEffectManager->SetEffect(D3DXVECTOR3(pos.x, pos.y + 120.0f, pos.z) + (m_Move * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
								pos = posDef;
							}
						}

						// SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK);
					}
					else
					{// �O�Վa�������Ԃł͂Ȃ�
						// SE���Đ�
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SWING);
					}
				}
				if (GetMotionManager()->GetAllCounter() == 34)
				{
					m_Move.x -= sinf(GetRot().y) * 25.0f;
					m_Move.z -= cosf(GetRot().y) * 25.0f;
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
			{// �����蔻��f�[�^�N���X����������Ă���
				for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
				{// �����蔻��f�[�^�̐������J��Ԃ�
					// �����蔻��ɕK�v�ȃf�[�^��錾
					int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();          // �����蔻�����郂�f���ԍ�
					float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();              // �����蔻������͈�
					D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();    // �����蔻������ʒu(���f������̃I�t�Z�b�g)
					int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();    // �����蔻����J�n����^�C�~���O
					int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();  // �����蔻����I������^�C�~���O
					int nDamage = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetDamage();              // ���������ۂ̃_���[�W��

					if (m_State == STATE_ATTACK_3)
					{// �Ō�̍U�����[�V����������
						if (m_bOrbitSlash == true)
						{// �O�Վa�������Ԃ�������
							fRange = 12.0f * (GetMotionManager()->GetAllCounter() - nStartTiming);
						}
						else
						{// �O�Վa�����Ȃ���Ԃ�������
							fRange = 50.0f;
							nDamage = 1;
						}
					}

					// ����J�n
					if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
					{// �����蔻��𔭐�������^�C�~���O�Ȃ��
						// �U�����̓����蔻��𔭐�������
						AttackCollision(nModelIdx, fRange, Offset, nDamage);

						if (m_nAvaterNum >= 1)
						{// ���g��1�̈ȏア��
							CModel **pModel = GetModel();
							D3DXVECTOR3 posDef = GetPos();
							D3DXVECTOR3 pos = GetPos();
							for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
							{// ���݂̕��g�̐������J��Ԃ�
								// ���g�̈ʒu���v�Z
								pos += m_AvaterOffset[nCntAvater];
								SetPos(pos);
								SetMtxWorld(CManager::GetRenderer()->GetDevice());
								for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
								{// �p�[�c�������J��Ԃ�
									pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
								}

								// ���g�̓����蔻��J�n
								AttackCollision(nModelIdx, fRange, Offset, nDamage);

								// �f�t�H���g�l�ɖ߂�
								pos = posDef;
							}

							// �f�t�H���g�̒l�ɖ߂�
							SetPos(posDef);
							SetMtxWorld(CManager::GetRenderer()->GetDevice());
							for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
							{// �p�[�c�������J��Ԃ�
								pModel[nCntParts]->SetMtxWorld(CManager::GetRenderer()->GetDevice());
							}
						}
					}
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetOrbit() != NULL)
			{// �O�Ճf�[�^�N���X����������Ă���
				if (m_bOrbitSlash == true)
				{// �O�Վa���o����Ԃł���
					if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetStartTiming() && m_bOrbit == false)
					{// �O�Ղ��o���^�C�~���O�ɂȂ���
						if (m_pOrbitEffect == NULL)
						{// �I�[�r�b�g�G�t�F�N�g�N���X�ւ̃|�C���^����ɂȂ��Ă���
							// �O�ՂɕK�v�ȏ���錾
							int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetModelIdx();                // �O�Ղ��o�����f���ԍ�
							int nTexIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetTexIdx();                    // �O�Ղɒ���t����e�N�X�`���̔ԍ�
							D3DXVECTOR3 OffSetPos1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos1();     // �O�Ղ̃I�t�Z�b�g(����)
							D3DXVECTOR3 OffSetPos2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos2();     // �O�Ղ̃I�t�Z�b�g(�㑤)
							D3DXVECTOR3 OffSetAmp1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp1();     // �O�Ղ̃I�t�Z�b�g���ǂꂭ�炢��������(����)
							D3DXVECTOR3 OffSetAmp2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp2();     // �O�Ղ̃I�t�Z�b�g���ǂꂭ�炢��������(�㑤)
							D3DXCOLOR ColUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColUp();                 // �O�Ղ̐F(�㑤)
							D3DXCOLOR ColDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColDown();             // �O�Ղ̐F(����)
							float fAlphaDecayUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayUp();      // �O�Ղ̓����x�����l(�㑤)
							float fAlphaDecayDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayDown();  // �O�Ղ̓����x�����l(����)
							int nXBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetXBlock();                    // �O�Ղ̉��̕�����
							int nYBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetYBlock();                    // �O�Ղ̏c�̕�����
							D3DXMATRIX *pMtxParent = GetModel(nModelIdx)->GetMtxWorldPointer();                                    // �O�Ղ���郏�[���h�}�g���b�N�X�ւ̃|�C���^
							m_pOrbitEffect = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, pMtxParent, EFFECT_PRIORITY);
							if (m_pOrbitEffect != NULL)
							{// �O�Ղ𐶐��ł���
								m_pOrbitEffect->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
							}
							if (m_nAvaterNum >= 1)
							{// ���g���P�̈ȏ�o�Ă���
								for (int nCntAvater = 0; nCntAvater < m_nAvaterNum; nCntAvater++)
								{// ���݂̕��g�̐������J��Ԃ�
									m_pOrbitEffectAvater[nCntAvater] = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, &m_AvaterModelMtxWorld[nCntAvater], EFFECT_PRIORITY);
									if (m_pOrbitEffectAvater[nCntAvater] != NULL)
									{// �O�Ղ𐶐��ł���
										m_pOrbitEffectAvater[nCntAvater]->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
									}
								}
							}
							m_bOrbit = true;
						}
					}
				}
				if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetFinishTiming())
				{// �O�Ղ����܂��^�C�~���O�ɂȂ���
					if (m_pOrbitEffect != NULL)
					{
						m_bOrbit = false;
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
					for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
					{// ���g���o���鐔�����J��Ԃ�
						if (m_pOrbitEffectAvater[nCntAvater] != NULL)
						{
							m_pOrbitEffectAvater[nCntAvater]->Uninit();
							m_pOrbitEffectAvater[nCntAvater] = NULL;
						}
					}
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// ���[�V�����Đ����I������
				m_bAction = false;
				m_bOrbit = false;
				m_State = STATE_NORMAL;
				m_bBooking = false;

				// �O�Ղ��J�����Ă���
				if (m_pOrbitEffect != NULL)
				{
					m_bOrbit = false;
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
				for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
				{// ���g���o���鐔�����J��Ԃ�
					if (m_pOrbitEffectAvater[nCntAvater] != NULL)
					{
						m_pOrbitEffectAvater[nCntAvater]->Uninit();
						m_pOrbitEffectAvater[nCntAvater] = NULL;
					}
				}

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}

	if (m_bLockOn == true)
	{// ���b�N�I������������
		if (m_pLockOnEnemy != NULL && m_pLockOnPoly != NULL)
		{// ���b�N�I���Ώۂ̓G�N���X�ւ̃|�C���^�ƃ��b�N�I���|���S���ւ̃|�C���^���擾�ł��Ă���
			D3DXVECTOR3 LocOnPolyPos = D3DXVECTOR3(m_pLockOnEnemy->GetPos().x, m_pLockOnEnemy->GetPos().y + PLAYER_LOCKONPOLY_HEIGHT, m_pLockOnEnemy->GetPos().z);
			m_pLockOnPoly->SetPos(LocOnPolyPos);
		}
	}

	if (m_bOrbitSlash == true)
	{// �O�Վa�������ԂȂ��
		m_fOrbitSlash -= PLAYER_ORBIT_CUTVALUE;
		m_nOrbitCounter = 0;
		if (m_fOrbitSlash <= 0.0f)
		{// �O�ՃQ�[�W�ʂ��Ȃ��Ȃ���
			m_fOrbitSlash = 0.0f;
			m_bOrbitSlash = false;
			if (m_pOrbitSlashGauge != NULL)
			{// ���������m�ۂł��Ă���
				m_pOrbitSlashGauge->DeleteGauge();
			}
		}
		else
		{// �܂��O�ՃQ�[�W�ʂ�����
			if (m_pOrbitSlashGauge != NULL)
			{// ���������m�ۂł��Ă���
				m_pOrbitSlashGauge->CutGauge(PLAYER_ORBIT_CUTVALUE);
			}
		}
	}
}


//=============================================================================
//    ���n�����Ƃ��̏���
//=============================================================================
void CPlayer::Landing(void)
{
	// �W�����v���Ă��Ȃ���Ԃ�
	m_bJump = false;

	if (m_bAction == false)
	{// �A�N�V���������Ă��Ȃ�
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// �ړ����Ă��Ȃ�
			// ���n�̃��[�V�����ɐ؂�ւ���
			m_State = STATE_LANDING;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);

			// SE���Đ�
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_LANDING);
		}
		else
		{// �ړ����Ă���
			m_State = STATE_MOVE;

			// ���[�V�����؂�ւ�����
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}

	if (GetLand() == true)
	{// �I�t�Z�b�g�ʒu�����[���h�}�g���b�N�X���g���Ċ���o��
		CalcOffsetRot();
	}
}

//=============================================================================
//    �U�����̓����蔻�菈��
//=============================================================================
bool CPlayer::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage)
{
	bool bHit = false;                 // �U�����q�b�g�������ǂ���
	float fEnemyRange = 0.0f;          // �G�̓����蔻������͈�
	D3DXVECTOR3 OffSet = Offset;       // �����蔻����Ƃ�ꏊ(�e���f������̃I�t�Z�b�g)
	float fHitRange = 0.0f;            // �����蔻�����钷��
	float fLength = 0.0f;              // �����蔻������ꏊ�ƓG�Ƃ̋���
	D3DXMATRIX mtxWorld;
	if (nDamage == 1)
	{// �_���[�W��������
		if (m_bOrbitSlash == true)
		{// �O�Վa�����Ă�����
			nDamage += 6;
		}
	}

	if (m_nAvaterNum >= 1)
	{// ���g��1�̈ȏ�o�Ă���
		if (m_bOrbitSlash == true)
		{// �O�Վa���o����Ԃł���
			nDamage += m_nAvaterNum;
		}
		else
		{// �O�Վa���o����ԂłȂ�
			nDamage += (m_nAvaterNum / 2) / 2;
		}
	}

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

	// ����J�n
	if (m_bOrbitSlash == true || m_bOrbitSlash != true && Offset.x < 100.0f && Offset.y < 100.0f && Offset.z > -100.0f)
	{// �O�Վa�����Ȃ���Ԃ��I�t�Z�b�g�����ꂷ���Ă��Ȃ��܂��͋O�Վa�������Ԃł���
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CEnemy *pEnemy = NULL;
		pScene = GetTop(ENEMY_PRIORITY);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G�N���X�ւ̃|�C���^������
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// �G�N���X�̃|�C���^���擾�ł���
					// �G�̓����蔻������͈͂��擾
					fEnemyRange = pEnemy->GetColRange();

					// �����蔻�����钷�����v�Z
					fHitRange = sqrtf((fEnemyRange + fRange) * (fEnemyRange + fRange));

					// �����蔻��������W�ƓG�̍��W�Ƃ̋����𑪂�
					D3DXMATRIX EnemyMtxWorld;
					D3DXVECTOR3 EnemyPos;
					CModel *pModel = pEnemy->GetModel(0);
					if (pModel != NULL)
					{
						EnemyMtxWorld = pModel->GetMtxWorld();
						EnemyPos = D3DXVECTOR3(EnemyMtxWorld._41, EnemyMtxWorld._42 + 10.0f, EnemyMtxWorld._43);
						fLength = sqrtf((EnemyPos.x - OffSet.x) * (EnemyPos.x - OffSet.x) + (EnemyPos.y - OffSet.y) * (EnemyPos.y - OffSet.y) + (EnemyPos.z - OffSet.z) * (EnemyPos.z - OffSet.z));
					}

					if (fLength <= fHitRange)
					{// �����蔻��������W�ƓG�̍��W�Ƃ̋����������蔻�����钷�����Z��
						if (pEnemy->GetDamage() != true && pEnemy->GetState() != CEnemy::STATE_DEATH)
						{// �G�̏�Ԃ��_���[�W��ԂłȂ�
							bool bDeath = false;
							D3DXVECTOR3 EffectPos = D3DXVECTOR3(pEnemy->GetModel()[0]->GetMtxWorld()._41, pEnemy->GetModel()[0]->GetMtxWorld()._42, pEnemy->GetModel()[0]->GetMtxWorld()._43);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_SLASH);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_SLASHPAR);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9);
							bDeath = pEnemy->Hit(nDamage);

							// SE���Đ�
							if (m_bOrbitSlash != true)
							{// �O�Վa�������Ԃł͂Ȃ�
								if (m_State != STATE_ORACLEATTACK)
								{// �I���N���A�^�b�N��Ԃł͂Ȃ�
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_HIT);
								}
								else
								{// �I���N���A�^�b�N�����Ă�����

								}
							}
							else
							{// �O�Վa�������ԂȂ�
								if (m_State != STATE_ATTACK_3)
								{// �Ō�̍U���ȊO��������
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHHIT);
								}
								else
								{// �Ō�̍U����������
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK);
								}
							}

							if (CManager::GetMode() == CManager::MODE_GAME)
							{// �Q�[������������
								CUI *pUi = CManager::GetGame()->GetUI();
								if (pUi != NULL)
								{// ���������m�ۂł���
									CCombo *pCombo = CManager::GetGame()->GetUI()->GetCombo();
									if (pCombo != NULL)
									{// ���������m�ۂ���Ă���
										pCombo->SetCombo(pCombo->GetCombo() + 1);
									}
									else if (pCombo == NULL)
									{// ���������m�ۂ���Ă��Ȃ�
										CManager::GetGame()->GetUI()->CreateCombo();
									}
								}
							}

							if (m_bOrbitSlash != true)
							{// �O�Վa���o����ԂłȂ�
								if (m_State != STATE_ORACLEATTACK)
								{// �I���N���A�^�b�N�Ŏ���ł��Ȃ�
									if (m_fSpecial < m_fMaxSpecial)
									{// �ő�l�ɒB���Ă��Ȃ�
										float fAddValue = PLAYER_SPECIALGAUGE_ORBITADD;
										m_fSpecial += fAddValue;
										if (m_fSpecial >= m_fMaxSpecial)
										{// �ő�l�ɒB����
											m_fSpecial = m_fMaxSpecial;
											if (m_pSpecialGauge != NULL)
											{// �K�E�Q�[�W���擾�ł��Ă���
												m_pSpecialGauge->MaxGauge();
											}
											// SE���Đ�
											CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_MAXORACLE);
										}
										else
										{// �ő�l�ɒB���Ă��Ȃ�
											if (m_pSpecialGauge != NULL)
											{// �K�E�Q�[�W���擾�ł��Ă���
												m_pSpecialGauge->AddGauge(fAddValue);
											}
										}
									}
								}

							}

							if (bDeath == true)
							{// ����ł���
								if (CManager::GetMode() == CManager::MODE_GAME)
								{// �Q�[�����ł���
									if (CManager::GetGame()->GetState() == CGame::STATE_KNOCKDOWNBOSS)
									{// �{�X��|������Ԃ�������
									    // ���b�N�I���̃|���S�����J������
										if (m_pLockOnPoly != NULL)
										{
											m_pLockOnPoly->Uninit();
											m_pLockOnPoly = NULL;
										}
									}
								}

								if (m_State != STATE_ORACLEATTACK)
								{// �I���N���A�^�b�N�Ŏ���ł��Ȃ�
									if (m_fSpecial < m_fMaxSpecial)
									{// �ő�l�ɒB���Ă��Ȃ�
										float fAddValue = PLAYER_SPECIALGAUGE_ADD;
										m_fSpecial += fAddValue;
										if (m_fSpecial >= m_fMaxSpecial)
										{// �ő�l�ɒB����
											m_fSpecial = m_fMaxSpecial;
											if (m_pSpecialGauge != NULL)
											{// �K�E�Q�[�W���擾�ł��Ă���
												m_pSpecialGauge->MaxGauge();
											}
											// SE���Đ�
											CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_MAXORACLE);
										}
										else
										{// �ő�l�ɒB���Ă��Ȃ�
											if (m_pSpecialGauge != NULL)
											{// �K�E�Q�[�W���擾�ł��Ă���
												m_pSpecialGauge->AddGauge(fAddValue);
											}
										}
									}
								}
							}

							bHit = true;
						}
						CDebugProc::Print("�U�����q�b�g���Ă��܂�\n");
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	return bHit;  // �q�b�g�������ǂ�����Ԃ�
}

//=============================================================================
//    �G�t�F�N�g����鏈��
//=============================================================================
void CPlayer::CreateEffect(void)
{
	if (m_pEffectManager != NULL)
	{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������o���Ă���
		if (m_State == STATE_MOVE)
		{// ��Ԃ�������Ԃł���
			if (m_bInRiver != true)
			{// ��ɓ����Ă��Ȃ�
				if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 0
					|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 2)
				{// �G�t�F�N�g�����^�C�~���O�ɂȂ���
					// SE���Đ�
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
					m_pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_DASH);
				}
			}
		}

		if (m_bOrbitSlash == true)
		{// �O�Վa�������Ԃł���
			m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 5.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
		}

		if (m_fSpecial >= m_fMaxSpecial)
		{// �K�E�Z�Q�[�W�����܂��Ă���
			m_pEffectManager->SetEffect(D3DXVECTOR3(GetPos().x, GetPos().y + 5.0f, GetPos().z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 29);
		}
	}
}

//=============================================================================
//    ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�������v�Z���鏈��
//=============================================================================
void CPlayer::CalcOffsetRot(void)
{
	if (GetLandScene() != NULL)
	{// �v���C���[������Ă���N���X�ւ̃|�C���^���擾�ł��Ă���
		if (GetLandScene()->GetObjType() == OBJTYPE_OBJECT)
		{// �z�u���N���X��������
			CObject *pObject = (CObject*)GetLandScene();
			D3DXVECTOR3 ObjRot = pObject->GetRot();
			D3DXVECTOR3 OffSetRot;
			OffSetRot = GetRot() - ObjRot;
			if (OffSetRot.y > D3DX_PI)
			{// �~�����𒴂���
				OffSetRot.y -= D3DX_PI * 2.0f;
			}
			if (OffSetRot.y < -D3DX_PI)
			{// �~�����𒴂���
				OffSetRot.y += D3DX_PI * 2.0f;
			}
			SetLandOffsetRot(OffSetRot);
		}
	}
}

//=============================================================================
//    �_���[�W���󂯂��Ƃ��̏���
//=============================================================================
void CPlayer::Damage(int nDamage)
{
	if (m_bDamage == false)
	{// �_���[�W��������Ă��Ȃ��Ȃ�
		bool bDeath = false;   // ���񂾂��ǂ���

		if (m_State != STATE_ORACLEATTACK)
		{// �I���N���A�^�b�N���łȂ����
			// ���g�̐������_���[�W��傫������
			if (m_nAvaterNum >= 1)
			{// ���g��1�̈ȏ�o�Ă���
				nDamage += m_nAvaterNum + (m_nAvaterNum / 2);
			}

			// �̗͂����炷
			m_nLife -= nDamage;

			// �G�t�F�N�g���o��
			if (m_pEffectManager != NULL)
			{
				m_pEffectManager->SetEffect(D3DXVECTOR3(GetModel()[0]->GetMtxWorld()._41, GetModel()[0]->GetMtxWorld()._42, GetModel()[0]->GetMtxWorld()._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10);
			}

			// �̗̓Q�[�W�����炷
			if (m_pLifeGauge != NULL)
			{
				m_pLifeGauge->CutGauge(nDamage);
			}

			if (m_nLife <= 0)
			{// 0���������
				m_nLife = 0;

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DAMAGE000);

				// �v���C���[������
				Uninit();

				if (CManager::GetMode() == CManager::MODE_GAME)
				{// �Q�[������������
					CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM002);
					CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM003);
					CGame *pGame = CManager::GetGame();
					if (pGame != NULL)
					{// �Q�[���N���X���擾�ł���
						pGame->SetPlayer(NULL);
						if (pGame->GetState() != CGame::STATE_END)
						{// �I����ԂłȂ�
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM004);
							pGame->SetState(CGame::STATE_END);
						}
					}
				}
			}
			else
			{// �̗͂��܂�����
				// �_���[�W��Ԃ�
				m_State = STATE_DAMAGE;

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				// �S�ẴA�N�V�����̏�Ԃ�߂�
				m_bAction = false;
				m_bBooking = false;
				m_bJump = false;
				m_bOrbit = false;

				// �O�Ղ�����
				if (m_pOrbitEffect != NULL)
				{
					m_bOrbit = false;
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
				for (int nCntAvater = 0; nCntAvater < PLAYER_AVATAER_NUM; nCntAvater++)
				{// ���g���o���鐔�����J��Ԃ�
					if (m_pOrbitEffectAvater[nCntAvater] != NULL)
					{
						m_pOrbitEffectAvater[nCntAvater]->Uninit();
						m_pOrbitEffectAvater[nCntAvater] = NULL;
					}
				}

				// SE���Đ�
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DAMAGE000);
			}
		}
	}
}

//=============================================================================
//    �߂��̓G�����b�N�I�����鏈��
//=============================================================================
void CPlayer::LockOn(void)
{
	if (m_bLockOn == false)
	{// �܂����b�N�I�����Ă��Ȃ�
	    // ����J�n
		float fDistance = 0.0f;         // �G�Ƃ̋���
		float fDistanceMin = 10000.0f;  // ��ԋ߂��G�Ƃ̋���
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CEnemy *pEnemy = NULL;
		pScene = GetTop(ENEMY_PRIORITY);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G�N���X�ւ̃|�C���^������
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// �G�N���X�̃|�C���^���擾�ł���
					D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
					D3DXVECTOR3 PlayerPos = GetPos();

					// �G�Ƃ̋������v�Z
					fDistance = sqrtf((EnemyPos.x - PlayerPos.x) * (EnemyPos.x - PlayerPos.x) + (EnemyPos.y - PlayerPos.y) * (EnemyPos.y - PlayerPos.y) + (EnemyPos.z - PlayerPos.z) * (EnemyPos.z - PlayerPos.z));

					if (fDistance <= fDistanceMin)
					{// ��ԋ߂��G�Ƃ̋������X�V���ꂽ
						fDistanceMin = fDistance;
						m_pLockOnEnemy = pEnemy;
					}
				}
			}
			pScene = pSceneNext;
		}

		if (fDistanceMin <= PLAYER_LOCKON_DISTANCE)
		{// ������������x�͈͓̔���������
			if (m_pLockOnPoly == NULL && m_pLockOnEnemy != NULL)
			{// ���������擾�ł����Ԃł���
				if (m_pLockOnEnemy->GetState() != CEnemy::STATE_DEATH)
				{// �G������ł��Ȃ�������
				    // ���b�N�I�����J�n����
					m_bLockOn = true;
					D3DXVECTOR3 LocOnPolyPos = D3DXVECTOR3(m_pLockOnEnemy->GetPos().x, m_pLockOnEnemy->GetPos().y + PLAYER_LOCKONPOLY_HEIGHT, m_pLockOnEnemy->GetPos().z);
					m_pLockOnPoly = CSceneBillboard::Create(LocOnPolyPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, PLAYER_LOCKONPOLY_WIDTH, PLAYER_LOCKONPOLY_HEIGHT, false, PLAYER_LOCKON_POLY_PRIORITY);
					if (m_pLockOnPoly != NULL)
					{// ���������m�ۂł���
						m_pLockOnPoly->BindTexture(GetTextureManager()->GetTexture(2));
					}

					// SE���Đ�
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_LOCKON);
				}
				else
				{// �G������ł����ԂȂ��
					m_pLockOnEnemy = NULL;
				}
			}
		}
		else
		{// ���������ꂷ���Ă���
			// ���b�N�I���Ώۂ̓G�N���X�ւ̃|�C���^����ɂ��Ă���
			m_pLockOnEnemy = NULL;
		}
	}
	else if (m_bLockOn == true)
	{// ���b�N�I�����Ă���Ȃ��
		LockLost();
	}
}

//=============================================================================
//    ���b�N�I�����������鏈��
//=============================================================================
void CPlayer::LockLost(void)
{
	// ���b�N�I������������
	m_bLockOn = false;

	// ���b�N�I���Ώۂ̓G�N���X�ւ̃|�C���^����ɂ��Ă���
	m_pLockOnEnemy = NULL;

	// ���b�N�I���|���S���̊J��
	if (m_pLockOnPoly != NULL)
	{// ���������m�ۂ���Ă���
		m_pLockOnPoly->Uninit();
		m_pLockOnPoly = NULL;
	}
}

//=============================================================================
//    �c���`��p�ɍ��W�������ۑ����鏈��
//=============================================================================
void CPlayer::StackAfterImage(void)
{
	// �z��̍Ō���ԍ��𑝂₷
	m_nCurrentStack++;
	if (m_nCurrentStack >= PLAYER_AFTERIMAGE_NUM)
	{// ����ȏ�X�^�b�N�ł��Ȃ�
		m_nCurrentStack = PLAYER_AFTERIMAGE_NUM - 1;
	}

	// �z��̍Ō���ɒl��ۑ�
	CModel **pModel = GetModel();
	m_AfterPos[m_nCurrentStack] = GetPos();
	m_AfterRot[m_nCurrentStack] = GetRot();
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// ���f���̃p�[�c�����J��Ԃ�
		m_AfterModelPos[m_nCurrentStack][nCntParts] = pModel[nCntParts]->GetAddPos();
		m_AfterModelRot[m_nCurrentStack][nCntParts] = pModel[nCntParts]->GetRot();
	}
}

//=============================================================================
//    �c����`�悷�鏈��
//=============================================================================
void CPlayer::DrawAfterImage(void)
{
	// ���݂̒l���i�[���Ă���
	D3DXVECTOR3 PosDef = GetPos();
	D3DXVECTOR3 RotDef = GetRot();
	D3DXVECTOR3 ModelAddPosDef[PLAYER_MAX_MODELPARTS];
	D3DXVECTOR3 ModelRotDef[PLAYER_MAX_MODELPARTS];
	CModel **pModel = GetModel();
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATERIAL *pMat;
	D3DXCOLOR ModelColorDef[4][PLAYER_MAX_MODELPARTS];
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// �p�[�c�������J��Ԃ�
		ModelAddPosDef[nCntParts] = pModel[nCntParts]->GetAddPos();
		ModelRotDef[nCntParts] = pModel[nCntParts]->GetRot();
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// �|�C���^���擾�ł���
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// �}�e���A�����̐������J��Ԃ�
				ModelColorDef[nCntMat][nCntParts] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}
	}

	// �c����`�悷��(�L������������)
	D3DXCOLOR ModelCol = PLAYER_AFTERIMAGE_COLOR;
	D3DXVECTOR3 ModelScale = PLAYER_AFTERIMAGE_SCALE;
	for (int nCntAfter = m_nCurrentStack; nCntAfter >= 0; nCntAfter--)
	{// �c�����L�������������J��Ԃ�
		SetPos(m_AfterPos[nCntAfter]);
		SetRot(m_AfterRot[nCntAfter]);
		for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
		{// �p�[�c�������J��Ԃ�
			pModel[nCntParts]->SetScale(ModelScale);
			pModel[nCntParts]->SetAddPos(m_AfterModelPos[nCntAfter][nCntParts]);
			pModel[nCntParts]->SetRot(m_AfterModelRot[nCntAfter][nCntParts]);
			pBuffMat = pModel[nCntParts]->GetBuffMat();
			nNumMat = pModel[nCntParts]->GetNumMat();
			if (pBuffMat != NULL)
			{// �|�C���^���擾�ł���
				pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
				{// �}�e���A�����̐������J��Ԃ�
					pMat[nCntMat].MatD3D.Diffuse = ModelCol;
				}
			}
		}

		// �L�����N�^�[�̕`�揈��
		CCharacter::Draw();

		// �}�e���A���̐F��ύX����
		ModelCol.r += PLAYER_AFTERIMAGE_REDUP;
		ModelCol.g +=PLAYER_AFTERIMAGE_GREENUP;
		ModelCol.a -= PLAYER_AFTERIMAGE_ALPHACUT;

		// �傫����ύX����
		ModelScale.x -= PLAYER_AFTERIMAGE_SCALECUT;
		ModelScale.y -= PLAYER_AFTERIMAGE_SCALECUT;
		ModelScale.z -= PLAYER_AFTERIMAGE_SCALECUT;
	}

	// ���݂̒l�ɖ߂�
	SetPos(PosDef);
	SetRot(RotDef);
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{// �p�[�c�������J��Ԃ�
		pModel[nCntParts]->SetAddPos(ModelAddPosDef[nCntParts]);
		pModel[nCntParts]->SetRot(ModelRotDef[nCntParts]);
		pModel[nCntParts]->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		pBuffMat = pModel[nCntParts]->GetBuffMat();
		nNumMat = pModel[nCntParts]->GetNumMat();
		if (pBuffMat != NULL)
		{// �|�C���^���擾�ł���
			pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
			{// �}�e���A�����̐������J��Ԃ�
				pMat[nCntMat].MatD3D.Diffuse = ModelColorDef[nCntMat][nCntParts];
			}
		}
	}
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
void CPlayer::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �ړ��ʂ�ݒ肷�鏈��
//=============================================================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    �̗͂�ݒ肷�鏈��
//=============================================================================
void CPlayer::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �����x��ݒ肷�鏈��
//=============================================================================
void CPlayer::SetAccel(const float fAccel)
{
	m_fAccel = fAccel;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
void CPlayer::SetInertia(const float fInertia)
{
	m_fInertia = fInertia;
}

//=============================================================================
//    �W�����v�ʂ�ݒ肷�鏈��
//=============================================================================
void CPlayer::SetJumpPower(const float fJumpPower)
{
	m_fJumpPower = fJumpPower;
}

//=============================================================================
//    ������␳����{����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetRivisionRot(const float fRivisionRot)
{
	m_fRivisionRot = fRivisionRot;
}

//=============================================================================
//    ���݂̋O�ՃQ�[�W�ʂ�ݒ肷�鏈��
//=============================================================================
void CPlayer::SetOrbitSlash(const float fOrbitSlash)
{
	m_fOrbitSlash = fOrbitSlash;
}

//=============================================================================
//    ���݂̕K�E�Z�Q�[�W�ʂ�ݒ肷�鏈��
//=============================================================================
void CPlayer::SetSpecial(const float fSpecial)
{
	m_fSpecial = fSpecial;
}

//=============================================================================
//    �W�����v���Ă��邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//=============================================================================
//    �A�N�V�������Ă��邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetAction(const bool bAction)
{
	m_bAction = bAction;
}

//=============================================================================
//    ���b�N�I�����Ă��邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetLockOn(const bool bLockOn)
{
	m_bLockOn = bLockOn;
}

//=============================================================================
//    �_���[�W���󂯂Ă��邩�ǂ����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetDamage(const bool bDamage)
{
	m_bDamage = bDamage;
}

//=============================================================================
//    �ړ��ʂ��擾���鏈��
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �̗͂��擾���鏈��
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �����x���擾���鏈��
//=============================================================================
float CPlayer::GetAccel(void)
{
	return m_fAccel;
}

//=============================================================================
//    �������擾���鏈��
//=============================================================================
float CPlayer::GetInertia(void)
{
	return m_fInertia;
}

//=============================================================================
//    �W�����v�ʂ��擾���鏈��
//=============================================================================
float CPlayer::GetJumpPower(void)
{
	return m_fJumpPower;
}

//=============================================================================
//    ������␳����{�����擾���鏈��
//=============================================================================
float CPlayer::GetRivisionRot(void)
{
	return m_fRivisionRot;
}

//=============================================================================
//    ���݂̋O�ՃQ�[�W�ʂ��擾���鏈��
//=============================================================================
float CPlayer::GetOrbitSlash(void)
{
	return m_fOrbitSlash;
}

//=============================================================================
//    ���݂̕K�E�Z�Q�[�W�ʂ��擾���鏈��
//=============================================================================
float CPlayer::GetSpecial(void)
{
	return m_fSpecial;
}

//=============================================================================
//    �W�����v���Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    �A�N�V�������Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CPlayer::GetAction(void)
{
	return m_bAction;
}
//=============================================================================
//    ���b�N�I�����Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CPlayer::GetLockOn(void)
{
	return m_bLockOn;
}

//=============================================================================
//    ���b�N�I���Ώۂ̓G�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEnemy *CPlayer::GetLockOnEnemy(void)
{
	return m_pLockOnEnemy;
}

//=============================================================================
//    �_���[�W���󂯂Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CPlayer::GetDamage(void)
{
	return m_bDamage;
}