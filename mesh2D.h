//*****************************************************************************
//
//     ���b�V��2D�|���S���̏���[mesh2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESH2D_H_
#define _MESH2D_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    ���b�V��2D�|���S���N���X�̒�`
//*****************************************************************************
class CMesh2D : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMesh2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESH2D);
	~CMesh2D();

	static CMesh2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	              // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;                    // ���b�V��2D�|���S���̍��W
	D3DXCOLOR               m_Col;                    // ���b�V��2D�|���S���̐F
	float                   m_fWidth;                 // 1�u���b�N���̕�
	float                   m_fHeight;                // 1�u���b�N������
	int                     m_nXBlock;                // ���̕�����
	int                     m_nYBlock;                // �c�̕�����
	int                     m_nTexSplitU;             // �e�N�X�`��U���W�̕�����
	int                     m_nTexSplitV;             // �e�N�X�`��V���W�̕�����
	int                     m_nNumVertex;             // ���_��
	int                     m_nNumIndex;              // �C���f�b�N�X��
	int                     m_nNumPolygon;            // �|���S����
};

#endif