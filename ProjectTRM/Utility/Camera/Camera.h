#pragma once

#include "../Singleton.h"
#include "../Vector2D.h"
#include "../../Objects/Character/Player/Oracle/Oracle.h"

class Camera : public Singleton<Camera>
{
private:
	Vector2D location;	// �J�������W
	Vector2D size;		// �X�N���[���T�C�Y
	Oracle* oracle;		// �ޏ��̃|�C���^
	bool tracking_flag;	// �ǐՃt���O
	float zoom = 1.0f;	// �Y�[���{��

	// �Y�[���{���͈̔�
	const float ZOOM_MIN = 1.0f;
	const float ZOOM_MAX = 2.0f;

public:
	// �R���X�g���N�^
	Camera();
	// �f�X�g���N�^
	~Camera();

public:
	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	/// <summary>
	///  ���s����
	/// </summary>
	void Update();

	/// <summary>
	/// �X�N���[���`��
	/// </summary>
	/// <param name="back_buffer">�\��������</param>
	void Draw(int back_buffer);

	/// <summary>
	/// �Y�[������
	/// </summary>
	void Zoom();

	/// <summary>
	/// �X�N���[������
	/// </summary>
	void Scroll();

	/// <summary>
	///	�J�������W�w�菈��
	/// </summary>
	/// <param name="location">�w����W</param>
	void SetCameraPos(Vector2D location);

	/// <summary>
	/// �ޏ����ݒ菈��
	/// </summary>
	/// <param name="oracle"></param>
	void SetPlayer(Oracle* oracle);

	/// <summary>
	/// �J�����g�嗦�w�菈��
	/// </summary>
	/// <param name="zoom"></param>
	void SetZoom(float zoom);

	/// <summary>
	/// �J�����g�嗦�擾����
	/// </summary>
	/// <returns></returns>
	float GetZoom();
};

