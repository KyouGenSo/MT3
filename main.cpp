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

	OBB obb{
		.center = Vector3(-1.0f, 0.0f, 0.0f),
		.axis = {
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f)
		},
		.size = Vector3(0.5f, 0.5f, 0.5f)
	};

	Segment segment{
		.origin = Vector3(-0.8f, -0.3f, 0.0f),
		.diff = Vector3(0.5f, 0.5f, 0.0f)
	};

	int color1 = WHITE;
	int color2 = WHITE;

	Vector3 gridScale(1.0f, 1.0f, 1.0f);
	Vector3 gridRotate(0.0f, 0.0f, 0.0f);
	Vector3 gridTranslate(0.0f, 0.0f, 0.0f);

	Vector3 obbRotate(0.0f, 0.0f, 0.0f);

	Vector3 cameraPosition(0.0f, 1.9f, -6.49f);
	Vector3 cameraRotation(0.26f, 0.0f, 0.0f);

	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, (float)windowX / (float)windowY, 0.1f, 100.0f);
	Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	Matrix4x4 viewportMtrix = MakeViewportMatrix(0, 0, float(windowX), float(windowY), 0.0f, 1.0f);

	Matrix4x4 gridWorldMatrix;
	Matrix4x4 gridWVPMatrix;

	Matrix4x4 obbRotateMatrix;
	Matrix4x4 obbTranslateMatrix;
	Matrix4x4 obbWorldMatrix;
	Matrix4x4 obbWorldMatrixInverse;
	Matrix4x4 obbWVPMatrix;


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

		viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

		gridWorldMatrix = MakeAffineMatrix(gridScale, gridRotate, gridTranslate);
		gridWVPMatrix = Multiply(gridWorldMatrix, viewProjectionMatrix);

		obbRotateMatrix = Multiply(MakeRotateMatrixX(obbRotate.x), Multiply(MakeRotateMatrixY(obbRotate.y), MakeRotateMatrixZ(obbRotate.z)));

		obb.axis[0].x = obbRotateMatrix.m[0][0];
		obb.axis[0].y = obbRotateMatrix.m[0][1];
		obb.axis[0].z = obbRotateMatrix.m[0][2];
		obb.axis[1].x = obbRotateMatrix.m[1][0];
		obb.axis[1].y = obbRotateMatrix.m[1][1];
		obb.axis[1].z = obbRotateMatrix.m[1][2];
		obb.axis[2].x = obbRotateMatrix.m[2][0];
		obb.axis[2].y = obbRotateMatrix.m[2][1];
		obb.axis[2].z = obbRotateMatrix.m[2][2];

		obb.axis[0] = Normalize(obb.axis[0]);
		obb.axis[1] = Normalize(obb.axis[1]);
		obb.axis[2] = Normalize(obb.axis[2]);

		obbTranslateMatrix = MakeTranslateMatrix(obb.center);
		obbWorldMatrix = Multiply(obbRotateMatrix, obbTranslateMatrix);
		obbWVPMatrix = Multiply(obbWorldMatrix, viewProjectionMatrix);

		cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), cameraRotation, cameraPosition);
		viewMatrix = Inverse(cameraMatrix);

		// 衝突判定
		obbWorldMatrixInverse = Inverse(obbWorldMatrix);

		bool isCollide = IsCollision(obb, segment, obbWorldMatrixInverse);

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

		DrawOBB(obb, viewProjectionMatrix, viewportMtrix, color1);

		DrawSegment(segment, viewProjectionMatrix, viewportMtrix, color2);

		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("OBB");
		ImGui::DragFloat3("Center", &obb.center.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Size", &obb.size.x, -0.01f, 0.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &obbRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::End();

		ImGui::Begin("Segment");
		ImGui::DragFloat3("Origin", &segment.origin.x, -0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("Diff", &segment.diff.x, -0.01f, -10.0f, 10.0f);
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
