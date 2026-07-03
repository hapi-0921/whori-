#include"UIController.h"

#include<imgui.h>

UIController::UIController()
{
	sprCard = new Sprite("Data/Sprite/UI.png");
	sprChain = new Sprite("Data/Sprite/UI.png"); 
}
UIController::~UIController()
{
	if (sprChain != nullptr) {
		delete sprChain;
		sprChain = nullptr;
	}
	if (sprCard != nullptr) {
		delete sprCard;
		sprCard = nullptr;
	}

}
void UIController::Initialize()
{
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
void UIController::Update(float elapsedTime	)
{

	if (targetManager != nullptr)
	{
		for(int i = 0; i < TargetManager::TARGET_MAX; ++i)
		{
			isCard[i] = targetManager->GetIsRender(i);
		}
	}
}
void UIController::Render(const RenderContext& rc)
{
	{//しりとり表示
		sprChain->Render(rc,
			chainData.dx, chainData.dy, 0,
			chainData.sw, chainData.sh, chainData.sx, chainData.sy,
			chainData.sw, chainData.sh, 0, 1, 1, 1, 1);
	}
	{//targetカード表示
		for (int i = -1; i < targetManager->GetKeepTargetSize()+1; ++i)
		{
			if (isCard[i])
			{
				//中心描画
				//sprCard->Render(rc,
				//	cardData.dx, cardData.dy, 0,
				//	cardData.sw, cardData.sh, cardData.sx, cardData.sy,
				//	cardData.sw, cardData.sh, 0, 1, 1, 1, 1);

				//チェーンに入れてく
				sprCard->Render(rc,
					chainData.dx, chainData.dy + (renSpan * (targetManager->GetChainCount()*i)), 0,
					cardData.dw, cardData.dh, cardData.sx, cardData.sy,
					cardData.sw, cardData.sh, 0, 1, 1, 1, 1);
			}
		}
	}
}

void UIController::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("camera", nullptr, ImGuiWindowFlags_None))
	{
		//折り畳み
		if (ImGui::CollapsingHeader("UIrender", ImGuiTreeNodeFlags_DefaultOpen))
		{

			for (int i = 0; i < TargetManager::TARGET_MAX; ++i)
			{
				ImGui::Checkbox("isCard ", &isCard[i]);
			}
		}
	}
	ImGui::End();
}

