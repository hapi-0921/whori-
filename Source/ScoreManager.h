#pragma once

#include<DirectXMath.h>

class ScoreManager
{
public:

	ScoreManager();
	~ScoreManager();

	void Update(float elapsedTime);

	void TargetUpload();//targetを提出

	void DrawDebugGUI();

	static ScoreManager& Instance()
	{
		static ScoreManager scoreManager;
		return scoreManager;
	}


	DirectX::XMFLOAT2 mousePos = {};
	bool upload = false;
	bool reset = false;


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


};