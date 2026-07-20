#pragma once

#include<DirectXMath.h>
#include"System/ModelRenderer.h"

#include"Stage.h"
#include "System/Sprite.h"
#include "System/Graphics.h"
#include"targetManager.h"

bool isRectJubge(float pointX, float pointY, float posX, float posY, float sizeX, float sizeY);
bool isCircleJubge(float pointX, float pointY, float posX, float posY, float radius);

//--- 設定画面の表示 ---
class OptionUI
{
public:
	OptionUI();
	~OptionUI();

	void UpdateOption(float elapsedTime);
	void RenderOption(const RenderContext& rc, ModelRenderer* renderer);
	void DrawDebugGUI();

	static OptionUI& Instance()
	{
		static OptionUI optionUI;
		return optionUI;
	}

	bool homeOpen = false;
	bool isOption = false;
	bool isHome = false;
	bool nowGameScene = false;

private:
	DirectX::XMFLOAT2 mousePos{};

    // --- 設定マーク----
	Sprite* sprOption = nullptr;
	DirectX::XMFLOAT2 optPos{ 1920-200  ,20 };
	DirectX::XMFLOAT4 optColor{ 0.3f,0.3f,0.3f,1 };

	float optSize = 128.0f;
	bool isSpin = true;
	float optAngle = 0.0f;
	float optionTimer = 0.0f;
	//

	// --- ルール説明----
	Sprite* sprSiritori = nullptr;//しりとり説明
	Sprite* sprSubmit = nullptr;//提出説明
	Sprite* sprSousa = nullptr;//操作説明
	DirectX::XMFLOAT2 siriPos{ 1666.0f ,375.0f+183*0};
	DirectX::XMFLOAT2 subPos{ 1666.0f ,375.0f+183*1};
	DirectX::XMFLOAT2 souPos{ 1666.0f ,375.0f+183*2};
	
	DirectX::XMFLOAT2 ruleSize = {237.0f,155.0f};
	enum RuleState
	{
		SIRITORI,SUBMIT,SOUSA
	};
	int ruleState = SIRITORI;
	//

	// --- ホームボタン----
	Sprite* sprSHome = nullptr;//セレクトシーン用
	Sprite* sprGHome = nullptr;//ゲームシーン用
	DirectX::XMFLOAT2 homePos{ 1590,0 };
	float homeSize = 55.0f;
    //

	bool ax = false;

};

//2D UI用
class UIController
{
public:
	UIController();
	~UIController();

	void Initialize();
	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void DrawDebugGUI();

private:
	struct Transform
	{
		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT4X4 transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	struct sprData
	{
		float dx, dy, dz;//pos   左上中心
		float dw, dh;//size
		float sx, sy;//texPos
		float sw, sh;//texSize
		float angle;
		float r, g, b, a;
	};

	float screenWidth ;
	float screenHeight;

	TargetManager* targetManager = nullptr;	

	Sprite* sprChain = nullptr;
	Sprite* sprChainOver = nullptr;

	float deltaTimer = 0.0f;

private://しりとり連鎖
	sprData chainData;

	float renSpan = 140;//連鎖表示の間隔

//targetカード
	sprData cardData;

	Sprite* sprite = nullptr;
	float cusolSize = 130;

	Sprite* centerCusol = nullptr;
	float centerSize = 13;

public:

	void SetTargetManager(TargetManager* manager)
	{
		targetManager = manager;
	}
};
