//*****************************************************************************
//
//     UI�̏���[ui.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ui.h"
#include "timer.h"
#include "score.h"
#include "combo.h"
#include "gauge.h"
#include "textureManager.h"
#include "game.h"
#include "manager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �f�[�^���͗p�}�N��
#define UI_NUM                 (8)                                          // �\������UI�I�u�W�F�N�g�̐�
#define UI_ADD_ALPHA           (1.0f)                                       // UI�̓����x��������l
#define UI_CUT_ALPHA           (0.08f)                                      // UI�̓����x���グ��l

// �^�C�}�[�p�}�N��
#define UI_TIMER_POS           (D3DXVECTOR3(680.0f,32.0f,0.0f))             // �^�C�}�[�̍��W
#define UI_TIMER_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �^�C�}�[�̐F
#define UI_TIMER_WIDTH         (30.0f)                                      // �^�C�}�[�̐����|���S���P���̕�
#define UI_TIMER_HEIGHT        (30.0f)                                      // �^�C�}�[�̐����|���S���P���̍���
#define UI_TIMER_WARNING       (30 * 60)                                    // �^�C�}�[���Ԃ��Ȃ�^�C�~���O

// �X�R�A�p�}�N��
#define UI_SCORE_POS           (D3DXVECTOR3(300.0f,22.0f,0.0f))             // �X�R�A�̍��W
#define UI_SCORE_COL           (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))             // �X�R�A�̐F
#define UI_SCORE_WIDTH         (20.0f)                                      // �X�R�A�̐����|���S���P���̕�
#define UI_SCORE_HEIGHT        (20.0f)                                      // �X�R�A�̐����|���S���P���̍���

// �̗̓Q�[�W�p�}�N��
#define UI_LIFEGAUGE_POS       (D3DXVECTOR3(1020.0f,660.0f,0.0f))           // �̗̓Q�[�W�̍��W
#define UI_LIFEGAUGE_COL       (D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))             // �̗̓Q�[�W�̐F
#define UI_LIFEGAUGE_WIDTH     (200.0f)                                     // �̗̓Q�[�W�̕�
#define UI_LIFEGAUGE_HEIGHT    (21.0f)                                      // �̗̓Q�[�W�̍���

// �O�Վa�Q�[�W�p�}�N��
#define UI_ORBITGAUGE_POS      (D3DXVECTOR3(230.0f,675.0f,0.0f))            // �O�ՃQ�[�W�̍��W
#define UI_ORBITGAUGE_COL      (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))             // �O�ՃQ�[�W�̐F
#define UI_ORBITGAUGE_WIDTH    (200.0f)                                     // �O�ՃQ�[�W�̕�
#define UI_ORBITGAUGE_HEIGHT   (14.0f)                                      // �O�ՃQ�[�W�̍���

// �K�E�Q�[�W�p�}�N��
#define UI_SPECIALGAUGE_POS    (D3DXVECTOR3(165.0f,608.0f,0.0f))            // �K�E�Q�[�W�̍��W
#define UI_SPECIALGAUGE_COL    (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))             // �K�E�Q�[�W�̐F
#define UI_SPECIALGAUGE_WIDTH  (142.0f)                                     // �K�E�Q�[�W�̕�
#define UI_SPECIALGAUGE_HEIGHT (13.0f)                                      // �K�E�Q�[�W�̍���

// �R���{���p�}�N��
#define UI_COMBO_POS           (D3DXVECTOR3(1500.0f,350.0f,0.0f))           // �R���{UI�̏������W
#define UI_COMBO_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �R���{UI�̐F
#define UI_COMBO_WIDTH         (22.0f)                                      // �R���{UI�̐����|���S���P���̕�
#define UI_COMBO_HEIGHT        (22.0f)                                      // �R���{UI�̐����|���S���P���̍���

// ���݂̃X�e�[�W���p�}�N��
#define UI_NUMSTAGE_POS        (D3DXVECTOR3(SCREEN_WIDTH / 2,40.0f,0.0f))   // �X�e�[�W��UI�̏������W
#define UI_NUMSTAGE_COL        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �X�e�[�W��UI�̐F
#define UI_NUMSTAGE_WIDTH      (90.0f)                                      // �X�e�[�W��UI�̃|���S���̕�
#define UI_NUMSTAGE_HEIGHT     (30.0f)                                      // �X�e�[�W��UI�̃|���S���̍���

// �ړI�p�}�N��
#define UI_PURPOSE_POS         (D3DXVECTOR3(SCREEN_WIDTH / 2,100.0f,0.0f))  // �ړI��UI�̏������W
#define UI_PURPOSE_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // �ړI��UI�̐F
#define UI_PURPOSE_WIDTH       (160.0f)                                     // �ړI��UI�̃|���S���̕�
#define UI_PURPOSE_HEIGHT      (30.0f)                                      // �ړI��UI�̃|���S���̍���

// �ړI�̔w�i�p�}�N��
#define UI_PAPER_POS           (D3DXVECTOR3(SCREEN_WIDTH / 2,76.0f,0.0f))   // ���̏������W
#define UI_PAPER_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // ���̐F
#define UI_PAPER_WIDTH         (180.0f)                                     // ���̃|���S���̕�
#define UI_PAPER_HEIGHT        (70.0f)                                      // ���̃|���S���̍���

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CUI::CUI()
{
	// �e��l�̃N���A
	m_State = STATE_NORMAL;    // ���
	m_pTimer = NULL;           // �^�C�}�[�N���X�ւ̃|�C���^
	m_pScore = NULL;           // �X�R�A�N���X�ւ̃|�C���^
	m_pGaugeLife = NULL;       // �v���C���[�̗̑̓Q�[�W�N���X�ւ̃|�C���^
	m_pGaugeOrbitSlash = NULL; // �v���C���[�̋O�ՃQ�[�W�N���X�ւ̃|�C���^
	m_pGaugeSpecial = NULL;    // �v���C���[�̕K�E�Z�Q�[�W�N���X�ւ̃|�C���^
	m_pCombo = NULL;           // �R���{UI�N���X�ւ̃|�C���^
	m_pPaper = NULL;           // ���w�i�|���S���N���X�ւ̃|�C���^
	m_pNumStage = NULL;        // ���݂̃X�e�[�W���|���S���N���X�ւ̃|�C���^
	m_pPurpose = NULL;         // �ړI�̃|���S���N���X�ւ̃|�C���^
	m_pTextureManager = NULL;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_bDisp = true;            // UI��`�悷�邩�ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    ��������
//=============================================================================
CUI *CUI::Create(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager)
{
	CUI *pUi = NULL;  // UI�N���X�^�̃|�C���^
	if (pUi == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pUi = new CUI;
		if (pUi != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pUi->Init(nTimer, nPlayerLife, fPlayerOrbitSlash, fPlayerSpecial, pTextureManager);
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
	return pUi;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CUI::Init(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager)
{
	// �K�v��UI�𐶐�����
	// �X�R�A
	m_pScore = CScore::Create(UI_SCORE_POS, UI_SCORE_COL, UI_SCORE_WIDTH, UI_SCORE_HEIGHT, UI_SCORE_PRIORITY);
	if (m_pScore != NULL)
	{// �X�R�A�N���X�������ł���
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[������������
			m_pScore->SetScoreTexture(pTextureManager->GetTexture(CGame::TEX_NUMBER_4));
		}
		else if(CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// �`���[�g���A������������
			m_pScore->SetScoreTexture(pTextureManager->GetTexture(CGame::TEX_NUMBER_5));
		}
	}

	int nGaugeTexIdx = 0;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[������������
		nGaugeTexIdx = 7;
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// �`���[�g���A������������
		nGaugeTexIdx = 9;
	}

	// �v���C���[�̗̑̓Q�[�W
	m_pGaugeLife = CGaugeLife2D::Create(UI_LIFEGAUGE_POS, UI_LIFEGAUGE_COL, UI_LIFEGAUGE_WIDTH, UI_LIFEGAUGE_HEIGHT, 0.0f, nPlayerLife, pTextureManager->GetTexture(nGaugeTexIdx), UI_LIFEGAUGE_PRIORITY);
	nGaugeTexIdx++;

	// �v���C���[�̋O�ՃQ�[�W
	m_pGaugeOrbitSlash = CGaugeOrbitSlash2D::Create(UI_ORBITGAUGE_POS, UI_ORBITGAUGE_COL, UI_ORBITGAUGE_WIDTH, UI_ORBITGAUGE_HEIGHT, 0.0f, fPlayerOrbitSlash, pTextureManager->GetTexture(nGaugeTexIdx), UI_ORBITGAUGE_PRIORITY);
	nGaugeTexIdx++;

	// �v���C���[�̕K�E�Q�[�W
	m_pGaugeSpecial = CGaugeSpecial2D::Create(UI_SPECIALGAUGE_POS, UI_SPECIALGAUGE_COL, UI_SPECIALGAUGE_WIDTH, UI_SPECIALGAUGE_HEIGHT, 0.0f, fPlayerSpecial, 0.0f, pTextureManager->GetTexture(nGaugeTexIdx), UI_SPECIALGAUGE_PRIORITY);

	// ���|���S��
	m_pPaper = CScene2D::Create(UI_PAPER_POS, UI_PAPER_COL, UI_PAPER_WIDTH, UI_PAPER_HEIGHT, 0.0f, UI_PAPER_PRIORITY);
	if (m_pPaper != NULL)
	{// ���|���S���N���X�������ł���
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[������������
			m_pPaper->BindTexture(pTextureManager->GetTexture(15));
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// �`���[�g���A������������
			m_pPaper->BindTexture(pTextureManager->GetTexture(8));
		}
	}

	// ���݂̃X�e�[�W���|���S��
	m_pNumStage = CScene2D::Create(UI_NUMSTAGE_POS, UI_NUMSTAGE_COL, UI_NUMSTAGE_WIDTH, UI_NUMSTAGE_HEIGHT, 0.0f, UI_NUMSTAGE_PRIORITY);
	if (m_pNumStage != NULL)
	{// ���݂̃X�e�[�W���N���X�������ł���
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[������������
			m_pNumStage->BindTexture(pTextureManager->GetTexture(10));
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// �`���[�g���A������������
			m_pNumStage->BindTexture(pTextureManager->GetTexture(6));
		}
	}

	// �ړI�̃|���S��
	m_pPurpose = CScene2D::Create(UI_PURPOSE_POS, UI_PURPOSE_COL, UI_PURPOSE_WIDTH, UI_PURPOSE_HEIGHT, 0.0f, UI_PURPOSE_PRIORITY);
	if (m_pPurpose != NULL)
	{// �ړI�̃|���S���N���X�������ł���
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[������������
			m_pPurpose->BindTexture(pTextureManager->GetTexture(13));
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// �`���[�g���A������������
			m_pPurpose->BindTexture(pTextureManager->GetTexture(7));
		}
	}

	// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^��ۑ����Ă���
	m_pTextureManager = pTextureManager;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CUI::Uninit(void)
{
	// �^�C�}�[�N���X�ւ̃|�C���^����ɂ���
	m_pTimer = NULL;

	// �X�R�A�N���X�ւ̃|�C���^����ɂ���
	m_pScore = NULL;

	// �̗̓Q�[�W�N���X�ւ̃|�C���^����ɂ���
	m_pGaugeLife = NULL;

	// �O�ՃQ�[�W�N���X�ւ̃|�C���^����ɂ���
	m_pGaugeOrbitSlash = NULL;

	// �K�E�Q�[�W�N���X�ւ̃|�C���^����ɂ���
	m_pGaugeSpecial = NULL;

	// �R���{UI�N���X�ւ̃|�C���^����ɂ���
	m_pCombo = NULL;

	// ���݂̃X�e�[�W���|���S���N���X�ւ̃|�C���^����ɂ���
	m_pNumStage = NULL;

	// �ړI�̃|���S���N���X�ւ̃|�C���^����ɂ���
	m_pPurpose = NULL;

	// ���w�i�|���S���N���X�ւ̃|�C���^����ɂ���
	m_pPaper = NULL;

	// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^����ɂ���
	m_pTextureManager = NULL;
}

//=============================================================================
//    �X�V����
//=============================================================================
void CUI::Update(void)
{
	if (m_State == STATE_ALPHACHANGE)
	{// �����x��ύX�����ԂȂ��
	    // �����x��ύX����
		int nCntOk = 0;
		D3DXCOLOR col;
		float fValue;

		if (m_bDisp != true)
		{// �`�悵�Ȃ���ԂȂ��
			// �����x��0�ɂ��Ă���
			fValue = -UI_ADD_ALPHA;

			// �X�R�A
			if (m_pScore != NULL)
			{// ��������Ă���
				col = m_pScore->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pScore->SetCol(col);
			}

			// �̗̓Q�[�W
			if (m_pGaugeLife != NULL)
			{// ��������Ă���
				col = m_pGaugeLife->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pGaugeLife->ChangeAlpha((float)col.a);
			}

			// �O�ՃQ�[�W
			if (m_pGaugeOrbitSlash != NULL)
			{// ��������Ă���
				col = m_pGaugeOrbitSlash->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pGaugeOrbitSlash->ChangeAlpha((float)col.a);
			}

			// �K�E�Q�[�W
			if (m_pGaugeSpecial != NULL)
			{// ��������Ă���
				col = m_pGaugeSpecial->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pGaugeSpecial->ChangeAlpha((float)col.a);
			}

			// �R���{UI
			if (m_pCombo != NULL)
			{// ��������Ă���
				col = m_pCombo->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pCombo->SetCol(col);
			}

			// ���݂̃X�e�[�W��
			if (m_pNumStage != NULL)
			{// ��������Ă���
				col = m_pNumStage->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pNumStage->SetCol(col);
			}

			// �ړI
			if (m_pPurpose != NULL)
			{// ��������Ă���
				col = m_pPurpose->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pPurpose->SetCol(col);
			}

			// ���w�i
			if (m_pPaper != NULL)
			{// ��������Ă���
				col = m_pPaper->GetCol();
				col.a += fValue;
				if (col.a <= 0.0f)
				{// �����ɂȂ�؂��Ă���
					col.a = 0.0f;
					nCntOk++;
				}
				m_pPaper->SetCol(col);
			}
		}
		else if (m_bDisp == true)
		{// �`�悷���ԂȂ��
			// �����x��1�ɂ��Ă���
			fValue = UI_CUT_ALPHA;

			// �X�R�A
			if (m_pScore != NULL)
			{// ��������Ă���
				col = m_pScore->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pScore->SetCol(col);
			}

			// �̗̓Q�[�W
			if (m_pGaugeLife != NULL)
			{// ��������Ă���
				col = m_pGaugeLife->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pGaugeLife->ChangeAlpha((float)col.a);
			}

			// �O�ՃQ�[�W
			if (m_pGaugeOrbitSlash != NULL)
			{// ��������Ă���
				col = m_pGaugeOrbitSlash->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pGaugeOrbitSlash->ChangeAlpha((float)col.a);
			}

			// �K�E�Q�[�W
			if (m_pGaugeSpecial != NULL)
			{// ��������Ă���
				col = m_pGaugeSpecial->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pGaugeSpecial->ChangeAlpha((float)col.a);
			}

			// �R���{UI
			if (m_pCombo != NULL)
			{// ��������Ă���
				col = m_pCombo->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pCombo->SetCol(col);
			}

			// ���݂̃X�e�[�W��
			if (m_pNumStage != NULL)
			{// ��������Ă���
				col = m_pNumStage->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pNumStage->SetCol(col);
			}

			// �ړI
			if (m_pPurpose != NULL)
			{// ��������Ă���
				col = m_pPurpose->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pPurpose->SetCol(col);
			}

			// ���w�i
			if (m_pPaper != NULL)
			{// ��������Ă���
				col = m_pPaper->GetCol();
				col.a += fValue;
				if (col.a >= 1.0f)
				{// �����x������ȏ㉺����Ȃ�
					col.a = 1.0f;
					nCntOk++;
				}
				m_pPaper->SetCol(col);
			}
		}

		if (nCntOk == UI_NUM)
		{// �S�Ă̓����x���ύX����Ă���
			m_State = STATE_NORMAL;
		}
	}
}

//=============================================================================
//    UI��`�悷�邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CUI::SetDisp(const bool bDisp)
{
	m_bDisp = bDisp;
	m_State = STATE_ALPHACHANGE;
}

//=============================================================================
//    UI��`�悷�邩�ǂ������擾���鏈��
//=============================================================================
bool CUI::GetDisp(void)
{
	return m_bDisp;
}

//=============================================================================
//    �R���{UI�N���X�𐶐����鏈��
//=============================================================================
void CUI::CreateCombo(void)
{
	if (m_pCombo == NULL)
	{// ���������m�ۂ���Ă��Ȃ�
		m_pCombo = CCombo::Create(UI_COMBO_POS, UI_COMBO_COL, UI_COMBO_WIDTH, UI_COMBO_HEIGHT, UI_COMBO_PRIORITY);
		if(m_pCombo != NULL)
		{// ���������m�ۂł���
			m_pCombo->SetNumberTexture(m_pTextureManager->GetTexture(CGame::TEX_NUMBER_4));
			m_pCombo->SetLogoTexture(m_pTextureManager->GetTexture(CGame::TEX_NUMBER_5), 0);
		}
	}
}

//=============================================================================
//    �R���{UI�N���X�ւ̃|�C���^����ɂ��鏈��
//=============================================================================
void CUI::ReleaseCombo(void)
{
	if (m_pCombo != NULL)
	{// ���������m�ۂ���Ă���
		m_pCombo = NULL;
	}
}

//=============================================================================
//    �X�R�A�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CScore *CUI::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
//    �^�C�}�[�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CTimer *CUI::GetTimer(void)
{
	return m_pTimer;
}

//=============================================================================
//    �R���{UI�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CCombo *CUI::GetCombo(void)
{
	return m_pCombo;
}

//=============================================================================
//    ���݂̃X�e�[�W���|���S���N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CScene2D *CUI::GetNumStage(void)
{
	return m_pNumStage;
}

//=============================================================================
//    �ړI�̃|���S���N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CScene2D *CUI::GetPurpose(void)
{
	return m_pPurpose;
}