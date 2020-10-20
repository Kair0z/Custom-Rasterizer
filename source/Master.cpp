#include "pch.h"
#include "Master.h"
#include "CatchInput.h"
#include <iostream>

#include "ScenesLib.h"
#include "UISheetLib.h"

#include "Rasterizer.h"
#include "Scenegraph.h"
#include "UI.h"

#pragma region Initialization
Master::Master()
	: m_pRasterizer{}
	, m_pSceneGraph{}
	, m_pUI{}
	, m_pTimer{}
	, m_ShouldTerminate{false}
	, m_PrintTimer{}
{
	CatchInput::PrintInputCommands();

	Initialize();

	// Seting up scenes:
	InitScenes();

	// Setting up UI:
	InitUISheets();
}

void Master::Initialize()
{
	m_pSceneGraph = std::make_unique<Scenegraph>();
	m_pRasterizer = std::make_unique<Rasterizer>();
	m_pUI = std::make_unique<UI>();

	m_pTimer = std::make_unique<Elite::Timer>();
	m_pTimer->Start();

	m_pRasterizer->AddBackgroundTexture(new Texture{"Resources/Backgrounds/starwars_bg.png"});
	m_pRasterizer->AddBackgroundTexture(new Texture{"Resources/Backgrounds/startrek_bg.png"});

}
#pragma endregion

#pragma region Cleanup
Master::~Master()
{
	Cleanup();
}

void Master::Cleanup()
{
}
#pragma endregion

// Setup scenes to render:
void Master::InitScenes()
{
	//m_pSceneGraph->AddScene(PremadeScenes::Vehicle());
	m_pSceneGraph->AddScene(PremadeScenes::Tie());
	m_pSceneGraph->AddScene(PremadeScenes::Box());
}

// Setup UISheets:
void Master::InitUISheets()
{
	m_pUI->AddSheet(PremadeSheets::ButtonSheet(this));
}

// Loop:
void Master::Loop()
{
	//--------- Timer ---------
	m_pTimer->Update();
	float elapsedSec = m_pTimer->GetElapsed();

	//------- Scenegraph -------
	m_pSceneGraph->Update(elapsedSec);

	//--------- UI -------------
	m_pUI->Update(elapsedSec);

	//--------- Renderers ------
	m_pRasterizer->Update(elapsedSec);
	m_pRasterizer->Render(m_pSceneGraph, m_pUI);
	
	PrintAfterFrame(elapsedSec);
}

void Master::PrintAfterFrame(float elapsedSec)
{
	m_PrintTimer += elapsedSec;

	if (m_PrintTimer >= 1.f)
	{
		m_PrintTimer = 0.f;

		Utils::ChangeConsoleTextColor(Utils::TextAttribute::Green);

		std::cout << "FPS: " << m_pTimer->GetFPS() << "\r";

		Utils::ChangeConsoleTextColor(Utils::TextAttribute::White);
	}
}

std::unique_ptr<Rasterizer>& Master::GetRasterizer()
{
	return m_pRasterizer;
}

std::unique_ptr<Scenegraph>& Master::GetSceneGraph()
{
	return m_pSceneGraph;
}

#pragma region InputHandling
void Master::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	bool inputConsumed{ false };
	// Process UI keydown:
	m_pUI->ProcessKeyDownEvent(e, inputConsumed);

	if (inputConsumed) return;

	// Process Master keydown:

	// Process Renderer keydown:
	m_pRasterizer->ProcessKeyDownEvent(e);

	// Process Scenegraph keydown:
	m_pSceneGraph->ProcessKeyDownEvent(e);
}

void Master::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	bool inputConsumed{ false };

	// Process UI keyup:
	m_pUI->ProcessKeyUpEvent(e, inputConsumed);

	if (inputConsumed) return;

	// Process Master keyup:
	switch (e.keysym.sym)
	{
	case SDL_Keycode(Action::PrintInput):
		CatchInput::PrintInputCommands();
		break;
	}

	// Process Renderer keyup:
	m_pRasterizer->ProcessKeyUpEvent(e);

	// Process Scenegraph keyup:
	m_pSceneGraph->ProcessKeyUpEvent(e);
}

void Master::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	bool inputConsumed{ false };

	// Process UI mousemotion:
	m_pUI->ProcessMouseMotionEvent(e, inputConsumed);
	if (inputConsumed) return;

	// Process Master mousemotion:

	// Process Renderer mousemotion:
	m_pRasterizer->ProcessMouseMotionEvent(e);

	// Process Scenegraph mousemotion:
	m_pSceneGraph->ProcessMouseMotionEvent(e);
}

void Master::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	bool inputConsumed{ false };

	// Process UI mousedown:
	m_pUI->ProcessMouseDownEvent(e, inputConsumed);
	if (inputConsumed) return;

	// Process Master mousedown:

	// Process Renderer mousedown:
	m_pRasterizer->ProcessMouseDownEvent(e);

	// Process Scenegraph mousedown:
	m_pSceneGraph->ProcessMouseDownEvent(e);
}

void Master::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	bool inputConsumed{ false };

	// Process UI mouseup:
	m_pUI->ProcessMouseUpEvent(e, inputConsumed);
	if (inputConsumed) return;

	// Process Master mouseup:

	// Process Renderer mouseup:
	m_pRasterizer->ProcessMouseUpEvent(e);

	// Process Scenegraph mouseup:
	m_pSceneGraph->ProcessMouseUpEvent(e);
}
#pragma endregion

#pragma region Termination
void Master::Terminate()
{
	m_ShouldTerminate = true;
}

bool Master::ShouldTerminate() const
{
	return m_ShouldTerminate;
}
#pragma endregion
