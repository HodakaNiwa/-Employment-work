//*****************************************************************************
//
//     �R���{��UI�̏���[combo.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _COMBO_H_
#define _COMBO_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_COMBO_NUMBER   (5)      // �R���{���̐����̍ő�l

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CNumber;
class CScene2D;

//*****************************************************************************
//     �R���{��UI�N���X�̒�`
//*****************************************************************************
class CCombo : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		STATE_APPEAR = 0,   // �o�����
		STATE_NORMAL,       // �ʏ���
		STAET_MAX
	}STATE;

	CCombo(int nPriority = 3, OBJTYPE objType = OBJTYPE_COMBOUI);
	~CCombo();

	static CCombo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumberTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetLogoTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetCombo(const int nCombo);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetCombo(void);

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9 m_pTexture;                     // �����e�N�X�`���ւ̃|�C���^
	STATE              m_State;                        // ���
	int                m_nCounter;                     // ��ԊǗ��J�E���^�[
	D3DXVECTOR3        m_Pos;                          // �R���{���̍��W
	D3DXCOLOR          m_Col;                          // �R���{���̐F
	float              m_fWidth;                       // �����|���S���P���̕�
	float              m_fHeight;                      // �����|���S���P���̍���
	int                m_nCombo;                       // ���݂̃R���{��
	int                m_nDigit;                       // ���݂̃R���{���̍ő�l
	CScene2D           *m_pComboLogo[3];               // UI�p�̕����|���S���N���X�ւ̃|�C���^
	CNumber            *m_apNumber[MAX_COMBO_NUMBER];  // �����|���S���N���X�^�̃|�C���^
};

#endif