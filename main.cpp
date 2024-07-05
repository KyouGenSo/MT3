#include <Novice.h>
#include <MatrixFunc.h>
#include <VectorFunc.h>
#include <3DFunc.h>
#include <imgui.h>
#include <ImGuiManager.h>
#include <imgui_impl_dx12.h>

const char kWindowTitle[] = "LE2A_10_キョウ_ゲンソ";

const Vector3 kGravity(0.0f, -9.8f, 0.0f);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	int windowX = 1280;
	int windowY = 720;
	Novice::Initialize(kWindowTitle, windowX, windowY);


	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float deltaTime = 1.0f / 60.0f;

	MassPoint p1{
		Vector3(0.8f, 0.2f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		2.0f,
		0.05f,
		BLUE
	};

	Spring s1{
		Vector3(0.0f, 1.0f, 0.0f),
		0.7f,
		100.0f,
		2.0f
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

		if (keys[DIK_Q] && !preKeys[DIK_Q]) {
			p1.velocity.y -= 5.0f;
		}

		Vector3 diff = p1.position - s1.anchor;
		float length = float(Length(diff));
		if (length != 0.0f) {
			Vector3 dir = Normalize(diff);
			Vector3 restPos = s1.anchor + dir * s1.natrualLength;
			Vector3 diplacement = (p1.position - restPos) * length;
			Vector3 restoringForce = diplacement * -s1.stiffness;
			Vector3 dampingForce = p1.velocity * -s1.damping;
			Vector3 gravityForce = kGravity * p1.mass;
			Vector3 totalForce = restoringForce + dampingForce + gravityForce;
			p1.Acceleration = totalForce / p1.mass;
		}

		p1.velocity += p1.Acceleration * deltaTime;
		p1.position += p1.velocity * deltaTime;

		///-------------------///
		/// ↑更新処理ここまで///
		///-------------------///



		///-------------------///
		/// ↓描画処理ここから///
		///-------------------///

		DrawGrid(viewProjectionMatrix, viewportMtrix);

		DrawSphere(Sphere(p1.position, p1.radius), viewProjectionMatrix, viewportMtrix, p1.color);

		DrawSegment(Segment(s1.anchor, p1.position - s1.anchor), viewProjectionMatrix, viewportMtrix, color1);


		ImGui::Begin("options");

		if (ImGui::BeginTabBar("TabBar")) {
			if (ImGui::BeginTabItem("Grid")) {
				ImGui::DragFloat3("Scale", &gridScale.x, -0.01f, 1.0f, 10.0f);
				ImGui::DragFloat3("Rotate", &gridRotate.x, -0.01f, 0.0f, 6.28f);
				ImGui::DragFloat3("Translate", &gridTranslate.x, -0.01f, -10.0f, 10.0f);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("MassPoint")) {
				ImGui::DragFloat3("Position", &p1.position.x, -0.01f, -10.0f, 10.0f);
				ImGui::DragFloat3("Velocity", &p1.velocity.x, -0.01f, -10.0f, 10.0f);
				ImGui::DragFloat("Mass", &p1.mass, -0.01f, 0.01f, 10.0f);
				ImGui::DragFloat("Radius", &p1.radius, -0.01f, 0.01f, 10.0f);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Spring")) {
				ImGui::DragFloat3("Anchor", &s1.anchor.x, -0.01f, -10.0f, 10.0f);
				ImGui::DragFloat("NatrualLength", &s1.natrualLength, -0.01f, 0.001f, 10.0f);
				ImGui::DragFloat("Stiffness", &s1.stiffness, -1.0f, 0.1f, 100.0f);
				ImGui::DragFloat("Damping", &s1.damping, -0.01f, 0.0f, 10.0f);
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
