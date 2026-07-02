#include "System/Graphics.h"
#include "SceneSelect.h"
#include "System/Input.h"
#include "System/Mouse.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Stage.h"
#include"Camera.h"

// 初期化
void SceneSelect::Initialize()
{
	// スプライト初期化
	sprite = new Sprite("Data/Sprite/Select.png"); // 背景のスプライト

	// ステージ
	Stage& stage = Stage::Instance();

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		2000.0f
	);

	cameraController = new CameraController();

}

// 終了化
void SceneSelect::Finalize()
{
	// スプライト終了化
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}

	// カメラコントローラー終了化
	if (cameraController != nullptr) {
		delete cameraController;
		cameraController = nullptr;
	}
}

// 更新処理
void SceneSelect::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	Stage& stage = Stage::Instance();
	stage.SetCamera(cameraController);
	stage.Update(elapsedTime);

	cameraController->Update(elapsedTime);


	// スケールとか位置とかその他もろもろ調整したいけど、//
	// Stage.cppから変えないといけないので要相談 //


	// 左クリックを押したらローディングシーンへ切り替え
	const MouseButton mouseButton =
		Mouse::BTN_LEFT;
	if (mouse.GetButtonDown() & mouseButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}

// 描画処理
void SceneSelect::Render()
{
	// 描画準備
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 2Dスプライト描画
	{
		// セレクト画面
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

		// デバッグ用
#ifndef NDEBUG
		
#endif // NDEBUG

	}

	// 3Dモデル描画
	{
		// ステージ１を描画
		Stage& stage1 = Stage::Instance();
		stage1.Render(rc, modelRenderer);
	}

	cameraController->Render(rc);
}

// GUI描画
void SceneSelect::DrawGUI()
{

}