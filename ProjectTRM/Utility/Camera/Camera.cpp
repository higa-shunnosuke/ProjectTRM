#include "Camera.h"
#include "../ProjectConfig.h"
#include "../Input/InputManager.h"
#include "../../Objects/GameObjectManager.h"
#include "../Collision.h"
#include <cmath>

// コンストラクタ
Camera::Camera():
	location(),
	size(),
	oracle(nullptr),
	tracking_flag(false)
{

}

// デストラクタ
Camera::~Camera()
{

}

// 初期化処理
void Camera::Initialize()
{
	// スクリーンの初期サイズを設定
	this->size.x = D_WIN_MAX_X;
	this->size.y = D_WIN_MAX_Y;

	// カメラの初期座標を設定
	this->location.x = ProjectConfig::STAGE_WIDTH - (size.x / 2);
	this->location.y = size.y / 2;
}

// 更新処理
void Camera::Update()
{
	//追跡処理
	InputManager* input = InputManager::GetInstance();

	// 手動スクロール
	if (input->GetLeftStick().x < 0.0f)
	{
		tracking_flag = false;
		location.x += (float)input->GetLeftStick().x * 1.5;
	}
	else if (input->GetLeftStick().x > 0.0f)
	{
		tracking_flag = false;
		location.x += (float)input->GetLeftStick().x * 1.5;
	}

	// 自動スクロール
	if (tracking_flag == true)
	{
		// オブジェクトマネージャーの情報取得
		GameObjectManager* object = GameObjectManager::GetInstance();
		// オブジェクトリストを取得
		std::vector<GameObject*> objects_list = object->GetObjectsList();

		// 最長距離を初期化
		float longest_distance = 0.0f;
		GameObject* front_line = nullptr;

		if (!objects_list.empty() && oracle != nullptr)
		{
			for (GameObject* obj : objects_list)
			{
				// ユニット以外は無視する
				if (obj->GetCollision().object_type != eObjectType::Player ||
					obj->GetMobility() == false)
				{
					continue;
				}

				// 巫女とユニットの距離を計算
				Vector2D diff = obj->GetLocation() - oracle->GetLocation();
				float distance = sqrtf(diff.x * diff.x + diff.y * diff.y);

				// 最長距離を更新
				if (distance > longest_distance)
				{
					longest_distance = distance;
					front_line = obj;
				}
			}
			// 前線を追跡
			if (front_line != nullptr)
			{
				location.x = front_line->GetLocation().x;
			}
		}
	}
	
	// 最前線へ移動
	if (input->GetButtonState(XINPUT_BUTTON_LEFT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = true;
	}

	// 初期座標へ移動
	if (input->GetButtonState(XINPUT_BUTTON_RIGHT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = false;
		// カメラの座標を初期化
		this->location.x = ProjectConfig::STAGE_WIDTH - (size.x / 2);
		this->location.y = size.y / 2;
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
	right = ProjectConfig::STAGE_WIDTH - (size.x / 2);
	left = size.x / 2;

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

// スクリーンサイズ取得処理
Vector2D Camera::GetScreeenSize()
{
	return this->size;
}

// 巫女情報設定処理
void Camera::SetPlayer(Oracle* oracle)
{
	this->oracle = oracle;
}