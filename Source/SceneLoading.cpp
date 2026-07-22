#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "GameManager.h"

#include"Camera.h"
#include <imgui.h>
#include "imgui_impl_win32.h"
#include <ModelCommon.h>
#include <fstream>
#include <Stage.h>
// 初期化
void SceneLoading::Initialize()
{
	// スプライト初期化
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");
	sprFadeRect = new Sprite("Data/Sprite/FadeRect.png");

	// ステージのインスタンスを取得
	Stage& stage = Stage::Instance();

	// 食べ物
	LoadItemFood[0] = new Sprite("Data/Sprite/chain/foods/Apple.png");
	LoadItemFood[1] = new Sprite("Data/Sprite/chain/foods/Avocado.png");
	LoadItemFood[2] = new Sprite("Data/Sprite/chain/foods/Bacon_Burned.png");
	LoadItemFood[3] = new Sprite("Data/Sprite/chain/foods/Banana.png");
	LoadItemFood[4] = new Sprite("Data/Sprite/chain/foods/Bread.png");
	LoadItemFood[5] = new Sprite("Data/Sprite/chain/foods/Broccoli.png");
	LoadItemFood[6] = new Sprite("Data/Sprite/chain/foods/Burger.png");
	LoadItemFood[7] = new Sprite("Data/Sprite/chain/foods/cabbage.png");
	LoadItemFood[8] = new Sprite("Data/Sprite/chain/foods/cake.png");
	LoadItemFood[9] = new Sprite("Data/Sprite/chain/foods/Carrot.png");
	LoadItemFood[10] = new Sprite("Data/Sprite/chain/foods/cauliflower.png");
	LoadItemFood[11] = new Sprite("Data/Sprite/chain/foods/cherries.png");
	LoadItemFood[12] = new Sprite("Data/Sprite/chain/foods/ChocolateBar.png");
	LoadItemFood[13] = new Sprite("Data/Sprite/chain/foods/Coconut_Half.png");
	LoadItemFood[14] = new Sprite("Data/Sprite/chain/foods/cohi.png");
	LoadItemFood[15] = new Sprite("Data/Sprite/chain/foods/cookie.png");
	LoadItemFood[16] = new Sprite("Data/Sprite/chain/foods/corn.png");
	LoadItemFood[17] = new Sprite("Data/Sprite/chain/foods/Cow_Cube.png");
	LoadItemFood[18] = new Sprite("Data/Sprite/chain/foods/cup.png");
	LoadItemFood[19] = new Sprite("Data/Sprite/chain/foods/Cupcake.png");
	LoadItemFood[20] = new Sprite("Data/Sprite/chain/foods/Donut1.png");
	LoadItemFood[21] = new Sprite("Data/Sprite/chain/foods/egg.png");
	LoadItemFood[22] = new Sprite("Data/Sprite/chain/foods/Egg_Whole_White.png");
	LoadItemFood[23] = new Sprite("Data/Sprite/chain/foods/Eggplant.png");
	LoadItemFood[24] = new Sprite("Data/Sprite/chain/foods/Fries.png");
	LoadItemFood[25] = new Sprite("Data/Sprite/chain/foods/Hotdog.png");
	LoadItemFood[26] = new Sprite("Data/Sprite/chain/foods/IceCream_2.png");
	LoadItemFood[27] = new Sprite("Data/Sprite/chain/foods/kyuri.png");
	LoadItemFood[28] = new Sprite("Data/Sprite/chain/foods/lemon.001.png");
	LoadItemFood[29] = new Sprite("Data/Sprite/chain/foods/Mushroom.png");
	LoadItemFood[30] = new Sprite("Data/Sprite/chain/foods/onion.png");
	LoadItemFood[31] = new Sprite("Data/Sprite/chain/foods/Orange.png");
	LoadItemFood[32] = new Sprite("Data/Sprite/chain/foods/Pancake.png");
	LoadItemFood[33] = new Sprite("Data/Sprite/chain/foods/pear.png");
	LoadItemFood[34] = new Sprite("Data/Sprite/chain/foods/Pepper_Red.png");
	LoadItemFood[35] = new Sprite("Data/Sprite/chain/foods/Pig_Cylinder.png");
	LoadItemFood[36] = new Sprite("Data/Sprite/chain/foods/Pizza.png");
	LoadItemFood[37] = new Sprite("Data/Sprite/chain/foods/Pumpkin.png");
	LoadItemFood[38] = new Sprite("Data/Sprite/chain/foods/strawberry.png");
	LoadItemFood[39] = new Sprite("Data/Sprite/chain/foods/Sushi_Nigiri2.png");
	LoadItemFood[40] = new Sprite("Data/Sprite/chain/foods/tohu.png");
	LoadItemFood[41] = new Sprite("Data/Sprite/chain/foods/Tomato.png");
	LoadItemFood[42] = new Sprite("Data/Sprite/chain/foods/Tumip.png");
	LoadItemFood[43] = new Sprite("Data/Sprite/chain/foods/waffle.png");
	LoadItemFood[44] = new Sprite("Data/Sprite/chain/foods/Watermelon_0101.png");

	LoadItemAnimal[0] = new Sprite("Data/Sprite/chain/animals/Alpaca.png");
	LoadItemAnimal[1] = new Sprite("Data/Sprite/chain/animals/Bear.000.png");
	LoadItemAnimal[2] = new Sprite("Data/Sprite/chain/animals/Camel.002.png");
	LoadItemAnimal[3] = new Sprite("Data/Sprite/chain/animals/Cat_Base_1.002.png");
	LoadItemAnimal[4] = new Sprite("Data/Sprite/chain/animals/chamaL.003.png");
	LoadItemAnimal[5] = new Sprite("Data/Sprite/chain/animals/ChickenLP.png");
	LoadItemAnimal[6] = new Sprite("Data/Sprite/chain/animals/Cow.001.png");
	LoadItemAnimal[7] = new Sprite("Data/Sprite/chain/animals/Cube.001.png");
	LoadItemAnimal[8] = new Sprite("Data/Sprite/chain/animals/Cube.png");
	LoadItemAnimal[9] = new Sprite("Data/Sprite/chain/animals/Deer.png");
	LoadItemAnimal[10] = new Sprite("Data/Sprite/chain/animals/Dolphin.png");
	LoadItemAnimal[11] = new Sprite("Data/Sprite/chain/animals/Elephant.png");
	LoadItemAnimal[12] = new Sprite("Data/Sprite/chain/animals/Fox.png");
	LoadItemAnimal[13] = new Sprite("Data/Sprite/chain/animals/Frog.png");
	LoadItemAnimal[14] = new Sprite("Data/Sprite/chain/animals/Giraffe.png");
	LoadItemAnimal[15] = new Sprite("Data/Sprite/chain/animals/Hamster_fbx.png");
	LoadItemAnimal[16] = new Sprite("Data/Sprite/chain/animals/HippoEdited.001.png");
	LoadItemAnimal[17] = new Sprite("Data/Sprite/chain/animals/Horse.png");
	LoadItemAnimal[18] = new Sprite("Data/Sprite/chain/animals/koala.png");
	LoadItemAnimal[19] = new Sprite("Data/Sprite/chain/animals/kuwagata.png");
	LoadItemAnimal[20] = new Sprite("Data/Sprite/chain/animals/Low.png");
	LoadItemAnimal[21] = new Sprite("Data/Sprite/chain/animals/MantaRay.png");
	LoadItemAnimal[22] = new Sprite("Data/Sprite/chain/animals/Penguin.png");
	LoadItemAnimal[23] = new Sprite("Data/Sprite/chain/animals/Pig.png");
	LoadItemAnimal[24] = new Sprite("Data/Sprite/chain/animals/Rabbit.png");
	LoadItemAnimal[25] = new Sprite("Data/Sprite/chain/animals/Rat.png");
	LoadItemAnimal[26] = new Sprite("Data/Sprite/chain/animals/Rhino.png");
	LoadItemAnimal[27] = new Sprite("Data/Sprite/chain/animals/Seahorse FBX.png");
	LoadItemAnimal[28] = new Sprite("Data/Sprite/chain/animals/Shark.png");
	LoadItemAnimal[29] = new Sprite("Data/Sprite/chain/animals/Sheep.png");
	LoadItemAnimal[30] = new Sprite("Data/Sprite/chain/animals/ShibaInu.png");
	LoadItemAnimal[31] = new Sprite("Data/Sprite/chain/animals/Snake.png");
	LoadItemAnimal[32] = new Sprite("Data/Sprite/chain/animals/Sphere.png");
	LoadItemAnimal[33] = new Sprite("Data/Sprite/chain/animals/Stegosaurus.png");
	LoadItemAnimal[34] = new Sprite("Data/Sprite/chain/animals/tanuki.png");
	LoadItemAnimal[35] = new Sprite("Data/Sprite/chain/animals/Trex.png");
	LoadItemAnimal[36] = new Sprite("Data/Sprite/chain/animals/Triceratops.png");
	LoadItemAnimal[37] = new Sprite("Data/Sprite/chain/animals/Triceratops1.png");
	LoadItemAnimal[38] = new Sprite("Data/Sprite/chain/animals/Whale.png");
	LoadItemAnimal[39] = new Sprite("Data/Sprite/chain/animals/wolf.png");
	LoadItemAnimal[40] = new Sprite("Data/Sprite/chain/animals/Zebra.png");
	
	
	
	// 描画するアイテム番号をランダムに設定
	srand((unsigned int)time(NULL)); //乱数種の初期化

	if(stage.stageType = Stage::StageType::MACHI)
	{
		ItemNum[0] = rand() % 45;
		ItemNum[1] = rand() % 45;
		ItemNum[2] = rand() % 45;
		ItemNum[3] = rand() % 45;
	}
	if (stage.stageType = Stage::StageType::SIMA)
	{
		ItemNum[0] = rand() % 41;
		ItemNum[1] = rand() % 41;
		ItemNum[2] = rand() % 41;
		ItemNum[3] = rand() % 41;
	}

	// 動くアイテムの高さ
	ItemNumPosY[0] = 1080 / 2 - ItemSize / 2;
	ItemNumPosY[1] = 1080 / 2 - ItemSize / 2;
	ItemNumPosY[2] = 1080 / 2 - ItemSize / 2;
	ItemNumPosY[3] = 1080 / 2 - ItemSize / 2;

	// 元の位置を保存
	MotonoPosition = 1080 / 2 - ItemSize / 2;

	// 上がったかどうか
	MoveUp[0] = false;
	MoveUp[1] = false;
	MoveUp[2] = false;
	MoveUp[3] = false;

	// 下がったかどうか
	MoveDown[0] = false;
	MoveDown[1] = false;
	MoveDown[2] = false;
	MoveDown[3] = false;

	// 動きが終わったかどうか
	MoveEnd[0] = false;
	MoveEnd[1] = false;
	MoveEnd[2] = false;
	MoveEnd[3] = false;

	// 動くアイテムの順番
	MoveNum = 0;
	
	// スレッド開始
	thread = new std::thread(LoadingThread, this);

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		2000.0f
	);

	cameraController = new CameraController();
}

