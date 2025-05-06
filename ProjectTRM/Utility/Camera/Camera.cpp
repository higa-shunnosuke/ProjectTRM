#include "Camera.h"
#include "../ProjectConfig.h"
#include "../Input/InputManager.h"
#include "DxLib.h"

// コンストラクタ
Camera::Camera():
	location()
{

}

// デストラクタ
Camera::~Camera()
{

}

// 初期化処理
void Camera::Initialize()
{
	// カメラの初期座標を設定
	this->location.x = BOX_SIZE * STAGE_SIZE_X - (D_WIN_MAX_X / 2);
	this->location.y = D_WIN_MAX_Y / 2;
}

// 更新処理
void Camera::Update()
{
	//追跡処理
	InputManager* input = InputManager::GetInstance();

	if (input->GetLeftStick().x < 0.0f)
	{
		location.x += input->GetLeftStick().x;
	}
	if (input->GetLeftStick().x > 0.3f)
	{
		location.x += input->GetLeftStick().x;
	}

#ifdef DEBUG
	//キーボード操作
	if (input->GetKeyState(KEY_INPUT_D) == eInputState::Hold)
	{
		location.x += 2.0f;
	}
	if (input->GetKeyState(KEY_INPUT_A) == eInputState::Hold)
	{
		location.x -= 2.0f;
	}
#endif

	/// ステージ外にいかないようにする処理
	float right,left;
	right = D_WIN_MAX_X / 2 + 40;
	left = D_WIN_MAX_X / 2;

	//右端の制限
	if (location.x > right)
	{
		location.x = right;
	}
	//左端の制限
	if (location.x < left)
	{
		location.x = left;
	}
}

// カメラ座標取得処理
Vector2D Camera::GetCameraPos()
{
	return this->location;
}

// カメラ座標設定処理
void Camera::SetCameraPos(Vector2D location)
{
	this->location = location;
}