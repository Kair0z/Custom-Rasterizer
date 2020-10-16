#pragma once
#include <string>

#include "Window.h"

class UIElement
{
public:
	virtual ~UIElement();
	bool ShouldRender() const;

	// Input:
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e, bool& inputConsumed);
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& inputConsumed);
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e, bool& inputConsumed);
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed);
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, bool& inputConsumed);

	virtual void Update(float elapsedSec);
	virtual void Render(Window& window) const;

protected:
	UIElement(const std::string& tag, bool shouldRender);

private:
	bool m_ShouldRender;
	std::string m_Tag;
};

