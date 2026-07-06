#include"SceneManager.h"
#include <System/Graphics.h>

// 更新処理
void SceneManager::Update(float elapsedTime)
{
	switch (fade_state)
	{
	case FADE_OUT:

		SprFade += 0.01f;
		//SprFade += (1.0f - SprFade) * 0.05f;
		
		if (SprFade >= 0.95f)
		{
			SprFade = 1.0f;
			if (nextScene != nullptr)
			{
				// 古いシーンを終了処理
				Clear();

				// 新しいシーンを設定
				currentScene = nextScene;
				nextScene = nullptr;

				// シーン初期化処理
				if (!currentScene->IsReady())
				{
					currentScene->Initialize();
				}
			}
			fade_state = FADE_IN;
		}
		break;

	case FADE_IN:
		if (nextScene != nullptr)
		{
			// 古いシーンを終了処理
			Clear();

			// 新しいシーンを設定
			currentScene = nextScene;
			nextScene = nullptr;

			// シーン初期化処理
			if (!currentScene->IsReady())
			{
				currentScene->Initialize();
			}
		}

		SprFade -= 0.01f;
		//SprFade += (0.0f - SprFade) * 0.05f;


		if (SprFade <= 0.05f)
		{
			SprFade = 0.0f;
			fade_state = FADE_NONE;
		}

		[[fallthrough]];
		/* fallthrough */

	case FADE_NONE:
		if (currentScene != nullptr)
		{
			currentScene->Update(elapsedTime);
		}
		break;
	}


	//if (nextScene != nullptr)
	//{
	//	// 古いシーンを終了処理
	//	Clear();

	//	// 新しいシーンを設定
	//	currentScene = nextScene;
	//	nextScene = nullptr;

	//	// シーン初期化処理
	//	if (!currentScene->IsReady())
	//	{
	//		currentScene->Initialize();
	//	}
	//}
	//if (currentScene != nullptr)
	//{
	//	currentScene->Update(elapsedTime);
	//}

}

// 描画処理
void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}

	// 2D描画
	{
		Graphics& graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics.GetDeviceContext();
		RenderState* renderState = graphics.GetRenderState();
		ModelRenderer* modelRenderer = graphics.GetModelRenderer();
		RenderContext rc;
		rc.deviceContext = dc;
		rc.renderState = graphics.GetRenderState();

		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		sprFadeRect->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, SprFade);
	}
}

// GUI描画
void SceneManager::DrawGUI()
{
	if (currentScene != nullptr)
	{
		currentScene->DrawGUI();
	}
}

// シーンクリア
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

// シーンクリア
void SceneManager::ChangeScene(Scene* scene)
{
	// 新しいシーンを設定
	nextScene = scene;
	
	SprFade = 0;
	fade_state = FADE::FADE_OUT;
}

void SceneManager::ChangeScene2(Scene* scene)
{
	// 新しいシーンを設定
	nextScene = scene;

	//SprFade = 0.0f;
	//fade_state = FADE::FADE_NONE;
	SprFade = 1.0f;
	fade_state = FADE::FADE_IN;
}

// フェードイン
void SceneManager::FadeIn(float fade)
{
	fade += 0.1f;
}

// フェードアウト
void SceneManager::FadeOut(float fade)
{
	fade += 0.1f;
}