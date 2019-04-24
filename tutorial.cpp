//*****************************************************************************
//
//     �`���[�g���A���̏���[tutorial.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"
#include "ui.h"
#include "player.h"
#include "effectManager.h"
#include "meshWall.h"
#include "shadow.h"


#include "boxCollider.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define TUTORIAL_SYSTEM_FILENAME   "data/TEXT/MODE/tutorial.ini"                 // �`���[�g���A���̏��������X�N���v�g�t�@�C����
#define TUTORIAL_MODECHANGE_TIMING (600)                                         // ��ʑJ�ڂ������ōs���^�C�~���O
#define TUTORIAL_POLYGON_PRIORITY  (11)                                          // �`���[�g���A���Ŏg�p����2D�|���S���̏����D�揇��

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                "NUM_TEXTURE = "                              // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME           "TEXTURE_FILENAME = "                         // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define PLAYER_FILENAME            "PLAYER_FILENAME = "                          // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
#define EFFECT_FILENAME            "EFFECT_FILENAME = "                          // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
#define MAP_FILENAME               "MAP_FILENAME = "                             // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

// �v���C���[���
#define PLAYERSET                  "PLAYERSET"                                   // �v���C���[���ǂݍ��݊J�n�̍��}
#define END_PLAYERSET              "END_PLAYERSET"                               // �v���C���[���ǂݍ��ݏI���̍��}
#define LIFE                       "LIFE = "                                     // �v���C���[�̗̑�
#define ORBIT_SLASH                "ORBIT_SLASH = "                              // �v���C���[�̋O�ՃQ�[�W��
#define MAX_SPECIAL                "MAX_SPECIAL = "                              // �v���C���[�̕K�E�Q�[�W�̍ő�l��
#define FILENAME                   "FILENAME = "                                 // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����

// PressEnter�p�̃}�N��
#define TUTORIAL_PRESS_POS         (D3DXVECTOR3(SCREEN_WIDTH / 2,600.0f,0.0f))   // PressEnter�̍��W
#define TUTORIAL_PRESS_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // PressEnter�̐F
#define TUTORIAL_PRESS_WIDTH       (400.0f)                                      // PressEnter�̕�
#define TUTORIAL_PRESS_HEIGHT      (100.0f)                                      // PressEnter�̍���

// ��ʑJ�ڃ`�F�b�N�p
#define TUTORIAL_MODE_LOGO_MAX        (3)                                           // �����p�̕����|���S���̍ő吔
#define TUTORIAL_MODE_BLACKCOL        (0.7f)                                        // �w�i�p�|���S���̓����x�ő�l(���̒l�ɒB������`�F�b�N�J�n)
#define TUTORIAL_MODE_BLACKCOL_UP     (0.02f)                                       // �w�i�p�|���S���̓����x���グ��l
#define TUTORIAL_MODE_LOGO_POS_0      (D3DXVECTOR3(426.0f,SCREEN_HEIGHT / 2, 0.0f)) // 1�ڂ̑I�����ڗp���S�|���S���̍��W
#define TUTORIAL_MODE_LOGO_POS_1      (D3DXVECTOR3(852.0f,SCREEN_HEIGHT / 2, 0.0f)) // 2�ڂ̑I�����ڗp���S�|���S���̍��W
#define TUTORIAL_MODE_LOGO_SCALE_NONE (100.0f)                                      // �I������Ă��Ȃ����S�|���S���̑傫��
#define TUTORIAL_MODE_LOGO_SCALE      (200.0f)                                      // �I������Ă��郍�S�|���S���̑傫��
#define TUTORIAL_MODE_LOGO_COL_NONE   (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))              // �I������Ă��Ȃ����S�|���S���̐F
#define TUTORIAL_MODE_LOGO_COL_MIN    (0.4f)                                        // �I������Ă��郍�S�|���S���̐F�����l
#define TUTORIAL_MODE_LOGO_COL_VALUE  (0.009f)                                      // �I������Ă��郍�S�|���S���̐F�̕ω���

