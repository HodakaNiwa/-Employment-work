//*****************************************************************************
//
//     ���U���g�̏���[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"
#include "ranking.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RESULT_SYSTEM_FILENAME         "data/TEXT/MODE/result.ini"                  // ���U���g�̏��������X�N���v�g�t�@�C����
#define RESULT_MODECHANGE_TIMING       (600)                                        // ��ʑJ�ڂ������ōs���^�C�~���O
#define RESULT_SCOREAPPEAR_TIMING      (60)                                         // �X�R�A�����X�ɏo���Ă����^�C�~���O
#define RESULT_NUMBER_INTERVAL         (1.0f)                                       // �����Ɛ����̊Ԋu

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                    "NUM_TEXTURE = "                             // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME               "TEXTURE_FILENAME = "                        // �ǂݍ��ރe�N�X�`���̃t�@�C����

// PressEnter�p�̃}�N��
#define RESULT_PRESS_POS               (D3DXVECTOR3(SCREEN_WIDTH / 2,690.0f,0.0f))  // PressEnter�̍��W
#define RESULT_PRESS_COL               (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // PressEnter�̐F
#define RESULT_PRESS_WIDTH             (280.0f)                                     // PressEnter�̕�
#define RESULT_PRESS_HEIGHT            (30.0f)                                      // PressEnter�̍���

// ���v�X�R�A�p�̃}�N��
#define RESULT_ALLSCORE_POS            (D3DXVECTOR3(1050.0f,610.0f,0.0f))           // ���v�X�R�A�̍��W
#define RESULT_ALLSCORE_COL            (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))             // ���v�X�R�A�̐F
#define RESULT_ALLSCORE_WIDTH          (40.0f)                                      // ���v�X�R�A�̕�
#define RESULT_ALLSCORE_HEIGHT         (40.0f)                                      // ���v�X�R�A�̍���

// �Q�[���X�R�A�p�̃}�N��
#define RESULT_GAMESCORE_POS           (D3DXVECTOR3(930.0f,175.0f,0.0f))            // �Q�[���X�R�A�̍��W
#define RESULT_GAMESCORE_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �Q�[���X�R�A�̐F
#define RESULT_GAMESCORE_WIDTH         (30.0f)                                      // �Q�[���X�R�A�̕�
#define RESULT_GAMESCORE_HEIGHT        (30.0f)                                      // �Q�[���X�R�A�̍���

// �X�e�[�W�N���A�{�[�i�X�p�̃}�N��
#define RESULT_STAGECLEAR_BONUS_POS    (D3DXVECTOR3(930.0f,257.0f,0.0f))            // �X�e�[�W�N���A�{�[�i�X�̍��W
#define RESULT_STAGECLEAR_BONUS_COL    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �X�e�[�W�N���A�{�[�i�X�̐F
#define RESULT_STAGECLEAR_BONUS_WIDTH  (30.0f)                                      // �X�e�[�W�N���A�{�[�i�X�̕�
#define RESULT_STAGECLEAR_BONUS_HEIGHT (30.0f)                                      // �X�e�[�W�N���A�{�[�i�X�̍���

// �ő�R���{���{�[�i�X�p�̃}�N��
#define RESULT_COMBOMAX_BONUS_POS      (D3DXVECTOR3(930.0f,342.0f,0.0f))            // �ő�R���{���{�[�i�X�̍��W
#define RESULT_COMBOMAX_BONUS_COL      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �ő�R���{���{�[�i�X�̐F
#define RESULT_COMBOMAX_BONUS_WIDTH    (30.0f)                                      // �ő�R���{���{�[�i�X�̕�
#define RESULT_COMBOMAX_BONUS_HEIGHT   (30.0f)                                      // �ő�R���{���{�[�i�X�̍���

