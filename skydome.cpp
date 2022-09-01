#include "skydome.h"
void skydome::Initialize(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = { 3.0f, 3.0f, 3.0f };
	Matrix4 matScale;

	matScale = affin::generateScaleMat(worldTransform_);

	worldTransform_.matWorld_.operator*=(matScale);
	
	worldTransform_.TransferMatrix();
}

void skydome::Update()
{

}

void skydome::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_);
}