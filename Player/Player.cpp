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
	//移動
	Move();
	//回転
	Rotate();
	MyFunc::UpdateWorldTransform(worldTransform_);

	//攻撃
	Attack();
	//弾更新
	if (bullet_)
	{
		bullet_->Update();
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z);
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
	
}

void Player::Rotate()
{
	//キャラクターの回転ベクトル
	Vector3 rotate = { 0,0,0 };

	const float kCharacterSpeed = 0.02f;

	//押した方向で移動量を変化
	if (input_->PushKey(DIK_U))
	{
		rotate.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_I))
	{
		rotate.y = -kCharacterSpeed;
	}

	//移動限界座標
	const float kRotateLimitX = 0.5f;

	//rotationにrotateを加算する
	worldTransform_.rotation_ += rotate;

	//範囲を超えない処理
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -kRotateLimitX);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +kRotateLimitX);

}

//描画
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	if (bullet_)
	{
		bullet_->Draw(viewProjection);
	}
}

//弾の発射
void Player::Attack() 
{

	if (input_->TriggerKey(DIK_SPACE)) {
		//弾を生成し初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullet_ = newBullet;

	}
}