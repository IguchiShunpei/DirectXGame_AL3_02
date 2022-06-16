#include "Player.h"

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	this->textureHandle_ = textureHandle;
	this->model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

//更新
void Player::Update()
{
	Move();
}

//移動
void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float kCharacterSpeed = 0.2f;

	//押した方向で移動量を変化
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		move.x = -kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP))
	{
		move.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y = -kCharacterSpeed;
	}

	//移動限界座標
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 20.0f;

	//transLationにmoveを加算する
	worldTransform_.translation_ += move;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
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

	debugText_->SetPos(50, 50);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

//描画
void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}