// 終了化
void SceneLoading::Finalize()
{
	// スレッド終了化
	if (thread != nullptr)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}

	// スプライト終了化
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
	if (sprFadeRect != nullptr)
	{
		delete sprFadeRect;
		sprFadeRect = nullptr;
	}
	for(int i = 0; i < 45; i++)
	{
		if (LoadItemFood[i] != nullptr)
		{
			delete LoadItemFood[i];
			LoadItemFood[i] = nullptr;
		}
	}
	for (int i = 0; i < 41; i++)
	{
		if (LoadItemAnimal[i] != nullptr)
		{
			delete LoadItemAnimal[i];
			LoadItemAnimal[i] = nullptr;
		}
	}

	// カメラコントローラー終了化
	if (cameraController != nullptr) {
		delete cameraController;
		cameraController = nullptr;
	}
}

// 更新処理
void SceneLoading::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;

	// ステージのインスタンスを取得
	Stage& stage = Stage::Instance();

	// 次のシーンの準備か完了したらシーンを切り替える
	if (nextScene != nullptr)
	{
		if (nextScene->IsReady())
		{
			//SceneManager::Instance().ChangeScene(nextScene);
			SceneManager::Instance().ChangeScene2(nextScene);
			nextScene = nullptr;
		}
	}


	// ロード中のアイテムカードの動き
	if(MoveNum < 4)
	{
		MoveUp[MoveNum] = true;
		if (MoveEnd[MoveNum] == false)
		{
			if (MoveUp[MoveNum] == true && MoveDown[MoveNum] == false)
			{
				ItemNumPosY[MoveNum] += (350 - ItemNumPosY[MoveNum]) * 0.05;
				if (fabs(ItemNumPosY[MoveNum] - 350.0f) < 1.0f)
				{
					ItemNumPosY[MoveNum] = 350;
					MoveDown[MoveNum] = true;
				}
			}
			if (MoveUp[MoveNum] == true && MoveDown[MoveNum] == true)
			{
				ItemNumPosY[MoveNum] += (MotonoPosition - ItemNumPosY[MoveNum]) * 0.05;
				if (fabs(ItemNumPosY[MoveNum] - MotonoPosition) < 1.0f)
				{
					ItemNumPosY[MoveNum] = MotonoPosition;
					MoveEnd[MoveNum] = true;
				}
			}
		}
		else
		{
			MoveNum++;
		}
	}
	else
	{
		MoveNum = 0;

		MoveUp[0] = false;
		MoveDown[0] = false;
		MoveEnd[0] = false;

		MoveUp[1] = false;
		MoveDown[1] = false;
		MoveEnd[1] = false;

		MoveUp[2] = false;
		MoveDown[2] = false;
		MoveEnd[2] = false;

		MoveUp[3] = false;
		MoveDown[3] = false;
		MoveEnd[3] = false;
	}
	
}

