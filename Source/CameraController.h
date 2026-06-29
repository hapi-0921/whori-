#pragma once
#include<DirectXMath.h>
#include"ModelCommon.h"
#include"System/Mouse.h"
//#include"Stage.h"

//カメラコントローラー
class CameraController :ModelCommon
{
public:
	bool cursorRay(DirectX::XMFLOAT3& hitDelta);

	//更新処理
	void Update(float elaosedTime);
	void Render(const RenderContext& rc);
	//ターゲット位置設定
	void SetTerget(const DirectX::XMFLOAT3& target) { this->target = target; }
	//デバッグ用GUI描画
	void DrawDebugGUI();

private:

	//距離制限
	float minRange = 0.0f;
	float maxRange = 1500.0f;

	DirectX::XMFLOAT3 target = { 0,0,0 };//注視点
	DirectX::XMFLOAT3 angle = { 0,-DirectX::XM_PI * 0.25f,0 };//回転角度
	DirectX::XMFLOAT3 eye;

	float rollSpeed = DirectX::XMConvertToRadians(90);//回転速度
	float zoomSpeed = 10;

	Model* model = nullptr;
	DirectX::XMFLOAT3 hitDelta;

	bool hitRay = false;

	DirectX::XMFLOAT3 rayStart;
	DirectX::XMFLOAT3 rayEnd;

	DirectX::XMVECTOR zoomTarget = XMLoadFloat3(&target);
	DirectX::XMVECTOR zoomEye = XMLoadFloat3(&eye);
	DirectX::XMVECTOR HitPosition = XMLoadFloat3(&hitDelta);


public:
	DirectX::XMFLOAT3 GetCameraTarget() { return target; }
	DirectX::XMFLOAT3 GetCameraAngle() { return angle; }
	DirectX::XMFLOAT3 GetCameraEye() { return eye; }

	float range = maxRange;//距離
	bool zoom = false;
	float t = 0;

};