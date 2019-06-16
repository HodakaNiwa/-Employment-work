//*****************************************************************************
//
//     �}�b�v�̏���[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "map.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sky.h"
#include "mountain.h"
#include "object.h"
#include "meshField.h"
#include "river.h"
#include "meshWall.h"
#include "item.h"
#include "billboardObject.h"
#include "modelManager.h"
#include "textureManager.h"
#include "effectManager.h"
#include "enemy.h"
#include "player.h"
#include "emitter.h"
#include "scene.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
//#define MAP_AUTOSAVE                                                           // �錾�� : �I�[�g�Z�[�u

// �}�b�v�z�u�p
#define MAP_SET_ENEMY              (true)                                        // �G��z�u���邩�ǂ���
#define MAP_SET_OBJECT             (true)                                        // ���f����z�u���邩�ǂ���
#define MAP_SET_EFFECT             (true)                                        // �G�t�F�N�g��z�u���邩�ǂ���

// �Z�[�u�f�[�^�p
#define MAP_OBJECT_SAVEFILENAME    "data/TEXT/MAP/OBJECT/save_object.txt"        // �z�u������ۑ�����e�L�X�g�t�@�C����
#define MAP_ENEMY_SAVEFILENAME     "data/TEXT/MAP/ENEMY/save_set_enemy.txt"      // �G�̔z�u����ۑ�����e�L�X�g�t�@�C����
#define MAP_BILLBOARD_SAVEFILENAME "data/TEXT/MAP/BILLBOARD/save_billboard.txt"  // �z�u�p�r���{�[�h����ۑ�����e�L�X�g�t�@�C����
#define MAP_EFFECT_SAVEFILENAME    "data/TEXT/MAP/EFFECT/save_effect.txt"        // �z�u�G�t�F�N�g����ۑ�����e�L�X�g�t�@�C����
#define MAP_WALL_SAVEFILENAME      "data/TEXT/MAP/WALL/save_wall.txt"            // �Ǐ���ۑ�����e�L�X�g�t�@�C����
#define MAP_ITEM_SAVEFILENAME      "data/TEXT/MAP/ITEM/save_item.txt"            // �A�C�e������ۑ�����e�L�X�g�t�@�C����

// �X�N���v�g�t�@�C���̓ǂݍ��ݗp�̃p�X��
#define MAPSET               "MAPSET"                  // �}�b�v�Z�b�g�J�n�̍��}
#define END_MAPSET           "END_MAPSET"              // �}�b�v�Z�b�g�I���̍��}
#define FIELDSET             "FIELDSET"                // �n�ʃZ�b�g�J�n�̍��}
#define END_FIELDSET         "END_FIELDSET"            // �n�ʃZ�b�g�I���̍��}
#define RIVERSET             "RIVERSET"                // ��Z�b�g�J�n�̍��}
#define END_RIVERSET         "END_RIVERSET"            // ��Z�b�g�I���̍��}
#define OBJECTSET            "OBJECTSET"               // �z�u���Z�b�g�J�n�̍��}
#define END_OBJECTSET        "END_OBJECTSET"           // �z�u���Z�b�g�I���̍��}
#define OBJECTMOVESET        "OBJECTMOVESET"           // �����z�u���Z�b�g�J�n�̍��}
#define END_OBJECTMOVESET    "END_OBJECTMOVESET"       // �����z�u���Z�b�g�I���̍��}
#define OBJECTGOALSET        "OBJECTGOALSET"           // �S�[���p�z�u���Z�b�g�J�n�̍��}
#define END_OBJECTGOALSET    "END_OBJECTGOALSET"       // �S�[���p�z�u���Z�b�g�I���̍��}
#define POLYGONSET           "POLYGONSET"              // �S�[���p�|���S�����J�n�̍��}
#define END_POLYGONSET       "END_POLYGONSET"          // �S�[���p�|���S�����I���̍��}
#define MOUNTAINSET          "MOUNTAINSET"             // �R�Z�b�g�J�n�̍��}
#define END_MOUNTAINSET      "END_MOUNTAINSET"         // �R�Z�b�g�I���̍��}
#define SKYSET               "SKYSET"                  // ��Z�b�g�J�n�̍��}
#define END_SKYSET           "END_SKYSET"              // ��Z�b�g�I���̍��}
#define ENEMYSET             "ENEMYSET"                // �G�̔z�u�f�[�^�ǂݍ��݊J�n�̍��}
#define END_ENEMYSET         "END_ENEMYSET"            // �G�̔z�u�f�[�^�ǂݍ��ݏI���̍��}
#define BILLBOARDSET         "BILLBOARDSET"            // �z�u�p�r���{�[�h�ǂݍ��݊J�n�̍��}
#define END_BILLBOARDSET     "END_BILLBOARDSET"        // �z�u�p�r���{�[�h�ǂݍ��ݏI���̍��}
#define EFFECTSET            "EFFECTSET"               // �z�u�G�t�F�N�g�ǂݍ��݊J�n�̍��}
#define END_EFFECTSET        "END_EFFECTSET"           // �z�u�G�t�F�N�g�ǂݍ��ݏI���̍��}
#define WALLSET              "WALLSET"                 // �ǃZ�b�g�ǂݍ��݊J�n�̍��}
#define END_WALLSET          "END_WALLSET"             // �ǃZ�b�g�ǂݍ��ݏI���̍��}
#define ITEMSET              "ITEMSET"                 // �A�C�e���ǂݍ��݊J�n�̍��}
#define END_ITEMSET          "END_ITEMSET"             // �A�C�e���ǂݍ��ݏI���̍��}
#define AISET                "AISET"                   // �G��AI���ǂݍ��݊J�n�̍��}
#define END_AISET            "END_AISET"               // �G��AI���ǂݍ��ݏI���̍��}

// �t�@�C�����ǂݎ��p
#define FIELD_FILENAME       "FIELD_FILENAME = "       // �n�ʂ̃X�N���v�g�t�@�C����
#define RIVER_FILENAME       "RIVER_FILENAME = "       // ��̃X�N���v�g�t�@�C����
#define OBJECT_FILENAME      "OBJECT_FILENAME = "      // �z�u���̃X�N���v�g�t�@�C����
#define MOUNTAIN_FILENAME    "MOUNTAIN_FILENAME = "    // �R�̃X�N���v�g�t�@�C����
#define SKY_FILENAME         "SKY_FILENAME = "         // ��̃X�N���v�g�t�@�C����
#define ENEMY_FILENAME       "ENEMY_FILENAME = "       // �G�̃X�N���v�g�t�@�C����
#define BILLBOARD_FILENAME   "BILLBOARD_FILENAME = "   // �z�u�p�r���{�[�h�̃X�N���v�g�t�@�C����
#define EFFECT_FILENAME      "EFFECT_FILENAME = "      // �z�u�G�t�F�N�g�̃X�N���v�g�t�@�C����
#define WALL_FILENAME        "WALL_FILENAME = "        // �ǂ̃X�N���v�g�t�@�C����
#define ITEM_FILENAME        "ITEM_FILENAME = "        // �A�C�e���̃X�N���v�g�t�@�C����
#define PLAYER_DEFAULT_SPAWN "PLAYER_DEFAULT_SPAWN = " // �v���C���[�̃f�t�H���g�X�|�[���ʒu
#define CAMERA_DEFAULT_ROT   "CAMERA_DEFAULT_ROT = "   // �J�����̃f�t�H���g����

// ���̑�
#define NUM_TEXTURE          "NUM_TEXTURE = "          // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME     "TEXTURE_FILENAME = "     // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL            "NUM_MODEL = "            // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME       "MODEL_FILENAME = "       // �ǂݍ��ރ��f���t�@�C����
#define ENEMYDATA_FILENAME   "ENEMYDATA_FILENAME = "   // �ǂݍ��ޓG�̃f�[�^�t�@�C����

