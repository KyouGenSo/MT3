#pragma once
#define _USE_MATH_DEFINES

#include <Novice.h>
#include "Matrix4x4.h"
#include "Vector3.h"
#include"math.h"
#include"cmath"
#include"cassert"


void Matrix4x4ScreenPrint(int x, int y, Matrix4x4 m, const char* string);

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Subtrsct(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 Transpose(const Matrix4x4& m);

Matrix4x4 MakeIdentityMatrix();

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateMatrixX(float angle);

Matrix4x4 MakeRotateMatrixY(float angle);

Matrix4x4 MakeRotateMatrixZ(float angle);

Matrix4x4 MakeRotateMatrixXYZ(Matrix4x4 mX, Matrix4x4 mY, Matrix4x4 mZ);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 Transform(const Matrix4x4& m, const Vector3& v);

Matrix4x4 MakePerspectiveMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthoMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);


