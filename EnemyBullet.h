#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
class EnemyBullet
{
public:
	//����
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//isDead��getter
	bool IsDead() const { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLiteTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLiteTime;

	//�f�X�t���O
	bool isDead_ = false;

	//�A�t�B��
	affin::AffinMat affinMat;
};