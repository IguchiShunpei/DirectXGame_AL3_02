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
#include "MyFunc/MyFunc.h"

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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	const float M_PI = 3.141592f;
	const float G = 9.8f;
	const float e = 0.8f;

	Vector3 startSpeed = {0.3f,0.0f,0.0f};

	float flame = 0.0f;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
