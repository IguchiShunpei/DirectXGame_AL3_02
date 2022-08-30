#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "WinApp.h"
#include "math.h"
#include "affin/affin.h"
#include "MyFunc.h"
#include "Player/Player.h"
#include "Enemy.h"
#include "skydome.h"
#include "RailCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public:
	//パーツID
	enum PartId
	{
		kRoot,  //大元
		kSpine,	//脊椎
		kChest,	//胸
		kHead,	//頭
		kArmL,	//左腕
		kArmR,	//右腕
		kHip,	//尻
		kLegL,	//左足
		kLegR,	//右足

		kNumPartId
	};

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//すべての子分の当たり判定をチェックする関数
	void CheckAllCollisions();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t enemyHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//天球
	skydome* skydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	const float M_PI = 3.141592f;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
