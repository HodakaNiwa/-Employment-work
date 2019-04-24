//*****************************************************************************
//
//     サウンドの処理[sound.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     サウンドクラスの定義
//*****************************************************************************
class CSound
{
public:    // 誰でもアクセス可能
	//------------------------
	//  サウンドラベル
	//------------------------
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,                  // タイトルのBGM
		SOUND_LABEL_BGM001,                      // チュートリアルのBGM
		SOUND_LABEL_BGM002,                      // ゲームのBGM
		SOUND_LABEL_BGM003,                      // ゲームのBGM(ボス戦時)
		SOUND_LABEL_BGM004,                      // リザルトのBGM(ゲームオーバー時)
		SOUND_LABEL_BGM005,                      // リザルトのBGM(ゲームクリア時)
		SOUND_LABEL_BGM006,                      // ランキングのBGM(ゲームオーバー時)
		SOUND_LABEL_SE_AVATER_SPAWN,             // 分身を出す音
		SOUND_LABEL_SE_AVATER_DELETE,            // 分身を消す音
		SOUND_LABEL_SE_ENEMY_DEATH,              // 敵が死んだときの音
		SOUND_LABEL_SE_ENEMY_SWING,              // 敵が攻撃したときの音
		SOUND_LABEL_SE_GET_SCOREPAR,             // スコアパーティクルを入手したときの音
		SOUND_LABEL_SE_GRASS,                    // 草むらを歩く音
		SOUND_LABEL_SE_LOCKON,                   // ロックオンしたときの音
		SOUND_LABEL_SE_ORACLEATTACK,             // オラクルアタック時の音
		SOUND_LABEL_SE_PAUSE_OPEN,               // ポーズ画面を開いたときの音
		SOUND_LABEL_SE_PLAYER_DAMAGE000,         // プレイヤーがダメージを受けた時の音(1種類目)
		SOUND_LABEL_SE_PLAYER_HIT,               // プレイヤーが通常攻撃を当てたときの音
		SOUND_LABEL_SE_PLAYER_HIT_OBJECT,        // プレイヤーが攻撃を配置物に当てたときの音
		SOUND_LABEL_SE_PLAYER_JUMP,              // プレイヤーがジャンプしたときの音
		SOUND_LABEL_SE_PLAYER_LANDING,           // プレイヤーが着地したときの音
		SOUND_LABEL_SE_PLAYER_MAXORACLE,         // 必殺技ゲージが満タンになった時の音
		SOUND_LABEL_SE_PLAYER_ORACLE_START,      // オラクルアタックをするときの音
		SOUND_LABEL_SE_PLAYER_ORBITSLASH,        // 攻撃を軌跡斬に変えたときの音
		SOUND_LABEL_SE_PLAYER_RIVER,             // プレイヤーが水上を走る音
		SOUND_LABEL_SE_PLAYER_RUN,               // プレイヤーが走る音
		SOUND_LABEL_SE_PLAYER_SLASHHIT,          // プレイヤーが軌跡斬を当てたときの音
		SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK,   // プレイヤーが最後の軌跡斬攻撃を出したときの音
		SOUND_LABEL_SE_PLAYER_SLASH_SWING,       // プレイヤーが軌跡斬を出したときの音
		SOUND_LABEL_SE_PLAYER_SWING,             // プレイヤーが通常攻撃を出したときの音
		SOUND_LABEL_SE_PRESS,                    // 決定ボタンを押したときの音
		SOUND_LABEL_SE_SELECT,                   // 項目を選択しているときの音
		SOUND_LABEL_SE_WATER_FALL,               // 滝の音
		SOUND_LABEL_SE_BOSS_TORNADE,             // ボスが竜巻を出すときの音
		SOUND_LABEL_SE_RESULT000,                // 合計スコア以外が表示されたときの音
		SOUND_LABEL_SE_RESULT001,                // 合計スコアが表示されたときの音
		SOUND_LABEL_SE_RANKING,                  // ランキングスコアが止まったときの音
		SOUND_LABEL_SE_BOSSAPPEAR,               // ボス出現時に使う音
		SOUND_LABEL_SE_BOSSAPPEAR_EXPLOSION,     // ボス出現時に使う音(爆発)
		SOUND_LABEL_SE_GET_HIDDEN_ITEM,          // 隠しアイテム取得時になる音
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label, bool bPause = false);
	void StopSound(SOUND_LABEL label, bool bPause = false);
	void StopSound(void);
	void SetVoluem(SOUND_LABEL label, float fValue);

private:   // このクラスだけがアクセス可能
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								    // XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			    // マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	//------------------------
	//  各音素材のパラメータ
	//------------------------
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =    // 音素材のパラメータ(サウンドラベル分用意)
	{// -1ならBGM  0ならSE
		{ "data/BGM/bgm000.wav",-1 },
		{ "data/BGM/bgm001.wav",-1 },
		{ "data/BGM/bgm002.wav",-1 },
		{ "data/BGM/bgm003.wav",-1 },
		{ "data/BGM/bgm004.wav",-1 },
		{ "data/BGM/bgm005.wav",-1 },
		{ "data/BGM/bgm006.wav",-1 },
		{ "data/SE/avater_appear.wav",0 },
		{ "data/SE/avater_delete.wav",0 },
		{ "data/SE/enemy_death.wav",0 },
		{ "data/SE/enemy_swing.wav",0 },
		{ "data/SE/get_scorepar.wav",0 },
		{ "data/SE/grass000.wav",0 },
		{ "data/SE/lockon.wav",0 },
		{ "data/SE/oracle_attack.wav",0 },
		{ "data/SE/pause_open.wav",0 },
		{ "data/SE/player_damage000.wav",0 },
		{ "data/SE/player_hit.wav",0 },
		{ "data/SE/player_hit_object.wav",0 },
		{ "data/SE/player_jump.wav",0 },
		{ "data/SE/player_landing.wav",0 },
		{ "data/SE/player_maxoracle.wav",0 },
		{ "data/SE/player_oracle_start.wav",0 },
		{ "data/SE/player_orbitslash.wav",0 },
		{ "data/SE/player_river.wav",0 },
		{ "data/SE/player_run.wav",0 },
		{ "data/SE/player_slash_hit.wav",0 },
		{ "data/SE/player_slash_lastattack.wav",0 },
		{ "data/SE/player_slash_swing.wav",0 },
		{ "data/SE/player_swing.wav",0 },
		{ "data/SE/press.wav",0 },
		{ "data/SE/select.wav",0 },
		{ "data/SE/water_fall.wav",-1 },
		{ "data/SE/boss_tornade.wav",0 },
		{ "data/SE/result_advance000.wav",0 },
		{ "data/SE/result_advance001.wav",0 },
		{ "data/SE/ranking_advance.wav",0 },
		{ "data/SE/boss_appear.wav",0 },
		{ "data/SE/boss_appear_explosion.wav",0 },
		{ "data/SE/get_hiddenitem.wav",0 },
	};
};

#endif