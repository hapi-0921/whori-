#include"Stage.h"
#include<imgui.h>
#include <algorithm>
#include "targetManager.h"
#include <Camera.h>

//コンストラクタ
Stage::Stage()
{

	if (stageType == MACHI)
	{
		stageNum = 4;

		mdlStage[0] = new Model("Data/Model/stage/machi/matimati/matimati1.mdl");
		mdlStage[1] = new Model("Data/Model/stage/machi/matimati/matimati2.mdl");
		mdlStage[2] = new Model("Data/Model/stage/machi/matimati/matimati3.mdl");
		mdlStage[3] = new Model("Data/Model/stage/machi/matimati/matimati4.mdl");
	}
	else if (stageType == SIMA)
	{
		stageNum = 7;

		mdlStage[0] = new Model("Data/Model/stage/shima/simasima/simasima1.1.mdl");
		mdlStage[1] = new Model("Data/Model/stage/shima/simasima/simasima1.2.mdl");
		mdlStage[2] = new Model("Data/Model/stage/shima/simasima/simasima1.3.mdl");
		mdlStage[3] = new Model("Data/Model/stage/shima/simasima/simasima1.mdl");
		mdlStage[4] = new Model("Data/Model/stage/shima/simasima/simasima2.mdl");
		mdlStage[5] = new Model("Data/Model/stage/shima/simasima/simasima3.mdl");
		mdlStage[6] = new Model("Data/Model/stage/shima/simasima/simasima4.mdl");
	}

	mdlCenterWall = new Model("Data/Model/wall/wall.mdl");

	centerWall[CENTER].angle.y = DirectX::XM_PI * 0.25f;
	centerWall[CENTER].scale.y = 5.0f;
	centerWall[CENTER].scale.z = 5.0f;

	centerWall[DEPTH].angle.y = DirectX::XM_PI * 0.25f;
	centerWall[DEPTH].scale.y = 4.5f;
	centerWall[DEPTH].scale.z = 8.0f;


}
Stage::~Stage()
{
	for (int i=0;i< stageNum;i++)
	{
		delete mdlStage[i];
	}

	delete mdlCenterWall;
}
//更新処理
void Stage::Update(float elapsedTime)
{
	Camera& mainCamera = Camera::Instance();
	DirectX::XMFLOAT3 cameraPos = mainCamera.GetEye();
	DirectX::XMFLOAT3 front = mainCamera.GetFront();
	DirectX::XMFLOAT3 target = camera->GetCameraTarget();

	//奥行き判定用壁
	float length_d = 2000;//farの半分に合わせる
	centerWall[DEPTH].position.x = mainCamera.GetEye().x+(front.x * length_d);
	centerWall[DEPTH].position.y = mainCamera.GetEye().y+(front.y * length_d);
	centerWall[DEPTH].position.z = mainCamera.GetEye().z+(front.z * length_d);

	centerWall[DEPTH].angle.z = camera->GetCameraAngle().x/*+DirectX::XM_PI * 0.5f*/;
	centerWall[DEPTH].angle.y = camera->GetCameraAngle().y + DirectX::XM_PI * 0.5f;
	freeUpdateTransform(centerWall[DEPTH].scale, centerWall[DEPTH].angle, centerWall[DEPTH].position, centerWall[DEPTH].transform);
	DirectX::XMStoreFloat3(&vecDepth,
		DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&centerWall[DEPTH].position)));


	//レイ用の壁
	float length_c = 1000;//farに合わせる
	centerWall[CENTER].position.x = mainCamera.GetEye().x + (front.x * length_c);
	centerWall[CENTER].position.y = mainCamera.GetEye().y + (front.y * length_c);
	centerWall[CENTER].position.z = mainCamera.GetEye().z + (front.z * length_c);

	centerWall[CENTER].angle.z = camera->GetCameraAngle().x/*+DirectX::XM_PI * 0.5f*/;
	centerWall[CENTER].angle.y = camera->GetCameraAngle().y + DirectX::XM_PI * 0.5f;
	freeUpdateTransform(centerWall[CENTER].scale, centerWall[CENTER].angle, centerWall[CENTER].position, centerWall[CENTER].transform);


}

void Stage::Render(const RenderContext& rc, ModelRenderer* renderer,StageTransform* stage)
{
	//ステージ描画
	freeUpdateTransform(stage->scale, stage->angle, stage->position, stage->transform);
	for (int i = 0; i < stageNum; i++)
	{
		renderer->Render(rc, stage->transform, mdlStage[i], ShaderId::Lambert);
	}

	//ズーム用判定壁
		//renderer->Render(rc, centerWall[CENTER].transform, mdlCenterWall, ShaderId::Lambert);
		
				//奥行き判定用壁
		//renderer->Render(rc, centerWall[DEPTH].transform, mdlCenterWall, ShaderId::Lambert);
		 

	
}

void Stage::DrawDebugGUI()
{

	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 200), ImGuiCond_Once);

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("stage", nullptr, ImGuiWindowFlags_None)) {
		//折り畳みメ
		//if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			//for (int i = 0; i < 6; i++)
			//{
			//	//位置
				ImGui::InputFloat3("Position", &centerWall[DEPTH].position.x);
				ImGui::InputFloat3("vecDepth", &vecDepth.x);
			//	//回転
				DirectX::XMFLOAT3 a;
				a.x = DirectX::XMConvertToDegrees(centerWall[DEPTH].angle.x);
				a.y = DirectX::XMConvertToDegrees(centerWall[DEPTH].angle.y);
				a.z = DirectX::XMConvertToDegrees(centerWall[DEPTH].angle.z);
				ImGui::InputFloat3("Angle", &a.x);
				centerWall[DEPTH].angle.x = DirectX::XMConvertToRadians(a.x);
				centerWall[DEPTH].angle.y = DirectX::XMConvertToRadians(a.y);
				centerWall[DEPTH].angle.z = DirectX::XMConvertToRadians(a.z);
			//	//スケール
			//	ImGui::InputFloat3("Scale", &wall[i].scale.x);
			//}
		}
		DirectX::XMFLOAT3 eye = camera->GetCameraEye();

		ImGui::Text("eye.x = %.1f", eye.x);
		ImGui::Text("eye.z = %.1f", eye.z);

		ImGui::Text("angle.y = %.3f", camera->GetCameraAngle().y);
	}
	ImGui::End();

}
