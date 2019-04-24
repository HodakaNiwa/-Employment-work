//*****************************************************************************
//
//     �^�C�g���̏���[title.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "mesh2D.h"
#include "particle2D.h"
#include "textureManager.h"
#include "effectManager.h"
#include "functionlib.h"
#include "map.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �f�[�^���͗p�}�N��
#define TITLE_SYSTEM_FILENAME       "data/TEXT/MODE/title.ini"                    // �^�C�g���̏��������X�N���v�g�t�@�C����
#define TITLE_MODECHANGE_TIMING     (1555)                                        // ��ʑJ�ڂ������ōs���^�C�~���O
#define TITLE_ALPHACUTSTART_TIMING  (40)                                          // �����|���S���̓����x�������n�߂�^�C�~���O
#define TITLE_POLYGON_PRIORITY      (11)                                          // �^�C�g���Ŏg�p����|���S���̏����D�揇��

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                 "NUM_TEXTURE = "                              // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "                         // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define EFFECT_FILENAME             "EFFECT_FILENAME = "                          // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
#define MAP_FILENAME                "MAP_FILENAME = "                             // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

// �^�C�g�����S�p�P�ڗp�̃}�N��
#define TITLE_LOGO_0_POS            (D3DXVECTOR3(SCREEN_WIDTH / 2,260.0f,0.0f))   // �^�C�g�����S�P�ڂ̍��W
#define TITLE_LOGO_0_COL            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // �^�C�g�����S�P�ڂ̐F
#define TITLE_LOGO_0_WIDTH          (640.0f)                                      // �^�C�g�����S�P�ڂ̕�
#define TITLE_LOGO_0_HEIGHT         (280.0f)                                      // �^�C�g�����S�P�ڂ̍���

// �^�C�g�����S�p�Q�ڗp�̃}�N��
#define TITLE_LOGO_1_TIMING         (45)                                          // �^�C�g�����S�Q�ڂ��o���܂ł̎���
#define TITLE_LOGO_1_POS            (D3DXVECTOR3(SCREEN_WIDTH / 2,260.0f,0.0f))   // �^�C�g�����S�Q�ڂ̍��W
#define TITLE_LOGO_1_COL            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // �^�C�g�����S�Q�ڂ̐F
#define TITLE_LOGO_1_WIDTH          (3000.0f)                                     // �^�C�g�����S�Q�ڂ̕�
#define TITLE_LOGO_1_HEIGHT         (1200.0f)                                     // �^�C�g�����S�Q�ڂ̍���
#define TITLE_LOGO_1_SCALE          (700.0f)                                      // �^�C�g�����S�Q�ڂ̑傫��
#define TITLE_LOGO_1_SCALEDOWN      (1.5f)                                        // �^�C�g�����S�Q�ڂ̑傫���̕ω���

// �����|���S���p�̃}�N��
#define TITLE_FADEOUT_SPEED         (0.05f)                                       // �����x���グ�Ă�������
#define TITLE_FADESTOP_TIME         (100)                                         // �t�F�[�h�A�E�g���Ă���t�F�[�h�C�����J�n����܂ł̃^�C�~���O
#define TITLE_FADEIN_SPEED          (0.01f)                                       // �����x�������Ă�������

// �������b�V���|���S���p�̃}�N��
#define TITLE_BLACK_POS             (D3DXVECTOR3(SCREEN_WIDTH / 2,240.0f,0.0f))   // �����|���S���̍��W
#define TITLE_BLACK_WIDTH           (10.0f)                                       // �����|���S����1�u���b�N���̕�
#define TITLE_BLACK_HEIGHT          (490.0f)                                      // �����|���S����1�u���b�N���̍���
#define TITLE_BLACK_XBLOCK          (128)                                         // �����|���S���̉��̕�����
#define TITLE_BLACK_YBLOCK          (1)                                           // �����|���S���̏c�̕�����
#define TITLE_BLACK_ALPHACUT        (0.5f)                                        // �����x�������Ă�������

// PressEnter�p�̃}�N��
#define TITLE_PRESS_POS             (D3DXVECTOR3(SCREEN_WIDTH / 2,645.0f,0.0f))   // PressEnter�̍��W
#define TITLE_PRESS_COL             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // PressEnter�̐F
#define TITLE_PRESS_WIDTH           (450.0f)                                      // PressEnter�̕�
#define TITLE_PRESS_HEIGHT          (50.0f)                                       // PressEnter�̍���

