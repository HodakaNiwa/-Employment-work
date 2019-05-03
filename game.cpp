//*****************************************************************************
//
//     �Q�[���̏���[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "fade.h"
#include "textureManager.h"
#include "pause.h"
#include "map.h"
#include "functionlib.h"
#include "player.h"
#include "enemy.h"
#include "effectManager.h"
#include "scene2D.h"
#include "ui.h"
#include "score.h"
#include "result.h"
#include "input.h"
#include "shadow.h"
#include "debuglog.h"
#include "river.h"
#include "grass.h"
#include "boxCollider.h"
#include "shadow.h"
#include "object.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define GAME_SET_MINIMAP            (true)                      // �~�j�}�b�v��\�����邩�ǂ���
#define GAME_SET_UI                 (true)                      // UI����ʂɕ\�����邩�ǂ���

#define GAME_SYSTEM_FILENAME        "data/TEXT/MODE/game.ini"   // �Q�[���̏��������X�N���v�g�t�@�C����
#define GAME_MODECHANGE_TIMING      (60)                        // ��ʑJ�ڂ������ōs���^�C�~���O
#define GAME_MAPCLEAR_CHANGETIMING  (60)                        // �}�b�v�N���A��ԂɂȂ��Ă���}�b�v��؂�ւ���܂ł̎���
#define GAME_POLYGON_PRIORITY       (11)                        // �Q�[���Ŏg�p����|���S���̏����D�揇��

#define GAME_STAGECLEAE_BONUS       (10000)                     // �X�e�[�W�N���A���P���̃{�[�i�X
#define GAME_COMBOMAX_BONUS         (100)                       // �R���{���P���̃{�[�i�X
#define GAME_HIDDENITEM_BONUS       (5000)                      // �B���A�C�e�����P���̃{�[�i�X

#define GAME_PLAYER_POSITION_POS    (D3DXVECTOR3(1125.0f,105.0f,0.0f)) // �~�j�}�b�v�ɕ\������v���C���[�̈ʒu�c���p�|���S���̍��W
#define GAME_PLAYER_POSITION_WIDTH  (8.0f)                             // �~�j�}�b�v�ɕ\������v���C���[�̈ʒu�c���p�|���S���̕�
#define GAME_PLAYER_POSITION_HEIGHT (8.0f)                             // �~�j�}�b�v�ɕ\������v���C���[�̈ʒu�c���p�|���S���̍���

#define GAME_DEST_POSITION_POS    (D3DXVECTOR3(1125.0f,10.0f,0.0f))    // �~�j�}�b�v�ɕ\������ړI�n�c���p�|���S���̍��W
#define GAME_DEST_POSITION_WIDTH  (6.0f)                               // �~�j�}�b�v�ɕ\������ړI�n�c���p�|���S���̕�
#define GAME_DEST_POSITION_HEIGHT (6.0f)                               // �~�j�}�b�v�ɕ\������ړI�n�c���p�|���S���̍���

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // �ǂݍ��ރe�N�X�`���̐�
#define NUM_MAP                     "NUM_MAP = "                // �ǂݍ��ރ}�b�v�̐�
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define EFFECT_FILENAME             "EFFECT_FILENAME = "        // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
#define MAP_FILENAME                "MAP_FILENAME = "           // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
#define GAME_TIMER                  "GAME_TIMER = "             // �^�C�}�[�̕b��

// �v���C���[���
#define PLAYERSET                   "PLAYERSET"                 // �v���C���[���ǂݍ��݊J�n�̍��}
#define END_PLAYERSET               "END_PLAYERSET"             // �v���C���[���ǂݍ��ݏI���̍��}
#define LIFE                        "LIFE = "                   // �v���C���[�̗̑�
#define ORBIT_SLASH                 "ORBIT_SLASH = "            // �v���C���[�̋O�ՃQ�[�W��
#define MAX_SPECIAL                 "MAX_SPECIAL = "            // �v���C���[�̕K�E�Q�[�W�̍ő�l��
#define FILENAME                    "FILENAME = "               // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����

// ��̉��p�}�N��
#define GAME_WATERFALL_SOUND_POS      (D3DXVECTOR3(1764.0f,-150.0f,4219.0f)) // ��̉��̍��W
#define GAME_WATERFALL_RANGE          (4000.0f)                              // ��̉�����������͈�

// �{�X�o�����̉��o�p�}�N��
#define GAME_BOSSAPPAER_CAMERA_POSV   (D3DXVECTOR3(100.0f,20.0f,-200.0f))    // �{�X�o�����̃J�������_�̈ʒu
#define GAME_BOSSAPPAER_CAMERA_POSR   (D3DXVECTOR3(0.0f,80.0,0.0f))          // �{�X�o�����̃J���������_�̈ʒu
#define GAME_BOSSAPPEAR_LOGO_TIMING_0 (120)                                  // ���S���o���^�C�~���O(1��)
#define GAME_BOSSAPPEAR_LOGO_TIMING_1 (180)                                  // ���S���o���^�C�~���O(2��)
#define GAME_BOSSAPPEAR_EFFECT_TIMING (240)                                  // �G�t�F�N�g���o���^�C�~���O
#define GAME_BOSSAPPEAR_STATE_TIMING  (380)                                  // ��Ԃ�؂�ւ���^�C�~���O

// �{�X���j���̉��o�p�}�N��
#define GAME_KNOCKDOWN_CAMERA_TIMING  (180)                                  // �{�X���j���̃J������ʏ펞�ɖ߂��^�C�~���O
#define GAME_KNOCKDOWN_CAMERA_LENGTH  (300.0f)                               // �{�X���j���̃J�����̒���
#define GAME_KNOCKDOWN_CAMERA_POSY    (280.0f)                               // �{�X���j���̃J������Y���W

