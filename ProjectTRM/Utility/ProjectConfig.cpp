#include "ProjectConfig.h"
#include "DxLib.h"

// デバックフラグの初期化
bool ProjectConfig::DEBUG = false;

// エラー内容を出力する
int ErrorThrow(std::string error_log)
{
	// Log.txtにエラー内容を追加する
	ErrorLogFmtAdd(error_log.c_str());
	return D_FAILURE;
}