#pragma once

#include "../Character.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/LightMapManager.h"
#include "../../../Scenes/InGame/InGame.h"

class UnitBase : public Character
{
protected:
	int Damage;	//�_���[�W
	int sounds;	//��
	float basic_power;	//��b�U����
	float basic_speed;	//��b�ړ����x
	LightMapManager* light; //����
	int anim_max_count;
	GameObjectManager* object;

	//�G�t�F�N�g��
	int effect_image;
	int effect_alpha;
	std::vector<int> Effect;
	int effect_max_count;	//�G�t�F�N�g�摜�̖���

	//�_���[�W�\�L�֘A
	std::vector<int> text;
	std::vector<int> reduction_amount;	//HP������
	std::vector<float> damage_time; //�_���[�W�̕\�L����
	int minus;

public:
	//�R���X�g���N�^
	UnitBase();
	//�f�X�g���N�^
	virtual ~UnitBase();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	virtual void Update(float delta_second) override;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(const Vector2D camera_pos) const override;
	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize() override;

public:
	/// <summary>
	/// �����蔻��ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnHitCollision(GameObject* hit_object) override;
	/// <summary>
	/// �U���͈͒ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnAreaDetection(GameObject* hit_object) override;
	/// <summary>
	/// �U���͈͒ʒm����
	/// </summary>
	virtual void NoHit() override;
	/// <summary>
	/// HP�Ǘ�����
	/// </summary>
	/// <param name="hit_object">�_���[�W</param>
	virtual void HPControl(int Damage) override;

protected:
	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack(GameObject* hit_object);
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void Movement(float delta_second);
	/// <summary>
	/// �A�j���[�V�������䏈��
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void AnimationControl(float delta_second);
	/// <summary>
	/// �G�t�F�N�g���䏈��
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void EffectControl(float delta_second);

	/// <summary>
	/// SE���䏈��
	/// </summary>
	virtual void SoundControl();
};

