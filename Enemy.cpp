#include "Enemy.h"
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = { 5,5,50 };

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
	
	//�e�𔭎�
	Fire();

	//�e�̍X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

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

	//���̕`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	worldTransform_.translation_ += approach_;

	//����̈ʒu�ɒ������珉���ʒu�ɖ߂�
	if (worldTransform_.translation_.z <= -40.0f)
	{
		worldTransform_.translation_.z = 50.0f;
	}

	//����̈ʒu�ɒ������痣�E��
	/*if (worldTransform_.translation_.z <= 0.0f)
	{
		phase_ = Phase::Leave;
	}*/
}

void Enemy::Leave()
{
	worldTransform_.translation_ += leave_;
}

void Enemy::Fire()
{
	delayTimer -= 0.1f;

	//���̑��x
	const float kBulletSpeed = 0.5f;

	//���@�_���e
	assert(player_);

	//�v���C���[�̃��[���h���W�̎擾
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition();
	//�G�̃��[���h���W�̎擾
	Vector3 enemyPosition;
	enemyPosition = GetWorldPosition();

	Vector3 velocity(0, 0, 0);

	//�����x�N�g�������߂�
	velocity = enemyPosition - playerPosition;

	//���������߂�
	Vector3Length(velocity);
	//���K��
	Vector3Normalize(velocity);
	//�x�N�g���̒�����,�����ɍ��킹��
	velocity *= kBulletSpeed;//���ꂪ���x�ɂȂ�

	//�N�[���^�C�����O�ɂȂ����Ƃ�
	if (delayTimer <= 0) {
		//���̐���
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//���̏�����
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//���̓o�^
		bullets_.push_back(std::move(newBullet));

		delayTimer = 10.0f;
	}
}

Vector3 Enemy::GetWorldPosition() {
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}