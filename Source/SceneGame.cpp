#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "GameManager.h"
#include"Camera.h"
#include "targetManager.h"

#include<imgui.h>

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
		gameStage.position = {0, 0, 0 };
		//gameStage.position = {0, -100, -2000 };
		gameStage.angle = { 0, 0, 0 };
		gameStage.scale = { 1, 1, 1 };
		stage.SetTransform(&gameStage);

		cameraController->range = cameraController->GetMaxRanget();
	}


	timer = new Font("Data/Sprite/number.png");
	sprTimer = new Sprite("Data/Sprite/chain/UI/timer.png");

	GameManager::Instance().SetPlaying(true);
}

// 終了化
void SceneGame::Finalize()
{
	if (sprTimer != nullptr) {
		delete sprTimer;
		sprTimer = nullptr;
	}
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

	GameTimer = 0;
	gameTimer = 0.0f;
	sec = 0;
	min = 0;



	GameManager::Instance().SetPlaying(false);
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{


	//GameManager::Instance().SetPlaying(false);でプレイ中かどうか入れる
	if (GameManager::Instance().IsPlaying())
	{

		gameTimer += elapsedTime;
		if (gameTimer >= 0.2f)
		{
			GameTimer++;
			gameTimer = 0.0;
		}
		sec = GameTimer % 60;
		min = GameTimer / 60;

		cameraController->Update(elapsedTime);
	}


	if (GameManager::Instance().needCameraReset)
	{
		cameraController->CameraReset();
		GameManager::Instance().needCameraReset = false;
	}


	Stage& stage = Stage::Instance();
	stage.Update(elapsedTime);

	targetManager->Update(elapsedTime);
	uiController->Update(elapsedTime);



	// 画面遷移 //
	GamePad& gamePad = Input::Instance().GetGamePad();
	
	// Zキーを押したらフェードインスタート
	//const GamePadButton ZKey = GamePad::BTN_A;

	
	if (targetManager->toResult || GameTimer >= 60 * 2 )
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
		targetManager->Render(rc);
		sprTimer->Render(rc,
			0, 0, 0,1920, 1080, 0, 0,1920, 1080, 0, 1, 1, 1, 1);


	}
	// 値描画
	{
		timer->DrawNumber0(rc, min, 150, 950, 2.0f);
		timer->DrawNumber0(rc, sec, 400, 950, 2.0f);
	}

	cameraController->Render(rc);
}

// GUI描画
void SceneGame::DrawGUI()
{
	//return;
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("timer", nullptr, ImGuiWindowFlags_None))
	{

		
		ImGui::InputFloat("gameTimer", &gameTimer);
		ImGui::InputInt("gameTimer", &GameTimer);

		ImGui::End();
	}

	//Stage& stage = Stage::Instance();
	//stage.DrawDebugGUI();

	//ScoreManager& scoreManager = ScoreManager::Instance();
	//scoreManager.DrawDebugGUI();
	//cameraController->DrawDebugGUI();
	//targetManager->DrawDebugGUI();
	//uiController->DrawDebugGUI();
}
