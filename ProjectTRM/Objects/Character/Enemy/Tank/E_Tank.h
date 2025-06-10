#pragma once

#include "../EnemyBase.h"

// �G�^���N�N���X
class E_Tank :public EnemyBase
{
private:
	static size_t count;

public:
	/// <summary>
	/// �G�̃J�E���g�擾����
	/// </summary>
	/// <returns>�G�̐�</returns>
	static size_t GetCount();

public:
	//�R���X�g���N�^
	E_Tank();
	//�f�X�g���N�^
	virtual ~E_Tank();

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


private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void Movement(float delta_second) override;
	/// <summary>
	/// �A�j���[�V�������䏈��
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void AnimationControl(float delta_second) override;
	/// <summary>
	/// �G�t�F�N�g���䏈��
	/// </summary>
	/// <param name="hit_object">1�t���[��������̎���</param>
	virtual void EffectControl(float delta_second) override;
};