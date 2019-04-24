//*****************************************************************************
//
//     �V�[���̏���[scene.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_PRIORITY_NUM (14)      // �����̗D�揇�ʂ̍ő吔
#define MAX_SCENE        (40000)   // �V�[�������鐔

//*****************************************************************************
//    �V�[���N���X�̒�`
//*****************************************************************************
class CScene
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �I�u�W�F�N�g�̎��
	//------------------------
	typedef enum
	{
		OBJTYPE_NONE = -1,     // ����
		OBJTYPE_TITLE,         // �^�C�g��
		OBJTYPE_TUTORIAL,      // �`���[�g���A��
		OBJTYPE_GAME,          // �Q�[��
		OBJTYPE_RESULT,        // ���U���g
		OBJTYPE_RANKING,       // �����L���O
		OBJTYPE_PAUSE,         // �|�[�Y
		OBJTYPE_2DPOLYGON,     // 2D�|���S��
		OBJTYPE_3DPOLYGON,     // 3D�|���S��
		OBJTYPE_GRASS,         // ��
		OBJTYPE_BILLBOARD,     // �r���{�[�h
		OBJTYPE_BILLBOARDOBJ,  // �z�u�r���{�[�h
		OBJTYPE_ITEM,          // �A�C�e��
		OBJTYPE_SETITEM,       // �z�u�p�̃A�C�e��
		OBJTYPE_XFILEMODEL,    // X�t�@�C�����f��
		OBJTYPE_MESH2D,        // ���b�V��2D�|���S��
		OBJTYPE_MESHFIELD,     // ���b�V���t�B�[���h(�n��)
		OBJTYPE_RIVER,         // ��
		OBJTYPE_MESHWALL,      // ���b�V���E�H�[��(��)
		OBJTYPE_MESHCYLINDER,  // ���b�V���V�����_�[(�~��)
		OBJTYPE_MESHDOME,      // ���b�V���h�[��(����)
		OBJTYPE_MESHSPHERE,    // ���b�V���X�t�B�A(��)
		OBJTYPE_MESHORBIT,     // ���b�V���I�[�r�b�g(�O��)
		OBJTYPE_MESHRING,      // ���b�V�������O(�ւ���)
		OBJTYPE_CHARACTER,     // �L�����N�^�[
		OBJTYPE_PLAYER,        // �v���C���[
		OBJTYPE_PLAYERMANAGER, // �v���C���[�Ǌ��N���X
		OBJTYPE_ENEMY,         // �G�l�~�[
		OBJTYPE_BOSS,          // �{�X
		OBJTYPE_SETENEMY,      // �z�u�p�̓G
		OBJTYPE_ENEMYMANAGER,  // �G�l�~�[�Ǌ��N���X
		OBJTYPE_EMITTER,       // �G�~�b�^�[
		OBJTYPE_PAREMITTER,    // �p�[�e�B�N���G�~�b�^�[
		OBJTYPE_RINGEMITTER,   // �����O�G�t�F�N�g�G�~�b�^�[
		OBJTYPE_PARTICLE,      // �p�[�e�B�N��
		OBJTYPE_PARTICLE2D,    // �p�[�e�B�N��(2D)
		OBJTYPE_SCOREPARTICLE, // �X�R�A�p�[�e�B�N��
		OBJTYPE_ORBITEFFECT,   // �O�ՃG�t�F�N�g
		OBJTYPE_RINGEFFECT,    // �����O�G�t�F�N�g
		OBJTYPE_EFFECTMANAGER, // �G�t�F�N�g�Ǌ��N���X
		OBJTYPE_SETMESHWALL,   // �z�u�p�̕�
		OBJTYPE_SKY,           // ��
		OBJTYPE_MOUNTAIN,      // �R
		OBJTYPE_GOALCYLINDER,  // �S�[���p�~��
		OBJTYPE_OBJECT,        // �z�u��
		OBJTYPE_OBJECTMOVE,    // �����z�u��
		OBJTYPE_OBJECTGOAL,    // �S�[���p�z�u��
		OBJTYPE_GAUGE,         // �Q�[�W
		OBJTYPE_GAUGE2D,       // �Q�[�W(2D)
		OBJTYPE_LIFEGAUGE,     // �̗̓Q�[�W
		OBJTYPE_LIFEGAUGE2D,   // �̗̓Q�[�W(2D)
		OBJTYPE_SLASHGAUGE2D,  // �O�ՃQ�[�W(2D)
		OBJTYPE_SPECIALGAUGE2D,// �K�E�Z�Q�[�W(2D)
		OBJTYPE_TIMER,         // �^�C�}�[
		OBJTYPE_SCORE,         // �X�R�A
		OBJTYPE_COMBOUI,       // �R���{UI
		OBJTYPE_SHADOW,        // �e(�X�v���C�g)
		OBJTYPE_TORNADE,       // ����
		OBJTYPE_BOXCOLLIDER,   // �����蔻��p�����f��
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int GetNumAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);
	static void DeathCheck(void);
	static CScene *GetTop(int nPriority);

	void SetObjType(OBJTYPE ObjType);
	OBJTYPE GetObjType(void);
	CScene *GetNext(void);
	CScene *GetPrev(void);
	int GetPriority(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void Release(void);

private:   // ���̃N���X�������A�N�Z�X�\
	static int    m_nNumAll;                      // �V�[���̍ő吔
	static CScene *m_apTop[MAX_PRIORITY_NUM];     // �擪�̃V�[���N���X�ւ̃|�C���^(�J�n�ʒu���킩��Ȃ��Ȃ�̂ŕۑ�����)
	static CScene *m_apCur[MAX_PRIORITY_NUM];     // ���݂̍Ō���V�[���N���X�ւ̃|�C���^(�V�����V�[���N���X�ւ̃|�C���^��ǉ����邽�߂ɕۑ�)
	CScene        *m_pPrev;                       // �����̑O�̃V�[���N���X�ւ̃|�C���^
	CScene        *m_pNext;                       // �����̎��̃V�[���N���X�ւ̃|�C���^
	bool          m_bDeath;                       // ���S�t���O
	OBJTYPE       m_ObjType;                      // �I�u�W�F�N�g�̎��
	int           m_nPriority;                    // �D�揇�ʂ̔ԍ�
};

#endif