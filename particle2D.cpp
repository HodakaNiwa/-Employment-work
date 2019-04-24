//*****************************************************************************
//
//     �p�[�e�B�N��(2D)�̏���[particle2D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "particle2D.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParticle2D::CParticle2D(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �ǂꂭ�炢�F���ω����邩
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �ړ���
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ړ��ʂ̕ω���
	m_fSpread = 0.0f;                                // �ǂꂭ�炢�ό`���邩
	m_fChangeRot = 0.0f;                             // �ǂꂭ�炢��]���邩
	m_nLife = 0;                                     // ����
	m_nRotPattern = 0;                               // ��]�̎��
	m_bDrawAddtive = false;                          // ���Z�����ŕ`�悷�邩�ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParticle2D::~CParticle2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CParticle2D *CParticle2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
	float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive, int nPriority)
{
	CParticle2D *pParticle2D = NULL;  // �p�[�e�B�N��(2D)�N���X�^�̃|�C���^
	if (pParticle2D == NULL)
	{// ����������ɂȂ��Ă���
		pParticle2D = new CParticle2D(nPriority);
		if (pParticle2D != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pParticle2D->Init(pos, move, ChangeMove, col, fWidth, fHeight, fRot, ChangeCol, fSpread, fChangeRot, nLife, nRotPattern, bDrawAddtive)))
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

	return pParticle2D;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CParticle2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
	float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive)
{
	// �e��l�̐ݒ�
	SetPos(pos);                       // ���W
	SetCol(col);                       // �F
	SetWidth(fWidth);                  // ��
	SetHeight(fHeight);                // ����
	SetRot(fRot);                      // ����
	m_Move = move;                     // �ړ���
	m_ChangeMove = ChangeMove;         // �ړ��ʂ̕ω���
	m_ChangeCol = ChangeCol;           // �F
	m_fSpread = fSpread;               // ���a�̕ω���
	m_fChangeRot = fChangeRot;         // �����̕ω���
	m_nLife = nLife;                   // ����
	m_nRotPattern = nRotPattern;       // ��]�̃p�^�[��
	m_bDrawAddtive = bDrawAddtive;     // ���Z�����ŕ`�悷�邩���Ȃ���

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
void CParticle2D::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CParticle2D::Update(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();  // ���W
	D3DXCOLOR col = GetCol();    // �F
	float fRot = GetRot();       // ����
	float fRadius = GetLength(); // ���a

	// �e��l�̍X�V
	pos += m_Move;           // �ʒu�̍X�V
	col += m_ChangeCol;      // �F�̍X�V
	m_nLife--;               // ���������炷
	fRadius += m_fSpread;    // �傫���̍X�V

	if (m_nLife <= 0 || fRadius < 0.0f || col.a <= 0.0f)
	{// ������0�ȉ��ɂȂ���
		Uninit();	// �I������
	}
	else
	{// �܂��\�����Ԃ�����
		// �ړ��ʂ�ω�������
		m_Move.x += (m_Move.x * m_ChangeMove.x);
		m_Move.y += (m_Move.y * m_ChangeMove.y);
		m_Move.z += (m_Move.z * m_ChangeMove.z);

		// ��������]������
		if (m_nRotPattern == 0)
		{// ���v���
			fRot -= m_fChangeRot;
		}
		else if (m_nRotPattern == 1)
		{// �����v���
			fRot += m_fChangeRot;
		}
		else if (m_nRotPattern == 2)
		{// �����_��
			if (rand() % 2 == 0)
			{// ���v���
				fRot += m_fChangeRot;
			}
			else
			{// �����v���
				fRot -= m_fChangeRot;
			}
		}
		if (fRot > D3DX_PI)
		{// �������~�����𒴂���
			fRot -= D3DX_PI * 2.0f;
		}
		if (fRot > D3DX_PI)
		{// �������~�����𒴂���
			fRot += D3DX_PI * 2.0f;
		}

		// �e��l�̐ݒ�
		SetPos(pos);          // ���W
		SetCol(col);          // �F
		SetRot(fRot);         // ����
		SetLength(fRadius);   // �傫��
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CParticle2D::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			if (m_bDrawAddtive == true)
			{// ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// �A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // �L����
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // �����x��0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���傫�����̂�`�悷��

			// ���ʂ̕`�揈��
			CScene2D::Draw();

			// �A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    �ړ��ʐݒ菈��
//=============================================================================
void CParticle2D::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    �ړ��ʂ̕ω��ʐݒ菈��
//=============================================================================
void CParticle2D::SetChangeMove(const D3DXVECTOR3 ChangeMove)
{
	m_ChangeMove = ChangeMove;
}

//=============================================================================
//    �F�̕ω��ʐݒ菈��
//=============================================================================
void CParticle2D::SetChangeCol(const D3DXCOLOR ChangeCol)
{
	m_ChangeCol = ChangeCol;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CParticle2D::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    ��]�p�^�[���ݒ菈��
//=============================================================================
void CParticle2D::SetRotPattern(const int nRotPattern)
{
	m_nRotPattern = nRotPattern;
}

//=============================================================================
//    �傫���̕ω��ʐݒ菈��
//=============================================================================
void CParticle2D::SetSpread(const float fSpread)
{
	m_fSpread = fSpread;
}

//=============================================================================
//    �����̕ω��ʐݒ菈��
//=============================================================================
void CParticle2D::SetChangeRot(const float fChangeRot)
{
	m_fChangeRot = fChangeRot;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����ݒ菈��
//=============================================================================
void CParticle2D::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    �ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CParticle2D::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �ړ��ʂ̕ω��ʎ擾����
//=============================================================================
D3DXVECTOR3 CParticle2D::GetChangeMove(void)
{
	return m_ChangeMove;
}

//=============================================================================
//    �F�̕ω��ʎ擾����
//=============================================================================
D3DXCOLOR CParticle2D::GetChangeCol(void)
{
	return m_ChangeCol;
}

//=============================================================================
//    �����擾����
//=============================================================================
int CParticle2D::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    ��]�p�^�[���擾����
//=============================================================================
int CParticle2D::GetRotPattern(void)
{
	return m_nRotPattern;
}

//=============================================================================
//    �傫���̕ω��ʎ擾����
//=============================================================================
float CParticle2D::GetSpread(void)
{
	return m_fSpread;
}

//=============================================================================
//    �����̕ω��ʎ擾����
//=============================================================================
float CParticle2D::GetChangeRot(void)
{
	return m_fChangeRot;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����擾����
//=============================================================================
bool CParticle2D::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}