#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "GameManager.h"
#include"Camera.h"
#include "targetManager.h"

// 初期化
void SceneGame::Initialize()
{

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),//fovY
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),//aspect
		0.1f,//nearZ
		3000.0f//farZ
	);

	cameraController = new CameraController();
	//targetManager = new TargetManager();
	targetManager = GameManager::Instance().GetTargetManager();

	uiController = new UIController();
	uiController->Initialize();
	uiController->SetTargetManager(targetManager);
	//targetManager->SetUIController(uiController);

	targetManager->moveCusol = false;
	
	Stage& stage = Stage::Instance();
	stage.SetCamera(cameraController);
	cameraController->SetTargetManager(targetManager);
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
	//if (targetManager != nullptr) {
	//	delete targetManager;
	//	targetManager = nullptr;
	//}

	// delete しない！ 所有権はGameManagerにある
	targetManager = nullptr;

	// アプリケーション終了時やタイトルに戻るとき
	GameManager::Instance().ReleaseTargetManager();

	//2D
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


	Stage& stage = Stage::Instance();
	stage.Update(elapsedTime);

	targetManager->Update(elapsedTime);
	uiController->Update(elapsedTime);

	//GameManager::Instance().SetPlaying(false);でプレイ中かどうか入れる
	if (GameManager::Instance().IsPlaying())
	{
		//タイマーを動かす



		cameraController->Update(elapsedTime);
	}


	// 画面遷移 //
	GamePad& gamePad = Input::Instance().GetGamePad();
	// Zキーを押したらフェードインスタート
	const GamePadButton ZKey = GamePad::BTN_A;

	if (gamePad.GetButtonDown() & ZKey)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneResult));
	}

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
		uiController->Render(rc, modelRenderer);
	}
	// 2Dデバッグ描画
	{

	}

	cameraController->Render(rc);
}

// GUI描画
void SceneGame::DrawGUI()
{
	Stage& stage = Stage::Instance();
	stage.DrawDebugGUI();

	cameraController->DrawDebugGUI();
	targetManager->DrawDebugGUI();
	uiController->DrawDebugGUI();

}
