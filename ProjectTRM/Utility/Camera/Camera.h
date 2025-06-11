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
	/// �J�������W�擾����
	/// </summary>
	/// <returns>�J�������W</returns>
	Vector2D GetCameraPos();

	/// <summary>
	///	�J�������W�w�菈��
	/// </summary>
	/// <param name="location">�w����W</param>
	void SetCameraPos(Vector2D location);

	/// <summary>
	/// �X�N���[���T�C�Y�擾����
	/// </summary>
	/// <returns>�X�N���[���T�C�Y</returns>
	Vector2D GetScreeenSize();

	/// <summary>
	/// �ޏ����ݒ菈��
	/// </summary>
	/// <param name="oracle"></param>
	void SetPlayer(Oracle* oracle);
};

