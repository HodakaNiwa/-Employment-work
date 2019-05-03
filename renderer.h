//*****************************************************************************
//
//     �����_�����O�̏���[renderer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_RENDERER_TEXTURE      (2)       // �����_�����O�Ɏg�p����e�N�X�`���̑���

//*****************************************************************************
//    �O���錾(���̂��Ȃ��錾)
//*****************************************************************************
class CPause;

//*****************************************************************************
//    �����_�����O�N���X�̒�`
//*****************************************************************************
class CRenderer
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		TARGET_DEFAULT = 0,   // �f�t�H���g�̐ݒ�
		TARGET_TEXTURE,       // �e�N�X�`��
		TARGET_MAX
	}TARGET;

	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFeedBack(int nFrame, float fScale = 0.995f, float fAlpha = 0.85f);

	LPDIRECT3DDEVICE9 GetDevice(void);
	void ReleasePause(void);

	void SetPause(bool bPause);
	bool GetPause(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void ReleaseTexture(void);

	void DefaultRender(void);
	void TextureRender(void);
	void FeedBackRender(void);
	void PolygonRender(void);

	void CreateTexture(void);
	void CreateVertexBuff(void);

	CPause                  *m_pPause;                              // �|�[�Y�N���X�^�̃|�C���^
	bool                    m_bPause;                               // �|�[�Y�����ǂ���
	LPDIRECT3D9				m_pD3D;			                        // Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;	                        // Device�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DTEXTURE9      m_apTextureMT[MAX_RENDERER_TEXTURE];    // �e�N�X�`���ւ̃|�C���^(���̃e�N�X�`���ɃI�u�W�F�N�g��`�悷��)
	LPDIRECT3DSURFACE9      m_apRenderMT[MAX_RENDERER_TEXTURE];     // �e�N�X�`���ɕ`�悷�邽�߂̃C���^�[�t�F�C�X�ւ̃|�C���^
	LPDIRECT3DSURFACE9      m_apBuffMT[MAX_RENDERER_TEXTURE];       // �e��o�b�t�@�ւ̃|�C���^(�e�N�X�`���Ƀ����_�����O����ۂ�Z�o�b�t�@��؂�ւ��邽��)
	D3DVIEWPORT9            m_ViewportMT;                           // �e�N�X�`�������_�����O�p�̕`��̈�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                             // ���_�o�b�t�@�ւ̃|�C���^(�I�u�W�F�N�g��`�悵���e�N�X�`�����g�p����)
	TARGET                  m_Target;                               // �I�u�W�F�N�g��`�悷�郌���_�����O�^�[�Q�b�g
	D3DXVECTOR3             m_Rot;                                  // �|���S���̌���
	float                   m_fAngle;                               // �|���S�����o���p�x
	float                   m_fWidth;                               // �|���S���̕�
	float                   m_fHeight;                              // �|���S���̍���
	float                   m_fLength;                              // �|���S���̒���
	bool                    m_bFeedbackEffect;                      // �t�B�[�h�o�b�N�G�t�F�N�g���o�����ǂ���
	int                     m_nFeedbackCounter;                     // �t�B�[�h�o�b�N�G�t�F�N�g���ǂꂭ�炢�o����
	float                   m_fFeedbackScale;                       // �t�B�[�h�o�b�N�G�t�F�N�g��������|���S���̑傫��
	float                   m_fFeedbackAlpha;                       // �t�B�[�h�o�b�N�G�t�F�N�g��������|���S���̓����x
};

#endif