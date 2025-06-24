#pragma once

#include "../../Utility/Singleton.h"

class Pause:public Singleton<Pause>
{
private:
	bool is_pause;	// �|�[�Y�t���O

public:
	// �R���X�g���N�^
	Pause();
	// �f�X�g���N�^
	~Pause();

public:
	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">�P�t���[��������̎���</param>
	void Update(float delta_second);
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() const;

public:
	/// <summary>
	/// �|�[�Y�t���O�ݒ菈��
	/// </summary>
	/// <param name="flag">true:�|�[�Y���Afalse:�|�[�Y����</param>
	void SetPause(bool flag);
	/// <summary>
	/// �|�[�Y�t���O�擾����
	/// </summary>
	/// <returns>�|�[�Y�t���O</returns>
	bool GetPause();
};

