#pragma once

#include <string>

/**********************************************************
* �}�N����`
***********************************************************/
#define	D_SUCCESS		(0)		// ����
#define	D_FAILURE		(-1)	// ���s
#define D_WIN_MAX_X		(1280)	// �X�N���[���T�C�Y�i���j
#define D_WIN_MAX_Y		(720)	// �X�N���[���T�C�Y�i�����j
#define D_COLOR_BIT		(32)	// �J���[�r�b�g

#define STAGE_WIDTH		(D_WIN_MAX_X)	// �X�e�[�W�̉���
#define STAGE_HEIGHT	(D_WIN_MAX_Y)	// �X�e�[�W�̍���


/**********************************************************
* �v���g�^�C�v�錾
***********************************************************/
// �G���[���e���o�͂���
int ErrorThrow(std::string error_log);

// �v���W�F�N�g�ݒ�N���X
class ProjectConfig
{
public:
	static bool DEBUG;	// �f�o�b�N���[�h�̐؂�ւ��p�ϐ�
};