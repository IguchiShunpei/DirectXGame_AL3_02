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
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	//更新処理
	void Update();

	//描画処理
	void Draw(const ViewProjection& viewprojection);

	//isDeadのgetter
	bool IsDead() const { return isDead_; }

private:
	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLiteTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLiteTime;

	//デスフラグ
	bool isDead_ = false;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};

