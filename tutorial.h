//*****************************************************************************
//
//     �`���[�g���A���̏���[tutorial.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CTextureManager;
class CMap;
class CUI;
class CPlayer;
class CEffectManager;
class CMeshWall;
class CScene2DFlash;
class CScene2D;

//*****************************************************************************
//    �`���[�g���A���N���X�̒�`
//*****************************************************************************
class CTutorial : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		STATE_NONE = 0,    // �������Ă��Ȃ����
		STATE_MODECHECK,   // ��ʑJ�ڂ��邩�ǂ������`�F�b�N����
		STATE_MODENONE,    // ��ʑJ�ڂ��Ȃ����
		STATE_MODECHANGE,  // ��ʑJ�ڂ�����
		STATE_MAX
	}STATE;

	//--------------------
	//  ��ʑJ�ڃ`�F�b�N
	//--------------------
	typedef enum
	{
		SELECT_CHANGE = 0,
		SELECT_NONE,
		SELECT_MAX
	}SELECT;

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter�p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTutorial(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TUTORIAL);
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	void SetGoalCol(const bool bGoalCol);

	STATE GetState(void);
	CPlayer *GetPlayer(void);
	CMeshWall *GetGoalWall(void);
	bool GetGoalCol(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void CreateModeLogo(void);
	void ModeChangeCheck(void);
	void SetBoxCollider(void);

	STATE           m_State;                     // ���
	CPlayer         *m_pPlayer;                  // �v���C���[�N���X�ւ̃|�C���^(�ۑ��p)
	CUI             *m_pUi;                      // UI�N���X�ւ̃|�C���^
	CMap            *m_pMap;                     // �}�b�v�N���X�ւ̃|�C���^
	int             m_nCounter;                  // �J�ڂ��Ǘ�����J�E���^�[
	CEffectManager  *m_pEffectManager;           // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;          // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	CMeshWall       *m_pGoalWall;                // �S�[���p�̕�(�v���C���[�������ɓ���������J�n���邩�ǂ����I�΂���)
	bool            m_bGoalCol;                  // �S�[���Ƀv���C���[���G��Ă��邩�ǂ���
	CScene2D        *m_pCheck;                   // ������@�\���ؑ֕\���p�|���S��
	CScene2D        *m_pSkip;                    // �X�L�b�v�����\���p�|���S��
	CScene2D        *m_pOperationInfo[2];        // ������@�p�̃|���S��
	bool            m_bOperationInfo;            // ������@��\�����邩�ǂ���

	// ��ʑJ�ڃ`�F�b�N�p
	CScene2DFlash   *m_pSelectMode[SELECT_MAX];  // ��ʑJ�ڂ��邩�ǂ����̍��ڕ\���p�|���S��
	int             m_nSelectMode;               // ��ʑJ�ڂ��邩�ǂ�����I�ׂ�ԍ�
	CScene2D        *m_pLogo[3];                 // �����p�̕����|���S��
	CScene2D        *m_pBlackPoly;               // �`�F�b�N����Ƃ��ɕ\�����鍕���|���S��
	float           m_fModeChangeCol;            // ���S�|���S���̐F�̕ω���
};

#endif