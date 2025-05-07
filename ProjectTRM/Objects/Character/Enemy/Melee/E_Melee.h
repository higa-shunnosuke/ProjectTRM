#pragma once

#include "../../Character.h"
#include <chrono>

// �Q�[���I�u�W�F�N�g���N���X
class E_Melee :public Character
{
private:
	static size_t count;	// �G�ߐڂ̃J�E���^
	std::chrono::steady_clock::time_point anime_time;	// �A�j���[�V�����J�n����
	std::chrono::steady_clock::time_point recovery_time;	// �d���J�n����

public:
	/// <summary>
	/// �G�ߐڂ̃J�E���g�擾����
	/// </summary>
	/// <returns>�G�ߐڂ̐�</returns>
	static size_t GetCount();

private:
	int Damage;	//�_���[�W
	int old_HP;

public:
	//�R���X�g���N�^
	E_Melee();
	//�f�X�g���N�^
	virtual ~E_Melee();

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
	/// HP�Ǘ�����
	/// </summary>
	/// <param name="hit_object">�_���[�W</param>
	virtual void HPControl(int Damage) override;

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
};