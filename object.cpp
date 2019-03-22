//*****************************************************************************
//
//     配置物の処理[object.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "object.h"
#include "goalCylinder.h"
#include "player.h"
#include "enemy.h"
#include "scene3D.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define OBJECTGOAL_CYLINDER_COL      (D3DXCOLOR(0.2f,0.7f,1.0f,1.0f))    // ゴール用円筒の色
#define OBJECTGOAL_CYLINDER_ROT      (D3DXVECTOR3(0.0f,0.0f,0.0f))       // ゴール用円筒の向き
#define OBJECTGOAL_CYLINDER_HEIGHT   (600.0f)                            // ゴール用円筒の高さ
#define OBJECTGOAL_CYLINDER_RADIUS   (300.0f)                            // ゴール用円筒の高さ
#define OBJECTGOAL_CYLINDER_XBLOCK   (20)                                // ゴール用円筒の横の分割数
#define OBJECTGOAL_CYLINDER_YBLOCK   (3)                                 // ゴール用円筒の縦の分割数
#define OBJECTGOAL_CYLINDER_PRIORITY (6)                                 // ゴール用円筒の処理優先順位

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************


//*****************************************************************************
//     CObjecctの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{
	// 各種値のクリア
	m_Collision = COL_TYPE_SQUARE;                // 当たり判定を取る種類
	m_ModelIdx = 0;                               // 使用しているモデルの番号
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 前回の座標
	m_RollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 回転するスピード
	m_PosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 生成時の座標
	m_RotStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 生成時の向き
	m_fColRange = 0.0f;                           // 当たり判定を取る範囲
	m_bHitAttack = false;                         // 攻撃がヒットしたかどうか
	m_nHitCounter = 0;                            // 攻撃がヒットしてからの時間
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
//    配置物の生成処理
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision, float fColRange, int nPriority)
{
	CObject *pObject = NULL;   // 配置物クラス型のポインタ
	if (pObject == NULL)
	{// メモリが空になっている
		pObject = new CObject(nPriority);
		if (pObject != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObject->Init(pos, rot, scale, RollSpeed, Collision, fColRange)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObject;   // インスタンスのアドレスを返す
}

//=============================================================================
//    配置物の初期化処理
//=============================================================================
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, COL_TYPE Collision, float fColRange)
{
	// 各種値の設定
	SetPos(pos);                // 現在の座標
	SetRot(rot);                // 現在の向き
	SetScale(scale);            // 大きさの倍率
	m_RotStd = rot;             // 生成時の向き
	m_RollSpeed = RollSpeed;    // 回転するスピード
	m_Collision = Collision;    // コリジョンの種類
	m_fColRange = fColRange;    // 当たり判定を取る範囲(円形で当たり判定する場合に使用)

	// 共通の初期化処理
	if (FAILED(CSceneX::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    配置物の終了処理
//=============================================================================
void CObject::Uninit(void)
{
	// 共通の終了処理
	CSceneX::Uninit();
}

//=============================================================================
//    配置物の更新処理
//=============================================================================
void CObject::Update(void)
{
	// 前回の座標更新
	m_PosOld = GetPos();

	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	// 回転させる
	rot += m_RollSpeed;

	// 向きチェック
	// X軸
	if (rot.x > D3DX_PI)
	{// 向きが円周率を超えた
		rot.x -= D3DX_PI * 2.0f;
	}
	if (rot.x < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.x += D3DX_PI * 2.0f;
	}

	// Y軸
	if (rot.y > D3DX_PI)
	{// 向きが円周率を超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.y += D3DX_PI * 2.0f;
	}

	// Z軸
	if (rot.z > D3DX_PI)
	{// 向きが円周率を超えた
		rot.z -= D3DX_PI * 2.0f;
	}
	if (rot.z < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.z += D3DX_PI * 2.0f;
	}

	// 向きの設定
	SetRot(rot);

	if (m_nHitCounter > 0)
	{
		m_nHitCounter--;
		if (m_nHitCounter <= 0)
		{
			m_bHitAttack = false;
		}
	}
}

//=============================================================================
//    配置物の描画処理
//=============================================================================
void CObject::Draw(void)
{
	// 共通の描画処理
	CSceneX::Draw();
}

//=============================================================================
//    配置物の当たり判定処理
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject)
{
	bool bLand = false;   // オブジェクトに乗っているかどうか

	if (m_Collision == COL_TYPE_SQUARE)
	{// 四角形で当たり判定を取るなら
		bLand = SquareCol(pPos, pPosOld, pMove, fHeight, fRange, pRideObject);
	}
	else if (m_Collision == COL_TYPE_CIRCLE)
	{// 円形で当たり判定を取るなら
		bLand = CircleCol(pPos, pPosOld, pMove, fHeight, fRange, pRideObject);
	}

	return bLand;
}

//=============================================================================
//    四角形で当たり判定を取る処理
//=============================================================================
bool CObject::SquareCol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject)
{
	bool bLand = false;

	// 当たり判定に必要な値を配置物から取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標
	D3DXVECTOR3 Rot = GetRot();          // 現在の向き
	D3DXVECTOR3 VtxMin = GetVtxMin();    // 頂点の座標最小値
	D3DXVECTOR3 VtxMax = GetVtxMax();    // 頂点の座標最大値

	// 外積の判定に必要な値を用意
	D3DXVECTOR3 ObjectCollisionPos[4] =
	{// 当たり判定用の四角(ローカル座標)
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
	};

	D3DXVECTOR3 ObjectCollisionWorldPos[4];  // 当たり判定用の四角(ワールド座標)
	D3DXMATRIX ObjectCollisionMtxWorld;      // 計算用マトリックス

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{// 判定する四角の座標分繰り返し
		D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// 親の情報を掛け合わせる
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ワールド座標を抜き出す
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}

	D3DXVECTOR3 vecA, vecB;        // 判定用ベクトル
	int nCntVecTrue = 0;           // ベクトルの判定を通った回数
	bool bObjectRand = false;      // オブジェクトの中にいるかどうか
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// 判定するベクトルの数だけ繰り返し
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// ベクトルの右側にいる
			nCntVecTrue++;   // 判定を通った回数を加算
			if (nCntVecTrue == 4)
			{// 全てのベクトルの右側にいる
				bObjectRand = true;
			}
		}
	}

	if (bObjectRand == true)
	{// 配置物の中にいる
	    // 乗っているかどうかの判定開始
		if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
		{// 配置物に上から入った
			pPos->y = pos.y + VtxMax.y;
			pMove->y = 0.0f;

			if (pRideObject->GetObjType() == OBJTYPE_PLAYER)
			{// 乗ってきたオブジェクトがプレイヤーだった
				CPlayer *pPlayer = (CPlayer*)pRideObject;

				// 必要な値を取得する
				CPlayer::STATE State = pPlayer->GetState();                     // 状態
				D3DXVECTOR3 Rot = pPlayer->GetRot();                            // 現在の向き
				D3DXVECTOR3 DestRot = pPlayer->GetDestRot();                    // 目的の向き
				D3DXVECTOR3 Move = pPlayer->GetMove();                          // 移動量
				D3DXVECTOR3 OffsetPos = pPlayer->GetLandOffsetPos();            // 乗っているオブジェクトからのオフセット距離
				D3DXVECTOR3 OffsetAdd = pPlayer->GetLandOffsetAdd();            // 乗っているオブジェクトからのオフセット距離に加算する値
				D3DXVECTOR3 OffsetRot = pPlayer->GetLandOffsetRot();            // 乗っているオブジェクトからのオフセット向き
				D3DXVECTOR3 OffsetRotStart = pPlayer->GetLandOffsetRotStart();  // 乗っているオブジェクトからのオフセット向きに加算する値
				D3DXVECTOR3 ObjRot = GetRot();                                  // オブジェクトの向き


				if (pPlayer->GetLand() == false)
				{// 前回乗っていなかった
					OffsetPos = D3DXVECTOR3(pPos->x - pos.x, 0.0f, pPos->z - pos.z); // オフセット座標を保存
					OffsetRot = Rot - ObjRot;                                        // プレイヤーのオフセット向きを保存
					if (OffsetRot.y > D3DX_PI)
					{// 円周率を超えた
						OffsetRot.y -= D3DX_PI * 2.0f;
					}
					if (OffsetRot.y < -D3DX_PI)
					{// 円周率を超えた
						OffsetRot.y += D3DX_PI * 2.0f;
					}
					OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // オブジェクトのスタート向きを初期化
					OffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // オフセットに加える値を初期化
				}
				else
				{// 前回何かに乗っている
					if (pPlayer->GetLandSceneOld() != this)
					{// 前回乗っていたオブジェクトが自分ではない
						OffsetPos = D3DXVECTOR3(pPos->x - pos.x, 0.0f, pPos->z - pos.z); // オフセット座標を保存
						OffsetRot = Rot - ObjRot;                                        // プレイヤーのオフセット向きを保存
						if (OffsetRot.y > D3DX_PI)
						{// 円周率を超えた
							OffsetRot.y -= D3DX_PI * 2.0f;
						}
						if (OffsetRot.y < -D3DX_PI)
						{// 円周率を超えた
							OffsetRot.y += D3DX_PI * 2.0f;
						}
						OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // オブジェクトのスタート向きを初期化
						OffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // オフセットに加える値を初期化
					}
				}


				// 向きを回転に沿わせる
				if (State != CPlayer::STATE_MOVE)
				{// 移動状態ではない
					Rot = OffsetRot + ObjRot;    // 向きを補正
					if (Rot.y > D3DX_PI)
					{// 円周率を超えた
						Rot.y -= D3DX_PI * 2.0f;
					}
					if (Rot.y < -D3DX_PI)
					{// 円周率を超えた
						Rot.y += D3DX_PI * 2.0f;
					}
					DestRot.y = Rot.y;  // 目的の向きも補正
				}

				// オフセットの向きに配置物の回転スピードを加算する
				OffsetRotStart.y += m_RollSpeed.y;  // オブジェクトが回転するスピードに合わせて補正
				if (OffsetRotStart.y > D3DX_PI)
				{// 円周率を超えた
					OffsetRotStart.y -= D3DX_PI * 2.0f;
				}
				else if (OffsetRotStart.y < -D3DX_PI)
				{// 円周率を超えた
					OffsetRotStart.y += D3DX_PI * 2.0f;
				}
				// オフセット位置にY軸回転に沿わせた移動量を計算する
				OffsetAdd.x += cosf(0.0f) * (cosf(OffsetRotStart.y) * Move.x + -sinf(OffsetRotStart.y) * Move.z) + sinf(0.0f) * (sinf(OffsetRotStart.y) * Move.x + cosf(OffsetRotStart.y) * Move.z);
				OffsetAdd.z += -sinf(0.0f) * (cosf(OffsetRotStart.y) * Move.x + -sinf(OffsetRotStart.y) * Move.z) + cosf(0.0f) * (sinf(OffsetRotStart.y) * Move.x + cosf(OffsetRotStart.y) * Move.z);

				// 位置をY軸回転に沿わせて移動させる
				float fMtxX = cosf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + sinf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
				float fMtxZ = -sinf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + cosf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
				pPos->x = fMtxX + pos.x;
				pPos->z = fMtxZ + pos.z;

				// 値を設定する
				pPlayer->SetRot(Rot);                              // 現在の向き
				pPlayer->SetDestRot(DestRot);                      // 目的の向き
				pPlayer->SetLandOffsetPos(OffsetPos);              // 乗っているオブジェクトからのオフセット距離
				pPlayer->SetLandOffsetAdd(OffsetAdd);              // 乗っているオブジェクトからのオフセット距離に加算する値
				pPlayer->SetLandOffsetRot(OffsetRot);              // 乗っているオブジェクトからのオフセット向き
				pPlayer->SetLandOffsetRotStart(OffsetRotStart);    // 乗っているオブジェクトからのオフセット向きに加算する値
			}
			else if (pRideObject->GetObjType() == OBJTYPE_ENEMY)
			{// 乗ってきたオブジェクトが敵だった
				CEnemy *pEnemy = (CEnemy*)pRideObject;

				// 必要な値を取得する
				CEnemy::STATE State = pEnemy->GetState();                      // 状態
				D3DXVECTOR3 Rot = pEnemy->GetRot();                            // 現在の向き
				D3DXVECTOR3 DestRot = pEnemy->GetDestRot();                    // 目的の向き
				D3DXVECTOR3 OffsetPos = pEnemy->GetLandOffsetPos();            // 乗っているオブジェクトからのオフセット距離
				D3DXVECTOR3 OffsetAdd = pEnemy->GetLandOffsetAdd();            // 乗っているオブジェクトからのオフセット距離に加算する値
				D3DXVECTOR3 OffsetRot = pEnemy->GetLandOffsetRot();            // 乗っているオブジェクトからのオフセット向き
				D3DXVECTOR3 OffsetRotStart = pEnemy->GetLandOffsetRotStart();  // 乗っているオブジェクトからのオフセット向きに加算する値
				D3DXVECTOR3 ObjRot = GetRot();                                 // オブジェクトの向き


				if (pEnemy->GetLand() == false)
				{// 前回乗っていなかった
				    // 向きを保存
					OffsetPos = D3DXVECTOR3(pPos->x - pos.x, 0.0f, pPos->z - pos.z); // オフセット座標を保存
					OffsetRot = Rot - ObjRot;                                        // プレイヤーのオフセット向きを保存
					if (OffsetRot.y > D3DX_PI)
					{// 円周率を超えた
						OffsetRot.y -= D3DX_PI * 2.0f;
					}
					if (OffsetRot.y < -D3DX_PI)
					{// 円周率を超えた
						OffsetRot.y += D3DX_PI * 2.0f;
					}
					OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // オブジェクトのスタート向きを初期化
					OffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // オフセットに加える値を初期化
				}
				else
				{// 前回何かに乗っている
					if (pEnemy->GetLandSceneOld() != this)
					{// 前回乗っていたオブジェクトが自分ではない
						OffsetPos = D3DXVECTOR3(pPos->x - pos.x, 0.0f, pPos->z - pos.z); // オフセット座標を保存
						OffsetRot = Rot - ObjRot;                                        // 敵のオフセット向きを保存
						if (OffsetRot.y > D3DX_PI)
						{// 円周率を超えた
							OffsetRot.y -= D3DX_PI * 2.0f;
						}
						if (OffsetRot.y < -D3DX_PI)
						{// 円周率を超えた
							OffsetRot.y += D3DX_PI * 2.0f;
						}
						OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // オブジェクトのスタート向きを初期化
						OffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // オフセットに加える値を初期化
					}
				}


				// 向きを回転に沿わせる
				if (State != CPlayer::STATE_MOVE)
				{// 移動状態ではない
					Rot = OffsetRot + ObjRot;    // 向きを補正
					if (Rot.y > D3DX_PI)
					{// 円周率を超えた
						Rot.y -= D3DX_PI * 2.0f;
					}
					if (Rot.y < -D3DX_PI)
					{// 円周率を超えた
						Rot.y += D3DX_PI * 2.0f;
					}
					DestRot.y = Rot.y;  // 目的の向きも補正
				}

				// オフセットの向きに配置物の回転スピードを加算する
				OffsetRotStart.y += m_RollSpeed.y;  // オブジェクトが回転するスピードに合わせて補正
				if (OffsetRotStart.y > D3DX_PI)
				{// 円周率を超えた
					OffsetRotStart.y -= D3DX_PI * 2.0f;
				}
				else if (OffsetRotStart.y < -D3DX_PI)
				{// 円周率を超えた
					OffsetRotStart.y += D3DX_PI * 2.0f;
				}

				// 位置をY軸回転に沿わせて移動させる
				float fMtxX = cosf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + sinf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
				float fMtxZ = -sinf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + cosf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
				pPos->x = fMtxX + pos.x;
				pPos->z = fMtxZ + pos.z;

				// 値を設定する
				pEnemy->SetRot(Rot);                              // 現在の向き
				pEnemy->SetDestRot(DestRot);                      // 目的の向き
				pEnemy->SetLandOffsetPos(OffsetPos);              // 乗っているオブジェクトからのオフセット距離
				pEnemy->SetLandOffsetAdd(OffsetAdd);              // 乗っているオブジェクトからのオフセット距離に加算する値
				pEnemy->SetLandOffsetRot(OffsetRot);              // 乗っているオブジェクトからのオフセット向き
				pEnemy->SetLandOffsetRotStart(OffsetRotStart);    // 乗っているオブジェクトからのオフセット向きに加算する値
			}
			// 乗っている判定にする
			bLand = true;
		}
		else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y)
		{// 配置物に下から入った
			pPos->y = pos.y + VtxMin.y - fHeight;
			pMove->y = 0.0f;
		}
		else
		{// 配置物に下からも上からも入っていない
			if (pPos->y <= pos.y + VtxMax.y && pPos->y + fHeight >= pos.y + VtxMin.y)
			{// オブジェクトのY軸の間にいる
				if (Rot.y != 0.0f)
				{// 向きが回転していない(矩形で判定を取る)
					for (int nCntVec = 0; nCntVec < 4; nCntVec++)
					{// 判定するベクトルの数だけ繰り返し
						vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
						vecB = *pPosOld - ObjectCollisionWorldPos[nCntVec];
						if ((vecA.x * vecB.z) - (vecA.z * vecB.x) >= -1)
						{// ベクトルの左側にいる
						    // 前回の座標を利用して配置物とのベクトルをしき、左にいるベクトルの交点を求める
							D3DXVECTOR3 Pos1 = *pPosOld;                                    // プレイヤーの前回の座標
							D3DXVECTOR3 Pos2 = pos;                                         // 配置物の座標
							D3DXVECTOR3 Pos3 = ObjectCollisionWorldPos[(nCntVec + 1) % 4];  // ベクトル判定に引っかかった頂点座標(1つ目)
							D3DXVECTOR3 Pos4 = ObjectCollisionWorldPos[nCntVec];            // ベクトル判定に引っかかった頂点座標(2つ目)

							// 直線の方程式から連立方程式を組み、XZ平面上の交点を求める
							float VecLine1 = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x);
							float VecLine2 = (Pos4.z - Pos3.z) / (Pos4.x - Pos3.x);
							float XPos = (VecLine1 * Pos1.x - Pos1.z - VecLine2 * Pos3.x + Pos3.z) / (VecLine1 - VecLine2);
							float ZPos = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x) * (XPos - Pos1.x) + Pos1.z;
							pPos->x = XPos;
							pPos->z = ZPos;
						}
					}
				}

				if (Rot.y == 0.0f)
				{// 向きが回転していない(矩形で判定を取る)
					if (pPos->z - fRange <= pos.z + VtxMax.z && pPos->z + fRange >= pos.z + VtxMin.z)
					{// オブジェクトのZ軸の間にいる
						if (pPosOld->x + fRange <= pos.x + VtxMin.x && pPos->x + fRange >= pos.x + VtxMin.x)
						{// 左からあたった
							pPos->x = pos.x + VtxMin.x - fRange;
						}
						else if (pPosOld->x - fRange >= pos.x + VtxMax.x && pPos->x - fRange <= pos.x + VtxMax.x)
						{// 右からあたった
							pPos->x = pos.x + VtxMax.x + fRange;
						}
					}
					if (pPos->x - fRange <= pos.x + VtxMax.x && pPos->x + fRange >= pos.x + VtxMin.x)
					{// オブジェクトのX軸の間にいる
						if (pPosOld->z + fRange<= pos.z + VtxMin.z && pPos->z + fRange >= pos.z + VtxMin.z)
						{// 後ろからあたった
							pPos->z = pos.z + VtxMin.z - fRange;
						}
						else if (pPosOld->z - fRange >= pos.z + VtxMax.z && pPos->z - fRange <= pos.z + VtxMax.z)
						{// 前からあたった
							pPos->z = pos.z + VtxMax.z + fRange + 0.1f;
						}
					}
				}
			}
		}
	}

	return bLand;
}

