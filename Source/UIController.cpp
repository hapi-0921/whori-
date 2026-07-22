#include"UIController.h"
#include "System/Graphics.h"

#include<imgui.h>
#include "System/Input.h"

#include"GameManager.h"

#include"tutorial.h"
#include"ScoreManager.h"
#include <SceneManager.h>

#include"SceneLoading.h"
#include"SceneTitle.h"
#include"SceneSelect.h"
#include"SceneGame.h"


#undef min
#undef max

UIController::UIController()
{
	sprChain = new Sprite("Data/Sprite/chain/UI/chain.png"); 
	sprChainOver = new Sprite("Data/Sprite/chain/UI/chain_over.png");

	sprite = new Sprite("Data/Sprite/cursor.png"); 
	centerCusol = new Sprite("Data/Sprite/centerCursor.png");

}
UIController::~UIController()
{
	if (sprChain != nullptr) {
		delete sprChain;
		sprChain = nullptr;
	}
	if (sprChainOver != nullptr) {
		delete sprChainOver;
		sprChainOver = nullptr;
	}

	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (centerCusol != nullptr)
	{
		delete centerCusol;
		centerCusol = nullptr;
	}


}
void UIController::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	screenWidth = static_cast<float>(graphics.GetScreenWidth());
	screenHeight = static_cast<float>(graphics.GetScreenHeight());
	{//チェーン
		chainData.dx = screenWidth - 100;
		chainData.dy = 130;
		chainData.dw = 101;
		chainData.dh = 630;
		chainData.sx = chainData.sy = 0;
		chainData.sw = 74;
		chainData.sh = 420;
	}
	{
		cardData.dw = cardData.dh = 740;//size
		cardData.dx = screenWidth * 0.5f - cardData.dw * 0.5f;//pos
		cardData.dy = screenHeight * 0.5f - cardData.dh * 0.5f;
	}


}
void UIController::Update(float elapsedTime)
{
	if (targetManager == nullptr) return;
	if (targetManager->moveCusol)
	{
		cusolSize = std::min(169.0f, cusolSize + elapsedTime * 650.0f);
		centerSize = std::min(20.0f, centerSize + elapsedTime * 230.0f);
	}
	else
	{
		cusolSize = std::max(130.0f, cusolSize - elapsedTime * 560.0f);
		centerSize = std::max(13.0f, centerSize - elapsedTime * 230.0f);
	}





}
void UIController::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//return;


	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	ScoreManager& scoreManager = ScoreManager::Instance();

	//しりとり表示（チェーンの背景）
	if (scoreManager.chainCount <= 5)
	{
		sprChain->Render(rc,
			0, 0, 0,
			1920, 1080, 0,
			1, 1, 1, 1.0f);
	}
	else
	{
		sprChainOver->Render(rc,
			0, 0, 0,
			1920, 1080, 0,
			1, 1, 1, 1.0f);
	}
		
	
	
	if (targetManager == nullptr) return;
	//中央カーソル
	{
		centerCusol->Render(rc,
			screenWidth * 0.5f - centerSize*0.5f, 
			screenHeight * 0.5f - centerSize*0.5f, 0,
			centerSize, centerSize, 0,
			1, 1, 1, 0.7f);
	}
	//カーソル
	{

		sprite->Render(rc,
			screenWidth * 0.5f - cusolSize *0.5f/*- targetManager->cusolPos*/, 
			screenHeight * 0.5f - cusolSize *0.5f/*- targetManager->cusolPos*/, 0,
			cusolSize, cusolSize, 0,
			1, 1, 1, 0.9f);

	}


}

void UIController::DrawDebugGUI()
{

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("camera", nullptr, ImGuiWindowFlags_None))
	{
		//折り畳み
		if (ImGui::CollapsingHeader("UIrender", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//for (int i = 0; i < targetManager->GetTargetSize(); ++i)
			//{
			//bool a = targetManager->GetMoveToChain(2);
			//	ImGui::Checkbox("GetMoveToChain ",&a);
			//}
		}
	}
	ImGui::End();
}



