#pragma once
#include"System/ModelRenderer.h"
#include"ModelCommon.h"

//プレイヤー
class Player :public ModelCommon
{
public:
	Player();
	~Player() override;
	//更新処理
	void Update(float elapsedTIme);
	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	//デバッグ用GUI描画
	void DrawDebugGUI();

private:
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

private:
	Model* model = nullptr;

	float moveSpeed = 5.0f;
};
