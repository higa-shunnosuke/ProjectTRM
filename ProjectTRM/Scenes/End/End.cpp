#include "End.h"
#include "../../Application.h"
#include "../../Utility/Input/InputManager.h"

// �R���X�g���N�^
End::End()
{

}

// �f�X�g���N�^
End::~End()
{

}

// ����������
void End::Initialize()
{
	// �e�N���X�̏������������Ăяo��
	__super::Initialize();

	// �摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	rm->UnLoadImages();
	rm->UnLoadSounds();

	y = 800;
	time = 0;

	background = LoadGraph("Resource/Images/BackGround/StageSelect.png");
	BGM = rm->GetSounds("Resource/Sounds/Ending/ED3.mp3");

	ChangeVolumeSoundMem(300 / 2, BGM);

	creditText = {
	"Torchguard",
	"Shadows Fall",
	"",
	"",
	"",
	"",
	"",
		"�g�p�f��",
	"",
	"BGM",
	"Audiostock",
	"",
	"���ʉ�",
	"Audiostock",
	"���ʉ����{",
	"",
	"���j�b�g",
	"Bringer Of Death",
	"Hero and Opponents Animation",
	"PIXEL ART ASSETS",
	"itch.io",
	"Quixote",
	"Cave Explorer",
	"Werewolf Sprite Pixel Characters",
	"EVil Wizard 2",
	"Pixel Wizard 2D Art",
	"",
	"�G�t�F�N�g",
	"�҂ۂ�",
	"Pixel Art VFX",
	"",
	"�v���O����",
	"��� �x�V��",
	"�m�O �q�P",
	"�呺 �ʓl",
	"����u �D��",
	"",
	"Special Thanks",
	"�v���C���[�̊F�l",
	"",
	"",
	"",
	"THANK YOU FOR PLAYING"
	};

	if (PlaySoundMem(BGM, DX_PLAYTYPE_LOOP) == -1)
	{
		MessageBoxA(NULL, "BGM1�̍Đ��Ɏ��s���܂���", "�G���[", MB_OK);
	}

}

// �X�V����
eSceneType End::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	if (input->GetButtonState(XINPUT_BUTTON_START) == eInputState::Released)
	{
		DeleteSoundMem(SoundHandle);
		return eSceneType::title;
	}

	time++;

	const int scrollStartTime = 300;

	if (time > scrollStartTime)
	{
		// A�{�^�������Ȃ���ŉ�������d�l
		if (input->GetButtonState(XINPUT_BUTTON_A) == eInputState::Hold)
		{
			y--;
			time++;
		}

		//�ʏ�X�N���[��(3�t���[�����ƂɂP�s�N�Z��)
		if (time % 3 == 0)
		{
			y--;
		}
	}

	time++;

	const int lineHeight = 80;
	const int totalScrollLength = static_cast<int>(creditText.size()) * 50 + 720;	//lineHeight=50

	if (y < -((int)creditText.size() * lineHeight + 200))
	{
		Application* app = Application::GetInstance();
		app->QuitGame();
	}

	return GetNowSceneType();
}

// �`�揈��
void End::Draw() const
{
	DrawExtendGraph((int)(0), (int)(0), (int)(1280), (int)(720), background, TRUE);

	const int startY = y; //���݂̃X�N���[���I�t�Z�b�g
	const int lineHeight = 80;
	const int centerX = 600; //��ʒ��SX�i�K�x�����j

	SetFontThickness(6);

	////�t�F�[�Y�̐���
	//if (time < 300) // �b���x�A�^�C�g���\���̂�
	//{
	//	SetFontSize(80);
	//	const char* titleLine1 = "Torchguard:";
	//	const char* titleLine2 = "Shadows Fall";
	//	// ������X�ʒu�i��ʕ�640px��z��j
	//	int centerX = 640 / 2;
	//	// Y�ʒu�����i�c�ɕ��ׂĒ����t�߂Ɂj
	//	int lineSpacing = 20;
	//	int fontHeight = 80; // �t�H���g�T�C�Y�Ɠ���
	//	int totalHeight = fontHeight * 2 + lineSpacing;
	//	int startY = 300 - totalHeight / 2;
	//	// 1�s��
	//	DrawString(
	//		centerX - GetDrawStringWidth(titleLine1, strlen(titleLine1)) / 2,
	//		startY,
	//		titleLine1,
	//		GetColor(255, 255, 255));
	//	// 2�s��
	//	DrawString(
	//		centerX - GetDrawStringWidth(titleLine2, strlen(titleLine2)) / 2,
	//		startY + fontHeight + lineSpacing,
	//		titleLine2,
	//		GetColor(255, 255, 255));
	//	return;
	//}

	//�X�N���[���N���W�b�g
	SetFontSize(64);

	for (size_t i = 0; i < creditText.size(); ++i)
	{
		int drawY = y + static_cast<int>(i * lineHeight);
		const std::string& text = creditText[i];

		//�f�t�H���g�T�C�Y�ƐF
		int fontsize = 64;
		int color = GetColor(255, 255, 255);

		//����̍s�����T�C�Y��傫������
		if (text == "Torchguard" || text == "Shadows Fall")
		{
			fontsize = 100;
		}

		if (text == "THANK YOU FOR PLAYING")
		{
			fontsize = 100;
		}
		//�t�H���g�ݒ�
		SetFontSize(fontsize);
		SetFontThickness(6);

		//�\���̈���Ȃ�`��
		if (drawY > -lineHeight && drawY < 720 + lineHeight)
		{
			DrawString(centerX - GetDrawStringWidth(text.c_str(), text.length()) / 2,
				drawY,
				text.c_str(),
				color);
		}

		//�Ō�̍s�iTHANK YOU�j�������ʂ�
		//bool isLastLine = (i == creditText.size() - 1);
		//if (isLastLine && y < -((int)creditText.size() - 5) * lineHeight)
		//{
		//	//�Ō�̍s����ɒ����璆���ɐÎ~�\��
		//	SetFontSize(64);
		//	DrawString(centerX - GetDrawStringWidth(creditText[i].c_str(), creditText[i].size()) / 2,
		//		300,
		//		creditText[i].c_str(),
		//		GetColor(255, 255, 255));
		//}
		//else
		//{
		//	int drawY = startY + static_cast<int>(i * lineHeight);
		//	if (drawY > -lineHeight && drawY < 720 + lineHeight)
		//	{
		//		DrawString(centerX - GetDrawStringWidth(creditText[i].c_str(), creditText[i].size()) / 2,
		//			drawY,
		//			creditText[i].c_str(),
		//			GetColor(255, 255, 255));
		//	}
		//}
	}
	__super::Draw();
}

// �I������
void End::Finalize()
{
	StopSoundMem(BGM);

	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType End::GetNowSceneType() const
{
	return eSceneType::end;
}