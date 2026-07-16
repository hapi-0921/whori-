#include"UIController.h"
#include "System/Graphics.h"

#include<imgui.h>


#undef min
#undef max

UIController::UIController()
{
	sprChain = new Sprite("Data/Sprite/chain/UI/chain.png"); 

	sprite = new Sprite("Data/Sprite/cursor.png"); 
	centerCusol = new Sprite("Data/Sprite/centerCursor.png");

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
	if (centerCusol != nullptr)
	{
		delete centerCusol;
		centerCusol = nullptr;
	}


}
void UIController::Initialize()
{
	//----------------------------３Ⅾ（モデル）------------------------------------


	//----------------------------２Ⅾ（スプリト）------------------------------------
	Graphics& graphics = Graphics::Instance();
	screenWidth = static_cast<float>(graphics.GetScreenWidth());
	screenHeight = static_cast<float>(graphics.GetScreenHeight());
	{//チェーン
		chainData.dx = screenWidth - 100;
		chainData.dy = 130;
		chainData.dw = 101;
		chainData.dh = 630;
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
		cusolSize = std::min(169.0f, cusolSize + elapsedTime * 650.0f);
		centerSize = std::min(20.0f, centerSize + elapsedTime * 230.0f);
	}
	else
	{
		cusolSize = std::max(130.0f, cusolSize - elapsedTime * 560.0f);
		centerSize = std::max(13.0f, centerSize - elapsedTime * 230.0f);
	}

}
void UIController::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//return;


	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());

	//----------------------------３Ⅾ（モデル）------------------------------------








	//----------------------------２Ⅾ（スプリト）------------------------------------

	{//しりとり表示

		sprChain->Render(rc,
			0, 0, 0,
			1920, 1080, 0,
			1, 1, 1, 1.0f);

	}
	
	if (targetManager == nullptr) return;
	//中央カーソル
	{
		centerCusol->Render(rc,
			screenWidth * 0.5f - centerSize*0.5f, 
			screenHeight * 0.5f - centerSize*0.5f, 0,
			centerSize, centerSize, 0,
			1, 1, 1, 0.7f);
	}
	//カーソル
	{

		sprite->Render(rc,
			screenWidth * 0.5f - cusolSize *0.5f/*- targetManager->cusolPos*/, 
			screenHeight * 0.5f - cusolSize *0.5f/*- targetManager->cusolPos*/, 0,
			cusolSize, cusolSize, 0,
			1, 1, 1, 0.9f);

	}

	{//targetカード表示
		//判定前
		for (int i = 0; i < targetManager->GetTargetSize(); ++i)
		{
			if (targetManager->GetTargetSpri(i) == nullptr)continue;

			Sprite* sp = targetManager->GetTargetSpri(i);

			if (targetManager->GetCarsRen(i))
			{
				float size = 740;
				sp->Render(rc,
					screenWidth*0.5f- size *0.5f, screenHeight*0.5f- size *0.5f, 0,
					size, size, 0,0,
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
					screenWidth - 230, 130 + (renSpan * i), 0,
					130,130,
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

