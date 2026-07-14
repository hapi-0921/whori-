#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "GameManager.h"

#include"Camera.h"
#include <imgui.h>
#include "imgui_impl_win32.h"
#include <ModelCommon.h>
// 初期化
void SceneLoading::Initialize()
{
	// スプライト初期化
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");
	sprFadeRect = new Sprite("Data/Sprite/FadeRect.png");

	// スレッド開始
	thread = new std::thread(LoadingThread, this);

	ItemNum = rand() % 40; // アイテムの配列番号
}

// 終了化
void SceneLoading::Finalize()
{
	// スレッド終了化
	if (thread != nullptr)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}

	// スプライト終了化
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (sprFadeRect != nullptr)
	{
		delete sprFadeRect;
		sprFadeRect = nullptr;
	}

	targetManager = nullptr;
}

// 更新処理
void SceneLoading::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;




	for (auto& lt : loadingTargets)
	{
		lt.angle.y += 0.5f * elapsedTime;           // 回転

		// Transform更新
		freeUpdateTransform(lt.scale, lt.angle, lt.position, lt.transform);

	}


	// 次のシーンの準備か完了したらシーンを切り替える
	if (nextScene != nullptr)
	{
		if (nextScene->IsReady())
		{
			//SceneManager::Instance().ChangeScene(nextScene);
			SceneManager::Instance().ChangeScene2(nextScene);
			nextScene = nullptr;
		}
	}
}

// 描画処理
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();


	// 2Dスプライト描画
	{
		// 画面右下にローディングアイコンを描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float spriteWidth = 256;
		float spriteHeight = 256;
		float positionX = screenWidth - spriteWidth;
		float positionY = screenHeight - spriteHeight;

		sprFadeRect->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

		sprite->Render(rc,
			positionX, positionY, 0, spriteWidth, spriteHeight,
			angle,
			1, 1, 1, 1);
	}


	//3D
	{
		rc.lightDirection = { 0.0f, -1.0f, 0.0f };

		Camera& camera = Camera::Instance();
		rc.view = camera.GetView();
		rc.projection = camera.GetProjection();

		ModelRenderer* modelRenderer = graphics.GetModelRenderer();

		// 3D描画
		if (targetManager)
		{
			//auto& targets = targetManager->GetTargets();
			auto& lt = loadingTargets[1];
				{
			//auto& target = targets;
				if (lt.model)
				{
					modelRenderer->Render(rc, lt.transform, lt.model, ShaderId::Lambert);
				}
			}
		}
	}
}

// GUI描画
void SceneLoading::DrawGUI()
{
	ImGui::Begin("Loading Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Loading Scene Debug");
	ImGui::Separator();

	ImGui::Text("targetManager: %s", targetManager ? "OK" : "NULL");


	if (targetManager)
	{
		ImGui::Text("Target Count: %zu", targetManager->GetTargetSize());

		ImGui::Separator();
		ImGui::Text("Targets Info:");

		auto& targets = targetManager->GetTargets();
		for (size_t i = 0; i < targets.size(); ++i)
		{
			auto& t = targets[i];
			ImGui::PushID(i);
			if (ImGui::CollapsingHeader(("Target " + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Position: %.1f, %.1f, %.1f", t.position.x, t.position.y, t.position.z);
				ImGui::Text("Angle: %.1f, %.1f, %.1f", t.angle.x, t.angle.y, t.angle.z);
				ImGui::Checkbox("isChainRender", &t.isChainRender);
			}
			ImGui::PopID();
		}
	}
	else
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "TargetManager is not loaded!");
	}

	ImGui::End();

}
// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	// COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);




	scene->targetManager = GameManager::Instance().GetTargetManager();

	if (scene->targetManager)
	{
		auto& src = scene->targetManager->GetTargets();
		scene->loadingTargets.clear();

		for (size_t i = 0; i < src.size(); ++i)
		{
			LoadingTarget lt;
			lt.model = src[i].model;
			lt.position = {};  // 横に並べる
			lt.scale = { 0.1f, 0.1f, 0.1f };                  // ちょうど良い大きさ
			lt.angle = { 0.0f, 0.0f, 0.0f };

			freeUpdateTransform(lt.scale, lt.angle, lt.position, lt.transform);
			scene->loadingTargets.push_back(lt);

		}
	}





	// 次のシーンの初期化を行う
	scene->nextScene->Initialize();

	// ロード時間を伸ばす
	Sleep(4000);

	// スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	// 次のシーンの準備完了設定
	scene->nextScene->SetReandy();

}