#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"



GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読みこむ
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyHandle_ = TextureManager::Load("texture.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵キャラの生成
	enemy_ = new Enemy();

	//敵キャラの初期化
	enemy_->Initialize(model_, enemyHandle_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//天球の生成
	skydome_ = new skydome();

	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update()
{
	player_->Update();
	enemy_->Update();
	skydome_->Update();
	isDebugCameraActive_ = true;


	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else
	{
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}

	//当たり判定
	CheckAllCollisions();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵キャラの描画
	enemy_->Draw(viewProjection_);

	//天球の描画
	skydome_->Draw(debugCamera_->GetViewProjection());

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを参照する(アドレス渡し)
	/*PrimitiveDrawer::GetInstance()->DrawLine3d()*/

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//それぞれの半径
	float pRadius = player_->GetRadius();
	float eRadius = enemy_->GetRadius();

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		posB = bullet->GetWorldPosition();
		float ebRadius = bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//弾と弾の交差判定
		if (
			((posB.x - posA.x) * (posB.x - posA.x)) +
			((posB.y - posA.y) * (posB.y - posA.y)) +
			((posB.z - posA.z) * (posB.z - posA.z)) <=
			((pRadius + ebRadius) * (pRadius + ebRadius)))
		{
			//コールバックを呼び出す
			player_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標
	posA = enemy_->GetWorldPosition();
	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		float pbRadius = bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//球と球
		if (
			((posB.x - posA.x) * (posB.x - posA.x)) +
			((posB.y - posA.y) * (posB.y - posA.y)) +
			((posB.z - posA.z) * (posB.z - posA.z)) <=
			((eRadius + pbRadius) * (eRadius + pbRadius))) {
			//コールバックを呼び出す
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	//自弾の座標
	for (const std::unique_ptr<PlayerBullet>& pBullet : playerBullets) {
		posA = pBullet->GetWorldPosition();
		float pbRadius = pBullet->GetRadius();
		//敵弾の座標
		for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets) {
			posB = eBullet->GetWorldPosition();
			float ebRadius = eBullet->GetRadius();
			//球と球
			if (
				((posB.x - posA.x) * (posB.x - posA.x)) +
				((posB.y - posA.y) * (posB.y - posA.y)) +
				((posB.z - posA.z) * (posB.z - posA.z)) <=
				((ebRadius + pbRadius) * (ebRadius + pbRadius)))
			{
				//コールバックを呼び出す
				pBullet->OnCollision();
				eBullet->OnCollision();
			}
		}
	}

#pragma endregion
}

