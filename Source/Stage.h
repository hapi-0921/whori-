#pragma once
#include"System/ModelRenderer.h"
#include"ModelCommon.h"
#include"CameraController.h"

//ステージ
class Stage:public ModelCommon
{
public:
	Stage();
	~Stage()override;

	//更新処理
	void Update(float elapsedTime);
	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	void FrontWall();

	//デバッグ用GUI描画
	void DrawDebugGUI();

private:
	struct Wall
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


		bool isFrontWall = false;
	};

	enum wallPos
	{
		FRONT_R,
		FRONT_L,
		BACK_R,
		BACK_L,
	
		UP,
		DOWN,
	};

	//壁を二枚透明化（一枚だけを追加実装）
	struct WallDistance
	{
		int index;
		float distance;
	};

private:
	Model* model = nullptr;
	Model* wall_mdl = nullptr;
	Wall wall[6];
	CameraController* camera = nullptr;
	std::vector<WallDistance> distances;

	float stageSize = 300;

public:
	void SetCamera(CameraController* camera)
	{
		this->camera = camera;
	}
};