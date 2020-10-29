#include "pch.h"
#include "Rasterizer.h"
#include "Scene.h"
#include "Camera.h"
#include "TriMesh.h"
#include "CatchInput.h"
#include "Albedo.h"
#include "UISheet.h"
#include "UIElement.h"

#pragma region Initialization
Rasterizer::Rasterizer()
	: m_Window{ "Re-Rasterizer - Arne Van Kerschaver", 1280, 900 }
{
	Initialize();
}

void Rasterizer::Initialize()
{
	m_RasterDesc.m_ClearColor = Elite::RGBColor{ Albedo::WineRed };
	m_RasterDesc.m_Viewtype = Viewtype::Materials;
	m_RasterDesc.m_BGType = Backgroundtype::Color;
	m_RasterDesc.m_Cullmode = Cullmode::Backface;

	m_Depthbuffer = Depthbuffer{ uint32_t(m_Window.GetWindowDimensions().x), uint32_t(m_Window.GetWindowDimensions().y), FLT_MAX };
}
#pragma endregion

#pragma region Cleanup
Rasterizer::~Rasterizer()
{
	Cleanup();
}

void Rasterizer::Cleanup()
{
	for (Texture* pTexture : m_pBackgrounds)
	{
		delete pTexture;
		pTexture = nullptr;
	}
}
#pragma endregion

void Rasterizer::Render(const std::unique_ptr<Scenegraph>& pScenegraph, const std::unique_ptr<UI>& pUI)
{
	// Lock the backbuffer
	m_Window.LockBB();
	// ClearScreen:
	Clear();

	if (pScenegraph->IsEmpty())
	{
		m_Window.Blit();
		return;
	}

	float AR = float(m_Window.GetWindowDimensions().x) / float(m_Window.GetWindowDimensions().y);

	// Get Camera Matrices:
	FMatrix4 viewMatrix = Elite::Inverse(pScenegraph->GetActiveScene()->GetCurrentCamera()->GetONB());
	FMatrix4 projMatrix = pScenegraph->GetActiveScene()->GetCurrentCamera()->GetProj(AR);
	FPoint3 camPos = pScenegraph->GetActiveScene()->GetCurrentCamera()->GetPosition();

	// For every object:
	for (TriMesh* pMesh : pScenegraph->GetActiveScene()->GetMeshes())
	{
		pMesh->Project(m_Window.GetWindowDimensions(), viewMatrix, projMatrix);
		pMesh->Rasterize(m_Window, m_RasterDesc, m_Depthbuffer, camPos, pScenegraph->GetActiveScene()->GetLights());
	}

	// For every ui element:
	for (UIElement* pUIElement : pUI->GetActiveSheet()->GetElements())
	{
		if (!pUIElement->ShouldRender()) continue;

		pUIElement->Render(m_Window);
	}
	
	m_Window.Blit();
}

void Rasterizer::Clear()
{
	switch (m_RasterDesc.m_BGType)
	{
	case Backgroundtype::Color:
		m_Window.ColorClear(m_RasterDesc.m_ClearColor);
		break;

	case Backgroundtype::Texture:
		if (m_pBackgrounds.empty()) break;

		m_Window.TextureClear(m_pBackgrounds[m_CurrentBackgroundIdx]);
		break;
	}
	
	m_Depthbuffer.Reset();
}


void Rasterizer::Update(float elapsedSec)
{

}

#pragma region Input
void Rasterizer::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{}
void Rasterizer::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDL_Keycode(Action::ToggleViewtype):
		ToggleViewtype();
		break;
	}
	
}

void Rasterizer::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{}
void Rasterizer::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{}
void Rasterizer::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{}
#pragma endregion

void Rasterizer::ToggleViewtype()
{
	m_RasterDesc.m_Viewtype = Viewtype(int(m_RasterDesc.m_Viewtype) + 1);
	if (int(m_RasterDesc.m_Viewtype) >= int(Viewtype::END)) m_RasterDesc.m_Viewtype = Viewtype(0);
}

void Rasterizer::ToggleBackground()
{
	if (m_RasterDesc.m_BGType == Backgroundtype::Texture)
	{
		++m_CurrentBackgroundIdx;

		if (m_CurrentBackgroundIdx >= m_pBackgrounds.size())
		{
			m_RasterDesc.m_BGType = Backgroundtype::Color;
			m_CurrentBackgroundIdx = 0;
		}
	}

	else if (m_RasterDesc.m_BGType == Backgroundtype::Color)
	{
		if (m_pBackgrounds.empty()) return;

		m_RasterDesc.m_BGType = Backgroundtype::Texture;
	}
}

void Rasterizer::ToggleCullmode()
{
	m_RasterDesc.m_Cullmode = Cullmode(int(m_RasterDesc.m_Cullmode) + 1);
	if (int(m_RasterDesc.m_Cullmode) >= int(Cullmode::END)) m_RasterDesc.m_Cullmode = Cullmode(0);
}

void Rasterizer::AddBackgroundTexture(Texture* pTex)
{
	m_pBackgrounds.emplace_back(pTex);
}

const Window& Rasterizer::GetWindow() const
{
	return m_Window;
}