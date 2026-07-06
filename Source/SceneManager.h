#pragma once

#include "Scene.h"
#include <System/Sprite.h>

// シーンマネージャー
class SceneManager
{
private:
	SceneManager()
	{
		sprFadeRect = new Sprite("Data/Sprite/FadeRect.png"); // フェード用のスプライト
		fade_state = FADE_IN;
		SprFade = 1.0f;
	}
	~SceneManager() 
	{
		// スプライト終了化
		if (sprFadeRect != nullptr)
		{
			delete sprFadeRect;
			sprFadeRect = nullptr;
		}
	}

public:
	// 唯一のインスタンス取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	// GUI描画
	void DrawGUI();

	// シーンクリア
	void Clear();

	// シーン切り替え
	void ChangeScene(Scene* scene);
	void ChangeScene2(Scene* scene);

	// フェード用関数
	void FadeIn(float fade);
	void FadeOut(float fade);

	bool is_fadeIn = false; // フェードインしているか
	float SprFade = 0.0f; // フェード用スプライトの透明度


	enum FADE
	{
		FADE_NONE = 0,
		FADE_OUT,		// フェイドアウト中（だんだん暗く）
		FADE_IN,		// フェイドイン中（だんだん明るく）
		FADE_NONE2
	};
	int fade_state = FADE::FADE_NONE;


private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

	Sprite* sprFadeRect = nullptr;
};