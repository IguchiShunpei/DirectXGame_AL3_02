#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <memory>
#include <list>
class Enemy
{

	enum class Phase
	{
		None,
		Approach,//接近
		Leave,   //離脱
	};

public:
	/// <summary>
	/// 生成
	/// </summary>

	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw(ViewProjection viewProjection);

	//接近
	void Approach();

	//離脱
	void Leave();

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 velocity_ = { 0.0f,0.0f,0.1f };

	//接近速度
	Vector3 approach_ = {0.0f,0.0f,-0.1f};

	//離脱速度
	Vector3 leave_ = {-0.15f,0.1f,0.1f};

	// Affin関数の構造体
	affin::AffinMat affinMat;

	//敵の行動パターン
	Phase phase_ = Phase::None;
};