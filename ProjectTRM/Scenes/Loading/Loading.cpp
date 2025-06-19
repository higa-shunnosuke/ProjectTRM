#include "Loading.h"

// �R���X�g���N�^
Loading::Loading():
	count(),
	size(),
	is_complete(false),
	next_scene(nullptr)
{

}

// �f�X�g���N�^
Loading::~Loading()
{

}

// ����������
void Loading::Initialize()
{
	count = 0;
	size = load_list.size();
	is_complete = false;
}

// �X�V����
eSceneType Loading::Update(const float& delta_second)
{
	// 1�t���[�����Ƃ�1�����[�h
	if (count < size)
	{
		// ���\�[�X�}�l�[�W���[�̃|�C���^
		ResourceManager* rm = ResourceManager::GetInstance();
		// �ǂݍ��ރt�@�C��
		LoadItem item = load_list[count];

		// �摜�̓ǂݍ���
		if (item.type == "image")
		{
			// �t�@�C���p�X
			std::string Path = "Resource/Images/" + item.path + ".png";

			if (!rm->GetImages(Path.c_str())[0])
			{
				// �G���[����
				MessageBoxA(NULL, "�摜�̓Ǎ��݂Ɏ��s���܂���", "�G���[", MB_OK);
			}
			
		}
		// �����̓ǂݍ���
		else if (item.type == "sound")
		{
			// �t�@�C���p�X
			std::string Path = "Resource/Sounds/" + item.path + ".mp3";

			if (!rm->GetSounds(Path.c_str()))
			{
				// �G���[����
				MessageBoxA(NULL, "�����̓Ǎ��݂Ɏ��s���܂���", "�G���[", MB_OK);
			}
		}

		count++;
	}
	else if (!is_complete)
	{
		// ���[�h����
		is_complete = true;

		// �^�C�g���V�[���֑J��
		return eSceneType::title;
	}

	// ���݂̃V�[������ԋp����
	return GetNowSceneType();
}

// �`�揈��
void Loading::Draw() const
{
	// UI�`��
	DrawString(300, 200, "Now Loading...", GetColor(255, 255, 255));

	// �Q�[�W�o�[�̕`��
	float rate = (float)count / size;
	int barWidth = 400;
	int barHeight = 20;
	int x = 200;
	int y = 300;
	DrawBox(x, y, x + barWidth, y + barHeight, GetColor(100, 100, 100), TRUE);         // �w�i
	DrawBox(x, y, x + (int)(barWidth * rate), y + barHeight, GetColor(0, 255, 0), TRUE); // �΂̐i��
	DrawBox(x, y, x + barWidth, y + barHeight, GetColor(255, 255, 255), FALSE);         // �g��
}

// �I������
void Loading::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType Loading::GetNowSceneType() const
{
	return eSceneType::loading;
}

// ���̃V�[���^�C�v�ݒ菈��
void Loading::SetNextSceneType(SceneBase* next_scene)
{
	this->next_scene = next_scene;
}