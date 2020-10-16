#include "pch.h"
#include "Scene.h"
#include "CatchInput.h"

#include "Camera.h"
#include "TriMesh.h"
#include "Light.h"

#pragma region Initialization
Scene::Scene()
	: m_CurCamIdx{}
	, m_ShouldUpdate{true}
{

}
#pragma endregion

#pragma region Cleanup
Scene::~Scene()
{
	Cleanup();
}

void Scene::Cleanup()
{
	for (Light* pLight : m_pLights)
	{
		delete pLight;
		pLight = nullptr;
	}

	for (TriMesh* pMesh : m_pMeshes)
	{
		delete pMesh;
		pMesh = nullptr;
	}

	for (Camera* pCamera : m_pCameras)
	{
		delete pCamera;
		pCamera = nullptr;
	}
}
#pragma endregion

#pragma region Adders
void Scene::AddLight(Light* pLight)
{
	m_pLights.emplace_back(pLight);
}

void Scene::AddCamera(Camera* pCamera)
{
	m_pCameras.emplace_back(pCamera);
}

void Scene::AddMesh(TriMesh* pMesh)
{
	m_pMeshes.emplace_back(pMesh);
}
#pragma endregion

#pragma region Accessors
Camera* Scene::GetCurrentCamera()
{
	return m_pCameras[m_CurCamIdx];
}

std::vector<Light*> Scene::GetLights()
{
	return m_pLights;
}

std::vector<TriMesh*> Scene::GetMeshes()
{
	return m_pMeshes;
}
#pragma endregion

void Scene::Update(float elapsedSec)
{
	m_pCameras[m_CurCamIdx]->ProcessInput(elapsedSec);

	if (!m_ShouldUpdate) return;

	for (TriMesh* pMesh : m_pMeshes)
	{
		pMesh->Update(elapsedSec);
	}
}

#pragma region Input
void Scene::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	// Meshes keydown:

	// Active-camera keydown:
	m_pCameras[m_CurCamIdx]->ProcessKeyDownEvent(e);

	// Lights keydown:
}
void Scene::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	// Meshes keyup:
	switch (e.keysym.sym)
	{
	case SDL_Keycode(Action::ToggleMaterial):
		ToggleMaterial();
		break;

	case SDL_Keycode(Action::ToggleCamera):
		ToggleCamera();
		break;

	case SDL_Keycode(Action::ResetCamera):
		ResetCamera();
		break;
	}

	// Active-camera keyup:
	m_pCameras[m_CurCamIdx]->ProcessKeyUpEvent(e);

	// Lights keyup:
}
void Scene::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	// Meshes mousemotion:

	// Active-camera mousemotion:
	m_pCameras[m_CurCamIdx]->ProcessMouseMotionEvent(e);

	// Lights mousemotion:
}
void Scene::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	// Meshes mousedown:

	// Active-camera mousedown:
	m_pCameras[m_CurCamIdx]->ProcessMouseDownEvent(e);

	// Lights mousedown:
}
void Scene::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	// Meshes mouseup:

	// Active-camera mouseup:
	m_pCameras[m_CurCamIdx]->ProcessMouseUpEvent(e);

	// Lights mouseup:
}
#pragma endregion

void Scene::ToggleMaterial()
{
	for (TriMesh* pMesh : m_pMeshes)
	{
		pMesh->ToggleMaterial();
	}
}

void Scene::ResetCamera()
{
	m_pCameras[m_CurCamIdx]->ResetPosition();
}

void Scene::ToggleCamera()
{
	++m_CurCamIdx;

	if (m_CurCamIdx >= m_pCameras.size()) m_CurCamIdx = 0;
}

void Scene::ToggleUpdate()
{
	m_ShouldUpdate = !m_ShouldUpdate;
}