// �p�[�e�B�N���p�̃}�N��
#define TITLE_PARTICLE_POS_START    (D3DXVECTOR3(-30.0f,460.0f,0.0f))             // �p�[�e�B�N�����o���ʒu�̃X�^�[�g�ꏊ
#define TITLE_PARTICLE_MOVE         (D3DXVECTOR3(10.3f,0.0f,0.0f))                // �p�[�e�B�N�����o���ʒu�𓮂�����
#define TITLE_PARTICLE_APPEAR       (5)                                           // ���̍X�V�ŏo���p�[�e�B�N���̗�

// �O�ՃG�t�F�N�g�p�̃}�N��
#define TITLE_ORBIT_PATTERN_NUM     (4)                                           // �O�ՃG�t�F�N�g�̃p�^�[����
#define TITLE_ORBIT_WIDTH           (40.0f)                                       // �O�ՃG�t�F�N�g�̉���
#define TITLE_ORBIT_HEIGHT          (40.0f)                                       // �O�ՃG�t�F�N�g�̏c��
#define TITLE_ORBIT_MOVESPEED       (0.03f)                                       // �O�ՃG�t�F�N�g�̈ړ��X�s�[�h
#define TITLE_ORBIT_SWITCH_MIN      (60)                                          // �O�ՃG�t�F�N�g�̃p�^�[����؂�ւ���^�C�~���O�̍ŏ��l
#define TITLE_ORBIT_SWITCH_RANDOM   (30)                                          // �O�ՃG�t�F�N�g�̃p�^�[����؂�ւ���^�C�~���O�̐U�ꕝ

// �p�^�[��1
#define TITLE_ORBIT_POS_START_0     (D3DXVECTOR3(-30.0f,0.0f,0.0f))               // �O�ՃG�t�F�N�g���o���ʒu�̃X�^�[�g�ʒu
#define TITLE_ORBIT_POS_DEST_0      (D3DXVECTOR3(1300.0f,300.0f,0.0f))            // �O�ՃG�t�F�N�g���o���ʒu�̖ڕW�ʒu

// �p�^�[��2
#define TITLE_ORBIT_POS_START_1     (D3DXVECTOR3(1300.0f,0.0f,0.0f))              // �O�ՃG�t�F�N�g���o���ʒu�̃X�^�[�g�ʒu
#define TITLE_ORBIT_POS_DEST_1      (D3DXVECTOR3(-30.0f,500.0f,0.0f))             // �O�ՃG�t�F�N�g���o���ʒu�̖ڕW�ʒu

// �p�^�[��3
#define TITLE_ORBIT_POS_START_2     (D3DXVECTOR3(-30.0f,700.0f,0.0f))             // �O�ՃG�t�F�N�g���o���ʒu�̃X�^�[�g�ʒu
#define TITLE_ORBIT_POS_DEST_2      (D3DXVECTOR3(1300.0f,0.0f,0.0f))              // �O�ՃG�t�F�N�g���o���ʒu�̖ڕW�ʒu

// �p�^�[��4
#define TITLE_ORBIT_POS_START_3     (D3DXVECTOR3(1300.0f,600.0f,0.0f))            // �O�ՃG�t�F�N�g���o���ʒu�̃X�^�[�g�ʒu
#define TITLE_ORBIT_POS_DEST_3      (D3DXVECTOR3(-30.0f,0.0f,0.0f))               // �O�ՃG�t�F�N�g���o���ʒu�̖ڕW�ʒu

// �^�C�g���J�����p�̃}�N��
#define TITLE_CAMERA_PATTERN_NUM    (3)                                           // �^�C�g���J�����̃p�^�[����
#define TITLE_CAMERA_START_POSV_Y   (D3DXVECTOR3(0.0f,350.0f,0.0f))               // �^�C�g���X�^�[�g���̃J�����̎��_�̍���
#define TITLE_CAMERA_START_POSR     (D3DXVECTOR3(450.0f,270.0f,-1760.0f))         // �^�C�g���X�^�[�g���̃J�����̒����_�̈ʒu
#define TITLE_CAMERA_DEST_POSR      (D3DXVECTOR3(450.0f,270.0f,3100.0f))          // �^�C�g���X�^�[�g���̃J�����̒����_�̖ړI�̈ʒu
#define TITLE_CAMERA_MOVESPEED      (0.0015f)                                     // �^�C�g���X�^�[�g���̃J�����̈ړ��X�s�[�h

