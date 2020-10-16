#include "pch.h"
#include "Scenegraph.h"
#include "Scene.h"
#include "CatchInput.h"

Scenegraph::Scenegraph()
	: m_pScenes{}
	, m_CurrentSceneIdx{}
{

}

Scenegraph::~Scenegraph()
{
	for (Scene* pScene : m_pScenes)
	{
		delete pScene;
		pScene = nullptr;
	}
}

void Scenegraph::AddScene(Scene* pScene)
{
	m_pScenes.emplace_back(pScene);
}

Scene* Scenegraph::GetActiveScene()
{
	return m_pScenes[m_CurrentSceneIdx];
}

void Scenegraph::ToggleActiveScene()
{
	++m_CurrentSceneIdx;

	if (m_CurrentSceneIdx >= m_pScenes.size()) m_CurrentSceneIdx = 0;
}

void Scenegraph::Update(float elapsedSec)
{
	if (m_pScenes.empty()) return;

	m_pScenes[m_CurrentSceneIdx]->Update(elapsedSec);
}

bool Scenegraph::IsEmpty() const
{
	return m_pScenes.empty();
}

#pragma region Input
void Scenegraph::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	m_pScenes[m_CurrentSceneIdx]->ProcessKeyDownEvent(e);
}
void Scenegraph::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDL_Keycode(Action::ToggleScene):
		ToggleActiveScene();
		break;
	}

	m_pScenes[m_CurrentSceneIdx]->ProcessKeyUpEvent(e);
}
void Scenegraph::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_pScenes[m_CurrentSceneIdx]->ProcessMouseMotionEvent(e);
}
void Scenegraph::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	m_pScenes[m_CurrentSceneIdx]->ProcessMouseDownEvent(e);
}
void Scenegraph::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	m_pScenes[m_CurrentSceneIdx]->ProcessMouseUpEvent(e);
}
#pragma endregion