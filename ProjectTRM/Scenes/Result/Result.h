#pragma once

#include "../SceneBase.h"
#include <chrono>

class Result : public SceneBase
{
private:
	int count = 10;
	int cursor = -1;
	bool show = false;
	std::chrono::steady_clock::time_point	prev_time;				// ���Z�p�ϐ�
	std::chrono::steady_clock::time_point	select_time;			// �I��p�ϐ�
	std::chrono::steady_clock::time_point	choice_time = std::chrono::steady_clock::now();			// �I��p�ϐ�
	int bgmHandle[4];				// BGM
	int CursorMoveSE;
	int DecisionSE;
	bool isDecision = false;
	eSceneType nextScene=eSceneType::result;
	int decisionWaitFrame = 0;
	int CountdownSE;
public:
	// �R���X�g���N�^
	Result();
	// �f�X�g���N�^
	virtual ~Result();

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

