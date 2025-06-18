#include "End.h"
#include "../../Application.h"


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
	
	// ���[�r�[�t�@�C�������[�h���܂��B
	MovieGraphHandle = LoadGraph("Resource/Movie/EndMov.mp4");
	SoundHandle = LoadSoundMem("Resource/Sounds/Ending/ED.mp3");

	// ���[�r�[�̍Đ��ʒu��0�b�ڂɕύX���܂�
	SeekMovieToGraph(MovieGraphHandle, 0000);

	ChangeVolumeSoundMem(255 / 2, SoundHandle);
}

// �X�V����
eSceneType End::Update(const float& delta_second)
{
	// ���͏����擾
	InputManager* input = InputManager::GetInstance();

	PlaySoundMem(SoundHandle, DX_PLAYTYPE_LOOP);


	// ���[�r�[���Đ���Ԃɂ��܂�
	PlayMovieToGraph(MovieGraphHandle);

	if (GetMovieStateToGraph(MovieGraphHandle) == 1)
	{
		//�Q�[�����I��������
		Application* app = Application::GetInstance();
		app->QuitGame();
	}

	// �e�N���X�̍X�V�������Ăяo��
	return __super::Update(delta_second);
}

// �`�揈��
void End::Draw() const
{

	// ���[�v�AGetMovieStateToGraph �֐��̓��[�r�[�̍Đ���Ԃ𓾂�֐��ł�
   // �߂�l���P�̊Ԃ͍Đ���Ԃł��̂Ń��[�v�𑱂��܂�
	while (ProcessMessage() == 0 && GetMovieStateToGraph(MovieGraphHandle) == 1)
	{
		ClearDrawScreen();

		// ���[�r�[�f������ʂ����ς��ɕ`�悵�܂�
		DrawExtendGraph(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, MovieGraphHandle, FALSE);

		// �E�G�C�g�������܂��A���܂葬���`�悷��Ɖ�ʂ����������ł�
		WaitTimer(17);

		ScreenFlip();
	}

	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw();

}

// �I������
void End::Finalize()
{
	// �e�N���X�̏I�����������Ăяo��
	__super::Finalize();
}

// ���݂̃V�[���^�C�v�擾����
const eSceneType End::GetNowSceneType() const
{
	return eSceneType::end;
}
