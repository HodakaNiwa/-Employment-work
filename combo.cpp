//*****************************************************************************
//
//     �R���{UI�̏���[combo.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "combo.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "ui.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define COMBO_MOVE_SPEED      (30.0f)   // �R���{UI������̈ʒu�Ɉړ�����X�s�[�h
#define COMBO_DESTORY_TIMING  (300)     // �R���{UI��������܂ł̎���(���̕b���ŃR���{�̌q����₷���������ł���)7
#define COMBO_MOVESTOP_POS    (1170.0f) // �R���{UI�̈ړ����~�߂�ʒu
#define COMBO_NUMBER_INTERVAL (0.7f)    // �����Ɛ����̊Ԋu

#define COMBO_LOGO_POS_0     (D3DXVECTOR3(64.0f, 4.0f, 0.0f))    // �R���{UI��1�ڂ̃��S�|���S���̃I�t�Z�b�g�ʒu
#define COMBO_LOGO_COL_0     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))    // �R���{UI��1�ڂ̃��S�|���S���̐F
#define COMBO_LOGO_WIDTH_0   (40.0f)                             // �R���{UI��1�ڂ̃��S�|���S���̕�
#define COMBO_LOGO_HEIGHT_0  (20.0f)                             // �R���{UI��1�ڂ̃��S�|���S���̍���

#define COMBO_LOGO_POS_1     (D3DXVECTOR3(63.0f, 43.0f, 0.0f))   // �R���{UI��2�ڂ̃��S�|���S���̃I�t�Z�b�g�ʒu
#define COMBO_LOGO_COL_1     (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))    // �R���{UI��2�ڂ̃��S�|���S���̐F
#define COMBO_LOGO_WIDTH_1   (180.5f)                            // �R���{UI��2�ڂ̃��S�|���S���̕�
#define COMBO_LOGO_HEIGHT_1  (4.5f)                              // �R���{UI��2�ڂ̃��S�|���S���̍���

