#include "System/Graphics.h"
#include "SceneSelect.h"
#include "System/Input.h"
#include "System/Mouse.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Stage.h"
#include"Camera.h"
#include "GameManager.h"  
#include"Tutorial.h"

// 初期化
void SceneSelect::Initialize()
{
	// スプライト初期化
	sprite = new Sprite("Data/Sprite/kumo.jpg"); // 背景のスプライト
	sprArrowRight = new Sprite("Data/Sprite/arrowright.png"); // 矢印のスプライト
	sprArrowLeft = new Sprite("Data/Sprite/arrowleft.png"); // 矢印のスプライト

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

	mdlMachi[0] = new Model("Data/Model/stage/machi/matimati/matimati1.mdl");
	mdlMachi[1] = new Model("Data/Model/stage/machi/matimati/matimati2.mdl");
	mdlMachi[2] = new Model("Data/Model/stage/machi/matimati/matimati3.mdl");
	mdlMachi[3] = new Model("Data/Model/stage/machi/matimati/matimati4.mdl");

	mdlShima[0] = new Model("Data/Model/stage/shima/simasima/simasima1.1.mdl");
	mdlShima[1] = new Model("Data/Model/stage/shima/simasima/simasima1.2.mdl");
	mdlShima[2] = new Model("Data/Model/stage/shima/simasima/simasima1.3.mdl");
	mdlShima[3] = new Model("Data/Model/stage/shima/simasima/simasima1.mdl");
	mdlShima[4] = new Model("Data/Model/stage/shima/simasima/simasima2.mdl");
	mdlShima[5] = new Model("Data/Model/stage/shima/simasima/simasima3.mdl");
	mdlShima[6] = new Model("Data/Model/stage/shima/simasima/simasima4.mdl");


	//stageTransform
	{
		//ここでステージのサイズ変える
		selectStage.position = { 0, 0, 0 };
		selectStage.angle = { 0, 0, 0 };
		selectStage.scale = { 0.005f, 0.005f, 0.005f };

		selectStage2.position = { 10, 0, 0 };
		selectStage2.angle = { 0, 0, 0 };
		selectStage2.scale = { 0.003f, 0.003f, 0.003f };

		//ここでカメラとの距離を変える
		cameraController->range = 0.0f;
	}


	SceneManager::Instance().is_fadeIn = false;

	arrowColorRight = 1.0f;
	arrowColorLeft = 1.0f;
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
	if (sprArrowRight != nullptr)
	{
		delete sprArrowRight;
		sprArrowRight = nullptr;
	}
	if (sprArrowLeft != nullptr)
	{
		delete sprArrowLeft;
		sprArrowLeft = nullptr;
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
	Stage& stage = Stage::Instance();

	Tutorial& tutorial = Tutorial::Instance();
	tutorial.Update(elapsedTime);

	OptionUI& optionUI = OptionUI::Instance();
		
	if (tutorial.toSelect)
	{
		stage.stageType = Stage::StageType::MACHI;
		//stage.Initialize();

		tutorial.isTutorial = false;
		tutorial.toSelect = false;
		optionUI.nowGameScene = false;

	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
	{
		optionUI.isOption = true;
		optionUI.isHome = true;
	}
	optionUI.UpdateOption(elapsedTime);//設定画面

	Mouse& mouse = Input::Instance().GetMouse();
	const MouseButton mouseButton = Mouse::BTN_LEFT;
	CursorX = mouse.GetPositionX();
	CursorY = mouse.GetPositionY();


	if (tutorial.toGame)
	{
		optionUI.nowGameScene = true;
		stage.stageType = Stage::StageType::MACHI;
		GameManager::Instance().CreateTargetManager();
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		return;
	}

	if (!optionUI.isOption)//設定を開いてない
	{
		stage.SetCamera(cameraController);
		stage.Update(elapsedTime);

		// ステージを回転させる
		selectStage.angle.y += 0.005f;
		selectStage2.angle.y += 0.005f;

		// ステージ変換 //
		// ステージ１が選択されている状態
		if (stageState == stageType::stage1)
		{
			//stageState = stageType::stage2;
			arrowColorRight = 0.5f;
			stage.stageType = Stage::StageType::MACHI;


			selectStage.position.x += (10 - selectStage.position.x) * 0.02f;
			selectStage.scale.x += (0.005f - selectStage.scale.x) * 0.13f;
			selectStage.scale.y += (0.005f - selectStage.scale.y) * 0.13f;
			selectStage.scale.z += (0.005f - selectStage.scale.z) * 0.13f;

			// �X�e�[�W�؂�ւ�
			selectStage2.position.x += (10 - selectStage2.position.x) * 0.05f;
			selectStage2.scale.x += (0.003f - selectStage2.scale.x) * 0.05f;
			selectStage2.scale.y += (0.003f - selectStage2.scale.y) * 0.05f;
			selectStage2.scale.z += (0.003f - selectStage2.scale.z) * 0.05f;

			if (selectStage.position.x > 0)
			{
				selectStage.position.x = 0;

				selectStage.scale.x = 0.005f;
				selectStage.scale.y = 0.005f;
				selectStage.scale.z = 0.005f;
			}
			if (selectStage2.position.x > 10)
			{
				selectStage2.position.x = 10;

				selectStage2.scale.x = 0.003f;
				selectStage2.scale.y = 0.003f;
				selectStage2.scale.z = 0.003f;
			}
		}
		// ステージ2が選択されている状態
		if (stageState == stageType::stage2)
		{
			//stageState = stageType::stage1;
			arrowColorLeft = 0.5f;
			stage.stageType = Stage::StageType::SIMA;

			selectStage.position.x += (-10 - selectStage.position.x) * 0.05f;
			selectStage.scale.x += (0.003f - selectStage.scale.x) * 0.05f;
			selectStage.scale.y += (0.003f - selectStage.scale.y) * 0.05f;
			selectStage.scale.z += (0.003f - selectStage.scale.z) * 0.05f;

			selectStage2.position.x += (-10 - selectStage2.position.x) * 0.02f;
			selectStage2.scale.x += (0.005f - selectStage2.scale.x) * 0.13f;
			selectStage2.scale.y += (0.005f - selectStage2.scale.y) * 0.13f;
			selectStage2.scale.z += (0.005f - selectStage2.scale.z) * 0.13f;

			if (selectStage.position.x <= -10)
			{
				selectStage.position.x = -10;

				selectStage.scale.x = 0.003f;
				selectStage.scale.y = 0.003f;
				selectStage.scale.z = 0.003f;
			}
			if (selectStage2.position.x <= 0)
			{
				selectStage2.position.x = 0;

				selectStage2.scale.x = 0.005f;
				selectStage2.scale.y = 0.005f;
				selectStage2.scale.z = 0.005f;
			}
		}

		if (mouse.GetButtonDown() & mouseButton)
		{

			if (stageState == stageType::stage1 &&
				CursorX < screenWidth / 2 + screenWidth / 4 && CursorX > screenWidth / 2 - screenWidth / 4)
			{
				optionUI.nowGameScene = true;

				GameManager::Instance().CreateTargetManager();

				//stage.Initialize();
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			}
			if (stageState == stageType::stage2 &&
				CursorX < screenWidth / 2 + screenWidth / 4 && CursorX > screenWidth / 2 - screenWidth / 4)
			{
				optionUI.nowGameScene = true;

				GameManager::Instance().CreateTargetManager();

				//stage.Initialize();
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			}

			if (stageState == stageType::stage1 &&
				CursorX > screenWidth - ArrowSize &&
				CursorY < screenHeight / 2 + ArrowSize / 2 && CursorY > screenHeight / 2 - ArrowSize / 2)
			{
				stageState = stageType::stage2;
			}
			if (stageState == stageType::stage2 &&
				CursorX < ArrowSize &&
				CursorY < screenHeight / 2 + ArrowSize / 2 && CursorY > screenHeight / 2 - ArrowSize / 2)
			{
				stageState = stageType::stage1;
			}
		}
	}

	// カーソルが当たると矢印の色を変える
	// 右の矢印
	if (CursorX > screenWidth - ArrowSize &&
		CursorY < screenHeight / 2 + ArrowSize / 2 && CursorY > screenHeight / 2 - ArrowSize / 2)
	{
		arrowColorRight = 0.5f;
	}
	else
	{
		arrowColorRight = 1.0f;
	}
	// 左の矢印
	if (CursorX < ArrowSize &&
		CursorY < screenHeight / 2 + ArrowSize / 2 && CursorY > screenHeight / 2 - ArrowSize / 2)
	{
		arrowColorLeft = 0.5f;
	}
	else
	{
		arrowColorLeft = 1.0f;
	}
}

// 描画処理
void SceneSelect::Render()
{
	// 描画準備
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	OptionUI& optionUI = OptionUI::Instance();
	Tutorial& tutorial = Tutorial::Instance();

	// 2Dスプライト描画
	{
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
		int MachiNum = 4;
		int ShimaNum = 7;

		// ステージ１を描画
		freeUpdateTransform(selectStage.scale, selectStage.angle, selectStage.position, selectStage.transform);
		for (int i = 0; i < MachiNum; i++)
		{
			modelRenderer->Render(rc, selectStage.transform, mdlMachi[i], ShaderId::Lambert);
		}

		// ステージ２を描画
		freeUpdateTransform(selectStage2.scale, selectStage2.angle, selectStage2.position, selectStage2.transform);
		for (int i = 0; i < ShimaNum; i++)
		{
			modelRenderer->Render(rc, selectStage2.transform, mdlShima[i], ShaderId::Lambert);
		}

	}

	// 2D
	{
		if(stageState == stageType::stage1)
		{
			sprArrowRight->Render(rc,
				screenWidth - ArrowSize, ArrowH, 0, ArrowSize, ArrowSize,
				0,
				arrowColorRight, arrowColorRight, arrowColorRight, 1);
		}
		if (stageState == stageType::stage2)
		{
			sprArrowLeft->Render(rc,
				0, ArrowH, 0, ArrowSize, ArrowSize,
				0,
				arrowColorLeft, arrowColorLeft, arrowColorLeft, 1);
		}

		//設定画面
		optionUI.RenderOption(rc, modelRenderer);
		tutorial.Render(rc, modelRenderer);
	}

	cameraController->Render(rc);
}

// GUI描画
void SceneSelect::DrawGUI()
{
	OptionUI& optionUI = OptionUI::Instance();
	optionUI.DrawDebugGUI();

	Tutorial& tutorial = Tutorial::Instance();
	tutorial.DrawDebugGUI();
	
}