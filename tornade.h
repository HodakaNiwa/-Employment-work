//*****************************************************************************
//
//     竜巻の処理[tornade.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TORNADE_H_
#define _TORNADE_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define TORNADE_EFFECT_NUM (11)   // 竜巻エフェクトの出せる数

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CEffectManager;
class CEmitter;
class CPlayer;

//*****************************************************************************
//     竜巻クラスの定義
//*****************************************************************************
class CTornade : public CScene
{
public:    // 誰でもアクセス可能
	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_HORMING = 0,   // ホーミングする状態
		STATE_NONE_HORMING,  // ホーミングしない状態
		STATE_MAX
	}STATE;

	CTornade(int nPriority = 3, OBJTYPE objType = OBJTYPE_TORNADE);
	~CTornade();

	static CTornade *Create(D3DXVECTOR3 pos, CEffectManager *pEffectManager, float fDestAngle, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(CPlayer *pPlayer);

	void SetPos(const D3DXVECTOR3 pos);
	void SetAngle(const float fAngle);
	void SetEffectManager(CEffectManager *pEffectManager);

	D3DXVECTOR3 GetPos(void);
	float GetAngle(void);
	CEffectManager *GetEffectManager(void);

private:   // このクラスだけがアクセス可能
	int            m_nEffectCounter;                        // エフェクトを出すタイミングを管理するカウンター
	CEffectManager *m_pEffectManager;                       // エフェクト管轄クラスへのポインタ
	STATE          m_State;                                 // 状態
	int            m_nLife;                                 // 寿命(追尾が終了したら減らし始める)
	D3DXVECTOR3    m_Pos;                                   // 座標
	CEmitter       *m_pTornadeEmitter[TORNADE_EFFECT_NUM];  // エフェクト生成エミッタクラスへのポインタ(最初に生成したものをついてこさせるため)
	float          m_fAngle;                                // 現在の向き
	float          m_fDestAngle;                            // 目的の向き
	float          m_fDiffAngle;                            // 目的の向きと現在の向きの差分
	float          m_fRivisionAngle;                        // どれだけ向きを補正したかどうか
};

#endif