#include "Pause.h"
#include "Dxlib.h"

// �R���X�g���N�^
Pause::Pause():
	is_pause(false)
{
	
}

// �f�X�g���N�^
Pause::~Pause()
{

}

// ����������
void Pause::Initialize()
{

}

// �X�V����
void Pause::Update(float delta_second)
{

}

// �`�揈��
void Pause::Draw() const
{
	SetFontSize(240);
	DrawFormatString(350, 250, 0xffffff, "PAUSE");
	SetFontSize(32);
}

// �|�[�Y�t���O�ݒ菈��
void Pause::SetPause(bool flag)
{
	this->is_pause = flag;
}

// �|�[�Y�t���O�擾����
bool Pause::GetPause()
{
	return is_pause;
}
