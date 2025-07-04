#include "Character.h"

// コンストラクタ
Character::Character():
	now_state(),
	old_state()
{

}

// デストラクタ
Character::~Character()
{

}

// 初期化処理
void Character::Initialize()
{

}

// 更新処理
void Character::Update(float delta_second)
{

}

// 描画処理
void Character::Draw() const
{
	// オフセット値を基に画像の描画を行う
	DrawRotaGraphF(location.x, location.y, 1.0, 0.0, image, TRUE);
}

// 終了時処理
void Character::Finalize()
{

}

// 移動処理
void Character::Movement(float delta_second)
{

}

// アニメーション制御処理
void Character::AnimationControl(float delta_second)
{

}
// エフェクト制御処理
void Character::EffectControl(float delta_second)
{

}

// SE制御処理
void Character::SoundControl()
{

}

// 攻撃処理
void Character::Attack(GameObject* hit_object)
{

}