// �p�^�[��1
#define TITLE_CAMERA_SWITCHTIMING_0 (585)                                         // 1�ڂ̃p�^�[������2�ڂ̃p�^�[���փ^�C�g���J������؂�ւ���^�C�~���O
#define TITLE_CAMERA_0_POSV_Y       (D3DXVECTOR3(0.0f,120.0f,0.0f))               // �p�^�[��2�ڂ̃J�����̎��_�̍���
#define TITLE_CAMERA_0_POSR         (D3DXVECTOR3(-89.0f,180.0f,1640.0f))          // �p�^�[��2�ڂ̃J�����̒����_�̈ʒu
#define TITLE_CAMERA_0_POSRDEST     (D3DXVECTOR3(101.0f,180.0f,2740.0f))          // �p�^�[��2�ڂ̃J�����̒����_�̖ړI�̈ʒu
#define TITLE_CAMERA_0_MOVESPEED    (0.0005f)                                     // �p�^�[��2�ڂ̃J�����̈ړ��X�s�[�h
#define TITLE_CAMERA_0_ROT          (D3DXVECTOR3(0.0f,-1.1f,0.0f))                // �p�^�[��2�ڂ̃J��������

// �p�^�[��2
#define TITLE_CAMERA_SWITCHTIMING_1 (580)                                         // 2�ڂ̃p�^�[������3�ڂ̃p�^�[���փ^�C�g���J������؂�ւ���^�C�~���O
#define TITLE_CAMERA_1_POSV_Y       (D3DXVECTOR3(0.0f,40.0f,0.0f))                // �p�^�[��3�ڂ̃J�����̎��_�̍���
#define TITLE_CAMERA_1_POSR         (D3DXVECTOR3(-1300.0f,-30.0f,4255.0f))        // �p�^�[��3�ڂ̃J�����̒����_�̈ʒu
#define TITLE_CAMERA_1_POSRDEST     (D3DXVECTOR3(1700.0f,40.0f,4255.0f))          // �p�^�[��3�ڂ̃J�����̒����_�̖ړI�̈ʒu
#define TITLE_CAMERA_1_MOVESPEED    (0.002f)                                      // �p�^�[��3�ڂ̃J�����̈ړ��X�s�[�h
#define TITLE_CAMERA_1_ROT          (D3DXVECTOR3(0.0f,D3DX_PI / 2,0.0f))          // �p�^�[��3�ڂ̃J��������

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTitle::CTitle(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{
	// �e��l�̃N���A
	m_State = STATE_NONE;          // ���
	m_pMap = NULL;                 // �}�b�v�N���X�ւ̃|�C���^
	m_nCounter = 0;                // �J�����̓������Ǘ�����J�E���^�[
	m_pPress = NULL;               // PressEnter�p��2D�|���S���N���X�ւ̃|�C���^
	m_pTextureManager = NULL;      // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pEffectManager = NULL;       // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_pWhitePoly = NULL;           // �t�F�[�h�C���Ɏg�p���锒���|���S��
	strcpy(m_aMapFileName, "\0");  // ��������}�b�v�̖��O
	m_fCutScaleValue = 0.0f;       // �傫�������炷����
	m_pBlackMesh = NULL;           // ���o�p�̍����|���S���N���X�ւ̃|�C���^
	m_nPatternCamera = 0;          // ���݂̃^�C�g���J�����̃p�^�[���ԍ�
	m_nPatternOrbit = 0;           // ���݂̋O�ՃG�t�F�N�g�p�^�[���ԍ�
	m_nModeCounter = 0;            // �J�ڂ��Ǘ�����J�E���^�[
	m_nEffectCounter = 0;          // �G�t�F�N�g���Ǘ�����J�E���^�[
	m_nEffectSwicthTiming = 0;     // �G�t�F�N�g�̃p�^�[����؂�ւ���^�C�~���O

	for (int nCntTitle = 0; nCntTitle < LOGOTYPE_MAX; nCntTitle++)
	{// �^�C�g�����S�̐������J��Ԃ�
		m_pTitleLogo[nCntTitle] = NULL;  // �^�C�g�����S�p��2D�|���S���N���X�ւ̃|�C���^
	}
	m_ParEmitterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �p�[�e�B�N�����o���ʒu
	m_ParEmitterDiffPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �p�[�e�B�N�����o���ړI�̈ʒu
	m_ParCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);          // �p�[�e�B�N���̐F
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;  // �^�C�g���N���X�^�̃|�C���^
	if (pTitle == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pTitle = new CTitle;
		if (pTitle != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pTitle->Init();
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
	return pTitle;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	// �^�C�g���̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(TITLE_SYSTEM_FILENAME, "r");
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
								strcpy(m_aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �G�t�F�N�g�Ǌ��N���X
					m_pEffectManager = CEffectManager::Create(aEffectFileName);

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

	// �ŏ��ɕK�v�ȃ��S��|���S���𐶐�����
	StartCreate();

	// �p�[�e�B�N�����o���ʒu��ݒ�
	m_ParEmitterPos = TITLE_PARTICLE_POS_START;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����ɂ��Ă���
	m_pEffectManager = NULL;

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �^�C�g�����S�̔j��
	for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX; nCntLogo++)
	{// ���S�̐������J��Ԃ�
		if (m_pTitleLogo[nCntLogo] != NULL)
		{
			m_pTitleLogo[nCntLogo]->Uninit();
			m_pTitleLogo[nCntLogo] = NULL;
		}
	}

	// �����|���S���̔j��
	if (m_pWhitePoly != NULL)
	{
		m_pWhitePoly->Uninit();
		m_pWhitePoly = NULL;
	}

	// PressEnter�̔j��
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	// �}�b�v�N���X���J������
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// �^�C�g���I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// �f�o�b�O�p
	//if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	//{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
	//	CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
	//}

	// �J�E���^�[��i�߂�
	m_nCounter++;

	if (m_State == STATE_OK_CHANGEMODE)
	{// ��ʑJ�ڂ��ł����ԂȂ��
		CreateOrbit();
		SwitchCamera();
		CheckModeChange();
	}
	else
	{// ��ʑJ�ڂ��ł��Ȃ���ԂȂ��
		if (m_State == STATE_NONE)
		{// �܂��Ȃɂ��o�Ă��Ȃ���ԂȂ��
			if (m_nCounter >= TITLE_ALPHACUTSTART_TIMING)
			{// �J�E���^�[������̒l�𒴂���
				m_nCounter = 0;
				m_State = STATE_ALPHACUT_BLACK;
			}
		}
		else if (m_State == STATE_ALPHACUT_BLACK)
		{// �����|���S���̓����x���������ԂȂ��
			AlphaCutBlack();
		}
		else if (m_State == STATE_STOP_LOGO)
		{// ���S���o���܂őҋ@�����Ԃ�������
			if (m_nCounter >= TITLE_LOGO_1_TIMING)
			{// ���S���o�����ԂɂȂ���
				// ���S�𐶐�����
				if (m_pTitleLogo[1] == NULL)
				{// ���������m�ۂł����Ԃł���
					m_pTitleLogo[1] = CScene2D::Create(TITLE_LOGO_1_POS, TITLE_LOGO_1_COL, TITLE_LOGO_1_WIDTH, TITLE_LOGO_1_HEIGHT, 0.0f, TITLE_POLYGON_PRIORITY + 1);
					if (m_pTitleLogo[1] != NULL)
					{// �|���S���������ł���
						m_pTitleLogo[1]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));
					}
				}
				m_State = STATE_APPEAR_LOGO;
			}
		}
		else if(m_State == STATE_APPEAR_LOGO)
		{// ���S���o��������Ԃ�������
			LogoCutScale();
		}
		else if (m_State == STATE_FADE_OUT)
		{// �t�F�[�h�A�E�g�����ԂȂ��
			FadeOut();
		}
		else if (m_State == STATE_FADE_STOP)
		{// �t�F�[�h���~�߂��ԂȂ��
			FadeStop();
		}
		else if (m_State == STATE_FADE_IN)
		{// �t�F�[�h�C����ԂȂ��
			FadeIn();
		}
	}

	CDebugProc::Print("�^�C�g�����\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
//    �^�C�g����ʂ̊J�n�Ɠ����ɐ������鏈��
//=============================================================================
void CTitle::StartCreate(void)
{
	// �O�Ղ��ۂ����̃|���S���𐶐�����
	if (m_pTitleLogo[0] == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pTitleLogo[0] = CScene2D::Create(TITLE_LOGO_0_POS, TITLE_LOGO_0_COL, TITLE_LOGO_0_WIDTH, TITLE_LOGO_0_HEIGHT, 0.0f, TITLE_POLYGON_PRIORITY);
		if (m_pTitleLogo[0] != NULL)
		{// �|���S���������ł���
			m_pTitleLogo[0]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
		}
	}

	// �����|���S���𐶐�����
	if (m_pBlackMesh == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pBlackMesh = CMesh2D::Create(TITLE_BLACK_POS,D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), TITLE_BLACK_WIDTH, TITLE_BLACK_HEIGHT, TITLE_BLACK_XBLOCK, TITLE_BLACK_YBLOCK, 1, 1, TITLE_POLYGON_PRIORITY + 1);
	}
}

//=============================================================================
//    �����|���S���̓����x����鏈��
//=============================================================================
void CTitle::AlphaCutBlack(void)
{
	float fCutValue = TITLE_BLACK_ALPHACUT;
	int nXBlock = 0;
	int nYBlock = 0;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	D3DXCOLOR col;

	if (m_pBlackMesh != NULL)
	{// �����|���S�����擾�ł��Ă���
		// ���_�o�b�t�@���擾
		pVtxBuff = m_pBlackMesh->GetVtxBuff();
		if (pVtxBuff != NULL)
		{// ���_�o�b�t�@���擾�ł���
			// ���_���̐ݒ�
			VERTEX_2D *pVtx;

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �e��l���擾
			nXBlock = m_pBlackMesh->GetXBlock();
			nYBlock = m_pBlackMesh->GetYBlock();
			for (int nCntV = 0; nCntV < nYBlock + 1; nCntV++)
			{// ���������̕����� + 1�����J��Ԃ�
				for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
				{// ���������̕����� + 1�����J��Ԃ�
					col = pVtx[nCntH].col;
					col.a -= fCutValue;
					if (col.a <= 0.0f)
					{// �����ɂȂ�؂��Ă���
						col.a = 0.0f;
					}
					else
					{// �܂������ɂȂ�؂��Ă��Ȃ�
						// �����x����銄�������炷
						fCutValue /= 2;
					}
					pVtx[nCntH].col = col;
				}
				// �����x����銄����߂�
				fCutValue = TITLE_BLACK_ALPHACUT;

				// �|�C���^��i�߂�
				pVtx += nXBlock + 1;
			}

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();

			if (col.a <= 0.0f)
			{// �S�Ă̒��_�̓����x��0�ɂȂ���
				// ���S���o���ҋ@��Ԃɂ���
				m_State = STATE_STOP_LOGO;
				m_nCounter = 0;
			}

			// �p�[�e�B�N�����o��
			float fAngle = 0.0f;  // �p�x
			float fMoveX = 0.0f;  // X�����̈ړ���
			float fMoveY = 0.0f;  // Y�����̈ړ���
			float fWidth = 0.0f;  // ����
			float fHeight = 0.0f; // �c��
			float fPosYDef = m_ParEmitterPos.y;

			for (int nCntPar = 0; nCntPar < TITLE_PARTICLE_APPEAR; nCntPar++)
			{// �o���������J��Ԃ�
				// ���O�Ƀ����_���Ōv�Z
				fAngle = (rand() % 72 * 0.01f) + (-D3DX_PI * 0.5f);
				fMoveX = sinf(fAngle) * (rand() % 5 + 5.0f);
				fMoveY = cosf(fAngle) * (rand() % 5 + 5.0f);
				fWidth = rand() % 10 + 25.0f;
				fHeight = rand() % 10 + 25.0f;
				CParticle2D *pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(fMoveX, fMoveY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fWidth, fHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.03f), -0.4f, 0.0f, 60, 0, true, TITLE_POLYGON_PRIORITY);
				if (pPar2D != NULL)
				{// ���������m�ۂł���
					pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_3));
				}
			}

			// �����ʒu�𓮂���
			m_ParEmitterPos.y = fPosYDef;
			m_ParEmitterPos += TITLE_PARTICLE_MOVE;
		}
	}
}

