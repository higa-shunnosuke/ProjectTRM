#include "Title.h"

// �R���X�g���N�^
Title::Title()
{

}

// �f�X�g���N�^
Title::~Title()
{

}

// ����������
void Title::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	// �摜�̓ǂݍ���
	LoadImages();
	
}

// �X�V����
eSceneType Title::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	// �C���Q�[���V�[���ɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_RETURN)==eInputState::Pressed)
	{
		return eSceneType::stage_select;
	}
	if (input->GetButtonState(XINPUT_BUTTON_START)== eInputState::Pressed)
	{
		return eSceneType::stage_select;
	}
	if (input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed)
	{
		return eSceneType::end;
	}


	Anim_flame += 1.0f;
	if (Anim_flame == 60.0f)
	{
		Anim_count ++;
		Anim_flame = 0.0f;
	}
	if (Anim_count == 4)
	{
		Anim_count = 0;
		Anim_flame = 0.0f;
	}


	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void Title::Draw() const
{
	SetFontSize(120);
	DrawFormatString(500, 150, 0xff5500, "Project\n -TRM-");
	SetFontSize(60);

	if (Anim_count <= 1)
	{
	DrawFormatString(500, 450, 0xff5500, "Press_AnyButton");
	}

#ifdef _DEBUG
	SetFontSize(60);
	DrawFormatString(100, 140, 0xffffff, "Title");
	SetFontSize(32);
	DrawFormatString(100, 300, 0xffffff, "Enter�������Ă�!");
#endif

	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

}

// �I������
void Title::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType Title::GetNowSceneType() const
{
	return eSceneType::title;
}

const void Title::LoadImages() const
{
	//���\�[�X�}�l�[�W���[�̃C���X�^���X�擾
	ResourceManager* rm = ResourceManager::GetInstance();

	//�摜�Ǎ���
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Tank/Tank_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Ranged/Ranged_Walk.png", 4, 4, 1, 32, 32);
	rm->GetImages("Resource/Images/Unit/Melee/Melee_Attack.png", 4, 4, 1, 32, 32);
}