#include "InGame.h"
#include "DxLib.h"
#include <fstream>

#include "../../Utility/StageData.h"
#include "../../Utility/Camera/Camera.h"

#include "../../Objects/Block/Ground.h"

#define PLAYER_INITIAL_LOCATION 3 // �v���C���[�����ʒu(���u���b�N�ځj

// �R���X�g���N�^
InGame::InGame()
{
	
}

// �f�X�g���N�^
InGame::~InGame()
{
	// ����Y��h�~
	Finalize();
}

// ����������
void InGame::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	//�J�����̏����擾
	Camera* camera = Camera::GetInstance();
	//�J�������W�̏�����
	camera->SetCameraPos(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2));

	//�X�e�[�W�ǂݍ���
	LoadStage();

}

// �X�V����
eSceneType InGame::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	//�J�����̏����擾
	Camera* camera = Camera::GetInstance();

	//�J�������̍X�V
	camera->Update();

	// ���U���g�V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Pressed)
	{
		return eSceneType::result;
	}
	
	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void InGame::Draw() const
{
	SetFontSize(60);
	DrawFormatString(120, 140, 0xffffff, "InGame");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enter�������Ă�������");

#if _DEBUG
	Camera* camera = Camera::GetInstance();

	DrawFormatString(0, 100, 0xffffff, "�J����X���W�F%f", camera->GetCameraPos().x);
#endif

	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();
}

// �I������
void InGame::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�����擾����
const eSceneType InGame::GetNowSceneType() const
{
	return eSceneType::in_game;
}

// �X�e�[�W��������
void InGame::LoadStage()
{
	// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
	GameObjectManager* object = GameObjectManager::GetInstance();
	
	FILE* fp = NULL;
	std::string file_name = "Resource/Map/Stage1.csv";

	// �w�肳�ꂽ�t�@�C�����J��
	errno_t result = fopen_s(&fp, file_name.c_str(), "r");

	// �G���[�`�F�b�N
	if (result != 0)
	{
		throw (file_name + "���J���܂���");
	}

	// �J�E���g�p�ϐ�
	int x = 0;			// ��
	int y = 0;			// �s

	// �t�@�C�����̕������m�F���Ă���
	while (true)
	{
		//���W�v�Z
		Vector2D location;
		location.x = (float)(x * BOX_SIZE + BOX_SIZE / 2 - D_WIN_MAX_X);
		location.y = (float)(y * BOX_SIZE + BOX_SIZE / 2);

		// �t�@�C������1�������o
		int c = fgetc(fp);

		// EOF�Ȃ烋�[�v�I��
		if (c == EOF)
		{
			break;
		}
		// ���s�����Ȃ�A���̍s��
		else if (c == '\n')
		{
			x = 0;
			y++;
		}
		// 0�Ȃ�A���̕�����
		else if (c == '0')
		{
			x++;
		}
		// G�Ȃ�A�n�ʂ𐶐�
		else if (c == 'G')
		{
			object->CreateObject<Ground>(Vector2D(location.x, location.y));
			x++;
		}
	}
	// �J�����t�@�C�������
	fclose(fp);
}

//	���j�b�g�Ǐ���
void UnitSelection()
{

}