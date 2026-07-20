#pragma once
#include"Stage.h"
#include"CameraController.h"
#include"Scene.h"
#include"targetManager.h"
#include "System/Sprite.h"
#include"UIController.h"
#include"Font.h"


// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {};
	~SceneGame() {};

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	// GUI描画
	void DrawGUI();
	float debugElapsedTime = 0.0f;
private:

	UIController* uiController = nullptr;
	TargetManager* targetManager = nullptr;
	CameraController* cameraController = nullptr;
	ShapeRenderer* shapeRenderer = nullptr;

	Stage::StageTransform gameStage;

	bool nowGame = false;

	float gameTimer = 0.0f;
	int GameTimer = 60;//本命タイマー

	int sec = 0;
	int min = 0;

	Sprite* sprTimer = nullptr;
	Font* timer = nullptr;

public:


	void SetNowGame(bool nowGame)
	{
		this->nowGame = nowGame;
	}
	void SetGameTimer(float gameTimer)
	{
		this->gameTimer = gameTimer;
	}
};
