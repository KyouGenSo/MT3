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
