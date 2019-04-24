//*****************************************************************************
//
//     �Q�[�W�̏���[gauge.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2D.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    �Q�[�W�N���X�̒�`
//*****************************************************************************
class CGauge : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CGauge(int nPriority = 3, OBJTYPE objType = OBJTYPE_GAUGE);
	~CGauge();

	static CGauge *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetGaugeRate(const float fGaugeRate);
	void SetLeftWidth(const float fLeftWidth);

	float GetGaugeRate(void);
	float GetLeftWidth(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fGaugeRate;    // �Q�[�W�̊���
	float m_fLeftWidth;    // �Q�[�W�̍����̒���
};

//*****************************************************************************
//    �̗̓Q�[�W�N���X�̒�`
//*****************************************************************************
class CGaugeLife : public CGauge
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,
		STATE_CUT,
		STATE_ADD,
		STATE_MAX
	}STATE;

	CGaugeLife(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE);
	~CGaugeLife();

	static CGaugeLife *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const int nCutValue);
	void AddGauge(const int nAddValue);

	void SetGaugePos(const D3DXVECTOR3 pos);
	void SetState(const STATE State);
	void SetLife(const int nLife);

	STATE GetState(void);
	int GetLife(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int             m_nCounter;      // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	STATE           m_State;         // ���
	int             m_nLife;         // �̗�
	CGauge         *m_pWhiteBill;    // �̗̓Q�[�W�̃t���[��
	CGauge         *m_pRedBill;      // �̗̓Q�[�W�����������ɕ\������Ԃ��Q�[�W
};


//*****************************************************************************
//    �Q�[�W�N���X(2D)�̒�`
//*****************************************************************************
class CGauge2D : public CScene2D
{
public:    // �N�ł��A�N�Z�X�\
	CGauge2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE);
	~CGauge2D();

	static CGauge2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetFrameTexture(LPDIRECT3DTEXTURE9 pFrameTexture);
	void SetGaugeRate(const float fGaugeRate);
	void SetLeftWidth(const float fLeftWidth);

	LPDIRECT3DTEXTURE9 GetFrameTexture(void);
	float GetGaugeRate(void);
	float GetLeftWidth(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9 m_pFrameTexture;   // �Q�[�W�t���[����p�̃e�N�X�`��
	float              m_fGaugeRate;      // �Q�[�W�̊���
	float              m_fLeftWidth;      // �Q�[�W�̍����̒���
};

//*****************************************************************************
//    �̗̓Q�[�W�N���X(2D)�̒�`
//*****************************************************************************
class CGaugeLife2D : public CGauge2D
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,
		STATE_CUT,
		STATE_ADD,
		STATE_MAX
	}STATE;

	CGaugeLife2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE2D);
	~CGaugeLife2D();

	static CGaugeLife2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nLife, LPDIRECT3DTEXTURE9 pFrameTexture = NULL, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const int nCutValue);
	void AddGauge(const int nAddValue);
	void ChangeAlpha(const float fAlpha);

	void SetState(const STATE State);
	void SetLife(const int nLife);

	STATE GetState(void);
	int GetLife(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int             m_nCounter;      // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	STATE           m_State;         // ���
	int             m_nLife;         // �̗�
	CGauge2D        *m_pBlack2D;     // �̗̓Q�[�W�̍��������p
	CGauge2D        *m_pFrame2D;     // �̗̓Q�[�W�̃t���[��
	CGauge2D        *m_pRed2D;       // �̗̓Q�[�W�����������ɕ\������Ԃ��Q�[�W
};

//*****************************************************************************
//    �O�ՃQ�[�W�N���X(2D)�̒�`
//*****************************************************************************
class CGaugeOrbitSlash2D : public CGauge2D
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,   // �������Ȃ����
		STATE_CUT,        // �����Ă�����
		STATE_ADD,        // �����Ă�����
		STATE_MAX
	}STATE;

	CGaugeOrbitSlash2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_SLASHGAUGE2D);
	~CGaugeOrbitSlash2D();

	static CGaugeOrbitSlash2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fOrbitSlash, LPDIRECT3DTEXTURE9 pFrameTexture = NULL, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const float fCutValue);
	void AddGauge(const float fAddValue);
	void DeleteGauge(void);
	void ChangeAlpha(const float fAlpha);

	void SetState(const STATE State);
	void SetOrbitSlash(const float fOrbitSlash);

	STATE GetState(void);
	float GetOrbitSlash(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int             m_nCounter;      // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	STATE           m_State;         // ���
	float           m_fOrbitSlash;   // �O�ՃQ�[�W��
	CGauge2D        *m_pBlack2D;     // �O�ՃQ�[�W�̍��������p
	CGauge2D        *m_pFrame2D;     // �O�ՃQ�[�W�̃t���[��
};

//*****************************************************************************
//    �K�E�Z�Q�[�W�N���X(2D)�̒�`
//*****************************************************************************
class CGaugeSpecial2D : public CGauge2D
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,   // �������Ȃ����
		STATE_CUT,        // �����Ă�����
		STATE_ADD,        // �����Ă�����
		STATE_MAX
	}STATE;

	CGaugeSpecial2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_SPECIALGAUGE2D);
	~CGaugeSpecial2D();

	static CGaugeSpecial2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, float fMaxSpecial, float fSpecial, LPDIRECT3DTEXTURE9 pFrameTexture = NULL, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const float fCutValue);
	void AddGauge(const float fAddValue);
	void MaxGauge(void);
	void DeleteGauge(void);
	void ChangeAlpha(const float fAlpha);

	void SetState(const STATE State);
	void SetSpecial(const float fSpecial);
	void SetMaxSpecial(const float fMaxSpecial);

	STATE GetState(void);
	float GetSpecial(void);
	float GetMaxSpecial(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int             m_nCounter;      // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	STATE           m_State;         // ���
	float           m_fMaxSpecial;   // �K�E�Q�[�W�̍ő�l
	float           m_fSpecial;      // �K�E�Z�Q�[�W��
	CGauge2D        *m_pBlack2D;     // �K�E�Z�Q�[�W�̍��������p
	CGauge2D        *m_pFrame2D;     // �K�E�Z�Q�[�W�̃t���[��
};


#endif