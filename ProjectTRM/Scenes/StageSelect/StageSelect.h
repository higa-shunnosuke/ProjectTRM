#pragma once

#include "..//SceneBase.h"


#define Set_StageX  D_WIN_MAX_X + 200
#define FirstStage 1
#define SecondStage 2
#define ThardStage 3
//�Ō�̃X�e�[�W
#define FinalStage ThardStage

#define Centher D_WIN_MAX_X/2

enum Stage : int
{
	DEFAULT,
	LMOVE,
	RMOVE,
	END
};

// �^�C�g���V�[��
class StageSelect : public SceneBase
{
private:
	Stage State = Stage::DEFAULT;
	int Stage_Image[3];
	int BackGroued_Image;
	int SerectStage = 1;

	int x = 200;
	int y = 100;
	const TCHAR* StageText[3] = { "Stage 1 : �n�܂�̐X","Stage 2 :�@���J�̈��","Stage 3 :�@����̋�", };
	const TCHAR* StageFlabarText[3] = { "�n�܂�̐X\n�ł̌R���̐ˌ󂪏P���|����E�E�E" , "�O�l�����̈��\n�Ñ���ł̖{��������!" , "�ŏI����̋�\n�z��͑S�͂��o���Ă��邼�I������I" };
	const TCHAR* StageSelectText[3] = {"1  ->" , "<-  2  ->" , "<-  3"};
	
	int w = x + 100;
	int h = y + 100;


	int ChangeX = Set_StageX;
	int BGM;
	int DecisionSE;
	int CursorMoveSE;
public:
	// �R���X�g���N�^
	StageSelect();
	// �f�X�g���N�^
	virtual ~StageSelect();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	/// <returns>���̃V�[���^�C�v���</returns>
	virtual eSceneType Update(const float& delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() const override;

	/// <summary>
	/// �I��������
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ���݂̃V�[���^�C�v�����擾����
	/// </summary>
	/// <returns>���݂̃V�[���^�C�v</returns>
	virtual const eSceneType GetNowSceneType() const override;
};

