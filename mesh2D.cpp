//*****************************************************************************
//
//     ���b�V��2D�|���S���̏���[mesh2D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mesh2D.h"
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
CMesh2D::CMesh2D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;                          // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;                          // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;	                        // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V��2D�|���S���̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ���b�V��2D�|���S���̐F
	m_fWidth = 0.0f;                            // 1�u���b�N���̕�
	m_fHeight = 0.0f;                           // 1�u���b�N������
	m_nXBlock = 0;                              // ���̕�����
	m_nYBlock = 0;                              // �c�̕�����
	m_nNumVertex = 0;                           // ���_��
	m_nNumIndex = 0;                            // �C���f�b�N�X��
	m_nNumPolygon = 0;                          // �|���S����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMesh2D::~CMesh2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMesh2D *CMesh2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nPriority)
{
	CMesh2D *pMesh2D = NULL;               // ���b�V��2D�|���S���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pMesh2D == NULL)
		{// ����������ɂȂ��Ă���
			pMesh2D = new CMesh2D(nPriority);
			if (pMesh2D != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pMesh2D->SetPos(pos);                   // ���b�V��2D�|���S���̍��W
				pMesh2D->SetCol(col);                   // ���b�V��2D�|���S���̐F
				pMesh2D->SetWidth(fWidth);              // 1�u���b�N���̕�
				pMesh2D->SetHeight(fHeight);            // 1�u���b�N���̍���
				pMesh2D->SetXBlock(nXBlock);            // ���̕�����
				pMesh2D->SetYBlock(nYBlock);            // �c�̕�����
				pMesh2D->SetTexSplitU(nTexSplitU);      // �e�N�X�`��U���W�̕�����
				pMesh2D->SetTexSplitV(nTexSplitV);      // �e�N�X�`��V���W�̕�����

				if (FAILED(pMesh2D->Init()))
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

	return pMesh2D;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMesh2D::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

			// ���_�o�b�t�@�̐���
			MakeVertex(pDevice);

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
		else
		{// �f�o�C�X���擾�ł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �����_�����O�N���X����������Ă��Ȃ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMesh2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pIdxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMesh2D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMesh2D::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

			// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetIndices(m_pIdxBuff);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			if (m_pTexture != NULL)
			{
				pDevice->SetTexture(0, m_pTexture);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CMesh2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �����ɕK�v�ȕϐ���錾
	D3DXCOLOR col = GetCol();
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
	float YPos = -(m_fHeight * m_nYBlock) / 2; // Y���W����ɐݒ�
	float fTexU = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
	float fTexV = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
		    // ���_���W
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f) + m_Pos;

			// �e�N�X�`���l
			pVtx[nCntH].rhw = 1.0f;

			// ���_�J���[
			pVtx[nCntH].col = col;

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

			XPos += m_fWidth;                              // X���W�����ɐi�߂�
			fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // �e�N�X�`��U���W��i�߂�
		}
		XPos -= m_fWidth * (m_nXBlock + 1);                               // X���W��i�߂Ă������߂�
		YPos += m_fHeight;                                                // Y���W�����ɐi�߂�
		fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // �e�N�X�`��U���W��i�߂Ă������߂�
		fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // �e�N�X�`��V���W��i�߂�

		// �|�C���^��i�߂�
		pVtx += m_nXBlock + 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CMesh2D::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// �c�̕��������J��Ԃ�
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// ���̕�����+�P�J��Ԃ�
		    // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
			pIdx[0] = nCntIdx + m_nXBlock + 1; // ����
			pIdx[1] = nCntIdx;                 // �㑤

			pIdx += 2;  // 2���i�߂�
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , �������c�̕������̍ŉ��w�ł͂Ȃ�
			 // 2 , ���̕��������ݒ肪�I�����
				pIdx[0] = nCntIdx;                       // �㑤
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // ���̉���

				pIdx += 2; // 2���i�߂�
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CMesh2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMesh2D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�ݒ菈��
//=============================================================================
void CMesh2D::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    ���b�V��2D�|���S���̍��W�ݒ菈��
//=============================================================================
void CMesh2D::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���b�V��2D�|���S���̐F�ݒ菈��
//=============================================================================
void CMesh2D::SetCol(const D3DXCOLOR col)
{
	// �F��ݒ�
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
			    // ���_�J���[
				pVtx[nCntH].col = m_Col;
			}
			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̕��ݒ菈��
//=============================================================================
void CMesh2D::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    1�u���b�N���̍����ݒ菈��
//=============================================================================
void CMesh2D::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMesh2D::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    �c�̕������ݒ菈��
//=============================================================================
void CMesh2D::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;
}

//=============================================================================
//    �e�N�X�`��U���W�̕���������
//=============================================================================
void CMesh2D::SetTexSplitU(const int nTexSplitU)
{
	m_nTexSplitU = nTexSplitU;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // �e�N�X�`��U���W

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].tex.x = fTexU;
				fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;
			}
			fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1);

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �e�N�X�`��V���W�̕������ݒ菈��
//=============================================================================
void CMesh2D::SetTexSplitV(const int nTexSplitV)
{
	m_nTexSplitV = nTexSplitV;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexV = 0.0f;   // �e�N�X�`��V���W

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].tex.y = fTexV;
			}
			fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMesh2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMesh2D::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CMesh2D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���b�V��2D�|���S���̍��W�擾����
//=============================================================================
D3DXVECTOR3 CMesh2D::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���b�V��2D�|���S���̐F�擾����
//=============================================================================
D3DXCOLOR CMesh2D::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1�u���b�N���̕��擾����
//=============================================================================
float CMesh2D::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1�u���b�N���̍����擾����
//=============================================================================
float CMesh2D::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMesh2D::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    �c�̕������擾����
//=============================================================================
int CMesh2D::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    �e�N�X�`��U���W�̕������擾����
//=============================================================================
int CMesh2D::GetTexSplitU(void)
{
	return m_nTexSplitU;
}

//=============================================================================
//    �e�N�X�`��V���W�̕������擾����
//=============================================================================
int CMesh2D::GetTexSplitV(void)
{
	return m_nTexSplitV;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMesh2D::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �C���f�b�N�X���擾����
//=============================================================================
int CMesh2D::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    �|���S�����擾����
//=============================================================================
int CMesh2D::GetNumPolygon(void)
{
	return m_nNumPolygon;
}