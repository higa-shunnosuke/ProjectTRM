#include "E_Melee.h"

// コンストラクタ
E_Melee::E_Melee() :
	Damage()
{

}

// デストラクタ
E_Melee::~E_Melee()
{

}

// 初期化処理
void E_Melee::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	is_mobility = true;
	is_aggressive = true;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 60.0f);
	collision.attack_size = Vector2D(100.0f, 100.0f);
	z_layer = 2;

	attack_flag = false;

	// 右へ移動
	velocity.x = 1.0f;

	//攻撃力
	Damage = 2;

	// HP初期化
	HP = 20;
}

// 更新処理
void E_Melee::Update(float delta_second)
{
	// 移動処理
	Movement(delta_second);

	if (attack_flag == true)
	{
		attack_flame += delta_second;
	}
}

// 描画処理
void E_Melee::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 近接ユニットの描画
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff00a0, TRUE);

#ifdef DEBUG
	//残りHPの表示
	DrawFormatString(position.x, position.y - 40.0f, 0xffffff, "%d", HP);

	// 中心を表示
	DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
	// 当たり判定表示
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, FALSE);
	// 攻撃範囲を表示
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0x0000ff, FALSE);
#endif
}

// 終了時処理
void E_Melee::Finalize()
{

}

// 当たり判定通知処理
void E_Melee::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void E_Melee::OnAreaDetection(GameObject* hit_object)
{
	Collision hit_col = hit_object->GetCollision();
	velocity.x = 1.0f;
	if (hit_col.object_type == eObjectType::Player)
	{
		attack_flag = true;
		velocity.x = 0.0f;
		if (attack_flame >= 2.0f)
		{
			Attack(hit_object);
		}
	}
}

// HP管理処理
void E_Melee::HPControl(int Damage)
{
	HP -= Damage;
}

// 攻撃処理
void E_Melee::Attack(GameObject* hit_object)
{
	hit_object->HPControl(Damage);
	attack_flag = false;
	attack_flame = 0.0f;
}

// 移動処理
void E_Melee::Movement(float delta_second)
{

	// 移動の実行
	location.x += velocity.x * 10 * delta_second;
}

// アニメーション制御処理
void E_Melee::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void E_Melee::EffectControl(float delta_second)
{

}
