//*****************************************************************************
//
//     �����L���O�̏���[ranking.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_RANKING  (5)    // �����L���O�X�R�A�̐�

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMap;
class CNumber;
class CScene2D;
class CTextureManager;

//*****************************************************************************
//    �����L���O�N���X�̒�`
//*****************************************************************************
class CRanking : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �����L���O�X�R�A�̏��
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // �����Ȃ����
		STATE_UPDATE,    // �X�V���ꂽ���
		STATE_MAX
	}STATE;

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // �����p
		TEX_NUMBER_1,      // ���ʗp
		TEX_NUMBER_2,      // �����L���O���S�p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CRanking(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_RANKING);
	~CRanking();

	static CRanking *Create(void);
	static void SetGameScore(int nScore);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Save(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void SlideRanking(void);
	void CreateOrbit(D3DXVECTOR3 pos, int nCntRanking);
	void SwitchCamera(void);

	static int      m_nGameScore;                   // �Q�[���̃n�C�X�R�A
	CMap            *m_pMap;                        // �}�b�v�N���X�ւ̃|�C���^
	CScene2D        *m_pRankingLogo;                // �����L���O���S�\���p�̃|���S��
	CScene2D        *m_apRankingRank[MAX_RANKING];  // �����L���O���ʕ\���p�̃|���S��
	CNumber         **m_apNumber[MAX_RANKING];      // �����L���O�X�R�A�\���p�̃|���S��
	int             m_aDigit[MAX_RANKING];          // �����L���O�X�R�A�̌���
	int             m_nModeCounter;                 // �J�ڂ��Ǘ�����J�E���^�[
	int             m_nCameraCounter;               // �J�����̓������Ǘ�����J�E���^�[
	int             m_nPatternCamera;               // ���݂̃����L���O�J�����̃p�^�[���ԍ�
	int             m_aRankingScore[MAX_RANKING];   // �����L���O�X�R�A
	STATE           m_aScoreState[MAX_RANKING];     // �����L���O�X�R�A�̏��
	D3DXVECTOR3     m_Move[MAX_RANKING];            // �����L���O�X�R�A�̈ړ���
	CTextureManager *m_pTextureManager;             // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^

	// �G�t�F�N�g��p�ϐ�
	D3DXVECTOR3     m_ParEmitterPos;                // �p�[�e�B�N�����o�����W(���X�ɉE�ɂ��炵�Ă���)
};

#endif