#define COMBO_LOGO_POS_2     (D3DXVECTOR3(60.0f, 40.0f, 0.0f))   // �R���{UI��3�ڂ̃��S�|���S���̃I�t�Z�b�g�ʒu
#define COMBO_LOGO_COL_2     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))    // �R���{UI��3�ڂ̃��S�|���S���̐F
#define COMBO_LOGO_WIDTH_2   (180.0f)                            // �R���{UI��3�ڂ̃��S�|���S���̕�
#define COMBO_LOGO_HEIGHT_2  (4.0f)                              // �R���{UI��3�ڂ̃��S�|���S���̍���

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCombo::CCombo(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntNumber = 0; nCntNumber < MAX_COMBO_NUMBER; nCntNumber++)
	{// �R���{UI�̌������J��Ԃ�
		m_apNumber[nCntNumber] = NULL;           // �����|���S���N���X�ւ̃|�C���^
	}
	for (int nCntLogo = 0; nCntLogo < 3; nCntLogo++)
	{// ���S�|���S���̐������J��Ԃ�
		m_pComboLogo[nCntLogo] = NULL;           // ���S�|���S���N���X�ւ̃|�C���^
	}
	m_State = STATE_APPEAR;                      // ���
	m_nCounter = 0;                              // ��ԊǗ��J�E���^�[
	m_nDigit = 0;                                // �R���{UI�̌���
	m_nCombo = 0;                                // �R���{��
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �R���{UI�̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �R���{UI�̐F
	m_fWidth = 0.0f;                             // �R���{UI�̐����|���S���P���̕�
	m_fHeight = 0.0f;                            // �R���{UI�̐����|���S���P���̕�
	m_pTexture = NULL;                           // �e�N�X�`���ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCombo::~CCombo()
{

}

//=============================================================================
//    ��������
//=============================================================================
CCombo *CCombo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CCombo *pCombo = NULL;                 // �R���{UI�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pCombo == NULL)
		{// ����������ɂȂ��Ă���
			pCombo = new CCombo(nPriority);
			if (pCombo != NULL)
			{// ���������m�ۂł���
			    // �e��l�̐ݒ�
				pCombo->SetPos(pos);         // �R���{UI�̍��W
				pCombo->SetCol(col);         // �R���{UI�̐F
				pCombo->SetWidth(fWidth);    // �R���{UI�̐����|���S���P���̕�
				pCombo->SetHeight(fHeight);  // �R���{UI�̐����|���S���P���̍���

				if (FAILED(pCombo->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// ���������m�ۂł��Ȃ�����
				return NULL;
			}
		}
		else
		{// ����������ł͂Ȃ�
			return NULL;
		}
	}
	else
	{// ����ȏ㐶���ł��Ȃ�
		return NULL;
	}

	return pCombo;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCombo::Init(void)
{
	D3DXVECTOR3 LogoPos = m_Pos + COMBO_LOGO_POS_1;
	D3DXCOLOR LogoCol = COMBO_LOGO_COL_1;
	float fLogoWidth = COMBO_LOGO_WIDTH_1;
	float fLogoHeight = COMBO_LOGO_HEIGHT_1;
	if (m_pComboLogo[1] == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pComboLogo[1] = CScene2D::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, 0.0f, GetPriority());
	}

	LogoPos = m_Pos + COMBO_LOGO_POS_2;
	LogoCol = COMBO_LOGO_COL_2;
	fLogoWidth = COMBO_LOGO_WIDTH_2;
	fLogoHeight = COMBO_LOGO_HEIGHT_2;
	if (m_pComboLogo[2] == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pComboLogo[2] = CScene2D::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, 0.0f, GetPriority());
	}

	// �e��l�̏�����
	D3DXVECTOR3 NumberPos = m_Pos;   // ���W
	D3DXCOLOR NumberCol = m_Col;     // �F
	float fNumberWidth = m_fWidth;   // ��
	float fNumberHeight = m_fHeight; // ����
	int nNumber = m_nCombo = 1;      // �\�����鐔��
	m_nDigit = 1;                    // ������1�ɂ��Ă���

	// �����|���S����1������������
	if (m_apNumber[0] == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apNumber[0] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());
	}

	// ���S�|���S���𐶐�����
	LogoPos = m_Pos + COMBO_LOGO_POS_0;
	LogoCol = COMBO_LOGO_COL_0;
	fLogoWidth = COMBO_LOGO_WIDTH_0;
	fLogoHeight = COMBO_LOGO_HEIGHT_0;
	if (m_pComboLogo[0] == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pComboLogo[0] = CScene2D::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, 0.0f, GetPriority());
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCombo::Uninit(void)
{
	// �����|���S���̊J������
	for (int nCntCombo = 0; nCntCombo < MAX_COMBO_NUMBER; nCntCombo++)
	{// �R���{�̌������J��Ԃ�
		if (m_apNumber[nCntCombo] != NULL)
		{// ���������m�ۂ���Ă���
			// �����|���S���̏I������
			m_apNumber[nCntCombo]->Uninit();
			m_apNumber[nCntCombo] = NULL;
		}
	}

	// ���S�|���S���̊J������
	for (int nCntLogo = 0; nCntLogo < 3; nCntLogo++)
	{// ���S�|���S���̐������J��Ԃ�
		if (m_pComboLogo[nCntLogo] != NULL)
		{// ���������m�ۂ���Ă���
			m_pComboLogo[nCntLogo]->Uninit();
			m_pComboLogo[nCntLogo] = NULL;
		}
	}

	if(CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		// �ő�R���{���̍X�V
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			int nComboMax = pGame->GetComboMax();
			if (nComboMax < m_nCombo)
			{// �ő�R���{�����X�V���ꂽ
				pGame->SetComboMax(m_nCombo);
			}
			// UI�N���X�̃|�C���^����ɂ���
			CUI *pUi = pGame->GetUI();
			if (pUi != NULL)
			{// �|�C���^���擾�ł���
				pUi->ReleaseCombo();
			}
		}
	}

	// �R���{UI�̏�������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CCombo::Update(void)
{
	m_nCounter++;   // �J�E���^�[�𑝂₷
	if (m_State == STATE_APPEAR)
	{// �o����Ԃ�������
		// �ʒu�����炷
		m_Pos.x -= COMBO_MOVE_SPEED;
		if (m_Pos.x <= COMBO_MOVESTOP_POS)
		{// ����̈ʒu�ɓ��B����
			m_Pos.x = COMBO_MOVESTOP_POS;   // �ʒu��߂�
			m_State = STATE_NORMAL;         // ��Ԃ�ʏ��Ԃ�
		}

		// �����̈ʒu�����炷
		D3DXVECTOR3 NumberPos = m_Pos;   // ���W
		for (int nCntCombo = 0; nCntCombo < m_nDigit; nCntCombo++)
		{// �R���{�̌������J��Ԃ�
			if (m_apNumber[nCntCombo] != NULL)
			{// ���������m�ۂ���Ă���
				m_apNumber[nCntCombo]->SetPos(NumberPos);
			}
			// ���ɐi�߂�
			NumberPos.x -= m_fWidth + (m_fWidth * 0.5f);
		}


		// ���S�̈ʒu�����炷
		D3DXVECTOR3 LogoPos = m_Pos + COMBO_LOGO_POS_0;
		if (m_pComboLogo[0] != NULL)
		{// ���������m�ۂ���Ă���
			m_pComboLogo[0]->SetPos(LogoPos);
		}

		LogoPos = m_Pos + COMBO_LOGO_POS_1;
		if (m_pComboLogo[1] != NULL)
		{// ���������m�ۂ���Ă���
			m_pComboLogo[1]->SetPos(LogoPos);
		}

		LogoPos = m_Pos + COMBO_LOGO_POS_2;
		if (m_pComboLogo[2] != NULL)
		{// ���������m�ۂ���Ă���
			m_pComboLogo[2]->SetPos(LogoPos);
		}
	}
	else if (m_State == STATE_NORMAL)
	{// �ʏ��ԂȂ��
		if(m_nCounter % COMBO_DESTORY_TIMING == 0)
		{// �Ō�̃R���{���炠����x���Ԃ�������
			Uninit();
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CCombo::Draw(void)
{

}

//=============================================================================
//    �����e�N�X�`����ݒ肷�鏈��
//=============================================================================
void CCombo::SetNumberTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// �e�N�X�`���ւ̃|�C���^���擾�ł���
		for (int nCntCombo = 0; nCntCombo < m_nDigit; nCntCombo++)
		{// �R���{UI�̌������J��Ԃ�
			if (m_apNumber[nCntCombo] != NULL)
			{// �����|���S������������Ă���
				m_apNumber[nCntCombo]->BindTexture(m_pTexture);
			}
		}
	}
}

//=============================================================================
//    ���S�e�N�X�`����ݒ肷�鏈��
//=============================================================================
void CCombo::SetLogoTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx)
{
	if (m_pComboLogo[nIdx] != NULL)
	{// ���������m�ۂ���Ă���
		m_pComboLogo[nIdx]->BindTexture(pTexture);
	}
}

//=============================================================================
//    ���W��ݒ肷�鏈��
//=============================================================================
void CCombo::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �F��ݒ肷�鏈��
//=============================================================================
void CCombo::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    �����|���S���P���̕���ݒ肷�鏈��
//=============================================================================
void CCombo::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    �����|���S���P���̍�����ݒ肷�鏈��
//=============================================================================
void CCombo::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    �R���{����ݒ肷�鏈��
//=============================================================================
void CCombo::SetCombo(const int nCombo)
{
	m_nCombo = nCombo;
	m_nCounter = 0;

	int nNumber = 0;    // �\�����鐔��
	for (int nCntCombo = 0; nCntCombo < m_nDigit; nCntCombo++)
	{// �R���{�̌������J��Ԃ�
		if (m_apNumber[nCntCombo] != NULL)
		{// ���������m�ۂ���Ă���
		    // �\�����鐔���̌v�Z
 			nNumber = (m_nCombo) % ((int)powf(10.0f, (float)nCntCombo) * 10) / (int)powf(10.0f, (float)nCntCombo);
			m_apNumber[nCntCombo]->SetNumber(nNumber);
		}
	}

	// �R���{�̌������v�Z����
	int nDigit = (int)log10f((float)(m_nCombo)) + 1;   // ���������߂�
	if (nDigit <= 0) { nDigit = 1; }	               // 0�ȉ��̂Ƃ�1�ɂ���
	if (nDigit != m_nDigit)
	{// ���ݐ�������Ă��鐔���̌����ƈقȂ�
		D3DXVECTOR3 NumberPos = m_Pos;   // ���W
		D3DXCOLOR NumberCol = m_Col;     // �F
		float fNumberWidth = m_fWidth;   // ��
		float fNumberHeight = m_fHeight; // ����

		if (nDigit > m_nDigit)
		{// �����������Ă���
			for (int nCntCombo = 0; nCntCombo < nDigit; nCntCombo++)
			{// �R���{�̌������J��Ԃ�
			    // �\�����鐔���̌v�Z
				nNumber = (m_nCombo) % ((int)powf(10.0f, (float)nCntCombo) * 10) / (int)powf(10.0f, (float)nCntCombo);

				if (m_apNumber[nCntCombo] == NULL)
				{// ���������m�ۂ���Ă��Ȃ�
					// �����|���S���̃C���X�^���X��������
					m_apNumber[nCntCombo] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());
					m_apNumber[nCntCombo]->BindTexture(m_pTexture);
				}
				// ���ɐi�߂�
				NumberPos.x -= m_fWidth + (m_fWidth * COMBO_NUMBER_INTERVAL);
			}
		}
		else if (nDigit < m_nDigit)
		{// �����������Ă���
			for (int nCntCombo = m_nDigit - 1; nCntCombo > nDigit - 1; nCntCombo--)
			{// �R���{�̌������J��Ԃ�
				if (m_apNumber[nCntCombo] != NULL)
				{// ���������m�ۂ���Ă���
					m_apNumber[nCntCombo]->Uninit();
					m_apNumber[nCntCombo] = NULL;
				}
			}
		}

		// ������ݒ肵�Ȃ���
		m_nDigit = nDigit;
	}
}

//=============================================================================
//    ���W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CCombo::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �^�C�}�[�̐F���擾���鏈��
//=============================================================================
D3DXCOLOR CCombo::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    �����|���S���P���̕����擾���鏈��
//=============================================================================
float CCombo::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    �����|���S���P���̍������擾���鏈��
//=============================================================================
float CCombo::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �R���{�����擾���鏈��
//=============================================================================
int CCombo::GetCombo(void)
{
	return m_nCombo;
}