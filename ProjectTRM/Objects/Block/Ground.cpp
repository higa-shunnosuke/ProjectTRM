#include "Ground.h"


Ground::Ground()
{

}

Ground::~Ground()
{

}

void Ground::Initialize()
{
	is_mobility = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Ground;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 60.0f);

	HP = 1;

}

void Ground::Update(float delta_second)
{

}

void Ground::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;
	position.y += camera_pos.y - D_WIN_MAX_Y / 2;


	if (ProjectConfig::DEBUG)
	{	// ìñÇΩÇËîªíËï\é¶
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x00ff00, FALSE);
	}
}

void Ground::Finalize()
{

}

// ìñÇΩÇËîªíËí ímèàóù
void Ground::OnHitCollision(GameObject* hit_object)
{

}