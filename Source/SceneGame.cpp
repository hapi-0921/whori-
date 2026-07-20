#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "GameManager.h"
#include"Camera.h"
#include "targetManager.h"
#include"Tutorial.h"
#include "SceneSelect.h"

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

	stage.Initialize();

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
	Tutorial& tutorial = Tutorial::Instance();

	// 画面遷移 //
	GamePad& gamePad = Input::Instance().GetGamePad();
	OptionUI& optionUI = OptionUI::Instance();

	if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
	{
		optionUI.isOption = true;
		optionUI.isHome = true;
	}

	optionUI.UpdateOption(elapsedTime);//設定画面


	tutorial.Update(elapsedTime);
	if (tutorial.toGame)
	{
		tutorial.isTutorial = true;
		tutorial.toGame = false;
	}

	targetManager->Update(elapsedTime);

	//GameManager::Instance().SetPlaying(false);でプレイ中かどうか入れる
	if (GameManager::Instance().IsPlaying())
	{
		if (!tutorial.isTutorial)
		{
			gameTimer += elapsedTime;
			if (gameTimer >= 0.3f)
			{
				GameTimer++;
				gameTimer = 0.0;
			}
			sec = GameTimer % 60;
			min = GameTimer / 60;
		}

		cameraController->Update(elapsedTime);
	}

	if (tutorial.toSelect)//チュートリアルをやめる
	{
		//optionUI.nowGameScene = false;
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
		//tutorial.toSelect = false;
	}


	if (GameManager::Instance().needCameraReset)
	{
		cameraController->CameraReset();
		GameManager::Instance().needCameraReset = false;
	}


	Stage& stage = Stage::Instance();
	stage.Update(elapsedTime);



	uiController->Update(elapsedTime);

	
	if (targetManager->toResult /*|| GameTimer >= 60 * 2*/ )
	{
		optionUI.nowGameScene = false;
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneResult));
	}
	// Update
	debugElapsedTime = elapsedTime;
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

	OptionUI& optionUI = OptionUI::Instance();
	Tutorial& tutorial = Tutorial::Instance();

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
	//設定画面
	optionUI.RenderOption(rc, modelRenderer);
	tutorial.Render(rc, modelRenderer);


	cameraController->Render(rc);
}

// GUI描画
void SceneGame::DrawGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("timer", nullptr, ImGuiWindowFlags_None))
	{

		// DrawGUI
		ImGui::Text("elapsedTime = %.6f", debugElapsedTime);
		ImGui::InputFloat("gameTimer", &gameTimer);
		ImGui::InputInt("gameTimer", &GameTimer);

		ImGui::End();
	}

	OptionUI& optionUI = OptionUI::Instance();
	optionUI.DrawDebugGUI();

	Tutorial& tutorial = Tutorial::Instance();
	tutorial.DrawDebugGUI();

	//Stage& stage = Stage::Instance();
	//stage.DrawDebugGUI();

	//ScoreManager& scoreManager = ScoreManager::Instance();
	//scoreManager.DrawDebugGUI();
	//cameraController->DrawDebugGUI();
	targetManager->DrawDebugGUI();
	//uiController->DrawDebugGUI();
}
