#include "Torch.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/LightMapManager.h"

// コンストラクタ
Torch::Torch() :
	life_second()
{

}

// デストラクタ
Torch::~Torch()
{

}

// 初期化処理
void Torch::Initialize()
{
	ResourceManager* rm = ResourceManager::GetInstance();
	animation = rm->GetImages("Resource/Images/Unit/Melee/Torch/Torch_Move.png", 4, 4, 1, 32, 32);
	life_second = 10.0f;
	now_state = State::Move;
	LightMapManager* light = LightMapManager::GetInstance();
	LightDetail detail;
	detail.object = this;
	detail.size = 0.5f;
	light->AddLight(detail);
}

// 更新処理
void Torch::Update(float delta_second)
{
	life_second -= delta_second;

	if (life_second <= 0.0f)
	{
		now_state = State::Death;
		life_second;
	}

	AnimationControl(delta_second);

	old_state = now_state;
}

// 描画処理
void Torch::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(position.x, position.y, 2.0, 0.0, image, TRUE);
}

// 終了時処理
void Torch::Finalize()
{
	//ゲームオブジェクトマネージャーのポインター
	GameObjectManager* object = GameObjectManager::GetInstance();
	LightMapManager* light = LightMapManager::GetInstance();
	light->DeleteLight(this);
	object->DestroyObject(this);
}

// 当たり判定通知処理
void Torch::OnHitCollision(GameObject* hit_object)
{

}

// アニメーション制御処理
void Torch::AnimationControl(float delta_second)
{
	if (now_state != old_state)
	{
		ResourceManager* rm = ResourceManager::GetInstance();
		Anim_count = 0;
		Anim_flame = 0;
		switch (now_state)
		{
		case State::Move:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Torch/Torch_Move.png", 4, 4, 1, 32, 32);
			break;
		case State::Death:
			animation = rm->GetImages("Resource/Images/Unit/Melee/Torch/Torch_Death.png", 4, 4, 1, 32, 32);
			break;
		default:
			break;
		}
	}

	Anim_flame += delta_second;

	if (Anim_flame >= 0.1f)
	{
		if (Anim_count < 3)
		{
			Anim_count++;
		}
		else
		{
			Anim_count = 0;
		}

		Anim_flame = 0.0f;
	}
	switch (now_state)
	{
	case State::Move:
		image = animation[Anim_count];
		break;
	case State::Death:
		image = animation[Anim_count];
		if (Anim_count >= 3)
		{
			Finalize();
		}
		break;
	default:
		break;
	}

}
// エフェクト制御処理
void Torch::EffectControl(float delta_second)
{

}