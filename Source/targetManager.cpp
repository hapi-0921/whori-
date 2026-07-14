#include"targetManager.h"

#include"System/Input.h"

#include"ModelCommon.h"
#include"GameManager.h"

#include<imgui.h>
#include"Stage.h"
#include"Camera.h"
#include <DirectXCollision.h>
#include<DirectXMath.h>
#include "Collision.h"
#include "System/ShapeRenderer.h"
#include "System/Graphics.h"
#include"primitiveRenderer.h"
#include<fstream>
#include <iostream>

#include"UIController.h"

#undef min

#include"SceneGame.h"


std::vector<TargetManager::TargetData> TargetManager::LoadTargets(const std::string& path)
{

    std::ifstream file(path);
    json j;
    file >> j;

    std::vector<TargetManager::TargetData> result;
    if (j.contains("targets") && j["targets"].is_array())
    {
        for (auto& t : j["targets"])
        {
            TargetManager::TargetData data;

            data.modelPath = t.value("model", "");
            data.spritePath = t.value("sprite", "");
            data.startN = t.value("startN", "");
            data.endN = t.value("endN", "");

            if (t.contains("charCount") && t["charCount"].is_number_integer())
            {
                data.charCount = t["charCount"].get<int>();
            }

            if (t.contains("position"))
            {
                data.position.x = t["position"]["x"];
                data.position.y = t["position"]["y"];
                data.position.z = t["position"]["z"];
            }
            data.cardPath = t.value("card", "");

            result.push_back(data);
        }
    }
    return result;
}

TargetManager::TargetManager()
{
    Stage& stage = Stage::Instance();

    //文字数
    mdlChrCount = new Model("Data/Model/target/char/charCircle.mdl");
    std::string jsonPath;

    if (stage.stageType == stage.MACHI) jsonPath = "Data/targetData/target.json";
    else if (stage.stageType == stage.SIMA) jsonPath = "Data/targetData/animals.json";

    auto data = LoadTargets(jsonPath);
    targets.resize(data.size());

    for (size_t i = 0; i < data.size(); ++i)
    {
        targets[i].model = new Model(data[i].modelPath.c_str());
        targets[i].sprite = new Sprite(data[i].spritePath.c_str());

        targets[i].startN = data[i].startN;
        targets[i].endN = data[i].endN;
        targets[i].charCount = data[i].charCount;

        targets[i].pos.x= -data[i].position.x;
        targets[i].pos.y= data[i].position.y;
        targets[i].pos.z= data[i].position.z;

        targets[i].cardPath = data[i].cardPath;

    }
}

TargetManager::~TargetManager()
{
    //return;

	for (auto& t : targets)
	{
        delete t.model;

		delete t.sprite;

        delete t.mdlCard;
        t.mdlCard = nullptr;

	}
    if (mdlChrCount != nullptr) {
        delete mdlChrCount;
        mdlChrCount = nullptr;
    }

    targets.clear();
}


//カーソル内かどうか
bool TargetManager::IsInCursorArea(const DirectX::XMFLOAT2& screenPos, float delta)
{
    Graphics& graphics = Graphics::Instance();
    Stage& stage = Stage::Instance();

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float cx = screenWidth * 0.5f;
    float cy = screenHeight * 0.5f;

    float halfSize = 25.0f + delta * 1.2f;

    return (screenPos.x >= cx - halfSize && screenPos.x <= cx + halfSize &&
        screenPos.y >= cy - halfSize && screenPos.y <= cy + halfSize);
}

