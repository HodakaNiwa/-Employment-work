//*****************************************************************************
//
//     ���̏���[grass.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GRASS_H_
#define _GRASS_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    ���N���X�̒�`
//*****************************************************************************
class CGrass : public CScene3D
{
public:    // �N�ł��A�N�Z�X�\
	//-----------------
	//  ���
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,    // �ʏ���
		STATE_COL_LEFT,      // �����瓖�����Ă�����
		STATE_COL_RIGHT,     // �E���瓖�����Ă�����
		STATE_MAX
	}STATE;

	CGrass(int nPriority = 3, OBJTYPE objType = OBJTYPE_GRASS);
	~CGrass();

	static CGrass *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, bool bSway, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, float fRange);

	void SetSway(const bool bSway);
	bool GetSway(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	STATE       m_State;             // ���
	D3DXVECTOR3 m_aOffsetVertex[2];  // �{�[���ɑ΂���I�t�Z�b�g
	D3DXVECTOR3 m_BonePos;           // �{�[���̍��W
	D3DXVECTOR3 m_BonePosDef;        // �{�[���̍��W(�f�t�H���g�l)
	D3DXVECTOR3 m_BoneRot;           // �{�[���̌���
	D3DXMATRIX  m_BoneMtxWorld;      // �{�[���̃��[���h�}�g���b�N�X
	int         m_nStateCounter;     // ��Ԃ��Ǘ�����J�E���^�[
	D3DXVECTOR3 m_SwayMove;          // ���̗h����
	D3DXVECTOR3 m_SwayMoveCurrent;   // ���݂̑��̗h����
	float       m_fRivisionSway;     // ���̗h���ʌv�Z�p
	int         m_nSwayCounter;      // ���̗h����Ǘ�����J�E���^�[
	int         m_nMaxSwayCounter;   // ���̗h����Ǘ�����J�E���^�[�̍ő�l
	bool        m_bSway;             // ����ɗh��邩�ǂ���
	float       m_fCameraLength;     // �J�����Ƃ̋����ۑ��p
};

#endif