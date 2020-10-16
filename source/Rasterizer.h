#pragma once
#include "RasterDesc.h"
#include "Window.h"
#include "Scenegraph.h"
#include "Depthbuffer.h"
#include "UI.h"
#include "Texture.h"
#include <vector>

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void Render(const std::unique_ptr<Scenegraph>& pScenegraph, const std::unique_ptr<UI>& pUI);
	void Update(float elapsedSec);

	void AddBackgroundTexture(Texture* pTex);

	const Window& GetWindow() const;

	// Input:
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

	void ToggleViewtype();
	void ToggleBackground();
	void ToggleCullmode();
		
private:
	void Cleanup();
	void Initialize();

	void Clear();

	RasterDesc m_RasterDesc;
	Window m_Window;
	Depthbuffer m_Depthbuffer;

	std::vector<Texture*> m_pBackgrounds;
	uint32_t m_CurrentBackgroundIdx;

public:
	Rasterizer(const Rasterizer&) = delete;
	Rasterizer(Rasterizer&&) = delete;
	Rasterizer& operator=(const Rasterizer&) = delete;
	Rasterizer& operator=(Rasterizer&&) = delete;
};

