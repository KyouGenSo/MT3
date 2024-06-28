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

	Vector3 controlPoints[4] = {
		Vector3(-0.8f, 0.58f, 1.0f),
		Vector3(1.76f, 1.0f, -0.3f),
		Vector3(0.95f, -0.7f, 2.3f),
		Vector3(-0.53f, -0.26f, -0.15f)
	};

	int color1 = WHITE;

	Vector3 gridScale(1.0f, 1.0f, 1.0f);
	Vector3 gridRotate(0.0f, 0.0f, 0.0f);
	Vector3 gridTranslate(0.0f, 0.0f, 0.0f);


	Vector3 cameraPosition(0.0f, 1.9f, -6.49f);
	Vector3 cameraRotation(0.26f, 0.0f, 0.0f);

	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, (float)windowX / (float)windowY, 0.1f, 100.0f);
	Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	Matrix4x4 viewportMtrix = MakeViewportMatrix(0, 0, float(windowX), float(windowY), 0.0f, 1.0f);

	Matrix4x4 gridWorldMatrix;
	Matrix4x4 gridWVPMatrix;


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

		cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);

		viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

		gridWorldMatrix = MakeAffineMatrix(gridScale, gridRotate, gridTranslate);
		gridWVPMatrix = Multiply(gridWorldMatrix, viewProjectionMatrix);




		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///



		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		DrawGrid(viewProjectionMatrix, viewportMtrix);

		DrawCatmullRom(controlPoints[0], controlPoints[0], controlPoints[1], controlPoints[2], viewProjectionMatrix, viewportMtrix, color1);

		DrawCatmullRom(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], viewProjectionMatrix, viewportMtrix, color1);

		DrawCatmullRom(controlPoints[1], controlPoints[2], controlPoints[3], controlPoints[3], viewProjectionMatrix, viewportMtrix, color1);

		DrawSphere(Sphere(controlPoints[0], 0.01f), viewProjectionMatrix, viewportMtrix, BLACK);

		DrawSphere(Sphere(controlPoints[1], 0.01f), viewProjectionMatrix, viewportMtrix, BLACK);

		DrawSphere(Sphere(controlPoints[2], 0.01f), viewProjectionMatrix, viewportMtrix, BLACK);

		DrawSphere(Sphere(controlPoints[3], 0.01f), viewProjectionMatrix, viewportMtrix, BLACK);

		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("CatmullRom");
		ImGui::DragFloat3("Control Point 1", &controlPoints[0].x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Control Point 2", &controlPoints[1].x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Control Point 3", &controlPoints[2].x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Control Point 4", &controlPoints[3].x, -0.01f, -10.0f, 10.0f);
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
