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

	const float kDeltaTime = 1.0f / 60.0f;

	bool isStart = false;

	MassPoint ball;
	ball.position = Vector3(0.8f, 1.2f, 0.3f);
	ball.velocity = Vector3(0.0f, 0.0f, 0.0f);
	ball.acceleration = Vector3(0.0f, -9.8f, 0.0f);
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;

	Plane plane;
	plane.normal = Vector3(-0.2f, 0.9f, -0.3f);
	plane.distance = 0.0f;


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

		if (isStart) {
			ball.velocity += ball.acceleration * kDeltaTime;
			ball.position += ball.velocity * kDeltaTime;

			if (IsCollision(Sphere{ ball.position, ball.radius }, plane)) {
				ball.color = RED;
				Vector3 reflected = Reflect(ball.velocity, plane.normal);
				Vector3 projToNormal = Project(reflected, plane.normal);
				Vector3 movingDirection = reflected - projToNormal;
				float energyLoss = 0.9f;
				ball.velocity = projToNormal * energyLoss + movingDirection;
			} else {
				ball.color = WHITE;
			}
		} else {
			ball.position = Vector3(0.8f, 1.2f, 0.3f);
			ball.velocity = Vector3(0.0f, 0.0f, 0.0f);
		}




		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///



		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		DrawGrid(viewProjectionMatrix, viewportMtrix);

		DrawSphere(Sphere{ ball.position, ball.radius }, viewProjectionMatrix, viewportMtrix, ball.color);

		DrawPlane(plane, viewProjectionMatrix, viewportMtrix, WHITE);

		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("window");
		if (ImGui::Button("Start")) {
			isStart = true;
		}
		if (ImGui::Button("Stop")) {
			isStart = false;
		}
		// ball position
		ImGui::DragFloat3("Position", &ball.position.x, -0.01f, -10.0f, 10.0f);
		// plane normal
		ImGui::DragFloat3("Normal", &plane.normal.x, -0.01f, -1.0f, 1.0f);
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
