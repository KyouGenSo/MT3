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

	OBB obb1{
		.center = Vector3(0.0f, 0.0f, 0.0f),
		.axis = {
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f)
		},
		.size = Vector3(0.83f, 0.26f, 0.24f)
	};

	OBB obb2{
		.center = Vector3(0.9f, 0.66f, 0.78f),
		.axis = {
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f)
		},
		.size = Vector3(0.5f, 0.37f, 0.5f)
	};

	int color1 = WHITE;
	int color2 = WHITE;

	Vector3 gridScale(1.0f, 1.0f, 1.0f);
	Vector3 gridRotate(0.0f, 0.0f, 0.0f);
	Vector3 gridTranslate(0.0f, 0.0f, 0.0f);

	Vector3 obb1Rotate(0.0f, 0.0f, 0.0f);
	Vector3 obb2Rotate(-0.05f, -2.49f, 0.15f);

	Vector3 cameraPosition(0.0f, 1.9f, -6.49f);
	Vector3 cameraRotation(0.26f, 0.0f, 0.0f);

	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, (float)windowX / (float)windowY, 0.1f, 100.0f);
	Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	Matrix4x4 viewportMtrix = MakeViewportMatrix(0, 0, float(windowX), float(windowY), 0.0f, 1.0f);

	Matrix4x4 gridWorldMatrix;
	Matrix4x4 gridWVPMatrix;

	Matrix4x4 obb1RotateMatrix;
	Matrix4x4 obb1TranslateMatrix;
	Matrix4x4 obb1WorldMatrix;
	Matrix4x4 obb1WVPMatrix;

	Matrix4x4 obb2RotateMatrix;
	Matrix4x4 obb2TranslateMatrix;
	Matrix4x4 obb2WorldMatrix;
	Matrix4x4 obb2WVPMatrix;

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

		obb1RotateMatrix = Multiply(MakeRotateMatrixX(obb1Rotate.x), Multiply(MakeRotateMatrixY(obb1Rotate.y), MakeRotateMatrixZ(obb1Rotate.z)));

		obb1.axis[0].x = obb1RotateMatrix.m[0][0];
		obb1.axis[0].y = obb1RotateMatrix.m[0][1];
		obb1.axis[0].z = obb1RotateMatrix.m[0][2];
		obb1.axis[1].x = obb1RotateMatrix.m[1][0];
		obb1.axis[1].y = obb1RotateMatrix.m[1][1];
		obb1.axis[1].z = obb1RotateMatrix.m[1][2];
		obb1.axis[2].x = obb1RotateMatrix.m[2][0];
		obb1.axis[2].y = obb1RotateMatrix.m[2][1];
		obb1.axis[2].z = obb1RotateMatrix.m[2][2];

		obb1.axis[0] = Normalize(obb1.axis[0]);
		obb1.axis[1] = Normalize(obb1.axis[1]);
		obb1.axis[2] = Normalize(obb1.axis[2]);

		obb1TranslateMatrix = MakeTranslateMatrix(obb1.center);
		obb1WorldMatrix = Multiply(obb1RotateMatrix, obb1TranslateMatrix);
		obb1WVPMatrix = Multiply(obb1WorldMatrix, viewProjectionMatrix);

		obb2RotateMatrix = Multiply(MakeRotateMatrixX(obb2Rotate.x), Multiply(MakeRotateMatrixY(obb2Rotate.y), MakeRotateMatrixZ(obb2Rotate.z)));

		obb2.axis[0].x = obb2RotateMatrix.m[0][0];
		obb2.axis[0].y = obb2RotateMatrix.m[0][1];
		obb2.axis[0].z = obb2RotateMatrix.m[0][2];
		obb2.axis[1].x = obb2RotateMatrix.m[1][0];
		obb2.axis[1].y = obb2RotateMatrix.m[1][1];
		obb2.axis[1].z = obb2RotateMatrix.m[1][2];
		obb2.axis[2].x = obb2RotateMatrix.m[2][0];
		obb2.axis[2].y = obb2RotateMatrix.m[2][1];
		obb2.axis[2].z = obb2RotateMatrix.m[2][2];

		obb2.axis[0] = Normalize(obb2.axis[0]);
		obb2.axis[1] = Normalize(obb2.axis[1]);
		obb2.axis[2] = Normalize(obb2.axis[2]);

		obb2TranslateMatrix = MakeTranslateMatrix(obb2.center);
		obb2WorldMatrix = Multiply(obb2RotateMatrix, obb2TranslateMatrix);
		obb2WVPMatrix = Multiply(obb2WorldMatrix, viewProjectionMatrix);

		// 衝突判定
		bool isCollide = IsCollision(obb1, obb2);

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

		DrawGrid(viewProjectionMatrix, viewportMtrix);

		DrawOBB(obb1, viewProjectionMatrix, viewportMtrix, color1);

		DrawOBB(obb2, viewProjectionMatrix, viewportMtrix, color2);

		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("OBB1");
		ImGui::DragFloat3("Center", &obb1.center.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Size", &obb1.size.x, -0.01f, 0.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &obb1Rotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::End();

		ImGui::Begin("OBB2");
		ImGui::DragFloat3("Center", &obb2.center.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Size", &obb2.size.x, -0.01f, 0.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &obb2Rotate.x, -0.01f, 0.0f, 6.28f);
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
