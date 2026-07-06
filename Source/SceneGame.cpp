#include "System/Graphics.h"
#include "SceneGame.h"
#include "GameManager.h"
#include"Camera.h"

// 初期化
void SceneGame::Initialize()
{
	sprite = new Sprite("Data/Sprite/cursor.png"); // デバッグspr

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
	targetManager = new TargetManager();

	uiController = new UIController();
	uiController->Initialize();
	uiController->SetTargetManager(targetManager);

	Stage& stage = Stage::Instance();
	stage.SetCamera(cameraController);

	{
		//stageTransform
		gameStage.position = { 0, 0, 0 };
		gameStage.angle = { 0, 0, 0 };
		gameStage.scale = { 1, 1, 1 };
		stage.SetTransform(&gameStage);

		cameraController->range = cameraController->GetMaxRanget();
	}

	GameManager::Instance().SetPlaying(true);
}

// 終了化
void SceneGame::Finalize()
{
	if (cameraController != nullptr) {
		delete cameraController;
		cameraController = nullptr;
	}
	if (targetManager != nullptr) {
		delete targetManager;
		targetManager = nullptr;
	}


	//2D
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (uiController != nullptr)
	{
		delete uiController;
		uiController = nullptr;
	}

	GameManager::Instance().SetPlaying(false);
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	if (GameManager::Instance().needCameraReset)
	{
		cameraController->CameraReset();
		GameManager::Instance().needCameraReset = false;
	}

	//GameManager::Instance().SetPlaying(false);でプレイ中かどうか入れる
	if (GameManager::Instance().IsPlaying()) 
	{
		//タイマーを動かす



		cameraController->Update(elapsedTime);
	}

	Stage& stage = Stage::Instance();
	stage.Update(elapsedTime);

	targetManager->Update(elapsedTime);
	uiController->Update(elapsedTime);

}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };

	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3Dモデル描画
	{
		Stage& stage = Stage::Instance();
		stage.Render(rc, modelRenderer, &gameStage);

		targetManager->Render(rc, modelRenderer);

	}

	// 3Dデバッグ描画
	{

	}

	// 2Dスプライト描画
	{
		uiController->Render(rc);
	}
	// 2Dデバッグ描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
			screenWidth * 0.5f - 50, screenHeight * 0.5f - 50, 0,
			100, 100, 0,
			1, 1, 1, 1);

	}

	cameraController->Render(rc);
}

// GUI描画
void SceneGame::DrawGUI()
{
	// プレイヤーデバッグ描画
	cameraController->DrawDebugGUI();
	targetManager->DrawDebugGUI();
	uiController->DrawDebugGUI();

	Stage& stage = Stage::Instance();
	stage.DrawDebugGUI();
}