//=============================================================================
//    円形で当たり判定を取る処理
//=============================================================================
bool CObject::CircleCol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject)
{
	bool bLand = false;   // 乗っているかどうか

	float fHitRange = GetColRange();  // 当たり判定を取る範囲
	float fHitLength = 0.0f;          // 当たり判定を取る距離
	float fDistance = 0.0f;           // 配置物と引数の座標への距離

	// 当たり判定を取る長さを計算
	fHitLength = sqrtf((fHitRange + fRange) * (fHitRange + fRange));

	// 現在の距離をXZ平面で計算
	fDistance = sqrtf((pPos->x - GetPos().x) * (pPos->x - GetPos().x) + (pPos->z - GetPos().z) * (pPos->z - GetPos().z));

	if (fDistance < fHitLength)
	{// 配置物と引数の座標との距離が当たり判定を取る長さより短い
	    // ベクトルで押し返す
		D3DXVECTOR3 VecA = GetPos() - *pPos;
		D3DXVec3Normalize(&VecA, &VecA);
		pPos->x -= VecA.x * (fHitLength - fDistance);
		pPos->z -= VecA.z * (fHitLength - fDistance);
	}

	return bLand;         // 乗っているかどうかを返す
}

//=============================================================================
//    配置物の色を透明にする処理
//=============================================================================
void CObject::SetTransParent(float fAlpha)
{
	LPD3DXBUFFER pBuffMat = GetBuffMat();  // マテリアル情報へのポインタ
	DWORD nNumMat = GetNumMat();           // マテリアル情報の数
	D3DXMATERIAL *pMat;                    // マテリアルデータへのポインタ

	if (pBuffMat != NULL)
	{// マテリアル情報へのポインタが取得できた
	    // マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
		{// 設定されていたマテリアルの数だけ繰り返し
			pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
		}
	}
}

