#include "PlayerBullet.h"
#include <cassert>
#include "MyFunc.h"

//�R���X�g���N�^
PlayerBullet::PlayerBullet() {

}

//�f�X�g���N�^
PlayerBullet::~PlayerBullet() {

}

//�����o�֐�
//������
void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	//null�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

}

//�X�V����
void PlayerBullet::Update() {

	MyFunc::UpdateWorldTransform(worldTransform_);

}

//�`�揈��
void PlayerBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textureHandle_);

}