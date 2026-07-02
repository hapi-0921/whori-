#include"Stage.h"
#include<imgui.h>
#include <algorithm>
#include "targetManager.h"

//コンストラクタ
Stage::Stage()
{
	//ステージ読み込み
	mdlStage = new Model("Data/Model/classroom/class_set/classroom_demoscene.mdl");
	mdlWall = new Model("Data/Model/wall/wall.mdl");
	mdlCenterWall = new Model("Data/Model/wall/wall.mdl");

	centerWall.angle.y = DirectX::XM_PI * 0.25f;
	centerWall.scale.y = 5.0f;
	centerWall.scale.z = 5.0f;


	wall[FRONT].angle.y = DirectX::XM_PI * 0.5f;
	wall[LEFT].angle.y = 0;
	wall[RIGHT].angle.y = 0;
	wall[BACK].angle.y = DirectX::XM_PI * 0.5f;
	wall[UP].angle.z = DirectX::XM_PI * 0.5f;
	wall[DOWN].angle.z = DirectX::XM_PI * 0.5f;

	wall[FRONT].position = { 0,stageSizeY * 0.5f,stageSizeX };
	wall[LEFT].position = { -stageSizeX,stageSizeY * 0.5f,0 };
	wall[RIGHT].position = { stageSizeX,stageSizeY * 0.5f,0 };
	wall[BACK].position = { 0, stageSizeY * 0.5f ,-stageSizeX };
	wall[UP].position = { 0,stageSizeY,0 };
	wall[DOWN].position = { 0,0,0 };
}
Stage::~Stage()
{
	delete mdlStage;
	delete mdlWall;
	delete mdlCenterWall;
}
//更新処理
void Stage::Update(float elapsedTime)
{
	for (int i = 0; i < 6; i++)//実際の壁
	{
		freeUpdateTransform(wall[i].scale, wall[i].angle, wall[i].position, wall[i].transform);
	}

	//レイ用の壁
	centerWall.angle.z = camera->GetCameraAngle().x/*+DirectX::XM_PI * 0.5f*/;
	centerWall.angle.y = camera->GetCameraAngle().y + DirectX::XM_PI * 0.5f;
	freeUpdateTransform(centerWall.scale, centerWall.angle, centerWall.position, centerWall.transform);


	FrontWall();

}

void Stage::FrontWall()
{
	if (camera == nullptr)	return;

	distancesX.clear();
	distancesY.clear();

	for (int i = 0; i < 4; i++)
	{
		wall[i].isFrontWallX = false;
	}
	for (int i = 4; i < 6; i++)
	{
		wall[i].isFrontWallY = false;
	}

	// 部屋の中心
	DirectX::XMFLOAT3 roomCenterPos = { -150,0,-150 };
	DirectX::XMVECTOR roomCenter = DirectX::XMLoadFloat3(&roomCenterPos);

	// カメラ位置
	DirectX::XMFLOAT3 eye = camera->GetCameraEye();
	DirectX::XMVECTOR cameraPos = DirectX::XMLoadFloat3(&eye);

	// 部屋中心 → カメラ
	DirectX::XMVECTOR cameraDir = DirectX::XMVector3Normalize(
		DirectX::XMVectorSubtract(cameraPos, roomCenter));


	//横壁
	{
		for (int i = 0; i < 4; i++)
		{
			DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&wall[i].position);

			// 部屋中心 → 壁中心
			DirectX::XMVECTOR wallDir = DirectX::XMVector3Normalize(
				DirectX::XMVectorSubtract(center, roomCenter));


			float dot = DirectX::XMVectorGetX(
				DirectX::XMVector3Dot(cameraDir, wallDir));

			distancesX.push_back({ i, dot });
		}

		std::sort(distancesX.begin(), distancesX.end(),
			[](const WallDistance& a, const WallDistance& b)
			{
				return a.distance > b.distance;
			}
		);

		const float EPS = 0.2f;

		if (fabs(distancesX[1].distance - distancesX[2].distance) < EPS)
		{
			// 1枚消す
			for (int i = 0; i < 4; i++)
			{
				wall[distancesX[i].index].isFrontWallX = false;
			}
			wall[distancesX[0].index].isFrontWallX = true;
		}
		else
		{
			// 2枚消す
			for (int i = 0; i < 4; i++)
			{
				wall[distancesX[i].index].isFrontWallX = false;
			}
			wall[distancesX[0].index].isFrontWallX = true;
			wall[distancesX[1].index].isFrontWallX = true;
		}
	}
	//上下壁
	{
		for (int i = 4; i < 6; i++)
		{
			DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&wall[i].position);

			// 部屋中心 → 壁中心
			DirectX::XMVECTOR wallDir = DirectX::XMVector3Normalize(
				DirectX::XMVectorSubtract(center, roomCenter));


			float dot = DirectX::XMVectorGetX(
				DirectX::XMVector3Dot(cameraDir, wallDir));

			distancesY.push_back({ i, dot });
		}

		std::sort(distancesY.begin(), distancesY.end(),
			[](const WallDistance& a, const WallDistance& b)
			{
				return a.distance > b.distance;
			}
		);

		const float EPS = 0.3f;

		if (fabs(distancesY[0].distance - distancesY[1].distance) < EPS)
		{
			// 1枚消す
			for (int i = 0; i < 2; i++)
			{
				wall[distancesY[i].index].isFrontWallY = false;
			}
		}
		else
		{
			//消さない
			for (int i = 0; i < 2; i++)
			{
				wall[distancesY[i].index].isFrontWallY = false;
			}
			wall[distancesY[0].index].isFrontWallY = true;

		}


		wall[distancesY[0].index].isFrontWallY = true;
	}

}
void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//ステージ描画
	//renderer->Render(rc, transform, mdlStage, ShaderId::Lambert);

	{//ズーム用判定壁
		//renderer->Render(rc, centerWall.transform, mdlCenterWall, ShaderId::Lambert);
		for (int i = 0; i < 4; i++)//X壁
		{
			if (!wall[i].isFrontWallX)
			{
				//renderer->Render(rc, wall[i].transform, mdlWall, ShaderId::Lambert);
			}
		}
		for (int i = 4; i < 6; i++)//y壁
		{
			if (!wall[i].isFrontWallY)
			{
				//renderer->Render(rc, wall[i].transform, mdlWall, ShaderId::Lambert);
			}
		}
	}
}

void Stage::DrawDebugGUI()
{
	return;

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 200), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("stage", nullptr, ImGuiWindowFlags_None)) {
		//折り畳みメ
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (int i = 0; i < 6; i++)
			{
				//位置
				ImGui::InputFloat3("Position", &wall[i].position.x);
				//回転
				DirectX::XMFLOAT3 a;
				a.x = DirectX::XMConvertToDegrees(wall[i].angle.x);
				a.y = DirectX::XMConvertToDegrees(wall[i].angle.y);
				a.z = DirectX::XMConvertToDegrees(wall[i].angle.z);
				ImGui::InputFloat3("Angle", &a.x);
				wall[i].angle.x = DirectX::XMConvertToRadians(a.x);
				wall[i].angle.y = DirectX::XMConvertToRadians(a.y);
				wall[i].angle.z = DirectX::XMConvertToRadians(a.z);
				//スケール
				ImGui::InputFloat3("Scale", &wall[i].scale.x);

			}
		}
		DirectX::XMFLOAT3 eye = camera->GetCameraEye();

		ImGui::Text("eye.x = %.1f", eye.x);
		ImGui::Text("eye.z = %.1f", eye.z);

		ImGui::Text("angle.y = %.3f", camera->GetCameraAngle().y);
	}
	ImGui::End();

}
