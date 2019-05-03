//*****************************************************************************
//
//     ���b�N�I���p�|���S���̏���[lockon_poly.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LOCKON_POLY_H_
#define _LOCKON_POLY_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    ���b�N�I���p�|���S���N���X�̒�`
//*****************************************************************************
class CLockOnPoly : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CLockOnPoly(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARD);
	~CLockOnPoly();

	static CLockOnPoly *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting = false, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting = false);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

#endif