//*****************************************************************************
//
//     �����_�����O�̏���[renderer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "pause.h"
#include "camera.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "ui.h"
#include "functionlib.h"
#include "player.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define RENDERER_WINDOW_COLOR (D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0xff))    // ��ʃN���A���̔w�i�F

#define PAUSE_DRAW            (true)     // �|�[�Y��`�悷�邩�ǂ���
#define PAUSE_PRIORITY        (10)       // �|�[�Y��ʂ̏����D�揇��
#define FEEDBACK_SCALE_INI    (0.995f)   // �t�B�[�h�o�b�N�G�t�F�N�g�̑傫��
#define FEEDBACK_ALPHA_INI    (0.9f)     // �t�B�[�h�o�b�N�G�t�F�N�g�̓����x

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	// �e��l�̃N���A
	m_pPause = NULL;            // �|�[�Y�N���X�ւ̃|�C���^
	m_bPause = false;           // �|�[�Y�����ǂ���
	m_pD3DDevice = NULL;        // �f�o�C�X�ւ̃|�C���^
	m_pD3D = NULL;              // Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	m_pVtxBuff = NULL;          // ���_�o�b�t�@�ւ̃|�C���^
	m_Target = TARGET_TEXTURE;  // �I�u�W�F�N�g��`�悷�郌���_�����O�^�[�Q�b�g
	m_fAngle = 0.0f;            // �|���S���o���p�x
	m_fWidth = 0.0f;            // �|���S���̕�
	m_fHeight = 0.0f;           // �|���S���̍���
	m_fLength = 0.0f;           // �|���S���̒���
	m_bFeedbackEffect = false;  // �t�B�[�h�o�b�N�G�t�F�N�g���o�����ǂ���
	m_nFeedbackCounter = 0;     // �t�B�[�h�o�b�N�G�t�F�N�g���ǂꂭ�炢�o����
	m_fFeedbackScale = FEEDBACK_SCALE_INI;
	m_fFeedbackAlpha = FEEDBACK_ALPHA_INI;
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntTex = 0; nCntTex < MAX_RENDERER_TEXTURE; nCntTex++)
	{
		m_apTextureMT[nCntTex] = NULL;   // �e�N�X�`���ւ̃|�C���^
		m_apRenderMT[nCntTex] = NULL;    // �e�N�X�`���ɕ`�悷�邽�߂̃C���^�[�t�F�C�X�ւ̃|�C���^
		m_apBuffMT[nCntTex] = NULL;      // �e��o�b�t�@�ւ̃|�C���^
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							            // �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				            // �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				            // �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;			            	// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		                // �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						    // �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�v�X�o�b�t�@�Ƃ���24bit�A�X�e���V���o�b�t�@�Ƃ���8bit���g��
	d3dpp.Windowed = bWindow;						                // �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	    // �C���^�[�o��
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;              // �_�u���X�e���V��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);              // ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                      // Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);             // ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                     // ���C�e�B���O���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);        // ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // �Q�Ԗڂ̃A���t�@����

	// �I�u�W�F�N�g�����_�����O�p�̃e�N�X�`�����쐬
	CreateTexture();

	// �e�N�X�`���𒣂�t����|���S���𐶐�
	CreateVertexBuff();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRenderer::Uninit(void)
{
	// �|�[�Y�N���X�̔j��
	ReleasePause();

	// �����_�����O�p�̃e�N�X�`����j��
	ReleaseTexture();

	// ���_�o�b�t�@��j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �f�o�C�X�̔j��
	DIRECT_RELEASE(m_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̔j��
	DIRECT_RELEASE(m_pD3D);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	CManager::MODE mode = CManager::GetMode();  // ���݂̃��[�h���擾

	if (mode == CManager::MODE_GAME)
	{// �Q�[������������
		if (CManager::GetGame()->GetState() != CGame::STATE_END)
		{// �Q�[�����ł���
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h���Ă��Ȃ�������
				if (CManager::GetKeyboard()->GetTrigger(DIK_P) == true || CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger(CJoyStick::DIJS_BUTTON_11) == TRUE)
				{// �|�[�YON/OFF
					m_bPause = m_bPause ? false : true;
					if (m_pPause == NULL && m_bPause == true)
					{// �|�[�Y���Ƀ��������m�ۂ���Ă��Ȃ�
						m_pPause = CPause::Create(PAUSE_PRIORITY);
						CManager::GetGame()->BindPauseTexture(m_pPause);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
					}
					else if (m_pPause != NULL)
					{// �|�[�Y�������Ƀ��������m�ۂ���Ă���
					    // �|�[�Y�̊J��
						ReleasePause();
					}
				}
			}
		}
	}

	if (m_bPause == false)
	{// �|�[�Y���łȂ�
	    // �V�[���N���X�̍X�V����
		CScene::UpdateAll();
	}
	else
	{// �|�[�Y���ł���
		if (m_pPause != NULL)
		{// ���������m�ۂ���Ă���
		    // �|�[�Y�̍X�V����
			m_pPause->Update();
			CScene::DeathCheck();
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	if (m_Target == TARGET_DEFAULT)
	{// �ʏ�̕`��`���Ȃ�
		DefaultRender();
	}
	else if (m_Target == TARGET_TEXTURE)
	{// �^�[�Q�b�g���e�N�X�`���Ȃ��
		TextureRender();
	}
}

//=============================================================================
//    �ʏ�̕`��`��
//=============================================================================
void CRenderer::DefaultRender(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n(�����_�����O�^�[�Q�b�g�̓e�N�X�`��)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �J�����̐ݒ�
		CManager::GetCamera()->SetCamera();

		// �V�[���N���X�̕`�揈��
		CScene::DrawAll();

		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[�����[�h����������
			CManager::GetGame()->MiniMapDraw();
			if (m_pPause != NULL)
			{// �|�[�Y�̕`��
				m_pPause->Draw();

				// �ړI��UI������\������
				CUI *pUi = CManager::GetGame()->GetUI();
				if (pUi != NULL)
				{
					pUi->DrawUI_Purpose();
				}
			}
			CManager::GetFade()->Draw();
		}

		// �f�o�b�N���\��
		CDebugProc::Draw();

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    �e�N�X�`���������_�����O�^�[�Q�b�g�ɂ����ꍇ�̕`��`��
//=============================================================================
void CRenderer::TextureRender(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n(�����_�����O�^�[�Q�b�g�̓e�N�X�`��)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		DWORD Lighting;
		D3DXMATRIX mtxViewOrg, mtxProjOrg;
		D3DVIEWPORT9 ViewportOrg;
		LPDIRECT3DSURFACE9 pRenderOrg = NULL;
		LPDIRECT3DSURFACE9 pBuffOrg = NULL;

		// ���݂̏���ۑ�����
		m_pD3DDevice->GetRenderTarget(0, &pRenderOrg);
		m_pD3DDevice->GetDepthStencilSurface(&pBuffOrg);
		m_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);

		// �����_�����O���1���ڂ̃e�N�X�`���ɐݒ�
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);

		// 1���ڂ̃e�N�X�`�����N���A
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), RENDERER_WINDOW_COLOR, 1.0f, 0);

		// �J�����̐ݒ�
		CManager::GetCamera()->SetCamera();

		m_pD3DDevice->GetTransform(D3DTS_VIEW, &mtxViewOrg);
		m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &mtxProjOrg);
		m_pD3DDevice->GetViewport(&ViewportOrg);

		// �V�[���N���X�̕`�揈��
		CScene::DrawAll();

		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[�����[�h����������
			CManager::GetGame()->MiniMapDraw();
			if (m_pPause != NULL)
			{// �|�[�Y�̕`��
				m_pPause->Draw();

				// �ړI��UI������\������
				CUI *pUi = CManager::GetGame()->GetUI();
				if (pUi != NULL)
				{
					pUi->DrawUI_Purpose();
				}
			}
			CManager::GetFade()->Draw();
		}

		// ���C�e�B���O�̐ݒ���O��
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �t�B�[�h�o�b�N�G�t�F�N�g�̕`��
		if (m_bFeedbackEffect == true)
		{
			m_nFeedbackCounter--;
			FeedBackRender();

			if (m_nFeedbackCounter <= 0)
			{// �o���t���[�����Ȃ��Ȃ���
				m_nFeedbackCounter = 0;
				m_bFeedbackEffect = false;
			}
		}

		// �����_�����O����o�b�N�o�b�t�@�ɐݒ�
		m_pD3DDevice->SetRenderTarget(0, pRenderOrg);
		m_pD3DDevice->SetDepthStencilSurface(pBuffOrg);

		// 1���ڂ̃e�N�X�`�����|���S���ɐݒ肵�ĕ`��
		PolygonRender();

		// �}�g���b�N�X�̐ݒ��߂�
		m_pD3DDevice->SetViewport(&ViewportOrg);
		m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxViewOrg);
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjOrg);

		// ���C�e�B���O�̐ݒ��߂�
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

		// �f�o�b�O���̕`��
		CDebugProc::Draw();

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    �t�B�[�h�o�b�N�G�t�F�N�g��`�悷�鏈��
//=============================================================================
void CRenderer::FeedBackRender(void)
{
	VERTEX_3D *pVtx;
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
	D3DXMATRIX mtxView, mtxProj;

	// 2���ڂ̃e�N�X�`����1���ڂɉ��Z����`�ŕ`��
	// �r���[�|�[�g(�`��̈�)�̐ݒ�
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProj);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixOrthoLH(&mtxProj,
		(float)m_ViewportMT.Width,
		(float)m_ViewportMT.Height,
		1.0f,
		25000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&mtxView,
		&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// �r���[�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// 3D�|���S����`��
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_fFeedbackScale, m_fFeedbackScale, m_fFeedbackScale);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	m_pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);

	// Z�o�b�t�@���N���A����
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), RENDERER_WINDOW_COLOR, 1.0f, 0);

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFeedbackAlpha);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �|���S���̕`��
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
//    �����_�����O�p�̃e�N�X�`����\�����鏈��
//=============================================================================
void CRenderer::PolygonRender(void)
{
	VERTEX_3D *pVtx;
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;
	D3DXMATRIX mtxView, mtxProj;

	// 2���ڂ̃e�N�X�`����1���ڂɉ��Z����`�ŕ`��
	// �r���[�|�[�g(�`��̈�)�̐ݒ�
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProj);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixOrthoLH(&mtxProj,
		(float)m_ViewportMT.Width,
		(float)m_ViewportMT.Height,
		1.0f,
		25000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&mtxView,
		&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// �r���[�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// 3D�|���S����`��
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	m_pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pD3DDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);

	// Z�o�b�t�@���g�p���Ȃ�
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ���C�e�B���O�̐ݒ���O��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �|���S���̕`��
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// Z�o�b�t�@���g�p����
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// �e�N�X�`���̃|�C���^�����ւ���
	LPDIRECT3DTEXTURE9 pTextureMTWk = m_apTextureMT[1];
	LPDIRECT3DSURFACE9 pRenderMTWk = m_apRenderMT[1];
	LPDIRECT3DSURFACE9 pBuffMTWk = m_apBuffMT[1];
	m_apTextureMT[1] = m_apTextureMT[0];
	m_apRenderMT[1] = m_apRenderMT[0];
	m_apBuffMT[1] = m_apBuffMT[0];
	m_apTextureMT[0] = pTextureMTWk;
	m_apRenderMT[0] = pRenderMTWk;
	m_apBuffMT[0] = pBuffMTWk;
}