// �{�X�o�����̕����|���S���p�}�N��(1��)
#define GAME_BOSSAPPEAR_POS_0         (D3DXVECTOR3(370.0f,350.0f,0.0f))      // �{�X�o�����̕����|���S���̍��W(1��)
#define GAME_BOSSAPPEAR_COL_0         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // �{�X�o�����̕����|���S���̐F(1��)
#define GAME_BOSSAPPEAR_WIDTH_0       (330.0f)                               // �{�X�o�����̕����|���S���̕�(1��)
#define GAME_BOSSAPPEAR_HEIGHT_0      (180.0f)                               // �{�X�o�����̕����|���S���̍���(1��)

// �{�X�o�����̕����|���S���p�}�N��(1��)
#define GAME_BOSSAPPEAR_POS_1         (D3DXVECTOR3(970.0f,350.0f,0.0f))      // �{�X�o�����̕����|���S���̍��W(2��)
#define GAME_BOSSAPPEAR_COL_1         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // �{�X�o�����̕����|���S���̐F(2��)
#define GAME_BOSSAPPEAR_WIDTH_1       (330.0f)                               // �{�X�o�����̕����|���S���̕�(2��)
#define GAME_BOSSAPPEAR_HEIGHT_1      (180.0f)                               // �{�X�o�����̕����|���S���̍���(2��)

// �����ǂݍ��ݗp�}�N��
#define GRASSSET                      "GRASSSET"                             // �����ǂݍ��݊J�n
#define SWAY                          "SWAY = "                              // �����h��邩�ǂ���
#define POS                           "POS = "                               // ���W���
#define ROT                           "ROT = "                               // �������
#define WIDTH                         "WIDTH = "                             // �����
#define HEIGHT                        "HEIGHT = "                            // �������
#define END_GRASSSET                  "END_GRASSSET"                         // �����ǂݍ��ݏI��

