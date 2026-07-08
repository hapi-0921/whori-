#pragma once
#include<DirectXMath.h>
#include "System/Sprite.h"

#include"System/ModelRenderer.h"
#include <json.hpp>
using json = nlohmann::json;

class TargetManager
{
public:
	TargetManager();
	~TargetManager();

	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void DrawDebugGUI();

	void TargetFocus();//当たってるか



	struct TargetData
	{
		std::string modelPath;
		std::string spritePath;
		std::string name;
		std::string startN;
		std::string endN;
	};


	std::vector<TargetManager::TargetData> LoadTargets(const std::string& path);
private:

	struct Target
	{
		Model* model = nullptr;
		Sprite* sprite = nullptr;

		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT4X4 transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		std::string name;
		std::string startN;
		std::string endN;


		float distance = 0.0f;
		DirectX::XMFLOAT3 hitPos = { 0.0f, 0.0f, 0.0f };

		bool isRayHit = false;//レイが当たったか（判定中）

		bool isFocus = false;//フォーカスが完了してるか
		bool preFocus = false;//前のisFocus情報

		bool isChainRender = false;//チェーンに表示するか

	};



	float maxDistance = 500.0f;
	DirectX::XMFLOAT3 rayEnd;
	DirectX::XMFLOAT3 rayStart;

	int chainCount = 0;//何連鎖中か

	std::string firstName = "";//最初の文字
	std::string endName = "";//前の文字



private:
	std::vector<Target> targets;//全ての対象物
	std::vector<Target*> getTargets;//獲得したもの保存

public:
	bool GetisFocus(int index) { return targets[index].isFocus; }
	int GetChainCount() { return chainCount - 1; }

	Target GetKeepTarget(int index) { return *getTargets[index]; }
	int GetKeepTargetSize() { return static_cast<int>(getTargets.size()); }

	int GetTargetSize() const { return static_cast<int>(targets.size()); }
	std::vector<Target> GetTargets() const { return targets; }



	Sprite* GetTargetSpri(int index)  { return targets[index].sprite; }
	Sprite* GetgetTargetSpri(int index)  { return getTargets[index]->sprite; }

};