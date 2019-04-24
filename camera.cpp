//*****************************************************************************
//
//     �J�����̏���[camera.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "player.h"
#include "object.h"
#include "meshField.h"
#include "meshWall.h"
#include "scene.h"
#include "game.h"
#include "tutorial.h"
#include "title.h"
#include "map.h"
#include "enemy.h"
#include "effectManager.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// ����p�J�����̃}�N��
#define CAMERA_MOVIE_MODE            (false)                 // ����p�̃J�����ɐ؂�ւ���(�����ő���ł���J�����ɂ���)
#define CAMERA_MOVIE_MOVE            (6.0f)                  // ����p�J�����̈ړ���

// �J�����N�����}�N��
#define CAMERA_ANGEL_VIEW            (45.0f)                 // �J�����̉�p
#define CAMERA_ZNER                  (10.0f)                 // �J�����̈�ԋ߂�������
#define CAMERA_ZFAR                  (25000.0f)              // �J�����̈�ԉ���������
#define CAMERA_ANGLE_SPEED           (0.01f)                 // �J�������}�E�X�ŉ�]������X�s�[�h�{��
#define CAMERA_MOVE_SPEED            (1.3f)                  // �J�������}�E�X�ňړ�������X�s�[�h�{��
#define CAMERA_LENGTH_MIN            (40.0f)                 // ���_�����_�̋����̍ŏ��l
#define CAMERA_LENGTH_MAX            (400.0f)                // ���_�����_�̋����̍ő�l
#define CAMERA_LENGTH_INI            (200.0f)                // �J���������̏����l
#define CAMERA_POSY_UP               (70.0f)                 // �J�����̍������グ��␳�l
#define CAMERA_POSY_RANGE            (300.0f)                // �J����Y���W�͈̔�
#define CAMERA_POSY_MIN              (50.0f)                 // �J����Y���W�̍ŏ��l

// �Q�[�����̃J�����p�}�N��
#define CAMERA_ANGLEY_MIN            (-0.8f)                 // �J�����c�ړ��̊p�x�ŏ��l
#define CAMERA_ANGLEY_MAX            (0.4f)                  // �J�����c�ړ��̊p�x�ő�l
#define CAMERA_MOVESPEED_XZ          (0.2f)                  // �J�����𓮂��������̈ړ��X�s�[�h(XZ����)
#define CAMERA_MOVESPEED_Y           (0.2f)                  // �J�����𓮂��������̈ړ��X�s�[�h(�c����)
#define CAMERA_ROLLSPEED_Z           (0.01f)                 // �J�����̌����𓮂��������̈ړ��X�s�[�h(Z��)
#define CAMERA_ROLLSPEED_Y           (0.025f)                // �J�����̌����𓮂��������̈ړ��X�s�[�h(Y��)
#define CAMERA_ROT_RIVISION_X        (0.45f)                 // �J�����̌�����␳����{��(X��)
#define CAMERA_ROT_RIVISION_Y        (0.45f)                 // �J�����̌�����␳����{��(Y��)
#define CAMERA_ROT_RIVISION_Z        (0.45f)                 // �J�����̌�����␳����{��(Z��)

// �J�����̃��b�N�I���p�}�N��
#define CAMERA_LOCKON_POSV_UP        (70.0f)                 // �J�����̃��b�N�I���Œ��Ɏ��_�̍������グ��␳�l
#define CAMERA_LOCKON_POSR_UP        (30.0f)                 // �J�����̃��b�N�I���Œ��ɒ����_�̍������グ��␳�l
#define CAMERA_LOCKPOSV_MOVESPEED_XZ (0.09f)                 // �J�����̎��_�����b�N�I�����Ă��鎞�ɓ����������̈ړ��X�s�[�h(XZ����)
#define CAMERA_LOCKPOSV_MOVESPEED_Y  (0.09f)                 // �J�����̎��_�����b�N�I�����Ă��鎞�ɓ����������̈ړ��X�s�[�h(�c����)
#define CAMERA_LOCKPOSR_MOVESPEED_XZ (0.05f)                 // �J�����̒����_�����b�N�I�����Ă��鎞�ɓ����������̈ړ��X�s�[�h(XZ����)
#define CAMERA_LOCKPOSR_MOVESPEED_Y  (0.05f)                 // �J�����̒����_�����b�N�I�����Ă��鎞�ɓ����������̈ړ��X�s�[�h(�c����)
#define CAMERA_LOCK_ROTRIVISION_X    (1.0f)                  // ���b�N�I�����ɃJ�����̌�����␳����{��(X��)
#define CAMERA_LOCK_ROTRIVISION_Y    (1.0f)                  // ���b�N�I�����ɃJ�����̌�����␳����{��(Y��)
#define CAMERA_LOCK_ROTRIVISION_Z    (1.0f)                  // ���b�N�I�����ɃJ�����̌�����␳����{��(Z��)
#define CAMERA_POSV_FIELDUP          (50.0f)                 // �J�����̍�����n�ʂ���ǂꂾ��������

// �ړ��{���}�N��
#define CAMERA_ANGLE_MOVERIVISION_X  (0.000024f)             // �J�������Q�[���p�b�h�œ��������Ƃ��̉��ړ��̔{��
#define CAMERA_ANGLE_MOVERIVISION_Y  (0.000017f)             // �J�������Q�[���p�b�h�œ��������Ƃ��̏c�ړ��̔{��
#define CAMERA_GAME_LENGTH	         (230.0f)                // �Q�[�����̃J�����̋���
#define CAMERA_LOCKON_LENGTH         (230.0f)                // ���b�N�I�����̃Q�[�����̃J�����̋���

// �^�C�g���̃J�����p�}�N��
#define CAMERA_TITLE_LENGTH_0         (300.0f)               // �^�C�g���̎��_�����_�Ԃ̋���
#define CAMERA_TITLE_POSYUP_0         (70.0f)                // �^�C�g���̎��_�����_��(Y����)�̍�
#define CAMERA_TITLEPOSR_MOVESPEED_XZ (0.003f)               // �J�����̒����_���^�C�g�����ɓ����������̈ړ��X�s�[�h(XZ����)
#define CAMERA_TITLEPOSR_MOVESPEED_Y  (0.003f)               // �J�����̒����_���^�C�g�����ɓ����������̈ړ��X�s�[�h(�c����)

// �}�b�v�J�����p�}�N��
#define MAPCAMERA_VIEW_WIDTH         (SCREEN_WIDTH * 2.0f)   // �}�b�v�J�����̕`��͈͂̕�
#define MAPCAMERA_VIEW_HEIGHT        (SCREEN_HEIGHT * 2.0f)  // �}�b�v�J�����̕`��͈͂̍���
#define MAPCAMERA_ZNER               (10.0f)                 // �}�b�v�J�����̈�ԋ߂�������
#define MAPCAMERA_ZFAR               (10000.0f)              // �}�b�v�J�����̈�ԉ���������
#define MAPCAMERA_POSV_Y             (5000)                  // �}�b�v�J������Y����̎��_�ʒu
#define MAPCAMERA_VIEWPORT_X         (975)                   // �}�b�v�J�����̕`��̈捶��X���W
#define MAPCAMERA_VIEWPORT_Y         (5)                     // �}�b�v�J�����̕`��̈捶��Y���W
#define MAPCAMERA_VIEWPORT_WIDTH     (300)                   // �}�b�v�J�����̕`��̈�̕�
#define MAPCAMERA_VIEWPORT_HEIGHT    (200)                   // �}�b�v�J�����̕`��̈�̍���

//*****************************************************************************
//    CCamera�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	// �e��l�̃N���A
	m_pPlayer = NULL;                           // �v���C���[�N���X�ւ̃|�C���^
	m_pLandScene = NULL;                        // ����J����������Ă���I�u�W�F�N�g�ւ̃|�C���^
	m_pLandSceneOld = NULL;                     // �O��J����������Ă���I�u�W�F�N�g�ւ̃|�C���^
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���݂̎��_
	m_PosVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړI�̒����_
	m_PosVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���݂̎��_�ƖړI�̒����_�̍���
	m_PosVOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �O��̎n�_
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���݂̒����_
	m_PosRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړI�̒����_
	m_PosRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���݂̒����_�ƖړI�̒����_�̍���
	m_MoveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���_�̈ړ���
	m_MoveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �����_�̈ړ���
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���݂̌���
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړI�̌���
	m_RotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړI�̌����ƌ��݂̌����̍���
	m_RotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �O��̌���
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ������x�N�g��
	D3DXMatrixIdentity(&m_MtxProjection);       // �v���W�F�N�V�����}�g���b�N�X
	D3DXMatrixIdentity(&m_MtxView);             // �r���[�}�g���b�N�X
	m_fLength = 0.0f;                           // ����
	m_bMoving = false;                          // �J�����𓮂����邩�ǂ���
	m_ViewPort.X = 0;                           // �`��̈捶��X���W
	m_ViewPort.Y = 0;                           // �`��̈捶��Y���W
	m_ViewPort.Width = 0;                       // �`��̈�̕�
	m_ViewPort.Height = 0;                      // �`��̈�̍���
	m_ViewPort.MinZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ŏ��l
	m_ViewPort.MaxZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ő�l
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	// �e��l�̏�����
	m_PosV = D3DXVECTOR3(0.0f, 300.0f, -600.0f); // ���_
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �����_
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // ����
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // ������x�N�g��
	m_bMoving = CAMERA_MOVIE_MODE;               // �J�����������ő���ł��邩�ǂ���

	// ���_�ƒ����_�Ƃ̋������v�Z
	m_fLength = sqrtf((m_PosV.x - m_PosR.x) * (m_PosV.x - m_PosR.x) + (m_PosV.y - m_PosR.y) * (m_PosV.y - m_PosR.y) + (m_PosV.z - m_PosR.z) * (m_PosV.z - m_PosR.z));
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
	m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;

	// �r���[�|�[�g�ݒ�
	m_ViewPort.X = 0;                           // �`��̈捶��X���W
	m_ViewPort.Y = 0;                           // �`��̈捶��Y���W
	m_ViewPort.Width = SCREEN_WIDTH;            // �`��̈�̕�
	m_ViewPort.Height = SCREEN_HEIGHT;          // �`��̈�̍���
	m_ViewPort.MinZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ŏ��l
	m_ViewPort.MaxZ = 1.0f;                     // �`��̈�[�x�o�b�t�@�ő�l

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CCamera::Update(void)
{

#if _DEBUG  // �f�o�b�O���̏���
	if (GetUpdate() == false)
	{// �X�V����������
		CInputMouse *pMouse = CManager::GetMouse();

		if (pMouse != NULL)
		{// �}�E�X���擾�ł���
			if (pMouse->GetMouseAxisZ() != 0)
			{// �}�E�X�̃z�C�[���{�^�����X�N���[�����ꂽ
				m_fLength += 0.1f * pMouse->GetMouseAxisZ();
				if (m_fLength < CAMERA_LENGTH_MIN)
				{// ���������̒l���߂��Ȃ���
					m_fLength = CAMERA_LENGTH_MIN;
				}
				else if (m_fLength > CAMERA_LENGTH_MAX)
				{// ���������̒l��艓���Ȃ���
					m_fLength = CAMERA_LENGTH_MAX;
				}

				// ���_�̌v�Z
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}

			if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// �}�E�X�̍��{�^���ƉE�{�^���������ɉ����ꂽ
				if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
				{// �}�E�X����������Ă���
					// �����ɉ����ꂽ�Ƃ��̈ʒu���獡�̃}�E�X�J�[�\���̈ʒu���王�_�����_���ړ�������������v�Z
					float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
					fAngle -= m_Rot.y;
					if (fAngle < -D3DX_PI)
					{// �������~�����𒴂���
						fAngle += D3DX_PI * 2.0f;
					}
					if (fAngle > D3DX_PI)
					{// �������~�����𒴂���
						fAngle -= D3DX_PI * 2.0f;
					}

					// �}�E�X��X��,Y������ړ��ʂ��v�Z
					float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
					fMovement *= CAMERA_MOVE_SPEED;

					// ���_���ړ�������
					m_PosR.x += sinf(fAngle) * fMovement;
					m_PosR.z -= cosf(fAngle) * fMovement;

					// �����_���ړ�������
					m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
					m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
				}
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
			{// �}�E�X�̍��N���b�N�݂̂������ꂽ
				// �������}�E�X�̑��x�ɍ��킹�ĉ�]������
				// �c��
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// ����
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// �������~�����𒴂���
					m_Rot.z = 0.0f;
				}

				// ���_�̈ʒu���v�Z����
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// �}�E�X�̉E�N���b�N�݂̂������ꂽ
				// �������}�E�X�̑��x�ɍ��킹�ĉ�]������
				// �c��
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// ����
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// �������~�����𒴂���
					m_Rot.z = 0.0f;
				}

				// �����_�̈ʒu���v�Z����
				m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
				m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
				m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
			}
		}
		// ������@�\��
		CDebugProc::Print("\n( �J�����̑�����@ )\n");
		CDebugProc::Print("[ �J�����̎��_�ړ�   ]   : �}�E�X���N���b�N\n");
		CDebugProc::Print("[ �J�����̒����_�ړ� ]   : �}�E�X�E�N���b�N\n");
		CDebugProc::Print("[ �J�����̈ړ�       ]   : �}�E�X���E�����N���b�N\n");
		CDebugProc::Print("[ �J�����̊g��k��   ]   : �}�E�X�z�C�[��\n");
	}
	else
	{// �X�V���Ȃ��
		// �O�����ۑ�
		m_PosVOld = m_PosV;   // ���_
		m_RotOld = m_Rot;     // ����

		if (CManager::GetMode() == CManager::MODE_TITLE)
		{// �^�C�g����ʂȂ��
			Title();
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// �`���[�g���A����ʂȂ��
			Tutorial();
		}
		else if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[����ʂȂ��
			if (CManager::GetRenderer()->GetPause() == false)
			{// �|�[�Y���łȂ��̂Ȃ�
				if (m_bMoving == false)
				{// �J�����������œ������Ȃ�
					Game();
				}
				else
				{// �J�����������œ�������Ȃ�
					MovieCamera();
				}
			}
		}
		else if (CManager::GetMode() == CManager::MODE_RESULT)
		{// ���U���g��ʂȂ��
			Result();
		}
		else if (CManager::GetMode() == CManager::MODE_RANKING)
		{// �����L���O��ʂȂ��
			Ranking();
		}
	}

	CDebugProc::Print("\n�J�����̎��_   : ( %.1f %.1f %.1f )\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CDebugProc::Print("�J�����̒����_ : ( %.1f %.1f %.1f )\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CDebugProc::Print("�J�����̌���   : ( %.2f %.2f %.2f )\n\n", m_Rot.x, m_Rot.y, m_Rot.z);

#else
	if (CManager::GetMode() == CManager::MODE_TITLE)
	{// �^�C�g����ʂȂ��
		Title();
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// �`���[�g���A����ʂȂ��
		Tutorial();
	}
	else if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂȂ��
		if (m_bMoving == false)
		{// �J�����������œ������Ȃ�
			Game();
		}
		else
		{// �J�����������œ�������Ȃ�
			MovieCamera();
		}
	}
	else if (CManager::GetMode() == CManager::MODE_RESULT)
	{// ���U���g��ʂȂ��
		Result();
	}
	else if (CManager::GetMode() == CManager::MODE_RANKING)
	{// �����L���O��ʂȂ��
		Ranking();
	}