// 描画処理
void SceneLoading::Render()
{
	// ステージのインスタンスを取得
	Stage& stage = Stage::Instance();

	// 描画準備
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 2Dスプライト描画
	{
		// 画面サイズ
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		// 回転するやつ
		float spriteWidth = 256; // 回転するやつの大きさ
		float spriteHeight = 256;
		float positionX = screenWidth - spriteWidth;
		float positionY = screenHeight - spriteWidth;

		// フェード用の矩形
		sprFadeRect->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

		// 右下で回転するやつ
		/*sprite->Render(rc,
			positionX, positionY, 0, spriteWidth, spriteHeight,
			angle,
			1, 1, 1, 1);*/

		
		if(stage.stageType = Stage::StageType::MACHI)
		{
			// カード１
			LoadItemFood[ItemNum[0]]->Render(rc,
				ItemPositionX - ItemSize * 2, ItemNumPosY[0], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
			// カード２
			LoadItemFood[ItemNum[1]]->Render(rc,
				ItemPositionX - ItemSize * 1, ItemNumPosY[1], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
			// カード３
			LoadItemFood[ItemNum[2]]->Render(rc,
				ItemPositionX + ItemSize * 0, ItemNumPosY[2], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
			// カード４
			LoadItemFood[ItemNum[3]]->Render(rc,
				ItemPositionX + ItemSize * 1, ItemNumPosY[3], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
		}
		if (stage.stageType = Stage::StageType::SIMA)
		{
			// カード１
			LoadItemAnimal[ItemNum[0]]->Render(rc,
				ItemPositionX - ItemSize * 2, ItemNumPosY[0], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
			// カード２
			LoadItemAnimal[ItemNum[1]]->Render(rc,
				ItemPositionX - ItemSize * 1, ItemNumPosY[1], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
			// カード３
			LoadItemAnimal[ItemNum[2]]->Render(rc,
				ItemPositionX + ItemSize * 0, ItemNumPosY[2], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
			// カード４
			LoadItemAnimal[ItemNum[3]]->Render(rc,
				ItemPositionX + ItemSize * 1, ItemNumPosY[3], 0, ItemSize, ItemSize,
				0,
				1, 1, 1, 1);
		}
	}


	//3D
	{

		
	}

	cameraController->Render(rc);
}

// GUI描画
void SceneLoading::DrawGUI()
{
	ImGui::Begin("Loading Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Loading Scene Debug");

	
	ImGui::InputFloat("ItemPosY", &ItemNumPosY[0]);

	ImGui::Separator();

	ImGui::End();

}
// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	// COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	// 次のシーンの初期化を行う
	scene->nextScene->Initialize();

	// ロード時間を伸ばす
	Sleep(4000);

	// スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	// 次のシーンの準備完了設定
	scene->nextScene->SetReandy();

}