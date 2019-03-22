//*****************************************************************************
//
//     �p�[�e�B�N��(2D)�̏���[particle2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    �p�[�e�B�N��(2D)�N���X�̒�`
//*****************************************************************************
class CParticle2D : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	CParticle2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLE2D);
	~CParticle2D();

	static CParticle2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col, float fWidth,
		float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
		float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col, float fWidth,
		float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
		float fChangeRot, int nLife, int nRotPattern, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(const D3DXVECTOR3 move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetLife(const int nLife);
	void SetRotPattern(const int nRotPattern);
	void SetSpread(const float fSpread);
	void SetChangeRot(const float fChangeRot);
	void SetDrawAddtive(const bool bDrawAddtive);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	int GetLife(void);
	int GetRotPattern(void);
	float GetSpread(void);
	float GetChangeRot(void);
	bool GetDrawAddtive(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_Move;           // �ړ���
	D3DXVECTOR3 m_ChangeMove;     // �ړ��ʂ̕ω���
	D3DXCOLOR   m_ChangeCol;      // �F�̕ω���
	int         m_nLife;          // ����
	int         m_nRotPattern;    // ��]�̎��
	float       m_fSpread;        // �傫���̕ω���
	float       m_fChangeRot;     // ��]�̕ω���
	bool        m_bDrawAddtive;   // ���Z�����ŕ`�悷�邩���Ȃ���
};

#endif