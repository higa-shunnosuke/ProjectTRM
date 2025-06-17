#include "GameObject.h"
#include "../Scenes/SceneBase.h"
#include "DxLib.h"

GameObject::GameObject() :
	location(0.0f),
	image(NULL),
	z_layer(0),
	HP(0),
	is_mobility(false),
	is_aggressive(false)
{

}

GameObject::~GameObject()
{

}

// ����������
void GameObject::Initialize()
{

}

// �X�V����
void GameObject::Update(float delta_second)
{

}

// �`�揈��
void GameObject::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// �I�t�Z�b�g�l����ɉ摜�̕`����s��
	DrawRotaGraphF(camera_pos.x, camera_pos.y, 1.0, 0.0, image, TRUE);
}

// �I��������
void GameObject::Finalize()
{

}

// �����蔻��ʒm����
void GameObject::OnHitCollision(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void GameObject::OnAreaDetection(GameObject* hit_object)
{

}

// �U���͈͒ʒm����
void GameObject::NoHit()
{

}

// ���C�g�͈͒ʒm����
void GameObject::InLightRange()
{

}

// ���C�g�͈͒ʒm����
void GameObject::OutLightRange()
{

}

// �ʒu���W�擾����
const Vector2D& GameObject::GetLocation() const
{
	return location;
}

// �ʒu���ύX����
void GameObject::SetLocation(const Vector2D& location)
{
	this->location = location;
}

// �����蔻��擾����
const Collision& GameObject::GetCollision() const
{
	return collision;
}

// Z���C���[���擾����
const unsigned char GameObject::GetZLayer() const
{
	return z_layer;
}

// �������̎擾����
const bool GameObject::GetMobility() const
{
	return is_mobility;
}

// �U�������̎擾����
const bool GameObject::GetAggressive() const
{
	return is_aggressive;
}

// �͈͍U���t���O�擾����
const bool GameObject::GetAoE() const
{
	return is_AoE;
}

// HP�Ǘ�����
void GameObject::HPControl(float Damage)
{
	this->HP -= Damage;
	if (this->HP < 0)
	{
		this->HP = 0;
	}
}

// HP�擾����
const int GameObject::GetHP() const
{
	return this->HP;
}

// ���C�g���t���O�擾����
const bool GameObject::GetInLight() const
{
	return this->in_light;
}

/// <summary>
/// InGame�̏��擾
/// </summary>
/// <param name="point">InGameScene��Pointer</param>
void GameObject::SetInGamePoint(class InGame* point)
{
	Ingame = point;
}