// ���X�{�X���j�{�[�i�X�p�̃}�N��
#define RESULT_LASTBOSS_BONUS_POS      (D3DXVECTOR3(930.0f,422.0f,0.0f))            // ���X�{�X���j�{�[�i�X�̍��W
#define RESULT_LASTBOSS_BONUS_COL      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // ���X�{�X���j�{�[�i�X�̐F
#define RESULT_LASTBOSS_BONUS_WIDTH    (30.0f)                                      // ���X�{�X���j�{�[�i�X�̕�
#define RESULT_LASTBOSS_BONUS_HEIGHT   (30.0f)                                      // ���X�{�X���j�{�[�i�X�̍���

// �B���A�C�e���{�[�i�X�p�̃}�N��
#define RESULT_HIDDENITEM_BONUS_POS    (D3DXVECTOR3(930.0f,505.0f,0.0f))            // �B���A�C�e���{�[�i�X�̍��W
#define RESULT_HIDDENITEM_BONUS_COL    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �B���A�C�e���{�[�i�X�̐F
#define RESULT_HIDDENITEM_BONUS_WIDTH  (30.0f)                                      // �B���A�C�e���{�[�i�X�̕�
#define RESULT_HIDDENITEM_BONUS_HEIGHT (30.0f)                                      // �B���A�C�e���{�[�i�X�̍���

// ���U���g���S�p�}�N��
// ���W
#define RESULT_LOGO_POS_0              (D3DXVECTOR3(360.0f,173.0f,0.0f))            // ���U���g���S�̃|���S�����W(1��)
#define RESULT_LOGO_POS_1              (D3DXVECTOR3(360.0f,259.0f,0.0f))            // ���U���g���S�̃|���S�����W(2��)
#define RESULT_LOGO_POS_2              (D3DXVECTOR3(360.0f,341.0f,0.0f))            // ���U���g���S�̃|���S�����W(3��)
#define RESULT_LOGO_POS_3              (D3DXVECTOR3(360.0f,427.0f,0.0f))            // ���U���g���S�̃|���S�����W(4��)
#define RESULT_LOGO_POS_4              (D3DXVECTOR3(360.0f,513.0f,0.0f))            // ���U���g���S�̃|���S�����W(5��)
#define RESULT_LOGO_POS_5              (D3DXVECTOR3(330.0f,612.0f,0.0f))            // ���U���g���S�̃|���S�����W(6��)

// ��
#define RESULT_LOGO_WIDTH_0            (130.0f)                                     // ���U���g���S�̃|���S���̕�(1��)
#define RESULT_LOGO_WIDTH_1            (130.0f)                                     // ���U���g���S�̃|���S���̕�(2��)
#define RESULT_LOGO_WIDTH_2            (130.0f)                                     // ���U���g���S�̃|���S���̕�(3��)
#define RESULT_LOGO_WIDTH_3            (130.0f)                                     // ���U���g���S�̃|���S���̕�(4��)
#define RESULT_LOGO_WIDTH_4            (130.0f)                                     // ���U���g���S�̃|���S���̕�(5��)
#define RESULT_LOGO_WIDTH_5            (150.0f)                                     // ���U���g���S�̃|���S���̕�(6��)

