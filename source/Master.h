#pragma once

//Project includes
#include "ETimer.h"
#include "Window.h"

class Scenegraph;
class Rasterizer;
class UI;

//Standard includes
#include <iostream>

//-----------------------------------------------------//
// Master class (I don't like working in the main.cpp) //
//-----------------------------------------------------//

class Master
{
public:
	Master();
	~Master();

	void Loop();
	void Terminate();
	bool ShouldTerminate() const;

	// Input:
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

	std::unique_ptr<Rasterizer>& GetRasterizer();
	std::unique_ptr<Scenegraph>& GetSceneGraph();

private:
	// Core elements:
	std::unique_ptr<Rasterizer, std::default_delete<Rasterizer>> m_pRasterizer;
	std::unique_ptr<Scenegraph, std::default_delete<Scenegraph>> m_pSceneGraph;
	std::unique_ptr<UI, std::default_delete<UI>> m_pUI;

	// Initialization:
	void Initialize();

	// Printing FPS:
	std::unique_ptr<Elite::Timer, std::default_delete<Elite::Timer>> m_pTimer;
	float m_PrintTimer;
	void PrintAfterFrame(float elapsedSec);

	// Termination:
	bool m_ShouldTerminate;
	void Cleanup();

	// AddingScenes:
	void InitScenes();

	// Adding UI:
	void InitUISheets();

public:
	Master(const Master& other) = delete;
	Master(Master&& other) = delete;
	void operator=(const Master& other) = delete;
	void operator=(Master&& other) = delete;
};