//=============================================================================
//    ���S�̑傫������鏈��
//=============================================================================
void CTitle::LogoCutScale(void)
{
	if (m_pTitleLogo[1] != NULL)
	{// ���S����������Ă���
		m_fCutScaleValue += TITLE_LOGO_1_SCALEDOWN;
		float fLength = m_pTitleLogo[1]->GetLength();

		// ���S�̑傫��������������
		fLength -= m_fCutScaleValue;
		m_pTitleLogo[1]->SetLength(fLength);

		if (fLength <= TITLE_LOGO_1_SCALE)
		{// �ŏI�I�ȃ��S�̑傫����菬�����Ȃ��Ă���
			// ���S�̑傫����߂��ݒ�
			fLength = TITLE_LOGO_1_SCALE;
			m_pTitleLogo[1]->SetLength(fLength);

			// �t�F�[�h�A�E�g��Ԃɂ���
			m_State = STATE_FADE_OUT;
			if (m_pWhitePoly == NULL)
			{// ���������m�ۂł����Ԃł���
				m_pWhitePoly = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, TITLE_POLYGON_PRIORITY + 1);
			}
		}
	}
}

//=============================================================================
//    �����|���S�����g���ăt�F�[�h�A�E�g���鏈��
//=============================================================================
void CTitle::FadeOut(void)
{
	if (m_pWhitePoly != NULL)
	{// �����|���S������������Ă���
		float fAlpha = m_pWhitePoly->GetCol().a;

		// �����|���S���̓����x�𑝂₷
		fAlpha += TITLE_FADEOUT_SPEED;
		if (fAlpha >= 1.0f)
		{// ���F�ɂȂ�؂���
			fAlpha = 1.0f;
			m_State = STATE_FADE_STOP;
			m_nCounter = 0;
		}

		// �����x��ݒ肷��
		D3DXCOLOR WhitePolyCol = m_pWhitePoly->GetCol();
		WhitePolyCol.a = fAlpha;
		m_pWhitePoly->SetCol(WhitePolyCol);
	}
}

