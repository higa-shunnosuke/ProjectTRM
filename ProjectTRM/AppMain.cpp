/****************************************************************
* TGS2025
* ProjectTRM
*****************************************************************/
#include "DxLib.h"
#include "Application.h"

// メイン関数
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	try
	{
		// プロジェクトを生成する
		Application* app = Application::GetInstance();

		// 初期化処理
		app->WakeUp();

		// 実行処理
		app->Run();

		// 終了時処理
		app->Shutdown();
	}
	catch (std::string error_log)
	{
		// エラー内容を出力する
		return ErrorThrow(error_log);
	}

	// 終了状態を通知する
	return D_SUCCESS;
}