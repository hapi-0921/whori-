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


//提出
void TargetManager::TargetUpload()
{
    Mouse& mouse = Input::Instance().GetMouse();
    
    DirectX::XMFLOAT2 pos = { 1626,932 };
    DirectX::XMFLOAT2 size = { 270,77 };

    if (pos.x  < mousePos.x &&//l
        pos.x + size.x > mousePos.x &&//r
        pos.y  < mousePos.y &&//t
        pos.y + size.y  > mousePos.y)  //b
    {
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
             upload = true;
        }
    }

    if (upload&& chainCount!=0)
    {
        chainCount = 0;

        if (chainCount == 1) return;

        //スコア換算
        resultData.score = 100 * resultData.allCharCount;//コンボ数追加
    }
}

//フォーカス判定
void TargetManager::TargetFocus(float elapsedTime)
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

    DirectX::XMFLOAT3 normal;

    //初期化
    chrCount = 0;
    charRen = false;
    moveCusol = false;

    Mouse& mouse = Input::Instance().GetMouse();
    const MouseButton mouseButton = Mouse::BTN_LEFT;
    mousePos.x = mouse.GetPositionX();
    mousePos.y = mouse.GetPositionY();

    //提出
    TargetUpload();

    // ---------- ヒット情報------------
    struct HitInfo {
        float distance = FLT_MAX;
        Target* pTarget = nullptr;
    };

    std::vector<HitInfo> hits;

    for (int i = 0; i < 4; i++)
    {
        // ステージ
        DirectX::XMFLOAT3 stageHitPos{};
        if (Collision::RayCast(rayStart, rayEnd,
            stage.GetTransform().transform,
            stage.GetStage(i),
            stageHitPos, normal))
        {
            float dx = stageHitPos.x - rayStart.x;
            float dy = stageHitPos.y - rayStart.y;
            float dz = stageHitPos.z - rayStart.z;
            distance = sqrtf(dx * dx + dy * dy + dz * dz);

            hits.push_back({ distance, nullptr });
        }
    }

    // ターゲット全部
    for (auto& t : targets)
    {
        t.preFocus = t.isFocus;
        t.isFocus = false;
        t.isRayHit = false;

        if (t.isChainRender) continue;
        //if (!IsInCursor(t)) continue;

        DirectX::XMFLOAT3 hitPos{};
        if (Collision::RayCast(rayStart, rayEnd, t.transform, t.model, hitPos, normal))
        {
            t.hitPos = hitPos;

            float dx = t.hitPos.x - rayStart.x;
            float dy = t.hitPos.y - rayStart.y;
            float dz = t.hitPos.z - rayStart.z;
            t.distance = sqrtf(dx * dx + dy * dy + dz * dz);

            t.isRayHit = true;

            hits.push_back({ t.distance, &t });
        }
        else
        {
            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = nonColor;
            }
        }
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

    if (closest.pTarget == nullptr)
    {
        // ステージが近い
        focusTimer = 0.0f;
        return;
    }

    // ターゲットが近い
    Target& t = *closest.pTarget;

    if (t.distance > 1000.0f)
    {
        //遠い
        focusTimer = 0.0f;
        t.isFocus = false;
        chrCount = 0;
        charRen = false;
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


    if (maxDistance > t.distance)   // フォーカス中
    {
        focusTimer += elapsedTime;

        //チカチカ
        lightTimer += elapsedTime;
        timer -= 0.004f;
        if (lightTimer >= timer * 1.9f)
        {
            lightTimer = 0.0f;
        }
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


        if (focusTimer >= 0.3f && !t.isFocus)//獲得
        {
            GameManager::Instance().SetPlaying(false);


            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = maxColor;
            }

            timer = 0.15f;
            lightTimer = 0.0f;

            if (t.mdlCard == nullptr)//カードの初期値
            {
                t.mdlCard = new Model(t.cardPath.c_str());

                tfCard.angle.x = -stage.centerWall[0].angle.z + DirectX::XM_PI * 0.5f;
                tfCard.angle.y = stage.centerWall[0].angle.y + DirectX::XM_PI * 0.5f;
                tfCard.angle.z = stage.centerWall[0].angle.x;

                tfCard.scale = { 0.5f,1,0.5f };

                tfCard.position = t.hitPos;

                //tfCard.position.x =camera.GetEye().x + front.x * 300.0f;
                //tfCard.position.y =camera.GetEye().y + front.y * 300.0f;
                //tfCard.position.z =camera.GetEye().z + front.z * 300.0f;

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

        focusTimer = 0.0f;
        t.isFocus = false;
    }

    if (t.isFocus)
    {


        DirectX::XMFLOAT3 targetPos;
        targetPos.x = camera.GetEye().x + front.x * 300.0f;
        targetPos.y = camera.GetEye().y + front.y * 300.0f;
        targetPos.z = camera.GetEye().z + front.z * 300.0f;

        float t = 10.0f * elapsedTime;

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
    if (cardTimer > 0.2f)
    {
        t.carsRen = true;
    }

    // クリック処理（獲得）
    if (t.carsRen && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
    {

        for (auto& material : t.model->GetResource()->GetMaterials())
        {
            material.emissionColor = nonColor;
        }


        if (t.endN == "n")
        {
            toResult = true;
            return;
        }



        t.carsRen = false;

        GameManager::Instance().SetPlaying(true);
        cardTimer = 0.0f;


        focusTimer = 0.0f;

        if (chainCount == 0)//最初の文字決定
        {

            endName = t.endN;
            getTargets.push_back(&t);
            t.isChainRender = true;
            chainCount++;
            resultData.allCharCount += t.charCount;

            t.isMoveToChain = true;


        }
        else if (t.startN == endName)//しりとり成功
        {
            //resultData
            resultData.siritoriNum++;

            chainCount++;
            resultData.allCharCount += t.charCount;
            getTargets.push_back(&t);
            t.isChainRender = true;
            endName = t.endN;

            t.isMoveToChain = true;
        }
        else//しりとり失敗
        {
            tfCard.scale.x -= 0.5f;
            tfCard.scale.y -= 0.5f;
            tfCard.scale.z -= 0.5f;
            freeUpdateTransform(tfCard.scale, tfCard.angle, tfCard.position, tfCard.transform);

            delete t.mdlCard;
            t.mdlCard = nullptr;

            // リセット処理
            //allCharCount = 0;
            getTargets.clear();
            chainCount = 0;
            //for (auto& target : targets) target.isChainRender = false;


            GameManager::Instance().needCameraReset = true;

        }
        delete t.mdlCard;
        t.mdlCard = nullptr;

    }


    Graphics& graphics = Graphics::Instance();
    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    DirectX::XMFLOAT2 goalScreenPos;
    goalScreenPos.x = screenWidth - 100;
    goalScreenPos.y = 130 + (renSpan * getTargets.size());

    if (charRen)
    {
        charRotate += elapsedTime * 3.5f;
        if (charRotate > DirectX::XM_PI * 2.0f)
        {
            charRotate -= DirectX::XM_PI * 2.0f;
        }
    }

    //クリア時の補間
    //if (t.isMoveToChain)
    //{
    //    camera.ScreenToWorld(goalScreenPos, 0.5f, chainPos);
    //    tfCard.position = chainPos;

    //    //tfCard.position.x += (chainPos.x - tfCard.position.x) * 10.0f * elapsedTime;
    //    //tfCard.position.y += (chainPos.y - tfCard.position.y) * 10.0f * elapsedTime;
    //    //tfCard.position.z += (chainPos.z - tfCard.position.z) * 10.0f * elapsedTime;

    //    freeUpdateTransform(tfCard.scale, tfCard.angle, tfCard.position, tfCard.transform);

    //    // 到着
    //    //if (Distance(tfCard.position, goalPos) < 10.0f)
    //    //{
    //    //    t.isMoveToChain = false;

    //    //}
    //}
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

        tfCharCount.angle.y = charRotate;

        for (int i = 0; i < chrCount; i++)
        {
            Target charTransform = tfCharCount;

            // 中心からのローカル位置
            float localX = i * spacing - offset;


            charTransform.position.z += localX * cosf(tfCharCount.angle.y);
            charTransform.position.x += localX * sinf(tfCharCount.angle.y);


            charTransform.angle.y = charRotate + DirectX::XM_PI * 0.5f;

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

	if (ImGui::Begin("socore", nullptr, ImGuiWindowFlags_None))
	{
		//折り畳み
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputInt("allCharCount", &resultData.allCharCount);
        }
		ImGui::Text("getTargets.size() = %zu", getTargets.size());
		ImGui::InputInt("t.chainCount", &chainCount);

	}
	//if (ImGui::Begin("targets", nullptr, ImGuiWindowFlags_None))
	//{
	//	//折り畳み
	//	//for (auto& t : targets)
	//	//{
 //       if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
 //       {
 //           ImGui::InputFloat("t.distance", &targets[4].distance);
 //           ImGui::Checkbox("t.upload.x ", &upload);
 //           ImGui::InputFloat("t.focusTimer", &focusTimer);
 //           ImGui::InputFloat("t.cardTimer", &cardTimer);
 //           ImGui::InputFloat("t.timer", &timer);
 //           //ImGui::Checkbox("isChainRender", &targets[4].isChainRender);
 //           //ImGui::Checkbox("t.isFocus", &targets[4].isFocus);
 //           ImGui::Checkbox("toResult", &toResult);
 //           ImGui::InputFloat("lightTimer", &lightTimer);
 //           ImGui::InputInt("t.charCount", &targets[0].charCount);
 //           ImGui::InputInt("allCharCount", &resultData.allCharCount);
 //           //ImGui::Checkbox("canZoom", &canZoom);
 //       }
	//	ImGui::Text("getTargets.size() = %zu", getTargets.size());
	//	ImGui::InputInt("t.chainCount", &chainCount);

	//}
	ImGui::End();
}