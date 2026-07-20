#pragma once

#include<DirectXMath.h>
#include"System/ModelRenderer.h"

#include"Stage.h"
#include "System/Sprite.h"
#include "System/Graphics.h"

class Tutorial

{
public:
	Tutorial();
	~Tutorial();

	void Initialize();
	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void DrawDebugGUI();

	void NowTutorial(float elapsedTime);
	void NowRender(const RenderContext& rc);

	static Tutorial& Instance()
	{
		static Tutorial tutorial;
		return tutorial;
	}

	bool isTutorial = false;
	bool toSelect = false;
	bool toGame= false;
private:
	DirectX::XMFLOAT2 mousePos{};

	Sprite* sprEscape = nullptr;
	Sprite* sprStart = nullptr;
	Sprite* sprRule[9] = {};
	Sprite* sprClick = nullptr;
	Sprite* sprArrow = nullptr;


	DirectX::XMFLOAT2 strPos{ 1445  ,975 };
	DirectX::XMFLOAT2 strSize{ 455  ,80 };
	DirectX::XMFLOAT4 strColor{ 0.2f,0.2f,0.2f,1 };

	float dlt = 300;
	DirectX::XMFLOAT2 endPos{ 1424  ,38+ 300 };
	DirectX::XMFLOAT2 endSize{ 198  ,62 };
	DirectX::XMFLOAT4 endColor{ 0.6f,0.6f,0.6f,1 };

	float blinkTimer = 0.0f;
	bool showClick = true;
	float arrowMove = 0.0f;

public:
	int tutoType = 0;

	float tuto2wheel = 0.0f;
	bool tuto2 = false;
	float tuto3drag = 0.0f;
	bool tuto3 = false;

	bool tuto4 = false;
	bool tuto6 = false;

	float tuto9Timer = 0.0f;

};

//#include"tutorial.h"
//Tutorial& tutorial = Tutorial::Instance();
