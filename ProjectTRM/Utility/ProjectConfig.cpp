#include "ProjectConfig.h"
#include "DxLib.h"

// �f�o�b�N�t���O�̏�����
bool ProjectConfig::DEBUG = false;

// �X�e�[�W�T�C�Y�̏�����
int ProjectConfig::STAGE_WIDTH = 1280;
int ProjectConfig::STAGE_HEIGHT = 720;

// �G���[���e���o�͂���
int ErrorThrow(std::string error_log)
{
	// Log.txt�ɃG���[���e��ǉ�����
	ErrorLogFmtAdd(error_log.c_str());
	return D_FAILURE;
}