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
#include<memory>
#include<list>
#include "WinApp.h"

//自キャラ
class Player
{
public:
	Player();
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);

	//更新
	void Update(const ViewProjection& viewProjection);

	//移動
	void Move();

	//回転
	void Rotate();

	//攻撃
	void Attack();

	//描画
	void Draw(ViewProjection viewProjection_);

	void DrawUI();

	//キャラの向きに応じた方向に球を出す
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//半径を返す関数
	float GetRadius();

	//親子構造のセッター
	void SetParent(WorldTransform* worldTransform);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//モデル
	Model* model_ = nullptr;

	Model* modelBullet_ = nullptr;
	//インプット
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;

	//打ち出すまでの時間
	float dalayTimer = 0.5f;

	//Affin関数の構造体
	affin::AffinMat affinMat;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	DebugText* debugText_ = nullptr;
	WinApp* winApp_ = nullptr;

	//半径
	float radius = 1.0f;
};