//=============================================================================
//    �����|���S�����g���ăt�F�[�h���~�߂Ă��鎞�̏���
//=============================================================================
void CTitle::FadeStop(void)
{
	if (m_nCounter >= TITLE_FADESTOP_TIME)
	{// �t�F�[�h�A�E�g���Ă��炠����x���Ԃ��o�߂���
		// �t�F�[�h�C����Ԃɂ���
		m_State = STATE_FADE_IN;

		// �J�����̒����_�̈ʒu��ݒ�
		CManager::GetCamera()->SetPosR(TITLE_CAMERA_START_POSR);

		// �J�����̖ړI�̒����_�̈ʒu�ƌ��݂̈ʒu�Ƃ̍�����ݒ�
		CManager::GetCamera()->SetPosRDiff((TITLE_CAMERA_DEST_POSR - TITLE_CAMERA_START_POSR) * TITLE_CAMERA_MOVESPEED);

		// �J�����̎��_�̍�����ݒ�
		CManager::GetCamera()->SetPosVDest(TITLE_CAMERA_START_POSV_Y);

		// �J�E���^�[��߂�
		m_nCounter = 0;

		// BGM���Đ�����
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM000);

		// �}�b�v�𐶐�����
		if (m_pMap == NULL)
		{
			m_pMap = CMap::Create(m_aMapFileName);
		}
	}
}

