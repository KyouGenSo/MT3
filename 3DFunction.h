#pragma once
#include<Vector3.h>
#include<Vector3Function.h>
#include<Matrix4x4.h>
#include<Matrix4x4Function.h>
#include<math.h>

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
		Vector3 screenStart = TransForm(viewProjectionMatrix, worldStart);
		Vector3 screenEnd = TransForm(viewProjectionMatrix, worldEnd);
		// スクリーン座標系からビューポート座標系に変換をかける
		screenStart = TransForm(viewportMatrix, screenStart);
		screenEnd = TransForm(viewportMatrix, screenEnd);
		// ライン描画
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0xAAAAAAFF);
	}

	// 左から右への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; zIndex++) {
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 worldStart(-kGridHalfWidth, -1.0f, kGridHalfWidth - kGridEvery * float(zIndex));
		Vector3 worldEnd(kGridHalfWidth, -1.0f, kGridHalfWidth - kGridEvery * float(zIndex));
		// スクリーン座標系まで変換をかける
		Vector3 screenStart = TransForm(viewProjectionMatrix, worldStart);
		Vector3 screenEnd = TransForm(viewProjectionMatrix, worldEnd);
		// スクリーン座標系からビューポート座標系に変換をかける
		screenStart = TransForm(viewportMatrix, screenStart);
		screenEnd = TransForm(viewportMatrix, screenEnd);
		// ライン描画
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0xAAAAAAFF);
	}
}