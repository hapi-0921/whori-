#include"ScoreManager.h"

#include"Stage.h"
#include"System/Input.h"
#include<imgui.h>

#include"tutorial.h"


ScoreManager::ScoreManager()
{
    sprShader = new Sprite("Data/Sprite/shade.png");
}

ScoreManager::~ScoreManager()
{
    if (sprShader != nullptr) {
        delete sprShader;
        sprShader = nullptr;
    }

}

//提出
void ScoreManager::TargetUpload()
{
    Mouse& mouse = Input::Instance().GetMouse();
    mousePos.x = mouse.GetPositionX();
    mousePos.y = mouse.GetPositionY();

    Stage& stage = Stage::Instance();
    if (stage.stageType == stage.MACHI)targetNum = 48;//foods
    else if (stage.stageType == stage.SIMA) targetNum = 41;//animals

    Tutorial& tutorial = Tutorial::Instance();
    //if (chainCount <= 0) return;
    if (tutorial.isTutorial)
    {
        if (tutorial.tutoType != 6)    return;
    }

    DirectX::XMFLOAT2 pos = { 1626,932 };
    DirectX::XMFLOAT2 size = { 270,77 };

    if (chainCount > 0)
    {
        if (pos.x  < mousePos.x &&//l
            pos.x + size.x > mousePos.x &&//r
            pos.y  < mousePos.y &&//t
            pos.y + size.y  > mousePos.y)  //b
        {
            if (tutorial.isTutorial)
            {
                speed = 0.3f;
            }
            else
            {
                speed = 3.0f;
            }

            texPosY -= speed;

            if (texPosY <= 0.0f)
            {
                upload = true;
                tutorial.tuto6 = true;

                texPosY += speed;
                if (77.0f >= texPosY)  texPosY = 77.0f;
            }
        }
        else
        {
            if (texPosY < 77.0f)
            {

                texPosY += speed;
                if (77.0f >= texPosY)  texPosY = 77.0f;
            }
        }
    }
    if (upload && chainCount != 0)
    {
        nowChain = chainCount;
        reset = true;
        
        if (chainCount == 1 || chainCount == 0)
        {
            chainCount = 0;

            return;
        }
        nowScore = true;//何スコア加算されたか

        chainCount = 0;

        //スコア換算
        if(nowChain>=0)
        score = 100 * allCharCount* comboMultiplier[nowChain-1];//倍率に変更

        allScore += score;
        //最大コンボなら更新
        if (maxCombo < conbo)
        {
            maxCombo = conbo;
        }

    }
    upload = false;

}
void ScoreManager::Render(const RenderContext& rc)
{
    DirectX::XMFLOAT2 pos = { 1626,935 };

    sprShader->Render(rc,
        pos.x, pos.y+ texPosY, 0,
        shadeSize.x, shadeSize.y,
        0, texPosY,
        270, 77,				
        0, 1, 1, 1, 0.7f);

}
void ScoreManager::DrawDebugGUI()
{

    ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
    ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("socore", nullptr, ImGuiWindowFlags_None))
    {
        //ImGui::InputInt("chainCount", &chainCount);

        //ImGui::InputInt("score", &score);
        //ImGui::InputInt("siritoriNum", &siritoriNum);

        //ImGui::InputInt("allCharCount", &allCharCount);
        //ImGui::InputInt("conbo", &conbo);
        //ImGui::InputInt("maxCombo", &maxCombo);
        //ImGui::InputInt("maxChar", &maxChar);
        //ImGui::InputInt("allScore", &allScore);
        ImGui::InputInt("getNum", &getNum);
        ImGui::InputFloat("speed", &speed);

        ImGui::Checkbox("reset", &reset);
    }
    ImGui::End();
}
