//SceneTitle.cpp
#include <algorithm>
#include<imgui.h>
#include <System/Input.h>
#include "System/Graphics.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Camera.h"

int title_timer;

// 初期化
void SceneTitle::Initialize()
{
	// スプライト初期化
	sprite = new Sprite("Data/Sprite/kumo.png"); // 背景のスプライト
	clicklogo = new Sprite("Data/Sprite/Click_to_Search.png");
	titlelogo = new Sprite("Data/Sprite/titlerogokari.png");
	titleStage = new TitleStage();

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 30, -100),
		DirectX::XMFLOAT3(0, 30, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		2000.0f
	);

	title_timer = 0;
}

// 終了化
void SceneTitle::Finalize()
{
	// スプライト終了化
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}

	if (titlelogo != nullptr)
	{
		delete titlelogo;
		titlelogo = nullptr;
	}

	if (titleStage != nullptr)
	{
		delete titleStage;
		titleStage = nullptr;
	}

}

// 更新処理
void SceneTitle::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	titleStage->Update(elapsedTime);

	const MouseButton mouseButton =
		Mouse::BTN_LEFT;
	if (mouse.GetButtonDown() & mouseButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
	}

	title_timer++;
}

// 描画処理
void SceneTitle::Render()
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

	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	sprite->Render(rc,
		0, 0, 0, screenWidth, screenHeight,
		0,
		1, 1, 1, 1);

	// 3Dモデル描画
	{
		titleStage->Render(rc, modelRenderer);
	}

	titlelogo->Render(rc,
		(screenWidth / 2) - (1980 / 2), (screenHeight / 2) - (350 / 2), 0, 1980, 350,
		0,
		1.0f, 1.0f, 1.0f, 1.0f);

	if (title_timer / 100 % 2 == 0)
	{
		clicklogo->Render(rc,
			(screenWidth / 2) - (1280 / 2), screenHeight - 300, 0, 1280, 200,
			0,
			1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void SceneTitle::DrawGUI()
{

}