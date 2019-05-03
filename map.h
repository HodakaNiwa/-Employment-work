//*****************************************************************************
//
//     マップの処理[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define SKY_PRIORITY       (1)   // 空の処理優先順位
#define MOUNTAIN_PRIORITY  (2)   // 山の処理優先順位
#define FIELD_PRIORITY     (2)   // 地面の処理優先順位
#define BILLBOARD_PRIORITY (5)   // ビルボードの処理優先順位
#define OBJECT_PRIORITY    (5)   // オブジェクトの処理優先順位
#define WALL_PRIORITY      (4)   // 壁の処理優先順位
#define RIVER_PRIORITY     (5)   // 川の処理優先順位
#define MAP_PRIORITY       (6)   // マップの処理優先順位

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMeshField;
class CMountain;
class CSky;
class CTextureManager;
class CModelManager;
class CEffectManager;
class CEnemyAssembly;

//*****************************************************************************
//     マップクラスの定義
//*****************************************************************************
class CMap
{
public:    // 誰でもアクセス可能
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

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
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

	CModelManager   *m_pModelManager;      // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;    // テクスチャ管理クラスへのポインタ
	CEffectManager  *m_pEffectManager;     // エフェクト管轄クラスへのポインタ
	CEnemyAssembly  *m_pEnemyAssembly;     // 敵管轄まとめクラスへのポインタ
	CMeshField      *m_pField;             // 地面クラスへのポインタ
	CMountain       *m_pMoutain;           // 山クラスへのポインタ
	CSky            *m_pSky;               // 空クラスへのポインタ
	int             m_nCntLoadEnemy;       // 敵データを読み込んだ回数
	char            m_aFileName[256];      // 読み込むスクリプトファイル名
	D3DXVECTOR3     m_PlayerDefaultPos;    // プレイヤーのデフォルト位置
	D3DXVECTOR3     m_PlayerDefaultRot;    // プレイヤーのデフォルト向き
};

#endif