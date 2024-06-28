#pragma once
#include<Vector3.h>
#include<VectorFunc.h>
#include<Matrix4x4.h>
#include<Matrix3x3.h>
#include<MatrixFunc.h>
#include<math.h>
#include"algorithm"

struct Sphere {
	Vector3 center;
	float radius;
};

struct Plane {
	Vector3 normal;
	float distance;
};

struct Triangle {
	Vector3 vertex[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;
	Vector3 axis[3];
	Vector3 size;
};

void CameraControl(Vector3& cameraPosition, Vector3& cameraRotation, float moveSpeed, float rotateSpeed, const char* keys);

Vector3 Perpendicular(const Vector3& v);


/// <summary>
/// 3Dオブジェクト描画関数
/// </summary>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);


/// <summary>
/// 衝突判定関数
/// </summary>
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

bool IsCollision(const Sphere& sphere, const Plane& plane);

bool IsCollision(const Segment& segment, const Plane& plane);

bool IsCollision(const Triangle& triangle, const Segment& segment);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

bool IsCollision(const AABB& aabb, const Sphere& sphere);

bool IsCollision(const AABB& aabb, const Segment& segment);

bool IsCollision(const OBB& obb, const Segment& segment, const Matrix4x4& obbInverse);

bool IsCollision(const OBB& obb, const Ray& ray, const Matrix4x4& obbInverse);

bool IsCollision(const OBB& obb, const Line& line, const Matrix4x4& obbInverse);

bool IsCollision(const OBB& obb1, const OBB& obb2);
