#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <memory>
#include <list>
#include "EnemyBullet.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

//���@�N���X�̑O���錾
class Player;

class Enemy
{

	enum class Phase
	{
		None,
		Approach,//�ڋ�
		Leave,   //���E
	};

public:
	/// ����
	void Initialize(Model* model, uint32_t textureHandle);

	/// �X�V
	void Update();

	/// �`��
	void Draw(ViewProjection viewProjection);

	//�ڋ�
	void Approach();

	//���E
	void Leave();

	//�e����
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	
	DebugText* debugText_ = nullptr;

	//���x
	Vector3 velocity_ = { 0.0f,0.0f,0.1f };

	//�ڋߑ��x
	Vector3 approach_ = {0.0f,0.0f,-0.1f};

	//���E���x
	Vector3 leave_ = {-0.15f,0.1f,0.1f};

	// Affin�֐��̍\����
	affin::AffinMat affinMat;

	//�ł��o���܂ł̎���
	float delayTimer = 0.0f;

	//�G�̍s���p�^�[��
	Phase phase_ = Phase::None;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//���L����
	Player* player_ = nullptr;

};

