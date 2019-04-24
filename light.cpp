//*****************************************************************************
//
//     ライトの処理[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#ifdef _DEBUG  // デバッグ中の処理
#define LIGHT_DEBUG_VECTOR_UPDOWN  (0.01f)  // ライトの向き情報をどれくらい動かすか
#define LIGHT_DEBUG_COLOR_UPDOWN   (0.01f)  // ライトの色情報をどれくらい変化させるか
#endif

//=============================================================================
//    コンストラクタ
//=============================================================================
CLight::CLight()
{
	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの数だけ繰り返し
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));
	}

#ifdef _DEBUG  // デバッグ中の処理
	m_nSelectLight = 0;   // 現在選択されているライトの色
#endif
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLight::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXVECTOR3 vecDir;  // カメラの法線設定用

		    // ライトの種類を設定
			m_Light[0].Type = D3DLIGHT_DIRECTIONAL;
			m_Light[1].Type = D3DLIGHT_DIRECTIONAL;
			m_Light[2].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_Light[1].Diffuse = D3DXCOLOR(0.23f, 0.23f, 0.23f, 1.0f);
			m_Light[2].Diffuse = D3DXCOLOR(0.23f, 0.23f, 0.23f, 1.0f);

			// ライトの方向の設定
			vecDir = D3DXVECTOR3(-0.56f, -0.47f, 1.0f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[0].Direction = vecDir;
			vecDir = D3DXVECTOR3(0.6f, -0.14f, -0.19f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[1].Direction = vecDir;
			vecDir = D3DXVECTOR3(-0.81f, -0.18f, -1.0f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[2].Direction = vecDir;

			// ライトを設定する
			for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
			{// ライトの数だけ繰り返し
				pDevice->SetLight(nCntLight, &m_Light[nCntLight]);
			}

			// 設定したライトを有効にする
			for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
			{// ライトの数だけ繰り返し
				pDevice->LightEnable(nCntLight, TRUE);
			}
		}
	}
	else
	{// レンダリングクラスが取得できなかった
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLight::Uninit(void)
{
	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの数だけ繰り返し
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CLight::Update(void)
{
#ifdef _DEBUG  // デバッグ中の処理
	if (GetUpdate() == false)
	{// 更新停止中だったら
		CInputKeyboard *pKeyboard = CManager::GetKeyboard();
		if (pKeyboard != NULL)
		{// キーボードが取得できた
			if (pKeyboard->GetTrigger(DIK_UP))
			{// 矢印上方向キーが押された
				m_nSelectLight = (m_nSelectLight + 1) % MAX_LIGHT;
			}
			else if (pKeyboard->GetTrigger(DIK_DOWN))
			{// 矢印下方向キーが押された
				m_nSelectLight = (m_nSelectLight + (MAX_LIGHT - 1)) % MAX_LIGHT;
			}
			else if (pKeyboard->GetPress(DIK_T))
			{// Tキーが押された
				m_Light[m_nSelectLight].Direction.x += LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_G))
			{// Gキーが押された
				m_Light[m_nSelectLight].Direction.x -= LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_Y))
			{// Yキーが押された
				m_Light[m_nSelectLight].Direction.y += LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_H))
			{// Hキーが押された
				m_Light[m_nSelectLight].Direction.y -= LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_U))
			{// Uキーが押された
				m_Light[m_nSelectLight].Direction.z += LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_J))
			{// Jキーが押された
				m_Light[m_nSelectLight].Direction.z -= LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_Q))
			{// Qキーが押された
				m_Light[m_nSelectLight].Diffuse.r += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_A))
			{// Aキーが押された
				m_Light[m_nSelectLight].Diffuse.r -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_W))
			{// Wキーが押された
				m_Light[m_nSelectLight].Diffuse.g += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_S))
			{// Sキーが押された
				m_Light[m_nSelectLight].Diffuse.g -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_E))
			{// Eキーが押された
				m_Light[m_nSelectLight].Diffuse.b += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// Dキーが押された
				m_Light[m_nSelectLight].Diffuse.b -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_R))
			{// Rキーが押された
				m_Light[m_nSelectLight].Diffuse.a += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_F))
			{// Fキーが押された
				m_Light[m_nSelectLight].Diffuse.a -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
		}

		// ライト情報チェック
		// ライトの向き
		// X軸
		if (m_Light[m_nSelectLight].Direction.x < -1.0f)
		{
			m_Light[m_nSelectLight].Direction.x = -1.0f;
		}
		else if (m_Light[m_nSelectLight].Direction.x > 1.0f)
		{
			m_Light[m_nSelectLight].Direction.x = 1.0f;
		}

		// Y軸
		if (m_Light[m_nSelectLight].Direction.y < -1.0f)
		{
			m_Light[m_nSelectLight].Direction.y = -1.0f;
		}
		else if (m_Light[m_nSelectLight].Direction.y > 1.0f)
		{
			m_Light[m_nSelectLight].Direction.y = 1.0f;
		}

		// Z軸
		if (m_Light[m_nSelectLight].Direction.z < -1.0f)
		{
			m_Light[m_nSelectLight].Direction.z = -1.0f;
		}
		else if (m_Light[m_nSelectLight].Direction.z > 1.0f)
		{
			m_Light[m_nSelectLight].Direction.z = 1.0f;
		}

		// ライトの色
		// 赤色
		if (m_Light[m_nSelectLight].Diffuse.r < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.r = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.r > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.r = 1.0f;
		}

		// 緑色
		if (m_Light[m_nSelectLight].Diffuse.g < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.g = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.g > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.g = 1.0f;
		}

		// 青色
		if (m_Light[m_nSelectLight].Diffuse.b < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.b = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.b > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.b = 1.0f;
		}

		// 透明色
		if (m_Light[m_nSelectLight].Diffuse.a < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.a = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.a > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.a = 1.0f;
		}

		// ライトを設定する
		CManager::GetRenderer()->GetDevice()->SetLight(m_nSelectLight, &m_Light[m_nSelectLight]);

		// ライトの情報を描画
		CDebugProc::Print("現在選択されているライト番号 : %d\n", m_nSelectLight);
		CDebugProc::Print("ライトの色   : ( %.3f %.3f %.3f %.3f )\n", m_Light[m_nSelectLight].Diffuse.r, m_Light[m_nSelectLight].Diffuse.g, m_Light[m_nSelectLight].Diffuse.b, m_Light[m_nSelectLight].Diffuse.a);
		CDebugProc::Print("ライトの向き : ( %.3f %.3f %.3f )\n", m_Light[m_nSelectLight].Direction.x, m_Light[m_nSelectLight].Direction.y, m_Light[m_nSelectLight].Direction.z);
	}
#endif
}