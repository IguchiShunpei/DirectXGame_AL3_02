#include "Player.h"
using namespace MathUtility;

Player::Player(){}

//������
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
    textureHandle_ = textureHandle;
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	winApp_ = WinApp::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = Vector3{ 0,-15,0 };

	modelBullet_ = Model::CreateFromOBJ("bullet", true);

	//3D���[���J�����̂̃��[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();

	//���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle = TextureManager::Load("reticle.png");

	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2(640, 320), Vector4(1.0f,1.0f,1.0f,1.0f), Vector2(0.5f, 0.5f)));
}

//�X�V
void Player::Update(const ViewProjection& viewProjection)
{

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//�ړ�
	Move();
	//��]
	Rotate();

	//�y�A�����g��X�V
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	MyFunc::UpdateWorldTransform(worldTransform_);

	//�U��
	Attack();
	//�e�X�V
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z
	{
		//���@����3D���e�B�N���ւ̋���
		const float kDistancePlayerTo3DReticle = 15.0f;

		//���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
		Vector3 offSet = {0,0,1.0f};

		//���@�̃��[���h�s��̉�]�𔽉f
		offSet = affin::MatVector(offSet, worldTransform_.matWorld_);

		//�x�N�g���̒����𐮂���
		offSet = affin::Normalize(offSet) * kDistancePlayerTo3DReticle;

		//3D���e�B�N���̍��W��ݒ�
		worldTransform3DReticle_.translation_ =
			affin::AddVector3(affin::GetWorldPosition(worldTransform_.matWorld_), offSet);

		//�s��̍X�V�Ɠ]��
		/*affin::generateTransMat (worldTransform3DReticle_);*/
		MyFunc::UpdateWorldTransform(worldTransform3DReticle_);
		worldTransform3DReticle_.TransferMatrix();

	}

	//3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z
	{
		// 3D���e�B�N���̃��[���h�s�񂩂�,���[���h���W���擾
		Vector3 positionReticle = affin::GetWorldPosition(worldTransform3DReticle_.matWorld_);
		//�r���[�|�[�g�s��
		Matrix4 matViewport = affin::SetViewport(worldTransform3DReticle_, winApp_, Vector3(0, 0, 0));

		//�r���[�s��ƃv���W�F�N�V�����s��,�r���[�|�[�g�s�����������
		Matrix4 matViewprojectionViewport =
			viewProjection.matView * viewProjection.matProjection * matViewport;
		//���[���h���X�N���[�����W�ϊ�
		positionReticle = affin::Division(positionReticle, matViewprojectionViewport);
		//���W�ݒ�
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	////���@�̍��W
	//debugText_->SetPos(50, 50);
	//debugText_->Printf("%f,%f,%f",
	//	worldTransform_.translation_.x,
	//	worldTransform_.translation_.y,
	//	worldTransform_.translation_.z);

	////���@�̊p�x
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("%f,%f,%f",
	//	worldTransform_.rotation_.x,
	//	worldTransform_.rotation_.y,
	//	worldTransform_.rotation_.z);

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

	/*if (input_->PushKey(DIK_UP))
	{
		move.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y = -kCharacterSpeed;
	}*/

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
	if (input_->PushKey(DIK_A))
	{
		rotate.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		rotate.y = -kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W))
	{
		rotate.x = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		rotate.x = -kCharacterSpeed;
	}


	//�ړ����E���W
	const float kRotateLimitY = 1.0f;
	const float kRotateLimitX = 1.0f;

	//rotation��rotate�����Z����
	worldTransform_.rotation_ += rotate;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -kRotateLimitY);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +kRotateLimitY);
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x, -kRotateLimitX);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x, +kRotateLimitX);
}

//�`��
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

//UI�`��
void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

//�e�̔���
void Player::Attack() 
{
		dalayTimer -= 0.1f;

		//���L�����̍��W���R�s�[
		Vector3 position = affin::GetWorldPosition(worldTransform_.matWorld_);

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//���@����W���I�u�W�F�N�g�ւ̃x�N�g��
		velocity = affin::GetWorldPosition(worldTransform3DReticle_.matWorld_) -
			affin::GetWorldPosition(worldTransform_.matWorld_);

		velocity = affin::Normalize(velocity) * kBulletSpeed;

		//�N�[���^�C�����O�ɂȂ����Ƃ�
		if (dalayTimer <= 0)
		{
			//���̐���
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//���̏�����
			newBullet->Initialize(modelBullet_, position, velocity);

			//���̓o�^
			bullets_.push_back(std::move(newBullet));

			dalayTimer = 1.0f;
		}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) 
{

	Vector3 result = { 0,0,0 };

	//����
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}

//���[���h���W���擾
Vector3 Player::GetWorldPosition()
{
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{

}

//���a��Ԃ��֐�
float Player::GetRadius()
{
	return radius;
}

void Player::SetParent(WorldTransform* worldTransform)
{
	worldTransform_.parent_ = worldTransform;
}