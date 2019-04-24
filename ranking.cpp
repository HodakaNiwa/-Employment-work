//*****************************************************************************
//
//     �����L���O�̏���[ranking.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"
#include "particle2D.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RANKING_POLYGON_PRIORITY     (10)                                        // �����L���O��ʂ̃|���S���̏����D�揇��
#define RANKING_SYSTEM_FILENAME      "data/TEXT/MODE/ranking.ini"                // �����L���O�̏��������X�N���v�g�t�@�C����
#define RANKING_SCORE_FILENAME       "data/TEXT/RANKING/ranking_score.txt"       // �����L���O�X�R�A���X�N���v�g�t�@�C����
#define RANKING_MODECHANGE_TIMING    (600)                                       // ��ʑJ�ڂ������ōs���^�C�~���O
#define RANKING_NUMBER_INTERVAL      (0.9f)                                      // �����Ɛ����̊Ԋu

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                  "NUM_TEXTURE = "                            // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME             "TEXTURE_FILENAME = "                       // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define MAP_FILENAME                 "MAP_FILENAME = "                           // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
#define RANKINGSCORE_FILENAME        "RANKINGSCORE_FILENAME = "                  // �����L���O�X�R�A�̃X�N���v�g�t�@�C����
#define RANKING_SCORE                "RANKING_SCORE = "                          // �����L���O�X�R�A

// �G�t�F�N�g��p�}�N��
#define RANKING_PARTICLE_APPEAR      (1)                                         // ���̍X�V�ŏo���p�[�e�B�N���̗�
#define RANKING_ORBIT_WIDTH          (45.0f)                                     // �O�ՃG�t�F�N�g�̉���
#define RANKING_ORBIT_HEIGHT         (25.0f)                                     // �O�ՃG�t�F�N�g�̏c��
#define RANKING_PARTICLE_COL_0       (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))            // �����L���O�p�[�e�B�N���̐F(1��)
#define RANKING_PARTICLE_COL_1       (D3DXCOLOR(0.6f,0.6f,0.6f,1.0f))            // �����L���O�p�[�e�B�N���̐F(2��)
#define RANKING_PARTICLE_COL_2       (D3DXCOLOR(0.6f,0.1f,0.0f,1.0f))            // �����L���O�p�[�e�B�N���̐F(3��)
#define RANKING_PARTICLE_COL_3       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O�p�[�e�B�N���̐F(4��)
#define RANKING_PARTICLE_COL_4       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O�p�[�e�B�N���̐F(5��)

// �����L���O�X�R�A�p�̃}�N��
#define RANKING_SCORE_POS_INI        (D3DXVECTOR3(3000.0f,165.0f,0.0f))          // �����L���O�X�R�A�̍��W(�����l)
#define RANKING_SCORE_POS_MAX        (950.0f)                                    // �����L���O�X�R�A�̍��W�̍ő�l
#define RANKING_SCORE_WIDTH_INI      (45.0f)                                     // �����L���O�X�R�A�̕�(�����l)
#define RANKING_SCORE_HEIGHT_INI     (40.0f)                                     // �����L���O�X�R�A�̍���(�����l)
#define RANKING_SCORE_MOVE_INI       (D3DXVECTOR3(-15.0f,0.0f,0.0f))             // �����L���O�X�R�A�̈ړ���(�����l)
#define RANKING_SCORE_COL_0          (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))            // �����L���O�X�R�A�̐F(1��)
#define RANKING_SCORE_COL_1          (D3DXCOLOR(0.6f,0.6f,0.6f,1.0f))            // �����L���O�X�R�A�̐F(2��)
#define RANKING_SCORE_COL_2          (D3DXCOLOR(0.9f,0.7f,0.0f,1.0f))            // �����L���O�X�R�A�̐F(3��)
#define RANKING_SCORE_COL_3          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O�X�R�A�̐F(4��)
#define RANKING_SCORE_COL_4          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O�X�R�A�̐F(5��)

// �����L���O���ʗp�̃}�N��
#define RANKING_RANK_POS_INI         (D3DXVECTOR3(2400.0f,165.0f,0.0f))          // �����L���O���ʂ̍��W(�����l)
#define RANKING_RANK_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O���ʂ̐F(�����l)
#define RANKING_RANK_WIDTH_INI       (100.0f)                                    // �����L���O���ʂ̕�(�����l)
#define RANKING_RANK_HEIGHT_INI      (70.0f)                                     // �����L���O���ʂ̍���(�����l)
#define RANKING_RANK_POS_MAX         (350.0f)                                    // �����L���O���ʂ̍��W�̍ő�l

