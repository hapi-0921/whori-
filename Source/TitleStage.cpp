//titleStage.cpp
#include "TitleStage.h"

TitleStage::TitleStage()
{
	mdlStage = new Model("Data/Model/stage/matimati.mdl");
	//モデルのロード
	{
		models.push_back(new Model("Data/Model/target/foods/Apple.mdl"));
		models.push_back(new Model("Data/Model/target/foods/Bread.mdl"));
		models.push_back(new Model("Data/Model/target/foods/cake.mdl"));
		models.push_back(new Model("Data/Model/target/foods/egg.mdl"));
		models.push_back(new Model("Data/Model/target/foods/tohu.mdl"));
		models.push_back(new Model("Data/Model/target/animals/ahiru.mdl"));
		models.push_back(new Model("Data/Model/target/animals/Sphere.mdl"));
		models.push_back(new Model("Data/Model/target/animals/tanuki.mdl"));
		models.push_back(new Model("Data/Model/target/animals/Stegosaurus.mdl"));
		models.push_back(new Model("Data/Model/target/animals/ShibaInu.mdl"));
	}

	//ランダムな位置に生成
	for (int i = 0;i < 30;i++)
	{
		Titleobj obj;

		obj.model = models[rand() % models.size()];
		obj.speed = 30.0f + rand() % 30;

		obj.angle.x = DirectX::XMConvertToRadians(static_cast<float>(rand() % 360));
		obj.angle.y = DirectX::XMConvertToRadians(static_cast<float>(rand() % 360));

		obj.position.x = static_cast<float>(rand() % 200 - 100);
		obj.position.y = static_cast<float>(rand() % 300 + 100);
		obj.position.z = 0;

		obj.rotateSpeed.x = 0.3f + (rand() % 100) / 100.0f;
		obj.rotateSpeed.y = 0.3f + (rand() % 100) / 100.0f;
		//obj.rotateSpeed.z = 0.3f + (rand() % 100) / 100.0f;

		objects.push_back(obj);
	}
}

TitleStage::~TitleStage()
{
	if (mdlStage != nullptr)
	{
		delete mdlStage;
		mdlStage = nullptr;
	}

	for (Model* model : models)
	{
		delete model;
	}
	models.clear();
}

void TitleStage::Update(float elapsedTime)
{
	const float groundY = 0.0f;
	const float resetY = -100.0f;


	for (auto& obj : objects)
	{
		obj.position.y -= obj.speed * elapsedTime;

		//obj.angle.x += obj.rotateSpeed.x * elapsedTime;
		//obj.angle.y += obj.rotateSpeed.y * elapsedTime;


		if (obj.position.y < resetY)
		{
			obj.model = models[rand() % models.size()];

			obj.position.x = static_cast<float>(rand() % 200 - 100);
			obj.position.y = static_cast<float>(rand() % 150 + 100);
		}

		freeUpdateTransform(
			obj.scale,
			obj.angle,
			obj.position,
			obj.transform
		);
	}


}

void TitleStage::SetZoom(float zoom)
{
	this->zoom = zoom;
}

void TitleStage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (auto& obj : objects)
	{
		renderer->Render(rc, obj.transform, obj.model, ShaderId::Lambert);
	}
}