// 1�ڂ̃��S�|���S���p
#define TUTORIAL_LOGO_POS_0           (D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f,0.0f))  // 1�ڂ̃��S�|���S���̍��W
#define TUTORIAL_LOGO_WIDTH_0         (550.0f)                                      // 1�ڂ̃��S�|���S���̕�
#define TUTORIAL_LOGO_HEIGHT_0        (70.0f)                                       // 1�ڂ̃��S�|���S���̍���

// 2�ڂ̃��S�|���S���p
#define TUTORIAL_LOGO_POS_1           (D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f,0.0f))  // 2�ڂ̃��S�|���S���̍��W
#define TUTORIAL_LOGO_WIDTH_1         (250.0f)                                      // 2�ڂ̃��S�|���S���̕�
#define TUTORIAL_LOGO_HEIGHT_1        (40.0f)                                       // 2�ڂ̃��S�|���S���̍���

// 3�ڂ̃��S�|���S���p
#define TUTORIAL_LOGO_POS_2           (D3DXVECTOR3(SCREEN_WIDTH / 2, 630.0f,0.0f))  // 3�ڂ̃��S�|���S���̍��W
#define TUTORIAL_LOGO_WIDTH_2         (180.0f)                                      // 3�ڂ̃��S�|���S���̕�
#define TUTORIAL_LOGO_HEIGHT_2        (35.0f)                                       // 3�ڂ̃��S�|���S���̍���

// ������@�p�|���S��1��
#define TUTORIAL_OPERATION_POS_0    (D3DXVECTOR3(330.0f,350.0f,0.0f))               // ������@�p�̃|���S���̍��W(1��)
#define TUTORIAL_OPERATION_COL_0    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // ������@�p�̃|���S���̐F(1��)
#define TUTORIAL_OPERATION_WIDTH_0  (330.0f)                                        // ������@�p�̃|���S���̕�(1��)
#define TUTORIAL_OPERATION_HEIGHT_0 (180.0f)                                        // ������@�p�̃|���S���̍���(1��)

// ������@�p�|���S��2��
#define TUTORIAL_OPERATION_POS_1    (D3DXVECTOR3(950.0f,350.0f,0.0f))               // ������@�p�̃|���S���̍��W(2��)
#define TUTORIAL_OPERATION_COL_1    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // ������@�p�̃|���S���̐F(2��)
#define TUTORIAL_OPERATION_WIDTH_1  (330.0f)                                        // ������@�p�̃|���S���̕�(2��)
#define TUTORIAL_OPERATION_HEIGHT_1 (180.0f)                                        // ������@�p�̃|���S���̍���(2��)

// ������@�؂�ւ��\���p�|���S��
#define TUTORIAL_OPERATION_POS      (D3DXVECTOR3(1060.0f,30.0f,0.0f))               // ������@�؂�ւ��\���p�|���S���̍��W
#define TUTORIAL_OPERATION_COL      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // ������@�؂�ւ��\���p�|���S���̐F
#define TUTORIAL_OPERATION_WIDTH    (220.0f)                                        // ������@�؂�ւ��\���p�|���S���̕�
#define TUTORIAL_OPERATION_HEIGHT   (30.0f)                                         // ������@�؂�ւ��\���p�|���S���̍���

// ������@�؂�ւ��\���p�|���S��
#define TUTORIAL_SKIP_POS           (D3DXVECTOR3(1080.0f,80.0f,0.0f))               // �X�L�b�v���@�\���p�|���S���̍��W
#define TUTORIAL_SKIP_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                // �X�L�b�v���@�\���p�|���S���̐F
#define TUTORIAL_SKIP_WIDTH         (200.0f)                                        // �X�L�b�v���@�\���p�|���S���̕�
#define TUTORIAL_SKIP_HEIGHT        (28.0f)                                         // �X�L�b�v���@�\���p�|���S���̍���

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pPlayer = NULL;           // �v���C���[�N���X�ւ̃|�C���^(�ۑ��p)
	m_pUi = NULL;               // UI�N���X�ւ̃|�C���^
	m_pMap = NULL;              // �}�b�v�N���X�ւ̃|�C���^
	m_nCounter = 0;             // �J�ڂ��Ǘ�����J�E���^�[
	m_pTextureManager = NULL;   // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_bGoalCol = false;         // �S�[���ɐG��Ă��邩�ǂ���
	m_pGoalWall = NULL;         // �S�[���p�̕ǃN���X�ւ̃|�C���^
	m_State = STATE_NONE;       // ���
	m_pSkip = NULL;             // �X�L�b�v���@�\���|���S���N���X�ւ̃|�C���^
	m_pCheck = NULL;            // ������@�\���ؑ֗p�|���S���N���X�ւ̃|�C���^
	m_bOperationInfo = false;   // ������@��\�����Ă��邩�ǂ���
	for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
	{// ������@�p�̃|���S���̐������J��Ԃ�
		m_pOperationInfo[nCntOpe] = NULL;
	}

	// ��ʑJ�ڃ`�F�b�N�p
	m_nSelectMode = 0;          // ��ʑJ�ڂ��邩�ǂ�����I�ׂ�ԍ�
	m_pBlackPoly = NULL;        // �`�F�b�N����Ƃ��ɕ\�����鍕���|���S��
	m_fModeChangeCol = 0.0f;    // ���S�|���S���̐F�̕ω���
	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// �I�����ڂ̐������J��Ԃ�
		m_pSelectMode[nCntSelect] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;  // �`���[�g���A���N���X�^�̃|�C���^
	if (pTutorial == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pTutorial->Init();
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	// �C���X�^���X��Ԃ�
	return pTutorial;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// �e���f���ǂݍ���
	CShadow::Load();

	// �`���[�g���A���̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(TUTORIAL_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTex = 0;                            // �e�N�X�`����ǂݍ��ސ�
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����
			char aEffectFileName[256] = "\0";           // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
			char aPlayerFileName[256] = "\0";           // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
			char aMapFileName[256] = "\0";              // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
			int nPlayerLife = 0;                        // �v���C���[�̗̑�
			float fPlayerOrbitSlash = 0.0f;             // �v���C���[�̋O�ՃQ�[�W��
			float fPlayerMaxSpecial = 0.0f;             // �v���C���[�̕K�E�Q�[�W�̍ő�l

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂł���
				    // �������������
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// �e�N�X�`���̌���񂪂�����
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// ���f������1�ȏ゠��
									m_pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// �e�N�X�`���̃t�@�C���p�X����񂪂�����
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
							else if (CFunctionLib::Memcmp(pStrCur, PLAYERSET) == 0)
							{// �v���C���[�f�[�^�ǂݍ��݊J�n�̍��}������
								while (1)
								{// ���[�v�J�n
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
									if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
									{// �̗͓ǂݍ��݂̍��}������
										nPlayerLife = CFunctionLib::ReadInt(pStrCur, LIFE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, ORBIT_SLASH) == 0)
									{// �O�ՃQ�[�W�ʓǂݍ��݂̍��}������
										fPlayerOrbitSlash = CFunctionLib::ReadFloat(pStrCur, ORBIT_SLASH);
									}
									else if (CFunctionLib::Memcmp(pStrCur, MAX_SPECIAL) == 0)
									{// �K�E�Q�[�W�̍ő�l�ʓǂݍ��݂̍��}������
										fPlayerMaxSpecial = CFunctionLib::ReadFloat(pStrCur, MAX_SPECIAL);
									}
									else if (CFunctionLib::Memcmp(pStrCur, FILENAME) == 0)
									{// �v���C���[�f�[�^�̃X�N���v�g�t�@�C����������
									 // �v���C���[�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
										pStr = CFunctionLib::ReadString(pStrCur, pStr, FILENAME);
										strcpy(aPlayerFileName, pStr);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_PLAYERSET) == 0)
									{// �v���C���[���ǂݍ��ݏI���̍��}������
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����������
							    // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
							    // �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �K�v�ȃN���X���쐬����
					m_pUi = CUI::Create(999 * 60, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial, m_pTextureManager);

					// �G�t�F�N�g�Ǌ��N���X
					m_pEffectManager = CEffectManager::Create(aEffectFileName);

					// �}�b�v�̐���
					m_pMap = CMap::Create(aMapFileName);

					// �v���C���[
					CPlayerManager::Create(aPlayerFileName, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial);

					// �v���C���[�N���X�ւ̃|�C���^��ۑ����Ă���
					// �v���C���[�N���X���擾����
					CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
					CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
					for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
					{// �`��D�揇�ʂ̐������J��Ԃ�
						pScene = CScene::GetTop(nCntPriority);
						while (pScene != NULL)
						{// ����������ɂȂ�܂ŌJ��Ԃ�
							pSceneNext = pScene->GetNext();
							if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
							{// �v���C���[�N���X�^�̃|�C���^��������
								m_pPlayer = (CPlayer*)pScene;
								m_pPlayer->SetPos(m_pMap->GetPlayerDefaultPos());
								break;
							}
							pScene = pSceneNext;
						}
					}

					// �F�̕ω��ʂ�ݒ�
					m_fModeChangeCol = TUTORIAL_MODE_LOGO_COL_VALUE;    // ���S�|���S���̐F�̕ω���

					// �G�t�F�N�g���o��
					if (m_pEffectManager != NULL)
					{
						m_pEffectManager->SetEffect(D3DXVECTOR3(0.0f, 0.0f, 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 30);
					}

					// ������@�p�̃|���S���̐���
					// 1��
					if (m_pOperationInfo[0] == NULL)
					{
						m_pOperationInfo[0] = CScene2D::Create(TUTORIAL_OPERATION_POS_0, TUTORIAL_OPERATION_COL_0, TUTORIAL_OPERATION_WIDTH_0, TUTORIAL_OPERATION_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pOperationInfo[0] != NULL)
						{
							m_pOperationInfo[0]->BindTexture(m_pTextureManager->GetTexture(12));
						}
					}

					// 2��
					if (m_pOperationInfo[1] == NULL)
					{
						m_pOperationInfo[1] = CScene2D::Create(TUTORIAL_OPERATION_POS_1, TUTORIAL_OPERATION_COL_1, TUTORIAL_OPERATION_WIDTH_1, TUTORIAL_OPERATION_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pOperationInfo[1] != NULL)
						{
							m_pOperationInfo[1]->BindTexture(m_pTextureManager->GetTexture(13));
						}
					}

					// ������@�\���ؑ֗p�|���S���̐���
					if (m_pCheck == NULL)
					{
						m_pCheck = CScene2D::Create(TUTORIAL_OPERATION_POS, TUTORIAL_OPERATION_COL, TUTORIAL_OPERATION_WIDTH, TUTORIAL_OPERATION_HEIGHT, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pCheck != NULL)
						{
							m_pCheck->BindTexture(m_pTextureManager->GetTexture(14));
						}
					}
					m_bOperationInfo = true;

					// �X�L�b�v���@�\���p�|���S���̐���
					if (m_pSkip == NULL)
					{
						m_pSkip = CScene2D::Create(TUTORIAL_SKIP_POS, TUTORIAL_SKIP_COL, TUTORIAL_SKIP_WIDTH, TUTORIAL_SKIP_HEIGHT, 0.0f, TUTORIAL_POLYGON_PRIORITY);
						if (m_pSkip != NULL)
						{
							m_pSkip->BindTexture(m_pTextureManager->GetTexture(15));
						}
					}

					// �����蔻��p�����f���̔z�u
					SetBoxCollider();

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
				}
				else
				{// ���������m�ۂł��Ȃ�����
					return E_FAIL;
				}
			}
			else
			{// ���������m�ۂł����ԂłȂ�
				return E_FAIL;
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
void CTutorial::Uninit(void)
{
	// �e���f���J��
	CShadow::UnLoad();

	// �v���C���[�N���X�ւ̃|�C���^����ɂ���
	m_pPlayer = NULL;

	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����ɂ���
	m_pEffectManager = NULL;

	// UI�N���X���J������
	if (m_pUi != NULL)
	{
		m_pUi->Uninit();

		delete m_pUi;
		m_pUi = NULL;
	}

	// �}�b�v�N���X���J������
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �`���[�g���A���I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	if (m_State == STATE_MODECHECK)
	{// ��ʑJ�ڂ��邩�`�F�b�N�����Ԃ�������
		ModeChangeCheck();
	}
	else if(m_State == STATE_MODENONE)
	{// ��ʑJ�ڂ��Ȃ�������
		if (m_pBlackPoly != NULL)
		{// �����|���S������������Ă���
			D3DXCOLOR BlackCol = m_pBlackPoly->GetCol();
			if (BlackCol.a > 0.0f)
			{// �܂������x���������
				BlackCol.a -= TUTORIAL_MODE_BLACKCOL_UP;
				// �F��ݒ�
				m_pBlackPoly->SetCol(BlackCol);
				if (BlackCol.a <= 0.0f)
				{// �����x��������؂���
					m_pBlackPoly->Uninit();
					m_pBlackPoly = NULL;
					m_State = STATE_NONE;
					m_nSelectMode = 0;
				}
			}
		}
	}
	else if (m_State == STATE_NONE)
	{// �ʏ��Ԃ�������
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
		    // �L�[�{�[�h���擾
			CInputKeyboard *pKeyboard = CManager::GetKeyboard();

			// �R���g���[���[���擾
			CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);
			if (pKeyboard != NULL)
			{// �L�[�{�[�h���擾�ł���
				if (pKeyboard->GetTrigger(DIK_P) == true)
				{// P�L�[�������ꂽ
					m_bOperationInfo = m_bOperationInfo ? false : true;
					if (m_bOperationInfo == true)
					{// �\������Ȃ�
					    // ������@�p�̃|���S���̐���
					    // 1��
						if (m_pOperationInfo[0] == NULL)
						{
							m_pOperationInfo[0] = CScene2D::Create(TUTORIAL_OPERATION_POS_0, TUTORIAL_OPERATION_COL_0, TUTORIAL_OPERATION_WIDTH_0, TUTORIAL_OPERATION_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
							if (m_pOperationInfo[0] != NULL)
							{
								m_pOperationInfo[0]->BindTexture(m_pTextureManager->GetTexture(12));
							}
						}

						// 2��
						if (m_pOperationInfo[1] == NULL)
						{
							m_pOperationInfo[1] = CScene2D::Create(TUTORIAL_OPERATION_POS_1, TUTORIAL_OPERATION_COL_1, TUTORIAL_OPERATION_WIDTH_1, TUTORIAL_OPERATION_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
							if (m_pOperationInfo[1] != NULL)
							{
								m_pOperationInfo[1]->BindTexture(m_pTextureManager->GetTexture(13));
							}
						}
					}
					else if (m_bOperationInfo == false)
					{// �\�����Ȃ��Ȃ�
					    // ������@�p�̃|���S���̊J��
						for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
						{// ������@�p�̃|���S���̐������J��Ԃ�
							if (m_pOperationInfo[nCntOpe] != NULL)
							{// ���������m�ۂ���Ă���
								m_pOperationInfo[nCntOpe]->Uninit();
								m_pOperationInfo[nCntOpe] = NULL;
							}
						}
					}
				}
				else if (pKeyboard->GetTrigger(DIK_RSHIFT) == true)
				{// RSHIFT�L�[�������ꂽ
					CManager::GetFade()->SetFade(CManager::MODE_GAME);
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
				}
				else
				{// �L�[�{�[�h�̃{�^����������Ă��Ȃ�
					if (pJoyStick != NULL)
					{// �R���g���[���[���擾�ł���
						if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_11) == true)
						{// START�{�^���������ꂽ
							m_bOperationInfo = m_bOperationInfo ? false : true;
							if (m_bOperationInfo == true)
							{// �\������Ȃ�
							    // ������@�p�̃|���S���̐���
							    // 1��
								if (m_pOperationInfo[0] == NULL)
								{
									m_pOperationInfo[0] = CScene2D::Create(TUTORIAL_OPERATION_POS_0, TUTORIAL_OPERATION_COL_0, TUTORIAL_OPERATION_WIDTH_0, TUTORIAL_OPERATION_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
									if (m_pOperationInfo[0] != NULL)
									{
										m_pOperationInfo[0]->BindTexture(m_pTextureManager->GetTexture(12));
									}
								}

								// 2��
								if (m_pOperationInfo[1] == NULL)
								{
									m_pOperationInfo[1] = CScene2D::Create(TUTORIAL_OPERATION_POS_1, TUTORIAL_OPERATION_COL_1, TUTORIAL_OPERATION_WIDTH_1, TUTORIAL_OPERATION_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
									if (m_pOperationInfo[1] != NULL)
									{
										m_pOperationInfo[1]->BindTexture(m_pTextureManager->GetTexture(13));
									}
								}
							}
							else if (m_bOperationInfo == false)
							{// �\�����Ȃ��Ȃ�
							    // ������@�p�̃|���S���̊J��
								for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
								{// ������@�p�̃|���S���̐������J��Ԃ�
									if (m_pOperationInfo[nCntOpe] != NULL)
									{// ���������m�ۂ���Ă���
										m_pOperationInfo[nCntOpe]->Uninit();
										m_pOperationInfo[nCntOpe] = NULL;
									}
								}
							}
						}
						else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_10) == true)
						{// BACK�{�^���������ꂽ
							CManager::GetFade()->SetFade(CManager::MODE_GAME);
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
						}
					}
				}
			}
		}
	}

	CDebugProc::Print("�`���[�g���A�����\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
//    ��ʑJ�ڂ��邩�`�F�b�N���鏈��
//=============================================================================
void CTutorial::ModeChangeCheck(void)
{
	if (m_pBlackPoly != NULL)
	{// �����|���S������������Ă���
		D3DXCOLOR BlackCol = m_pBlackPoly->GetCol();
		if(BlackCol.a < TUTORIAL_MODE_BLACKCOL)
		{// �܂������x���グ���
			BlackCol.a += TUTORIAL_MODE_BLACKCOL_UP;
			if (BlackCol.a >= TUTORIAL_MODE_BLACKCOL)
			{// �����x���オ��؂���
				BlackCol.a = TUTORIAL_MODE_BLACKCOL;
				CreateModeLogo();
			}
			// �F��ݒ�
			m_pBlackPoly->SetCol(BlackCol);
		}
		else
		{// �����グ����Ȃ�
			// ���S�̐F��ω�������
			if (m_pSelectMode[m_nSelectMode] != NULL)
			{// �|���S������������Ă���
				m_pSelectMode[m_nSelectMode]->Update();
			}

			// ��ʑJ�ڃ`�F�b�N�J�n
			// �L�[�{�[�h���擾
			CInputKeyboard *pKeyboard = CManager::GetKeyboard();

			// �R���g���[���[���擾
			CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

			if (pKeyboard != NULL)
			{// �L�[�{�[�h���擾�ł���
				if (pKeyboard->GetTrigger(DIK_RETURN) == true)
				{// ENTER�L�[�������ꂽ
					if (m_nSelectMode == SELECT_CHANGE)
					{// ��ʑJ�ڂ���ԍ�������
						if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
						{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
							CManager::GetFade()->SetFade(CManager::MODE_GAME);
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
							m_State = STATE_MODECHANGE;
						}
					}
					else if(m_nSelectMode == SELECT_NONE)
					{// ��ʑJ�ڂ��Ȃ��ԍ�������
						m_State = STATE_MODENONE;
						for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
						{// �I�����ڂ̐������J��Ԃ�
							if (m_pSelectMode[nCntSelect] != NULL)
							{// ���������m�ۂ���Ă���
								m_pSelectMode[nCntSelect]->Uninit();
								m_pSelectMode[nCntSelect] = NULL;
							}
						}
						for (int nCntLogo = 0; nCntLogo < TUTORIAL_MODE_LOGO_MAX; nCntLogo++)
						{// ���S�̐������J��Ԃ�
							if (m_pLogo[nCntLogo] != NULL)
							{// ���������m�ۂ���Ă���
								m_pLogo[nCntLogo]->Uninit();
								m_pLogo[nCntLogo] = NULL;
							}
						}
					}
				}
				else if (pKeyboard->GetTrigger(DIK_A) == true || pKeyboard->GetRepeat(DIK_A) == true)
				{// A�L�[�������ꂽ
					// �I������Ă������S�̐ݒ�
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
					m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

					// �ԍ��̐ݒ�
					m_nSelectMode = (m_nSelectMode + (SELECT_MAX - 1)) % SELECT_MAX;

					// ���ꂩ��I������郍�S�̐ݒ�
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
					m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

					// ����炷
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				}
				else if (pKeyboard->GetTrigger(DIK_D) == true || pKeyboard->GetRepeat(DIK_D) == true)
				{// D�L�[�������ꂽ
				    // �I������Ă������S�̐ݒ�
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
					m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

					// �ԍ��̐ݒ�
					m_nSelectMode = (m_nSelectMode + 1) % SELECT_MAX;

					// ���ꂩ��I������郍�S�̐ݒ�
					m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
					m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
					m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

					// ����炷
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				}
				else
				{// ENTER�L�[��������Ă��Ȃ�
					if (pJoyStick != NULL)
					{// �R���g���[���[���擾�ł���
						if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == true)
						{// B�{�^���������ꂽ
							if (m_nSelectMode == SELECT_CHANGE)
							{// ��ʑJ�ڂ���ԍ�������
								if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
								{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
									CManager::GetFade()->SetFade(CManager::MODE_GAME);
									CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
									m_State = STATE_MODECHANGE;
								}
							}
							else if (m_nSelectMode == SELECT_NONE)
							{// ��ʑJ�ڂ��Ȃ��ԍ�������
								m_State = STATE_MODENONE;
								for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
								{// �I�����ڂ̐������J��Ԃ�
									if (m_pSelectMode[nCntSelect] != NULL)
									{// ���������m�ۂ���Ă���
										m_pSelectMode[nCntSelect]->Uninit();
										m_pSelectMode[nCntSelect] = NULL;
									}
								}
								for (int nCntLogo = 0; nCntLogo < TUTORIAL_MODE_LOGO_MAX; nCntLogo++)
								{// ���S�̐������J��Ԃ�
									if (m_pLogo[nCntLogo] != NULL)
									{// ���������m�ۂ���Ă���
										m_pLogo[nCntLogo]->Uninit();
										m_pLogo[nCntLogo] = NULL;
									}
								}
							}
						}
						else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_22) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_22) == true)
						{// ���X�e�B�b�N���E�ɓ������ꂽ
						    // �I������Ă������S�̐ݒ�
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
							m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

							// �ԍ��̐ݒ�
							m_nSelectMode = (m_nSelectMode + 1) % SELECT_MAX;

							// ���ꂩ��I������郍�S�̐ݒ�
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
							m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

							// ����炷
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
						}
						else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_23) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_23) == true)
						{// ���X�e�B�b�N�����ɓ������ꂽ
						    // �I������Ă������S�̐ݒ�
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_NONE);
							m_pSelectMode[m_nSelectMode]->SetCol(TUTORIAL_MODE_LOGO_COL_NONE);
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);

							// �ԍ��̐ݒ�
							m_nSelectMode = (m_nSelectMode + (SELECT_MAX - 1)) % SELECT_MAX;

							// ���ꂩ��I������郍�S�̐ݒ�
							m_pSelectMode[m_nSelectMode]->SetState(CScene2DFlash::STATE_SELECT);
							m_pSelectMode[m_nSelectMode]->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
							m_pSelectMode[m_nSelectMode]->SetLength(TUTORIAL_MODE_LOGO_SCALE);

							// ����炷
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
						}
					}
				}
			}
			CDebugProc::Print("��ʑJ�ڂ��܂���\n");
			CDebugProc::Print("�I��ԍ� %d\n", m_nSelectMode);
		}
	}
}

