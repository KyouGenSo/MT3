#include <Novice.h>
#include <MatrixFunc.h>
#include <VectorFunc.h>
#include <3DFunc.h>
#include <imgui.h>
#include <ImGuiManager.h>
#include <imgui_impl_dx12.h>

const char kWindowTitle[] = "LE2A_10_キョウ_ゲンソ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	int windowX = 1280;
	int windowY = 720;
	Novice::Initialize(kWindowTitle, windowX, windowY);


	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	AABB aabb = { Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f) };
	Segment segment = { Vector3(-0.7f, 0.3f, 0.0f), Vector3(2.0f, -0.5f, 0.0f) };

	int color1 = WHITE;
	int color2 = WHITE;

	Vector3 gridScale(1.0f, 1.0f, 1.0f);
	Vector3 gridRotate(0.0f, 0.0f, 0.0f);
	Vector3 gridTranslate(0.0f, 0.0f, 0.0f);

	Vector3 aabbScale(1.0f, 1.0f, 1.0f);
	Vector3 aabbRotate(0.0f, 0.0f, 0.0f);
	Vector3 aabbTranslate(0.0f, 0.0f, 0.0f);

	Vector3 segmentScale(1.0f, 1.0f, 1.0f);
	Vector3 segmentRotate(0.0f, 0.0f, 0.0f);
	Vector3 segmentTranslate(0.0f, 0.0f, 0.0f);

	Vector3 cameraPosition(0.0f, 0.0f, 10.0f);
	Vector3 cameraRotation(0.0f, 0.0f, 0.0f);

	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), cameraPosition);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, (float)windowX / (float)windowY, 0.1f, 100.0f);
	Matrix4x4 viewportMtrix = MakeViewportMatrix(0, 0, float(windowX), float(windowY), 0.0f, 1.0f);

	Matrix4x4 gridWorldMatrix = MakeAffineMatrix(gridScale, gridRotate, gridTranslate);
	Matrix4x4 gridWVPMatrix = Multiply(gridWorldMatrix, Multiply(viewMatrix, projectionMatrix));

	Matrix4x4 aabbWorldMatrix = MakeAffineMatrix(aabbScale, aabbRotate, aabbTranslate);
	Matrix4x4 aabbWVPMatrix = Multiply(aabbWorldMatrix, Multiply(viewMatrix, projectionMatrix));

	Matrix4x4 segmentWorldMatrix = MakeAffineMatrix(segmentScale, segmentRotate, segmentTranslate);
	Matrix4x4 segmentWVPMatrix = Multiply(segmentWorldMatrix, Multiply(viewMatrix, projectionMatrix));

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///-------------------///
		/// ↓更新処理ここから///
		///-------------------///

		CameraControl(cameraPosition, cameraRotation, 0.1f, 0.01f, keys);

		gridWorldMatrix = MakeAffineMatrix(gridScale, gridRotate, gridTranslate);
		gridWVPMatrix = Multiply(gridWorldMatrix, Multiply(viewMatrix, projectionMatrix));

		aabbWorldMatrix = MakeAffineMatrix(aabbScale, aabbRotate, aabbTranslate);
		aabbWVPMatrix = Multiply(aabbWorldMatrix, Multiply(viewMatrix, projectionMatrix));

		segmentWorldMatrix = MakeAffineMatrix(segmentScale, segmentRotate, segmentTranslate);
		segmentWVPMatrix = Multiply(segmentWorldMatrix, Multiply(viewMatrix, projectionMatrix));

		cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);

		Vector3 start = Transform(viewportMtrix, Transform(segmentWVPMatrix, segment.origin));
		Vector3 end = Transform(viewportMtrix, Transform(segmentWVPMatrix, Add(segment.origin, segment.diff)));

		// AABBの衝突判定
		bool isCollide = IsCollision(aabb, segment);

		if (isCollide) {
			color1 = RED;
		} else {
			color1 = WHITE;
		}

		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///


		
		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		DrawGrid(gridWVPMatrix, viewportMtrix);

		DrawAABB(aabb, aabbWVPMatrix, viewportMtrix, color1);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color2);

		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("AABB");
		ImGui::DragFloat3("min", &aabb.min.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("max", &aabb.max.x, -0.01f, -10.0f, 10.0f);
		aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
		aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
		aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
		aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
		aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
		aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
		ImGui::End();

		ImGui::Begin("segment");
		ImGui::DragFloat3("origin", &segment.origin.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("diff", &segment.diff.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();


		///-------------------///
		/// ↑描画処理ここまで///
		///-------------------///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
