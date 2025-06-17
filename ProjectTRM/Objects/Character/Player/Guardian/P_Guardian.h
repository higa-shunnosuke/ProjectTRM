#pragma once
#include "../../Character.h"
#include "../../../GameObjectManager.h"

#define BASIC_Guardian_POWER (20)
#define BASIC_Guardian_SPEED (-4.0f)

class P_Guardian : public Character
{
private:
	static size_t count;

public:
	static size_t GetCount();

private:
	std::vector<int> Effect;
	int effect_image;
	int Damage;	//�_���[�W
	GameObjectManager* object;
	int anim_max_count;		// �A�j���[�V�����摜�̖���
	int sounds;
	int effect_alpha;	//�G�t�F�N�g�̓����x�����p
	int effect_max_count;
	std::vector<int> text;
	std::vector<int> reduction_amount;	//HP������
	std::vector<float> damage_time; //�_���[�W�̕\�L����
	int minus;

public:
	//�R���X�g���N�^
	P_Guardian();
	//�f�X�g���N�^
	virtual ~P_Guardian();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	virtual void Update(float delta_second);
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(const Vector2D camera_pos) const;
	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize();

public:
	/// <summary>
	/// �����蔻��ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnHitCollision(GameObject* hit_object);

	/// <summary>
	/// �U���͈͒ʒm����
	/// </summary>
	/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
	virtual void OnAreaDetection(GameObject* hit_object);

	/// <summary>
	/// �U���͈͒ʒm����
	/// </summary>
	virtual void NoHit() override;

	/// <summary>
	/// HP�Ǘ�����
	/// </summary>
	/// <param name="hit_object">�_���[�W</param>
	virtual void HPControl(float Damage) override;

private:
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