// �����蔻��p�����f�����ǂݍ��ݗp�}�N��
#define BOXCOLLIDERSET                "BOXCOLLIDERSET"                       // �����蔻��p�����f�����ǂݍ��݊J�n
#define DEPTH                         "DEPTH = "                             // ���̉��s���
#define RETURN                        "RETURN = "                            // ���������ۂɉ����߂����ǂ���
#define END_BOXCOLLIDERSET            "END_BOXCOLLIDERSET"                   // �����蔻��p�����f�����ǂݍ��ݏI��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGame::CGame(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pPlayer = NULL;                           // �v���C���[�N���X�ւ̃|�C���^�ۑ��p
	m_pUi = NULL;                               // UI�N���X�ւ̃|�C���^�ۑ��p
	m_pEffectManager = NULL;                    // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^�ۑ��p
	m_pMap = NULL;                              // �}�b�v�N���X�ւ̃|�C���^
	m_State = STATE_NORMAL;                     // ���
	m_nStateCounter = 0;                        // ��Ԃ��Ǘ�����J�E���^�[
	m_pTextureManager = NULL;                   // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_nClearStageNum = 0;                       // �N���A�����X�e�[�W�̐�
	m_nGetHiddenItem = 0;                       // ��ɓ��ꂽ�B���A�C�e���̐�
	m_nComboMax = 0;                            // �Q�[����ʂ����ő�R���{��
	m_nNumMap = 0;                              // �}�b�v��ǂݍ��ސ�
	m_nMapIdx = 0;                              // ���݂̃}�b�v�ԍ�
	m_bMapClear = false;                        // �}�b�v���N���A�������ǂ���
	m_bMiniMapDisp = false;                     // �~�j�}�b�v��`�悷�邩�ǂ���
	m_nBossAppearCounter = 0;                   // �{�X�o�����̉��o���Ǘ�����J�E���^�[
	m_nKnockDownCounter = 0;                    // �{�X���j���̉��o���Ǘ�����J�E���^�[
	m_BossPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �{�X�̍��W(�ۑ��p)
	m_pDestPosition = NULL;
	m_pPlayerPosition = NULL;
	for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
	{// ���S���o���������J��Ԃ�
		m_pBossAppearLogo[nCntAppear] = NULL;
	}
	for (int nCntMap = 0; nCntMap < GAME_MAX_MAPLOAD; nCntMap++)
	{// �}�b�v��ǂݍ��߂�񐔕��J��Ԃ�
		strcpy(m_aMapFileName[nCntMap], "\0");
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;  // �Q�[���N���X�^�̃|�C���^
	if (pGame == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pGame = new CGame;
		if (pGame != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pGame->Init();
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
	return pGame;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGame::Init(void)
{
	// �e���f���ǂݍ���
	CShadow::Load();

	// �Q�[���̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(GAME_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTex = 0;                            // �e�N�X�`����ǂݍ��ސ�
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉�
			int nCntMap = 0;                            // �}�b�v��ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����
			char aPlayerFileName[256] = "\0";           // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
			char aEffectFileName[256] = "\0";           // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
			int nTimer = 10 * 60;                       // �^�C�}�[�̕b��
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
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MAP) == 0)
							{// �}�b�v�̌���񂪂�����
								m_nNumMap = CFunctionLib::ReadInt(pStrCur, NUM_MAP);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
								// �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(m_aMapFileName[nCntMap], pStr);
								nCntMap++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, GAME_TIMER) == 0)
							{// �^�C�}�[�̕b��������
								nTimer = CFunctionLib::ReadInt(pStrCur, GAME_TIMER);
								nTimer *= 60;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}


					// �K�v�ȃN���X���쐬����
					// UI
					if (GAME_SET_UI)
					{
						m_pUi = CUI::Create(nTimer, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial, m_pTextureManager);
					}

					// �G�t�F�N�g�Ǌ��N���X
					m_pEffectManager = CEffectManager::Create(aEffectFileName);

					// �}�b�v
					m_pMap = CMap::Create(m_aMapFileName[0]);

					// �v���C���[
					CPlayerManager::Create(aPlayerFileName, nPlayerLife, fPlayerOrbitSlash, fPlayerMaxSpecial);

					if (m_nClearStageNum == 0)
					{// �܂�1�}�b�v�ڂ�����
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WATER_FALL);
						CManager::GetSound()->SetVoluem(CSound::SOUND_LABEL_SE_WATER_FALL, 0.0f);
					}

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

					if(m_nMapIdx == 0)
					{// 1�}�b�v�ڂ�����
						CRiver *pRiver = NULL;
						pRiver = CRiver::Create(D3DXVECTOR3(426.5f, -59.8f, 4243.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 420.5f, 1196.0f, 2, 1, 1, 1, 0.001f, 0.0f, NULL, RIVER_PRIORITY);
						if (pRiver != NULL)
						{// ��𐶐��ł���
							pRiver->BindTexture(m_pMap->GetTextureManager()->GetTexture(5));
						    // ���_���̐ݒ�
							VERTEX_3D *pVtx;
							LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pRiver->GetVtxBuff();
							if (pVtxBuff != NULL)
							{// ���_�o�b�t�@����������Ă���
							    // ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
								pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

								// �����ɕK�v�ȕϐ���錾
								D3DXCOLOR col = pRiver->GetCol();
								int nXBlock = pRiver->GetXBlock();
								int nZBlock = pRiver->GetZBlock();
								float fWidth = pRiver->GetWidth();
								float fHeight = pRiver->GetHeight();
								int fTexSplitU = pRiver->GetTexSplitU();
								int fTexSplitV = pRiver->GetTexSplitV();
 								float XPos = -(fWidth * nXBlock) / 2;  // X���W�����ɐݒ�
								float ZPos = (fHeight * nZBlock) / 2;  // Z���W�����ɐݒ�
								float fTexU = 0.0f;                    // �e�N�X�`����U���W���E��ɐݒ�
								float fTexV = 0.0f;                    // �e�N�X�`����V���W���E��ɐݒ�
								float YPos = 0.0f;                     // Y���W

								for (int nCntV = 0; nCntV < nZBlock + 1; nCntV++)
								{// ���������̕����� + 1�����J��Ԃ�
									for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
									{// ���������̕����� + 1�����J��Ԃ�
									    // ���_���W
										pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

										// ���_�J���[
										pVtx[nCntH].col = col;

										// �e�N�X�`�����W
										pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

										XPos += fWidth;                            // X���W�����ɐi�߂�
										fTexU += (1.0f / (nXBlock)) * fTexSplitU;  // �e�N�X�`��U���W��i�߂�
									}
									XPos -= fWidth * (nXBlock + 1);                               // X���W��i�߂Ă������߂�
									ZPos -= fHeight;                                              // Z���W����O�ɐi�߂�
									fTexU -= ((1.0f / (nXBlock)) * fTexSplitU) * (nXBlock + 1);   // �e�N�X�`��U���W��i�߂Ă������߂�
									fTexV += ((1.0f / nZBlock) * fTexSplitV);                     // �e�N�X�`��V���W��i�߂�

									// �|�C���^��i�߂�
									pVtx += nXBlock + 1;
								}

								// ���_�o�b�t�@���A�����b�N����
								pVtxBuff->Unlock();
							}
						}

						//// ���̔z�u(�����_��)
						//D3DXVECTOR3 GrassPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						//D3DXVECTOR3 GrassRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						//bool bSway = false;
						//int nSway = 0;
						//float fGrassWidth = 0.0f;
						//float fGrassHeight = 0.0f;
						//for (int nCntGrass = 0; nCntGrass < 500; nCntGrass++)
						//{
						//	nSway = rand() % 2;
						//	if (nSway == 0)
						//	{
						//		bSway = false;
						//	}
						//	else if (nSway == 1)
						//	{
						//		bSway = true;
						//	}
						//	GrassPos.x = (rand() % 530) + 1200.0f + (rand() % 150 - 75);
						//	GrassPos.z = (rand() % 500) - 200.0f + (rand() % 150 - 75);
						//	GrassRot.y = ((rand() % 628) * 0.01f) - D3DX_PI;
						//	fGrassWidth = rand() % 15 + 15.0f;
						//	fGrassHeight = rand() % 10 + 20.0f;
						//	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
						//	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
						//	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
						//	bool bFieldLand = false;             // �n�ʂɏ���Ă��邩�ǂ���
						//	float fFieldHeight = 0.0f;           // �n�ʂ̍���

						//	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < FIELD_PRIORITY + 1; nCntPriority++)
						//	{// �`��D�揇�ʂ̐������J��Ԃ�
						//		pScene = CScene::GetTop(nCntPriority);
						//		while (pScene != NULL)
						//		{// ����������ɂȂ�܂ŌJ��Ԃ�
						//			pSceneNext = pScene->GetNext();
						//			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
						//			{// �n�ʃN���X�ւ̃|�C���^��������
						//				if (bFieldLand != true)
						//				{// �܂��n�ʂɏ���Ă��Ȃ�
						//					pField = (CMeshField*)pScene;
						//					if (pField != NULL)
						//					{// �n�ʂ��擾�ł���
						//						fFieldHeight = pField->GetPolyHeight(GrassPos, &bFieldLand, NULL);
						//						if (bFieldLand == true)
						//						{// �n�ʂ̒��ɂ���
						//							GrassPos.y = fFieldHeight;
						//						}
						//					}
						//				}
						//			}
						//			pScene = pSceneNext;
						//		}
						//	}
						//	// ����z�u����
						//	CGrass *pGrass = CGrass::Create(D3DXVECTOR3(GrassPos.x, GrassPos.y + 11.0f, GrassPos.z), GrassRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fGrassWidth, fGrassHeight, bSway, 9);
						//	if (pGrass != NULL)
						//	{
						//		pGrass->BindTexture(m_pMap->GetTextureManager()->GetTexture(9));
						//	}
						//}
					}

					// �v���C���[�̏ꏊ��\������|���S���𐶐�����
					m_pPlayerPosition = CScene2D::Create(GAME_PLAYER_POSITION_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GAME_PLAYER_POSITION_WIDTH, GAME_PLAYER_POSITION_HEIGHT, 0.0f, GAME_POLYGON_PRIORITY);
					if (m_pPlayerPosition != NULL)
					{
						m_pPlayerPosition->BindTexture(m_pTextureManager->GetTexture(6));
					}

					// �ړI�n��\������|���S���𐶐�����
					m_pDestPosition = CScene2D::Create(GAME_DEST_POSITION_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GAME_DEST_POSITION_WIDTH, GAME_DEST_POSITION_HEIGHT, 0.0f, GAME_POLYGON_PRIORITY);
					if (m_pDestPosition != NULL)
					{
						m_pDestPosition->BindTexture(m_pTextureManager->GetTexture(20));
					}

					// ���̔z�u
					LoadGrassTransform(pStrCur, pLine);

					// ���̓����蔻���ݒ肷��
					SetBridgeCollider();

					// �~�j�}�b�v��`�悷�邩�ǂ����ݒ肷��
					m_bMiniMapDisp = GAME_SET_MINIMAP;

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
void CGame::Uninit(void)
{
	// ���̔z�u����ۑ�����
	//SaveGrassTransform();

	// ��̉����~�߂�
	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WATER_FALL);

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

	// �Q�[���I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGame::Update(void)
{
	// �~�j�}�b�v�J�������X�V����
	CMapCamera *pMapCamera = CManager::GetMapCamera();
	if (pMapCamera != NULL)
	{
		pMapCamera->Update();
	}

	// UI���X�V����
	if (m_pUi != NULL)
	{
		m_pUi->Update();
	}

	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_NORMAL:        // �ʏ���
		if (CManager::GetKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_nMapIdx = 0;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_8) == true)
		{
			m_nMapIdx = 1;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_9) == true)
		{
			m_nMapIdx = 2;
			SetMap();
		}

#ifdef _DEBUG

		if (CManager::GetKeyboard()->GetTrigger(DIK_F9) == true)
		{
			m_State = STATE_END;
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_6) == true)
		{
			if (m_pUi != NULL)
			{
				m_pUi->SetDisp(m_pUi->GetDisp() ? false : true);
			}
			m_bMiniMapDisp = m_bMiniMapDisp ? false : true;
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_nMapIdx = 0;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_8) == true)
		{
			m_nMapIdx = 1;
			SetMap();
		}
		else if (CManager::GetKeyboard()->GetTrigger(DIK_9) == true)
		{
			m_nMapIdx = 2;
			SetMap();
		}