// ����
#define RESULT_LOGO_HEIGHT_0           (30.0f)                                      // ���U���g���S�̃|���S���̍���(1��)
#define RESULT_LOGO_HEIGHT_1           (30.0f)                                      // ���U���g���S�̃|���S���̍���(2��)
#define RESULT_LOGO_HEIGHT_2           (30.0f)                                      // ���U���g���S�̃|���S���̍���(3��)
#define RESULT_LOGO_HEIGHT_3           (30.0f)                                      // ���U���g���S�̃|���S���̍���(4��)
#define RESULT_LOGO_HEIGHT_4           (30.0f)                                      // ���U���g���S�̃|���S���̍���(5��)
#define RESULT_LOGO_HEIGHT_5           (45.0f)                                      // ���U���g���S�̃|���S���̍���(6��)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
int CResult::m_nGameScore = 0;         // �Q�[���̃X�R�A
int CResult::m_nStageClearBonus = 0;   // �X�e�[�W�N���A�{�[�i�X�̃X�R�A
int CResult::m_nComboMaxBonus = 0;     // �ő�R���{���{�[�i�X�̃X�R�A
int CResult::m_nLastBossBonus = 0;     // ���X�{�X���j�{�[�i�X�̃X�R�A
int CResult::m_nHiddenItemBonus = 0;   // �B���A�C�e���{�[�i�X�̃X�R�A

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CResult::CResult(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_State = STATE_NONE;      // ���
	m_nAllScore = 0;           // ���v�X�R�A
	m_nCounter = 0;            // ��Ԃ��Ǘ�����J�E���^�[
	m_pPress = NULL;           // �v���X�G���^�[�p��2D�|���S���N���X�ւ̃|�C���^
	m_pTextureManager = NULL;  // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	m_pResultBg = NULL;        // �w�i�p�̃|���S��

	for (int nCntResult = 0; nCntResult < TYPE_SCORE_MAX; nCntResult++)
	{// �X�R�A�̎�ނ����J��Ԃ�
		m_apNumber[nCntResult] = NULL;     // ���ꂼ��̃X�R�A�\���p�̃|���S��
		m_aDigit[nCntResult] = NULL;       // ���ꂼ��̃X�R�A�̌���
		m_apScoreLogo[nCntResult] = NULL;  // ���ꂼ��̃X�R�A�̃��S�\���p�|���S��
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//    ��������
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pResult = NULL;  // ���U���g�N���X�^�̃|�C���^
	if (pResult == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pResult = new CResult;
		if (pResult != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pResult->Init();
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
	return pResult;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	// ���U���g�̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(RESULT_SYSTEM_FILENAME, "r");
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
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
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

	// �w�i�̍쐬
	if (m_pResultBg == NULL)
	{
		m_pResultBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (m_pResultBg != NULL)
		{// ���������m�ۂł���
			m_pResultBg->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_6));
		}
	}

	// ���v�X�R�A�̌v�Z�����Ă���
	m_nAllScore = m_nGameScore + m_nStageClearBonus + m_nComboMaxBonus + m_nLastBossBonus + m_nHiddenItemBonus;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// PressEnter�̔j��
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	// �����|���S���̔j��
	for (int nCntResult = 0; nCntResult < TYPE_SCORE_MAX; nCntResult++)
	{// �X�R�A�̎�ނ����J��Ԃ�
		if (m_apNumber[nCntResult] != NULL)
		{// ���������m�ۂ���Ă���
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntResult]; nCntDigit++)
			{// �����L���O�X�R�A�̌������J��Ԃ�
				if (m_apNumber[nCntResult][nCntDigit] != NULL)
				{// ���������m�ۂ���Ă���
					m_apNumber[nCntResult][nCntDigit]->Uninit();
					m_apNumber[nCntResult][nCntDigit] = NULL;
				}
			}
			delete m_apNumber[nCntResult];
			m_apNumber[nCntResult] = NULL;
		}
	}

	// �����L���O�ɍ��v�X�R�A��ݒ�
	CRanking::SetGameScore(m_nAllScore);

	// �X�R�A�����������Ă���
	m_nGameScore = 0;           // �Q�[���̃X�R�A
	m_nStageClearBonus = 0;     // �X�e�[�W�N���A�{�[�i�X
	m_nComboMaxBonus = 0;       // �ő�R���{�{�[�i�X
	m_nLastBossBonus = 0;       // ���X�{�X���j�{�[�i�X
	m_nHiddenItemBonus = 0;     // �X�e�[�W�N���A�{�[�i�X

	// ���U���g�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CResult::Update(void)
{
	// �f�o�b�O�p
	//if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	//{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
	//	CManager::GetFade()->SetFade(CManager::MODE_RANKING);
	//}

	m_nCounter++;  // �J�E���^�[��i�߂�

	if (m_State == STATE_APPEAR_ALLSCORE)
	{// �S�ẴX�R�A���o�����Ă����ԂȂ��
		ModeChangeCheck();
	}
	else
	{// ����ȊO�̏�ԂȂ��
		if (m_State == STATE_NONE)
		{// �܂������o�Ă��Ȃ���Ԃ�������
			if (m_nCounter >= RESULT_SCOREAPPEAR_TIMING + 60)
			{// �J�E���^�[������̒l�𒴂���
				CreateNumber();
				m_nCounter = 0;
			}
		}
		else
		{// �X�R�A�����X�ɏo�Ă����Ԃ�������
			if (m_nCounter >= RESULT_SCOREAPPEAR_TIMING)
			{// �J�E���^�[������̒l�𒴂���
				CreateNumber();
				m_nCounter = 0;
			}
		}
	}

	CDebugProc::Print("���U���g���\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
//    �X�R�A��\�������鏈��
//=============================================================================
void CResult::CreateNumber(void)
{
	// �����|���S�������ɕK�v�ȕϐ���錾
	D3DXVECTOR3 NumberPos;   // ���W
	D3DXCOLOR NumberCol;     // �F
	float fNumberWidth;      // ��
	float fNumberHeight;     // ����
	int nNumber = 0;         // �\�����鐔���̌v�Z�p
	int nTexNumber = 0;      // �g�p����e�N�X�`���̔ԍ�
	int nScore = 0;          // �\������X�R�A(�����v�Z�p)

	// ���S�|���S�������ɕK�v�ȕϐ���錾
	D3DXVECTOR3 LogoPos;
	float fLogoWidth;
	float fLogoHeight;

	// �ϐ��ɒl����(��Ԃɂ���ď����킯)
	switch (m_State)
	{
	case STATE_NONE:                     // �܂������o�Ă��Ȃ����
		NumberPos = RESULT_GAMESCORE_POS;               // ���W
		NumberCol = RESULT_GAMESCORE_COL;               // �F
		fNumberWidth = RESULT_GAMESCORE_WIDTH;          // ��
		fNumberHeight = RESULT_GAMESCORE_HEIGHT;        // ����
		nTexNumber = TEX_NUMBER_0;                      // �g�p����e�N�X�`���̔ԍ�
		nScore = m_nGameScore;                          // �\������X�R�A
		m_State = STATE_APPEAR_GAME_SCORE;              // ���
		LogoPos = RESULT_LOGO_POS_0;                    // ���S�̍��W
		fLogoWidth = RESULT_LOGO_WIDTH_0;               // ���S�̕�
		fLogoHeight = RESULT_LOGO_HEIGHT_0;             // ���S�̍���
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_GAME_SCORE:        // �Q�[���̃X�R�A���o�Ă�����
		NumberPos = RESULT_STAGECLEAR_BONUS_POS;        // ���W
		NumberCol = RESULT_STAGECLEAR_BONUS_COL;        // �F
		fNumberWidth = RESULT_STAGECLEAR_BONUS_WIDTH;   // ��
		fNumberHeight = RESULT_STAGECLEAR_BONUS_HEIGHT; // ����
		nTexNumber = TEX_NUMBER_1;                      // �g�p����e�N�X�`���̔ԍ�
		nScore = m_nStageClearBonus;                    // �\������X�R�A
		m_State = STATE_APPEAR_STAGECLEAR_BONUS;        // ���
		LogoPos = RESULT_LOGO_POS_1;                    // ���S�̍��W
		fLogoWidth = RESULT_LOGO_WIDTH_1;               // ���S�̕�
		fLogoHeight = RESULT_LOGO_HEIGHT_1;             // ���S�̍���
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_STAGECLEAR_BONUS:  // �X�e�[�W�N���A�{�[�i�X���o�Ă�����
		NumberPos = RESULT_COMBOMAX_BONUS_POS;          // ���W
		NumberCol = RESULT_COMBOMAX_BONUS_COL;          // �F
		fNumberWidth = RESULT_COMBOMAX_BONUS_WIDTH;     // ��
		fNumberHeight = RESULT_COMBOMAX_BONUS_HEIGHT;   // ����
		nTexNumber = TEX_NUMBER_2;                      // �g�p����e�N�X�`���̔ԍ�
		nScore = m_nComboMaxBonus;                      // �\������X�R�A
		m_State = STATE_APPEAR_COMBOMAX_BONUS;          // ���
		LogoPos = RESULT_LOGO_POS_2;                    // ���S�̍��W
		fLogoWidth = RESULT_LOGO_WIDTH_2;               // ���S�̕�
		fLogoHeight = RESULT_LOGO_HEIGHT_2;             // ���S�̍���
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_COMBOMAX_BONUS:    // �ő�R���{�{�[�i�X���o�Ă�����
		NumberPos = RESULT_LASTBOSS_BONUS_POS;          // ���W
		NumberCol = RESULT_LASTBOSS_BONUS_COL;          // �F
		fNumberWidth = RESULT_LASTBOSS_BONUS_WIDTH;     // ��
		fNumberHeight = RESULT_LASTBOSS_BONUS_HEIGHT;   // ����
		nTexNumber = TEX_NUMBER_3;                      // �g�p����e�N�X�`���̔ԍ�
		nScore = m_nLastBossBonus;                      // �\������X�R�A
		m_State = STATE_APPEAR_LASTBOSS_BONUS;          // ���
		LogoPos = RESULT_LOGO_POS_3;                    // ���S�̍��W
		fLogoWidth = RESULT_LOGO_WIDTH_3;               // ���S�̕�
		fLogoHeight = RESULT_LOGO_HEIGHT_3;             // ���S�̍���
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_LASTBOSS_BONUS:    // ���X�{�X���j�{�[�i�X���o�Ă�����
		NumberPos = RESULT_HIDDENITEM_BONUS_POS;        // ���W
		NumberCol = RESULT_HIDDENITEM_BONUS_COL;        // �F
		fNumberWidth = RESULT_HIDDENITEM_BONUS_WIDTH;   // ��
		fNumberHeight = RESULT_HIDDENITEM_BONUS_HEIGHT; // ����
		nTexNumber = TEX_NUMBER_4;                      // �g�p����e�N�X�`���̔ԍ�
		nScore = m_nHiddenItemBonus;                    // �\������X�R�A
		m_State = STATE_APPEAR_HIDDENITEM_BONUS;        // ���
		LogoPos = RESULT_LOGO_POS_4;                    // ���S�̍��W
		fLogoWidth = RESULT_LOGO_WIDTH_4;               // ���S�̕�
		fLogoHeight = RESULT_LOGO_HEIGHT_4;             // ���S�̍���
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT000);
		break;
	case STATE_APPEAR_HIDDENITEM_BONUS:  // �B���A�C�e���{�[�i�X���o�Ă�����
		NumberPos = RESULT_ALLSCORE_POS;                // ���W
		NumberCol = RESULT_ALLSCORE_COL;                // �F
		fNumberWidth = RESULT_ALLSCORE_WIDTH;           // ��
		fNumberHeight = RESULT_ALLSCORE_HEIGHT;         // ����
		nTexNumber = TEX_NUMBER_5;                      // �g�p����e�N�X�`���̔ԍ�
		nScore = m_nAllScore;                           // �\������X�R�A
		m_State = STATE_APPEAR_ALLSCORE;                // ���
		LogoPos = RESULT_LOGO_POS_5;                    // ���S�̍��W
		fLogoWidth = RESULT_LOGO_WIDTH_5;               // ���S�̕�
		fLogoHeight = RESULT_LOGO_HEIGHT_5;             // ���S�̍���
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RESULT001);
		break;
	}

	// �\������X�R�A�̌������v�Z
	m_aDigit[nTexNumber] = (int)log10f((float)nScore) + 1;  // ���������߂�
	if (m_aDigit[nTexNumber] <= 0)
	{// ������0�ȉ��Ȃ��
		m_aDigit[nTexNumber] = 1; // 0��\�����镪��ǉ�
	}

	// �����̐���
	m_apNumber[nTexNumber] = new CNumber*[m_aDigit[nTexNumber]];   // �������������m��
	if (m_apNumber[nTexNumber] != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntScore = 0; nCntScore < m_aDigit[nTexNumber]; nCntScore++)
		{// �X�R�A�̌������J��Ԃ�
		    // �\�����鐔���̌v�Z
			nNumber = nScore % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

			// �����|���S���̃C���X�^���X��������
			m_apNumber[nTexNumber][nCntScore] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber);
			if (m_apNumber[nTexNumber][nCntScore] != NULL)
			{// ���������m�ۂł���
				m_apNumber[nTexNumber][nCntScore]->BindTexture(m_pTextureManager->GetTexture(8));
			}

			// ���ɐi�߂�
			NumberPos.x -= fNumberWidth + (fNumberWidth * RESULT_NUMBER_INTERVAL);
		}
	}

	// ���S�|���S���̐���
	if (m_apScoreLogo[nTexNumber] == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apScoreLogo[nTexNumber] = CScene2D::Create(LogoPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fLogoWidth, fLogoHeight);
		if (m_apScoreLogo[nTexNumber] != NULL)
		{// �����ł���
			m_apScoreLogo[nTexNumber]->BindTexture(m_pTextureManager->GetTexture(nTexNumber));
		}
	}

	if (m_State == STATE_APPEAR_ALLSCORE)
	{// ���v�X�R�A���o�Ă����ԂɂȂ���
	    // PressEnter�̍쐬
		if (m_pPress == NULL)
		{
			m_pPress = CScene2DPress::Create(RESULT_PRESS_POS, RESULT_PRESS_COL, RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT);
			if (m_pPress != NULL)
			{// ���������m�ۂł���
				m_pPress->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_7));
			}
		}
	}
}

