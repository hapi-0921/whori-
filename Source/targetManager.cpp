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
#undef max

#include"SceneGame.h"

#include"tutorial.h"

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
            if (t.contains("right"))
            {
                data.right = t.value("right", false);
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

        targets[i].right = data[i].right;

    }

    sprMiss = new Sprite("Data/Sprite/chain/UI/miss.png");
    number = new Font("Data/Sprite/number.png");
    sprConbo= new Sprite("Data/Sprite/conbo.png");
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
    if (sprMiss != nullptr) {
        delete sprMiss;
        sprMiss = nullptr;
    }

    if (mdlChrCount != nullptr) {
        delete mdlChrCount;
        mdlChrCount = nullptr;
    }

    targets.clear();
}


//フォーカス判定
void TargetManager::TargetFocus(float elapsedTime)
{
    Stage& stage = Stage::Instance();
    Camera& camera = Camera::Instance();
    ScoreManager& scoreManager = ScoreManager::Instance();
    Tutorial& tutorial = Tutorial::Instance();

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
    scoreManager.mousePos.x = mouse.GetPositionX();
    scoreManager.mousePos.y = mouse.GetPositionY();

    //提出
    scoreManager.TargetUpload();
    if (scoreManager.reset)
    {
        getTargets.clear();
        scoreManager.reset = false;

        for (auto& t : targets)
        {
            t.carsRen = false;

        }
    }


    if (tutorial.isTutorial)
    {
        if (tutorial.tutoType != 4)    return;
    }
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
        //t.carsRen = false;


        if (t.isChainRender) continue;

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
            if (t.right == true)
            {
                for (auto& material : t.model->GetResource()->GetMaterials())
                {
                    material.emissionColor = lightColor;
                }
            }
            else
            {
                for (auto& material : t.model->GetResource()->GetMaterials())
                {
                    material.emissionColor = nonColor;
                }

            }
        }
    }




    // 一番近いもの
    std::sort(hits.begin(), hits.end(), [](const HitInfo& a, const HitInfo& b) {
        return a.distance < b.distance;
        });


    //一番近いものの算出（stage,target含む）

    if (hits.empty())
    {
        // フォーカスを解除
        distance = FLT_MAX;
        canZoom = false;
        focusTimer = 0.0f;

        // 必要なら全ターゲットのフォーカスも解除
        for (auto& t : targets)
        {
            t.isFocus = false;
        }

        return;
    }
    canZoom = (distance <= 200);

    HitInfo closest = hits[0];
    distance = closest.distance;

    if (hits.empty() || closest.pTarget == nullptr || closest.distance > 1000.0f)
    {
        focusTimer = 0.0f;
        // フォーカス対象がいないときだけリセット
        timer = 0.15f;
        lightTimer = 0.0f;
        canZoom = false;
        return;
    }

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

                tfCard.position.x = t.hitPos.x + front.x * 200.0f;
                tfCard.position.y = t.hitPos.y + front.y * 200.0f;
                tfCard.position.z = t.hitPos.z + front.z * 200.0f;

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

        if (t.right == true)
        {
            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = lightColor;
            }
        }
        else
        {
            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = nonColor;
            }

        }

        focusTimer = 0.0f;
        t.isFocus = false;
    }

    if (t.isFocus)//フォーカス中
    {
        GameManager::Instance().SetPlaying(false);

        DirectX::XMFLOAT3 targetPos;
        targetPos.x = camera.GetEye().x + front.x * 300.0f;
        targetPos.y = camera.GetEye().y + front.y * 300.0f;
        targetPos.z = camera.GetEye().z + front.z * 300.0f;

        float t = 10.0f * elapsedTime;

        tfCard.position.x += (targetPos.x - tfCard.position.x) * t;//正面に向かう
        tfCard.position.y += (targetPos.y - tfCard.position.y) * t;
        tfCard.position.z += (targetPos.z - tfCard.position.z) * t;


        freeUpdateTransform(tfCard.scale, tfCard.angle, tfCard.position, tfCard.transform);


        cardTimer += elapsedTime;
    }

