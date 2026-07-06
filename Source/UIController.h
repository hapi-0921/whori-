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
	void Render(const RenderContext& rc);
	void DrawDebugGUI();

private:
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

private://しりとり連鎖
	Sprite* sprChain = nullptr;
	sprData chainData;

	float renSpan = 90;//連鎖表示の間隔
	//int rupeMax = 5;

private://targetカード
	Sprite* sprCard = nullptr;

	sprData cardData;

public:
	void SetTargetManager(TargetManager* manager)
	{
		targetManager = manager;
	}
};