#endif
}

//=============================================================================
//    �J�����̐ݒ�
//=============================================================================
void CCamera::SetCamera(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// �r���[�|�[�g(�`��̈�)�̐ݒ�
			pDevice->SetViewport(&m_ViewPort);

		    // �v���W�F�N�V�����}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_MtxProjection);

			// �v���W�F�N�V�����}�g���b�N�X���쐬
			D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
				D3DXToRadian(CAMERA_ANGEL_VIEW),
				(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
				CAMERA_ZNER,
				CAMERA_ZFAR);

			// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

			// �r���[�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_MtxView);

			// �r���[�}�g���b�N�X���쐬
			D3DXMatrixLookAtLH(&m_MtxView,
				&m_PosV,
				&m_PosR,
				&m_VecU);

			// �r���[�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
		}
	}
}

//=============================================================================
//    �^�C�g����ʂ̃J��������
//=============================================================================
void CCamera::Title(void)
{
	CTitle *pTitle = CManager::GetTitle();
	if (pTitle != NULL)
	{// �^�C�g���N���X���擾�ł���
		if (pTitle->GetState() == CTitle::STATE_OK_CHANGEMODE || pTitle->GetState() == CTitle::STATE_FADE_IN)
		{// �J�����������Ă�����Ԃł���
			// ���݂̒����_��ړI�̒����_�̏ꏊ�Ɉړ�������
			m_PosR.x += m_PosRDiff.x;
			m_PosR.y += m_PosRDiff.y;
			m_PosR.z += m_PosRDiff.z;

			// �����_���王�_�̈ʒu�����肷��
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * CAMERA_TITLE_LENGTH_0;
			m_PosV.y = m_PosVDest.y;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * CAMERA_TITLE_LENGTH_0;
		}
	}
}

//=============================================================================
//    �`���[�g���A����ʂ̃J��������
//=============================================================================
void CCamera::Tutorial(void)
{
	// �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^��ۑ�
	m_pLandSceneOld = m_pLandScene;

	// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
	m_pLandScene = NULL;

	// �O��̎��_���W��ۑ�
	m_PosVOld = m_PosV;

	// �v���C���[�N���X���擾����
	CTutorial *pTutorial = CManager::GetTutorial();
	if (pTutorial != NULL)
	{// �`���[�g���A���N���X���擾�ł���
		m_pPlayer = pTutorial->GetPlayer();
	}

	//  �J�����̋�����ݒ�
	m_fLength = CAMERA_GAME_LENGTH;
	if (m_pPlayer != NULL)
	{// �v���C���[�N���X���擾�ł���
	    // �W���C�p�b�h���擾
		CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);
		if (CManager::GetTutorial()->GetState() == CTutorial::STATE_NONE)
		{// �������Ă��Ȃ���Ԃ�������
			if (pJoyStick != NULL)
			{// �W���C�X�e�B�b�N���擾�ł���
				if (pJoyStick->GetRightAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisX() <= -DI_STICK_AXIS_MIN
					|| pJoyStick->GetRightAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisY() <= -DI_STICK_AXIS_MIN)
				{// �E�A�i���O���͂�����Ă���
					float RightAxisX = pJoyStick->GetRightAxisX();
					float RightAxisY = pJoyStick->GetRightAxisY();

					m_RotDest.y += RightAxisX * CAMERA_ANGLE_MOVERIVISION_X;
					m_RotDest.z += RightAxisY * CAMERA_ANGLE_MOVERIVISION_Y;

					if (m_RotDest.y > D3DX_PI)
					{// �������~�����𒴂���
						m_RotDest.y -= D3DX_PI * 2.0f;
					}
					if (m_RotDest.y < -D3DX_PI)
					{// �������~�����𒴂���
						m_RotDest.y += D3DX_PI * 2.0f;
					}

					if (m_RotDest.z < CAMERA_ANGLEY_MIN)
					{// �������ő�l�𒴂���
						m_RotDest.z = CAMERA_ANGLEY_MIN;
					}
					if (m_RotDest.z > CAMERA_ANGLEY_MAX)
					{// �������ŏ��l�𒴂���
						m_RotDest.z = CAMERA_ANGLEY_MAX;
					}
				}
			}
			if (CManager::GetKeyboard()->GetPress(DIK_LEFT) == true)
			{
				m_RotDest.y -= CAMERA_ROLLSPEED_Y;
				if (m_RotDest.y > D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y -= D3DX_PI * 2.0f;
				}
				if (m_RotDest.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y += D3DX_PI * 2.0f;
				}
			}
			else if (CManager::GetKeyboard()->GetPress(DIK_RIGHT) == true)
			{
				m_RotDest.y += CAMERA_ROLLSPEED_Y;
				if (m_RotDest.y > D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y -= D3DX_PI * 2.0f;
				}
				if (m_RotDest.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y += D3DX_PI * 2.0f;
				}
			}
			if (CManager::GetKeyboard()->GetPress(DIK_UP) == true)
			{
				m_RotDest.z -= CAMERA_ROLLSPEED_Z;
				if (m_RotDest.z < CAMERA_ANGLEY_MIN)
				{// �������ő�l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MIN;
				}
				if (m_RotDest.z > CAMERA_ANGLEY_MAX)
				{// �������ŏ��l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MAX;
				}
			}
			else if (CManager::GetKeyboard()->GetPress(DIK_DOWN) == true)
			{
				m_RotDest.z += CAMERA_ROLLSPEED_Z;
				if (m_RotDest.z < CAMERA_ANGLEY_MIN)
				{// �������ő�l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MIN;
				}
				if (m_RotDest.z > CAMERA_ANGLEY_MAX)
				{// �������ŏ��l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MAX;
				}
			}
		}
	}

	if (m_pPlayer != NULL)
	{// �v���C���[�N���X���擾�ł���
	    // �v���C���[�̏���ۑ�
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();

		// �ړI�̒����_�̍��W��ݒ�
		if (m_pPlayer->GetLockOn() == false)
		{// ���b�N�I�������Ă��Ȃ�
		    // �����̌v�Z
			CalcRot();

			// �����_���v���C���[�̈ʒu�ɂ���
			m_PosRDest = PlayerPos;

			// �����_�������グ��
			m_PosRDest.y += CAMERA_POSY_UP;

			// ���_�̖ړI�ʒu���v�Z����
			m_PosVDest.x = m_PosRDest.x - sinf(m_Rot.y) * m_fLength;
			m_PosVDest.y = m_PosRDest.y + sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
			m_PosVDest.z = m_PosRDest.z - cosf(m_Rot.y) * m_fLength;

			// �J�����ƒn�ʂ̓����蔻��
			FieldCollision();

			// ���݂̎��_�ƖړI�̎��_�̍������v�Z
			m_PosVDiff = m_PosVDest - m_PosV;

			// ���݂̒����_�ƖړI�̒����_�̍������v�Z
			m_PosRDiff = m_PosRDest - m_PosR;

			// ���݂̎��_��ړI�̎��_�ɋ߂Â���
			m_MoveV.x = m_PosVDiff.x * CAMERA_MOVESPEED_XZ;
			m_MoveV.y = m_PosVDiff.y * CAMERA_MOVESPEED_Y;
			m_MoveV.z = m_PosVDiff.z * CAMERA_MOVESPEED_XZ;
			m_PosV += m_MoveV;

			// ���݂̒����_��ړI�̒����_�ɋ߂Â���
			m_PosR.x += m_PosRDiff.x * CAMERA_MOVESPEED_XZ;
			m_PosR.y += m_PosRDiff.y * CAMERA_MOVESPEED_Y;
			m_PosR.z += m_PosRDiff.z * CAMERA_MOVESPEED_XZ;

			// �ǂƂ̓����蔻��
			//WallCollision();
		}
		else if (m_pPlayer->GetLockOn() == true)
		{// ���b�N�I�������Ă���
		    // �����_�����b�N�I���Ώۂ̓G�̈ʒu�ɂ���
			D3DXVECTOR3 LockOnPos = m_pPlayer->GetLockOnEnemy()->GetPos();
			m_PosRDest = LockOnPos;

			// �����_�������グ��
			m_PosRDest.y += CAMERA_LOCKON_POSR_UP;

			// �v���C���[�Ƃ̊p�x���v�Z
			float fAngle = atan2f(PlayerPos.x - LockOnPos.x, PlayerPos.z - LockOnPos.z);
			m_RotDest.y = fAngle - D3DX_PI;
			if (m_RotDest.y > D3DX_PI)
			{// �������~�����𒴂���
				m_RotDest.y -= D3DX_PI * 2.0f;
			}
			if (m_RotDest.y < -D3DX_PI)
			{// �������~�����𒴂���
				m_RotDest.y += D3DX_PI * 2.0f;
			}

			// ���݂̌����v�Z
			CalcRot();

			// ���_�̖ړI�ʒu���v�Z����
			m_PosVDest.x = PlayerPos.x - sinf(m_Rot.y) * CAMERA_LOCKON_LENGTH;
			m_PosVDest.y = PlayerPos.y + CAMERA_LOCKON_POSV_UP;
			m_PosVDest.z = PlayerPos.z - cosf(m_Rot.y) * CAMERA_LOCKON_LENGTH;

			// ���݂̎��_�ƖړI�̎��_�̍������v�Z
			m_PosVDiff = m_PosVDest - m_PosV;

			// ���݂̒����_�ƖړI�̒����_�̍������v�Z
			m_PosRDiff = m_PosRDest - m_PosR;

			// ���݂̎��_��ړI�̎��_�ɋ߂Â���
			m_PosV.x += m_PosVDiff.x * CAMERA_LOCKPOSV_MOVESPEED_XZ;
			m_PosV.y += m_PosVDiff.y * CAMERA_LOCKPOSV_MOVESPEED_Y;
			m_PosV.z += m_PosVDiff.z * CAMERA_LOCKPOSV_MOVESPEED_XZ;

			// ���݂̒����_��ړI�̒����_�ɋ߂Â���
			m_PosR.x += m_PosRDiff.x * CAMERA_LOCKPOSR_MOVESPEED_XZ;
			m_PosR.y += m_PosRDiff.y * CAMERA_LOCKPOSR_MOVESPEED_Y;
			m_PosR.z += m_PosRDiff.z * CAMERA_LOCKPOSR_MOVESPEED_XZ;

			// �ǂƂ̓����蔻��
			//WallCollision();
		}
	}
}

