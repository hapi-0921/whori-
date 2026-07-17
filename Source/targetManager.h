#pragma once
#include<DirectXMath.h>
#include "System/Sprite.h"

#include "System/Model.h"

#include"System/ModelRenderer.h"
#include <json.hpp>
using json = nlohmann::json;

#include"ScoreManager.h"

#include"Font.h"

class TargetManager
{
public:
	TargetManager();
	~TargetManager();

	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void Render(const RenderContext& rc);
	void DrawDebugGUI();

	void TargetFocus(float elapsedTime);//当たってるか

	//void TargetUpload();//targetを提出

	struct TargetData
	{
		std::string modelPath;
		std::string spritePath;
		std::string cardPath;

		std::string startN;
		std::string endN;
		int charCount = 0;

		DirectX::XMFLOAT3 position = { 0,0,0 };
	};

	Font* number = nullptr;


	std::vector<TargetManager::TargetData> LoadTargets(const std::string& path);
private:
	// クラスメンバー
	float cardSpinAngle = 0.0f;

	int displayIndex = 0;

	struct Target
	{
		Model* mdlCard = nullptr;
		std::string cardPath;

		Model* model = nullptr;
		Sprite* sprite = nullptr;
		std::string startN;
		std::string endN;
		int charCount = 0;
		DirectX::XMFLOAT3 pos = { 0,0,0 };//文字数用

		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT4X4 transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		float distance = 0.0f;
		DirectX::XMFLOAT3 hitPos = { 0.0f, 0.0f, 0.0f };

		bool isRayHit = false;//レイが当たったか（判定中）

		bool isFocus = false;//フォーカスが完了してるか
		bool preFocus = false;//前のisFocus情報

		bool isChainRender = false;//チェーンに表示するか


		//カード
		bool carsRen = false;//正面にはる
		bool isMoveToChain = false;//カードをチェーンに
		float moveTimer = 0.0f;//カード移動時間（成功時）
		float cardX = 1920 * 0.5f - 740* 0.5f;
		float cardY = 1080 * 0.5f - 740 * 0.5f;
		float cardW = 740.0f;
		float cardH = 740.0f;

		float shiftTimer = 0.0f;
		bool isShift = false;
		bool shifted = false;

		float stayTimer = 0.0f;
		float failTimer = 0.0f;
		float drawScale = 1.0f;
		//---




		DirectX::XMFLOAT3 vec;

	};
	void UpdateCardMove(float elapsedTime);

	Sprite* sprMiss = nullptr;
	bool nonChain = false;//しりとり失敗
	float stayTime = 0.0f;//失敗時


	Target tfCard;

	DirectX::XMFLOAT3 chainPos = {};

	//フォーカス中の光
	DirectX::XMFLOAT4 maxColor = { 0.6f,0.6f,0.6f,1.0f };
	DirectX::XMFLOAT4 minColor = { 0.3f,0.3f,0.3f,1.0f };
	DirectX::XMFLOAT4 nonColor = {};
	float timer = 0.15f;

	float maxDistance = 500.0f;
	int chrCount =0.0f;
	DirectX::XMFLOAT3 rayEnd;
	DirectX::XMFLOAT3 rayStart;


	//std::string firstName = "";//最初の文字
	std::string endName = "";//前の文字

	float focusTimer = 0.0f;
	float lightTimer = 0.0f;

	float stageVec;
	float targetVec;

	float renSpan = 165;//連鎖表示の間隔

	//カーソル判定（面）
	//bool TargetManager::IsInCursor(Target& t);

	//float deltaTimer = 0.0f;

private:
	//UIController* uiController = nullptr;

	std::vector<Target> targets;//全ての対象物
	std::vector<Target*> getTargets;//獲得したもの保存

	//文字数
	Target tfCharCount;
	Model* mdlChrCount = nullptr;

	float cardTimer = 0.0f;

	float spinAngle = 0.0;

	bool charRen = false;

	float charRotate = 0.0f;//文字数回転
public:
	float cusolPos = 50;
	//bool IsInCursorArea(const DirectX::XMFLOAT2& screenPos, float delta);

	float distance = 0;
	bool canZoom = false;

	bool moveCusol = false;//カーソルを動かす
	

	//float posForRotate;//回転用position

	//void AddMoveTime(int index, float elapsedTime)
	//{
	//	targets[index].moveTimer += elapsedTime;
	//}

	//float GetMoveTime(int index)
	//{
	//	return targets[index].moveTimer;
	//}


	int GetKeepTargetSize() { return static_cast<int>(getTargets.size()); }//獲得したtargetの大きさ
	Sprite* GetgetTargetSpri(int index) { return getTargets[index]->sprite; }//獲得したカード（スプライト）
	//*****

	bool GetCarsRen(int index) { return targets[index].carsRen; }
	bool GetMoveToChain(int index) { return targets[index].isMoveToChain; }
	//bool GetisFocus(int index) { return targets[index].isFocus; }

	//Target GetKeepTarget(int index) { return *getTargets[index]; }

	int GetTargetSize() const { return static_cast<int>(targets.size()); }
	std::vector<Target> GetTargets() const { return targets; }



	Sprite* GetTargetSpri(int index)  { return targets[index].sprite; }


	//void TargetManager::SetUIController(UIController* pUI)
	//{
	//	uiController = pUI;
	//}


	bool toResult = false;//リザルトへ

};