//*****************************************************************************
//
//     ��̏���[river.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "river.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRiver::CRiver(int nPriority, OBJTYPE objType) : CMeshField(nPriority, objType)
{
	// �e��l�̃N���A
	m_fAddTexU = 0.0f;   // �e�N�X�`��U���W���ǂꂭ�炢��������
	m_fAddTexV = 0.0f;   // �e�N�X�`��V���W���ǂꂭ�炢��������
	m_fTexU = 0.0f;      // �e�N�X�`��U���W
	m_fTexV = 0.0f;      // �e�N�X�`��V���W
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRiver::~CRiver()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRiver *CRiver::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, float fAddTexU, float fAddTexV, char *pFileName, int nPriority)
{
	CRiver *pRiver = NULL;                 // ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pRiver == NULL)
		{// ����������ɂȂ��Ă���
			pRiver = new CRiver(nPriority);
			if (pRiver != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pRiver->SetPos(pos);                   // ���b�V���t�B�[���h�̍��W
				pRiver->SetRot(rot);                   // ���b�V���t�B�[���h�̌���
				pRiver->SetCol(col);                   // ���b�V���t�B�[���h�̐F
				pRiver->SetWidth(fWidth);              // 1�u���b�N���̕�
				pRiver->SetHeight(fHeight);            // 1�u���b�N���̍���
				pRiver->SetXBlock(nXBlock);            // ���̕�����
				pRiver->SetZBlock(nZBlock);            // ���s�̕�����
				pRiver->SetTexSplitU(nTexSplitU);      // �e�N�X�`��U���W�̕�����
				pRiver->SetTexSplitV(nTexSplitV);      // �e�N�X�`��V���W�̕�����
				pRiver->SetAddTexU(fAddTexU);          // �e�N�X�`��U���W���ǂꂭ�炢��������
				pRiver->SetAddTexV(fAddTexV);          // �e�N�X�`��V���W���ǂꂭ�炢��������
				pRiver->SetBinaryFileName(pFileName);  // ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����

				if (FAILED(pRiver->Init()))
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

	return pRiver;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRiver::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CMeshField::Init()))
	{
		return E_FAIL;
	}

	// �@���x�N�g���������������Ă���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < GetZBlock() + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
				pVtx[nCntH].col = GetCol();
			}
			// �|�C���^��i�߂�
			pVtx += GetXBlock() + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRiver::Uninit(void)
{
	// ���ʂ̏I������
	CMeshField::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRiver::Update(void)
{
	// �e�N�X�`�����W�𓮂���
	m_fTexU += m_fAddTexU;
	m_fTexV += m_fAddTexV;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // �e�N�X�`��U���W
		float fTexV = 0.0f;   // �e�N�X�`��V���W

		for (int nCntV = 0; nCntV < GetZBlock() + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				// �e�N�X�`�����W��ݒ�
				pVtx[nCntH].tex.x = fTexU + m_fTexU;
				pVtx[nCntH].tex.y = fTexV + m_fTexV;

				// �e�N�X�`��U���W�����炷
				fTexU += (1.0f / (GetXBlock())) * GetTexSplitU();
			}
			// �e�N�X�`��U���W�����炵�����߂�
			fTexU -= ((1.0f / (GetXBlock())) * GetTexSplitU()) * (GetXBlock() + 1);

			// �e�N�X�`��V���W��i�߂�
			fTexV += ((1.0f / GetZBlock()) * GetTexSplitV());

			// �|�C���^��i�߂�
			pVtx += GetXBlock() + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRiver::Draw(void)
{
	// ���ʂ̕`�揈��
	CMeshField::Draw();
}

//=============================================================================
//    �e�N�X�`��U���W���ǂꂭ�炢���������ݒ肷�鏈��
//=============================================================================
void CRiver::SetAddTexU(const float fAddTexU)
{
	m_fAddTexU = fAddTexU;
}

//=============================================================================
//    �e�N�X�`��V���W���ǂꂭ�炢���������ݒ肷�鏈��
//=============================================================================
void CRiver::SetAddTexV(const float fAddTexV)
{
	m_fAddTexV = fAddTexV;
}