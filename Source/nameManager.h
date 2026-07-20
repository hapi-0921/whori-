#pragma once

#include<DirectXMath.h>
#include"System/ModelRenderer.h"

#include "System/Sprite.h"
#include "System/Graphics.h"

class NameManagger
{
public:
	NameManagger();
	~NameManagger();

	void Initialize();
	bool Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void DrawDebugGUI();

	static NameManagger& Instance()
	{
		static NameManagger nameManagger;
		return nameManagger;
	}

private:
	Sprite* sprNamePlate = nullptr;
	Sprite* sprNowTyping = nullptr;
	Sprite* sprBar = nullptr;
};

//#include"nameManager.h"
//NameManagger& nameManagger = NameManagger::Instance();
