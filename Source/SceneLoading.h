#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include <thread>

#include "targetManager.h"

// ローディングシーン
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() override {};

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	// GUI描画
	void DrawGUI() override;


	struct LoadingTarget  
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
	};

private:


	Sprite* sprite = nullptr;
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;
	float LoadTimer = 4000;
	Sprite* sprFadeRect = nullptr;

	int ItemNum = {};

	// 3Dモデル
	Model* mdlApple = nullptr;

	TargetManager* targetManager = nullptr;
	std::vector<LoadingTarget> loadingTargets;


	// ローディングスレッド
	static void LoadingThread(SceneLoading* scene);
};

