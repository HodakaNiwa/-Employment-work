//*****************************************************************************
//
//     �����̏���[tornade.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "tornade.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"
#include "model.h"
#include "emitter.h"
#include "effectManager.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define TORNADE_MOVE            (13.0f)           // �����̈ړ���
#define TORNADE_DIFF_ANGLE      (0.2f)            // �����̌�����␳�ł���{��
#define TORNADE_RIVISION_ANGLE  (D3DX_PI * 0.2f)  // �������ǂꂭ�炢������␳�ł��邩
#define TORNADE_MAX_LIFE        (70)              // �����̎����ő�l
#define TORNADE_MIN_LIFE        (50)              // �����̎����ŏ��l
#define TORNADE_COLLISION_RANGE (30.0f)           // �����̓����蔻�������͈�
#define TORNADE_DAMAGE          (3)               // �����̃_���[�W
#define TORNADE_UP              (15.0f)           // �����̃G�t�F�N�g���ǂꂾ���グ�邩

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTornade::CTornade(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_nEffectCounter = 0;                          // �G�t�F�N�g���o���^�C�~���O���Ǘ�����J�E���^�[
	m_pEffectManager = NULL;                       // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_State = STATE_HORMING;                       // ���
	m_nLife = 0;                                   // ����(�ǔ����I�������猸�炵�n�߂�)
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);           // ���W
	m_fAngle = 0.0f;                               // ���݂̌���
	m_fDestAngle = 0.0f;                           // �ړI�̌���
	m_fDiffAngle = 0.0f;                           // �ړI�̌����ƌ��݂̌����̍���
	m_fRivisionAngle = 0.0f;                       // �ǂꂾ��������␳�������ǂ���

	for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
	{// �G�t�F�N�g�𐶐����鐔�����J��Ԃ�
		m_pTornadeEmitter[nCntEffect] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTornade::~CTornade()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTornade *CTornade::Create(D3DXVECTOR3 pos, CEffectManager *pEffectManager, float fDestAngle, int nPriority)
{
	CTornade *pTornade = NULL;             // �����N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pTornade == NULL)
		{// ����������ɂȂ��Ă���
			pTornade = new CTornade(nPriority);
			if (pTornade != NULL)
			{// ���������m�ۂł���
			    // �e��l�̐ݒ�
				pTornade->SetPos(pos);                       // �����̍��W
				pTornade->SetAngle(fDestAngle);              // �����̌���
				pTornade->SetEffectManager(pEffectManager);  // �����̃G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^

				if (FAILED(pTornade->Init()))
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

	return pTornade;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTornade::Init(void)
{
	// �����̃G�t�F�N�g�𐶐�����
	if (m_pEffectManager != NULL)
	{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾�ł��Ă���
		D3DXVECTOR3 EffectPos = D3DXVECTOR3(m_Pos.x, m_Pos.y + 10.0f, m_Pos.z);
		for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
		{// �G�t�F�N�g�𐶐����鐔�����J��Ԃ�
			m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33 + nCntEffect);
			EffectPos.y += TORNADE_UP;
		}
	}

	// �������v�Z�ŏo��
	m_nLife = rand() % (TORNADE_MAX_LIFE - TORNADE_MIN_LIFE) + TORNADE_MIN_LIFE;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTornade::Uninit(void)
{
	// �G�~�b�^�̊J��
	for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
	{// �G�t�F�N�g�𐶐����鐔�����J��Ԃ�
		if (m_pTornadeEmitter[nCntEffect] != NULL)
		{// �G�~�b�^����������Ă���
			m_pTornadeEmitter[nCntEffect]->Uninit();
			m_pTornadeEmitter[nCntEffect] = NULL;
		}
	}

	// �����̏�������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTornade::Update(void)
{
	m_nEffectCounter++;
	if (m_nEffectCounter % 1 == 0)
	{// �G�t�F�N�g���o���^�C�~���O�ɂȂ���
	    // �����̃G�t�F�N�g�𐶐�����
		if (m_pEffectManager != NULL)
		{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾�ł��Ă���
			D3DXVECTOR3 EffectPos = D3DXVECTOR3(m_Pos.x, m_Pos.y + 10.0f, m_Pos.z);
			for (int nCntEffect = 0; nCntEffect < TORNADE_EFFECT_NUM; nCntEffect++)
			{// �G�t�F�N�g�𐶐����鐔�����J��Ԃ�
				m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33 + nCntEffect);
				EffectPos.y += TORNADE_UP;
			}
		}
	}

	// �v���C���[�N���X���擾����
	CPlayer *pPlayer = NULL;
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			pPlayer = pGame->GetPlayer();
		}
	}
	if (m_State == STATE_HORMING)
	{// �ǔ������ԂȂ��
		if (pPlayer != NULL)
		{// �v���C���[���擾�ł���
			// �����̌v�Z���s��
			m_fDestAngle = atan2f(pPlayer->GetPos().x - m_Pos.x, pPlayer->GetPos().z - m_Pos.z);
		}

		// �����̌��݂̌����ƖړI�̌����̍������v�Z
		m_fDiffAngle = m_fDestAngle - m_fAngle;
		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI * 2.0f;
		}

		// �����̌�����␳����
		m_fAngle += m_fDiffAngle * TORNADE_DIFF_ANGLE;
		if (m_fAngle > D3DX_PI)
		{
			m_fAngle -= D3DX_PI * 2.0f;
		}
		if (m_fAngle < -D3DX_PI)
		{
			m_fAngle += D3DX_PI * 2.0f;
		}

		// �������ړ�������
		m_Pos.x += sinf(m_fAngle) * TORNADE_MOVE;
		m_Pos.z += cosf(m_fAngle) * TORNADE_MOVE;

		float fRivision = sqrtf((m_fDiffAngle * TORNADE_DIFF_ANGLE) * (m_fDiffAngle * TORNADE_DIFF_ANGLE));
		m_fRivisionAngle += fRivision;
		if (m_fRivisionAngle >= TORNADE_RIVISION_ANGLE)
		{// �␳���������̗ʂ������Ȃ肷����
			m_State = STATE_NONE_HORMING;
		}

		if (pPlayer != NULL)
		{// �v���C���[���擾�ł���
			// �����蔻����s��
			Collision(pPlayer);
		}
	}
	else if(m_State == STATE_NONE_HORMING)
	{// �ǔ����Ȃ���ԂȂ��
	    // �����̌��݂̌����ƖړI�̌����̍������v�Z
		m_fDiffAngle = m_fDestAngle - m_fAngle;
		if (m_fDiffAngle > D3DX_PI)
		{
			m_fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (m_fDiffAngle < -D3DX_PI)
		{
			m_fDiffAngle += D3DX_PI * 2.0f;
		}

	    // �����̌�����␳����
		m_fAngle += m_fDiffAngle * TORNADE_DIFF_ANGLE;
		if (m_fAngle > D3DX_PI)
		{
			m_fAngle -= D3DX_PI * 2.0f;
		}
		if (m_fAngle < -D3DX_PI)
		{
			m_fAngle += D3DX_PI * 2.0f;
		}

	    // �������ړ�������
		m_Pos.x += sinf(m_fAngle) * TORNADE_MOVE;
		m_Pos.z += cosf(m_fAngle) * TORNADE_MOVE;

	    // �����蔻����s��
		Collision(pPlayer);

		// �������ւ炷
		m_nLife--;
		if (m_nLife <= 0.0f)
		{// �������Ȃ��Ȃ���
			Uninit();
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTornade::Draw(void)
{

}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CTornade::Collision(CPlayer *pPlayer)
{
	bool bHit = false;                 // �U�����q�b�g�������ǂ���
	float fPlayerRange = 0.0f;         // �v���C���[�̓����蔻������͈�
	D3DXVECTOR3 OffSet = m_Pos;        // �����蔻����Ƃ�ꏊ
	float fHitRange = 0.0f;            // �����蔻�����钷��
	float fLength = 0.0f;              // �����蔻������ꏊ�ƓG�Ƃ̋���

	// ����J�n
	if (pPlayer != NULL)
	{// �v���C���[�N���X�ւ̃|�C���^���擾�ł��Ă���
	    // �v���C���[�̓����蔻������͈͂��擾
		fPlayerRange = pPlayer->GetColRange();

		// �����蔻�����钷�����v�Z
		fHitRange = sqrtf((fPlayerRange + TORNADE_COLLISION_RANGE) * (fPlayerRange + TORNADE_COLLISION_RANGE));

		// �����蔻��������W�ƓG�̍��W�Ƃ̋����𑪂�
		D3DXMATRIX PlayerMtxWorld;
		D3DXVECTOR3 PlayerPos;
		CModel *pModel = pPlayer->GetModel(0);
		if (pModel != NULL)
		{
			PlayerMtxWorld = pModel->GetMtxWorld();
			PlayerPos = D3DXVECTOR3(PlayerMtxWorld._41, PlayerMtxWorld._42, PlayerMtxWorld._43);
			fLength = sqrtf((PlayerPos.x - OffSet.x) * (PlayerPos.x - OffSet.x) + (PlayerPos.y - OffSet.y) * (PlayerPos.y - OffSet.y) + (PlayerPos.z - OffSet.z) * (PlayerPos.z - OffSet.z));
		}

		if (fLength <= fHitRange)
		{// �����蔻��������W�ƓG�̍��W�Ƃ̋����������蔻�����钷�����Z��
			if (pPlayer->GetState() != CPlayer::STATE_DAMAGE)
			{// �v���C���[�̏�Ԃ��_���[�W��ԂłȂ�
				pPlayer->Damage(TORNADE_DAMAGE);
				m_State = STATE_NONE_HORMING;
				bHit = true;
			}
		}
	}

	return bHit;
}

//=============================================================================
//    ���W��ݒ肷�鏈��
//=============================================================================
void CTornade::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
void CTornade::SetAngle(const float fAngle)
{
	m_fAngle = fAngle;
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CTornade::SetEffectManager(CEffectManager *pEffectManager)
{
	m_pEffectManager = pEffectManager;
}

//=============================================================================
//    ���W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CTornade::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �������擾���鏈��
//=============================================================================
float CTornade::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEffectManager *CTornade::GetEffectManager(void)
{
	return m_pEffectManager;
}