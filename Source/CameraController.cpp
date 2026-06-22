#include"System/Input.h"
#include<imgui.h>
#include<string>

#include"CameraController.h"
#include"Camera.h"
#include <algorithm>

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

	//ホイールで拡大・縮小
	float wheel = ImGui::GetIO().MouseWheel;
	range -= wheel * 70;

	//========ズームの制限=======
	if (range < minRange) {
		range = minRange;
	}
	if (range > maxRange) {
		range = maxRange;
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
		//中ドラッグで平行移動
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

		eye.x = target.x - front.x * range;
		eye.y = target.y - front.y * range;
		eye.z = target.z - front.z * range;

		Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
	
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
			//ズーム
			ImGui::InputFloat("range", &range);
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