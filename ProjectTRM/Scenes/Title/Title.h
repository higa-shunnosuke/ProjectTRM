#pragma once

#include "..//SceneBase.h"

enum TitleState :int
{
	SELECT_WAIT,
	SELECTED,
	USEING,
	FINAL
};

#define MAX_SELECT 1

// �^�C�g���V�[��
class Title : public SceneBase
{
private:
	TitleState State = TitleState::SELECT_WAIT;
	int		Anim_count;
	float	Anim_flame;
	int Selected = 0;
	int Y;

	int MovieHandle;

	int BackGroundImage;
	int SelectBackGroundImage;
	int BackBackGroundImage;
	int Select_End;
	int Default_End;
	int Select_Start;
	int Default_Start;
	std::vector<int> SellectImage;
	int BGM;
	int DecisionSE;
public:
	// �R���X�g���N�^
	Title();
	// �f�X�g���N�^
	virtual ~Title();

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

