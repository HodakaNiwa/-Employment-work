//*****************************************************************************
//
//     �e�̏���[shadow.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SHADOW_PRIORITY       (3)                        // �e�̏����D�揇��
#define SHADOW_MODEL_FILENAME "data/MODEL/shadow000.x"   // �e���f���̃t�@�C����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH CShadow::m_pMesh = NULL;       // ���b�V���ւ̃|�C���^
LPD3DXBUFFER CShadow::m_pBuffMat = NULL;  // �}�e���A�����ւ̃|�C���^
DWORD CShadow::m_nNumMat = 0;             // �}�e���A�����̐�

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CShadow::CShadow(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
//    ��������
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CShadow *pShadow = NULL;  // ���f��(�V�[���h��)�N���X�^�̃|�C���^
	if (pShadow == NULL)
	{// ����������ɂȂ��Ă���
		pShadow = new CShadow(SHADOW_PRIORITY);
		if (pShadow != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pShadow->Init(pos, rot, scale)))
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

	return pShadow;
}

//=============================================================================
//    �e���f���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CShadow::Load(void)
{
	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(SHADOW_MODEL_FILENAME,
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=============================================================================
//    �e���f���̊J������
//=============================================================================
void CShadow::UnLoad(void)
{
	// ���b�V���̊J��
	DIRECT_RELEASE(m_pMesh);

	// �}�e���A�����̊J��
	DIRECT_RELEASE(m_pBuffMat);
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// �e��l�̐ݒ�
	SetPos(pos);      // ���W
	SetRot(rot);      // ����
	SetScale(scale);  // �傫���̔{��

	 // ���f���̐ݒ�
	BindModel(m_pMesh, m_pBuffMat, m_nNumMat, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CShadow::Uninit(void)
{
	// ���ʂ̏I������
	CSceneX::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CShadow::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	// �����_�����O�N���X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// �X�e���V���o�b�t�@�ɏ������ސݒ��
			pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

			// Z�o�b�t�@�ɏ������܂Ȃ��ݒ��
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			// �F���������܂Ȃ��ݒ��
			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

			// �X�e���V���}�X�N�l�̐ݒ�
			pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

			// �X�e���V���e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);        // �K���X�e���V���e�X�g�ɍ��i����
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);    // �����̃e�X�g�ɍ��i������
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);   // Z�e�X�g�ɂ̂ݎ��s�����ꍇ
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);    // �������s�����ꍇ

			// �\�ʂ��X�e���V���o�b�t�@�ɕ`��
			CSceneX::Draw();

			// �J�����O�̐ݒ�𔽑΂ɂ���(�\�ʃJ�����O)
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

			// �Q�ƒl��ݒ肷��
			pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);                   // �X�e���V���e�X�g�̂��߂̎Q�ƒl
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);     // �Q�ƒl���Ⴂ���̂��X�e���V���e�X�g�����i�Ƃ���
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);    // �����̃e�X�g�ɍ��i������
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);   // Z�e�X�g�ɂ̂ݎ��s�����ꍇ
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);    // �X�e���V���e�X�g�Ɏ��s�����ꍇ

			// ���ʂ��X�e���V���o�b�t�@�ɕ`��
			CSceneX::Draw();

			// �J�����O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);             // ���ʃJ�����O

			// �F���������ސݒ��
			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);

			// �������|���S������ʂ̍őO�ʂɕ`�悵�X�e���V���o�b�t�@�̒l�Ńe�X�g���؂���(�؂������������e�ƂȂ�)
			pDevice->SetRenderState(D3DRS_STENCILREF, 0x2);                   // �X�e���V���e�X�g�̂��߂̎Q�ƒl
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);     // �Q�ƒl�ȉ���`�悵�Ȃ�
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);    // �������i�����ꍇ
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);   // Z�e�X�g�Ɏ��s�����ꍇ
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);    // �X�e���V���e�X�g�Ɏ��s�����ꍇ

			// �|���S���`��p
			struct SHADOW_POLYGON
			{
				D3DXVECTOR4 VertexPos;
				D3DCOLOR VertexCol;
			};
			SHADOW_POLYGON ShadowPolygon[4];

			// ���_���W�̐ݒ�
			ShadowPolygon[0].VertexPos = D3DXVECTOR4(0, 0, 0, 1);
			ShadowPolygon[1].VertexPos = D3DXVECTOR4(SCREEN_WIDTH, 0, 0, 1);
			ShadowPolygon[2].VertexPos = D3DXVECTOR4(0, SCREEN_HEIGHT, 0, 1);
			ShadowPolygon[3].VertexPos = D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1);

			// ���_�J���[�̐ݒ�
			ShadowPolygon[0].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			ShadowPolygon[1].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			ShadowPolygon[2].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
			ShadowPolygon[3].VertexCol = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

			// �|���S���`��
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, ShadowPolygon, sizeof(SHADOW_POLYGON));

			// �����_�����O�X�e�[�g�����ɖ߂�
			pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);   // �X�e���V���e�X�g�����Ȃ��ݒ��
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);          // Z�o�b�t�@���g�p����ݒ��
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	   // Z�o�b�t�@�ɏ������ސݒ��

			// �X�e���V���o�b�t�@���N���A���Ă���
			pDevice->Clear(0, NULL, (D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);
		}
	}
}