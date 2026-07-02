#include"targetManager.h"

#include"ModelCommon.h"
#include<imgui.h>
#include"Camera.h"
#include <DirectXCollision.h>
#include "Collision.h"
#include "System/ShapeRenderer.h"
#include "System/Graphics.h"
#include"primitiveRenderer.h"


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
	TargetFocus();
	freeUpdateTransform(targets.scale, targets.angle, targets.position, targets.transform);

}

void TargetManager::TargetFocus()
{
	Camera& camera = Camera::Instance();
	rayStart = camera.GetEye();
	rayEnd = rayStart;
	rayEnd.x += camera.GetFront().x * 2000.0f;
	rayEnd.y += camera.GetFront().y * 2000.0f;
	rayEnd.z += camera.GetFront().z * 2000.0f;

	DirectX::XMFLOAT3 hit, normal;

	if (Collision::RayCast(rayStart, rayEnd,
		targets.transform, modelTargets, hit, normal))
	{
		targets.isFocus = true;
		return;
	}

	targets.isFocus = false;
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
			ImGui::InputFloat3("rayStart", &rayStart.x);
			ImGui::InputFloat3("rayEnd", &rayEnd.x);

			ImGui::Checkbox("targets.isFocus", &targets.isFocus);

		}
	}
	ImGui::End();
}

