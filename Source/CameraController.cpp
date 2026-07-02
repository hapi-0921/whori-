#include"System/Input.h"
#include<imgui.h>
#include<string>

#include <DirectXCollision.h>
#include "Collision.h"

#include "System/Graphics.h"
#include"CameraController.h"
#include"Camera.h"
#include <algorithm>
#include "Stage.h"
#include "System/ShapeRenderer.h"
#include <cmath>

#undef min
#undef max

using namespace DirectX;

bool CameraController::cursorRay(DirectX::XMFLOAT3& hitDelta)
{
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	POINT cursor;
	::GetCursorPos(&cursor);
	::ScreenToClient(Graphics::Instance().GetWindowHandle(), &cursor);

	Camera& camera = Camera::Instance();
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR ScreenPosition, WorldPosition;
	DirectX::XMFLOAT3 screenPosition;
	screenPosition.x = static_cast<float>(cursor.x);
	screenPosition.y = static_cast<float>(cursor.y);

	//------------レイの始点-----------------
	screenPosition.z = 0.0f;
	ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);

	WorldPosition = DirectX::XMVector3Unproject
	(
		ScreenPosition,
		0.0f, 0.0f,
		screenWidth, screenHeight,
		0.0f, 1.0f,
		Projection,
		View,
		World
	);

	DirectX::XMStoreFloat3(&rayStart, WorldPosition);

	//--------------レイの終点-----------------
	screenPosition.z = 1.0f;
	ScreenPosition = DirectX::XMLoadFloat3(&screenPosition);

	WorldPosition = DirectX::XMVector3Unproject(
		ScreenPosition,
		0.0f, 0.0f,
		screenWidth, screenHeight,
		0.0f, 1.0f,
		Projection,
		View,
		World
	);

	DirectX::XMStoreFloat3(&rayEnd, WorldPosition);
	//------------------------------------------

	XMFLOAT3 realHit;
	bool hitReal = false;
	float nearest = FLT_MAX;

	Stage& stage = Stage::Instance();

	// 実際の6枚の壁
	for (int i = 0; i < 6; i++)
	{
		XMFLOAT3 hit, normal;
		if (Collision::RayCast(rayStart, rayEnd,
			stage.wall[i].transform, stage.GetWall(), hit, normal))
		{
			float distSq = XMVectorGetX(XMVector3LengthSq(//
				XMLoadFloat3(&hit) - XMLoadFloat3(&rayStart)));

			if (distSq < nearest)
			{
				nearest = distSq;
				realHit = hit;
				hitReal = true;
			}
		}
	}

	if (!hitReal) return false;

	// 中央の仮想壁
	XMFLOAT3 centerHit;
	XMFLOAT3 dummyNormal;
	if (!Collision::RayCast(rayStart, rayEnd,
		stage.centerWall.transform, stage.GetWall(), centerHit, dummyNormal))
	{
		return false;
	}

	// ================ ================
	XMVECTOR delta = XMLoadFloat3(&centerHit) - XMLoadFloat3(&realHit);

	//
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	XMVECTOR forward = XMVector3Normalize(rot.r[2]);
	float dot = XMVectorGetX(XMVector3Dot(delta, forward));
	delta = XMVectorSubtract(delta, XMVectorScale(forward, dot));

	// 反転防止
	XMVECTOR targetToHit = XMLoadFloat3(&centerHit) - XMLoadFloat3(&target);
	float check = XMVectorGetX(XMVector3Dot(delta, targetToHit));
	if (check < 0.0f)
	{
		delta = XMVectorNegate(delta);
	}

	XMStoreFloat3(&hitDelta, delta);

	return true;
}
//更新処理
void CameraController::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	//中ドラッグで回転
	if (mouse.GetButton() & Mouse::BTN_MIDDLE)
	{
		float dx = static_cast<float>(
			mouse.GetPositionX() - mouse.GetOldPositionX());

		float dy = static_cast<float>(
			mouse.GetPositionY() - mouse.GetOldPositionY());


		float mouseRotateSpeed = 0.01f;

		angle.y += dx * mouseRotateSpeed;
		angle.x += dy * mouseRotateSpeed;
	}

	//======回転の制限======
	//x軸
	angle.x = std::clamp(
		angle.x,
		-DirectX::XM_PIDIV2 + 0.01f,
		DirectX::XM_PIDIV2 - 0.01f
	);
	//ｙ軸
	if (angle.y < -DirectX::XM_PI) {
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI) {
		angle.y -= DirectX::XM_2PI;
	}


	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMFLOAT3 right;
	DirectX::XMStoreFloat3(&right, Transform.r[0]);

	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);


	//＊制作用
	{
		//左ドラッグで平行移動(パン移動)
		//if (mouse.GetButton() & Mouse::BTN_LEFT)
		//{
		//	float dx = static_cast<float>(
		//		mouse.GetPositionX() - mouse.GetOldPositionX());
		//	float dy = static_cast<float>(
		//		mouse.GetPositionY() - mouse.GetOldPositionY());
		//	float moveSpeed = 5.0f;
		//	target.x -= right.x * dx * moveSpeed;
		//	target.z -= right.z * dx * moveSpeed;
		//	target.x += front.x * dy * moveSpeed;
		//	target.z += front.z * dy * moveSpeed;
		//}
	}


	// ================== ホイールズーム ==================
	float wheel = ImGui::GetIO().MouseWheel;
	if (wheel != 0.0f)
	{
		// ================ ズームイン ================
		if (wheel > 0.0f)
		{
			XMFLOAT3 hitDelta;
			if (cursorRay(hitDelta))
			{
				XMVECTOR targetV = XMLoadFloat3(&target);
				XMVECTOR deltaV = XMLoadFloat3(&hitDelta);

				float distToHit = XMVectorGetX(XMVector3Length(deltaV));

				if (distToHit > 0.05f)
				{
					XMVECTOR dir = XMVector3Normalize(deltaV);

					float strength = wheel * 0.3f;
					float maxMove = distToHit * 0.42f;

					float moveAmount = std::min(strength * distToHit, maxMove);

					XMVECTOR offset = XMVectorScale(dir, moveAmount);
					XMVECTOR newTarget = targetV + offset;
					XMStoreFloat3(&target, newTarget);
				}
			}
		}
		// ================ ズームアウト ===============
		else
		{
			XMVECTOR targetV = XMLoadFloat3(&target);
			XMVECTOR center = XMVectorZero();

			XMVECTOR toCenter = center - targetV;
			float dist = XMVectorGetX(XMVector3Length(toCenter));

			if (dist > 0.2f)
			{
				XMVECTOR dir = XMVector3Normalize(toCenter);
				float strength = std::abs(wheel) * 0.20f;
				float moveDist = strength * dist * 0.1f;

				XMVECTOR offset = XMVectorScale(dir, moveDist);
				XMVECTOR newTarget = targetV + offset;
				XMStoreFloat3(&target, newTarget);
			}
		}

		// range調整
		range = std::clamp(range - wheel * zoomSpeed * 1.15f, minRange, maxRange);
		//range = range - wheel * zoomSpeed * 1.15f;
	}

	// カメラ更新
	float safeRange = std::max(range, 0.0001f);

	eye.x = target.x - front.x * safeRange;
	eye.y = target.y - front.y * safeRange;
	eye.z = target.z - front.z * safeRange;

	Camera::Instance().SetLookAt(eye, target, XMFLOAT3(0, 1, 0));
}

void CameraController::Render(const RenderContext& rc)
{
	ShapeRenderer* shapeRenderer = Graphics::Instance().GetShapeRenderer();

	XMFLOAT3 pos{ 0,0,0 };
	XMFLOAT4 color{ 1,1,1,1 };
	XMFLOAT4 color1{ 1,0,0,1 };
	//shapeRenderer->RenderSphere(rc, pos, 1.0f, color);
	shapeRenderer->RenderSphere(rc, rayStart, 0.001f, color);
	//shapeRenderer->RenderSphere(rc, hitPosition, 0.01f, color1);
}

void CameraController::DrawDebugGUI()
{

	//ウィンドウの位置
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 1000, pos.y + 10), ImGuiCond_Once);
	//ウィンドウサイズ
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	//ウィンドウ開始
	if (ImGui::Begin("camera", nullptr, ImGuiWindowFlags_None)) {
		//折り畳みメニュー
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			//位置
			ImGui::InputFloat3("target", &target.x);
			ImGui::InputFloat3("rayStart", &rayStart.x);
			//ズーム
			ImGui::InputFloat("range", &range);
			ImGui::Checkbox("hitRay", &hitRay);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);


		}
	}
	ImGui::End();

}