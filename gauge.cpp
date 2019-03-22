//*****************************************************************************
//
//     �Q�[�W�̏���[gauge.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "gauge.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define GAUGE_LIFE_CUTTIMING     (60)    // �̗̓Q�[�W�����炵���n�܂�^�C�~���O
#define GAUGE_WIDTH_ADD          (42.0f) // �Q�[�W�̕�����銄��
#define SPECIALGAUGE_WIDTH_ADD   (30.0f) // �K�E�Z�Q�[�W�̕�����銄��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CGauge�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGauge::CGauge(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// �e��l�̃N���A
	m_fGaugeRate = 0.0f;  // �Q�[�W�̊���
	m_fLeftWidth = 0.0f;  // �Q�[�W�̍������W
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nPriority)
{
	CGauge *pGauge = NULL;                 // �Q�[�W�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGauge == NULL)
		{// ����������ɂȂ��Ă���
			pGauge = new CGauge(nPriority);
			if (pGauge != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGauge->SetPos(pos);         // �|���S���̍��W
				pGauge->SetCol(col);         // �|���S���̐F
				pGauge->SetRot(fRot);        // �|���S���̌���
				pGauge->SetWidth(fWidth);    // �|���S���̕�
				pGauge->SetHeight(fHeight);  // �|���S���̍���

				if (FAILED(pGauge->Init()))
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

	return pGauge;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGauge::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGauge::Uninit(void)
{
	// ���ʂ̏I������
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGauge::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGauge::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;

			// ���C�e�B���O�̐ݒ���O��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// ���ʂ̕`�揈��
			CSceneBillboard::Draw();

			// ���C�e�B���O�̐ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGauge::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	m_fLeftWidth = -GetWidth();  // �Q�[�W�̍����I�t�Z�b�g
}

//=============================================================================
//    �Q�[�W�̊����ݒ菈��
//=============================================================================
void CGauge::SetGaugeRate(const float fGaugeRate)
{
	m_fGaugeRate = fGaugeRate;
}

//=============================================================================
//    �Q�[�W�̍����̕��ݒ菈��
//=============================================================================
void CGauge::SetLeftWidth(const float fLeftWidth)
{
	m_fLeftWidth = fLeftWidth;
}

//=============================================================================
//    �Q�[�W�̊����擾����
//=============================================================================
float CGauge::GetGaugeRate(void)
{
	return m_fGaugeRate;
}

//=============================================================================
//    �Q�[�W�̍����̕��擾����
//=============================================================================
float CGauge::GetLeftWidth(void)
{
	return m_fLeftWidth;
}



//*****************************************************************************
//    CGaugeLife�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGaugeLife::CGaugeLife(int nPriority, OBJTYPE objType) : CGauge(nPriority, objType)
{
	// �e��l�̃N���A
	m_nCounter = 0;            // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	m_State = STATE_NONE;      // ���
	m_nLife = 0;               // �̗�
	m_pRedBill = NULL;         // �̗̓Q�[�W�����������ɕ\������Ԃ��Q�[�W
	m_pWhiteBill = NULL;       // �̗̓Q�[�W�̃t���[��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGaugeLife::~CGaugeLife()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGaugeLife *CGaugeLife::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, int nPriority)
{
	CGaugeLife *pGaugeLife = NULL;         // �̗̓Q�[�W�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGaugeLife == NULL)
		{// ����������ɂȂ��Ă���
			pGaugeLife = new CGaugeLife(nPriority);
			if (pGaugeLife != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGaugeLife->SetPos(pos);         // �|���S���̍��W
				pGaugeLife->SetCol(col);         // �|���S���̐F
				pGaugeLife->SetRot(fRot);        // �|���S���̌���
				pGaugeLife->SetWidth(fWidth);    // �|���S���̕�
				pGaugeLife->SetHeight(fHeight);  // �|���S���̍���
				pGaugeLife->SetLife(nLife);      // �̗�

				if (FAILED(pGaugeLife->Init()))
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

	return pGaugeLife;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGaugeLife::Init(void)
{
	// �������Ɏ��s����
	if (FAILED(CGauge::Init()))
	{
		return E_FAIL;
	}

	// �̗̓Q�[�W�̃t���[���𐶐�����
	if (m_pWhiteBill == NULL)
	{
		m_pWhiteBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (GetWidth() * 1.04f), (GetHeight() * 1.1f), 0.0f,2);
	}

	// ���̐Ԃ��Q�[�W�𐶐�����
	if (m_pRedBill == NULL)
	{
		m_pRedBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, 2);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGaugeLife::Uninit(void)
{
	// �̗̓Q�[�W�̃t���[���J������
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->Uninit();
		m_pWhiteBill = NULL;
	}

	// �Ԃ��Q�[�W�̊J������
	if (m_pRedBill != NULL)
	{
		m_pRedBill->Uninit();
		m_pRedBill = NULL;
	}

	// ���ʂ̕`�揈��
	CGauge::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGaugeLife::Update(void)
{
	if (m_State == STATE_CUT)
	{// �Q�[�W�����炷��Ԃ�������
		m_nCounter++;  // �J�E���^�[�𑝂₷
		if (m_nCounter >= GAUGE_LIFE_CUTTIMING)
		{// �Q�[�W�����炷�^�C�~���O�ɂȂ��Ă���
			// ���̐Ԃ��Q�[�W�����炷
			if (m_pRedBill != NULL)
			{// �Ԃ��Q�[�W����������Ă���
				// �Ԃ��Q�[�W�̕������������炷
				float fWidth = m_pRedBill->GetWidth();
				fWidth -= 0.5f;
				if (fWidth <= GetWidth())
				{// �Ԃ��Q�[�W�̕����̗̓Q�[�W���������
					fWidth = GetWidth();   // ����߂�
					m_State = STATE_NONE;  // ��Ԃ�߂�
					m_nCounter = 0;        // �J�E���^�[��߂�
				}
				m_pRedBill->SetWidth(fWidth);

				// �|���S���̕������炷
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRedBill->GetVtxBuff();
				if (pVtxBuff != NULL)
				{// ���_�o�b�t�@����������Ă���
					// ���_���̐ݒ�
					VERTEX_3D *pVtx;

					// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// ���_���W
					pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(fWidth, GetHeight(), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f);

					// ���_�o�b�t�@���A�����b�N����
					pVtxBuff->Unlock();
				}
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGaugeLife::Draw(void)
{
	// ���ʂ̕`�揈��
	CGauge::Draw();
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGaugeLife::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	SetLeftWidth(-GetWidth());                    // �Q�[�W�̍����I�t�Z�b�g
	SetGaugeRate((GetWidth() * 2.0f) / m_nLife);  // �Q�[�W��{���̊���
}

//=============================================================================
//    �Q�[�W�����炷����
//=============================================================================
void CGaugeLife::CutGauge(const int nCutValue)
{
	// ��Ԃ�ݒ�
	m_State = STATE_CUT;

	// �Q�[�W�̕������炷
	float fWidth = GetWidth();  // ��
	fWidth -= GetGaugeRate() * nCutValue;
	SetWidth(fWidth);

	// �|���S���̕������炷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();

		if (fWidth == GetLeftWidth())
		{// ��������0��������
		    // ��Ԃ�ݒ�
			m_State = STATE_NONE;

			// �|���S���̕������炷
			pVtxBuff = m_pRedBill->GetVtxBuff();
			m_pRedBill->SetWidth(0.0f);
			if (pVtxBuff != NULL)
			{// ���_�o�b�t�@����������Ă���
			    // ���_���̐ݒ�
				VERTEX_3D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(fWidth, GetHeight(), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f);

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    �Q�[�W�𑝂₷����
//=============================================================================
void CGaugeLife::AddGauge(const int nAddValue)
{
	// �Q�[�W�̕��𑝂₷
	float fWidth = GetWidth();  // ��
	fWidth += GetGaugeRate() * nAddValue;
	SetWidth(fWidth);

	// �|���S���̕��𑝂₷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W�̍��W�����炷����
//=============================================================================
void CGaugeLife::SetGaugePos(const D3DXVECTOR3 pos)
{
	SetPos(pos);

	// �Ԃ��Q�[�W����������Ă���
	if (m_pRedBill != NULL)
	{
		m_pRedBill->SetPos(pos);
	}

	// �Q�[�W�̃t���[������������Ă���
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->SetPos(pos);
	}
}

//=============================================================================
//    ��Ԑݒ菈��
//=============================================================================
void CGaugeLife::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �̗͐ݒ菈��
//=============================================================================
void CGaugeLife::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    ��Ԏ擾����
//=============================================================================
CGaugeLife::STATE CGaugeLife::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �̗͎擾����
//=============================================================================
int CGaugeLife::GetLife(void)
{
	return m_nLife;
}


//*****************************************************************************
//    CGauge2D�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGauge2D::CGauge2D(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_pFrameTexture = NULL;  // �Q�[�W�t���[���p�̃e�N�X�`��
	m_fGaugeRate = 0.0f;     // �Q�[�W�̊���
	m_fLeftWidth = 0.0f;     // �Q�[�W�̍������W
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGauge2D::~CGauge2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGauge2D *CGauge2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nPriority)
{
	CGauge2D *pGauge2D = NULL;             // �Q�[�W(2D)�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGauge2D == NULL)
		{// ����������ɂȂ��Ă���
			pGauge2D = new CGauge2D(nPriority);
			if (pGauge2D != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGauge2D->SetPos(pos);         // �|���S���̍��W
				pGauge2D->SetCol(col);         // �|���S���̐F
				pGauge2D->SetRot(fRot);        // �|���S���̌���
				pGauge2D->SetWidth(fWidth);    // �|���S���̕�
				pGauge2D->SetHeight(fHeight);  // �|���S���̍���

				if (FAILED(pGauge2D->Init()))
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

	return pGauge2D;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGauge2D::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGauge2D::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGauge2D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGauge2D::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGauge2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f) + GetPos();

	// �e�N�X�`���l
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	m_fLeftWidth = -GetWidth();  // �Q�[�W�̍����I�t�Z�b�g
}

//=============================================================================
//    �Q�[�W�t���[���p�̃e�N�X�`���ݒ菈��
//=============================================================================
void CGauge2D::SetFrameTexture(LPDIRECT3DTEXTURE9 pFrameTexture)
{
	m_pFrameTexture = pFrameTexture;
}

//=============================================================================
//    �Q�[�W�̊����ݒ菈��
//=============================================================================
void CGauge2D::SetGaugeRate(const float fGaugeRate)
{
	m_fGaugeRate = fGaugeRate;
}

//=============================================================================
//    �Q�[�W�̍����̕��ݒ菈��
//=============================================================================
void CGauge2D::SetLeftWidth(const float fLeftWidth)
{
	m_fLeftWidth = fLeftWidth;
}

//=============================================================================
//    �Q�[�W�t���[���p�̃e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CGauge2D::GetFrameTexture(void)
{
	return m_pFrameTexture;
}

//=============================================================================
//    �Q�[�W�̊����擾����
//=============================================================================
float CGauge2D::GetGaugeRate(void)
{
	return m_fGaugeRate;
}

//=============================================================================
//    �Q�[�W�̍����̕��擾����
//=============================================================================
float CGauge2D::GetLeftWidth(void)
{
	return m_fLeftWidth;
}


//*****************************************************************************
//    CGaugeLife2D�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGaugeLife2D::CGaugeLife2D(int nPriority, OBJTYPE objType) : CGauge2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_nCounter = 0;            // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	m_State = STATE_NONE;      // ���
	m_nLife = 0;               // �̗�
	m_pBlack2D = NULL;         // �̗̓Q�[�W�̍��������p
	m_pRed2D = NULL;           // �̗̓Q�[�W�����������ɕ\������Ԃ��Q�[�W
	m_pFrame2D = NULL;         // �̗̓Q�[�W�̃t���[��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGaugeLife2D::~CGaugeLife2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGaugeLife2D *CGaugeLife2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, LPDIRECT3DTEXTURE9 pFrameTexture, int nPriority)
{
	CGaugeLife2D *pGaugeLife2D = NULL;     // �̗̓Q�[�W�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGaugeLife2D == NULL)
		{// ����������ɂȂ��Ă���
			pGaugeLife2D = new CGaugeLife2D(nPriority);
			if (pGaugeLife2D != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGaugeLife2D->SetPos(pos);                     // �|���S���̍��W
				pGaugeLife2D->SetCol(col);                     // �|���S���̐F
				pGaugeLife2D->SetRot(fRot);                    // �|���S���̌���
				pGaugeLife2D->SetWidth(fWidth);                // �|���S���̕�
				pGaugeLife2D->SetHeight(fHeight);              // �|���S���̍���
				pGaugeLife2D->SetLife(nLife);                  // �̗�
				pGaugeLife2D->SetFrameTexture(pFrameTexture);  // �t���[���p�̃e�N�X�`��

				if (FAILED(pGaugeLife2D->Init()))
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

	return pGaugeLife2D;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGaugeLife2D::Init(void)
{
	// �������Ɏ��s����
	if (FAILED(CGauge2D::Init()))
	{
		return E_FAIL;
	}

	// ���̍��w�i�𐶐�����
	if (m_pBlack2D == NULL)
	{
		m_pBlack2D = CGauge2D::Create(GetPos(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pBlack2D != NULL)
		{
			// �|���S���̕������炷
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBlack2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// ���_�o�b�t�@����������Ă���
			    // ���_���̐ݒ�
				VERTEX_2D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
		}
	}

	// ���̐Ԃ��Q�[�W�𐶐�����
	if (m_pRed2D == NULL)
	{
		m_pRed2D = CGauge2D::Create(GetPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pRed2D != NULL)
		{
			// �|���S���̕������炷
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRed2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// ���_�o�b�t�@����������Ă���
			    // ���_���̐ݒ�
				VERTEX_2D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
		}
	}

	// �̗̓Q�[�W�̃t���[���𐶐�����
	if (m_pFrame2D == NULL)
	{
		m_pFrame2D = CGauge2D::Create(D3DXVECTOR3(1035.0f, 660.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250.0f, 75.0f, 0.0f, GetPriority());
		if (m_pFrame2D != NULL)
		{// �����ł���
			LPDIRECT3DTEXTURE9 pFrameTexture = GetFrameTexture();
			if (pFrameTexture != NULL)
			{// �e�N�X�`���ւ̃|�C���^���擾�ł���
				m_pFrame2D->BindTexture(pFrameTexture);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGaugeLife2D::Uninit(void)
{
	// �����Q�[�W�̊J������
	if (m_pBlack2D != NULL)
	{
		m_pBlack2D = NULL;
	}

	// �̗̓Q�[�W�̃t���[���J������
	if (m_pFrame2D != NULL)
	{
		m_pFrame2D = NULL;
	}

	// �Ԃ��Q�[�W�̊J������
	if (m_pRed2D != NULL)
	{
		m_pRed2D = NULL;
	}

	// ���ʂ̕`�揈��
	CGauge2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGaugeLife2D::Update(void)
{
	if (m_State == STATE_CUT)
	{// �Q�[�W�����炷��Ԃ�������
		m_nCounter++;  // �J�E���^�[�𑝂₷
		if (m_nCounter >= GAUGE_LIFE_CUTTIMING)
		{// �Q�[�W�����炷�^�C�~���O�ɂȂ��Ă���
		 // ���̐Ԃ��Q�[�W�����炷
			if (m_pRed2D != NULL)
			{// �Ԃ��Q�[�W����������Ă���
			    // �Ԃ��Q�[�W�̕������������炷
				float fWidth = m_pRed2D->GetWidth();
				fWidth -= 2.0f;
				if (fWidth <= GetWidth())
				{// �Ԃ��Q�[�W�̕����̗̓Q�[�W���������
					fWidth = GetWidth();   // ����߂�
					m_State = STATE_NONE;  // ��Ԃ�߂�
					m_nCounter = 0;        // �J�E���^�[��߂�
				}
				m_pRed2D->SetWidth(fWidth);

				// �|���S���̕������炷
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRed2D->GetVtxBuff();
				if (pVtxBuff != NULL)
				{// ���_�o�b�t�@����������Ă���
				    // ���_���̐ݒ�
					VERTEX_2D *pVtx;

					// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// ���_���W
					pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
					pVtx[1].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f) + GetPos();
					pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
					pVtx[3].pos = D3DXVECTOR3(fWidth + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

					// ���_�o�b�t�@���A�����b�N����
					pVtxBuff->Unlock();
				}
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGaugeLife2D::Draw(void)
{
	// ���ʂ̕`�揈��
	CGauge2D::Draw();
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGaugeLife2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

	// �e�N�X�`���l
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	SetLeftWidth(-GetWidth());                    // �Q�[�W�̍����I�t�Z�b�g
	SetGaugeRate((GetWidth() * 2.0f) / m_nLife);  // �Q�[�W��{���̊���
}

//=============================================================================
//    �Q�[�W�����炷����
//=============================================================================
void CGaugeLife2D::CutGauge(const int nCutValue)
{
	// ��Ԃ�ݒ�
	m_State = STATE_CUT;

	// �Q�[�W�̕������炷
	float fWidth = GetWidth();  // ��
	fWidth -= GetGaugeRate() * nCutValue;
	SetWidth(fWidth);

	// �|���S���̕������炷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(),-GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();

		if (fWidth == GetLeftWidth())
		{// ��������0��������
		    // ��Ԃ�ݒ�
			m_State = STATE_NONE;

		    // �|���S���̕������炷
			pVtxBuff = m_pRed2D->GetVtxBuff();
			m_pRed2D->SetWidth(0.0f);
			if (pVtxBuff != NULL)
			{// ���_�o�b�t�@����������Ă���
			    // ���_���̐ݒ�
				VERTEX_2D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(fWidth + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    �Q�[�W�𑝂₷����
//=============================================================================
void CGaugeLife2D::AddGauge(const int nAddValue)
{
	// �Q�[�W�̕��𑝂₷
	float fWidth = GetWidth();  // ��
	fWidth += GetGaugeRate() * nAddValue;
	SetWidth(fWidth);

	// �|���S���̕��𑝂₷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �����x��ύX���鏈��
//=============================================================================
void CGaugeLife2D::ChangeAlpha(const float fAlpha)
{
	D3DXCOLOR col = GetCol();
	col.a = fAlpha;
	SetCol(col);

	// �Ԃ��Q�[�W�̓����x���ݒ肷��
	if (m_pRed2D != NULL)
	{// ��������Ă���
		col = m_pRed2D->GetCol();
		col.a = fAlpha;
		m_pRed2D->SetCol(col);
	}

	// �Q�[�W�t���[���̓����x���ݒ肷��
	if (m_pFrame2D != NULL)
	{// ��������Ă���
		col = m_pFrame2D->GetCol();
		col.a = fAlpha;
		m_pFrame2D->SetCol(col);
	}

	// �����|���S���̓����x���ݒ肷��
	if (m_pBlack2D != NULL)
	{// ��������Ă���
		col = m_pBlack2D->GetCol();
		col.a = fAlpha;
		m_pBlack2D->SetCol(col);
	}
}

//=============================================================================
//    ��Ԑݒ菈��
//=============================================================================
void CGaugeLife2D::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �̗͐ݒ菈��
//=============================================================================
void CGaugeLife2D::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    ��Ԏ擾����
//=============================================================================
CGaugeLife2D::STATE CGaugeLife2D::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �̗͎擾����
//=============================================================================
int CGaugeLife2D::GetLife(void)
{
	return m_nLife;
}


//*****************************************************************************
//    CGaugeOrbitSlash2D�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGaugeOrbitSlash2D::CGaugeOrbitSlash2D(int nPriority, OBJTYPE objType) : CGauge2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_nCounter = 0;            // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	m_State = STATE_NONE;      // ���
	m_fOrbitSlash = 0.0f;      // �O�ՃQ�[�W��
	m_pBlack2D = NULL;         // �����w�i�p�|���S��
	m_pFrame2D = NULL;         // �O�ՃQ�[�W�̃t���[��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGaugeOrbitSlash2D::~CGaugeOrbitSlash2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGaugeOrbitSlash2D *CGaugeOrbitSlash2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fOrbitSlash, LPDIRECT3DTEXTURE9 pFrameTexture, int nPriority)
{
	CGaugeOrbitSlash2D *pGaugeOrbirtSlash2D = NULL;  // �O�ՃQ�[�W�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();             // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGaugeOrbirtSlash2D == NULL)
		{// ����������ɂȂ��Ă���
			pGaugeOrbirtSlash2D = new CGaugeOrbitSlash2D(nPriority);
			if (pGaugeOrbirtSlash2D != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGaugeOrbirtSlash2D->SetPos(pos);                     // �|���S���̍��W
				pGaugeOrbirtSlash2D->SetCol(col);                     // �|���S���̐F
				pGaugeOrbirtSlash2D->SetRot(fRot);                    // �|���S���̌���
				pGaugeOrbirtSlash2D->SetWidth(fWidth);                // �|���S���̕�
				pGaugeOrbirtSlash2D->SetHeight(fHeight);              // �|���S���̍���
				pGaugeOrbirtSlash2D->SetOrbitSlash(fOrbitSlash);      // �O�ՃQ�[�W��
				pGaugeOrbirtSlash2D->SetFrameTexture(pFrameTexture);  // �t���[���p�̃e�N�X�`��

				if (FAILED(pGaugeOrbirtSlash2D->Init()))
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

	return pGaugeOrbirtSlash2D;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGaugeOrbitSlash2D::Init(void)
{
	// �������Ɏ��s����
	if (FAILED(CGauge2D::Init()))
	{
		return E_FAIL;
	}

	// ���̍��w�i�𐶐�����
	if (m_pBlack2D == NULL)
	{
		m_pBlack2D = CGauge2D::Create(GetPos(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pBlack2D != NULL)
		{
			// �|���S���̕������炷
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBlack2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// ���_�o�b�t�@����������Ă���
			    // ���_���̐ݒ�
				VERTEX_2D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
		}
	}

	// �O�ՃQ�[�W�̃t���[���𐶐�����
	if (m_pFrame2D == NULL)
	{
		m_pFrame2D = CGauge2D::Create(D3DXVECTOR3(245.0f, 678.0f, GetPos().z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 250.0f, 50.0f, 0.0f, GetPriority());
		if(m_pFrame2D != NULL)
		{// �����ł���
			LPDIRECT3DTEXTURE9 pFrameTexture = GetFrameTexture();
			if (pFrameTexture != NULL)
			{// �e�N�X�`���ւ̃|�C���^���擾�ł���
				m_pFrame2D->BindTexture(pFrameTexture);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGaugeOrbitSlash2D::Uninit(void)
{
	// �����Q�[�W�̊J������
	if (m_pBlack2D != NULL)
	{
		m_pBlack2D = NULL;
	}

	// �O�ՃQ�[�W�̃t���[���J������
	if (m_pFrame2D != NULL)
	{
		m_pFrame2D = NULL;
	}

	// ���ʂ̕`�揈��
	CGauge2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGaugeOrbitSlash2D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGaugeOrbitSlash2D::Draw(void)
{
	// ���ʂ̕`�揈��
	CGauge2D::Draw();
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGaugeOrbitSlash2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

	// �e�N�X�`���l
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	SetLeftWidth(-GetWidth());                          // �Q�[�W�̍����I�t�Z�b�g
	SetGaugeRate((GetWidth() * 2.0f) / m_fOrbitSlash);  // �Q�[�W��{���̊���
}

//=============================================================================
//    �Q�[�W�����炷����
//=============================================================================
void CGaugeOrbitSlash2D::CutGauge(const float fCutValue)
{
	// ��Ԃ�ݒ�
	m_State = STATE_CUT;

	// �Q�[�W�̕������炷
	float fWidth = GetWidth();  // ��
	fWidth -= GetGaugeRate() * fCutValue;
	SetWidth(fWidth);

	// �|���S���̕������炷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W�𑝂₷����
//=============================================================================
void CGaugeOrbitSlash2D::AddGauge(const float fAddValue)
{
	// �Q�[�W�̕��𑝂₷
	float fWidth = GetWidth();  // ��
	fWidth += GetGaugeRate() * fAddValue;
	SetWidth(fWidth);

	// �|���S���̕��𑝂₷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W���Ȃ�������
//=============================================================================
void CGaugeOrbitSlash2D::DeleteGauge(void)
{
	// �Q�[�W�̕����Ȃ���
	float fWidth = GetWidth();  // ��
	fWidth = GetLeftWidth();
	SetWidth(fWidth);

	// �|���S���̕����Ȃ���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + GAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �����x��ύX���鏈��
//=============================================================================
void CGaugeOrbitSlash2D::ChangeAlpha(const float fAlpha)
{
	D3DXCOLOR col = GetCol();
	col.a = fAlpha;
	SetCol(col);

	// �Q�[�W�t���[���̓����x���ݒ肷��
	if (m_pFrame2D != NULL)
	{// ��������Ă���
		col = m_pFrame2D->GetCol();
		col.a = fAlpha;
		m_pFrame2D->SetCol(col);
	}

	// �����|���S���̓����x���ݒ肷��
	if (m_pBlack2D != NULL)
	{// ��������Ă���
		col = m_pBlack2D->GetCol();
		col.a = fAlpha;
		m_pBlack2D->SetCol(col);
	}
}

//=============================================================================
//    ��Ԑݒ菈��
//=============================================================================
void CGaugeOrbitSlash2D::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �O�ՃQ�[�W�ʐݒ菈��
//=============================================================================
void CGaugeOrbitSlash2D::SetOrbitSlash(const float fOrbitSlash)
{
	m_fOrbitSlash = fOrbitSlash;
}

//=============================================================================
//    ��Ԏ擾����
//=============================================================================
CGaugeOrbitSlash2D::STATE CGaugeOrbitSlash2D::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �O�ՃQ�[�W�ʎ擾����
//=============================================================================
float CGaugeOrbitSlash2D::GetOrbitSlash(void)
{
	return m_fOrbitSlash;
}


//*****************************************************************************
//    CGaugeSpecial2D�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGaugeSpecial2D::CGaugeSpecial2D(int nPriority, OBJTYPE objType) : CGauge2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_nCounter = 0;            // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	m_State = STATE_NONE;      // ���
	m_fSpecial = 0.0f;         // �K�E�Q�[�W��
	m_fMaxSpecial = 0.0f;      // �K�E�Q�[�W�̍ő�l
	m_pBlack2D = NULL;         // �����w�i�p�|���S��
	m_pFrame2D = NULL;         // �K�E�Q�[�W�̃t���[��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGaugeSpecial2D::~CGaugeSpecial2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGaugeSpecial2D *CGaugeSpecial2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fMaxSpecial, float fSpecial, LPDIRECT3DTEXTURE9 pFrameTexture, int nPriority)
{
	CGaugeSpecial2D *pGaugeSpecial2D = NULL;  // �K�E�Z�Q�[�W�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();      // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGaugeSpecial2D == NULL)
		{// ����������ɂȂ��Ă���
			pGaugeSpecial2D = new CGaugeSpecial2D(nPriority);
			if (pGaugeSpecial2D != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGaugeSpecial2D->SetPos(pos);                     // �|���S���̍��W
				pGaugeSpecial2D->SetCol(col);                     // �|���S���̐F
				pGaugeSpecial2D->SetRot(fRot);                    // �|���S���̌���
				pGaugeSpecial2D->SetWidth(fWidth);                // �|���S���̕�
				pGaugeSpecial2D->SetHeight(fHeight);              // �|���S���̍���
				pGaugeSpecial2D->SetSpecial(fSpecial);            // �K�E�Z�Q�[�W��
				pGaugeSpecial2D->SetMaxSpecial(fMaxSpecial);      // �K�E�Q�[�W�̍ő�l
				pGaugeSpecial2D->SetFrameTexture(pFrameTexture);  // �t���[���p�̃e�N�X�`��

				if (FAILED(pGaugeSpecial2D->Init()))
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

	return pGaugeSpecial2D;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGaugeSpecial2D::Init(void)
{
	// �������Ɏ��s����
	if (FAILED(CGauge2D::Init()))
	{
		return E_FAIL;
	}

	// ���̍��w�i�𐶐�����
	if (m_pBlack2D == NULL)
	{
		m_pBlack2D = CGauge2D::Create(GetPos(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), GetWidth(), GetHeight(), 0.0f, GetPriority() - 1);
		if (m_pBlack2D != NULL)
		{
			// �|���S���̕������炷
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBlack2D->GetVtxBuff();
			if (pVtxBuff != NULL)
			{// ���_�o�b�t�@����������Ă���
			    // ���_���̐ݒ�
				VERTEX_2D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
				pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
				pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
		}
	}

	// �K�E�Q�[�W�̃t���[���𐶐�����
	if (m_pFrame2D == NULL)
	{
		m_pFrame2D = CGauge2D::Create(D3DXVECTOR3(174.0f,610.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180.0f, 50.0f, 0.0f, GetPriority());
		if (m_pFrame2D != NULL)
		{// �����ł���
			LPDIRECT3DTEXTURE9 pFrameTexture = GetFrameTexture();
			if (pFrameTexture != NULL)
			{// �e�N�X�`���ւ̃|�C���^���擾�ł���
				m_pFrame2D->BindTexture(pFrameTexture);
			}
		}
	}

	// �Q�[�W�̕������Z�b�g
	DeleteGauge();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGaugeSpecial2D::Uninit(void)
{
	// �����Q�[�W�̊J������
	if (m_pBlack2D != NULL)
	{
		m_pBlack2D = NULL;
	}

	// �K�E�Q�[�W�̃t���[���J������
	if (m_pFrame2D != NULL)
	{
		m_pFrame2D = NULL;
	}

	// ���ʂ̕`�揈��
	CGauge2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGaugeSpecial2D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGaugeSpecial2D::Draw(void)
{
	// ���ʂ̕`�揈��
	CGauge2D::Draw();
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGaugeSpecial2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[1].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f) + GetPos();
	pVtx[2].pos = D3DXVECTOR3(-GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
	pVtx[3].pos = D3DXVECTOR3(-GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

	// �e�N�X�`���l
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	SetLeftWidth(-GetWidth());                          // �Q�[�W�̍����I�t�Z�b�g
	SetGaugeRate((GetWidth() * 2.0f) / m_fMaxSpecial);  // �Q�[�W��{���̊���
}

//=============================================================================
//    �Q�[�W�����炷����
//=============================================================================
void CGaugeSpecial2D::CutGauge(const float fCutValue)
{
	// ��Ԃ�ݒ�
	m_State = STATE_CUT;

	// �Q�[�W�̕������炷
	float fWidth = GetWidth();  // ��
	fWidth -= GetGaugeRate() * fCutValue;
	SetWidth(fWidth);

	// �|���S���̕������炷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W�𑝂₷����
//=============================================================================
void CGaugeSpecial2D::AddGauge(const float fAddValue)
{
	// �Q�[�W�̕��𑝂₷
	float fWidth = GetWidth();  // ��
	fWidth += GetGaugeRate() * fAddValue;
	SetWidth(fWidth);

	// �|���S���̕��𑝂₷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W���Ȃ�������
//=============================================================================
void CGaugeSpecial2D::DeleteGauge(void)
{
	// �Q�[�W�̕����Ȃ���
	float fWidth = GetWidth();  // ��
	fWidth = GetLeftWidth();
	SetWidth(fWidth);

	// �|���S���̕����Ȃ���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W���ő�ɂ��鏈��
//=============================================================================
void CGaugeSpecial2D::MaxGauge(void)
{
	// �Q�[�W�̕��𑝂₷
	float fWidth = GetWidth();  // ��
	fWidth = (m_fMaxSpecial * GetGaugeRate()) / 2;
	SetWidth(fWidth);

	// �|���S���̕��𑝂₷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetLeftWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f) + GetPos();
		pVtx[2].pos = D3DXVECTOR3(GetLeftWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();
		pVtx[3].pos = D3DXVECTOR3(GetWidth() + SPECIALGAUGE_WIDTH_ADD, GetHeight(), 0.0f) + GetPos();

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �����x��ύX���鏈��
//=============================================================================
void CGaugeSpecial2D::ChangeAlpha(const float fAlpha)
{
	D3DXCOLOR col = GetCol();
	col.a = fAlpha;
	SetCol(col);

	// �Q�[�W�t���[���̓����x���ݒ肷��
	if (m_pFrame2D != NULL)
	{// ��������Ă���
		col = m_pFrame2D->GetCol();
		col.a = fAlpha;
		m_pFrame2D->SetCol(col);
	}

	// �����|���S���̓����x���ݒ肷��
	if (m_pBlack2D != NULL)
	{// ��������Ă���
		col = m_pBlack2D->GetCol();
		col.a = fAlpha;
		m_pBlack2D->SetCol(col);
	}
}

//=============================================================================
//    ��Ԑݒ菈��
//=============================================================================
void CGaugeSpecial2D::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �K�E�Q�[�W�ʐݒ菈��
//=============================================================================
void CGaugeSpecial2D::SetSpecial(const float fSpecial)
{
	m_fSpecial = fSpecial;
}

//=============================================================================
//    �K�E�Q�[�W�̍ő�l�ݒ菈��
//=============================================================================
void CGaugeSpecial2D::SetMaxSpecial(const float fMaxSpecial)
{
	m_fMaxSpecial = fMaxSpecial;
}

//=============================================================================
//    ��Ԏ擾����
//=============================================================================
CGaugeSpecial2D::STATE CGaugeSpecial2D::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �K�E�Q�[�W�ʎ擾����
//=============================================================================
float CGaugeSpecial2D::GetSpecial(void)
{
	return m_fSpecial;
}

//=============================================================================
//    �K�E�Q�[�W�̍ő�l�擾����
//=============================================================================
float CGaugeSpecial2D::GetMaxSpecial(void)
{
	return m_fMaxSpecial;
}