//=============================================================================
//    配置物の当たり判定を取る種類設定処理
//=============================================================================
void CObject::SetCollision(const COL_TYPE Collision)
{
	m_Collision = Collision;
}

//=============================================================================
//    配置物の回転スピード設定処理
//=============================================================================
void CObject::SetRollSpeed(const D3DXVECTOR3 RollSpeed)
{
	m_RollSpeed = RollSpeed;
}

//=============================================================================
//    配置物の当たり判定を取る範囲処理
//=============================================================================
void CObject::SetColRange(const float fColRange)
{
	m_fColRange = fColRange;
}

//=============================================================================
//    配置物の前回の座標設定処理
//=============================================================================
void CObject::SetPosOld(const D3DXVECTOR3 PosOld)
{
	m_PosOld = PosOld;
}

//=============================================================================
//    配置物の向き(生成時)設定処理
//=============================================================================
void CObject::SetRotStd(const D3DXVECTOR3 RotStd)
{
	m_RotStd = RotStd;
}

//=============================================================================
//    配置物の攻撃がヒットしたかどうか設定処理
//=============================================================================
void CObject::SetHitAttack(const bool bHitAttack)
{
	m_bHitAttack = bHitAttack;
}

//=============================================================================
//    配置物の攻撃がヒットしてからの時間設定処理
//=============================================================================
void CObject::SetHitCounter(const int nHitCounter)
{
	m_nHitCounter = nHitCounter;
}

