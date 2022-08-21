#include "Enemy.h"
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = { 5,5,50 };

}

void Enemy::Update()
{
	//単位行列を設定
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	switch(phase_)
	{
	case Phase::Approach: //接近フェーズ
	default:
		//移動(ベクトルを加算)
		Approach();
		break;
	case Phase::Leave:   //離脱フェーズ
		Leave();
		break;
	
	}
	//行列の計算
	affinMat.translate = affin::generateTransMat(worldTransform_);
	
	//行列の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	
	//弾を発射
	Fire();

	//弾の更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//デバックテキスト
	debugText_->SetPos(50, 90);
	debugText_->Printf("%f,%f,%f", 
		worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//球の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	worldTransform_.translation_ += approach_;

	//既定の位置に着いたら初期位置に戻す
	if (worldTransform_.translation_.z <= -40.0f)
	{
		worldTransform_.translation_.z = 50.0f;
	}

	//既定の位置に着いたら離脱へ
	/*if (worldTransform_.translation_.z <= 0.0f)
	{
		phase_ = Phase::Leave;
	}*/
}

void Enemy::Leave()
{
	worldTransform_.translation_ += leave_;
}

void Enemy::Fire()
{
	delayTimer -= 0.1f;

	//球の速度
	const float kBulletSpeed = 0.5f;

	//自機狙い弾
	assert(player_);

	//プレイヤーのワールド座標の取得
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition();
	//敵のワールド座標の取得
	Vector3 enemyPosition;
	enemyPosition = GetWorldPosition();

	Vector3 velocity(0, 0, 0);

	//差分ベクトルを求める
	velocity = enemyPosition - playerPosition;

	//長さを求める
	Vector3Length(velocity);
	//正規化
	Vector3Normalize(velocity);
	//ベクトルの長さを,速さに合わせる
	velocity *= kBulletSpeed;//これが速度になる

	//クールタイムが０になったとき
	if (delayTimer <= 0) {
		//球の生成
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//球の初期化
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//球の登録
		bullets_.push_back(std::move(newBullet));

		delayTimer = 10.0f;
	}
}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}