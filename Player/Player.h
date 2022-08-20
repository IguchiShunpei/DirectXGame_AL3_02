#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "DebugText.h"
#include "Input.h"
#include "affin/affin.h"
#include "ViewProjection.h"
#include "MyFunc.h"
#include "PlayerBullet.h"

//自キャラ
class Player
{
public:
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);

	//更新
	void Update();

	//移動
	void Move();

	//回転
	void Rotate();

	//攻撃
	void Attack();

	//描画
	void Draw(ViewProjection viewProjection_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//弾
	PlayerBullet* bullet_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
};

