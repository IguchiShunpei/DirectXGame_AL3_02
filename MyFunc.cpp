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
	//アフィン変換用の行列
	affin::AffinMat affinMat;

	//Translateの情報を入れる
	affin::setScaleMat(affinMat.scale, worldTransform);
	affin::setRotateMat(affinMat, worldTransform);
	affin::setTranslateMat(affinMat.translate, worldTransform);

	//matWorldに単位行列を入れる
	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	//行列の計算
	affin::setTransformationWolrdMat(affinMat, worldTransform);

	//行列の転送
	worldTransform.TransferMatrix();
}

//水平投射 + バウンドの処理
void MyFunc::HorizontalProjection(WorldTransform& worldTransform_, Vector3& startSpeed, float G, float e, float& flame)
{
	Vector3 speed;

	speed.z = startSpeed.z;
	//速度を計算(鉛直投げ上げ)
	speed.y = startSpeed.y - G * (static_cast<float>(flame) / 60.0f);

	//translationにspeedを加算する
	worldTransform_.translation_ += speed;

	//アフィン変換用の行列
	affin::AffinMat affinMat;

	//Translateの情報を入れる
	affin::setTranslateMat(affinMat.translate, worldTransform_);

	//matWorldに単位行列を入れる
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//行列の計算
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();

	//フレームを追加
	flame++;
}