//=============================================================================
//    ��ʑJ�ڃ`�F�b�N�p�̃��S�𐶐����鏈��
//=============================================================================
void CTutorial::CreateModeLogo(void)
{
	// �I�����ڗp�̃��S�|���S���𐶐�
	m_pSelectMode[0] = CScene2DFlash::Create(TUTORIAL_MODE_LOGO_POS_0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f, 0.0f, CScene2DFlash::STATE_SELECT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.4f), TUTORIAL_MODE_LOGO_COL_VALUE, 4, TUTORIAL_POLYGON_PRIORITY);
	if (m_pSelectMode[0] != NULL)
	{// �����ł���
		m_pSelectMode[0]->BindTexture(m_pTextureManager->GetTexture(3));
		m_pSelectMode[0]->SetLength(TUTORIAL_MODE_LOGO_SCALE);
	}

	m_pSelectMode[1] = CScene2DFlash::Create(TUTORIAL_MODE_LOGO_POS_1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f, 0.0f, CScene2DFlash::STATE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.4f), TUTORIAL_MODE_LOGO_COL_VALUE, 4, TUTORIAL_POLYGON_PRIORITY);
	if (m_pSelectMode[1] != NULL)
	{// �����ł���
		m_pSelectMode[1]->BindTexture(m_pTextureManager->GetTexture(4));
		m_pSelectMode[1]->SetLength(TUTORIAL_MODE_LOGO_SCALE_NONE);
	}

	// �����p�̃��S�|���S���𐶐�
	m_pLogo[0] = CScene2D::Create(TUTORIAL_LOGO_POS_0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIAL_LOGO_WIDTH_0, TUTORIAL_LOGO_HEIGHT_0, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	if (m_pLogo[0] != NULL)
	{// �����ł���
		m_pLogo[0]->BindTexture(m_pTextureManager->GetTexture(0));
	}

	m_pLogo[1] = CScene2D::Create(TUTORIAL_LOGO_POS_1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIAL_LOGO_WIDTH_1, TUTORIAL_LOGO_HEIGHT_1, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	if (m_pLogo[1] != NULL)
	{// �����ł���
		m_pLogo[1]->BindTexture(m_pTextureManager->GetTexture(1));
	}

	m_pLogo[2] = CScene2D::Create(TUTORIAL_LOGO_POS_2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIAL_LOGO_WIDTH_2, TUTORIAL_LOGO_HEIGHT_2, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	if (m_pLogo[2] != NULL)
	{// �����ł���
		m_pLogo[2]->BindTexture(m_pTextureManager->GetTexture(2));
	}
}

//=============================================================================
//    �`���[�g���A���p�̓����蔻��p�����f����z�u���鏈��
//=============================================================================
void CTutorial::SetBoxCollider(void)
{
	// �ǂ̉E��
	CBoxCollider::Create(D3DXVECTOR3(495.0f,0.0f,-1000.0f), 10.0f, 300.0f, 1000.0f, true, 5);

	// �ǂ̍���
	CBoxCollider::Create(D3DXVECTOR3(-505.0f, 0.0f, -1000.0f), 10.0f, 300.0f, 1000.0f, true, 5);

	// �ǂ̌�둤
	CBoxCollider::Create(D3DXVECTOR3(-500.0f, 0.0f, -1005.0f), 1000.0f, 300.0f, 10.0f, true, 5);

	// �ǂ̑O�̉E��
	CBoxCollider::Create(D3DXVECTOR3(112.0f, 0.0f, -5.0f), 500.0f, 300.0f, 10.0f, true, 5);

	// �ǂ̑O�̍���
	CBoxCollider::Create(D3DXVECTOR3(-512.0f, 0.0f, -5.0f), 400.0f, 300.0f, 10.0f, true, 5);
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CTutorial::SetState(const STATE state)
{
	m_State = state;
	if (m_State == STATE_MODECHECK)
	{// ��ʑJ�ڂ��邩�ǂ����`�F�b�N�����ԂɂȂ���
		m_pBlackPoly = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, TUTORIAL_POLYGON_PRIORITY);
	}
}

//=============================================================================
//    �S�[���ɐG��Ă��邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CTutorial::SetGoalCol(const bool bGoalCol)
{
	m_bGoalCol = bGoalCol;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CTutorial::STATE CTutorial::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �v���C���[�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
//    �S�[���p�̕ǃN���X���擾���鏈��
//=============================================================================
CMeshWall *CTutorial::GetGoalWall(void)
{
	return m_pGoalWall;
}

//=============================================================================
//    �S�[���ɐG��Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CTutorial::GetGoalCol(void)
{
	return m_bGoalCol;
}