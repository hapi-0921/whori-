#pragma once

#include<DirectXMath.h>
#include"System/ModelRenderer.h"

#include"Stage.h"
#include "System/Sprite.h"
#include "System/Graphics.h"
#include"targetManager.h"

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