//=============================================================================
//    �����|���S�����g���ăt�F�[�h�C�����鏈��
//=============================================================================
void CTitle::FadeIn(void)
{
	if (m_pWhitePoly != NULL)
	{// �����|���S������������Ă���
		float fAlpha = m_pWhitePoly->GetCol().a;

		// �����|���S���̓����x�𑝂₷
		fAlpha -= TITLE_FADEIN_SPEED;
		if (fAlpha <= 0.0f)
		{// �����ɂȂ�؂���
		    // �����|���S���̔j��
			if (m_pWhitePoly != NULL)
			{
				m_pWhitePoly->Uninit();
				m_pWhitePoly = NULL;
			}

		    // PressEnter�̍쐬
			if (m_pPress == NULL)
			{
				m_pPress = CScene2DPress::Create(TITLE_PRESS_POS, TITLE_PRESS_COL, TITLE_PRESS_WIDTH, TITLE_PRESS_HEIGHT, 0.0f, 80, 100, 4, TITLE_POLYGON_PRIORITY);
				if (m_pPress != NULL)
				{// ���������m�ۂł���
					m_pPress->BindTexture(m_pTextureManager->GetTexture(LOGOTYPE_MAX));
				}
			}

			// ��Ԃ�ݒ肷��
			m_State = STATE_OK_CHANGEMODE;

			// �O�ՃG�t�F�N�g�̍��W��ݒ�
			m_ParEmitterPos = TITLE_ORBIT_POS_START_0;
			m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_0 - TITLE_ORBIT_POS_START_0) * TITLE_ORBIT_MOVESPEED;

			// �O�ՃG�t�F�N�g�̐F�������_���ŏo��
			m_ParCol.r = rand() % 10 * 0.1f;
			m_ParCol.g = rand() % 10 * 0.1f;
			m_ParCol.b = rand() % 10 * 0.1f;
			m_ParCol.a = 1.0f;

			// �؂�ւ���^�C�~���O�������_���ŏo��
			m_nEffectSwicthTiming = (rand() % TITLE_ORBIT_SWITCH_RANDOM) + TITLE_ORBIT_SWITCH_MIN;
		}
		else
		{// �܂������ɂȂ�؂��Ă��Ȃ�
			D3DXCOLOR WhitePolyCol = m_pWhitePoly->GetCol();
			WhitePolyCol.a = fAlpha;
			m_pWhitePoly->SetCol(WhitePolyCol);
		}
	}
}