//=============================================================================
//    配置物の当たり判定を取る種類処理
//=============================================================================
CObject::COL_TYPE CObject::GetCollision(void)
{
	return m_Collision;
}

//=============================================================================
//    配置物の回転スピード取得処理
//=============================================================================
D3DXVECTOR3 CObject::GetRollSpeed(void)
{
	return m_RollSpeed;
}

//=============================================================================
//    配置物の前回の座標取得処理
//=============================================================================
D3DXVECTOR3 CObject::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    配置物の向き(生成時)処理
//=============================================================================
D3DXVECTOR3 CObject::GetRotStd(void)
{
	return m_RotStd;
}

//=============================================================================
//    配置物の当たり判定を取る範囲取得処理
//=============================================================================
float CObject::GetColRange(void)
{
	return m_fColRange;
}

//=============================================================================
//    配置物の攻撃がヒットしたかどうか取得処理
//=============================================================================
bool CObject::GetHitAttack(void)
{
	return m_bHitAttack;
}

//*****************************************************************************
//     CObjectMoveの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectMove::CObjectMove(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// 各種値の設定
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	m_MoveCurrent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 現在の移動量
	m_nCounter = 0;                                 // 移動量を管理するカウンター
	m_nMaxMoveCounter = 0;                          // 移動量が最大値になる時間
	m_fRivisionMove = 0.0f;                         // 移動の倍率
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectMove::~CObjectMove()
{

}

