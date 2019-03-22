//*****************************************************************************
//
//     ���U���g�̏���[result.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMap;
class CScene2D;
class CScene2DPress;
class CTextureManager;
class CNumber;

//*****************************************************************************
//    ���U���g�N���X�̒�`
//*****************************************************************************
class CResult : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//--------------------
	//  �X�R�A�̎��
	//--------------------
	typedef enum
	{
		TYPE_SCORE_GAMESCORE = 0,       // �Q�[���̃X�R�A
		TYPE_SCORE_STAGECLEAR_BONUS,    // �X�e�[�W�N���A�{�[�i�X
		TYPE_SCORE_COMBOMAX_BONUS,      // �ő�R���{���{�[�i�X
		TYPE_SCORE_LASTBOSS_BONUS,      // ���X�{�X���j�{�[�i�X
		TYPE_SCORE_HIDDENITEM_BONUS,    // �B���A�C�e���擾�{�[�i�X
		TYPE_SCORE_ALLSCORE,            // ���v�X�R�A
		TYPE_SCORE_MAX
	}TYPE_SCORE;

	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		STATE_NONE = 0,                   // �����Ȃ����
		STATE_APPEAR_GAME_SCORE,          // �Q�[���̃X�R�A�܂ł�\�����Ă�����
		STATE_APPEAR_STAGECLEAR_BONUS,    // �X�e�[�W�N���A�{�[�i�X�܂ł�\�����Ă�����
		STATE_APPEAR_COMBOMAX_BONUS,      // �ő�R���{�{�[�i�X�܂ł�\�����Ă�����
		STATE_APPEAR_LASTBOSS_BONUS,      // ���X�{�X���j�{�[�i�X�܂ł�\�����Ă�����
		STATE_APPEAR_HIDDENITEM_BONUS,    // �B���A�C�e���{�[�i�X�܂ł�\�����Ă�����
		STATE_APPEAR_ALLSCORE,            // ���v�X�R�A�܂ł�\�����Ă�����(�J�ڂł����Ԃł�����)
		STATE_MAX
	}STATE;

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // �Q�[���̃X�R�A�p
		TEX_NUMBER_1,      // �X�e�[�W�N���A�{�[�i�X�p
		TEX_NUMBER_2,      // �ő�R���{�{�[�i�X�p
		TEX_NUMBER_3,      // ���X�{�X���j�{�[�i�X�p
		TEX_NUMBER_4,      // �B���A�C�e���{�[�i�X�p
		TEX_NUMBER_5,      // ���v�X�R�A�p
		TEX_NUMBER_6,      // �w�i�p
		TEX_NUMBER_7,      // PressEnter�p
		TEX_NUMBER_8,      // �����p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CResult(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_RESULT);
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetGameScore(int nGameScore);
	static void SetStageClearBonus(int nStageBonus);
	static void SetComboMaxBonus(int nComboMaxBonus);
	static void SetLastBossBonus(int nLastBossBonus);
	static void SetHiddenItemBonus(int nHiddenItemBonus);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void ModeChangeCheck(void);
	void CreateNumber(void);

	static int      m_nGameScore;           // �Q�[���̃X�R�A
	static int      m_nStageClearBonus;     // �X�e�[�W�N���A�{�[�i�X
	static int      m_nComboMaxBonus;       // �ő�R���{�{�[�i�X
	static int      m_nLastBossBonus;       // ���X�{�X���j�{�[�i�X
	static int      m_nHiddenItemBonus;     // �X�e�[�W�N���A�{�[�i�X

	STATE           m_State;                         // ���
	int             m_nAllScore;                     // ���v�X�R�A
	int             m_nCounter;                      // ��Ԃ��Ǘ�����J�E���^�[
	CNumber         **m_apNumber[TYPE_SCORE_MAX];    // ���ꂼ��̃X�R�A�\���p�̃|���S��
	int             m_aDigit[TYPE_SCORE_MAX];        // ���ꂼ��̃X�R�A�̌���
	CScene2D        *m_apScoreLogo[TYPE_SCORE_MAX];  // ���ꂼ��̃X�R�A�̃��S�\���p�|���S��
	CScene2DPress   *m_pPress;                       // �v���X�G���^�[�p��2D�|���S���N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;              // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	CScene2D        *m_pResultBg;                    // �w�i�p�̃|���S��
};

#endif