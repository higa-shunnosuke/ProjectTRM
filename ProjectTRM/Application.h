#pragma once

#include "DxLib.h"

#include "Utility/Singleton.h"
#include "Utility/ProjectConfig.h"
#include "Utility/Input/InputManager.h"
#include "Scenes/SceneManager.h"

class Application : public Singleton<Application>
{
private:
	// グローバル変数定義
	LONGLONG old_time = 0;			// 前回計測値
	LONGLONG now_time = 0;			// 現在計測値
	float delta_second = 0.0f;		// １フレームあたりの時間
	bool end_flg = false;			// ゲーム終了フラグ

public:

	// 起動
	bool WakeUp()
	{
		// ウィンドウモードで起動する
		ChangeWindowMode(TRUE);

		// ウィンドウサイズの設定
		SetGraphMode(D_WIN_MAX_X, D_WIN_MAX_Y, D_COLOR_BIT);

		// ウィンドウタイトルの設定
		SetWindowText("Torchguard ～Shadows Fall～");

		// 垂直同期を行わない
		SetWaitVSyncFlag(FALSE);

		// Log.txtファイルの生成制御（Debugモードのみ生成する）
#if _DEBUG
		SetOutApplicationLogValidFlag(TRUE);
#else
		SetOutApplicationLogValidFlag(FALSE);
#endif // _DEBUG

		// Dxライブラリの初期化
		if (DxLib_Init() == D_FAILURE)
		{
			throw std::string("Dxライブラリの初期化に失敗しました！\n");
		}

		// 描画先を表画面に反映する
		SetDrawScreen(DX_SCREEN_BACK);

		// シーンマネージャー情報を取得
		SceneManager* manager = SceneManager::GetInstance();

		// シーンマネージャーを初期化
		manager->Initialize();

		// 非アクティブ状態でも動作させる
		SetAlwaysRunFlag(TRUE);

		return true;
	}

	// 実行
	void Run()
	{
		// シーンマネージャー情報を取得
		SceneManager* manager = SceneManager::GetInstance();

		// 入力情報を取得
		InputManager* input = InputManager::GetInstance();

		// メインループ
		while (ProcessMessage() == D_SUCCESS)
		{			
			// 入力情報の更新
			input->Update();

			// フレームレートの制御
			UpdateDeltaTime();

			// シーンマネージャーを更新
			manager->Update(delta_second);
		
			// 強制終了するか確認
			if ((input->GetButtonState(XINPUT_BUTTON_BACK) == eInputState::Released) ||
				(input->GetKeyState(KEY_INPUT_ESCAPE) == eInputState::Released))
			{
				// ゲームを終了
				QuitGame();
			}

			// ゲームを終了するか確認
			if (end_flg == true)
			{
				// メインループを終了
				break;
			}
		}
	}

	//終了
	void Shutdown()
	{
		// シーンマネージャー情報を取得
		SceneManager* manager = SceneManager::GetInstance();

		// 終了時処理
		manager->Finalize();

		// Dxライブラリの使用を終了する
		DxLib_End();
	}

	//ゲーム終了通知処理
	void QuitGame()
	{
		// ゲーム終了フラグを立てる
		this->end_flg = true;
	}

private:
	void UpdateDeltaTime()
	{
		// 現在時間の取得（μ秒）
		now_time = GetNowHiPerformanceCount();

		// 開始時間から現在時間までに経過した時間を計算する（μ秒）
		// 分解能をμ秒→秒に変換する
		delta_second = (float)(now_time - old_time) * 1.0e-6f;

		// 計測開始時間を更新する
		old_time = now_time;

		// ディスプレイのリフレッシュレートを取得する
		float refresh_rate = (float)GetRefreshRate();

		// １フレーム当たりの時間が1/リフレッシュレート秒を超えたら、整える
		if (delta_second > (1.0f / refresh_rate))
		{
			delta_second = (1.0f / refresh_rate);
		}
	}
};