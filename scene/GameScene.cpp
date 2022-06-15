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


	//ワールドトランスフォームの初期化

	//親(大元)
	worldTransforms_[PartId::kRoot].Initialize();

	//子(脊椎)
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0.0f,0.0f,0.0f };

	//子(胸)
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0.0f,0.0f,0.0f };

	//子(頭)
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0.0f,2.5f,0.0f };

	//子(左腕)
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].translation_ = { 2.5f,0.0f,0.0f };

	//子(右腕)
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].translation_ = { -2.5f,0.0f,0.0f };

	//子(尻)
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].translation_ = { 0.0f,-2.5f,0.0f };

	//子(左足)
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].translation_ = { 2.5f,-3.0f,0.0f };

	//子(右足)
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].translation_ = { -2.5f,-3.0f,0.0f };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

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
	////デバッグカメラの更新
	//debugCamera_->Update();

	//キャラクターの移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0.0f,0.0f,0.0f };

		const float kCharacterSpeed = 0.2f;

		//押した方向で移動量を変化
		if (input_->PushKey(DIK_RIGHT))
		{
			move.x += kCharacterSpeed;
		}
		else if (input_->PushKey(DIK_LEFT))
		{
			move.x -= kCharacterSpeed;
		}

		//[0]のtransLationにmoveを加算する
		worldTransforms_[PartId::kRoot].translation_ += move;

		//アフィン変換用の行列
		affin::AffinMat affinMat;

		//Translateの情報を入れる
		affin::setTranslateMat(affinMat.translate, worldTransforms_[PartId::kRoot]);

		//matWorldに単位行列を入れる
		worldTransforms_[PartId::kRoot].matWorld_ = MathUtility::Matrix4Identity();

		//行列の計算
		affin::setTransformationWolrdMat(affinMat, worldTransforms_[PartId::kRoot]);

		//行列の転送
		worldTransforms_[PartId::kRoot].TransferMatrix();

		debugText_->SetPos(50, 150);
		debugText_->Printf("translation:%f", worldTransforms_[PartId::kRoot].translation_);
	
		//上半身回転処理
		{
			//押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_U))
			{
				worldTransforms_[PartId::kChest].rotation_.y += 0.1;
			}
			else if (input_->PushKey(DIK_I))
			{
				worldTransforms_[PartId::kChest].rotation_.y -= 0.1;
			}
		}

		//上半身回転処理
		{
			//押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_J))
			{
				worldTransforms_[PartId::kHip].rotation_.y += 0.1;
			}
			else if (input_->PushKey(DIK_K))
			{
				worldTransforms_[PartId::kHip].rotation_.y -= 0.1;
			}
		}
	
	}
	{
		//大元から順に更新していく
		for (int i = PartId::kSpine; i < PartId::kNumPartId; i++)
		{
			MyFunc::UpdateWorldTransform(worldTransforms_[i]);
		}
	
	}

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

	//3Dモデル描画
	for (int i = kChest; i <= kLegR; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

	

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