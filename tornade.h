//*****************************************************************************
//
//     �����̏���[tornade.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TORNADE_H_
#define _TORNADE_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define TORNADE_EFFECT_NUM (11)   // �����G�t�F�N�g�̏o���鐔

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CEffectManager;
class CEmitter;
class CPlayer;

//*****************************************************************************
//     �����N���X�̒�`
//*****************************************************************************
class CTornade : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_HORMING = 0,   // �z�[�~���O������
		STATE_NONE_HORMING,  // �z�[�~���O���Ȃ����
		STATE_MAX
	}STATE;

	CTornade(int nPriority = 3, OBJTYPE objType = OBJTYPE_TORNADE);
	~CTornade();

	static CTornade *Create(D3DXVECTOR3 pos, CEffectManager *pEffectManager, float fDestAngle, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(CPlayer *pPlayer);

	void SetPos(const D3DXVECTOR3 pos);
	void SetAngle(const float fAngle);
	void SetEffectManager(CEffectManager *pEffectManager);

	D3DXVECTOR3 GetPos(void);
	float GetAngle(void);
	CEffectManager *GetEffectManager(void);

private:   // ���̃N���X�������A�N�Z�X�\
	int            m_nEffectCounter;                        // �G�t�F�N�g���o���^�C�~���O���Ǘ�����J�E���^�[
	CEffectManager *m_pEffectManager;                       // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	STATE          m_State;                                 // ���
	int            m_nLife;                                 // ����(�ǔ����I�������猸�炵�n�߂�)
	D3DXVECTOR3    m_Pos;                                   // ���W
	CEmitter       *m_pTornadeEmitter[TORNADE_EFFECT_NUM];  // �G�t�F�N�g�����G�~�b�^�N���X�ւ̃|�C���^(�ŏ��ɐ����������̂����Ă������邽��)
	float          m_fAngle;                                // ���݂̌���
	float          m_fDestAngle;                            // �ړI�̌���
	float          m_fDiffAngle;                            // �ړI�̌����ƌ��݂̌����̍���
	float          m_fRivisionAngle;                        // �ǂꂾ��������␳�������ǂ���
};

#endif