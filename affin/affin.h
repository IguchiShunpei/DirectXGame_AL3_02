#pragma once

#include "Matrix4.h"
#include "WorldTransform.h"
#include "math.h"
#include "cmath"
#include "Vector4.h"
#include "MathUtility.h"
#include "WinApp.h"

namespace affin {

	enum AXIS_NO {
		AXIS_NONE,	//0
		AXIS_X,		//1
		AXIS_Y,		//2
		AXIS_XY,	//3
		AXIS_Z,		//4
		AXIS_XZ,	//5
		AXIS_YZ,	//6
		AXIS_XYZ	//7
	};

	typedef struct AffinMat {
		Matrix4 scale = MathUtility::Matrix4Identity();
		Matrix4 rotateX = MathUtility::Matrix4Identity();
		Matrix4 rotateY = MathUtility::Matrix4Identity();
		Matrix4 rotateZ = MathUtility::Matrix4Identity();
		Matrix4 translate = MathUtility::Matrix4Identity();
	};

	//(1)

	//(1)-1 設定する関数
	void setScaleMat(Matrix4& mat, WorldTransform& worldTransform);
	void setRotateMat(AffinMat& mat, WorldTransform& worldTransform);
	void setTranslateMat(Matrix4& mat, WorldTransform& worldTransform);

	//(2)-2 行列を使って変換した値を返す関数
	Matrix4 generateScaleMat(WorldTransform& worldTransform);

	Matrix4 generateRotateXMat(WorldTransform& worldTransform);
	Matrix4 generateRotateYMat(WorldTransform& worldTransform);
	Matrix4 generateRotateZMat(WorldTransform& worldTransform);

	Matrix4 generateTransMat(WorldTransform& worldTransform);

	//(2) ワールド行列を生成する関数
	void setTransformationWolrdMat(AffinMat& affinMat, WorldTransform& worldTransform);

	Vector3 MatVector(Vector3 v, Matrix4 mat);

	//正規化関数
	Vector3 Normalize(Vector3& v);

	Vector3 GetWorldPosition(Matrix4 mat);

	const Vector3 AddVector3(const Vector3& v1, const Vector3& v2);

	const Vector3 Division(const Vector3& v, Matrix4 mat);

	Matrix4 SetViewport(WorldTransform& worldTransform, WinApp* window, const Vector3& v);

}
