//*****************************************************************************
//
//		��̏���[river.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RIVER_H_
#define _RIVER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshField.h"

//*****************************************************************************
//    ��N���X�̒�`
//*****************************************************************************
class CRiver : public CMeshField
{
public:    // �N�ł��A�N�Z�X�\
	CRiver(int nPriority = 3, OBJTYPE objType = OBJTYPE_RIVER);
	~CRiver();

	static CRiver *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, float fAddTexU, float fAddTexV, char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAddTexU(const float fAddTexU);
	void SetAddTexV(const float fAddTexV);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fAddTexU;  // �e�N�X�`��U���W���ǂꂭ�炢��������
	float m_fAddTexV;  // �e�N�X�`��V���W���ǂꂭ�炢��������
	float m_fTexU;     // �e�N�X�`����U���W
	float m_fTexV;     // �e�N�X�`����V���W

};

#endif