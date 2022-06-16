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
}

void GameScene::Initialize() {

	//乱数シード生成器
	std::random_device seed_gen;

	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//乱数範囲の設定

	//角度
	std::uniform_real_distribution<float> rota(0, M_PI);
	//座標
	std::uniform_real_distribution<float> trans(-10, 10);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読みこむ
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE))
	{
		isDebugCameraActive_ = tlue;
	}
#endif
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

	player_->Update();
}

void GameScene::Update()
{
	////デバッグカメラの更新
	//debugCamera_->Update();

	////視点移動処理
	//{
	//	//視点の移動ベクトル
	//	Vector3 move = { 0.0f, 0.0f,0.0f };

	//	//視点の移動速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W))
	//	{
	//		move.z += kEyeSpeed;
	//	}
	//	else if (input_->PushKey(DIK_S))
	//	{
	//		move.z -= kEyeSpeed;
	//	}

	//	//視点移動(ベクトルの加算)
	//	viewProjection_.eye += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//debugText_->SetPos(50, 50);
	//debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}

	////注視点移動処理
	//{
	//	//注視点の移動ベクトル
	//	Vector3 move = { 0.0f, 0.0f,0.0f };

	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT))
	//	{
	//		move.x -= kTargetSpeed;
	//	}
	//	else if (input_->PushKey(DIK_RIGHT))
	//	{
	//		move.x += kTargetSpeed;
	//	}

	//	//視点移動(ベクトルの加算)
	//	viewProjection_.target += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//}

	////上方向回転処理
	//{
	//	//上方向の回転速さ[ラジアン/flame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE))
	//	{
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, M_PI * 2.0f);
	//	}

	//	//上方向ベクトルを計算(半径1の円周上の座標)
	//	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//debugText_->SetPos(50, 90);
	//debugText_->Printf("up:(%f,%f,%f,)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	////}

	////FoV変更処理
	//{
	//	//↑キーで視野角が広がる
	//	if (input_->PushKey(DIK_UP))
	//	{
	//		viewProjection_.fovAngleY += 0.01f;
	//		if (viewProjection_.fovAngleY >= M_PI)
	//		{
	//			viewProjection_.fovAngleY = M_PI;
	//		}
	//		
	//	}

	//	//↓キーで視野角が広がる
	//	else if (input_->PushKey(DIK_DOWN))
	//	{
	//		viewProjection_.fovAngleY -= 0.01f;
	//		if (viewProjection_.fovAngleY <= 0.01f)
	//		{
	//			viewProjection_.fovAngleY = 0.01f;
	//		}
	//	}

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 110);
	//	debugText_->Printf("fovAngle(Degree):%f", Deg(viewProjection_.fovAngleY));
	//}

	//クリップ距離変更処理
	{
		//上下キーでニアクリップ距離を増減
		if (input_->PushKey(DIK_UP))
		{
			viewProjection_.nearZ += 0.1f;
		}
		else if (input_->PushKey(DIK_DOWN))
		{
			viewProjection_.nearZ -= 0.1f;
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	}
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