#endif

		break;
	case STATE_MAPCHANGE:       // �}�b�v�؂�ւ����
		CDebugProc::Print("�}�b�v�؂�ւ���\n");
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CFade::TYPE_SWITCHMAP);
		}
		break;
	case STATE_MAPCLEAR:        // �}�b�v�N���A���
		CDebugProc::Print("�}�b�v�N���A�[\n");
		ClearMapEvent();
		break;
	case STATE_BOSSAPPEAR:      // �{�X���o���������
		CDebugProc::Print("�{�X�o����\n");
		BossAppearEvent();
		break;
	case STATE_KNOCKDOWNBOSS:   // �{�X�����񂾏�ԏ��
		CDebugProc::Print("�{�X�����ɂ܂���n");
		KonckDownBossEvent();
		break;
	case STATE_END:             // �I�����
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_MODECHANGE_TIMING)
		{// �J�E���^�[������̒l�ɒB����
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);

				// ���U���g�ɕK�v�Ȑ��l��n���Ă���
				if (m_pUi != NULL)
				{
					CResult::SetHiddenItemBonus(m_nGetHiddenItem * GAME_HIDDENITEM_BONUS);
					CResult::SetComboMaxBonus(m_nComboMax * GAME_COMBOMAX_BONUS);
					CResult::SetStageClearBonus(m_nClearStageNum * GAME_STAGECLEAE_BONUS);
					CResult::SetGameScore(m_pUi->GetScore()->GetScore());
				}
			}
		}
		break;
	}

	CDebugProc::Print("�Q�[�����\n");
	CDebugProc::Print("\n");

	if (m_nClearStageNum == 0)
	{// �܂�1�}�b�v�ڂ�����
		if(m_pPlayer != NULL)
		{// �v���C���[���擾�ł��Ă���
			D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
			D3DXVECTOR3 WaterPos = GAME_WATERFALL_SOUND_POS;
			float fLength = sqrtf((PlayerPos.x - WaterPos.x) * (PlayerPos.x - WaterPos.x) + (PlayerPos.y - WaterPos.y) * (PlayerPos.y - WaterPos.y) + (PlayerPos.z - WaterPos.z) * (PlayerPos.z - WaterPos.z));
			if (fLength < GAME_WATERFALL_RANGE)
			{// ��̉��͈͓̔��ł���
				float fRivision = fLength / GAME_WATERFALL_RANGE;
				float fValue = cosf((D3DX_PI * 0.5f) * fRivision);
				CManager::GetSound()->SetVoluem(CSound::SOUND_LABEL_SE_WATER_FALL, fValue);
			}
			else
			{// ��̉��͈̔͊O�ł���
				CManager::GetSound()->SetVoluem(CSound::SOUND_LABEL_SE_WATER_FALL, 0.0f);
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    �|�[�Y�N���X�Ƀe�N�X�`����ݒ肷�鏈��
//=============================================================================
void CGame::BindPauseTexture(CPause *pPause)
{
	if (pPause != NULL)
	{// ���������m�ۂ���Ă���
		// �|�[�Y�w�i�p�̃e�N�X�`����ݒ肷��
		pPause->GetPauseBg()->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));

		// �I�����ڗp�̃e�N�X�`����ݒ肷��
		int nCntScene2DFlash = 0;
		for (int nCntTex = TEX_NUMBER_1; nCntTex < TEX_NUMBER_1 + 3; nCntTex++, nCntScene2DFlash++)
		{// �e�N�X�`����ǂݍ��މ񐔌J��Ԃ�
			pPause->GetScene2DFlash(nCntScene2DFlash)->BindTexture(m_pTextureManager->GetTexture(nCntTex));
		}

		// ������@�p�̃e�N�X�`����ݒ肷��
		int nCntOpeInfo = 0;
		for (int nCntTex = 16; nCntTex < 18; nCntTex++, nCntOpeInfo++)
		{// �e�N�X�`����ǂݍ��ސ������J��Ԃ�
			pPause->GetOpeInfo(nCntOpeInfo)->BindTexture(m_pTextureManager->GetTexture(nCntTex));
		}
	}
}

