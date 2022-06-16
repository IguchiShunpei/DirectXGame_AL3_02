#include "MyFunc.h"

float MyFunc::Deg2Rad(float x)
{
	const float M_PI = 3.141592f;
	return x * M_PI / 180;
}

float MyFunc::Rad2Deg(float x)
{
	const float M_PI = 3.141592f;
	return x / (M_PI / 180);
}

void MyFunc::UpdateWorldTransform(WorldTransform& worldTransform)
{
	//�A�t�B���ϊ��p�̍s��
	affin::AffinMat affinMat;

	//Translate�̏�������
	affin::setScaleMat(affinMat.scale, worldTransform);
	affin::setRotateMat(affinMat, worldTransform);
	affin::setTranslateMat(affinMat.translate, worldTransform);

	//matWorld�ɒP�ʍs�������
	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	//�s��̌v�Z
	affin::setTransformationWolrdMat(affinMat, worldTransform);
	worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;

	//�s��̓]��
	worldTransform.TransferMatrix();
}

//�������� + �o�E���h�̏���
void MyFunc::HorizontalProjection(WorldTransform& worldTransform_,Vector3 &startSpeed, float G, float e,float &flame)
{
	Vector3 speed;

	speed.x = startSpeed.x;
	//���x���v�Z(���������グ)
	speed.y = startSpeed.y - G * (static_cast<float>(flame) / 60.0f);

	//�ړ��̌��E
	const float kMoveLimitX = 100.0f;
	const float kMoveLimitY = 40.0f;

	//translation��speed�����Z����
	worldTransform_.translation_ += speed;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�A�t�B���ϊ��p�̍s��
	affin::AffinMat affinMat;

	//Translate�̏�������
	affin::setTranslateMat(affinMat.translate, worldTransform_);

	//matWorld�ɒP�ʍs�������
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//�s��̌v�Z
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();

	//�n�ʂɂԂ������璵�˂�
	if (worldTransform_.translation_.y <= -kMoveLimitY)
	{
		//�����xy�ɔ��]�������x�Ɣ����W�������������̂���
		startSpeed.y = speed.y * -1 * e;

		//�t���[����0�ɂ���
		flame = 0;

		//�����߂�
		worldTransform_.translation_.y = -kMoveLimitY;
	}

	//�t���[����ǉ�
	flame++;
}