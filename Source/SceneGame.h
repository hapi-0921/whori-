#pragma once
#include"Stage.h"
#include"CameraController.h"
#include"Scene.h"
#include"targetManager.h"
#include "System/Sprite.h"
#include"UIController.h"
#include"Font.h"


// ƒQ[ƒ€ƒV[ƒ“
class SceneGame : public Scene
{
public:
	SceneGame() {};
	~SceneGame() {};

	// ‰Šú‰»
	void Initialize();

	// I—¹‰»
	void Finalize();

	// XVˆ—
	void Update(float elapsedTime);

	// •`‰æˆ—
	void Render();

	// GUI•`‰æ
	void DrawGUI();
private:

	UIController* uiController = nullptr;
	TargetManager* targetManager = nullptr;
	CameraController* cameraController = nullptr;
	ShapeRenderer* shapeRenderer = nullptr;

	Stage::StageTransform gameStage;

	bool nowGame = false;

	float gameTimer = 0.0f;
	int GameTimer = 0;

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
