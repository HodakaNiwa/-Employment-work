//*****************************************************************************
//
//     �e�̏���[shadow.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    �e(�X�v���C�g)�N���X�̒�`
//*****************************************************************************
class CShadow : public CScene3D
{
public:    // �N�ł��A�N�Z�X�\
	CShadow(int nPriority = 3, OBJTYPE objType = OBJTYPE_3DPOLYGON);
	~CShadow();

	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetVertexHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pLandNormal);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_pShadowTexture;     // �e�p�̃e�N�X�`��
};

#endif