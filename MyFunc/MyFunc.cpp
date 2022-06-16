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
	worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;

	//行列の転送
	worldTransform.TransferMatrix();
}

//水平投射 + バウンドの処理
void MyFunc::HorizontalProjection(WorldTransform& worldTransform_,Vector3 &startSpeed, float G, float e,float &flame)
{
	Vector3 speed;

	speed.x = startSpeed.x;
	//速度を計算(鉛直投げ上げ)
	speed.y = startSpeed.y - G * (static_cast<float>(flame) / 60.0f);

	//移動の限界
	const float kMoveLimitX = 100.0f;
	const float kMoveLimitY = 40.0f;

	//translationにspeedを加算する
	worldTransform_.translation_ += speed;

	//範囲を超えない処理
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

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

	//地面にぶつかったら跳ねる
	if (worldTransform_.translation_.y <= -kMoveLimitY)
	{
		//初速度yに反転した速度と反発係数をかけたものを代入
		startSpeed.y = speed.y * -1 * e;

		//フレームを0にする
		flame = 0;

		//押し戻し
		worldTransform_.translation_.y = -kMoveLimitY;
	}

	//フレームを追加
	flame++;
}