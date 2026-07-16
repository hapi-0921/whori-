//titleStage.h
#pragma once
#include "System/ModelRenderer.h"
#include "ModelCommon.h"

class TitleStage
{
public:
	TitleStage();
	~TitleStage();

	void Update(float elapsedTime);
	void SetZoom(float zoom);
	void Render(const RenderContext& rc, ModelRenderer* renderer);


private:
	Model* mdlStage = nullptr;


	struct Titleobj
	{
		Model* model = nullptr;

		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 0.1f,0.1f,0.1f };
		DirectX::XMFLOAT3 rotateSpeed = { 1.0f,1.0f,1.0f };

		DirectX::XMFLOAT4X4 transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		float speed = 0.0f;
	};

	std::vector<Model*> models;
	std::vector<Titleobj> objects;

	float rotY = 0.0f;
	float zoom = 0.05f;
};