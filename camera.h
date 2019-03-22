//*****************************************************************************
//
//     �J�����̏���[camera.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CPlayer;
class CScene;

//*****************************************************************************
//    �J�����N���X�̒�`
//*****************************************************************************
class CCamera
{
public:    // �N�ł��A�N�Z�X�\
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosVDest(const D3DXVECTOR3 posVDest);
	void SetPosVDiff(const D3DXVECTOR3 posVDiff);
	void SetPosVOld(const D3DXVECTOR3 posVOld);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetPosRDest(const D3DXVECTOR3 posRDest);
	void SetPosRDiff(const D3DXVECTOR3 posRDiff);
	void SetMoveV(const D3DXVECTOR3 MoveV);
	void SetMoveR(const D3DXVECTOR3 MoveR);
	void SetRot(const D3DXVECTOR3 Rot);
	void SetRotDest(const D3DXVECTOR3 RotDest);
	void SetRotDiff(const D3DXVECTOR3 RotDiff);
	void SetRotOld(const D3DXVECTOR3 RotOld);
	void SetMtxProjection(const D3DXMATRIX MtxProjection);
	void SetMtxView(const D3DXMATRIX MtxView);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosVDest(void);
	D3DXVECTOR3 GetPosVDiff(void);
	D3DXVECTOR3 GetPosVOld(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetPosRDest(void);
	D3DXVECTOR3 GetPosRDiff(void);
	D3DXVECTOR3 GetMoveV(void);
	D3DXVECTOR3 GetMoveR(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void);
	D3DXVECTOR3 GetRotDiff(void);
	D3DXVECTOR3 GetRotOld(void);
	D3DXMATRIX GetMtxProjection(void);
	D3DXMATRIX GetMtxView(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	bool FieldCollision(void);
	bool WallCollision(void);
	void Title(void);
	void Tutorial(void);
	void Game(void);
	void Result(void);
	void Ranking(void);
	void CalcRot(void);
	void MovieCamera(void);

	CPlayer      *m_pPlayer;        // �v���C���[�N���X�ւ̃|�C���^
	CScene       *m_pLandScene;     // ���݃J����������Ă���I�u�W�F�N�g�N���X�ւ̃|�C���^
	CScene       *m_pLandSceneOld;  // �O��J����������Ă���I�u�W�F�N�g�N���X�ւ̃|�C���^
	D3DXVECTOR3  m_PosV;            // ���݂̎��_
	D3DXVECTOR3  m_PosVDest;        // �ړI�̎��_
	D3DXVECTOR3  m_PosVDiff;        // ���݂̎��_�ƖړI�̎��_�̍���
	D3DXVECTOR3  m_PosVOld;         // �O��̎��_
	D3DXVECTOR3  m_PosR;            // ���݂̒����_
	D3DXVECTOR3  m_PosRDest;        // �ړI�̒����_
	D3DXVECTOR3  m_PosRDiff;        // ���݂̒����_�ƖړI�̒����_�̍���
	D3DXVECTOR3  m_MoveV;           // ���_�̈ړ���
	D3DXVECTOR3  m_MoveR;           // �����_�̈ړ���
	D3DXVECTOR3  m_Rot;             // ���݂̌���
	D3DXVECTOR3  m_RotDest;         // �ړI�̌���
	D3DXVECTOR3  m_RotDiff;         // �ړI�̌����ƌ��݂̌����̍���
	D3DXVECTOR3  m_RotOld;          // �O��̌���
	D3DXVECTOR3  m_VecU;            // ������x�N�g��
	D3DXMATRIX   m_MtxProjection;   // �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX   m_MtxView;         // �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_ViewPort;        // �r���[�|�[�g(�`��̈�)
	float        m_fLength;         // ����
	bool         m_bMoving;         // �J�����𓮂����邩�ǂ���(����p)
};

//*****************************************************************************
//    �~�j�}�b�v�J�����N���X�̒�`
//*****************************************************************************
class CMapCamera
{
public:    // �N�ł��A�N�Z�X�\
	CMapCamera();
	~CMapCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CPlayer      *m_pPlayer;        // �v���C���[�N���X�ւ̃|�C���^
	D3DXVECTOR3  m_PosV;            // ���݂̎��_
	D3DXVECTOR3  m_PosR;            // ���݂̒����_
	D3DXVECTOR3  m_Rot;             // ���݂̌���
	D3DXVECTOR3  m_VecU;            // ������x�N�g��
	D3DXMATRIX   m_MtxProjection;   // �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX   m_MtxView;         // �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_ViewPort;        // �r���[�|�[�g(�`��̈�)
	float        m_fLength;         // ����
};

#endif