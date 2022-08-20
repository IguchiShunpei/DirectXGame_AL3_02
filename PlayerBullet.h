#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

class PlayerBullet
{
public:
	//コンストラクタ
	PlayerBullet();

	//デストラクタ
	~PlayerBullet();

	//メンバ関数
		//初期化
	void Initialize(Model* model, const Vector3& position);

	//更新処理
	void Update();

	//描画処理
	void Draw(const ViewProjection& viewprojection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};

