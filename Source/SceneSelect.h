#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include "Stage.h"
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

	// スプライト
	Sprite* sprite = nullptr;
	Sprite* sprArrowRight = nullptr;
	Sprite* sprArrowLeft = nullptr;
	Sprite* sprfirst = nullptr;
	// カメラ
	CameraController* cameraController = nullptr;
	ShapeRenderer* shapeRenderer = nullptr;

	// ステージ行列
	Stage::StageTransform selectStage;
	Stage::StageTransform selectStage2;

	// ステージモデル
	Model* mdlMachi[4] = {};
	Model* mdlShima[7] = {};

	// マウスカーソルの位置
	float CursorX = {};
	float CursorY = {};

	// ステージの状態
	int stageState = {};

	// 矢印の色味
	float arrowColorRight = {};
	float arrowColorLeft = {};
	
	// ステージ
	enum stageType
	{
		stage1 = 0,
		stage2 = 1
	};
	static bool selectstart;
	static bool first;
	int selecttimer = 0;
	// 描画関数以外でも使いたいのでヘッダーに移動させる
	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	float ArrowSize = 256; // 矢印UIのサイズ
	float ArrowH = screenHeight * 0.5f - ArrowSize * 0.5; // 矢印UIの描画位置（Y軸）
};