//*****************************************************************************
//
//     �Q�[���̏���[game.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define GAME_MAX_MAPLOAD     (10)    // �Q�[���̃}�b�v��ǂݍ��߂鐔

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2D;
class CPause;
class CTextureManager;
class CMap;
class CUI;
class CPlayer;
class CEffectManager;

//*****************************************************************************
//    �Q�[���N���X�̒�`
//*****************************************************************************
class CGame : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//-----------------
	//  ���
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,    // �ʏ���
		STATE_MAPCHANGE,     // �}�b�v�؂�ւ����
		STATE_MAPCLEAR,      // �}�b�v�N���A���
		STATE_BOSSAPPEAR,    // �{�X�o�����
		STATE_KNOCKDOWNBOSS, // �{�X��|�������
		STATE_END,           // �I�����
		STATE_MAX
	}STATE;

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // �|�[�Y�w�i�p
		TEX_NUMBER_1,      // �|�[�Y�I�����ڗp(CONTINUE)
		TEX_NUMBER_2,      // �|�[�Y�I�����ڗp(RETRY)
		TEX_NUMBER_3,      // �|�[�Y�I�����ڗp(QUIT)
		TEX_NUMBER_4,      // ����
		TEX_NUMBER_5,      // �R���{��UI����
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CGame(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_GAME);
	~CGame();

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MiniMapDraw(void);
	void ClearMapEvent(void);

	void SetPlayer(CPlayer *pPlayer);
	void BindPauseTexture(CPause *pPause);
	void SetMapClear(bool bMapClear);
	void SetMap(void);
	void SetState(STATE state);
	void SetHiddenItem(const int nGetHiddenItem);
	void SetComboMax(const int nComboMax);
	void SetBossPos(const D3DXVECTOR3 BossPos);

	CUI *GetUI(void);
	CEffectManager *GetEffectManager(void);
	CPlayer *GetPlayer(void);
	STATE GetState(void);
	bool GetMapClear(void);
	int GetMapIdx(void);
	int GetNumMap(void);
	int GetHiddenItem(void);
	int GetComboMax(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void BossAppearEvent(void);
	void KonckDownBossEvent(void);
	void SetBridgeCollider(void);

	void SaveGrassTransform(void);
	void LoadGrassTransform(char *pStrCur, char *pLine);

	CPlayer         *m_pPlayer;                               // �v���C���[�N���X�ւ̃|�C���^(�ۑ��p)
	CUI             *m_pUi;                                   // UI�N���X�ւ̃|�C���^
	CMap            *m_pMap;                                  // �}�b�v�N���X�ւ̃|�C���^
	char            m_aMapFileName[GAME_MAX_MAPLOAD][256];    // �ǂݍ��ރ}�b�v�f�[�^�̃X�N���v�g�t�@�C����
	STATE           m_State;                                  // ���
	int             m_nStateCounter;                          // ��Ԃ��Ǘ�����J�E���^�[
	CEffectManager  *m_pEffectManager;                        // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;                       // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	int             m_nClearStageNum;                         // �N���A�����X�e�[�W�̐�
	int             m_nGetHiddenItem;                         // ��ɓ��ꂽ�B���A�C�e���̐�
	int             m_nComboMax;                              // �Q�[����ʂ����ő�R���{��
	int             m_nNumMap;                                // �ǂݍ��ރ}�b�v�̐�
	int             m_nMapIdx;                                // ���݂̃}�b�v�ԍ�
	bool            m_bMapClear;                              // �}�b�v���N���A�������ǂ���
	bool            m_bMiniMapDisp;                           // �~�j�}�b�v��`�悷�邩�ǂ���
	CScene2D        *m_pPlayerPosition;                       // �v���C���[�̍��W������2D�|���S��

	// �{�X�o�����o�p�ϐ�
	CScene2D        *m_pBossAppearLogo[2];                    // �{�X�o�����ɏo�������|���S��
	int             m_nBossAppearCounter;                     // �{�X�o�����̉��o���Ǘ�����J�E���^�[

	// �{�X���j���o�p�ϐ�
	D3DXVECTOR3     m_BossPos;                                // �{�X�̍��W(�ۑ��p)
	int             m_nKnockDownCounter;                      // �{�X���j���̉��o���Ǘ�����J�E���^�[
};

#endif