//=============================================================================
//    �}�b�v��؂�ւ��鏈��
//=============================================================================
void CGame::SetMap(void)
{
	m_State = STATE_NORMAL;  // ��Ԃ�߂��Ă���
	m_nStateCounter = 0;     // �J�E���^�[��߂�
	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WATER_FALL);

	if (m_nMapIdx == 0)
	{// 1�}�b�v�ڂɐ؂�ւ���������
		if (m_pUi != NULL)
		{
			if (m_pUi->GetNumStage() != NULL)
			{
				m_pUi->GetNumStage()->BindTexture(m_pTextureManager->GetTexture(10));
			}
		}
	}
	else if (m_nMapIdx == 1)
	{// 2�}�b�v�ڂɐ؂�ւ���������
		if (m_pUi != NULL)
		{
			if (m_pUi->GetNumStage() != NULL)
			{
				m_pUi->GetNumStage()->BindTexture(m_pTextureManager->GetTexture(11));
			}
		}
	}
	else if (m_nMapIdx == 2)
	{// 3�}�b�v�ڂɐ؂�ւ���������
		if (m_pUi != NULL)
		{
			if (m_pUi->GetPurpose() != NULL)
			{
				m_pUi->GetPurpose()->BindTexture(m_pTextureManager->GetTexture(14));
			}
			if (m_pUi->GetNumStage() != NULL)
			{
				m_pUi->GetNumStage()->BindTexture(m_pTextureManager->GetTexture(12));
			}
		}
		// BGM���~
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM002);

		// �{�X�o����Ԃɂ���
		m_State = STATE_BOSSAPPEAR;

		// UI�̕\��������
		if (m_pUi != NULL)
		{
			m_pUi->SetDisp(false);
		}
		m_bMiniMapDisp = false;
		if (m_pPlayerPosition != NULL)
		{// ���������m�ۂ���Ă���
			m_pPlayerPosition->Uninit();
			m_pPlayerPosition = NULL;
		}
		if (m_pDestPosition != NULL)
		{// ���������m�ۂ���Ă���
			m_pDestPosition->Uninit();
			m_pDestPosition = NULL;
		}
	}

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}

	// �}�b�v�̐���
	if (m_pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMap = CMap::Create(m_aMapFileName[m_nMapIdx]);
		if (m_pMap != NULL && m_pPlayer != NULL)
		{
			m_pPlayer->SetPos(m_pMap->GetPlayerDefaultPos());
		}
	}

	if (m_nMapIdx == 0)
	{// 1�}�b�v�ڂ�������
		// ���̓����蔻���ݒ肷��
		SetBridgeCollider();
	}

	// �ړI��UI���ĕ\��
	if (m_pUi != NULL)
	{
		m_pUi->ResetPurpose();
	}

	// �J�����̈ʒu���Đݒ�
	CManager::GetCamera()->SetDefaultPos(m_pMap->GetPlayerDefaultPos());

	m_bMapClear = false;    // �}�b�v�𖢃N���A��Ԃɂ���
}

//=============================================================================
//    �}�b�v���N���A�����Ƃ��̏���
//=============================================================================
void CGame::ClearMapEvent(void)
{
	m_nStateCounter++;
	if (m_nStateCounter >= GAME_MAPCLEAR_CHANGETIMING == 0)
	{// �}�b�v�؂�ւ��̃^�C�~���O�ɂȂ���
		SetState(CGame::STATE_MAPCHANGE);
		m_nClearStageNum++;
		m_nMapIdx++;
	}
}

