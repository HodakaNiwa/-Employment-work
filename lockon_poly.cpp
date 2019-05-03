//*****************************************************************************
//
//     ���b�N�I���p�|���S���̏���[lockon_poly.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "lockon_poly.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLockOnPoly::CLockOnPoly(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLockOnPoly::~CLockOnPoly()
{

}

//=============================================================================
//    ��������
//=============================================================================
CLockOnPoly *CLockOnPoly::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, int nPriority)
{
	CLockOnPoly *pLockOnPoly = NULL;        // ���b�N�I���p�|���S���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();    // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pLockOnPoly == NULL)
		{// ����������ɂȂ��Ă���
			pLockOnPoly = new CLockOnPoly(nPriority);
			if (pLockOnPoly != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pLockOnPoly->Init(pos, col, fWidth, fHeight, fRot, bLighting)))
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

	return pLockOnPoly;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLockOnPoly::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting)
{
	// �e��l�̑��
	SetPos(pos);                    // �|���S���̍��W
	SetCol(col);                    // �|���S���̐F
	SetRot(fRot);                   // �|���S���̌���
	SetWidth(fWidth);               // �|���S���̕�
	SetHeight(fHeight);             // �|���S���̍���
	SetLighting(bLighting);         // ���C�e�B���O���邩���Ȃ���

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
void CLockOnPoly::Uninit(void)
{
	// ���ʂ̏I������
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLockOnPoly::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CLockOnPoly::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

			// ���ʂ̕`�揈��
			CSceneBillboard::Draw();

			// Z�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			if (CManager::GetGame()->GetPlayer() != NULL)
			{
				CManager::GetGame()->GetPlayer()->Draw();
			}
		}
	}
}