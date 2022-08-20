#include "PlayerBullet.h"
#include <cassert>
#include "MyFunc.h"

//コンストラクタ
PlayerBullet::PlayerBullet() {

}

//デストラクタ
PlayerBullet::~PlayerBullet() {

}

//メンバ関数
//初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	//nullポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

}

//更新処理
void PlayerBullet::Update() {

	MyFunc::UpdateWorldTransform(worldTransform_);

}

//描画処理
void PlayerBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textureHandle_);

}