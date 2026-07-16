#include"ScoreManager.h"

#include"System/Input.h"
#include<imgui.h>


ScoreManager::ScoreManager()
{
}

ScoreManager::~ScoreManager()
{
}

//提出
void ScoreManager::TargetUpload()
{
    Mouse& mouse = Input::Instance().GetMouse();
    mousePos.x = mouse.GetPositionX();
    mousePos.y = mouse.GetPositionY();

    DirectX::XMFLOAT2 pos = { 1626,932 };
    DirectX::XMFLOAT2 size = { 270,77 };

    if (pos.x  < mousePos.x &&//l
        pos.x + size.x > mousePos.x &&//r
        pos.y  < mousePos.y &&//t
        pos.y + size.y  > mousePos.y)  //b
    {
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            conbo = chainCount-1;
            upload = true;
        }
    }

    if (upload && chainCount != 0)
    {
        nowChain = chainCount;
        if (chainCount == 1 || chainCount == 0)
        {
            return;
        }

        chainCount = 0;
        reset = true;

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
void ScoreManager::DrawDebugGUI()
{

    ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
    ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("socore", nullptr, ImGuiWindowFlags_None))
    {
        ImGui::InputInt("chainCount", &chainCount);

        ImGui::InputInt("score", &score);
        ImGui::InputInt("siritoriNum", &siritoriNum);

        ImGui::InputInt("allCharCount", &allCharCount);
        ImGui::InputInt("conbo", &conbo);
        ImGui::InputInt("maxCombo", &maxCombo);
        ImGui::InputInt("maxChar", &maxChar);
        ImGui::InputInt("allScore", &allScore);

        ImGui::Checkbox("reset", &reset);
    }
}
