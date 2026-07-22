#include"Tutorial.h"

#include<imgui.h>
#include "System/Input.h"
#include "SceneManager.h"

#include"UIController.h"
#include"GameManager.h"

Tutorial::Tutorial()
{

	sprStart = new Sprite("Data/Sprite/tutorial/tutorial.png");
	sprEscape = new Sprite("Data/Sprite/tutorial/escape.png");

	sprRule[0] = new Sprite("Data/Sprite/tutorial/1.png");
	sprRule[1] = new Sprite("Data/Sprite/tutorial/2.png");
	sprRule[2] = new Sprite("Data/Sprite/tutorial/3.png");
	sprRule[3] = new Sprite("Data/Sprite/tutorial/4.png");
	sprRule[4] = new Sprite("Data/Sprite/tutorial/5.png");
	sprRule[5] = new Sprite("Data/Sprite/tutorial/6.png");
	sprRule[6] = new Sprite("Data/Sprite/tutorial/7.png");
	sprRule[7] = new Sprite("Data/Sprite/tutorial/8.png");
	sprRule[8] = new Sprite("Data/Sprite/tutorial/9.png");

	sprClick = new Sprite("Data/Sprite/tutorial/click.png");
	sprArrow = new Sprite("Data/Sprite/tutorial/arrow.png");

	{//初期化
		tutoType = 0;

		tuto2wheel = 0.0f;
		tuto2 = false;
		tuto3drag = 0.0f;
		tuto3 = false;

		tuto4 = false;
		tuto6 = false;

		tuto9Timer = 0.0f;

	}
}

Tutorial::~Tutorial()
{
	if (sprEscape != nullptr) {
		delete sprEscape;
		sprEscape = nullptr;
	}
	if (sprStart != nullptr) {
		delete sprStart;
		sprStart = nullptr;
	}

	for (int i = 0; i < 9; i++)
	{
		delete sprRule[i];
		sprRule[i] = nullptr;
	}
}
void Tutorial::Initialize()
{

}
void Tutorial::Update(float elapsedTime)
{
	OptionUI& optionUI = OptionUI::Instance();
	if (optionUI.isOption) return;

	Mouse& mouse = Input::Instance().GetMouse();
	mousePos.x = mouse.GetPositionX();
	mousePos.y = mouse.GetPositionY();

	if (!optionUI.nowGameScene)
	{
		if (isRectJubge(mousePos.x, mousePos.y, strPos.x, strPos.y, strSize.x, strSize.y))
		{
			strColor = { 0,0,0,1 };
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				toGame = true;
				tutoType = 1;
			}
		}
		else
		{
			strColor = { 0.5f,0.5f,0.5f,1 };
		}
	}
	if (optionUI.nowGameScene && isTutorial)
	{
		NowTutorial(elapsedTime);

		if (isRectJubge(mousePos.x, mousePos.y, endPos.x, endPos.y, endSize.x, endSize.y))
		{
			endColor = {1,1,1,1 };
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				toSelect = true;
				tutoType = 1;
			}
		}
		else
		{
			endColor = { 0.5f,0.5f,0.5f,1 };
		}
	}
}
void Tutorial::NowTutorial(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	ScoreManager& scoreManager = ScoreManager::Instance();


	blinkTimer += elapsedTime;

	if (blinkTimer >= 0.3f) 
	{
		blinkTimer = 0.0f;
		showClick = !showClick;
	}


	if (showClick)	arrowMove += 1.0f;
	else  arrowMove -= 1.0f;
		

	switch (tutoType)
	{
	case 1:
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT) tutoType = 2;
		
		break;
	case 2:
		if (tuto2)
		{
			tutoType = 3;
		}
		break;
	case 3:
		tuto2wheel = 0.0f;
		tuto2 = false;

		if (tuto3)
		{
			tutoType = 4;
		}
		break;
	case 4:
		tuto3drag = 0.0f;
		tuto3 = false;

		if (tuto4)
		{
			tutoType = 5;
		}
		break;
	case 5:
		tuto4 = false;

		if (mouse.GetButtonDown() & Mouse::BTN_LEFT) tutoType = 6;
		break;
	case 6:
		if (tuto6)
		{
			tutoType = 7;
		}
		break;
	case 7:
		tuto6 = false;
		if (!tuto6&&mouse.GetButtonUp() & Mouse::BTN_LEFT) tutoType = 8;
		break;
	case 8:
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT) tutoType = 9;
		break;
	case 9:
		tuto9Timer += elapsedTime;
		if (tuto9Timer >= 2.0f)
		{
			tutoType = 10;
			tuto9Timer = 0.0f;
		}
		break;

	case 10://チュートリアル終了
		scoreManager.ResetData();

		//GameManager::Instance().ReleaseTargetManager();
		//GameManager::Instance().CreateTargetManager();

		toSelect = true;
		break;

	}
}
void Tutorial::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	OptionUI& optionUI = OptionUI::Instance();
	if (optionUI.isOption) return;

	if (!optionUI.nowGameScene)//select
	{
		sprStart->Render(rc, 0, 0, 0, 1920, 1080, 0, strColor.x, strColor.y, strColor.z, strColor.w);
	}
	else if(optionUI.nowGameScene&& isTutorial)//game
	{
		NowRender(rc);
		sprEscape->Render(rc, 0, 0+ 300, 0, 1920, 1080, 0, endColor.x, endColor.y, endColor.z, endColor.w);
	}
}
void Tutorial::NowRender(const RenderContext& rc)
{
	Mouse& mouse = Input::Instance().GetMouse();

	switch (tutoType)
	{
	case 1:
		sprRule[0]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

		if(showClick)
		sprClick->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

		break;
	case 2:
		sprRule[1]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		break;
	case 3:

		sprRule[2]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

		break;
	case 4:
		sprRule[3]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		break;
	case 5:
		sprRule[4]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

		if (showClick)
		sprClick->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		break;
	case 6:
		sprRule[5]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		sprArrow->Render(rc, 0, -50+arrowMove, 0, 1920, 1080, 0, 1, 1, 1, 1);
		break;
	case 7:
		sprRule[6]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

		if (showClick)
		sprClick->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		break;
	case 8:
		sprRule[7]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

		if (showClick)
		sprClick->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

		break;
	case 9:
		sprRule[8]->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		break;

	case 10://チュートリアル終了
		toSelect = true;
		break;

	}
}

void Tutorial::	DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("timer", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::InputFloat2("mousePos", &mousePos.x);
		ImGui::Checkbox("isTutorial", &isTutorial);

	}
	ImGui::End();


}

