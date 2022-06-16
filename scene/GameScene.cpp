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
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読みこむ
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//カメラ視点座標を設定
	viewProjection_.eye = {50.0f,50.0f,-50.0f};

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


	//x,y,z方向のスケーリングを設定
	worldTransform_.scale_ = { 10.0f,10.0f,10.0f };

	//x,y,z軸周りの回転角を設定
	worldTransform_.rotation_ = { MyFunc::Deg2Rad(0.0f),MyFunc::Deg2Rad(0.0f),MyFunc::Deg2Rad(0.0f) };

	//X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = { -60.0f,30.0f,0.0f };

	affin::AffinMat affinMat;

	///////////スケーリング ここから///////////////// 

	affin::setScaleMat(affinMat.scale, worldTransform_);

	///////////スケーリング ここまで/////////////////

	///////////////回転 ここから///////////////////

	affin::setRotateMat(affinMat, worldTransform_);

	///////////////回転 ここまで///////////////////

	///////////////平行 ここから///////////////////

	affin::setTranslateMat(affinMat.translate, worldTransform_);

	///////////////平行 ここまで///////////////////

	//合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}

void GameScene::Update()
{
	////デバッグカメラの更新
	//debugCamera_->Update();

	MyFunc::HorizontalProjection(worldTransform_, startSpeed, G, e, flame);

	debugText_->SetPos(50, 50);
	debugText_->Printf("translation:%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
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

	//3Dモデル描画
		model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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

