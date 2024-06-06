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

	AABB aabb1 = { Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.0f, 0.0f, 0.0f) };
	AABB aabb2 = { Vector3(0.2f, 0.2f, 0.2f), Vector3(1.0f, 1.0f, 1.0f) };

	int color1 = WHITE;
	int color2 = WHITE;

	Vector3 gridScale(1.0f, 1.0f, 1.0f);
	Vector3 gridRotate(0.0f, 0.0f, 0.0f);
	Vector3 gridTranslate(0.0f, 0.0f, 0.0f);

	Vector3 aabb1Scale(1.0f, 1.0f, 1.0f);
	Vector3 aabb1Rotate(0.0f, 0.0f, 0.0f);
	Vector3 aabb1Translate(0.0f, 0.0f, 0.0f);

	Vector3 aabb2Scale(1.0f, 1.0f, 1.0f);
	Vector3 aabb2Rotate(0.0f, 0.0f, 0.0f);
	Vector3 aabb2Translate(0.0f, 0.0f, 0.0f);

	Vector3 cameraPosition(0.0f, 0.0f, 10.0f);
	Vector3 cameraRotation(0.0f, 0.0f, 0.0f);

	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), cameraPosition);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, (float)windowX / (float)windowY, 0.1f, 100.0f);
	Matrix4x4 viewportMtrix = MakeViewportMatrix(0, 0, float(windowX), float(windowY), 0.0f, 1.0f);

	Matrix4x4 gridWorldMatrix = MakeAffineMatrix(gridScale, gridRotate, gridTranslate);
	Matrix4x4 gridWVPMatrix = Multiply(gridWorldMatrix, Multiply(viewMatrix, projectionMatrix));

	Matrix4x4 aabb1WorldMatrix = MakeAffineMatrix(aabb1Scale, aabb1Rotate, aabb1Translate);
	Matrix4x4 aabb1WVPMatrix = Multiply(aabb1WorldMatrix, Multiply(viewMatrix, projectionMatrix));

	Matrix4x4 aabb2WorldMatrix = MakeAffineMatrix(aabb2Scale, aabb2Rotate, aabb2Translate);
	Matrix4x4 aabb2WVPMatrix = Multiply(aabb2WorldMatrix, Multiply(viewMatrix, projectionMatrix));

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

		aabb1WorldMatrix = MakeAffineMatrix(aabb1Scale, aabb1Rotate, aabb1Translate);
		aabb1WVPMatrix = Multiply(aabb1WorldMatrix, Multiply(viewMatrix, projectionMatrix));

		aabb2WorldMatrix = MakeAffineMatrix(aabb2Scale, aabb2Rotate, aabb2Translate);
		aabb2WVPMatrix = Multiply(aabb2WorldMatrix, Multiply(viewMatrix, projectionMatrix));

		cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);

		// AABBの衝突判定
		bool isCollide = IsAABBCollision(aabb1, aabb2);

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

		DrawAABB(aabb1, gridWVPMatrix, viewportMtrix, color1);
		DrawAABB(aabb2, gridWVPMatrix, viewportMtrix, color2);


		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("AABB1");
		ImGui::DragFloat3("min", &aabb1.min.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("max", &aabb1.max.x, -0.01f, -10.0f, 10.0f);
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);
		ImGui::End();

		ImGui::Begin("AABB2");
		ImGui::DragFloat3("min", &aabb2.min.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("max", &aabb2.max.x, -0.01f, -10.0f, 10.0f);
		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);
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
