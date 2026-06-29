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
	//カメラが部屋の中にいるかどうか
	bool IsCameraInRoom();
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

private:
	Model* mdlStage = nullptr;//ステージ本体
	Model* mdlWall = nullptr;//壁モデル
	Model* mdlCenterWall = nullptr;//中央の壁モデル

	CameraController* camera = nullptr;
	std::vector<WallDistance> distances;

	float stageSizeX = 300;
	float stageSizeY = 350;

public:
	Wall wall[6];
	Wall centerWall;

	Model* GetWall() { return mdlWall; }
	Model* GetCenterWall() { return mdlCenterWall; }

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