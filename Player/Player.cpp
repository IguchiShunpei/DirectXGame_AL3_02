#include "Player.h"

//������
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	this->textureHandle_ = textureHandle;
	this->model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

//�X�V
void Player::Update()
{
	//�ړ�
	Move();
	//��]
	Rotate();
	MyFunc::UpdateWorldTransform(worldTransform_);

	//�U��
	Attack();
	//�e�X�V
	if (bullet_)
	{
		bullet_->Update();
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z);
}

//�ړ�
void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��ʂ�ω�
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		move.x = -kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP))
	{
		move.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y = -kCharacterSpeed;
	}

	//�ړ����E���W
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 20.0f;

	//transLation��move�����Z����
	worldTransform_.translation_ += move;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
}

void Player::Rotate()
{
	//�L�����N�^�[�̉�]�x�N�g��
	Vector3 rotate = { 0,0,0 };

	const float kCharacterSpeed = 0.02f;

	//�����������ňړ��ʂ�ω�
	if (input_->PushKey(DIK_U))
	{
		rotate.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_I))
	{
		rotate.y = -kCharacterSpeed;
	}

	//�ړ����E���W
	const float kRotateLimitX = 0.5f;

	//rotation��rotate�����Z����
	worldTransform_.rotation_ += rotate;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -kRotateLimitX);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +kRotateLimitX);

}

//�`��
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	if (bullet_)
	{
		bullet_->Draw(viewProjection);
	}
}

//�e�̔���
void Player::Attack() 
{

	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�𐶐���������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//�e��o�^����
		bullet_ = newBullet;

	}
}