#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include "Stage.h"
#include <vector>
#include"Font.h"

// セレクトシーン
class SceneResult : public Scene
{
public:
	SceneResult() {}
	~SceneResult() override {}

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
	Sprite* sprresult=nullptr;
	Sprite* sprresultback = nullptr;
	Sprite* sprresultback2 = nullptr;

	Sprite* sprranking = nullptr;

	Sprite* sprs = nullptr;
	Sprite* sprrank[5] = {};
	Sprite* sprrankword[5] = {};
	Sprite* sprnew = nullptr;
	int resultTimer = 0;
	int rankTimer = 0;
	int rank=-1;
	int CursorX = 0;
	int	CursorY = 0;
	int rankset = 0;
	bool ranking = false;
	struct iti
	{
		int posx = 0;
		int posy = -300;
		int angle = 0;
		int lastposy = 0;
	};
	struct Record
	{
		int posx = 1000;
		int posy = 0;
	};
	Record record[4];
	float scorescale = 2;
	float rankScale = 1.0f;

	int getDown = 0;
	bool rankScaleAnim = false;
	bool rankFinish = false;   
	bool next = false;
	iti  sikaku[30];
	int nowCard = 0;
	json data;
	Font* font=nullptr;
	Font* Numberfont = nullptr;
};