// �����L���O���S�p�̃}�N��
#define RANKING_LOGO_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 2,65.0f,0.0f))  // �����L���O���S�̍��W(�����l)
#define RANKING_LOGO_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O���S�̐F(�����l)
#define RANKING_LOGO_WIDTH_INI       (300.0f)                                    // �����L���O���S�̕�(�����l)
#define RANKING_LOGO_HEIGHT_INI      (60.0f)                                     // �����L���O���S�̍���(�����l)

// �����L���O�J�����p�̃}�N��
#define RANKING_CAMERA_PATTERN_NUM    (3)                                        // �����L���O�J�����̃p�^�[����
#define RANKING_CAMERA_START_POSV_Y   (D3DXVECTOR3(0.0f,575.0f,0.0f))            // �����L���O�X�^�[�g���̃J�����̎��_�̍���
#define RANKING_CAMERA_START_POSR     (D3DXVECTOR3(-4200.0f,450.0f,-4450.0f))    // �����L���O�X�^�[�g���̃J�����̒����_�̈ʒu
#define RANKING_CAMERA_DEST_POSR      (D3DXVECTOR3(-4200.0f,450.0f,100.0f))      // �����L���O�X�^�[�g���̃J�����̒����_�̖ړI�̈ʒu
#define RANKING_CAMERA_MOVESPEED      (0.002f)                                   // �����L���O�X�^�[�g���̃J�����̈ړ��X�s�[�h

// �p�^�[��1
#define RANKING_CAMERA_SWITCHTIMING_0 (300)                                      // 1�ڂ̃p�^�[������2�ڂ̃p�^�[���փ^�C�g���J������؂�ւ���^�C�~���O
#define RANKING_CAMERA_0_POSV_Y       (D3DXVECTOR3(0.0f,50.0f,0.0f))             // �p�^�[��2�ڂ̃J�����̎��_�̍���
#define RANKING_CAMERA_0_POSR         (D3DXVECTOR3(1200.0f,110.0f,-3780.0f))     // �p�^�[��2�ڂ̃J�����̒����_�̈ʒu
#define RANKING_CAMERA_0_POSRDEST     (D3DXVECTOR3(-1500.0f,110.0f,-3780.0f))    // �p�^�[��2�ڂ̃J�����̒����_�̖ړI�̈ʒu
#define RANKING_CAMERA_0_MOVESPEED    (0.0025f)                                  // �p�^�[��2�ڂ̃J�����̈ړ��X�s�[�h
#define RANKING_CAMERA_0_ROT          (D3DXVECTOR3(0.0f,-D3DX_PI / 2,0.0f))      // �p�^�[��2�ڂ̃J��������

// �p�^�[��2
#define RANKING_CAMERA_SWITCHTIMING_1 (480)                                       // 2�ڂ̃p�^�[������3�ڂ̃p�^�[���փ^�C�g���J������؂�ւ���^�C�~���O
#define RANKING_CAMERA_1_POSV_Y       (D3DXVECTOR3(0.0f,40.0f,0.0f))              // �p�^�[��3�ڂ̃J�����̎��_�̍���
#define RANKING_CAMERA_1_POSR         (D3DXVECTOR3(1200.0f,100.0f,-3780.0f))      // �p�^�[��3�ڂ̃J�����̒����_�̈ʒu
#define RANKING_CAMERA_1_POSRDEST     (D3DXVECTOR3(1700.0f,100.0f,-3780.0f))      // �p�^�[��3�ڂ̃J�����̒����_�̖ړI�̈ʒu
#define RANKING_CAMERA_1_MOVESPEED    (0.002f)                                    // �p�^�[��3�ڂ̃J�����̈ړ��X�s�[�h
#define RANKING_CAMERA_1_ROT          (D3DXVECTOR3(0.0f,D3DX_PI / 2,0.0f))        // �p�^�[��3�ڂ̃J��������

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
int CRanking::m_nGameScore = 0;       // �Q�[���̃X�R�A

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRanking::CRanking(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		m_aDigit[nCntRanking] = 0;                            // �����L���O�X�R�A�̌���
		m_apNumber[nCntRanking] = NULL;                       // �����L���O�X�R�A�\���p�̃|���S��
		m_Move[nCntRanking] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �����L���O�X�R�A�̈ړ���
		m_apRankingRank[nCntRanking] = NULL;                  // �����L���O���ʕ\���p�̃|���S��
		m_aRankingScore[nCntRanking] = 0;                     // �����L���O�X�R�A
		m_aScoreState[nCntRanking] = STATE_NONE;              // �����L���O�X�R�A�̏��
	}
	m_pMap = NULL;              // �}�b�v�N���X�ւ̃|�C���^
	m_pTextureManager = NULL;   // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pRankingLogo = NULL;      // �����L���O���S�p�\���p�̃|���S��
	m_nModeCounter = 0;         // �J�ڂ��Ǘ�����J�E���^�[
	m_nCameraCounter = 0;       // �J�����̓������Ǘ�����J�E���^�[
	m_nPatternCamera = 0;       // ���݂̃����L���O�J�����̃p�^�[���ԍ�
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRanking *CRanking::Create(void)
{
	CRanking *pRanking = NULL;  // �����L���O�N���X�^�̃|�C���^
	if (pRanking == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pRanking = new CRanking;
		if (pRanking != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pRanking ->Init();
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
	return pRanking;
}

//=============================================================================
//    �Q�[���X�R�A�̐ݒ�
//=============================================================================
void CRanking::SetGameScore(int nScore)
{
	m_nGameScore = nScore;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	// �����L���O�̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(RANKING_SYSTEM_FILENAME, "r");
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
			char aMapFileName[256] = "\0";              // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
			char aRankingScoreFileName[256] = "\0";     // �����L���O�X�R�A�̃X�N���v�g�t�@�C����

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
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
							    // �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RANKINGSCORE_FILENAME) == 0)
							{// �����L���O�X�R�A�̃X�N���v�g�t�@�C����������
							    // �����L���O�X�R�A�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, RANKINGSCORE_FILENAME);
								strcpy(aRankingScoreFileName, pStr);

								// �����L���O�X�R�A���O���t�@�C������ǂݍ���
								FILE *pRankingScoreFile = NULL;  // �t�@�C���|�C���^
								int nCntRankingScore = 0;        // �����L���O�X�R�A��ǂݎ������

								if (pRankingScoreFile == NULL)
								{// �t�@�C���|�C���^���m�ۂł����Ԃł���
									pRankingScoreFile = fopen(aRankingScoreFileName, "r");
									if (pRankingScoreFile != NULL)
									{// �t�@�C�����J����
										pStrCur = CFunctionLib::ReadLine(pRankingScoreFile, pLine);  // �L���ȕ������������
										if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
										{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
											while (1)
											{// ���[�v�J�n
												pStrCur = CFunctionLib::ReadLine(pRankingScoreFile, pLine);  // �L���ȕ������������
												if (CFunctionLib::Memcmp(pStrCur, RANKING_SCORE) == 0)
												{// �����L���O�X�R�A������
													m_aRankingScore[nCntRankingScore] = CFunctionLib::ReadInt(pStrCur, RANKING_SCORE);
													nCntRankingScore++;
												}
												else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
												{// �X�N���v�g�ǂݍ��ݏI���̍��}������
													break;  // ���[�v�I��
												}
											}
										}

										// �t�@�C�������
										fclose(pRankingScoreFile);
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �}�b�v�̐���
					m_pMap = CMap::Create(aMapFileName);

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

	// �����L���O�X�R�A������ւ���Ă��Ȃ����`�F�b�N����
	int nScoreChange = m_nGameScore; // ����ւ���X�R�A
	int nScoreOld = 0;               // �����L���O�X�R�A�̈ꎞ�I�Ȋi�[�ꏊ
	bool bSet = false;               // �����L���O���X�V���ꂽ���ǂ���

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
		if (m_aRankingScore[nCntRanking] < nScoreChange)
		{// �o�^����Ă���X�R�A������ւ���X�R�A���傫��
			nScoreOld = m_aRankingScore[nCntRanking];    // ���̃X�R�A���i�[
			m_aRankingScore[nCntRanking] = nScoreChange; // �X�R�A�X�V
			nScoreChange = nScoreOld;                    // ���̃X�R�A�����ւ���Ώۂ�
			if (bSet == false)
			{// �X�V����Ă��Ȃ�������
				m_aScoreState[nCntRanking] = STATE_UPDATE;     // �X�V���ꂽ��Ԃ�
				bSet = true;                                   // �X�V������Ԃ�
			}
		}
	}


	// �����L���O�X�R�A�̐���
	D3DXVECTOR3 NumberPos = RANKING_SCORE_POS_INI;     // ���W
	float fNumberWidth = RANKING_SCORE_WIDTH_INI;      // ��
	float fNumberHeight = RANKING_SCORE_HEIGHT_INI;    // ����
	int nNumber = 0;                                   // �\�����鐔���̌v�Z�p
	CNumber::STATE NumberState = CNumber::STATE_NONE;  // ���
	D3DXCOLOR NumberCol[MAX_RANKING] =                 // �F
	{
		RANKING_SCORE_COL_0,
		RANKING_SCORE_COL_1,
		RANKING_SCORE_COL_2,
		RANKING_SCORE_COL_3,
		RANKING_SCORE_COL_4,
	};

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		// ��Ԃ̐ݒ�
		if (m_aScoreState[nCntRanking] == STATE_UPDATE)
		{// �X�V���ꂽ��Ԃ�������
			NumberState = CNumber::STATE_FLASH;
		}

		// �����̌v�Z
		m_aDigit[nCntRanking] = (int)log10f((float)m_aRankingScore[nCntRanking]) + 1;  // ���������߂�
		if (m_aDigit[nCntRanking] <= 0) { m_aDigit[nCntRanking] = 1; }	               // 0�ȉ��̂Ƃ�1�ɂ���
		m_apNumber[nCntRanking] = new CNumber*[m_aDigit[nCntRanking]];		           // �������������m��
		if (m_apNumber[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			for (int nCntScore = 0; nCntScore < m_aDigit[nCntRanking]; nCntScore++)
			{// �����L���O�X�R�A�̌������J��Ԃ�
				// �\�����鐔���̌v�Z
				nNumber = m_aRankingScore[nCntRanking] % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

				// �����|���S���̃C���X�^���X��������
				m_apNumber[nCntRanking][nCntScore] = CNumber::Create(NumberPos, NumberCol[nCntRanking], fNumberWidth, fNumberHeight, 0.0f, NumberState, nNumber, 3, RANKING_POLYGON_PRIORITY);
				if (m_apNumber[nCntRanking][nCntScore] != NULL)
				{// ���������m�ۂł���
					m_apNumber[nCntRanking][nCntScore]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
				}

				// ���ɐi�߂�
				NumberPos.x -= fNumberWidth + (fNumberWidth * RANKING_NUMBER_INTERVAL);
			}
		}
		// ���ɐi�߂�
		NumberPos = RANKING_SCORE_POS_INI;
		NumberPos.x += 500.0f * (nCntRanking + 1);
		NumberPos.y += (RANKING_SCORE_HEIGHT_INI + (RANKING_SCORE_HEIGHT_INI * 2.0f)) * (nCntRanking + 1);

		if (nCntRanking == 2)
		{// �����L���O��4�߂ɂȂ���
			fNumberWidth *= 0.75f;
			fNumberHeight *= 0.75f;
		}
		// ��Ԃ�߂��Ă���
		NumberState = CNumber::STATE_NONE;
	}

	// �����L���O���ʃ|���S���̃C���X�^���X����
	D3DXVECTOR3 RankPos = RANKING_RANK_POS_INI;   // ���W
	float fRankWidth = RANKING_RANK_WIDTH_INI;    // ����
	float fRankHeight = RANKING_RANK_HEIGHT_INI;  // �c��
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
	    // �ړ��ʂ̐ݒ�
		m_Move[nCntRanking] = RANKING_SCORE_MOVE_INI;

		if (m_apRankingRank[nCntRanking] == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_apRankingRank[nCntRanking] = CScene2D::Create(RankPos, RANKING_RANK_COL_INI, fRankWidth, fRankHeight, 0.0f, RANKING_POLYGON_PRIORITY);
			if (m_apRankingRank[nCntRanking] != NULL)
			{// ���������m�ۂł���
				m_apRankingRank[nCntRanking]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));

				// �e�N�X�`�����W�����炷
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apRankingRank[nCntRanking]->GetVtxBuff();
				if(pVtxBuff != NULL)
				{// ���_�o�b�t�@���擾�ł���
				    // ���_���
					VERTEX_2D *pVtx;

					// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// �e�N�X�`�����W
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f + (0.2f * nCntRanking));
					pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f + (0.2f * nCntRanking));

					// ���_�o�b�t�@���A�����b�N����
					pVtxBuff->Unlock();
				}
			}
			RankPos = RANKING_RANK_POS_INI;
			RankPos.x += 500.0f * (nCntRanking + 1);
			RankPos.y += (RANKING_RANK_HEIGHT_INI + (RANKING_RANK_HEIGHT_INI * 0.75f)) * (nCntRanking + 1);

			if (nCntRanking == 2)
			{// �����L���O��4�߂ɂȂ���
				fRankWidth *= 0.75f;
				fRankHeight *= 0.75f;
			}
		}
	}

	// �����L���O���S�̐���
	if (m_pRankingLogo == NULL)
	{
		m_pRankingLogo = CScene2D::Create(RANKING_LOGO_POS_INI, RANKING_LOGO_COL_INI, RANKING_LOGO_WIDTH_INI, RANKING_LOGO_HEIGHT_INI, 0.0f, RANKING_POLYGON_PRIORITY);
		if (m_pRankingLogo != NULL)
		{// ���������m�ۂł���
			m_pRankingLogo->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_2));
		}
	}

	// �J�����̒����_�̈ʒu��ݒ�
	CManager::GetCamera()->SetPosR(RANKING_CAMERA_START_POSR);

	// �J�����̖ړI�̒����_�̈ʒu�ƌ��݂̈ʒu�Ƃ̍�����ݒ�
	CManager::GetCamera()->SetPosRDiff((RANKING_CAMERA_DEST_POSR - RANKING_CAMERA_START_POSR) * RANKING_CAMERA_MOVESPEED);

	// �J�����̎��_�̍�����ݒ�
	CManager::GetCamera()->SetPosVDest(RANKING_CAMERA_START_POSV_Y);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRanking::Uninit(void)
{
	// �Q�[���̃X�R�A������������
	m_nGameScore = 0;

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

	// �����L���O�X�R�A�̔j��
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		if (m_apNumber[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
			{// �����L���O�X�R�A�̌������J��Ԃ�
				if (m_apNumber[nCntRanking][nCntDigit] != NULL)
				{// ���������m�ۂ���Ă���
					m_apNumber[nCntRanking][nCntDigit]->Uninit();
					m_apNumber[nCntRanking][nCntDigit] = NULL;
				}
			}
			delete m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}
	}

	// �����L���O���S�̔j��
	if (m_pRankingLogo != NULL)
	{
		m_pRankingLogo->Uninit();
		m_pRankingLogo = NULL;
	}

	// �����L���O���ʂ̔j��
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
		if (m_apRankingRank[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			m_apRankingRank[nCntRanking]->Uninit();
			m_apRankingRank[nCntRanking] = NULL;
		}
	}

	// �����L���O�X�R�A�̕ۑ�
	Save();

	// �����L���O�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRanking::Update(void)
{
	// �f�o�b�O�p
	//if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	//{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
	//	CManager::GetFade()->SetFade(CManager::MODE_TITLE);
	//}

	// �J�����𓮂���
	SwitchCamera();

	// �����L���O���X���C�h������
	SlideRanking();

	m_nModeCounter++;  // �J�E���^�[��i�߂�

	if (m_nModeCounter >= RANKING_MODECHANGE_TIMING)
	{// �J�E���^�[������̒l�𒴂���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	else
	{// �J�E���^�[���܂�����̒l�𒴂��Ă��Ȃ�
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						CManager::GetFade()->SetFade(CManager::MODE_TITLE);
					}
				}
			}
		}
	}

	CDebugProc::Print("�����L���O���\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
//    �J�����̃p�^�[����؂�ւ��鏈��
//=============================================================================
void CRanking::SwitchCamera(void)
{
	// �J�E���^�[�𑝂₷
	m_nCameraCounter++;

	int nSwitchTiming = 0;   // �J�����̃p�^�[����؂�ւ���^�C�~���O
	switch (m_nPatternCamera)
	{// ���݂̃����L���O�J�����̃p�^�[���ɂ���ď����킯
	case 0:   // �ŏ��̃p�^�[��
		nSwitchTiming = RANKING_CAMERA_SWITCHTIMING_0;
		break;
	case 1:   // �ŏ��̃p�^�[��
		nSwitchTiming = RANKING_CAMERA_SWITCHTIMING_1;
		break;
	}

	if (nSwitchTiming > 0)
	{// �؂�ւ���^�C�~���O���ݒ肳�ꂽ
		if (m_nCameraCounter % nSwitchTiming == 0)
		{// �؂�ւ���^�C�~���O�ɂȂ��Ă���
		    // �J�E���^�[��߂�
			m_nCameraCounter = 0;

			// �p�^�[���ԍ��ɂ���Đݒ肷��l��ς���
			D3DXVECTOR3 PosVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 PosRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fMoveSpeed = 0.0f;
			D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (m_nPatternCamera)
			{// ���݂̃^�C�g���J�����̃p�^�[���ɂ���ď����킯
			case 0:   // �ŏ��̃p�^�[��
				PosVDest = RANKING_CAMERA_0_POSV_Y;
				PosR = RANKING_CAMERA_0_POSR;
				PosRDiff = (RANKING_CAMERA_0_POSRDEST - RANKING_CAMERA_0_POSR);
				fMoveSpeed = RANKING_CAMERA_0_MOVESPEED;
				Rot = RANKING_CAMERA_0_ROT;
				break;
			case 1:   // �ŏ��̃p�^�[��
				PosVDest = RANKING_CAMERA_1_POSV_Y;
				PosR = RANKING_CAMERA_1_POSR;
				PosRDiff = (RANKING_CAMERA_1_POSRDEST - RANKING_CAMERA_1_POSR);
				fMoveSpeed = RANKING_CAMERA_1_MOVESPEED;
				Rot = RANKING_CAMERA_1_ROT;
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
//    �����L���O��������X���C�h�����鏈��
//=============================================================================
void CRanking::SlideRanking(void)
{
	D3DXVECTOR3 RankPos;
	D3DXVECTOR3 NumberPos;

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̐����J��Ԃ�
		// ���ʃ|���S���𓮂���
		if (m_apRankingRank[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			RankPos = m_apRankingRank[nCntRanking]->GetPos();
			RankPos += m_Move[nCntRanking];
			m_apRankingRank[nCntRanking]->SetPos(RankPos);
		}
		// �����|���S���𓮂���
		if (m_apNumber[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
			{// �������J��Ԃ�
				if (m_apNumber[nCntRanking][nCntDigit] != NULL)
				{// ���������m�ۂ���Ă���
					NumberPos = m_apNumber[nCntRanking][nCntDigit]->GetPos();
					NumberPos += m_Move[nCntRanking];
					m_apNumber[nCntRanking][nCntDigit]->SetPos(NumberPos);
				}
			}
			if (RankPos.x <= RANKING_RANK_POS_MAX)
			{// ����ȏ㉡�ɍs���Ȃ�
				if (m_Move[nCntRanking].x != 0.0f)
				{// ����ȏ㉡�ɍs���Ȃ�
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RANKING);
				}

				m_Move[nCntRanking].x = 0.0f;

				// �ʒu�̍Đݒ�
				RankPos.x = RANKING_RANK_POS_MAX;
				NumberPos.x = RANKING_SCORE_POS_MAX;

				// ���ʃ|���S��
				m_apRankingRank[nCntRanking]->SetPos(RankPos);

				// �����|���S��
				for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
				{// �������J��Ԃ�
					if (m_apNumber[nCntRanking][nCntDigit] != NULL)
					{// ���������m�ۂ���Ă���
						m_apNumber[nCntRanking][nCntDigit]->SetPos(NumberPos);
				        // ���ɐi�߂�
						NumberPos.x -= m_apNumber[nCntRanking][nCntDigit]->GetWidth() + (m_apNumber[nCntRanking][nCntDigit]->GetWidth() * RANKING_NUMBER_INTERVAL);
					}
				}
			}
			else
			{// �܂���������
				D3DXVECTOR3 ParEmitterPos = D3DXVECTOR3(m_apRankingRank[nCntRanking]->GetPos().x - m_apRankingRank[nCntRanking]->GetWidth() + 10.0f, m_apRankingRank[nCntRanking]->GetPos().y + m_apRankingRank[nCntRanking]->GetHeight() - 10.0f, 0.0f);
				CreateOrbit(ParEmitterPos, nCntRanking);
			}
		}
	}
}

//=============================================================================
//    �O�Ղ̂悤�ȃG�t�F�N�g�𐶐����鏈��
//=============================================================================
void CRanking::CreateOrbit(D3DXVECTOR3 pos, int nCntRanking)
{
	// �p�[�e�B�N�����o��
	CParticle2D *pPar2D = NULL;
	float fAngle = 0.0f;  // �p�x
	float fMoveX = 0.0f;  // X�����̈ړ���
	float fMoveY = 0.0f;  // Y�����̈ړ���
	float fWidth = 0.0f;  // ����
	float fHeight = 0.0f; // �c��
	float fPosYDef = m_ParEmitterPos.y;
	float fParWidth = RANKING_ORBIT_WIDTH;
	float fParHeight = RANKING_ORBIT_HEIGHT;
	D3DXVECTOR3 ParEmitterPosDef = m_ParEmitterPos;
	D3DXCOLOR ParCol[MAX_RANKING] =
	{
		RANKING_PARTICLE_COL_0,
		RANKING_PARTICLE_COL_1,
		RANKING_PARTICLE_COL_2,
		RANKING_PARTICLE_COL_3,
		RANKING_PARTICLE_COL_4
	};

	if (nCntRanking >= 3)
	{
		fParHeight *= 0.5f;
	}

	// ���o�ꏊ��ݒ�
	m_ParEmitterPos = pos;

	if (m_nModeCounter % 2 == 0)
	{
		for (int nCntPar = 0; nCntPar < RANKING_PARTICLE_APPEAR; nCntPar++)
		{// �o���������J��Ԃ�
		    // ���O�Ƀ����_���Ōv�Z
			fAngle = (D3DX_PI * 0.5f) + ((rand() % 72 * 0.01f) - (rand() % 72 * 0.01f));
			fMoveX = sinf(fAngle) * (rand() % 5 + 5.0f);
			fMoveY = cosf(fAngle) * (rand() % 5 + 5.0f);
			fWidth = rand() % 10 + 25.0f;
			fHeight = rand() % 10 + 25.0f;
			pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(fMoveX, fMoveY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fWidth, fHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.03f), -0.4f, 0.0f, 60, 0, true, RANKING_POLYGON_PRIORITY - 1);
			if (pPar2D != NULL)
			{// ���������m�ۂł���
				pPar2D->BindTexture(m_pTextureManager->GetTexture(3));
			}
		}

		pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ParCol[nCntRanking], fParWidth, fParHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, RANKING_POLYGON_PRIORITY - 1);
		if (pPar2D != NULL)
		{// ���������m�ۂł���
			pPar2D->BindTexture(m_pTextureManager->GetTexture(4));
		}

		// �o���ʒu�𓮂���
		m_ParEmitterPos -= (m_Move[nCntRanking] * 0.3f);

		pPar2D = CParticle2D::Create(m_ParEmitterPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ParCol[nCntRanking], fParWidth, fParHeight, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.01f), 0.0f, 0.0f, 140, 0, true, RANKING_POLYGON_PRIORITY - 1);
		if (pPar2D != NULL)
		{// ���������m�ۂł���
			pPar2D->BindTexture(m_pTextureManager->GetTexture(4));
		}
	}
}

//=============================================================================
//    �����L���O�X�R�A�̕ۑ�����
//=============================================================================
void CRanking::Save(void)
{
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(RANKING_SCORE_FILENAME, "w");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			// �R�����g�������쐬
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "# �����L���O�X�R�A�t�@�C�� [ranking_score.txt]\n");
			fprintf(pFile, "# Author : HODAKA NIWA\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n",SCRIPT);


			// �����L���O�X�R�A�̕ۑ�
			for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
			{// �����L���O�X�R�A�̌������J��Ԃ�
				fprintf(pFile, "%s%d		# %d�ʂ̃����L���O�X�R�A\n", RANKING_SCORE, m_aRankingScore[nCntRanking], nCntRanking + 1);
			}

			// �R�����g�������쐬
			fprintf(pFile, "\n%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

			// �t�@�C�������
			fclose(pFile);
		}
	}
}