//=============================================================================
//    �~�j�}�b�v��`�悷�鏈��
//=============================================================================
void CGame::MiniMapDraw(void)
{
	if (m_bMiniMapDisp && m_bMiniMapDisp == GAME_SET_MINIMAP)
	{// �~�j�}�b�v��`�悷��Ȃ��
		CRenderer *pRenderer = CManager::GetRenderer();
		if (pRenderer != NULL)
		{// �����_�����O�N���X���擾�ł���
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// �f�o�C�X���擾�ł���
				// �~�j�}�b�v�ɂ��`�悷��
				D3DVIEWPORT9 ViewportDef;                           // �`��̈�ۑ��p
				CMapCamera *pMapCamera = CManager::GetMapCamera();  // �~�j�}�b�v�p�̃J����

				// �`��̈��ۑ����Ă���
				pDevice->GetViewport(&ViewportDef);

				// �}�b�v�J������ݒ肷��
				pMapCamera->SetCamera();

				// �f�o�C�X���N���A����
				pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

				// �}�b�v���\������v�f��`�悷��
				D3DXVECTOR3 GoalPos;
				CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
				CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
				CObject *pObjcet = NULL;
				CObjectGoal *pObjGoal = NULL;
				for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
				{// �`��D�揇�ʂ̐������J��Ԃ�
					pScene = CScene::GetTop(nCntPriority);
					while (pScene != NULL)
					{// ����������ɂȂ�܂ŌJ��Ԃ�
						pSceneNext = pScene->GetNext();
						if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT || pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE || pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL
							|| pScene->GetObjType() == CScene::OBJTYPE_BILLBOARDOBJ || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER
							|| pScene->GetObjType() == CScene::OBJTYPE_ENEMYMANAGER || pScene->GetObjType() == CScene::OBJTYPE_ENEMY || pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD
							|| pScene->GetObjType() == CScene::OBJTYPE_MOUNTAIN || pScene->GetObjType() == CScene::OBJTYPE_SKY || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE
							|| pScene->GetObjType() == CScene::OBJTYPE_RINGEFFECT || pScene->GetObjType() == CScene::OBJTYPE_MESHWALL || pScene->GetObjType() == CScene::OBJTYPE_RIVER)
						{// �}�b�v���\������I�u�W�F�N�g��������
							if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
							{
								pObjcet = (CObject*)pScene;
								pObjcet->SetThin(false);
								pObjcet->SetAlpha(1.0f);
							}
							else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
							{
								pObjGoal = (CObjectGoal*)pScene;
								GoalPos = pObjGoal->GetPos();
							}

							pScene->Draw();
						}
						pScene = pSceneNext;
					}
				}

				if (m_pPlayerPosition != NULL)
				{// ���������m�ۂ���Ă���
					if (m_pPlayer != NULL)
					{// �v���C���[����������Ă���
						m_pPlayerPosition->SetRot(m_pPlayer->GetRot().y);
						m_pPlayerPosition->Draw();
					}
				}

				if (m_pDestPosition != NULL)
				{// ���������m�ۂ���Ă���
					if (m_pPlayer != NULL)
					{// �v���C���[����������Ă���
						float fRot = atan2f(m_pPlayer->GetPos().x - GoalPos.x, m_pPlayer->GetPos().z- GoalPos.z);
						m_pDestPosition->SetRot(fRot);
						m_pDestPosition->Draw();
					}
				}

				if (CManager::GetFade()->GetState() != CFade::STATE_NONE)
				{// �t�F�[�h���g�p����Ă���
					if (CManager::GetFade()->GetScene2D() != NULL)
					{// �t�F�[�h�Ɏg�p����|���S������������Ă���
						CManager::GetFade()->GetScene2D()->Draw();
					}
				}

				// �`��̈�����ɖ߂�
				pDevice->SetViewport(&ViewportDef);
			}
		}
	}
}