//=============================================================================
//    動く配置物の生成処理
//=============================================================================
CObjectMove *CObjectMove::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision, float fColRange, int nPriority)
{
	CObjectMove *pObjectMove = NULL;   // 配置物クラス型のポインタ
	if (pObjectMove == NULL)
	{// メモリが空になっている
		pObjectMove = new CObjectMove(nPriority);
		if (pObjectMove != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObjectMove->Init(pos, rot, scale, RollSpeed, Move, nMaxMoveCounter, Collision, fColRange)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObjectMove;   // インスタンスのアドレスを返す
}

//=============================================================================
//    動く配置物の初期化処理
//=============================================================================
HRESULT CObjectMove::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, COL_TYPE Collision, float fColRange)
{
	// 各種値の設定
	m_Move = Move;                                              // 移動量
	m_nMaxMoveCounter = nMaxMoveCounter;                        // 移動量が最大値になる時間
	m_nCounter = nMaxMoveCounter / 2;                           // 移動量を管理するカウンター
	m_fRivisionMove = (D3DX_PI / nMaxMoveCounter) * m_nCounter; // 移動量を補正する倍率
	m_PosStd = pos;                                             // モデルの座標(生成時)

	// 共通の初期化処理
	if (FAILED(CObject::Init(pos, rot, scale, RollSpeed, Collision, fColRange)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    動く配置物の終了処理
//=============================================================================
void CObjectMove::Uninit(void)
{
	// 共通の終了処理
	CObject::Uninit();
}

//=============================================================================
//    動く配置物の更新処理
//=============================================================================
void CObjectMove::Update(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();        // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();  // 前回の座標
	D3DXVECTOR3 rot = GetRot();        // 現在の向き

	// 前回の座標更新
	posOld = pos;

	// 移動量を反転
	m_nCounter++;
	if (m_nMaxMoveCounter != 0)
	{// 最大値が0ではない
		if (m_nCounter % m_nMaxMoveCounter == 0)
		{
			m_fRivisionMove = 0.0f;
			m_Move *= -1;
			m_nCounter = 0;
		}

		// 移動させる
		m_fRivisionMove += D3DX_PI / m_nMaxMoveCounter;

		// 移動量を設定
		m_MoveCurrent.x = sinf(m_fRivisionMove) * m_Move.x;
		m_MoveCurrent.y = sinf(m_fRivisionMove) * m_Move.y;
		m_MoveCurrent.z = sinf(m_fRivisionMove) * m_Move.z;
		pos += m_MoveCurrent;

		if (m_fRivisionMove > D3DX_PI)
		{
			m_fRivisionMove -= D3DX_PI * 2.0f;
		}
	}

	// 座標の設定
	SetPos(pos);

	// 回転させる
	rot += GetRollSpeed();

	// 向きチェック
	// X軸
	if (rot.x > D3DX_PI)
	{// 向きが円周率を超えた
		rot.x -= D3DX_PI * 2.0f;
	}
	if (rot.x < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.x += D3DX_PI * 2.0f;
	}

	// Y軸
	if (rot.y > D3DX_PI)
	{// 向きが円周率を超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.y += D3DX_PI * 2.0f;
	}

	// Z軸
	if (rot.z > D3DX_PI)
	{// 向きが円周率を超えた
		rot.z -= D3DX_PI * 2.0f;
	}
	if (rot.z < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.z += D3DX_PI * 2.0f;
	}

	// 向きの設定
	SetRot(rot);
}

//=============================================================================
//    動く配置物の描画処理
//=============================================================================
void CObjectMove::Draw(void)
{
	// 共通の描画処理
	CObject::Draw();
}

//=============================================================================
//    動く配置物の移動量設定処理
//=============================================================================
void CObjectMove::SetMove(const D3DXVECTOR3 Move)
{
	m_Move = Move;
}

//=============================================================================
//    動く配置物の現在の移動量設定処理
//=============================================================================
void CObjectMove::SetMoveCurrent(const D3DXVECTOR3 MoveCurrent)
{
	m_MoveCurrent = MoveCurrent;
}

//=============================================================================
//    動く配置物の移動量を管理するカウンター設定処理
//=============================================================================
void CObjectMove::SetCounter(const int nCounter)
{
	m_nCounter = nCounter;
}

//=============================================================================
//    動く配置物の移動量を切り替える時間設定処理
//=============================================================================
void CObjectMove::SetMaxMoveCounter(const int nMaxMoveCounter)
{
	m_nMaxMoveCounter = nMaxMoveCounter;
}

//=============================================================================
//    動く配置物の移動量の倍率処理
//=============================================================================
void CObjectMove::SetRivisionMove(const float fRivisionMove)
{
	m_fRivisionMove = fRivisionMove;
}

//=============================================================================
//    動く配置物の座標(生成時)取得処理
//=============================================================================
void CObjectMove::SetPosStd(const D3DXVECTOR3 PosStd)
{
	m_PosStd = PosStd;
}

//=============================================================================
//    動く配置物の移動量取得処理
//=============================================================================
D3DXVECTOR3 CObjectMove::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    動く配置物の現在の移動量取得処理
//=============================================================================
D3DXVECTOR3 CObjectMove::GetMoveCurrent(void)
{
	return m_MoveCurrent;
}

//=============================================================================
//    動く配置物の移動量を管理するカウンター処理
//=============================================================================
int CObjectMove::GetCounter(void)
{
	return m_nCounter;
}

//=============================================================================
//    動く配置物の移動量を切り替える時間取得処理
//=============================================================================
int CObjectMove::GetMaxMoveCounter(void)
{
	return m_nMaxMoveCounter;
}

//=============================================================================
//    動く配置物の移動量の倍率取得処理
//=============================================================================
float CObjectMove::GetRivisionMove(void)
{
	return m_fRivisionMove;
}

//=============================================================================
//    動く配置物の座標(生成時)取得処理
//=============================================================================
D3DXVECTOR3 CObjectMove::GetPosStd(void)
{
	return m_PosStd;
}


//*****************************************************************************
//     CObjectGoalの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectGoal::CObjectGoal(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// 各種値のクリア
	m_pGoalCylinder = NULL;   // ゴール用円筒クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectGoal::~CObjectGoal()
{

}

//=============================================================================
//    配置物の生成処理
//=============================================================================
CObjectGoal *CObjectGoal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nPriority)
{
	CObjectGoal *pObjectGoal = NULL;   // ゴール用の配置物クラス型のポインタ
	if (pObjectGoal == NULL)
	{// メモリが空になっている
		pObjectGoal = new CObjectGoal(nPriority);
		if (pObjectGoal != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObjectGoal->Init(pos, rot, scale, RollSpeed)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObjectGoal;   // インスタンスのアドレスを返す
}

//=============================================================================
//    配置物の初期化処理
//=============================================================================
HRESULT CObjectGoal::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed)
{
	// 共通の初期化処理
	if (FAILED(CObject::Init(pos, rot, scale, RollSpeed)))
	{
		return E_FAIL;
	}

	// ゴール用の円筒を作成する
	if (m_pGoalCylinder == NULL)
	{
		m_pGoalCylinder = CGoalCylinder::Create(pos, OBJECTGOAL_CYLINDER_ROT, OBJECTGOAL_CYLINDER_COL, OBJECTGOAL_CYLINDER_HEIGHT, OBJECTGOAL_CYLINDER_RADIUS, OBJECTGOAL_CYLINDER_XBLOCK, OBJECTGOAL_CYLINDER_YBLOCK, (D3DX_PI * 0.1f), 0.0f, (D3DX_PI * 0.01f), OBJECTGOAL_CYLINDER_PRIORITY);
	}

	return S_OK;
}

//=============================================================================
//    配置物の終了処理
//=============================================================================
void CObjectGoal::Uninit(void)
{
	// ゴール用円筒を開放する
	if (m_pGoalCylinder != NULL)
	{
		m_pGoalCylinder->Uninit();
		m_pGoalCylinder = NULL;
	}

	// 共通の終了処理
	CObject::Uninit();
}

//=============================================================================
//    配置物の更新処理
//=============================================================================
void CObjectGoal::Update(void)
{
	// 共通の更新処理
	CObject::Update();
}

//=============================================================================
//    配置物の描画処理
//=============================================================================
void CObjectGoal::Draw(void)
{
	// 共通の描画処理
	CObject::Draw();
}

//=============================================================================
//    配置物の当たり判定処理
//=============================================================================
bool CObjectGoal::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject)
{
	bool bCollision = false;   // オブジェクトに当たっているかどうか

	// 当たり判定に必要な値を配置物から取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標
	D3DXVECTOR3 Rot = GetRot();          // 現在の向き
	D3DXVECTOR3 VtxMin = GetVtxMin();    // 頂点の座標最小値
	D3DXVECTOR3 VtxMax = GetVtxMax();    // 頂点の座標最大値

	// 外積の判定に必要な値を用意
	D3DXVECTOR3 ObjectCollisionPos[4] =
	{// 当たり判定用の四角(ローカル座標)
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
	};

	D3DXVECTOR3 ObjectCollisionWorldPos[4];  // 当たり判定用の四角(ワールド座標)
	D3DXMATRIX ObjectCollisionMtxWorld;      // 計算用マトリックス

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{// 判定する四角の座標分繰り返し
		D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// 親の情報を掛け合わせる
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ワールド座標を抜き出す
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}

	D3DXVECTOR3 vecA, vecB;        // 判定用ベクトル
	int nCntVecTrue = 0;           // ベクトルの判定を通った回数
	bool bObjectRand = false;      // オブジェクトの中にいるかどうか
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// 判定するベクトルの数だけ繰り返し
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// ベクトルの右側にいる
			nCntVecTrue++;   // 判定を通った回数を加算
			if (nCntVecTrue == 4)
			{// 全てのベクトルの右側にいる
				bObjectRand = true;
			}
		}
	}

	if (bObjectRand == true)
	{// 配置物の中にいる
		if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y)
		{// 配置物に下から入った
			pPos->y = pos.y + VtxMin.y - fHeight;
			pMove->y = 0.0f;
		}
		else
		{// 配置物に下からも上からも入っていない
			if (pPos->y <= pos.y + VtxMax.y && pPos->y + fHeight >= pos.y + VtxMin.y)
			{// オブジェクトのY軸の間にいる
				if (Rot.y != 0.0f)
				{// 向きが回転していない(矩形で判定を取る)
					for (int nCntVec = 0; nCntVec < 4; nCntVec++)
					{// 判定するベクトルの数だけ繰り返し
						vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
						vecB = *pPosOld - ObjectCollisionWorldPos[nCntVec];
						if ((vecA.x * vecB.z) - (vecA.z * vecB.x) >= -1)
						{// ベクトルの左側にいる
						    // 前回の座標を利用して配置物とのベクトルをしき、左にいるベクトルの交点を求める
							D3DXVECTOR3 Pos1 = *pPosOld;                                    // プレイヤーの前回の座標
							D3DXVECTOR3 Pos2 = pos;                                         // 配置物の座標
							D3DXVECTOR3 Pos3 = ObjectCollisionWorldPos[(nCntVec + 1) % 4];  // ベクトル判定に引っかかった頂点座標(1つ目)
							D3DXVECTOR3 Pos4 = ObjectCollisionWorldPos[nCntVec];            // ベクトル判定に引っかかった頂点座標(2つ目)

							// 直線の方程式から連立方程式を組み、XZ平面上の交点を求める
							float VecLine1 = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x);
							float VecLine2 = (Pos4.z - Pos3.z) / (Pos4.x - Pos3.x);
							float XPos = (VecLine1 * Pos1.x - Pos1.z - VecLine2 * Pos3.x + Pos3.z) / (VecLine1 - VecLine2);
							float ZPos = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x) * (XPos - Pos1.x) + Pos1.z;
							pPos->x = XPos;
							pPos->z = ZPos;

							// 当たっている判定にする
							bCollision = true;
						}
					}
				}

				if (Rot.y == 0.0f)
				{// 向きが回転していない(矩形で判定を取る)
					if (pPos->z - fRange <= pos.z + VtxMax.z && pPos->z + fRange >= pos.z + VtxMin.z)
					{// オブジェクトのZ軸の間にいる
						if (pPosOld->x + fRange <= pos.x + VtxMin.x && pPos->x + fRange >= pos.x + VtxMin.x)
						{// 左からあたった
							pPos->x = pos.x + VtxMin.x - fRange;
							// 当たっている判定にする
							bCollision = true;
						}
						else if (pPosOld->x - fRange >= pos.x + VtxMax.x && pPos->x - fRange <= pos.x + VtxMax.x)
						{// 右からあたった
							pPos->x = pos.x + VtxMax.x + fRange;
							// 当たっている判定にする
							bCollision = true;
						}
					}
					if (pPos->x - fRange <= pos.x + VtxMax.x && pPos->x + fRange >= pos.x + VtxMin.x)
					{// オブジェクトのX軸の間にいる
						if (pPosOld->z + fRange <= pos.z + VtxMin.z && pPos->z + fRange >= pos.z + VtxMin.z)
						{// 後ろからあたった
							pPos->z = pos.z + VtxMin.z - fRange;
							// 当たっている判定にする
							bCollision = true;
						}
						else if (pPosOld->z - fRange >= pos.z + VtxMax.z && pPos->z - fRange <= pos.z + VtxMax.z)
						{// 前からあたった
							pPos->z = pos.z + VtxMax.z + fRange + 1.0f;
							// 当たっている判定にする
							bCollision = true;
						}
					}
				}
			}
		}
	}

	return bCollision;
}

//=============================================================================
//    ドアを開く状態かどうか設定する処理
//=============================================================================
void CObjectGoal::SetDoorOpen(const bool bDoorOpen)
{
	m_bDoorOpen = bDoorOpen;
}

//=============================================================================
//    ドアを開く状態かどうか取得する処理
//=============================================================================
bool CObjectGoal::GetDoorOpen(void)
{
	return m_bDoorOpen;
}