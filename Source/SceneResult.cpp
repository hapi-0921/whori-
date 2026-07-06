#include "System/Graphics.h"
#include "SceneResult.h"
#include "System/Input.h"
#include "System/Mouse.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Stage.h"
#include"Camera.h"

// 初期化
void SceneResult::Initialize()
{

}

// 終了化
void SceneResult::Finalize()
{

}

// 更新処理
void SceneResult::Update(float elapsedTime)
{

}

// 描画処理
void SceneResult::Render()
{
	// 描画準備
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	// 2Dスプライト描画
	{


		// デバッグ用
#ifndef NDEBUG

#endif // NDEBUG

	}

	// 3Dモデル描画
	{
		
	}

}

// GUI描画
void SceneResult::DrawGUI()
{

}