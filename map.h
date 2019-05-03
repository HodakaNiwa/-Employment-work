//*****************************************************************************
//
//     �}�b�v�̏���[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define SKY_PRIORITY       (1)   // ��̏����D�揇��
#define MOUNTAIN_PRIORITY  (2)   // �R�̏����D�揇��
#define FIELD_PRIORITY     (2)   // �n�ʂ̏����D�揇��
#define BILLBOARD_PRIORITY (5)   // �r���{�[�h�̏����D�揇��
#define OBJECT_PRIORITY    (5)   // �I�u�W�F�N�g�̏����D�揇��
#define WALL_PRIORITY      (4)   // �ǂ̏����D�揇��
#define RIVER_PRIORITY     (5)   // ��̏����D�揇��
#define MAP_PRIORITY       (6)   // �}�b�v�̏����D�揇��

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMeshField;
class CMountain;
class CSky;
class CTextureManager;
class CModelManager;
class CEffectManager;
class CEnemyAssembly;

//*****************************************************************************
//     �}�b�v�N���X�̒�`
//*****************************************************************************
class CMap
{
public:    // �N�ł��A�N�Z�X�\
	CMap();
	~CMap();

	static CMap *Create(char *aFileName);

	void Init(char *aFileName);
	void Uninit(void);
	void SetObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, D3DXVECTOR3 RollSpeed, int nModelIdx);
	void SetObjectMove(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, D3DXVECTOR3 RollSpeed, int nModelIdx, D3DXVECTOR3 Move, int nMaxMoveCounter);
	void SetBillboard(D3DXVECTOR3 Pos, D3DXCOLOR Col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType);
	void SetEffect(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType);
	void SetEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nScore);
	void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx);
	void SetItem(D3DXVECTOR3 Pos, D3DXCOLOR Col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType);

	char *GetMapName(void);
	int GetCntEnemyLoad(void);
	CModelManager *GetModelManager(void);
	CTextureManager *GetTextureManager(void);
	D3DXVECTOR3 GetPlayerDefaultPos(void);
	D3DXVECTOR3 GetPlayerDefaultRot(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Save(void);
	void SaveObject(void);
	void SaveEnemy(void);
	void SaveBillboard(void);
	void SaveEffect(void);
	void SaveWall(void);
	void SaveItem(void);

	void FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine);
	void RiverLoad(char *pRiverFileName, char *pStrCur, char *pLine);
	void ObjectLoad(char *pObjectFileName, char *pStrCur, char *pLine);
	void MountainLoad(char *pMountainFileName, char *pStrCur, char *pLine);
	void SkyLoad(char *pSkyFileName, char *pStrCur, char *pLine);
	void EnemyLoad(char *pEnemyFileName, char *pStrCur, char *pLine);
	void BillboardLoad(char *pBillboardFileName, char *pStrCur, char *pLine);
	void EffectLoad(char *pEffectFileName, char *pStrCur, char *pLine);
	void WallLoad(char *pWallFileName, char *pStrCur, char *pLine);
	void ItemLoad(char *pItemFileName, char *pStrCur, char *pLine);

	CModelManager   *m_pModelManager;      // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;    // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	CEffectManager  *m_pEffectManager;     // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CEnemyAssembly  *m_pEnemyAssembly;     // �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^
	CMeshField      *m_pField;             // �n�ʃN���X�ւ̃|�C���^
	CMountain       *m_pMoutain;           // �R�N���X�ւ̃|�C���^
	CSky            *m_pSky;               // ��N���X�ւ̃|�C���^
	int             m_nCntLoadEnemy;       // �G�f�[�^��ǂݍ��񂾉�
	char            m_aFileName[256];      // �ǂݍ��ރX�N���v�g�t�@�C����
	D3DXVECTOR3     m_PlayerDefaultPos;    // �v���C���[�̃f�t�H���g�ʒu
	D3DXVECTOR3     m_PlayerDefaultRot;    // �v���C���[�̃f�t�H���g����
};

#endif