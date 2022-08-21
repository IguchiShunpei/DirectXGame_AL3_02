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


private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 velocity_{ 0.0f,0.0f,0.1f };

	// Affin関数の構造体
	affin::AffinMat affinMat;
};