void TargetManager::TargetFocus(float elapsedTime)
{
    Stage& stage = Stage::Instance();
    Camera& camera = Camera::Instance();

    // 共通Ray
    DirectX::XMFLOAT3 rayStart = camera.GetEye();
    DirectX::XMFLOAT3 front = camera.GetFront();
    DirectX::XMFLOAT3 rayEnd = rayStart;
    rayEnd.x += front.x * 2000.0f;
    rayEnd.y += front.y * 2000.0f;
    rayEnd.z += front.z * 2000.0f;

    //初期化
    chrCount = 0;
    charRen = false;
    moveCusol = false;


    //ヒット情報
    struct HitInfo {
        float distance = FLT_MAX;
        Target* pTarget = nullptr;
    };

    std::vector<HitInfo> hits;

    // StageRayCast（ズーム制限等のため）
    DirectX::XMFLOAT3 stageHitPos{};
    DirectX::XMFLOAT3 normal{};
    for (int i = 0; i < stage.stageNum; i++)
    {
        if (Collision::RayCast(rayStart, rayEnd,
            stage.GetTransform().transform,
            stage.GetStage(i), stageHitPos, normal))
        {
            float dx = stageHitPos.x - rayStart.x;
            float dy = stageHitPos.y - rayStart.y;
            float dz = stageHitPos.z - rayStart.z;
            distance = sqrtf(dx * dx + dy * dy + dz * dz);

            hits.push_back({ distance, nullptr });
        }
    }

    // ターゲット判定
    for (auto& t : targets)
    {
        t.preFocus = t.isFocus;
        t.isFocus = false;
        t.isRayHit = false;

        if (t.isChainRender) continue;

        //-------------- 面で判定----------
        DirectX::XMFLOAT2 screenPos{};
        bool visible = camera.WorldToScreen(t.pos, screenPos);//target➝2D

        bool inCursor = false;
        if (visible )
        {
            inCursor =IsInCursorArea(screenPos, cusolPos);
        }

        if (!inCursor) continue;  

        // targetRayCast（ズーム制限等のため）
        DirectX::XMFLOAT3 hitPos{};
        if (Collision::RayCast(rayStart, rayEnd, t.transform, t.model, hitPos, normal))
        {
            t.hitPos = hitPos;

            float dx = t.hitPos.x - rayStart.x;
            float dy = t.hitPos.y - rayStart.y;
            float dz = t.hitPos.z - rayStart.z;
            t.distance = sqrtf(dx * dx + dy * dy + dz * dz);

            t.isRayHit = true;
        }
        else
        {
            float dx = t.hitPos.x - rayStart.x;
            float dy = t.hitPos.y - rayStart.y;
            float dz = t.hitPos.z - rayStart.z;
            t.distance = sqrtf(dx * dx + dy * dy + dz * dz);


            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = nonColor;
            }


        }

        hits.push_back({ t.distance, &t });
    }

    if (hits.empty())
    {
        focusTimer = 0.0f;
        distance = FLT_MAX;
        canZoom = false;
        return;
    }

    // 一番近いもの
    std::sort(hits.begin(), hits.end(), [](const HitInfo& a, const HitInfo& b) {
        return a.distance < b.distance;
        });

    //一番近いものの算出（stage,target含む）
    HitInfo closest = hits[0];
    distance = closest.distance;
    canZoom = (distance <= 200);


    // Stageが一番近い
    if (closest.pTarget == nullptr) 
    {
        focusTimer = 0.0f;
        return;
    }


    // ターゲットが近い
    Target& t = *closest.pTarget;

    if (t.distance > 1000.0f)
    {
        focusTimer = 0.0f;
        return;
    }

    //------------- 一番近いtargetだけの処理-----------

    // 文字数表示
    charRen = true;//表示の有無

    tfCharCount.position = t.pos;
    tfCharCount.position.y += 50.0f;
    freeUpdateTransform(tfCharCount.scale, tfCharCount.angle, tfCharCount.position, tfCharCount.transform);
    chrCount = t.charCount;
 
    moveCusol = true;

    if (charRen)//ちかちか
    {

    }

    if (maxDistance > t.distance)   // フォーカス中
    {

        focusTimer += elapsedTime;
        lightTimer += elapsedTime;
        timer -= 0.003f;

        if (lightTimer >= timer * 1.9f)
        {
            lightTimer = 0.0f;
        }
        //チカチカさせる
        if (lightTimer > timer)
        {
            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = minColor;
            }
        }
        else
        {
            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = maxColor;
            }
        }


        if (focusTimer >= 0.5f)//獲得
        {
            GameManager::Instance().SetPlaying(false);


            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = maxColor;


            }
            timer = 0.0f;
            lightTimer = 0.0f;

            if (t.mdlCard == nullptr)//カードの初期値
            {
                t.mdlCard = new Model(t.cardPath.c_str());

                tfCard.angle.x = -stage.centerWall[0].angle.z + DirectX::XM_PI * 0.5f;
                tfCard.angle.y = stage.centerWall[0].angle.y + DirectX::XM_PI * 0.5f;
                tfCard.angle.z = stage.centerWall[0].angle.x;

                tfCard.scale = { 0.5f,1,0.5f };
                tfCard.position = t.pos;
                freeUpdateTransform(tfCard.scale, tfCard.angle, tfCard.position, tfCard.transform);

            }

            t.isFocus = true;


        }
    }
    else
    {
        for (auto& material : t.model->GetResource()->GetMaterials())
        {
            material.emissionColor = nonColor;
        }
        lightTimer = 0.0f;

        focusTimer = 0.0f;
        t.isFocus = false;
    }

    if (t.isFocus)
    {


        DirectX::XMFLOAT3 targetPos;
        targetPos.x = camera.GetEye().x + front.x * 300.0f;
        targetPos.y = camera.GetEye().y + front.y * 300.0f;
        targetPos.z = camera.GetEye().z + front.z * 300.0f;

        float t = 5.0f * elapsedTime;

        tfCard.position.x += (targetPos.x - tfCard.position.x) * t;//正面に向かう
        tfCard.position.y += (targetPos.y - tfCard.position.y) * t;
        tfCard.position.z += (targetPos.z - tfCard.position.z) * t;

        //DirectX::XMVECTOR axis = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&camera.GetFront()));
        //spinAngle += elapsedTime * DirectX::XM_PI * 5.0f;
        //if (spinAngle > DirectX::XM_PI * 2.0f)
        //{
        //    spinAngle -= DirectX::XM_PI * 2.0f;
        //}
        //DirectX::XMMATRIX S =  DirectX::XMMatrixScaling(tfCard.scale.x, tfCard.scale.y, tfCard.scale.z);
        // DirectX::XMMATRIX Billboard =  DirectX::XMMatrixRotationRollPitchYaw(tfCard.angle.x,tfCard.angle.y,tfCard.angle.z);
        // DirectX::XMMATRIX Spin =  DirectX::XMMatrixRotationAxis(axis, spinAngle);
        // DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(tfCard.position.x, tfCard.position.y, tfCard.position.z);
        // DirectX::XMMATRIX W = S * Spin * Billboard * T;

        //DirectX::XMStoreFloat4x4(&tfCard.transform, W);



        freeUpdateTransform(tfCard.scale, tfCard.angle, tfCard.position, tfCard.transform);


        cardTimer += elapsedTime;
    }
    if (cardTimer > 0.65f)
    {
        t.carsRen = true;
    }

    // クリック処理（獲得）
    Mouse& mouse = Input::Instance().GetMouse();
    if (t.carsRen && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
    {

        t.carsRen = false;

        GameManager::Instance().SetPlaying(true);
        cardTimer = 0.0f;

        delete t.mdlCard;
        t.mdlCard = nullptr;

        focusTimer = 0.0f;

        if (chainCount == 0)//最初の文字決定
        {
            endName = t.endN;
            getTargets.push_back(&t);
            t.isChainRender = true;
            chainCount++;
            allCharCount += t.charCount;
        }
        else if (t.startN == endName)//しりとり成功
        {
            chainCount++;
            allCharCount += t.charCount;
            getTargets.push_back(&t);
            t.isChainRender = true;
            endName = t.endN;
        }
        else//しりとり失敗
        {
            tfCard.scale.x -= 0.5f;
            tfCard.scale.y -= 0.5f;
            tfCard.scale.z -= 0.5f;
            freeUpdateTransform(tfCard.scale, tfCard.angle, tfCard.position, tfCard.transform);


            // リセット処理
            allCharCount = 0;
            getTargets.clear();
            chainCount = 0;
            for (auto& target : targets) target.isChainRender = false;

            if (t.endN == "ん")
            {

            }
            GameManager::Instance().needCameraReset = true;

        }
    }
}
void TargetManager::Update(float elapsedTime)
{
    //return;

	//フォーカス判定
	TargetFocus(elapsedTime);

}


void TargetManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	for (auto& t : targets)
	{
		if (!t.isChainRender)//target
		{
			renderer->Render(rc, t.transform, t.model, ShaderId::Lambert);
		}

        //カード　
        if (t.mdlCard != nullptr)
        {
            renderer->Render(rc, tfCard.transform, t.mdlCard, ShaderId::Basic);
        }

	}

    //文字数
    if (charRen)
    {
        Stage& stage = Stage::Instance();
        float spacing = 30.0f;
        float offset = (chrCount - 1) * spacing * 0.5f;

        tfCharCount.angle.y = stage.centerWall[0].angle.y;

        for (int i = 0; i < chrCount; i++)
        {
            Target charTransform = tfCharCount;

            // 中心からのローカル位置
            float localX = i * spacing - offset;

            charTransform.position.z += localX * cosf(tfCharCount.angle.y);
            charTransform.position.x += localX * sinf(tfCharCount.angle.y);

            charTransform.angle.y = stage.centerWall[0].angle.y + DirectX::XM_PI * 0.5f;

            freeUpdateTransform(charTransform.scale, charTransform.angle, charTransform.position, charTransform.transform);

            renderer->Render(rc, charTransform.transform, mdlChrCount, ShaderId::Lambert);
        }
    }
}

void TargetManager::DrawDebugGUI()
{
    //return;

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("targets", nullptr, ImGuiWindowFlags_None))
	{
		//折り畳み
		//for (auto& t : targets)
		//{
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat("t.distance", &targets[4].distance);
            ImGui::InputFloat("t.focusTimer", &focusTimer);
            ImGui::InputFloat("t.cardTimer", &cardTimer);
            ImGui::InputFloat("t.spinAngle", &spinAngle);
            //ImGui::Checkbox("isChainRender", &targets[4].isChainRender);
            //ImGui::Checkbox("t.isFocus", &targets[4].isFocus);
            ImGui::Checkbox("charRen", &charRen);
            ImGui::InputFloat("lightTimer", &lightTimer);
            ImGui::InputInt("t.charCount", &targets[0].charCount);
            ImGui::InputInt("allCharCount", &allCharCount);
            //ImGui::Checkbox("canZoom", &canZoom);
        }
		ImGui::Text("getTargets.size() = %zu", getTargets.size());
		ImGui::InputInt("t.chainCount", &chainCount);

	}
	ImGui::End();
}