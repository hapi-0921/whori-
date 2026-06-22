#pragma once
#include<DirectXMath.h>
#include"System/Mouse.h"//移動・回転操作

//カメラコントローラー
class CameraController
{
public:
	//更新処理
	void Update(float elaosedTime);
	//ターゲット位置設定
	void SetTerget(const DirectX::XMFLOAT3& target) { this->target = target; }
	//デバッグ用GUI描画
	void DrawDebugGUI();

private:

	//距離制限
	float minRange = 630.0f;
	float maxRange = 1500.0f;

	DirectX::XMFLOAT3 target = { -150,0,-150 };//注視点
	DirectX::XMFLOAT3 angle = { 0,0,0 };//回転角度
	DirectX::XMFLOAT3 eye;

	float rollSpeed = DirectX::XMConvertToRadians(90);//回転速度


public:
	DirectX::XMFLOAT3 GetCameraTarget() { return target; }
	DirectX::XMFLOAT3 GetCameraAngle() { return angle; }
	DirectX::XMFLOAT3 GetCameraEye() { return eye; }

	float range = maxRange;//距離

};