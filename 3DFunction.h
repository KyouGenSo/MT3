#pragma once
#include<Vector3.h>
#include<Vector3Function.h>
#include<Matrix4x4.h>
#include<Matrix4x4Function.h>
#include<math.h>

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

void CameraControl(Vector3& cameraPosition, Vector3& cameraRotation, float moveSpeed, float rotateSpeed, const char* keys) {

	if (keys[DIK_W]) {
		cameraPosition.z -= moveSpeed;
	}

	if (keys[DIK_S]) {
		cameraPosition.z += moveSpeed;
	}

	if (keys[DIK_A]) {
		cameraPosition.x -= moveSpeed;
	}

	if (keys[DIK_D]) {
		cameraPosition.x += moveSpeed;
	}

	if (keys[DIK_SPACE]) {
		cameraPosition.y += moveSpeed;
	}

	if (keys[DIK_LSHIFT]) {
		cameraPosition.y -= moveSpeed;
	}

	if (keys[DIK_UP]) {
		cameraRotation.x += rotateSpeed;
	}

	if (keys[DIK_DOWN]) {
		cameraRotation.x -= rotateSpeed;
	}

	if (keys[DIK_LEFT]) {
		cameraRotation.y += rotateSpeed;
	}

	if (keys[DIK_RIGHT]) {
		cameraRotation.y -= rotateSpeed;
	}
}

Vector3 Perpendicular(const Vector3& v) {
	if (v.x != 0.0f && v.y != 0.0f) {
		return Vector3(-v.y, v.x, 0.0f);
	}
	return Vector3(0.0f, -v.z, v.y);
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f; // グリッドの半分の幅
	const uint32_t kSubdivision = 10; // 1分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1分割の長さ

	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; xIndex++) {
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 worldStart(-kGridHalfWidth + kGridEvery * float(xIndex), -1.0f, kGridHalfWidth);
		Vector3 worldEnd(-kGridHalfWidth + kGridEvery * float(xIndex), -1.0f, -kGridHalfWidth);
		// スクリーン座標系まで変換をかける
		Vector3 screenStart = Transform(viewProjectionMatrix, worldStart);
		Vector3 screenEnd = Transform(viewProjectionMatrix, worldEnd);
		// スクリーン座標系からビューポート座標系に変換をかける
		screenStart = Transform(viewportMatrix, screenStart);
		screenEnd = Transform(viewportMatrix, screenEnd);
		// ライン描画
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0xAAAAAAFF);
	}

	// 左から右への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; zIndex++) {
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 worldStart(-kGridHalfWidth, -1.0f, kGridHalfWidth - kGridEvery * float(zIndex));
		Vector3 worldEnd(kGridHalfWidth, -1.0f, kGridHalfWidth - kGridEvery * float(zIndex));
		// スクリーン座標系まで変換をかける
		Vector3 screenStart = Transform(viewProjectionMatrix, worldStart);
		Vector3 screenEnd = Transform(viewProjectionMatrix, worldEnd);
		// スクリーン座標系からビューポート座標系に変換をかける
		screenStart = Transform(viewportMatrix, screenStart);
		screenEnd = Transform(viewportMatrix, screenEnd);
		// ライン描画
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0xAAAAAAFF);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 20; // 1分割数
	const float kLonEvery = 2.0f * 3.14159265359f / float(kSubdivision); // 経度の1分割の角度 phi
	const float kLatEvery = 3.14159265359f / float(kSubdivision); // 緯度の1分割の角度 theta

	// 緯度方向のループ
	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {
		float lat = -3.14159265359f / 2.0f + kLatEvery * float(latIndex);
		// 経度方向のループ
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
			float lon = kLonEvery * float(lonIndex);
			// 球の表面上の点を求める
			Vector3 a, b, c;
			a.x = sphere.center.x + sphere.radius * cosf(lat) * cosf(lon);
			a.y = sphere.center.y + sphere.radius * sinf(lat);
			a.z = sphere.center.z + sphere.radius * cosf(lat) * sinf(lon);
			b.x = sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon);
			b.y = sphere.center.y + sphere.radius * sinf(lat + kLatEvery);
			b.z = sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon);
			c.x = sphere.center.x + sphere.radius * cosf(lat) * cosf(lon + kLonEvery);
			c.y = sphere.center.y + sphere.radius * sinf(lat);
			c.z = sphere.center.z + sphere.radius * cosf(lat) * sinf(lon + kLonEvery);

			// スクリーン座標系まで変換をかける
			Vector3 screenA = Transform(viewProjectionMatrix, a);
			Vector3 screenB = Transform(viewProjectionMatrix, b);
			Vector3 screenC = Transform(viewProjectionMatrix, c);

			// スクリーン座標系からビューポート座標系に変換をかける
			screenA = Transform(viewportMatrix, screenA);
			screenB = Transform(viewportMatrix, screenB);
			screenC = Transform(viewportMatrix, screenC);

			// ライン描画
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenB.x), int(screenB.y), int(screenC.x), int(screenC.y), color);
		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.normal, plane.distance);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int i = 0; i < 4; i++) {
		Vector3 extend = Multiply(perpendiculars[i], 2.0f);
		Vector3 point = Add(center, extend);
		points[i] = Transform(viewportMatrix, Transform(viewProjectionMatrix, point));
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 points[3];
	for (int i = 0; i < 3; i++) {
		points[i] = Transform(viewportMatrix, Transform(viewProjectionMatrix, triangle.vertex[i]));
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 points[12];
	points[0] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.min.y, aabb.min.z)));
	points[1] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.min.y, aabb.min.z)));
	points[2] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.min.y, aabb.max.z)));
	points[3] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.min.y, aabb.max.z)));
	points[4] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.max.y, aabb.min.z)));
	points[5] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.max.y, aabb.min.z)));
	points[6] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.max.y, aabb.max.z)));
	points[7] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.max.y, aabb.max.z)));
	points[8] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.min.y, aabb.min.z)));
	points[9] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.max.y, aabb.min.z)));
	points[10] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.min.y, aabb.min.z)));
	points[11] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.max.y, aabb.min.z)));

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
	Novice::DrawLine(int(points[4].x), int(points[4].y), int(points[5].x), int(points[5].y), color);
	Novice::DrawLine(int(points[5].x), int(points[5].y), int(points[6].x), int(points[6].y), color);
	Novice::DrawLine(int(points[6].x), int(points[6].y), int(points[7].x), int(points[7].y), color);
	Novice::DrawLine(int(points[7].x), int(points[7].y), int(points[4].x), int(points[4].y), color);
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[4].x), int(points[4].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[5].x), int(points[5].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[6].x), int(points[6].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[7].x), int(points[7].y), color);
}

