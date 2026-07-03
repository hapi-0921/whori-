#pragma once
#include<DirectXMath.h>
#include"System/ModelRenderer.h"

class TargetManager
{
public:
	TargetManager();
	~TargetManager();

	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void DrawDebugGUI();

	void TargetFocus();

private:
	struct Target
	{
		Model* model = nullptr;

		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT4X4 transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		float distance = 0.0f;
		bool isFocus = false;
		bool isRender = false;
		bool preRender = false;
	};
	
	float maxDistance = 500.0f;
	DirectX::XMFLOAT3 rayEnd;
	DirectX::XMFLOAT3 rayStart;

	int chainCount = 0;//âΩòAçΩíÜÇ©

public:

	enum targetType
	{
		CLOCK,
		RADIO,
		TARGET_MAX
	};

private:
	Target targets[TARGET_MAX];//ëSÇƒÇÃëŒè€ï®
	std::vector<Target*> keepTargets;//älìæÇµÇΩÇ‡ÇÃï€ë∂

public:
	bool GetIsRender(int index) { return targets[index].isRender; }
	int GetChainCount() { return chainCount-1; }

	Target GetKeepTarget(int index) { return *keepTargets[index]; }
	int GetKeepTargetSize() { return static_cast<int>(keepTargets.size()); }
};