//設定画面の表示
OptionUI::OptionUI()
{
	sprOption = new Sprite("Data/Sprite/option/optionUI.png");

	sprSiritori = new Sprite("Data/Sprite/option/siritori.png");
	sprSubmit = new Sprite("Data/Sprite/option/submit.png");
	sprSousa = new Sprite("Data/Sprite/option/sousa.png");

	sprSHome = new Sprite("Data/Sprite/option/selectHome.png");
	sprGHome = new Sprite("Data/Sprite/option/gameHome.png");
}
OptionUI::~OptionUI()
{
	if (sprOption != nullptr) {
		delete sprOption;
		sprOption = nullptr;
	}
	if (sprSiritori != nullptr) {
		delete sprSiritori;
		sprSiritori = nullptr;
	}
	if (sprSubmit != nullptr) {
		delete sprSubmit;
		sprSubmit = nullptr;
	}
	if (sprSousa != nullptr) {
		delete sprSousa;
		sprSousa = nullptr;
	}
	if (sprSHome != nullptr) {
		delete sprSHome;
		sprSHome = nullptr;
	}
	if (sprGHome != nullptr) {
		delete sprGHome;
		sprGHome = nullptr;
	}
}

bool escape = false;
void OptionUI::UpdateOption(float elapsedTime)
{
	Tutorial& tutorial = Tutorial::Instance();
	ScoreManager& scoreManager = ScoreManager::Instance();
	OptionUI& optionUI = OptionUI::Instance();

	
	Mouse& mouse = Input::Instance().GetMouse();
	mousePos.x = mouse.GetPositionX();
	mousePos.y = mouse.GetPositionY();

	Stage& stage = Stage::Instance();


	if (!nowGameScene)//シーンセレクトの時
	{
		optColor = { 0.3f,0.3f,0.3f,1 };
		optPos = { 1920 - 200  ,20 };

		if (isCircleJubge(mousePos.x, mousePos.y, optPos.x, optPos.y, optSize * 0.5f) &&
			mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			if (isOption&& !isHome)
			{
				isOption = false;
				return;
			}

			isOption = true;
			ruleState = SIRITORI;
		}
	}
	else//シーンゲームの時
	{
		optColor = { 0.7f,0.7f,0.7f,1 };

		optPos={ 50  ,20 };
		DirectX::XMFLOAT2 optPosGame{ 1920 - 200  ,20 };

		if (tutorial.isTutorial)	return;

		if (isCircleJubge(mousePos.x, mousePos.y, optPosGame.x, optPosGame.y, optSize * 0.5f) &&
			mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			if (isOption && !isHome)
			{
				isOption = false;
			}

		}
		else if (isCircleJubge(mousePos.x, mousePos.y, optPos.x, optPos.y, optSize * 0.5f) &&
			mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			isOption = true;
			ruleState = SIRITORI;
		}

	}


	if (isOption)
	{
		GameManager::Instance().SetPlaying(false);
		GamePad& gamePad = Input::Instance().GetGamePad();

		//ルール説明の選択
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT && !isHome)
		{
			if (isRectJubge(mousePos.x, mousePos.y, siriPos.x, siriPos.y, ruleSize.x, ruleSize.y))
				ruleState = SIRITORI;
			else if (isRectJubge(mousePos.x, mousePos.y, subPos.x, subPos.y, ruleSize.x, ruleSize.y))
				ruleState = SUBMIT;
			else if (isRectJubge(mousePos.x, mousePos.y, souPos.x, souPos.y, ruleSize.x, ruleSize.y))
				ruleState = SOUSA;
		}

		//ホームボタン
		if ((isCircleJubge(mousePos.x, mousePos.y, homePos.x, homePos.y, homeSize) &&
			mouse.GetButtonDown() & Mouse::BTN_LEFT))
		{
			if (isHome)	isHome = false;
			else isHome = true;
		}
		if (isHome)
		{

			if (!nowGameScene)//シーンセレクトの時
			{
				//タイトルへ
				{
					DirectX::XMFLOAT2 pos = { 1276,144 };
					DirectX::XMFLOAT2 size = { 368,108 };
					if (isRectJubge(mousePos.x, mousePos.y, pos.x, pos.y, size.x, size.y) &&
						mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						scoreManager.ResetData();

						optionUI.isOption = false;
						optionUI.isHome = false;

						//GameManager::Instance().ReleaseTargetManager();
						//GameManager::Instance().CreateTargetManager();

						SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
					}
				}
				//ゲームをやめる
				{
					DirectX::XMFLOAT2 pos = { 1276,339 };
					DirectX::XMFLOAT2 size = { 368,154 };
					if (isRectJubge(mousePos.x, mousePos.y, pos.x, pos.y, size.x, size.y) &&
						mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						scoreManager.ResetData();

						optionUI.isOption = false;
						optionUI.isHome = false;

						GameManager::Instance().ReleaseTargetManager();

						HWND hWnd = GetActiveWindow();
						PostMessage(hWnd, WM_CLOSE, 0, 0);
					}
				}
			}
			else
			{
				//セレクトへ
				{
					DirectX::XMFLOAT2 pos = { 1256,142 };
					DirectX::XMFLOAT2 size = { 389,92 };
					if (isRectJubge(mousePos.x, mousePos.y, pos.x, pos.y, size.x, size.y) &&
						mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						scoreManager.ResetData();

						optionUI.isOption = false;
						optionUI.isHome = false;

						//GameManager::Instance().ReleaseTargetManager();
						//stage.stageType = Stage::StageType::MACHI;
						//GameManager::Instance().CreateTargetManager();

						SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));

					}
				}

				//ゲームをやめる
				{
					DirectX::XMFLOAT2 pos = { 1256,317 };
					DirectX::XMFLOAT2 size = { 389,170 };
					if (isRectJubge(mousePos.x, mousePos.y, pos.x, pos.y, size.x, size.y) &&
						mouse.GetButtonDown() & Mouse::BTN_LEFT)
					{
						scoreManager.ResetData();

						optionUI.isOption = false;
						optionUI.isHome = false;

						GameManager::Instance().ReleaseTargetManager();

						HWND hWnd = GetActiveWindow();
						PostMessage(hWnd, WM_CLOSE, 0, 0);

					}
				}

			}
		}
	}
	else
	{
		if (!nowGameScene)//シーンセレクトの時
		{
			optionTimer += elapsedTime;

			if (!isSpin)
			{
				if (optionTimer >= 2.0f)
				{
					isSpin = true;
					optionTimer = 0.0f;
				}
			}
			else
			{
				optAngle += 700 * elapsedTime;

				if (optionTimer >= 0.5f)
				{
					isSpin = false;
					optionTimer = 0.0f;
				}
			}
		}
		else
		{
			GameManager::Instance().SetPlaying(true);
		}
	}
}
void OptionUI::RenderOption(const RenderContext& rc, ModelRenderer* renderer)
{
	sprOption->Render(rc,
		optPos.x, optPos.y, 0,
		optSize, optSize,
		optAngle, optColor.x, optColor.y, optColor.z, optColor.w);

	if (isOption)
	{
		//しりとり説明
		if (ruleState == SIRITORI)
		{
			sprSiritori->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		}
		//提出説明
		else if (ruleState == SUBMIT)
		{
			sprSubmit->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		}
		//操作説明
		else if (ruleState == SOUSA)
		{
			sprSousa->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		}


		if (isHome)//ホームボタン
		{
			if(!nowGameScene)//selectScene
			sprSHome->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

			else if(nowGameScene)//gameScene
			sprGHome->Render(rc, 0, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);
		}
	}

}
void OptionUI::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("timer", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Checkbox("nowGameScene", &nowGameScene);
		ImGui::InputFloat("optionTimer", &optionTimer);
		ImGui::InputFloat2("mousePos", &mousePos.x);

	}
	ImGui::End();

}

//------------------------*マウスと物の判定*--------------------------------
bool isRectJubge(float pointX, float pointY, float posX, float posY, float sizeX, float sizeY)
{
	return (posX  < pointX &&//左
		posX + sizeX> pointX &&//右
		posY < pointY &&//上
		posY + sizeY  > pointY); //下
}
bool isCircleJubge(float pointX, float pointY, float posX, float posY, float radius)
{
	float centerX = posX + radius;
	float centerY = posY + radius;

	return (pointX - centerX) * (pointX - centerX) +
		(pointY - centerY) * (pointY - centerY)
		<= radius * radius;
}
//------------------------------------------------------------------------------
