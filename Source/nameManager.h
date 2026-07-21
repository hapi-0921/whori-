#pragma once

#include<DirectXMath.h>
#include"System/ModelRenderer.h"

#include "System/Sprite.h"
#include "System/Graphics.h"

#include <string>
#include "Font.h"
#include "System/GamePad.h"

class NameManagger
{
public:
	NameManagger();
	~NameManagger();

	void Initialize();
	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void DrawDebugGUI();
	bool IsKeyTrigger(int vk);
	void Reset();

	static NameManagger& Instance()
	{
		static NameManagger nameManagger;
		return nameManagger;
	}

	bool nameYet = false;//Šl“¾Ï‚İ

	static const int MAX_LENGTH = 17;


private:
	Sprite* sprNamePlate = nullptr;
	Sprite* sprNowTyping = nullptr;
	Sprite* sprBar = nullptr;

	DirectX::XMFLOAT2 mousePos{};

	std::string name;
	bool isChar = false;   // •¶š‚ª‚ ‚é‚©
	bool nowName = false;     // “ü—Í’†
	int  cursorBlink = 0;

	Font* fName = nullptr;


public:

	const std::string& GetName() const { return name; }
	bool IsConfirmed() const { return nameYet; }
};

//#include"nameManager.h"
//NameManagger& nameManagger = NameManagger::Instance();