//=============================================================================
//    �{�X���o�����̃C�x���g����
//=============================================================================
void CGame::BossAppearEvent(void)
{
	// �J�����̐ݒ�
	CManager::GetCamera()->SetPosV(GAME_BOSSAPPAER_CAMERA_POSV);
	CManager::GetCamera()->SetPosR(GAME_BOSSAPPAER_CAMERA_POSR);

	m_nBossAppearCounter++;  // �J�E���^�[�𑝂₷

	if (m_nBossAppearCounter >= GAME_BOSSAPPEAR_STATE_TIMING)
	{// ��Ԃ�ς���^�C�~���O�Ȃ���
		for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
		{// ���S���o���������J��Ԃ�
			if (m_pBossAppearLogo[nCntAppear] != NULL)
			{
				m_pBossAppearLogo[nCntAppear]->Uninit();
				m_pBossAppearLogo[nCntAppear] = NULL;
			}
		}

		// BGM���Đ�
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM003);

		// ��Ԃ�߂�
		m_State = STATE_NORMAL;

		// UI�̕\����߂�
		if (m_pUi != NULL)
		{
			m_pUi->SetDisp(true);
		}
		m_bMiniMapDisp = true;

		// �v���C���[�̏ꏊ��\������|���S�����Đ�������
		m_pPlayerPosition = CScene2D::Create(GAME_PLAYER_POSITION_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GAME_PLAYER_POSITION_WIDTH, GAME_PLAYER_POSITION_HEIGHT, 0.0f, GAME_POLYGON_PRIORITY);
		if (m_pPlayerPosition != NULL)
		{
			m_pPlayerPosition->BindTexture(m_pTextureManager->GetTexture(6));
		}
	}
	if (m_nBossAppearCounter == GAME_BOSSAPPEAR_LOGO_TIMING_0)
	{// 1�ڂ̃��S���o���^�C�~���O�Ȃ���
		if (m_pBossAppearLogo[0] == NULL)
		{
			m_pBossAppearLogo[0] = CScene2D::Create(GAME_BOSSAPPEAR_POS_0, GAME_BOSSAPPEAR_COL_0, GAME_BOSSAPPEAR_WIDTH_0, GAME_BOSSAPPEAR_HEIGHT_0, 0.0f, GAME_POLYGON_PRIORITY);
			if (m_pBossAppearLogo[0] != NULL)
			{
				m_pBossAppearLogo[0]->BindTexture(m_pTextureManager->GetTexture(18));
			}

			// SE���Đ�
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSSAPPEAR);
		}
	}
	if (m_nBossAppearCounter == GAME_BOSSAPPEAR_LOGO_TIMING_1)
	{// 2�ڂ̃��S���o���^�C�~���O�Ȃ���
		if (m_pBossAppearLogo[1] == NULL)
		{
			m_pBossAppearLogo[1] = CScene2D::Create(GAME_BOSSAPPEAR_POS_1, GAME_BOSSAPPEAR_COL_1, GAME_BOSSAPPEAR_WIDTH_1, GAME_BOSSAPPEAR_HEIGHT_1, 0.0f, GAME_POLYGON_PRIORITY);
			if (m_pBossAppearLogo[1] != NULL)
			{
				m_pBossAppearLogo[1]->BindTexture(m_pTextureManager->GetTexture(19));
			}

			// SE���Đ�
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSSAPPEAR);
		}
	}
	if (m_nBossAppearCounter == GAME_BOSSAPPEAR_EFFECT_TIMING)
	{// �G�t�F�N�g���o���^�C�~���O�Ȃ���
		// �G�t�F�N�g���o��
		m_pEffectManager->SetEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20);

		// �t�B�[�h�o�b�N�G�t�F�N�g���o��
		CManager::GetRenderer()->SetFeedBack(60, 0.994f);

		// SE���Đ�
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BOSSAPPEAR_EXPLOSION);
	}
}

//=============================================================================
//    �{�X�����񂾂Ƃ��̃C�x���g����
//=============================================================================
void CGame::KonckDownBossEvent(void)
{
	// �J�����̐ݒ�
	D3DXVECTOR3 PosV;
	PosV.x = m_BossPos.x + sinf(0.0f) * GAME_KNOCKDOWN_CAMERA_LENGTH;
	PosV.y = GAME_KNOCKDOWN_CAMERA_POSY;
	PosV.z = m_BossPos.z + cosf(0.0f) * GAME_KNOCKDOWN_CAMERA_LENGTH;
	CManager::GetCamera()->SetPosV(PosV);
	CManager::GetCamera()->SetPosR(m_BossPos);

	m_nKnockDownCounter++;   // �J�E���^�[��i�߂�
	if (m_nKnockDownCounter >= GAME_KNOCKDOWN_CAMERA_TIMING)
	{// �J������ʏ�̃J�����ɖ߂��^�C�~���O�ɂȂ���
		// ��Ԃ�ύX
		m_State = STATE_END;
	}
}

//=============================================================================
//    �v���C���[�N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CGame::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CGame::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    �}�b�v���N���A�������ǂ�����ݒ肷��
//=============================================================================
void CGame::SetMapClear(bool bMapClear)
{
	m_bMapClear = bMapClear;
}

//=============================================================================
//    �擾�����B���A�C�e���̐���ݒ肷�鏈��
//=============================================================================
void CGame::SetHiddenItem(const int nGetHiddenItem)
{
	m_nGetHiddenItem = nGetHiddenItem;
}

//=============================================================================
//    �R���{���̍ő�l��ݒ肷�鏈��
//=============================================================================
void CGame::SetComboMax(const int nComboMax)
{
	m_nComboMax = nComboMax;
}

//=============================================================================
//    �{�X�̍��W��ݒ肷�鏈��
//=============================================================================
void CGame::SetBossPos(const D3DXVECTOR3 BossPos)
{
	m_BossPos = BossPos;
}

//=============================================================================
//    UI�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CUI *CGame::GetUI(void)
{
	return m_pUi;
}

//=============================================================================
//    �v���C���[�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEffectManager *CGame::GetEffectManager(void)
{
	return m_pEffectManager;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �}�b�v���N���A�������ǂ������擾����
//=============================================================================
bool CGame::GetMapClear(void)
{
	return m_bMapClear;
}

//=============================================================================
//    ���݂̃}�b�v�ԍ����擾���鏈��
//=============================================================================
int CGame::GetMapIdx(void)
{
	return m_nMapIdx;
}

//=============================================================================
//    �}�b�v�̓ǂݍ��ݐ����擾���鏈��
//=============================================================================
int CGame::GetNumMap(void)
{
	return m_nNumMap;
}

//=============================================================================
//    �擾�����B���A�C�e���̐����擾���鏈��
//=============================================================================
int CGame::GetHiddenItem(void)
{
	return m_nGetHiddenItem;
}

//=============================================================================
//    �R���{���̍ő�l���擾���鏈��
//=============================================================================
int CGame::GetComboMax(void)
{
	return m_nComboMax;
}

