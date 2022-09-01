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
#include<memory>
#include<list>
#include "WinApp.h"

//���L����
class Player
{
public:
	Player();
	//������
	void Initialize(Model* model,uint32_t textureHandle);

	//�X�V
	void Update(const ViewProjection& viewProjection);

	//�ړ�
	void Move();

	//��]
	void Rotate();

	//�U��
	void Attack();

	//�`��
	void Draw(ViewProjection viewProjection_);

	void DrawUI();

	//�L�����̌����ɉ����������ɋ����o��
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//���a��Ԃ��֐�
	float GetRadius();

	//�e�q�\���̃Z�b�^�[
	void SetParent(WorldTransform* worldTransform);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	//���f��
	Model* model_ = nullptr;

	Model* modelBullet_ = nullptr;
	//�C���v�b�g
	Input* input_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

	//�ł��o���܂ł̎���
	float dalayTimer = 0.5f;

	//Affin�֐��̍\����
	affin::AffinMat affinMat;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	DebugText* debugText_ = nullptr;
	WinApp* winApp_ = nullptr;

	//���a
	float radius = 1.0f;
};