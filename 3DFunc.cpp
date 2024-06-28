#include"3Dfunc.h"
#include"VectorFunc.h"

void CameraControl(Vector3& cameraPosition, Vector3& cameraRotation, float moveSpeed, float rotateSpeed, const char* keys) {

	if (keys[DIK_W]) {
		cameraPosition.z += moveSpeed;
	}

	if (keys[DIK_S]) {
		cameraPosition.z -= moveSpeed;
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
		cameraRotation.x -= rotateSpeed;
	}

	if (keys[DIK_DOWN]) {
		cameraRotation.x += rotateSpeed;
	}

	if (keys[DIK_LEFT]) {
		cameraRotation.y -= rotateSpeed;
	}

	if (keys[DIK_RIGHT]) {
		cameraRotation.y += rotateSpeed;
	}
}

Vector3 Perpendicular(const Vector3& v) {
	if (v.x != 0.0f && v.y != 0.0f) {
		return Vector3(-v.y, v.x, 0.0f);
	}
	return Vector3(0.0f, -v.z, v.y);
}


/// <summary>
/// 3Dオブジェクト描画関数
/// </summary>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f; // グリッドの半分の幅
	const uint32_t kSubdivision = 10; // 1分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1分割の長さ

	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; xIndex++) {
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 worldStart(-kGridHalfWidth + kGridEvery * float(xIndex), 0.0f, kGridHalfWidth);
		Vector3 worldEnd(-kGridHalfWidth + kGridEvery * float(xIndex), 0.0f, -kGridHalfWidth);
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
		Vector3 worldStart(-kGridHalfWidth, 0.0f, kGridHalfWidth - kGridEvery * float(zIndex));
		Vector3 worldEnd(kGridHalfWidth, 0.0f, kGridHalfWidth - kGridEvery * float(zIndex));
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

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	Vector3 start = Transform(viewportMatrix, Transform(viewProjectionMatrix, segment.origin));
	Vector3 end = Transform(viewportMatrix, Transform(viewProjectionMatrix, Add(segment.origin, segment.diff)));

	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);

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
	Vector3 vertices[8];

	vertices[0] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.min.y, aabb.min.z)));
	vertices[1] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.min.y, aabb.min.z)));
	vertices[2] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.max.y, aabb.min.z)));
	vertices[3] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.max.y, aabb.min.z)));
	vertices[4] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.min.y, aabb.max.z)));
	vertices[5] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.min.y, aabb.max.z)));
	vertices[6] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.min.x, aabb.max.y, aabb.max.z)));
	vertices[7] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Vector3(aabb.max.x, aabb.max.y, aabb.max.z)));

	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[1].x), int(vertices[1].y), color);
	Novice::DrawLine(int(vertices[1].x), int(vertices[1].y), int(vertices[3].x), int(vertices[3].y), color);
	Novice::DrawLine(int(vertices[3].x), int(vertices[3].y), int(vertices[2].x), int(vertices[2].y), color);
	Novice::DrawLine(int(vertices[2].x), int(vertices[2].y), int(vertices[0].x), int(vertices[0].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[5].x), int(vertices[5].y), int(vertices[7].x), int(vertices[7].y), color);
	Novice::DrawLine(int(vertices[7].x), int(vertices[7].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[6].x), int(vertices[6].y), int(vertices[4].x), int(vertices[4].y), color);
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[4].x), int(vertices[4].y), color);
	Novice::DrawLine(int(vertices[1].x), int(vertices[1].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[2].x), int(vertices[2].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[3].x), int(vertices[3].y), int(vertices[7].x), int(vertices[7].y), color);
}

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 vertices[8];

	Vector3 extendX = Multiply(obb.axis[0], obb.size.x);
	Vector3 extendY = Multiply(obb.axis[1], obb.size.y);
	Vector3 extendZ = Multiply(obb.axis[2], obb.size.z);

	vertices[0] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Subtract(obb.center, Subtract(extendX, Subtract(extendY, extendZ)))));
	vertices[1] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Add(obb.center, Subtract(extendX, Subtract(extendY, extendZ)))));
	vertices[2] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Subtract(obb.center, Subtract(extendX, Add(extendY, extendZ)))));
	vertices[3] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Add(obb.center, Subtract(extendX, Add(extendY, extendZ)))));
	vertices[4] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Subtract(obb.center, Add(extendX, Subtract(extendY, extendZ)))));
	vertices[5] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Add(obb.center, Add(extendX, Subtract(extendY, extendZ)))));
	vertices[6] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Subtract(obb.center, Add(extendX, Add(extendY, extendZ)))));
	vertices[7] = Transform(viewportMatrix, Transform(viewProjectionMatrix, Add(obb.center, Add(extendX, Add(extendY, extendZ)))));

	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[2].x), int(vertices[2].y), color);
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[1].x), int(vertices[1].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[2].x), int(vertices[2].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[7].x), int(vertices[7].y), int(vertices[2].x), int(vertices[2].y), color);
	Novice::DrawLine(int(vertices[7].x), int(vertices[7].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[7].x), int(vertices[7].y), int(vertices[1].x), int(vertices[1].y), color);
	Novice::DrawLine(int(vertices[3].x), int(vertices[3].y), int(vertices[1].x), int(vertices[1].y), color);
	Novice::DrawLine(int(vertices[3].x), int(vertices[3].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[3].x), int(vertices[3].y), int(vertices[5].x), int(vertices[5].y), color);

}

void DrawBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 50; // 分割数
	const float kEvery = 1.0f / float(kSubdivision); // 1分割の長さ

	Vector3 points[2];
	points[0] = p0;
	for (uint32_t i = 1; i <= kSubdivision; i++) {
		float t = kEvery * float(i);
		points[1] = Lerp(Lerp(p0, p1, t), Lerp(p1, p2, t), t);
		DrawSegment({ points[0], Subtract(points[1], points[0]) }, viewProjectionMatrix, viewportMatrix, color);
		points[0] = points[1];
	}

}


/// <summary>
/// 衝突判定関数
/// </summary>
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2) {
	float distance = float(Length(Subtract(sphere2.center, sphere1.center)));

	if (distance < sphere1.radius + sphere2.radius) {
		return true;
	} else {
		return false;
	}
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {
	float distance = Dot(sphere.center, plane.normal) - plane.distance;

	if (distance < sphere.radius) {
		return true;
	} else {
		return false;
	}
}

bool IsCollision(const Segment& segment, const Plane& plane) {
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

bool IsCollision(const Triangle& triangle, const Segment& segment) {
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

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x &&
		aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y &&
		aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	Vector3 closestPoint;
	closestPoint.x = std::clamp(sphere.center.x, aabb.min.x, aabb.max.x);
	closestPoint.y = std::clamp(sphere.center.y, aabb.min.y, aabb.max.y);
	closestPoint.z = std::clamp(sphere.center.z, aabb.min.z, aabb.max.z);

	float distance = float(Length(Subtract(closestPoint, sphere.center)));

	if (distance < sphere.radius) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Segment& segment) {
	Vector3 min = aabb.min;
	Vector3 max = aabb.max;
	float tMinX, tMaxX, tMinY, tMaxY, tMinZ, tMaxZ;

	assert(segment.diff.x != 0.0f || segment.diff.y != 0.0f || segment.diff.z != 0.0f);

	if (segment.diff.x == 0.0f) {
		if (segment.origin.x < min.x || segment.origin.x > max.x) {
			return false;
		}
		tMaxX = INFINITY;
		tMinX = -INFINITY;
	} else {
		tMinX = (min.x - segment.origin.x) / segment.diff.x;
		tMaxX = (max.x - segment.origin.x) / segment.diff.x;
	}

	if (segment.diff.y == 0.0f) {
		if (segment.origin.y < min.y || segment.origin.y > max.y) {
			return false;
		}
		tMaxY = INFINITY;
		tMinY = -INFINITY;
	} else {
		tMinY = (min.y - segment.origin.y) / segment.diff.y;
		tMaxY = (max.y - segment.origin.y) / segment.diff.y;
	}

	if (segment.diff.z == 0.0f) {
		if (segment.origin.z < min.z || segment.origin.z > max.z) {
			return false;
		}
		tMaxZ = INFINITY;
		tMinZ = -INFINITY;
	} else {
		tMinZ = (min.z - segment.origin.z) / segment.diff.z;
		tMaxZ = (max.z - segment.origin.z) / segment.diff.z;
	}

	float tNearX = (std::min)(tMinX, tMaxX);
	float tFarX = (std::max)(tMinX, tMaxX);
	float tNearY = (std::min)(tMinY, tMaxY);
	float tFarY = (std::max)(tMinY, tMaxY);
	float tNearZ = (std::min)(tMinZ, tMaxZ);
	float tFarZ = (std::max)(tMinZ, tMaxZ);

	float tMin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	float tMax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	if (tMin <= tMax) {
		if (tMin >= 0.0f && tMin <= 1.0f || tMax >= 0.0f && tMax <= 1.0f) {
			return true;
		}
	}

	return false;
}

bool IsCollision(const OBB& obb, const Segment& segment,const Matrix4x4& obbInverse) {

	Vector3 localOrigin = Transform(obbInverse, segment.origin);
	Vector3 localEnd = Transform(obbInverse, Add(segment.origin, segment.diff));

	AABB localAABB = {
		.min = { -obb.size.x, -obb.size.y, -obb.size.z },
		.max = { obb.size.x, obb.size.y, obb.size.z }
	};

	Segment localSegment = {
		.origin = localOrigin,
		.diff = Subtract(localEnd, localOrigin)
	};

	return IsCollision(localAABB, localSegment);

}

bool IsCollision(const OBB& obb, const Ray& ray, const Matrix4x4& obbInverse) {

	Vector3 localOrigin = Transform(obbInverse, ray.origin);
	Vector3 localEnd = Transform(obbInverse, Add(ray.origin, ray.diff));

	AABB localAABB = {
		.min = { -obb.size.x, -obb.size.y, -obb.size.z },
		.max = { obb.size.x, obb.size.y, obb.size.z }
	};

	Segment localSegment = {
		.origin = localOrigin,
		.diff = Subtract(localEnd, localOrigin)
	};

	return IsCollision(localAABB, localSegment);
}

bool IsCollision(const OBB& obb, const Line& line, const Matrix4x4& obbInverse) {

	Vector3 localOrigin = Transform(obbInverse, line.origin);
	Vector3 localEnd = Transform(obbInverse, Add(line.origin, line.diff));

	AABB localAABB = {
		.min = { -obb.size.x, -obb.size.y, -obb.size.z },
		.max = { obb.size.x, obb.size.y, obb.size.z }
	};

	Segment localSegment = {
		.origin = localOrigin,
		.diff = Subtract(localEnd, localOrigin)
	};

	return IsCollision(localAABB, localSegment);
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {
	Vector3 obb1Vertices[8];
	Vector3 obb2Vertices[8];
	Vector3 obb1Sides[3];
	Vector3 obb2Sides[3];

	obb1Vertices[0] = Subtract(obb1.center, Subtract(Multiply(obb1.axis[0], obb1.size.x), Subtract(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));
	obb1Vertices[1] = Add(obb1.center, Subtract(Multiply(obb1.axis[0], obb1.size.x), Subtract(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));
	obb1Vertices[2] = Subtract(obb1.center, Subtract(Multiply(obb1.axis[0], obb1.size.x), Subtract(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));
	obb1Vertices[3] = Add(obb1.center, Subtract(Multiply(obb1.axis[0], obb1.size.x), Subtract(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));
	obb1Vertices[4] = Subtract(obb1.center, Add(Multiply(obb1.axis[0], obb1.size.x), Subtract(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));
	obb1Vertices[5] = Add(obb1.center, Add(Multiply(obb1.axis[0], obb1.size.x), Subtract(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));
	obb1Vertices[6] = Subtract(obb1.center, Add(Multiply(obb1.axis[0], obb1.size.x), Add(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));
	obb1Vertices[7] = Add(obb1.center, Add(Multiply(obb1.axis[0], obb1.size.x), Add(Multiply(obb1.axis[1], obb1.size.y), Multiply(obb1.axis[2], obb1.size.z))));

	obb2Vertices[0] = Subtract(obb2.center, Subtract(Multiply(obb2.axis[0], obb2.size.x), Subtract(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));
	obb2Vertices[1] = Add(obb2.center, Subtract(Multiply(obb2.axis[0], obb2.size.x), Subtract(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));
	obb2Vertices[2] = Subtract(obb2.center, Subtract(Multiply(obb2.axis[0], obb2.size.x), Subtract(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));
	obb2Vertices[3] = Add(obb2.center, Subtract(Multiply(obb2.axis[0], obb2.size.x), Subtract(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));
	obb2Vertices[4] = Subtract(obb2.center, Add(Multiply(obb2.axis[0], obb2.size.x), Subtract(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));
	obb2Vertices[5] = Add(obb2.center, Add(Multiply(obb2.axis[0], obb2.size.x), Subtract(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));
	obb2Vertices[6] = Subtract(obb2.center, Add(Multiply(obb2.axis[0], obb2.size.x), Add(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));
	obb2Vertices[7] = Add(obb2.center, Add(Multiply(obb2.axis[0], obb2.size.x), Add(Multiply(obb2.axis[1], obb2.size.y), Multiply(obb2.axis[2], obb2.size.z))));

	obb1Sides[0] = Multiply(obb1.axis[0], obb1.size.x);
	obb1Sides[1] = Multiply(obb1.axis[1], obb1.size.y);
	obb1Sides[2] = Multiply(obb1.axis[2], obb1.size.z);

	obb2Sides[0] = Multiply(obb2.axis[0], obb2.size.x);
	obb2Sides[1] = Multiply(obb2.axis[1], obb2.size.y);
	obb2Sides[2] = Multiply(obb2.axis[2], obb2.size.z);

	// すべての頂点を分離軸（面法線）に対して射影して判定する
	for (int i = 0; i < 3; i++) {
		Vector3 axis = obb1.axis[i];
		float min1 = INFINITY;
		float max1 = -INFINITY;
		float min2 = INFINITY;
		float max2 = -INFINITY;
		float L1, L2;

		for (int j = 0; j < 8; j++) {
			float projection = Dot(obb1Vertices[j], axis);
			min1 = (std::min)(min1, projection);
			max1 = (std::max)(max1, projection);
		}

		for (int j = 0; j < 8; j++) {
			float projection = Dot(obb2Vertices[j], axis);
			min2 = (std::min)(min2, projection);
			max2 = (std::max)(max2, projection);
		}

		L1 = max1 - min1;
		L2 = max2 - min2;
		float sumSpan = L1 + L2;
		float longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);

		if (sumSpan < longSpan) {
			return false;
		}
	}

	// すべての頂点を分離軸（各辺の組み合わせのクロス積）に対して射影して判定する
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Vector3 axis = Cross(obb1Sides[i], obb2Sides[j]);
			float min1 = INFINITY;
			float max1 = -INFINITY;
			float min2 = INFINITY;
			float max2 = -INFINITY;
			float L1, L2;

			for (int k = 0; k < 8; k++) {
				float projection = Dot(obb1Vertices[k], axis);
				min1 = (std::min)(min1, projection);
				max1 = (std::max)(max1, projection);
			}

			for (int k = 0; k < 8; k++) {
				float projection = Dot(obb2Vertices[k], axis);
				min2 = (std::min)(min2, projection);
				max2 = (std::max)(max2, projection);
			}

			L1 = max1 - min1;
			L2 = max2 - min2;
			float sumSpan = L1 + L2;
			float longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);

			if (sumSpan < longSpan) {
				return false;
			}
		}
	}

	return true;
}