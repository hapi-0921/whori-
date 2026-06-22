#include"Stage.h"
#include<imgui.h>
#include <algorithm>

//コンストラクタ
Stage::Stage()
{
	//ステージ読み込み
	model = new Model("Data/Model/classroom/class_set/classroom_demoscene.mdl");
	wall_mdl = new Model("Data/Model/cube/wall.mdl");

	wall[FRONT_R].angle.y = DirectX::XM_PI * 0.25f;
	wall[FRONT_L].angle.y = -DirectX::XM_PI * 0.25f;
	wall[BACK_R].angle.y = -DirectX::XM_PI * 0.25f;
	wall[BACK_L].angle.y = DirectX::XM_PI * 0.25f;

	wall[UP].angle.z = DirectX::XM_PI * 0.5f;
	wall[UP].angle.y = DirectX::XM_PI * 0.25f;
	wall[DOWN].angle.z = DirectX::XM_PI * 0.5f;
	wall[DOWN].angle.y = DirectX::XM_PI * 0.25f;

	wall[FRONT_R].position = { -stageSize,0,0 };
	wall[FRONT_L].position = { 0,0,0 };
	wall[BACK_R].position= {-stageSize,0,-stageSize };
	wall[BACK_L].position = { 0,0,-stageSize };
	wall[UP].position= {0,0,0};
	wall[DOWN].position = { 0,0,0 };
}
Stage::~Stage()
{
	//ステージモデルを破棄
	delete model;
	delete wall_mdl;
}
//更新処理
void Stage::Update(float elapsedTime)
{
	for (int i = 0; i < 6; i++)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(wall[i].scale.x, wall[i].scale.y, wall[i].scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(wall[i].angle.x, wall[i].angle.y, wall[i].angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(wall[i].position.x, wall[i].position.y, wall[i].position.z);
		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMStoreFloat4x4(&wall[i].transform, W);
	}
	FrontWall();

}

void Stage::FrontWall()
{
	if (camera == nullptr)	return;


	distances.clear();
	for (int i = 0; i < 6; i++)
	{
		wall[i].isFrontWall = false;
	}

	// 部屋の中心
	DirectX::XMFLOAT3 roomCenterPos = { -150,0,-150 };
	DirectX::XMVECTOR roomCenter = DirectX::XMLoadFloat3(&roomCenterPos);

	// カメラ位置
	DirectX::XMFLOAT3 eye = camera->GetCameraEye();
	DirectX::XMVECTOR cameraPos = DirectX::XMLoadFloat3(&eye);

	// 部屋中心 → カメラ
	DirectX::XMVECTOR cameraDir =DirectX::XMVector3Normalize(
			DirectX::XMVectorSubtract(cameraPos,roomCenter));

	for (int i = 0; i < 4; i++)
	{
		DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&wall[i].position);

		// 部屋中心 → 壁中心
		DirectX::XMVECTOR wallDir =DirectX::XMVector3Normalize(
			DirectX::XMVectorSubtract(center,roomCenter));


		float dot =DirectX::XMVectorGetX(
				DirectX::XMVector3Dot(cameraDir,wallDir));

		distances.push_back({ i, dot });
	}

	std::sort(distances.begin(),distances.end(),
		[](const WallDistance& a,const WallDistance& b)
		{
			return a.distance > b.distance;
		}
	);

	const float EPS = 0.3f;

	if (fabs(distances[1].distance - distances[2].distance) < EPS)
	{
		// 1枚消す
		for (int i = 0; i < 4; i++)
		{
			wall[distances[i].index].isFrontWall = false;
		}
		wall[distances[0].index].isFrontWall = true;
	}
	else
	{
		// 2枚消す
		for (int i = 0; i < 4; i++)
		{
			wall[distances[i].index].isFrontWall = false;
		}
		wall[distances[0].index].isFrontWall = true;
		wall[distances[1].index].isFrontWall = true;
	}
}

void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//ステージ描画
	renderer->Render(rc, transform, model, ShaderId::Lambert);

	//壁の描画
	for (int i = 0; i < 6; i++)
	{
		if (!wall[i].isFrontWall) 
		{
			renderer->Render(rc, wall[i].transform, wall_mdl, ShaderId::Lambert);
		}
	}
}

void Stage::DrawDebugGUI()
{
	//ウィンドウの位置
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 200), ImGuiCond_Once);
	//ウィンドウサイズ
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	//ウィンドウ開始
	if (ImGui::Begin("stage", nullptr, ImGuiWindowFlags_None)) {
		//折り畳みメニュー
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
		ImGui::Text("0 : %d", wall[0].isFrontWall);
		ImGui::Text("1 : %d", wall[1].isFrontWall);
		ImGui::Text("2 : %d", wall[2].isFrontWall);
		ImGui::Text("3 : %d", wall[3].isFrontWall);

		DirectX::XMFLOAT3 eye = camera->GetCameraEye();

		ImGui::Text("eye.x = %.1f", eye.x);
		ImGui::Text("eye.z = %.1f", eye.z);

		ImGui::Text("angle.y = %.3f", camera->GetCameraAngle().y);
	}
	ImGui::End();

}
