#pragma once
#include"Stage.h"
#include"CameraController.h"
#include"Scene.h"
#include"targetManager.h"
#include "System/Sprite.h"
#include"UIController.h"


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

public:

	void SetNowGame(bool nowGame)
	{
		this->nowGame = nowGame;
	}
};
