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

	//stageTransform
	{
		//ここでステージのサイズ変える
		selectStage.position = { 0, 0, 0 };
		selectStage.angle = { 0, 0, 0 };
		selectStage.scale = { 0.005f, 0.005f, 0.005f };

		//ここでカメラとの距離を変える
		cameraController->range = 0.0f;
	}

	SceneManager::Instance().is_fadeIn = false;
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
	Mouse& mouse = Input::Instance().GetMouse();
	const MouseButton mouseButton = Mouse::BTN_LEFT;
	CursorX = mouse.GetPositionX();
	CursorY = mouse.GetPositionY();

	Stage& stage = Stage::Instance();
	stage.SetCamera(cameraController);
	stage.Update(elapsedTime);

	// ステージを回転させる
	selectStage.angle.y += 0.005f;

	// ステージ変換 //
	// ステージ１が選択されている状態
	if(stageState == stageType::stage1)
	{
		// ステージ切り替え
		selectStage.position.x += (10 - selectStage.position.x) * 0.02f;
		selectStage.scale.x += (0.005f - selectStage.scale.x) * 0.13f;
		selectStage.scale.y += (0.005f - selectStage.scale.y) * 0.13f;
		selectStage.scale.z += (0.005f - selectStage.scale.z) * 0.13f;

		if (selectStage.position.x > 0)
		{
			selectStage.position.x = 0;

			selectStage.scale.x = 0.005f;
			selectStage.scale.y = 0.005f;
			selectStage.scale.z = 0.005f;
		}
	}
	// ステージ2が選択されている状態
	if (stageState == stageType::stage2)
	{
		// ステージ切り替え
		selectStage.position.x += (-10 - selectStage.position.x) * 0.05f;
		selectStage.scale.x += (0.003f - selectStage.scale.x) * 0.05f;
		selectStage.scale.y += (0.003f - selectStage.scale.y) * 0.05f;
		selectStage.scale.z += (0.003f - selectStage.scale.z) * 0.05f;

		if (selectStage.position.x <= -10)
		{
			selectStage.position.x = -10;

			selectStage.scale.x = 0.003f;
			selectStage.scale.y = 0.003f;
			selectStage.scale.z = 0.003f;
		}
	}


	// 左クリックを押したら、
	if (mouse.GetButtonDown() & mouseButton)
	{
		// stage1が選択されている状態　＋　ステージが選択された場合
		if(stageState == stageType::stage1 &&
			CursorX < screenWidth / 2 + screenWidth / 4 && CursorX > screenWidth / 2 - screenWidth / 4)
		{
			GameManager::Instance().CreateTargetManager();

			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
		// stage1が選択されている状態　＋　右の矢印選択された場合
		else if (stageState == stageType::stage1 &&
			CursorX > screenWidth - ArrowSize &&
			CursorY < screenHeight / 2 + ArrowSize / 2 && CursorY > screenHeight / 2 - ArrowSize / 2)
		{
			stageState = stageType::stage2;
		}
		// stage2が選択されている状態　＋　左の矢印選択された場合
		if (stageState == stageType::stage2 &&
			CursorX < ArrowSize &&
			CursorY < screenHeight / 2 + ArrowSize / 2 && CursorY > screenHeight / 2 - ArrowSize / 2)
		{
			stageState = stageType::stage1;
		}
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
		// ステージ１を描画
		Stage& stage1 = Stage::Instance();
		stage1.Render(rc, modelRenderer, &selectStage);
	}

	// 矢印描画（2D）
	{
		sprArrowRight->Render(rc,
			screenWidth - ArrowSize, ArrowH, 0, ArrowSize, ArrowSize,
			0,
			1, 1, 1, 1);
		sprArrowLeft->Render(rc,
			0, ArrowH, 0, ArrowSize, ArrowSize,
			0,
			1, 1, 1, 1);
	}

	cameraController->Render(rc);
}

// GUI描画
void SceneSelect::DrawGUI()
{
	
}