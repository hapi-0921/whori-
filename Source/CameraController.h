#pragma once
#include<DirectXMath.h>
#include"ModelCommon.h"
#include"System/Mouse.h"
#include "targetManager.h"

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

	void CameraReset();

	const DirectX::XMFLOAT3 DEFAULT_TARGET = { 0.0f, 0.0f, 0.0f };
	const DirectX::XMFLOAT3 DEFAULT_ANGLE = { DirectX::XM_PI * 0.25f, -DirectX::XM_PI * 0.25f,  DirectX::XM_PI * 0.25f };

private:
	//距離制限
	float maxRange = 2000.0f;
	float minRange = 0.0f;
	float minVec;

	//回転
	DirectX::XMFLOAT3 rotateCenter;//回転用注視点
	bool isRotating = false;



	Model* model = nullptr;
	DirectX::XMFLOAT3 hitDelta;

	bool hitRay = false;


	DirectX::XMFLOAT3 rayStart;
	DirectX::XMFLOAT3 rayEnd;

	DirectX::XMVECTOR zoomTarget = XMLoadFloat3(&target);
	DirectX::XMVECTOR zoomEye = XMLoadFloat3(&eye);
	DirectX::XMVECTOR HitPosition = XMLoadFloat3(&hitDelta);

	TargetManager* targetManager = nullptr;

public:
	void SetTargetManager(TargetManager* manager)
	{
		targetManager = manager;
	}


	DirectX::XMFLOAT3 target = DEFAULT_TARGET;//注視点
	DirectX::XMFLOAT3 angle = DEFAULT_ANGLE;//回転角度
	DirectX::XMFLOAT3 eye;

	float GetMaxRanget() { return maxRange; }

	DirectX::XMFLOAT3 GetCameraTarget() { return target; }
	DirectX::XMFLOAT3 GetCameraAngle() { return angle; }
	DirectX::XMFLOAT3 GetCameraEye() { return eye; }

	float range = maxRange;//距離

};