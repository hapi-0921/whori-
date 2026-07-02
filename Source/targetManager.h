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

private:
	struct Target
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

		float distance;
	};

private:
	Model* modelTargets = nullptr;

	Target targets;
	std::vector<Target*> keepTargets;//•Û‘¶—p
};