bool IsSphereCollision(const Sphere& sphere1, const Sphere& sphere2) {
	float distance = float(Length(Subtract(sphere2.center, sphere1.center)));

	if (distance < sphere1.radius + sphere2.radius) {
		return true;
	}
	else {
		return false;
	}
}

bool IsSpherePlaneCollision(const Sphere& sphere, const Plane& plane) {
	float distance = Dot(sphere.center, plane.normal) - plane.distance; 

	if (distance < sphere.radius) {
		return true;
	} else {
		return false;
	}
}

bool IsSegmentPlaneCollision(const Segment& segment, const Plane& plane) {
	float dot = Dot(plane.normal, segment.diff);

	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(plane.normal, segment.origin)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	float distance1 = Dot(segment.origin, plane.normal) - plane.distance;
	float distance2 = Dot(Add(segment.origin, segment.diff), plane.normal) - plane.distance;

	if (distance1 * distance2 < 0.0f) {
		return true;
	} else {
		return false;
	}
}

bool IsTriangleSegmentCollision(const Triangle& triangle, const Segment& segment) {
	Vector3 v01 = Subtract(triangle.vertex[1], triangle.vertex[0]);
	Vector3 v12 = Subtract(triangle.vertex[2], triangle.vertex[1]);
	Vector3 v20 = Subtract(triangle.vertex[0], triangle.vertex[2]);

	// 衝突点が三角形の内側にあるかどうかを判定する
	Vector3 normal = Cross(v01, v12);
	float distance = Dot(triangle.vertex[0], normal);
	float distance1 = Dot(segment.origin, normal) - distance;
	float distance2 = Dot(Add(segment.origin, segment.diff), normal) - distance;

	if (distance1 * distance2 < 0.0f) {
		Vector3 collisionPoint = Add(segment.origin, Multiply(segment.diff, distance1 / (distance1 - distance2)));
		Vector3 v0p = Subtract(collisionPoint, triangle.vertex[0]);
		Vector3 v1p = Subtract(collisionPoint, triangle.vertex[1]);
		Vector3 v2p = Subtract(collisionPoint, triangle.vertex[2]);

		Vector3 c01 = Cross(v01, v1p);
		Vector3 c12 = Cross(v12, v2p);
		Vector3 c20 = Cross(v20, v0p);

		if (Dot(c01, c12) >= 0.0f && Dot(c12, c20) >= 0.0f) {
			return true;
		}
	}

	return false;
}

bool IsAABBCollision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x &&
		aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y &&
		aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z) {
		return true;
	}

	return false;
}
