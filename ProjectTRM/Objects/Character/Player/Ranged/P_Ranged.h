#pragma once

#include "../../Character.h"
#include "../../../GameObjectManager.h"

#define BASIC_SPEED (-3.0f)

// �Q�[���I�u�W�F�N�g���N���X
class P_Ranged :public Character
{
private:
	static size_t count;
public:
	static size_t GetCount();
private:
	int effect_image;
	GameObjectManager* object;
	int effect_alpha;
	std::vector<int> Effect;
	int sounds;	//��
	Vector2D target_loc;
	int anim_max_count;		// �A�j���[�V�����摜�̖���
	int effect_max_count;
	std::vector<int> text;
	std::vector<int> reduction_amount;	//HP������
	std::vector<float> damage_time; //�_���[�W�̕\�L����
	int minus;

public:
	//�R���X�g���N�^
	P_Ranged();
	//�f�X�g���N�^
	virtual ~P_Ranged();

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
	virtual void HPControl(int Damage) override;

private:
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
	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack(GameObject* hit_object);
};