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

	Vector3 rotate{0.4f, 1.43f, -0.8f};
	Matrix4x4 rotateMatrixX = MakeRotateMatrixX(rotate.x);
	Matrix4x4 rotateMatrixY = MakeRotateMatrixY(rotate.y);
	Matrix4x4 rotateMatrixZ = MakeRotateMatrixZ(rotate.z);
	Matrix4x4 rotateMatrixXYZ = Multiply(rotateMatrixX, Multiply(rotateMatrixY, rotateMatrixZ));
	

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




		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///

		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		Matrix4x4ScreenPrint(0, 0, rotateMatrixX, "RotateMatrixX");
		Matrix4x4ScreenPrint(0, kRowHeight * 5, rotateMatrixY, "RotateMatrixY");
		Matrix4x4ScreenPrint(0, kRowHeight * 10, rotateMatrixZ, "RotateMatrixZ");
		Matrix4x4ScreenPrint(0, kRowHeight * 15, rotateMatrixXYZ, "RotateMatrixXYZ");


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
