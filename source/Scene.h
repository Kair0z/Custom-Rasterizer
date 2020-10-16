#pragma once
#include <vector>

class Light;
class Camera;
class TriMesh;

class Scene
{
public:
	Scene();
	~Scene();

	void AddLight(Light* pLight);
	void AddCamera(Camera* pCamera);
	void AddMesh(TriMesh* pMesh);

	Camera* GetCurrentCamera();
	std::vector<Light*> GetLights();
	std::vector<TriMesh*> GetMeshes();

	void Update(float elapsedSec);

	void ToggleMaterial();
	void ResetCamera();
	void ToggleCamera();
	void ToggleUpdate();

	// Input:
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

private:
	void Cleanup();

	std::vector<Light*> m_pLights;
	std::vector<TriMesh*> m_pMeshes;
	std::vector<Camera*> m_pCameras;
	uint32_t m_CurCamIdx;

	bool m_ShouldUpdate;

public:
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;
};