// �f�[�^�ǂݎ��p
#define TEX_TYPE             "TEX_TYPE = "             // �e�N�X�`���̔ԍ����
#define TYPE                 "TYPE = "                 // ��ޏ��
#define POS                  "POS = "                  // ���W���
#define POS_TARGET           "POS_TARGET = "           // �ړI�̈ʒu
#define OFFSET               "OFFSET = "               // �I�t�Z�b�g�ʒu���
#define ROT                  "ROT = "                  // �������
#define SCALE                "SCALE = "                // �傫�����
#define COL                  "COL = "                  // �F���
#define MOVE                 "MOVE = "                 // �ړ��ʏ��
#define MOVE_MAX             "MOVE_MAX = "             // �ړ��ʂ�؂�ւ��鎞�ԏ��
#define PUSH                 "PUSH = "                 // �����邩�ǂ���
#define BREAK                "BREAK = "                // �󂹂邩�ǂ���
#define ROLL_SPEED           "ROLL_SPEED = "           // ��]����X�s�[�h�ʏ��
#define CARRY_TIME           "CARRY_TIME = "           // �v���C���[���^�Ԃ̂ɂ����鎞��
#define COL_RANGE            "COL_RANGE = "            // �����蔻�������͈�
#define COLLISION            "COLLISION = "            // �����蔻������邩�ǂ���(�܂��͎��)
#define WIDTH                "WIDTH = "                // 1�u���b�N���̕����
#define DEPTH                "DEPTH = "                // 1�u���b�N�����s���
#define RADIUS               "RADIUS = "               // ���a���
#define HEIGHT               "HEIGHT = "               // �������
#define XBLOCK               "XBLOCK = "               // ���̕��������
#define YBLOCK               "YBLOCK = "               // �c�̕��������
#define ZBLOCK               "ZBLOCK = "               // ���s�̕��������
#define TEXSPLIT_U           "TEXSPLIT_U = "           // �e�N�X�`��U���W�̕��������
#define TEXSPLIT_V           "TEXSPLIT_V = "           // �e�N�X�`��V���W�̕��������
#define ADDTEX_U             "ADDTEX_U = "             // �e�N�X�`��U���W���ǂꂭ�炢��������
#define ADDTEX_V             "ADDTEX_V = "             // �e�N�X�`��V���W���ǂꂭ�炢��������
#define CULLING              "CULLING = "              // �J�����O���邩�ǂ���
#define BINARY_FILENAME      "BINARY_FILENAME = "      // ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����
#define NEXT_MAPNAME         "NEXT_MAPNAME = "         // ���̃}�b�v�f�[�^�̃t�@�C����
#define LIFE                 "LIFE = "                 // �̗�
#define SCORE                "SCORE = "                // �|�������ɑ�����X�R�A��
#define LIGHTING             "LIGHTING = "             // ���C�e�B���O���邩���Ȃ���
#define DRAW_ADDTIVE         "DRAW_ADDTIVE = "         // ���Z�����ŕ`�悷�邩���Ȃ���
#define CHEACKDISTANCE       "CHEACKDISTANCE = "       // �x���ł���͈�
#define ATTACKDISTANCE       "ATTACKDISTANCE = "       // �U�����d�|����͈�
#define ATTACKRANDOM         "ATTACKRANDOM = "         // �U�����d�|����^�C�~���O

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMap::CMap()
{
	// �e��l�̏�����
	m_pModelManager = NULL;    // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pTextureManager = NULL;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pEffectManager = NULL;   // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_pEnemyAssembly = NULL;   // �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^
	m_nCntLoadEnemy = 0;       // �G�f�[�^��ǂݍ��񂾉�
	m_PlayerDefaultPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PlayerDefaultRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMap *CMap::Create(char *aFileName)
{
	CMap *pMap = NULL;  // �}�b�v�N���X�^�̃|�C���^
	if (pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pMap = new CMap;
		if (pMap != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pMap->Init(aFileName);
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	// �C���X�^���X��Ԃ�
	return pMap;
}

//=============================================================================
//    ����������
//=============================================================================
void CMap::Init(char *aFileName)
{
	// �}�b�v�̊O���t�@�C������ݒ�
	strcpy(m_aFileName, aFileName);

	// �X�N���v�g��ǂݍ���
	ScriptLoad_Open();
}

//=============================================================================
//    �I������
//=============================================================================
void CMap::Uninit(void)
{
#ifdef MAP_AUTOSAVE
	// �z�u����ۑ�����
	Save();
#endif

	// �}�b�v�Ǌ��N���X���J������
	ReleaseModelManager();

	// �e�N�X�`���Ǌ��N���X���J������
	ReleaseTextureManager();

	// �G�̊Ǌ��܂Ƃ߃N���X�̃��������J������
	ReleaseEnemyAssembley();

	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����ɂ���
	m_pEffectManager = NULL;

	// �}�b�v�̗v�f�ł���N���X���폜����
	DeleteMapObject();
}

//=============================================================================
//    �X�N���v�g�ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::ScriptLoad_Open(void)
{
	char aStrCur[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		strcpy(aStrCur, pFileLoader->GetString(aStrCur));
		if (CFunctionLib::Memcmp(aStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			ScriptLoad(pFileLoader, aStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �X�N���v�g�ǂݍ��ݏ���
//=============================================================================
void CMap::ScriptLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	int nCntTex = 0;    // �e�N�X�`����ǂݍ��񂾉�
	int nCntModel = 0;  // ���f����ǂݍ��񂾉�

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
		{// �e�N�X�`���̌���񂪂�����
			CreateTextureManager(CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
		{// �e�N�X�`���̃t�@�C���p�X����񂪂�����
			TextureLoad(pStrCur, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
		{// �e�N�X�`���̌���񂪂�����
			CreateModelManager(CFunctionLib::ReadInt(pStrCur, NUM_MODEL));
		}
		else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
		{// ���f���̃t�@�C���p�X����񂪂�����
			ModelLoad(pStrCur, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, FIELD_FILENAME) == 0)
		{// �n�ʂ̃X�N���v�g�t�@�C������������
			FieldLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, FIELD_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RIVER_FILENAME) == 0)
		{// ��̃X�N���v�g�t�@�C������������
			RiverLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, RIVER_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, OBJECT_FILENAME) == 0)
		{// �z�u���̃X�N���v�g�t�@�C������������
			if (MAP_SET_OBJECT)
			{// ���f����z�u����Ȃ�
				ObjectLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, OBJECT_FILENAME), pStrCur);
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, MOUNTAIN_FILENAME) == 0)
		{// �R�̃X�N���v�g�t�@�C������������
			MountainLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, MOUNTAIN_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, SKY_FILENAME) == 0)
		{// ��̃X�N���v�g�t�@�C������������
			SkyLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, SKY_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ENEMY_FILENAME) == 0)
		{// �G�̃X�N���v�g�t�@�C������������
			if (CManager::GetMode() == CManager::MODE_GAME && MAP_SET_ENEMY
				|| CManager::GetMode() == CManager::MODE_TUTORIAL && MAP_SET_ENEMY)
			{// �`���[�g���A����ʂ��Q�[����ʂ��G��z�u����Ȃ�
				EnemyLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, ENEMY_FILENAME), pStrCur);
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, BILLBOARD_FILENAME) == 0)
		{// �z�u�p�r���{�[�h�̃X�N���v�g�t�@�C������������
			BillboardLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, BILLBOARD_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
		{// �z�u�G�t�F�N�g�̃X�N���v�g�t�@�C������������
			if (MAP_SET_EFFECT)
			{// �G�t�F�N�g��z�u����Ȃ�
				EffectLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, EFFECT_FILENAME), pStrCur);
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, WALL_FILENAME) == 0)
		{// �ǂ̃X�N���v�g�t�@�C������������
			WallLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, WALL_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ITEM_FILENAME) == 0)
		{// �A�C�e���̃X�N���v�g�t�@�C������������
			ItemLoad_Open(CFunctionLib::ReadString(pStrCur, pStrCur, ITEM_FILENAME), pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PLAYER_DEFAULT_SPAWN) == 0)
		{// �v���C���[�̃f�t�H���g�X�|�[���ʒu��������
			PlayerDefaultSpawnLoad(pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, CAMERA_DEFAULT_ROT) == 0)
		{// �J�����̃f�t�H���g������������
			CameraDefaultRotLoad(pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�𐶐����鏈��
//=============================================================================
void CMap::CreateTextureManager(int nNumTexture)
{
	if (nNumTexture >= 1)
	{// ���f������1�ȏ゠��
		m_pTextureManager = CTextureManager::Create(nNumTexture);
	}
}

//=============================================================================
//    �e�N�X�`�����ǂݍ��ݏ���
//=============================================================================
void CMap::TextureLoad(char *pStrCur, int nCntTex)
{
	char aFileName[256] = "\0";
	LPDIRECT3DTEXTURE9 pTexture = NULL;  // �e�N�X�`���ǂݎ��p

	// �e�N�X�`���̃t�@�C���p�X����ǂݎ��
	strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, TEXTURE_FILENAME));

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aFileName, &pTexture);
	if (pTexture != NULL)
	{// �e�N�X�`����ǂݍ��߂�
		m_pTextureManager->BindTexture(pTexture, nCntTex);
		m_pTextureManager->SetFileName(aFileName, nCntTex);
		pTexture = NULL;
	}
}

//=============================================================================
//    ���f���Ǌ��N���X�𐶐����鏈��
//=============================================================================
void CMap::CreateModelManager(int nNumModel)
{
	if (nNumModel >= 1)
	{// ���f������1�ȏ゠��
		m_pModelManager = CModelManager::Create(nNumModel);
	}
}

//=============================================================================
//    ���f�����ǂݍ��ݏ���
//=============================================================================
void CMap::ModelLoad(char *pStrCur, int nCntModel)
{
	char aFileName[256] = "\0";
	LPD3DXMESH pMesh = NULL;         // ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;    // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;               // �}�e���A�����̐�

	// ���f���̃t�@�C���p�X����ǂݎ��
	strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, MODEL_FILENAME));

	// ���f���̓ǂݍ���
	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(aFileName,
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&pBuffMat,
		NULL,
		&nNumMat,
		&pMesh);

	// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
	m_pModelManager->SetMesh(pMesh, nCntModel);
	m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
}

//=============================================================================
//    �v���C���[�̃X�|�[���ʒu�ǂݍ��ݏ���
//=============================================================================
void CMap::PlayerDefaultSpawnLoad(char *pStrCur)
{
	m_PlayerDefaultPos = CFunctionLib::ReadVector3(pStrCur, PLAYER_DEFAULT_SPAWN);
}

