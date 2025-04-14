#include "Camera.h"
#include "../ProjectConfig.h"

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
	this->location.x = D_WIN_MAX_X / 2;
	this->location.y = D_WIN_MAX_Y / 2;
}

// 更新処理
void Camera::Update()
{
	
	//追跡処理


	//ステージ外にいかないようにする処理
	float right;
	right = STAGE_SIZE_X * BOX_SIZE - D_WIN_MAX_X / 2;
	if (location.x > right)
	{
		location.x = right;
	}
}

// カメラ情報取得処理
Vector2D Camera::GetCameraPos()
{
	return this->location;
}

// 追跡対象情報設定処理
void Camera::SetPlayer()
{
	
}