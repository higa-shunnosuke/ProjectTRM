#pragma once

#include "../UnitBase.h"
#include "../../../GameObjectManager.h"

#define BASIC_Ranged_SPEED (-3.0f)

// �Q�[���I�u�W�F�N�g���N���X
class P_Ranged :public UnitBase
{
private:
	static size_t count;
public:
	static size_t GetCount();
private:
	Vector2D target_loc;


public:
	//�R���X�g���N�^
	P_Ranged();
	//�f�X�g���N�^
	virtual ~P_Ranged();

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