//if (t.isFocus)
//{
//    DirectX::XMFLOAT3 targetPos = {
//        camera.GetEye().x + front.x * 300.0f,
//        camera.GetEye().y + front.y * 300.0f,
//        camera.GetEye().z + front.z * 300.0f
//    };
//
//    float moveSpeed = 10.0f * elapsedTime;
//    tfCard.position.x += (targetPos.x - tfCard.position.x) * moveSpeed;
//    tfCard.position.y += (targetPos.y - tfCard.position.y) * moveSpeed;
//    tfCard.position.z += (targetPos.z - tfCard.position.z) * moveSpeed;
//
//    cardSpinAngle += elapsedTime * 5.0f;
//
//    // ==================== 調整しやすい版 ====================
//    DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&camera.GetEye());
//    DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&tfCard.position);
//    DirectX::XMVECTOR lookDir = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(eye, pos));
//
//    DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
//
//    DirectX::XMMATRIX billboard = DirectX::XMMatrixLookToLH(DirectX::XMVectorZero(), lookDir, up);
//    billboard = DirectX::XMMatrixInverse(nullptr, billboard);
//
//    // 自転
//    DirectX::XMMATRIX spin = DirectX::XMMatrixRotationY(cardSpinAngle);
//
//    DirectX::XMMATRIX baseRotation = DirectX::XMMatrixRotationY(DirectX::XM_PI) *
//        DirectX::XMMatrixRotationX(DirectX::XM_PI * 0.15f); 
//
//     DirectX::XMMATRIX S = DirectX::XMMatrixScaling(tfCard.scale.x, tfCard.scale.y, tfCard.scale.z);
//    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(tfCard.position.x, tfCard.position.y, tfCard.position.z);
//
//    DirectX::XMMATRIX world = S * spin * baseRotation * billboard * T;
//
//    DirectX::XMStoreFloat4x4(&tfCard.transform, world);
//
//    cardTimer += elapsedTime;
//}

    if (cardTimer > 0.2f)
    {
        t.carsRen = true;

        scoreManager.getNum++;

        delete t.mdlCard;
        t.mdlCard = nullptr;

    }

    // 獲得
    if (t.carsRen)
    {
        t.carsRen = false;


        if (t.right == true)
        {
            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = lightColor;
            }
        }
        else
        {
            for (auto& material : t.model->GetResource()->GetMaterials())
            {
                material.emissionColor = nonColor;
            }

        }

        if (scoreManager.maxChar < t.charCount)
        {
            scoreManager.maxChar = t.charCount;
        }


        if (t.endN == "n"&& !tutorial.isTutorial)
        {
            toResult = true;
            return;
        }

        cardTimer = 0.0f;

        focusTimer = 0.0f;

        getTargets.push_back(&t);
        t.isChainRender = true;

        if (tutorial.tutoType == 4)
        {
            tutorial.tuto4 = true;
        }


        if (scoreManager.chainCount == 0)//最初の文字決定
        {
            GameManager::Instance().SetPlaying(true);

            endName = t.endN;

            scoreManager.chainCount++;
            scoreManager.allCharCount += t.charCount;

            t.isMoveToChain = true;

        }
        else if (t.startN == endName)//しりとり成功
        {
            GameManager::Instance().SetPlaying(true);

            //resultData
            scoreManager.siritoriNum++;
            scoreManager.chainCount++;
            scoreManager.conbo = scoreManager.chainCount - 1;
            scoreManager.allCharCount += t.charCount;

            scoreManager.nowCombo = true;//何コンボ中か表示

            endName = t.endN;

            t.isMoveToChain = true;

            if (getTargets.size() > 5)
            {
                for (auto* target : getTargets)
                {
                    target->isShift = true;
                    target->shiftTimer = 0.0f;
                    target->shifted = false;
                }
            }


        }
        else//しりとり失敗
        {
            t.carsRen = false;
            t.isMoveToChain = false;

            delete t.mdlCard;
            t.mdlCard = nullptr;

            scoreManager.chainCount = 0;

            for (auto& t : targets)
            {
                t.carsRen = false;

            }

            nonChain = true;
            t.failTimer = 0.0f;
            t.drawScale = 1.0f;
        }
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
}


float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

