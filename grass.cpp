//*****************************************************************************
//
//     ���̏���[grass.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "grass.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define GRASS_COLLISION_RANGE (12.0f)                           // ���Ƃ̓����蔻������͈�
#define GRASS_SWAY_POWER      (D3DXVECTOR3(0.15f, 0.0f, 0.0f))  // ���̗h����

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGrass::CGrass(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_aOffsetVertex[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aOffsetVertex[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BonePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BoneRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_State = STATE_NORMAL;
	m_nStateCounter = 0;
	m_SwayMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SwayMoveCurrent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRivisionSway = 0.0f;
	m_nSwayCounter = 0;
	m_nMaxSwayCounter = 0;
	m_bSway = false;
	m_fCameraLength = 0.0f;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGrass::~CGrass()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGrass *CGrass::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, bool bSway, int nPriority)
{
	CGrass *pGrass = NULL;                 // ���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGrass == NULL)
		{// ����������ɂȂ��Ă���
			pGrass = new CGrass(nPriority);
			if (pGrass != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGrass->SetPos(pos);         // �|���S���̍��W
				pGrass->SetRot(rot);         // �|���S���̌���
				pGrass->SetCol(col);         // �|���S���̐F
				pGrass->SetWidth(fWidth);    // �|���S���̕�
				pGrass->SetHeight(fHeight);  // �|���S���̍���
				pGrass->SetSway(bSway);      // ����ɗh��邩�ǂ���

				if (FAILED(pGrass->Init()))
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

	return pGrass;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGrass::Init(void)
{
	m_nStateCounter = 250;

	m_nMaxSwayCounter = rand() % 150 + 150;
	m_nSwayCounter = m_nMaxSwayCounter / 2;
	m_SwayMove = GRASS_SWAY_POWER;

	if (FAILED(CScene3D::Init()))
	{// �������Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGrass::Uninit(void)
{
	// ���ʂ̏I������
	CScene3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGrass::Update(void)
{
	if (m_State == STATE_COL_LEFT)
	{// ���ɗh���Ȃ�
		m_BonePos.x += (20.0f - m_BonePos.x) * 0.07f;
		m_BonePos.y += (8.0f - m_BonePos.y) * 0.07f;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
		if (pVtxBuff != NULL)
		{// ���_�o�b�t�@����������Ă���
		    // ���_���̐ݒ�
			VERTEX_3D *pVtx;

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			D3DXVECTOR3 Pos;

			for (int nCntVer = 0; nCntVer < 2; nCntVer++)
			{// ���_�������J��Ԃ�
				D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

				// ���_���W
				pVtx[nCntVer].pos = Pos;
			}

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();
		}
	}
	else if (m_State == STATE_COL_RIGHT)
	{// �E�ɗh���Ȃ�
		m_BonePos.x += (-20.0f - m_BonePos.x) * 0.07f;
		m_BonePos.y += (8.0f - m_BonePos.y) * 0.07f;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
		if (pVtxBuff != NULL)
		{// ���_�o�b�t�@����������Ă���
		    // ���_���̐ݒ�
			VERTEX_3D *pVtx;

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			D3DXVECTOR3 Pos;

			for (int nCntVer = 0; nCntVer < 2; nCntVer++)
			{// ���_�������J��Ԃ�
				D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

				// ���_���W
				pVtx[nCntVer].pos = Pos;
			}

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();
		}
	}
	else if (m_State == STATE_NORMAL)
	{// �Ȃɂ����Ȃ�
		if (m_bSway == true)
		{// �h��鑐�Ȃ�
			if (rand() % 20 == 0)
			{
				D3DXVECTOR3 CameraPosV = CManager::GetCamera()->GetPosV();
				m_fCameraLength = sqrtf((GetPos().x - CameraPosV.x) * (GetPos().x - CameraPosV.x) + (GetPos().z - CameraPosV.z) * (GetPos().z - CameraPosV.z));
			}

			if (m_fCameraLength <= 1000.0f)
			{// �J�����Ƃ̋��������ł���
				m_nSwayCounter++;
				m_fRivisionSway += D3DX_PI / m_nMaxSwayCounter;
				if (m_nSwayCounter % m_nMaxSwayCounter == 0)
				{// �Ǘ��J�E���^�[�����̒l�ɒB����
					m_fRivisionSway = 0.0f;
					m_SwayMove *= -1;
				}

				m_SwayMoveCurrent.x = sinf(m_fRivisionSway) * m_SwayMove.x;
				m_SwayMoveCurrent.y = sinf(m_fRivisionSway) * m_SwayMove.y;
				m_SwayMoveCurrent.z = sinf(m_fRivisionSway) * m_SwayMove.z;
				m_BonePos += m_SwayMoveCurrent;

				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
				if (pVtxBuff != NULL)
				{// ���_�o�b�t�@����������Ă���
					// ���_���̐ݒ�
					VERTEX_3D *pVtx;

					// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					D3DXVECTOR3 Pos;

					for (int nCntVer = 0; nCntVer < 2; nCntVer++)
					{// ���_�������J��Ԃ�
						D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

						// ���_���W
						pVtx[nCntVer].pos = Pos;
					}

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
void CGrass::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Culling;

			// �J�����O���Ȃ��ݒ��
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		    // �A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // �L����
			pDevice->SetRenderState(D3DRS_ALPHAREF, 140);             // �����x��0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���傫�����̂�`�悷��

			// ���ʂ̏���
			CScene3D::Draw();

			// �A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// �J�����O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CGrass::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CScene3D::SetMtxWorld(mtxWorld);

	// �{�[���̃��[���h�}�g���b�N�X���쐬
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_BoneMtxWorld);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_BonePos.x, m_BonePos.y, m_BonePos.z);
	D3DXMatrixMultiply(&m_BoneMtxWorld, &m_BoneMtxWorld, &mtxTrans);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGrass::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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

	// �I�t�Z�b�g�̒��������߂�
	float fLength = 0.0f;
	float fAngle = 0.0f;
	fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));

	// �I�t�Z�b�g�̊p�x�����߂�
	fAngle = atan2f(GetWidth(), GetHeight());

	// �{�[���̃I�t�Z�b�g��ݒ�
	m_BonePos = D3DXVECTOR3(0.0f, cosf(fAngle) * fLength, 0.0f);
	m_BonePosDef = m_BonePos;

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, cosf(fAngle) * fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * fLength, cosf(fAngle) * fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 0.0f);

	// �I�t�Z�b�g��ݒ�
	m_aOffsetVertex[0] = pVtx[0].pos - m_BonePos;
	m_aOffsetVertex[1] = pVtx[1].pos - m_BonePos;

	// �@������
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	// ���_�o�b�t�@�̐ݒ�
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CGrass::Collision(D3DXVECTOR3 *pPos, float fColRange)
{
	bool bCol = false;       // �������Ă��邩�ǂ���
	float fHitRange = 0.0f;  // �������������Ԃ��͈�
	float fLength = 0.0f;    // �r���{�[�h�̍��W�ƈ����̍��W�Ƃ̋���

	// �����蔻�����钷�����v�Z
	fHitRange = sqrtf((fColRange + GRASS_COLLISION_RANGE) * (fColRange + GRASS_COLLISION_RANGE));

	// �r���{�[�h�ƈ����̍��W�Ƃ̋������v�Z
	fLength = sqrtf((GetPos().x - pPos->x) * (GetPos().x - pPos->x) + (GetPos().y - pPos->y) * (GetPos().y - pPos->y) + (GetPos().z - pPos->z) * (GetPos().z - pPos->z));

	// ����J�n
	if (fLength <= fHitRange)
	{// �r���{�[�h�̍��W�ƈ����̍��W�Ƃ̋����������蔻�����钷�����Z��
		bCol = true;   // ������������ɂ���
		float fAngle = atan2f(pPos->x - GetPos().x, pPos->z - GetPos().z) + GetRot().y;
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}
		if (fAngle < 0.0f)
		{
			if (m_State != STATE_COL_LEFT)
			{
				m_nStateCounter = 0;
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
				m_State = STATE_COL_LEFT;
			}
		}
		else if(fAngle > 0.0f)
		{
			if (m_State != STATE_COL_RIGHT)
			{
				m_nStateCounter = 0;
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GRASS);
				m_State = STATE_COL_RIGHT;
			}
		}
	}
	else
	{
		if (m_nStateCounter <= 200)
		{
			m_nStateCounter++;
			if (m_State != STATE_NORMAL)
			{
				m_State = STATE_NORMAL;
				m_nStateCounter = 0;
			}
			m_BonePos += (m_BonePosDef - m_BonePos) * 0.08f;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
			if (pVtxBuff != NULL)
			{// ���_�o�b�t�@����������Ă���
			    // ���_���̐ݒ�
				VERTEX_3D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				D3DXVECTOR3 Pos;

				for (int nCntVer = 0; nCntVer < 2; nCntVer++)
				{// ���_�������J��Ԃ�
					D3DXVec3TransformCoord(&Pos, &m_aOffsetVertex[nCntVer], &m_BoneMtxWorld);

					// ���_���W
					pVtx[nCntVer].pos = Pos;
				}

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
		}
	}

	return bCol;
}


//=============================================================================
//    ����ɗh��邩�ǂ����ݒ肷�鏈��
//=============================================================================
void CGrass::SetSway(const bool bSway)
{
	m_bSway = bSway;
}

//=============================================================================
//    ����ɗh��邩�ǂ����擾���鏈��
//=============================================================================
bool CGrass::GetSway(void)
{
	return m_bSway;
}