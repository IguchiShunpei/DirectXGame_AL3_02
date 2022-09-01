#include "Player.h"
using namespace MathUtility;

Player::Player(){}

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
    textureHandle_ = textureHandle;
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	winApp_ = WinApp::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = Vector3{ 0,-15,0 };

	modelBullet_ = Model::CreateFromOBJ("bullet", true);

	//3Dレールカメラののワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");

	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2(640, 320), Vector4(1.0f,1.0f,1.0f,1.0f), Vector2(0.5f, 0.5f)));
}

//更新
void Player::Update(const ViewProjection& viewProjection)
{

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//移動
	Move();
	//回転
	Rotate();

	//ペアレント先更新
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	MyFunc::UpdateWorldTransform(worldTransform_);

	//攻撃
	Attack();
	//弾更新
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 15.0f;

		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offSet = {0,0,1.0f};

		//自機のワールド行列の回転を反映
		offSet = affin::MatVector(offSet, worldTransform_.matWorld_);

		//ベクトルの長さを整える
		offSet = affin::Normalize(offSet) * kDistancePlayerTo3DReticle;

		//3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ =
			affin::AddVector3(affin::GetWorldPosition(worldTransform_.matWorld_), offSet);

		//行列の更新と転送
		/*affin::generateTransMat (worldTransform3DReticle_);*/
		MyFunc::UpdateWorldTransform(worldTransform3DReticle_);
		worldTransform3DReticle_.TransferMatrix();

	}

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		// 3Dレティクルのワールド行列から,ワールド座標を取得
		Vector3 positionReticle = affin::GetWorldPosition(worldTransform3DReticle_.matWorld_);
		//ビューポート行列
		Matrix4 matViewport = affin::SetViewport(worldTransform3DReticle_, winApp_, Vector3(0, 0, 0));

		//ビュー行列とプロジェクション行列,ビューポート行列を合成する
		Matrix4 matViewprojectionViewport =
			viewProjection.matView * viewProjection.matProjection * matViewport;
		//ワールド→スクリーン座標変換
		positionReticle = affin::Division(positionReticle, matViewprojectionViewport);
		//座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	////自機の座標
	//debugText_->SetPos(50, 50);
	//debugText_->Printf("%f,%f,%f",
	//	worldTransform_.translation_.x,
	//	worldTransform_.translation_.y,
	//	worldTransform_.translation_.z);

	////自機の角度
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("%f,%f,%f",
	//	worldTransform_.rotation_.x,
	//	worldTransform_.rotation_.y,
	//	worldTransform_.rotation_.z);

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

	/*if (input_->PushKey(DIK_UP))
	{
		move.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.y = -kCharacterSpeed;
	}*/

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
	if (input_->PushKey(DIK_A))
	{
		rotate.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		rotate.y = -kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W))
	{
		rotate.x = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		rotate.x = -kCharacterSpeed;
	}


	//移動限界座標
	const float kRotateLimitY = 1.0f;
	const float kRotateLimitX = 1.0f;

	//rotationにrotateを加算する
	worldTransform_.rotation_ += rotate;

	//範囲を超えない処理
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -kRotateLimitY);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +kRotateLimitY);
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x, -kRotateLimitX);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x, +kRotateLimitX);
}

//描画
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

//UI描画
void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

//弾の発射
void Player::Attack() 
{
		dalayTimer -= 0.1f;

		//自キャラの座標をコピー
		Vector3 position = affin::GetWorldPosition(worldTransform_.matWorld_);

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//自機から標準オブジェクトへのベクトル
		velocity = affin::GetWorldPosition(worldTransform3DReticle_.matWorld_) -
			affin::GetWorldPosition(worldTransform_.matWorld_);

		velocity = affin::Normalize(velocity) * kBulletSpeed;

		//クールタイムが０になったとき
		if (dalayTimer <= 0)
		{
			//球の生成
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//球の初期化
			newBullet->Initialize(modelBullet_, position, velocity);

			//球の登録
			bullets_.push_back(std::move(newBullet));

			dalayTimer = 1.0f;
		}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) 
{

	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}

//ワールド座標を取得
Vector3 Player::GetWorldPosition()
{
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{

}

//半径を返す関数
float Player::GetRadius()
{
	return radius;
}

void Player::SetParent(WorldTransform* worldTransform)
{
	worldTransform_.parent_ = worldTransform;
}