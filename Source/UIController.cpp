#include"UIController.h"
#include "System/Graphics.h"

#include<imgui.h>

UIController::UIController()
{
	sprChain = new Sprite("Data/Sprite/chain/UI/UI.png"); 

	sprite = new Sprite("Data/Sprite/cursor.png"); // デバッグspr

}
UIController::~UIController()
{
	if (sprChain != nullptr) {
		delete sprChain;
		sprChain = nullptr;
	}

	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}


}
void UIController::Initialize()
{
	//----------------------------３Ⅾ（モデル）------------------------------------


	//----------------------------２Ⅾ（スプリト）------------------------------------
	Graphics& graphics = Graphics::Instance();
	screenWidth = static_cast<float>(graphics.GetScreenWidth());
	screenHeight = static_cast<float>(graphics.GetScreenHeight());
	{
		chainData.dx = screenWidth - 100;
		chainData.dy = 130;
		chainData.sx = chainData.sy = 0;
		chainData.sw = 74;
		chainData.sh = 420;
	}
	{
		cardData.dw = cardData.dh = 70;//size
		cardData.sx = chainData.sw;//texPos
		cardData.sy = 0;
		cardData.sw = cardData.sh =190;//texSize
		cardData.dx = screenWidth * 0.5f-(cardData.sw*0.5f);//pos
		cardData.dy = screenHeight * 0.5f-(cardData.sh*0.5f);
	}

}
void UIController::Update(float elapsedTime)
{
	//----------------------------３Ⅾ（モデル）------------------------------------


	//----------------------------２Ⅾ（スプリト）------------------------------------
	if (targetManager == nullptr) return;
	if (targetManager->moveCusol)
	{
		if (targetManager->cusolPos < 100)
		{
			targetManager->cusolPos += elapsedTime*150.0f;
		}
	}
	else
	{
		if (targetManager->cusolPos > 50)
		{
			targetManager->cusolPos -= elapsedTime*150.0f;
		}
	}

}
void UIController::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//return;


	Graphics& graphics = Graphics::Instance();

	//----------------------------３Ⅾ（モデル）------------------------------------








	//----------------------------２Ⅾ（スプリト）------------------------------------

	{//しりとり表示
		sprChain->Render(rc,
			chainData.dx, chainData.dy, 0,
			chainData.sw, chainData.sh, chainData.sx, chainData.sy,
			chainData.sw, chainData.sh, 0, 1, 1, 1, 1);
	}
	
	if (targetManager == nullptr) return;

	//カーソル
	{

		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,//left
			screenWidth * 0.5f - 10- targetManager->cusolPos, screenHeight * 0.5f - 10- targetManager->cusolPos, 0,
			20, 20, 0,
			1, 1, 1, 1);
		sprite->Render(rc,//right
			screenWidth * 0.5f - 10+ targetManager->cusolPos, screenHeight * 0.5f - 10+ targetManager->cusolPos, 0,
			20, 20, 180,
			1, 1, 1, 1);

	}

	{//targetカード表示
		//判定前
		for (int i = 0; i < targetManager->GetTargetSize(); ++i)
		{
			if (targetManager->GetTargetSpri(i) == nullptr)continue;

			Sprite* sp = targetManager->GetTargetSpri(i);

			if (targetManager->GetCarsRen(i))
			{

				sp->Render(rc,
					screenWidth*0.5f-500*0.5f, screenHeight*0.5f-500*0.5f, 0,
					500,500, 0,0,
					750,750, 0, 1, 1, 1, 1);
			}

		}
		//判定後
		for (int i = 0; i < targetManager->GetKeepTargetSize(); ++i) 
		{
			if (targetManager->GetgetTargetSpri(i) == nullptr)continue;

			Sprite* sp = targetManager->GetgetTargetSpri(i);
				//チェーンに入れてく
				sp->Render(rc,
					screenWidth - 100, 130 + (renSpan * i), 0,
				70,70,
					0,0,
					750, 750, 0, 1, 1, 1, 1);
		}
	}	

}

void UIController::DrawDebugGUI()
{
	//return;

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("camera", nullptr, ImGuiWindowFlags_None))
	{
		//折り畳み
		if (ImGui::CollapsingHeader("UIrender", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//for (int i = 0; i < targetManager->GetTargetSize(); ++i)
			//{
				ImGui::Checkbox("moveCusol ",&targetManager->moveCusol);
			//}
		}
	}
	ImGui::End();
}

