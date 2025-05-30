#pragma once

#include "../../Character.h"
#include <chrono>

// �G�������N���X
class E_Ranged :public Character
{
private:
	static size_t count;
	int anim_max_count;		// �A�j���[�V�����摜�̖���
	float recovery_time;	// �d������
	float damage_rate;		// �����_���[�W�̕p�x
	float anim_rate;		// �A�j���[�V�����̑���
	float speed;			// ��������
	int Damage;				//�_���[�W
	bool old_light;			// �O��̃��C�g�t���O

	// �G�t�F�N�g
	int effect;
	std::vector<int> effect_image;
	float effect_flame;		// �G�t�F�N�g�A�j���[�V�����̑���
	float effect_count;		// �G�t�F�N�g�A�j���[�V�����̃J�E���g

public:
	/// <summary>
	/// �G�ߐڂ̃J�E���g�擾����
	/// </summary>
	/// <returns>�G�ߐڂ̐�</returns>
	static size_t GetCount();

public:
	//�R���X�g���N�^
	E_Ranged();
	//�f�X�g���N�^
	virtual ~E_Ranged();

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
	/// ���C�g�͈͒ʒm����
	/// </summary>
	virtual void InLightRange() override;
	/// <summary>
	/// ���C�g�͈͒ʒm����
	/// </summary>
	virtual void OutLightRange() override;
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
	/// �U������
	/// </summary>
	virtual void Attack(GameObject* hit_object);
};