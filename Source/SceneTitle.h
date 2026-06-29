#pragma once
#include <System/Model.h>
#include <System/ModelRenderer.h>


class SceneTitle
{
public:
	SceneTitle();

	void Initialize();
	void Finalize();
	void Update();
	void Render(const RenderContext& rc, ModelRenderer* renderer);



private:
	enum class State
	{
		Wait,
		Zoom
	};
	State m_state = State::Wait;

	enum class ZoomState
	{
		Approach,
		Cubeanim,
		PullBack
	};
	ZoomState m_zoomState = ZoomState::Approach;

	Model* titleCube = nullptr;
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};