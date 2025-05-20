#include "Oracle.h"
#include "../Guardian/P_Guardian.h"
#include "../../../../Scenes/InGame/InGame.h"


// コンストラクタ
Oracle::Oracle() :
	summon_flag(),
	player()
{

}

// デストラクタ
Oracle::~Oracle()
{

}

// 初期化処理
void Oracle::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	EffectImage = rm->GetImages("Resource/Images/Effect/EnemyPawn.png", 117, 13, 9, 64, 64);
	image = rm->GetImages("Resource/Images/Unit/Oracle/Oracle01.png",1,1,1,2048,2048)[0];

	is_mobility = false;
	is_aggressive = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Player;
	collision.hit_object_type.push_back(eObjectType::Enemy);
	collision.collision_size = Vector2D(60.0f, 120.0f);
	collision.hitbox_size = Vector2D(200.0f, 200.0f);
	z_layer = 1;

	summon_flag = false;

	// HP初期化
	HP = 100;
}

// 更新処理
void Oracle::Update(float delta_second)
{


	if (CheckHitKey(KEY_INPUT_6))
	{
		HP--;
	}

	/*if (summon_flag == true)
	{
		attack_flame += delta_second;
	}*/
}

// 描画処理
void Oracle::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	if (ProjectConfig::DEBUG)
	{
		// 巫女の描画
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),//数字を変える
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0xff0000, TRUE);
		// 中心を表示
		DrawCircle((int)position.x, (int)position.y, 2, 0x0000ff, TRUE);
		// 当たり判定表示
		DrawBox((int)(position.x - collision.collision_size.x / 2), (int)(position.y - collision.collision_size.y / 2),
			(int)(position.x + collision.collision_size.x / 2), (int)(position.y + collision.collision_size.y / 2), 0x0000ff, FALSE);
		// 攻撃範囲を表示
		DrawBox((int)position.x, (int)(position.y - collision.hitbox_size.y / 2),
			(int)(position.x - collision.hitbox_size.x), (int)(position.y + collision.hitbox_size.y / 2), 0x0000ff, FALSE);
		//残りHPの表示
		DrawFormatString(position.x, position.y - 40.0f, 0xffffff, "%d", HP);
	}



	DrawBoxAA(position.x - 50.0f, position.y - 150.0f, position.x + (50.0f - (100 - HP)), position.y - 135.0f, 0xFFFFFF, true);


	DrawRotaGraphF(position.x-5.0f, position.y+1.0f, 0.06, 0.0, image, TRUE, flip_flag);



}

// 終了時処理
void Oracle::Finalize()
{

}

// 当たり判定通知処理
void Oracle::OnHitCollision(GameObject* hit_object)
{

}

// 攻撃範囲通知処理
void Oracle::OnAreaDetection(GameObject* hit_object)
{
	//Collision hit_col = hit_object->GetCollision();
	//velocity.x = -1.0f;
	//if (hit_col.object_type == eObjectType::Enemy)
	//{
	//	summon_flag = true;
	//	velocity.x = 0.0f;
	//	if (attack_flame >= 2.0f)
	//	{
	//		//ここでユニットの召喚をする？

	//	}
	//}
}

// 攻撃処理
void Oracle::Attack()
{
	
}


// HP管理処理
void Oracle::HPControl(int Damage)
{
	HP -= Damage;
	
	GameObjectManager* object = GameObjectManager::GetInstance();

	if (!summon_flag)
	{

			summon_flag = true;
	
			object->CreateObject<P_Guardian>(Vector2D(location.x, location.y + 15.0f));
	}

}

// 移動処理
void Oracle::Movement(float delta_second)
{

}

// アニメーション制御処理
void Oracle::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void Oracle::EffectControl(float delta_second)
{

}