//=============================================================================
//    �J�ڂ��邩�ǂ����`�F�b�N���鏈��
//=============================================================================
void CResult::ModeChangeCheck(void)
{
	if (m_nCounter >= RESULT_MODECHANGE_TIMING)
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
				CManager::GetFade()->SetFade(CManager::MODE_RANKING);
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
						CManager::GetFade()->SetFade(CManager::MODE_RANKING);
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
//    �Q�[���X�R�A��ݒ肷�鏈��
//=============================================================================
void CResult::SetGameScore(int nGameScore)
{
	m_nGameScore = nGameScore;
}

//=============================================================================
//    �X�e�[�W�N���A�{�[�i�X��ݒ肷�鏈��
//=============================================================================
void CResult::SetStageClearBonus(int nStageBonus)
{
	m_nStageClearBonus = nStageBonus;
}

//=============================================================================
//    �ő�R���{���{�[�i�X��ݒ肷�鏈��
//=============================================================================
void CResult::SetComboMaxBonus(int nComboMaxBonus)
{
	m_nComboMaxBonus = nComboMaxBonus;
}

//=============================================================================
//    ���X�{�X���j�{�[�i�X��ݒ肷�鏈��
//=============================================================================
void CResult::SetLastBossBonus(int nLastBossBonus)
{
	m_nLastBossBonus = nLastBossBonus;
}

//=============================================================================
//    �B���A�C�e���{�[�i�X��ݒ肷�鏈��
//=============================================================================
void CResult::SetHiddenItemBonus(int nHiddenItemBonus)
{
	m_nHiddenItemBonus = nHiddenItemBonus;
}