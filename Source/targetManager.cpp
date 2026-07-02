#include"targetManager.h"

#include"ModelCommon.h"
#include<imgui.h>


TargetManager::TargetManager()
{
	modelTargets = new Model("Data/Model/target/clock.mdl");
}

TargetManager::~TargetManager()
{
	delete modelTargets;
}

void TargetManager::Update(float elapsedTime)
{
	freeUpdateTransform(targets.scale, targets.angle, targets.position, targets.transform);

}
void TargetManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, targets.transform, modelTargets, ShaderId::Lambert);
}
void TargetManager::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("targets", nullptr, ImGuiWindowFlags_None))
	{
		//ê‹ÇËèÙÇ›
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat3("Position", &targets.position.x);

		}
	}
	ImGui::End();
}