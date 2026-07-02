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
	
	//フォーカス判定
	TargetFocus();

	//transformの更新
	freeUpdateTransform(targets.scale, targets.angle, targets.position, targets.transform);

}

void TargetManager::TargetFocus()
{
	//カメラ➝もの

	auto Ray = [this]() -> bool 
	{
		Camera& camera = Camera::Instance();
		DirectX::XMFLOAT3 rayStart = camera.GetEye();
		DirectX::XMFLOAT3 rayEnd = rayStart;

		DirectX::XMFLOAT3 front = camera.GetFront();
		rayEnd.x += front.x * 2000.0f;
		rayEnd.y += front.y * 2000.0f;
		rayEnd.z += front.z * 2000.0f;

		DirectX::XMFLOAT3 hit, normal;

		return (Collision::RayCast(rayStart, rayEnd,
			targets.transform, modelTargets, hit, normal));
	};
	auto Distance = [this]() -> bool 
	{
		DirectX::XMFLOAT3 t = targets.position;
		DirectX::XMFLOAT3 e = Camera::Instance().GetEye();

		float dx = t.x - e.x;
		float dy = t.y - e.y;
		float dz = t.z - e.z;

		targets.distance = sqrt(dx * dx + dy * dy + dz * dz);

		return(maxDistance > targets.distance);
	};

	if (Ray() && Distance())
	{
		targets.isFocus = true;
		targets.isRender = true;
		return;
	}

	targets.isFocus = false;
}


void TargetManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	if (!targets.isRender)
	{
		renderer->Render(rc, targets.transform, modelTargets, ShaderId::Lambert);
	}
}

void TargetManager::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("targets", nullptr, ImGuiWindowFlags_None))
	{
		//折り畳み
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat3("rayStart", &rayStart.x);
			ImGui::InputFloat3("rayEnd", &rayEnd.x);
			ImGui::InputFloat("targets.distance", &targets.distance);

			ImGui::Checkbox("targets.isFocus", &targets.isFocus);
		}
	}
	ImGui::End();
}

