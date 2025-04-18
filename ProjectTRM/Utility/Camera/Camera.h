#pragma once

#include "../Singleton.h"
#include "../Vector2D.h"

class Camera : public Singleton<Camera>
{
private:
	Vector2D location;	// �J�������W

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
};

