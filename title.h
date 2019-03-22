//*****************************************************************************
//
//     �^�C�g���̏���[title.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2D;
class CScene2DPress;
class CMesh2D;
class CTextureManager;
class CEffectManager;
class CMap;

//*****************************************************************************
//    �^�C�g���N���X�̒�`
//*****************************************************************************
class CTitle : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		STATE_NONE = 0,        // �����o�Ă��Ȃ����
		STATE_ALPHACUT_BLACK,  // �����|���S���̓����x����������
		STATE_STOP_LOGO,       // ���S�|���S�����o��܂őҋ@������
		STATE_APPEAR_LOGO,     // �^�C�g�����S���o�����
		STATE_FADE_OUT,        // �t�F�[�h�A�E�g���J�n�������
		STATE_FADE_STOP,       // �t�F�[�h���X�g�b�v��������J�n�������
		STATE_FADE_IN,         // �t�F�[�h�C�����J�n�������
		STATE_OK_CHANGEMODE,   // ��ʑJ�ڂ��ł����ԏ��
		STATE_MAX
	}STATE;

	//--------------------
	//  �^�C�g�����S�̐�
	//--------------------
	typedef enum
	{
		LOGOTYPE_0 = 0,
		LOGOTYPE_1,
		LOGOTYPE_MAX
	}LOGOTYPE;

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // �^�C�g�����S�P�ڗp
		TEX_NUMBER_1,      // �^�C�g�����S�Q�ڗp
		TEX_NUMBER_2,      // PressEnter�p
		TEX_NUMBER_3,      // �p�[�e�B�N���p
		TEX_NUMBER_4,      // �O�ՃG�t�F�N�g�p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTitle(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TITLE);
	~CTitle();

	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	STATE GetState(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void StartCreate(void);
	void AlphaCutBlack(void);
	void LogoCutScale(void);
	void FadeOut(void);
	void FadeStop(void);
	void FadeIn(void);
	void CheckModeChange(void);
	void SwitchCamera(void);
	void CreateOrbit(void);

	STATE           m_State;                        // ���
	CMap            *m_pMap;                        // �}�b�v�N���X�ւ̃|�C���^
	int             m_nCounter;                     // �J�����̓������Ǘ�����J�E���^�[
	int             m_nModeCounter;                 // �J�ڂ��Ǘ�����J�E���^�[
	int             m_nPatternCamera;               // ���݂̃^�C�g���J�����̃p�^�[���ԍ�
	int             m_nPatternOrbit;                // ���݂̋O�ՃG�t�F�N�g�p�^�[���ԍ�
	char            m_aMapFileName[256];            // ��������}�b�v�̖��O
	CScene2D        *m_pTitleLogo[LOGOTYPE_MAX];    // �^�C�g�����S�p��2D�|���S���N���X�ւ̃|�C���^
	CScene2DPress   *m_pPress;                      // �v���X�G���^�[�p��2D�|���S���N���X�ւ̃|�C���^
	CEffectManager  *m_pEffectManager;              // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;             // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	CScene2D        *m_pWhitePoly;                  // �t�F�[�h�C���Ɏg�p����|���S��
	float           m_fCutScaleValue;               // ���S�̑傫�������炵�Ă����{��
	CMesh2D         *m_pBlackMesh;                  // ���o�Ɏg�p���鍕���|���S��

	// �G�t�F�N�g�p�ϐ�
	int             m_nEffectCounter;               // �G�t�F�N�g���Ǘ�����J�E���^�[
	int             m_nEffectSwicthTiming;          // �G�t�F�N�g�̃p�^�[����؂�ւ���^�C�~���O
	D3DXVECTOR3     m_ParEmitterPos;                // �p�[�e�B�N�����o�����W(���X�ɉE�ɂ��炵�Ă���)
	D3DXVECTOR3     m_ParEmitterDiffPos;            // �p�[�e�B�N�����o���ړI�̈ʒu�Ƃ̍���
	D3DXCOLOR       m_ParCol;                       // �p�[�e�B�N���̐F
};

#endif