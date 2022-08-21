#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
class EnemyBullet
{
public:
	//生成
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//isDeadのgetter
	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLiteTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLiteTime;

	//デスフラグ
	bool isDead_ = false;

	//アフィン
	affin::AffinMat affinMat;
};
