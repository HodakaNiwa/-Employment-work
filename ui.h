//*****************************************************************************
//
//     UI�̏���[ui.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define UI_TIMER_PRIORITY        (11)    // �^�C�}�[�̏����D�揇��
#define UI_SCORE_PRIORITY        (11)    // �X�R�A�̏����D�揇��
#define UI_LIFEGAUGE_PRIORITY    (11)    // �̗̓Q�[�W�̏����D�揇��
#define UI_ORBITGAUGE_PRIORITY   (11)    // �O�ՃQ�[�W�̏����D�揇��
#define UI_SPECIALGAUGE_PRIORITY (11)    // �K�E�Q�[�W�̏����D�揇��
#define UI_PAPER_PRIORITY        (11)    // ���̏����D�揇��
#define UI_NUMSTAGE_PRIORITY     (11)    // �X�e�[�W���̏����D�揇��
#define UI_PURPOSE_PRIORITY      (11)    // �ړI�̏����D�揇��
#define UI_COMBO_PRIORITY        (11)    // �R���{UI�̏����D�揇��

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CTimer;
class CScore;
class CCombo;
class CGaugeLife2D;
class CGaugeOrbitSlash2D;
class CGaugeSpecial2D;
class CScene2D;
class CTextureManager;

//*****************************************************************************
//    UI�N���X�̒�`
//*****************************************************************************
class CUI
{
public:    // �N�ł��A�N�Z�X�\
	//-----------------
	//  ���
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,    // �ʏ���
		STATE_ALPHACHANGE,   // �����x�������Ă�����
		STATE_MAX
	}STATE;

	CUI();
	~CUI();

	static CUI *Create(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager);

	HRESULT Init(int nTimer, int nPlayerLife, float fPlayerOrbitSlash, float fPlayerSpecial, CTextureManager *pTextureManager);
	void Uninit(void);
	void Update(void);
	void SetDisp(const bool bDisp);
	bool GetDisp(void);
	void CreateCombo(void);
	void ReleaseCombo(void);

	CScore *GetScore(void);
	CTimer *GetTimer(void);
	CCombo *GetCombo(void);
	CScene2D *GetNumStage(void);
	CScene2D *GetPurpose(void);

private:   // ���̃N���X�������A�N�Z�X�\
	STATE              m_State;             // ���
	CScore             *m_pScore;           // �X�R�A�N���X�ւ̃|�C���^
	CTimer             *m_pTimer;           // �^�C�}�[�N���X�ւ̃|�C���^
	CCombo             *m_pCombo;           // �R���{�N���X�ւ̃|�C���^
	CGaugeLife2D       *m_pGaugeLife;       // �̗̓Q�[�W�N���X�ւ̃|�C���^
	CGaugeOrbitSlash2D *m_pGaugeOrbitSlash; // �O�ՃQ�[�W�N���X�ւ̃|�C���^
	CGaugeSpecial2D    *m_pGaugeSpecial;    // �K�E�Q�[�W�N���X�ւ̃|�C���^
	CScene2D           *m_pPaper;           // ���w�i�|���S���N���X�ւ̃|�C���^
	CScene2D           *m_pNumStage;        // ���݂̃X�e�[�W���|���S���N���X�ւ̃|�C���^
	CScene2D           *m_pPurpose;         // �ړI�̃|���S���N���X�ւ̃|�C���^
	CTextureManager    *m_pTextureManager;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	bool               m_bDisp;             // UI��`�悷�邩�ǂ���
};

#endif