//=============================================================================
//    �Q�[����ʂ̃J��������
//=============================================================================
void CCamera::Game(void)
{
	// �O��̎��_���W��ۑ�
	m_PosVOld = m_PosV;

	if (CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
	{// �Q�[�����ʏ��Ԃł���
		// �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^��ۑ�
		m_pLandSceneOld = m_pLandScene;

		// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
		m_pLandScene = NULL;

		// �v���C���[�N���X���擾����
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// �Q�[���N���X���擾�ł���
			m_pPlayer = pGame->GetPlayer();
		}

		//  �J�����̋�����ݒ�
		m_fLength = CAMERA_GAME_LENGTH;
		if (m_pPlayer != NULL && CManager::GetGame()->GetState() != CGame::STATE_END)
		{// �v���C���[�N���X���擾�ł���
			// �W���C�p�b�h���擾
			CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);
			if (pJoyStick != NULL)
			{// �W���C�X�e�B�b�N���擾�ł���
				if (pJoyStick->GetRightAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisX() <= -DI_STICK_AXIS_MIN
					|| pJoyStick->GetRightAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisY() <= -DI_STICK_AXIS_MIN)
				{// �E�A�i���O���͂�����Ă���
					float RightAxisX = pJoyStick->GetRightAxisX();
					float RightAxisY = pJoyStick->GetRightAxisY();

					// �ړI�̊p�x�ύX
					m_RotDest.y += RightAxisX * CAMERA_ANGLE_MOVERIVISION_X;
					m_RotDest.z += RightAxisY * CAMERA_ANGLE_MOVERIVISION_Y;

					if (m_RotDest.y > D3DX_PI)
					{// �������~�����𒴂���
						m_RotDest.y -= D3DX_PI * 2.0f;
					}
					if (m_RotDest.y < -D3DX_PI)
					{// �������~�����𒴂���
						m_RotDest.y += D3DX_PI * 2.0f;
					}

					if (m_RotDest.z < CAMERA_ANGLEY_MIN)
					{// �������ő�l�𒴂���
						m_RotDest.z = CAMERA_ANGLEY_MIN;
					}
					if (m_RotDest.z > CAMERA_ANGLEY_MAX)
					{// �������ŏ��l�𒴂���
						m_RotDest.z = CAMERA_ANGLEY_MAX;
					}
				}
			}
			if (CManager::GetKeyboard()->GetPress(DIK_LEFT) == true)
			{
				m_RotDest.y -= CAMERA_ROLLSPEED_Y;
				if (m_RotDest.y > D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y -= D3DX_PI * 2.0f;
				}
				if (m_RotDest.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y += D3DX_PI * 2.0f;
				}
			}
			else if (CManager::GetKeyboard()->GetPress(DIK_RIGHT) == true)
			{
				m_RotDest.y += CAMERA_ROLLSPEED_Y;
				if (m_RotDest.y > D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y -= D3DX_PI * 2.0f;
				}
				if (m_RotDest.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y += D3DX_PI * 2.0f;
				}
			}
			if (CManager::GetKeyboard()->GetPress(DIK_UP) == true)
			{
				m_RotDest.z -= CAMERA_ROLLSPEED_Z;
				if (m_RotDest.z < CAMERA_ANGLEY_MIN)
				{// �������ő�l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MIN;
				}
				if (m_RotDest.z > CAMERA_ANGLEY_MAX)
				{// �������ŏ��l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MAX;
				}
			}
			else if (CManager::GetKeyboard()->GetPress(DIK_DOWN) == true)
			{
				m_RotDest.z += CAMERA_ROLLSPEED_Z;
				if (m_RotDest.z < CAMERA_ANGLEY_MIN)
				{// �������ő�l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MIN;
				}
				if (m_RotDest.z > CAMERA_ANGLEY_MAX)
				{// �������ŏ��l�𒴂���
					m_RotDest.z = CAMERA_ANGLEY_MAX;
				}
			}
		}

		if (m_pPlayer != NULL)
		{// �v���C���[�N���X���擾�ł���
			// �v���C���[�̏���ۑ�
			D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
			D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();

			// �ړI�̒����_�̍��W��ݒ�
			if (m_pPlayer->GetLockOn() == false)
			{// ���b�N�I�������Ă��Ȃ�
				// �����̌v�Z
				CalcRot();

				// �����_���v���C���[�̈ʒu�ɂ���
				m_PosRDest = PlayerPos;

				// �����_�������グ��
				m_PosRDest.y += CAMERA_POSY_UP;

				// ���_�̖ړI�ʒu���v�Z����
				m_PosVDest.x = m_PosRDest.x - sinf(m_Rot.y) * m_fLength;
				m_PosVDest.y = m_PosRDest.y + sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
				m_PosVDest.z = m_PosRDest.z - cosf(m_Rot.y) * m_fLength;

				// �J�����ƒn�ʂ̓����蔻��
				FieldCollision();

				// ���݂̎��_�ƖړI�̎��_�̍������v�Z
				m_PosVDiff = m_PosVDest - m_PosV;

				// ���݂̒����_�ƖړI�̒����_�̍������v�Z
				m_PosRDiff = m_PosRDest - m_PosR;

				// ���݂̎��_��ړI�̎��_�ɋ߂Â���
				m_PosV.x += m_PosVDiff.x * CAMERA_MOVESPEED_XZ;
				m_PosV.y += m_PosVDiff.y * CAMERA_MOVESPEED_Y;
				m_PosV.z += m_PosVDiff.z * CAMERA_MOVESPEED_XZ;

				// ���݂̒����_��ړI�̒����_�ɋ߂Â���
				m_PosR.x += m_PosRDiff.x * CAMERA_MOVESPEED_XZ;
				m_PosR.y += m_PosRDiff.y * CAMERA_MOVESPEED_Y;
				m_PosR.z += m_PosRDiff.z * CAMERA_MOVESPEED_XZ;

				// �ǂƂ̓����蔻��
				//WallCollision();
			}
			else if (m_pPlayer->GetLockOn() == true)
			{// ���b�N�I�������Ă���
				// �����_�����b�N�I���Ώۂ̓G�̈ʒu�ɂ���
				D3DXVECTOR3 LockOnPos = m_pPlayer->GetLockOnEnemy()->GetPos();
				m_PosRDest = LockOnPos;

				// �����_�������グ��
				m_PosRDest.y += CAMERA_LOCKON_POSR_UP;

				// �v���C���[�Ƃ̊p�x���v�Z
				float fAngle = atan2f(PlayerPos.x - LockOnPos.x, PlayerPos.z - LockOnPos.z);
				m_RotDest.y = fAngle - D3DX_PI;
				if (m_RotDest.y > D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y -= D3DX_PI * 2.0f;
				}
				if (m_RotDest.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_RotDest.y += D3DX_PI * 2.0f;
				}

				// ���݂̌����v�Z
				CalcRot();

				// ���_�̖ړI�ʒu���v�Z����
				m_PosVDest.x = PlayerPos.x - sinf(m_Rot.y) * CAMERA_LOCKON_LENGTH;
				m_PosVDest.y = PlayerPos.y + CAMERA_LOCKON_POSV_UP;
				m_PosVDest.z = PlayerPos.z - cosf(m_Rot.y) * CAMERA_LOCKON_LENGTH;

				// ���݂̎��_�ƖړI�̎��_�̍������v�Z
				m_PosVDiff = m_PosVDest - m_PosV;

				// ���݂̒����_�ƖړI�̒����_�̍������v�Z
				m_PosRDiff = m_PosRDest - m_PosR;

				// ���݂̎��_��ړI�̎��_�ɋ߂Â���
				m_PosV.x += m_PosVDiff.x * CAMERA_LOCKPOSV_MOVESPEED_XZ;
				m_PosV.y += m_PosVDiff.y * CAMERA_LOCKPOSV_MOVESPEED_Y;
				m_PosV.z += m_PosVDiff.z * CAMERA_LOCKPOSV_MOVESPEED_XZ;

				// ���݂̒����_��ړI�̒����_�ɋ߂Â���
				m_PosR.x += m_PosRDiff.x * CAMERA_LOCKPOSR_MOVESPEED_XZ;
				m_PosR.y += m_PosRDiff.y * CAMERA_LOCKPOSR_MOVESPEED_Y;
				m_PosR.z += m_PosRDiff.z * CAMERA_LOCKPOSR_MOVESPEED_XZ;

				// �ǂƂ̓����蔻��
				//WallCollision();
			}
		}
	}
}