//=============================================================================
//    ��ʑJ�ڂ��邩�ǂ����`�F�b�N���鏈��
//=============================================================================
void CTitle::CheckModeChange(void)
{
	// �J�E���^�[��i�߂�
	m_nModeCounter++;

	if (m_nModeCounter >= TITLE_MODECHANGE_TIMING)
	{// �J�E���^�[������̒l�𒴂���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	else
	{// �J�E���^�[���܂�����̒l�𒴂��Ă��Ȃ�
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
				if (m_pPress != NULL)
				{// ���������m�ۂ���Ă���
					m_pPress->FlashStart();
				}
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
						if (m_pPress != NULL)
						{// ���������m�ۂ���Ă���
							m_pPress->FlashStart();
						}
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
					}
				}
			}
		}
	}
}

//=============================================================================
//    �J�����̃p�^�[����؂�ւ��鏈��
//=============================================================================
void CTitle::SwitchCamera(void)
{
	int nSwitchTiming = 0;   // �J�����̃p�^�[����؂�ւ���^�C�~���O
	switch (m_nPatternCamera)
	{// ���݂̃^�C�g���J�����̃p�^�[���ɂ���ď����킯
	case 0:   // �p�^�[��1
		nSwitchTiming = TITLE_CAMERA_SWITCHTIMING_0;
		break;
	case 1:   // �p�^�[��2
		nSwitchTiming = TITLE_CAMERA_SWITCHTIMING_1;
		break;
	}

	if (nSwitchTiming > 0)
	{// �؂�ւ���^�C�~���O���ݒ肳�ꂽ
		if (m_nCounter % nSwitchTiming  == 0)
		{// �؂�ւ���^�C�~���O�ɂȂ��Ă���
			// �J�E���^�[��߂�
			m_nCounter = 0;

			// �p�^�[���ԍ��ɂ���Đݒ肷��l��ς���
			D3DXVECTOR3 PosVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fMoveSpeed = 0.0f;
			D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (m_nPatternCamera)
			{// ���݂̃^�C�g���J�����̃p�^�[���ɂ���ď����킯
			case 0:   // �p�^�[��1
				PosVDest = TITLE_CAMERA_0_POSV_Y;
				PosR = TITLE_CAMERA_0_POSR;
				PosRDiff = (TITLE_CAMERA_0_POSRDEST - TITLE_CAMERA_0_POSR);
				fMoveSpeed = TITLE_CAMERA_0_MOVESPEED;
				Rot = TITLE_CAMERA_0_ROT;
				break;
			case 1:   // �p�^�[��2
				PosVDest = TITLE_CAMERA_1_POSV_Y;
				PosR = TITLE_CAMERA_1_POSR;
				PosRDiff = (TITLE_CAMERA_1_POSRDEST - TITLE_CAMERA_1_POSR);
				fMoveSpeed = TITLE_CAMERA_1_MOVESPEED;
				Rot = TITLE_CAMERA_1_ROT;
				break;
			}

			// �J�����̎��_�̍�����ݒ�
			CManager::GetCamera()->SetPosVDest(PosVDest);

			// �J�����̒����_�̈ʒu��ݒ�
			CManager::GetCamera()->SetPosR(PosR);

			// �J�����̖ړI�̒����_�̈ʒu�ƌ��݂̈ʒu�Ƃ̍�����ݒ�
			CManager::GetCamera()->SetPosRDiff(PosRDiff * fMoveSpeed);

			// �J�����̌�����ݒ�
			CManager::GetCamera()->SetRot(Rot);

			// �p�^�[���ԍ���i�߂�
			m_nPatternCamera++;
		}
	}
}