//=============================================================================
//    �J�����̌����������ǂݍ��ݏ���
//=============================================================================
void CMap::CameraDefaultRotLoad(char *pStrCur)
{
	D3DXVECTOR3 DefaultCameraRot = CFunctionLib::ReadVector3(pStrCur, CAMERA_DEFAULT_ROT);

	// ���������W�A���p�ɖ߂�
	DefaultCameraRot.x = D3DXToRadian(DefaultCameraRot.x);
	DefaultCameraRot.y = D3DXToRadian(DefaultCameraRot.y);
	DefaultCameraRot.z = D3DXToRadian(DefaultCameraRot.z);
	// X��
	if (DefaultCameraRot.x > D3DX_PI)
	{
		DefaultCameraRot.x -= D3DX_PI * 2.0f;
	}
	// Y��
	if (DefaultCameraRot.y > D3DX_PI)
	{
		DefaultCameraRot.y -= D3DX_PI * 2.0f;
	}
	// Z��
	if (DefaultCameraRot.z > D3DX_PI)
	{
		DefaultCameraRot.z -= D3DX_PI * 2.0f;
	}

	CManager::GetCamera()->SetRot(DefaultCameraRot);
	CManager::GetCamera()->SetRotDest(DefaultCameraRot);
	CManager::GetCamera()->SetRotDiff(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//    �n�ʓǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::FieldLoad_Open(char *pFieldFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pFieldFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			FieldLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �n�ʓǂݍ��ݏ���
//=============================================================================
void CMap::FieldLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, FIELDSET) == 0)
		{// �n�ʏ��ǂݍ��݊J�n�̍��}������
			FieldLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �n�ʏ��ǂݍ��ݏ���
//=============================================================================
void CMap::FieldLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �n�ʗp�̊i�[�ϐ�
	int nFieldTexIdx = 0;                                    // �n�ʂ̃e�N�X�`���ԍ�
	D3DXVECTOR3 FieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �n�ʂ̍��W
	D3DXVECTOR3 FieldRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �n�ʂ̌���
	D3DXCOLOR FieldCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �n�ʂ̐F
	float fFieldWidth = 0.0f;                                // �n�ʂ�1�u���b�N���̕�
	float fFieldDepth = 0.0f;                                // �n�ʂ�1�u���b�N���̉��s
	int nFieldXblock = 0;                                    // �n�ʂ̉��̃u���b�N��
	int nFieldZblock = 0;                                    // �n�ʂ̉��s�̃u���b�N��
	int nFieldTexSplitU = 0;                                 // �n�ʂ̃e�N�X�`��U���W�̕�����
	int nFieldTexSplitV = 0;                                 // �n�ʂ̃e�N�X�`��V���W�̕�����
	bool bFieldCulling = true;                               // �J�����O���邩�ǂ���
	char aFileName[256] = "\0";                              // �n�ʂ̒��_��񂪊i�[���ꂽ�o�C�i���t�@�C����

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nFieldTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			FieldPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			FieldRot.x = D3DXToRadian(Rot.x);
			FieldRot.y = D3DXToRadian(Rot.y);
			FieldRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
		{// �F��񂾂���
			FieldCol = CFunctionLib::ReadVector4(pStrCur, COL);
		}
		else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
		{// 1�u���b�N���̕���񂾂���
			fFieldWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
		{// 1�u���b�N���̉��s��񂾂���
			fFieldDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
		{// ���̕�����������
			nFieldXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
		{// ���s�̕�����������
			nFieldZblock = CFunctionLib::ReadInt(pStrCur, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
		{// �e�N�X�`��U���W�̕�����������
			nFieldTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
		{// �e�N�X�`��V���W�̕�����������
			nFieldTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
		}
		else if (CFunctionLib::Memcmp(pStrCur, CULLING) == 0)
		{// �J�����O���邩�ǂ���������
			bFieldCulling = CFunctionLib::ReadBool(pStrCur, CULLING);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BINARY_FILENAME) == 0)
		{// ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����������
			strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_FIELDSET) == 0)
		{// �n�ʏ��ǂݍ��ݏI���̍��}������
		    // �n�ʂ𐶐�����
			CMeshField *pField = CMeshField::Create(FieldPos, FieldRot, FieldCol, fFieldWidth, fFieldDepth, nFieldXblock, nFieldZblock, nFieldTexSplitU, nFieldTexSplitV, aFileName, bFieldCulling, FIELD_PRIORITY);
			if (pField != NULL)
			{// �n�ʂ������ł���
				pField->BindTexture(m_pTextureManager->GetTexture(nFieldTexIdx));
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    ��ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::RiverLoad_Open(char *pRiverFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pRiverFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			RiverLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    ��ǂݍ��ݏ���
//=============================================================================
void CMap::RiverLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, RIVERSET) == 0)
		{// ����ǂݍ��݊J�n�̍��}������
			RiverLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    ����ǂݍ��ݏ���
//=============================================================================
void CMap::RiverLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// ��p�̊i�[�ϐ�
	int nRiverTexIdx = 0;                                    // ��̃e�N�X�`���ԍ�
	D3DXVECTOR3 RiverPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̍��W
	D3DXVECTOR3 RiverRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̌���
	D3DXCOLOR RiverCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ��̐F
	float fRiverWidth = 0.0f;                                // ���1�u���b�N���̕�
	float fRiverDepth = 0.0f;                                // ���1�u���b�N���̉��s
	int nRiverXblock = 0;                                    // ��̉��̃u���b�N��
	int nRiverZblock = 0;                                    // ��̉��s�̃u���b�N��
	int nRiverTexSplitU = 0;                                 // ��̃e�N�X�`��U���W�̕�����
	int nRiverTexSplitV = 0;                                 // ��̃e�N�X�`��V���W�̕�����
	float fRiverAddTexU = 0;                                 // ��̃e�N�X�`��U���W���ǂꂭ�炢��������
	float fRiverAddTexV = 0;                                 // ��̃e�N�X�`��V���W���ǂꂭ�炢��������
	char aFileName[256] = "\0";                              // �n�ʂ̒��_��񂪊i�[���ꂽ�o�C�i���t�@�C����

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nRiverTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			RiverPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			RiverRot.x = D3DXToRadian(Rot.x);
			RiverRot.y = D3DXToRadian(Rot.y);
			RiverRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
		{// �F��񂾂���
			RiverCol = CFunctionLib::ReadVector4(pStrCur, COL);
		}
		else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
		{// 1�u���b�N���̕���񂾂���
			fRiverWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
		{// 1�u���b�N���̉��s��񂾂���
			fRiverDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
		{// ���̕�����������
			nRiverXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
		{// ���s�̕�����������
			nRiverZblock = CFunctionLib::ReadInt(pStrCur, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
		{// �e�N�X�`��U���W�̕�����������
			nRiverTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
		{// �e�N�X�`��V���W�̕�����������
			nRiverTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ADDTEX_U) == 0)
		{// �e�N�X�`��U���W���ǂꂭ�炢����������񂾂���
			fRiverAddTexU = CFunctionLib::ReadFloat(pStrCur, ADDTEX_U);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ADDTEX_V) == 0)
		{// �e�N�X�`��V���W���ǂꂭ�炢����������񂾂���
			fRiverAddTexV = CFunctionLib::ReadFloat(pStrCur, ADDTEX_V);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BINARY_FILENAME) == 0)
		{// ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����������
			strcpy(aFileName, CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_RIVERSET) == 0)
		{// ����ǂݍ��ݏI���̍��}������
		    // ��𐶐�����
			CRiver *pRiver = CRiver::Create(RiverPos, RiverRot, RiverCol, fRiverWidth, fRiverDepth, nRiverXblock, nRiverZblock, nRiverTexSplitU, nRiverTexSplitV, fRiverAddTexU, fRiverAddTexV, aFileName, RIVER_PRIORITY);
			if (pRiver != NULL)
			{// �n�ʂ������ł���
				pRiver->BindTexture(m_pTextureManager->GetTexture(nRiverTexIdx));
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �z�u���ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::ObjectLoad_Open(char *pObjectFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pObjectFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			ObjectLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �z�u���ǂݍ��ݏ���
//=============================================================================
void CMap::ObjectLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, OBJECTSET) == 0)
		{// �z�u�����ǂݍ��݊J�n�̍��}������
			ObjectLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, OBJECTMOVESET) == 0)
		{// �����z�u�����ǂݍ��݊J�n�̍��}������
			ObjectMoveLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, OBJECTGOALSET) == 0)
		{// �S�[���p�z�u�����ǂݍ��݊J�n�̍��}������
			ObjectGoalLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �z�u�����ǂݍ��ݏ���
//=============================================================================
void CMap::ObjectLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �z�u���p�̊i�[�ϐ�
	int nObjectType = 0;                                            // �z�u���̃��f���ԍ�
	int nObjectColType = 0;                                         // �z�u���̓����蔻��̎��
	D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̍��W
	D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̌���
	D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // �z�u���̑傫���̔{��
	D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �z�u���̉�]�X�s�[�h
	float fObjectColRange = 0.0f;                                   // �z�u���̓����蔻������͈�(�~�`�ɓ����蔻�肷��ۂɎg�p����)
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// �g�p���郂�f���ԍ���񂾂���
			nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// �����蔻��̎�ޏ�񂾂���
			nObjectColType = CFunctionLib::ReadInt(pStrCur, COLLISION);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͏�񂾂���
			fObjectColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			ObjectRot.x = D3DXToRadian(Rot.x);
			ObjectRot.y = D3DXToRadian(Rot.y);
			ObjectRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
		{// �X�P�[����񂾂���
			ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
		{// ��]����X�s�[�h��񂾂���
			D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
			ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
			ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
			ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTSET) == 0)
		{// �z�u�����ǂݍ��ݏI���̍��}������
		    // �z�u���𐶐�����
			CObject *pObject = CObject::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, (CObject::COL_TYPE)nObjectColType, fObjectColRange, OBJECT_PRIORITY);
			if (pObject != NULL)
			{
				pObject->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
				pObject->SetModelIdx(nObjectType);
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �����z�u�����ǂݍ��ݏ���
//=============================================================================
void CMap::ObjectMoveLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �����z�u���p�̊i�[�ϐ�
	int nObjectType = 0;                                            // �z�u���̃��f���ԍ�
	int nObjectColType = 0;                                         // �z�u���̓����蔻��̎��
	D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̍��W
	D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̌���
	D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // �z�u���̑傫���̔{��
	D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �z�u���̉�]�X�s�[�h
	float fObjectColRange = 0.0f;                                   // �z�u���̓����蔻������͈�(�~�`�ɓ����蔻�肷��ۂɎg�p����)
	D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �z�u���̈ړ���
	int nObjectMaxCounter = 0;                                      // �z�u���̈ړ��ʂ�؂�ւ��鎞��

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// �g�p���郂�f���ԍ���񂾂���
			nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// �����蔻��̎�ޏ�񂾂���
			nObjectColType = CFunctionLib::ReadInt(pStrCur, COLLISION);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͏�񂾂���
			fObjectColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			ObjectRot.x = D3DXToRadian(Rot.x);
			ObjectRot.y = D3DXToRadian(Rot.y);
			ObjectRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
		{// �X�P�[����񂾂���
			ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
		{// ��]����X�s�[�h��񂾂���
			D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
			ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
			ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
			ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
		{// �ړ��ʏ�񂾂���
			ObjectMove = CFunctionLib::ReadVector3(pStrCur, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, MOVE_MAX) == 0)
		{// �ړ��ʂ�؂�ւ��鎞�ԏ�񂾂���
			nObjectMaxCounter = CFunctionLib::ReadInt(pStrCur, MOVE_MAX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTMOVESET) == 0)
		{// �����z�u�����ǂݍ��ݏI���̍��}������
		    // �����z�u���𐶐�����
			CObjectMove *pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, ObjectMove, nObjectMaxCounter, (CObject::COL_TYPE)nObjectColType, fObjectColRange, OBJECT_PRIORITY);
			if (pObjectMove != NULL)
			{
				pObjectMove->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
				pObjectMove->SetModelIdx(nObjectType);
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �S�[���p�z�u�����ǂݍ��ݏ���
//=============================================================================
void CMap::ObjectGoalLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �S�[���z�u���p�̊i�[�ϐ�
	int nObjectType = 0;                                            // �z�u���̃��f���ԍ�
	int nObjectColType = 0;                                         // �z�u���̓����蔻��̎��
	D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̍��W
	D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̌���
	D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // �z�u���̑傫���̔{��
	D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �z�u���̉�]�X�s�[�h
	float fObjectColRange = 0.0f;                                   // �z�u���̓����蔻������͈�(�~�`�ɓ����蔻�肷��ۂɎg�p����)

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// �g�p���郂�f���ԍ���񂾂���
			nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			ObjectRot.x = D3DXToRadian(Rot.x);
			ObjectRot.y = D3DXToRadian(Rot.y);
			ObjectRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
		{// �X�P�[����񂾂���
			ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
		{// ��]����X�s�[�h��񂾂���
			D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
			ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
			ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
			ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTGOALSET) == 0)
		{// �S�[���z�u�����ǂݍ��ݏI���̍��}������
		    // �S�[���z�u���𐶐�����
			CObjectGoal *pObjectGoal = CObjectGoal::Create(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, OBJECT_PRIORITY);
			if (pObjectGoal != NULL)
			{
				pObjectGoal->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
				pObjectGoal->SetModelIdx(nObjectType);
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �R�ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::MountainLoad_Open(char *pMountainFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pMountainFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			MountainLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �R�ǂݍ��ݏ���
//=============================================================================
void CMap::MountainLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, MOUNTAINSET) == 0)
		{// �R���ǂݍ��݊J�n�̍��}������
			MountainLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �z�u�����ǂݍ��ݏ���
//=============================================================================
void CMap::MountainLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �R�p�̊i�[�ϐ�
	int nMountainTexIdx = 0;                                    // �R�̃e�N�X�`���ԍ�
	D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �R�̍��W
	D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �R�̌���
	D3DXCOLOR MountainCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �R�̐F
	float fMountainRadius = 0.0f;                               // �R�̔��a
	float fMountainHeight = 0.0f;                               // �R�̍���
	int nMountainXblock = 0;                                    // �R�̉��̃u���b�N��
	int nMountainYblock = 0;                                    // �R�̏c�̃u���b�N��

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nMountainTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			MountainPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			MountainRot.x = D3DXToRadian(Rot.x);
			MountainRot.y = D3DXToRadian(Rot.y);
			MountainRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
		{// �F��񂾂���
			MountainCol = CFunctionLib::ReadVector4(pStrCur, COL);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
		{// �R�̔��a��񂾂���
			fMountainRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
		{// �R�̍�����񂾂���
			fMountainHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
		{// ���̕�����������
			nMountainXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
		{// �c�̕�����������
			nMountainYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOUNTAINSET) == 0)
		{// �R���ǂݍ��ݏI���̍��}������
		    // �R�𐶐�����
			CMountain *pMountain = CMountain::Create(MountainPos, MountainRot, MountainCol, fMountainHeight, fMountainRadius, nMountainXblock, nMountainYblock, MOUNTAIN_PRIORITY);
			if (pMountain != NULL)
			{// �R�������ł���
				pMountain->BindTexture(m_pTextureManager->GetTexture(nMountainTexIdx));
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    ��ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::SkyLoad_Open(char *pSkyFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pSkyFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			SkyLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    ��ǂݍ��ݏ���
//=============================================================================
void CMap::SkyLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SKYSET) == 0)
		{// ����ǂݍ��݊J�n�̍��}������
			SkyLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    ����ǂݍ��ݏ���
//=============================================================================
void CMap::SkyLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// ��p�̊i�[�ϐ�
	int nSkyTexIdx = 0;                                    // ��̃e�N�X�`���ԍ�
	D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̍��W
	D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̌���
	D3DXCOLOR SkyCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ��̐F
	float fSkyRadius = 0.0f;                               // ��̔��a
	int nSkyXblock = 0;                                    // ��̉��̃u���b�N��
	int nSkyYblock = 0;                                    // ��̏c�̃u���b�N��
	float fRotSpeed = 0.0f;                                // ��̉�]����X�s�[�h

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nSkyTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			SkyPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			SkyRot.x = D3DXToRadian(Rot.x);
			SkyRot.y = D3DXToRadian(Rot.y);
			SkyRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
		{// �F��񂾂���
			SkyCol = CFunctionLib::ReadVector4(pStrCur, COL);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
		{// ��̔��a��񂾂���
			fSkyRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
		{// ���̕�����������
			nSkyXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
		{// �c�̕�����������
			nSkyYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
		{// �c�̕�����������
			fRotSpeed = CFunctionLib::ReadFloat(pStrCur, ROLL_SPEED);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SKYSET) == 0)
		{// ����ǂݍ��ݏI���̍��}������
		    // ��𐶐�����
			CSky *pSky = CSky::Create(SkyPos, SkyRot, SkyCol, fSkyRadius, nSkyXblock, nSkyYblock, fRotSpeed, SKY_PRIORITY);
			if (pSky != NULL)
			{// �󂪐����ł���
				pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �G�ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::EnemyLoad_Open(char *pEnemyFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pEnemyFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			EnemyLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �G�ǂݍ��ݏ���
//=============================================================================
void CMap::EnemyLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	int nCntEnemyLoad = 0;                              // �G�l�~�[�f�[�^��ǂݍ��񂾉�
	char aEnemyDataFileName[256] = "\0";                // �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C����
	CEnemyManager *apEnemyManager[MAX_ENEMY_TYPE] = {}; // �G�Ǌ��N���X�ւ̃|�C���^

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, ENEMYDATA_FILENAME) == 0)
		{// �G�̃f�[�^���ǂݍ��݊J�n�̍��}������
			strcpy(aEnemyDataFileName, CFunctionLib::ReadString(pStrCur, aEnemyDataFileName, ENEMYDATA_FILENAME));
			apEnemyManager[nCntEnemyLoad] = EnemyLoad_Manager(pFileLoader, aEnemyDataFileName, nCntEnemyLoad);
			nCntEnemyLoad++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, ENEMYSET) == 0)
		{// �G���ǂݍ��݊J�n�̍��}������
			EnemyLoad_Info(pFileLoader, pStrCur, apEnemyManager);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			SetEnemyManagerToAssembley(apEnemyManager, nCntEnemyLoad);
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �G�Ǌ��N���X����ǂݍ��ޏ���
//=============================================================================
CEnemyManager *CMap::EnemyLoad_Manager(CFileLoader *pFileLoader, char *pFileName, int nCntEnemyLoad)
{
	// �G�Ǌ��N���X�̃|�C���^�𐶐�
	CEnemyManager *pEnemyManager = CEnemyManager::Create(nCntEnemyLoad, pFileName);
	return pEnemyManager;
}

//=============================================================================
//    �G���ǂݍ��ݏ���
//=============================================================================
void CMap::EnemyLoad_Info(CFileLoader *pFileLoader, char *pStrCur, CEnemyManager **apEnemyManager)
{
	// �G���i�[�p�ϐ�
	int nEnemyType = 0;
	D3DXVECTOR3 EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 EnemyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fEnemyAI_CheackDistance = 0.0f;
	float fEnemyAI_AttackDistance = 0.0f;
	int nEnemyAI_AttackTiming = 1;
	int nEnemyLife = 0;
	int nEnemyAddScore = 0;

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, AISET) == 0)
		{// AI�̏��ǂݍ��݊J�n�̍��}������
			EnemyLoad_Info_AI(pFileLoader, pStrCur, &fEnemyAI_CheackDistance, &fEnemyAI_AttackDistance, &nEnemyAI_AttackTiming);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// �G�̎�ނ�����
			nEnemyType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
		{// �̗͂�����
			nEnemyLife = CFunctionLib::ReadInt(pStrCur, LIFE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, SCORE) == 0)
		{// �G�̍��W��񂪂���
			nEnemyAddScore = CFunctionLib::ReadInt(pStrCur, SCORE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// �G�̍��W��񂪂���
			EnemyPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// �G�̌�����񂪂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			EnemyRot.x = D3DXToRadian(Rot.x);
			EnemyRot.y = D3DXToRadian(Rot.y);
			EnemyRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ENEMYSET) == 0)
		{// �G�̔z�u���ǂݍ��ݏI���̍��}������
			if (apEnemyManager[nEnemyType] != NULL)
			{// �G�̊Ǌ��N���X�ւ̃|�C���^���擾�ł��Ă���
				apEnemyManager[nEnemyType]->SetEnemy(EnemyPos, EnemyRot, nEnemyType, fEnemyAI_CheackDistance, fEnemyAI_AttackDistance, nEnemyAI_AttackTiming, nEnemyLife, nEnemyAddScore);
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �G��AI���ǂݍ��ݏ���
//=============================================================================
void CMap::EnemyLoad_Info_AI(CFileLoader *pFileLoader, char *pStrCur, float *pCheackDistance, float *pAttackDistance, int *pAttackTiming)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, CHEACKDISTANCE) == 0)
		{// �x���ł���͈͏�񂪂���
			*pCheackDistance = CFunctionLib::ReadFloat(pStrCur, CHEACKDISTANCE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKDISTANCE) == 0)
		{// �U�����d�|����͈͏�񂪂���
			*pAttackDistance = CFunctionLib::ReadFloat(pStrCur, ATTACKDISTANCE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKRANDOM) == 0)
		{// �U�����d�|����^�C�~���O��񂪂���
			*pAttackTiming = CFunctionLib::ReadInt(pStrCur, ATTACKRANDOM);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_AISET) == 0)
		{// AI�̏��ǂݍ��ݏI���̍��}������
			break;
		}
	}
}

//=============================================================================
//    �G�Ǌ��܂Ƃ߃N���X�֓G�Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CMap::SetEnemyManagerToAssembley(CEnemyManager **apEnemyManager, int nCntEnemyLoad)
{
	// �G�Ǌ��܂Ƃ߃N���X�փ|�C���^��ݒ肷��
	if (m_pEnemyAssembly == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pEnemyAssembly = new CEnemyAssembly;
		if (m_pEnemyAssembly != NULL)
		{// ���������m�ۂł���
			for (int nCntEnemy = 0; nCntEnemy < nCntEnemyLoad; nCntEnemy++)
			{// �G�̃f�[�^��ǂݍ��񂾉񐔕��J��Ԃ�
				m_pEnemyAssembly->SetEnemyManager(apEnemyManager[nCntEnemy], nCntEnemy);
			}
			// �ǂݍ��񂾉񐔂�ۑ�����
			m_nCntLoadEnemy = nCntEnemyLoad;
		}
	}
}

//=============================================================================
//    �z�u�p�r���{�[�h�ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::BillboardLoad_Open(char *pBillboardFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pBillboardFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			BillboardLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �z�u�p�r���{�[�h�ǂݍ��ݏ���
//=============================================================================
void CMap::BillboardLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, BILLBOARDSET) == 0)
		{// �z�u�p�r���{�[�h���ǂݍ��݊J�n�̍��}������
			BillboardLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �z�u�p�r���{�[�h���ǂݍ��ݏ���
//=============================================================================
void CMap::BillboardLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �z�u�p�r���{�[�h���i�[�p�ϐ�
	int nBillboardType = 0;
	int nBillboardTexIdx = 0;
	D3DXVECTOR3 BillboardPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR BillboardCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fBillboardColRange = 0.0f;
	float fBillboardRot = 0.0f;
	float fBillboardWidth = 0.0f;
	float fBillboardHeight = 0.0f;
	bool bBillboardLighting = false;
	bool bBillboardDrawAddtive = false;

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// ��ޔԍ���񂪂���
			nBillboardType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
		{// �g�p����e�N�X�`���ԍ���񂪂���
			nBillboardTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͏�񂪂���
			fBillboardColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂪂���
			BillboardPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
		{// �F��񂪂���
			BillboardCol = CFunctionLib::ReadVector4(pStrCur, COL);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂪂���
			float fRot = CFunctionLib::ReadFloat(pStrCur, ROT);
			fBillboardRot = D3DXToRadian(fRot);
		}
		else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
		{// ����񂪂���
			fBillboardWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
		{// ������񂪂���
			fBillboardHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, LIGHTING) == 0)
		{// ���C�e�B���O���邩���Ȃ�����񂪂���
			bBillboardLighting = CFunctionLib::ReadBool(pStrCur, LIGHTING);
		}
		else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
		{// ���Z�����ŕ`�悷�邩���Ȃ�����񂪂���
			bBillboardDrawAddtive = CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_BILLBOARDSET) == 0)
		{// �z�u�p�r���{�[�h���ǂݍ��ݏI���̍��}������
			CBillboardObject *pBillboardObj = CBillboardObject::Create(BillboardPos, BillboardCol, fBillboardWidth, fBillboardHeight, fBillboardRot, bBillboardLighting, bBillboardDrawAddtive, nBillboardTexIdx, fBillboardColRange, nBillboardType, BILLBOARD_PRIORITY);
			if (pBillboardObj != NULL)
			{// �����ł���
				pBillboardObj->BindTexture(m_pTextureManager->GetTexture(nBillboardTexIdx));
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �z�u�p�G�t�F�N�g�ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::EffectLoad_Open(char *pEffectFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pEffectFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			EffectLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �z�u�p�G�t�F�N�g�ǂݍ��ݏ���
//=============================================================================
void CMap::EffectLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾
	SearchEffectManager();
	if (m_pEffectManager == NULL) return;

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, EFFECTSET) == 0)
		{// �z�u�p�G�t�F�N�g���ǂݍ��݊J�n�̍��}������
			EffectLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
void CMap::SearchEffectManager(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_EFFECTMANAGER)
			{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������
				m_pEffectManager = (CEffectManager*)pScene;
				break;
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u�p�G�t�F�N�g���ǂݍ��ݏ���
//=============================================================================
void CMap::EffectLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	//�@�z�u�p�G�t�F�N�g���i�[�p�ϐ�
	int nEffectType = 0;
	D3DXVECTOR3 EffectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 EffectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// ��ޏ�񂪂���
			nEffectType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂪂���
			EffectPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂪂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			EffectRot.x = D3DXToRadian(Rot.x);
			EffectRot.y = D3DXToRadian(Rot.y);
			EffectRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_EFFECTSET) == 0)
		{// �z�u�G�t�F�N�g���ǂݍ��ݏI���̍��}������
			m_pEffectManager->SetEffect(EffectPos, EffectRot, nEffectType, EFFECT_PRIORITY);
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �Ǔǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::WallLoad_Open(char *pWallFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pWallFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			WallLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �Ǔǂݍ��ݏ���
//=============================================================================
void CMap::WallLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, WALLSET) == 0)
		{// �Ǐ��ǂݍ��݊J�n�̍��}������
			WallLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �Ǐ��ǂݍ��ݏ���
//=============================================================================
void CMap::WallLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �Ǐ��i�[�p�ϐ�
	int nWallTexIdx = 0;                                    // �ǂ̃e�N�X�`���ԍ�
	D3DXVECTOR3 WallPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ǂ̍��W
	D3DXVECTOR3 WallRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ǂ̌���
	D3DXCOLOR WallCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �ǂ̐F
	float fWallWidth = 0.0f;                                // �ǂ�1�u���b�N���̕�
	float fWallHeight = 0.0f;                               // �ǂ�1�u���b�N���̉��s
	int nWallXblock = 0;                                    // �ǂ̉��̃u���b�N��
	int nWallYblock = 0;                                    // �ǂ̏c�̃u���b�N��
	int nWallTexSplitU = 0;                                 // �ǂ̃e�N�X�`��U���W�̕�����
	int nWallTexSplitV = 0;                                 // �ǂ̃e�N�X�`��V���W�̕�����

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
		{// �g�p����e�N�X�`���̔ԍ���񂾂���
			nWallTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂾂���
			WallPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂾂���
			D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
			WallRot.x = D3DXToRadian(Rot.x);
			WallRot.y = D3DXToRadian(Rot.y);
			WallRot.z = D3DXToRadian(Rot.z);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
		{// �F��񂾂���
			WallCol = CFunctionLib::ReadVector4(pStrCur, COL);
		}
		else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
		{// 1�u���b�N���̕���񂾂���
			fWallWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
		{// 1�u���b�N���̍�����񂾂���
			fWallHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
		{// ���̕�����������
			nWallXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
		{// �c�̕�����������
			nWallYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
		{// �e�N�X�`��U���W�̕�����������
			nWallTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
		{// �e�N�X�`��V���W�̕�����������
			nWallTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_WALLSET) == 0)
		{// �Ǐ��ǂݍ��ݏI���̍��}������
		    // �ǂ𐶐�����
			CMeshWall *pMeshWall = CMeshWall::Create(WallPos, WallRot, WallCol, fWallWidth, fWallHeight, nWallXblock, nWallYblock, nWallTexSplitU, nWallTexSplitV, nWallTexIdx, WALL_PRIORITY);
			if (pMeshWall != NULL)
			{// �ǂ������ł���
				pMeshWall->BindTexture(m_pTextureManager->GetTexture(nWallTexIdx));
			}
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �A�C�e���ǂݍ��݂̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::ItemLoad_Open(char *pItemFileName, char *pStrCur)
{
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(pItemFileName);
	if (pFileLoader != NULL)
	{// �t�@�C���I�[�v���ɐ���
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
			ItemLoad(pFileLoader, pStrCur);
		}

		// �t�@�C���N���[�Y
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �A�C�e���ǂݍ��ݏ���
//=============================================================================
void CMap::ItemLoad(CFileLoader *pFileLoader, char *pStrCur)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, ITEMSET) == 0)
		{// �A�C�e�����ǂݍ��݊J�n�̍��}������
			ItemLoad_Info(pFileLoader, pStrCur);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �A�C�e�����ǂݍ��ݏ���
//=============================================================================
void CMap::ItemLoad_Info(CFileLoader *pFileLoader, char *pStrCur)
{
	// �A�C�e�����i�[�p�ϐ�
	int nItemTexIdx = 0;
	int nItemType = 0;
	float fItemColRange = 0.0f;
	D3DXVECTOR3 ItemPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR ItemCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fItemRot = 0.0f;
	float fItemWidth = 0.0f;
	float fItemHeight = 0.0f;
	bool bItemLighting = false;
	bool bItemDrawAddtive = false;

	while (1)
	{// ���[�v�J�n
		pStrCur = pFileLoader->GetString(pStrCur);
		if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
		{// �g�p����e�N�X�`���ԍ���񂪂���
			nItemTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// ��ޏ�񂪂���
			nItemType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͏�񂪂���
			fItemColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W��񂪂���
			ItemPos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
		{// �F��񂪂���
			ItemCol = CFunctionLib::ReadVector4(pStrCur, COL);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ������񂪂���
			float fRot = CFunctionLib::ReadFloat(pStrCur, ROT);
			fItemRot = D3DXToRadian(fRot);
		}
		else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
		{// ����񂪂���
			fItemWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
		{// ������񂪂���
			fItemHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, LIGHTING) == 0)
		{// ���C�e�B���O���邩���Ȃ�����񂪂���
			bItemLighting = CFunctionLib::ReadBool(pStrCur, LIGHTING);
		}
		else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
		{// ���Z�����ŕ`�悷�邩���Ȃ�����񂪂���
			bItemDrawAddtive = CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ITEMSET) == 0)
		{// �A�C�e�����ǂݍ��ݏI���̍��}������
			SetItem(ItemPos, ItemCol, fItemWidth, fItemHeight, fItemRot, bItemLighting, bItemDrawAddtive,
				nItemTexIdx, fItemColRange, nItemType);
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �e��z�u����ۑ����鏈��
//=============================================================================
void CMap::Save(void)
{
	// �z�u���̔z�u���
	SaveObject_Open();

	// �G�̔z�u���
	SaveEnemy_Open();

	// �r���{�[�h�̔z�u���
	SaveBillboard_Open();

	// �G�t�F�N�g�̔z�u���
	SaveEffect_Open();

	// �ǂ̔z�u���
	SaveWall_Open();

	// �A�C�e���̔z�u���
	SaveItem_Open();
}

//=============================================================================
//    �z�u����ۑ����邽�߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::SaveObject_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_OBJECT_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C���I�[�v���ɐ���
	    // �`����������������
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       �z�u�����X�N���v�g�t�@�C��[save_object.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

		// �z�u����ۑ�����
		SaveObject(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

		// �t�@�C���N���[�Y
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    �z�u���ۑ�����
//=============================================================================
void CMap::SaveObject(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  ���f�����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// �z�u���N���X�ւ̃|�C���^������
				SaveObject_Info(pFileSaver, (CObject*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
			{// �����z�u���N���X�ւ̃|�C���^������
				SaveObjectMove_Info(pFileSaver, (CObjectMove*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
			{// �S�[���p�z�u���N���X�ւ̃|�C���^������
				SaveObjectGoal_Info(pFileSaver, (CObjectGoal*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u�����ۑ�����
//=============================================================================
void CMap::SaveObject_Info(CFileSaver *pFileSaver, CObject *pObject)
{
	// ���O�Ɍv�Z�����܂��Ă���
	D3DXVECTOR3 ModelRot = pObject->GetRotStd();
	D3DXVECTOR3 ModelRollSpeed = pObject->GetRollSpeed();
	// ����
	ModelRot.x = D3DXToDegree(ModelRot.x);
	ModelRot.y = D3DXToDegree(ModelRot.y);
	ModelRot.z = D3DXToDegree(ModelRot.z);

	// ��]����X�s�[�h
	ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
	ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
	ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

	// �z�u���̏�����������
	pFileSaver->Print("%s\n", OBJECTSET);
	pFileSaver->Print("	%s%d						# �g�p���郂�f���̔ԍ�\n", TYPE, pObject->GetModelIdx());
	pFileSaver->Print("	%s%d					# �����蔻��̎��\n", COLLISION, pObject->GetCollision());
	pFileSaver->Print("	%s%.1f					# �����蔻������͈�\n", COL_RANGE, pObject->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f			# ���f���̍��W\n", POS, pObject->GetPos().x, pObject->GetPos().y, pObject->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ���f���̌���\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ���f���̑傫��\n", SCALE, pObject->GetScale().x, pObject->GetScale().y, pObject->GetScale().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���f���̉�]����X�s�[�h\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
	pFileSaver->Print("%s\n", END_OBJECTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �����z�u�����ۑ�����
//=============================================================================
void CMap::SaveObjectMove_Info(CFileSaver *pFileSaver, CObjectMove *pObjectMove)
{
	// ���O�Ɍv�Z�����܂��Ă���
	D3DXVECTOR3 ModelRot = pObjectMove->GetRotStd();
	D3DXVECTOR3 ModelRollSpeed = pObjectMove->GetRollSpeed();
	// ����
	ModelRot.x = D3DXToDegree(ModelRot.x);
	ModelRot.y = D3DXToDegree(ModelRot.y);
	ModelRot.z = D3DXToDegree(ModelRot.z);

	// ��]����X�s�[�h
	ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
	ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
	ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

	// �����z�u���̏�����������
	pFileSaver->Print("%s\n", OBJECTMOVESET);
	pFileSaver->Print("	%s%d						# �g�p���郂�f���̔ԍ�\n", TYPE, pObjectMove->GetModelIdx());
	pFileSaver->Print("	%s%d					# �����蔻��̎��\n", COLLISION, pObjectMove->GetCollision());
	pFileSaver->Print("	%s%.1f					# �����蔻������͈�\n", COL_RANGE, pObjectMove->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f			# ���f���̍��W\n", POS, pObjectMove->GetPosStd().x, pObjectMove->GetPosStd().y, pObjectMove->GetPosStd().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ���f���̌���\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ���f���̑傫��\n", SCALE, pObjectMove->GetScale().x, pObjectMove->GetScale().y, pObjectMove->GetScale().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���f���̉�]����X�s�[�h\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ���f���̈ړ���\n", MOVE, pObjectMove->GetMove().x, pObjectMove->GetMove().y, pObjectMove->GetMove().z);
	pFileSaver->Print("	%s%d					# �ړ��ʂ�؂�ւ��鎞��\n", MOVE_MAX, pObjectMove->GetMaxMoveCounter());
	pFileSaver->Print("%s\n", END_OBJECTMOVESET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �S�[���p�z�u�����ۑ�����
//=============================================================================
void CMap::SaveObjectGoal_Info(CFileSaver *pFileSaver, CObjectGoal *pObjectGoal)
{
	// ���O�Ɍv�Z�����܂��Ă���
	D3DXVECTOR3 ModelRot = pObjectGoal->GetRotStd();
	D3DXVECTOR3 ModelRollSpeed = pObjectGoal->GetRollSpeed();
	// ����
	ModelRot.x = D3DXToDegree(ModelRot.x);
	ModelRot.y = D3DXToDegree(ModelRot.y);
	ModelRot.z = D3DXToDegree(ModelRot.z);

	// ��]����X�s�[�h
	ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
	ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
	ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

	// �S�[���p�z�u���̏�����������
	pFileSaver->Print("%s\n", OBJECTGOALSET);
	pFileSaver->Print("	%s%d						# �g�p���郂�f���̔ԍ�\n", TYPE, pObjectGoal->GetModelIdx());
	pFileSaver->Print("	%s%d					# �����蔻��̎��\n", COLLISION, pObjectGoal->GetCollision());
	pFileSaver->Print("	%s%.1f					# �����蔻������͈�\n", COL_RANGE, pObjectGoal->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f			# ���f���̍��W\n", POS, pObjectGoal->GetPos().x, pObjectGoal->GetPos().y, pObjectGoal->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ���f���̌���\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# ���f���̑傫��\n", SCALE, pObjectGoal->GetScale().x, pObjectGoal->GetScale().y, pObjectGoal->GetScale().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���f���̉�]����X�s�[�h\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
	pFileSaver->Print("%s\n", END_OBJECTGOALSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �G���ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::SaveEnemy_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_ENEMY_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C���I�[�v���ɐ���
	    // �`����������������
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       �G�̔z�u���X�N���v�g�t�@�C��[save_set_enemy.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

		// �G�Ǌ����ۑ�����
		SaveEnemy_Manager(pFileSaver);

		// �G�ۑ�����
		SaveEnemy(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

		// �t�@�C���N���[�Y
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    �G�Ǌ����ۑ�����
//=============================================================================
void CMap::SaveEnemy_Manager(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �g�p����G�̃f�[�^\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_TYPE; nCntEnemy++)
	{// �G�̃f�[�^��ǂݍ��߂�񐔕��J��Ԃ�
		if (m_pEnemyAssembly != NULL)
		{
			CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nCntEnemy);
			if (pEnemyManager != NULL)
			{
				pFileSaver->Print("%s%s		# �G%d�̖�\n", ENEMYDATA_FILENAME, pEnemyManager->GetFileName(), nCntEnemy + 1);
			}
		}
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    �G�ۑ�����
//=============================================================================
void CMap::SaveEnemy(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �G�̔z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �G�̔z�u����ۑ�
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G�N���X�ւ̃|�C���^������
				SaveEnemy_Info(pFileSaver, (CEnemy*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �G���ۑ�����
//=============================================================================
void CMap::SaveEnemy_Info(CFileSaver *pFileSaver, CEnemy *pEnemy)
{
	// ���O�Ɍv�Z�����܂��Ă���
	D3DXVECTOR3 EnemyRot = pEnemy->GetRotStd();
	// ����
	EnemyRot.x = D3DXToDegree(EnemyRot.x);
	EnemyRot.y = D3DXToDegree(EnemyRot.y);
	EnemyRot.z = D3DXToDegree(EnemyRot.z);

	// �G�̔z�u������������
	pFileSaver->Print("%s\n", ENEMYSET);
	pFileSaver->Print("	%s						# AI�̏��\n", AISET);
	pFileSaver->Print("		%s%f	# �x���ł���͈�", CHEACKDISTANCE, pEnemy->GetAI_CheackDistance());
	pFileSaver->Print("		%s%f	# �U�����d�|����͈�", ATTACKDISTANCE, pEnemy->GetAI_AttackDistance());
	pFileSaver->Print("		%s%d		# �U�����d�|����^�C�~���O", ATTACKRANDOM, pEnemy->GetAI_AttackTiming());
	pFileSaver->Print("	%s\n\n", END_AISET);
	pFileSaver->Print("	%s%d					# ���\n", TYPE, pEnemy->GetType());
	pFileSaver->Print("	%s%d					# �̗�\n", LIFE, pEnemy->GetLife());
	pFileSaver->Print("	%s%d				# �|�������ɑ�����X�R�A��\n", SCORE, pEnemy->GetAddScore());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, pEnemy->GetPosStd().x, pEnemy->GetPosStd().y, pEnemy->GetPosStd().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f			# ����\n", ROT, EnemyRot.x, EnemyRot.y, EnemyRot.z);
	pFileSaver->Print("%s\n", END_ENEMYSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u�p�r���{�[�h���ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::SaveBillboard_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_BILLBOARD_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C���I�[�v���ɐ���
	    // �`����������������
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       �z�u�p�r���{�[�h���X�N���v�g�t�@�C��[save_billboard.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

		// �z�u�p�r���{�[�h��ۑ�����
		SaveBillboard(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

		// �t�@�C���N���[�Y
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    �z�u�p�r���{�[�h�ۑ�����
//=============================================================================
void CMap::SaveBillboard(CFileSaver *pFileSaver)
{
	// �r���{�[�h����ۑ�
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBillboardObject *pBillboardObject = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BILLBOARDOBJ)
			{// �z�u�p�r���{�[�h�N���X�ւ̃|�C���^������
				SaveBillboard_Info(pFileSaver, (CBillboardObject*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u�p�r���{�[�h���ۑ�����
//=============================================================================
void CMap::SaveBillboard_Info(CFileSaver *pFileSaver, CBillboardObject *pBillObj)
{
	// ���O�Ɍv�Z�����܂��Ă���
	float fBillboardRot = pBillObj->GetRot();
	// ����
	fBillboardRot = D3DXToDegree(fBillboardRot);

	// �z�u���̏�����������
	pFileSaver->Print("%s\n", BILLBOARDSET);
	pFileSaver->Print("	%s%d				# ��ޔԍ�\n", TYPE, pBillObj->GetType());
	pFileSaver->Print("	%s%d				# �g�p����e�N�X�`���̔ԍ�\n", TEX_TYPE, pBillObj->GetTexIdx());
	pFileSaver->Print("	%s%.1f				# �����蔻������͈�\n", COL_RANGE, pBillObj->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, pBillObj->GetPos().x, pBillObj->GetPos().y, pBillObj->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# �F\n", COL, pBillObj->GetCol().r, pBillObj->GetCol().g, pBillObj->GetCol().b, pBillObj->GetCol().a);
	pFileSaver->Print("	%s%.1f					# ����\n", ROT, fBillboardRot);
	pFileSaver->Print("	%s%.1f				# ��\n", WIDTH, pBillObj->GetWidth());
	pFileSaver->Print("	%s%.1f				# ����\n", HEIGHT, pBillObj->GetHeight());
	pFileSaver->Print("	%s%d				# ���C�e�B���O���邩���Ȃ���\n", LIGHTING, pBillObj->GetLighting());
	pFileSaver->Print("	%s%d			# ���Z�����ŕ`�悷�邩���Ȃ���\n", DRAW_ADDTIVE, pBillObj->GetDrawAddtive());
	pFileSaver->Print("%s\n", END_BILLBOARDSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u�p�G�t�F�N�g���ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::SaveEffect_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_EFFECT_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C���I�[�v���ɐ���
	    // �`����������������
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       �z�u�G�t�F�N�g���X�N���v�g�t�@�C��[save_effect.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

		// �z�u�p�G�t�F�N�g��ۑ�����
		SaveEffect(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

		// �t�@�C���N���[�Y
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    �z�u�p�G�t�F�N�g�ۑ�����
//=============================================================================
void CMap::SaveEffect(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �z�u�G�t�F�N�g���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
			{// �G�~�b�^�N���X�ւ̃|�C���^������
				SaveEffect_Info(pFileSaver, (CEmitter*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u�p�G�t�F�N�g���ۑ�����
//=============================================================================
void CMap::SaveEffect_Info(CFileSaver *pFileSaver, CEmitter *pEmitter)
{
	// ���O�Ɍv�Z�����܂��Ă���
	D3DXVECTOR3 EffectRot = pEmitter->GetRot();
	// ����
	EffectRot.x = D3DXToDegree(EffectRot.x);
	EffectRot.y = D3DXToDegree(EffectRot.y);
	EffectRot.z = D3DXToDegree(EffectRot.z);

	// �z�u�G�t�F�N�g�̏�����������
	pFileSaver->Print("%s\n", EFFECTSET);
	pFileSaver->Print("	%s%d				# �g�p����G�t�F�N�g�̎�ޔԍ�\n", TYPE, pEmitter->GetType());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, pEmitter->GetPos().x, pEmitter->GetPos().y, pEmitter->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ����\n", ROT, EffectRot.x, EffectRot.y, EffectRot.z);
	pFileSaver->Print("%s\n", END_EFFECTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �Ǐ��ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::SaveWall_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_WALL_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C���I�[�v���ɐ���
	    // �`����������������
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       �Ǐ��X�N���v�g�t�@�C��[save_wall.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

		// �ǂ�ۑ�����
		SaveWall(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

		// �t�@�C���N���[�Y
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    �Ǖۑ�����
//=============================================================================
void CMap::SaveWall(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �Ǐ��\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_MESHWALL)
			{// �ǃN���X�ւ̃|�C���^������
				SaveWall_Info(pFileSaver, (CMeshWall*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �Ǐ��ۑ�����
//=============================================================================
void CMap::SaveWall_Info(CFileSaver *pFileSaver, CMeshWall *pMeshWall)
{
	// ���O�Ɍv�Z�����܂��Ă���
	D3DXVECTOR3 WallRot = pMeshWall->GetRot();
	// ����
	WallRot.x = D3DXToDegree(WallRot.x);
	WallRot.y = D3DXToDegree(WallRot.y);
	WallRot.z = D3DXToDegree(WallRot.z);

	// �ǂ̏�����������
	pFileSaver->Print("%s\n", WALLSET);
	pFileSaver->Print("	%s%d				# �g�p����e�N�X�`���̔ԍ�\n", TEX_TYPE, pMeshWall->GetTexIdx());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, pMeshWall->GetPos().x, pMeshWall->GetPos().y, pMeshWall->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f			# ����\n", ROT, WallRot.x, WallRot.y, WallRot.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# �F\n", COL, pMeshWall->GetCol().r, pMeshWall->GetCol().g, pMeshWall->GetCol().b, pMeshWall->GetCol().a);
	pFileSaver->Print("	%s%.1f				# 1�u���b�N���̕�\n", WIDTH, pMeshWall->GetWidth());
	pFileSaver->Print("	%s%.1f				# 1�u���b�N���̍���\n", HEIGHT, pMeshWall->GetHeight());
	pFileSaver->Print("	%s%d					# ���̕�����\n", XBLOCK, pMeshWall->GetXBlock());
	pFileSaver->Print("	%s%d					# �c�̕�����\n", YBLOCK, pMeshWall->GetYBlock());
	pFileSaver->Print("	%s%d				# �e�N�X�`��U���W�̕�����\n", TEXSPLIT_U, pMeshWall->GetTexSplitU());
	pFileSaver->Print("	%s%d				# �e�N�X�`��V���W�̕�����\n", TEXSPLIT_V, pMeshWall->GetTexSplitV());
	pFileSaver->Print("%s\n", END_WALLSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �A�C�e�����ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
void CMap::SaveItem_Open(void)
{
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(MAP_WALL_SAVEFILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C���I�[�v���ɐ���
	    // �`����������������
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#       �A�C�e�����X�N���v�g�t�@�C��[save_item.txt]\n");
		pFileSaver->Print("#       Auther : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#******************************************************************************\n");
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

		// �A�C�e����ۑ�����
		SaveItem(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

		// �t�@�C���N���[�Y
		delete pFileSaver;
		pFileSaver = NULL;
	}
}

//=============================================================================
//    �A�C�e���ۑ�����
//=============================================================================
void CMap::SaveItem(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �A�C�e�����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM)
			{// �A�C�e���N���X�ւ̃|�C���^������
				SaveItem_Info(pFileSaver, (CItem*)pScene);
			}
			pScene = pSceneNext;
		}
	}

}

//=============================================================================
//    �A�C�e�����ۑ�����
//=============================================================================
void CMap::SaveItem_Info(CFileSaver *pFileSaver, CItem *pItem)
{
	// ���O�Ɍv�Z�����܂��Ă���
	float fItemRot = pItem->GetRot();
	// ����
	fItemRot = D3DXToDegree(fItemRot);

	// �A�C�e���̏�����������
	pFileSaver->Print("%s\n", ITEMSET);
	pFileSaver->Print("	%s%d				# �g�p����e�N�X�`���̔ԍ�\n", TEX_TYPE, pItem->GetTexIdx());
	pFileSaver->Print("	%s%d					# ���\n", TYPE, pItem->GetType());
	pFileSaver->Print("	%s%.1f				# �����蔻������͈�\n", COL_RANGE, pItem->GetColRange());
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, pItem->GetPos().x, pItem->GetPos().y, pItem->GetPos().z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# �F\n", COL, pItem->GetCol().r, pItem->GetCol().g, pItem->GetCol().b, pItem->GetCol().a);
	pFileSaver->Print("	%s%.1f					# ����\n", ROT, fItemRot);
	pFileSaver->Print("	%s%.1f				# ��\n", WIDTH, pItem->GetWidth());
	pFileSaver->Print("	%s%.1f				# ����\n", HEIGHT, pItem->GetHeight());
	pFileSaver->Print("	%s%d				# ���C�e�B���O���邩���Ȃ���\n", LIGHTING, pItem->GetLighting());
	pFileSaver->Print("	%s%d			# ���Z�����ŕ`�悷�邩���Ȃ���\n", DRAW_ADDTIVE, pItem->GetDrawAddtive());
	pFileSaver->Print("%s\n", END_ITEMSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u����z�u���鏈��
//=============================================================================
void CMap::SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 rollSpeed, int nModelIdx)
{
	// �z�u���𐶐�����
	CObject *pObject = CObject::Create(pos, rot, scale, rollSpeed);
	if (pObject != NULL)
	{
		pObject->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObject->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    �����z�u����z�u���鏈��
//=============================================================================
void CMap::SetObjectMove(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 rollSpeed, int nModelIdx, D3DXVECTOR3 move, int nMaxMoveCounter)
{
	// �����z�u���𐶐�����
	CObjectMove *pObjectMove = CObjectMove::Create(pos, rot, scale, rollSpeed, move, nMaxMoveCounter);
	if (pObjectMove != NULL)
	{
		pObjectMove->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObjectMove->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    �r���{�[�h��z�u���鏈��
//=============================================================================
void CMap::SetBillboard(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	CBillboardObject *pBillObj = CBillboardObject::Create(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType);
	if (pBillObj != NULL)
	{
		pBillObj->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    �G�t�F�N�g��z�u���鏈��
//=============================================================================
void CMap::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->SetEffect(pos, rot, nType);
	}
}

//=============================================================================
//    �G��z�u���鏈��
//=============================================================================
void CMap::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nScore)
{
	if (m_pEnemyAssembly != NULL)
	{
		CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nType);
		if (pEnemyManager != NULL)
		{
			pEnemyManager->SetEnemy(pos, rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nScore);
		}
	}
}

//=============================================================================
//    �ǂ�z�u���鏈��
//=============================================================================
void CMap::SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx)
{
	// �ǂ𐶐�����
	CMeshWall *pMeshWall = CMeshWall::Create(pos, rot, col, fWidth, fHeight, nXBlock, nYBlock, nTexSplitU, nTexSplitV, nTexIdx);
	if (pMeshWall != NULL)
	{// �ǂ������ł���
		pMeshWall->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    �A�C�e����z�u���鏈��
//=============================================================================
void CMap::SetItem(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nType)
{
	CItem *pItem = CItem::Create(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange, nType, BILLBOARD_PRIORITY);
	if (pItem != NULL)
	{
		pItem->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    ���f���Ǌ��N���X��j�����鏈��
//=============================================================================
void CMap::ReleaseModelManager(void)
{
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();
		delete m_pModelManager;
		m_pModelManager = NULL;
	}
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X��j�����鏈��
//=============================================================================
void CMap::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    �G�Ǌ��N���X��j�����鏈��
//=============================================================================
void CMap::ReleaseEnemyAssembley(void)
{
	if (m_pEnemyAssembly != NULL)
	{
		delete m_pEnemyAssembly;
		m_pEnemyAssembly = NULL;
	}
}

//=============================================================================
//    �}�b�v�̃I�u�W�F�N�g��j�����鏈��
//=============================================================================
void CMap::DeleteMapObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT || pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE || pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL
				|| pScene->GetObjType() == CScene::OBJTYPE_BILLBOARDOBJ || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER
				|| pScene->GetObjType() == CScene::OBJTYPE_ENEMYMANAGER || pScene->GetObjType() == CScene::OBJTYPE_ENEMY || pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD
				|| pScene->GetObjType() == CScene::OBJTYPE_MOUNTAIN || pScene->GetObjType() == CScene::OBJTYPE_SKY || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE
				|| pScene->GetObjType() == CScene::OBJTYPE_RINGEFFECT || pScene->GetObjType() == CScene::OBJTYPE_MESHWALL || pScene->GetObjType() == CScene::OBJTYPE_RIVER
				|| pScene->GetObjType() == CScene::OBJTYPE_ITEM || pScene->GetObjType() == CScene::OBJTYPE_GRASS || pScene->GetObjType() == CScene::OBJTYPE_BOXCOLLIDER)
			{// �}�b�v�̗v�f���`������N���X�ւ̃|�C���^������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}
	CScene::DeathCheck();
}

//=============================================================================
//    �ǂݍ��񂾃}�b�v�̃t�@�C�������擾���鏈��
//=============================================================================
char *CMap::GetMapName(void)
{
	return m_aFileName;
}

//=============================================================================
//    �G�f�[�^��ǂݍ��񂾉񐔂��擾���鏈��
//=============================================================================
int CMap::GetCntEnemyLoad(void)
{
	return m_nCntLoadEnemy;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CMap::GetModelManager(void)
{
	return m_pModelManager;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CTextureManager *CMap::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
D3DXVECTOR3 CMap::GetPlayerDefaultPos(void)
{
	return m_PlayerDefaultPos;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
D3DXVECTOR3 CMap::GetPlayerDefaultRot(void)
{
	return m_PlayerDefaultRot;
}