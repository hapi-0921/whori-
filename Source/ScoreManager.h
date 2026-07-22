#pragma once

#include<DirectXMath.h>
#include "System/Sprite.h"

class ScoreManager
{
public:

	ScoreManager();
	~ScoreManager();

	void Update(float elapsedTime);
	void Render(const RenderContext& rc);

	void TargetUpload();//targetを提出

	void DrawDebugGUI();

	static ScoreManager& Instance()
	{
		static ScoreManager scoreManager;
		return scoreManager;
	}

	Sprite* sprShader = nullptr;
	DirectX::XMFLOAT2 shadeSize = { 270,76 };
	float texPosY = 77.0f;


	DirectX::XMFLOAT2 mousePos = {};
	bool upload = false;
	bool reset = false;



	bool nowCombo = false;//conbo
	bool nowScore = false;//score
	float comboTimer = 0.0f;
	float scoreTimer = 0.0f;
	DirectX::XMFLOAT2 comboPos = { 1400 ,200 };
	DirectX::XMFLOAT2 scorePos = { 400,950 };
	float comboScale = 7.0f;
	float scoreScale = 6.0f;

	float speed = 2.0f;

	int chainCount = 0;//何連鎖中か
	int conbo = 0;//コンボ数
	int nowChain = 0;

	//=============== result ===============
	//ScoreManager& scoreManager = ScoreManager::Instance();
	//　↑リザルトでこれ必須
	// ランクはresultで区別
	//---スコア計算用---
	int score = 0;//一回一回のスコア
	int allCharCount = 0;//合計文字数
	float comboMultiplier[20] = { //コンボ倍率
		1.0f ,1.2f,1.4f ,1.6f ,1.8f ,2.0f ,2.1f ,2.2f ,2.3f ,2.4f ,
		2.5f ,2.6f ,2.7f ,2.8f ,2.9f ,3.0f ,3.1f ,3.2f ,3.3f ,3.4f
	};

	//---そのまま表示---
	int allScore = 0;//スコア
	int siritoriNum = 0;//しりとり数

	int getNum = 0;//見つけた数
	int targetNum = 0;//target合計

	int maxCombo = 0;//最大コンボ
	int maxChar = 0;//最大文字数

	void ResetData()
	{
		chainCount = 0;//何連鎖中か
		conbo = 0;//コンボ数
		nowChain = 0;

		score = 0;//一回一回のスコア
		allCharCount = 0;//合計文字数

		allScore = 0;//スコア
		siritoriNum = 0;//しりとり数

		getNum = 0;//見つけた数
		targetNum = 0;//target合計

		maxCombo = 0;//最大コンボ
		maxChar = 0;//最大文字数
	}
};


//scoreManager.getNum++;
