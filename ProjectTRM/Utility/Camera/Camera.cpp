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
	// ズーム倍率を設定
	zoom = 2.0f;

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
	InputManager* input = InputManager::GetInstance();

	// ズーム
	Zoom();

	// スクロール
	Scroll();

	// 最前線へ移動
	if (input->GetButtonState(XINPUT_BUTTON_LEFT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = true;
	}

	// 初期座標へ移動
	if (input->GetButtonState(XINPUT_BUTTON_RIGHT_SHOULDER) == eInputState::Pressed)
	{
		tracking_flag = false;
		// カメラの初期化
		zoom = 2.0f;
		this->location.x = ProjectConfig::STAGE_WIDTH - (size.x / 2);
		this->location.y = size.y / 2;
	}

	/// ステージ外にいかないようにする処理
	float maxCameraX = 1280 - (1280 / zoom);
	if (location.x < 0) location.x = 0.0f;
	if (location.x > maxCameraX) location.x = maxCameraX;
}

// 描画処理
void Camera::Draw(int back_buffer)
{
	// スケーリング後のサイズ
	int drawW = (int)(D_WIN_MAX_X * zoom);
	int drawH = (int)(D_WIN_MAX_Y * zoom);

	// 描画位置（X はスクロールに応じて、Y は下合わせ）
	int drawX = -(int)(location.x * zoom);
	int drawY = 720 - drawH;  // 画面下端に合わせる

	// 背景を拡大・縮小＋スクロールして描画
	DrawExtendGraph(drawX, drawY, drawX + drawW, drawY + drawH, back_buffer, TRUE);
}

// ズーム処理
void Camera::Zoom()
{
	// ズーム変更前の値を記録
	float prevZoom = zoom;

	// マウス
	int wheel = GetMouseWheelRotVol();
	if (wheel != 0) 
	{
		// ズーム倍率を計算
		zoom += wheel * 0.1f;
	}
	// コントローラー
	InputManager* input = InputManager::GetInstance();
	if (input->GetLeftTrigger() > 0.0f)
	{
		// 倍率を計算
		zoom -= input->GetLeftTrigger() * 0.01f;
	}
	if (input->GetRightTrigger() > 0.0f)
	{
		// 倍率を計算
		zoom += input->GetRightTrigger() * 0.01f;
	}


	if (zoom < ZOOM_MIN) zoom = ZOOM_MIN;
	if (zoom > ZOOM_MAX) zoom = ZOOM_MAX;

	// 画面中央のワールド座標を保つためにカメラ位置を補正
	float centerX = location.x + (640.0f / prevZoom);
	float centerY = location.y + (360.0f / prevZoom);

	location.x = centerX - (640.0f / zoom);
	location.y = centerY - (360.0f / zoom);
}

// スクロール処理
void Camera::Scroll()
{
	InputManager* input = InputManager::GetInstance();

	// 手動スクロール（コントローラー）
	if (input->GetLeftStick().x < 0.0f)
	{
		tracking_flag = false;
		location.x += (float)(input->GetLeftStick().x * 1.5);
	}
	else if (input->GetLeftStick().x > 0.0f)
	{
		tracking_flag = false;
		location.x += (float)(input->GetLeftStick().x * 1.5);
	}
	// 手動スクロール（キーボード）
	if (input->GetKeyState(KEY_INPUT_D) == eInputState::Hold)
	{
		location.x += 2.0f;
	}
	if (input->GetKeyState(KEY_INPUT_A) == eInputState::Hold)
	{
		location.x -= 2.0f;
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
}

// カメラ座標設定処理
void Camera::SetCameraPos(Vector2D location)
{
	this->location = location;
}

// 巫女情報設定処理
void Camera::SetPlayer(Oracle* oracle)
{
	this->oracle = oracle;
}

void Camera::SetZoom(float zoom)
{
	this->zoom = zoom;
}

float Camera::GetZoom()
{
	return zoom;
}