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
	for (int i = 0; i < 5; i++)
	{
		std::string path = "Data/Sprite/rank" + std::to_string(i + 1) + ".png";
		sprrank[i] = new Sprite(path.c_str());
	}	
	for (int i = 0; i < 5; i++)
	{
		std::string path = "Data/Sprite/rankword" + std::to_string(i + 1) + ".png";
		sprrankword[i] = new Sprite(path.c_str());
	}

	sprs= new Sprite("Data/Sprite/chain/foods/Apple.png");
	std::ifstream file("Data/resultData/result.json");
	if (!file)
	{
		return;
	}
	file >> data;
	font = new Font("Data/Font/font2.png");
	Numberfont= new Font("Data/Sprite/number.png");
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
	ScoreManager& scoreManager = ScoreManager::Instance();
	if (scoreManager.allScore <= 1000)
	{
		rankset = 0;
    }
	else if (scoreManager.allScore <= 2000)
	{
		rankset = 1;
	}
	else if (scoreManager.allScore <= 3000)
	{
		rankset = 2;
	}
	else if (scoreManager.allScore <= 4000)
	{
		rankset = 3;
	}
	else 
	{
		rankset = 4;
	}
	if ((mouse.GetButtonDown() & mouseButton)&&next)
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
		sikaku[nowCard].posy += 5;

		if (sikaku[nowCard].posy >= sikaku[nowCard].lastposy)
		{
			nowCard++;
		}
	}
	resultTimer++;
	rankTimer++;
	if (resultTimer>=250&& resultTimer <= 300)
	{
		scorescale = 2.5;
	}
	else
	{
		scorescale = 2;
	}
	if (rankTimer >= 800)
	{
		if (rank < rankset)
		{
			rank++;
		}

		// ランクが確定した瞬間
		if (rank == rankset && !rankFinish)
		{
			rankFinish = true;
			rankScaleAnim = true;
			rankScale = 0.0f;      
		}

		rankTimer = 650;
	}
	if (rankScaleAnim)
	{
		rankScale += 0.008f;

		if (rankScale >= 1.0f)
		{
			rankScale = 1.0f;
			rankScaleAnim = false;
			rankTimer = 0;
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
	ScoreManager& scoreManager = ScoreManager::Instance();
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

			if (resultTimer >= 600)
			{
				float w = 1920 * rankScale;
				float h = 1080 * rankScale;

				float x = (1920 - w) * 0.5f;
				float y = (1080 - h) * 0.5f;

				sprrank[rank]->Render(
					rc,
					x, y, 0,
					w, h,
					0, 0,
					1920, 1080,
					0, 1, 1, 1, 1);
			}
			if (rank == rankset && rankScale == 1)
			{
				if (rankTimer >= 150)
				{
					next = true;
					sprrankword[rankset]->Render(
						rc,
						0, 0, 0,
						1920, 1080,
						0, 0,
						1920, 1080,
						0, 1, 1, 1, 1);

				}
			}


			Numberfont->DrawNumber(
				rc,
				scoreManager.getNum,
				350,
				580,
				1.0f);
			Numberfont->DrawNumber(
				rc,
				scoreManager.siritoriNum,
				750,
				580,
				1.0f);
			Numberfont->DrawNumber(
				rc,
				scoreManager.maxCombo,
				1100,
				580,
				1.0f);
			Numberfont->DrawNumber(
				rc,
				scoreManager.maxChar,
				1500,
				580,
				1.0f);
			if (resultTimer >= 250)
			{

				Numberfont->DrawNumber(
					rc,
					scoreManager.allScore,
					960,
					200,
					scorescale);
			}
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