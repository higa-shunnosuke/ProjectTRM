#include "GameObject.h"
#include "../Scenes/SceneBase.h"
#include "DxLib.h"

GameObject::GameObject() :
	location(0.0f),
	image(NULL),
	z_layer(0),
	is_mobility(false)
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
