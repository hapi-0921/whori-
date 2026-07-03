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
	targets[CLOCK].model= new Model("Data/Model/target/clock.mdl");
	targets[RADIO].model= new Model("Data/Model/target/radio.mdl");

	targets[CLOCK].position = { 0,0,0 };
	targets[RADIO].position = { 100,0,0 };

}
TargetManager::~TargetManager()
{
	for (auto& t : targets)
	{
		delete t.model;
	}
}

void TargetManager::Update(float elapsedTime)
{
	
	//フォーカス判定
	TargetFocus();

	//transformの更新
	for(auto& t : targets)
	{
		freeUpdateTransform(t.scale, t.angle, t.position, t.transform);
	}
}

void TargetManager::TargetFocus()
{
	//カメラ➝ステージ


	//カメラ➝対象物以外の物


	//カメラ➝もの
	for (auto& t : targets)
	{
		auto Ray = [this, &t]() -> bool
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
					t.transform, t.model, hit, normal));
			};
		auto Distance = [this, &t]() -> bool
			{
				DirectX::XMFLOAT3 tPos = t.position;
				DirectX::XMFLOAT3 e = Camera::Instance().GetEye();

				float dx = tPos.x - e.x;
				float dy = tPos.y - e.y;
				float dz = tPos.z - e.z;

				t.distance = sqrt(dx * dx + dy * dy + dz * dz);

				return(maxDistance > t.distance);
			};
		t.preRender = t.isRender;

		//獲得
		if (Ray() && Distance())
		{
			t.isFocus = true;//瞬間のみtrue（=>何回もなる）
			t.isRender = true;//チェーンが外れない限りfalseにならない
		}
		else
		{
			t.isFocus = false;
		}

		//獲得した瞬間
		if(t.preRender !=t.isRender)
		{
			chainCount++;
			keepTargets.push_back(&t);
		}

	}

}
//void TargetManager::TargetFocus()
//{
//	Camera& cam = Camera::Instance();
//	DirectX::XMFLOAT3 rayOrigin = cam.GetEye();
//	DirectX::XMFLOAT3 rayDir = cam.GetFront();   // 正規化されている前提
//
//	for (auto& t : targets)
//	{
//		DirectX::XMFLOAT3 toTarget = {
//			t.transform.position.x - rayOrigin.x,
//			t.transform.position.y - rayOrigin.y,
//			t.transform.position.z - rayOrigin.z
//		};
//
//		float targetDistance = sqrtf(toTarget.x * toTarget.x + toTarget.y * toTarget.y + toTarget.z * toTarget.z);
//		if (targetDistance > maxDistance)
//		{
//			t.isFocus = false;
//			continue;
//		}
//
//		DirectX::XMFLOAT3 rayEnd = rayOrigin;
//		rayEnd.x += rayDir.x * targetDistance * 1.1f;  // 少し余裕を持たせる
//		rayEnd.y += rayDir.y * targetDistance * 1.1f;
//		rayEnd.z += rayDir.z * targetDistance * 1.1f;
//
//		DirectX::XMFLOAT3 hitPoint, hitNormal;
//		float closestDistance = FLT_MAX;
//		bool blocked = false;
//
//		// =========== ステージとの判定 ============
//		if (stageModel)
//		{
//			if (Collision::RayCast(rayOrigin, rayEnd, stageTransform, stageModel, hitPoint, hitNormal))
//			{
//				float d = Distance(rayOrigin, hitPoint);
//				if (d < closestDistance)
//				{
//					closestDistance = d;
//					blocked = true;
//				}
//			}
//		}
//
//		// ===========対象物との判定===========
//		for (const auto& other : targets)
//		{
//			if (&other == &t) continue;  // 自分自身は除外
//
//			if (Collision::RayCast(rayOrigin, rayEnd, other.transform, other.model, hitPoint, hitNormal))
//			{
//				float d = Distance(rayOrigin, hitPoint);
//				if (d < closestDistance)
//				{
//					closestDistance = d;
//					blocked = true;
//				}
//			}
//		}
//
//		// ============対象物自身との判定 ==============
//		bool hitSelf = Collision::RayCast(rayOrigin, rayEnd, t.transform, t.model, hitPoint, hitNormal);
//
//		t.isFocus = hitSelf && !blocked;
//		t.distance = targetDistance;
//	}
//}

void TargetManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for(auto & t : targets)
	{
		if(!t.isRender)
		{
			renderer->Render(rc, t.transform, t.model, ShaderId::Lambert);
		}
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
		for (auto& t : targets)
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::InputFloat("t.distance", &t.distance);
				ImGui::Checkbox("t.isFocus", &t.isFocus);
			}
		}
		ImGui::Text("keepTargets.size() = %zu", keepTargets.size());
		ImGui::InputInt("t.chainCount", &chainCount);

	}
	ImGui::End();
}

