#include "ProjectConfig.h"
#include "DxLib.h"

// �f�o�b�N�t���O�̏�����
bool ProjectConfig::DEBUG = false;

// �G���[���e���o�͂���
int ErrorThrow(std::string error_log)
{
	// Log.txt�ɃG���[���e��ǉ�����
	ErrorLogFmtAdd(error_log.c_str());
	return D_FAILURE;
}