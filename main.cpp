#include <Novice.h>
#include <Matrix4x4Function.h>
#include <Vector3Function.h>

const char kWindowTitle[] = "LE2A_10_キョウ_ゲンソ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	int windowX = 1280;
	int windowY = 720;
	Novice::Initialize(kWindowTitle, windowX, windowY);


	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 v1(1.2f, -3.9f, 2.5f);
	Vector3 v2(2.8f, 0.4f, -1.3f);
	Vector3 cross = Cross(v1, v2);

	Vector3 localVertices[3] = { Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, -1.0f, 0.0f), Vector3(-1.0f, -1.0f, 0.0f) };
	Vector3 scale(1.0f, 1.0f, 1.0f);
	Vector3 rotate(0.0f, 0.0f, 0.0f);
	Vector3 translate(0.0f, 0.0f, 0.0f);
	Vector3 cameraPosition(0.0f, 0.0f, 10.0f);

	Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), cameraPosition);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix); 
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(0.45f, (float)windowX / (float)windowY, 0.1f, 100.0f);
	Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	Matrix4x4 viewportMtrix = MakeViewportMatrix(0, 0, float(windowX), float(windowY),0.0f, 1.0f);
	Vector3 screenVertices[3];


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
			translate.z -= 0.1f;
		} else if (keys[DIK_S]) {
			translate.z += 0.1f;
		}

		if (keys[DIK_A]) {
			translate.x -= 0.1f;
		} else if (keys[DIK_D]) {
			translate.x += 0.1f;
		}

		rotate.y += 0.05f;

		if (rotate.y > 6.28f) {
			rotate.y = 0.0f;
		}

		worldMatrix = MakeAffineMatrix(scale, rotate, translate);
		wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

		for (uint32_t i = 0; i < 3; i++) {
			Vector3 ndcVertex = TransForm(wvpMatrix, localVertices[i]);
			screenVertices[i] = TransForm(viewportMtrix, ndcVertex);
		}


		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///



		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		Vector3ScreenPrint(0, 0, cross, "Cross");

		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y), 
			int(screenVertices[1].x), int(screenVertices[1].y), 
			int(screenVertices[2].x), int(screenVertices[2].y),
			RED, kFillModeSolid);

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
