//*****************************************************************************
//
//     アイテムの処理[item.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "billboardObject.h"

//*****************************************************************************
//    アイテムクラスの定義
//*****************************************************************************
class CItem : public CBillboardObject
{
public:    // 誰でもアクセス可能
	//--------------------
	//  種類
	//--------------------
	typedef enum
	{
		TYPE_NORMAL = 0,  // 通常
		TYPE_HIDDEN,      // 隠しアイテム
		TYPE_MAX,
	}TYPE;

	CItem(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItem();

	static CItem *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(const int nType);
	int GetType(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int   m_nType;      // アイテムの種類番号
};

//*****************************************************************************
//    配置専用のアイテムクラスの定義
//*****************************************************************************
class CSetItem : public CItem
{
public:    // 誰でもアクセス可能
	CSetItem(int nPriority = 3, OBJTYPE objType = OBJTYPE_SETITEM);
	~CSetItem();

	static CSetItem *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nType = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

#endif