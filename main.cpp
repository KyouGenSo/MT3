#include <Novice.h>
#include <Matrix4x4Function.h>
#include <Vector3Function.h>
#include <3DFunction.h>
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

	Segment segment = { Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f) };
	Plane plane = { Vector3(0.0f, 1.0f, 0.0f), 0.0f };

	int color1 = WHITE;
	int color2 = WHITE;

	Vector3 gridScale(1.0f, 1.0f, 1.0f);
	Vector3 gridRotate(0.0f, 0.0f, 0.0f);
	Vector3 gridTranslate(0.0f, 0.0f, 0.0f);

	Vector3 PlaneScale(1.0f, 1.0f, 1.0f);
	Vector3 PlaneRotate(0.0f, 0.0f, 0.0f);
	Vector3 PlaneTranslate(0.0f, 0.0f, 0.0f);

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

	Matrix4x4 PlaneWorldMatrix = MakeAffineMatrix(PlaneScale, PlaneRotate, PlaneTranslate);
	Matrix4x4 PlaneWvpMatrix = Multiply(PlaneWorldMatrix, Multiply(viewMatrix, projectionMatrix));

	Matrix4x4 segmentWorldMatrix = MakeAffineMatrix(segmentScale, segmentRotate, segmentTranslate);
	Matrix4x4 segmentWvpMatrix = Multiply(segmentWorldMatrix, Multiply(viewMatrix, projectionMatrix));

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

		PlaneWorldMatrix = MakeAffineMatrix(PlaneScale, PlaneRotate, PlaneTranslate);
		PlaneWvpMatrix = Multiply(PlaneWorldMatrix, Multiply(viewMatrix, projectionMatrix));

		segmentWorldMatrix = MakeAffineMatrix(segmentScale, segmentRotate, segmentTranslate);
		segmentWvpMatrix = Multiply(segmentWorldMatrix, Multiply(viewMatrix, projectionMatrix));

		Vector3 segmentOrigin = Transform(viewportMtrix, Transform(segmentWvpMatrix, segment.origin));
		Vector3 segmentEnd = Transform(viewportMtrix, Transform(segmentWvpMatrix, Add(segment.origin, segment.diff)));

		cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);

		// 線と平面の衝突判定
		bool isCollide = IsSegmentPlaneCollision(segment, plane);

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

		DrawPlane(plane, PlaneWvpMatrix, viewportMtrix, color2);

		Novice::DrawLine(int(segmentOrigin.x), int(segmentOrigin.y), int(segmentEnd.x), int(segmentEnd.y), color1);

		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("Segment");
		ImGui::DragFloat3("Origin", &segment.origin.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Diff", &segment.diff.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("Plane");
		ImGui::DragFloat3("Scale", &PlaneScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &PlaneRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &PlaneTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Normal", &plane.normal.x, -0.01f, -10.0f, 10.0f);
		plane.normal = Normalize(plane.normal);
		ImGui::DragFloat("Distance", &plane.distance, -0.01f, -10.0f, 10.0f);
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
