#include"nameManager.h"

#include<imgui.h>
#include "System/Input.h"
#include "SceneManager.h"


NameManagger::NameManagger()
{
	sprNamePlate = new Sprite("Data/Sprite/name/namePlate.png");
	sprNowTyping = new Sprite("Data/Sprite/name/nowTyping.png");
	sprBar = new Sprite("Data/Sprite/name/bar.png");

}

NameManagger::~NameManagger()
{
	if (sprNamePlate != nullptr)
	{
		delete sprNamePlate;
		sprNamePlate = nullptr;
	}
	if (sprNowTyping != nullptr)
	{
		delete sprNowTyping;
		sprNowTyping = nullptr;
	}
	if (sprBar != nullptr)
	{
		delete sprBar;
		sprBar = nullptr;
	}

}
void NameManagger::Initialize()
{

}
bool NameManagger::Update(float elapsedTime)
{


	return true;
}
void NameManagger::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//sprClick->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

}
void NameManagger::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("timer", nullptr, ImGuiWindowFlags_None))
	{

	}
	ImGui::End();


}

