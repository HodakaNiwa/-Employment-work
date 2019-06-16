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
class CFileLoader;
class CFileSaver;
class CMeshField;
class CRiver;
class CMountain;
class CSky;
class CObject;
class CObjectMove;
class CObjectGoal;
class CEnemy;
class CBillboardObject;
class CEmitter;
class CMeshWall;
class CItem;
class CTextureManager;
class CModelManager;
class CEffectManager;
class CEnemyManager;
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
	void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 rollSpeed, int nModelIdx);
	void SetObjectMove(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 rollSpeed, int nModelIdx, D3DXVECTOR3 move, int nMaxMoveCounter);
	void SetBillboard(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType);
	void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
	void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nScore);
	void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx);
	void SetItem(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType);

	char *GetMapName(void);
	int GetCntEnemyLoad(void);
	CModelManager *GetModelManager(void);
	CTextureManager *GetTextureManager(void);
	D3DXVECTOR3 GetPlayerDefaultPos(void);
	D3DXVECTOR3 GetPlayerDefaultRot(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Save(void);
	void SaveObject_Open(void);
	void SaveObject(CFileSaver *pFileSaver);
	void SaveObject_Info(CFileSaver *pFileSaver, CObject *pObject);
	void SaveObjectMove_Info(CFileSaver *pFileSaver, CObjectMove *pObjectMove);
	void SaveObjectGoal_Info(CFileSaver *pFileSaver, CObjectGoal *pObjectGoal);
	void SaveEnemy_Open(void);
	void SaveEnemy_Manager(CFileSaver *pFileSaver);
	void SaveEnemy(CFileSaver *pFileSaver);
	void SaveEnemy_Info(CFileSaver *pFileSaver, CEnemy *pEnemy);
	void SaveBillboard_Open(void);
	void SaveBillboard(CFileSaver *pFileSaver);
	void SaveBillboard_Info(CFileSaver *pFileSaver, CBillboardObject *pBillObj);
	void SaveEffect_Open(void);
	void SaveEffect(CFileSaver *pFileSaver);
	void SaveEffect_Info(CFileSaver *pFileSaver, CEmitter *pEmitter);
	void SaveWall_Open(void);
	void SaveWall(CFileSaver *pFileSaver);
	void SaveWall_Info(CFileSaver *pFileSaver, CMeshWall *pMeshWall);
	void SaveItem_Open(void);
	void SaveItem(CFileSaver *pFileSaver);
	void SaveItem_Info(CFileSaver *pFileSaver, CItem *pItem);

	void ScriptLoad_Open(void);
	void ScriptLoad(CFileLoader *pFileLoader, char *pStrCur);
	void CreateTextureManager(int nNumTexture);
	void TextureLoad(char *pStrCur, int nCntTex);
	void CreateModelManager(int nNumModel);
	void ModelLoad(char *pStrCur, int nCntModel);
	void PlayerDefaultSpawnLoad(char *pStrCur);
	void CameraDefaultRotLoad(char *pStrCur);
	void FieldLoad_Open(char *pFieldFileName, char *pStrCur);
	void FieldLoad(CFileLoader *pFileLoader, char *pStrCur);
	void FieldLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void RiverLoad_Open(char *pRiverFileName, char *pStrCur);
	void RiverLoad(CFileLoader *pFileLoader, char *pStrCur);
	void RiverLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void ObjectLoad_Open(char *pObjectFileName, char *pStrCur);
	void ObjectLoad(CFileLoader *pFileLoader, char *pStrCur);
	void ObjectLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void ObjectMoveLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void ObjectGoalLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void MountainLoad_Open(char *pMountainFileName, char *pStrCur);
	void MountainLoad(CFileLoader *pFileLoader, char *pStrCur);
	void MountainLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void SkyLoad_Open(char *pSkyFileName, char *pStrCur);
	void SkyLoad(CFileLoader *pFileLoader, char *pStrCur);
	void SkyLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void EnemyLoad_Open(char *pEnemyFileName, char *pStrCur);
	void EnemyLoad(CFileLoader *pFileLoader, char *pStrCur);
	CEnemyManager *EnemyLoad_Manager(CFileLoader *pFileLoader, char *pFileName, int nCntEnemyLoad);
	void EnemyLoad_Info(CFileLoader *pFileLoader, char *pStrCur, CEnemyManager **apEnemyManager);
	void EnemyLoad_Info_AI(CFileLoader *pFileLoader, char *pStrCur, float *pCheackDistance, float *pAttackDistance, int *pAttackTiming);
	void SetEnemyManagerToAssembley(CEnemyManager **apEnemyManager, int nCntEnemyLoad);
	void BillboardLoad_Open(char *pBillboardFileName, char *pStrCur);
	void BillboardLoad(CFileLoader *pFileLoader, char *pStrCur);
	void BillboardLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void EffectLoad_Open(char *pEffectFileName, char *pStrCur);
	void EffectLoad(CFileLoader *pFileLoader, char *pStrCur);
	void SearchEffectManager(void);
	void EffectLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void WallLoad_Open(char *pWallFileName, char *pStrCur);
	void WallLoad(CFileLoader *pFileLoader, char *pStrCur);
	void WallLoad_Info(CFileLoader *pFileLoader, char *pStrCur);
	void ItemLoad_Open(char *pItemFileName, char *pStrCur);
	void ItemLoad(CFileLoader *pFileLoader, char *pStrCur);
	void ItemLoad_Info(CFileLoader *pFileLoader, char *pStrCur);

	void ReleaseModelManager(void);
	void ReleaseTextureManager(void);
	void ReleaseEnemyAssembley(void);

	void DeleteMapObject(void);

	CModelManager   *m_pModelManager;      // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;    // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	CEffectManager  *m_pEffectManager;     // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CEnemyAssembly  *m_pEnemyAssembly;     // �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^
	int             m_nCntLoadEnemy;       // �G�f�[�^��ǂݍ��񂾉�
	char            m_aFileName[256];      // �ǂݍ��ރX�N���v�g�t�@�C����
	D3DXVECTOR3     m_PlayerDefaultPos;    // �v���C���[�̃f�t�H���g�ʒu
	D3DXVECTOR3     m_PlayerDefaultRot;    // �v���C���[�̃f�t�H���g����
};

#endif