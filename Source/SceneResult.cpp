#include "System/Graphics.h"
#include "SceneResult.h"
#include "System/Input.h"
#include "System/Mouse.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Stage.h"
#include"Camera.h"
#include <imgui.h>
#include"targetManager.h"
#include <fstream>
#include <GameManager.h>
#include"SceneSelect.h"
#include"SceneTitle.h"

// 初期化
void SceneResult::Initialize()
{
	sprresult= new Sprite("Data/Sprite/result.png");
	sprresultback= new Sprite("Data/Sprite/resultback.png");
	sprresultback2 = new Sprite("Data/Sprite/resultback2.png");
	sprranking= new Sprite("Data/Sprite/ranking.png");
	sprs= new Sprite("Data/Sprite/chain/foods/Apple.png");
	std::ifstream file("Data/resultData/result.json");
	if (!file)
	{
		return;
	}
	file >> data;
	font = new Font("Data/Font/font2.png");
}

// 終了化
void SceneResult::Finalize()
{
	delete font;
	font = nullptr;
}

// 更新処理
void SceneResult::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	const MouseButton mouseButton = Mouse::BTN_LEFT;
	CursorX = mouse.GetPositionX();
	CursorY = mouse.GetPositionY();

	if ((mouse.GetButtonDown() & mouseButton))
	{
		ranking = true;
	}
	if (ranking)
	{
		if ((mouse.GetButtonDown() & mouseButton))
		{
			if (CursorY >= 912 && CursorY <= 1012)
			{
				//ステージ選択
				if (CursorX >= 148 && CursorX <= 611)
				{
					//今はシーンセレクトないからコメントアウト
					SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
				}
				//もう一度
				else if (CursorX >= 667 && CursorX <=1157)
				{
					GameManager::Instance().CreateTargetManager();

					SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
				}
				//タイトルへ
				else if (CursorX >= 1216 && CursorX <= 1681)
				{
					//今はシーンタイトルないからコメントアウト
					//SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
				}

			}
		}
		

	}
	for (int i = 0; i < data["result"].size(); i++)
	{
		sikaku[i].posx = data["result"][i]["position"]["x"];//初期x位置
		sikaku[i].lastposy = data["result"][i]["position"]["y"];//めっちゃややこいけど上から降ってきた後の着地位置
		sikaku[i].angle = data["result"][i]["position"]["angle"];//初期角度
	}
	if (nowCard < data["result"].size())
	{
		sikaku[nowCard].posy += 4;

		if (sikaku[nowCard].posy >= sikaku[nowCard].lastposy)
		{
			nowCard++;
		}
	}
}

// 描画処理
void SceneResult::Render()
{
	// 描画準備
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	// 2Dスプライト描画
	{
		
		if (!ranking)
		{
			sprresultback->Render(rc,
				0, 0, 0,
				1920, 1080, 0, 0,
				1920, 1080, 0, 1, 1, 1, 1);

			for (int i = 0;i < data["result"].size();i++)
			{
				sprs->Render(rc,
					sikaku[i].posx, sikaku[i].posy, 0,
					225, 225, 0, 0,
					750, 750, sikaku[i].angle, 1, 1, 1, 1);
			}
			sprresultback2->Render(rc,
				0, 0, 0,
				1920, 1080, 0, 0,
				1920, 1080, 0, 1, 1, 1, 1);
			sprresult->Render(rc,
				0, 0, 0,
				1920, 1080, 0, 0,
				1920, 1080, 0, 1, 1, 1, 1);
		}
		else
		{
			sprranking->Render(rc,
				0, 0, 0,
				1920, 1080, 0, 0,
				1920, 1080, 0, 1, 1, 1, 1);
		}
		
		/*font->Draw(
			rc,
			"Hello World!",
			100,
			100,
			10.0f);*/
		// デバッグ用
#ifndef NDEBUG

#endif // NDEBUG

	}

	// 3Dモデル描画
	{
		
	}

}

// GUI描画
void SceneResult::DrawGUI()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("Debug");
	for (int i = 0;i < data["result"].size();i++)
	{
		ImGui::PushID(i);

		ImGui::Text("四角 %d", i);

		ImGui::DragInt("X", &sikaku[i].posx);
		ImGui::DragInt("Y", &sikaku[i].posy);
		ImGui::DragInt("Angle", &sikaku[i].angle);

		ImGui::Separator();

		ImGui::PopID();
	}
	
	ImGui::End();
}