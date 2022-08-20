#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "DebugText.h"
#include "Input.h"
#include "affin/affin.h"
#include "ViewProjection.h"
#include "MyFunc.h"
#include "PlayerBullet.h"

//���L����
class Player
{
public:
	//������
	void Initialize(Model* model,uint32_t textureHandle);

	//�X�V
	void Update();

	//�ړ�
	void Move();

	//��]
	void Rotate();

	//�U��
	void Attack();

	//�`��
	void Draw(ViewProjection viewProjection_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�e
	PlayerBullet* bullet_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
};