//=============================================================================
//    �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

//=============================================================================
//    �|�[�Y�̊J��
//=============================================================================
void CRenderer::ReleasePause(void)
{
	if (m_pPause != NULL)
	{// �|�[�Y�������Ƀ��������m�ۂ���Ă���
	    // �|�[�Y�̏I������
		m_pPause->Uninit();

		// �������̊J��
		delete m_pPause;
		m_pPause = NULL;
	}
}

//=============================================================================
//    �����_�����O�p�̃e�N�X�`�����J�����鏈��
//=============================================================================
void CRenderer::ReleaseTexture(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RENDERER_TEXTURE; nCntTex++)
	{
		DIRECT_RELEASE(m_apTextureMT[nCntTex]);
		DIRECT_RELEASE(m_apRenderMT[nCntTex]);
		DIRECT_RELEASE(m_apBuffMT[nCntTex]);
	}
}

//=============================================================================
//    �����_�����O�p�̃e�N�X�`�����쐬���鏈��
//=============================================================================
void CRenderer::CreateTexture(void)
{
	// �����_�����O�N���X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			LPDIRECT3DSURFACE9 pRenderOrg = NULL;
			LPDIRECT3DSURFACE9 pBuffOrg = NULL;

			// �f�t�H���g�l��ۑ�
			pDevice->GetRenderTarget(0, &pRenderOrg);
			pDevice->GetDepthStencilSurface(&pBuffOrg);

			// �`��̈��ݒ�
			m_ViewportMT.X = 0;
			m_ViewportMT.Y = 0;
			m_ViewportMT.Width = SCREEN_WIDTH;
			m_ViewportMT.Height = SCREEN_HEIGHT;
			m_ViewportMT.MinZ = 0.0f;
			m_ViewportMT.MaxZ = 1.0f;

			for (int nCntTex = 0; nCntTex < MAX_RENDERER_TEXTURE; nCntTex++)
			{
				// �����_�����O�p�̃e�N�X�`�����쐬
				pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1,
					D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_apTextureMT[nCntTex], NULL);

				// �e�N�X�`���̃T�[�t�F�C�X�����擾
				m_apTextureMT[nCntTex]->GetSurfaceLevel(0, &m_apRenderMT[nCntTex]);

				// �e�N�X�`���Ƀ����_�����O����Ƃ��Ɏg���o�b�t�@�𐶐�
				pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT,
					D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_apBuffMT[nCntTex], NULL);

				// �e�N�X�`���̃T�[�t�F�C�X����ݒ�
				pDevice->SetRenderTarget(0, m_apRenderMT[nCntTex]);
				pDevice->SetDepthStencilSurface(m_apBuffMT[nCntTex]);

				// �T�[�t�F�C�X�����N���A����
				pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

			}

			// �f�t�H���g�l�ɖ߂�
			pDevice->SetRenderTarget(0, pRenderOrg);
			pDevice->SetDepthStencilSurface(pBuffOrg);
		}
	}
}

//=============================================================================
//    �����_�����O�p��3D�|���S�����쐬���鏈��
//=============================================================================
void CRenderer::CreateVertexBuff(void)
{
	// ���_�o�b�t�@�𐶐�
	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 6.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 6.0f);
	pVtx[2].pos = D3DXVECTOR3(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 6.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 6.0f);

	// �@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f - 0.00128f, 0.0f - 0.00072f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f - 0.00072f);
	pVtx[2].tex = D3DXVECTOR2(0.0f - 0.00128f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �|�[�Y�̐ݒ�
//=============================================================================
void CRenderer::SetPause(bool bPause)
{
	m_bPause = bPause;
}

//=============================================================================
//    �|�[�Y�̎擾
//=============================================================================
bool CRenderer::GetPause(void)
{
	return m_bPause;
}

//=============================================================================
//    �t�B�[�h�o�b�N�G�t�F�N�g���o���悤�ɐݒ肷��
//=============================================================================
void CRenderer::SetFeedBack(int nFrame, float fScale, float fAlpha)
{
	m_bFeedbackEffect = true;
	m_nFeedbackCounter = nFrame;
	m_fFeedbackScale = fScale;
	m_fFeedbackAlpha = fAlpha;
}