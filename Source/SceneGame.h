#pragma once
#include"Stage.h"
#include"CameraController.h"
#include"Scene.h"
#include"targetManager.h"
#include "System/Sprite.h"


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
private:
	Sprite* sprite = nullptr;

	TargetManager* targetManager = nullptr;
	CameraController* cameraController = nullptr;
	ShapeRenderer* shapeRenderer = nullptr;
};