//=============================================================================
//   �@�O�Ղ̂悤�ȃG�t�F�N�g�𐶐����鏈��
//=============================================================================
void CTitle::CreateOrbit(void)
{
	// �J�E���^�[��i�߂�
	m_nEffectCounter++;

	// �p�[�e�B�N�����o��
	CParticle2D *pPar2D = NULL;
	float fAngle = 0.0f;  // �p�x
	float fMoveX = 0.0f;  // X�����̈ړ���
	float fMoveY = 0.0f;  // Y�����̈ړ���
	float fWidth = 0.0f;  // ����
	float fHeight = 0.0f; // �c��
	float fPosYDef = m_ParEmitterPos.y;
	D3DXVECTOR3 ParEmitterDestPos;
	switch (m_nPatternOrbit)
	{// ���݂̋O�ՃG�t�F�N�g�̃p�^�[���ɂ���ď����킯
	case 0:   // �p�^�[��1
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_0;
		break;
	case 1:   // �p�^�[��2
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_1;
		break;
	case 2:   // �p�^�[��3
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_2;
		break;
	case 3:   // �p�^�[��4
		ParEmitterDestPos = TITLE_ORBIT_POS_DEST_3;
		break;
	}

	for (int nCntPar = 0; nCntPar < TITLE_PARTICLE_APPEAR - 3; nCntPar++)
	{// �o���������J��Ԃ�
	    // ���O�Ƀ����_���Ōv�Z
		fAngle = atan2f(ParEmitterDestPos.x - m_ParEmitterPos.x, ParEmitterDestPos.y - m_ParEmitterPos.y) + ((rand() % 72 * 0.01f) - (rand() % 72 * 0.01f));
		fMoveX = sinf(fAngle) * (rand() % 5 + 5.0f);
		fMoveY = cosf(fAngle) * (rand() % 5 + 5.0f);
		fWidth = rand() % 10 + 25.0f;
		fHeight = rand() % 10 + 25.0f;
		pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(fMoveX, fMoveY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fWidth, fHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.03f), -0.4f, 0.0f, 60, 0, true, TITLE_POLYGON_PRIORITY - 1);
		if (pPar2D != NULL)
		{// ���������m�ۂł���
			pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_3));
		}
	}

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// ���������m�ۂł���
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// �o���ʒu�𓮂���
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// ���������m�ۂł���
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// �o���ʒu�𓮂���
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// ���������m�ۂł���
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// �o���ʒu�𓮂���
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_ParCol, TITLE_ORBIT_WIDTH, TITLE_ORBIT_HEIGHT, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, TITLE_POLYGON_PRIORITY - 1);
	if (pPar2D != NULL)
	{// ���������m�ۂł���
		pPar2D->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
	}

	// �o���ʒu�𓮂���
	m_ParEmitterPos += (m_ParEmitterDiffPos / 5);

	if (m_nEffectSwicthTiming > 0)
	{// �^�C�~���O���ݒ�ł��Ă���
		if (m_nEffectCounter % m_nEffectSwicthTiming == 0)
		{// �J�E���^�[������̒l�𒴂���
			// �J�E���^�[��߂�
			m_nEffectCounter = 0;

			// �p�^�[���ԍ���i�߂�
			m_nPatternOrbit = (m_nPatternOrbit + 1) % TITLE_ORBIT_PATTERN_NUM;

			switch (m_nPatternOrbit)
			{// ���݂̋O�ՃG�t�F�N�g�̃p�^�[���ɂ���ď����킯
			case 0:   // �p�^�[��1
				m_ParEmitterPos = TITLE_ORBIT_POS_START_0;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_0 - TITLE_ORBIT_POS_START_0) * TITLE_ORBIT_MOVESPEED;
				break;
			case 1:   // �p�^�[��2
				m_ParEmitterPos = TITLE_ORBIT_POS_START_1;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_1 - TITLE_ORBIT_POS_START_1) * TITLE_ORBIT_MOVESPEED;
				break;
			case 2:   // �p�^�[��3
				m_ParEmitterPos = TITLE_ORBIT_POS_START_2;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_2 - TITLE_ORBIT_POS_START_2) * TITLE_ORBIT_MOVESPEED;
				break;
			case 3:   // �p�^�[��4
				m_ParEmitterPos = TITLE_ORBIT_POS_START_3;
				m_ParEmitterDiffPos = (TITLE_ORBIT_POS_DEST_3 - TITLE_ORBIT_POS_START_3) * TITLE_ORBIT_MOVESPEED;
				break;
			}

			// �O�ՃG�t�F�N�g�̐F�������_���ŏo��
			m_ParCol.r = rand() % 10 * 0.1f;
			m_ParCol.g = rand() % 10 * 0.1f;
			m_ParCol.b = rand() % 10 * 0.1f;

			// �؂�ւ���^�C�~���O�������_���ŏo��
			m_nEffectSwicthTiming = (rand() % TITLE_ORBIT_SWITCH_RANDOM) + TITLE_ORBIT_SWITCH_MIN;
		}
	}
	else
	{// �ݒ肳��Ă��Ȃ�
	    // �؂�ւ���^�C�~���O�������_���ŏo��
		m_nEffectSwicthTiming = (rand() % TITLE_ORBIT_SWITCH_RANDOM) + TITLE_ORBIT_SWITCH_MIN;
	}
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CTitle::STATE CTitle::GetState(void)
{
	return m_State;
}