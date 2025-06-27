#pragma once

#include "..//SceneBase.h"
#include <chrono>

// �^�C�g���V�[��
class End : public SceneBase
{
private:
	int background;
	int BGM;
	int y = 700;
	int time = 0;
	std::vector<std::string> creditText;
	int titleImage;
	int imageWidth;
	int imageHeight;
public:
	// �R���X�g���N�^
	End();

	int MovieGraphHandle;
	int SoundHandle;
	// �f�X�g���N�^
	virtual ~End();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	/// <returns>���̃V�[���^�C�v���</returns>
	virtual eSceneType Update(const float& delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() const override;

	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ���݂̃V�[���^�C�v�����擾����
	/// </summary>
	/// <returns>���݂̃V�[���^�C�v</returns>
	virtual const eSceneType GetNowSceneType() const override;
};

