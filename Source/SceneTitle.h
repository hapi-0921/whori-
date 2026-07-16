//SceneTitle.h
#pragma once
#include "System/Sprite.h"
#include "Scene.h"
#include "TitleStage.h"
#include "CameraController.h"

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	void Initialize() override;
	void Finalize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void DrawGUI() override;


private:
	TitleStage* titleStage = nullptr;
	Sprite* sprite = nullptr;
	Sprite* titlelogo = nullptr;
	Sprite* clicklogo = nullptr;
};