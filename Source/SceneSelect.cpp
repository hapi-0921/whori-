#include "System/Graphics.h"
#include "SceneSelect.h"
#include "System/Input.h"
#include "System/Mouse.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"

// 初期化
void SceneSelect::Initialize()
{
	// スプライト初期化
	sprite = new Sprite("Data/Sprite/Select.png");
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
}

// 更新処理
void SceneSelect::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

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
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	// 描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	// 2Dスプライト描画
	{
		// セレクト画面
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);
	}
}

// GUI描画
void SceneSelect::DrawGUI()
{

}