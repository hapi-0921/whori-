#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include"CameraController.h"

// セレクトシーン
class SceneSelect : public Scene
{
public:
	SceneSelect() {}
	~SceneSelect() override {}

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

private:
	Sprite* sprite = nullptr;
	CameraController* cameraController = nullptr;
	ShapeRenderer* shapeRenderer = nullptr;

	// セレクト画面でのステージ用のスケール調整変数
	DirectX::XMFLOAT3 StageSelectScale = { 0.1f, 0.1f, 0.1f };


};