#pragma once
#include<DirectXMath.h>


//フリーのtransform更新関数
void freeUpdateTransform(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT4X4& transform);


//キャラクター（player・enemy用）
class ModelCommon// 基底クラス
{
public:
	ModelCommon() {}
	virtual ~ModelCommon() {}

	//行列更新処理
	void UpdateTransform();

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	//位置設定
	//		クラスのメンバ変数と引数の変数の名前が同じやから、thisポインタでアクセス
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	//回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	//回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	//スケール設定
	void Setscale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

protected://継承先でもアクセス可能
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};