void TargetManager::UpdateCardMove(float elapsedTime)
{
    Tutorial& tutorial = Tutorial::Instance();

    if (nonChain)//失敗
    {
        for (int i = 0; i < getTargets.size(); i++)
        {
            Target* t = getTargets[i];

            t->stayTimer += elapsedTime;
            stayTime = 0.1f;

            if (t->stayTimer>= stayTime)
            {
                t->failTimer += elapsedTime;

                if (t->failTimer >= 0.17f)
                {
                    t->drawScale -= elapsedTime * 20.0f;

                }
                if (t->drawScale <= 0.5f)
                {
                    // リセット処理

                    GameManager::Instance().needCameraReset = true;
                    GameManager::Instance().SetPlaying(true);

                    t->failTimer = 0.0f;
                    t->drawScale = 1.0f;
                    getTargets.clear();
                    nonChain = false;
                    getTargets.clear();

                }
            }
        }
        return;
    }

    const float startW = 740.0f;
    const float endW = 130.0f;
    const float startH = 740.0f;
    const float endH = 130.0f;

    float screenW = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenH = static_cast<float>(Graphics::Instance().GetScreenHeight());

    int offset = std::max(0, (int)getTargets.size() - 5);

    for (int i = 0; i < getTargets.size(); i++)
    {
        Target* t = getTargets[i];

        float goalX = screenW - 227.0f;
        float goalY = 90.0f + renSpan * (i - offset);

        if (t->isMoveToChain)
        {
            float speed = 1.8f;
            if (tutorial.isTutorial) speed = 0.4f;

            t->moveTimer += elapsedTime* speed;

            float rate = std::min(t->moveTimer * 5.0f, 1.0f);

            t->cardW = Lerp(startW, endW, rate);
            t->cardH = Lerp(startH, endH, rate);

            t->cardX = Lerp(
                screenW * 0.5f - startW * 0.5f,
                goalX,
                rate);

            t->cardY = Lerp(
                screenH * 0.5f - startH * 0.5f,
                goalY,
                rate);

            if (rate >= 1.0f)
            {
                t->isMoveToChain = false;
                t->moveTimer = 0.0f;
            }
        }
        else
        {
            t->cardX = goalX;
            t->cardW = endW;
            t->cardH = endH;
        }

        // ----- 5枚超えた時 ------
        if (t->isShift)
        {
            t->shiftTimer += elapsedTime;

            // 縮む
            if (t->shiftTimer < 0.08f)
            {
                float r = t->shiftTimer / 0.08f;
                t->drawScale = Lerp(1.0f, 0.0f, r);
            }
            // 大きく
            else if (t->shiftTimer < 0.16f)
            {
                if (!t->shifted)
                {
                    if (displayIndex >= 0)
                    {
                        t->cardY = goalY;
                    }

                    t->shifted = true;
                }

                float r = (t->shiftTimer - 0.08f) / 0.08f;
                t->drawScale = Lerp(0.0f, 1.0f, r);
            }
            // 終了
            else
            {
                t->drawScale = 1.0f;
                t->isShift = false;
                t->shifted = false;
            }
        }
        else
        {
            t->drawScale = 1.0f;
        }
    }
}
void TargetManager::Update(float elapsedTime)
{
    //return;

	//フォーカス判定
	TargetFocus(elapsedTime);
    UpdateCardMove(elapsedTime);

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

//2D
void TargetManager::Render(const RenderContext& rc)
{
    ScoreManager& scoreManager = ScoreManager::Instance();


    float screenW = static_cast<float>(Graphics::Instance().GetScreenWidth());
    float screenH = static_cast<float>(Graphics::Instance().GetScreenHeight());

    for (int i = 0; i < getTargets.size(); i++)
    {
        Target* t = getTargets[i];

        if (!t->sprite) continue;

        float drawW = t->cardW * t->drawScale;
        float drawH = t->cardH * t->drawScale;

        float drawX = t->cardX + (t->cardW - drawW) * 0.5f;
        float drawY = t->cardY + (t->cardH - drawH) * 0.5f;

        t->sprite->Render(
            rc,
            drawX,
            drawY,
            0,
            drawW,
            drawH,
            0, 0,
            750, 750,
            0,
            1, 1, 1, 1);

        if (t->stayTimer >= stayTime&& nonChain)
        {

            sprMiss->Render(rc,
                0, 0, 0,
                1920, 1080, 0,
                1, 1, 1, 1.0f);

        }
    }

    if (scoreManager.nowCombo)
    {
        scoreManager.comboTimer++;
        scoreManager.comboScale -= 0.2;
        number->DrawNumber(rc, scoreManager.conbo, scoreManager.comboPos.x, scoreManager.comboPos.y, scoreManager.comboScale);
        
        if (scoreManager.comboScale < 0.7f)
        {
            scoreManager.nowCombo = false;
            scoreManager.comboScale = 4.0;
        }
    }
    if (scoreManager.nowScore)
    {
        scoreManager.scoreTimer++;
        scoreManager.scoreScale -= 10;

        number->DrawNumber(rc, scoreManager.score, scoreManager.scorePos.x, scoreManager.scorePos.y, scoreManager. scoreScale);

        if (scoreManager.scoreTimer >= 100.0)
        {
            scoreManager.nowScore = false;
            scoreManager.scoreTimer = 0.0f;
        }

    }

    scoreManager.Render(rc);
}


void TargetManager::DrawDebugGUI()
{
    //return;

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    ScoreManager& scoreManager = ScoreManager::Instance();

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
            //ImGui::InputFloat("t.deltaTimer", &deltaTimer);
            //ImGui::Checkbox("isMoveToChain", &targets[2].isMoveToChain);
 //           //ImGui::Checkbox("t.isFocus", &targets[4].isFocus);
 //           ImGui::Checkbox("toResult", &toResult);
 //           ImGui::InputFloat("lightTimer", &lightTimer);
            ImGui::InputInt("conbo", &scoreManager.conbo);
            ImGui::InputInt("score", &scoreManager.score);
 //           ImGui::InputInt("allCharCount", &resultData.allCharCount);
 //           //ImGui::Checkbox("canZoom", &canZoom);
 //       }
	//	ImGui::Text("getTargets.size() = %zu", getTargets.size());
	//	ImGui::InputInt("t.chainCount", &chainCount);

	//}
	//ImGui::End();
}