//=============================================================================
//    ���U���g��ʂ̃J��������
//=============================================================================
void CCamera::Result(void)
{

}

//=============================================================================
//    �����L���O��ʂ̃J��������
//=============================================================================
void CCamera::Ranking(void)
{
	// ���݂̒����_��ړI�̒����_�̏ꏊ�Ɉړ�������
	m_PosR.x += m_PosRDiff.x;
	m_PosR.y += m_PosRDiff.y;
	m_PosR.z += m_PosRDiff.z;

	// �����_���王�_�̈ʒu�����肷��
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * CAMERA_TITLE_LENGTH_0;
	m_PosV.y = m_PosVDest.y;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * CAMERA_TITLE_LENGTH_0;
}


//=============================================================================
//    �J�����̓�������(����p)
//=============================================================================
void CCamera::MovieCamera(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetPress(DIK_J) == true)
	{// �������̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_I) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pKeyboard->GetPress(DIK_K) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
	}
	else if (pKeyboard->GetPress(DIK_L) == true)
	{// �E�����̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_I) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pKeyboard->GetPress(DIK_K) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
	}
	else if (pKeyboard->GetPress(DIK_I) == true)
	{// ������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.x += sinf(m_Rot.y) * CAMERA_MOVIE_MOVE;
		m_PosR.z += cosf(m_Rot.y) * CAMERA_MOVIE_MOVE;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_K) == true)
	{// �������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.x += sinf(m_Rot.y + D3DX_PI) * CAMERA_MOVIE_MOVE;
		m_PosR.z += cosf(m_Rot.y + D3DX_PI) * CAMERA_MOVIE_MOVE;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_U) == true)
	{// ������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.y += 2.0f;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_O) == true)
	{// �������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.y -= 2.0f;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}

	CInputMouse *pMouse = CManager::GetMouse();

	if (pMouse != NULL)
	{// �}�E�X���擾�ł���
		if (pMouse->GetMouseAxisZ() != 0)
		{// �}�E�X�̃z�C�[���{�^�����X�N���[�����ꂽ
			m_fLength += 0.1f * pMouse->GetMouseAxisZ();
			if (m_fLength < CAMERA_LENGTH_MIN)
			{// ���������̒l���߂��Ȃ���
				m_fLength = CAMERA_LENGTH_MIN;
			}
			else if (m_fLength > CAMERA_LENGTH_MAX)
			{// ���������̒l��艓���Ȃ���
				m_fLength = CAMERA_LENGTH_MAX;
			}

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}

		if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// �}�E�X�̍��{�^���ƉE�{�^���������ɉ����ꂽ
			if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
			{// �}�E�X����������Ă���
			 // �����ɉ����ꂽ�Ƃ��̈ʒu���獡�̃}�E�X�J�[�\���̈ʒu���王�_�����_���ړ�������������v�Z
				float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
				fAngle -= m_Rot.y;
				if (fAngle < -D3DX_PI)
				{// �������~�����𒴂���
					fAngle += D3DX_PI * 2.0f;
				}
				if (fAngle > D3DX_PI)
				{// �������~�����𒴂���
					fAngle -= D3DX_PI * 2.0f;
				}

				// �}�E�X��X��,Y������ړ��ʂ��v�Z
				float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
				fMovement *= CAMERA_MOVE_SPEED;

				// ���_���ړ�������
				m_PosR.x += sinf(fAngle) * fMovement;
				m_PosR.z -= cosf(fAngle) * fMovement;

				// �����_���ړ�������
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
		{// �}�E�X�̍��N���b�N�݂̂������ꂽ
		    // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
		    // �c��
			m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (m_Rot.y > D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y -= D3DX_PI * 2.0f;
			}
			if (m_Rot.y < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y += D3DX_PI * 2.0f;
			}

			// ����
			m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (m_Rot.z < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.z = -D3DX_PI;
			}
			if (m_Rot.z > 0.0f)
			{// �������~�����𒴂���
				m_Rot.z = 0.0f;
			}

			// ���_�̈ʒu���v�Z����
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// �}�E�X�̉E�N���b�N�݂̂������ꂽ
		 // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
		    // �c��
			m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (m_Rot.y > D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y -= D3DX_PI * 2.0f;
			}
			if (m_Rot.y < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y += D3DX_PI * 2.0f;
			}

			// ����
			m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (m_Rot.z < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.z = -D3DX_PI;
			}
			if (m_Rot.z > 0.0f)
			{// �������~�����𒴂���
				m_Rot.z = 0.0f;
			}

			// �����_�̈ʒu���v�Z����
			m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
			m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
			m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
		}
	}
}

//=============================================================================
//    �J�����̒n�ʂƂ̓����蔻�菈��
//=============================================================================
bool CCamera::FieldCollision(void)
{
	bool bHit = false;   // �����������ǂ���

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	bool bFieldLandPosV = false;         // �n�ʂɏ���Ă��邩�ǂ���
	float fFieldHeightPosV = 0.0f;
	float fFieldHeightPosR = 0.0f;
	D3DXVECTOR3 PolyNormal;

	// �O�����Ă����I�u�W�F�N�g�̔���
	if (m_pLandSceneOld != NULL)
	{// �|�C���^����ł͂Ȃ�
		if (m_pLandSceneOld->GetObjType() == CScene::OBJTYPE_MESHFIELD)
		{// �O�����Ă������̂͒n�ʂ�����
			pField = (CMeshField*)pScene;
			if (pField != NULL)
			{// �n�ʂ��擾�ł���
				fFieldHeightPosV = pField->GetPolyHeight(m_PosVDest, &bFieldLandPosV, &PolyNormal);
				D3DXVECTOR3 VecA = m_PosVOld - D3DXVECTOR3(pField->GetPos().x, fFieldHeightPosV, pField->GetPos().z);
				D3DXVec3Normalize(&VecA, &VecA);
				CDebugProc::Print("%.3f %.3f %.3f\n", VecA.x, VecA.y, VecA.z);
				if (VecA.y >= 0.0f && m_PosVDest.y < fFieldHeightPosV + CAMERA_POSV_FIELDUP)
				{// �ړI�̎��_�ʒu���n�ʂ̍������Ⴍ�Ȃ��Ă��܂���
					m_pLandScene = pField;
					m_PosVDest.y = fFieldHeightPosV + CAMERA_POSV_FIELDUP;
				}
			}
		}
	}


	pScene = CScene::GetTop(FIELD_PRIORITY);
	while (pScene != NULL)
	{// ����������ɂȂ�܂ŌJ��Ԃ�
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
		{// �n�ʃN���X�ւ̃|�C���^��������
			if (bFieldLandPosV != true)
			{// �܂��n�ʂ̒��ɂ���Ɣ��肳��ĂȂ�
				pField = (CMeshField*)pScene;
				if (pField != NULL)
				{// �n�ʂ��擾�ł���
					fFieldHeightPosV = pField->GetPolyHeight(m_PosVDest, &bFieldLandPosV, &PolyNormal);
					if (bFieldLandPosV == true)
					{// �n�ʂ̒��ɂ���Ɣ��肳�ꂽ
						D3DXVECTOR3 VecA = m_PosVOld - D3DXVECTOR3(pField->GetPos().x, fFieldHeightPosV, pField->GetPos().z);
						D3DXVec3Normalize(&VecA, &VecA);
						//CDebugProc::Print("%.3f %.3f %.3f\n", VecA.x, VecA.y, VecA.z);
						if (VecA.y >= 0.0f && m_PosVDest.y < fFieldHeightPosV + CAMERA_POSV_FIELDUP)
						{// �ړI�̎��_�ʒu���n�ʂ̍������Ⴍ�Ȃ��Ă��܂���
							m_pLandScene = pField;
							m_PosVDest.y = fFieldHeightPosV + CAMERA_POSV_FIELDUP;
						}
						else
						{
							bFieldLandPosV = false;
							if (m_PosVOld.y >= fFieldHeightPosV + CAMERA_POSV_FIELDUP  && m_PosVDest.y < fFieldHeightPosV + CAMERA_POSV_FIELDUP)
							{// �ړI�̎��_�ʒu���n�ʂ̍������Ⴍ�Ȃ��Ă��܂���
								m_PosVDest.y = fFieldHeightPosV + CAMERA_POSV_FIELDUP;
							}
						}
					}
				}
			}
		}
		pScene = pSceneNext;
	}


	bFieldLandPosV = false;
	for (int nCntPriority = 0; nCntPriority < EFFECT_PRIORITY; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �n�ʃN���X�ւ̃|�C���^��������
				if (bFieldLandPosV != true)
				{
					pField = (CMeshField*)pScene;
					if (pField != NULL)
					{// �n�ʂ��擾�ł���
						fFieldHeightPosV = pField->GetPolyHeight(m_PosVDest, &bFieldLandPosV, &PolyNormal);
						if (bFieldLandPosV == true && pField->GetCulling() == false)
						{// �n�ʂ̒��ɂ���Ɣ��肳�ꂽ
							if (m_PosVOld.y - 20.0f <= fFieldHeightPosV && m_PosVDest.y >= fFieldHeightPosV)
							{
								m_PosVDest.y = fFieldHeightPosV;
								m_Rot.z = m_RotOld.z;
							}
						}
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	return bHit;
}

//=============================================================================
//    �J�����̕ǂƂ̓����蔻����s������
//=============================================================================
bool CCamera::WallCollision(void)
{
	bool bHit = false;   // �����������ǂ���

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	CMeshWall *pWall = NULL;             // �ǃN���X�ւ̃|�C���^

	pScene = CScene::GetTop(WALL_PRIORITY);
	while (pScene != NULL)
	{// ����������ɂȂ�܂ŌJ��Ԃ�
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_MESHWALL)
		{// �ǃN���X�ւ̃|�C���^��������
			pWall = (CMeshWall*)pScene;
			if (pWall != NULL)
			{// �ǂ��擾�ł���
				D3DXVECTOR3 InterPoint = m_PosVDest;
				if (pWall->Collision(&m_PosVDest, &m_PosV, &m_MoveV, D3DXVECTOR3(0.0f, 0.0f, 10.0f), &InterPoint) == true)
				{// �ǂɓ������Ă���
					if (bHit == true)
					{// ���łɕǂɓ������Ă���
						m_PosVDest.x = InterPoint.x;
						m_PosVDest.z = InterPoint.z;
					}
					//m_PosVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					// �����̍Čv�Z
					float fLength = sqrtf((m_PosVDest.x - m_PosR.x) * (m_PosVDest.x - m_PosR.x) + (m_PosVDest.z - m_PosR.z) * (m_PosVDest.z - m_PosR.z));
					if (fLength <= CAMERA_POSY_RANGE)
					{// �������Z��
						float fRivision = fLength / CAMERA_POSY_RANGE;
						m_PosVDest.y = m_PosR.y + cosf((D3DX_PI * 0.5f) * fRivision) * CAMERA_POSY_RANGE;
					}

					// �����̍Čv�Z
					float fRotY = atan2f(m_PosR.x - m_PosVDest.x, m_PosR.z - m_PosVDest.z);
					m_Rot.y = fRotY;
					m_RotDest.y = fRotY;
					m_RotDiff.y = 0.0f;

					// ������������ɂ���
					bHit = true;
				}
			}
		}
		pScene = pSceneNext;
	}

	return bHit;
}

//=============================================================================
//    �J�����̌��݂̌������v�Z���鏈��
//=============================================================================
void CCamera::CalcRot(void)
{
	// �������v�Z
	m_RotDiff = m_RotDest - m_Rot;

	// �����`�F�b�N
	// X��
	if (m_RotDiff.x > D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.x -= D3DX_PI * 2.0f;
	}
	if (m_RotDiff.x < -D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.x += D3DX_PI * 2.0f;
	}

	// Y��
	if (m_RotDiff.y > D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.y -= D3DX_PI * 2.0f;
	}
	if (m_RotDiff.y < -D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.y += D3DX_PI * 2.0f;
	}

	// Z��
	if (m_Rot.z > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z -= D3DX_PI * 2.0f;
	}
	if (m_Rot.z < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z += D3DX_PI * 2.0f;
	}

	// ���݂̌�������������␳����
	if (m_pPlayer == NULL)
	{// �v���C���[�N���X�ւ̃|�C���^����ł���
		m_Rot.x += m_RotDiff.x * CAMERA_ROT_RIVISION_X;
		m_Rot.y += m_RotDiff.y * CAMERA_ROT_RIVISION_Y;
		m_Rot.z += m_RotDiff.z * CAMERA_ROT_RIVISION_Z;
	}
	else
	{// �v���C���[�N���X���擾�ł��Ă���
		if (m_pPlayer->GetLockOn() == false)
		{// ���b�N�I�������Ă��Ȃ�
			m_Rot.x += m_RotDiff.x * CAMERA_ROT_RIVISION_X;
			m_Rot.y += m_RotDiff.y * CAMERA_ROT_RIVISION_Y;
			m_Rot.z += m_RotDiff.z * CAMERA_ROT_RIVISION_Z;
		}
		else if (m_pPlayer->GetLockOn() == true)
		{// ���b�N�I�����Ă���
			m_Rot.x += m_RotDiff.x * CAMERA_LOCK_ROTRIVISION_X;
			m_Rot.y += m_RotDiff.y * CAMERA_LOCK_ROTRIVISION_Y;
			m_Rot.z += m_RotDiff.z * CAMERA_LOCK_ROTRIVISION_Z;
		}
	}

	// ���݂̌����`�F�b�N
	// X��
	if (m_Rot.x > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.x -= D3DX_PI * 2.0f;
	}
	if (m_Rot.x < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.x += D3DX_PI * 2.0f;
	}

	// Y��
	if (m_Rot.y > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.y += D3DX_PI * 2.0f;
	}

	// Z��
	if (m_Rot.z > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z -= D3DX_PI * 2.0f;
	}
	if (m_Rot.z < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z += D3DX_PI * 2.0f;
	}
}

//=============================================================================
//    �J�����̌��݂̎��_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	m_PosV = posV;
}

//=============================================================================
//    �J�����̖ړI�̎��_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosVDest(const D3DXVECTOR3 posVDest)
{
	m_PosVDest = posVDest;
}

//=============================================================================
//    �J�����̌��݂̎��_�ƖړI�̎��_�̍����ݒ菈��
//=============================================================================
void CCamera::SetPosVDiff(const D3DXVECTOR3 posVDiff)
{
	m_PosVDiff = posVDiff;
}

//=============================================================================
//    �J�����̑O��̎��_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosVOld(const D3DXVECTOR3 posVOld)
{
	m_PosVOld = posVOld;
}

//=============================================================================
//    �J�����̌��݂̒����_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	m_PosR = posR;
}

//=============================================================================
//    �J�����̖ړI�̒����_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosRDest(const D3DXVECTOR3 posRDest)
{
	m_PosRDest = posRDest;
}

//=============================================================================
//    �J�����̌��݂̒����_�ƖړI�̒����_�̍����ݒ菈��
//=============================================================================
void CCamera::SetPosRDiff(const D3DXVECTOR3 posRDiff)
{
	m_PosRDiff = posRDiff;
}

//=============================================================================
//    �J�����̎��_�̈ړ��ʂ̎擾����
//=============================================================================
void CCamera::SetMoveV(const D3DXVECTOR3 MoveV)
{
	m_MoveV = MoveV;
}

//=============================================================================
//    �J�����̒����_�̈ړ��ʂ̎擾����
//=============================================================================
void CCamera::SetMoveR(const D3DXVECTOR3 MoveR)
{
	m_MoveR = MoveR;
}

//=============================================================================
//    �J�����̌��݂̌����̐ݒ菈��
//=============================================================================
void CCamera::SetRot(const D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//=============================================================================
//    �J�����̖ړI�̌����̐ݒ菈��
//=============================================================================
void CCamera::SetRotDest(const D3DXVECTOR3 RotDest)
{
	m_RotDest = RotDest;
}

//=============================================================================
//    �J�����̖ړI�̌����ƌ��݂̌����̍����̐ݒ菈��
//=============================================================================
void CCamera::SetRotDiff(const D3DXVECTOR3 RotDiff)
{
	m_RotDiff = RotDiff;
}

//=============================================================================
//    �J�����̑O��̌����̐ݒ菈��
//=============================================================================
void CCamera::SetRotOld(const D3DXVECTOR3 RotOld)
{
	m_RotOld = RotOld;
}

//=============================================================================
//    �J�����̌����̐ݒ菈��
//=============================================================================
void CCamera::SetMtxProjection(const D3DXMATRIX MtxProjection)
{
	m_MtxProjection = MtxProjection;
}

//=============================================================================
//    �J�����̌����̐ݒ菈��
//=============================================================================
void CCamera::SetMtxView(const D3DXMATRIX MtxView)
{
	m_MtxView = MtxView;
}

//=============================================================================
//    �J�����̌��݂̎��_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_PosV;
}

//=============================================================================
//    �J�����̖ړI�̎��_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVDest(void)
{
	return m_PosVDest;
}

//=============================================================================
//    �J�����̌��݂̎��_�ƖړI�̎��_�̍����擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVDiff(void)
{
	return m_PosVDiff;
}

//=============================================================================
//    �J�����̑O��̎��_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVOld(void)
{
	return m_PosVOld;
}

//=============================================================================
//    �J�����̌��݂̒����_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_PosR;
}

//=============================================================================
//    �J�����̖ړI�̒����_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosRDest(void)
{
	return m_PosRDest;
}

//=============================================================================
//    �J�����̌��݂̒����_�ƖړI�̒����_�̍����擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosRDiff(void)
{
	return m_PosRDiff;
}

//=============================================================================
//    �J�����̎��_�̈ړ��ʂ̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetMoveV(void)
{
	return m_MoveV;
}

//=============================================================================
//    �J�����̒����_�̈ړ��ʂ̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetMoveR(void)
{
	return m_MoveR;
}

//=============================================================================
//    �J�����̌��݂̌����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �J�����̖ړI�̌����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRotDest(void)
{
	return m_RotDest;
}

//=============================================================================
//    �J�����̖ړI�̌����ƌ��݂̌����̍����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRotDiff(void)
{
	return m_RotDiff;
}

//=============================================================================
//    �J�����̑O��̌����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRotOld(void)
{
	return m_RotOld;
}

//=============================================================================
//    �J�����̃v���W�F�N�V�����}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CCamera::GetMtxProjection(void)
{
	return m_MtxProjection;
}

//=============================================================================
//    �J�����̃r���[�}�g���b�N�X�̎擾����
//=============================================================================
D3DXMATRIX CCamera::GetMtxView(void)
{
	return m_MtxView;
}


//*****************************************************************************
//    CMapCamera�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMapCamera::CMapCamera()
{
	// �e��l�̃N���A
	m_pPlayer = NULL;                           // �v���C���[�N���X�ւ̃|�C���^
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���݂̎��_
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���݂̒����_
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ����
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ������x�N�g��
	D3DXMatrixIdentity(&m_MtxProjection);       // �v���W�F�N�V�����}�g���b�N�X
	D3DXMatrixIdentity(&m_MtxView);             // �r���[�}�g���b�N�X
	m_fLength = 0.0f;                           // ����
	m_ViewPort.X = 0;                           // �`��̈捶��X���W
	m_ViewPort.Y = 0;                           // �`��̈捶��Y���W
	m_ViewPort.Width = 0;                       // �`��̈�̕�
	m_ViewPort.Height = 0;                      // �`��̈�̍���
	m_ViewPort.MinZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ŏ��l
	m_ViewPort.MaxZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ő�l
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMapCamera::~CMapCamera()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMapCamera::Init(void)
{
	// �e��l�̏�����
	m_PosV = D3DXVECTOR3(0.0f, MAPCAMERA_POSV_Y, 0.0f);     // ���_
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // �����_
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                  // ����
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);                 // ������x�N�g��

	// �r���[�|�[�g�ݒ�
	m_ViewPort.X = MAPCAMERA_VIEWPORT_X;                    // �`��̈捶��X���W
	m_ViewPort.Y = MAPCAMERA_VIEWPORT_Y;                    // �`��̈捶��Y���W
	m_ViewPort.Width = MAPCAMERA_VIEWPORT_WIDTH;            // �`��̈�̕�
	m_ViewPort.Height = MAPCAMERA_VIEWPORT_HEIGHT;          // �`��̈�̍���
	m_ViewPort.MinZ = 0.0f;                                 // �`��̈�[�x�o�b�t�@�ŏ��l
	m_ViewPort.MaxZ = 1.0f;                                 // �`��̈�[�x�o�b�t�@�ő�l

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixOrthoLH(&m_MtxProjection,
		MAPCAMERA_VIEW_WIDTH,
		MAPCAMERA_VIEW_HEIGHT,
		MAPCAMERA_ZNER,
		MAPCAMERA_ZFAR);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMapCamera::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CMapCamera::Update(void)
{
	// �v���C���[�N���X���擾����
	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{// �Q�[���N���X���擾�ł���
		m_pPlayer = pGame->GetPlayer();
	}

	if (m_pPlayer != NULL)
	{// �v���C���[���擾�ł���
		// �v���C���[�̏���ۑ�
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();

		// ���_��XZ���ʏ�̈ʒu��ݒ�
		m_PosV.x = PlayerPos.x;
		m_PosV.y = MAPCAMERA_POSV_Y;
		m_PosV.z = PlayerPos.z - 20.0f;

		// �����_�̏ꏊ���v���C���[�̈ʒu�ɐݒ�
		m_PosR = PlayerPos;
	}
}

//=============================================================================
//    �}�b�v�J�����̐ݒ�
//=============================================================================
void CMapCamera::SetCamera(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// �w�i�p�̃r���[�|�[�g���쐬
			D3DVIEWPORT9 MapBgViewport;
			MapBgViewport.X = MAPCAMERA_VIEWPORT_X - 2;
			MapBgViewport.Y = MAPCAMERA_VIEWPORT_Y - 2;
			MapBgViewport.Width = MAPCAMERA_VIEWPORT_WIDTH + 4;
			MapBgViewport.Height = MAPCAMERA_VIEWPORT_HEIGHT + 4;
			MapBgViewport.MinZ = 0.0f;
			MapBgViewport.MaxZ = 1.0f;

			// �w�i�p�r���[�|�[�g(�`��̈�)�̐ݒ�
			pDevice->SetViewport(&MapBgViewport);

			// �w�i���N���A����
			pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff), 1.0f, 0);

			if (CManager::GetFade()->GetState() != CFade::STATE_NONE)
			{// �t�F�[�h���g�p����Ă���
				if (CManager::GetFade()->GetScene2D() != NULL)
				{// �t�F�[�h�Ɏg�p����|���S������������Ă���
					CManager::GetFade()->GetScene2D()->Draw();
				}
			}

		    // �r���[�|�[�g(�`��̈�)�̐ݒ�
			pDevice->SetViewport(&m_ViewPort);

			// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

			// �r���[�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_MtxView);

			// �r���[�}�g���b�N�X���쐬
			D3DXMatrixLookAtLH(&m_MtxView,
				&m_PosV,
				&m_PosR,
				&m_VecU);

			// �r���[�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
		}
	}
}