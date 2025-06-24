#include "Pause.h"

// コンストラクタ
Pause::Pause():
	is_pause(false)
{
	
}

// デストラクタ
Pause::~Pause()
{

}

// 初期化処理
void Pause::Initialize()
{

}

// 更新処理
void Pause::Update(float delta_second)
{

}

// 描画処理
void Pause::Draw() const
{

}

// ポーズフラグ設定処理
void Pause::SetPause(bool flag)
{
	this->is_pause = flag;
}

// ポーズフラグ取得処理
bool Pause::GetPause()
{
	return is_pause;
}
