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

		Matrix4x4 orthographicMatrix = MakeOrthoMatrix(-160.f, 160.f, 200.0f, 300.0f, 0.0f, 1000.0f);
		Matrix4x4 perspectiveMatrix = MakePerspectiveMatrix(0.63f, 1.33f, 0.1f, 1000.0f);
		Matrix4x4 viewportMatrix = MakeViewPortMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);


		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///



		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		Matrix4x4ScreenPrint(0, 0, orthographicMatrix, "Orthographic Matrix");
		Matrix4x4ScreenPrint(0, kRowHeight * 5, perspectiveMatrix, "Perspective Matrix");
		Matrix4x4ScreenPrint(0, kRowHeight * 10, viewportMatrix, "Viewport Matrix");


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
