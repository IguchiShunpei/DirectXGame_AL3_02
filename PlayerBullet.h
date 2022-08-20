#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

class PlayerBullet
{
public:
	//�R���X�g���N�^
	PlayerBullet();

	//�f�X�g���N�^
	~PlayerBullet();

	//�����o�֐�
		//������
	void Initialize(Model* model, const Vector3& position);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw(const ViewProjection& viewprojection);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

};

