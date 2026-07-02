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
		bool isFocus = false;//フォーカスされているか
		bool isRender = false;//描画するか
	};
	
	float maxDistance = 500.0f;
	DirectX::XMFLOAT3 rayEnd;
	DirectX::XMFLOAT3 rayStart;

private:

	Model* modelTargets = nullptr;
	Target targets;
	//std::vector<Target*> keepTargets;//保存用


public:
	bool GetIsRender() { return targets.isRender; }
	//bool GetIsRender(int i/*配列要素数*/) { return target; }

};