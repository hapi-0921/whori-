#include "SceneTitle.h"
#include <System/Mouse.h>
#include <System/Input.h>




void SceneTitle::Initialize()
{
	titleCube = new Model("Data/Model/TitleCube/TitleCube.mdl");
}


void SceneTitle::Finalize()
{

}


//XV
void SceneTitle::Update()
{
	switch (m_state)
	{
	case State::Wait:
		if (Input::Instance().GetMouse().GetWheel() > 0)
		{
			m_state = State::Zoom;
		}
		break;

	case State::Zoom:
		switch (m_zoomState)
		{
		case ZoomState::Approach:
			break;

		case ZoomState::Cubeanim:
			break;

		case ZoomState::PullBack:
			break;
		}
		break;
	}
}

void SceneTitle::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, titleCube, ShaderId::Lambert);
}