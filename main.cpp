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

	Vector3 translate[3] = { 
		Vector3(0.2f, 1.0f, 0.0f), 
		Vector3(0.4f, 0.0f, 0.0f), 
		Vector3(0.3f, 0.0f, 0.0f) 
	};

	Vector3 rotate[3] = {
	Vector3(0.0f, 0.0f, -6.8f),
	Vector3(0.0f, 0.0f, -1.4f),
	Vector3(0.0f, 0.0f, 0.0f)
	};

	Vector3 scale[3] = {
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(1.0f, 1.0f, 1.0f)
	};

	Sphere sphere[3];
	sphere[0].center = Vector3(0.0f, 0.0f, 0.0f);
	sphere[0].radius = 0.05f;
	sphere[1].center = Vector3(0.0f, 0.0f, 0.0f);
	sphere[1].radius = 0.05f;
	sphere[2].center = Vector3(0.0f, 0.0f, 0.0f);
	sphere[2].radius = 0.05f;

	Segment segment[2];

	Matrix4x4 worldMatrix[3];

	int color1 = RED;
	int color2 = GREEN;
	int color3 = BLUE; 

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

		// ワールド行列の計算
		worldMatrix[0] = MakeAffineMatrix(scale[0], rotate[0], translate[0]);
		worldMatrix[1] = MakeAffineMatrix(scale[1], rotate[1], translate[1]);
		worldMatrix[1] = Multiply(worldMatrix[1], worldMatrix[0]);
		worldMatrix[2] = MakeAffineMatrix(scale[2], rotate[2], translate[2]);
		worldMatrix[2] = Multiply(worldMatrix[2], worldMatrix[1]);

		// ワールド行列とビュープロジェクション行列を掛け合わせる
		Matrix4x4 wvpMatrix[3];
		wvpMatrix[0] = Multiply(worldMatrix[0], viewProjectionMatrix);
		wvpMatrix[1] = Multiply(worldMatrix[1], viewProjectionMatrix);
		wvpMatrix[2] = Multiply(worldMatrix[2], viewProjectionMatrix);

		// 球の間に線を引く
		segment[0].origin = Transform(worldMatrix[0], sphere[0].center);
		segment[0].diff = Transform(worldMatrix[1], sphere[1].center) - segment[0].origin;
		segment[1].origin = Transform(worldMatrix[1], sphere[1].center);
		segment[1].diff = Transform(worldMatrix[2], sphere[2].center) - segment[1].origin;

		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///



		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		DrawGrid(viewProjectionMatrix, viewportMtrix);

		DrawSphere(sphere[0], wvpMatrix[0], viewportMtrix, color1);
		DrawSphere(sphere[1], wvpMatrix[1], viewportMtrix, color2);
		DrawSphere(sphere[2], wvpMatrix[2], viewportMtrix, color3);

		DrawSegment(segment[0], viewProjectionMatrix, viewportMtrix, WHITE);
		DrawSegment(segment[1], viewProjectionMatrix, viewportMtrix, WHITE);

		ImGui::Begin("Grid");
		ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
		ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
		ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Begin("Sphere");
		if (ImGui::BeginTabBar("Sphere")) {
			if (ImGui::BeginTabItem("Sphere1")) {
				ImGui::DragFloat3("Translate", &translate[0].x, -0.01f, -10.0f, 10.0f);
				ImGui::DragFloat3("Rotate", &rotate[0].x, -0.01f, 0.0f, 6.28f);
				ImGui::DragFloat3("Scale", &scale[0].x, -0.01f, 1.0f, 10.0f);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Sphere2")) {
				ImGui::DragFloat3("Translate", &translate[1].x, -0.01f, -10.0f, 10.0f);
				ImGui::DragFloat3("Rotate", &rotate[1].x, -0.01f, 0.0f, 6.28f);
				ImGui::DragFloat3("Scale", &scale[1].x, -0.01f, 1.0f, 10.0f);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Sphere3")) {
				ImGui::DragFloat3("Translate", &translate[2].x, -0.01f, -10.0f, 10.0f);
				ImGui::DragFloat3("Rotate", &rotate[2].x, -0.01f, 0.0f, 6.28f);
				ImGui::DragFloat3("Scale", &scale[2].x, -0.01f, 1.0f, 10.0f);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
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
