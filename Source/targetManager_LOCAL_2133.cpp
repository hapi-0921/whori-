#include"targetManager.h"

#include"System/Input.h"

#include"ModelCommon.h"
#include"GameManager.h"

#include<imgui.h>
#include"Stage.h"
#include"Camera.h"
#include <DirectXCollision.h>
#include "Collision.h"
#include "System/ShapeRenderer.h"
#include "System/Graphics.h"
#include"primitiveRenderer.h"
#include<fstream>
#include <iostream>


#include"SceneGame.h"

std::vector<TargetManager::TargetData> TargetManager::LoadTargets(const std::string& path)
{
    std::ifstream file(path);
    json j;
    file >> j;

    if (j.contains("firstStartN")) {
        firstName = j["firstStartN"].get<std::string>();
    }

    std::vector<TargetManager::TargetData> result;
    if (j.contains("targets") && j["targets"].is_array())
    {
        for (auto& t : j["targets"])
        {
            TargetManager::TargetData data;

            data.modelPath = t.value("model", "");
            data.spritePath = t.value("sprite", "");
            data.name = t.value("name", "");
            data.startN = t.value("startN", "");
            data.endN = t.value("endN", "");

            if (data.modelPath.empty() || data.name.empty()) {
                std::cout << "Warning: Invalid target data skipped." << std::endl;
                continue;
            }

            result.push_back(data);
        }
    }

    std::cout << "Loaded " << result.size() << " targets from JSON." << std::endl;
    return result;
}

TargetManager::TargetManager()
{
    auto data = LoadTargets("Data/targetData/target.json");

    targets.resize(data.size());

    for (size_t i = 0; i < data.size(); ++i)
    {
        targets[i].model = new Model(data[i].modelPath.c_str());
        targets[i].sprite = new Sprite(data[i].spritePath.c_str());
        targets[i].name = data[i].name;

        targets[i].startN = data[i].startN;
        targets[i].endN = data[i].endN;

        targets[i].position = { 0.0f, 300.0f, 0.0f };
    }
    endName = firstName;
}

TargetManager::~TargetManager()
{
	for (auto& t : targets)
	{
		delete t.model;
	}
	for (auto& t : targets)
	{
		delete t.sprite;
	}
    targets.clear();
}


//フォーカス判定
void TargetManager::TargetFocus()
{

    Stage& stage = Stage::Instance();
    Camera& camera = Camera::Instance();

    // 共通レイ
    DirectX::XMFLOAT3 rayStart = camera.GetEye();
    DirectX::XMFLOAT3 front = camera.GetFront();

    DirectX::XMFLOAT3 rayEnd = rayStart;
    rayEnd.x += front.x * 2000.0f;
    rayEnd.y += front.y * 2000.0f;
    rayEnd.z += front.z * 2000.0f;


    //--------- ステージとの判定 -----------
    bool isStageRayHit = false;
    DirectX::XMFLOAT3 stageHitPos = { 0, 0, 0 };
    DirectX::XMFLOAT3 normal;
    float stgdistance = FLT_MAX;

    if (Collision::RayCast(rayStart, rayEnd,
        stage.GetTransform().transform,
        stage.GetStage(),
        stageHitPos, normal))
    {
        isStageRayHit = true;

        float dx = stageHitPos.x - rayStart.x;
        float dy = stageHitPos.y - rayStart.y;
        float dz = stageHitPos.z - rayStart.z;
        stgdistance = sqrtf(dx * dx + dy * dy + dz * dz);
    }

    //--- 各ターゲットとの判定 （描画中のみ）---
    for (auto& t : targets)
    {
        t.preFocus = t.isFocus;        // 前の状態保存

        if (t.isChainRender)
        {
            t.isFocus = false;
            t.isRayHit = false;
            continue;
        }

        //--------------------未獲得のみ--------------------
        // ターゲットとのレイ
        t.isRayHit = Collision::RayCast(rayStart, rayEnd, t.transform, t.model, t.hitPos, normal);

        if (!t.isRayHit)
        {
            t.isFocus = false;
            continue;
        }

        float dx = t.hitPos.x - rayStart.x;
        float dy = t.hitPos.y - rayStart.y;
        float dz = t.hitPos.z - rayStart.z;
        t.distance = sqrtf(dx * dx + dy * dy + dz * dz);

        // 障害物チェック
        if (isStageRayHit && t.distance > stgdistance)
        {
            // ステージが手前
            t.isFocus = false;
        }
        else if (maxDistance > t.distance)
        {
            //ゲット
            t.isFocus = true;
            //t.isChainRender = true;
        }
        else
        {
            t.isFocus = false;
        }

        Mouse& mouse = Input::Instance().GetMouse();

        // 獲得した瞬間
        if (t.isFocus)
        {
            if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
            {
                if (t.startN == endName)
                {
                    std::cout << "しりとり成功！ ";

                    chainCount++;
                    getTargets.push_back(&t);
                    t.isChainRender = true;

                    endName = t.endN;


                }
                else
                {
                    std::cout << "しりとり失敗... ";

                    getTargets.clear();
                    chainCount = 0;
                    endName = firstName;
                    t.isFocus = false;
                    t.isChainRender = false;

                    if (t.endN == "ん")
                    {

                    }
                }
                GameManager::Instance().needCameraReset = true;
            }
        }

    }
}

void TargetManager::Update(float elapsedTime)
{

	//フォーカス判定
	TargetFocus();

	//transformの更新
	for (auto& t : targets)
	{
		freeUpdateTransform(t.scale, t.angle, t.position, t.transform);
	}
}


void TargetManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (auto& t : targets)
	{
		if (!t.isChainRender)
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
		ImGui::Text("getTargets.size() = %zu", getTargets.size());
		ImGui::InputInt("t.chainCount", &chainCount);

	}
	ImGui::End();
}