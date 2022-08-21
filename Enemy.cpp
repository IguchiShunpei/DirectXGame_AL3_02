#include "Enemy.h"
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = { 0,5,50 };

}

void Enemy::Update()
{
	//�P�ʍs���ݒ�
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	switch(phase_)
	{
	case Phase::Approach: //�ڋ߃t�F�[�Y
	default:
		//�ړ�(�x�N�g�������Z)
		Approach();
		break;
	case Phase::Leave:   //���E�t�F�[�Y
		Leave();
		break;
	
	}
	//�s��̌v�Z
	affinMat.translate = affin::generateTransMat(worldTransform_);
	
	//�s��̍���
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
	
	//�f�o�b�N�e�L�X�g
	debugText_->SetPos(50, 90);
	debugText_->Printf("%f,%f,%f", 
		worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach()
{
	worldTransform_.translation_ += approach_;
	//����̈ʒu�ɒ������痣�E��
	if (worldTransform_.translation_.z <= 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	worldTransform_.translation_ += leave_;
}