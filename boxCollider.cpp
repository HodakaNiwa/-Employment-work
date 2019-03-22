//*****************************************************************************
//
//     �����蔻��p�����f���̏���[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "boxCollider.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBoxCollider::CBoxCollider(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{// ���̒��_�̐������J��Ԃ�
		m_Vertex[nCntVer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vertex[nCntVer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	for (int nCntIndex = 0; nCntIndex < 24; nCntIndex++)
	{// �`��Ɏg�p����C���f�b�N�X���̐������J��Ԃ�
		m_wIndex[nCntIndex] = 0;
	}
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ����
	D3DXMatrixIdentity(&m_MtxWorld);          // ���[���h�}�g���b�N�X
	m_fWidth = 0.0f;                          // ���̉���
	m_fHeight = 0.0f;                         // ���̍���
	m_fDepth = 0.0f;                          // ���̉��s
	m_bReturnFlag = false;                    // ���������ۂɖ߂����ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag, int nPriority)
{
	CBoxCollider *pBoxCollider = NULL;      // �����蔻��p�����f���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pBoxCollider == NULL)
		{// ����������ɂȂ��Ă���
			pBoxCollider = new CBoxCollider(nPriority);
			if (pBoxCollider != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̐ݒ�
				pBoxCollider->SetPos(pos);                 // ���W
				pBoxCollider->SetWidth(fWidth);            // ���̉���
				pBoxCollider->SetHeight(fHeight);          // ���̍���
				pBoxCollider->SetDepth(fDepth);            // ���̉��s
				pBoxCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɖ߂����ǂ���

				if (FAILED(pBoxCollider->Init()))
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

	return pBoxCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBoxCollider::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���_�o�b�t�@�̐���
			MakeVertex();
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBoxCollider::Uninit(void)
{
	Release();  // ���g�̃I�u�W�F�N�g��j��
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBoxCollider::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBoxCollider::Draw(void)
{
#ifdef _DEBUG
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Culling;
			DWORD Lighting;

			// ���C�e�B���O���Ȃ��ݒ��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// �J�����O���O���Ă���
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// ���̕`��
			pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(BOX_VERTEX));

			// �J�����O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ���C�e�B���O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}

#endif
}

//=============================================================================
//    ���_��񐶐�����
//=============================================================================
void CBoxCollider::MakeVertex(void)
{
	// ���_���W��ݒ�(���Ԃ� �㑤�� ���� -> �E�� -> -> ���O -> �E��
	//                       ������ ���� -> �E�� -> -> ���O -> �E��)
	m_Vertex[0].pos = D3DXVECTOR3(0.0f, m_fHeight, m_fDepth);
	m_Vertex[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	m_Vertex[2].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	m_Vertex[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	m_Vertex[4].pos = D3DXVECTOR3(0.0f, 0.0f, m_fDepth);
	m_Vertex[5].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	m_Vertex[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Vertex[7].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);

	// ���_�J���[��ݒ�(�S�ĐԐF�ɐݒ�)
	m_Vertex[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// �C���f�b�N�X����ݒ�
	m_wIndex[0] = 0;
	m_wIndex[1] = 1;
	m_wIndex[2] = 1;
	m_wIndex[3] = 3;
	m_wIndex[4] = 3;
	m_wIndex[5] = 2;
	m_wIndex[6] = 2;
	m_wIndex[7] = 0;
	m_wIndex[8] = 0;
	m_wIndex[9] = 4;
	m_wIndex[10] = 1;
	m_wIndex[11] = 5;
	m_wIndex[12] = 2;
	m_wIndex[13] = 6;
	m_wIndex[14] = 3;
	m_wIndex[15] = 7;
	m_wIndex[16] = 4;
	m_wIndex[17] = 5;
	m_wIndex[18] = 5;
	m_wIndex[19] = 7;
	m_wIndex[20] = 7;
	m_wIndex[21] = 6;
	m_wIndex[22] = 6;
	m_wIndex[23] = 4;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CBoxCollider::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxScale, mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // �����������ǂ���

	// Y����̔���
	if (pPos->y < m_Pos.y + m_fHeight && pPos->y + ColRange.y > m_Pos.y)
	{// ���݂̍��������̒��ɂ���
		// X����̔���
		if (pPos->z + ColRange.z > m_Pos.z && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
		{// Z����ɂ��邩�ǂ�������
			if (pPosOld->x + ColRange.x <= m_Pos.x && pPos->x + ColRange.x > m_Pos.x)
			{// ���̍������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = m_Pos.x - ColRange.x;
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= m_Pos.x + m_fWidth && pPos->x - ColRange.x < m_Pos.x + m_fWidth)
			{// ���̉E�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = m_Pos.x + m_fWidth + ColRange.x;
				}
				bHit = true;
			}
		}
		// Z����̔���
		if (pPos->x + ColRange.x > m_Pos.x && pPos->x - ColRange.x < m_Pos.x + m_fWidth)
		{// X����ɂ��邩�ǂ�������
			if (pPosOld->z - ColRange.z >= m_Pos.z + m_fDepth && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
			{// ���̉������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = m_Pos.z + m_fDepth + ColRange.z;
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= m_Pos.z && pPos->z + ColRange.z > m_Pos.z)
			{// ���̎�O�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = m_Pos.z - ColRange.z;
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= m_Pos.y + m_fHeight && pPos->y < m_Pos.y + m_fHeight)
	{// �������ォ�甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > m_Pos.x && pPos->x - ColRange.x < m_Pos.x + m_fWidth
			&& pPos->z + ColRange.z > m_Pos.z && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = m_Pos.y + m_fHeight;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= m_Pos.y && pPos->y + ColRange.y > m_Pos.y)
	{// �����������甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > m_Pos.x && pPos->x - ColRange.x < m_Pos.x + m_fWidth
			&& pPos->z + ColRange.z > m_Pos.z && pPos->z - ColRange.z < m_Pos.z + m_fDepth)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = m_Pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    ���W�ݒ菈��
//=============================================================================
void CBoxCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CBoxCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CBoxCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���̉����ݒ菈��
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    ���̍����ݒ菈��
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ���̉��s�ݒ菈��
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    ���W�擾����
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �����擾����
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CBoxCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���̉����擾����
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ���̍����擾����
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̉��s�擾����
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}