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
#include "sceneX.h"

//*****************************************************************************
//    ���f���N���X(�V�[���N���X�h��)�̒�`
//*****************************************************************************
class CShadow : public CSceneX
{
public:    // �N�ł��A�N�Z�X�\
	CShadow(int nPriority = 3, OBJTYPE objType = OBJTYPE_SHADOW);
	~CShadow();

	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPD3DXMESH   m_pMesh;     // ���b�V���ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMat;  // �}�e���A�����ւ̃|�C���^
	static DWORD        m_nNumMat;   // �}�e���A�����̐�
};

#endif