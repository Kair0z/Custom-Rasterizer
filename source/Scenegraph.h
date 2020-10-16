#pragma once
#include <vector>

class Scene;

class Scenegraph final
{
public:
	Scenegraph();
	~Scenegraph();

	void AddScene(Scene* pScene);

	Scene* GetActiveScene();
	void ToggleActiveScene();

	void Update(float elapsedSec);

	bool IsEmpty() const;

	// Input:
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
	
private:
	std::vector<Scene*> m_pScenes;
	uint32_t m_CurrentSceneIdx;

public:
	Scenegraph(const Scenegraph&) = delete;
	Scenegraph(Scenegraph&&) = delete;
	Scenegraph& operator=(const Scenegraph&) = delete;
	Scenegraph& operator=(Scenegraph&&) = delete;
};