//=============================================================================
//    ���̓����蔻���ݒ肷�鏈��
//=============================================================================
void CGame::SetBridgeCollider(void)
{
	FILE *pFile = NULL;      // �t�@�C���|�C���^
	char *pLine = NULL;      // 1�s���ǂݎ��p
	char *pStrCur = NULL;    // ���݂̐擪�|�C���^
	D3DXVECTOR3 ColliderPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fColliderWidth = 0.0f;
	float fColliderHeight = 0.0f;
	float fColliderDepth = 0.0f;
	bool bColliderReturnFlag = false;

	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen("data/TEXT/MAP/COLLIDER/boxcollider000.txt", "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			if (pLine == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				if (pLine != NULL)
				{// ���������m�ۂł���
				    // �������������
					strcpy(pLine, "\0");
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, BOXCOLLIDERSET) == 0)
							{// �����蔻��p�����f���̔z�u���ǂݍ��݊J�n�̍��}������
								while (1)
								{// ���[�v�J�n
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
									if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
									{// ���W��񂪂���
										ColliderPos = CFunctionLib::ReadVector3(pStrCur, POS);
									}
									else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
									{// ����񂪂���
										fColliderWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
									}
									else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
									{// ������񂪂���
										fColliderHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
									}
									else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
									{// ���s��񂪂���
										fColliderDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
									}
									else if (CFunctionLib::Memcmp(pStrCur, RETURN) == 0)
									{// ���������ۂɉ����߂����ǂ�����񂪂���
										bColliderReturnFlag = CFunctionLib::ReadBool(pStrCur, RETURN);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_BOXCOLLIDERSET) == 0)
									{// �����蔻��p�����f���̔z�u���ǂݍ��ݏI���̍��}������
										CBoxCollider::Create(ColliderPos, fColliderWidth, fColliderHeight, fColliderDepth, bColliderReturnFlag, 5);
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					if (pLine != NULL)
					{// ���������m�ۂ���Ă���
						delete[] pLine;
						pLine = NULL;
					}
				}
			}
		}
	}
}

//=============================================================================
//    ���̈ʒu�������ۑ����鏈��
//=============================================================================
void CGame::SaveGrassTransform(void)
{
	FILE *pFile = NULL;
	pFile = fopen("data/TEXT/save_grass.txt", "w");
	if (pFile != NULL)
	{
		// �R�����g��������������
		fprintf(pFile, "#******************************************************************************\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#       ���̔z�u���X�N���v�g�t�@�C��[save_grass.txt]\n");
		fprintf(pFile, "#       Auther : Hodaka Niwa\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#******************************************************************************\n");
		fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#  ���̔z�u���\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		CGrass *pGrass = NULL;
		pScene = CScene::GetTop(9);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_GRASS)
			{// ���N���X��������
				pGrass = (CGrass*)pScene;
				if (pGrass != NULL)
				{// ���N���X���擾�ł���
					D3DXVECTOR3 GrassRot = pGrass->GetRot();
					// ����
					GrassRot.x = D3DXToDegree(GrassRot.x);
					GrassRot.y = D3DXToDegree(GrassRot.y);
					GrassRot.z = D3DXToDegree(GrassRot.z);

					// �z�u���̏�����������
					fprintf(pFile, "%s\n", GRASSSET);
					fprintf(pFile, "	%s%d						# �����h��邩�ǂ���\n", SWAY, pGrass->GetSway());
					fprintf(pFile, "	%s%.1f %.1f %.1f			# ���̍��W\n", POS, pGrass->GetPos().x, pGrass->GetPos().y, pGrass->GetPos().z);
					fprintf(pFile, "	%s%.1f %.1f %.1f				# ���̌���\n", ROT, GrassRot.x, GrassRot.y, GrassRot.z);
					fprintf(pFile, "	%s%.1f					# ���̕�\n", WIDTH, pGrass->GetWidth());
					fprintf(pFile, "	%s%.1f					# ���̍���\n", HEIGHT, pGrass->GetHeight());
					fprintf(pFile, "%s\n", END_GRASSSET);
					fprintf(pFile, "\n");
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}

		// �R�����g��������������
		fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);
	}
}

//=============================================================================
//    ���̈ʒu�������ǂݍ��ޏ���
//=============================================================================
void CGame::LoadGrassTransform(char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen("data/TEXT/MAP/GRASS/grass.txt", "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, GRASSSET) == 0)
					{// ���̔z�u���ǂݍ��݊J�n�̍��}������
						D3DXVECTOR3 GrassPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXVECTOR3 GrassRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						bool bGrassSway = false;
						float fGrassWidth = 0.0f;
						float fGrassHeight = 0.0f;
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, SWAY) == 0)
							{// ���W��񂪂���
								bGrassSway = CFunctionLib::ReadBool(pStrCur, SWAY);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂪂���
								GrassPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂪂���
								GrassRot = CFunctionLib::ReadVector3(pStrCur, ROT);
								GrassRot.x = D3DXToRadian(GrassRot.x);
								GrassRot.y = D3DXToRadian(GrassRot.y);
								GrassRot.z = D3DXToRadian(GrassRot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// ����񂪂���
								fGrassWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// ������񂪂���
								fGrassHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_GRASSSET) == 0)
							{// ���̔z�u���ǂݍ��ݏI���̍��}������
							 	// ����z�u����
							 	CGrass *pGrass = CGrass::Create(D3DXVECTOR3(GrassPos.x, GrassPos.y + 11.0f, GrassPos.z), GrassRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fGrassWidth, fGrassHeight, bGrassSway, 9);
							 	if (pGrass != NULL)
							 	{
							 		pGrass->BindTexture(m_pMap->GetTextureManager()->GetTexture(9));
							 	}
								break;  // ���[�v�I��
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��ݏI���̍��}������
						break;  // ���[�v�I��
					}
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}