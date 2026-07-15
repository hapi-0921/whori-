#pragma once
#include"System/ModelRenderer.h"
#include"ModelCommon.h"
#include"CameraController.h"

//ステージ
class Stage :public ModelCommon
{
public:
	struct StageTransform
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


		bool isFrontWallX = false;
		bool isFrontWallY = false;

		bool isCameraOutWall = false;
	};

	enum wallPos
	{
		FRONT,
		LEFT,
		RIGHT,
		BACK,
		UP,
		DOWN,
	};

	//壁を二枚透明化（一枚だけを追加実装）
	struct WallDistance
	{
		int index;
		float distance;
	};

	DirectX::XMFLOAT3 vecDepth{};
	DirectX::XMFLOAT3 vecCenter{};

public:
	float minVec = 0;

	Stage();
	~Stage()override;

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Stage::Render(const RenderContext& rc, ModelRenderer* renderer, StageTransform* transform);

	//void FrontWall();

	//デバッグ用GUI描画
	void DrawDebugGUI();


public:
	int stageNum = 0;//ステージの個数

	int stageType = SIMA;
	enum StageType
	{
		MACHI = 1,
		SIMA
	};

private:

	Model* mdlStage[7] = {};//ステージ本体
	Model* mdlCenterWall = nullptr;//壁モデル

	CameraController* camera = nullptr;
	std::vector<WallDistance> distancesX;
	std::vector<WallDistance> distancesY;

	float stageSizeX = 300;
	float stageSizeY = 350;

	StageTransform* transform = nullptr;


public:

	enum wallType
	{
		CENTER,
		DEPTH
	};
	Wall centerWall[2];

	Model* GetStage(int index) { return mdlStage[index]; }
	//Model* GetWall() { return mdlWall; }
	Model* GetCenterWall() { return mdlCenterWall; }


	void SetTransform(StageTransform* t)
	{
		transform = t;
	}

	const StageTransform& GetTransform() const
	{
		return *transform;
	}


	void SetCamera(CameraController* camera)
	{
		this->camera = camera;
	}

	static Stage& Instance()
	{
		static Stage stage;
		return stage;
	}

};