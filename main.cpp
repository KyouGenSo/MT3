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

	Sphere sphere1 = { Vector3(-1.5f, 0.0f, 0.0f), 1.0f };
	Sphere sphere2 = { Vector3(1.5f, 0.0f, 0.0f), 1.0f };

	int color1 = WHITE;
	int color2 = WHITE;

	Vector3 gridScale(1.0f, 1.0f, 1.0f);
	Vector3 gridRotate(0.0f, 0.0f, 0.0f);
	Vector3 gridTranslate(0.0f, 0.0f, 0.0f);

	Vector3 scale(1.0f, 1.0f, 1.0f);
	Vector3 rotate(0.0f, 0.0f, 0.0f);
	Vector3 translate(0.0f, 0.0f, 0.0f);

	Vector3 scale2(1.0f, 1.0f, 1.0f);
	Vector3 rotate2(0.0f, 0.0f, 0.0f);
	Vector3 translate2(0.0f, 0.0f, 0.0f);

	Vector3 cameraPosition(0.0f, 0.0f, 10.0f);
	Vector3 cameraRotation(0.0f, 0.0f, 0.0f);

	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), cameraPosition);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, (float)windowX / (float)windowY, 0.1f, 100.0f);
	Matrix4x4 viewportMtrix = MakeViewportMatrix(0, 0, float(windowX), float(windowY), 0.0f, 1.0f);

	Matrix4x4 gridWorldMatrix = MakeAffineMatrix(gridScale, gridRotate, gridTranslate);
	Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
	Matrix4x4 worldMatrix2 = MakeAffineMatrix(scale2, rotate2, translate2);

	Matrix4x4 gridWVPMatrix = Multiply(gridWorldMatrix, Multiply(viewMatrix, projectionMatrix));
	Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	Matrix4x4 wvpMatrix2 = Multiply(worldMatrix2, Multiply(viewMatrix, projectionMatrix));


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

		if (keys[DIK_W]) {
			cameraPosition.z -= 0.1f;
		}

		if (keys[DIK_S]) {
			cameraPosition.z += 0.1f;
		}

		if (keys[DIK_A]) {
			cameraPosition.x -= 0.1f;
		}

		if (keys[DIK_D]) {
			cameraPosition.x += 0.1f;
		}

		if (keys[DIK_SPACE]) {
			cameraPosition.y += 0.1f;
		}

		if (keys[DIK_LSHIFT]) {
			cameraPosition.y -= 0.1f;
		}

		if (keys[DIK_UP]) {
			cameraRotation.x += 0.01f;
		}

		if (keys[DIK_DOWN]) {
			cameraRotation.x -= 0.01f;
		}

		if (keys[DIK_LEFT]) {
			cameraRotation.y += 0.01f;
		}

		if (keys[DIK_RIGHT]) {
			cameraRotation.y -= 0.01f;
		}


		gridWorldMatrix = MakeAffineMatrix(gridScale, gridRotate, gridTranslate);
		gridWVPMatrix = Multiply(gridWorldMatrix, Multiply(viewMatrix, projectionMatrix));

		worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

		worldMatrix2 = MakeAffineMatrix(scale2, rotate2, translate2);
		wvpMatrix2 = Multiply(worldMatrix2, Multiply(viewMatrix, projectionMatrix));

		cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);

		// 球と球の当たり判定
		bool isHit = IsSphereCollision(sphere1, sphere2);
		if (isHit) {
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

		DrawSphere(sphere1, wvpMatrix, viewportMtrix, color1);
		DrawSphere(sphere2, wvpMatrix2, viewportMtrix, color2);


		ImGui::Begin("sphere1");
		ImGui::DragFloat3("Scale", &scale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &rotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &translate.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Center", &sphere1.center.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("Radius", &sphere1.radius, -0.01f, 0.0f, 50.0f);
		ImGui::End();

		ImGui::Begin("sphere2");
		ImGui::DragFloat3("Scale", &scale2.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &rotate2.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &translate2.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Center", &sphere2.center.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat("Radius", &sphere2.radius, -0.01f, 0.0f, 50.0f);
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
