#include "Heretic.h"

#include"../../../../Scenes/InGame/InGame.h"

#include "../../Player/Tank/P_Tank.h"
#include "../../Player/Melee/P_Melee.h"
#include "../../Player/Ranged/P_Ranged.h"

#include "../Tank/E_Tank.h"
#include "../Ranged/E_Ranged.h"
#include "../Melee/E_Melee.h"

#define Enemy_Plan_Evaluation // 戦場評価型
#ifdef Enemy_Plan_Evaluation
#else
#define Enemy_Plan_WAVE//ウェーブ型
#endif // Evalution


// コンストラクタ
Heretic::Heretic() :
	summon_flag(),
	CountFlame(0.0f),
	CountTime(0),
	Cost(0)
{

}

// デストラクタ
Heretic::~Heretic()
{

}

// 初期化処理
void Heretic::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	GOM = GOM->GetInstance();

	is_mobility = false;

	collision.is_blocking = true;
	collision.object_type = eObjectType::Enemy;
	collision.hit_object_type.push_back(eObjectType::Player);
	collision.box_size = Vector2D(60.0f, 120.0f);
	collision.attack_size = Vector2D(400.0f, 200.0f);
	z_layer = 1;

	// HP初期化
	HP = 100;
}

// 更新処理
void Heretic::Update(float delta_second)
{
	if (Fstflag)
	{
		Ingame->CreateEnemy(E_enemy::Melee);
		Fstflag = false;
	}

	CountFlame += 1.0f;
	if (CountFlame < 60.0f)
	{
		CountTime++;
		Cost+= 10;
		CountFlame = 0.0f;
	}

	int Pcount_sum =0;
	int Ecount_sum =0;

	int Ptank_count	  = (int)P_Tank::  GetCount() * TANK_eva;
	int Pmelee_count  = (int)P_Melee:: GetCount() * MELEE_eva;
	int Prange_count  = (int)P_Ranged::GetCount() * RANGE_eva;
	Pcount_sum += (Ptank_count + Pmelee_count + Prange_count);

	int Etank_count  = (int)E_Tank::  GetCount() * TANK_eva;
	int Emelee_count = (int)E_Melee:: GetCount() * MELEE_eva;
	int Erange_count = (int)E_Ranged::GetCount() * RANGE_eva;
	Ecount_sum += (Etank_count + Emelee_count + Erange_count);

	//・コストが０以下になるなら生成しない。
	if (Cost >= ENEMY_BOTTOM_COST)
	{
		//・生成するでな
		//この辺の処理は関数に飛ばそうかな…
	}
	//・コストが最大になるなら手持ち最大コストを生成する。
	if (Cost >= 500)
	{
		//・生成するでな
		//この辺の処理は関数に飛ばそうかな…
	}
	//・相手の評価が高くなった際に手持ち最大コストを生成する。
	if (Pcount_sum > Ecount_sum)
	{

	}
	//・相手の評価が低くなった際に手持ち最小コストを生成する。
	else
	{

	}

}

// 描画処理
void Heretic::Draw(const Vector2D camera_pos) const
{
	Vector2D position = this->GetLocation();
	position.x -= camera_pos.x - D_WIN_MAX_X / 2;

	// 異端者の描画
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0x0000ff, TRUE);

#ifdef DEBUG
	// 中心を表示
	DrawCircle((int)position.x, (int)position.y, 2, 0xff0000, TRUE);
	// 当たり判定表示
	DrawBox((int)(position.x - collision.box_size.x / 2), (int)(position.y - collision.box_size.y / 2),
		(int)(position.x + collision.box_size.x / 2), (int)(position.y + collision.box_size.y / 2), 0xff0000, FALSE);
	// 攻撃範囲を表示
	DrawBox((int)(position.x - collision.attack_size.x / 2), (int)(position.y - collision.attack_size.y / 2),
		(int)(position.x + collision.attack_size.x / 2), (int)(position.y + collision.attack_size.y / 2), 0xff0000, FALSE);
#endif
}

// 終了時処理
void Heretic::Finalize()
{

}

void Heretic::SetInGamePoint(InGame* point)
{
	Ingame = point;
}

// 当たり判定通知処理
void Heretic::OnHitCollision(GameObject* hit_object)
{

}

// HP管理処理
void Heretic::HPControl(int Damage)
{

}

// 移動処理
void Heretic::Movement(float delta_second)
{

}

// アニメーション制御処理
void Heretic::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void Heretic::EffectControl(float delta_second)
{

}

// 攻撃処理
void Heretic::Attack()
{

}