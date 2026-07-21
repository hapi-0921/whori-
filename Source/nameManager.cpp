#include"nameManager.h"

#include<imgui.h>
#include "System/Input.h"
#include "SceneManager.h"
#include <windows.h>

#include"UIController.h"

NameManagger::NameManagger()
{
	sprNamePlate = new Sprite("Data/Sprite/name/namePlate.png");
	sprNowTyping = new Sprite("Data/Sprite/name/nowTyping.png");
	sprBar = new Sprite("Data/Sprite/name/bar.png");

	fName = new Font("Data/Font/font2.png");

	Reset();
}
void NameManagger::Reset()
{
	//リザルトで保存してからdelete
	name.clear();
	isChar = false;
	nameYet = false;
	nowName = false;
	cursorBlink = 0;
}
NameManagger::~NameManagger()
{
	if (sprNamePlate != nullptr)
	{
		delete sprNamePlate;
		sprNamePlate = nullptr;
	}
	if (sprNowTyping != nullptr)
	{
		delete sprNowTyping;
		sprNowTyping = nullptr;
	}
	if (sprBar != nullptr)
	{
		delete sprBar;
		sprBar = nullptr;
	}

	delete fName;
	fName = nullptr;

	name.clear();

}
void NameManagger::Initialize()
{

}
void NameManagger::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	mousePos.x = mouse.GetPositionX();
	mousePos.y = mouse.GetPositionY();


	//クリックしてね
	if (isRectJubge(mousePos.x, mousePos.y, 654,570, 669, 98))
	{
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			nowName = true;
		}
	}
	else
	{
		if (name.empty())
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				nowName = false;
			}
		}
	}
	if(nowName)
	{
		cursorBlink = (cursorBlink + 1) % 60;


		for (int vk = 'A'; vk <= 'Z'; ++vk)
		{
			if (IsKeyTrigger(vk))
			{
				if (name.length() < MAX_LENGTH)
					name += (char)vk;
			}
		}

		for (int vk = '0'; vk <= '9'; ++vk)
		{
			if (IsKeyTrigger(vk))
			{
				if (name.length() < MAX_LENGTH)
					name += (char)vk;
			}
		}

		if (IsKeyTrigger(VK_SPACE) && name.length() < MAX_LENGTH)
		{
			name += ' ';
		}
		if (IsKeyTrigger(VK_BACK))//消す
		{
			if (!name.empty())
			{
				name.pop_back();       
			}
		}
	}


	//決定
	if (!name.empty())
	{
		if (isRectJubge(mousePos.x, mousePos.y, 867, 707, 264, 69)||
			IsKeyTrigger(VK_SPACE))
		{
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				nameYet = true;
			}
		}
	}
}

bool NameManagger::IsKeyTrigger(int vk)
{
	static bool prev[256] = {};
	bool current = (GetAsyncKeyState(vk) & 0x8000) != 0;
	bool trigger = current && !prev[vk];
	prev[vk] = current;
	return trigger;
}

void NameManagger::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	if (!nameYet)
	{
		if (!nowName)
		{
			sprNamePlate->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		}
		else
		{
			sprNowTyping->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);


			float baseX = 960.0f - 280.0f;   
			float baseY = 540.0f+60;
			float scale = 1.4f;

			// 文字描画
			fName->Draw(rc, name.c_str(), baseX, baseY, scale);

			int char_w = 5;
			if ((cursorBlink / 20) % 2 == 0)
			{

				sprBar->Render(rc,
					(name.length() * char_w * scale),
					0,
					0,
					1920, 1080,
					0, 1, 1, 1, 1);

			}
		}
	}
}
void NameManagger::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("timer", nullptr, ImGuiWindowFlags_None))
	{

	}
	ImGui::End();


}

