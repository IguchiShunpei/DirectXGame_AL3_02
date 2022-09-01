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

	//�s��̓]��
	worldTransform.TransferMatrix();
}

//�������� + �o�E���h�̏���
void MyFunc::HorizontalProjection(WorldTransform& worldTransform_, Vector3& startSpeed, float G, float e, float& flame)
{
	Vector3 speed;

	speed.z = startSpeed.z;
	//���x���v�Z(���������グ)
	speed.y = startSpeed.y - G * (static_cast<float>(flame) / 60.0f);

	//translation��speed�����Z����
	worldTransform_.translation_ += speed;

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

	//�t���[����ǉ�
	flame++;
}