//*****************************************************************************
//
//     �T�E���h�̏���[sound.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �T�E���h�N���X�̒�`
//*****************************************************************************
class CSound
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �T�E���h���x��
	//------------------------
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,                  // �^�C�g����BGM
		SOUND_LABEL_BGM001,                      // �`���[�g���A����BGM
		SOUND_LABEL_BGM002,                      // �Q�[����BGM
		SOUND_LABEL_BGM003,                      // �Q�[����BGM(�{�X�펞)
		SOUND_LABEL_BGM004,                      // ���U���g��BGM(�Q�[���I�[�o�[��)
		SOUND_LABEL_BGM005,                      // ���U���g��BGM(�Q�[���N���A��)
		SOUND_LABEL_BGM006,                      // �����L���O��BGM(�Q�[���I�[�o�[��)
		SOUND_LABEL_SE_AVATER_SPAWN,             // ���g���o����
		SOUND_LABEL_SE_AVATER_DELETE,            // ���g��������
		SOUND_LABEL_SE_ENEMY_DEATH,              // �G�����񂾂Ƃ��̉�
		SOUND_LABEL_SE_ENEMY_SWING,              // �G���U�������Ƃ��̉�
		SOUND_LABEL_SE_GET_SCOREPAR,             // �X�R�A�p�[�e�B�N������肵���Ƃ��̉�
		SOUND_LABEL_SE_GRASS,                    // ���ނ�������
		SOUND_LABEL_SE_LOCKON,                   // ���b�N�I�������Ƃ��̉�
		SOUND_LABEL_SE_ORACLEATTACK,             // �I���N���A�^�b�N���̉�
		SOUND_LABEL_SE_PAUSE_OPEN,               // �|�[�Y��ʂ��J�����Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_DAMAGE000,         // �v���C���[���_���[�W���󂯂����̉�(1��ޖ�)
		SOUND_LABEL_SE_PLAYER_HIT,               // �v���C���[���ʏ�U���𓖂Ă��Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_HIT_OBJECT,        // �v���C���[���U����z�u���ɓ��Ă��Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_JUMP,              // �v���C���[���W�����v�����Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_LANDING,           // �v���C���[�����n�����Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_MAXORACLE,         // �K�E�Z�Q�[�W�����^���ɂȂ������̉�
		SOUND_LABEL_SE_PLAYER_ORACLE_START,      // �I���N���A�^�b�N������Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_ORBITSLASH,        // �U�����O�Վa�ɕς����Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_RIVER,             // �v���C���[������𑖂鉹
		SOUND_LABEL_SE_PLAYER_RUN,               // �v���C���[�����鉹
		SOUND_LABEL_SE_PLAYER_SLASHHIT,          // �v���C���[���O�Վa�𓖂Ă��Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_SLASHLASTATTACK,   // �v���C���[���Ō�̋O�Վa�U�����o�����Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_SLASH_SWING,       // �v���C���[���O�Վa���o�����Ƃ��̉�
		SOUND_LABEL_SE_PLAYER_SWING,             // �v���C���[���ʏ�U�����o�����Ƃ��̉�
		SOUND_LABEL_SE_PRESS,                    // ����{�^�����������Ƃ��̉�
		SOUND_LABEL_SE_SELECT,                   // ���ڂ�I�����Ă���Ƃ��̉�
		SOUND_LABEL_SE_WATER_FALL,               // ��̉�
		SOUND_LABEL_SE_BOSS_TORNADE,             // �{�X���������o���Ƃ��̉�
		SOUND_LABEL_SE_RESULT000,                // ���v�X�R�A�ȊO���\�����ꂽ�Ƃ��̉�
		SOUND_LABEL_SE_RESULT001,                // ���v�X�R�A���\�����ꂽ�Ƃ��̉�
		SOUND_LABEL_SE_RANKING,                  // �����L���O�X�R�A���~�܂����Ƃ��̉�
		SOUND_LABEL_SE_BOSSAPPEAR,               // �{�X�o�����Ɏg����
		SOUND_LABEL_SE_BOSSAPPEAR_EXPLOSION,     // �{�X�o�����Ɏg����(����)
		SOUND_LABEL_SE_GET_HIDDEN_ITEM,          // �B���A�C�e���擾���ɂȂ鉹
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

private:   // ���̃N���X�������A�N�Z�X�\
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								    // XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;			    // �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

	//------------------------
	//  �e���f�ނ̃p�����[�^
	//------------------------
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =    // ���f�ނ̃p�����[�^(�T�E���h���x�����p��)
	{// -1�Ȃ�BGM  0�Ȃ�SE
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